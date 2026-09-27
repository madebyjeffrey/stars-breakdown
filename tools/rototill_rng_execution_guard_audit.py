#!/usr/bin/env python3
"""Check fixture guards that cut original Rototill indirect RNG paths."""

import hashlib
import json
from collections import Counter, deque
from pathlib import Path

from tools.race_file_compare import Inspector
from tools.rototill_rng_indirect_resolution import resolve
from tools.stars_game_stream import decode_stream


ROOT = Path(__file__).resolve().parents[1]
BASE = ROOT / "analysis/decompiler"
RUN = ROOT / "runs/rototill-original-turn-r1"
OUTPUT = ROOT / "analysis/compat/do-rototill-rng-execution-guards-r1.json"


def raw(name):
    matches = list((BASE / "export/raw").glob("*_" + name + ".c"))
    if len(matches) != 1:
        raise ValueError("expected one original raw procedure: " + name)
    return matches[0].read_text()


def reachable(graph, cut):
    outgoing = {}
    for row in graph:
        outgoing.setdefault(row["caller_name"], set()).update(
            row["target_names"])
    seen = {"FGenerateTurn"}
    queue = deque(["FGenerateTurn"])
    while queue:
        caller = queue.popleft()
        for target in sorted(outgoing.get(caller, ())):
            if (caller, target) not in cut and target not in seen:
                seen.add(target)
                queue.append(target)
    return seen


def audit():
    immediate = resolve()
    if len(immediate["indirect_sites"]) != 8:
        raise AssertionError("indirect inventory changed")
    inspector = Inspector()
    x2_path = RUN / "backup/CLEAN.X2"
    x2 = decode_stream(x2_path, inspector)
    kinds = Counter(row["kind"] for row in x2)
    if kinds != Counter({27: 4, 8: 1, 9: 1, 4: 1, 1: 1,
                         29: 1, 0: 1}):
        raise AssertionError("original X2 record kinds changed")
    design_low_nibbles = [row["body"][0] & 15 for row in x2
                          if row["kind"] == 27]
    if design_low_nibbles != [1, 1, 1, 1]:
        raise AssertionError("original design-add guard changed")
    guards = json.loads((ROOT / "analysis/compat/do-rototill-rng-guards-r1.json").read_text())
    orders = guards["input_fleet_owner_id_count_kind_warp_task"]
    output_orders = guards["output_fleet_owner_id_count_kind_warp_task"]
    if len(orders) != 10 or any(row[2] != 1 or row[3] != 1 or row[5] != 0
                                for row in orders + output_orders):
        raise AssertionError("fleet order task/object guard changed")
    if "case 0x17:" not in raw("FRunLogRecord") or \
            "case 0x25:" not in raw("FRunLogRecord") or \
            "case 0x1b:" not in raw("FRunLogRecord") or \
            "if ((*(uint *)lpb & 0xf) == 0)" not in raw("FRunLogRecord"):
        raise AssertionError("log-record fleet/design guards changed")
    if "(uStack_78 & 0xf) == 4" not in raw("SatisfyOrders") or \
            "(uStack_78 >> 8 & 0xf) == 2" not in raw("SatisfyOrders"):
        raise AssertionError("fleet-merge order guard changed")
    for name in ("FLookupFleet", "FLookupPlanet"):
        if "InvalidateReport(1,0)" not in raw(name) and \
                "InvalidateReport(0,0)" not in raw(name):
            raise AssertionError(name + " report-invalidation arguments changed")
    report = raw("InvalidateReport")
    if "if (fReload != 0)" not in report or \
            "SortReportCache(vprptCur->irpt,vprptCur->icolSort)" not in report:
        raise AssertionError("report sorting guard changed")
    if "0xf7ff | 0x800" not in raw("StartTutor") or \
            "if (((uint)gd.overlap_0.grBits_view.grBits._0_2_ >> 0xb & 1) != 0)" \
            not in raw("EndTutor"):
        raise AssertionError("tutorial bit lifecycle changed")
    frame = raw("FrameWndProc")
    if "EnsureAis();\n          FGenerateTurn();" not in frame or \
            "POSTQUITMESSAGE(vretExitValue)" not in frame:
        raise AssertionError("batch generation context changed")
    run = json.loads((RUN / "RUN.JSON").read_text())
    if run["command"] != "C:\\RTORIG1\\STARS.EXE -g C:\\RTORIG1\\CLEAN.HST":
        raise AssertionError("original command context changed")

    graph = json.loads((BASE / "callgraph.json").read_text())
    comparator_edges = {target for row in graph
                        if row["caller_name"] == "ICompFleetPoint2"
                        for target in row["target_names"]}
    if comparator_edges:
        raise AssertionError("bsearch comparator gained descendants")
    cut = {
        ("FRunLogRecord", "FDeleteFleet"),
        ("FRunLogRecord", "DestroyAllIshdef"),
        ("FRunLogRecord", "FFleetMergeAll"),
        ("SatisfyOrders", "Merge2Fleets"),
        ("EndTutor", "RestoreGameState"),
        ("EndTutor", "Randomize2"),
        ("InvalidateReport", "SortReportCache"),
    }
    seen = reachable(graph, cut)
    for name in ("DrawPlanShip", "SortReportCache", "Randomize2",
                 "StartTutor"):
        if name in seen:
            raise AssertionError("guarded indirect path remains: " + name)
    if "Random" not in seen or "_bsearch" not in seen:
        raise AssertionError("audit unexpectedly removed ordinary generation")
    return {
        "schema": "rototill-rng-execution-guards-r1",
        "original_x2_sha256": hashlib.sha256(x2_path.read_bytes()).hexdigest(),
        "original_run_command": run["command"],
        "x2_record_kinds": dict(sorted(kinds.items())),
        "four_design_low_nibbles": design_low_nibbles,
        "retained_fleet_order_count": len(orders),
        "cut_edges": [{"caller": a, "target": b} for a, b in sorted(cut)],
        "reachable_after_guard_count": len(seen),
        "draw_plan_ship_reachable_after_guards": False,
        "sort_report_cache_reachable_after_guards": False,
        "randomize2_reachable_after_guards": False,
        "start_tutor_directly_reachable": False,
        "scope_note": "The cut edges follow retained X2 design/addition "
                      "and fleet-order guards and original raw branches. "
                      "This is a retained-turn call-context proof, not a "
                      "general claim about other UI or game states.",
    }


def main():
    result = audit()
    OUTPUT.write_text(json.dumps(result, indent=2, sort_keys=True) + "\n")
    print("%d guarded edges; DrawPlanShip, SortReportCache, Randomize2 "
          "unreachable" % len(result["cut_edges"]))


if __name__ == "__main__":
    main()
