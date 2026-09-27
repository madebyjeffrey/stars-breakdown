# PlanetWndProc native checkpoint

The fresh Win98 guest run is preserved in `runs/vmctl-vc152pwp/`.

- `PWPSTA.TXT`: `COMPILE=PASS`, `LINK=PASS`
- `PWPCL.LOG`: VC++ 1.52c compilation completed for the current
  `planet_wnd_proc.c` source and probe
- `PWPLINK.LOG`: segmented link completed with `PWP.DEF`
- `PWPR.TXT`: `PLANET_WND_PROC_CHECKS=43 FAILURES=0 RESULT=PASS`
- `PWPR-CONSOLE.TXT`: empty; the process exited cleanly

The run used fresh `C:\PWP`, directory-first CRLF wrappers, a local
`WINSTUB.EXE`, and a native-only report path. The guest `windows` check after
execution showed no program-error dialog. This validates child creation,
paint/background/color routing, cursor and minimum-size handling, owner-draw
combo messages, keyboard adjacent-object navigation, ship/combo notifications,
transfer and adjacent-selection actions, ship-builder/production actions,
planet-list redraw/tutor dispatch, mouse clicks, activation, focus, and
default-procedure boundaries. Exact child layout, packed transfer lookup,
resources, edit subclass, and full drawing/focus integration remain outside
this checkpoint.

## E1 exact child-layout checkpoint

The follow-up package in `compat/vc152/planet-wnd-proc-layout-e1/` was run in
fresh `C:\PWLE` on the same connected `StarDev` guest. The native report is
`runs/vmctl-vc152pwle/PWLE1.TXT`:

`PLANET_WND_PROC_LAYOUT_E1_CHECKS=48 FAILURES=0 RESULT=PASS`

`PWLESTA2.TXT` records `COMPILE=PASS` and `LINK=PASS`; the process exited 0,
and the post-run window inventory contained no Program Error dialog. This
packet closes E1 exact child layout, resource-ID sequence, font assignment,
edit limit/subclass boundary, and typed handle propagation. The first stale
package-copy link diagnostic is retained beside the passing rerun and does not
represent a source failure.


## E2 packed-transfer checkpoint

The follow-up package in `compat/vc152/planet-wnd-proc-transfer-e2/` was run in
fresh `C:\PWTE2` on the connected `StarDev` guest. The native report is
`runs/vmctl-vc152pwte2/PWTE2.TXT`:

`PLANET_WND_PROC_TRANSFER_E2_CHECKS=12 FAILURES=0 RESULT=PASS`

`PWTESTA.TXT` records `COMPILE=PASS` and `LINK=PASS`; the process exited 0,
and the post-run window inventory contained no Program Error dialog. The
packet checks fleet-before-thing ordinal ordering, planet filtering, the
packed `_xfer` headers and copied payloads, TransferStuff mode 1, the
fleet-only `grobj == 2` selection boundary, and rejection of thing `grobj ==
8` by that selector.

## E3 message/focus/drawing checkpoint

The follow-up package in `compat/vc152/planet-wnd-proc-integration-e3/` was
run in fresh `C:\PWTE3` on the connected `StarDev` guest. The native report is
`runs/vmctl-vc152pwte3/PWTE3.TXT`:

`PLANET_WND_PROC_INTEGRATION_E3_CHECKS=10 FAILURES=0 RESULT=PASS`

`PWTE3STA.TXT` records `COMPILE=PASS` and `LINK=PASS`; the process exited 0,
and the post-run window inventory contained no Program Error dialog. This
closes the typed paint/erase, minimum-size, owner-draw, measure, cursor,
focus, and message-service boundary. E1, E2, and E3 are complete; the
procedure is ready for final Step-6 ledger promotion.
