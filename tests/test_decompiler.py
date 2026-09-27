"""Independent ABI/boundary checks for the retained step-3 analysis evidence."""
import json
from pathlib import Path
import unittest

ROOT = Path(__file__).resolve().parents[1]
D = ROOT/'analysis/decompiler'

def read(name):
    return json.loads((D/name).read_text())

class DecompilerWorkflowTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.functions = {f['name']:f for f in read('functions.json')}

    def test_every_debug_procedure_has_an_export(self):
        procedures=json.loads((ROOT/'analysis/debug/procedures.json').read_text())
        for p in procedures:
            f=self.functions[p['name']]
            self.assertEqual(f['address'],f"{0x1000+8*(p['ne_segment']-1):04x}:{p['ne_offset']:04x}")
            self.assertTrue(f['decompiled'])
        self.assertGreater(len(self.functions),len(procedures))

    def test_loader_and_branch_validation(self):
        check=read('loader-check.json')
        self.assertEqual(check['unmatched_bytes'],0)
        self.assertEqual(check['segments'],37)
        self.assertEqual(check['relocation_sites'],15327)
        self.assertEqual(check['absolute_export_sites'],13)
        self.assertEqual(read('summary.json')['invalid_direct_flows'],0)

    def test_far_cdecl_and_split_return(self):
        f=self.functions['GetFuelFree']
        self.assertEqual(f['convention'],'__cdecl16far')
        self.assertEqual(f['return_storage'],'DX:2,AX:2')
        self.assertEqual(f['parameters'][0]['storage'],'Stack[0x4]:4')
        self.assertEqual(f['purge'],0)
        asm=(D/'acceptance/1050_6004.asm').read_text()
        self.assertIn('CALLF 0x1050:40f2',asm)
        self.assertIn('ADD SP,0x6',asm)
        self.assertIn('LES BX',asm)
        self.assertIn('SBB DX',asm)

    def test_pascal_callback_order_and_cleanup(self):
        for name in ('AskSaveDialog','FrameWndProc'):
            f=self.functions[name]
            self.assertEqual(f['convention'],'__stdcall16far')
            self.assertEqual(f['purge'],10)
            self.assertEqual([p['storage'] for p in f['parameters']],
                             ['Stack[0xc]:2','Stack[0xa]:2','Stack[0x8]:2','Stack[0x4]:4'])
        c=next((D/'acceptance').glob('*AskSaveDialog.c')).read_text()
        self.assertIn('ENDDIALOG(hwnd,arg1)',c)

    def test_sign_extension_and_16_bit_truncation(self):
        f=self.functions['GetRaceStat']
        self.assertEqual(f['parameters'][0]['size'],2)
        self.assertEqual(f['return_bytes'],2)
        c=next((D/'acceptance').glob('*GetRaceStat.c')).read_text()
        self.assertIn('pplr->rgAttr[iStat]',c)
        self.assertIn('CBW',(D/'acceptance/10e0_30d2.asm').read_text())
        # ICompLong performs a long subtraction but its declared/observed return is AX.
        self.assertEqual(self.functions['ICompLong']['return_storage'],'AX:2')
        self.assertIn('SBB DX',(D/'acceptance/1040_1d74.asm').read_text())

    def test_runtime_helpers_and_dispatch(self):
        f=self.functions['__aFlmul']
        self.assertEqual(f['return_storage'],'DX:2,AX:2')
        self.assertEqual(f['purge'],8)
        self.assertIn('RETF 0x8',(D/'acceptance/1118_0c7e.asm').read_text())
        dispatch=read('runtime-dispatch.json')
        self.assertEqual(len(dispatch['rows']),13)
        self.assertEqual(len(dispatch['error_dispatch']['targets']),4)
        self.assertEqual(len(dispatch['math_dispatch']['targets']),39)
        for row in dispatch['rows']:
            self.assertEqual(len(row['targets']),4**row['argument_count'])
        self.assertNotIn('__wflags',self.functions)

    def test_switch_table_is_data_not_instructions(self):
        rows=read('coverage.json')['ranges']
        table=[r for r in rows if r['ne_segment']==2 and r['start']<0x5874 and r['end_exclusive']>0x5852]
        self.assertTrue(table)
        self.assertTrue(all(r['classification']=='data' for r in table))
        switch=next(r for r in read('switches.json') if r['function']=='FLookupPart')
        self.assertEqual(len(switch['targets']),17)

    def test_coverage_partitions_memory_without_claiming_source_completion(self):
        coverage=read('coverage.json');ends={}
        for r in coverage['ranges']:
            seg=r['ne_segment']
            self.assertEqual(r['start'],ends.get(seg,0))
            self.assertGreater(r['end_exclusive'],r['start'])
            ends[seg]=r['end_exclusive']
        self.assertEqual(sum(ends.values()),1042584)
        self.assertEqual(len(ends),37)
        baseline=json.loads((ROOT/'analysis/coverage.json').read_text())
        # Automatic disassembly is not compiled coverage: only rows with an
        # explicit reconstructed source may contribute to the independent
        # baseline. A partial source may be compiled before whole-procedure
        # semantic review is complete.
        compiled=[r for r in baseline['ranges']
                  if r.get('compilation_status')=='host_c89_compiled']
        self.assertEqual(baseline['summary']['compiled_reconstruction_bytes'],
                         sum(r['bytes'] for r in compiled))
        self.assertTrue(all(r['review_status'] in ('manually_reviewed','partial') and
                            r['reconstruction_path'].startswith('reconstructed/src/')
                            for r in compiled))

    def test_dos_stub_is_separate_and_message_is_not_code(self):
        funcs=read('dos-functions.json')
        self.assertEqual([f['address'] for f in funcs],['1000:0000','1000:0056'])
        self.assertEqual(sum(f['body_bytes'] for f in funcs),15)
        self.assertTrue(all(f['decompiled'] for f in funcs))
        asm=(D/'acceptance/dos/1000_0056.asm').read_text()
        self.assertEqual(asm.count('INT 0x21'),2)
        self.assertIn('MOV AX,0x4c01',asm)

    def test_no_indirect_target_is_silently_dropped(self):
        rows=read('indirect-targets.json')
        self.assertEqual(len({r['address'] for r in rows}),len(rows))
        self.assertTrue(all(r['status'] and r['resolution_task'] for r in rows))
        self.assertEqual(sum(r['status']=='unresolved' for r in rows),read('summary.json')['unresolved_indirect_instructions'])

if __name__=='__main__':unittest.main()
