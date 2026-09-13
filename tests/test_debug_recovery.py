"""Cross-record invariants and instruction-backed exceptions for full debug recovery."""
import json
from pathlib import Path
import shutil
import subprocess
import tempfile
import unittest
from tools.recover_debug import recover
from tools.debug_headers import generate

ROOT=Path(__file__).resolve().parents[1]

class DebugRecoveryChecks(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.d=recover();cls.t={t['index']:t for t in cls.d['types']};cls.s={s['id']:s for s in cls.d['symbols']}
        cls.b=(ROOT/'target/stars.exe').read_bytes()

    def test_all_kinds_and_references_audited(self):
        self.assertEqual(len(self.d['subsection_audit']),234)
        self.assertEqual(self.d['summary']['semantic_unknown_record_kinds'],0)
        refs=[s for s in self.s.values() if s['kind'] in ('S_PROCREF','S_DATAREF')]
        self.assertEqual(len(refs),901)
        for r in refs:self.assertEqual(r['resolved_name'],self.s[r['target_id']]['name'])
        for s in self.s.values():
            p=s['record_file_offset'];raw=bytes.fromhex(s['raw'])
            self.assertEqual(self.b[p:p+len(raw)],raw)
        for t in self.t.values():
            p=t['record_file_offset']+2;raw=bytes.fromhex(t['raw'])
            self.assertEqual(self.b[p:p+len(raw)],raw)

    def test_scope_nesting_and_duplicate_names_preserved(self):
        scopes={s['id']:s for s in self.s.values() if s['kind'] in ('S_BLOCK16','S_GPROC16','S_LPROC16')}
        self.assertEqual(len(scopes),1483)
        for s in self.s.values():
            if s['kind']=='S_END':self.assertIn(s['ends_scope_id'],scopes)
            if s['kind']=='S_BPREL16':self.assertIn(s['scope_id'],scopes)
        same_name=[s for s in self.s.values() if s.get('name')=='i' and s['kind']=='S_BPREL16']
        self.assertGreater(len(set(s['scope_id'] for s in same_name)),1)

    def test_calling_conventions_and_effective_parameters(self):
        fs={f['name']:f for f in self.d['procedures']}
        ask=fs['AskSaveDialog'];raw=self.b[ask['file_offset']:ask['file_offset']+ask['size']]
        self.assertEqual(self.t[ask['type_index']]['parameter_count'],0)
        self.assertEqual(ask['effective_parameter_types'],[115,115,33,18])
        self.assertTrue(raw.endswith(bytes.fromhex('ca0a00')))
        frame=fs['FrameWndProc']
        regs=[self.s[i] for i in frame['variables'] if self.s[i]['kind']=='S_REGISTER16']
        self.assertEqual([(r['name'],r['register']) for r in regs],[('hwnd','SI'),('wParam','DI')])
        self.assertTrue(all(len(f['effective_parameter_types'])==len(f['effective_parameter_names']) for f in fs.values()))

    def test_layouts_and_machine_accesses(self):
        structures=[t for t in self.t.values() if t['leaf']=='0005']
        self.assertEqual(len(structures),192)
        for t in structures:
            for m in t['members']:self.assertLessEqual(m['offset']+m['size_bytes'],t['size'])
        def member(name,field):return next(m for t in structures if t['name']==name for m in t['members'] if m['name']==field)
        fs={f['name']:f for f in self.d['procedures']}
        def code(name):
            f=fs[name];return self.b[f['file_offset']:f['file_offset']+f['size']]
        self.assertEqual(member('_player','rgAttr')['offset'],62)
        self.assertIn(bytes.fromhex('b83e00'),code('GetRaceStat'))
        self.assertEqual(member('_player','grbitAttr')['offset'],78)
        self.assertIn(bytes.fromhex('23474e235750'),code('GetRaceGrbit'))
        # Fleet cargo array occupies 5 longs; the fourth index begins at 76+4*4=92.
        m=member('_fleet','rgwtMin');self.assertEqual((m['offset'],m['size_bytes']),(76,20))
        self.assertIn(bytes.fromhex('262b475c261b575e'),code('GetFuelFree'))
        self.assertEqual(next(t['size'] for t in structures if t['name']=='_engine'),78)
        self.assertIn(bytes.fromhex('b84e00f76e06'),code('LpengineFromId'))
        self.assertEqual(next(t['size'] for t in structures if t['name']=='_huldef'),143)
        self.assertIn(bytes.fromhex('b88f00f76e06'),code('LphuldefSBFromId'))

    def test_absolute_constants_and_library_gaps_explicit(self):
        c=next(s for s in self.s.values() if s.get('name')=='FIARQQ')
        self.assertEqual(c['signed_value'],-462)
        self.assertEqual(c['address_kind'],'absolute')
        self.assertEqual(self.d['summary']['unmapped_library_indices'],[256])
        self.assertEqual(sum(m['library_name']=='mlibcew.lib' for m in self.d['modules'].values()),108)

    def test_retained_recovery_matches_regeneration(self):
        for name,value in self.d.items():
            self.assertEqual(value,json.loads((ROOT/'analysis/debug'/(name+'.json')).read_text()),name)

    @unittest.skipUnless(shutil.which('clang'),'Host C compiler unavailable')
    def test_headers_compile_in_c89_and_match_generation(self):
        with tempfile.TemporaryDirectory(prefix='stars-headers-') as td:
            out=Path(td);generate(out)
            for p in out.iterdir():self.assertEqual(p.read_bytes(),(ROOT/'reconstructed/include'/p.name).read_bytes())
            subprocess.run(['clang','-std=c89','-pedantic-errors','-Wno-strict-prototypes','-fsyntax-only',str(out/'checklay.c')],check=True,capture_output=True)

if __name__=='__main__':unittest.main()
