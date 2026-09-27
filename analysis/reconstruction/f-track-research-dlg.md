# FTrackResearchDlg reconstruction

`reconstructed/src/f_track_research_dlg.c` is a typed semantic reconstruction of research-tracking dialog initialization and selection, preserving component rows, current research state, controls, and close/help handling.

Validation: `tests/f_track_research_dlg_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ew/FTRSTA.TXT` and `runs/vmctl-vc152ew/FTRRES.TXT`.
