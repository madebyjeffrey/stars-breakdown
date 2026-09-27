# FDoCoolBattle native checkpoint

The fresh Win98 guest run is preserved in `runs/vmctl-vc152fcb/`.

- `CBSTA.TXT`: `COMPILE=PASS`, `LINK=PASS`
- `CBCL.LOG`: VC++ 1.52c compiled the native wrapper containing the exact
  reviewed `COOL.C` and `PROBE.C` source
- `CBLINK.LOG`: segmented link completed with the retained `WINSTUB.EXE`
- `CBR.TXT`: `F_DO_COOL_BATTLE_CHECKS=53 FAILURES=0 RESULT=PASS`
- the post-run guest window list showed no Program Error dialog

The transferred `COOL.C` is byte-identical to
`reconstructed/src/f_do_cool_battle.c`, and `PROBE.C` is byte-identical to
`tests/f_do_cool_battle_source_probe.c`. The package also carries the exact
reviewed board-initialization and movement helpers plus the speed-rounding
boundary required by the source.

The probe covers battle-log allocation and serialization, initialization and
cleanup, board-derived token construction, round-zero shield behavior,
movement and initiative dispatch, attack termination, active-player filtering,
defending-starbase participation, report summaries, and overflow handling.

This advances `FDoCoolBattle` from Phase 2 step 3/6 to step 4/6. Packed
token/global-state mapping, exact damage behavior, full battle-log integration,
and remaining record cases remain open.
