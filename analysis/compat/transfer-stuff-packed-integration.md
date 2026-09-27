# TransferStuff packed-record integration checkpoint

`TransferStuff` receives two selected transfer objects and orchestrates the
dialog, split/jettison, balancing, and refresh services. The packed adapter
decodes the recovered 128-byte `_xfer` records into the typed object boundary.

The checked direct fields are `_xfer.id` at offset 0, `_xfer.grobj` at offset
2, and the overlapping fleet/planet/thing view at offset 4. The adapter
verifies fleet cargo presence from the fleet ship-count bytes, fleet owner
identity, split and cancellation cleanup, mode-0 jettison, valid-hull
enumeration, cargo balancing, scanner invalidation, selection refresh, and
transfer-mode reset.

Host strict C89 and native VC++ 1.52c reports are:

`TRANSFER_STUFF_PACKED_CHECKS=14 FAILURES=0 RESULT=PASS`

The native build in `runs/vmctl-vc152trint/` reports `COMPILE=PASS` and
`LINK=PASS`; the process exited 0 and the post-run window list contained no
Program Error dialog.

This advances `TransferStuff` within Phase 2 step 5/6. It does not promote
the procedure: the later E2 packet separately closes exact cargo mutation
through the reconstructed balance service. Complete packed fleet/planet/thing
lookup and mutation, exact transfer-dialog behavior, Win16 lookup/refresh
behavior, original differential coverage, and final review remain open.

Evidence:

- `tests/transfer_stuff_packed_probe.c`
- `compat/vc152/transfer-stuff-packed/`
- `analysis/debug/types.json`
- `analysis/decompiler/export/raw/1050_4faa_TransferStuff.c`
- `analysis/reconstruction/transfer-stuff.md`
- `runs/vmctl-vc152trint/TRINTSTA.TXT`
- `runs/vmctl-vc152trint/TRINTCL.LOG`
- `runs/vmctl-vc152trint/TRINTLINK.LOG`
- `runs/vmctl-vc152trint/TRINT.TXT`
