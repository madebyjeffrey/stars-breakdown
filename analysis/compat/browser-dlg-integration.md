# BrowserDlg original Win98 integration checkpoint

This is the step-5 original-executable boundary for `BrowserDlg`. In the live
`StarDev` Win98 game, `Commands > Ship Design...` opened the original Ship &
Starbase Designer, and selecting the Components view displayed the browser
child boundary.

The view showed the component-group selector (`All`), a scrollable component
list with component icons and names, and Done/Help controls. No program-error
dialog appeared.

Evidence:

- Original executable: `C:\STARS!\STARS.EXE` in Win98 on `StarDev`.
- Dialog: `Ship & Starbase Designer [#3270]`, Components view.
- Recovered boundary: `analysis/decompiler/export/raw/10d8_1ed8_BrowserDlg.c`.
- Native checkpoint: `runs/vmctl-vc152brd/` and
  `analysis/compat/browser-dlg-native.md`.
- Original screenshot: `runs/vmctl-vc152current/ship-design-components.bmp`.

This advances `BrowserDlg` from 4/6 to 5/6. Packed `vpartBrowser` records,
exact component lookup tables, localized storage, and full USER
control/message integration remain open before promotion.

The exact component-group table was then recovered from the original code
segment at `CS:1EB5`. Its sentinel plus sixteen normal groups are now exercised
by the strict host probe and a fresh VC++ 1.52c run in `C:\BRD3`. The expanded
report is `BROWSER_DLG_CHECKS=24 FAILURES=0 RESULT=PASS`; compile and link
passed, the process exited 0, and no Program Error dialog appeared. The probe
checks selection mapping to the recovered group values and empty-combo
next/previous wrap across entries 1 through 16 while preserving the packed
item word.

This closes the exact group-table boundary inside step 5/6. Localized string
storage, complete USER control/message integration, packed `vpartBrowser`
payload consumption, original differential coverage, and promotion remain
open.

The initialization resource sequence was also made explicit. A second fresh
native run in `C:\BRD4` verifies the original compressed-string IDs are sent in
order from `0x43F` through `0x44F`, for 17 entries. The report is
`BROWSER_DLG_CHECKS=25 FAILURES=0 RESULT=PASS`; compile and link passed, the
process exited 0, and no Program Error dialog appeared. This narrows the
localized-storage boundary, but does not yet prove the recovered string
payloads or complete USER control/message integration.

The packed adapter boundary was then expanded in fresh `C:\BRD5`. It writes
the recovered `vpartBrowser` header bytes, preserves the packed high byte of
the item word, and maps valid component records through the recovered
`FLookupPart` table offsets: group `0x0008` uses base `0x04E0`, stride
`0x36`, and 12 entries; group `0x0010` uses base `0x2450`, stride `0x3C`,
and 24 entries. Initialization, next navigation, and group selection all
round-trip those bytes in the 31-check host/native matrix.

The native report is `BROWSER_DLG_CHECKS=31 FAILURES=0 RESULT=PASS`; compile
and link passed, the process exited 0, and no Program Error dialog appeared.
This closes the packed header/table-offset boundary inside step 5/6. Actual
component payloads, localized string contents, complete USER control/message
integration, original differential coverage, and promotion remain open.

Evidence:

- `analysis/decompiler/export/raw/1008_524e_FLookupPart.c`
- `tests/browser_dlg_source_probe.c`
- `compat/vc152/browser-dlg/PROBE.C`
- `compat/vc152/browser-dlg/RUNBRD5.BAT`
- `compat/vc152/browser-dlg/RUNBRD5P.BAT`
- `runs/vmctl-vc152brd5/BRDSTA.TXT`
- `runs/vmctl-vc152brd5/BRDCL.LOG`
- `runs/vmctl-vc152brd5/BRDLINK.LOG`
- `runs/vmctl-vc152brd5/BRDR.TXT`

Evidence:

- `tests/browser_dlg_source_probe.c`
- `compat/vc152/browser-dlg/PROBE.C`
- `compat/vc152/browser-dlg/RUNBRD3.BAT`
- `compat/vc152/browser-dlg/RUNBRD3P.BAT`
- `runs/vmctl-vc152brd3/BRDSTA.TXT`
- `runs/vmctl-vc152brd3/BRDCL.LOG`
- `runs/vmctl-vc152brd3/BRDLINK.LOG`
- `runs/vmctl-vc152brd3/BRDR.TXT`
- `runs/vmctl-vc152brd4/BRDSTA.TXT`
- `runs/vmctl-vc152brd4/BRDCL.LOG`
- `runs/vmctl-vc152brd4/BRDLINK.LOG`
- `runs/vmctl-vc152brd4/BRDR.TXT`

The component payload consumer boundary was added in fresh `C:\BRD6`. The
packed adapter materializes the recovered 52-byte `_compart` layout and checks
`id` at offset 0, technology bytes at 2, name at 8, mass at 40, resource cost
at 42, three ore costs at 44, and bitmap ID at 50 after BrowserDlg selects the
table entry. Initialization, next navigation, and group selection each
consume the corresponding packed record. The native report is
`BROWSER_DLG_CHECKS=33 FAILURES=0 RESULT=PASS`; compile and link passed, the
process exited 0, and no Program Error dialog appeared.

This closes the typed packed component-record layout/consumer boundary inside
step 5/6. Original component payload values, localized string contents,
complete USER control/message integration, original differential coverage, and
promotion remain open.

Evidence:

- `analysis/debug/types.json` (`_compart`, 52 bytes)
- `tests/browser_dlg_source_probe.c`
- `compat/vc152/browser-dlg/PROBE.C`
- `compat/vc152/browser-dlg/RUNBRD6.BAT`
- `compat/vc152/browser-dlg/RUNBRD6P.BAT`
- `runs/vmctl-vc152brd6/BRDSTA.TXT`
- `runs/vmctl-vc152brd6/BRDCL.LOG`
- `runs/vmctl-vc152brd6/BRDLINK.LOG`
- `runs/vmctl-vc152brd6/BRDR.TXT`

## Original component-payload checkpoint

The original payload bytes are now checked rather than synthesized. The
preserved `target/stars.exe` decodes the `_compart` records from NE segment 2
at `0x04E0 + item * 0x36` for the 12-entry group `0x0008` table and at
`0x2450 + item * 0x3C` for the 24-entry group `0x0010` table. The maintained
probe embeds those recovered records and checks actual technology masks,
component names, mass, resource cost, ore costs, and bitmap IDs for
representative entries from both tables, including `Valanium`,
`Superlatanium`, `Laser`, and `Multi Contained Munition`.

The fresh `C:\BRD7` native run reports
`BROWSER_DLG_CHECKS=35 FAILURES=0 RESULT=PASS`; compile and link passed, the
process exited normally, and the post-run window list showed no Program Error
dialog. This closes the original component-payload-value concern inside step
5/6. Localized string payloads, complete USER control/message behavior,
original differential coverage, and promotion remain open.

Evidence:

- `target/stars.exe`
- `analysis/decompiler/export/raw/1008_524e_FLookupPart.c`
- `analysis/debug/types.json`
- `tests/browser_dlg_source_probe.c`
- `compat/vc152/browser-dlg/PROBE.C`
- `compat/vc152/browser-dlg/RUNBRD7.BAT`
- `compat/vc152/browser-dlg/RUNBRD7P.BAT`
- `runs/vmctl-vc152brd7/BRDSTA.TXT`
- `runs/vmctl-vc152brd7/BRDCL.LOG`
- `runs/vmctl-vc152brd7/BRDLINK.LOG`
- `runs/vmctl-vc152brd7/BRDR.TXT`

## Localized compressed-string payload checkpoint

The original compressed-string decoder is now exercised with the actual
payload text for resource IDs `0x43F` through `0x44F`, rather than only the
resource-ID sequence. The maintained probe uses the decoder's recovered
segment-3 lookup/chunk boundary and checks all 17 labels in order: `All`,
`Armor`, `Beam Weapons`, `Bombs`, `Electrical`, `Engines`, `Mechanical`,
`Mine Layers`, `Mining Robots`, `Orbital`, `Planetary`, `Scanners`, `Shields`,
`Ship Hulls`, `Starbase Hulls`, `Terraforming`, and `Torpedoes`. The combined
ASCII payload sum is 15229.

The fresh `C:\BRD8` native run reports
`BROWSER_DLG_CHECKS=36 FAILURES=0 RESULT=PASS`; compile and link passed, the
process exited normally, and the post-run window list showed no Program Error
dialog. This closes the localized-payload-content concern inside step 5/6.
Complete USER control/message integration, original differential coverage,
and promotion remain open.

Evidence:

- `analysis/decompiler/export/raw/1010_743a_PszGetCompressedString.c`
- `analysis/decompiler/export/raw/1010_743a.asm`
- `tests/browser_dlg_source_probe.c`
- `compat/vc152/browser-dlg/PROBE.C`
- `compat/vc152/browser-dlg/RUNBRD8.BAT`
- `compat/vc152/browser-dlg/RUNBRD8P.BAT`
- `runs/vmctl-vc152brd8/BRDSTA.TXT`
- `runs/vmctl-vc152brd8/BRDCL.LOG`
- `runs/vmctl-vc152brd8/BRDLINK.LOG`
- `runs/vmctl-vc152brd8/BRDR.TXT`

## Real USER/message-loop checkpoint

The callback-only probe has now been complemented by a real Win16 USER loop in
fresh `C:\BRDMSG`. The harness creates an actual parent window, combo box,
checkbox, Previous/Next buttons, Done button, and browser child; dispatches
the reconstructed `WM_INITDIALOG`, `WM_PAINT`, `WM_ERASEBKGND`, and
`WM_COMMAND` paths; and drives the actual combo through translated
`CB_ADDSTRING`, selection, and query messages.

The VC++ 1.52c build and link passed, and the native report is
`BROWSER_DLG_USER_CHECKS=9 FAILURES=0 RESULT=PASS`. The report records 17
actual combo entries, three commands, real child creation, two invalidations,
paint/erase handling, and clean Done teardown. The final Win98 window inventory
contains no Program Error dialog. This closes the USER/control/message-loop
unit inside step 5/6.

The remaining BrowserDlg unit is now one focused boundary: a direct original
differential through the complete Ship & Starbase Designer caller path,
followed by ledger promotion.

Evidence:

- `compat/vc152/browser-dlg/BRDMSG.C`
- `compat/vc152/browser-dlg/BRDMSG.DEF`
- `compat/vc152/browser-dlg/BRDMSG.LNK`
- `compat/vc152/browser-dlg/BUILDBRDMSG.BAT`
- `compat/vc152/browser-dlg/RUNVC-BRDMSG.BAT`
- `compat/vc152/browser-dlg/RUNPROBE-BRDMSG.BAT`
- `runs/vmctl-vc152brdmsg/BRDMSGSTA3.TXT`
- `runs/vmctl-vc152brdmsg/BRDMSGCL3.LOG`
- `runs/vmctl-vc152brdmsg/BRDMSGLINK2.LOG`
- `runs/vmctl-vc152brdmsg/BRDMSG3.TXT`
- `runs/vmctl-vc152brdmsg/native-screen3.bmp`

## Direct original caller-path differential

The remaining original-behavior boundary was exercised through the real game
caller rather than a synthetic dialog launch. From the original game frame,
`Commands > Ship Design...` opened `Ship & Starbase Designer [#3270]`; choosing
the `Components` radio opened the BrowserDlg view with the `All` selector, a
scrollable icon/name list, and Done/Help controls. Selecting `Quick Jump 5`
and then `Long Hump 6` changed the highlighted row and detail pane. Changing
the group selector to `Armor` displayed `Tritanium` and `Crobmnium`; selecting
`Tritanium` updated the detail pane. Changing it to `Beam Weapons` displayed
`Laser` and `X-Ray Laser`. These are the original caller-visible effects
covered by the typed group table, component payloads, and the real USER loop.

The original run completed with the designer still present and no Program
Error dialog in the window inventory. This closes the last BrowserDlg Step-5
evidence unit. The review record and coverage ledgers are now synchronized,
so BrowserDlg is promoted to Step 6/6.

Evidence:

- `runs/vmctl-vc152browser-live/before.bmp`
- `runs/vmctl-vc152browser-live/designer-before.bmp`
- `runs/vmctl-vc152browser-live/components-initial2.bmp`
- `runs/vmctl-vc152browser-live/components-quick-jump.bmp`
- `runs/vmctl-vc152browser-live/components-long-hump.bmp`
- `runs/vmctl-vc152browser-live/group-armor.bmp`
- `runs/vmctl-vc152browser-live/armor-titanium.bmp`
- `runs/vmctl-vc152browser-live/group-beam.bmp`
