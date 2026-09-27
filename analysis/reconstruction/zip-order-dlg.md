# ZipOrderDlg reconstruction

## Current Phase 2 status

## Final Phase 2 status

`ZipOrderDlg` is promoted at Phase 2 step 6/6. E1 packed order/name
storage, E2 exact localized resource IDs/text, and E3 real USER/message-loop
mutation all pass. The native USER run covers actual radio controls, paint,
Create/Rename/Delete, control-color, Help, and OK teardown; the original
`Rename Fleet` caller boundary opens and cancels without changing game state.
The review and coverage ledgers record promotion.

E1 is closed: the packed four-record `_ziporder` layout and its five-word
Create, name, validity, Rename, Delete, and Cancel boundaries pass the
11-check host/native matrix in `runs/vmctl-vc152zodint/`. E2 is also closed:
the exact original resource IDs and strings are checked in the host/native
probe, including the title, caption, empty-slot message, and five order-action
labels. The procedure remains at 5/6 with only E3 complete live Win16 and
original-behavior integration still open.

`ZipOrderDlg` manages the four stored ship-order presets.  The reconstruction
keeps the dialog message routing, selected-slot state, default-name handling,
ampersand escaping, create/rename/delete behavior, tutor and sticky-position
hooks, and the paint/control-color paths.  Stored order words and Win16 USER/
GDI operations are exposed through typed state and callbacks.

## Recovered behavior

- Initialization sets the dialog title, hides the unused list control, selects
  slot one, enables the matching buttons, populates all four radio labels, and
  saves the sticky position.
- Paint draws the framed selected-order area and renders either the empty-order
  message or the five stored order rows.  Valid rows also preserve the raw
  measured-caption, right-aligned resource-label, and per-resource text-color
  sequence through typed state and callbacks.
- Radio commands select a slot, refresh button enablement, and invalidate the
  dialog.  Create and rename pass a bounded working name through the rename
  service, escape ampersands for radio-button text, and refresh the slot.
- Delete clears the selected slot and restores its numbered default label.
  OK/cancel save the sticky position and close with the original result; help
  dispatches the original context identifier.

The source probe covers initialization and dialog-handle lifetime, first/last
radio frame geometry, fill-rect erase, empty-rename fallback, paint,
control-color, selection, create, delete, help, and close paths.

A fresh native checkpoint is complete in `StarDev` using the exact current
`reconstructed/src/zip_order_dlg.c` source and guest directory `C:\ZOD`. VC++
1.52c reported `COMPILE=PASS` and `LINK=PASS`; the Win98 report was
`ZIP_ORDER_DLG_CHECKS=10 FAILURES=0 RESULT=PASS`, and the redirected console
file was empty. The native window list showed only the DOS prompt, agent,
desktop, and file manager; no program-error dialog appeared.

Packed order storage, localized resources, original rename-dialog behavior,
and full Win16 integration remain open before formal review. The original
Win98 acceptance boundary was also exercised in the running game: `Rename
Fleet` opened for `Stalwart Defender #5`, and Cancel returned without changing
the fleet name. See `analysis/compat/zip-order-integration.md` and its paired
JSON evidence for that step-5 checkpoint.

A packed-record checkpoint now binds the typed state to four recovered
24-byte `_ziporder` records in a fresh native harness at `C:\ZODINT`. The
strict host and VC++ 1.52c reports each pass
`ZIP_ORDER_DLG_PACKED_CHECKS=11 FAILURES=0 RESULT=PASS`, covering five-word
Create copying from the packed order source, name and ampersand handling,
Delete validity cleanup, Rename persistence, and Cancel closure. This
strengthens Phase 2 step 5/6; localized resources, exact original dialog
behavior, complete live Win16 integration, original differential coverage,
and final promotion remain open.
