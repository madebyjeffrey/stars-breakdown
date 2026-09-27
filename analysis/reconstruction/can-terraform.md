# FCanTerraformLppl reconstruction

`reconstructed/src/can_terraform.c` is a typed semantic reconstruction of the terraforming eligibility predicate, preserving owner, environment, technology, resource and special-case gates.

Validation: `tests/can_terraform_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152eu/CTERSTA.TXT` and `runs/vmctl-vc152eu/CTERRES.TXT`.
