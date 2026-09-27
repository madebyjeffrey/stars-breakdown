# FAttack torpedo split and residual-loop source map — E20 analysis

This is an evidence map for the next source implementation step, not a claim
that torpedo damage handling is complete. References are to the reviewed raw
Ghidra listing and are kept separate from the inferred semantic names below.

## Observed raw flow

1. The available packet budget is loaded from `uStack_3c` into `uStack_42`,
   then `CTorpHit` produces a distinct successful-hit value in `uStack_62`
   (`10f0_69ac_FAttack.c:620-625`). They are not interchangeable.
2. The target state supplies ship count, total shield capacity, and effective
   hull capacity. The `_dv` modifier reduces hull capacity before split
   selection (`:627-659`). The selected weapon's `wtFuelMax` supplies the
   damage scale; special items 8–11 set damage class bit 8 and can double the
   scale when no shield capacity remains (`:660-675`).
3. If ship count is below the current packet budget and the initial
   `hit_count * damage_scale` exceeds effective hull capacity, the code scans
   candidate allocation sizes from ship count up to the packet budget. At
   each size it computes a rounded hit allocation and its complement, applies
   separate `/8` and `/2` damage scales, subtracts the shield term from the
   shield capacity with a zero floor, and stops when its hull threshold is
   reached (`:677-726`). The fallback sets the two terms to
   `budget - hit_count` and `hit_count` (`:728-732`).
4. The first allocation term produces shield-only damage (`term * scale / 8`)
   and calls `FDamageTok` with `grfWeapon | 0x80`, `fShieldsOnly=1`, and no
   packet pointer. A successful return increments the hit record; the
   callback-mutated beam amount is retained (`:733-747`). The second term
   produces hull damage (`term * scale / 2`); the sum of both terms is passed
   by pointer as the allocated packet count. The hull call's result is
   ignored and the hit record increments unconditionally (`:748-763`).
5. The machine branch at `10f0:7c84–7cb2` repeats the group-0x20 pass at
   `10f0:6e04` while either signed 32-bit stack quantity is positive:
   `[BP-0x24]:[BP-0x22]` or `[BP-0x3a]:[BP-0x38]`, with the first word of each
   pair the low word. CodeView `S_BPREL16` locals map these to `dpMain` (BP
   offset -36 decimal, `long`) and `cTorpsLeft` (BP offset -58 decimal,
   `long`), respectively. The latter pair is initialized on the non-`0x10`
   weapon-kind path and has the selected split terms subtracted at
   `10f0:7c65–7c68`. `dpMain` is computed on the `0x10` branch and zeroed on
   the alternate path, then adjusted in the target loop; its full update
   semantics are not yet reconciled. The raw C names
   `lVar19`, `uStack_26`, and `lVar16` at lines 764–769 do not match these
   tested stack slots and are not a reliable storage map. See the E52
   instruction audit in `f-attack-outer-loop-assembly-e52.md`.
6. `FDamageTok` itself may mutate the token, ship count, damage pointer, and
   packet pointer. The raw fleet path records casualties and writes its
   remaining packet count back through `pcTorp` (`10f0_81d4_FDamageTok.c:272-299`).

## Current readable-path gap

This note originally described the pre-split implementation. The current
`reconstructed/src/f_attack.c` now computes both allocation terms, dispatches
shield-only and hull damage separately, passes their sum through the mutable
packet pointer, and subtracts the allocated amount from the residual budget.
E46 routes the multi-pass probe through original group-0x20 record bytes and
passes the item-8 85/42 residual sequence in both host C89 and fresh Win98
VC++ 1.52c runs. Its evidence is
`analysis/compat/f-attack-torpedo-record-residual-native-e46.{md,json}`.

Remaining gaps are material: E52 mapped the loop condition to CodeView locals
(`dpMain > 0 || cTorpsLeft > 0`), and E53 traced a `dpMain * dp / dpT` update
capped at `dpMain - 1`. `dpT` snapshots mutable `dp` before shield-only
`FDamageTok`, so the update scales `dpMain` by the post-shield beam remainder.
The readable source still does not model this independent state or update;
it currently uses `remaining_packets > 0` plus a `made_progress` guard. Also,
post-`FDamageTok` target/hull/shield mutations are not explicitly rebound into
the next target-score/allocation iteration unless the callback context aliases
the same target storage. E46's scripted random/target fixture does not prove
whole-game mutation order, every split-scan threshold, or a full
original-executable differential.

## Implementation and proof plan

The split allocation and a first record-backed residual sequence are now in
place. Next resolve `dpMain`'s update semantics and trace both quantities
through `FDamageTok` mutations; then make the source state explicit. After
that, add original-backed cases for fallback and scan branches,
item-class/doubled-scale interaction, threshold equality, shield exhaustion,
hull depletion, and both residuals; run them in strict C89 and under VC++
1.52c before reassessing the named Step-5 gate. E46/E51 are supporting
evidence only: the authoritative TODO6 score remains 120/152 closed, 32 open,
10 promotion units, and 42 practical open work items.
