# DrawPlanetProduction native checkpoint

## Current Step-5 status

The localized production-label unit is closed in fresh `C:\DPRES` evidence.
The native probe checks original IDs/text `0x32e` / `Production`, `0x218` /
`Completion: `, and `0x4e9` / `Route to`; compile/link passed and
`runs/vmctl-vc152dpres/DPRES.TXT` reports
`DRAW_PLANET_PRODUCTION_RESOURCE_CHECKS=3 FAILURES=0 RESULT=PASS`. The final
window list contained no Program Error dialog. The exact display-service
geometry/control boundary is closed below; only the remaining original
differential work remains open.

The fresh Win98 guest run is preserved in `runs/vmctl-vc152dpro/`.

- `DPPSTA.TXT`: `COMPILE=PASS`, `LINK=PASS`
- `DPPCL.LOG`: VC++ 1.52c compilation completed for the current
  `draw_planet_production.c` source and probe; the warnings are the expected
  signed/unsigned conversions in the recovered sentinel rectangle code
- `DPPLINK.LOG`: segmented link completed with `DPP.DEF`
- `DPR.TXT`: `DRAW_PLANET_PRODUCTION_CHECKS=9 FAILURES=0 RESULT=PASS`
- `DPP-CONSOLE.TXT`: empty; the process exited cleanly

The run used fresh `C:\DPROD`, directory-first CRLF wrappers, a local
`WINSTUB.EXE`, and a native-only report path. The guest `windows` check after
execution showed no program-error dialog. This validates tile reset and hidden
cleanup, list sizing/placement, ETA and planet-name rows, production-button
placement and visibility, queue-driven enablement, and the action reference
rectangle. Packed planet/production records and the exact display-service
geometry are covered by the separate checkpoints below; the localized
production-label resource boundary is closed separately above.

The packed production-record unit is closed separately in fresh
`runs/vmctl-vc152dprec/` evidence. The host/native record probe reports
`DRAW_PLANET_PRODUCTION_RECORD_CHECKS=16 FAILURES=0 RESULT=PASS` for the linked
`PLPROD` header, two variable production records, packed kind/item/count
decoding, listbox callback flow, and direct-item formatting. Compile/link passed
and the final window list contained no Program Error dialog. Exact geometry,
detailed GDI/control integration, and original differential coverage remain.

## E3 service and geometry checkpoint

The fresh `C:\DPSV` harness closes the exact display-service boundary. The
14-check host/native probe verifies the pre/post list client-rectangle reads,
full and compact row heights, the Win16 resize flags (`0x14` and `0x15`),
list and button placement, localized label order, queue-driven enablement,
action-button kind, reference rectangle, and tile-reset handling. The native
report is `DRAW_PLANET_PRODUCTION_SERVICE_CHECKS=14 FAILURES=0 RESULT=PASS`;
compile/link passed, the process exited 0, and the final window inventory had
no Program Error dialog. Artifacts are preserved in
`runs/vmctl-vc152dpsv/` and `compat/vc152/draw-planet-production-service/`.

This closes E3. The original-executable differential is recorded in
`analysis/compat/draw-planet-production-packed-integration.md`: selected
`Oop Be Gone`, empty production queue, queue-dialog contents, and
frame-refresh/fleet-selection captures all agree without a Program Error.
`DrawPlanetProduction` is now `4/4` evidence units; only final ledger
promotion remains.
