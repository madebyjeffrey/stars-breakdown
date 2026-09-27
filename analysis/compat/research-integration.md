# Research dialog step-5 integration checkpoint

The original generated game was continued in the connected `StarDev` Win98
guest. `Commands > Research...` opened the `Research` dialog with Technology
Status, Currently Researching, Expected Research Benefits, and Resource
Allocation sections. The observed values included Energy level 3, Energy Tech
Level 4, 390 resources needed, and a 15% research allocation. Selecting the
Weapons field changed the selection and redrew the dialog without a Program
Error. Screenshots are preserved in `runs/vmctl-vc152current/` and the exact
boundary is recorded in `analysis/compat/research-integration.json`.

The raw procedure is `analysis/decompiler/export/raw/10d8_0000_ResearchDlg.c`;
the focused native checkpoint remains `runs/vmctl-vc152rsd/RSDR.TXT`.

This advances `ResearchDlg` from Phase 2 step 4/6 to step 5/6 for the original
dialog and radio-selection boundary. Packed player storage, localized
resources, exact Win16 control handles, and promotion remain open.

The packed/native checkpoint was expanded in fresh `C:\RSD3`. The strict
host and VC++ 1.52c/Win98 matrix now reports
`RESEARCH_DLG_CHECKS=11 FAILURES=0 RESULT=PASS`. It verifies the six original
technology labels (`Energy` through `Biotechnology`), the `Research` title,
the eight future-field resource IDs `0x53` through `0x5a`, and the exact
radio (`0x431`–`0x436`), combo (`0x43b`), Done (`2`), and Help (`0x76`)
control lookups. Compile and link passed, and the final window list contained
no Program Error dialog. This closes the resource-ID/control-lookup portion of
step 5; full live USER message-loop behavior and promotion remain open.
