import json
import unittest
from tools.audit_segment_moves import segment_move, audit, ROOT


class SegmentMoveTests(unittest.TestCase):
    def test_opcode_direction_and_width(self):
        self.assertEqual(segment_move(bytes.fromhex('8ed9')), 'MOV DS,CX')
        self.assertEqual(segment_move(bytes.fromhex('8cd9')), 'MOV CX,DS')
        self.assertEqual(segment_move(bytes.fromhex('668ed9')), 'MOV DS,ECX')
        self.assertEqual(segment_move(bytes.fromhex('8ec0')), 'MOV ES,AX')
        self.assertIsNone(segment_move(bytes.fromhex('8e19')))
        self.assertIsNone(segment_move(b''))

    def test_retained_audit_and_battle_plan_evidence(self):
        result = audit()
        self.assertEqual(result, json.loads((ROOT / 'analysis/reconstruction/segment-moves.json').read_text()))
        row = next(r for r in result['corrections'] if r['segment'] == 16 and r['offset'] == 0xdb)
        self.assertEqual(row['bytes'], '8ed9')
        self.assertEqual(row['corrected_text'], 'MOV DS,CX')
