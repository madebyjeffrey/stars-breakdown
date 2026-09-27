# DoMacintiAiTurn policy boundary — E2 host support

The new strict-C89 host probe exercises the readable Macinti per-planet and
fleet-policy boundary without claiming a native or original-turn replay.

It checks four planet states (owned, unowned, hostile starbase, and hostile
ordinary), mineral-score saturation and viability, the zero-limit queue rule,
queue affordability, batch affordability, the raw threat calculation and its
starbase increment, and three owned fleet roles (repair, transport, and
colonizer). A foreign fleet is excluded from the typed turn visit exactly as
the caller requires.

Host result:

`MACINTI_AI_POLICY_BOUNDARY_CHECKS=11 FAILURES=0 RESULT=PASS`

This is supporting evidence only. Native replay, exact original retarget /
colonizer / transport predicates, broader packed order mutation, and the
original full-turn differential remain open. The native guest agent is still
disconnected, so no Step-5 unit or promotion count changes.
