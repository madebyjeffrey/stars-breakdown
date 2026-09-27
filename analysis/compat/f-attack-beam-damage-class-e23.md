# FAttack raw beam damage class — E23

## Source mapping

In raw `FAttack` (`analysis/decompiler/export/raw/10f0_69ac_FAttack.c`,
approximately lines 572–580), the group-0x10 beam branch sets `uStack_44` to
1 when the selected part's word at byte offset 54 is below 200, and to 2
otherwise. The raw decompiler describes the value using its inferred
`_hul.wtFuelMax` overlay. The recovered 60-byte `_beam` record identifies
offset 54 as `dp` (`STARS_OFFSET_12ac_dp=54`), so the readable input is
`StarsAttackSection.raw_part_dp`.

`attack_apply_beam_damage` now derives the weapon class from that field for
raw complex-beam damage calls instead of forwarding the previously
caller-supplied `raw_damage_flags`. The comparison is strict: 199 maps to 1;
200 maps to 2. Other FDamageTok arguments are unchanged.

## Validation

- Strict-C89 `f_attack_raw_damage_probe`: 9 checks pass; it dispatches resolved
  `dp=50` and `dp=200` cases and observes classes 1 and 2.
- `python3 -m unittest discover -s tests -q`: 519 tests pass, 153 expected
  skips, 2,241 generated layout assertions, and 846 function declarations.
- `git diff --check` passes.
- Port 2122 has a live host listener but no established guest agent session;
  native validation is not claimed.

This is host source support only and closes no named TODO6 Step-5 unit. Native
replay, original differentials, and the full battle path remain open.
