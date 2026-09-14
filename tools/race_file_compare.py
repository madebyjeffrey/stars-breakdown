"""Inspect bounded race fixtures without flattening encoding or checksum quirks.

The reconstructed codec has separate original-instruction regression coverage.
This inspector is a comparison aid, not an independent native acceptance oracle.
"""
import ctypes as C
import functools
import hashlib
import operator
from pathlib import Path
import struct
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT))
from tools.ne_layout import NE

ORIGINAL_SHA256 = 'ccab6682b55dae14dbf104e491cfbd3b32a6280022020032ae0615500949ba59'
PTR = C.POINTER(C.c_ubyte)


class Seeds(C.Structure):
    _fields_ = [('first', C.c_uint32), ('second', C.c_uint32)]


def player_fields(record):
    signed = lambda values: [x if x < 128 else x-256 for x in values]
    flags = struct.unpack_from('<I', record, 78)[0]
    return dict(singular=record[128:160].split(b'\0')[0].decode('cp1252'),
                plural=record[160:192].split(b'\0')[0].decode('cp1252'),
                portrait=(struct.unpack_from('<H', record, 6)[0] >> 3) & 31,
                environment=dict(centers=signed(record[16:19]), low=signed(record[19:22]),
                                 high=signed(record[22:25])), growth=record[25],
                stats=signed(record[62:78]), trait_bits=f'{flags:08x}',
                random_template=bool(flags & (1 << 30)),
                has_password=bool(struct.unpack_from('<I', record, 12)[0]))


class Inspector:
    def __init__(self, lib=None):
        original = ROOT / 'target/stars.exe'
        if hashlib.sha256(original.read_bytes()).hexdigest() != ORIGINAL_SHA256:
            raise ValueError('Original executable differs from the reviewed baseline')
        ne = NE(original)
        self.initial = bytes(ne.raw[36]).ljust(65536, b'\0')
        self.strings = (C.c_ubyte*65536).from_buffer_copy(bytes(ne.raw[2]).ljust(65536, b'\0'))
        if lib is None:
            build = subprocess.run([sys.executable, str(ROOT/'tools/build_core.py')],
                                   check=True, capture_output=True, text=True)
            lib = C.CDLL(build.stdout.strip())
        self.lib = lib
        lib.stars_set_file_xor_stream.argtypes = [C.POINTER(Seeds), C.c_uint32]+[C.c_int16]*4
        lib.stars_set_file_xor_stream.restype = None
        lib.stars_next_file_xor.argtypes = [C.POINTER(Seeds)]
        lib.stars_next_file_xor.restype = C.c_uint32
        lib.stars_read_player_record.argtypes = [PTR, PTR, C.c_uint16, C.c_uint16]
        lib.stars_read_player_record.restype = None
        lib.stars_race_advantage_points.argtypes = [PTR, C.c_uint16]
        lib.stars_race_advantage_points.restype = C.c_int16

    def crypt(self, body, state):
        output = bytearray(body)
        for i in range(0, len(body), 4):
            key = self.lib.stars_next_file_xor(C.byref(state))
            for j in range(min(4, len(body)-i)):
                output[i+j] ^= (key >> (j*8)) & 255
        return bytes(output)

    @staticmethod
    def bounded_player(body):
        if len(body) < 8:
            raise ValueError('Player prefix is truncated')
        at = 8
        if body[6] & 7 == 7:
            if len(body) < 113 or body[112] > 16:
                raise ValueError('Extended player prefix or relation count is invalid')
            at = 113 + body[112]
        for _ in range(2):
            if at >= len(body):
                raise ValueError('Name length is missing')
            length = body[at]
            at += 1
            if length:
                if length > 31 or at+length > len(body):
                    raise ValueError('Compressed name is outside writer bounds')
                at += length
            else:
                end = body.find(b'\0', at)
                if end < 0 or end-at > 31:
                    raise ValueError('Raw name is unterminated or outside writer bounds')
                at = end+1
        if at != len(body):
            raise ValueError('Unexpected bytes after player names')

    def inspect(self, raw):
        if not 24 <= len(raw) <= 2048:
            raise ValueError('Race fixture size is outside supported bounds')
        records, at = [], 0
        state = Seeds()
        while at < len(raw):
            if at+2 > len(raw):
                raise ValueError('Truncated record header')
            header, = struct.unpack_from('<H', raw, at)
            kind, count = header >> 10, header & 1023
            at += 2
            body = raw[at:at+count]
            if len(body) != count:
                raise ValueError('Truncated record body')
            at += count
            if not records:
                if kind != 8 or count != 16:
                    raise ValueError('Expected 16-byte BOF first')
                marker, game, version, turn, salt_player, flags = struct.unpack('<4sI4H', body)
                if marker != b'J3J3' or flags & 255 != 5 or version != 0x2a60:
                    raise ValueError('Expected current-version race BOF')
                salt = salt_player >> 5
                player = salt_player & 31
                self.lib.stars_set_file_xor_stream(C.byref(state), game,
                    salt if salt < 1024 else salt-2048, C.c_int16(turn).value,
                    player if player < 16 else player-32, (flags >> 12) & 1)
                bof = dict(marker=marker.decode('ascii'), game_id=game, version=version,
                           turn=turn, salt=salt, player=player, flags=flags)
            elif kind:
                body = self.crypt(body, state)
            records.append(dict(type=kind, length=count, body=body.hex()))
        if [r['type'] for r in records] != [8, 6, 0] or records[2]['length'] != 2:
            raise ValueError('Expected exactly BOF/player/checksum records')
        body = bytes.fromhex(records[1]['body'])
        self.bounded_player(body)
        data = (C.c_ubyte*65536).from_buffer_copy(self.initial)
        C.memmove(C.byref(data, 0x3500), body, len(body))
        C.memmove(C.byref(data, 0x750), struct.pack('<H', version), 2)
        self.lib.stars_read_player_record(data, self.strings, 0x3000, 0x3500)
        decoded = bytes(data[0x3000:0x30c0])
        computed = functools.reduce(operator.xor, struct.unpack('<96H', decoded), 0)
        stored, = struct.unpack('<H', bytes.fromhex(records[2]['body']))
        return dict(sha256=hashlib.sha256(raw).hexdigest(), size=len(raw), bof=bof,
                    records=records, decoded_player=decoded.hex(), fields=player_fields(decoded),
                    advantage_points=self.lib.stars_race_advantage_points(data, 0x3000),
                    checksum=dict(stored=stored, computed=computed, valid=stored==computed))


def compare(expected, actual):
    """Only BOF salt may differ automatically; other header differences need review."""
    headers = {key: dict(expected=value, actual=actual['bof'][key])
               for key, value in expected['bof'].items() if actual['bof'][key] != value}
    payload_equal = expected['records'][1:] == actual['records'][1:]
    a, b = bytes.fromhex(expected['decoded_player']), bytes.fromhex(actual['decoded_player'])
    offsets = [i for i, (x, y) in enumerate(zip(a, b)) if x != y]
    state_equal = not offsets and expected['checksum'] == actual['checksum']
    status = ('FAIL' if not payload_equal or not state_equal else
              'REVIEW_HEADER' if set(headers)-{'salt'} else 'PASS')
    return dict(status=status, exact_file=expected['sha256']==actual['sha256'],
                plaintext_equal=payload_equal, decoded_player_differences=offsets,
                bof_differences=headers, checksum=actual['checksum'])
