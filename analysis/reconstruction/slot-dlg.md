# SlotDlg reconstruction

`reconstructed/src/slot_dlg.c` is a typed C reconstruction of the 8,447-byte
Win16 build-dialog controller at NE `001a:0550–264f`.  The dispatcher keeps
the original message ordering and state transitions while routing platform and
packed-record details through explicit callbacks.

Covered paths include initialization, paint/erase/control-color/draw-item and
measure-item messages, spin/slot mouse forwarding, starbase and build-mode
selection, list and edit notifications, tutorial gates, queue checks, design
delete/copy/edit acceptance, cancel/close behavior, invalidation and list
refreshes, and help dispatch.  The state record makes `mdBuild`, starbase
mode, hull-copy mode, selection validity, edit guarding, design counts, and
class text visible to callers.

The host C89 probe covers ten representative dispatch and state-transition
checks.  The same source and probe compiled and linked with VC1.52c in Win98;
the native report passed with zero failures.
