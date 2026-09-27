"""Prepare a bounded, experimental order-bearing copy of clean2400 M1.

This is a candidate input for an isolated original-executable run, not an
accepted original-behavior oracle. Never overwrite the verified source file.
"""

import argparse
import hashlib
from pathlib import Path
import struct

from tools.race_file_compare import Inspector
from tools.stars_game_stream import decode_stream, encode_stream


SOURCE_SHA256 = "95d2d7fd27a39ea1668685beeb4785d876d9ed3cfb657f58173ea47e04e8c0a7"
HOST_SHA256 = "74a27a31221f67328a98200c5dfde54f4c955a6498cc37ac08624b64a33997f6"


def make_candidate(source, inspector):
    source = Path(source)
    if hashlib.sha256(source.read_bytes()).hexdigest() != SOURCE_SHA256:
        raise ValueError("clean2400 M1 input differs from verified source")
    records = decode_stream(source, inspector)
    if encode_stream(records, inspector) != source.read_bytes():
        raise ValueError("unmodified stream does not round-trip byte-exactly")
    matches = [(index, record) for index, record in enumerate(records)
               if record["kind"] == 20 and record["turn"] == 0]
    if len(matches) != 6:
        raise ValueError("expected six compact turn-0 orders")
    index, record = matches[0]
    if record["body"].hex() != "680414050a000011":
        raise ValueError("first compact order differs from clean fixture")
    body = bytearray(record["body"] + bytes(10))
    # Candidate task 5 (scrap), preserving point, target, object kind,
    # valid bit and zeroing the unproven task-specific 10-byte union. The
    # original executable must validate whether this is meaningful.
    struct.pack_into("<H", body, 6, 0x1105)
    records[index] = {**record, "kind": 19, "length": 18, "body": bytes(body)}
    return encode_stream(records, inspector)


def make_host_candidate(source, inspector):
    """Apply the same single-order experiment to the verified host stream."""
    source = Path(source)
    if hashlib.sha256(source.read_bytes()).hexdigest() != HOST_SHA256:
        raise ValueError("clean2400 HST input differs from verified source")
    records = decode_stream(source, inspector)
    if encode_stream(records, inspector) != source.read_bytes():
        raise ValueError("unmodified host stream does not round-trip")
    record = records[45]
    if (record["kind"], record["body"].hex()) != (20, "680414050a000011"):
        raise ValueError("first host compact order differs from clean fixture")
    body = bytearray(record["body"] + bytes(10))
    struct.pack_into("<H", body, 6, 0x1105)
    records[45] = {**record, "kind": 19, "length": 18, "body": bytes(body)}
    return encode_stream(records, inspector)


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("source", type=Path)
    parser.add_argument("output", type=Path)
    args = parser.parse_args()
    if args.source.resolve() == args.output.resolve():
        parser.error("output must be different from the verified source")
    if args.output.exists():
        parser.error("output exists; choose a fresh candidate path")
    output = make_candidate(args.source, Inspector())
    args.output.parent.mkdir(parents=True, exist_ok=True)
    args.output.write_bytes(output)
    print("candidate=%s bytes=%d sha256=%s" % (
        args.output, len(output), hashlib.sha256(output).hexdigest()))


if __name__ == "__main__":
    main()
