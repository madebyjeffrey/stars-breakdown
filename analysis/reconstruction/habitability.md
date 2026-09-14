# Planet habitability and innate-race scoring

`PctPlanetDesirability`, NE 10:6E1E–7095, now has reviewed C89 source in
`reconstructed/src/planet.c` as `stars_planet_desirability`. It contributes 631
original procedure bytes. `LInnateRaceHabitability` and `CAdvantagePoints` remain
unreconstructed; no coverage is claimed for their partial analysis below.

## Planet desirability algorithm

The original takes a far planet pointer and a signed player index, returning a
signed word through AX with far C caller cleanup. Planet environment bytes start
at planet+12. The player record is selected from DGROUP:59A2 with stride 192;
its centers, low endpoints, and high endpoints begin at offsets 16, 19 and 22.
All environment bytes are sign-extended. The host adapter supplies one complete
data-segment image in place of far-pointer resolution.

For each of the three dimensions:

* A negative high endpoint means immunity. It contributes a perfect squared
  score of 10,000, regardless of the other endpoint bytes.
* A planet outside the accepted range adds distance to the nearest endpoint,
  capped at 15, to a hostile-condition total.
* Inside the range, let distance be the absolute distance from the center and
  width be the half-range on that side. The score is
  `100 − floor(distance × 100 / width)`; its square is added to the sum.
* Beyond the halfway point of a half-range, an additional penalty is applied.
  With `edge = 2×distance − width`, the running penalty becomes
  `floor(penalty × (2×width − edge) / (2×width))` when edge is positive.
  Penalty starts at 10,000 and each dimensional division occurs separately.

If any hostile-condition total exists, the routine returns its negative value,
down to −45. Otherwise it computes
`floor(sqrt(squared_sum / 3) + 0.9)`, multiplies that integer by the accumulated
penalty, and divides by 10,000 with truncation.

The **0.9 bias** is the original double at DGROUP:1D36; the divisor 3.0 is at
1D2E. This is not ordinary rounding to nearest. For centered ranges 25–75 and
planet values (25,25,49), the original returns 14; using 0.5 incorrectly gives 13.
This difference was found by differential testing and is retained as a regression.

## Removing the host floating-point dependency

For the documented valid domain, the squared sum is an integer from 0 to 30,000.
The host implementation finds the result by comparing it with the thresholds
`sqrt(sum/3) >= n + 0.1`, equivalently
`100×sum >= 3×(10×n+1)^2`. Equality cannot occur: the right side is not divisible
by 100. Thus the mathematical rounding rule can be evaluated entirely with
32-bit integer arithmetic, without a host sqrt implementation. The other
arithmetic stays within 32-bit bounds for valid environmental ranges.

Original x87/CRT execution, rather than this algebra alone, is the comparison
oracle. The tests initialize x87 to control word 037F and execute the original
sqrt, floating conversion, multiply and divide helpers unchanged. No math or
Windows imports are stubbed. NE 10 was added to the isolated loader at its own
selector. CRT scratch and stack changes are distinguished from the unchanged
application records in comparisons.

## Tests and limits

`tests/test_habitability_machine.py` tests the center and hostile extremes,
2,197 grid combinations around range boundaries, all immunity combinations,
2,000 random asymmetric ranges/planets over all sixteen player slots, 2,000
additional in-range cases, narrow ranges, and near-offset wrap. It compares
return values and verifies that the original leaves the input records unchanged.

The host API requires valid 0–100 nonimmune ranges with the center between the
endpoints and a nonzero selected half-width. Negative high endpoints select
immunity as described above. Application records must not overlap CRT scratch,
private stack or code. Arbitrary corrupt-memory behavior, selector faults and
other x87 configurations are not certified by these comparisons.

A degenerate range with center=high=planet value causes the original IDIV at
NE 10:6F35 to raise a divide exception. This is checked separately without calling
the C adapter outside its nonzero-width contract. No fallback score or silent
repair has been introduced. The fact that BoundsCheckPlayer can leave degenerate
ranges means caller guarantees still need auditing before integration.

## Innate-race calculation: next implementation boundary

Static review of `LInnateRaceHabitability` (NE 29:4C6E–54D4) establishes:

* It saves global player zero, copies the supplied player into that slot, and
  restores the saved record only when the supplied pointer differs from it.
* It samples three terraforming scenarios: adjustments 0, 5, 15 normally,
  or 0, 8, 17 when race trait bit 1 is enabled. Their score weights are 7, 5, 6.
* Nonimmune dimensions use eleven samples; immune dimensions use one sample
  and compensating factors of eleven. PctPlanetDesirability supplies each sample.
* It can replace invalid environmental triples with immunity and set the
  correction flag. This is another mutating calculation.
* It combines integer inner sums with stored double accumulators and finally
  divides by 10.0, adds 0.5, and converts to long. The constants at DGROUP:1DD2
  and 1DDA differ from the planet routine's 0.9 bias.

The raw decompiler output omits important floating accumulator operations, so it
is insufficient as source. Remaining work is to reconstruct the full loop and
state-restoration sequence, preserve each integer/double rounding boundary, and
compare it under original x87 execution before connecting advantage-point scoring.
