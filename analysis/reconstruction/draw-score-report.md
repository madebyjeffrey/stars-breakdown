# DrawScoreReport reconstruction

`reconstructed/src/draw_score_report.c` is a typed semantic reconstruction of score-report rendering, preserving score calculation, player ordering, labels, ranges, and drawing operations.

Validation: `tests/draw_score_report_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ew/DSCSTA.TXT` and `runs/vmctl-vc152ew/DSCRES.TXT`.
