# Message expansion and naming paths

`format.c` implements PszFormatString (NE 7:85CC–8F67), PszFormatIds
(7:924C–9278), PszCalcEnvVar (10:5FCC–6057) and PszCalcGravity
(10:6058–60E6). The parameter stream contains little-endian words; strings
and pointed-to records must fit the original storage contracts. The formatter
returns DGROUP:535A. Its text, name and working caches remain observable.

| Escapes | Meaning |
|---|---|
| E, e | Formatted environment value; environment-axis name |
| F, s | Fleet design-word name; full fleet name |
| G, I, j | Compressed game-string selections |
| L, l | Player name with capital/article/plural/grammar flags |
| M, m | Object/mineral labels; m also remembers the unit |
| O, S, Z | Owner, conditional ownership suffix, or player-set listing |
| P, i, u | Percentage, signed word, unsigned word |
| U, V, v | Signed 32-bit value with remembered, explicit, or absent unit |
| X | Consume and discard one parameter |
| f, h, r, t, y | Base filename and original player/type suffix |
| g, k | Thing name or component name |
| n, o, p | Location/object/planet naming |
| w, z | Current work buffer or ship/starbase design name |
| other | Copy the escaped byte literally |

Parameter advancement is part of the behavior: V reads its unit word without
consuming it; zero Z consumes no parameter; filename branches that use the
number formatter advance by one word. The r suffix is `.h%d` in this binary.
Percentages below the integer threshold use the original remainder formatting,
including signed-word cases. Numeric conversion explicitly preserves 16/32-bit
widths on a host with a different C `long` width.

Planet names use the original nibble-coded NE 9 tables and capitalize at word
boundaries. IDs above 998 are reduced modulo 999. The cache marker at 137A is
a **signed byte**, tested but not updated by the routine; the neighboring byte
belongs to another object. All 999 names and signed cache hits are checked.

The formatter contains private paths for LpthFromId, LpflFromId, planet/fleet/
thing names, fleet primary-design selection and coordinate naming. It preserves
the sorted fleet search, player prefix rules, named-fleet fallback, mixed-design
suffix, special hull tie handling, thing type/owner fields, and ship versus
starbase design tables. These paths are validated through original formatter
execution, not credited as twelve extra complete procedures.

FLookupPart's return status is unused by the component-name escape. Its
availability queries do not mutate game globals. The recovered formatter
therefore selects the same component pointer directly from the original NE 2
table and copies its name. Every valid entry in all sixteen tables is compared
while the original executes FLookupPart, FShouldPartBeHidden and TechStatus.
This is a caller-specific reduction; those general availability routines still
need separate reconstruction for other game subsystems. PszGetLocName likewise
needs only the fixed grobj=0/id=-1 path here, and primary-design selection always
receives a non-null count output. See [the audit](race-dependencies.json).

Inputs must use valid component IDs, valid owner/design/fleet storage and
terminated byte strings within the original message/name buffers. Invalid
selector faults, arbitrary name overflows, unbounded negative name-table IDs
and corrupt-memory execution are outside this comparison claim. In particular,
this code does not implement a new file-format sanitizer.

`tests/test_race_format.py` tests every escape category, parameter advancement,
signed boundaries, units, all environment endpoints, player grammar, object
and owner branches, every valid component entry, and all planet-name entries.
Application state is compared as well as the displayed output. The component
and OS-model limitations are recorded in [race-component.md](race-component.md).
