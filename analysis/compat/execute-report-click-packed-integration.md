# ExecuteReportClick packed-record integration checkpoint

The production packed lookup seam maps recovered CodeView layouts into the
typed `ExecuteReportClick` path. It reads the 124-byte `_fleet`, 56-byte
`_planet`, and 14-byte `_btldata` records, then verifies popup output against
the 22-byte `_popupdata` union offsets for fleet detail, mine/factory,
mineral, resource, starbase, planet, and defense-part cells, including the
resource-exempt flag and the production/transfer modal guards.

The strict host C89 result is:

`EXECUTE_REPORT_CLICK_PACKED_CHECKS=34 FAILURES=0 RESULT=PASS`

The first two package attempts exposed a response-file defect: their linker
response files omitted the `.DEF` line, producing `L4021: no stack segment`
and the misleading Win98 `not enough space for arguments` dialog. After adding
the definitions-file line, a fresh `C:\EXINT3` build compiled and linked and
the native report was:

`EXECUTE_REPORT_CLICK_PACKED_CHECKS=34 FAILURES=0 RESULT=PASS`

The follow-up fresh `C:\EXINT4` package adds the starbase and planet popup
union branches, the defense-part payload, the resource-exempt bitfield path,
and the production/transfer modal guards. VC++ 1.52c compiled and linked the
package, and the native report returned
`EXECUTE_REPORT_CLICK_PACKED_CHECKS=34 FAILURES=0 RESULT=PASS`.

The process exited 0 and the post-run window list contained no Program Error
dialog. A separate smaller native layout probe also passed 16 checks in
`C:\EXLAY3`, confirming the raw record sizes and offsets independently.

The packed probe now exercises the production raw-record helpers directly;
the typed callbacks remain available as a fallback for callers without raw
records. The historical native packet still validates the equivalent adapter
boundary, but a fresh native packet containing this production path is
pending the guest-agent reconnect.

The procedure remains at Phase 2 step 5/6, not promotion. Exact original
`GlobalPD` owner/part pointer cases, live USER/GDI handles, and full report
caller/layout integration remain open.

Evidence:

- `tests/execute_report_click_packed_probe.c`
- `compat/vc152/execute-report-click-packed/`
- `analysis/debug/types.json`
- `analysis/decompiler/export/raw/1108_7cd6_ExecuteReportClick.c`
- `runs/vmctl-vc152expacked3/EXINTSTA.TXT`
- `runs/vmctl-vc152expacked3/EXINTCL.LOG`
- `runs/vmctl-vc152expacked3/EXINTLINK.LOG`
- `runs/vmctl-vc152expacked3/EXINT.TXT`
- `runs/vmctl-vc152exint4/EXINTSTA.TXT`
- `runs/vmctl-vc152exint4/EXINTCL.LOG`
- `runs/vmctl-vc152exint4/EXINTLINK.LOG`
- `runs/vmctl-vc152exint4/EXINT.TXT`
- `runs/vmctl-vc152exlayout3/EXLAYSTA.TXT`
- `runs/vmctl-vc152exlayout3/EXLAY.TXT`
