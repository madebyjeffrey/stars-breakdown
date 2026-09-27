# PlanetWndProc E1 exact child layout

This packet closes the first named Step-5 evidence unit for `PlanetWndProc`:
the exact WM_CREATE child-control layout and its immediate USER/resource
services.

The typed reconstruction now records the raw creation sequence: three order
comboboxes, the four-character order edit with its real edit-procedure capture
and fake subclass installation, battle and ship comboboxes, the ship/fleet/
production listboxes, thirteen command buttons, and the report button. The
probe checks every child class, title token, style, 100/100/200 geometry,
height, parent, font, compressed-string ID, and resulting handle propagation.
It also checks the ship dropdown client-height query and the edit control's
`GetWindowLong(-4)`/`SetWindowLong(-4)` boundary.

Evidence:

- Host: `tests/planet_wnd_proc_layout_probe.c` reports
  `PLANET_WND_PROC_LAYOUT_E1_CHECKS=48 FAILURES=0 RESULT=PASS`.
- Native package: `compat/vc152/planet-wnd-proc-layout-e1/`.
- Native build: `runs/vmctl-vc152pwle/PWLESTA2.TXT`, with
  `COMPILE=PASS` and `LINK=PASS`.
- Native compile/link logs: `runs/vmctl-vc152pwle/PWLECL2.LOG` and
  `runs/vmctl-vc152pwle/PWLELINK2.LOG`.
- Native report: `runs/vmctl-vc152pwle/PWLE1.TXT` reports
  `PLANET_WND_PROC_LAYOUT_E1_CHECKS=48 FAILURES=0 RESULT=PASS`.
- The first package-copy diagnostic is retained as
  `runs/vmctl-vc152pwle/PWLESTA-fprintf-failure.TXT`,
  `PWLECL-fprintf-failure.LOG`, and `PWLELINK-fprintf-failure.LOG`.

The Win98 process exited 0 and no Program Error dialog appeared. This closes
E1 only. The remaining step-5 units are E2 packed transfer lookup and E3
message/focus/drawing integration; the separate Step-6 promotion item remains
after those two units.
