# SimpleNewGameDlg packed-player checkpoint

This checkpoint closes E1 for `SimpleNewGameDlg`. The original custom-race
success path copies the complete temporary 0xC0-byte player record into the
selected new-player slot. The typed reconstruction now exposes that boundary
as an optional packed-record adapter while retaining the callback-owned race
wizard and list services.

The strict host C89 probe reports 3,074/3,074 checks. It verifies every byte
of the 192-byte selected record and every byte of all 15 untouched neighboring
records. The fresh VC++ 1.52c/Win98 run in `runs/vmctl-vc152sngdp/` reports
compile/link success and
`SIMPLE_NEW_GAME_DLG_PACKED_E1_CHECKS=3074 FAILURES=0 RESULT=PASS`; the
post-run window inventory showed no Program Error dialog. The package and
CRLF wrappers are preserved under
`compat/vc152/simple-new-game-dlg-packed/`.

E2 resource/control geometry and E3 complete creation mutation remain open;
this packet advances the procedure from `0/3` to `1/3` named Step-5 evidence
units, not to promotion.
