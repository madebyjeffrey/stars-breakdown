# PlanetWndProc original Win98 integration checkpoint

This is the step-5 original-executable boundary for `PlanetWndProc`. In the
connected `StarDev` Win98 guest, the original `starsframe` displayed the
planet and fleet child areas together for `Oop Be Gone` and
`Stalwart Defender #5`.

The visible child-panel boundary included the planet summary and population,
environment gauges, mineral bars, production queue, starbase values, selected
fleet bitmap, fuel/cargo, fleet composition, and fleet waypoints. Clicking the
map changed the selected fleet and refreshed its bitmap, fuel/cargo, and
composition panels. No program-error dialog appeared.

Evidence:

- Original executable: `C:\STARS!\STARS.EXE` in Win98 on `StarDev`.
- Recovered boundary: `analysis/decompiler/export/raw/1048_0000_PlanetWndProc.c`.
- Native checkpoint: `runs/vmctl-vc152pwp/` and
  `analysis/compat/planet-wnd-proc-native.md`.
- Original screenshots:
  `runs/vmctl-vc152current/planet-frame-current.bmp`,
  `runs/vmctl-vc152current/planet-click.bmp`, and
  `runs/vmctl-vc152current/current-continue.bmp`.

This advances `PlanetWndProc` from 4/6 to 5/6. Exact child geometry, packed
transfer lookup, resource handles, real edit subclass behavior, and complete
drawing/focus integration remain open before promotion.

## Step-5 evidence-unit status

The exact child geometry/resource/edit-subclass concern is now closed as E1.
The host and native layout packet each report 48 checks with zero failures;
the native build and link pass in a fresh `C:\PWLE` package, the process exits
0, and the post-run window inventory shows no Program Error dialog. See
`analysis/compat/planet-wnd-proc-layout-e1.md` and `.json`.

The remaining work is now only the final promotion item:

- E2 — **closed**: recover and exercise the packed transfer lookup, including
  object kind, selected planet, and rejection/acceptance boundaries. The host
  and native reports each pass 12/12; see
  `analysis/compat/planet-wnd-proc-transfer-e2.md` and `.json`.
- E3 — **closed**: the host and native message/focus/drawing packet each pass
  10/10; see `analysis/compat/planet-wnd-proc-integration-e3.md` and `.json`.
- P — synchronize the review record, coverage, TODO5, TODO6, and reconstruction
  note after the complete E1/E2/E3 packet.
- P — synchronize the review record, coverage, TODO5, TODO6, and reconstruction
  note after E2 and E3 pass.
