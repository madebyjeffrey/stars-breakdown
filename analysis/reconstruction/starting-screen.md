# Reconstructed starting-screen milestone

The project now builds a new Win16 executable that opens and draws the original
starting screen. It uses recovered C for its buttons, fonts, colors, title
bitmap, palette handling and version text, with a new Windows bootstrap and OS
adapter. Game creation/loading and the main game window remain disconnected.
This milestone does not mark all of startup, runtime data or Part 5 complete.

The [native build/run record](../compat/startup1-native.json) identifies the exact
executable and retains the compile, link, resource-binding and first-run logs.
The transfer folder and ZIP are `ftp-share/START1` and `ftp-share/START1.ZIP`;
[instructions](../../compat/startup/README.TXT) cover running the supplied EXE
or compiling the source with `BUILD C:\MSVC`.

## Source recovered

Five complete routines add **1,959 original bytes**, bringing reviewed named
game coverage to **127 / 846 routines, 55,129 bytes**. The coverage ledger does
not credit the whole title procedure or INI reader for their partial paths.

| Routine | Original half-open range | Reconstructed entry |
|---|---|---|
| ReadBigBlock | NE 9:5310–53b4 | stars_read_big_block |
| HdibLoadBigResource | NE 9:5220–530f | stars_load_big_resource |
| HpalFromDib | NE 9:4b50–4c9b | stars_palette_from_dib |
| GetIniWinRc | NE 1:1020–124c | stars_ini_window_rect |
| DrawABunchOfStars | NE 8:5fe2–627f | stars_draw_stars |

Implementation: [resources.c](../../reconstructed/src/resources.c),
[title.c](../../reconstructed/src/title.c),
[public interface](../../reconstructed/include/title.h).
Every promoted routine was checked against its complete assembly export.
Previously reconstructed DibNumColors, PaletteSize, DibBlt and RcCtrTextOut are
reused, alongside the existing memory manager and startup foundation.

The title component also recovers these identified paths of TitleWndProc
(NE 5:9126–9840): WM_CREATE, both normal painting branches, iconic painting,
WM_QUERYNEWPALETTE/WM_PALETTECHANGED, and bitmap release during destruction.
It preserves the unusual 500/501-pixel button-height boundary, the separate
500-pixel font threshold and the 650-pixel button-width threshold. Palette
changes restore the previously selected palette and repaint only when needed.
The font configuration slice of ReadIniSettings is NE 1:249d–2544; font names
of 5–31 characters are accepted. Other configuration paths remain open.

## What the code reveals

The 800×600, 8-bit title DIB occupies **481,088 bytes** including its original
resource padding. The loader opens the resource as a file and reads it in
30,000-byte chunks. A 16-bit offset overflow advances the selector by eight,
allowing reads beyond one 64 KiB segment. Success deliberately leaves the
resource locked. Failed reads close the resource file and free the allocation.

Palette construction keeps a fixed 40-byte header skip even for a CORE header;
it swaps blue/red components while preserving each fourth byte. Its color-count
test is signed, and LocalAlloc failure is unchecked in the original. Tests
exercise valid palette buffers and do not invent a recovery branch for that
invalid-memory case.

GetIniWinRc parses one mode letter and four four-character decimal fields.
A minus sign can appear anywhere inside a field, including multiple minus
signs. Invalid input restores default coordinates; only the main frame defaults
to maximized. Upper window-flag bits survive either path.

The fallback starfield uses literal tables embedded at CS:5f7e–5fdf. Raw Ghidra
labels misleadingly identify these as player data. Small stars are opaque
one-pixel text rectangles; larger stars use overlapping three-pixel rectangles
to form crosses. The window background supplies the black fill.

Its PushRandom call supplies two 16-bit constants, although the callee reads two
32-bit seeds. Consequently the first seed becomes 000b0011h, and the second
comes from the caller's saved DI/SI words. The recovered API exposes those words
as `stack_seed`, avoiding undefined host-C reads while preserving the original
instruction behavior for any supplied register pair. The new application uses
zero for this input so native fallback repaints are stable. Exact original
fallback positions require the original calling context; see U022.

The original SzVersion instructions format **Version 2.60j**. The reconstructed
screen displays that text unchanged. The user's identification as retail
2.70j RC3 remains the provenance record in U001; the displayed version alone
does not overturn it.

## Native integration and resources

[APP.C](../../compat/startup/APP.C) is explicitly new integration code. It
registers a hidden owner frame and a title window, initializes the recovered
fonts/colors/tiles, exercises allocation/free through the real Win16 heap
adapter, dispatches drawing and palette messages, and runs recovered cleanup.
New/Open commands display a milestone notice; Continue is disabled without a
previous game. It does not call or run the original executable. It also does
not claim recovery of WinMain, FCreateStuff, FrameWndProc, all command-line
options, registration, or WriteIniSettings.

Configuration is isolated to `STARSREC.INI` beside the new executable. Only
font settings and a saved owner-frame rectangle are read. No settings are read
from an installed original game. Text, build scripts and manifests use ASCII
with DOS CRLF endings.

[The exporter](../../tools/package_startup.py) emits Win16 .RES records from the
allowlisted original. An independent RC 3.11 RCDATA probe established the record
encoding. After native RC binding, all **122 resource types, identifiers,
flags, lengths and SHA-256 hashes** match the original NE resources. This
includes dialogs, menus, icons, cursors, accelerators, bitmaps and custom data;
embedding a resource does not complete the code that uses it.

The package also carries original DGROUP initialization and the static prefix
of the compressed-string segment. Executable string-segment code beginning at
743ah is removed from the data carrier. All **1,414 strings** compare identically
with the original complete carrier. The reviewed starfield tables are expressed
as C constants. Remaining CRT data fixups, seven library records without code,
and other application tables are still separate work.

Native compilation uses `/AL /GA /GEs /Zp1 /Od /Zi /W3`, LLIBCEW and LIBW.
This is the established large-model semantic adapter; the original medium-model
layout remains a later target. Linking needs `/ALIGN:64` for the complete
resource bundle. `/ALIGN:16` compiled/linked but RC rejected the resulting
resource extent as too large; the current build passes all three stages.

The native adapter maps original near allocations to reserved slots in its
64 KiB DGROUP image and tracks real LocalAlloc/LocalFree lifetimes. Palette and
bitmap pointers retain real Windows selectors, including huge-pointer reads.
The recovered game allocator uses actual GlobalAlloc/Lock/Unlock/Free calls.
Only the subset needed by the starting screen is wired to native USER/GDI;
unrecognized operations terminate with a diagnostic rather than returning
invented success.

## Validation and remaining acceptance

The focused run passed **19 tests** across test_title_machine,
test_startup_package and test_startup_machine. They cover large reads and short
reads, every resource-loader failure exit, palette conversion, configuration
syntax, all string IDs, title layout thresholds, file-presence behavior,
starfield pixel events and RNG restoration, bitmap/fallback/iconic drawing,
palette repaint decisions, and existing font/color/cleanup integration.
The CPU oracle executes original game instructions; only OS services are
modeled. Its huge-pointer backing store accounts for protected-mode selector
increments. KERNEL ordinal 113 is resolved as the constant __AHSHIFT=3, rather
than a function address; this agrees with the
[Wine Win16 export definition](https://raw.githubusercontent.com/wine-mirror/wine/master/dlls/krnl386.exe16/krnl386.exe16.spec).

The first native run in the existing WfW 3.11 session visibly displayed the
original artwork, four buttons and version text. APP.LOG records 17 reads,
481,088 bytes, two paints, resource release, complete cleanup and successful
exit through Alt+X.

That session also showed Program Manager drawing corruption before launch and
reported a PROGMAN page fault in USER.EXE at 0001:4F7F during subsequent keyboard
testing. Its cause is **unresolved**, not attributed to the environment or the
new code without evidence. A clean repeat run is required (U023).

Automatic approval review rejected the next launch of the locally built EXE
as unrecognized software requiring user confirmation. No alternate execution
route was used. Once confirmed, run these remaining checks:

1. Start a fresh Windows session; run `START.EXE /SMOKE`. Require FIRST_PAINT,
   REPAINT, TITLE_RELEASE, CLEANUP and EXIT to pass in APP.LOG without guest faults.
2. Preserve that log, then run `START.EXE /FALLBACK /SMOKE`. Check the original
   starfield/text rendering path and cleanup.
3. Run interactively; check Alt+N and Alt+O show the milestone notice, dismissal
   repaints correctly, and Exit/Alt+X/Alt+F4 close cleanly.
4. Repeat font/default settings and another display resolution as needed to
   isolate any discrepancy. Keep native UI integration distinct from the
   deterministic original-instruction event comparisons.

Next implementation work is the full application initialization/configuration
chain and then connecting useful creation/opening flows to this native shell.
The checked title milestone supplies a visible application to extend while the
rest of Part 5 continues.
