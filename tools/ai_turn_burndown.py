#!/usr/bin/env python3
"""Count the small AI-turn acceptance units in the inspection ledger."""

import argparse
import re
from collections import Counter
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
LEDGER = ROOT / "analysis/compat/ai-turn-burndown.md"
TASK = re.compile(r"^- \[([ x])\] \*\*([A-Z]{2}-(?:E[1-4]|P)\.[a-z])\*\*", re.M)
PARENTS = {
    "RB-E2", "RB-E3", "RB-E4", "RB-P",
    "TU-E1", "TU-E2", "TU-E3", "TU-E4", "TU-P",
    "AU-E1", "AU-E3", "AU-E4", "AU-P",
    "RT-E1", "RT-E2", "RT-E3", "RT-E4", "RT-P",
    "MA-E1", "MA-E2", "MA-E3", "MA-E4", "MA-P",
    "CY-E3", "CY-E4", "CY-P",
}
NAMES = {
    "RB": "Robotoid", "TU": "Turin Drone", "AU": "Automitron",
    "RT": "Rototill", "MA": "Macinti", "CY": "Cyber",
}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--check", action="store_true",
                        help="fail if the visible count table is stale")
    args = parser.parse_args()
    source = LEDGER.read_text()
    tasks = TASK.findall(source)
    ids = [identifier for _, identifier in tasks]
    parents = {identifier.rsplit(".", 1)[0] for identifier in ids}
    if len(ids) != len(set(ids)) or parents != PARENTS:
        raise SystemExit("duplicate unit ID or missing/unexpected parent gate")
    counts = Counter()
    for mark, identifier in tasks:
        prefix = identifier[:2]
        kind = "P" if "-P." in identifier else "E"
        counts[(prefix, kind, "total")] += 1
        if mark == " ":
            counts[(prefix, kind, "open")] += 1
    open_evidence = sum(counts[(prefix, "E", "open")] for prefix in NAMES)
    open_promotions = sum(counts[(prefix, "P", "open")] for prefix in NAMES)
    print("AI-turn units: %d open (%d evidence, %d promotion), %d closed, %d total" %
          (open_evidence + open_promotions, open_evidence, open_promotions,
           len(tasks) - open_evidence - open_promotions, len(tasks)))
    for prefix, name in NAMES.items():
        evidence = counts[(prefix, "E", "open")]
        promotion = counts[(prefix, "P", "open")]
        print("%-12s %2d evidence + %d promotion = %2d open" %
              (name, evidence, promotion, evidence + promotion))
        if args.check:
            heading = re.search(r"^## " + re.escape(name) +
                                r" — (\d+) open$", source, re.M)
            if not heading or int(heading.group(1)) != evidence + promotion:
                raise SystemExit("stale section heading: " + name)
            pattern = re.compile(r"^\| " + re.escape(name) +
                                 r" \| (\d+) \| (\d+) \| (\d+) \|$", re.M)
            match = pattern.search(source)
            if not match or tuple(map(int, match.groups())) != (
                    evidence, promotion, evidence + promotion):
                raise SystemExit("stale count table row: " + name)
    if args.check:
        expected = (open_evidence, open_promotions,
                    open_evidence + open_promotions)
        headline = re.search(r"\*\*(\d+) small acceptance units remain; "
                             r"(\d+) (?:is|are) complete\.\*\*", source)
        if not headline or tuple(map(int, headline.groups())) != (
                expected[2], len(tasks) - expected[2]):
            raise SystemExit("stale burndown headline")
        match = re.search(r"^\| \*\*All six\*\* \| \*\*(\d+)\*\* \| "
                          r"\*\*(\d+)\*\* \| \*\*(\d+)\*\* \|$", source, re.M)
        if not match or tuple(map(int, match.groups())) != expected:
            raise SystemExit("stale all-six count row")
        print("visible count table: OK")


if __name__ == "__main__":
    main()
