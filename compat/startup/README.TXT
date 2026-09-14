STARS RECONSTRUCTED STARTING SCREEN - MILESTONE 1

This is a new Win16 program compiled from reconstructed C89 source.
It contains the original resources. It does not run the original executable.

BUILD
The delivered package includes a prebuilt START.EXE and successful build logs.
You can run that executable directly, or rebuild it with the following steps.
Copy the whole folder to a new directory in Windows 98 or Windows 3.1.
From that directory at a DOS prompt, run:
  BUILD C:\MSVC
Use your compiler's actual installation directory if different.
See BLDSTAT.TXT, BUILD.LOG, LINK.LOG and RC.LOG. Keep screen errors too.

RUN
From Windows Program Manager or Explorer, run START.EXE in this folder.
The original title artwork, button layout, font selection and version text
are reconstructed. On a 16-color display the original starfield path is used.
New Game and Open Game display a clear milestone notice. Continue is disabled.
Exit and Alt+X close the application. Alt+F4 also works.
Game creation, loading and the main game window are not connected yet.

The program reads only its own STARSREC.INI in this folder. Optional [Fonts]
settings use the original font keys; use defaults for the first run.
It does not read settings from any installed copy of the original game.
APP.LOG records initialization, first paint, cleanup and any adapter error.
The original starfield consumes unspecified stack values as a second seed;
this executable uses zero for that explicit input so repaints are stable.

OPTIONAL CHECKS
START.EXE /FALLBACK forces the original starfield drawing path.
START.EXE /SMOKE automatically repaints and closes after several seconds.
START.EXE /FALLBACK /SMOKE combines these checks.
Return the entire folder if anything fails. User will provide serial number
if a later milestone requires it; this starting-screen milestone does not.

All text files are ordinary ASCII with DOS CRLF line endings.
This build uses the validated large-model semantic adapter, not the original
medium-model segment/link layout. Byte-identical executable matching remains
a separate, later objective. MANIFEST.TXT records source and resource hashes.

RECORDED TEST STATUS
The native build has opened and painted in WfW 3.11, read all 481088 title
bitmap bytes, repainted, exited through Alt+X, and logged cleanup success.
That existing Windows session also reported a PROGMAN page fault during
keyboard testing. Its cause is unresolved. A fresh /SMOKE run, /FALLBACK run
and New/Open action checks remain pending. This is a development milestone.
