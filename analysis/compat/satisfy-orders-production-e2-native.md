# SatisfyOrders production-service E2 native replay

The SatisfyOrders E2 boundary was exercised against the existing production
technology-learning and salvage implementations, rather than callback-only
test doubles. The typed path transferred resources into a planet order and
invoked `stars_itech_learn_a_tech`, verifying the research mutation, event
mark, and result/goto value. A point-drop order invoked
`stars_drop_salvage`, verifying the actual two-object 30,000-unit split,
values, mineral contents, and coordinates. The packed path repeated the
technology service through the raw 124-byte `_fleet` and 18-byte `_order`
adapter.

The strict-C89 host probe and fresh VC++ 1.52c/Win98 replay in `C:\SYOE8`
report:

`SATISFY_ORDERS_PRODUCTION_E2_CHECKS=18 TYPED=12 PACKED=6 FAILURES=0 RESULT=PASS EXIT=0`

The guest build reports `COMPILE=PASS` and `LINK=PASS`; the process exited 0,
and the final window inventory contained no Program Error dialog. This is a
stronger supporting E2 milestone, but it does not close the named E2 unit:
original technology-table/result binding, actual fleet/ship-design record
copying, complete raw message argument layouts, and the E3 full-turn/original
differential remain open. The point-drop callback is now bound to the
concrete salvage-object implementation; the original destruction-to-salvage
caller semantics still require a separate comparison.

Evidence:

- `tests/satisfy_orders_production_e2_probe.c`
- `compat/vc152/satisfy-orders-production-e2-native/`
- `runs/vmctl-vc152syoe8/SYO8-production-pass.TXT`
- `runs/vmctl-vc152syoe8/SYO8STA-final.TXT`
- `runs/vmctl-vc152syoe8/SYO8CL-final.LOG`
- `runs/vmctl-vc152syoe8/SYO8LINK-final.LOG`
- `reconstructed/src/itech_learn_a_tech.c`
- `reconstructed/src/drop_salvage.c`
