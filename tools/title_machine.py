"""Original title/resource instructions; only Win16 OS calls are modeled."""
import struct
from tools.startup_machine import StartupMachine, StartupWorld
from tools.race_ui_machine import signed, words_bytes, reg


class TitleWorld(StartupWorld):
    def __init__(self, write_near, read_near):
        super().__init__(write_near, read_near)
        self.client = (0, 0, 800, 600)
        self.position = (0, 0)
        self.metrics.update({0: 800, 1: 600})
        self.huge = bytearray(600000)
        self.resource = bytes((i * 37 + 11) & 255 for i in range(100000))
        self.file_position = 0
        self.failure = None
        self.short_read_at = None
        self.reads = 0
        self.profile = {}
        self.iconic = 0
        self.realized = 1
        self.next_window = 201

    def call(self, name, h=0, a=0, b=0, v=0, p=None, q=None):
        result, output = 1, None
        if name == 'FindResource': result = 0 if self.failure == name else 50
        elif name == 'AllocResource': result = 0 if self.failure == name else 60
        elif name == 'AccessResource':
            result = 65535 if self.failure == name else 7
            self.file_position = 0
        elif name in ['LockResource', 'GlobalLock']:
            result = 0 if self.failure == name else 0xc0000000
        elif name == 'SizeofResource': result = len(self.resource)
        elif name == '_lread':
            count = a - 1 if self.reads == self.short_read_at else a
            chunk = self.resource[self.file_position:self.file_position + count]
            result = len(chunk)
            offset = (((v >> 16) - 0xc000) & 65535) // 8 * 65536 + (v & 65535)
            assert offset + result <= len(self.huge)
            self.huge[offset:offset + result] = chunk
            self.file_position += result
            self.reads += 1
        elif name == '_lclose': result = 0
        elif name == 'CreatePalette': result = 0 if self.failure == name else 77
        elif name == 'CreateWindow':
            result = self.next_window; self.next_window += 1
            cls, label, x, y, width, height, instance, menu = p
            self.rectangles[result] = (signed(x), signed(y), signed(x) + signed(width), signed(y) + signed(height))
            self.text[result] = label
        elif name == 'SendMessage' and a == 0x30: result = 0
        elif name == 'IsIconic': result = self.iconic
        elif name == 'DrawIcon': pass
        elif name == 'RealizePalette': result = self.realized
        elif name == 'InflateRect':
            output = tuple(x & 65535 for x in (p[0]-signed(a), p[1]-signed(b), p[2]+signed(a), p[3]+signed(b)))
        elif name == 'GetPrivateProfileString':
            section, key, fallback, file = q
            output = self.profile.get((section, key), fallback)[:a-1] + b'\0'
            result = len(output)-1
        elif name == 'DefWindowProc': result = 0x12345678
        else: return super().call(name, h, a, b, v, p, q)
        self.events.append((name, h, a, b, v, p, q))
        return result, output


class TitleMachine(StartupMachine):
    SEGMENTS = dict(StartupMachine.SEGMENTS)
    SEGMENTS.update({5: 0xa000, 8: 0xb000})
    WINDOWS = dict(StartupMachine.WINDOWS)
    EXTRA = {
        'KERNEL!#60': ('FindResource', 10), 'KERNEL!#66': ('AllocResource', 8),
        'KERNEL!#64': ('AccessResource', 4), 'KERNEL!#62': ('LockResource', 2),
        'KERNEL!#65': ('SizeofResource', 4), 'KERNEL!#82': ('_lread', 8),
        'KERNEL!#81': ('_lclose', 2), 'GDI!#360': ('CreatePalette', 4),
        'USER!#41': ('CreateWindow', 30), 'USER!#31': ('IsIconic', 2),
        'USER!#84': ('DrawIcon', 8), 'USER!#78': ('InflateRect', 8),
        'KERNEL!#128': ('GetPrivateProfileString', 22),
        'USER!#107': ('DefWindowProc', 10),
    }
    WINDOWS.update(EXTRA)

    def __init__(self):
        super().__init__()
        # KERNEL ordinal 113 is the exported constant __AHSHIFT=3, not a
        # function. Resolve its NE9:538a immediate-word import relocation.
        # The base CPU loader otherwise gives unknown imports offset zero.
        self.cpu.mem_write(0x1538a, struct.pack('<H', 3))
        self.world = TitleWorld(self.put, self.get)

    def _window_api(self, cpu, address, size, unused):
        name, cleanup = self.window_slots[address]
        if name not in {n for n, _ in self.EXTRA.values()}:
            return super()._window_api(cpu, address, size, unused)
        w = struct.unpack('<' + 'H' * (cleanup // 2), bytes(cpu.mem_read(self._stack() + 4, cleanup)))
        def ptr(i): return w[i] | w[i+1] << 16
        def linear(i): return w[i] + w[i+1] * 16
        def string(i): return self._string(linear(i))
        h = a = b = v = 0
        p = q = output_pointer = None
        if name == 'FindResource': h, a, b = w[4], w[2], w[0]
        elif name == 'AllocResource': h, a, v = w[3], w[2], ptr(0)
        elif name in ['AccessResource', 'SizeofResource']: h, a = w[1], w[0]
        elif name in ['LockResource', '_lclose', 'IsIconic']: h = w[0]
        elif name == '_lread': h, a, v = w[3], w[0], ptr(1)
        elif name == 'CreatePalette':
            count = struct.unpack('<H', bytes(cpu.mem_read(linear(0)+2, 2)))[0]
            p = bytes(cpu.mem_read(linear(0), 4+count*4))
        elif name == 'CreateWindow':
            h, v = w[4], ptr(9)
            p = (string(13), string(11), w[8], w[7], w[6], w[5], w[2], w[3])
        elif name == 'DrawIcon': h, a, b, v = w[3], w[2], w[1], w[0]
        elif name == 'InflateRect':
            a, b = w[1], w[0]
            p = struct.unpack('<4H', bytes(cpu.mem_read(linear(2), 8)))
            output_pointer = linear(2)
        elif name == 'GetPrivateProfileString':
            a = w[2]; q = (string(9), string(7), string(5), string(0))
            output_pointer = linear(3)
        elif name == 'DefWindowProc': h, a, b, v = w[4], w[3], w[2], ptr(0)
        else: raise AssertionError(name)
        result, output = self.world.call(name, h, a, b, v, p, q)
        if name == '_lread': cpu.mem_write(0xc0000, bytes(self.world.huge[:65536]))
        if output_pointer is not None and output is not None:
            cpu.mem_write(output_pointer, words_bytes(output) if isinstance(output, tuple) else output)
        cpu.reg_write(reg.UC_X86_REG_AX, result & 65535)
        cpu.reg_write(reg.UC_X86_REG_DX, result >> 16)
