import hashlib,json,unittest
from tools.compare_byte_match import object_module
from tools.compare_init_fonts_match import ROOT,compare
class InitFontsByteMatchTests(unittest.TestCase):
 @classmethod
 def setUpClass(cls):
  cls.report=json.loads((ROOT/'analysis/compat/init-fonts-byte-match.json').read_text());cls.fixture=json.loads((ROOT/'tests/fixtures/init-fonts-byte-match.json').read_text())
 def test_exact_match_and_current_source(self):
  r=self.report['functions'][0];self.assertTrue(self.report['all_functions_match']);self.assertEqual((r['original_bytes'],r['generated_bytes'],r['differing_bytes']),(702,702,0));self.assertEqual(hashlib.sha256((ROOT/'compat/init-fonts-match/INIT.C').read_bytes()).hexdigest(),self.report['source_sha256'])
 def test_fixture_reproduces_match(self):
  m,_=object_module(bytes.fromhex(self.fixture['object_hex']));self.assertIn('FCREATEFONTS',m.publics[0]['name'].upper());self.assertTrue(compare(bytes.fromhex(self.fixture['object_hex']))['all_functions_match'])
if __name__=='__main__':unittest.main()
