# ExecuteReportClick ER-E1 — live original GlobalPD differential

## Acceptance and outcome

ER-E1 covers the exact 22-byte original `GlobalPD` owner/part union at the
`ExecuteReportClick` popup boundary, including far pointers and bytes left
unchanged by later popup kinds. **Closed** for the seven popup families
reachable from the original Fleet and Planet Summary Reports. This does not
close live USER/GDI (ER-E2) or complete caller/layout coverage (ER-E3).

The original executable ran in an isolated `C:\GPLIVE1` copy of the retained
clean2400 fixture, not against the user's game files. A VC1.52c Win16 helper
used ToolHelp `ModuleFindName("STARS!")`, `GlobalEntryModule` for segment 37,
`GlobalHandleToSel`, and `MemoryRead` at offset `0x0B80`. It synchronously
sent report mouse-down, read the 22 bytes before popup release, and sent
mouse-up to dismiss the popup. The helper never wrote original module memory.
The `CAPTURE-*.TXT` returns identify the original module path and report
window, show `READ=22`, `STATUS=PASS`, and `POPUP_RELEASED=1`; each has a
corresponding 22-byte `.BIN` return.

| Original click | Kind | Captured `GlobalPD` hex | Return |
|---|---:|---|---|
| Planet starbase | 11 | `0B001200E71901000000000000000000000000000000` | `CAPTURE-STARBASE-DIFF` |
| Planet name, after starbase | 7 | `07000A00E71901000000000000000000000000000000` | `CAPTURE-PLANET-LIVE` |
| Resources, after planet | 12 | `0C000A0023001E000000000000000000000000000000` | `CAPTURE-RESOURCE-LIVE` |
| Defense, after resources | 9 | `0900008009139E183711000000000000000000000000` | `CAPTURE-DEFENSE-LIVE` |
| Fleet detail | 3 | `03008C019F1901000000FF0000000000000000000000` | `CAPTURE-FLEET-LIVE` |
| Ironium mineral | 1 | `01000000000001000000D50100004400000006000000` | `CAPTURE-MINERAL-LIVE` |
| Mine, after mineral | 8 | `08000A00B0040A001900000000004400000006000000` | `CAPTURE-MINE-LIVE` |
| Factory, after mine | 8 | `08000A00B0040A001900010000004400000006000000` | `CAPTURE-FACTORY-LIVE` |

All returns are under `runs/vmctl-gp-live-preflight/`. The original starbase
pointer is `19E7:0012`: the helper independently read the player-0 ship
definition base `19E7:0012` and selected planet's design index 0. The fleet
popup pointer is `199F:018C`. The defense popup copies all eight bytes
`00 80 09 13 9E 18 37 11`, not merely a 16-bit part ID. Resource bytes
encode planet ID 10, gross 35, adjusted 30. The planet click preserves the
preceding starbase far-pointer selector/value, while the installation clicks
preserve the preceding mineral's concentration 68 and estimate 6. These are
observable union-persistence requirements, not zero-initialized records.

Production `reconstructed/src/execute_report_click.c` now accepts an optional
caller-owned 22-byte `globalpd_raw` store. `popup_clear` starts from that store
and `show_popup` writes back the branch's exact overlay. The existing packed
fleet and ship-definition pointer tables supply far addresses; installation,
mineral, defense, and resource branches fill the original byte offsets.

`tests/execute_report_click_globalpd_probe.c` compares all eight complete
22-byte payloads against the original captures, including continuous
starbase → planet → resource → defense and mineral → mine → factory sequences.
Its other checks cover packed pointer formulas, bounds, and fallback. The
host probe passes **49/49**. `tests/test_globalpd_original_capture.py` also
checks all eight retained binary captures against their text reports and
expected hex. Fresh Win98 VC1.52c package `C:\GPDI7` compiled and linked:
`GPDI7STA.TXT` records `COMPILE=PASS`, `LINK=PASS`, and `GPDI7.TXT` records
`EXECUTE_REPORT_CLICK_GLOBALPD_CHECKS=49 FAILURES=0 RESULT=PASS`.
`GPDI7CL.LOG`, `GPDI7LINK.LOG`, map, object, and executable are retained; the
guest window inventory after execution showed no Program Error dialog. The
full host suite passed **541 tests, 153 expected skips**.
The passing full run used `unittest discover -v`; on this host, two `-q`
runs exited with signal-derived status 138 before completion. That runner
behavior remains unexplained and is not counted as a passing quiet run.

The live-original capture helper was itself compiled/linked in `C:\GPREAD5`;
its build status, compiler/linker logs, map, object, executable, capture
binaries/reports, and guest agent log were returned. The isolated game was
closed normally; the guest agent remained running.

## Remaining boundaries

- **ER-E2:** verify the actual Win98 USER/GDI selection, geometry, refresh,
  and control/message path against original observations.
- **ER-E3:** complete production report/click/popup caller and layout cases,
  including resource-exempt and modal behavior, with original result checks.
- **ER-P:** Step-6 promotion awaits E2 and E3 and final ledger synchronization.
