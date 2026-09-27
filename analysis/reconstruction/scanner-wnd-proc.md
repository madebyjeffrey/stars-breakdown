# ScannerWndProc reconstruction

`ScannerWndProc` is the scanner child-window message router at NE
`000c:0032-0efb` (3,785 bytes). The reviewed source is
[`reconstructed/src/scanner_wnd_proc.c`](../../reconstructed/src/scanner_wnd_proc.c).

The reconstruction keeps the original message ordering and boundaries:

- creation initializes both scan origins to 1000; resize posts the current zoom
  command to the frame;
- paint acquires the paint DC, draws the scrollbar and scanner only when fleet
  data is available and drawing is not busy, then ends painting;
- character input preserves the `-`/other zoom clamps of -4 through +4 and
  routes `V` to the scanner crosshair operation;
- horizontal and vertical scroll commands preserve line/page/thumb selection,
  the `& 0xfffc` thumb alignment, scroll-position readback, and transformed
  delta dispatch;
- cursor hit testing distinguishes the scrollbar strip, scan-add mode,
  waypoint grab, and ordinary scanner mode;
- mouse routing keeps the scanner-area boundary, nearest-object radius,
  measuring-tape buttons, right-click popup selection, waypoint insertion/drag,
  scan-selection redraw, and main-object selection as explicit callbacks;
- activation and unrecognized messages retain the original active-window and
  default-procedure paths.

The callbacks are the typed boundary for Win16 handles, GDI, global object
tables, planet/fleet popup enumeration, and the existing scanner helpers. This
is semantic C source, not emitted machine code or an assembly wrapper.

Validation:

- host C89 probe: `SCANNER_WND_CHECKS=8 FAILURES=0 RESULT=PASS`;
- native VC1.52c compile/link: `runs/vmctl-vc152ex/scanner-wnd/SWSTA.TXT`;
- native Win98 execution: `runs/vmctl-vc152ex/scanner-wnd/SWR.TXT`, with
  `SCANNER_WND_CHECKS=8 FAILURES=0 RESULT=PASS`.
