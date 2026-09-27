# SO-E3 Gate 2 — main RNG provenance audit

Status: **G2-13 closed for the pinned pair, 2026-09-25.**

Scope: the complete seeded control/action coordinator replays for
`vmctl-sye3k0` and `vmctl-sye3k1`. This is not a proof for arbitrary games or
an independent historical clock capture.

## Acceptance and evidence

| Requirement | Current evidence |
|---|---|
| One correctly initialized main state per run | `main` invokes source `stars_randomize2` once with the previously recovered effective low-14-bit startup tick. Both seed words are retained as the initial continuity checkpoint. |
| Every runtime draw is source-generated | `turn_main_random` calls `stars_random` on `trace.seeds`. All ordinary RNG callbacks call this wrapper. The five BOF writers execute their own source Random(2000), transferring both seed words into and out of writer storage. |
| Continuity across services | `turn_audit_main_draw` checks the pre-draw seed pair equals the preceding post-draw pair, including the first draw. A separate seed copy verifies precisely one source advance and its return value. The copy never supplies runtime state or values. |
| Complete original call order | The generated expected transcript flattens the retained original phase audit into all 57 control and 39 action limit/value pairs. Each runtime draw is compared at its exact index. Missing, extra, reordered, or different-limit calls fail. Final count and both final seed words are checked immediately after the coordinator returns. |
| No scoped witnesses feed the seeded route | `scrap_tech_random`, `random_value`, AI preparation/names, mining, event, wormhole, guess, and saved-flag callbacks use the same source wrapper. The separate 100 nonseeded scout cases retain their explicit witnesses; seeded execution cannot use that branch. Removed the seeded scout-witness storage; its two assertion consumers now compare explicit expectations or the recorded draw. |
| Writer RNG is not reset | Both startup and all three final BOF writers take the evolving main seeds and return the advanced pair; each is recorded in the same transcript with limit 2000. |
| Independent crypto/metadata state does not perturb main RNG | File XOR state uses record storage offsets 0x2272/0x2276; main writer state uses 0x1370/0x1374. Archived-INI decode uses a distinct local `StarsSeeds`. Source read/write log round trips preserve the main transcript. |
| Original output order | Saved-flag Random(8) occurs after scores/design refresh/backup moves and before output BOFs. All five full BOFs match original records. |

## Complete transcript by phase

| Phase | Control positions | Action positions |
|---|---:|---:|
| AI design names | 1–4 | 1–4 |
| Scout and starbase upgrade | 5–6 | 5–6 |
| X2/H2 BOFs | 7–8 | 7–8 |
| Player-log shuffle, including Random(1) | 9–10 | 9–10 |
| Task-5 technology | 11–30 | 11–12 |
| Mining | 31–36 | 13–18 |
| Event gates | 37–39 | 19–21 |
| Wormholes | 40–51 | 22–33 |
| Guesses | 52–53 | 34–35 |
| Saved flag | 54 | 36 |
| M1/M2/HST BOFs | 55–57 | 37–39 |

The existing service guards and closed package audits establish zero calls in
inapplicable branches (including one-planet AI shuffle, turn-zero punishment,
race-gated services, and absent minefield/packet work). Any newly eligible
unsupported branch fails explicitly. Later changes that add a missing service
must retain this audit and can reopen this package.

## External replay inputs and limits

Startup effective ticks (13747 control, 9953 action) were recovered by exhaustive
matching of the original AI-name sequence; they are equivalent initialization
inputs, not claimed wall-clock timestamps. BOF clocks are low-11-bit residues
recovered as `(original salt - recovered Random(2000)) mod 2048`:

- Control: 1560, 1560, 1654, 1656, 1657.
- Action: 1858, 1859, 1952, 1954, 1956.

Those residues are explicitly external replay inputs. They affect file salts,
not gameplay draws, and do not prove the original full clock readings. Expected
BOF bytes and expected transcript values only assert generated results.

The audit stops at coordinator return. Subsequent deliberate perturbation tests
operate on copies and are outside the original turn transcript. Whole callback
coverage, global-state lifetime, and all output streams remain separate package
or gate obligations; this RNG audit cannot substitute for them.

## Verification

`python3 -m unittest tests.test_satisfy_orders_turin_binding -q`

**PASS 1/1, 12.898 seconds**: both seeded runs and 100 separate nonseeded scout cases. All 57/39 draw pairs, seed continuity, and five exact BOFs pass.
