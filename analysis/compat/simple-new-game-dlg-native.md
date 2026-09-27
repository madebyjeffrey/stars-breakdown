# SimpleNewGameDlg native checkpoint

The fresh Win98 guest run is preserved in `runs/vmctl-vc152sngd/`.

- `SNGDSTA.TXT`: `COMPILE=PASS`, `LINK=PASS`
- `SNGDCL.LOG`: VC++ 1.52c compilation completed for the current
  `simple_new_game_dlg.c` source and probe
- `SNGDLINK.LOG`: segmented link completed with `SNGD.DEF`
- `SNGDR.TXT`: `SIMPLE_NEW_GAME_DLG_CHECKS=5 FAILURES=0 RESULT=PASS`
- `SNGDR-CONSOLE.TXT`: empty; the process exited cleanly

The run used fresh `C:\SNGD`, directory-first CRLF wrappers, a local
`WINSTUB.EXE`, and a native-only report path. The guest `windows` check after
execution showed no program-error dialog. This validates the recovered dialog
initialization, radio defaults and no-selection sentinel, list population,
accept/cancel paths, custom-race load/wizard/save flow, list rotation and
limit notification, focus restoration, and sticky-position boundary. Packed
player records, localized resources, exact frame geometry, and full Win16
control integration remain outside this checkpoint.

The packed-player E1 follow-up is preserved in `runs/vmctl-vc152sngdp/`.
`SNGDPSTA.TXT` reports compile/link success and `SNGDPE1.TXT` reports
`SIMPLE_NEW_GAME_DLG_PACKED_E1_CHECKS=3074 FAILURES=0 RESULT=PASS`. It checks
the complete 192-byte selected record copy and preservation of all neighboring
records; no Program Error dialog appeared.

The resource/geometry E2 follow-up is preserved in `runs/vmctl-vc152sngdr/`.
`SNGDRSTA.TXT` reports compile/link success and `SNGDRE2.TXT` reports
`SIMPLE_NEW_GAME_DLG_RESOURCE_E2_CHECKS=117 FAILURES=0 RESULT=PASS` for all
16 controls in resource 0xD1; no Program Error dialog appeared.

The creation/caller E3 follow-up is preserved in `runs/vmctl-vc152sngdi/`.
`SNGDISTA.TXT` reports compile/link success and `SNGDIE3.TXT` reports
`SIMPLE_NEW_GAME_DLG_INTEGRATION_E3_CHECKS=12 FAILURES=0 RESULT=PASS`; it
checks accepted radio/list state at the EndDialog boundary, custom-race list
mutation and focus restoration, and cancel preservation. No Program Error
dialog appeared, and this closes the final Step-5 unit before promotion.
