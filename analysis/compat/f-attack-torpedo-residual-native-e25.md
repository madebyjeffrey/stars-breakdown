# FAttack residual torpedo loop — native E25 / current-source E27

## Scope and source state

The production `reconstructed/src/f_attack.c` residual packet loop was exercised
through a fresh VC++ 1.52c Win98 packet. The E25 `FA.C` source matches the
production file after normalizing CRLF and adapting only the included header
name from `core.h` to the DOS package's `CORE.H`. Its raw damage, target, and
dispatch probes are the current host probe sources.

The E24 attempt is retained separately. Its batch process exited 0, but
`FAPNATSTA.TXT` said `COMPILE=FAIL / LINK=FAIL`; `RAWLINK.LOG` contained
unresolved `_printf`. The E24 `RAWNAT.EXE` therefore was not run or credited.
E25 adds the no-op `printf` shim around included probe code and passes the fresh
packet compile/link.

## Native E25 results

The complete six-program packet reports `COMPILE=PASS` and `LINK=PASS`.
Retrieved per-program compile/link logs are clean. Fresh native report files
show:

- `LOOKUP_PART_TABLE_CHECKS=34 FAILURES=0 RESULT=PASS`
- `F_ATTACK_PACKED_PART_CHECKS=13 FAILURES=0 RESULT=PASS`
- `F_ATTACK_COMPLEX_SCORE_CHECKS=9 FAILURES=0 RESULT=PASS`, including
  `MIN_HULL_ACTUAL=18181 EXPECTED=18181`
- `F_ATTACK_COMPLEX_SELECTION_CHECKS=3 FAILURES=0 RESULT=PASS`
- `F_ATTACK_RAW_DAMAGE_CHECKS=9 FAILURES=0 RESULT=PASS`
- `F_ATTACK_TORPEDO_TARGET_CHECKS=5 FAILURES=0 RESULT=PASS`
- `F_ATTACK_RAW_TORPEDO_DISPATCH_CHECKS=9 FAILURES=0 RESULT=PASS`

The E25 scaling report initially failed because its copied `FASCALE.C` was
stale: it expected six instead of four hits in the residual-budget case and
did not initialize the target's resource/hull fields. This was a fixture
failure, not evidence against current source behavior. The current
`tests/f_attack_source_probe.c` was packaged unchanged as `FASCALE.C` in the
separate E27 packet; under VC++ 1.52c it compiled/linked and returned
`F_ATTACK_CHECKS=34 FAILURES=0 RESULT=PASS`. E26 is retained as a packaging
failure: its reduced packet omitted `TORP.C`; E27 includes that dependency.

E25 and E27 processes exited 0. The post-run E25 window inventory contained
the Win98 desktop and agent only, with no Program Error dialog. Reports,
compile/link logs, and the relevant E25/E27 object, executable, and map files
are preserved under `runs/vmctl-vc152fapne25/` and
`runs/vmctl-vc152fapne27/`.

## Host validation and remaining work

`python3 -m unittest discover -s tests -q`: 519 tests pass, 153 expected
skips, 2,241 generated layout assertions, and 846 function declarations.
`python3 -m unittest tests.test_vc152_package -v`: 3 tests pass.
Fresh strict-C89 host builds/runs also pass the current FAttack source probe
(34 checks), raw-damage probe (9), torpedo-target probe (5), and raw-dispatch
probe (9), all with zero failures. `git diff --check` passes.

This strengthens Phase-2 steps 2–4 but closes **0 named Step-5 units** and
does not promote `FAttack`. Step 5 still needs the runtime-backed group-0x20
torpedo record resolver (the E21 callback remains a fixture), an original
behavior differential for the shield/hull and residual-target sequence, and
complete native battle replay across packed token/global state. The
authoritative Phase-2 counters remain 120/152 named Step-5 units closed, 32
open, and 10 step-6 promotions outstanding (42 practical open work items).
