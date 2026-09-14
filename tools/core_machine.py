"""Execute selected original routines, with real NE relocations.

This is a CPU-level oracle, not a Windows/selector-protection emulator.
Unmapped modules/imports trap. Explicit import slots may be supplied by
specialized harnesses; the base harness substitutes no operating-system functions.
"""
from pathlib import Path
import hashlib
import struct
import sys

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT / '.tools/python-validation'))
from unicorn import Uc, UC_ARCH_X86, UC_MODE_16, UC_HOOK_CODE
from unicorn import x86_const as reg
from tools.ne_layout import NE, patch


class OriginalCore:
    SEGMENTS = {9: 0x1000, 36: 0x2000, 37: 0x3000, 29: 0x4000, 16: 0x7000, 10: 0x8000, 3: 0x9000}
    DATA = 0x30000
    STOP = 0x60000

    def __init__(self, imports=None):
        path = ROOT / 'target/stars.exe'
        assert hashlib.sha256(path.read_bytes()).hexdigest() == (
            'ccab6682b55dae14dbf104e491cfbd3b32a6280022020032ae0615500949ba59')
        ne = NE(path)
        self.cpu = Uc(UC_ARCH_X86, UC_MODE_16)
        self.cpu.mem_map(0x10000, max(self.SEGMENTS.values()) * 16)
        for number, selector in self.SEGMENTS.items():
            memory = bytearray(ne.raw[number - 1])
            for relocation in ne.relocations:
                if relocation['segment'] != number:
                    continue
                if relocation['target_kind'] == 'os_fpu_fixup':
                    continue
                target = self.SEGMENTS.get(relocation.get('target_segment'), 0xf000)
                offset = relocation.get('target_offset', 0)
                if relocation.get('import_key') in (imports or {}):
                    target, offset = imports[relocation['import_key']]
                for site in relocation['sites']:
                    patch(memory, site, relocation['address_kind'], target,
                          offset, relocation['additive'])
            self.cpu.mem_write(selector * 16, bytes(memory))
        self.cpu.mem_write(self.STOP, b'\xf4')
        self.cpu.hook_add(UC_HOOK_CODE, self._stop, begin=self.STOP, end=self.STOP)
        self.returned = False

    def _stop(self, cpu, address, size, data):
        self.returned = True
        cpu.emu_stop()

    def put(self, offset, data):
        self.cpu.mem_write(self.DATA + offset, data)

    def get(self, offset, size):
        return bytes(self.cpu.mem_read(self.DATA + offset, size))

    def seeds(self, file=False, value=None):
        offset = 0x2272 if file else 0x1370
        if value is not None:
            self.put(offset, struct.pack('<II', *value))
        return struct.unpack('<II', self.get(offset, 8))

    def call(self, offset, arguments=b'', segment=9, shared_stack=False, instruction_budget=20000000, entry_si=0x1234):
        cpu = self.cpu
        self.returned = False
        for name, value in dict(CS=self.SEGMENTS[segment], IP=offset,
                                DS=0x3000, ES=0x3000, SS=0x3000 if shared_stack else 0x5000,
                                SP=0xff00, BP=0, AX=0, BX=0, CX=0, DX=0,
                                SI=entry_si, DI=0x5678, EFLAGS=2).items():
            cpu.reg_write(getattr(reg, 'UC_X86_REG_' + name), value)
        cpu.mem_write(0x3ff00 if shared_stack else 0x5ff00,
                      struct.pack('<HH', 0, 0x6000) + arguments)
        cpu.emu_start(self.SEGMENTS[segment] * 16 + offset, self.STOP + 1,
                      count=instruction_budget)
        if not self.returned:
            raise RuntimeError('Original routine exceeded instruction budget')
        assert cpu.reg_read(reg.UC_X86_REG_SP) == 0xff04, 'Unexpected calling convention'
        assert cpu.reg_read(reg.UC_X86_REG_SI) == entry_si
        assert cpu.reg_read(reg.UC_X86_REG_DI) == 0x5678
        return cpu.reg_read(reg.UC_X86_REG_AX) | (cpu.reg_read(reg.UC_X86_REG_DX) << 16)
