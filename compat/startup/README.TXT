STARS RECONSTRUCTION - START2: STARTING SCREEN AND RACE WORKSHOP

QUICK START
Copy this entire folder to a NEW directory in the Windows 98 / 86Box VM,
for example C:\START2. Run START.EXE while Windows is running.
A prebuilt executable is included; compilation is optional.

Click Race Workshop, or press F6 / Alt+W on the starting screen.
In the workshop, choose Race > New Race. The original six-page race wizard
is live: names, predefined/random/custom races, primary and lesser traits,
habitability/growth, economy, research, advantage points and race artwork.
Next, Back, Finish and Cancel use the recovered game code.
Finish opens the real Windows Save dialog. Choose a new .R1 filename.
Race > Open Race reopens saved files; Recent Race opens the last saved/opened
race. These files can also be opened by the original Stars! executable.

Help > Settings exposes font names and two new package preferences:
use the starfield title, and start directly in Race Workshop. Save settings,
exit using Race > Exit, then restart. Window position and race-dialog position
are remembered in STARSREC.INI beside START.EXE. That file is separate from
any installed game's settings. Settings take effect on the next launch.
The workshop's close button returns to the starting screen; Race > Exit
closes the application. Starting-screen Exit / Alt+X also exits.

AUTOMATIC CHECK
Run START.EXE /CHECK from Windows, or use Help > Run Acceptance Check.
It visibly visits and paints all six wizard pages, writes a new CHECKnnn.R1,
reloads it, compares all 192 player-state bytes, then checks cancellation.
The command-line check closes the application on completion. Its APP.LOG
must end EXIT=PASS and contain ACCEPTANCE=PASS, WIZARD_CHECK=PASS,
ROUNDTRIP=PASS and CANCEL_CHECK=PASS. Existing CHECK files are preserved.
The Help-menu version displays a result dialog and leaves the workshop open.
CHECK.LOG records the developer's completed native check, when supplied.

For the broader compiler/output regression, open the REGRESS subfolder and
run RACEVC.EXE from Windows. It runs 88 scenarios / 89 wizard sessions with
visible progress. Expected RESULTS.TXT: STATUS=PASS, CHECKS=816, FAILURES=0.
It creates 61 race files and 89 state snapshots. This separate test uses
modeled UI inputs; START.EXE exercises the real Windows dialogs and drawing.
Preserve an old REGRESS results folder and use a fresh copy for another run.

MANUAL ACCEPTANCE
See CHECKS.TXT for a short sequence covering interaction, file exchange,
settings and cancellation. Use new filenames for test races.
Page painting took about 1.1-1.2 seconds on the developer's DOSBox setup;
VM speed can differ. APP.LOG is closed after each checkpoint so progress
can be inspected even during a slow operation. No long initial silent test
run is necessary: the interactive application is available immediately.

WHAT TO RETURN
Return this entire folder in a new RETURN directory on the FTP share.
Include APP.LOG, STARSREC.INI, your synthetic test race files and notes about
any wrong drawing or behavior. If run, include REGRESS/RESULTS.TXT, TRACE.TXT
and ACTUAL. Preserve any build-screen errors in OUTPUT.TXT as well.
APP.LOG starts fresh each launch; copy a failing log before another launch.
Do not include personal game files or private passwords in the return.

OPTIONAL BUILD
From a DOS prompt inside Windows, change into this folder and run:
  BUILD C:\MSVC
Use your actual VC++ installation directory if different.
Read BLDSTAT.TXT, BUILD.LOG, LINK.LOG and RC.LOG. All must pass.
REGRESS has its own BUILD.BAT if you also want to rebuild that test.
All delivered text is ASCII with DOS CRLF endings, not UTF-16.

BOUNDARIES
This is newly compiled C89 source with all 122 original resources embedded.
It does not launch or use the original executable to perform race operations.
New Game and Open Game still display milestone notices; game generation,
turn processing, the full game frame, full startup and all game preferences
are later work. Help-file browsing is not bundled. This is a study build.

It uses the validated large-model semantic interface and exact bounded x87
arithmetic implemented with integer words. It does not yet reproduce the
original medium-model linker layout or yield a byte-identical executable.
The original stack-dependent starfield seed is explicitly zero here.

Other useful commands:
  START.EXE /SMOKE             title repaint and automatic exit
  START.EXE /FALLBACK /SMOKE   starfield repaint and automatic exit
The /FALLBACK option forces starfield drawing for that run.
The earlier guest drawing fault did not recur in the current checks; its
cause remains unresolved. Windows 98 / 86Box acceptance of START2 is pending
your run. User will provide serial number if a later milestone requires it.
