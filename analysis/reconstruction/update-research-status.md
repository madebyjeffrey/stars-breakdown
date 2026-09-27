# UpdateResearchStatus reconstruction

`reconstructed/src/update_research_status.c` is a typed semantic reconstruction of research advancement. It handles multi-level advancement, component unlock reporting, pooled research redistribution, fixed-point rounding, and selection-mode updates through explicit state and callback interfaces.

Validation: `tests/update_research_status_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and two-case native probe pass in `runs/vmctl-vc152ew/RSSTA3.TXT` and `runs/vmctl-vc152ew/RSRES.TXT`.
