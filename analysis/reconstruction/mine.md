# Mineral window procedure recovery

`reconstructed/src/mine_wnd_proc.c` is a genuine C reconstruction of
MineWndProc (NE 6:0000–0409). It models the Win16 child creation and font
setup, framed title painting with the original width limit, selection arrow
and survey drawing, background and control-color handling, cursor hit testing,
checkbox state persistence through the record-memory log, and popup-safe
mouse dispatch. Window, GDI, and game actions are explicit typed callbacks;
the implementation does not replay emitted bytes.

The source probe covers all of those message families in 9 checks. The source
also compiles cleanly with strict host C89. VC1.52c under the Win98 StarDev
environment compiled and linked it, and the native probe passed all 9 checks
with zero failures (`runs/vmctl-vc152ew/mine-wnd-proc/MWPRES.TXT`). This is
recorded as touched semantic source rather than a formal ledger promotion
until the live mineral-window globals and caller integration are reviewed.
