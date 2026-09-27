# IDropPart native checkpoint

The fresh Win98 guest run is preserved in `runs/vmctl-vc152idp/`.

- `IDPSTA.TXT`: `COMPILE=PASS`, `LINK=PASS`
- `IDPCL.LOG`: VC++ 1.52c compilation completed for the current
  `i_drop_part.c` source and probe
- `IDPLINK.LOG`: segmented link completed with `IDP.DEF`
- `IDPR.TXT`: `I_DROP_PART_CHECKS=26 FAILURES=0 RESULT=PASS`
- `IDPR-CONSOLE.TXT`: empty; the process exited cleanly

The run used fresh `C:\IDP`, directory-first CRLF wrappers, a local
`WINSTUB.EXE`, and a native-only report path. The guest `windows` check after
execution showed no program-error dialog. This validates modifier
normalization, empty-side drops, same-slot rejection, capacity-limited
compatible merges, source-slot decrement, incompatible-drop rejection,
return codes, and update/invalidate/beep/tutor side effects. Exact packed
hull-slot layout, Win16 hit testing, and full build-dialog integration remain
outside this checkpoint.

The packed hit-table E1 follow-up is preserved in
`runs/vmctl-vc152idphit/`:

- `IDPHSTA.TXT`: `COMPILE=PASS`, `LINK=PASS`
- `IDPHCL.LOG`: VC++ 1.52c compilation completed for the packed hit-table source/probe
- `IDPHLINK.LOG`: segmented link completed with `IDPH.DEF`
- `IDPHE1.TXT`: `I_DROP_PART_HIT_TABLE_E1_CHECKS=12 FAILURES=0 RESULT=PASS`

The process exited with status 0 and the post-run window inventory showed no
Program Error dialog. This closes E1 of the Step-5 packet; drag-state
mutation, redraw/control integration, and complete original drop behavior
remain open.

The mutation E2 follow-up is preserved in `runs/vmctl-vc152idpmut/`:

- `IDPMSTA.TXT`: `COMPILE=PASS`, `LINK=PASS`
- `IDPMCL.LOG`: VC++ 1.52c compilation completed for the mutation source/probe
- `IDPMLINK.LOG`: segmented link completed with `IDPM.DEF`
- `IDPME2.TXT`: `I_DROP_PART_MUTATION_E2_CHECKS=11 FAILURES=0 RESULT=PASS`

The process exited with status 0 and the post-run window inventory showed no
Program Error dialog. This closes E2 of the Step-5 packet; redraw/control
integration, complete original drop behavior, and ledger promotion remain
open.

The redraw/control E3 follow-up is preserved in `runs/vmctl-vc152idpr/`:

- `IDPRSTA2.TXT`: `COMPILE=PASS`, `LINK=PASS`
- `IDPRCL2.LOG`: VC++ 1.52c compilation completed for the redraw source/probe
- `IDPRLINK2.LOG`: segmented link completed with `IDPR.DEF`
- `IDPRE3.TXT`: `I_DROP_PART_REDRAW_E3_CHECKS=8 FAILURES=0 RESULT=PASS`
- `native-screen.bmp`: post-run Win98 screen capture

The process exited with status 0 and the post-run window inventory showed no
Program Error dialog. The packet checks the recovered empty-side and merge
service order, rectangle-top replacement, erase flag, selected-component and
selected-hull arguments, and original-instruction boundary. The first link
attempt in this run folder is retained as a package filename correction
record; it does not affect the passing rerun. The controller limitation on a
real press/move/release drag trace remains documented in the live diagnostic.
