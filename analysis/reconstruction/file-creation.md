# File creation, names and error presentation

Five reviewed C89 semantic adapters add 784 original procedure bytes. The
source/signature records are in `analysis/functions.json` and
`analysis/debug/procedures.json`; the retained instruction export supplies the
control-flow evidence. These are host adapters, not a Win16 executable.

| Original | NE range, half-open | Host entry / source |
|---|---|---|
| SetSzWorkFromDt | 15:8CFE–8E15 | stars_set_file_name / file.c |
| FCreateFile | 15:8E16–8EA4 | stars_create_file / file.c |
| AlertSz | 9:2160–221D | stars_alert_string / alerts.c |
| FileError | 15:4A10–4A5F | stars_file_error / alerts.c |
| FBadFileError | 15:524E–52AD | stars_bad_file_error / alerts.c |

All use original far C calls with caller cleanup. FCreateFile, AlertSz and
FBadFileError return signed words through AX; the other two return void.
These routines extend [file streams](file-streams.md). OutputSz, PszFormatIds,
FSaveRace and FWasRaceFile remain unreconstructed dependencies/callers.

## Filename rules

SetSzWorkFromDt operates on the global base name at DGROUP:56A2, modifying it
in place. It removes the final dot and everything after it only if the dot
follows the last backslash, or if there is no backslash. A dot in a directory
component is retained. A forward slash has no special meaning here. This is
the original byte-string rule, not host filesystem path normalization.

The original `%s.` format first copies that base and a dot to szWork at 57A4.
It then chooses a suffix using the full unsigned type word:

| Type | Suffix |
|---|---|
| 1 | x followed by signed 16-bit player+1 |
| 2 | hst |
| 3 | m followed by signed 16-bit player+1 |
| 4 | h followed by signed 16-bit player+1 |
| 0 or any other word | xy |

Player addition wraps before signed decimal formatting: 32767 becomes -32768,
and -1 becomes 0. Type 5 does not automatically create a race extension; the
race-save caller supplies a forced filename. The fixed format/suffix strings
at 0A0C–0A1B are checked against the executable by the oracle.

## Creation and local recovery

FCreateFile uses the forced near filename if nonzero; otherwise it first calls
SetSzWorkFromDt. It saves penvMem from DGROUP:006E, points that global at a local
jump buffer, and establishes a setjmp recovery point. It calls StreamOpen with
mode 1012h, then WriteBOF(player,type,0). Success restores penvMem and returns 1.
An error that longjmps to that recovery point restores penvMem and returns 0.

There is no close on either return path. Success deliberately leaves the stream
open for following records. **A failed header write also leaves the successfully
opened handle open.** The tests exercise failure during the two-byte record
header and during the sixteen-byte BOF body; partial bytes and RNG changes are
retained. A failed open leaves the invalid handle from StreamOpen. No cleanup
or rollback has been invented in the reconstruction.

The host adapter establishes a real native setjmp/longjmp scope and delegates
the existing stream callbacks through it. Its additional `environment` argument
is a near-offset token used for the observable penvMem global, not a Win16 jump
buffer implementation. Direct original calls in the tests have BP=FEFE and
their local environment at FEE8, so the host uses that token when comparing API
observations. Both restore the supplied previous value at return.

Tokens must be distinct for active scopes and disjoint from application data.
If an abort is directed at a different token, the adapter delegates it to the
outer nonreturning abort callback. Raw Win16 jump-buffer layout, stack unwinding
through Windows, arbitrary reentrant callbacks and foreign-frame restoration
are outside the current equivalence claim. The native scope adds no heap
allocation and does not replace the original caller-visible return convention.

## Error classification and presentation

FBadFileError returns 1 only for ids 0, 1, 2, 3, 22, 28 and 29. Every other
signed 16-bit value returns 0. Its complete 65,536-value input space is compared
with original execution; it makes no data changes.

FileError always stores its argument at DGROUP:074E, even when silent. It calls
PszFormatIds(id,NULL), then AlertSz(text,16), only if the full word at 074C is
zero and bit 1 at 07CA is clear. Suppression skips formatting as well as display.
The formatted-message helper remains an explicit callback; the adapter does
not invent message text or skip its integration effects.

AlertSz reads the settings word at 229A. It chooses OutputSz when bit 14 is set,
or when bits 15 and 3 are both set. That branch formats the original `Error: %s`
prefix into a 258-byte local buffer. It passes channel 7 if bit 14 is set,
otherwise channel 6, and returns 6 regardless of the original requested dialog
type. Channel selection reloads the settings after formatting.

Other settings choose the MessageBox path. It obtains the current focus as the
owner, supplies the original caption at 1385 (`Stars!`), passes through the raw
dialog type, and returns the API result. There is no fallback owner selection.
OutputSz itself includes file/log and timestamp behavior; that work is still
pending and is represented by a synchronous output callback.

## Differential validation and limits

`tests/test_file_machine.py` builds the strict C89 library and compares original
instructions against the compiled adapters. It covers path punctuation, empty
and 255-byte base names, high-byte characters, type defaults, signed player
wrap, all error-classification ids, silent flags, dialog/log selection, bounded
text lengths, dialog returns, forced/generated filenames, open retry, timeout,
and both partial and failed header/body transfers.

Comparisons include application data below E000, output bytes, callback order,
arguments, live recovery tokens during OS calls, return values, stack balance
and preserved registers. Private original stack bytes are excluded: native
local buffers and jump buffers intentionally have different representations.

`tools/file_machine.py` runs unchanged FileError, AlertSz, FCreateFile, stream
code and original CRT setjmp/longjmp. Windows calls use synthetic handles,
messages, tick values and the verified `%s.`, `%c%d` and `Error: %s` formats.
PszFormatIds is explicitly replaced at its entry by a fixture pointer, and
OutputSz by a recorder of its channel and complete text. The new tests therefore
do not validate real message expansion, actual log files, Windows rendering or
OS filesystem behavior. The previous stream unit-boundary tests remain intact.

The native filename adapter requires the original static formats, a base name
terminated within its 256-byte buffer, and disjoint contiguous output storage
large enough for the formatted name. The alert's prefixed result, including
NUL, must fit 258 bytes; the tested maximum input text is 250 bytes. Callback
text is borrowed for the duration of the call. Inputs cannot alias the private
stack or immutable format tables. Protected-mode selector faults and overflows
remain outside these bounded adapters.

## Race-name initialization audit: U020

The [previous full-caller tests](file-streams.md) prove that nonzero unused name
bytes or player flag bit 3 can survive saving but fail the subsequent load
checksum. This pass inspected RaceWizardDlg1's original code to locate where
name storage is prepared:

* At NE 29:0DAA–0E00, a race-choice command clears each 32-byte global name
  field before GetDlgItemText fills it. The fields are 501A and 503A, or player
  offsets +128 and +160 relative to vplr at 4F9A.
* At 0E66–0EE3, the predefined-name replacement branch clears both fields before
  writing the embedded singular and generated plural names.
* At 0C99–0CC7, the page-exit branch reads both controls directly into those
  fields without a preceding clear in that branch. Immediately beforehand,
  selecting a predefined race can replace the entire player with a template;
  the custom selection retains the existing player.

This is a static control-flow observation. It does not yet establish which
unused bytes actual Windows controls leave after a user shortens a name, or
prove a live wizard/save failure. U020 remains open for the affected control
sequence, incoming player state, other initialization callers and flag bit 3.
No normalization has been added to hide the discrepancy.
