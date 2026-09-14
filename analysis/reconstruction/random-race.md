# Random race creation

`CreateRandomRace`, NE 29:5B08–65D4 (2,764 bytes), is reconstructed in
`reconstructed/src/race.c` as `stars_create_random_race`. It takes a near pointer
to an existing 192-byte player record, mutates that record and returns void via
a far C call. Local names, offsets and the recovered signature are retained in
`analysis/functions.json` and `analysis/debug/procedures.json`.

The host implementation uses the reconstructed RNG, bounds/stat/trait helpers,
innate habitability, advantage-point scoring, and embedded string decoding.
Original static defaults and string tables are supplied in memory images. It
does not execute original code. The comparison oracle executes all original
callees, including the original strcmp/strcpy and arithmetic helpers, unchanged.

## Initial generation

Clear the byte at player+160, then draw Random(25). Outcomes 0–3 produce three
immune axes; 4–6 produce full 0–100 ranges. Outcomes 9–24 choose widths from 20
to 98 in increments of two, and an allowed low endpoint. Within that group,
9–11 replaces a random axis with immunity, 12–13 with the full range, and 14–16
with width 20 at a new random position. Growth uses different draw ranges for
the different groups, preserving each call and its order.

**Outcomes 7–8 retain an unusual original behavior.** On each axis a Random(2)
choice of zero installs the full range. A nonzero choice instead consumes
Random(4) and changes growth, leaving the incoming axis unchanged. That growth
value is overwritten by another draw after the loop. On the third axis, if the
first two centers are equal, the choice becomes whether the first center is
nonzero. The instructions at 5C91–5CA3 confirm the growth write. Changing this
branch to initialize immunity would alter both behavior and the RNG sequence.
The input therefore needs an initialized player record, not empty storage.

The next draws choose research costs (stats 8–13), primary type (stat 14), low
14 trait bits, then trait bits 29 and 31. Other trait bits remain as supplied.
Production stats 0–7 are either chosen from the original min/max tables or
copied from the first default record (stats 0–6) with a separate draw for stat 7.
The live default record begins at DGROUP:0DA2; its production fields start at
0DE0. The adapter does not replace that data with inferred modern defaults.

The player name at +128 is compared with embedded string 0x532. Only a matching
placeholder is replaced, using one of 24 strings beginning at 0x56E. A supplied
name is retained. The plural-name byte at +160 was already cleared.

## Point adjustment

Score the generated record and return immediately when its points are 0–50.
Otherwise start a loop that re-scores at its head. Distance from the target is
`max(-points,points-50)`, so successful moves must strictly reduce that value;
equal-distance changes are rejected.

Each trial chooses one of these categories with Random(10):

* 0–2: change one research stat, trying decrement before increment.
* 3–5: choose a low trait bit and try disabled before enabled.
* 6–8: choose a production stat and try old value minus one before plus one,
  using the clamping stat setter for both trials.
* 9: randomly try growth (decrement before increment, upper trial limited to 15)
  or change one environment between immunity and a randomly placed width-70
  range.

The original uses different rollback scopes. Scalar trials restore their old
field when rejected. Changing an immune axis to a range restores its triple if
rejected. Changing a range to immunity saves the entire player and restores all
192 bytes on rejection. The reconstruction retains these choices, including any
bounds-correction side effects from scoring between attempts.

After 251 unsuccessful trial iterations, the next loop check falls back to the
first default record. All 192 bytes are replaced, then the generated or supplied
singular name is restored from the saved record. It returns without re-scoring
that fallback. The counter uses a post-increment comparison with 250; this is
not a 250-attempt limit.

Instruction anchors: 5B08–5E59 initial environments/growth; 5E59–6003 research,
traits and production; 6003–606D names and initial score; 6072–60DA loop/fallback;
60DA–6223 research trials; 6223–62D1 trait trials; 62D1–6383 production trials;
6383–6455 growth trials; 6455–65D4 environment trials and return.

## Validation and scope

`tests/test_random_race_machine.py` compares complete seeded generations with the
original and verifies the resulting player, global player zero, default record,
RNG words, string-cache marker and string scratch. Seeded cases cover all initial
branch families, retained and placeholder names, global-zero and distinct input
records, and adjustment/rejection paths. A separate zero/zero RNG case drives
repeated choices through the actual 251-attempt fallback without replacing any
callee or editing the original instruction stream. Branch hooks count attempts;
the original has a larger bounded instruction budget for this full-chain test.

The input must be an initialized, nondegenerate race with a NUL-terminated
singular name within its 32-byte field. Source/destination records, default data,
RNG state and shared string scratch must have the documented disjoint layout.
Scoring's control-word-037F and valid-environment contract applies. Arbitrary
corrupt default data, stack aliasing, invalid retained ranges and Win16 selector
faults are outside the host adapter. The generator is now runnable as host code;
this does not implement the race wizard UI, race-file loading/saving or a full
Windows application.

## Running the reconstructed code

From the project root:

```sh
python3 tools/race_demo.py --seed 0
```

This builds the C89 library and prints a synthetic race as JSON, using only
reviewed host code plus static template/string data from the verified original.
No emulator or Windows installation is required. Seed 0 with the default name
placeholder produces **Golem**, with **25 advantage points**. The demo reads no
user-created race files and writes no generated races into the runtime setup.
