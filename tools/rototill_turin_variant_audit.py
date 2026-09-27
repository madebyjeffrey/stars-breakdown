"""Identify the reusable part of the retained Turin and Rototill turns."""

from difflib import SequenceMatcher
import hashlib
import json
from pathlib import Path

from tools.race_file_compare import Inspector
from tools.stars_game_stream import decode_stream

ROOT = Path(__file__).resolve().parents[1]
TURIN = ROOT / "runs/vmctl-sye3k0"
ROTOTILL = ROOT / "runs/rototill-original-turn-r1"
REPORT = ROOT / "analysis/compat/do-rototill-turin-variant-r1.json"


def _digest(path):
    return hashlib.sha256(path.read_bytes()).hexdigest()


def _stream(path, inspector, turn):
    return [row for row in decode_stream(path, inspector)
            if row["turn"] == turn]


def audit():
    inspector = Inspector()
    inputs = {}
    outputs = {}
    for suffix in ("HST", "M1", "M2", "XY"):
        turin_input = TURIN / ("CLEAN.XY" if suffix == "XY" else
                               "BACKUP/CLEAN." + suffix)
        rototill_input = ROTOTILL / "input" / ("CLEAN." + suffix)
        item = {"turin_sha256": _digest(turin_input),
                "rototill_sha256": _digest(rototill_input),
                "bytes_identical": turin_input.read_bytes() ==
                                   rototill_input.read_bytes()}
        if suffix != "XY":
            left = _stream(turin_input, inspector, 0)
            right = _stream(rototill_input, inspector, 0)
            if len(left) != len(right):
                raise ValueError("input record count differs")
            differences = []
            for index, (a, b) in enumerate(zip(left, right)):
                if a["kind"] != b["kind"] or len(a["body"]) != len(b["body"]):
                    raise ValueError("input record shape differs")
                byte_offsets = [offset for offset, (old, new) in
                                enumerate(zip(a["body"], b["body"]))
                                if old != new]
                if byte_offsets:
                    differences.append({
                        "record_index": index, "kind": a["kind"],
                        "byte_offsets": byte_offsets,
                        "before_hex": bytes(a["body"][offset]
                                            for offset in byte_offsets).hex(),
                        "after_hex": bytes(b["body"][offset]
                                           for offset in byte_offsets).hex(),
                    })
            item["record_count"] = len(left)
            item["decoded_differences"] = differences
        inputs[suffix] = item

    for suffix in ("HST", "M1", "M2"):
        turin_output = TURIN / ("CLEAN." + suffix)
        rototill_output = ROTOTILL / "output" / ("CLEAN." + suffix)
        left = _stream(turin_output, inspector, 1)
        right = _stream(rototill_output, inspector, 1)
        matcher = SequenceMatcher(None,
                                  [(row["kind"], row["body"]) for row in left],
                                  [(row["kind"], row["body"]) for row in right],
                                  autojunk=False)
        operations = []
        equal_count = 0
        for action, a0, a1, b0, b1 in matcher.get_opcodes():
            if action == "equal":
                equal_count += a1 - a0
            else:
                operations.append({"action": action,
                                   "turin_range": [a0, a1],
                                   "rototill_range": [b0, b1],
                                   "turin_kinds": [row["kind"] for row in
                                                   left[a0:a1]],
                                   "rototill_kinds": [row["kind"] for row in
                                                      right[b0:b1]]})
        outputs[suffix] = {
            "turin_sha256": _digest(turin_output),
            "rototill_sha256": _digest(rototill_output),
            "turin_turn_1_records": len(left),
            "rototill_turn_1_records": len(right),
            "identical_records_in_order": equal_count,
            "differences": operations,
        }
    return {"schema": "rototill-turin-variant-r1-v1",
            "input": inputs, "output": outputs}


if __name__ == "__main__":
    REPORT.write_text(json.dumps(audit(), indent=2) + "\n",
                      encoding="utf-8")
