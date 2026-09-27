# DoAutomitronAiTurn AU-E1 — production lifecycle order

The original planet loop calls ChangeMainObjSel and InitProduction before
examining the current PLPROD queue. If that queue has a bypass item, it
calls FinishProduction(0) and proceeds to the next planet. Otherwise it
performs the ordered queue decisions and calls FinishProduction with a
boolean indicating whether any item was added.

The production source now exposes optional InitProduction and
FinishProduction callbacks around its one-time packed queue scan. It
computes the changed flag from every reconstructed early, seed, support,
and hull item, rather than only the old hull subtotal. Non-production
planets do not call either lifecycle service. The strict-C89 host probe
passes 16/16: ordered visit/init/two additions/finish(1), a subsequent
packed-bypass planet's init/finish(0), a non-entry planet, and failures
from init and finish.

This is source/callback-order evidence, not yet the live original service
binding or a native/original AI-turn differential. Fresh r11 Win16 packet
includes the probe but has not run. AU-E1 remains open; no named evidence
unit or promotion closes here.
