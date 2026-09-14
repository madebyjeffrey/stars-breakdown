"""Original memory.obj and its game error callees, with modeled Global* APIs.

All allocation policy executes from the pinned EXE. The OS supplies independent
selector images and may relocate unlocked blocks. The unhandled CRT longjmp
is captured at entry; formatting and alert selection execute unchanged.
"""
import struct
from tools.core_machine import OriginalCore, reg
from unicorn import UC_HOOK_CODE


class MemoryAbort(Exception):
    pass


class HeapServices:
    """Deterministic OS fixture, shared only at the Windows API boundary."""
    SELECTORS = (0x7000, 0x8000, 0xa000, 0xb000, 0xc000, 0xf000)

    def __init__(self, read, write, base=32):
        self.read, self.write, self.base = read, write, base
        self.blocks = {}
        self.events = []
        self.next_handle = 100
        self.move = False
        self.change_handle = False
        self.fail_alloc = False
        self.fail_realloc = False
        self.touched = set()

    def _slot(self):
        used = {b['selector'] for b in self.blocks.values() if b['live']}
        return next(s for s in self.SELECTORS if s not in used)

    def _fresh(self, selector, handle):
        # Explicitly not GMEM_ZEROINIT. Retain guard/payload bytes in comparisons.
        self.write(selector, 0, bytes(((i * 13 + handle) & 255 for i in range(65536))))
        self.touched.add(selector)

    def alloc(self, flags, size):
        assert flags == 0x22 and size <= 65536 - self.base
        if self.fail_alloc:
            self.events.append(('GlobalAlloc', flags, size, 0))
            return 0
        h = self.next_handle
        self.next_handle += 1
        selector = self._slot()
        self._fresh(selector, h)
        self.blocks[h] = dict(selector=selector, base=self.base, size=size, locks=0, live=True)
        self.events.append(('GlobalAlloc', flags, size, h))
        return h

    def realloc(self, handle, size, flags):
        b = self.blocks[handle]
        assert b['live'] and not b['locks'] and flags == 0x22
        assert size <= 65536 - b['base']
        if self.fail_realloc:
            self.events.append(('GlobalReAlloc', handle, size, flags, 0))
            return 0
        new = self.next_handle if self.change_handle else handle
        if self.change_handle:
            self.next_handle += 1
        selector = self._slot() if self.move else b['selector']
        payload = self.read(b['selector'], b['base'], min(b['size'], size))
        if self.move:
            self._fresh(selector, new)
            self.write(selector, b['base'], payload)
        self.write(selector, b['base'] + b['size'], b'\xa5' * (size - b['size']))
        b['live'] = False
        self.blocks[new] = dict(selector=selector, base=b['base'], size=size, locks=0, live=True)
        self.events.append(('GlobalReAlloc', handle, size, flags, new))
        return new

    def lock(self, handle):
        b = self.blocks[handle]
        assert b['live']
        b['locks'] += 1
        p = (b['selector'] << 16) | b['base']
        self.events.append(('GlobalLock', handle, p))
        return p

    def unlock(self, handle):
        b = self.blocks[handle]
        assert b['live'] and b['locks']
        b['locks'] -= 1
        result = int(bool(b['locks']))
        self.events.append(('GlobalUnlock', handle, result))
        return result

    def free(self, handle):
        b = self.blocks[handle]
        assert b['live'] and not b['locks']
        b['live'] = False
        self.events.append(('GlobalFree', handle, 0))
        return 0


class OriginalMemory(OriginalCore):
    SEGMENTS = {13: 0x1000, 36: 0x2000, 37: 0x3000, 7: 0x4000, 9: 0x9000, 3: 0xd000}

    def __init__(self, base=32):
        slots = {f'KERNEL!#{n}': (0xe000, (n - 14) * 0x100) for n in range(15, 20)}
        slots.update({'USER!#23': (0xe000, 0x800), 'USER!#1': (0xe000, 0x900)})
        super().__init__(slots)
        self.cpu.mem_map(0xe0000, 0x20000)
        self.heap = HeapServices(self.read_heap, self.write_heap, base)
        self.events = self.heap.events
        self.visited = set()
        for off, cleanup in [(0x100, 6), (0x200, 8), (0x300, 2), (0x400, 2),
                             (0x500, 2), (0x800, 0), (0x900, 12)]:
            self.cpu.mem_write(0xe0000 + off, b'\xca' + struct.pack('<H', cleanup) if cleanup else b'\xcb')
        self.cpu.hook_add(UC_HOOK_CODE, self._api, begin=0xe0000, end=0xe0900)
        self.cpu.hook_add(UC_HOOK_CODE, self._abort, begin=0x209ed, end=0x209ed)
        self.cpu.hook_add(UC_HOOK_CODE, self._visit, begin=0x10000, end=0x10958)

    def read_heap(self, selector, offset, count):
        return bytes(self.cpu.mem_read(selector * 16 + offset, count))

    def write_heap(self, selector, offset, value):
        self.cpu.mem_write(selector * 16 + offset, bytes(value))

    def _visit(self, cpu, address, size, unused):
        self.visited.add(address - 0x10000)

    def _stack(self):
        return self.cpu.reg_read(reg.UC_X86_REG_SS) * 16 + self.cpu.reg_read(reg.UC_X86_REG_SP)

    def _string(self, address):
        value = bytes(self.cpu.mem_read(address, 1024))
        return value[:value.index(0)]

    def _abort(self, cpu, address, size, unused):
        env, value = struct.unpack('<Hh', bytes(cpu.mem_read(self._stack() + 4, 4)))
        self.events.append(('abort', env, value))
        raise MemoryAbort()

    def _api(self, cpu, address, size, unused):
        args = bytes(cpu.mem_read(self._stack() + 4, 12))
        if address == 0xe0100:
            count, flags = struct.unpack_from('<IH', args)
            result = self.heap.alloc(flags, count)
        elif address == 0xe0200:
            flags, count, handle = struct.unpack_from('<HIH', args)
            result = self.heap.realloc(handle, count, flags)
        elif address in (0xe0300, 0xe0400, 0xe0500):
            handle = struct.unpack_from('<H', args)[0]
            fn = {0xe0300: self.heap.free, 0xe0400: self.heap.lock, 0xe0500: self.heap.unlock}[address]
            result = fn(handle)
        elif address == 0xe0800:
            result = 0x1234
            self.events.append(('focus', result))
        elif address == 0xe0900:
            kind, cap, cs, text, ts, owner = struct.unpack('<6H', args)
            self.events.append(('message', owner, self._string(ts * 16 + text), self._string(cs * 16 + cap), kind))
            result = 1
        else:
            raise AssertionError(hex(address))
        cpu.reg_write(reg.UC_X86_REG_AX, result & 65535)
        cpu.reg_write(reg.UC_X86_REG_DX, result >> 16)
