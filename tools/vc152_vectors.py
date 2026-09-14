#!/usr/bin/env python3
"""Capture small deterministic Win16 probe vectors from the original x86 oracle."""
import argparse
import json
from pathlib import Path
import struct

ROOT = Path(__file__).resolve().parents[1]
OUTPUT = ROOT / 'compat/vc152/vectors.json'
ORIGINAL_SHA256 = 'ccab6682b55dae14dbf104e491cfbd3b32a6280022020032ae0615500949ba59'


def capture():
    # Deferred: ordinary packaging must not require the emulator or its JIT.
    from tools.core_machine import OriginalCore
    machine = OriginalCore()
    rows = []

    def add(kind, address, args, **expected):
        rows.append(dict(kind=kind, original=f'NE 9:{address:04X}',
                         args=list(args), **expected))

    seeds = [0, 1, 63, 64, 65, 127, 128, 4095, 0x2e35, 16383, 0xffffffff]
    for kind, address in [('randomize', 0x15ea), ('randomize2', 0x165a)]:
        for seed in seeds:
            machine.call(address, struct.pack('<I', seed))
            add(kind, address, [seed], state=list(machine.seeds()))
    pairs = [(0, 0), (1, 1), (17, 41), (53668, 52774),
             (2147483562, 2147483398), (0x7fffffff, 0x7fffffff),
             (0x80000000, 0xffffffff), (0xffffffff, 0x80000000),
             (0x12345678, 0x9abcdef0)]
    for first, second in pairs:
        for count in [-32768, -1, 0, 17, 32767]:
            machine.seeds(value=(first, second))
            value = machine.call(0x16d2, struct.pack('<h', count)) & 0xffff
            add('random', 0x16d2, [first, second, count],
                result=value, state=list(machine.seeds()))
        machine.seeds(file=True, value=(first, second))
        value = machine.call(0x1b54)
        add('next_file_xor', 0x1b54, [first, second],
            result=value, state=list(machine.seeds(file=True)))
    streams = [(0, 0, 0, 0, 0), (3, 1024, 3, 3, 0),
               (0xdeadbeef, 2047, -1, -1, 0), (0xffffffff, -1, -32768, 32767, 0),
               *[(3, 1024, 3, 3, extra) for extra in [-32768, -65, -64, -63, 32767]]]
    for args in streams:
        machine.call(0x1aa6, struct.pack('<Ihhhh', *args))
        add('set_file_xor_stream', 0x1aa6, args, state=list(machine.seeds(file=True)))
    for pair in [(0, 0), (0xffffffff, 0x80000000), (17, 41)]:
        machine.call(0x1a7c, struct.pack('<II', *pair))
        state = list(machine.seeds(file=True))
        machine.call(0x1a4e, struct.pack('<HH', 0x2800, 0x2804))
        outputs = list(struct.unpack('<II', machine.get(0x2800, 8)))
        machine.call(0x1a4e, struct.pack('<HH', 0x2800, 0x2800))
        alias = struct.unpack('<I', machine.get(0x2800, 4))[0]
        add('file_seeds', 0x1a7c, pair, state=state, outputs=outputs, alias=alias,
            getter_original='NE 9:1A4E')
    distances = [(0, 0, 0, 0), (0, 0, 3, 4), (-1, -1, 1, 1),
                 (-32768, -32768, 0, 0), (32767, 32767, -32768, -32768),
                 (-32768, 32767, 32767, -32768), (30000, -30000, -30000, 30000),
                 (32767, 0, -1, -32768)]
    for args in distances:
        add('distance_squared', 0x685c, args,
            result=machine.call(0x685c, struct.pack('<hhhh', *args)))
    for pair in [(0, 0), (0, 1), (1, 0), (65536, 0), (32768, 0),
                 (32767, 0), (0xffffffff, 0), (0x80000000, 0x7fffffff),
                 (0x7fffffff, 0x80000000)]:
        machine.put(0x2800, struct.pack('<II', *pair))
        value = machine.call(0x1d74, struct.pack('<HH', 0x2800, 0x2804)) & 0xffff
        add('compare_long', 0x1d74, pair, result=value)
    initial = (0, 32768, 65535, 32767)
    for kind, address in [('expand_rect', 0x2f0c), ('offset_rect', 0x2f3e)]:
        for dx, dy in [(0, 0), (1, -1), (-32768, 32767), (32767, -32768)]:
            machine.put(0x2800, struct.pack('<HHHH', *initial))
            machine.call(address, struct.pack('<Hhh', 0x2800, dx, dy))
            add(kind, address, [*initial, dx, dy],
                rect=list(struct.unpack('<HHHH', machine.get(0x2800, 8))))
    return dict(schema=1, original_sha256=ORIGINAL_SHA256,
                method='OriginalCore executes relocated original x86 instructions; no host C expected values.',
                cases=rows)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--check', action='store_true', help='Compare without overwriting retained evidence')
    args = parser.parse_args()
    result = capture()
    if args.check:
        if json.loads(OUTPUT.read_text()) != result:
            raise SystemExit('Retained probe vectors differ from the original-machine results')
        print(f"Original-machine vectors match: {len(result['cases'])} cases")
    else:
        OUTPUT.write_text(json.dumps(result, indent=2) + '\n')
        print(f"Captured {len(result['cases'])} cases in {OUTPUT}")


if __name__ == '__main__':
    main()
