# FAttack torpedo loop condition: raw stack/branch audit E52

## Result

The retained original instruction listing at
`analysis/decompiler/export/raw/10f0_69ac.asm` resolves the machine-level
condition at the end of the group-0x20 damage pass. At `10f0:7c84–7cb2`, the
code branches back to `10f0:6e04` when either of two signed 32-bit stack
quantities is positive:

```text
(*(int32_t *)(BP - 0x24) > 0) || (*(int32_t *)(BP - 0x3a) > 0)
```

The first dword occupies `[BP-0x24]` low word / `[BP-0x22]` high word and
matches CodeView local `dpMain` (`S_BPREL16 bp_offset=-36`, primitive type
18, `long`). The second occupies `[BP-0x3a]` low word / `[BP-0x38]` high word
and matches `cTorpsLeft` (`bp_offset=-58`, also `long`). The instruction
sequence compares each high word as signed, then its low word as unsigned
when the high word is zero; it therefore implements
`dpMain > 0 || cTorpsLeft > 0`, not an unsigned comparison. The alternative
branch reaches `10f0:6a02`, which increments the outer hull/section index.

The second dword is initialized on the non-`0x10` branch at
`10f0:6ddd–6e01` (the weapon-kind dispatch distinguishes the beam `0x10`
branch from the alternate path, which includes torpedoes) and is decremented
by the selected split terms at `10f0:7c65–7c68` before the loop test. This
confirms its role as a split torpedo residual. `dpMain` is computed on the
`0x10` branch at `10f0:6dca–6dcd`; on the alternate path it is zeroed at
`10f0:6ddd–6de2`, then updated in the target loop (`10f0:7753–77fe`). Its
complete update inputs still need tracing. The raw decompiler expression at
`10f0_69ac_FAttack.c:764–769` labels this test through `lVar19`, `uStack_26`,
and `lVar16`; the CodeView mapping resolves the machine-tested slots to
`dpMain` and `cTorpsLeft`. Those aliases do not match the stack slots tested
by the branch and must not be used as the storage map. See the follow-up
arithmetic and callback audit in `f-attack-dpmain-loop-arithmetic-e53.md`.

## Readable-source gap

`reconstructed/src/f_attack.c` currently loops while `remaining_packets > 0`
and exits on `!made_progress`. That does not yet represent the original
`dpMain > 0 || cTorpsLeft > 0` disjunction. The source does not model `dpMain`
as independent signed-long state, and its full update semantics and
interactions with `FDamageTok` remain unresolved. No source condition change
is justified from this condition-only identification.

## Evidence and ledger impact

- Listing: `analysis/decompiler/export/raw/10f0_69ac.asm`, especially
  `10f0:6dca–6e01`, `10f0:7753–77fe`, `10f0:7c65–7c68`, and
  `10f0:7c84–7cb2`.
- CodeView locals: `analysis/debug/symbols.json`, FAttack procedure
  `180:00001bcc`, locals `dpMain` and `cTorpsLeft`; type index 18 is 4-byte
  `long` in `analysis/debug/primitives.json`.
- Raw C cross-reference: `analysis/decompiler/export/raw/10f0_69ac_FAttack.c`.
- Strict-C89 targeted FAttack packed-part and raw-torpedo tests pass (2 tests).
- No source behavior changed and no named Step-5 evidence unit closes.
  FAttack remains 0/3 named Step-5 units and 5/6 overall; TODO6 counters are
  unchanged.

## Next evidence step

Trace `dpMain`'s definitions and aliases across target selection and damage
service calls, then compare its mutations with `FDamageTok`'s original
instructions. Only after that mapping is stable should host/native state be
expanded to represent both independent residuals and compare full mutation
order against a focused original-executable case.
