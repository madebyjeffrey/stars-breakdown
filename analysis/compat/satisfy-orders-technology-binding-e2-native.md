# SatisfyOrders technology-table/result binding E2 native replay

The resource-resolution technology branch now exposes the original signed
`ITechLearnATech` result through `stars_bind_satisfy_technology`. Point
destinations select the recovered negative/positive message pair `0x013c` /
`0x013d`; planet destinations select `0x013e` / `0x013f`. Negative signed
results map to trader-part IDs with `-(result + 1)`, positive results map to
the next battle-technology ID with `result - 1`, and zero preserves the
no-learning result.

The strict-C89 host probe invokes the production
`stars_satisfy_orders` → `technology_binding` path and then the real
`stars_itech_learn_a_tech` implementation with actual trader-chance,
battle-threshold, research, event, and goto state. The point/trader case and
planet/research case report:

`SATISFY_ORDERS_TECHNOLOGY_BINDING_E2_CHECKS=12 FAILURES=0 RESULT=PASS`

The fresh VC++ 1.52c/Win98 packet in `C:\SYOE11` reports the same 12 checks.
The corrected response file includes `SYO11.DEF`, so the linker applies the
Win16 stack, heap, and stub directives. The final native build reports
`COMPILE=PASS` and `LINK=PASS`; the process exits 0, and the final guest
window inventory contains no Program Error dialog. The final packet uses the
normal `/O` compiler setting; the earlier `/Od` and malformed-response-file
failures are retained as diagnostic evidence, not as passing results.

This closes the signed technology-result/message-code boundary as supporting
E2 evidence, but does not close the named SatisfyOrders E2 unit. Complete raw
message argument layouts, original transfer differential, and E3 full-turn /
original differential remain open.

Evidence:

- `tests/satisfy_orders_technology_binding_e2_probe.c`
- `compat/vc152/satisfy-orders-technology-binding-e2-native/`
- `runs/vmctl-vc152syoe11/SYO11-final-opt.TXT`
- `runs/vmctl-vc152syoe11/SYO11-final-opt-run.TXT`
- `runs/vmctl-vc152syoe11/SYO11-final-opt-STA.TXT`
- `runs/vmctl-vc152syoe11/SYO11-final-opt-CL.LOG`
- `runs/vmctl-vc152syoe11/SYO11-final-opt-LINK.LOG`
- `runs/vmctl-vc152syoe11/SYO11-final-opt.MAP`
- `reconstructed/src/satisfy_orders.c`
- `reconstructed/src/satisfy_technology.c`
- `reconstructed/src/itech_learn_a_tech.c`
- `reconstructed/include/core.h`
