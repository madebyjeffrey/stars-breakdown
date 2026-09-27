# Game-creation dependencies and text-format recovery

This pass adds seven reviewed C89 routines in `reconstructed/src/create.c`,
covering 829 original procedure bytes. `GenNewGameFromFile` is now covered by
the separate semantic reconstruction described in
`analysis/reconstruction/gen-new-game-from-file.md` and counted in the formal
ledger below.

## Earlier pre-review semantic source

The two files below were partial at this checkpoint. Both are now reviewed
in the current [TODO5 ledger](../../TODO5.md); their later acceptance evidence
is linked from those entries.

`reconstructed/src/new_game_dlg3.c` is a genuine C reconstruction of
NewGameDlg3 (NE 16:99f4–9d4c). It keeps the page's paint and erase behavior,
control-color handling, cursor hit-testing, seven victory-condition control
updates, race-button result mapping, read-only disabling, help dispatch, and
mouse tracking as explicit typed operations. The source probe covers those
message paths and the native VC1.52c probe passed all 10 checks with zero
failures (`runs/vmctl-vc152ew/new-game-dlg3/NG3RES.TXT`). It was separate
from this checkpoint's reviewed count while the real wizard state and callers
were being integrated.

`reconstructed/src/init_new_game_plr.c` is a genuine C reconstruction of
InitNewGamePlr, then kept separate from the reviewed count while integration
was pending. It makes the original map-size and AI-level player-count choices
explicit, assigns the corresponding computer-player template bands, and
preserves the original RNG call order during the template shuffle. The
callback boundary represents the original SetVCVal and Random services while
the host adapter is still being integrated. The source probe covers the
map-size/difficulty branches, step gating, victory-value update, template
selection, and player-zero preservation. VC1.52c under the Win98 StarDev
environment compiled and linked the source, and the native probe passed all
8 checks with zero failures (`runs/vmctl-vc152ew/init-new-game-plr/INPRES.TXT`).

## Reviewed routines

| Original | NE 16 range, half-open | Host entry |
|---|---|---|
| InitBattlePlan | 00C0–0136 | stars_init_battle_plan |
| InitNewGame3 | 74F8–7507 | stars_init_new_game3 |
| LpplrComp | B570–B5A7 | stars_computer_player_pointer |
| SetVCCheck | B5BC–B60B | stars_set_victory_check |
| GetVCCheck | B60C–B644 | stars_get_victory_check |
| SetVCVal | B644–B70F | stars_set_victory_value |
| GetVCVal | B710–B83F | stars_get_victory_value |

Original signatures and source mappings are retained in
`analysis/debug/procedures.json` and `reconstructed/include/sfuncs.h`.
These are far C calls with caller cleanup. The host uses explicit memory and
selector values rather than native far pointers.

Victory fields occupy bytes at game+20+index. Their top bit is the enabled flag;
the remaining seven bits encode a value. SetVCCheck treats any nonzero argument
as enabled and preserves the value bits. GetVCCheck returns exactly zero or one.

| Index | Stored maximum | Decoded value |
|---:|---:|---|
| 0 | 16 | raw × 5 + 20 |
| 1 | 18 | raw + 8 |
| 2 | 4 | raw + 2 |
| 3 | 19 | raw × 1000 + 1000 |
| 4 | 28 | raw × 10 + 20 |
| 5 | 49 | raw × 10 + 10 |
| 6 | 29 | raw × 10 + 10 |
| 7 | 87 | raw × 10 + 30 |
| 8 | 7 | min(raw, enabled-condition count) |
| 9 | 47 | raw × 10 + 30 |

The maximum table is original CS:B5A8–B5BC. Negative setter inputs clamp to zero.
Nonzero GetVCVal's final argument requests raw values without conversion.
Decoded arithmetic returns the signed low word, preserving overflow for bytes
outside the setter's normal range. Index 8 and out-of-range getter indices count
enabled fields 0–7, excluding field 2, in the **global** game at DGROUP:0070.
They do not count the argument record's flags. SetVCVal also applies this global
count when setting index 8, and preserves its enabled bit.

InitBattlePlan copies one of five 36-byte templates from CS:000C–00C0, replaces
the low four bits of its first word with the requested plan ID, and conditionally
changes the next word when the global fSinglePlr flag is set and template zero
was chosen. The global flag is read after copying, including when output aliases
the global record. Template bytes are retained in `battle_plan_templates.inc`.
Raw bytes `8e d9` at NE 16:00DB load DS from CX (which holds CS); the text export
prints misleading operand order. Tests execute the raw instruction and confirm
the code-segment source. This discrepancy is tracked as U016.

LpplrComp returns CS:(A370 + race×768 + level×192), with 16-bit offset wrap and
no bounds check. The intended six races have four 192-byte difficulty variants
each. The pointer helper is reconstructed; the full template semantics and
player initialization remain later work. InitNewGame3 is genuinely empty in
the original, containing only frame/register setup and return. Its empty C body
is a recovered behavior, not a placeholder.

## GenNewGameFromFile: recovered format outline

The procedure occupies NE 16:4812–5E5E (5,708 bytes). It clears the 64-byte global
game record, reads a text file into allocated memory, and uses the already
reconstructed PszGetLine/CParseNumbers helpers. Leading literal spaces are
skipped; trailing spaces are retained. Numbers accept digits and spaces only,
wrap at 32 bits, and stop parsing once the requested count is satisfied.
This is not a strict modern whitespace-separated format.

The observed line order is:

1. Game name: nonempty, at most 31 bytes after leading spaces are skipped.
2. Up to four numbers: size 0–4, density 0–3, start distance 0–3, optional RNG seed.
   Missing entries retain cleared values. The optional seed calls Randomize.
3. Up to seven Boolean numbers, in recovered field order:
   fExtraFuel, fSlowTech, fBBSPlay, fNoRandom, fAisBand, fVisScores, fClumping.
   Each supplied number must be 0 or 1; these names come from debug structure
   metadata, not guesses at user-interface labels.
4. Player count, 1–16.
5. One line per player: a race-file path, or (after the first player only)
   `#race difficulty`. Computer values are 0–6 and 0–4, where zero requests a
   random choice. Nonzero inputs are reduced by one. These two range checks use
   the low signed 16 bits of the parsed numbers, a difference from earlier
   full-32-bit checks that must be preserved in implementation.
6. Eight victory-condition lines, detailed below.
7. Output base name. A lowercase `.xy` suffix on names longer than three bytes
   is stripped. Optional trailing universe-definition handling is not yet
   resolved; do not treat this outline as a complete format specification.

The victory lines use the following bounds and conversions. All upper bounds
shown are inclusive, checked in the original branches. Names for their gameplay
meaning await UI/resource cross-checking; indices are authoritative here.

| Line | Enabled input | Value bounds when enabled | Writes |
|---:|---|---|---|
| 1 | 0 or 1 | 20–100 | field 0 = (value−20)/5 |
| 2 | 0 or 1 | 8–26, then 2–6 | fields 1 = value−8; 2 = value−2 |
| 3 | 0 or 1 | 1000–20000 | field 3 = (value−1000)/1000 |
| 4 | 0 or 1 | 20–300 | field 4 = (value−20)/10 |
| 5 | 0 or 1 | 10–500 | field 5 = (value−10)/10 |
| 6 | 0 or 1 | 10–300 | field 6 = (value−10)/10 |
| 7 | 0 or 1 | 30–900 | field 7 = (value−30)/10 |
| 8 | count 0–7 | 30–500 when count is nonzero | field 8 = count; field 9 = (value−30)/10 |

The first seven lines enable their associated field(s) through SetVCCheck;
disabled lines do not require values. Integer divisions truncate: values need
not be exact multiples of the step size. Numerous cursor-versus-file-end checks
require following text; a faithful parser must preserve their exact placement.
The initial file-length check narrows the result to a signed 16-bit word before
comparison with 15,999, so it must not be assumed to be a general safe file-size
guard. These malformed-input and end-of-file paths still need execution tests.

After parsing, the original loads or copies races, checks advantage points,
fills missing and duplicate race names, resolves duplicate player bitmap IDs,
and calls GenerateWorld(1). RNG call ordering is observable. Stream management,
allocation/nonlocal error recovery, alerts, race loading, world generation, and
cleanup remain unresolved dependencies for reconstructing the full procedure.
No fake implementations of those dependencies were added.

The reviewed source keeps those operations behind typed callbacks and does not
pretend that the callbacks are a complete replacement for the surrounding
game-global storage.

## Validation and limits

`tests/test_create_machine.py` compares memory and return values with original
NE 16 execution. It tests all stored byte values; all 256 global enabled-bit
combinations; raw/decoded values; signed extrema and clamping boundaries; valid
battle templates with aliased globals and wrapped destinations; and computer
pointer arithmetic including out-of-range inputs. The oracle now loads NE 16
at a separate selector; unavailable imports still fail rather than being stubbed.

Setter indices must be 0–9 and battle template indices 0–4; invalid indices read
unrelated original code-segment storage and are outside this host adapter's
contract. Other memory/selector limits are as in `core.md`. Embedded table data
The parser's range is credited only after its host source probe and native
VC++1.52c probe pass; whole-game race loading and world generation remain
separate integration work.
