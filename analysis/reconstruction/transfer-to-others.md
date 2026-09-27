# TransferToOthers reconstruction

`reconstructed/src/transfer_to_others.c` is a typed semantic reconstruction of resource transfer between players, preserving eligibility, amount limits, recipient selection, and resulting resource/message updates.

Validation: `tests/transfer_to_others_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ew/XTOSTA.TXT` and `runs/vmctl-vc152ew/XTORES.TXT`.
