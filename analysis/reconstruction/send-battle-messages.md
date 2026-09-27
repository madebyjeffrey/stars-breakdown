# SendBattleMessages reconstruction

`SendBattleMessages` (`001f:9c0e-ae05`, 4,599 bytes) converts battle losses
and participant visibility into player-specific report messages. The typed C
implementation resolves the battle location, marks ship-design losses, emits
observer/spectator/missed-fleet notifications, classifies ship and design
outcomes, handles starbase loss and special-race cleanup, and emits the
corresponding technology notifications.

The source uses named report-event and loss-flag constants rather than assembly
or emitted-byte replay. Its three-case source/native probe covers observer and
spectator reporting, missed-fleet notification, two-side loss dispatch, and
starbase cleanup. The strict host C89 probe and native VC++ 1.52c/Win98 build,
link, and runtime report all pass: `CHECKS=3`, `FAILURES=0`, `RESULT=PASS` in
`runs/vmctl-vc152ew/SBMRES.TXT`.
