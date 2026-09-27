# AnimateAttack reconstruction

`reconstructed/src/animate_attack.c` is a typed semantic reconstruction of attack animation rendering. It computes battle-square center-line geometry, distinguishes enemy starbase styling, draws static and animated impact icons, suppresses hidden icons, and redraws affected tokens through callbacks.

Validation: `tests/animate_attack_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and two-case native probe pass in `runs/vmctl-vc152ew/ANASTA.TXT` and `runs/vmctl-vc152ew/ANARES.TXT`.
