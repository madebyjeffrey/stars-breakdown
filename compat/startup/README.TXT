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
The unchanged START1 executable passed a fresh WfW 3.11 session:
  /SMOKE: bitmap load, repaint, automatic exit and cleanup passed.
  /FALLBACK /SMOKE: starfield drawing, repaint, exit and cleanup passed.
  Alt+N and Alt+O: notices appeared and dismissal restored the artwork.
  Alt+X and a separate Alt+F4 run: exit and cleanup passed.
The original title also drew and closed cleanly in the same session.
No guest fault appeared; the user confirmed the artifacts were absent.
The earlier session's PROGMAN fault remains unexplained. These checks used
800x600/256 colors and keyboard controls. Other display drivers/resolutions
and mouse automation remain unverified. This is a development milestone.
The delivered START1 folder and ZIP are preserved as originally packaged;
their README contains the earlier first-run status. No rebuild is needed.
