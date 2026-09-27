# AutoTerraform reconstruction

`reconstructed/src/auto_terraform.c` is a typed semantic reconstruction of automatic terraforming, preserving eligibility, resource cost, environment changes, and resulting planet state.

Validation: `tests/auto_terraform_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ew/ATSTA2.TXT` and `runs/vmctl-vc152ew/ATRES.TXT`.
