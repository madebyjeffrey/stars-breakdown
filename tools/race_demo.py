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


def generate(seed):
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
    return {
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


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--seed', type=lambda value: int(value, 0), default=0)
    args = parser.parse_args()
    if not 0 <= args.seed <= 0xffffffff: parser.error('seed must be an unsigned 32-bit integer')
    print(json.dumps(generate(args.seed), indent=2))


if __name__ == '__main__': main()
