# SatisfyOrders advanced E2 native boundary replay

The production `stars_satisfy_orders` and `stars_satisfy_orders_packed`
sources now expose explicit service boundaries for the remaining advanced
order effects: technology discovery after transferred resources, ship-design
copy completion for a created player transfer, salvage handoff for a point
drop, and a richer message-layout trace for ordinary and player-transfer
messages.

The strict-C89 host probe and fresh VC++ 1.52c/Win98 replay in `C:\SYOE7`
both report
`SATISFY_ORDERS_ADVANCED_E2_CHECKS=20 FAILURES=0 RESULT=PASS`.
The matrix checks typed and packed paths, transfer totals, technology amount,
salvage mineral vectors, created-transfer ship-copy dispatch, no-design
suppression, recipient/message-code pairs, and the packed technology flag.

The guest build returned `COMPILE=PASS` and `LINK=PASS`, the process exited 0,
and the final window inventory contained no Program Error dialog. This is a
supporting E2 milestone, not closure of the full named E2 unit: original
technology tables, actual fleet/ship-design record copying, concrete salvage
object creation, complete raw message argument layouts, and the E3 full-turn /
original differential still require binding and comparison.

Evidence:

- `tests/satisfy_orders_advanced_e2_probe.c`
- `compat/vc152/satisfy-orders-advanced-e2-native/`
- `runs/vmctl-vc152syoe7/SYOE7-host.TXT`
- `runs/vmctl-vc152syoe7/SYOSTA-final.TXT`
- `runs/vmctl-vc152syoe7/SYOCL-final.LOG`
- `runs/vmctl-vc152syoe7/SYOLINK-final.LOG`
- `runs/vmctl-vc152syoe7/SYOE7-pass.TXT`
- `analysis/decompiler/export/raw/10b0_6798_SatisfyOrders.c`
