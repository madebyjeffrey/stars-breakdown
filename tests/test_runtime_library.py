"""OMF boundaries and negative controls for the original CRT mapping audit."""
import copy
import json
import struct
import unittest
from unittest.mock import patch
from tools.omf import Cursor, library
from tools.review_runtime_library import ROOT, review


def record(kind, body=b''):
    raw = bytes([kind]) + struct.pack('<H', len(body) + 1) + body
    return raw + bytes([-sum(raw) & 255])


def string(text):
    return bytes([len(text)]) + text.encode('ascii')


def member(data_records, size=16):
    return (record(0x80, string('fixture.obj')) +
            record(0x96, string('_TEXT') + string('CODE')) +
            record(0x98, b'\x48' + struct.pack('<H', size) + b'\x01\x02\x00') +
            record(0x8c, string('_callee') + b'\0') + b''.join(data_records) + record(0x8a, b'\0'))


def data(value, offset=0):
    return record(0xa0, b'\x01' + struct.pack('<H', offset) + value)


class OmfTests(unittest.TestCase):
    def test_page_alignment_checksum_and_indices(self):
        item = member([data(b'\x90\xcb')])
        header = record(0xf0, bytes(12))
        payload = header + item
        payload += bytes((-len(payload)) % 16) + record(0xf1)
        parsed = library(payload)
        self.assertEqual(len(parsed), 1)
        self.assertEqual(parsed[0].offset, 16)
        self.assertEqual(parsed[0].segments[1].data[:2], b'\x90\xcb')
        self.assertEqual(parsed[0].segments[1].initialized[:3], b'\1\1\0')
        self.assertEqual(Cursor(b'\x81\x23').index(), 0x123)
        damaged = bytearray(payload)
        damaged[20] ^= 1
        with self.assertRaisesRegex(ValueError, 'checksum'):
            library(damaged)
        with self.assertRaisesRegex(ValueError, 'Truncated'):
            Cursor(b'\x81').index()
        with self.assertRaisesRegex(ValueError, 'Invalid OMF'):
            library(item[:-1])

    def test_threads_and_iterated_fixup_expansion(self):
        # Define frame/target threads before LEDATA, then reference both.
        threads = record(0x9c, b'\x54\x08\x01')
        fix = record(0x9c, b'\xc4\x00\x8c')
        parsed = library(member([threads, data(bytes(2)), fix]))[0]
        self.assertEqual(parsed.segments[1].fixups[0]['target'], (2, 1))
        self.assertEqual(parsed.segments[1].fixups[0]['frame'], (5, None))
        # Two copies of a two-byte leaf; the fixup refers to the encoded leaf.
        repeated = record(0xa2, b'\x01\0\0' + struct.pack('<HHB', 2, 0, 2) + b'\x34\x12')
        fix = record(0x9c, b'\xc4\x05\x56\x01')
        segment = library(member([repeated, fix]))[0].segments[1]
        self.assertEqual(segment.data[:4], b'\x34\x12\x34\x12')
        self.assertEqual([f['offset'] for f in segment.fixups], [0, 2])

    def test_reject_crossing_fields_and_oversized_expansions(self):
        with self.assertRaisesRegex(ValueError, 'encoded data field'):
            library(member([data(bytes(1)), record(0x9c, b'\xc4\x00\x56\x01')]))
        with self.assertRaisesRegex(ValueError, 'declared segment'):
            library(member([data(bytes(17))]))
        huge = record(0xa2, b'\x01\0\0' + struct.pack('<HHB', 65535, 0, 2) + b'\0\0')
        with self.assertRaisesRegex(ValueError, 'expansion exceeds'):
            library(member([huge]))

    def test_overwrite_order_and_unsupported_exclusion(self):
        segment = library(member([data(b'\xcb\0\0'), data(b'\xca\x08\0')]))[0].segments[1]
        self.assertEqual(segment.data[:3], b'\xca\x08\0')
        self.assertEqual(len(segment.overwrites), 1)
        with self.assertRaisesRegex(ValueError, 'earlier fixup'):
            library(member([data(bytes(2)), record(0x9c, b'\xc4\x00\x56\x01'), data(bytes(2))]))
        parsed = library(member([record(0xc2, b'unsupported'), data(bytes(2))]))[0]
        self.assertEqual(parsed.unsupported[0]['kind'], 0xc2)
        self.assertFalse(any(parsed.segments[1].initialized))


class RuntimeLibraryTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.directory = ROOT / 'runs/step4/guest/MSVC/LIB'
        if not all((cls.directory / f).is_file() for f in ['MLIBCEW.LIB', 'LIBW.LIB']):
            raise unittest.SkipTest('Local VC runtime libraries required for installed-library comparisons')
        cls.members = library((cls.directory / 'MLIBCEW.LIB').read_bytes())
        cls.imports = library((cls.directory / 'LIBW.LIB').read_bytes())
        cls.report = review(cls.directory)

    def changed_report(self, members=None, imports=None):
        with patch('tools.review_runtime_library.library', side_effect=[
                members if members is not None else copy.deepcopy(self.members),
                imports if imports is not None else copy.deepcopy(self.imports)]):
            return review(self.directory)

    def test_complete_code_mapping_and_retained_snapshot(self):
        self.assertEqual(self.report, json.loads((ROOT / 'analysis/reconstruction/runtime-library.json').read_text()))
        self.assertEqual(self.report['summary'], {'no_code_span': 7, 'relocation_consistent_code_match': 101})
        rows = self.report['modules']
        self.assertEqual(sum(r['code_bytes'] for r in rows), 15192)
        sites = [(r['ne_segment'], i) for r in rows if r['code_bytes']
                 for i in range(r['ne_offset'], r['ne_offset'] + r['code_bytes'])]
        self.assertEqual(len(set(sites)), 15192)
        self.assertEqual(sum(len(r.get('fixups', [])) for r in rows), 856)
        self.assertFalse(any(d['mismatches'] for r in rows for d in r.get('data_anchors', [])))

    def test_literal_mutation_is_not_masked(self):
        row = next(r for r in self.report['modules'] if r.get('literal_bytes', 0) > 10)
        members = copy.deepcopy(self.members)
        selected = next(m for m in members if m.offset == row['member_offset'])
        segment = next(s for s in selected.segments[1:] if s.class_name == 'CODE' and s.size == row['code_bytes'])
        mask = {i for f in segment.fixups for i in range(f['offset'], f['offset'] + f['width'])}
        segment.data[next(i for i in range(segment.size) if i not in mask)] ^= 1
        result = next(r for r in self.changed_report(members=members)['modules'] if r['module'] == row['module'])
        self.assertEqual(result['status'], 'unresolved_code_match')

    def test_linker_field_mutation_is_not_excused_by_literal_match(self):
        row, fix = next((r, f) for r in self.report['modules'] for f in r.get('fixups', [])
                        if f['status'] == 'verified_linked_word' and f['target_method'] == 2)
        members = copy.deepcopy(self.members)
        selected = next(m for m in members if m.offset == row['member_offset'])
        segment = next(s for s in selected.segments[1:] if s.class_name == 'CODE' and s.size == row['code_bytes'])
        target = next(f for f in segment.fixups if f['offset'] == fix['offset'])
        target['displacement'] += 1
        result = next(r for r in self.changed_report(members=members)['modules'] if r['module'] == row['module'])
        self.assertEqual(result['status'], 'literal_match_pending_fixups')
        self.assertTrue(any(f['status'] == 'unresolved_linked_word' for f in result['fixups']))

    def test_wrong_import_is_rejected(self):
        row, fix = next((r, f) for r in self.report['modules'] for f in r.get('fixups', [])
                        if f['status'] == 'verified_import')
        imports = copy.deepcopy(self.imports)
        for m in imports:
            if fix['symbol'] in m.imports:
                m.imports[fix['symbol']] = 'KERNEL!#65535'
        result = next(r for r in self.changed_report(imports=imports)['modules'] if r['module'] == row['module'])
        self.assertEqual(result['status'], 'literal_match_pending_fixups')
        self.assertTrue(any(f['status'] == 'unresolved_import' for f in result['fixups']))
