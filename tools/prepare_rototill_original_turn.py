"""Stage a pinned, isolated selector-3 Rototill original-turn input.

Only owner-1's personality bits in the HST and M2 player records change.
The original executable is not run by this script.
"""

import argparse
import ctypes
import hashlib
import json
from pathlib import Path
import struct

from tools.race_file_compare import Inspector
from tools.stars_game_stream import decode_stream, encode_stream


ROOT = Path(__file__).resolve().parents[1]
SOURCE = ROOT / "runs/vmctl-sye3k0/BACKUP"
PINNED = {
    "CLEAN.HST": "74a27a31221f67328a98200c5dfde54f4c955a6498cc37ac08624b64a33997f6",
    "CLEAN.M1": "95d2d7fd27a39ea1668685beeb4785d876d9ed3cfb657f58173ea47e04e8c0a7",
    "CLEAN.M2": "588830ac2225056422fa447cb201c7bd326e472652a8d3abf222b5fcceb99d4b",
    "CLEAN.XY": "580e4c3be92eee570c95c97faa2557e3b4fe92856387970cf548700e1bb3fd88",
    "STARS.EXE": "ccab6682b55dae14dbf104e491cfbd3b32a6280022020032ae0615500949ba59",
}
SOURCE_PATHS = {
    "CLEAN.HST": SOURCE / "CLEAN.HST",
    "CLEAN.M1": SOURCE / "CLEAN.M1",
    "CLEAN.M2": SOURCE / "CLEAN.M2",
    "CLEAN.XY": ROOT / "runs/vmctl-sye3k0/CLEAN.XY",
    "STARS.EXE": ROOT / "target/stars.exe",
}


def sha256(data):
    return hashlib.sha256(data).hexdigest()


def change_personality(source_path, inspector, filename):
    source_records = decode_stream(source_path, inspector)
    if encode_stream(source_records, inspector) != source_path.read_bytes():
        raise ValueError(filename + " does not round-trip")
    matching = [record for record in source_records
                if record["kind"] == 6 and record["turn"] == 0 and
                len(record["body"]) >= 8 and record["body"][0] == 1]
    if len(matching) != 1:
        raise ValueError(filename + " has no unique owner-1 player record")
    record = matching[0]
    body = bytearray(record["body"])
    before, = struct.unpack_from("<H", body, 6)
    if before != 0x2797 or (before >> 13) != 1:
        raise ValueError(filename + " has unexpected Turin personality")
    after = (before & 0x1fff) | (3 << 13)
    struct.pack_into("<H", body, 6, after)
    record["body"] = bytes(body)
    encoded = encode_stream(source_records, inspector)
    decoded = decode_stream_bytes(encoded, inspector)
    original = decode_stream(source_path, inspector)
    changed = [index for index, (old, new) in enumerate(zip(original, decoded))
               if old["body"] != new["body"]]
    if (changed != [record["index"]] or
            len(decoded) != len(source_records)):
        raise ValueError(filename + " changed another record")
    original_body = original[record["index"]]["body"]
    candidate_body = decoded[record["index"]]["body"]
    if [offset for offset, (old, new) in enumerate(zip(
            original_body, candidate_body)) if old != new] != [7]:
        raise ValueError(filename + " changed bytes beyond selector")
    return encoded, before, after


def decode_stream_bytes(data, inspector):
    """Decode candidate bytes without modifying a pinned input."""
    from tempfile import TemporaryDirectory
    with TemporaryDirectory() as directory:
        path = Path(directory) / "candidate"
        path.write_bytes(data)
        return decode_stream(path, inspector)


def prepare(destination, library=None):
    destination = Path(destination)
    if destination.exists():
        raise ValueError("destination exists; use a fresh directory")
    if library is None:
        library = ROOT / "build/libstars_core.dylib"
    inspector = Inspector(ctypes.CDLL(str(Path(library).resolve())))
    contents = {}
    changes = {}
    for name, path in SOURCE_PATHS.items():
        data = path.read_bytes()
        if sha256(data) != PINNED[name]:
            raise ValueError("pinned source changed: " + str(path))
        if name in ("CLEAN.HST", "CLEAN.M2"):
            data, before, after = change_personality(path, inspector, name)
            changes[name] = {"record_owner": 1, "mode_before": before,
                             "mode_after": after, "selector_before": 1,
                             "selector_after": 3}
        contents[name] = data
    manifest = {
        "status": "input_only_not_original_behavior_evidence",
        "parent": "runs/vmctl-sye3k0/BACKUP",
        "changes": changes,
        "pre_run_absent": ["CLEAN.X2", "CLEAN.H2"],
        "files": {name: {"bytes": len(data), "sha256": sha256(data),
                         "source_sha256": PINNED[name]}
                  for name, data in sorted(contents.items())},
    }
    destination.mkdir(parents=True)
    for name, data in contents.items():
        (destination / name).write_bytes(data)
    (destination / "MANIFEST.JSON").write_text(
        json.dumps(manifest, indent=2, sort_keys=True) + "\n",
        encoding="ascii")
    return manifest


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("destination", type=Path)
    parser.add_argument("--library", type=Path)
    args = parser.parse_args()
    result = prepare(args.destination, args.library)
    print("staged %d verified files in %s" %
          (len(result["files"]), args.destination))


if __name__ == "__main__":
    main()
