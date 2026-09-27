# FDoCoolBattle attack-mask ordering checkpoint

Date: 2026-09-22

## Scope

The recovered `FDoCoolBattle` loop builds the active-player mask, regenerates
and moves active tokens, then applies each player's attack mask before entering
the descending-initiative attack loop. It also rechecks the filtered survivor
mask before each candidate attack. The readable coordinator had applied the
attack-mask filter before movement and had rechecked only unfiltered token
ownership.

The source was corrected to match that recovered ordering. The focused host
probe uses two active players, lets both tokens move, gives player 0 a legal
target mask, and gives player 1 no legal target. The expected result is two
movement callbacks, no attack callback, and one surviving player after the
post-movement filter.

## Result

```text
F_DO_COOL_BATTLE_ATTACK_MASK_ORDER_CHECKS=3 FAILURES=0 RESULT=PASS
```

The strict C89 probe passed with `-pedantic -Wall -Wextra -Werror`. Existing
FDoCoolBattle source, orchestrator/FAttack integration, packed-token, and
packed-field probes also remain green: 58, 18, 11, and 24 checks respectively.

This is host-source fidelity evidence only. It closes no authoritative
Step-5 unit because native movement/initiative replay, exact damage and record
behavior, and original full-battle differential evidence remain outstanding.

## Evidence

- `reconstructed/src/f_do_cool_battle.c`
- `tests/f_do_cool_battle_attack_mask_order_probe.c`
- `tests/f_do_cool_battle_source_probe.c`
- `tests/f_do_cool_battle_attack_integration_probe.c`
- `analysis/decompiler/export/raw/10f0_8bcc_FDoCoolBattle.c`
