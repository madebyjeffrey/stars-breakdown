# MessageWndProc reconstruction

`MessageWndProc` (`0007:5c92-7218`, 5,510 bytes) owns the message-window
child controls and dispatches layout, keyboard navigation, filtering, message
links, object actions, painting, cursor, and close-related behavior. The
reconstruction expresses those branches with named Win16 message constants and
typed callbacks rather than assembly or emitted-byte replay.

The host source probe covers creation/layout, navigation, filtering,
mode-switch, paint, command, erase-background, and cursor paths. It passes
strict C89 checks and the native VC++ 1.52c/Win98 validation; the retained
native report is `CHECKS=9`, `FAILURES=0`, `RESULT=PASS` in
`runs/vmctl-vc152ew/MWPRES.TXT`.
