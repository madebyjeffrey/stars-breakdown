# FAttack large-intermediate shield-ratio E59

E59 checks the residual arithmetic beyond 32-bit intermediate range while
retaining the original-backed target-mode order and live-state refresh from
E55/E58.

## Large-ratio oracle

Original group-`0x10` beam item 21 has `dp=204` (E34). With two installed
components and 1,000 attacker ships, the low-32-bit initial pool is
`204 × 2 × 1000 = 408000`. Each ratio numerator (`dpMain × post-shield dp`)
is over 32 bits and must use the high/low product implementation.

| Pass | Target | Pre-shield damage | Post-shield damage | Derived residual pool |
|---|---:|---:|---:|---:|
| Initial | mode 1 / target 1 | 163,200 | 163,194 | `floor(408000 × 163194 / 163200) = 407,985` |
| Rescan | mode 0 / target 0 | 163,193 | 163,187 | `floor(407985 × 163187 / 163193) = 407,969` |

The native/host probe asserts the exact per-call input/output values, target
order, and shield depletion. These observed second-pass values distinguish the
64-bit ratio update from a stale-pool or 32-bit-wrapped calculation.

## Verification

- Strict-C89 raw damage/residual probe: `F_ATTACK_RAW_DAMAGE_CHECKS=14
  FAILURES=0 RESULT=PASS`.
- Related host probes pass: beam scaling 86/86, packed part 24/24, torpedo
  dispatch 17/17.
- Full host suite after E59 passes: 525 tests, 153 expected skips, 2,241
  generated layout assertions, and 846 function declarations.
- Fresh E59 VC++ 1.52c/Win98 compile/link pass. Native reports: packed part
  24/24, torpedo dispatch 17/17, raw damage/large ratio 14/14. All three
  processes exit 0; copied original segment matches; post-run window inventory
  has no Program Error.
- The native raw harness retains two test-only VC++ C4135 conversion warnings
  for mapping signed shield/hull fixture values into raw unsigned token fields.
- Package: `compat/vc152/f-attack-raw-residual-e59/`.
- Returns: `runs/vmctl-fapraw-e59/` (reports, logs, maps, objects, executables,
  screenshot, original segment, and agent log).

E59 closes **0 named Step-5 units**. FAttack remains 0/3 at 5/6; broader
original damage differential, complete native battle replay, and Step-6
promotion remain. TODO6 remains 120/152 evidence units closed, 32 open, 10
promotions open, and 42 practical open work items.
