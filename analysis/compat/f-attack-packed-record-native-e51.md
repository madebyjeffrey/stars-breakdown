# FAttack packed raw-part resolver native integration E51

## Result

Fresh package `compat/vc152/f-attack-packed-record-native-e51/` integrates
`stars_lookup_part_record` with packed FAttack and copies the unrelocated NE
segment-2 image from `target/stars.exe`. The packed probe uses the production
record resolver for the original beam record; the companion torpedo probe
uses it for the original group-0x20 record and residual-damage sequence.

## Evidence

- Host strict-C89 regressions pass:
  `F_ATTACK_PACKED_PART_CHECKS=24 FAILURES=0 RESULT=PASS` and
  `F_ATTACK_RAW_TORPEDO_DISPATCH_CHECKS=17 FAILURES=0 RESULT=PASS`.
- Fresh Win98 / VC++ 1.52c build: `COMPILE=PASS`, `LINK=PASS`. Both native
  executables exit 0 and report the same 24/24 and 17/17 results.
- Guest window inventory after execution contains the agent, Explorer, and
  Program Manager windows, with no Program Error dialog.
- Returned `SEG2.BIN` is byte-identical to the package image; its SHA-256 is
  `52b2fb4ebc7c8ea4527b2d277fcaf3218c916053d9e295e33900cc637a753698`. The
  source executable SHA-256 is
  `ccab6682b55dae14dbf104e491cfbd3b32a6280022020032ae0615500949ba59`.
- Reports, compiler/linker logs, both EXEs, maps, objects, screenshot, segment,
  and agent log are preserved in `runs/vmctl-fapne51/`.
- `git diff --check` passes. No source changed during this native run; the
  full-suite evidence remains the preceding E47 checkpoint's 524 passing
  tests, 153 expected skips, 2,241 layout assertions, and 846 declarations.

## Limits and ledger impact

This proves native execution of reconstructed code using copied static
original bytes and scripted services. It is not a live Stars! process CS/table
mapping, complete `FLookupPart` race/hidden/technology service execution,
whole-battle replay, or an original-executable behavioral differential. It
closes **0 named Step-5 units**; TODO6 evidence/promotion counters do not
change. `FAttack` remains 0/3 named Step-5 units and 5/6 overall.

## Source and package

- `reconstructed/src/f_attack.c`
- `reconstructed/src/lookup_part.c`
- `tests/f_attack_packed_part_probe.c`
- `tests/f_attack_raw_torpedo_dispatch_probe.c`
- Package: `compat/vc152/f-attack-packed-record-native-e51/`
- Guest: `C:\\FAPNE51`
- Returns: `runs/vmctl-fapne51/`
