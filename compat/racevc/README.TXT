STARS RACE COMPONENT - VC++ 1.52c COMPILE AND OUTPUT TEST

This package contains C source to compile, an automatic test program, and
frozen original-executable expectations. It is ready for its FIRST native
build. A host build is checked separately; native success remains pending.

1. Copy the complete RACEVC1 folder, including EXPECT and ACTUAL, to a fresh
   folder in the Windows 98 VM, for example C:\RACEVC1. Keep names as supplied.
2. Open a DOS prompt INSIDE Windows. Change into C:\RACEVC1.
3. Run BUILD C:\MSVC (replace C:\MSVC with your compiler installation root).
4. If both compile and link say PASS, run RACEVC.EXE while Windows is running.
   It runs 88 scenarios / 89 wizard sessions automatically. It shows a summary
   when finished. The wizard is driven internally; no mouse input is needed.
5. Return the ENTIRE folder in a new return directory on the FTP share.
   Include RESULTS.TXT, ACTUAL, BUILD.LOG, LINK.LOG, BLDSTAT.TXT, EXE, OBJ, MAP.
   Preserve on-screen compiler/linker errors in OUTPUT.TXT too: this old DOS
   shell can leave diagnostics out of redirected logs. Return failures too.
   Use a fresh copy for another attempt; existing results are not overwritten.

RESULTS

Expected success: STATUS=PASS; CASES=88; SESSIONS=89; FAILURES=0.
There should be 61 newly generated .R1 files in ACTUAL. Sixty reloads succeed.
ACTUAL also contains 89 .STA files: full 192-byte player-state snapshots.
U020B.R1 is intentionally expected to fail reload with the original checksum
behavior. That predicted rejection counts as a passing assertion.
Twenty-eight sessions intentionally create no file (negative points/cancel).
REPORT TEXT IS ASCII, not UTF-16. Open RESULTS.TXT in an editor if TYPE renders
it oddly in this VM. BUILD prints its final status directly, without TYPE.

WHAT RUNS

The recovered C handles new-race initialization, all six wizard callbacks,
trait/environment/economy/research changes, scoring, password confirmation,
record encoding, encryption/checksum, save, and reload. The test program feeds
input events, supplies deterministic Windows-service answers, and uses real
C runtime file reads and writes. It does not copy EXPECT files into ACTUAL.
It compares wizard results, scores, all 192 player bytes, entire saved files,
and reload decisions with frozen original-x86 observations. Fixed RNG state
and clock inputs make an exact file-byte comparison meaningful.

This is a native COMPILER/OUTPUT test with modeled UI services. It does not
create real race-wizard windows or validate native GDI/USER rendering, dialog
message routing, real mouse integration, or equivalence of every game input.
Those need a later real Windows UI adapter and original-game acceptance.
Existing RACEACC1 manual recipes and baseline observations cover that separate
track. This package makes progress without waiting for mouse automation.

BUILD NOTES

This runner uses /AL (large model), /GA /GEs /Zp1 /Od /Zi /W3, LLIBCEW and LIBW.
The recovered semantic interface uses ordinary C pointers to a full 64 KiB
original data image. Large-model far data pointers let that image live outside
the runner's own DGROUP. This choice does not claim that the original game
used the same memory model, optimization flags, or final ABI.

Only quoted local include names are rewritten for DOS 8.3 filenames. Recovered
game logic is otherwise copied unchanged. MANIFEST.JSN records source and
package hashes. INDATA, STRINGS and PLANETS are static images derived solely
from the verified original executable and synthetic oracle initialization.
RSDATA contains deterministic control geometry from the original resources.
ACTUAL begins with only this package's explanatory README, no race outputs.
