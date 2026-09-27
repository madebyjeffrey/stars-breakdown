# DestroyAllIshdef reconstruction

`reconstructed/src/destroy_ishdef.c` is a typed semantic reconstruction of ship-design teardown, preserving iteration, owned allocation release, cache reset, and repeated-call behavior.

Validation: `tests/destroy_ishdef_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152eu/DISHSTA.TXT` and `runs/vmctl-vc152eu/DISHRES.TXT`.
