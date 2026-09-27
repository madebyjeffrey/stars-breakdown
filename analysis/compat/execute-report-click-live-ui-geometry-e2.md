# ExecuteReportClick ER-E2 — live original popup geometry milestone

This is **supporting ER-E2 evidence, not closure**. The previous native
`ExecuteReportClick -> Popup -> DrawPopup` replay used a top-left popup
placement rule that was incompatible with original instructions. Its test
coordinates happened to clamp to the same result, hiding the error.

## Live original Win98 observation

The original `C:\GPLIVE1\STARS.EXE` ran against an isolated copy of the
clean2400 player fixture. Fresh VC1.52c helper `C:\GPUI2\CAPGPD.EXE` sent
report clicks, used ToolHelp to read `GlobalPD`, and queried USER window
state before releasing each popup. The Planet Summary Report window was
`(340,312)-(940,712)`, with client-to-screen offset `(344,335)` for the
tested cells.

| Cell | Client click | Popup kind | Original popup rectangle | Client size |
|---|---:|---:|---:|---:|
| Planet name | `(265,26)` | 7 | `(396,281)-(609,361)` | 211×78 |
| Resources | `(406,26)` | 12 | `(553,281)-(750,361)` | 195×78 |
| Mine | `(160,26)` | 8 | `(305,281)-(504,361)` | 197×78 |
| Factory | `(205,26)` | 8 | `(356,281)-(549,361)` | 191×78 |

Each popup's **bottom-right** coincided with the clicked screen point.
Their window size was the measured client size plus two pixels per axis.
Every capture reported class `starspopup`, parent equal to the report HWND,
visible window, popup-owned mouse capture and focus, effective style
`0x94800000`, 22-byte `GlobalPD` read, successful popup release, and no
Program Error. The additional starbase capture showed a large clamped popup
rectangle `(194,0)-(534,454)` at the left/top screen edge. Reports,
binary payloads, screenshot, helper build logs/map/object/executable, and
guest agent log are retained in `runs/vmctl-gp-ui-r2/`.

## Source correction and differential

The original `Popup` instructions subtract measured width and height from
the client-to-screen click point **before** clamping. The production
`reconstructed/src/popup.c` now does the same. The typed `CreateWindow`
callback carries the full 32-bit original style argument `0x90800000`
instead of a truncated 16-bit zero; the observed effective window style is
recorded above but is not assumed to equal the raw API argument.

An interior, non-clamped host case was added to
`tests/popup_source_probe.c`, and the 148-case Popup E2 oracle now uses the
original bottom-right anchoring. `tests/execute_report_click_popup_integration_probe.c`
feeds the four observed report clicks through the production
`ExecuteReportClick -> Popup -> DrawPopup` chain, comparing rectangles,
semantic payloads, report parent, raw style argument, font message, capture,
paint branch, and report-DC symmetry. Strict-C89 host replays pass:

- `POPUP_CHECKS=31 FAILURES=0 RESULT=PASS`
- `POPUP_PACKED_CHECKS=22 FAILURES=0 RESULT=PASS`
- `POPUP_E2_CHECKS=148 FAILURES=0 RESULT=PASS`
- `EXECUTE_REPORT_CLICK_POPUP_INTEGRATION_CHECKS=86 FAILURES=0 RESULT=PASS`

Fresh VC1.52c/Win98 `C:\ERUI2` compiled/linked and passed the 86-case
production caller replay. Fresh `C:\PUIE4` compiled/linked and passed the
148-case Popup E2 matrix against the corrected source. Their reports,
compiler/linker logs, maps, objects, executables, and a post-run window
inventory without Program Error are retained in `runs/vmctl-gp-ui-r2/`.
These revalidations preserve Popup's already-promoted 6/6 status after the
source correction; the older geometry-only native results are historical.
The full host suite also passed **541 tests, 153 expected skips** using
`unittest discover -v`.

## Still needed for ER-E2

This matrix observes original USER window placement and the reconstructed
callback arguments, but the reconstruction's callback does **not** itself
create and paint a real Win16 popup. ER-E2 still requires a live native
USER/GDI bridge that checks actual window creation/style, report selection,
paint/refresh behavior, and popup teardown/capture release against the
original. ER-E3 full caller/layout branch acceptance also remains separate.
