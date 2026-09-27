# DoAutomitronAiTurn AU-E1 — support design queue and census order

Original 1098:~04E0 completes the unowned-viable planet census before
entering the planet mutation/production pass. The production source now
matches that two-pass order; the first planet-visit callback sees the
complete count. A strict-C89 host probe passes 5/5, including repeat-call
counter reset.

At original 1098:~0910-09B0, once the caller has established a production
entry and absence of the bypassing queue item, Automitron scans fleets in
roster order at that planet. It calls FPotentISWarFleet(fleet, 2) and stops at
the first potent match. If that fleet's slot-2 plus slot-3 count meets
vrgAiArmadaPotency[2], it calls AddItemToQueue(design_escort, 4, 2, 1)
and skips the later hull batch. The production source now implements that
selection/queue path behind explicit support_queue_eligible and
is_potent_war_fleet caller bindings. Its strict-C89 host probe passes 15/15:
owner/planet filtering, first-potent short circuit, threshold, raw queue
tuple, hull-batch exclusion, missing eligibility/design, and queue failure.

The callback still requires a live original FPotentISWarFleet binding, and
the full original production queue includes additional branches not yet
reconstructed. Fresh r6 Win16 packet includes these probes but has **not**
run. AU-E1 remains open; no evidence unit or promotion closes here.
