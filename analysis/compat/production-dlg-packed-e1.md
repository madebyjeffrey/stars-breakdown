# ProductionDlg packed-global E1 validation

The readable `ProductionDlg` source now owns the raw `vrgZipProd` caller
boundary through `stars_production_dlg_decode_zip_queues`. The adapter preserves
the recovered 40-byte `_zipprodq` layout: the 13-byte name, `valid` byte at
offset 13, `no_research` byte at offset 14, item count at offset 15, and twelve
little-endian 16-bit production words beginning at offset 16. It caps the
dialog's consumed table at the original five-record storage boundary, while the
dialog's saved-queue popup retains the original four-entry visible/menu cap and
sparse slot indices.

Host strict C89 validation passes:

`PRODUCTION_DLG_PACKED_CHECKS=11 FAILURES=0 RESULT=PASS`

Fresh VC++ 1.52c/Win98 validation in `C:\PDGP` also passes the same 11 checks.
Compile and link pass, the process exits 0, redirected console output is empty,
and the post-run Win98 window list contains no Program Error dialog.

This closes the named E1 packed-production-global unit for `ProductionDlg`.
The E2 complete localized resource comparison, E3 original queue differential,
and final Step-6 promotion remain open.

Evidence:

- `reconstructed/src/production_dlg.c`
- `reconstructed/include/core.h`
- `tests/production_dlg_packed_probe.c`
- `compat/vc152/production-dlg-packed-e1/`
- `runs/vmctl-vc152pdgp/PDGPSTA.TXT`
- `runs/vmctl-vc152pdgp/PDGPCL.LOG`
- `runs/vmctl-vc152pdgp/PDGPLINK.LOG`
- `runs/vmctl-vc152pdgp/PDGPE1.TXT`
- `runs/vmctl-vc152pdgp/PDGP-CONSOLE.TXT`
