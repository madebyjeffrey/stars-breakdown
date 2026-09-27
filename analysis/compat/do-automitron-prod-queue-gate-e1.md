# DoAutomitronAiTurn AU-E1 — packed production-queue bypass

Original NE 20:~05F0 scans the live PLPROD allocation immediately after
InitProduction. The recovered layout is a four-byte header with iprodMac
at offset 3 and four-byte _prod entries starting at offset 4. A queued
entry bypasses the Automitron construction branches when its packed
type bits 17–19 equal 2 and design bits 10–16 are at least 16.

The production source now reads an optional bounded raw PLPROD allocation
and computes that bypass once per planet before any AddItemToQueue calls.
When the raw allocation is present, it supersedes the earlier caller
eligibility flags for early, seed, support, and live hull-batch decisions.
Without the raw allocation, those older bindings remain a compatibility
fallback. The strict-C89 host probe passes 13/13: empty queue override,
exact type/design threshold and adjacent non-matches, a later inserted
bypass item not changing the current pass, and short-allocation rejection
before AI-record writes.

The scan has not run under VC1.52c or been compared to an original full
turn. InitProduction/FinishProduction callbacks and remaining action
branches are still pending. AU-E1 remains open; this subcheck closes no
named evidence unit.
