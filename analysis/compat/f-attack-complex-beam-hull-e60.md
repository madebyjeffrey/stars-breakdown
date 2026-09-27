# FAttack complex-beam live hull-damage checkpoint E60

E60 extends the raw complex-beam live-target probe from shield-only callbacks
to a full shield-plus-hull damage call. A 408-point residual beam is applied to
a two-ship target with 3 shield points and 10 hull DP per ship. The probe
checks that the shield layer absorbs 6, the remaining beam reaches the hull,
both ships are killed, the live target's ship count becomes zero, the mutable
beam amount is consumed, and FAttack records the hit and first target.

## Verification

- Strict-C89 raw-damage probe: `F_ATTACK_RAW_DAMAGE_CHECKS=15 FAILURES=0
  RESULT=PASS`.
- Related strict-C89 probes pass: beam scaling 86/86, packed part 24/24, and
  raw torpedo dispatch 17/17.
- Fresh `C:\FAE60` package compiles and links with VC++ 1.52c in Win98.
  Native reports pass: packed part 24/24, raw torpedo dispatch 17/17, raw
  damage/hull casualty 15/15. All three guest processes exit 0.
- The returned original segment matches the package byte-for-byte. The
  post-run window inventory has no Program Error dialog.
- The full host suite passes 525 tests, 153 expected skips, 2,241 generated
  layout assertions, and 846 function declarations.
- VC++ reports two test-only C4135 signed/unsigned fixture-conversion warnings
  in `RAW.C`; compile and link still pass.
- Package: `compat/vc152/f-attack-raw-residual-e60/`.
- Returns: `runs/vmctl-fapraw-e60/` (reports, logs, maps, objects,
  executables, screenshot, original segment, and agent log).

E60 adds supporting integration evidence but closes **0 named Step-5 units**.
FAttack remains 0/3 at 5/6; complete weapon/hull/target service acceptance,
broader original damage differentials, complete native battle replay, and
promotion remain open. TODO6 remains 120/152 evidence units closed, 32 open,
10 promotions open, and 42 practical open items.
