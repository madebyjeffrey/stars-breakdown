# RemoveIshdefFromAllQueues reconstruction

`reconstructed/src/remove_ishdef_queues.c` is a typed semantic reconstruction of ship-design removal from production queues, preserving queue iteration, item compaction, count updates, and cache invalidation.

Validation: `tests/remove_ishdef_queues_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152eu/PQUESTA.TXT` and `runs/vmctl-vc152eu/PQUERES.TXT`.
