# ScoreGuessBattleDamage reconstruction

`reconstructed/src/score_guess_battle_damage.c` is a typed semantic reconstruction of battle-damage scoring. It preserves target-class filtering, give/take tactic scoring, incoming/outgoing damage accumulation, and the original score combination path.

Validation: the strict host C89 probe links with the reconstructed `md_target.c` and `battle.c` helpers; `tests/score_guess_battle_damage_source_probe.c` passes. The VC1.52c Win98 native report passes in `runs/vmctl-vc152ew/SGSTA2.TXT` and `runs/vmctl-vc152ew/SGRES.TXT`.
