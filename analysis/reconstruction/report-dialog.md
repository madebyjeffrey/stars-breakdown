# Score report dialog recovery

`reconstructed/src/score_x_dlg.c` is a genuine C reconstruction of ScoreXDlg
(NE 22:0F66–13B6). It preserves the three report-mode drawing routes, history
cursor hit region, initialization and sticky-position state, cancel/help and
tutor transitions, mode cycling, and the eight-entry history popup with its
selection and validity-bit updates. The report renderers and Win16 services
are explicit typed callbacks; the implementation does not replay emitted
bytes.

The source probe covers initialization, painting, mode cycling, help,
cancel, history cursor handling, and popup selection in 7 checks. VC1.52c
under the Win98 StarDev environment compiled and linked it, and the native
probe passed all 7 checks with zero failures
(`runs/vmctl-vc152ew/score-x-dlg/SXDRES.TXT`).
