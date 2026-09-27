# DoMacintiAiTurn MA-E2: first-sweep transport target repair

Status: **supporting evidence only; MA-E2 remains open**.

The original first fleet sweep asks FIsAiAttack before FIsAiTransport
at 10a0:1f18–2061, on the owner-specific non-attack-design path when
the fleet has no colonizer or the turn is below 41. For a positive
transport answer it chooses the current planet ID (packed fleet offset
6) when cord is below 2 or the first PLORD kind low nibble is nonzero.
Otherwise, a second PLORD task-1 high nibble at offset 28 selects the
second order's planet ID at offset 26. A missing target exits this
repair branch. If LpplFromId gives no planet or a foreign planet and
both cargo words at fleet offsets 88 and 90 are zero, the original
selects the fleet, reduces cord and PLORD.iordMac to 1, refreshes with
FLookupFleet, and calls ClearAiCurrentTask with mode 0.

The bound source now offers this service path when is_ai_attack is
installed. In that mode the attack, transport, clear-task, select, and
refresh callbacks must all be present before mutations begin. The
planet lookup uses the preflighted packed planet table. The same
selected-order truncation helper serves the earlier far-waypoint
branch. The selected fleet passed to callbacks is now rebound from
the packed record before use, avoiding stale typed owner/cord/ship
fields. When this opt-in service path is absent, the older typed
action adapter remains in use; the full original first sweep is not
yet claimed.

The strict-C89 tests/do_macinti_first_transport_e2_probe.c passes
**10/10** checks: foreign second-order target with ATSRC service/
selection/refresh/clear sequence, first-order current-planet
selection, no second task-1 target, nonzero cargo exclusion, owned
target exclusion, attack and false-transport exclusions, late-turn
colonizer exclusion, missing-clear preflight, and short selected
PLORD rejection. It verifies the unmodified second record and that
the clear callback sees cord/iordMac equal to 1. Earlier Macinti
probes pass 31/31, 12/12, 8/8, and 14/14.
The full host suite passes **574 tests**, 153 expected skips, zero
failures; baseline and diff checks are clean.

The fresh combined DOS-8.3/CRLF VC1.52c packet at
compat/vc152/macinti-planet-table-e1-r16/ targets C:\MAE1N16 and
contains all seven probes (31/13/12/10/16/8/14). It is staged, not compiled
or run in Win98. Exact attack and transport branch integration,
subsequent buddy/miner/colonizer routing, native return, and an
action-bearing original-executable comparison remain. The 10/10
host result closes no named Step-5 unit or procedure promotion.
