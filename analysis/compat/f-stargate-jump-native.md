# FStargateJump native checkpoint

The final fresh Win98 guest run is preserved in `runs/vmctl-vc152sgj/`.

- `SGJSTA.TXT`: `COMPILE=PASS`, `LINK=PASS`
- `SGJCL.LOG`: VC++ 1.52c compiled `SGJ.C` and `PROBE.C` without warnings
- `SGJLINK.LOG`: segmented link completed with the retained `WINSTUB.EXE`
- `SGJR.TXT` and `SGJR2.TXT`: `F_STARGATE_JUMP_CHECKS=4 FAILURES=0 RESULT=PASS`
- `SGJ-CONSOLE.TXT`: zero bytes after redirected execution

`SGJ.C` was transferred directly from the reviewed
`reconstructed/src/f_stargate_jump.c` file. The native probe exercises the
damage-abort message envelope, total cargo loss and empty-fleet handling,
surviving cargo with stargate flag and balance-transfer behavior, and the
fixed-point cargo-state damage boundary.

This advances `FStargateJump` from Phase 2 step 3/6 to step 4/6. Packed
original fleet layout, full game service tables, original damage differential
cases, and complete cargo integration remain open.
