# NewGameWizard native checkpoint

The fresh Win98 guest run is preserved in `runs/vmctl-vc152nwt/`.

- `NWSTA2.TXT`: `COMPILE=PASS`, `LINK=PASS`
- `NWCL2.LOG`: VC++ 1.52c compilation completed for the current
  `new_game_wizard.c` source and 30-case probe
- `NWLINK2.LOG`: segmented link completed with `NW.DEF`
- `NWR2.TXT`: `NEW_GAME_WIZARD_CHECKS=30 FAILURES=0 RESULT=PASS`

The run used fresh `C:\NWT`, CRLF wrappers, a local `WINSTUB.EXE`, and a
native-only report path. The result validates the typed wizard state-machine
slice; packed player storage, localized dialog resources, and full
`GenerateWorld` integration remain outside this checkpoint.

The packed-player E1 follow-up is preserved in `runs/vmctl-vc152nwp/`.
`NWPSTA2.TXT` and `NWPLINK2.LOG` report compile/link success, and
`NWPE1.TXT` reports `NEW_GAME_WIZARD_PACKED_E1_CHECKS=11 FAILURES=0
RESULT=PASS`. The focused adapter checks flags, salt, dead marker, names, and
preservation of unowned record bytes; no Program Error dialog appeared.

The shared creation E3 follow-up is preserved in `runs/vmctl-vc152ngint/`.
`NGINTSTA.TXT` reports compile/link success and `NGINTE3.TXT` reports
`NEW_GAME_CREATION_E3_CHECKS=11 FAILURES=0 RESULT=PASS`. It closes the
complete page-transition, player-commit, name, world-generation, and host-
dispatch boundary; no Program Error dialog appeared. The remaining E2
resource/control boundary is now closed by the fresh `C:\NWRES` run in
`runs/vmctl-vc152nwres/`: `NWRESSTA.TXT` reports compile/link success and
`NWRESE2.TXT` reports `NEW_GAME_WIZARD_RESOURCE_E2_CHECKS=366 FAILURES=0
RESULT=PASS` for all 45 controls across resources 0x186–0x188; no Program
Error dialog appeared.
