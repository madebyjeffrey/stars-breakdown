# PlanetWndProc

`PlanetWndProc` is represented as a typed message dispatcher in
`reconstructed/src/planet_wnd_proc.c`. It preserves child-control creation,
paint/background and color handling, cursor hit-testing, minimum-size and
owner-draw messages, keyboard adjacent-object navigation, ship-command and
combo notifications, adjacent planet/starbase selection, transfer actions,
ship builder and production actions, planet-list redraw/tutor dispatch,
mouse-click routing, activation state, focus, and default-procedure
boundaries.

The exact Win16 child-window layout, list/combo resources, and real edit
subclass are now represented by a typed creation-layout helper and have a
48-check host/native E1 packet. Packed transfer lookup is covered by a typed
callback contract and a 12-check host/native E2 packet; the remaining
drawing/focus helpers remain callback-owned.
The original `starsframe` boundary has also been exercised in Win98: the
planet and fleet child panels were visible together, and a map selection
refreshed the selected fleet controls without a program-error dialog. See
`analysis/compat/planet-wnd-proc-integration.md`.
The host probe covers the routed message and action paths. Fresh VC++ 1.52c /
Win98 checkpoints now cover the 43-check dispatcher packet, the 48-check
exact-layout packet, the 12-check packed-transfer packet, and the 10-check
message/focus/drawing packet without runtime error dialogs. E1, E2, and E3
are closed; only Step 6 ledger synchronization remains before promotion.

An additional host integration probe routes `WM_PAINT` through the parent
dispatcher into the typed `DrawPlanetStats` child renderer. The `BPE`
lifecycle and eight checks pass, including packed stats loading, all seven
value rows, references, separators, and selected field values. This
strengthens the already-promoted parent paint evidence without changing its
Step-6 status. See
`analysis/compat/planet-stats-wndproc-integration-e3.{md,json}`.
