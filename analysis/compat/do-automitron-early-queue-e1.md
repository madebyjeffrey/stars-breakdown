# DoAutomitronAiTurn AU-E1 — early production queue

The original NE 20:~0670-07F0 queue block runs after the complete
unowned-viable planet census and before the stochastic colonizer seed.
CheckAiShdefStatus is called four times in order; its return is status/count
data, not a Boolean success value. The second return is compared against
the greater of twice the AI-data group count and one tenth of the player's
owned-planet count. Above technology level four, a selected support design
is queued unconditionally below that threshold, or with Random(4)==0
under the 10/7 pressure margin. If a viable unowned planet exists, turn is
greater than ten, and design 1 does not already exist, the original queues
one design-1 colonizer after that support decision.

The production source now follows that order using explicit live caller
bindings for AI-data count, owned-planet count, tech, design existence, and
the previously recovered queue-eligibility predicate. It also permits
scanned/mineral-eligible unowned planets to enter production, as the
original does. The strict-C89 host probe passes 17/17: valid zero
design-status return and all four calls, deterministic and random gates,
both queue tuples/order, tech exclusion, no-pressure case, failure
propagation, and unowned-planet production. Bounds on caller counts keep
the arithmetic inside Win16 signed-int range.

Fresh r8 Win16 packet includes this probe but has not run. Remaining
production branches, live native bindings, and an original-turn differential
keep AU-E1 open; this milestone closes no named evidence unit.
