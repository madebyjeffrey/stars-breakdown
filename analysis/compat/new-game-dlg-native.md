# NewGameDlg native checkpoint

The fresh Win98 guest run is preserved in `runs/vmctl-vc152ngd/`.

- `NGDSTA2.TXT`: `COMPILE=PASS`, `LINK=PASS`
- `NGDCL2.LOG`: VC++ 1.52c compilation completed for the current
  `new_game_dlg.c` source and probe
- `NGDLINK2.LOG`: segmented link completed with `NGD.DEF`
- `NGDR.TXT`: `NEW_GAME_DLG_CHECKS=22 FAILURES=0 RESULT=PASS`
- `NGDR-CONSOLE.TXT`: empty; the process exited cleanly

The run used fresh `C:\NGD`, directory-first CRLF wrappers, a local
`WINSTUB.EXE`, and a native-only report path. The guest `windows` check after
execution showed no program-error dialog. The initial harness build also
preserved `NGDLINK.LOG`, which recorded an unresolved `_printf`; replacing
the probe's console formatting with the no-CRT-safe `puts`/`fputs` path fixed
the harness without changing the reconstructed procedure. This checkpoint
validates the three radio groups, packed option-bit boundary, paint/erase and
control-color paths, initialization/read-only disabling, help, race result,
no-selection fallback, name fallback, sticky cleanup, and accept/cancel
transitions. Packed game records, exact race-button tables, localized
resources, and full Win16 control integration remain outside this checkpoint.

The resource/geometry E2 follow-up is preserved in `runs/vmctl-vc152ngdr/`.
`NGDRSTA.TXT` and `NGDRLINK.LOG` report compile/link success, and
`NGDPE2.TXT` reports `NEW_GAME_DLG_RESOURCE_E2_CHECKS=30 FAILURES=0
RESULT=PASS`. It checks all 27 controls in the recovered 0x186 dialog
resource; no Program Error dialog appeared.

The packed-game E1 follow-up is preserved in `runs/vmctl-vc152ngdp/`.
`NGDPSTA.TXT` and `NGDPLINK.LOG` report compile/link success, and
`NGDPE1.TXT` reports `NEW_GAME_DLG_PACKED_E1_CHECKS=7 FAILURES=0
RESULT=PASS`. The adapter checks offsets 4, 6, 12, 16, and 32 while
preserving unowned bytes; no Program Error dialog appeared.

The shared creation E3 follow-up is preserved in `runs/vmctl-vc152ngint/`.
`NGINTSTA.TXT` reports compile/link success and `NGINTE3.TXT` reports
`NEW_GAME_CREATION_E3_CHECKS=11 FAILURES=0 RESULT=PASS`. It closes the
dialog-to-wizard mutation and original creation replay boundary; no Program
Error dialog appeared. `NewGameDlg` is ready for final ledger promotion.
