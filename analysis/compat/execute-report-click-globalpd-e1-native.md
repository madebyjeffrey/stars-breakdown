# ExecuteReportClick raw GlobalPD owner/part E1 native support

The readable `ExecuteReportClick` boundary now preserves the overlapping
22-byte `GlobalPD` / `_popupdata` representation alongside its typed popup
fields.  The dispatcher materializes the raw pointer words for fleet and
starbase popups, the eight-byte `_part` payload for defense popups, and the
resource part word used by the resource popup.  It also retains the five
overlapping `rgi` long views for callers that need the original union layout.

The strict host probe reports:

```text
EXECUTE_REPORT_CLICK_GLOBALPD_CHECKS=20 FAILURES=0 RESULT=PASS
```

A corrected fresh VC++ 1.52c / Win98 packet in
`compat/vc152/execute-report-click-globalpd-e1-native-r2/` compiled and linked
the current production source, header, and probe.  The guest report is:

```text
EXECUTE_REPORT_CLICK_GLOBALPD_CHECKS=20 FAILURES=0 RESULT=PASS
```

The process exited 0 and the post-run window list contained the game, agent,
and shell windows only; no Program Error dialog appeared.  This is supporting
native evidence for the owner/part union boundary.  It does not by itself
close the named Step-5 unit: the exact original pointer values, live USER/GDI
report caller behavior, complete report-layout coverage, and final original
differential remain open.

Evidence:

- `reconstructed/include/core.h`
- `reconstructed/src/execute_report_click.c`
- `tests/execute_report_click_globalpd_probe.c`
- `compat/vc152/execute-report-click-globalpd-e1-native-r2/`
- `runs/vmctl-vc152gpde2/GPDE2.TXT`
- `runs/vmctl-vc152gpde2/GPDE2STA.TXT`
- `runs/vmctl-vc152gpde2/GPDE2CL.LOG`
- `runs/vmctl-vc152gpde2/GPDE2LINK.LOG`
- `runs/vmctl-vc152gpde2/GPDE2.MAP`

The earlier `GPDE1` run is retained as superseded evidence; it passed the
same matrix before the starbase `iPlanVal=1` oracle was corrected.
