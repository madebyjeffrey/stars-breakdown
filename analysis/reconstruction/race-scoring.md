# Race advantage points and growth

Two reviewed functions in `reconstructed/src/race.c` add 2,169 original bytes:

| Original | NE 29 range | Host entry |
|---|---|---|
| CAdvantagePoints | 444C–4C6D | stars_race_advantage_points |
| PctTrueMaxGrowth | 65D4–662C | stars_true_max_growth |

Both use original far C caller cleanup and return a signed word in AX.
CAdvantagePoints takes a near player pointer. PctTrueMaxGrowth takes a signed
player index and computes DGROUP:59A2 + 192*index with near-offset wrapping.
Host entry names are reconstruction choices; signatures and locals are retained
in `analysis/debug/procedures.json` and `analysis/functions.json`.

## Scoring sequence

CAdvantagePoints first calls BoundsCheckPlayer, reads primary race stat 14, then
calls LInnateRaceHabitability and divides its long result by 2,000. Those callees
are reconstructed and execute in full in the differential tests. No scoring
callee is replaced with a preset answer.

Growth is read as a signed byte at player+25. BoundsCheckPlayer already caps it
at 20; the scoring routine changes remaining out-of-range values to 1 and sets
the correction flag. Its retained defensive upper check also assigns 1, although
the preceding bounds check makes that branch unreachable on ordinary memory.

Start with 1,650 points. For growth 1–5 add `(6-growth)*4200`. Growth 6,7,8,9 gets
additional 3,600, 2,250, 600, 225 respectively. The habitability multiplier is
raw growth for 1–5, `2*(growth-5)+5` for 6–13, `3*(growth-13)+21` for 14–19, and
45 for 20. Subtract `floor((innate/2000)*multiplier/24)`, preserving the separate
integer divisions.

Nonimmune environmental centers add four points per unit away from 50. Two or
three immune dimensions incur a 150-point penalty. If stat 1 or stat 3 exceeds
10, subtract `max(stat3-9,1)*max(stat1-9,1)*primary_factor*raw_growth/divisor`,
where primary_factor is 3 for primary 0 and 2 otherwise, and divisor is 2 for
at least two immunities or 9 otherwise.

Stat 0 values 7,8,9 incur costs 2,400, 1,260, 600. Values above 10 add 120 per
step. Primary 8 adds 210 and skips both following production subtotals and the
trait-31 cost. Other primaries use:

* Let a=10-stat1, b=10-stat2, c=10-stat3. Add a times 100 when positive or 121
  otherwise; subtract b times 55 when negative, otherwise b squared times 60;
  add c times 40 when positive or 35 otherwise. Compress a subtotal above 700
  to `700+(subtotal-700)/3`. Further penalties begin at stat3=17,22,25 and
  stat1=13, as retained explicitly in the reviewed source. Trait 31 costs 175.
* Let a=10-stat4, b=3-stat5, c=10-stat6. Add a times 100 when positive or 169
  otherwise; add `80-65*b` when b is nonpositive or subtract 360 otherwise;
  add c times 40 when positive or 35 otherwise.

Then apply three original **CS-relative** tables. Raw Ghidra output incorrectly
presents these as fields inside ship-definition structures; their addresses and
original words establish independent tables:

| Table | NE 29 location | Signed words in index order |
|---|---|---|
| Primary cost, subtracted | 4410 | 40,95,45,10,-100,-150,120,180,90,-66 |
| Trait 0–13 adjustment, added | 4424 | -235,-25,-159,-201,40,-240,-155,160,240,255,325,180,70,30 |
| Research credit | 4440 | 150,330,540,780,1050,1380 |

Enabled traits are counted separately by the sign of their table adjustment.
More than four traits cost `count*10*(count-4)`. An excess of positive over
negative adjustments beyond three costs 60 per excess trait; the reverse costs
40. Trait 10 additionally costs 280 for primary 6, 200 for primary 1, or 40 for
primary 9.

Sum `(stat-1)` across stats 8–13, giving research balance -6..6. Positive balance
costs `balance^2*130`, with rebates 1,430 at 6 and 520 at 5. Negative balance
indexes the credit table; balances -5 and -6 cost another 190 if stat0<10.
Trait 29 costs 180. Primary 8 with stat8=2 costs another 100. Finally divide the
signed long point total by three, truncating toward zero, and return its low
word as signed. The adapter makes negative truncation explicit for C89.

## Growth helper

PctTrueMaxGrowth sign-extends the raw growth byte without validating or repairing
it. It doubles that value only when primary stat 14 is zero. Even negative and
out-of-normal-range bytes retain that behavior; the result fits a signed word.

## Validation and caller contract

`tests/test_advantage_machine.py` covers every growth-byte representation, stat
boundaries and repairs, production threshold combinations, all 729 research
combinations, trait counts and primary-specific interactions, 100 mixed random
environment/production cases, and global-zero versus distinct player records.
It also checks growth across all byte values, multiple primary values and
wrapped signed player indices. Complete affected player records are compared.

An original-machine test proves that CAdvantagePoints itself can reach the
existing divide-by-zero at NE 10:6F35 after BoundsCheckPlayer leaves an equal
low/high endpoint. This narrows U017: this caller does not add a nondegeneracy
guarantee. The C host adapter requires immune ranges or positive half-widths
after the initial bounds correction; it does not invent an error score.
Records must satisfy the documented habitability memory/FPU contract. Score
subtotals stay within their original signed 16-bit ranges after stat validation;
long totals stay within signed 32-bit range. Win16 integration and the handling
of malformed races at higher-level entry points remain outstanding.
