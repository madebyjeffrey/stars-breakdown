# Stars! Player's Guide as reverse-engineering evidence

The exported help file is a behavioral reference for the 2.7-era Stars! build
under study. It is not source code and it is not sufficient to establish the
exact implementation of a rule, but it is unusually useful for recovering
names, units, UI state, file boundaries, formulas, and the intended meaning of
many otherwise opaque fields.

The unmodified export is in [`help-file/out`](../../help-file/out/). Start with
the help-file's [`The Guts`](../../help-file/out/t10002.html) page, the
[full topic index](../../help-file/out/index.html), the
[keyword index](../../help-file/out/keywords.html), and the
[help-context map](../../help-file/out/contexts.html).

## Export inventory and provenance

| Artifact | Count / fact | RE use |
|---|---:|---|
| Topic HTML pages | 415 | Human-readable behavioral evidence |
| Topic pages with context metadata | 413 | Direct mapping from a help topic to a WinHelp context and original topic offset |
| Bitmap illustrations | 129 | UI screenshots, graphs, and the technology-statistic tables |
| `index.html` | 1 | Contents/index navigation and canonical topic titles |
| `keywords.html` | 1 | Alternate terminology and cross-topic search vocabulary |
| `contexts.html` | 1 | Dialog/control help IDs mapped to topics |

Two topic files are exceptional: `t3309d6.html` is an untitled/duplicate
wizard page, and `t4a89c3.html` is a bare topic-offset placeholder. They have
no `Help context ...` line in the export. Do not infer a missing game feature
from either file.

### Topic naming and offsets

Most files are named `t<hex-topic-offset>.html`. The page repeats that offset,
for example [`Movement, Initiative and Firing in Battle`](../../help-file/out/t418589.html)
contains `topic offset 0x418589`. This is a convenient provenance key when
comparing help calls or recovered constants. The HTML filename is not a
function or resource ID.

The context number is separate from the topic offset. For example, the same
movement page has context `0x7de`; the [context map](../../help-file/out/contexts.html)
shows which dialog/control asks for that help. Preserve both numbers in notes.

## How to use this evidence

Treat each help statement as one of three evidence classes:

1. **Contract** — names, units, visible ranges, file suffixes, button behavior,
   and user-visible sequencing. These are strong clues for interfaces and
   state transitions.
2. **Algorithm hint** — formulas, examples, rounding language, and descriptions
   of combat or production. These are excellent test-case generators but must
   be checked against instructions and saved-game observations.
3. **Player-facing simplification** — strategy advice, prose explanations, or
   claims that omit edge cases. Do not promote these to an implementation rule
   without a binary or runtime confirmation.

When recording a reconstruction result, cite the topic link, the help context,
and the relevant recovered procedure or structure. A useful notation is:

```text
HELP: t418589 / ctx 0x7de / offset 0x418589
CODE: battle.c / Movement, Initiative and Firing in Battle
STATUS: formula candidate; rounding and weight units still require a fixture
```

## High-value behavioral rules

The links below are the primary sources for the compact rule summaries. They
are intentionally organized by subsystem rather than by the player's-guide
navigation order.

### Turn and file lifecycle

The help file names the main game artifacts in
[`Files Used in Stars!`](../../help-file/out/t448968.html):

| Suffix | Meaning in the help file | Likely RE role |
|---|---|---|
| `.hst` | Host state for a game; normally host-only | Host-side universe/player state and turn-generation inputs |
| `.xy` | Immutable universe/planet-position file | Shared map geometry and generated planet data |
| `.mN` | Player turn file, `N = 1..16`; player empire at the beginning of a turn | Player-visible state snapshot |
| `.rN` | Saved custom race description; extension is not required | Race wizard serialization |
| `.xN` | Player order/log file for the current turn | Submitted changes applied by the host |
| `.hN` | Player history/knowledge file | Persistent visibility and report history |
| `stars.ini` | Options, window arrangement, scanner view/overlays, and registration state | UI/configuration persistence |

The documented lifecycle is: open `.mN`; if a matching `.xN` exists, load it
to update the current state; the host incorporates submitted logs into host
state and returns a new `.mN`; the player maintains `.hN` as knowledge seen
over time. The help page also documents rotating backup directories and a
`[Misc]` `Backups=N` option. This is consistent with, and should be read beside,
the current [file-streams](file-streams.md),
[save coordinator](save-coordinator.md), [player records](player-records.md),
and [race component](race-component.md) notes.

### Planet, population, mining, and production

The [planet overview](../../help-file/out/t908f7.html),
[population](../../help-file/out/t987fe.html),
[mines](../../help-file/out/ta811c.html),
[factories](../../help-file/out/tb019b.html), and
[production conditions](../../help-file/out/t130349.html) pages expose the
following state relationships:

| Area | Help-file rule or invariant | RE consequence |
|---|---|---|
| Habitability | A 100% planet supports 1,000,000 people; positive values below 5% are treated as 5% for support capacity | Keep displayed habitability and support-capacity calculations distinct |
| Population traits | Hyper-Expansion halves the normal population limit; Only Basic Remote Mining adds 10%; excess population works at 50% efficiency | Race traits modify both capacity and productivity, not just UI labels |
| Growth | Maximum growth is 1–20% at 100% value and falls proportionally with planet value; growth plateaus after 25% capacity | Test low-population and near-capacity branches separately |
| Killer worlds | Negative habitability kills `abs(value)/10` percent of colonists per year | Confirm signed-value handling and rounding |
| Minerals | Ironium, boranium, and germanium have surface stock, underground concentration, and annual mining output; concentration is displayed 0–200 | Model surface stock, concentration, and extraction as separate fields |
| Mine depletion | Approximate mine-years to reduce concentration by one: `12500 / current concentration`; concentration bottoms out at 1 | Efficiency affects output, not depletion rate |
| Homeworld | Starting-world concentration never falls below 30%, even after ownership changes | A homeworld flag survives conquest/ownership transitions |
| Factories | Factories consume no ongoing input; they add resources each year; default cost is 4 kT germanium, or 3 with Costs One Less | Production has a mineral-cost path separate from annual resource generation |
| Defenses | Any number can be built, but only a population-operable number is active; technology upgrades existing defenses for free | Store built and operating counts separately |
| Production queue | One queue per planet; items run top-to-bottom; inserting before partial work delays it; running out of minerals blocks the queue | Queue entries need order, progress, blocking, and resource state |
| Unique/upgrade items | Unique installations disappear from inventory when queued/built; upgrades replace older inventory entries and charge only the enhancement | Inventory is derived from planet state and current technology |
| Auto-build | Auto-build entries remain until removed, never block, are skipped when impossible, and attempt their target each year | Do not treat an auto-build entry like a finite normal queue item |

See [How Production Works](../../help-file/out/t108164.html),
[Auto-build](../../help-file/out/t128000.html), and
[Production Templates](../../help-file/out/t110747.html) for the queue
semantics and the default-colony template example.

### Research and terraforming

The [fields of study](../../help-file/out/t1383fe.html),
[research cost](../../help-file/out/t140ab1.html),
[terraforming](../../help-file/out/te02d7.html), and
[terraforming technology](../../help-file/out/tf06fd.html) pages establish:

- The six research fields are Energy, Weapons, Propulsion, Construction,
  Electronics, and Biotechnology. Levels run from 0 through 26.
- Technology above an item's required level gives 5% cost reduction per extra
  level, up to a documented 75% reduction. The research cost includes a
  field/level-dependent added cost of 10 resources per already-achieved level;
  the help calls the progression Fibonacci-like, so the exact table still
  needs binary confirmation.
- Only one research field is active at once, but progress in an interrupted
  field is retained. A queued next field takes over at the next level.
- Temperature, gravity, and radiation terraforming use separate technologies.
  Normally each axis can be improved from 3% through 15%; Total Terraforming
  uses only Biotechnology, costs 30% fewer resources, and raises the ceiling to
  30%.
- A terraforming queue item changes the environmental factor currently furthest
  out of range. A 1% task changes one factor by 1%, not all three at once.
- If a newly colonized planet cannot reach habitability 0% or better within a
  year, colonists begin to die. Claim Adjusters can terraform another player's
  planet from orbit, positively for a friend and negatively for an enemy; the
  owner's and visitor's terraforming limits are not additive.

The already reconstructed race work should be cross-checked against the
[race scoring](race-scoring.md), [habitability](habitability.md),
[innate habitability](innate-habitability.md), and
[random race](random-race.md) reports rather than replacing those reports with
the guide's simplified prose.

### Battle model

The [Guts of Combat](../../help-file/out/t3f026b.html) page links the detailed
combat topics. The most useful recovered formulas and state rules are:

#### Movement and initiative

[`Movement, Initiative and Firing in Battle`](../../help-file/out/t418589.html)
gives movement in quarter-squares:

```text
movement = (ideal_engine_speed - 4) / 4
         - weight / 70 / 4 / number_of_engines
         + 1/4 * maneuvering_jets
         + 1/2 * overthrusters
```

The page explicitly warns that multiplication and division happen before
addition/subtraction. Battle movement is capped at 2.5 squares per round.
Movement is performed in three phases: tokens able to move three squares get a
first square, then tokens able to move two or more get a square, then all
tokens able to move get a square. Within a phase the order is heaviest to
lightest with a stated ±15% margin, and tactics select the preferred square.

Firing is highest initiative first, with weapon slots and range breaking ties.
Firing initiative is described as hull initiative + battle computers + racial
modifier + weapon initiative. The standalone
[initiative definition](../../help-file/out/t468752.html) page uses “initiative”
for movement order as well, so keep movement initiative and firing initiative
distinct until the code resolves the terminology.

#### Armor, shields, and damage

[`Armor, Shields and Damage`](../../help-file/out/t3f8505.html) says that
shields are a pooled token resource and are depleted before armor, except that
torpedoes apply a direct armor component. Damage remaining after destroying
ships is spread across the remaining ships in the token. The game stores a
separate damage value per token and displays damage as `C @ P%`, where `C` is
the number of damaged tokens and `P` is percentage damage.

This implies a useful test matrix: one token containing many ships versus many
one-ship tokens, with the same total armor/shield points. The guide explicitly
states that those cases produce different losses.

#### Weapons, computers, and jammers

[`Weapon Properties`](../../help-file/out/t400317.html) records these rules:

- Beam weapons always hit and lose 10% of strength pro-rated over maximum
  range. A beam can stream excess damage into other tokens in the same square,
  limited by the number of attacking ships.
- Each beam weapon sweeps up to `damage × range²` mines per year.
- A torpedo rolls independently per fired torpedo. Half of a hit's damage is
  applied directly to armor and half to shields; a strike cannot destroy more
  ships than the number of torpedoes that hit.
- A miss causes target-only collateral equal to one-eighth normal damage and
  affects shields only. Capital Ship Missiles do twice their stated damage
  when the target has no shields.
- Jammers multiply accuracy down repeatedly. Battle Computers reduce
  inaccuracy, not accuracy. The guide's formula is:

  ```text
  modified_accuracy = 100 - ((100 - base_accuracy) × product(1 - computer_bonus))
  ```

  Computer and jammer percentages cancel 1% for 1% before the remaining
  modifier is applied.
- An Energy Dampener reduces every ship's movement by one square per round for
  the battle, even if its carrier is destroyed. Multiple dampeners do not stack.
- Capacitors multiply beam damage; the guide documents a 250% maximum extra
  damage from multiple capacitors.

#### Repair

[`Damage Repair`](../../help-file/out/t410661.html) supplies a particularly
useful lookup table:

| Fleet location/state | Annual armor repair |
|---|---:|
| Moving through space | 1% |
| Stopped in space | 2% |
| Orbiting, not bombing an opponent | 3% |
| Orbiting an owned planet with a space dock | 20% |
| Orbiting an owned planet with a starbase but no space dock | 8% |
| Orbiting an owned planet without a starbase | 5% |
| Orbiting a planet being bombed | 0% |
| At least one Fuel Transport hull | +5% |
| At least one Super Fuel Xport hull | +10% (replaces +5%) |

Starbases repair at 10% per year normally and 15% for Inner Strength. No repair
occurs during a stargate year; a starbase under attack counts as absent for
repair. These conditions should become explicit branches in a battle/turn
fixture, not a single “repair rate” field.

### Cloaking and scanning

[`The Guts of Cloaking`](../../help-file/out/t4281de.html) defines a two-stage
calculation:

1. `total_cloak_units = empty_ship_mass × device_units_per_kT`.
2. `units_per_kT = total_cloak_units / loaded_fleet_mass`.

The guide's device values are: Transport Cloak 300/75%, Stealth Cloak 70/35%,
Super-Stealth Cloak 140/55%, Ultra-Stealth Cloak 540/85%, Shadow Shield
70/35%, Depleted Neutronium Armor 50/25%, and Chameleon Scanner 40/20%.
Starred entries require Super Stealth. In a multi-ship fleet, uncloaked ships
are counted as cargo; Super Stealth races do not count cargo for this purpose.

The guide's piecewise approximation from cloak points per kT is:

```text
if points <= 100: percent = points / 2
else: points = points - 100
  if points <= 200: percent = 50 + points / 8
  else: points = points - 200
    if points <= 312: percent = 75 + points / 24
    else: points = points - 312
      if points <= 512: percent = 88 + points / 64
      else if points < 768: percent = 96
      else if points < 1000: percent = 97
      else: percent = 98
```

The exported prose has boundary/ordering inconsistencies in this pseudocode
(the `points < 768` branch follows a `points <= 1124` branch). Preserve the
page as evidence, but resolve the actual branch order from
[`PctCloakFromHuldef`](../../analysis/reconstruction/core.md) and controlled
visibility fixtures. Multiple Tachyon Detectors are described as repeated
95%-of-current effectiveness, equivalent to `95% ^ sqrt(detector_count)` in
the guide's notation.

The ordinary visibility rules are in
[Scanning and Cloaking](../../help-file/out/t240a75.html),
[Detecting an Opponent's Fleets](../../help-file/out/t25830a.html), and
[Scanner Technology](../../help-file/out/t248339.html). They distinguish
normal range, penetrating range, minefield detection, cloak attenuation, and
estimated enemy paths. The current [visibility report](visibility.md) is the
appropriate code crosswalk.

### Minefields

[`The Guts of Minefields`](../../help-file/out/t4305f4.html) gives three mine
types:

| Type | Safe speed | Damage / ship | Minimum fleet damage | Stated trigger rate |
|---|---:|---:|---:|---|
| Normal | Warp 4 | 100 per engine, 125 for ramscoops | 500, 600 for ramscoops | 0.3% per light-year per warp above safe |
| Heavy | Warp 6 | 500 per engine, 600 for ramscoops | 2000, 2500 for ramscoops | 1% per light-year per warp above safe |
| Speed Trap | Warp 5 | none | none | 3% per light-year per warp above safe; stops fleet |

Minefields have a cloaked value of 0% to penetrating scanners and 82% to
non-penetrating scanners. A player sees an opponent's center/radius when the
center is in penetrating range, after hitting it and having the center in normal
range, or while inside it. In a minefield, ship cloak effectiveness is treated
as an absolute detection value.

The page's numerical examples do not read as a single simple linear formula in
all cases. Treat the examples as regression fixtures and verify probability,
distance prorating, caps, and rounding against the turn code. The Space
Demolition, Inner Strength, and War Monger dispenser restrictions are also
useful trait-gating tests.

### Mass drivers

[`The Guts of Mass Drivers`](../../help-file/out/t438576.html) is the most
formula-dense page in the export:

```text
packet_speed   = packet_warp²
receiver_speed = receiver_accel²
caught_safely  = receiver_speed / packet_speed
recovered      = packet_kT × caught_safely
               + packet_kT × (1 - caught_safely) / 3
raw_damage     = (packet_speed - receiver_speed) × packet_weight / 160
damage_after_defense = raw_damage × (1 - defense_coverage)
colonists_killed = max(damage_after_defense × population / 1000,
                       damage_after_defense × 100)
```

The help states that packets travel `warp²` light-years per year. Overdriving a
mass driver by one, two, or three warp levels causes 10%, 25%, or 50% annual
decay, with a minimum 10 kT of each mineral per year. Decay applies in both
launch and arrival years and is prorated by distance. Packet Physics halves both
decay and the minimum; Interstellar Traveller changes the no-overdrive and
overdrive rules. The page's defense-destruction sentence has an ambiguous
minimum/rounding clause; use its worked examples as fixtures but confirm the
implementation before naming a field.

## Race construction and game setup

The wizard context range is especially useful for connecting dialog controls
to recovered state:

| Context range | Wizard/dialog area | Primary source |
|---|---|---|
| `0x3ea–0x3ff` | Basic/advanced game setup and race wizard entry | [context map](../../help-file/out/contexts.html) |
| `0x400–0x410` | Race name, predefined races, primary traits | [Race Name and Password](../../help-file/out/t338259.html), [Primary Traits](../../help-file/out/t3402ef.html) |
| `0x411–0x422` | Lesser traits, growth, efficiency, research cost, save | [Lesser Traits](../../help-file/out/t36806f.html), [Finish and Save](../../help-file/out/t3886f2.html) |
| `0x449–0x455` | Added setup/race/battle/production dialogs | [context map](../../help-file/out/contexts.html) |

The [primary-trait page](../../help-file/out/t3402ef.html) lists ten primary
traits and the [lesser-trait page](../../help-file/out/t36806f.html) lists
fourteen lesser traits. The individual topic pages are the best source for
trait-specific rules; the current reconstructed wizard and saved-race behavior
is documented in [race-component](race-component.md) and
[race validation](race-validation.md).

Important trait gates exposed by the guide include:

- Alternate Reality races live on starbases and cannot build ordinary
  planetary installations.
- Super Stealth changes cloak/cargo behavior and enables Pick Pocket/Robber
  Baron piracy.
- Space Demolition changes minefield scanning and dispenser availability.
- Packet Physics changes packet decay and minimum decay.
- Interstellar Traveller changes stargate visibility and packet decay.
- Claim Adjuster enables orbital terraforming and the Retro Bomb.
- Inner Strength changes research/repair/device availability and minefield
  options.
- Hyper-Expansion changes growth/capacity behavior; Total Terraforming,
  Improved Starbases, Advanced Remote Mining, Mineral Alchemy, and the other
  lesser traits alter specific production, technology, or population paths.

## UI and Win16 reconstruction clues

The help file documents the visible object model more clearly than the binary
names do. The [Stars! Screen](../../help-file/out/t8000.html) and the context
groups around `0x5de–0x5fd` identify the main layout, tiles, panes, and command
operations:

- Planet command and fleet command are separate modes.
- Planet tiles include Planet, Production, Status, Minerals on Hand, Fleets in
  Orbit, and Starbase.
- Fleet tiles include Fleet, Location, Fuel and Cargo, Fleet Composition,
  Other Fleets Here, Fleet Waypoints, and Waypoint Task.
- The Scanner pane has view modes and overlays for minerals, planet value,
  population, radar, minefields, fleets, names, ship counts, idle fleets, and
  filters.
- The Messages pane has filtering, navigation, and player-to-player mail.
- Help contexts are part of the UI contract, not just documentation links;
  dialogs pass numeric context IDs to WinHelp.

The [dialog context group](../../help-file/out/contexts.html),
[keyboard shortcuts](../../help-file/out/t458204.html), and the popup-topic
pages (`t480*.html`, `t488*.html`, and `t490*.html`) are useful for identifying
control boundaries and command accelerators. Cross-reference them with
[race-window-api](race-window-api.md), [starting-screen](starting-screen.md),
[tutorial](tutorial.md), and the reconstructed modules under
`reconstructed/src/`. A topic title is a hypothesis about a subsystem; it is
not proof that a same-named C procedure exists.

## Technology tables: image evidence, not HTML data

The seventeen technology-table topics contain screenshots rather than semantic
HTML tables. Keep the bitmaps as the authoritative export and do not rely on
low-quality OCR without checking the image manually.

| Topic | Bitmap | Category |
|---|---|---|
| [`Armor`](../../help-file/out/t3e88ea.html) | `bm117.bmp` | Armor |
| [`Beam Weapons`](../../help-file/out/t3e890e.html) | `bm118.bmp` | Beam weapons |
| [`Bombs`](../../help-file/out/t3f0016.html) | `bm119.bmp` | Bombs |
| [`Electrical Devices`](../../help-file/out/t3f003a.html) | `bm120.bmp` | Electrical devices |
| [`Engines`](../../help-file/out/t3f006b.html) | `bm121.bmp` | Engines |
| [`Mechanical Devices`](../../help-file/out/t3f0091.html) | `bm122.bmp` | Mechanical devices |
| [`Mine Laying`](../../help-file/out/t3f00c2.html) | `bm123.bmp` | Mine-laying devices |
| [`Mining`](../../help-file/out/t3f00ec.html) | `bm124.bmp` | Mining devices |
| [`Orbital Devices`](../../help-file/out/t3f0111.html) | `bm125.bmp` | Stargates and mass drivers |
| [`Planetary Installations`](../../help-file/out/t3f013e.html) | `bm126.bmp` | Planetary installations |
| [`Scanners`](../../help-file/out/t3f0174.html) | `bm127.bmp` | Scanners |
| [`Shields`](../../help-file/out/t3f019b.html) | `bm128.bmp` | Shields |
| [`Ship Hulls`](../../help-file/out/t3f01c1.html) | `bm129.bmp` | Ship hulls |
| [`Starbase Hulls`](../../help-file/out/t3f01ea.html) | `bm130.bmp` | Starbase hulls |
| [`Terraforming`](../../help-file/out/t3f0217.html) | `bm131.bmp` | Terraforming |
| [`Torpedoes`](../../help-file/out/t3f0242.html) | `bm132.bmp` | Torpedoes |

The [table introduction](../../help-file/out/t3e8469.html) defines the units:
technology categories are levels; weight and minerals are kT; damage is points
or a percentage; range is battle-board squares; engine `Fuel #` is percentage
of standard fuel use at the indicated warp; and `Ability` is technology-type
specific. Those column meanings are important when recovering component
records from the data segments.

## Suggested RE work queue

1. **Make a table transcription with provenance.** Transcribe each `bm117`–
   `bm132` table into a machine-readable fixture, retaining the bitmap name,
   row/column headers, and confidence per value. These tables are likely the
   cleanest bridge between technology records and hull/component fields.
2. **Turn formulas into differential tests.** Start with movement, cloak
   piecewise boundaries, jammer/computer cancellation, repair-rate branches,
   and mass-driver examples. Include the guide's examples even where they look
   inconsistent; discrepancies are valuable.
3. **Map help contexts to resources and callers.** Use `contexts.html` to build
   a context-to-dialog/control ledger, then compare each ID with recovered
   `WinHelp` calls, dialog resources, and reconstructed callback names.
4. **Validate file lifecycle against serialized records.** Compare the
   `.hst/.xy/.mN/.xN/.hN/.rN` descriptions with the record types and save
   coordinators already recovered. Pay special attention to host-only versus
   player-visible state and the point at which history is updated.
5. **Separate display values from stored values.** The guide uses percentages,
   “up to” values, estimated scanner paths, and rounded reports. Preserve the
   raw field and the display conversion as separate functions.
6. **Track contradictions explicitly.** Known examples include the minefield
   probability examples, the cloak pseudocode branch ordering, and the
   mass-driver defense-destruction sentence. Do not silently “correct” them in
   the reconstruction.

## Existing project crosswalk

| Help evidence | Current project starting point |
|---|---|
| Race wizard, traits, advantage points, saved race | [race-component](race-component.md), [race scoring](race-scoring.md), [race validation](race-validation.md) |
| Habitability, population, terraforming | [habitability](habitability.md), [innate habitability](innate-habitability.md), `reconstructed/src/planetary_climate_change.c`, `reconstructed/src/can_terraform.c` |
| Files, logs, turn/state persistence | [file-streams](file-streams.md), [save coordinator](save-coordinator.md), [player records](player-records.md), `reconstructed/src/file.c` |
| Scanner, cloak, overlays, visibility | [visibility](visibility.md), `reconstructed/src/scan_orientation.c`, `reconstructed/src/pct_cloak.c`, `reconstructed/src/set_vis_pf_things.c` |
| Production, queues, resources | `reconstructed/src/produce.c`, `reconstructed/src/queued_production.c`, `reconstructed/src/change_production.c`, `reconstructed/src/finish_production.c` |
| Combat, tokens, movement, damage | `reconstructed/src/battle.c`, `reconstructed/src/battle_vcr.c`, `reconstructed/src/dxy_move_range_to_consider.c`, `reconstructed/src/c_torp_hit.c`, `reconstructed/src/heal_ships.c` |
| Fleet orders, cargo, minefields, packets | `reconstructed/src/do_orders.c`, `reconstructed/src/transfer_to_others.c`, `reconstructed/src/sweep_for_mines.c`, `reconstructed/src/add_packet_queue.c`, `reconstructed/src/do_bombing.c` |
| Dialogs, tiles, help, popups | [race-window-api](race-window-api.md), [starting-screen](starting-screen.md), [tutorial](tutorial.md), `reconstructed/src/ui.c`, `reconstructed/src/popup.c`, `reconstructed/src/text.c` |

The HTML export should remain unchanged. New transcriptions, fixtures, or
interpretive notes should cite this document and the original topic/bitmap
rather than modifying the exported pages.
