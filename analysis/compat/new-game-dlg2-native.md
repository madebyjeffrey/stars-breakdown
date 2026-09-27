# NewGameDlg2 native checkpoint

The fresh Win98 guest run is preserved in `runs/vmctl-vc152nd2/`.

- `NWD2STA.TXT`: `COMPILE=PASS`, `LINK=PASS`
- `NWD2CL.LOG`: VC++ 1.52c compilation completed for the current
  `new_game_dlg2.c` source and 29-case probe
- `NWD2LINK.LOG`: segmented link completed with `NWD2.DEF`
- `NWD2R.TXT`: `NEW_GAME_DLG2_CHECKS=29 FAILURES=0 RESULT=PASS`

The run used fresh `C:\ND2`, CRLF wrappers, a local `WINSTUB.EXE`, and a
native-only report path. This validates the typed dialog state and
control-color slice; menu/resource integration, packed race storage, and
exact Win16 geometry remain outside this checkpoint.

The E2 resource follow-up is preserved in `runs/vmctl-vc152nd2res2/`.
`NWD2RSTA.TXT` reports compile/link success and `NWD2RE2.TXT` reports
`NEW_GAME_DLG2_RESOURCE_E2_CHECKS=40 FAILURES=0 RESULT=PASS` for all five
controls in resource 0x187; no Program Error dialog appeared. The first
stale-header compile is retained in `runs/vmctl-vc152nd2res/` as diagnostic
evidence, not as a source result.

The E3 transition follow-up is preserved in `runs/vmctl-vc152nd2int/`.
`NWD2ISTA.TXT` reports compile/link success and `NWD2IE3.TXT` reports
`NEW_GAME_DLG2_INTEGRATION_E3_CHECKS=9 FAILURES=0 RESULT=PASS`; no Program
Error dialog appeared. Together with the original three-page replay, this
closes the transition boundary; packed race/password storage remains.

The E1 packed follow-up is preserved in `runs/vmctl-vc152nd2pack/`.
`NWD2PSTA.TXT` reports compile/link success and `NWD2PE1.TXT` reports
`NEW_GAME_DLG2_PACKED_E1_CHECKS=1238 FAILURES=0 RESULT=PASS`; it checks the
Create/Open/Edit record and race-file commits, password rejection, and
neighbor preservation. No Program Error dialog appeared, and the procedure
is promoted at Phase 2 step 6/6.
