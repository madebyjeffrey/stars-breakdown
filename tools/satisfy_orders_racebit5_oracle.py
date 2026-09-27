"""Decode the isolated original task-5 race-bit-5 resource outcome."""

import hashlib
import json
from pathlib import Path
import struct

from tools.race_file_compare import Inspector
from tools.stars_game_stream import decode_stream
from tools.satisfy_orders_planet10_oracle import decode_planet10
from tools.satisfy_orders_original_oracle import decode_owner_fleet


ROOT = Path(__file__).resolve().parents[1]
SOURCES = {
    "source_hst": ("runs/vmctl-syact3/BACKUP/CLEAN.HST",
                   "aa6161bb6c95c8276b7a5d12e5a12d9999e3dea0fa242a3be89a94a05c45df3c"),
    "racebit_hst": ("runs/syorig-racebit5-bundle-1/CLEAN.HST",
                    "22e08ebd6db8ccf2c163e84a6637cf507a79cf4fa78fd103cbbeb9b5a72d4c4b"),
    "clean_m1": ("runs/vmctl-syorig1/CLEAN.M1",
                 "96e388d36aa02b6e6640b97e3f0e18e81ffb8b052a244807723c1b516e48bb80"),
    "racebit_m1": ("runs/vmctl-syrb5/CLEAN.M1",
                   "030f2d47dbf4c4d4ff4dd6385423fa0c80ca96ac6a20c1f9e83a740aa0bd7230"),
}


def recover():
    inspector = Inspector()
    records = {}
    for label, (relative, expected) in SOURCES.items():
        path = ROOT / relative
        if hashlib.sha256(path.read_bytes()).hexdigest() != expected:
            raise ValueError("source hash changed: " + relative)
        records[label] = decode_stream(path, inspector)
    source = records["source_hst"]
    candidate = records["racebit_hst"]
    if len(source) != len(candidate):
        raise ValueError("candidate changed HST record count")
    for index, (before, after) in enumerate(zip(source, candidate)):
        if index == 1:
            original = before["body"]
            edited = after["body"]
            if (before["kind"] != 6 or after["kind"] != 6 or
                    original[:78] != edited[:78] or
                    original[82:] != edited[82:]):
                raise ValueError("player record changed outside race bits")
            if (struct.unpack_from("<I", original, 78)[0],
                    struct.unpack_from("<I", edited, 78)[0]) != (0, 0x20):
                raise ValueError("candidate race-bit change is unexpected")
        elif (before["kind"], before["body"]) != (
                after["kind"], after["body"]):
            raise ValueError("candidate changed HST record " + str(index))
    outputs = {}
    for label in ("clean_m1", "racebit_m1"):
        rows = records[label]
        planets = [decode_planet10(row["body"]) for row in rows
                   if row["turn"] == 1 and row["kind"] == 13
                   and row["body"][:2] == b"\x0a\x00"]
        if len(planets) != 1:
            raise ValueError("planet 10 output is not unique")
        messages = [row["body"].hex() for row in rows
                    if row["turn"] == 1 and row["kind"] == 12]
        fleets = [decode_owner_fleet(row["body"])["id"] for row in rows
                  if row["turn"] == 1 and row["kind"] == 16
                  and int.from_bytes(row["body"][:2], "little") < 6]
        outputs[label] = dict(planet10=planets[0], messages=messages,
                              owner0_fleet_ids=fleets)
    clean = outputs["clean_m1"]
    action = outputs["racebit_m1"]
    mineral_delta = [a - b for a, b in zip(
        action["planet10"]["minerals"], clean["planet10"]["minerals"])]
    if mineral_delta != [10, 7, 6, 0]:
        raise ValueError("resource branch mineral return changed")
    if action["owner0_fleet_ids"] != [1, 2, 3, 4, 5]:
        raise ValueError("task-5 fleet was not retired")
    if action["messages"] != [
            "5d000a000017000a0e0043010a000017000a0e001701ffff3f000a000a"]:
        raise ValueError("resource message sequence changed")
    return dict(sources={label: dict(path=relative, sha256=digest)
                         for label, (relative, digest) in SOURCES.items()},
                candidate_race_bits=0x20, outputs=outputs,
                resource_scrap_minus_clean_minerals=mineral_delta,
                resource_message_ids=[0x5d, 0x143, 0x3f])


if __name__ == "__main__":
    print(json.dumps(recover(), indent=2))
