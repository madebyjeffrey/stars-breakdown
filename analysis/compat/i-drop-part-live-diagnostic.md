# IDropPart live original-executable diagnostic

The connected original Win98 `StarDev` session reached the Ship & Starbase
Designer through the Ship Design command. Copying the existing `Armed Probe`
opened the editable design surface with the component inventory on the left,
three populated component slots on the right, the selected component summary,
and the recalculated design-cost summary. Selecting `Long Hump 6` changed the
inventory selection and detail pane without producing an error dialog.

This confirms the surrounding build-dialog surface and slot/inventory boundary,
but it does not establish `IDropPart` acceptance behavior: the host controller
does not provide a press/move/release drag sequence, and no component was
actually dropped into a slot. The temporary copied design was cancelled, so no
design change was committed.

Evidence is preserved in `runs/vmctl-vc152idp-live/ship-copy.png`,
`runs/vmctl-vc152idp-live/idrop-selected.png`, and the corresponding raw
screenshots. This diagnostic alone was a step-4 boundary observation; the
later packed integration checkpoint advances `IDropPart` to Phase 2 step 5/6.
Exact Win16 hit testing and a real drop/update/redraw acceptance trace remain
open.
