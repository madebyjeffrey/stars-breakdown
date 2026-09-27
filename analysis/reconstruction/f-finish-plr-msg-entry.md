# FFinishPlrMsgEntry reconstruction

`reconstructed/src/f_finish_plr_msg_entry.c` is a typed semantic reconstruction of player-message entry finalization, preserving text termination, length/count updates, message flags, and storage callbacks.

Validation: `tests/f_finish_plr_msg_entry_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ew/FFMSTA2.TXT` and `runs/vmctl-vc152ew/FFMRES.TXT`.
