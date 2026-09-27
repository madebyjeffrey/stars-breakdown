# ExecuteReportClick ER-E2 — real Win16 USER/GDI lifecycle

## Completed lifecycle milestone

A fresh VC1.52c/Win98 harness in `C:\ERUSER1` compiled and linked the current
production `ExecuteReportClick`, `Popup`, `DrawPopup`, and `PopupWndProc`
sources. Unlike the earlier callback-only geometry matrix, it registered
real Win16 `starsreport` and `starspopup` window classes and used actual
USER/GDI calls for `CreateWindow`, `GetWindowRect`, `GetClientRect`,
`ClientToScreen`, `GetWindowLong`, `SetCapture`, `BeginPaint`, `TextOut`,
`SetPixel`, `EndPaint`, `DestroyWindow`, and `ReleaseCapture`.

The report window matched the isolated original `(340,312)-(940,712)` with
client origin `(344,335)`. Four production report-cell clicks produced real
popup windows matching the previously captured original rectangles and
effective `0x94800000` style:

| Cell | Original and harness rectangle | Native result |
|---|---:|---|
| Planet | `(396,281)-(609,361)` | match |
| Resources | `(553,281)-(750,361)` | match |
| Mine | `(305,281)-(504,361)` | match |
| Factory | `(356,281)-(549,361)` | match |

The native report records matching `starspopup` class, report parent,
visibility, popup-owned capture, selected planet, invalidation callback,
report-DC symmetry, real `WM_PAINT`/GDI output, real destruction, and capture
release for each case: `CHECKS=54 FAILURES=0 RESULT=PASS`. Build status,
compiler/linker logs, and the report are retained in `runs/vmctl-eruser-r1/`.
The guest window inventory after completion contained no Program Error.

The harness supplies the four measured client sizes from independent
original captures; it does not claim original localized text measurement or
pixel-identical popup content. It proves the reconstructed lifecycle with
real Win16 USER/GDI, not merely callback trace predictions.

## Remaining ER-E2 boundary

The first harness invoked the production `ExecuteReportClick` function
directly from its test driver. ER-E2 remains open until the production
`ReportDlg` mouse hit-test/message dispatch and actual report refresh are
validated in the same live Win16 path, with the original observations as
oracle. ER-E3 complete report-layout/caller branches remain separate.

The expanded `ReportDlg` message/refresh harness was transferred to
`C:\\ERUSER2`, but its process-status request received no reply before the
agent connection was lost. Neither build nor run status has been returned;
do not reuse that guest directory or infer a result. A fresh
[`ERUSER3` package](../../compat/vc152/execute-report-click-live-user-e2-r3/)
was prepared with a DOS/ASCII/CRLF build wrapper that records compile and
link outcomes separately and refuses to overwrite an existing `BLDSTAT.TXT`.
It was **not** transferred. The source was subsequently corrected for Win16
`MulDiv` resource-share behavior; the current fresh packet is
[`ERUSER4`](../../compat/vc152/execute-report-click-live-user-e2-r4/),
which adds six native resource arithmetic checks after the four original
USER/GDI lifecycle cases. The host controller's local protocol self-test
passes, but this does not establish a live guest connection. Once STARSAGT
reconnects, first retrieve any available `ERUSER2` status/logs, then use the
current `ERUSER4` packet if a new build is needed.
