# DoAutomitronAiTurn AU-E1 — two resource-limited hull batches

Original NE 20:~09D0-0D21 checks design-existence limits for the selected
IS and attack designs using game.cPlanMax/12+8 and game.cPlanMax/24+4.
For each eligible design it calls GetResourcesAvailable and GetProdQCost
again, subtracts the queue cost, then repeats GetTrueHullCost and
AddItemToQueue(design, 1, 2, 1) up to five times. A deficit stops before
the next queue item. A deficit in the first design skips the second design;
if the first design is merely over its existence limit, the second is
still considered.

The production source now implements this original flow when live
game-planet/design counts and budget/cost/queue service callbacks are
bound. The previous single-design all-or-nothing callback path remains
only as an unbound compatibility fallback. The strict-C89 host probe
passes 18/18: two complete five-item batches, a fresh second budget after
the first five queue calls, per-item cost calls, partial first batch and
second-batch skip, design limit, initial resource deficit, queue failure,
and missing queue eligibility. This source path is not yet a live
original-table binding or a native/original behavioral differential.

Fresh r9 Win16 packet includes the probe but has not run. AU-E1 remains
open; this milestone closes no named evidence unit.
