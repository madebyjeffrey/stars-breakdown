# Innate race habitability

`LInnateRaceHabitability`, NE 29:4C6E–54D4 (2,150 bytes), is reconstructed in
`reconstructed/src/planet.c` as `stars_innate_race_habitability`. The original
accepts a near player pointer and returns a signed long in DX:AX, using far C
caller cleanup. The host C89 adapter accepts a complete 65,536-byte data image.
The `stars_` name and helper names are reconstruction choices. Procedure and
local-variable evidence comes from `analysis/functions.json` and
`analysis/debug/procedures.json`; the original instructions, including x87
operations omitted from raw pseudocode, establish the algorithm below.

## State changes

The routine saves all 192 bytes of global player zero at DGROUP:59A2, reads
trait bit 1 from the supplied player, and copies that player into the global
slot. It checks each signed environmental center/low/high triple during each
pass. Values outside 0–100 cause replacement with (-1,-1,-1), except an existing
triple of all -1, which already means immunity. Repairs write high, low, center,
set bit 4 at player+84, and recopy the complete player into the global slot.
Ordered endpoints are not enforced by this check.

At exit, the saved global record is restored only if the supplied player was
elsewhere. Repairs to that supplied record persist. When the supplied pointer
is global player zero, repairs persist in that record instead. The adapter
preserves forward word-copy order; partial overlap between distinct records is
outside its supported domain. Tests compare complete records, including bytes
unrelated to habitability, and check restoration independently.

## Sampling and scoring

Three passes use allowances 0,5,15, or 0,8,17 with trait bit 1, and respective
weights 7,5,6. For each nonimmune axis the sampling range extends the original
endpoints by the allowance, clipped to 0–100. Eleven values are selected using
integer division: `start + index*span/10`. An immune axis samples 50 once and
later contributes a factor of eleven.

On passes 1 and 2, each nonimmune sample moves toward its center by up to the
allowance. The remaining **signed** center-minus-sample displacement is retained
for each axis. These three displacement variables are initialized once before
all passes. Their sum is not a sum of absolute distances.

Each resulting planet is scored using PctPlanetDesirability for global player
zero. When the displacement sum exceeds the allowance, subtract the excess
from desirability and clamp negative results to zero. Outside that branch,
negative desirability remains negative and is then squared. Multiply the square
by the pass weight and accumulate a signed 32-bit inner sum.

After the third-axis loop, multiply its sum by that axis's span and divide by
100 with integer truncation, or multiply by eleven for immunity. Add the integer
to a stored double row accumulator. After the second-axis loop, scale the row
by span/100 or eleven, then add it to a stored double pass accumulator. After
the first-axis loop, similarly scale the pass and add it to the stored double
total. Finally divide by 10.0, add 0.5, and truncate to long. Full immunity gives
23,958,000 in both implementations.

Important instruction anchors are 4C77–4CD5 (save, trait, copy, displacement
initialization), 4D35–4F56 (repair and ranges), 4F81–5318 (three loops, signed
penalty and weighted squares), 531B–538A (integer scaling and row addition),
538D–5486 (floating scaling/addition), and 5496–54D3 (restore and return).

## Validation and remaining limits

`tests/test_innate_machine.py` executes the unchanged original, its planet-scoring
callee and original arithmetic/CRT helpers using `tools/core_machine.py`. It
initializes x87 control word 037F and uses shared SS/DS for the original near
stack copies. Every call also checks far return, stack balance and SI/DI
preservation. Only synthetic records and original static constants are used.

Coverage includes all eight immunity combinations with both trait states,
narrow and clipped symmetric ranges, 120 deterministic asymmetric records,
global-zero and distinct input pointers, and 30 invalid-triple repair cases.
Rounding-boundary fixtures are also compared with original execution.

The host requires ordered nonimmune endpoints with strictly positive half-widths,
or invalid triples that the original repairs to immunity. U017 tracks the
original divide-by-zero case and unresolved caller guarantees. Records must be
valid 192-byte regions, disjoint from original CRT scratch, stack and code, and
must either be disjoint from global player zero or equal that record.

## Portable x87 rounding (U018 resolved for this routine)

A first implementation using native double/long double passed the initial cases,
but boundary searching exposed an actual one-point mismatch. For centers
(10,20,15), lows (4,9,6), highs (22,25,88), with trait bit 1 enabled, that version
returned 1,043,305 while the original returned 1,043,306. ARM64 long double has
only a 53-bit significand, so retaining C double stores alone was insufficient.
The failing input is retained as an explicit regression among nine fixtures
selected by scanning 300,000 synthetic host inputs (Python Random seed 27187) for totals near
`10*n+5`. That scan was candidate selection, not 300,000 original comparisons.

`reconstructed/src/innate_fp.h` now implements the necessary positive arithmetic
using four 32-bit integer limbs and a binary exponent. Addition, multiplication
and division round to 64 significand bits, nearest with ties to even; each
original double store separately rounds to 53 bits. Division retains the exact
remainder as a sticky bit. The final divide/add rounds at the original x87
boundaries before truncation. No native floating arithmetic remains in the
sampler, and intermediate double rounding is preserved rather than collapsed
into a single mathematical rounding.

This helper is deliberately bounded to the sampler: nonnegative finite values,
integer scaling factors at most 100, and divisions by 100 or 10. The weighted
sum cannot exceed `10000*11^3*(7+5+6) = 239580000`; its first integer scaling
is at most 77,000,000. Nonzero floating accumulations arise from integers
scaled by hundredths at most twice, so their magnitude is at least about 0.0001.
The resulting aligned sums fit within 128 bits. Multiplication needs at most
71 significand bits before rounding; division shifts its nonzero numerator to
128 bits and retains both quotient and remainder. There are no negative,
nonfinite, underflow or overflow branches within the documented input domain.

`tests/test_innate_fp.py` independently compares 1,000 sets of arithmetic
operations against exact rational nearest-even rounding, plus carry, tie, zero,
small-quotient and final-half-boundary cases. The machine tests then check the
complete algorithm against unchanged original x87 execution. This establishes
the implemented control-word-037F path, not a general x87 emulator, software-FPU
runtime, or every alternative floating environment. Win16 integration and U017
caller guarantees remain outstanding.
