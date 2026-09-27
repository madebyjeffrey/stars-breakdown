# FAttack complex-beam shield-ratio oracle E58

E58 strengthens E55's raw residual-rescan evidence with exact values derived
from retained original evidence, rather than checking only that another pass
occurs.

## Oracle and observed sequence

E34 records original group-`0x10` item 21 `_beam.dp=204`. The test uses two
installed components and one attacker ship, so original `dpMain` initializes
to `204 * 2 * 1 = 408`. Both tokens begin with two ships, three shield points
per ship, and beam defense 80%; the capacity factor is 50% and integer distance
loss is zero.

The native/host trace is:

| Pass | Chosen target | Pre-shield beam value | Post-shield beam value | Residual pool |
|---|---:|---:|---:|---:|
| Initial | mode 1, target 1 | 163 | 157 | `floor(408 × 157 / 163) = 392` |
| Rescan | mode 0, target 0 | 156 | 150 | `floor(392 × 150 / 156) = 376` |

The next live scan sees both shield capacities exhausted and exits. The test
also confirms the callback can return false for shield-only damage while still
mutating the live target; FAttack must reread that target before selection.

## Verification

- Strict-C89 raw-damage regression: `F_ATTACK_RAW_DAMAGE_CHECKS=13
  FAILURES=0 RESULT=PASS`.
- Full host suite passes 525 tests, with 153 expected skips, 2,241 layout
  assertions, and 846 declarations.
- Related strict-C89 tests pass: beam scaling 86/86, packed-part 24/24, raw
  torpedo dispatch 17/17.
- Fresh E58 VC++ 1.52c/Win98 build and link pass. Native reports pass: packed
  part 24/24, raw torpedo dispatch 17/17, raw-damage/rescan/ratio 13/13. All
  processes exit 0; the copied NE segment matches the package; post-run window
  inventory contains no Program Error.
- The raw test harness has two VC++ level-3 C4135 conversion warnings in its
  test-only assignment of typed signed shield/hull values to raw unsigned
  fields; these are not errors and do not occur in the production source.
- Package: `compat/vc152/f-attack-raw-residual-e58/`.
- Full return: `runs/vmctl-fapraw-e58/` (reports, logs, maps, objects,
  executables, screenshot, segment, and agent log).

This closes **0 named Step-5 units**. FAttack remains 0/3 named units and 5/6
overall. Broader original damage differentials, full native battle replay,
and Step-6 promotion remain. TODO6 counters remain 120/152 evidence units
closed, 32 open, 10 promotions open, and 42 practical open items.
