# DrawPlanetShipList detail-dispatch checkpoint

Date: 2026-09-21

The readable `DrawPlanetShipList` source now exposes the fleet-versus-thing
detail branch through an optional typed `draw_details` callback. It receives
the detail rectangle, the resolved transfer kind, and the transfer index after
the combo lookup and before the three action buttons are positioned. The
legacy `fill_details` callback remains as a compatibility fallback. Detail
drawing is suppressed for unavailable transfers and for ship-mode rendering,
matching the recovered branch gates.

The strict-C89 host report is:

```text
DRAW_PLANET_SHIP_LIST_DETAILS_CHECKS=9 FAILURES=0 RESULT=PASS
```

The existing 19-check source and 18-check packed-record probes remain green.
This is host-source evidence only and closes **0** authoritative Step-5 units.
Native fleet/thing detail rendering, exact USER/GDI handles, localized
text/gauges, complete transfer integration, original differential, and
promotion remain open.

Evidence:

- `analysis/decompiler/export/raw/1048_377e_DrawPlanetShipList.c`
- `reconstructed/include/core.h`
- `reconstructed/src/draw_planet_ship_list.c`
- `tests/draw_planet_ship_list_details_probe.c`
- `tests/draw_planet_ship_list_packed_probe.c`
