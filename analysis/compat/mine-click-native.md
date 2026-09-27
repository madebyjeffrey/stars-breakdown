# MineClick native checkpoint

The fresh Win98 guest run is preserved in `runs/vmctl-vc152mcl/`.

- `MCLSTA.TXT`: `COMPILE=PASS`, `LINK=PASS`
- `MCLCL.LOG`: VC++ 1.52c compiled `MCL.C` and `PROBE.C`
- `MCLLINK.LOG`: segmented link completed with the retained `WINSTUB.EXE`
- `MCLR.TXT`: `MINE_CLICK_CHECKS=9 FAILURES=0 RESULT=PASS`
- the post-run guest window list showed no Program Error dialog

`MCL.C` was transferred directly from the reviewed
`reconstructed/src/mine_click.c` file. The native probe exercises prepared
popup routing for the hit-code families, mineral-graph threshold selection and
selected-index forwarding, mine-window invalidation, conditional scanner
invalidation, single- and double-click mineral-scan behavior, fleet-cargo
popup routing, and double-click suppression for unrelated hit codes.

This advances `MineClick` from Phase 2 step 3/6 to step 4/6. Packed
`GlobalPD`/planet/fleet/cargo/minefield/part records, localized popup
construction, derived mining/terraform arithmetic, original behavior evidence,
and full minefield integration remain open.

A repaired fresh run is preserved in `runs/vmctl-vc152mcl2/`. The package now
includes the exact current `MCL.C` source, a directory-first wrapper, and the
retained Win32 stub; VC1.52c compile/link pass, the native report is
`MINE_CLICK_CHECKS=9 FAILURES=0 RESULT=PASS`, redirected console output is
empty, and the post-run window list contains no Program Error dialog. This
strengthens the existing Phase 2 step 5/6 evidence without closing the packed
record, localization, derived-arithmetic, or full minefield integration scope.

The synchronized current probe was then rerun in fresh `runs/vmctl-vc152mcl3/`.
The native report is
`MINE_CLICK_CHECKS=25 FAILURES=0 RESULT=PASS`; the redirected console file is
empty and the post-run window list again contains no Program Error dialog.
