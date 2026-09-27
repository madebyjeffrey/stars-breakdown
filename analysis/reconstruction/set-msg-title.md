# SetMsgTitle reconstruction

`reconstructed/src/set_msg_title.c` is a typed semantic reconstruction of message-window title selection and formatting, preserving player/context labels and the empty/default title path.

Validation: `tests/set_msg_title_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ew/SMTSTA2.TXT` and `runs/vmctl-vc152ew/SMTRES.TXT`.
