# FAttack complex-beam residual loop and target rescan audit E54

## Result

E54 traces the complex group-`0x10` branch around the E53 `dpMain` arithmetic
in the retained raw C and instruction listing. It confirms that the gap is a
loop/state-integration problem, not just a missing arithmetic helper:

- The attacker's initial beam pool is `_beam.dp * installed_component_count *
  attacker_tok.csh` (low 32 bits). The packed simple/broadcast beam branch is
  distinct; this note concerns the complex target-ranking branch.
- The complex branch enters a residual-driven `do` loop. On each iteration it
  searches target mode 1 first and mode 0 second. It stops searching as soon
  as a mode has a best eligible target; mode 0 is fallback only when mode 1
  has no candidate. It then applies damage and repeats the search while
  `dpMain > 0` (or the torpedo residual is positive on the group-`0x20` path).
- For a selected beam target, the mutable beam value is snapshotted in `dpT`,
  passed by address to `FDamageTok`, then `dpMain` becomes zero when either
  the post-call damage or pre-call snapshot is nonpositive; otherwise it is
  `min(dpMain - 1, floor(dpMain * dp / dpT))`.
- The new scan must see the target token's post-damage state. Original
  `FDamageTok` mutates the live token (shield points and/or ship count) before
  the next candidate ranking. The current `StarsAttackInput.targets` is a
  `const StarsAttackToken *` snapshot, and `StarsAttackRawDamageToken` does not
  return refreshed token state. The current fixed `beam_passes` loop cannot
  faithfully repeat the raw rescan using that stale view.

## Implementation boundary

Do not implement the residual loop against a stale target array. The next
source change needs an evidence-backed way to refresh/read target state after
each damage callback, plus explicit attacker ship count and component count
inputs for initializing the beam pool. Then replace the fixed raw-complex
beam pass with mode-priority rescans while preserving the ordinary typed path
and broadcast branch. Add tests for target depletion changing the next
selection, mode-0 fallback, shield absorption ratio, and pool termination.

This audit changes no reconstructed behavior and closes **0 named Step-5
units**. FAttack remains 0/3 named Step-5 units and 5/6 overall. TODO6 remains
120/152 evidence units closed, 32 open, 10 promotions open, 42 practical
open work items.

## Evidence

- `analysis/decompiler/export/raw/10f0_69ac_FAttack.c:241–258` initializes the
  complex-beam pool and enters the common residual loop.
- `:259–289`, `:512–529` search mode 1 then 0, break on the first mode with a
  candidate, and stop when neither mode selects a target.
- `:533–615` applies beam scaling, dispatches mutable damage, and updates
  `dpMain` from the post-/pre-shield ratio.
- `analysis/decompiler/export/raw/10f0_69ac.asm:77a9–780e` implements the
  quotient and residual update; `:7c84–7cb5` tests the two residuals.
- Current readable implementation: `reconstructed/src/f_attack.c`,
  `attack_beam_section`; API limitation: `reconstructed/include/core.h`,
  `StarsAttackInput.targets` and `StarsAttackRawDamageToken`.
