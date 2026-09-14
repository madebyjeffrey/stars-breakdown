# Scenario-driven race acceptance

The user's proposed workflow is implemented as a generated scenario package:
one named set of wizard actions drives the recovered C, the original-instruction
oracle, and written recipes for independently recreating the races in DOSBox or
86Box. The native VC++ Windows adapter remains a separate pending task. This
package contains race files and instructions; it is not a Win16 executable.

The catalog contains **88 scenarios**, with **15 selected for the initial live
replay**. These cover every template, all ten primary and fourteen lesser traits,
each research field, all seven economy controls, environmental bands/immunity,
growth limits, portraits, allocation choices, passwords, navigation and U020.
The catalog is representative, not an exhaustive Cartesian product or a claimed
pairwise covering array. Some isolated trait choices overspend the Humanoid
baseline; their expected result is rejection followed by cancellation, not a
fabricated valid file. Further valid combinations can be added under new IDs.

## Generation and transfer

```sh
python3 tools/package_race_acceptance.py
```

The default outputs are `ftp-share/RACEACC1/` and `ftp-share/RACEACC1.ZIP`.
Generation refuses to overwrite either output, including prior observations.
Use `--output` with a fresh directory for another run. Unicorn's JIT requires
the same host permission as the existing original-instruction tests.

Each independent case starts with the New Race menu path in a fresh synthetic
application state. Inputs are edits, radio/checkbox notifications, combo choice,
navigation and mouse messages. Mouse locations are derived from rectangles
produced by the actual callbacks, separately in each implementation. No scenario
silently writes the requested final values into the player record. U020 preserves
state across its explicitly specified save/load/edit sequence.

The original executable's hash is checked. Both paths execute the six recovered
callbacks and full save path with modeled Windows services. Comparisons include
return values, complete application data below the private stack, drawing/control
events, scores and exact output bytes. Existing exclusions for the private CRT
floating result and the expired outer-caller rectangle pointer are retained.
Generated files are loaded by both reconstructed and original loaders with
password checking disabled for this structural check. The synthetic password
case separately exercises incorrect and correct confirmation in the wizard.

`MODEL` files come from original x86 instructions with modeled Windows services;
they are **not live DOSBox captures**. `CANDIDAT` files come from the reconstructed
C. `EXPECT.JSN` retains original outcomes, raw 192-byte player records, decoded
payloads and controlled inputs. The source catalog and recipes share case IDs.
`PILOT.TXT` gives the initial live actions; `ALLCASES.TXT` contains every case.
All transfer names use DOS 8.3 syntax, and text uses ASCII with CRLF line endings.

## Live acceptance and comparison

Restart the original game for each independent case and follow the pilot recipe.
The title-screen route is New Game (Alt+N), then Customize Race (Alt+C), keeping
the New Game race choice at Humanoid. This enters the wizard without creating
a world. The modeled runner uses the recovered New Race menu slice; live checks
also test whether the surrounding real caller supplies equivalent race state.
Record scores, messages and load outcomes in `RETURN/OBSERVE.TXT`. Keep original
live creations separate from the generated fixtures. Both file directions matter:
load `CANDIDAT` files in Stars!, and return independently created original files
for checking and loading in the reconstruction. Do not resave a generated file
and label it an independently recreated wizard case.

The DOSBox keyboard path is available for standard controls. The unresolved
automated pointer-positioning issue may affect portrait, band and spinner cases;
physical mouse operation works. If an 86Box check is needed, provide the user the
exact case instructions, expected result and named files to return. A callback
model match never substitutes for the pending real Windows input/rendering check.

```sh
python3 tools/package_race_acceptance.py --review ftp-share/RACEACC1 \
    --returned ftp-share/RACEACC1/RETURN
```

The review reads only allowlisted scenario filenames. It retains raw files,
decrypts each body with that file's own BOF state, and compares record order,
lengths and complete plain payloads, all decoded player bytes and both checksum
values. The file's clock/RNG-derived salt is the only automatically allowed
header difference. Other header differences are `REVIEW_HEADER`; payload or
checksum differences are `FAIL`. Missing files and absent observations do not
count as passes. The CLI returns nonzero until every listed file comparison is
PASS; intentionally fileless scenarios still require observation review.

A comparison PASS means agreement with the predicted file behavior. U020B has
an intentionally invalid checksum in the model, so matching it is a comparison
PASS with `checksum.valid=false`. Its real edit-control behavior still needs
live confirmation. No name tails or game-field differences are normalized away.

The inspector reuses the separately tested reconstructed codec. Its report is a
comparison aid, not a second independent native oracle, and it accepts only the
current-version BOF/player/checksum race format within writer bounds. Historical
versions and malformed-input fuzzing remain in the existing dedicated tests.

## Checks and remaining work

On 2026-09-14 all 88 scenarios (89 wizard sessions including U020's second
stage) matched the original-instruction model. They produced 61 file pairs:
60 reloaded successfully and U020B was rejected with the predicted checksum
mismatch. The other 28 sessions intentionally produced no file. Four new
acceptance tests and all thirteen existing wizard regression tests passed.

One live baseline, T000, has also completed both file directions in DOSBox-X /
Windows for Workgroups 3.11. The original wizard saved the named Humanoid race
with 25 points; its file had identical complete plaintext payload and decoded
player bytes to the prediction, with only BOF salt changing from 1429 to 68.
The original game reloaded that file, the reconstructed/original-instruction
loaders both accepted it, and a fresh original-game process loaded the generated
candidate file successfully. No candidate file was resaved as a live capture.
The [retained report](../../analysis/compat/race-acceptance.json) includes hashes
and the distinct modeled/live status. This does not complete the 15-case pilot.

The live Open dialog initially produced a missing-XY error when given the full
file path while displaying another directory. Navigating into the fixture
directory first and then opening the bare filename worked. This is a recorded
surrounding-UI observation, not a repaired serializer defect. After copying a
new candidate into the mounted host folder, DOSBox's Drive > C > Rescan drive
made it available to the guest. Automated mouse clicks still failed to select
radio controls; the live baseline used keyboard input.

```sh
python3 -m unittest tests.test_race_acceptance tests.test_race_ui -v
```

The acceptance checks exercise real original/C sessions, clock-dependent encoding,
deliberately changed checksums, an unapproved header difference, truncated input,
U020, dynamic mouse actions, ASCII/8.3 transfer integrity and preserved returns.
The complete catalog is also compared during package generation.

Remaining acceptance work is the rest of the live pilot, inspection of returned observations,
and the native VC++ adapter/build of these new source files. The subsequent
[compiler/output package](../racevc/README.md) now includes all recovered source
and an automatic C runner for the same 88 scenarios. RETURN3 compiled all source;
RACEVC2 addresses its linker failure. Native execution is pending, and the
modeled UI does not finish the live pilot. Later components can
use the same structure: named actions, original expected outcomes, candidate
artifacts, a small live sample and explicit unresolved differences.
