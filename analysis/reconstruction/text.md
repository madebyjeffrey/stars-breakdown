# Text and numeric utility reconstruction

Six routines in `reconstructed/src/text.c` add 896 reviewed, host-compiled
procedure bytes. Addresses below are NE ordinals and half-open offsets.

| Original | NE range | Host entry |
|---|---|---|
| AddBackTrailingSpaces | 9:280C–2841 | stars_add_back_trailing_spaces |
| ChopLastWord | 9:2842–28C6 | stars_chop_last_word |
| ChopTrailingSpaces | 9:28C6–28FC | stars_chop_trailing_spaces |
| IntToRoman | 9:5830–58D7 | stars_int_to_roman |
| PszGetLine | 9:68BA–6985 | stars_get_line |
| CParseNumbers | 9:6986–6AA5 | stars_parse_numbers |

All use far C calls with caller cleanup. Recovered signatures and source
mappings remain in `analysis/debug/procedures.json`, `analysis/source_lines.json`,
and `reconstructed/include/sfuncs.h`. New host entry names are inferred adapters.
The original functions' basic blocks were reviewed against the full instruction
export, including branches that skip work and returns after partial output.

## Exact behavior

The three space helpers edit a stored near-pointer word, rather than moving or
terminating text. Their bounds comparisons are unsigned. Only byte 32 is a
space: tabs, newlines and even NUL are not treated as whitespace. ChopLastWord
removes trailing spaces, a run of non-space bytes, then spaces preceding that
run. AddBackTrailingSpaces advances the cursor only over spaces before its end.
The reconstruction reloads pointer words so pointer/text aliasing is preserved.

IntToRoman writes an empty string for nonpositive signed inputs. Positive values
use only I, V and X: 40 becomes `XXXX`, and 100 becomes ten X characters.
It does not use L, C, D or M. The maximum positive 16-bit input is supported by
the original loop but needs a sufficiently large destination. This is a hull
name helper rather than a general Roman-numeral formatter.

PszGetLine skips leading literal spaces, scans for CR, LF or NUL, and replaces
that end marker with NUL. CRLF advances the caller's cursor two bytes; all other
end markers advance one, **including an existing NUL**. A caller must therefore
manage the end of its allocation instead of assuming the cursor stays at the
terminator. The selector part of its far pointer is retained while the offset
wraps to 16 bits. Cursor writes precede the terminating-byte write; this matters
when the cursor slot aliases the text.

CParseNumbers accepts only decimal digits and literal spaces. Leading and repeated
spaces are skipped. Each value is accumulated modulo 2^32; 4294967296 becomes 0.
It returns -1 on another byte, without rolling back already completed numbers.
An unfinished value is not stored on error. For example, `1 2x 3` stores 1,
returns -1 and leaves the other output slots untouched. A nonpositive maximum
returns zero without inspecting input. Once the maximum count is reached,
remaining input is not validated: with maximum 1, `12 x` succeeds with 12, while
`12x` fails before completing a value.

## Caller evidence

The full instruction export contains 14 direct PszGetLine calls and 12 direct
CParseNumbers calls, all in `GenNewGameFromFile` (NE 16). The first call sites
are 16:4942 and 16:4A01 respectively. This links the helpers to text-driven
game creation; the enclosing format and caller bounds are not yet reconstructed.
IntToRoman's direct caller is `DecorateHullName`, at NE 8:5F61.

The nearby PszFromInt/PszFromLong/CommaFormatLong routines were inspected but are
not counted in this pass: they depend on Windows formatting behavior that the
current isolated oracle intentionally does not stub.

## Validation and scope

`tests/test_text_machine.py` compares returns and the entire synthetic 65,536-byte
data image against original x86 execution. It covers hundreds of fixed and
deterministic random strings, whitespace distinctions, unsigned bounds, pointer
slot aliasing, invalid bytes, decimal overflow, partial writes, negative counts,
input/output overlap, CR/LF/CRLF, signed integer extrema, and offset wrap.
Original arithmetic helpers execute unchanged in the oracle; no runtime helper
is counted as reconstructed merely because the test calls it.

The memory adapter supplies one complete segment. Original far text sources must
refer to that supplied image; the selector is retained as metadata, without
selector lookup or protection checks. It excludes aliasing with code or the
function's private stack and requires each original word access to remain valid
within its segment. Strings must reach a relevant stopping condition; no new
iteration bound, trimming rule, overflow error or malformed-input repair is
introduced. Faulting or nonterminating original memory configurations are not
certified. These are internal semantic adapters, not a complete Win16 build.

Run `python3 -m unittest tests.test_text_machine -v` with the pinned Unicorn
installation and execution permission described in `core.md`.
