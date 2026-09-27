# DrawResearchDlg

`DrawResearchDlg` is a large research-window renderer.  Its original body
combines USER/GDI calls, player-record offsets, component-list traversal, and
fixed-point research-cost arithmetic.  The reconstruction keeps the dialog
as a sequence of typed phases: heading and future-technology rows, current
technology/cost display, totals and historical values, percentage/budget
lines, and spin-button geometry.  Record lookup, localization, text metrics,
and drawing remain explicit callbacks rather than packed global accesses.

The source probe checks DC ownership, frame/text/right-aligned output,
technology-list traversal, cost calculation, spin buttons, and the caller
owned-DC path.  It compiles as strict host C89 and is also exercised through
the VC++ 1.52c Win98 probe package.
