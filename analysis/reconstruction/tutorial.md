# Tutorial dialog recovery

`reconstructed/src/tutor_dlg.c` is a semantic C reconstruction of TutorDlg
(NE 32:0000–0269). The dialog procedure keeps the tutor window handle and
visibility/panic bits in caller-supplied state, while Win16 drawing, menu,
dialog, message, help, and tutor-service operations are explicit callbacks.

The destroy path saves the sticky position, clears the tutor handle, unchecks
the tutor menu item, ends the dialog, and calls EndTutor(1). Paint and erase
messages draw or fill the client area. Character messages are forwarded to the
main frame and deliberately return zero. Initialization publishes the dialog
handle, applies the original zero-size SetWindowPos call, and restores its
sticky position.

The command path preserves the original distinction between the panic command,
hide command, and help command. Panic results set the tutor panic bit only for
result 0x09CA and post the result to the frame. Hiding invokes ShowTutor(0),
reports and clears the visible bit when necessary, and returns handled. The
host source probe covers these branches and service ordering. VC1.52c under
the Win98 StarDev environment compiled and linked the source; the native probe
passed all 8 checks with zero failures
(`runs/vmctl-vc152ew/tutor-dlg/TUTRES.TXT`).

This is touched semantic source, not a formal ledger promotion: the original
global tutor record and final application callback integration remain separate
review gates.

`reconstructed/src/start_tutor.c` is the companion coordinator reconstruction
for StartTutor (NE 32:06b4–0a2f). It models the tutorial-active guard,
procedure-instance creation and failure alert, title-window transition, scan
zoom selection, existing-game recovery, tutor-world initialization,
task positioning, dialog creation, and final visibility invalidation. The saved
game, Win16 window, resource, and tutor-service operations are explicit
callbacks; the source does not replay emitted bytes. Its source probe covers
procedure failure, fresh startup, and existing-game recovery. VC1.52c under the
Win98 StarDev environment compiled and linked it; the native probe passed all
3 checks with zero failures
(`runs/vmctl-vc152ew/start-tutor/STTRES.TXT`).

This is also touched semantic source, not a formal ledger promotion: the
application's real global records and all caller integration remain separate
review gates.

`reconstructed/src/draw_tutor_text.c` is the companion semantic renderer for
the text panel. It preserves the original client-relative panel construction,
shadow/highlight border order, fill, eight-line string limit, paragraph spacing,
bold-line color reversal, and WrapTextOut arguments. Resource decoding and
Win16 drawing are explicit callbacks. Its local and StarDev native probes both
pass; the native report is stored at
`runs/vmctl-vc152ew/draw-tutor-text/DTTRES.TXT`.
