# IDropPart

`IDropPart` is represented as typed drop-classification logic in
`reconstructed/src/i_drop_part.c`.  It preserves modifier-key normalization,
empty-side versus slot hit classification, same-slot rejection, incompatible
drop rejection, compatible merge selection, separate current/capacity packed
quantities, source-slot decrement by the actual moved amount, return codes,
and the update/invalidate/tutor side-effect boundaries.

The recovered packed slot rectangle table is now accepted directly with
Win16 `PtInRect` edge semantics; the callback hit tester remains an adapter
fallback. The host probe covers empty drops, same-slot drops,
capacity-limited compatible merges, source-slot decrement, and rejection/beep
paths. Native VC++ 1.52c/Win98 validation now compiles, links, and reports 26
checks without a runtime error dialog. A fresh packed integration checkpoint
binds the recovered 123-byte `_hul` record, its `rghs` table at offset 58,
four-byte `_hs` entries, and `chs` at offset 122; the host and Win98 adapter
each pass 24 checks for raw slot round-trip, empty removal, capacity-limited
merging, source decrement, rejection, and no-modify behavior. Exact Win16 hit
testing and build-dialog redraw/update calls remain callback-owned, so this
remains a partial semantic source. A live original-executable diagnostic
reached the editable Ship &
Starbase Designer and confirmed the component inventory, populated slot
surface, and selected-component summary, but the available controller could
not perform the required press/move/release drag sequence. The temporary
copied design was cancelled without committing a change; see
`analysis/compat/i-drop-part-live-diagnostic.md` and
`runs/vmctl-vc152idp-live/`. This is useful boundary evidence, but it does
not provide a real press/move/release drag trace. The typed callback contract
now also reproduces the raw empty-side and merge redraw/control order:
`UpdateShdefCost`, client-rect retrieval, slot/selection redraw, erase
invalidation, and conditional tutor advancement. Host and fresh Win98 E3
packets report eight checks with no Program Error dialog. E1, E2, and the E3
redraw/control acceptance boundary are closed; only the final ledger
promotion remains. The controller limitation on an interactive drag capture
is retained as an explicit scope note rather than silently treated as direct
UI evidence.
