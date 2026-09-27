# DoAutomitronAiTurn E2 packed first-order repair closure

The original Automitron branch at `1098:1225–129E` requires nonzero ship
slot 6, `cord == 1`, and an empty low kind nibble in the first `PLORD` order.
It copies the selected fleet, writes kind 6 at order offset 10 and the two
`0x0005` words at offsets 12 and 14, then calls `FLookupFleet`. It does not
write PLORD header byte 3 (`iordMac`) or later orders. This is directly
visible in `analysis/decompiler/export/raw/1098_01e0.asm`; the structured C
listing's apparent `else` contradiction is a decompiler control-flow artifact.

The production-source probe seeds three orders and checks the exact entire
PLORD buffer, including unchanged count and queued orders. It also checks
slot-6 absence, `cord != 1`, an already-ordered first record, foreign owner,
repair/plan/visit ordering, and callback rejection. A follow-up source audit
found that the original repair executes in the owned-fleet loop, independent
of which visitor callback is installed. The source now preserves that behavior
with typed, legacy, or no visitor. Strict host C89 reports
`AUTOMITRON_MULTI_ORDER_REPAIR_CHECKS=22 FAILURES=0 RESULT=PASS`.

Fresh `C:\AUTRE3` compiled and linked the corrected
`do_automitron_ai_turn.c` and shared `ai_plord.c` under VC++ 1.52c.
`BLDSTAT.TXT` reports `COMPILE=PASS` and `LINK=PASS`; the process exited 0 and
native `AUTREP.TXT` reports the same 22/22 result. The post-run window
inventory contained no Program Error dialog. The current package is
`compat/vc152/automitron-multi-order-repair-e2-r2/`; build logs, map,
object, executable, report, and agent log are preserved in
`runs/vmctl-autre3/`. The prior 17-check return in `runs/vmctl-autre2/` is
retained as the initial E2 closure packet.

This closes only Automitron Step-5 E2. E1 colonizer/mineral handling, E3
route and broader packed-order integration, E4 original-behavior coverage,
and Step-6 promotion remain. The evidence is instruction-derived for this
specific branch, not a complete original-turn differential.
