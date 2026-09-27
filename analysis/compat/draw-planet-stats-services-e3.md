# DrawPlanetStats service-resolution checkpoint

Date: 2026-09-21

`DrawPlanetStats` now exposes a typed post-tile `resolve_data` seam. The
callback receives the accepted packed planet view, the selected player, and
the typed seven-row data record. This makes the original race/resource,
scanner, defense, and survival service boundary explicit instead of requiring
those calculations to be hidden inside an untyped loader or pre-populated
caller state. The callback runs after tile acceptance and before value-row
rendering, matching the recovered control-flow boundary.

The strict-C89 host report is:

```text
DRAW_PLANET_STATS_SERVICES_CHECKS=12 FAILURES=0 RESULT=PASS
```

The probe checks selected-player forwarding, packed 56-byte service-word
decoding, mineral-rate decoding, all seven value-row consumers, and the
post-tile callback boundary. The existing source, lookup, packed-record, and
formatting probes remain green:

```text
DRAW_PLANET_STATS_CHECKS=22 FAILURES=0 RESULT=PASS
DRAW_PLANET_STATS_LOOKUP_CHECKS=9 FAILURES=0 RESULT=PASS
DRAW_PLANET_STATS_PACKED_CHECKS=15 FAILURES=0 RESULT=PASS
```

The packed decoder is now owned by production source as
`stars_draw_planet_stats_packed` in `reconstructed/src/draw_planet_stats.c`;
the packed probe calls that bridge directly. A fresh VC++ 1.52c/Win98 packet
under `C:\DPSSE3` now compiles and links the service-resolution source and
probe, and returns the same 12-check report. The redirected console is empty,
the process exits 0, and the post-run window inventory contains no Program
Error dialog.

This closes the named E3 service-resolution unit. Detailed GDI/original
differential and final promotion remain open; localized text remains part of
that broader acceptance boundary rather than being inferred from this service
packet.

Evidence:

- `analysis/decompiler/export/raw/1048_1716_DrawPlanetStats.c`
- `reconstructed/include/core.h`
- `reconstructed/src/draw_planet_stats.c`
- `tests/draw_planet_stats_services_probe.c`
- `tests/draw_planet_stats_lookup_probe.c`
- `tests/draw_planet_stats_packed_probe.c`
- `compat/vc152/draw-planet-stats-services-e3/`
- `runs/vmctl-vc152dpsse3/`
