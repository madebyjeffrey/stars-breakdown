# FAttack raw complex-beam residual rescan E55

## Source change

E54 found that the original complex group-`0x10` branch is residual-driven and
rescans candidates against target state mutated by `FDamageTok`. The former
read-only `targets` snapshot could not express that behavior. E55 adds the
optional `StarsAttackOps.read_target` callback and the raw attacker
`attacker_ship_count` input. For raw complex beams with resolved part damage,
part count, and live-target access, `attack_beam_section_raw_residual` now:

1. Seeds a low-32-bit damage pool from `raw_part_dp * part_count *
   attacker_ship_count`.
2. Searches mode 1 first and uses mode 0 only if mode 1 has no candidate.
3. Reads the selected live token before damage and reruns candidate scoring
   from live state after the callback.
4. Updates the residual pool using the mapped
   `min(dpMain - 1, floor(dpMain * dp / dpT))` relation, with a C89 64-bit
   product/divide helper.

The existing typed/fixed-pass beam path remains in use when the raw callbacks
or raw part fields are absent. The source does not yet claim complete FAttack
fidelity.

## Verification

- Strict-C89 host reports pass: raw-damage/rescan 12/12, beam-scaling 86/86,
  packed-part 24/24, and raw torpedo dispatch 17/17.
- Full host suite passes 525 tests, with 153 expected skips, 2,241 layout
  assertions, and 846 declarations.
- Fresh E57 VC++ 1.52c/Win98 package compiles and links. Native reports pass:
  packed part 24/24, torpedo dispatch 17/17, raw damage/rescan/mode-0 fallback
  12/12. The two-target shield-only case proves a callback can return false
  while still mutating the first target; a subsequent live scan rejects its
  exhausted shields and selects the mode-0 target.
- Each native process exits 0. The copied original NE segment matches the
  package byte-for-byte. Post-run windows and the retained screenshot show no
  Program Error dialog.
- Package: `compat/vc152/f-attack-raw-residual-e57/`.
- Native returns, reports, logs, maps, objects, executables, screenshot,
  segment, and agent log: `runs/vmctl-fapraw-e57/`.
- E56's earlier 11-check native pass is retained at `runs/vmctl-fapraw-e56/`.
- E55's initial wrapper-link failure is preserved at
  `runs/vmctl-fapraw-e55/`; it was fixed by renaming the included host probe's
  `main` symbol in the native wrapper, then re-running in a fresh directory.

## Status and remaining scope

This is supporting evidence only and closes **0 named Step-5 units**. FAttack
remains 0/3 named Step-5 units and 5/6 overall. The fixture proves live shield
depletion is visible on a subsequent scan, mode-0 fallback occurs after mode 1
loses all eligible targets, and the residual path terminates. It does not yet
establish the full original shield-ratio differential, broad damage
acceptance, or a complete battle replay. Full promotion remains open.

TODO6 counters remain 120/152 Step-5 evidence units closed, 32 open, 10
promotion units open, and 42 practical open items.
