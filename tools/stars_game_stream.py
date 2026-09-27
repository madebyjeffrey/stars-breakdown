"""Inspect original Stars! game streams through the reconstructed file XOR.

This is a diagnostic for retained local fixtures, not a whole-game parser.
Record framing and BOF fields are checked before decrypting bodies.  The
actual XOR/seed operations come from the independently probed C reconstruction.
"""

import argparse
import ctypes
import hashlib
import json
from pathlib import Path
import struct

from tools.race_file_compare import Inspector, Seeds


def decode_stream(path, inspector):
    raw = Path(path).read_bytes()
    if len(raw) > 16 * 1024 * 1024:
        raise ValueError("stream exceeds diagnostic size limit")
    records = []
    cursor = 0
    turn = None
    seeds = Seeds()
    while cursor < len(raw):
        if cursor + 2 > len(raw):
            raise ValueError("truncated record header at %d" % cursor)
        header, = struct.unpack_from("<H", raw, cursor)
        kind, length = header >> 10, header & 1023
        end = cursor + 2 + length
        if end > len(raw):
            raise ValueError("truncated record body at %d" % cursor)
        body = raw[cursor + 2:end]
        if kind == 8:
            if length != 16:
                raise ValueError("unexpected BOF size at %d" % cursor)
            marker, game, version, turn, salt_player, flags = struct.unpack(
                "<4sI4H", body)
            if marker != b"J3J3" or version != 0x2a60:
                raise ValueError("unexpected BOF identity at %d" % cursor)
            salt, player = salt_player >> 5, salt_player & 31
            inspector.lib.stars_set_file_xor_stream(
                ctypes.byref(seeds), game, salt if salt < 1024 else salt - 2048,
                ctypes.c_int16(turn).value, player if player < 16 else player - 32,
                (flags >> 12) & 1)
        elif turn is None:
            raise ValueError("record before BOF at %d" % cursor)
        elif kind:
            body = inspector.crypt(body, seeds)
        records.append(dict(index=len(records), offset=cursor, kind=kind,
                            length=length, turn=turn, body=body))
        cursor = end
    return records


def encode_stream(records, inspector):
    """Re-encode decoded records, reseeding the XOR at each original BOF."""
    output = bytearray()
    seeds = Seeds()
    have_bof = False
    for record in records:
        kind = record["kind"]
        body = record["body"]
        if kind < 0 or kind > 63 or len(body) > 1023:
            raise ValueError("record kind or length is outside header bounds")
        if kind == 8:
            if len(body) != 16:
                raise ValueError("unexpected BOF size")
            marker, game, version, turn, salt_player, flags = struct.unpack(
                "<4sI4H", body)
            if marker != b"J3J3" or version != 0x2a60:
                raise ValueError("unexpected BOF identity")
            salt, player = salt_player >> 5, salt_player & 31
            inspector.lib.stars_set_file_xor_stream(
                ctypes.byref(seeds), game, salt if salt < 1024 else salt - 2048,
                ctypes.c_int16(turn).value, player if player < 16 else player - 32,
                (flags >> 12) & 1)
            have_bof = True
        elif not have_bof:
            raise ValueError("record before BOF")
        elif kind:
            body = inspector.crypt(body, seeds)
        output += struct.pack("<H", (kind << 10) | len(body))
        output += body
    return bytes(output)


def compare_records(left, right):
    differences = []
    for index, (a, b) in enumerate(zip(left, right)):
        if (a["kind"], a["length"]) == (b["kind"], b["length"]) and \
                a["body"] == b["body"]:
            continue
        offsets = [i for i, (x, y) in enumerate(zip(a["body"], b["body"]))
                   if x != y]
        differences.append(dict(index=index, kind_left=a["kind"],
                                kind_right=b["kind"], length_left=a["length"],
                                length_right=b["length"],
                                differing_bytes=len(offsets) +
                                abs(a["length"] - b["length"]),
                                first_offsets=offsets[:16]))
    return dict(records_left=len(left), records_right=len(right),
                framing_kinds_equal=[r["kind"] for r in left] ==
                [r["kind"] for r in right],
                plaintext_equal=len(left) == len(right) and not differences,
                differences=differences,
                extra_record_count=abs(len(left) - len(right)))


def summarize(path, records):
    kinds = {}
    for record in records:
        key = str(record["kind"])
        kinds[key] = kinds.get(key, 0) + 1
    return dict(file=str(path), sha256=hashlib.sha256(Path(path).read_bytes()).hexdigest(),
                bytes=Path(path).stat().st_size, records=len(records),
                bof_turns=[r["turn"] for r in records if r["kind"] == 8],
                kind_counts=kinds,
                message_records=[dict(index=r["index"], turn=r["turn"],
                                      bytes=r["length"],
                                      first_id=(struct.unpack_from("<H", r["body"])[0]
                                                & 0x1ff) if r["length"] >= 2 else None)
                                 for r in records if r["kind"] == 12])


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("left", type=Path)
    parser.add_argument("right", nargs="?", type=Path)
    parser.add_argument("--library", type=Path, default=Path("build/libstars_core.dylib"))
    args = parser.parse_args()
    inspector = Inspector(ctypes.CDLL(str(args.library.resolve())))
    left = decode_stream(args.left, inspector)
    result = {"left": summarize(args.left, left)}
    if args.right:
        right = decode_stream(args.right, inspector)
        result["right"] = summarize(args.right, right)
        result["comparison"] = compare_records(left, right)
    print(json.dumps(result, indent=2, sort_keys=True))


if __name__ == "__main__":
    main()
