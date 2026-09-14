"""Portable runner/output checks; these do not stand in for a native VC build."""
import json
from pathlib import Path
import subprocess
import tempfile
import unittest

from tools.package_racevc import ROOT, payload, sha


class RaceVcPackageTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.files = payload()
        cls.temp = tempfile.TemporaryDirectory()
        cls.root = Path(cls.temp.name)
        cls.source = cls.root / 'source'
        cls.write_package(cls.source)
        cls.executable = cls.root / 'runner'
        subprocess.run(['cc', '-x', 'c', '-std=c89', '-pedantic', '-Wall', '-Wextra',
                        '-Werror', '-O2', '-DHOST_TEST', '-I', str(cls.source),
                        *map(str, sorted(cls.source.glob('*.C'))),
                        '-o', str(cls.executable)], check=True, capture_output=True)

    @classmethod
    def tearDownClass(cls):
        cls.temp.cleanup()

    @classmethod
    def write_package(cls, directory):
        directory.mkdir()
        for name, data in cls.files.items():
            path = directory / name
            path.parent.mkdir(exist_ok=True)
            path.write_bytes(data)

    def run_at(self, name):
        folder = self.root / name
        self.write_package(folder)
        return folder

    def test_original_provenance_and_complete_compiler_input(self):
        frozen = json.loads((ROOT / 'compat/racevc/vectors.json').read_text())
        audit = json.loads((ROOT / 'analysis/compat/race-acceptance.json').read_text())
        tracked = {s['filename']: s for c in audit['cases'] for s in c['sessions']}
        for row in frozen['cases']:
            for session in row['sessions']:
                expected = tracked[session['filename']]
                self.assertEqual(sha(bytes.fromhex(session['player'])), expected['player_sha256'])
                if session['wire'] is not None:
                    self.assertEqual(sha(bytes.fromhex(session['wire'])), expected['wire_sha256'])
        manifest = json.loads(self.files['MANIFEST.JSN'])
        self.assertEqual(manifest['native_status'], 'pending user build/run')
        for name, expected in manifest['files'].items():
            self.assertEqual(sha(self.files[name]), expected)
        self.assertEqual(payload(), self.files)
        self.assertIn(b'/AL ', self.files['RACEVC.RSP'])
        self.assertIn(b'LLIBCEW.LIB LIBW.LIB /NOD', self.files['RACEVC.LNK'])
        for name, data in self.files.items():
            for part in name.split('/'):
                self.assertRegex(part, r'^[A-Z0-9_]{1,8}(\.[A-Z0-9_]{1,3})?$')
            if name.endswith(('.C', '.H', '.INC', '.BAT', '.RSP', '.LNK', '.DEF', '.TXT', '.JSN')):
                data.decode('ascii')
                self.assertNotIn(b'\n', data.replace(b'\r\n', b''))
            if name.endswith('.C'):
                self.assertIn(name.encode('ascii'), self.files['RACEVC.RSP'])
                self.assertIn((Path(name).stem+'.OBJ').encode('ascii'), self.files['RACEVC.LNK'])
        self.assertEqual([n for n in self.files if n.startswith('ACTUAL/')], ['ACTUAL/README.TXT'])

    def test_all_88_scenarios_create_and_reload_real_files(self):
        folder = self.run_at('full')
        run = subprocess.run([str(self.executable)], cwd=folder, timeout=30)
        self.assertEqual(run.returncode, 0)
        log = (folder / 'RESULTS.TXT').read_text()
        self.assertIn('CASES=88 SESSIONS=89 CHECKS=812 FAILURES=0', log)
        self.assertIn('STATUS=PASS', log)
        self.assertIn('PASS U020B.R1 points=25 load=0', log)
        self.assertEqual(len(list((folder / 'ACTUAL').glob('*.R1'))), 61)
        self.assertEqual(len(list((folder / 'ACTUAL').glob('*.STA'))), 89)
        for row in json.loads((ROOT / 'compat/racevc/vectors.json').read_text())['cases']:
            for s in row['sessions']:
                self.assertEqual((folder / 'ACTUAL' / (Path(s['filename']).stem+'.STA')).read_bytes(),
                                 bytes.fromhex(s['player']))
                if s['wire'] is not None:
                    self.assertEqual((folder / 'ACTUAL' / s['filename']).read_bytes(), bytes.fromhex(s['wire']))
        hashes = {p.name: sha(p.read_bytes()) for p in (folder / 'ACTUAL').iterdir()}
        repeat = subprocess.run([str(self.executable)], cwd=folder, timeout=30)
        self.assertEqual(repeat.returncode, 2)
        self.assertEqual(log, (folder / 'RESULTS.TXT').read_text())
        self.assertEqual(hashes, {p.name: sha(p.read_bytes()) for p in (folder / 'ACTUAL').iterdir()})

    def test_wrong_expectation_fails_without_changing_generated_output(self):
        folder = self.run_at('mutation')
        p = folder / 'EXPECT/B000.R1'
        original = p.read_bytes(); changed = bytearray(original); changed[5] ^= 1
        p.write_bytes(changed)
        run = subprocess.run([str(self.executable)], cwd=folder, timeout=30)
        self.assertEqual(run.returncode, 1)
        self.assertEqual((folder / 'ACTUAL/B000.R1').read_bytes(), original)
        log = (folder / 'RESULTS.TXT').read_text()
        self.assertIn('FAIL entire race file', log)
        self.assertIn('FAILURES=1', log)
        self.assertIn('STATUS=FAIL', log)

    def test_truncated_vectors_fail_clearly(self):
        folder = self.run_at('truncated')
        (folder / 'CASES.DAT').write_bytes(self.files['CASES.DAT'][:70])
        run = subprocess.run([str(self.executable)], cwd=folder, timeout=30)
        self.assertEqual(run.returncode, 1)
        self.assertIn('FATAL: Truncated test input', (folder / 'RESULTS.TXT').read_text())

    def test_windows_launcher_uses_declared_crt_entry(self):
        # Compile the normally excluded Windows branch against the documented
        # declaration subset. This catches the RETURN3 alias mistake; it is
        # not a native Win16 compiler, ABI, SDK-header or library-link test.
        headers = self.root / 'declarations'
        headers.mkdir()
        (headers / 'windows.h').write_text('''
typedef unsigned short HGLOBAL;
typedef unsigned short HINSTANCE;
typedef void *HWND;
typedef char *LPSTR;
#define PASCAL
#define GMEM_FIXED 0
#define GMEM_ZEROINIT 0x40
#define MB_OK 0
#define MB_ICONEXCLAMATION 0x30
#define MB_ICONINFORMATION 0x40
HGLOBAL GlobalAlloc(unsigned, unsigned long);
void *GlobalLock(HGLOBAL);
int GlobalUnlock(HGLOBAL);
HGLOBAL GlobalFree(HGLOBAL);
unsigned GetModuleFileName(HINSTANCE, LPSTR, unsigned);
int MessageBox(HWND, const char *, const char *, unsigned);
''')
        (headers / 'dos.h').write_text('#define FP_OFF(p) ((unsigned long)(p) & 65535UL)\n')
        (headers / 'direct.h').write_text('int _chdir(const char *);\nint _chdrive(int);\n')
        flags = ['cc', '-x', 'c', '-std=c89', '-pedantic', '-Wall', '-Wextra', '-Werror',
                 '-fsyntax-only', '-D_MSC_VER=800', '-I', str(headers), '-I', str(self.source)]
        subprocess.run(flags+[str(self.source / 'RUNNER.C')], check=True, capture_output=True)
        wrong = headers / 'MUTANT.C'
        source = (self.source / 'RUNNER.C').read_text()
        changed = source.replace('if (_chdir(message))', 'if (chdir(message))')
        self.assertNotEqual(source, changed)
        wrong.write_text(changed)
        result = subprocess.run(flags+[str(wrong)], capture_output=True, text=True)
        self.assertNotEqual(result.returncode, 0)
        self.assertIn('chdir', result.stderr)


if __name__ == '__main__':
    unittest.main()
