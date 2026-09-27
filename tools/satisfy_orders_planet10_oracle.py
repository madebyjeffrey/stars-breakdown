"""Decode planet 10's retained original input and two turn-1 outputs.

This is a bounded decoder for the original type-13 planet record used by the
SatisfyOrders task-5 experiment. Width selectors follow FReadPlanet's packed
record rules; all source file hashes are pinned before the comparison.
"""

import hashlib
import json
from pathlib import Path

from tools.race_file_compare import Inspector
from tools.stars_game_stream import decode_stream


ROOT = Path(__file__).resolve().parents[1]
SOURCES = {
    "clean_hst": (
        "runs/vmctl-syorig1/BACKUP/CLEAN.HST",
        "74a27a31221f67328a98200c5dfde54f4c955a6498cc37ac08624b64a33997f6",
    ),
    "action_hst": (
        "runs/vmctl-syact3/BACKUP/CLEAN.HST",
        "aa6161bb6c95c8276b7a5d12e5a12d9999e3dea0fa242a3be89a94a05c45df3c",
    ),
    "m1_only_hst": (
        "runs/vmctl-syact1/BACKUP/CLEAN.HST",
        "74a27a31221f67328a98200c5dfde54f4c955a6498cc37ac08624b64a33997f6",
    ),
    "clean_m1": (
        "runs/vmctl-syorig1/CLEAN.M1",
        "96e388d36aa02b6e6640b97e3f0e18e81ffb8b052a244807723c1b516e48bb80",
    ),
    "action_m1": (
        "runs/vmctl-syact3/CLEAN.M1",
        "c1ff47096624165fd3a94c1b39c617f832a05a92ee60d9b5bff262529d321663",
    ),
    "m1_only_m1": (
        "runs/vmctl-syact1/CLEAN.M1",
        "f85ef1df5b69998995a7ca7d405559b6ec69fbed54f3d18d36838bebf7f9dbf2",
    ),
}


def decode_planet_record(body, expected_id=None):
    if len(body) < 20:
        raise ValueError("planet record is truncated")
    planet_id = int.from_bytes(body[:2], "little") & 0x7ff
    if expected_id is not None and planet_id != expected_id:
        raise ValueError("unexpected planet id")
    flags = int.from_bytes(body[2:4], "little")
    if (flags & 0x7F) != 7 or flags & 0x2000 == 0:
        raise ValueError("unexpected planet detail or mineral flag")
    cursor = 4
    level_selector = body[cursor]
    cursor += 1
    levels = []
    for index in range(3):
        code = (level_selector >> (index * 2)) & 3
        if code == 0:
            levels.append(0)
        elif code == 1:
            levels.append(body[cursor])
            cursor += 1
        else:
            raise ValueError("unsupported mineral-level selector")
    concentrations = list(body[cursor:cursor + 3])
    cursor += 3
    environment = list(body[cursor:cursor + 3])
    cursor += 3
    if flags & 0x0400:
        cursor += 3
    owner = (int.from_bytes(body[:2], "little") >> 11) & 31
    if owner != 31:
        auxiliary_word = int.from_bytes(body[cursor:cursor + 2], "little")
        cursor += 2
    else:
        auxiliary_word = None
    selector = body[cursor]
    cursor += 1
    minerals = []
    for index in range(4):
        width = (0, 1, 2, 4)[(selector >> (index * 2)) & 3]
        if cursor + width > len(body):
            raise ValueError("truncated mineral field")
        minerals.append(int.from_bytes(body[cursor:cursor + width], "little")
                        if width else 0)
        cursor += width
    improvements = None
    if flags & 0x0800:
        if cursor + 8 > len(body):
            raise ValueError("truncated planet improvements")
        packed = int.from_bytes(body[cursor:cursor + 8], "little")
        improvements = dict(population_delta=packed & 0xFF,
                            mines=(packed >> 8) & 0xFFF,
                            factories=(packed >> 20) & 0xFFF,
                            defenses=(packed >> 32) & 0xFFF)
        cursor += 8
    if flags & 0x0200:
        cursor += 4
    if flags & 0x4000:
        cursor += 2
    if cursor != len(body):
        raise ValueError("planet record has an unparsed tail")
    return dict(owner=owner, flags=flags, levels=levels,
                concentrations=concentrations, environment=environment,
                auxiliary_word=auxiliary_word, minerals=minerals,
                improvements=improvements,
                mineral_selector=selector, body_hex=body.hex())


def decode_planet10(body):
    """Decode the retained owner-0 planet 10 record."""
    return decode_planet_record(body, expected_id=10)


def recover():
    inspector = Inspector()
    planet_records = {}
    paths = {}
    for label, (relative_path, expected_hash) in SOURCES.items():
        path = ROOT / relative_path
        actual_hash = hashlib.sha256(path.read_bytes()).hexdigest()
        if actual_hash != expected_hash:
            raise ValueError("source hash changed: " + relative_path)
        paths[label] = dict(path=relative_path, sha256=expected_hash)
        turn = 0 if label.endswith("hst") else 1
        matches = [record["body"] for record in decode_stream(path, inspector)
                   if record["kind"] == 13 and record["turn"] == turn
                   and record["body"][:2] == b"\x0a\x00"]
        if len(matches) != 1:
            raise ValueError("planet 10 record is not unique: " + label)
        planet_records[label] = matches[0]
    player_records = [record["body"] for record in
                      decode_stream(ROOT / SOURCES["action_hst"][0], inspector)
                      if record["kind"] == 6 and record["turn"] == 0
                      and record["body"][0] == 0]
    if len(player_records) != 1 or len(player_records[0]) < 0x4E:
        raise ValueError("owner-0 full player record is missing")
    owner = player_records[0]
    mine_efficiency = owner[0x3E + 4]  # PLAYER.rgAttr[rsMineProd]
    mine_operating_limit = owner[0x3E + 6]  # rsMineOperate
    if (planet_records["clean_hst"] != planet_records["action_hst"] or
            planet_records["clean_hst"] != planet_records["m1_only_hst"]):
        raise ValueError("planet 10 inputs differ before task-5 order")
    original_input = decode_planet10(planet_records["clean_hst"])
    clean_output = decode_planet10(planet_records["clean_m1"])
    action_output = decode_planet10(planet_records["action_m1"])
    m1_only_output = decode_planet10(planet_records["m1_only_m1"])
    baseline_delta = [end - start for end, start in zip(
        clean_output["minerals"], original_input["minerals"])]
    action_delta = [end - start for end, start in zip(
        action_output["minerals"], original_input["minerals"])]
    action_minus_clean = [end - start for end, start in zip(
        action_output["minerals"], clean_output["minerals"])]
    isolated_scrap = [9, 6, 5, 0]
    residual = [observed - expected for observed, expected in zip(
        action_minus_clean, isolated_scrap)]
    mines = original_input["improvements"]["mines"]
    max_operating_from_population = (original_input["minerals"][3] *
                                     mine_operating_limit) // 100
    operating_mines = min(mines, max_operating_from_population)
    mining_hundredths = [operating_mines * concentration * mine_efficiency // 10
                         for concentration in original_input["concentrations"]]
    clean_mining_candidate = baseline_delta[:3]
    action_mining_candidate = [value - scrap for value, scrap in
                               zip(action_delta[:3], isolated_scrap[:3])]
    return dict(sources=paths, original_input=original_input,
                clean_output=clean_output, action_output=action_output,
                m1_only_output=m1_only_output,
                m1_only_minus_clean=[end - start for end, start in zip(
                    m1_only_output["minerals"], clean_output["minerals"])],
                clean_minus_input=baseline_delta,
                action_minus_input=action_delta,
                action_minus_clean=action_minus_clean,
                isolated_scrap_return=isolated_scrap,
                remaining_full_turn_delta=residual,
                mining_rounding_candidate=dict(
                    owner_mine_efficiency=mine_efficiency,
                    owner_mine_operating_limit=mine_operating_limit,
                    operating_mines=operating_mines,
                    raw_hundredths=mining_hundredths,
                    floor=[value // 100 for value in mining_hundredths],
                    remainder=[value % 100 for value in mining_hundredths],
                    clean_turn_gain=clean_mining_candidate,
                    action_turn_gain_after_scrap=action_mining_candidate))


if __name__ == "__main__":
    print(json.dumps(recover(), indent=2))
