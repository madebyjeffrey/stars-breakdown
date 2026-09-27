import hashlib,json,unittest
from tools.compare_byte_match import object_module
from tools.compare_mdi_bringup_host_match import ROOT,compare

class MdiBringupHostByteMatchTests(unittest.TestCase):
 @classmethod
 def setUpClass(cls):
  cls.report=json.loads((ROOT/'analysis/compat/mdi-bringup-host-byte-match.json').read_text());cls.fixture=json.loads((ROOT/'tests/fixtures/mdi-bringup-host-byte-match.json').read_text())
 def test_exact_match_and_current_source(self):
  r=self.report['functions'][0];self.assertTrue(self.report['all_functions_match']);self.assertEqual((r['original_bytes'],r['generated_bytes'],r['differing_bytes']),(580,580,0));self.assertEqual(hashlib.sha256((ROOT/'compat/mdi-bringup-host-match/MDI.C').read_bytes()).hexdigest(),self.report['source_sha256'])
 def test_fixture_reproduces_match(self):
  m,_=object_module(bytes.fromhex(self.fixture['object_hex']));self.assertIn('BRINGUPHOSTDLG',m.publics[0]['name'].upper());self.assertTrue(compare(bytes.fromhex(self.fixture['object_hex']))['all_functions_match'])
if __name__=='__main__':unittest.main()
