"""Known-binary cross-checks: independently anchor metadata to machine bytes."""
import hashlib
import json
from pathlib import Path
import unittest

ROOT=Path(__file__).resolve().parents[1]

class DebugEvidence(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.binary=(ROOT/'target/stars.exe').read_bytes()
        cls.functions=json.loads((ROOT/'analysis/functions.json').read_text())
        cls.types=json.loads((ROOT/'analysis/types.json').read_text())

    def test_input_identity(self):
        self.assertEqual(hashlib.sha256(self.binary).hexdigest(),
                         'ccab6682b55dae14dbf104e491cfbd3b32a6280022020032ae0615500949ba59')

    def test_get_race_stat_matches_machine_code(self):
        f=next(f for f in self.functions if f['name']=='GetRaceStat')
        self.assertEqual((f['ne_segment'],f['ne_offset'],f['size']),(29,0x30d2,34))
        self.assertEqual(self.binary[f['file_offset']:f['file_offset']+f['size']].hex(),
                         '558bec81ec02005657b83e008b5e0603d88b460803d88a0798e900005f5e8be55dcb')
        self.assertEqual([(x['name'],x['bp_offset']) for x in f['locals']], [('pplr',6),('iStat',8)])
        player=next(t for t in self.types if t.get('name')=='_player' and t['size']==192)
        attr=next(m for m in player['members'] if m['name']=='rgAttr')
        self.assertEqual(attr['offset'],0x3e)
        array=next(t for t in self.types if t['index']==attr['type_index'])
        self.assertEqual((array['element_type'],array['size']),(0x70,16))

    def test_non_identity_segment_translation(self):
        # Runtime logical segment 38 maps to NE segment 36, not segment 38.
        maps=json.loads((ROOT/'analysis/segment_map.json').read_text())
        self.assertEqual(maps[37]['frame'],36)
        self.assertEqual(maps[37]['size'],15253)

if __name__=='__main__': unittest.main()
