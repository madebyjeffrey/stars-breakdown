# FDoCoolBattle packed-log/original checkpoint

The reconstructed battle lifecycle was checked through its raw serialized
log boundary. The probe validates the 14-byte battle header, six-byte move and
attack records, token coordinate and movement-state fields, appending a
battle after an existing log prefix, the per-battle record length excluding
that prefix, and the two-byte `0xffff` end marker when the persistent log has
no room.

Those cases ran in the existing fresh VC++ 1.52c/Win98 harness at `C:\CB`:

`COMPILE=PASS`  
`LINK=PASS`  
`F_DO_COOL_BATTLE_CHECKS=53 FAILURES=0 RESULT=PASS`

The post-run Win98 window list contained no Program Error dialog. The
original battle-report surface is preserved in
`runs/vmctl-vc152current/report-battles.bmp`; it establishes the consumer/UI
boundary but does not claim a populated original-versus-reconstructed battle
replay.

This advances `FDoCoolBattle` to Phase 2 step 5/6, not promotion. Packed
token/global mapping, global log compaction, exact movement geometry, exact
`FAttack`/damage behavior, remaining record cases, and full battle replay
remain open.

Evidence:

- `tests/f_do_cool_battle_source_probe.c`
- `compat/vc152/f-do-cool-battle/COOL.C`
- `compat/vc152/f-do-cool-battle/PROBE.C`
- `compat/vc152/f-do-cool-battle/CBNAT.C`
- `runs/vmctl-vc152fcb/CBSTA.TXT`
- `runs/vmctl-vc152fcb/CBCL.LOG`
- `runs/vmctl-vc152fcb/CBLINK.LOG`
- `runs/vmctl-vc152fcb/CBR.TXT`
- `runs/vmctl-vc152current/report-battles.bmp`

## Staged-log compaction checkpoint

The expanded probe adds an exact prefix-preservation case for a staged battle
append. It verifies that the existing five-byte persistent-log prefix remains
unchanged while the newly serialized 50-byte battle is copied immediately
after it, and that the report length remains 50 bytes rather than including
the prefix.

The fresh `C:\CB2` VC++ 1.52c / Win98 run reports
`F_DO_COOL_BATTLE_CHECKS=58 FAILURES=0 RESULT=PASS`; compile and link passed,
the process exited normally, and the guest window inventory contained no
Program Error dialog. This closes the staged-copy/prefix-preservation part of
the global log-compaction boundary. Packed token/global mapping, exact
`FAttack`/damage behavior, remaining record cases, full battle replay, and
promotion remain open.

Evidence:

- `tests/f_do_cool_battle_source_probe.c`
- `compat/vc152/f-do-cool-battle/PROBE.C`
- `compat/vc152/f-do-cool-battle/CBNAT.C`
- `compat/vc152/f-do-cool-battle/WINSTUB.EXE`
- `compat/vc152/f-do-cool-battle/RUNBLD2.BAT`
- `compat/vc152/f-do-cool-battle/RUN2.BAT`
- `runs/vmctl-vc152fcb2/CBSTA2.TXT`
- `runs/vmctl-vc152fcb2/CBCL2.LOG`
- `runs/vmctl-vc152fcb2/CBLINK2.LOG`
- `runs/vmctl-vc152fcb2/CBR2.TXT`
