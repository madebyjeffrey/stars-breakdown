"""Resource compiler reference record and safe static-data extraction."""
import ctypes as C
from pathlib import Path
import struct
import tempfile
import unittest
from tools import package_startup as package
from tools.ne_layout import NE
from tests import test_common_machine as common_tests


class StartupPackageTests(unittest.TestCase):
    def test_rc311_reference_record_and_resource_payloads(self):
        # Generated independently by the installed Microsoft RC 3.11:
        # 1 RCDATA BEGIN 1, 2, 3 END
        class Probe:
            b=struct.pack('<3H',1,2,3)
            resources=[dict(type=10,id=1,flags=0x30,bytes=6,file_offset=0)]
        self.assertEqual(package.resource_file(Probe()),bytes.fromhex('ff0a00ff0100300006000000010002000300'))
        ne=NE(package.ROOT/'target/stars.exe');blob=package.resource_file(ne);cursor=0;rows=[]
        def identifier():
            nonlocal cursor
            if blob[cursor]==255:
                value=struct.unpack_from('<H',blob,cursor+1)[0];cursor+=3;return value
            end=blob.index(0,cursor);value=blob[cursor:end].decode('cp1252');cursor=end+1;return value
        while cursor<len(blob):
            typ,name=identifier(),identifier()
            flags,count=struct.unpack_from('<HI',blob,cursor);cursor+=6
            data=blob[cursor:cursor+count];cursor+=count
            rows.append((typ,name,flags,count,package.sha(data)))
        self.assertEqual(rows,[(r['type'],r['id'],r['flags'],r['bytes'],r['sha256']) for r in ne.resources])
        self.assertEqual(cursor,len(blob));self.assertEqual(len(rows),122)

    def test_workshop_payload_is_dos_text_and_has_all_native_inputs(self):
        files=package.payload()
        for name in ['APP.H','NATIVE.C','CONFIG.C','CONFIG.H','CHECKS.TXT']:
            self.assertIn(name,files)
        self.assertIn(b'COMMDLG TOOLHELP',files['START.LNK'])
        for name,data in files.items():
            self.assertRegex(name,r'^[A-Z0-9_]{1,8}\.[A-Z0-9_]{1,3}$')
            if Path(name).suffix in ['.C','.H','.INC','.TXT','.BAT','.LNK','.RSP','.DEF']:
                data.decode('ascii')
                self.assertNotIn(b'\n',data.replace(b'\r\n',b''))
        self.assertIn(b'REGRESS',files['README.TXT'])
        self.assertNotIn('STARSREC.INI',files)

    def test_stale_native_source_is_rejected_before_export(self):
        files=package.payload()
        first=next(n for n in files if Path(n).suffix in ['.C','.H','.INC','.RES','.BIN','.RSP','.LNK','.DEF'])
        with tempfile.TemporaryDirectory() as directory:
            root=Path(directory);build=root/'build';build.mkdir()
            (build/first).write_bytes(b'stale input')
            with self.assertRaisesRegex(ValueError,'Native build input differs'):
                package.export(root/'output',native_build=build)
            self.assertFalse((root/'output').exists())

    def test_native_roundtrip_when_local_build_available(self):
        exe=package.ROOT/'runs/step4/guest/STARTDEV/START.EXE'
        if not exe.exists():self.skipTest('optional local VC152 build absent')
        self.assertEqual(package.compare_resources(NE(package.ROOT/'target/stars.exe'),NE(exe)),122)


class StartupStringsTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        common_tests.CommonMachineTests.setUpClass.__func__(cls)
        ptr=C.POINTER(C.c_ubyte)
        cls.lib.stars_copy_game_string.argtypes=[ptr,ptr,C.c_int16,C.c_uint16]
        cls.lib.stars_copy_game_string.restype=C.c_int16

    @classmethod
    def tearDownClass(cls):cls.temp.cleanup()

    def test_all_strings_survive_code_removal_from_data_carrier(self):
        ne=NE(package.ROOT/'target/stars.exe')
        data=(C.c_ubyte*65536).from_buffer_copy(ne.raw[36].ljust(65536,b'\0'))
        full=(C.c_ubyte*65536).from_buffer_copy(ne.raw[2].ljust(65536,b'\0'))
        extracted=(C.c_ubyte*65536).from_buffer_copy(ne.raw[2][:0x743a].ljust(65536,b'\0'))
        for id in range(1414):
            data[0x1484]=255
            n=self.lib.stars_copy_game_string(data,full,id,0xc000)
            text=bytes(data[0xc000:0xc001+n])
            n2=self.lib.stars_copy_game_string(data,extracted,id,0xc000)
            self.assertEqual(n,n2);self.assertEqual(text,bytes(data[0xc000:0xc001+n2]))


if __name__=='__main__':unittest.main()
