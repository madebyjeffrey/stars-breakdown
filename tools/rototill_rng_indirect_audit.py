#!/usr/bin/env python3
"""Inventory original indirect transfers and RNG seed writers reachable from FGenerateTurn."""

import hashlib
import json
from collections import deque
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
BASE = ROOT / "analysis/decompiler"
OUTPUT = ROOT / "analysis/compat/do-rototill-rng-indirect-r1.json"


def path_to(name, previous):
    if name not in previous:
        return None
    result = []
    while name is not None:
        result.append(name)
        name = previous[name]
    return list(reversed(result))


def main():
    files = {
        name: BASE / name for name in (
            "callgraph.json", "functions.json", "indirect-targets.json",
            "code-pointers.json", "data-references.json")
    }
    data = {name: json.loads(path.read_text()) for name, path in files.items()}
    hashes = {name: hashlib.sha256(path.read_bytes()).hexdigest()
              for name, path in files.items()}
    outgoing = {}
    for edge in data["callgraph.json"]:
        outgoing.setdefault(edge["caller_name"], set()).update(
            edge["target_names"])
    previous = {"FGenerateTurn": None}
    queue = deque(["FGenerateTurn"])
    while queue:
        caller = queue.popleft()
        for target in sorted(outgoing.get(caller, ())):
            if target not in previous:
                previous[target] = caller
                queue.append(target)

    functions = {row["name"]: row["address"]
                 for row in data["functions.json"]}
    random_address = functions["Random"]
    indirect = []
    for row in data["indirect-targets.json"]:
        paths = [path_to(owner, previous) for owner in row["owners"]
                 if owner in previous]
        if not paths:
            continue
        indirect.append({
            "site": row["address"],
            "instruction": row["instruction"],
            "kind": row["kind"],
            "status": row["status"],
            "owners": row["owners"],
            "one_shortest_path": min(paths, key=lambda p: (len(p), p)),
            "target_count": len(row["targets"]),
            "targets_include_random": random_address in row["targets"],
        })
    indirect.sort(key=lambda row: row["site"])
    address_taken = [row for row in data["code-pointers.json"]
                     if row["target"] == random_address]
    mutators = {}
    for name in ("PushRandom", "PopRandom", "Randomize", "Randomize2",
                 "SetFileSeeds"):
        mutators[name] = {
            "address": functions[name],
            "one_shortest_path": path_to(name, previous),
        }
    result = {
        "schema": "rototill-rng-indirect-r1",
        "source_sha256": hashes,
        "root": "FGenerateTurn",
        "static_reachable_name_count": len(previous),
        "random_address": random_address,
        "reachable_indirect_transfers": indirect,
        "reachable_indirect_count": len(indirect),
        "reachable_unresolved_count": sum(
            row["status"] == "unresolved" for row in indirect),
        "reachable_os_interrupt_count": sum(
            row["status"].startswith("OS interrupt") for row in indirect),
        "recovered_indirect_targets_include_random": any(
            row["targets_include_random"] for row in indirect),
        "address_taken_random_relocations": address_taken,
        "seed_mutators": mutators,
        "scope_note": "Static direct-call reachability is an overapproximation; "
                      "missing address-taken relocations do not exclude "
                      "constructed runtime function pointers.",
    }
    OUTPUT.write_text(json.dumps(result, indent=2, sort_keys=True) + "\n")
    print("%d static names, %d indirect transfers (%d unresolved, %d OS); "
          "Random targets=%s, address-taken=%d" % (
              len(previous), len(indirect), result["reachable_unresolved_count"],
              result["reachable_os_interrupt_count"],
              result["recovered_indirect_targets_include_random"],
              len(address_taken)))


if __name__ == "__main__":
    main()
