# DoAiTurn reconstruction

`reconstructed/src/do_ai_turn.c` is a typed semantic reconstruction of AI-turn dispatch, preserving per-player iteration, personality routing, task execution, and turn-state cleanup.

Validation: `tests/do_ai_turn_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ew/DATSTA.TXT` and `runs/vmctl-vc152ew/DATRES.TXT`.
