"""Re-encrypt a Stars stream with a different BOF salt, preserving records."""

from pathlib import Path
import struct

from tools.race_file_compare import Inspector
from tools.stars_game_stream import decode_stream, encode_stream


def rekey(source, destination, salt):
    if not 0 <= salt < 2048:
        raise ValueError("salt must fit the 11-bit Stars BOF field")
    source, destination = Path(source), Path(destination)
    inspector = Inspector()
    records = decode_stream(source, inspector)
    if not records or records[0]["kind"] != 8:
        raise ValueError("stream does not start with a BOF")
    bof = bytearray(records[0]["body"])
    player = struct.unpack_from("<H", bof, 12)[0] & 31
    struct.pack_into("<H", bof, 12, (salt << 5) | player)
    records[0] = {**records[0], "body": bytes(bof)}
    encoded = encode_stream(records, inspector)
    destination.write_bytes(encoded)
    decoded = decode_stream(destination, inspector)
    if len(decoded) != len(records) or any(
            (left["kind"], left["body"]) != (right["kind"], right["body"])
            for left, right in zip(records, decoded)):
        destination.unlink(missing_ok=True)
        raise ValueError("rekeyed stream failed plaintext round-trip")
    return len(encoded)


if __name__ == "__main__":
    import argparse
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("source", type=Path)
    parser.add_argument("destination", type=Path)
    parser.add_argument("salt", type=int)
    args = parser.parse_args()
    print("bytes=%d salt=%d" % (rekey(args.source, args.destination, args.salt),
                                 args.salt))
