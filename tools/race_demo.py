#!/usr/bin/env python3
"""Generate a synthetic race using only the reconstructed host C89 routines.

The original executable supplies static template/string data. No original
instructions, emulator, Windows installation, or user race files are used.
"""
import argparse
import ctypes as C
import hashlib
import json
from pathlib import Path
import struct
import subprocess
import sys

ROOT = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(ROOT))
from tools.ne_layout import NE


class Seeds(C.Structure):
    _fields_ = [('first', C.c_uint32), ('second', C.c_uint32)]


def generate(seed, record_roundtrip=False, tick=0):
    original = ROOT / 'target/stars.exe'
    if hashlib.sha256(original.read_bytes()).hexdigest() != 'ccab6682b55dae14dbf104e491cfbd3b32a6280022020032ae0615500949ba59':
        raise ValueError('Original executable differs from the reviewed baseline')
    build = subprocess.run([sys.executable, str(ROOT / 'tools/build_core.py')],
                           check=True, capture_output=True, text=True)
    lib = C.CDLL(build.stdout.strip())
    lib.stars_randomize.argtypes = [C.POINTER(Seeds), C.c_uint32]
    lib.stars_create_random_race.argtypes = [C.POINTER(C.c_ubyte)] * 2 + [C.c_uint16]
    lib.stars_create_random_race.restype = None
    lib.stars_race_advantage_points.argtypes = [C.POINTER(C.c_ubyte), C.c_uint16]
    lib.stars_race_advantage_points.restype = C.c_int16
    lib.stars_copy_game_string.argtypes = [C.POINTER(C.c_ubyte)] * 2 + [C.c_int16, C.c_uint16]
    lib.stars_copy_game_string.restype = C.c_int16
    ne = NE(original)
    memory = bytearray(bytes(ne.raw[36]).ljust(65536, b'\0'))
    strings = (C.c_ubyte * 65536).from_buffer_copy(bytes(ne.raw[2]).ljust(65536, b'\0'))
    player = 0x3000
    memory[player:player+192] = memory[0xda2:0xe62]
    memory[0x1484] = 255
    state = Seeds()
    lib.stars_randomize(C.byref(state), seed)
    struct.pack_into('<II', memory, 0x1370, state.first, state.second)
    host = (C.c_ubyte * 65536).from_buffer(memory)
    lib.stars_copy_game_string(host, strings, 0x532, player+128)
    lib.stars_create_random_race(host, strings, player)
    score = lib.stars_race_advantage_points(host, player)
    record = memory[player:player+192]
    signed = lambda values: [v if v < 128 else v-256 for v in values]
    result = {
        'scope': 'Synthetic race; reconstructed host code using original static data',
        'seed': seed,
        'name': bytes(record[128:160]).split(b'\0')[0].decode('cp1252'),
        'advantage_points': score,
        'growth': signed(record[25:26])[0],
        'environment': {'centers': signed(record[16:19]), 'low': signed(record[19:22]),
                        'high': signed(record[22:25])},
        'stats_0_through_15': signed(record[62:78]),
        'trait_bits': f'{struct.unpack_from("<I", record, 78)[0]:08x}',
        'rng_after': list(struct.unpack_from('<II', memory, 0x1370)),
    }
    if record_roundtrip:
        result['record_roundtrip'] = roundtrip_records(lib, memory, strings, player, tick)
    return result


def roundtrip_records(lib, memory, strings, player, tick):
    """Serialize and read a controlled BOF/player/checksum sequence in memory."""
    pointer = C.POINTER(C.c_ubyte)
    read_type = C.CFUNCTYPE(None, C.c_void_p, pointer, C.c_uint16)
    write_type = C.CFUNCTYPE(None, C.c_void_p, pointer, C.c_uint16)
    tick_type = C.CFUNCTYPE(C.c_uint32, C.c_void_p)
    lib.stars_write_record.argtypes = [pointer,C.c_int16,C.c_uint16,pointer,write_type,C.c_void_p]
    lib.stars_write_player_record.argtypes = [pointer,C.c_uint16,C.c_uint16,write_type,C.c_void_p]
    lib.stars_read_record.argtypes = [pointer,read_type,C.c_void_p]
    lib.stars_read_player_record.argtypes = [pointer,pointer,C.c_uint16,C.c_uint16]
    lib.stars_race_checksum.argtypes = [pointer,C.c_uint16]
    lib.stars_race_checksum.restype = C.c_uint16
    lib.stars_write_bof.argtypes = [pointer,C.c_int16,C.c_int16,C.c_int16,tick_type,write_type,C.c_void_p]
    for name in ['stars_write_record','stars_write_player_record','stars_read_record','stars_read_player_record','stars_write_bof']:
        getattr(lib,name).restype = None
    host = (C.c_ubyte*65536).from_buffer(memory)
    encoded = bytearray()
    writer = write_type(lambda _,p,n: encoded.extend(C.string_at(p,n)))
    # Controlled environment; the reconstructed header routine consumes the
    # current game RNG before asking for the supplied Windows tick value.
    struct.pack_into('<I',memory,0x70,0x12345678)
    struct.pack_into('<HH',memory,0x80,0,0)
    struct.pack_into('<I',memory,0x7ca,0)
    clock = tick_type(lambda _:tick)
    lib.stars_write_bof(host,-1,5,0,clock,writer,None)
    salt = struct.unpack_from('<H',encoded,14)[0] >> 5
    rng_after_bof = list(struct.unpack_from('<II',memory,0x1370))
    lib.stars_write_player_record(host,player,0,writer,None)
    checksum = lib.stars_race_checksum(host,player)
    expected = bytes(memory[player:player+192])
    struct.pack_into('<H',memory,0x3400,checksum)
    lib.stars_write_record(host,0,2,C.cast(C.byref(host,0x3400),pointer),writer,None)
    cursor = [0]
    def read(_,p,n):
        C.memmove(p,bytes(encoded[cursor[0]:cursor[0]+n]),n)
        cursor[0] += n
    reader = read_type(read)
    record_types = []
    for _ in range(3):
        lib.stars_read_record(host,reader,None)
        kind = struct.unpack_from('<H',memory,0x2696)[0] >> 10
        record_types.append(kind)
        if kind == 8:
            memory[0x750:0x752] = memory[0x4ba0:0x4ba2]
        elif kind == 6:
            lib.stars_read_player_record(host,strings,0x3800,0x4b98)
        elif kind == 0:
            stored_checksum = struct.unpack_from('<H',memory,0x4b98)[0]
    decoded = bytes(memory[0x3800:0x38c0])
    decoded_checksum = lib.stars_race_checksum(host,0x3800)
    assert cursor[0] == len(encoded)
    return {'encoded_bytes':len(encoded),'record_types':record_types,
            'player_bytes_equal':decoded == expected,'checksum':checksum,
            'checksum_matches':stored_checksum == decoded_checksum,
            'changed_player_offsets':[i for i,(a,b) in enumerate(zip(expected,decoded)) if a != b],
            'stream_sha256':hashlib.sha256(encoded).hexdigest(),
            'bof_inputs':{'game_id':0x12345678,'turn':0,'tick':tick},
            'bof_salt':salt,'rng_after_bof':rng_after_bof}



def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--seed', type=lambda value: int(value, 0), default=0)
    parser.add_argument('--record-roundtrip', action='store_true', help='also encode and read a synthetic player-record stream')
    parser.add_argument('--tick', type=lambda value:int(value,0), default=0,
                        help='controlled Windows tick value for --record-roundtrip')
    args = parser.parse_args()
    if not 0 <= args.seed <= 0xffffffff: parser.error('seed must be an unsigned 32-bit integer')
    if not 0 <= args.tick <= 0xffffffff: parser.error('tick must be an unsigned 32-bit integer')
    print(json.dumps(generate(args.seed, args.record_roundtrip, args.tick), indent=2))


if __name__ == '__main__': main()
