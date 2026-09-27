# EstMineralsMined native checkpoint

The fresh Win98 guest run is preserved in `runs/vmctl-vc152emm/`.

- `EMMSTA.TXT`: `COMPILE=PASS`, `LINK=PASS`
- `EMMCL.LOG`: VC++ 1.52c compiled `EMM.C` and `PROBE.C`; it emitted two
  C4135 integral-conversion warnings at the existing concentration assignments
  in `EMM.C`, but no errors
- `EMMLINK.LOG`: segmented link completed with the retained `WINSTUB.EXE`
- `EMMR.TXT`: `EST_MINERALS_MINED_CHECKS=5 FAILURES=0 RESULT=PASS`
- the post-run guest window list showed no Program Error dialog

`EMM.C` was transferred directly from the reviewed
`reconstructed/src/est_minerals_mined.c` file. The native probe exercises the
invalid-planet sentinel, special-race concentration floor and fixed-point
rounding, apply/depletion callbacks, recursive eligible-fleet mine
aggregation/order consumption, and the typed `rgwtMin` weight/percentage path.

This advances `EstMineralsMined` from Phase 2 step 3/6 to step 4/6. Packed
planet/fleet layout, exact fixed-point differential behavior, and complete
mineral-record mutation remain open.
