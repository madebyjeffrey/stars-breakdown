"""Startup resource oracle. Only Windows services are modeled; no game stubs."""
import struct
from tools.race_ui_machine import RaceUiMachine, WindowWorld, reg


class StartupWorld(WindowWorld):
    def __init__(self, write_near, read_near):
        super().__init__(write_near, read_near)
        self.colors = {i: (i * 0x010307) & 0xffffff for i in range(32)}
        self.metrics = {0: 1024, 1: 768, 4: 20, 32: 3, 33: 4}
        self.caps = {12: 8, 14: 1, 90: 96}
        self.brush = 0x1200
        self.fail_brush = False
        self.fail_font = False

    def call(self, name, h=0, a=0, b=0, v=0, p=None, q=None):
        result = None
        if name == 'GetSysColor':
            result = self.colors[a]
        elif name == 'GetSystemMetrics':
            result = self.metrics[a]
        elif name == 'GetDeviceCaps':
            result = self.caps[a]
        elif name == 'CreateSolidBrush':
            result = 0 if self.fail_brush else self.brush
            self.brush += 1
        elif name == 'CreateFontIndirect':
            result = 0 if self.fail_font else self.font
            self.font += 1
        elif name == 'LoadCursor':
            result = 0x7f00
        elif name in ['FreeResource', 'FreeProcInstance', 'DestroyCursor', 'DestroyIcon', 'GlobalFree']:
            result = 0
        if result is None:
            return super().call(name, h, a, b, v, p, q)
        self.events.append((name, h, a, b, v, p, q))
        return result & 0xffffffff, None


class StartupMachine(RaceUiMachine):
    SEGMENTS = {n: s for n, s in RaceUiMachine.SEGMENTS.items() if n not in [5, 8, 10, 15, 29, 30]}
    SEGMENTS.update({1: 0x7000, 13: 0x4000})
    WINDOWS = dict(RaceUiMachine.WINDOWS)
    WINDOWS.update({'GDI!#66': ('CreateSolidBrush', 4), 'GDI!#80': ('GetDeviceCaps', 4),
                    'USER!#180': ('GetSysColor', 2), 'USER!#173': ('LoadCursor', 6),
                    'USER!#457': ('DestroyIcon', 2), 'USER!#458': ('DestroyCursor', 2),
                    'KERNEL!#63': ('FreeResource', 2), 'KERNEL!#17': ('GlobalFree', 2)})

    def __init__(self):
        super().__init__()
        self.world = StartupWorld(self.put, self.get)

    def _window_api(self, cpu, address, size, unused):
        name, cleanup = self.window_slots[address]
        added = ['CreateSolidBrush', 'GetDeviceCaps', 'GetSysColor', 'LoadCursor',
                 'DestroyIcon', 'DestroyCursor', 'FreeResource', 'GlobalFree', 'FreeProcInstance']
        if name not in added:
            return super()._window_api(cpu, address, size, unused)
        w = struct.unpack('<' + 'H' * (cleanup // 2), bytes(cpu.mem_read(self._stack() + 4, cleanup)))
        h = a = v = 0
        if name in ['CreateSolidBrush', 'FreeProcInstance']:
            v = w[0] | w[1] << 16
        elif name == 'GetDeviceCaps':
            h, a = w[1], w[0]
        elif name == 'GetSysColor':
            a = w[0]
        elif name == 'LoadCursor':
            h, v = w[2], w[0] | w[1] << 16
        else:
            h = w[0]
        result, output = self.world.call(name, h, a, 0, v)
        assert output is None
        cpu.reg_write(reg.UC_X86_REG_AX, result & 65535)
        cpu.reg_write(reg.UC_X86_REG_DX, result >> 16)
