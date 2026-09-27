# PlanetWndProc E2 packed transfer lookup

This packet closes the second named Step-5 evidence unit for `PlanetWndProc`:
the packed transfer lookup used by the Transfer and adjacent-fleet command
buttons.

The raw procedure calls `FLookupOrbitingXfer` with the selected planet, the
ship-combobox ordinal, a local `_xfer`, and `idSkip = -1`. The recovered
helper's packed boundary is explicit: fleet records are 124 bytes, thing
records are 18 bytes, and the transfer union stores its copied payload at
offset four with `id` at offset zero and `grobj` at offset two. The probe then
checks the actual PlanetWndProc consumer behavior: `grobj == 2` is accepted
by the adjacent-fleet selector, while thing `grobj == 8` is transferable but
not a fleet-selection target.

Evidence:

- Host: `tests/planet_wnd_proc_transfer_probe.c` reports
  `PLANET_WND_PROC_TRANSFER_E2_CHECKS=12 FAILURES=0 RESULT=PASS`.
- Native package: `compat/vc152/planet-wnd-proc-transfer-e2/`.
- Native build: `runs/vmctl-vc152pwte2/PWTESTA.TXT`, with
  `COMPILE=PASS` and `LINK=PASS`.
- Native compile/link logs: `runs/vmctl-vc152pwte2/PWTECL.LOG` and
  `runs/vmctl-vc152pwte2/PWTELINK.LOG`.
- Native report: `runs/vmctl-vc152pwte2/PWTE2.TXT` reports
  `PLANET_WND_PROC_TRANSFER_E2_CHECKS=12 FAILURES=0 RESULT=PASS`.

The Win98 process exited 0 and no Program Error dialog appeared. This closes
E2 only. E3 message/focus/drawing integration remains, followed by the
separate Step-6 ledger promotion item.
