# FStargateJump reconstruction

`reconstructed/src/f_stargate_jump.c` is a source-level reconstruction of
`FStargateJump` (`0011:0cfe-152d`, 2,095 bytes), not a byte or assembly
replay. It models the original 16 cargo slots with typed records and keeps
the external game services explicit through callbacks.

The implementation covers the three damage-service outcomes (`-2` abort,
`-1` failure, and ordinary cargo processing), the stargate flag update, full
cargo loss, per-unit random cargo loss, fixed-point cargo-state damage, loss
severity messages, empty-fleet handling, and the surviving-fleet cargo-balance
handoff. The cargo-state update preserves the original intermediate truncation
between the weighted average and its final ship-weight scale. Cargo-state
products, weighted averages, and scale factors use explicit 32-bit integer
multiply/divide steps with the original truncation boundaries rather than host
floating point, and the source
probe exercises that boundary with deterministic service doubles in addition to
the abort, total-loss, and surviving-cargo paths.
The message adapter also exposes the separate raw fleet ID carried alongside
the flagged fleet reference, and the probe checks the complete abort and empty
message envelope.

Host validation: strict C89 compilation and `F_STARGATE_JUMP_SOURCE=PASS`.
Native VC1.52c/Win98 validation is complete at the focused procedure boundary.
A fresh run in `StarDev` used the exact current source in guest directory
`C:\SGJ`; VC++ 1.52c reported `COMPILE=PASS` and `LINK=PASS`, and the Win98
report was `F_STARGATE_JUMP_CHECKS=4 FAILURES=0 RESULT=PASS`. The redirected
console file was empty and the native window list showed no program-error
dialog.

Packed original fleet layout, full game service tables, original damage
differential cases, and complete cargo integration remain pending.

A second native harness now binds the source to the recovered packed fleet ID,
player, flags, planet, cargo-count, and cargo-state fields, together with the
ship-design mass and ship-weight fields. It passes two integration checks for
the abort message envelope and a surviving-cargo jump with destination flags,
cargo-state mutation, and cargo-balance transfer. The original Fleet Summary
Report / Fuel & Cargo surface shows the fleet identity and cargo state without
a program-error dialog. This advances the procedure to Phase 2 step 5/6; full
original damage-service differential coverage and game-service cargo
integration remain open.

The packed integration harness was then expanded to seven deterministic native
checks in a fresh `C:\SGJINT2` directory. It covers the damage-failure
envelope, inactive and total-loss empty-fleet paths, surviving-cargo state and
loss-message write-back, the race-stat loss exemption, and two active cargo
slots with independent design weights. VC1.52c compilation and linking passed,
the Win98 report was
`F_STARGATE_JUMP_INTEGRATION_CHECKS=7 FAILURES=0 RESULT=PASS`, and the post-run
window list contained no Program Error dialog. This strengthens, but does not
close, Phase 2 step 5/6; the original damage-service differential and full
game-service cargo integration remain open.

The packed/native harness was expanded again in fresh `C:\SGJINT3`. Ten
strict-C89 and VC++ 1.52c/Win98 checks now cover the damage callback's source,
destination, distance, and design-mass arguments, zero cargo-damage state,
destination transfer fields, and a 32-bit large-loss message amount across
sixteen cargo slots. Compile and link passed, the native report was
`F_STARGATE_JUMP_INTEGRATION_CHECKS=10 FAILURES=0 RESULT=PASS`, and the final
window list contained no Program Error dialog. This narrows the remaining
step-5 boundary to full original damage-service differential cases and the
real game-service cargo transfer comparison; it does not promote the row.

The original damage-service unit is now closed in fresh `C:\SGJINT4`. The
strict host and VC++ 1.52c/Win98 harnesses each pass
`F_STARGATE_JUMP_INTEGRATION_CHECKS=2310 FAILURES=0 RESULT=PASS`, including an
independent matrix over eight source/destination armor records and their
distance and defense/weight thresholds. Compile and link passed, and the
final guest window list contains no Program Error dialog. This closes E1 in
TODO6.

The E2 service boundary is now closed in fresh `C:\SGJINT5`. The raw
assembly shows that `FStargateJump` keeps the surviving local source record,
constructs a zeroed lost-fleet record with `0x0407` flags and lost ship counts,
calls `FleetTransferCargoBalance`, and copies the surviving source back to the
live fleet. The reconstruction now preserves that order instead of replacing
the live fleet with the temporary destination record. Its typed fleet model
also exposes the five packed resource dwords at offset 76. A real adapter
invokes the reconstructed balance service and checks source/destination ship
counts, flags, source preservation, and resource redistribution (fuel 30 to
source 23 plus lost record 7). Strict host C89 and VC++ 1.52c/Win98 each pass
`F_STARGATE_JUMP_INTEGRATION_CHECKS=2311 FAILURES=0 RESULT=PASS`; compile and
link pass, and the final window list contains no Program Error dialog. E2 is
closed and `FStargateJump` is promoted to Phase 2 step 6/6.
