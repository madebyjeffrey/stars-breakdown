import hashlib,json,tempfile,unittest
from pathlib import Path
from tools.compare_startup_rest_byte_match import TARGETS,ROOT,compare
class StartupRestByteMatchTests(unittest.TestCase):
 @classmethod
 def setUpClass(cls): cls.report=json.loads((ROOT/'analysis/compat/startup-rest-byte-match.json').read_text())
 def test_exact_match_and_current_sources(self):
  self.assertTrue(self.report['all_functions_match'])
  self.assertTrue(all(r['exact_after_address_resolution'] and r['differing_bytes']==0 and r['original_bytes']==r['generated_bytes'] for r in self.report['functions']))
  for r in self.report['functions']: self.assertEqual(hashlib.sha256((ROOT/r['source']).read_bytes()).hexdigest(),r['source_sha256'])
 def test_fixture_reproduces_match(self):
  with tempfile.TemporaryDirectory() as d:
   p=Path(d)
   for name,blob in self.report['objects'].items():
    obj=next(x[4] for x in TARGETS if x[0]==name);(p/obj).write_bytes(bytes.fromhex(blob))
   self.assertTrue(compare(p)['all_functions_match'])
if __name__=='__main__':unittest.main()
