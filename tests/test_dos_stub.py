"""Assemble the reviewed DOS path and compare bytes and DOS-visible effects."""
from pathlib import Path
import os
import platform
import struct
import subprocess
import sys
import tempfile
import unittest
from tools.core_machine import ROOT, Uc, UC_ARCH_X86, UC_MODE_16, reg
from unicorn import UC_HOOK_INTR
from tools.review_runtime_library import sha


def elf_text(blob):
    if blob[:7] != b'\x7fELF\x01\x01\x01':
        raise ValueError('Expected little-endian ELF32 object')
    shoff = struct.unpack_from('<I', blob, 32)[0]
    size, count, names = struct.unpack_from('<3H', blob, 46)
    sections = [struct.unpack_from('<10I', blob, shoff + i * size) for i in range(count)]
    strings = blob[sections[names][4]:sections[names][4] + sections[names][5]]
    text = [s for s in sections if strings[s[0]:].split(b'\0')[0] == b'.text']
    if len(text) != 1 or any(s[1] in [4, 9] and s[5] for s in sections):
        raise ValueError('DOS stub must have one text section and no relocations')
    s = text[0]
    return blob[s[4]:s[4] + s[5]]


class DosStubTests(unittest.TestCase):
    def test_exact_assembled_bytes_and_exit(self):
        if (sys.platform == 'darwin' and platform.machine() == 'arm64' and
                os.environ.get('STARS_ALLOW_UNICORN') != '1'):
            self.skipTest('Unicorn x86-16 execution disabled on macOS arm64')
        with tempfile.TemporaryDirectory() as directory:
            obj = Path(directory) / 'stub.o'
            subprocess.run(['clang', '--target=i386-unknown-linux-gnu', '-c',
                str(ROOT / 'reconstructed/src/dos_stub.s'), '-o', str(obj)], check=True)
            rebuilt = elf_text(obj.read_bytes())
        original = (ROOT / 'target/stars.exe').read_bytes()
        self.assertEqual(sha(original), 'ccab6682b55dae14dbf104e491cfbd3b32a6280022020032ae0615500949ba59')
        base = struct.unpack_from('<H', original, 8)[0] * 16
        self.assertEqual(base, 0x200)
        self.assertEqual(len(rebuilt), 0x62)
        self.assertEqual(rebuilt, original[base:base + len(rebuilt)])
        for image in [original[base:base + len(rebuilt)], rebuilt]:
            cpu = Uc(UC_ARCH_X86, UC_MODE_16)
            cpu.mem_map(0x10000, 0x20000)
            cpu.mem_write(0x10000, image)
            for register, value in [('CS', 0x1000), ('SS', 0x2000), ('SP', 0xfffe),
                                    ('DS', 0x2222), ('EFLAGS', 2)]:
                cpu.reg_write(getattr(reg, 'UC_X86_REG_' + register), value)
            events = []
            def interrupt(machine, number, _):
                self.assertEqual(number, 0x21)
                ax = machine.reg_read(reg.UC_X86_REG_AX)
                if ax >> 8 == 9:
                    pointer = machine.reg_read(reg.UC_X86_REG_DS) * 16 + machine.reg_read(reg.UC_X86_REG_DX)
                    events.append(('print', bytes(machine.mem_read(pointer, 83)).split(b'$')[0]))
                elif ax >> 8 == 0x4c:
                    events.append(('exit', ax & 255))
                    machine.emu_stop()
                else:
                    self.fail('Unexpected DOS service')
            cpu.hook_add(UC_HOOK_INTR, interrupt)
            cpu.emu_start(0x10000, 0x10062, count=20)
            self.assertEqual(events, [('print', b'This program requires Microsoft Windows.\r\n'), ('exit', 1)])
            self.assertEqual(cpu.reg_read(reg.UC_X86_REG_SP), 0xfffe)
