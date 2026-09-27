# PopupMenu native checkpoint

The fresh Win98 guest run is preserved in `runs/vmctl-vc152pmenu/`.

- `PMENUSTA.TXT`: `COMPILE=PASS`, `LINK=PASS`
- `PMENUCL.LOG`: VC++ 1.52c compiled the native wrapper containing the exact
  reviewed `PMENU.C` and `PROBE.C` source
- `PMENULINK.LOG`: segmented link completed with the retained `WINSTUB.EXE`
- `PMENUR.TXT`: `POPUP_MENU_CHECKS=22 FAILURES=0 RESULT=PASS`
- the post-run guest window list showed no Program Error dialog

The transferred `PMENU.C` is byte-identical to
`reconstructed/src/popup_menu.c`, and `PROBE.C` is byte-identical to
`tests/popup_menu_source_probe.c`. The probe covers direct and flagged menu
items, separators, submenu creation and cleanup, ampersand escaping, checked
state, resource-kind resolution, right-button tracking, and command decoding.

This advances `PopupMenu` from Phase 2 step 3/6 to step 4/6. Localized name
lookup, full Win16 menu API behavior, caller-owned records, and
original-behavior evidence remain open.
