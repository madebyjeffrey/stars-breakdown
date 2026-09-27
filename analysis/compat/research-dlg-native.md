# ResearchDlg native checkpoint

The fresh Win98 guest run is preserved in `runs/vmctl-vc152rsd/`.

- `RSDSTA.TXT`: `COMPILE=PASS`, `LINK=PASS`
- `RSDCL.LOG`: VC++ 1.52c compiled `RSD.C` and `PROBE.C`; it emitted one
  C4769 near-pointer-to-long warning at the intentional Win16 combo
  `WM_ADDSTRING` pointer transport in `RSD.C`, but no errors
- `RSDLINK.LOG`: segmented link completed with the retained `WINSTUB.EXE`
- `RSDR.TXT`: `RESEARCH_DLG_CHECKS=9 FAILURES=0 RESULT=PASS`
- the post-run guest window list showed no Program Error dialog

`RSD.C` was transferred directly from the reviewed
`reconstructed/src/research_dlg.c` file. The native probe exercises dialog
initialization and geometry, resource aggregation and projected budget,
paint/erase/control-color/cursor paths, technology tracking, radio selection,
packed commit/writeback, tutor hooks, help, and close behavior.

This advances `ResearchDlg` from Phase 2 step 3/6 to step 4/6. Packed player
storage, localized resources, and full Win16 control integration remain open.

## Expanded packed-player checkpoint

The maintained source probe was synchronized into a fresh `C:\RSD2` package.
The expanded matrix adds the exact player-record write boundary at offset
`0x22` with length 2, verifies the ordinary packed value `0x12`, and verifies
the boundary packed value `0x65` for zero-percent allocation, technology 5,
and future technology 6. VC++ 1.52c compile and link passed; the existing
intentional near-pointer transport warning remains at the combo
`WM_ADDSTRING` boundary. The Win98 report is
`RESEARCH_DLG_CHECKS=10 FAILURES=0 RESULT=PASS`, the process exited 0, and the
post-run window list showed no Program Error dialog.

Evidence is preserved in `runs/vmctl-vc152rsd2/`,
`compat/vc152/research-dlg/RSD.C`,
`compat/vc152/research-dlg/PROBE.C`, and
`tests/research_dlg_source_probe.c`. This strengthens step 5/6 by closing the
packed write-location and nibble-boundary cases; localized resources, exact
Win16 control integration, and promotion remain open.

The fresh `C:\RSD3` run expands this boundary to 11 checks. It verifies the
six technology label payloads and title, the eight future-field resource IDs,
and the exact radio, combo, Done, and Help control lookups. VC++ 1.52c reports
`COMPILE=PASS` and `LINK=PASS`; the Win98 report is
`RESEARCH_DLG_CHECKS=11 FAILURES=0 RESULT=PASS`, and the final window list has
no Program Error dialog. Full live USER message-loop behavior and promotion
remain open.

## Live USER message-loop checkpoint

The remaining live USER boundary was exercised in isolated `C:\RSDM` with a
new native-only `RSDMSG.C` harness. It creates a real Win16 parent window and
radio, combo, Done, and Help child controls, runs the
`GetMessage`/`TranslateMessage`/`DispatchMessage` loop, and routes
initialization, radio selection, and Done through the reconstructed procedure.

- `runs/vmctl-vc152rsdmsg/RSDMSGST-FINAL.TXT`: `COMPILE=PASS`, `LINK=PASS`.
- `runs/vmctl-vc152rsdmsg/RSDMSG-FINAL.TXT`:
  `RESEARCH_DLG_USER_CHECKS=9 FAILURES=0 RESULT=PASS`.
- The report records two parent commands, five pumped messages, dialog close,
  one committed change, and the exact two-byte write at `0x22`.
- The final guest window inventory contained no Program Error dialog.

This closes the one remaining Step-5 evidence unit for `ResearchDlg`. The
harness deliberately leaves resource-string and detailed GDI assertions to
the existing callback/native probes, so it proves live USER/control/message
integration rather than claiming full original resource-dialog equivalence.
The final review-ledger promotion was subsequently recorded, so this
procedure is now promoted to `6/6`.
