import ctypes as C
import json
import subprocess
import unittest
from pathlib import Path

from tools.compare_tutorial_match import original_image


ROOT = Path(__file__).resolve().parents[1]
PTR = C.POINTER(C.c_ubyte)


class StreamApi(C.Structure):
    _fields_ = [('open', C.c_void_p), ('close', C.c_void_p),
                ('read', C.c_void_p), ('write', C.c_void_p),
                ('ticks', C.c_void_p), ('memory', C.c_void_p),
                ('file_error', C.c_void_p), ('write_error', C.c_void_p),
                ('abort', C.c_void_p)]


class AlertApi(C.Structure):
    _fields_ = [('focus', C.c_void_p), ('message', C.c_void_p),
                ('output', C.c_void_p), ('format_id', C.c_void_p)]


class Platform(C.Structure):
    _fields_ = [(name, C.c_void_p) for name in (
        'open', 'close', 'read', 'write', 'ticks', 'memory', 'access',
        'seek', 'date', 'time', 'timer_count', 'window', 'dialog',
        'save_choice', 'abort')]


class Runtime(C.Structure):
    _fields_ = [('data', PTR), ('strings', PTR), ('platform', C.c_void_p),
                ('context', C.c_void_p), ('stream', StreamApi),
                ('alerts', AlertApi), ('scratch_bottom', C.c_uint16),
                ('scratch_top', C.c_uint16), ('recovery', C.c_void_p),
                ('segments', PTR * 38)]


class MessageSourceTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        subprocess.run(['python3', 'tools/build_core.py'], cwd=ROOT, check=True)
        cls.lib = C.CDLL(str(ROOT / 'build/libstars_core.dylib'))
        cls.lib.stars_get_compressed_message.restype = C.c_uint16
        cls.lib.stars_get_compressed_message.argtypes = [
            PTR, PTR, C.c_int16
        ]
        cls.lib.stars_format_message.restype = C.c_uint16
        cls.lib.stars_format_message.argtypes = [
            C.POINTER(Runtime), C.c_int16, PTR
        ]
        cls.lib.stars_get_compressed_planet.restype = C.c_uint16
        cls.lib.stars_get_compressed_planet.argtypes = [
            C.POINTER(Runtime), C.c_int16
        ]
        cls.lib.stars_planet_name.restype = C.c_uint16
        cls.lib.stars_planet_name.argtypes = [C.POINTER(Runtime), C.c_uint16]
        for name in ('stars_thing_name', 'stars_fleet_name'):
            fn = getattr(cls.lib, name)
            fn.restype = C.c_uint16
            fn.argtypes = [C.POINTER(Runtime), C.c_uint16]
        cls.lib.stars_fleet_word_name.restype = C.c_uint16
        cls.lib.stars_fleet_word_name.argtypes = [C.POINTER(Runtime),
                                                  C.c_uint16]
        cls.lib.stars_location_name.restype = C.c_uint16
        cls.lib.stars_location_name.argtypes = [
            C.POINTER(Runtime), C.c_int16, C.c_int16, C.c_int16, C.c_int16
        ]
        cls.lib.stars_distance_string.restype = C.c_uint16
        cls.lib.stars_distance_string.argtypes = [
            C.POINTER(Runtime), C.c_int16, C.c_int16, C.c_int16, C.c_int16
        ]
        image = original_image()
        cls.data = (C.c_ubyte * 65536).from_buffer_copy(
            bytes(image[36]).ljust(65536, b'\0')
        )
        cls.messages = (C.c_ubyte * 65536).from_buffer_copy(
            bytes(image[6]).ljust(65536, b'\0')
        )

    def test_decompresses_every_message_table_entry(self):
        data = self.data
        messages = self.messages
        message_bytes = bytes(messages)
        ids = []
        for group in range(7):
            lengths = 0x5934 + group * 64
            for index in range(64):
                ids.append(group * 64 + index)
        for idm in ids:
            data[0x0b75] = 0xff
            data[0x0b76] = 0xff
            actual = self.lib.stars_get_compressed_message(
                data, messages, idm
            )
            self.assertEqual(actual, 0x505c)
            # Re-run the same indexed record independently, then compare the
            # complete output span including its terminator.
            lengths = 0x5934 + (idm >> 6) * 64
            total = sum(message_bytes[lengths:lengths + (idm & 63)])
            cursor = int.from_bytes(
                message_bytes[0x5ab8 + (idm >> 6) * 2:
                               0x5ab8 + (idm >> 6) * 2 + 2], 'little'
            ) + total // 2
            high = not (total & 1)
            count = message_bytes[lengths + (idm & 63)]
            expected = bytearray()
            character = 0
            for _ in range(count):
                if high:
                    nibble = message_bytes[cursor] >> 4
                else:
                    nibble = message_bytes[cursor] & 15
                    cursor += 1
                high = not high
                character += nibble
                if nibble != 15:
                    expected.append(message_bytes[0x5ac6 + character])
                    character = 0
            expected.append(0)
            self.assertEqual(bytes(data[0x505c:0x505c + len(expected)]),
                             bytes(expected), idm)

    def test_format_message_composes_real_c_routines(self):
        data = (C.c_ubyte * 65536)()
        messages = (C.c_ubyte * 65536)()
        # A one-nibble synthetic table entry keeps this test independent of
        # the decompressor test's expected-value implementation.  The source
        # routine still uses the original table locations and cache word.
        messages[0x5934] = 1
        messages[0x5ab8] = 0x00
        messages[0x5ab9] = 0x60
        messages[0x6000] = 0x00
        messages[0x5ac6] = ord('A')
        data[0x0b75] = 0xff
        data[0x0b76] = 0xff
        runtime = Runtime()
        runtime.data = data
        runtime.segments[7] = messages
        parameters = (C.c_ubyte * 2)()
        self.assertEqual(self.lib.stars_format_message(
            C.byref(runtime), 0, parameters), 0x535a)
        self.assertEqual(bytes(data[0x505c:0x505d]), b'A')
        self.assertEqual(bytes(data[0x535a:0x535c]), b'A\0')

    def test_decompresses_every_planet_name_entry(self):
        image = original_image()
        planet_bytes = bytes(image[8]).ljust(65536, b'\0')
        planet_segment = (C.c_ubyte * 65536).from_buffer_copy(planet_bytes)
        data = (C.c_ubyte * 65536)()
        runtime = Runtime()
        runtime.data = data
        runtime.segments[9] = planet_segment
        for idp in range(999):
            data[0x137a] = 0xff
            actual = self.lib.stars_get_compressed_planet(
                C.byref(runtime), idp
            )
            self.assertEqual(actual, 0x22c8)
            group = idp >> 6
            lengths = 0x1004 + group * 64
            total = sum(planet_bytes[lengths:lengths + (idp & 63)])
            cursor = int.from_bytes(
                planet_bytes[0x13ec + group * 2:0x13ec + group * 2 + 2],
                'little'
            ) + total // 2
            high = not (total & 1)
            count = planet_bytes[lengths + (idp & 63)]
            expected = bytearray()
            character = 0
            capitalize = True
            for _ in range(count):
                if high:
                    nibble = planet_bytes[cursor] >> 4
                else:
                    nibble = planet_bytes[cursor] & 15
                    cursor += 1
                high = not high
                character += nibble
                if nibble != 15:
                    ch = planet_bytes[0x140c + character]
                    if capitalize and 0x61 <= ch <= 0x7a:
                        ch -= 0x20
                    expected.append(ch)
                    capitalize = ch in (ord(' '), ord('-'))
                    character = 0
            expected.append(0)
            self.assertEqual(bytes(data[0x22c8:0x22c8 + len(expected)]),
                             bytes(expected), idp)

    def test_planet_name_uses_the_original_planet_index_table(self):
        image = original_image()
        planet_bytes = bytes(image[8]).ljust(65536, b'\0')
        planet_segment = (C.c_ubyte * 65536).from_buffer_copy(planet_bytes)
        data = (C.c_ubyte * 65536)()
        runtime = Runtime()
        runtime.data = data
        runtime.segments[9] = planet_segment
        for index in range(999):
            data[0x276e + index * 2] = index & 0xff
            data[0x276f + index * 2] = index >> 8
            data[0x137a] = 0xff
            self.assertEqual(self.lib.stars_planet_name(
                C.byref(runtime), index), 0x57a4)
            expected = bytes(data[0x22c8:0x22c8 + 64]).split(b'\0', 1)[0] + b'\0'
            self.assertEqual(bytes(data[0x57a4:0x57a4 + len(expected)]),
                             expected, index)

    def test_shared_name_entry_points_preserve_empty_object_paths(self):
        image = original_image()
        data = (C.c_ubyte * 65536)()
        strings = (C.c_ubyte * 65536).from_buffer_copy(
            bytes(image[2]).ljust(65536, b'\0'))
        runtime = Runtime()
        runtime.data = data
        runtime.strings = strings
        data[0x5356] = 0
        data[0x5357] = 0
        self.assertEqual(self.lib.stars_thing_name(
            C.byref(runtime), 0), 0x57a4)
        self.assertEqual(data[0x57a4], 0)
        self.assertEqual(self.lib.stars_fleet_name(
            C.byref(runtime), 0), 0x57a4)
        self.assertEqual(self.lib.stars_location_name(
            C.byref(runtime), 0, -1, -1, -1), 0x57a4)
        self.assertIn(0, bytes(data[0x57a4:0x57a4 + 256]))

    def test_fleet_word_name_uses_design_name_and_plus_marker(self):
        data = (C.c_ubyte * 65536)()
        design_segment = (C.c_ubyte * 65536)()
        data[0xfe] = 0
        data[0x100] = 1
        data[0x18c] = 0
        for offset, payload in ((0x532, b'%s %d\0'), (8, b'Scout\0')):
            target = data if offset == 0x532 else design_segment
            for delta, value in enumerate(payload):
                target[offset + delta] = value
        platform = Platform()
        callback_type = C.CFUNCTYPE(C.c_void_p, C.c_void_p, C.c_uint16,
                                    C.c_uint16)

        @callback_type
        def memory(_context, selector, _offset):
            return C.cast(design_segment, C.c_void_p).value if selector == 1 else None

        platform.memory = C.cast(memory, C.c_void_p).value
        runtime = Runtime()
        runtime.data = data
        runtime.platform = C.cast(C.pointer(platform), C.c_void_p).value
        result = self.lib.stars_fleet_word_name(
            C.byref(runtime), 0x2000 | 3)
        self.assertEqual(result, 0x57a4)
        self.assertEqual(bytes(data[0x57a4:0x57a4 + 10]).split(b'\0', 1)[0],
                         b'Scout+ 4')

    def test_distance_string_uses_the_original_format_selector(self):
        image = original_image()
        strings = bytes(image[2]).ljust(65536, b'\0')
        data = (C.c_ubyte * 65536)()
        string_segment = (C.c_ubyte * 65536).from_buffer_copy(strings)
        runtime = Runtime()
        runtime.data = data
        runtime.strings = string_segment
        for font_height, fmt_id in ((14, 0x360), (15, 0x361)):
            data[0x23fa] = font_height
            data[0x1484] = 0xff
            data[0x1485] = 0xff
            self.assertEqual(self.lib.stars_distance_string(
                C.byref(runtime), 0, 0, 300, 400), 0x57a4)
            # The two source formats are decoded by the already independent
            # table image; compare the actual rendered result to that text.
            group = fmt_id >> 6
            lengths = 0x6e32 + group * 64
            total = sum(strings[lengths:lengths + (fmt_id & 63)])
            cursor = int.from_bytes(
                strings[0x73b8 + group * 2:0x73b8 + group * 2 + 2], 'little'
            ) + total // 2
            high = not (total & 1)
            count = strings[lengths + (fmt_id & 63)]
            fmt = bytearray()
            character = 0
            for _ in range(count):
                if high:
                    nibble = strings[cursor] >> 4
                else:
                    nibble = strings[cursor] & 15
                    cursor += 1
                high = not high
                character += nibble
                if nibble != 15:
                    fmt.append(strings[0x73e6 + character])
                    character = 0
            fmt = fmt.decode('ascii')
            expected = (fmt % (5, 0)).encode('ascii') + b'\0'
            self.assertEqual(bytes(data[0x57a4:0x57a4 + len(expected)]),
                             expected)


if __name__ == '__main__':
    unittest.main()
