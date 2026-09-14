STARS RACE COMPONENT - VC++ 1.52c COMPILE AND OUTPUT TEST, REVISION 3

This package contains C source to compile, an automatic test program, and
frozen original-executable expectations. RETURN4 compiled and linked revision
2 cleanly. Its interrupted run completed 11 cases with matching outputs.
The same returned EXE then passed all 816 checks in DOSBox / WfW 3.11, with
all 61 race files and 89 player snapshots matching the original expectations.
Revision 3 adds a visible progress window, Windows message processing,
Cancel/Esc, elapsed time, and TRACE.TXT checkpoints closed after each stage.
Recovered game code and the frozen test inputs are unchanged from revision 2.

1. Copy the complete RACEVC3 folder, including EXPECT and ACTUAL, to a fresh
   folder in the Windows 98 VM, for example C:\RACEVC3. Preserve prior returns.
2. Open a DOS prompt INSIDE Windows. Change into C:\RACEVC3.
3. Run BUILD C:\MSVC (replace C:\MSVC with your compiler installation root).
4. If both compile and link say PASS, run RACEVC.EXE while Windows is running.
   A progress window should appear immediately. It shows the scenario, wizard
   action, elapsed time, completed sessions, check count and failure count.
   The test runs 88 scenarios / 89 wizard sessions without wizard clicking.
   This unoptimized diagnostic build can take several minutes or longer,
   depending on VM speed. A single scoring action may take several seconds.
5. Let it reach a PASS/FAIL summary. Cancel, Esc, or the close button requests
   a safe stop at the next checkpoint; it does not discard partial results.
   If a stage stays unchanged for two minutes, request Cancel and report the
   stage. If it cannot stop, recover the VM and return the partial folder.
6. Return the ENTIRE folder in a new return directory on the FTP share.
   Include RESULTS.TXT, TRACE.TXT, ACTUAL, BUILD.LOG, LINK.LOG, BLDSTAT.TXT,
   EXE, OBJ, and MAP. Preserve any on-screen compiler/linker errors in
   OUTPUT.TXT too: the old DOS shell can omit them from redirected logs.
   Return failures or cancelled runs too. Use a fresh copy for another attempt;
   existing results or traces are not overwritten.

RESULTS

Expected success: STATUS=PASS; CASES=88; SESSIONS=89; CHECKS=816; FAILURES=0.
STATUS=CANCELLED means incomplete, even when FAILURES=0. TRACE.TXT records the
last entered stage, and its next line establishes that the preceding stage
returned. A final STATUS in RESULTS.TXT is required for a complete run.
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
