# ExecuteReportClick E1 fleet and starbase far-pointer table binding

Original `ExecuteReportClick` resolves a fleet report row through
`vlprgidFleet[row]` into the four-byte far-pointer array `rglpfl` before
writing the fleet popup's union words (raw export lines 37–72). The
starbase popup obtains the player's ship-definition table far pointer,
adds `(lStarbase & 0x0f) * 0x93` to its 16-bit offset, and writes the
resulting offset/selector pair into the same union (lines 160–170).

The production dispatcher now accepts optional packed versions of those
tables. With both fleet tables supplied, it reads the row ID and exact
four-byte pointer, rejects out-of-range rows or pointer entries, and no
longer trusts an unrelated caller-supplied far value. With the player
ship-definition pointer table supplied, it reads the design slot from
packed planet offset 44, forms the original stride-adjusted pointer with
16-bit offset wrap, and rejects a missing player entry. The previous
caller-supplied pointer fields remain fallbacks when raw tables are absent.

The strict-C89 and fresh VC++ 1.52c/Win98 `C:\GPDP4` matrices pass
**30/30 independently counted checks**. They cover original fallback
payloads, the corrected resource union, table-derived fleet pointer,
starbase stride/wrap, and invalid-table rejection. The guest build reports
`COMPILE=PASS LINK=PASS`; process exit is zero, report is
`EXECUTE_REPORT_CLICK_GLOBALPD_CHECKS=30 FAILURES=0 RESULT=PASS`, and no
Program Error window appears. The full host suite passes 540 tests with
153 expected skips. Package:
`compat/vc152/execute-report-click-pointer-tables-e1/`; returned logs,
map, object, executable, report, and agent log: `runs/vmctl-gpdp4/`.

This is instruction-derived table-formula evidence, not capture of the
original process's live pointer values. Full live fleet/starbase and
defense-part capture and differential across report click kinds remain
before ER-E1 can close. ER-E2/ER-E3 and promotion are separate.
