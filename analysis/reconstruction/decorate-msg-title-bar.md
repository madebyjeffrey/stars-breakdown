# DecorateMsgTitleBar reconstruction

`reconstructed/src/decorate_msg_title_bar.c` is a typed semantic reconstruction of message-title-bar decoration, preserving title selection, geometry, colors, and drawing callbacks.

Validation: `tests/decorate_msg_title_bar_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ew/DMTSTA4.TXT` and `runs/vmctl-vc152ew/DMTRES.TXT`.
