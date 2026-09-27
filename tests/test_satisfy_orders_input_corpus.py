"""Complete input framing and original-instruction player initialization."""
from collections import Counter
import ctypes as C
from dataclasses import replace
import hashlib
from pathlib import Path
import struct
import subprocess
import tempfile
import unittest

from tools.record_machine import RecordMachine
from tools.satisfy_orders_input_corpus import InputReader, ROOT, record_runtime_references
from tools.stars_game_stream import decode_stream
from tools.stars_fleet_input import FleetState
from tools.stars_planet_input import PlanetState
from tools.satisfy_orders_input_bridge import complete_input_header
from tools.satisfy_orders_original_oracle import decode_owner_fleet
from tests.test_satisfy_orders_fresh_ai_pair import _decompress_user_name


# Literal original NE 02 tables, independent of the source part resolver.
PART_TABLES = {
    1: (0x0000, 78), 2: (0x0768, 56), 4: (0x0ae8, 54),
    8: (0x04e0, 54), 16: (0x2450, 60), 32: (0x2180, 60),
    64: (0x1e1a, 58), 128: (0x1508, 54), 256: (0x12ec, 54),
    512: (0x4d8a, 56), 1024: (0x4872, 143), 2048: (0x0d04, 54),
    4096: (0x109a, 54), 8192: (0x19e2, 54), 16384: (0x29f0, 143),
    32768: (0x16b8, 54),
}


def expected_planet_state(record, loading_player):
    """Independent wire-to-runtime oracle using the original PLANET layout."""
    body = record.body
    identity, flags = struct.unpack_from('<HH', body)
    state = PlanetState()
    state.id = identity & 2047
    state.player = (identity >> 11) if identity >> 11 != 31 else -1
    state.turn = record.turn
    state.flags = ((flags & 127) | (flags & 0x300) |
                   ((flags & 0x80) << 3) |
                   (0x800 if loading_player != -1 else (flags & 0x8000) >> 4))
    if state.player == -1:
        state.flags &= ~0x200
    cursor = 4

    def number(width):
        nonlocal cursor
        value = int.from_bytes(body[cursor:cursor + width], 'little')
        cursor += width
        return value

    if flags & 127 >= 3:
        selectors = number(1)
        state.mineral_levels[:] = [number((selectors >> (2 * i)) & 3) for i in range(3)]
        state.mineral_concentrations[:] = [number(1) for _ in range(3)]
        state.environment[:] = [number(1) for _ in range(3)]
        state.environment_original[:] = ([number(1) for _ in range(3)]
            if flags & 0x400 else state.environment)
        if state.player != -1:
            state.routing_word = number(2)
        if flags & 127 > 3 and flags & 0x2000:
            selectors = number(1)
            state.minerals[:] = [number((0, 1, 2, 4)[(selectors >> (2 * i)) & 3])
                                 for i in range(4)]
    if record.kind == 13 and flags & 127 > 3:
        if flags & 0x800:
            state.opaque_20[:] = [number(4), number(4)]
        else:
            state.opaque_20[1] = 0x1f000 | ((flags & 0x1000) << 10)
        if state.player != -1:
            if flags & 0x200:
                state.opaque_44 = number(4)
            if flags & 0x4000:
                state.opaque_48 = number(2)
    elif state.flags & 0x200:
        state.opaque_44 = number(1) & 15
    assert cursor == len(body)
    return state


def expected_saved_design(body, player, segment):
    """Independent wire/table oracle for the retained complete designs.

    Assert the pinned cases need no invalid-slot repair or special race cost
    rules; then build every byte directly from the wire, names and EXE tables.
    """
    def part(group, item):
        base, stride = PART_TABLES[group]
        return segment[base + item * stride:base + (item + 1) * stride]

    def word(data, offset):
        return int.from_bytes(data[offset:offset + 2], 'little')

    assert body[0] == 7
    assert player[76] in (1, 9)
    race_bits = int.from_bytes(player[78:82], 'little')
    assert not race_bits & ((1 << 8) | (1 << 12))
    hull_id, bitmap, count = body[2], body[3], body[6]
    hull = part(16384, hull_id) if hull_id < 32 else part(1024, hull_id - 32)
    expected = bytearray(147)
    struct.pack_into('<H', expected, 0, hull_id)
    struct.pack_into('<H', expected, 50,
                     bitmap if hull[50] <= bitmap < hull[50] + 4 else hull[50] | (bitmap & 3))
    expected[58:58 + count * 4] = body[17:17 + count * 4]
    expected[122] = count
    expected[123:125] = body[:2]
    expected[125:135] = body[7:17]
    name_at = 17 + count * 4
    size = body[name_at]
    name = (_decompress_user_name(body[name_at + 1:name_at + 1 + size]).encode('cp1252')
            if size else body[name_at + 1:].split(b'\0')[0])
    expected[8:8 + len(name)] = name
    costs = [0] * 4
    weight, damage = word(hull, 40), word(hull, 56)
    price_parts = [(hull, 1)]
    for slot in range(count):
        at = 17 + slot * 4
        group, item, quantity = struct.unpack_from('<HBB', body, at)
        if not quantity:
            assert slot != 0 or word(hull, 58) != 1
            continue
        component = part(group, item)
        assert group & word(hull, 58 + slot * 4)
        assert quantity <= hull[61 + slot * 4]
        assert all(have >= need for have, need in zip(player[26:32], component[2:8]))
        price_parts.append((component, quantity))
        weight += quantity * word(component, 40)
        if group == 4 and item in (3, 6):
            damage += quantity * 65
        elif group == 8:
            extra = quantity * word(component, 52)
            damage += extra // 2 if race_bits & (1 << 13) else extra
        elif group == 4096 and item == 4:
            damage += quantity * 50
    for component, quantity in price_parts:
        differences = [have - need for have, need in zip(player[26:32], component[2:8]) if need]
        progress = min(differences) if differences else min(player[26:32])
        discount = min(75, max(0, progress) * 4)
        for i, base in enumerate(struct.unpack_from('<4H', component, 42)):
            costs[i] += quantity * (max(1, base - base * discount // 100) if base else 0)
    struct.pack_into('<H4H', expected, 40, weight & 65535, *[v & 65535 for v in costs])
    struct.pack_into('<H', expected, 56, damage & 65535)
    expected[135:139] = b'\xff' * 4
    return bytes(expected)


class SatisfyOrdersInputCorpusTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.reader = InputReader()
        cls.runs = {label: cls.reader.load_run(ROOT / f'runs/vmctl-sye3k{i}')
                    for i, label in enumerate(('control', 'action'))}

    def test_every_input_record_and_coordinate_consumed(self):
        for label, files in self.runs.items():
            self.assertEqual(set(files), {'XY', 'HST', 'M1', 'M2'})
            self.assertEqual(len(files['XY'].coordinates), 32)
            game = files['XY'].records[1].body
            self.assertEqual(struct.unpack_from('<HH', game, 8), (2, 32))
            for suffix, expected in [('HST', 82), ('M1', 31), ('M2', 21)]:
                with self.subTest(run=label, file=suffix):
                    loaded = files[suffix]
                    reference = decode_stream(loaded.path, self.reader.inspector)
                    self.assertEqual(len(loaded.records), expected)
                    self.assertEqual([(r.offset, r.kind, r.body, r.turn)
                                      for r in loaded.records],
                                     [(r['offset'], r['kind'], r['body'], r['turn'])
                                      for r in reference])
                    self.assertEqual(sum(2 + len(r.body) for r in loaded.records),
                                     loaded.path.stat().st_size)
            self.assertEqual(set(files['HST'].players), {0, 1})
            self.assertEqual(set(files['M1'].players), {0})
            self.assertEqual(set(files['M2'].players), {1})
        control = Counter(r.kind for r in self.runs['control']['HST'].records)
        action = Counter(r.kind for r in self.runs['action']['HST'].records)
        self.assertEqual((control[19], control[20]), (0, 10))
        self.assertEqual((action[19], action[20]), (1, 9))

    def test_every_record_has_a_concrete_runtime_destination(self):
        count = 0
        for files in self.runs.values():
            for loaded in files.values():
                references = record_runtime_references(loaded)
                self.assertEqual(set(references), {r.offset for r in loaded.records})
                self.assertTrue(all(references.values()))
                count += len(references)
        self.assertEqual(count, 274)
        loaded = self.runs['action']['HST']
        planets = dict(loaded.planets)
        del planets[24]
        with self.assertRaisesRegex(ValueError, 'Planet record'):
            record_runtime_references(replace(loaded, planets=planets))
        with self.assertRaisesRegex(ValueError, 'Fleet/order record'):
            record_runtime_references(replace(loaded, fleets={}))
        with self.assertRaisesRegex(ValueError, 'finalized runtime player'):
            record_runtime_references(replace(loaded, runtime_players={}))

    def test_xy_and_save_files_validate_their_distinct_eof_markers(self):
        files = self.runs['control']
        self.assertEqual(int.from_bytes(files['XY'].records[-1].body, 'little'), 2)
        for suffix in ('HST', 'M1', 'M2'):
            self.assertEqual(int.from_bytes(files[suffix].records[-1].body, 'little'), 0)
        with tempfile.TemporaryDirectory() as directory:
            for suffix, loaded in files.items():
                raw = bytearray(loaded.path.read_bytes())
                raw[-1] ^= 1
                path = Path(directory) / ('BAD.' + suffix)
                path.write_bytes(raw)
                with self.assertRaisesRegex(ValueError, 'EOF marker'):
                    self.reader.read(path, hashlib.sha256(raw).hexdigest(), xy=suffix == 'XY')

    def test_all_eight_player_states_match_original_readrtplr(self):
        machine = RecordMachine()
        for label, files in self.runs.items():
            for suffix in ('HST', 'M1', 'M2'):
                for record in files[suffix].records:
                    if record.kind != 6:
                        continue
                    player = record.body[0]
                    with self.subTest(run=label, file=suffix, player=player):
                        machine.put(0x750, struct.pack('<H', 0x2a60))
                        machine.put(0x3500, record.body)
                        machine.call(0x05e2, struct.pack('<HH', 0x3000, 0x3500),
                                     segment=15, shared_stack=True)
                        self.assertEqual(files[suffix].players[player],
                                         machine.get(0x3000, 192))

    def test_floadgame_rebuilds_player_counts_from_initialized_objects(self):
        for files in self.runs.values():
            self.assertEqual([int.from_bytes(files['HST'].players[p][2:4], 'little')
                              for p in (0, 1)], [32, 0])
            for suffix in ('HST', 'M1', 'M2'):
                loaded = files[suffix]
                self.assertEqual(set(loaded.runtime_players), set(loaded.players))
                for owner, actual in loaded.runtime_players.items():
                    expected = bytearray(loaded.players[owner])
                    # This corpus has exactly one owned planet per player in
                    # every view. Saved HST and M1 counts differ from that.
                    struct.pack_into('<H', expected, 2, 1)
                    self.assertEqual(actual, bytes(expected))
                    self.assertEqual(int.from_bytes(actual[4:6], 'little') & 4095,
                                     6 if owner == 0 else 4)
                    self.assertEqual(actual[1], 6 if owner == 0 else 3)

    def test_all_40_fleet_states_and_40_orders_from_complete_inputs(self):
        count = 0
        for label, files in self.runs.items():
            for suffix, expected_count in (('HST', 10), ('M1', 6), ('M2', 4)):
                loaded = files[suffix]
                self.assertEqual(len(loaded.fleets), expected_count)
                for index, record in enumerate(loaded.records):
                    if record.kind != 16:
                        continue
                    wire = decode_owner_fleet(record.body)
                    fleet = loaded.fleets[wire['owner'], wire['id']]
                    with self.subTest(run=label, file=suffix, fleet=wire['id']):
                        expected = FleetState()
                        expected.fleet_flags = wire['saved_flags'] | (0 if any(wire['ship_counts']) else 0x400)
                        expected.ship_counts[:] = wire['ship_counts'] + [
                            (value & 127) | (min(value >> 7, 499) << 7)
                            for value in wire['extra_counts']]
                        expected.minerals[:] = wire['cargo']
                        expected.design_plan = wire['design_plan']
                        expected.order_count = wire['order_count']
                        expected.planet = wire['planet']
                        expected.x, expected.y = wire['point']
                        self.assertEqual(wire['order_count'], 1)
                        order = loaded.records[index + 1]
                        expanded = bytearray(order.body.ljust(18, b'\0'))
                        expanded[7] &= 0xdf  # CodeView ORDER bit 13.
                        expected.orders[:18] = expanded
                        self.assertEqual(bytes(fleet.state), bytes(expected))
                        self.assertEqual(fleet.id_word, int.from_bytes(record.body[:2], 'little'))
                        self.assertEqual(fleet.record_offsets, (record.offset, order.offset))
                        self.assertEqual(wire['point'], list(files['XY'].coordinates[wire['planet']][:2]))
                    count += 1
        self.assertEqual(count, 40)
        action = self.runs['action']['HST']
        expanded = next(r for r in action.records if r.kind == 19)
        fleet = next(f for f in action.fleets.values() if expanded.offset in f.record_offsets)
        self.assertEqual(fleet.state.orders[6] & 15, 5)
        self.assertEqual(bytes(fleet.state.orders[8:18]), expanded.body[8:18])

    def test_all_72_planet_states_and_discovery_messages(self):
        count = 0
        for label, files in self.runs.items():
            for suffix, expected_count, player in (('HST', 32, -1), ('M1', 3, 0), ('M2', 1, 1)):
                loaded = files[suffix]
                self.assertEqual(len(loaded.planets), expected_count)
                for record in loaded.records:
                    if record.kind not in (13, 14):
                        continue
                    expected = expected_planet_state(record, player)
                    with self.subTest(run=label, file=suffix, planet=expected.id):
                        self.assertEqual(bytes(loaded.planets[expected.id]), bytes(expected))
                    count += 1
                if suffix != 'M1':
                    self.assertEqual(loaded.planet_messages, ())
            # Original FReadPlanet uses PctDesirability * PctTrueMaxGrowth
            # as parameter 1, and the planet ID as parameter 2. The retained
            # race is not Hyper Expansion, so its growth byte is unscaled.
            player = files['M1'].players[0]
            self.assertNotEqual(player[76], 0)
            self.assertEqual(files['M1'].planet_messages,
                ((0, 0xac, 7, 33 * player[25], 7),
                 (0, 0xac, 12, 17 * player[25], 12)))
        self.assertEqual(count, 72)

    def test_planet_input_rejects_truncation_trailing_bytes_and_bad_environment(self):
        loaded = self.runs['control']['M1']
        index = next(i for i, r in enumerate(loaded.records) if r.kind == 14)
        original = loaded.records[index]
        invalid_environment = bytearray(original.body)
        invalid_environment[8] = 101
        for body in (original.body[:-1], original.body + b'\0', bytes(invalid_environment)):
            records = list(loaded.records)
            records[index] = replace(original, body=body)
            with self.assertRaises(ValueError):
                self.reader.planet_reader.read_file(replace(loaded, records=records), 0, 32)

    def test_saved_messages_and_planet_discoveries_form_complete_runtime_table(self):
        for files in self.runs.values():
            loaded = files['M1']
            messages = loaded.messages
            growth = loaded.players[0][25]
            expected = ((127, -1, ()), (128, -1, ()), (129, -1, ()),
                        (130, -1, ()), (169, 10, (10,)),
                        (172, 7, (33 * growth, 7)), (172, 12, (17 * growth, 12)))
            self.assertEqual(messages.decoded, expected)
            self.assertEqual(messages.current_message, 0)
            sent = bytearray(64)
            packed = bytearray()
            for identity, destination, params in expected:
                sent[identity // 8] |= 1 << (identity % 8)
                mask = sum((value > 255 or value < 0) << i for i, value in enumerate(params))
                packed.extend(struct.pack('<Hh', identity | (mask << 9), destination))
                for i, value in enumerate(params):
                    packed.extend(value.to_bytes(2 if mask & (1 << i) else 1, 'little', signed=value < 0))
            self.assertEqual(messages.sent_bits, bytes(sent))
            self.assertEqual(messages.buffer, bytes(packed))
            for suffix in ('HST', 'M2'):
                self.assertEqual(files[suffix].messages.buffer, b'')
                self.assertEqual(files[suffix].messages.decoded, ())
                self.assertEqual(files[suffix].messages.current_message, -1)

    def test_messages_reject_incomplete_headers_and_arguments(self):
        loaded = self.runs['control']['M1']
        index = next(i for i, r in enumerate(loaded.records) if r.kind == 12)
        for body in (b'\x7f\0\xff', b'\xa9\0\x0a\0'):
            records = list(loaded.records)
            records[index] = replace(records[index], body=body)
            with self.assertRaisesRegex(ValueError, 'Truncated'):
                self.reader.message_reader.read_file(replace(loaded, records=records))

    def test_complete_input_c_bridge_preserves_all_runtime_tables(self):
        class Fleet(C.Structure):
            _fields_ = [('id_word', C.c_uint16), ('owner', C.c_int16), ('state', FleetState)]

        class View(C.Structure):
            _fields_ = [('player_present', C.c_uint8 * 2),
                ('players', (C.c_uint8 * 192) * 2),
                ('ship_designs', (C.c_uint8 * 2352) * 2),
                ('starbase_designs', (C.c_uint8 * 1470) * 2),
                ('design_present', (C.c_uint8 * 32) * 2),
                ('battle_plan_present', (C.c_uint8 * 16) * 2),
                ('battle_plans', ((C.c_uint8 * 36) * 16) * 2),
                ('planet_count', C.c_uint16), ('planets', PlanetState * 32),
                ('fleet_count', C.c_uint16), ('fleets', Fleet * 10),
                ('thing_count', C.c_uint16), ('things', (C.c_uint8 * 18) * 4),
                ('message_bytes', C.c_uint16), ('message_count', C.c_uint16),
                ('current_message', C.c_int16), ('message_sent', C.c_uint8 * 64),
                ('messages', C.c_uint8 * 512)]

        class Complete(C.Structure):
            _fields_ = [('game', C.c_uint8 * 64), ('bof', C.c_uint8 * 16),
                ('coordinate_count', C.c_uint16), ('coordinates', (C.c_int16 * 3) * 32),
                ('views', View * 3)]

        with tempfile.TemporaryDirectory(prefix='soe3-input-bridge-') as directory:
            path = Path(directory)
            (path / 'input.h').write_text(complete_input_header(self.runs))
            (path / 'probe.c').write_text('#include "core.h"\n#include "input.h"\n'
                'const Soe3CompleteInput *get_input(int run) { return &soe3_inputs[run]; }\n'
                'unsigned long input_size(void) { return sizeof(Soe3CompleteInput); }\n')
            library = path / 'probe.dylib'
            subprocess.run(['cc', '-std=c89', '-pedantic', '-Wall', '-Wextra', '-Werror',
                '-shared', '-fPIC', '-I' + str(ROOT / 'reconstructed/include'),
                str(path / 'probe.c'), '-o', str(library)], check=True, capture_output=True)
            bridge = C.CDLL(str(library))
            bridge.input_size.restype = C.c_ulong
            self.assertEqual(bridge.input_size(), C.sizeof(Complete))
            bridge.get_input.argtypes = [C.c_int]
            bridge.get_input.restype = C.POINTER(Complete)
            for index, label in enumerate(('control', 'action')):
                files = self.runs[label]
                actual = bridge.get_input(index).contents
                self.assertEqual(bytes(actual.game), files['XY'].records[1].body)
                self.assertEqual(bytes(actual.bof), files['HST'].records[0].body)
                self.assertEqual(actual.coordinate_count, len(files['XY'].coordinates))
                self.assertEqual(tuple(tuple(p) for p in actual.coordinates), files['XY'].coordinates)
                for view_index, suffix in enumerate(('HST', 'M1', 'M2')):
                    view, loaded = actual.views[view_index], files[suffix]
                    for owner in range(2):
                        self.assertEqual(view.player_present[owner], owner in loaded.players)
                        self.assertEqual(bytes(view.players[owner]), loaded.runtime_players.get(owner, bytes(192)))
                        for starbase, size, field in ((False, 2352, 'ship_designs'), (True, 1470, 'starbase_designs')):
                            self.assertEqual(bytes(getattr(view, field)[owner]),
                                loaded.design_banks.get((owner, starbase), bytes(size)))
                        self.assertEqual(list(view.design_present[owner]),
                            [int((owner, slot) in loaded.designs) for slot in range(32)])
                        for slot in range(16):
                            self.assertEqual(view.battle_plan_present[owner][slot], (owner, slot) in loaded.battle_plans)
                            self.assertEqual(bytes(view.battle_plans[owner][slot]),
                                loaded.battle_plans.get((owner, slot), bytes(36)))
                    self.assertEqual(view.planet_count, len(loaded.planets))
                    for i, planet in enumerate(loaded.planets.values()):
                        self.assertEqual(bytes(view.planets[i]), bytes(planet))
                    self.assertEqual(view.fleet_count, len(loaded.fleets))
                    for i, fleet in enumerate(loaded.fleets.values()):
                        self.assertEqual((view.fleets[i].id_word, view.fleets[i].owner), (fleet.id_word, fleet.owner))
                        self.assertEqual(bytes(view.fleets[i].state), bytes(fleet.state))
                    self.assertEqual(view.thing_count, len(loaded.things))
                    self.assertEqual(tuple(bytes(view.things[i]) for i in range(view.thing_count)), loaded.things)
                    self.assertEqual(view.message_bytes, len(loaded.messages.buffer))
                    self.assertEqual(bytes(view.messages[:view.message_bytes]), loaded.messages.buffer)
                    self.assertEqual(view.message_count, len(loaded.messages.decoded))
                    self.assertEqual(view.current_message, loaded.messages.current_message)
                    self.assertEqual(bytes(view.message_sent), loaded.messages.sent_bits)

    def test_fleet_input_rejects_missing_orders_and_unconsumed_tails(self):
        loaded = self.runs['action']['HST']
        coordinates = self.runs['action']['XY'].coordinates
        index = next(i for i, r in enumerate(loaded.records) if r.kind == 16)
        records = list(loaded.records)
        del records[index + 1]
        with self.assertRaisesRegex(ValueError, 'order'):
            self.reader.fleet_reader.read_file(replace(loaded, records=records), coordinates)
        records = list(loaded.records)
        records[index] = replace(records[index], body=records[index].body + b'\0')
        with self.assertRaisesRegex(ValueError, 'tail'):
            self.reader.fleet_reader.read_file(replace(loaded, records=records), coordinates)

    def test_fleet_order_clears_auto_track_flag_without_changing_warp(self):
        loaded = self.runs['action']['HST']
        records = list(loaded.records)
        index = next(i for i, r in enumerate(records) if r.kind == 16)
        order = records[index + 1]
        body = bytearray(order.body)
        flags = int.from_bytes(body[6:8], 'little')
        flags = (flags & ~0xf0) | 0x70 | 0x2000
        body[6:8] = flags.to_bytes(2, 'little')
        records[index + 1] = replace(order, body=bytes(body))
        result = self.reader.fleet_reader.read_file(
            replace(loaded, records=records), self.runs['action']['XY'].coordinates)
        fleet = next(f for f in result.values() if order.offset in f.record_offsets)
        actual = int.from_bytes(bytes(fleet.state.orders[6:8]), 'little')
        self.assertEqual(actual, flags & ~0x2000)
        self.assertEqual((actual >> 4) & 15, 7)

    def test_all_44_complete_design_states_from_wire_and_original_parts(self):
        checked = 0
        for label, files in self.runs.items():
            for suffix, count in (('HST', 11), ('M1', 7), ('M2', 4)):
                loaded = files[suffix]
                self.assertEqual(len(loaded.designs), count)
                records = [r for r in loaded.records if r.kind == 26]
                for ((owner, slot), actual), record in zip(loaded.designs.items(), records):
                    with self.subTest(run=label, file=suffix, owner=owner, design=slot):
                        expected = expected_saved_design(record.body, loaded.players[owner],
                                                         self.reader.design_reader.segment_bytes)
                        self.assertEqual(actual, expected)
                        checked += 1
                for (owner, starbase), bank in loaded.design_banks.items():
                    self.assertEqual(len(bank), (10 if starbase else 16) * 147)
                    for index in range(len(bank) // 147):
                        key = (owner, index + (16 if starbase else 0))
                        actual = bank[index * 147:(index + 1) * 147]
                        free_slot = bytearray(147)
                        struct.pack_into('<H', free_slot, 123, 0x200)
                        self.assertEqual(actual, loaded.designs.get(key, bytes(free_slot)))
        self.assertEqual(checked, 44)

    def test_partial_design_keeps_known_state_until_free_or_hull_changes(self):
        loaded = self.runs['control']['M2']
        original = loaded.designs[1, 0]
        body = struct.pack('<HBBH', 0x103, original[0], original[50], 123) + b'\0Unknown\0'
        bank = bytearray(16 * 147)
        bank[:147] = original
        reader = self.reader.design_reader
        self.assertEqual(reader.read(body, loaded.players[1], bank, -1), original)
        bank[124] |= 2  # fFree is bit 9, not bit 1 of the low flag byte.
        changed = reader.read(body, loaded.players[1], bank, -1)
        self.assertEqual(changed[8:16], b'Unknown\0')
        self.assertEqual(changed[40:42], struct.pack('<H', 123))
        self.assertEqual(changed[123:125], struct.pack('<H', 0x103))

    def test_all_40_battle_plans_preserve_flags_and_decode_names(self):
        checked = 0
        names = (b'Default', b'Kill Starbase', b'Max-Defense', b'Sniper', b'Chicken')
        for files in self.runs.values():
            for suffix, count in (('HST', 10), ('M1', 5), ('M2', 5)):
                loaded = files[suffix]
                self.assertEqual(len(loaded.battle_plans), count)
                for record in loaded.records:
                    if record.kind != 30:
                        continue
                    body = record.body
                    key = body[0] & 15, body[0] >> 4
                    name = names[key[1]]
                    self.assertEqual(loaded.battle_plans[key], body[:4] + name.ljust(32, b'\0'))
                    checked += 1
        self.assertEqual(checked, 40)

    def test_all_eight_things_are_usable_by_source_lookup(self):
        pointer = C.POINTER(C.c_ubyte)
        lookup = self.reader.inspector.lib.stars_lookup_thing_record
        lookup.argtypes = [C.c_int16, pointer, C.c_uint16, C.c_uint16, pointer, C.c_int16]
        lookup.restype = C.c_int16
        checked = 0
        for files in self.runs.values():
            self.assertFalse(files['M1'].things)
            self.assertFalse(files['M2'].things)
            things = files['HST'].things
            self.assertEqual(len(things), 4)
            table = (C.c_ubyte * 72).from_buffer_copy(b''.join(things))
            source_records = [r.body for r in files['HST'].records if r.kind == 43][1:]
            for actual, original in zip(things, source_records):
                output = (C.c_ubyte * 18)()
                identity = int.from_bytes(actual[:2], 'little')
                self.assertEqual(lookup(identity, table, 4, 18, output, 0), 1)
                self.assertEqual(bytes(output), original)
                checked += 1
        self.assertEqual(checked, 8)

    def test_complete_player_bytes_and_names_from_wire(self):
        checked = 0
        for files in self.runs.values():
            for suffix in ('HST', 'M1', 'M2'):
                for record in files[suffix].records:
                    if record.kind != 6:
                        continue
                    body = record.body
                    expected = bytearray(192)
                    self.assertEqual(body[6] & 7, 7)
                    expected[:112] = body[:112]
                    relations = body[112]
                    expected[112:112 + relations] = body[113:113 + relations]
                    at = 113 + relations
                    for offset in (128, 160):
                        size = body[at]
                        at += 1
                        if size:
                            name = _decompress_user_name(body[at:at + size]).encode('cp1252')
                            at += size
                        else:
                            end = body.index(0, at)
                            name = body[at:end]
                            at = end + 1
                        expected[offset:offset + len(name)] = name
                    self.assertEqual(at, len(body))
                    expected[84] &= ~8
                    self.assertEqual(files[suffix].players[body[0]], bytes(expected))
                    checked += 1
        self.assertEqual(checked, 8)

    def test_wrong_hash_and_truncated_stream_rejected(self):
        original = self.runs['control']['HST'].path
        with self.assertRaisesRegex(ValueError, 'hash mismatch'):
            self.reader.read(original, '0' * 64)
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / 'TRUNC.HST'
            raw = original.read_bytes()[:-1]
            path.write_bytes(raw)
            with self.assertRaisesRegex(ValueError, 'Truncated'):
                self.reader.read(path, hashlib.sha256(raw).hexdigest())


if __name__ == '__main__':
    unittest.main()
