# FAttack `dpMain` loop arithmetic and callback alias audit E53

## Result

The E52 branch audit established that the group-0x20 loop repeats while
`dpMain > 0 || cTorpsLeft > 0`. E53 traces more of those values through the
retained instruction listing at `analysis/decompiler/export/raw/10f0_69ac.asm`:

- On the `grfWeapon == 0x10` path, `dpMain` is initialized as a product of
  three values: selected group-`0x10` part word `dp` at offset `0x36`, the
  current attacker's installed component count, and attacker token `csh` at
  offset `0x13`. Machine code extracts the component count byte from the
  current `_shdef.hul.rghs[i].overlap_2` at `10f0:6a66–6adb`; CodeView names
  the 16-bit temporary `dxRangeCur`, but that name is misleading here.
  CodeView gives `_beam.dp` offset 54 (`0x36`) and `_tok.csh` offset 19
  (`0x13`). The raw decompiler calls the overlapping part pointer
  `_hul *p_Var11` and labels the word `wtFuelMax`; those are not the right
  semantic names for this group-`0x10` access. The two calls at `10f0:6dbe`
  and `10f0:6dc5` use the retained `1118:0c7e` unsigned 32-by-32 multiply
  helper; the second multiply retains the low 32 bits. This is aggregate
  beam damage potential (`part dp × installed count × attacker ship count`).
  The alternate weapon-kind branch zeroes `dpMain`.
- In the later target-selection update at `10f0:7753–77fe`, an integer fast
  path and an x87 fallback compute `dpMain * dp / dpT`; CodeView maps the
  dword operands at BP `-0x66` and `-0x58` to locals `dp` (BP -102 decimal)
  and `dpT` (BP -88 decimal), respectively. Immediately before the
  shield-only `FDamageTok` call at `10f0:76e4`, `dpT` snapshots `dp`; that
  call receives `&dp` as its mutable beam-damage argument. The quotient
  therefore scales `dpMain` by the remaining beam-damage ratio `dp / dpT`.
  The value assigned back to `dpMain` is capped at `dpMain - 1`, so each
  successful update strictly decreases a positive `dpMain`. `dpT` is not
  `cTorpsLeft` (BP -58 decimal, machine BP -0x3a); do not replace the
  CodeView-to-instruction mapping with raw decompiler stack aliases.
- At `10f0:7c20`, the hull `FDamageTok` call receives `&cTorpsLeft` as
  `pcTorp`. The raw `FDamageTok` listing at `10f0:81d4` reads that pointed
  long, subtracts the casualty count with a zero floor, and writes it back.
  After returning, `FAttack` separately subtracts the selected split sum from
  `cTorpsLeft` at `10f0:7c65–7c68`. Therefore the original can mutate this
  residual in both the damage callback and the caller's split-accounting step.

This is stronger than identifying the branch condition, but it is not yet a
full semantic reconstruction. The beam damage and shield-ratio operands are
mapped, but the interaction/order of the two `cTorpsLeft`
mutations still needs an original-backed case before the readable C loop is
changed.

## Readable-source gap and status

`reconstructed/src/f_attack.c` currently uses `remaining_packets > 0` and a
`made_progress` break; it has neither an independent `dpMain` state nor the
observed callback/caller dual residual mutation. The current
`reconstructed/src/f_damage_tok.c` does mutate the packet pointer by
casualties, but its existing torpedo probe does not yet prove this complete
`FAttack` call sequence against the original executable.

No source behavior changed and no named Step-5 evidence unit closes at E53.
`FAttack` remains 0/3 named Step-5 units and 5/6 overall. TODO6 remains
120/152 Step-5 units closed, 32 open, 10 promotions open, 42 practical work
items open.

## Evidence and next step

- `10f0:6d9f–6e04`: kind split, `dpMain` product initialization, and
  `cTorpsLeft` initialization.
- `10f0:7753–780e`: bounded quotient update and zeroing path for `dpMain`.
- `10f0:7c13–7c68`: `&cTorpsLeft` passed to hull `FDamageTok`, then caller
  subtraction of selected split terms.
- `10f0_81d4_FDamageTok.c` and retained FDamageTok instructions: the pointed
  packet residual is reduced by casualties and floored at zero.
- `1118_0c7e.asm`: 32-bit multiply helper. `1118_11fc.asm`: x87 integer
  conversion helper used by the fallback.

Next make `dpMain`/shield-ratio state explicit in readable source and add
focused original-executable cases that observe both the shield-ratio update
to `dpMain` and the hull callback mutation followed by caller subtraction.
Only after those behaviors are evidenced should the remaining loop model be
promoted.
