# DoMacintiAiTurn MA-E1/E2: inner attack queue gate and pressure increment

Status: **supporting evidence only; MA-E1 and MA-E2 remain open**.

The original 10a0:12f2 branch increments local iStack_86 immediately
before AddItemToQueue(uStack_64,1,2,1). That local begins with the
design-available, owned fleet census and later drives the first-sweep
buddy decision. It is not game.turn. Other AddItemToQueue branches in
the same planet pass do not increment it.

The source now exposes the bounded inner decision at original
10a0:1153–1307 as `stars_macinti_should_queue_attack`. Given the caller's
outer-branch decision, selected attack design, current pressure and original
`cExist`, it checks design 14/15, pressure below 60, `cExist` below 5000,
then draws `Random(2)`. On zero, it scans the packed fleet table in order for
the first owned fleet at the selected packed planet with a positive attack-
design ship count, using a `CMineFromLpfl` callback for that fleet. No match
means zero mines. Mine count at least 1001 rejects the queue. Otherwise the
three packed mineral-concentration bytes are summed and tripled; mines below
that score and score above 150 queue directly. The remaining case queues only
when pressure is below 30 and `Random(10)` is nonzero. The helper reports the
decision without changing pressure; the marked plan described below performs
the pre-queue increment. Packed records are validated before random draws.

The preceding original 10a0:0f5b/0ffc control route is now separately
reconstructed as `stars_macinti_attack_outer_route`. It preserves the
`FShouldWeBuildColonizers`/count/turn condition for drawing `Random(100)`;
draws 8–99 enter the attack branch, while draws 0–7 enter the colonizer
path. That path bypasses directly to attack above 49 colonizers after turn
120, otherwise skips the remainder of production if any signed packed
`rgwtMin[0..2]` is below 30. It then calls the planet-colonizer decision:
false enters attack; true enters either early-turn colonizer-only or
colonizer-then-attack flow after turn 4. The helper does **not** enqueue
colonizers or perform the later growth/queue side effects. The caller must
execute those before using the inner attack result.

`stars_macinti_run_attack_production` now composes the original
`10a0:0f5b` mission-design queue check with the outer route, intervening
colonizer queues, and inner attack choice. The mission check compares its
running count with 64 and player `cPlanet / 4`, draws `Random(3)` before the
outer `Random(100)`, and increments the mission count before its queue call;
it never increments attack pressure. The source uses packed `rgwtMin[3]`
population and the original 32-bit-wrapped growth product to
choose one, two, or three colonizer queue calls. It increments the local
colonizer count immediately before the first and second calls, but not the
third. On the attack choice it increments attack pressure immediately before
the attack queue call. The callback trace covers both the early-turn
colonizer-only path and the later colonizer-then-attack path, including
partial state at a failed synthetic queue service. It rejects malformed
fleet records before consuming RNG or queueing a mission item. This
composition is still the 10a0:0f5b–1307 slice, **not** the complete planet
production sequence:
earlier design/resource branches and subsequent queues are not included, and
the main per-planet dispatcher has not yet switched to this bounded path.

The bound source now supports a derived-pressure mode. A caller opts in
only when all pre-fleet attack-production additions are represented by
plan_production plans. The source seeds pressure from its packed fleet
census and accepts an attack_pressure_increment marker only on a queue
plan for the selected attack design (14 or 15), quantity 1, group 2,
mode 1. It increments pressure immediately before the queue callback,
matching the original call order and before the first fleet sweep. Every
unmarked queue leaves pressure unchanged;
mismatched/invalid marked plans fail before their queue callback.
The host adapter returns on queue failure with the pre-call increment
visible; that synthetic error path does not claim the original void
AddItemToQueue call's post-failure state.

The strict-C89 tests/do_macinti_attack_pressure_queue_e1_probe.c passes
**16/16** checks: census-to-queue increment, unmarked queue exclusion,
wrong design/quantity/group/mode, marked non-queue action, invalid
marker, failed queue, marker without derived mode, missing design
status, unavailable attack design, and no selected production planet. The
bound path also verifies planning and queueing despite generic typed budget
and limit fields, normal `FinishProduction(1)` after a queued plan, and
single `FinishProduction(0)` after no-action or explicit finish plans. It
verifies that the stored pressure before fleet processing reflects only the
marked successful addition. The strict-C89
tests/do_macinti_attack_gate_e1_probe.c now passes **39/39** checks for the
inner decision and bounded outer route: direct/fallback choices, score and
mine thresholds, pressure/design limits, `Random(100)` route and early
stockpile exit, mission-before-outer RNG and queue order, packed fleet binding,
and malformed
input. The pressure probe passes **19/19**, including the opt-in bridge
from the selected-planet loop and pre-queue counter visibility. Earlier
Macinti probes pass 31/31, 12/12,
10/10, 16/16, 8/8, and 14/14.
The full host suite passes **575 tests**, 153 expected skips, zero
failures; baseline and diff checks are clean.

The DOS-8.3/CRLF VC1.52c packet at
compat/vc152/macinti-planet-table-e1-r24/ ran in C:\MAE1N24 and
passed compile, link, and all eight probes (31/19/39/12/10/16/8/14);
see `runs/vmctl-mae1n24/BLDSTAT.TXT` and `MAE1RES.TXT`. The bounded
outer/colonizer/attack sequence is now opt-in wired into the selected-planet
loop, but not all surrounding production branches are reconstructed;
earlier and later queue side effects remain delegated to plan_production. The
caller's opt-in completeness assertion still needs native/original
integration evidence. Wider routing and complete turn comparison also
remain open. No named Step-5 unit or promotion closes from this host
result alone.
