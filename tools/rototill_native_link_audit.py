#!/usr/bin/env python3
"""Map a VC1.52c Rototill link diagnostic to candidate source modules."""

import argparse
import hashlib
import json
import re
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
DEFAULT_LOG = ROOT / "runs/rototill-segmented-stage9-r1/LINK2.LOG"


def audit(log_path):
    log = log_path.read_text(encoding="latin-1")
    symbols = sorted(set(re.findall(r"error L2029: '_([^']+)'", log)))
    if not symbols:
        raise ValueError("no L2029 unresolved externals in linker log")
    sources = sorted((ROOT / "reconstructed/src").glob("*.c"))
    source_text = {path: path.read_text(encoding="utf-8", errors="replace")
                   for path in sources}
    candidates = {}
    for symbol in symbols:
        pattern = re.compile(r"\b" + re.escape(symbol) +
                             r"\s*\([^;{}]*\)\s*\{", re.S)
        candidates[symbol] = [str(path.relative_to(ROOT)) for path in sources
                              if pattern.search(source_text[path])]
    used = sorted(set(path for paths in candidates.values() for path in paths))
    return {
        "schema": "rototill-native-link-audit-r1",
        "log": str(log_path.relative_to(ROOT)),
        "log_sha256": hashlib.sha256(log_path.read_bytes()).hexdigest(),
        "unresolved_count": len(symbols),
        "candidate_source_count": len(used),
        "candidate_sources": used,
        "symbols_without_source_candidate": [symbol for symbol in symbols
                                             if not candidates[symbol]],
        "candidate_definitions": candidates,
        "scope": "Regex-derived candidate definitions; compile and link are still required",
    }


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--log", type=Path, default=DEFAULT_LOG)
    parser.add_argument("--output", type=Path, required=True)
    args = parser.parse_args()
    result = audit(args.log.resolve())
    args.output.write_text(json.dumps(result, indent=2, sort_keys=True) + "\n")
    print("%d unresolved, %d candidate source modules, %s without candidates" %
          (result["unresolved_count"], result["candidate_source_count"],
           ", ".join(result["symbols_without_source_candidate"])))


if __name__ == "__main__":
    main()
