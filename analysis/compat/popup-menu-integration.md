# PopupMenu packed/Win16 integration checkpoint

The reconstructed `PopupMenu` was exercised through a fresh VC++ 1.52c /
Win98 harness in `C:\\PMUINT`. The integration probe covers direct labels,
separators, ampersand escaping, checked items, all four flagged name-source
families, submenu creation, caller-owned submenu headers, right-button
tracking, command decoding, and menu cleanup.

The source boundary was corrected before the native run: a submenu header is
passed to `AppendMenu` from the caller-owned label array, rather than being
sent through the kind-0 localized-name resolver. The native callback uses the
real Win16 `CreatePopupMenu`, `AppendMenu`, and `DestroyMenu` APIs. Win16
automatically destroys a child submenu with its parent; the source's later
child cleanup call is therefore allowed to return false as an expected
ownership effect, while unexpected API failures remain fatal.

Host strict C89 reports 30 structural checks; the native wrapper adds one
real-tracking case. The final native report is:

`POPUP_MENU_INTEGRATION_CHECKS=31 FAILURES=0 RESULT=PASS`

The native build reports `COMPILE=PASS` and `LINK=PASS` in
`runs/vmctl-vc152pmuint/`. The initial native case displayed the real Win16
`#32768` popup and exposed an interaction detail: Enter dismissed the
checked-item menu without producing a command. The final fresh `C:\PMUINT3`
case selected the first item with a real mouse click and reported
`POPUP_MENU_E2_CHECKS=30 FAILURES=0 RESULT=PASS`, with command `15000` decoded
to selection `0`. The callback uses the real `TrackPopupMenu` dispatch,
observes the owner queue without posting a synthetic command, and the later
`PeekMessage` removes that real `WM_COMMAND` using the exact recovered
arguments (`hwnd`, `0x111`, `0x111`, `2`). The E2 trace also resolves packed
fleet, thing, planet, and deep-space records and checks their storage widths.
Compile and link passed, the process exited 0, and the final window inventory
contained no Program Error dialog. The retained native screen and logs are
runtime safety evidence.

This closes `PopupMenu` at Phase 2 step 6/6 and promotes it. The native E2
report is retained as `runs/vmctl-vc152pmuint3/PMENUINT-click.TXT`; the DOS
8.3 guest name is `PMENUINT.TXT` even though the wrapper source is
`PMENUINTE2.C`.

Evidence:

- `tests/popup_menu_integration_probe.c`
- `compat/vc152/popup-menu-integration/`
- `analysis/decompiler/export/raw/10c0_136c_PopupMenu.c`
- `analysis/reconstruction/popup-menu.md`
- `runs/vmctl-vc152pmuint/PMUISTA.TXT`
- `runs/vmctl-vc152pmuint/PMUICL.LOG`
- `runs/vmctl-vc152pmuint/PMUILINK.LOG`
- `runs/vmctl-vc152pmuint/PMINT.TXT`
- `runs/vmctl-vc152pmuint/PMENUINT.MAP`
- `runs/vmctl-vc152pmuint/native-screen-track.bmp`

## Compressed-string lookup checkpoint

The integration probe now feeds the actual compressed-string IDs `0x43F`
through `0x44F` through `PopupMenu`'s flagged-name path. The resolver uses
the recovered original payloads—`All`, `Armor`, `Beam Weapons`, `Bombs`,
`Electrical`, `Engines`, `Mechanical`, `Mine Layers`, `Mining Robots`,
`Orbital`, `Planetary`, `Scanners`, `Shields`, `Ship Hulls`, `Starbase Hulls`,
`Terraforming`, and `Torpedoes`—and checks their order, first/last IDs, and
combined ASCII sum of 15229.

The fresh `C:\PMUINT2` VC++ 1.52c / Win98 run reports
`POPUP_MENU_INTEGRATION_CHECKS=32 FAILURES=0 RESULT=PASS`; compile and link
passed, the real popup was displayed and selected with Enter, and no Program
Error dialog appeared. This closes the compressed-string payload concern.
The final E2 run in `C:\PMUINT3` closes the remaining owner/message-queue and
caller-record boundaries without a harness bridge.

Evidence:

- `analysis/decompiler/export/raw/1010_743a_PszGetCompressedString.c`
- `tests/popup_menu_integration_probe.c`
- `compat/vc152/popup-menu-integration/PROBE.C`
- `compat/vc152/popup-menu-integration/RUNBLD2.BAT`
- `compat/vc152/popup-menu-integration/RUNPRB2.BAT`
- `runs/vmctl-vc152pmuint2/PMUISTA.TXT`
- `runs/vmctl-vc152pmuint2/PMUICL.LOG`
- `runs/vmctl-vc152pmuint2/PMUILINK.LOG`
- `runs/vmctl-vc152pmuint2/PMINT.TXT`

## Final E2 closure

The strict host E2 probe reports `POPUP_MENU_E2_CHECKS=30 FAILURES=0
RESULT=PASS`. The fresh VC++ 1.52c / Win98 run in `C:\PMUINT3` reports the
same result after a real first-item mouse selection, with `COMMAND=15000` and
`MASK=0`. It uses no `PostMessage` bridge: the native callback captures the
actual owner `WM_COMMAND` for the recovered `PeekMessage` boundary, and the
probe validates the exact owner/minimum/maximum/remove arguments. The packed
caller-record adapter covers fleet, thing, planet, and deep-space IDs, while
the existing compressed-string run covers IDs `0x43F`–`0x44F`.

Evidence:

- `tests/popup_menu_e2_probe.c`
- `compat/vc152/popup-menu-integration/PMENUINTE2.C`
- `compat/vc152/popup-menu-integration/BUILDE2.BAT`
- `compat/vc152/popup-menu-integration/RUNVC-E2.BAT`
- `compat/vc152/popup-menu-integration/RUNPROBE-E2.BAT`
- `runs/vmctl-vc152pmuint3/PMENUINTE2STA.TXT`
- `runs/vmctl-vc152pmuint3/PMENUINTE2CL.LOG`
- `runs/vmctl-vc152pmuint3/PMENUINTE2LINK.LOG`
- `runs/vmctl-vc152pmuint3/PMENUINTE2.MAP`
- `runs/vmctl-vc152pmuint3/PMENUINT-click.TXT`
- `runs/vmctl-vc152pmuint3/menu-open.bmp`
