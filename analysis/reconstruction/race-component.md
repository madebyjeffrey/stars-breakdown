# Race save/load and creation component

The game-code dependencies for both milestones are implemented and integrated.
The component can open, edit, create, password-check and save a race through
recovered C code. All six wizard procedures execute; the platform supplies
Windows services and messages. The [compiler/output package](../../compat/racevc/README.md)
now contains the recovered source, an automatic runner and a VC++ 1.52c build
recipe. Its host build passes all 88 scenarios. RETURN4 compiled and linked
revision 2 cleanly; the exact returned executable then passed **816 native
checks** in DOSBox / WfW 3.11, with all 89 states and 61 race files matching.
See the [native review](../compat/racevc-return4.md). It models UI services and
uses real CRT file I/O. Revision 3 adds a real runner progress window and
cancellation. [RETURN5](../compat/racevc-return5.md) passes all 816 checks in
the user's Win98 SE workflow, and the same EXE passes native progress/keyboard
cancellation checks in WfW 3.11. The earlier
[scenario package](../../compat/race/README.md) supplies live replay recipes;
a real Windows UI adapter and completion of that pilot remain separate work.

This adds **40 reviewed entry points, covering 27,718 original procedure bytes**.
The project now has **99 reviewed named routines / 45,906 procedure bytes**.
Twelve additional helper paths are implemented inside the message formatter
without crediting their complete original procedures. See the reproducible
[dependency audit](race-dependencies.json) and [formatter report](message-formatting.md).
The audit accounts for 88 game-code targets, including existing generation and
scoring code, and explicitly adds the seven modal callbacks omitted by an
ordinary direct-call traversal. No required game-code target is unaccounted for.

## Component entry points

Declarations are in `reconstructed/include/runtime.h` and `raceui.h`.

| Host entry | Behavior and original evidence |
|---|---|
| stars_runtime_init | Host integration context: complete DGROUP, original constant segments, OS callbacks and a disjoint near scratch arena. This is new adapter code. |
| stars_new_race | Copies the default 192-byte player, then opens the wizard. Reconstructs only CommandHandler's command 0x81 path, NE 5:3243–3269; other menu commands remain outside this component. |
| stars_race_wizard | RaceCreationWizard, NE 29:0000–03AC: page order, back/next/finish/cancel, score rejection, password check, save and restart after failure. |
| stars_save_race | FSaveRace, NE 29:58D4–5AB7: common save dialog, BOF, player record, checksum/EOF, close and saved basename. |
| stars_was_race_file | FWasRaceFile, NE 5:5DA8–5FFC: header/version/type checks, player decoding, checksum, password, publication and local error recovery. |
| stars_create_random_race | Previously reconstructed generator. The wizard's Random selection saves its marker; GenerateWorld calls this routine later. The wizard does not secretly generate another race on selection. |

`stars_race_wizard` edits the current player at DGROUP:4F9A; callers can load a
race first or use `stars_new_race`. The New Race caller retains the existing
password and filename buffers. An adapter must not add resets that the original
caller does not perform. The `fDontWrite` argument is unused in this executable:
passing it as one still reaches the save dialog and produces a file.

The original C entry points use far calls, word-sized arguments and AX or
DX:AX returns. Dialog callbacks use the Windows Pascal convention and remove
ten argument bytes. The host APIs use explicit state and native pointers;
their successful compilation does not establish a native Win16 ABI build.

## Save/load and password behavior

The save dialog's Win16 OPENFILENAME is 72 bytes. The adapter receives its
meaningful fields separately: owner, flags 0x8806, filter index one, 256-byte
filename/title buffers, empty initial directory, double-NUL race-file filter,
default extension and returned basename offset. OS cancellation returns zero.
The platform chooses the path; C code performs the subsequent game serialization.

Loading accepts major version two with minor versions 49 through 83. It checks
the BOF record type, file type five, player record type six and EOF checksum.
The new player is published only after the checks succeed. Current password
salt, error-silencing state and the caller's recovery target are restored on
the applicable paths. File-version state can change even when a later check
fails. Preserve the observed return distinctions: success one, some rejected
inputs zero, and version/password rejection minus one.

Header-write failure returns through FCreateFile's local recovery and retains
the original open-handle behavior. Failure while writing the later player/EOF
records transfers to the outer recovery target; FSaveRace does not acquire a
new recovery scope around those writes. The platform's unhandled `abort` hook
must not return. The test-only C bridge demonstrates a safe outer catcher.

Password hashing alternates signed-byte addition and multiplication with
32-bit wrap; an empty password is zero and a nonempty zero hash becomes one.
The dialog preserves the unsigned 16-bit failed-attempt counter, delay tiers,
cache-suppression flag, help path and default-password shortcuts. Delay uses
TOOLHELP TimerCount; file salts use USER GetTickCount. They are separate services.
Every password used in the comparisons is synthetic.

Logging now executes its real game path, including version/date/time expansion,
file existence checks, append seeking and ignored short-write results. Its
private file handle does not replace the active game-file handle.

## Wizard and drawing

| Logical page | Original callback | Dialog resource | Implemented interactions |
|---|---|---|---|
| 1 | RaceWizardDlg1 | 0x92 | Templates, Custom/Random, singular/plural names, password, leftover-point allocation, portraits |
| 2 | RaceWizardDlg4 | 0x95 | Ten primary traits, descriptions, Alternate Reality economy reset |
| 3 | RaceWizardDlg5 | 0x96 | Fourteen lesser traits and descriptions |
| 4 | RaceWizardDlg2 | 0x93 | Three environmental bands, immunity, drag/shift/widen/narrow controls, growth |
| 5 | RaceWizardDlg3 | 0x94 | Seven economy settings, shift increments, factory-cost checkbox, Alternate Reality restrictions |
| 6 | RaceWizardDlg6 | 0x97 | Six research-cost selections and the starting-tech checkbox |
| Password | PasswordDlg | 0x8C | Edit limit, OK/cancel/help, failed attempts and cache rules |

Shared source includes button rendering/tracking, mouse-message filtering,
sticky positioning, text alignment/clipping, DIB color/palette sizes and blitting,
advantage-point drawing/invalidation, gravity/environment strings and titles.
Calls are compared in order, with coordinates, text, colors, handles, button
states and bitmap geometry. FGetMouseMove consumes unrelated queued messages
without dispatching them. Button repetition and coordinate arithmetic retain
the original word widths and signed comparisons.

The wizard's Random marker and the generator are different stages. Existing
random-generation tests cover seeded output, fallback paths and names. New
modal tests also select Random and compare the saved file, so the milestone
does not conflate saving a random template with generating a world.

## U020: name-tail checksum mismatch

A controlled sequence now executes the original callbacks end to end:

1. Save a race named `LongRaceName` / `LongRaceNames` and successfully load it.
2. Reopen the wizard, select Custom (including its WM_COMMAND notification),
   then change the fields to `X` / `Xs` and finish.
3. Both implementations save the same bytes. Both reject that file on reload.

The Custom notification clears buffers and copies the then-current long names.
Later page exit writes the shorter strings into the existing buffers without
clearing their tails. Serialization drops those tails but IRaceChecksum includes
all 192 bytes. The same underlying mismatch was previously shown for flag bit
three at player offset 84, which the reader clears. This pass does not establish
a normal wizard action that introduces that flag.

The callback experiment models GetDlgItemText as copying returned characters
and the terminator, leaving the unused destination untouched. The actual
Windows edit-control implementation remains a specific native acceptance check;
this is not presented as a newly captured live UI failure. No normalization or
bug fix has been added to the reconstructed behavior.

## OS and resource contract for the next build

The component expects the application's initialized Windows objects. Startup,
resource allocation, the main application message loop and unrelated game
subsystems are not implicitly declared reconstructed. A small native adapter
can supply these services without reconstructing the entire startup routine.

* Load the six original dialog resources plus 0x8C. Their decoded control IDs,
  styles and original-byte hashes are in `analysis/ne/layout.json` and the audit.
* Supply the 256-by-128 portrait DIB from RT_BITMAP 0x85; the extracted resource
  is `analysis/ne/resources/resource_052_type_2.bin`, 33,856 bytes. DIB blits use
  32-by-32 portrait cells. Original startup obtains it with HdibLoadBigResource;
  that shared startup loader is outside the wizard call tree.
* Populate the brush handles at 0010/0012/0014/0016/001C, colors at 22BA/23FE/
  25F6/3EEC/48A6, Arial font at 26B6 and line height at 23FA, palette at 5314,
  portrait DIB at 5476, cursor handles at 525E/5304, instance at 5310 and window
  owners at 0352/258C as appropriate. Environmental brushes live at 5680.
* Preserve original DGROUP constants and templates, plus NE 3's compressed text,
  NE 9's planet-name tables and NE 2's component tables. `segments[3]` and `[37]`
  are initialized by runtime_init; the caller supplies `[9]` and `[2]` when needed.
* Map each StarsWindowCall to the named Windows API. The exact p/q conventions
  are described in [the call contract](race-window-api.md). `dialog` performs
  MakeProcInstance, DialogBox and FreeProcInstance with the supplied recovered
  callback. It must actually dispatch messages, not replace a whole game dialog
  with a scripted success result in the native application.
* Supply file access, read/write/close, seeking, date/time, the two clocks, far
  memory resolution and the nonreturning outer recovery hook.

The host comparison uses a 64 KiB DGROUP and scratch range E000–FF00. A native
build will need its own near/far storage adapter and callback thunks. A scratch
arena must not overlap globals, arguments, external memory or OS allocations.
After an outer nonlocal exit, the host event-loop adapter restores its arena
checkpoint. This is a storage contract, not an extra game rule.

## Verification and practical limits

The full suite passed **159 tests** in 366.888 seconds at the integration
checkpoint. Subsequent focused checks passed the added outer-write recovery,
Alternate Reality/environment-limit, and all-planet-name/cache tests. There
are **162 test methods** in the current suite. The formatter's signed-byte cache
fix was followed by all five formatter groups. Strict optimized C89 compilation
uses warnings as errors.

The comparisons execute the hash-verified original instructions with their NE
relocations. Only OS services have deterministic synthetic implementations.
The modal driver dispatches messages into the original Pascal callbacks on its
emulated stack and separately invokes the C callbacks. It validates complete
create/save sessions, back/cancel/read-only flows, score rejection, password
retry/cancellation, all mouse controls, trait changes, malformed/truncated files,
short/error reads and writes, and exact saved bytes. The retained step-4 STUDY.R1
fixture also loads successfully through both implementations when present.

Comparisons cover application DGROUP below E000, file bytes, callback results
and OS-event arguments. The eight-byte CRT floating result buffer at 1706–170D
is excluded: original NE 36 floating helpers write it as private return storage.
Dead stack bytes and the expired wizard rectangle pointer's numeric address
under the larger outer menu-handler frame are not portable host state.

This establishes component behavior under the stated service/input contracts.
It does not establish native Windows rendering, protected-mode selector faults,
arbitrary malformed pointer graphs, allocation-exhaustion behavior, or a
byte-identical executable. Native linking/execution is now independently
established for the 88-case compiler/output catalog in the RETURN4 replay and
RETURN5's complete Win98 SE run; that catalog retains modeled Windows services. The earlier native VC++
compatibility results remain valid for their tested source subset.

```sh
python3 tools/build_core.py
python3 -m unittest tests.test_race_runtime tests.test_race_format tests.test_race_ui -v
python3 tools/race_dependencies.py --check
python3 tools/baseline.py --check
```

On this macOS host, the original-instruction oracle needs permission to execute
Unicorn's JIT outside the restricted sandbox. It accesses only synthetic inputs
and allowlisted original assets; no live emulator interaction is required.
