# FAttack torpedo packet-budget forwarding — E19

E19 corrects the packet-count input forwarded to raw `FDamageTok`. Raw
`FAttack` retains the launched torpedo budget in `uStack42` before calling
`CTorpHit` (`analysis/decompiler/export/raw/10f0_69ac_FAttack.c:620-625`).
The hit count is a separate value (`uStack62`). The torpedo allocation
branches partition the budget into `uStack34` and `uStack38` and pass their
sum through `pcTorp` at lines 677-757. Therefore successful hits must not be
substituted for the packet budget.

The readable path now initializes the callback packet count from
`section->torpedo_count`, not `best_hits`. A strict-C89 probe makes two of
three torpedoes hit and verifies the hull callback still receives packet
count 3 (and can mutate it to 2). The same seven-check dispatch probe continues
to cover the E18 item-7/item-8 damage-class boundary.

## Native status and remaining scope

A fresh 43-file packet is prepared at
`compat/vc152/f-attack-complex-native-e19/` for `C:\FAPNE19`: 42 ASCII/CRLF
text files plus the DOS stub. FAttack and the raw/torpedo probe pass
strict-C89 syntax checking against the packet's reduced `CORE.H`; no guest
compile/run report is yet recorded.

This fixes the distinction between total launched packets and successful
hits, but does not yet calculate the raw `uStack34`/`uStack38` split from
shield/hull capacities. The inner residual loop (`lVar19`/`lVar16`, raw lines
764-769) is also not represented: the current typed loop chooses and attacks
by pass, and does not feed the post-`FDamageTok` target state back into the
same-target split. These must not be approximated by blindly retrying the
callback. Broader damage differential, full battle replay, native replay, and
promotion remain open. This closes zero named Step-5 units; the scorecard
remains 120/152 closed, 32 open, 10 promotion units, and 42 practical open
work items.
