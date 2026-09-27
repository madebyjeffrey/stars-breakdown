# TransferStuff E2 cargo-mutation integration checkpoint

This packet closes the named `TransferStuff` E2 boundary: the transfer
orchestration now drives the reconstructed `FleetTransferCargoBalance`
service at the same balance point as the original routine. The probe checks
the callback sequence, conservation across all five cargo fields, fuel moving
from the reduced fleet to the receiving fleet, packed per-design status
mutation, and the dialog-cancel path that must not balance cargo.

Host strict C89 reports:

`TRANSFER_STUFF_CARGO_E2_CHECKS=12 FAILURES=0 RESULT=PASS`

The fresh VC++ 1.52c / Win98 package in `C:\TRCNE2` reports the same result.
`TRCNE2STA.TXT` records `COMPILE=PASS` and `LINK=PASS`; the process exited 0,
and the post-run window inventory contained no Program Error dialog.

The successful sequence is recorded as prepare, split, hull enumeration,
tutor advance, dialog, cargo balance, selection refresh, and two invalidation
calls. The cancel sequence records dialog, delete-fleet, and cancel-memory,
with zero cargo-balance calls. This is E2 evidence, not final promotion: the
remaining E3 boundary is the complete packed planet/fleet/thing lookup and
Win16 transfer-dialog, refresh, and original differential path.

Evidence:

- `tests/transfer_stuff_cargo_e2_probe.c`
- `reconstructed/src/transfer_stuff.c`
- `reconstructed/src/fleet_transfer_cargo_balance.c`
- `compat/vc152/transfer-stuff-cargo-e2/`
- `compat/vc152/fleet-balance/FBAL.C`
- `compat/vc152/fleet-balance/FBAL.H`
- `runs/vmctl-vc152trcne2/TRCNE2STA.TXT`
- `runs/vmctl-vc152trcne2/TRCNE2CL.LOG`
- `runs/vmctl-vc152trcne2/TRCNE2LINK.LOG`
- `runs/vmctl-vc152trcne2/TRCNE2.TXT`
