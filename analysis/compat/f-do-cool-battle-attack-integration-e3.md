# FDoCoolBattle to FAttack integration (host E3)

## Result

The strict host C89 probe passes:

```text
F_DO_COOL_BATTLE_ATTACK_INTEGRATION_CHECKS=18 FAILURES=0 RESULT=PASS
```

## Scope

`tests/f_do_cool_battle_attack_integration_probe.c` connects the actual
reconstructed `FDoCoolBattle` orchestrator to the actual reconstructed
`FAttack` section selector and a damage callback. A deterministic two-token
battle initializes both participants, moves both tokens, dispatches the
descending initiative attack through `FAttack`, applies damage to the target
token, deactivates that token, stops when one player remains, and emits the
typed movement/attack events and battle summary. The probe checks the
attacker/target identity, damage amount, event counts, token mutation,
survivor mask, header/report/release lifecycle, and zero-round termination.

## Status

A fresh production-source VC++ 1.52c packet in
`compat/vc152/f-do-cool-battle-attack-production-e3/` ran in `C:\CBATP`.
The first native replay passed but exposed a signed/unsigned warning in the
`FAttack` section-index comparison. The production source was corrected to
make the unsigned 16-bit boundary explicit, the packet was refreshed, and the
final native compile/link replay completed without that warning. The process
exited 0, and the final native report is:

`F_DO_COOL_BATTLE_ATTACK_INTEGRATION_CHECKS=18 FAILURES=0 RESULT=PASS`

The final evidence is `runs/vmctl-vc152cbatp/COOLATSTA-final2.TXT`,
`COOLACL-final2.LOG`, `COOLATLINK-final2.LOG`, and `CBATRES-final2.TXT`.

The post-run Win98 window inventory contained no Program Error dialog. This
closes the native battle-chain replay subcheck, but not a named Step-5 unit:
packed token/global mapping, exact part lookup and damage scaling, complete
battle-log/original differential behavior, full-battle replay, and promotion
remain open.

## Inputs

- `reconstructed/src/f_do_cool_battle.c`
- `reconstructed/src/f_attack.c`
- `reconstructed/src/c_torp_hit.c`
- `reconstructed/src/dxy_move_tok_to.c`
- `reconstructed/src/dz_move_range_to_consider.c`
- `reconstructed/src/initialize_board.c`
- `reconstructed/src/battle.c`
- `reconstructed/src/md_target.c`
- `reconstructed/include/core.h`
- `tests/f_do_cool_battle_attack_integration_probe.c`
