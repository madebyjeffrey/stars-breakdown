# DrawPlanetStats formatting boundary

The readable `DrawPlanetStats` reconstruction now exposes an optional typed
value-formatting path. Existing callers may continue to use the legacy
`draw_value` callback, while a caller that owns localization and fixed-point
formatting can provide `format_value` and `draw_formatted_value` together.
The source keeps the original seven-row order, right-edge alignment, and
hidden-tile behavior while making the value text an explicit boundary instead
of an undocumented callback assumption.

The strict host C89 probe reports:

`DRAW_PLANET_STATS_FORMAT_CHECKS=16 FAILURES=0 RESULT=PASS`

It checks the mineral-rate, resource, scanner, defense, best-defense, and
survival rows, including the zero mineral-rate and unavailable scanner/defense
cases. The existing 22-check source probe and 15-check packed-record probe
also remain passing after the interface extension.

The fresh VC++ 1.52c packet under
`compat/vc152/draw-planet-stats-format-e2/` compiled, linked, and ran in
`C:\DPSFE2`. The native report is:

`DRAW_PLANET_STATS_FORMAT_CHECKS=16 FAILURES=0 RESULT=PASS`

The host probe reports the same result. The process exited normally and the
post-run Win98 window list contained no Program Error dialog. A probe
accounting defect was corrected during the replay: the second-pass `77/0`
defense string is now an explicit check, so the reported 16 checks are real.
This closes the named E2 formatting boundary.

This closes only the E2 formatting boundary; complete packed planet/race
lookup, exact localized resource comparison, scanner/defense service, detailed
GDI, original differential, and promotion remain open.

Native evidence:

- `runs/vmctl-vc152dpsfe2/DPSFSTA.TXT`
- `runs/vmctl-vc152dpsfe2/DPSFCL.LOG`
- `runs/vmctl-vc152dpsfe2/DPSFLINK.LOG`
- `runs/vmctl-vc152dpsfe2/DPSFE2.TXT`
