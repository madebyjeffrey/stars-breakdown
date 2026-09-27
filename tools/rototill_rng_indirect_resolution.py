#!/usr/bin/env python3
"""Resolve the eight remaining original indirect sites in the Rototill RNG audit."""

import hashlib
import json
import struct
from collections import deque
from pathlib import Path

from tools.ne_layout import NE


ROOT = Path(__file__).resolve().parents[1]
BASE = ROOT / "analysis/decompiler"
OUTPUT = ROOT / "analysis/compat/do-rototill-rng-indirect-resolution-r1.json"
EXE = ROOT / "target/stars.exe"


def source(name):
    matches = list((BASE / "export/raw").glob("*_" + name + ".c"))
    if len(matches) != 1:
        raise ValueError("expected one raw listing for " + name)
    return matches[0].read_text()


def shortest_path(graph, target):
    links = {}
    for row in graph:
        links.setdefault(row["caller_name"], set()).update(row["target_names"])
    seen = {"FGenerateTurn": None}
    queue = deque(["FGenerateTurn"])
    while queue:
        caller = queue.popleft()
        for child in sorted(links.get(caller, ())):
            if child not in seen:
                seen[child] = caller
                queue.append(child)
    if target not in seen:
        return None
    path = []
    while target is not None:
        path.append(target)
        target = seen[target]
    return list(reversed(path))


def resolve():
    inventory = json.loads((BASE.parent / "compat/do-rototill-rng-indirect-r1.json").read_text())
    graph_path = BASE / "callgraph.json"
    graph = json.loads(graph_path.read_text())
    functions_path = BASE / "functions.json"
    functions = json.loads(functions_path.read_text())
    names = {row["address"]: row["name"] for row in functions}
    random_address = inventory["random_address"]
    if random_address != "1040:16d2":
        raise AssertionError("original Random address changed")
    if shortest_path(graph, "StartTutor") is not None:
        raise AssertionError("StartTutor entered direct generation graph")
    if shortest_path(graph, "EndTutor") is None:
        raise AssertionError("EndTutor audit path disappeared")

    link = source("LinkFleets")
    report = source("SortReportCache")
    if "0x13,0x1038" not in link or "0x5bb8,0x1108" not in report:
        raise AssertionError("comparator pointer callsite changed")
    if names.get("1038:1fa2") != "ICompFleetPoint2" or \
            names.get("1108:5bb8") != "ICompReport":
        raise AssertionError("comparator target address changed")
    draw = source("DrawPlanShip")
    if "iStack_1c = 0x90e" not in draw or "iStack_1c = 0x7fc" not in draw \
            or "iStack_14 * 0x10 + 6" not in draw:
        raise AssertionError("DrawPlanShip table layout changed")

    ne = NE(EXE)
    data_segment = next(i for i, segment in enumerate(ne.segments)
                        if segment["kind"] == "data")
    pointers = []
    for base, count in ((0x7fc, 6), (0x90e, 7)):
        for index in range(count):
            offset = base + index * 16 + 6
            proc_offset, selector = struct.unpack_from(
                "<HH", ne.memory[data_segment], offset)
            target = "%04x:%04x" % (selector, proc_offset)
            if target == random_address or selector not in (0x1050, 0x1058):
                raise AssertionError("unexpected DrawPlanShip callback: " + target)
            pointers.append({"descriptor": "1128:%04x" % (offset - 6),
                             "target": target})

    stack = source("__aFchkstk")
    math = source("__trandisp1")
    if "__aaltstkovr != 0xffff" not in stack or \
            "__amsg_exit" not in stack:
        raise AssertionError("stack-check continuation changed")
    if "in_DX +" not in math or "WARNING: Could not recover jumptable" not in math:
        raise AssertionError("math dispatch changed")
    if "0xf7ff | 0x800" not in source("StartTutor") or \
            "0xf7ff" not in source("EndTutor") or \
            "if (((uint)gd.overlap_0.grBits_view.grBits._0_2_ >> 0xb & 1) != 0)" \
            not in source("FGenerateTurn"):
        raise AssertionError("tutorial reset gate changed")
    draw_orders = source("DrawShipWayPtOrders")
    if "iStack_26 = (int)uStack_3c" not in draw_orders or \
            "SENDMESSAGE(rghwndOrderDD[0],0x407,0,0)" not in draw_orders or \
            "iStack_26 != 3" not in draw_orders or \
            "EstMineralsMined(uStack_18,&uStack_60,tStack_4c._4_4_,0)" \
            not in draw_orders:
        raise AssertionError("drawing callback mining guard changed")

    unresolved = {row["site"]: row for row in inventory["reachable_indirect_transfers"]
                  if row["status"] == "unresolved"}
    expected = {
        "1048:102f": "DrawPlanShip descriptor callback",
        "1118:07ee": "ICompFleetPoint2 comparator",
        "1118:082e": "ICompFleetPoint2 comparator",
        "1118:088a": "ICompReport comparator",
        "1118:0911": "ICompReport comparator",
        "1118:092f": "ICompReport comparator",
        "1118:12ed": "CRT stack-overflow continuation",
        "1118:2420": "same-segment math dispatch",
    }
    if set(unresolved) != set(expected):
        raise AssertionError("unresolved-site inventory changed")
    return {
        "schema": "rototill-rng-indirect-resolution-r1",
        "original_exe_sha256": hashlib.sha256(EXE.read_bytes()).hexdigest(),
        "callgraph_sha256": hashlib.sha256(graph_path.read_bytes()).hexdigest(),
        "functions_sha256": hashlib.sha256(functions_path.read_bytes()).hexdigest(),
        "random_address": random_address,
        "start_tutor_directly_reachable": False,
        "end_tutor_path": shortest_path(graph, "EndTutor"),
        "bsearch_comparator": "1038:1fa2 ICompFleetPoint2",
        "qsort_comparator": "1108:5bb8 ICompReport",
        "draw_plan_ship_table_callbacks": pointers,
        "drawing_mining_random_path": {
            "path": ["DrawShipWayPtOrders", "EstMineralsMined", "Random"],
            "ui_dropdown_selection_required": 3,
            "retained_ui_dropdown_selection": None,
        },
        "indirect_sites": [{"site": site, "classification": expected[site]}
                           for site in sorted(expected)],
        "scope_note": "Resolves immediate binary callback targets and callsite "
                      "types. DrawShipWayPtOrders can still reach Random "
                      "through a UI mining-preview path when dropdown "
                      "selection 3 is active. Retained files do not carry "
                      "that UI state. Tutorial and callback reentry need "
                      "execution-context review.",
    }


def main():
    result = resolve()
    OUTPUT.write_text(json.dumps(result, indent=2, sort_keys=True) + "\n")
    print("resolved %d indirect sites; %d drawing descriptors; StartTutor "
          "directly reachable=%s" %
          (len(result["indirect_sites"]),
           len(result["draw_plan_ship_table_callbacks"]),
           result["start_tutor_directly_reachable"]))


if __name__ == "__main__":
    main()
