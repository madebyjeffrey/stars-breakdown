STARS RACE ACCEPTANCE - PACKAGE 1

This package contains generated race variations and live replay instructions.
It is not yet a native VC++ application. There is nothing to compile here.

PILOT.TXT       Representative cases for the first DOSBox/86Box replay.
ALLCASES.TXT    The complete named scenario set, with expected results.
CANDIDAT       Race files created through the reconstructed C wizard callbacks.
MODEL          Matching files from the original x86 wizard callbacks, running
               with modeled Windows services. These are NOT live VM captures.
EXPECT.JSN    Actions, original predictions, raw player records, decoded files,
               checksums and controlled clock/random inputs for comparison.
MANIFEST.JSN  SHA-256 integrity values for generated inputs and expectations.
RETURN         Put independently created live races and observations here.

Run the pilot first. Restart Stars! for each independent case. From the title
screen choose New Game (Alt+N), then Customize Race (Alt+C); leave the New Game
race choice at Humanoid. Do not create a world. Do not overwrite
the generated MODEL or CANDIDAT files. Follow the U020 sequence without a restart
between its two stages; other cases must not inherit names or password state.

Two file directions matter:
1. Load CANDIDAT race files in the original game and record accepted/rejected.
2. Recreate the same cases using the original wizard, return those race files,
   and compare their decoded content and load behavior in the reconstruction.

To load: choose Open Game (Alt+O). First navigate into the fixture directory:
type its full path followed by \*.R1 and press Enter. Check that the Open dialog
shows that directory, then type the bare race filename and press Enter.
In the observed Win3.11 session, entering a full file path without first changing
the dialog directory caused a missing-XY error. Directory-first loading worked.

Negative-points and Cancel cases intentionally produce no race file. Record the
visible result in RETURN\OBSERVE.TXT. Missing files alone do not prove a pass.
If a case differs from its prediction, retain the file and describe the steps;
do not adjust the original game merely to force the predicted result.

The live save clock and random state affect header salt and encrypted bytes.
Comparison decrypts each file using its own header, then checks the complete
plain record payload, all 192 decoded player bytes, and checksum validity.
Only a salt difference is automatically allowed. Other header changes require
review. Original captures are retained intact. U020's mismatch is preserved.

The Random template saves a deferred-generation flag. It does not generate a
random race when selected; seeded random generation belongs to a separate test.

Current evidence: original/C agreement with modeled Windows services. Native
rendering, real edit-control behavior and live cross-loading remain pending.
The later VC++ 1.52c adapter will use these same scenario IDs and expectations.

Agent-operated reference checks can use DOSBox. When an 86Box check is needed,
the user receives exact steps and files to return through the existing share.
Text files are ASCII with DOS CRLF line endings; no console status workaround
or Unicode conversion is needed for this package.
