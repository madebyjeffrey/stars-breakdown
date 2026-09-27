# RemoteTerraforming reconstruction

`reconstructed/src/remote_terraforming.c` is a typed semantic reconstruction of remote terraforming, preserving source/target eligibility, range, cost, environment changes, and resulting resource/state updates.

Validation: `tests/remote_terraforming_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ew/RTSTA3.TXT` and `runs/vmctl-vc152ew/RTRES.TXT`.
