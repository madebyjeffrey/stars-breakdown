# ExecuteReportClick ER-E3 — Win16 research-share arithmetic

This is a source-fidelity and host-integration milestone, **not** ER-E3
closure. The original `ExecuteReportClick` decompilation at
`analysis/decompiler/export/raw/1108_7cd6_ExecuteReportClick.c` calls
`MULDIV(resources, (char)pctResearch, 100)` in the non-exempt resource popup
branch. Its production reconstruction had used plain C
`resources * percent / 100`, which truncates half values and can overflow
before division when compiled by the 16-bit VC1.52c toolchain.

Microsoft's [historical Win16 `MulDiv` account](https://devblogs.microsoft.com/oldnewthing/20120514-00/?p=7633)
describes a 32-bit intermediate, rounding to the nearest integer, and
signed-16 saturation on overflow. `reconstructed/src/execute_report_click.c`
now implements that fixed-denominator behavior explicitly, including negative
half rounding. The original resource-exempt bit still bypasses the research
deduction, as shown by the original branch structure.

The production `ReportDlg` → `ExecuteReportClick` host chain checks six
additional cases: `21×50%` rounds to 11 and leaves 10, `1000×50%` leaves
500 without a 16-bit product wrap, `32767×100%` leaves zero, the same amount
with the exemption bit stays 32767, `32767×127%` saturates the deduction to
32767, and `21×−50%` rounds −10.5 to −11 and leaves 32. The full chain is
`EXECUTE_REPORT_CLICK_REPORT_CHAIN_CHECKS=33 FAILURES=0 RESULT=PASS`.

Fresh package `compat/vc152/execute-report-click-live-user-e2-r4/` targets
`C:\ERUSER4`. After the four original USER/GDI popup cases, its real Win16
report-message harness checks those six resource values and the 22-byte
`GlobalPD` overlay with the popup window suppressed only for the new cases
(their dimensions were not observed in the original). It has **not** yet been
transferred, compiled, or run in the guest. The earlier `ERUSER3` packet is
superseded for this arithmetic source change. An original-executable
resource-exempt result and the complete report-layout/scroll boundary still
remain open.
