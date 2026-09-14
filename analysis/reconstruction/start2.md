# START2: native race workshop and startup configuration

The new Win16 application combines the reconstructed title with an interactive
race workshop: all six original wizard dialogs, points and artwork, real Windows
file pickers, save/reopen, recent race, cancellation and a settings dialog.
The application and regression runner are built with VC++ 1.52c and supplied in
`ftp-share/START2` plus `START2.ZIP`. START1 remains frozen.

## Recovered code and integration boundaries

Three additional whole procedures are reviewed and compared against original
instructions: ReadIniTileSettings (NE 1:2b8a, 612 bytes), GetWindowRc
(NE 5:79ac, 73 bytes), and SetWindowIniString (NE 5:79f6, 128 bytes).
The total is 813 original bytes; named-source coverage reaches 130 procedures /
55,942 bytes. The one-byte gap after GetWindowRc is not credited as source.

`config.c` additionally reconstructs explicitly bounded ReadIniSettings paths:
main-frame geometry, layout, six pane dimensions, toolbar flag, tile ordering,
backup count and fonts. Its writer persists frame geometry, layout, pane sizes
and toolbar flag. These paths do not complete ReadIniSettings or WriteIniSettings.
Original signed clamps, tile case/group rules and the SetWindowIniString quirk
(ignoring its output parameter and always using szWork) are retained.

`compat/startup/APP.C`, `APP.H` and `NATIVE.C` supply the new bootstrap, workshop,
settings window and Windows services. These are integration code, not recovered
WinMain, FrameWndProc or the complete FCreateStuff. The recovered memory manager,
fonts/colors, resource loader, palette, title and terminal cleanup are reused.
Only the three race DIBs and their palette are added to live startup; all 122
original resources remain embedded with exactly matching payloads and identifiers.
A new package preference selects the starfield or direct workshop startup.
Settings live in the package's own STARSREC.INI. The original game INI is not used.

The native adapters use real USER/GDI dialogs, controls, drawing, message routing,
MakeProcInstance callback lifetimes, common file dialogs, ToolHelp timing, and
Windows file reads/writes. The recovered race routines compute and serialize all
race state. New-race session setup clears the filename/password scratch buffers;
this is an explicit caller policy. The last successfully saved/opened path is a
new package preference. Cancel preserves the on-disk file; unsaved working state
may remain visible in the workshop until another race is opened or created.

The startup settings dialog is new and exposes the original regular/bold/italic/
bold-italic font keys. It also remembers main-window and race-dialog placement.
Game generation, saved-game loading, full game UI, command-line batch processing,
and remaining game preferences stay outside this milestone. No whole-application
or byte-identical executable claim is made.

## Race responsiveness

`innate_fp.h` retains the exact bounded positive x87 model: four 32-bit limbs,
64-bit intermediate precision, 53-bit stores and ties-to-even rounding. Binary
bit-length tests, word shifts and base-65536 multiply/divide replace long chains
of single-bit operations. No host floating point, 64-bit C type or changed race
formula is introduced. Rational-arithmetic tests cover carries/rounding; new
128-bit shift tests cover word boundaries and shifts beyond the value width.
Original-x86 scoring comparisons and a fresh VC152 regression check this change.

The native dialog log records paint durations and closes at each checkpoint.
One diagnostic build painted wizard pages in approximately 1.1-1.2 seconds at
20,000 DOSBox cycles/ms; the final run displayed 53,430 cycles/ms and logged
approximately 0.4 seconds. The live cycle change was not attributed, so these
are observations rather than a controlled performance comparison or a
Windows 98 performance guarantee. The workshop caches its summary score to avoid
recalculating it on every exposed-window repaint.

## Acceptance and delivery

See [native evidence](../compat/startup2-native.json) for exact executable hashes,
build results, completed check logs, cross-application observations and limits.
The `/CHECK` mode paints and advances six real dialogs, saves to an unused
CHECKnnn.R1, clears the in-memory player, reloads the file, compares all 192 bytes,
then checks cancellation. The Help-menu version leaves the application open.
This test supplies the save filename automatically; the actual Windows Save and
Open dialogs are separately exercised interactively.

The independent REGRESS executable covers the existing 88 scenario / 89 session
catalog, comparing 816 assertions, 61 race files and 89 state snapshots against
frozen original-executable expectations. It supplies modeled UI input and uses
real native file I/O; it complements the real-window checks in START.EXE.
Both programs and their buildable sources are included. The [delivery audit](../compat/startup2-delivery.json) records the final ZIP hash and manifest checks. REGRESS starts without
new-run results; developer results are labeled BASELINE.TXT. CHECKS.TXT describes
user interaction, cancellation, settings and original-game exchange checks.

The packager verifies every executable input before including a prebuilt file,
requires passing build/resource checks, and ties CHECK.LOG to the tested EXE hash.
It also requires a matching completed regression build. Each original resource is
compared by type, identifier, flags, size and SHA-256. Text is ASCII with CRLF.
The user can run START.EXE immediately; rebuilding is optional. Windows 98 / 86Box
acceptance is pending the user's returned package.

## START2A: habitability colors

The user found that page 4's blue/red/green ranges were missing in START2.
The bootstrap loaded the race bitmaps and palette but omitted the six
`rghbrPlanetAttr[3][2]` brushes at DGROUP 5680h-568Bh. DrawRace2 therefore
selected empty handles. The original range geometry and race state were present.

START2A adds the bounded FCreateStuff slice at NE 1:023d-02af (end exclusive).
It obtains the original dark/bright blue, red and green pairs through HbrGet;
FreeStuff already releases these six references. Tests compare the original
instructions' complete data state and OS calls for fresh, shared and failed
allocations, then compare reference release. This partial slice does not count
as a completed FCreateStuff or change the 130-procedure / 55,942-byte total.

The previous automatic acceptance checked dialog painting and race bytes but
did not inspect rendered colors, so it missed the omission. START2A samples
each range interior on page 4 and requires its original color or the device's
nearest color, allowing a small neighborhood for palette dithering. A failure
is included in the acceptance result. The log records all three sampled colors.

The final VC++ 1.52c build passed all six pages, color sampling, 192-byte
save/reload, cancellation and cleanup with WfW 3.11's standard VGA driver.
Manual inspection also showed the three filled ranges. The same final executable
failed the color check after restoring the existing ET4000 / SVGA256 display
setup: black ranges remained, with colored fills appearing below the dialog.
The original Stars! wizard showed the same misplaced fills in that SVGA session.
A Windows restart alone did not remove the issue. An attempted LatchCapable=No
test was inconclusive because the driver rewrote it to Yes during startup.
SVGA also failed with `/FALLBACK /CHECK`, excluding title-bitmap loading as a
necessary trigger for the misplaced fills.
These observations implicate the SVGA display path in this symptom, but do not
isolate its driver/emulator mechanism or explain the older USER.EXE fault (U023).
The previous display configuration was restored after the VGA test.

See [START2A native evidence](../compat/startup2a-native.json) and
[delivery audit](../compat/startup2a-delivery.json). The new `ftp-share/START2A`
folder and ZIP contain the tested executable, sources and updated checks.
START1 and START2 remain frozen. The separate regression runner and its 816-pass
baseline are reused only after verifying every regression input is unchanged;
that runner was not rebuilt or rerun for this startup-only correction.
