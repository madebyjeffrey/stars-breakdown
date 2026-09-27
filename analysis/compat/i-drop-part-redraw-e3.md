# IDropPart redraw/control integration — E3

This checkpoint closes the redraw/control integration boundary for
`IDropPart`. The typed source now exposes the recovered Win16 service sequence
instead of collapsing it into one generic invalidation callback.

For an empty-side removal, the callback trace is:

`UpdateShdefCost → GetClientRect → DrawSlotDlg(0, rect, iSrc) →
InvalidateRect(rect with top=yBuildInfoSum, erase=1) →
DrawBuildSelComp(0, -1) → DrawBuildSelHull(0, -1, NULL) → AdvanceTutor`

For a compatible merge, the trace is:

`UpdateShdefCost → SetBuildSelection(slot) → GetClientRect →
InvalidateRect(rect with top=yBuildInfoSum, erase=1) → AdvanceTutor`

The strict host C89 probe reports
`I_DROP_PART_REDRAW_E3_CHECKS=8 FAILURES=0 RESULT=PASS`. It checks both
service orders, the mode and slot arguments, the null hull rectangle, the
client rectangle preservation, the `yBuildInfoSum` top replacement, and the
erase flag.

The fresh VC++ 1.52c/Win98 run in `runs/vmctl-vc152idpr/` reports
`COMPILE=PASS`, `LINK=PASS`, and the same eight-check result. The first package
link attempt was discarded as a copied-package filename error (`IDPM.DEF`);
the corrected rerun compiled and linked cleanly, exited 0, and showed no
Program Error dialog. The initial failed logs remain in the same run folder
for auditability, while `IDPRSTA2.TXT`, `IDPRCL2.LOG`, and `IDPRLINK2.LOG`
are the passing build artifacts.

The raw original-instruction listing establishes the exact service order and
the live original diagnostic establishes the surrounding editable Ship &
Starbase Designer boundary. The available controller still cannot perform a
press/move/release drag trace, so this packet closes the recovered
redraw/control acceptance boundary rather than claiming a controller-created
interactive drag capture.
