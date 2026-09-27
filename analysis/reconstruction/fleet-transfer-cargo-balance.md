# FleetTransferCargoBalance

`FleetTransferCargoBalance` is the ship-subsystem split/merge balancing pass
at NE `000b:ae74–c280` (5,132 bytes). The raw body operates on two newly-created
fleet records and looks up the corresponding pre-transfer records.

The semantic C boundary exposes those dependencies explicitly:

- `StarsFleetBalanceFleet` names the sixteen design counts, sixteen packed
  status words, and five signed cargo dwords.
- `lookup_fleet` supplies each pre-transfer snapshot; a fleet with the raw
  special/deleted bit uses a zero snapshot owned by the first fleet's player.
- `max_weight` supplies the per-design cargo and fuel capacities formerly
  returned by `WtMaxShdefStat`.

The implementation first computes resolved-minus-new ship deltas and capacity
totals. Opposite deltas merge the low status percentage and high status field
with the same count-weighted integer arithmetic and donor/receiver clearing
rules as the original. Cargo is then redistributed by the positive capacity
delta: fuel is balanced first, mineral cargo is reduced proportionally, and
integer remainders are removed one unit at a time. Every committed adjustment
is added to one new fleet and subtracted from the other, preserving the five
resource totals.

Validation covers the ordinary split, proportional fuel/mineral redistribution,
packed status update, and special-fleet path. The strict host C89 probe passed
6 checks. The same source compiled and linked with VC++ 1.52c in StarDev, and
the native Win98 probe reported `FLEET_BALANCE_CHECKS=6 FAILURES=0 RESULT=PASS`.
