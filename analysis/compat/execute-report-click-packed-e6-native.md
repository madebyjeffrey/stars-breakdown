# ExecuteReportClick packed mineral-status E6 native replay

The production packed `_planet` bridge now preserves the original
`overlap_4` status word at offset `0x04`. The mineral popup uses bit 10 of
that word for the raw `GlobalPD.iPlanVal` field at popup offset `0x06`, as in
the original `ExecuteReportClick` branch.

The strict-C89 host packet reports:

```text
EXECUTE_REPORT_CLICK_PACKED_CHECKS=34 FAILURES=0 RESULT=PASS
```

A fresh VC++ 1.52c / Win98 packet in `C:\EXINT6` compiled and linked the
current production header/source/probe. Its native report is the same 34/34
pass. The process exited 0, and the final guest window inventory contained no
Program Error dialog.

Evidence:

- `reconstructed/include/core.h`
- `reconstructed/src/execute_report_click.c`
- `tests/execute_report_click_packed_probe.c`
- `compat/vc152/execute-report-click-packed-e6-native/`
- `runs/vmctl-vc152exint6/EXINT.TXT`
- `runs/vmctl-vc152exint6/EXINTSTA.TXT`
- `runs/vmctl-vc152exint6/EXINTCL.LOG`
- `runs/vmctl-vc152exint6/EXINTLINK.LOG`
- `runs/vmctl-vc152exint6/EXINT.MAP`

This is a supporting packed-record/native subcheck. Exact original pointer and
part ownership, live USER/GDI report behavior, complete report-layout caller
coverage, original differential, and promotion remain open.
