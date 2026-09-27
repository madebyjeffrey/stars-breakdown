# SatisfyOrders raw transfer-failure E2 native replay

The production source now exposes a caller-resolved failure-frame builder for
the remaining exact message layouts recovered from `SatisfyOrders`:

- `0x0121` set-amount failure and `0x0122` set-colonists failure use the
  eight-word `FSendPlrMsg` frame `[fleet|0x8000, fleet, resource, requested,
  0, target_x, target_word, 0]`.
- `0x0165` deep-space colonist rejection uses
  `FSendPlrMsg2(fleet|0x8000, fleet, 0)`.
- `0x011e` futile mineral-packet transfer uses
  `FSendPlrMsg2(fleet|0x8000, fleet, thing_kind)`.

The strict-C89 host probe reports
`SATISFY_ORDERS_RAW_FAILURE_CHECKS=44 FAILURES=0 RESULT=PASS`. Fresh VC++
1.52c/Win98 validation in `C:\SYOE22` reports the same result; compile/link
pass, the process exits 0, and the final guest window inventory contains no
Program Error dialog. All eight words or all three words are checked for each
branch.

This remains supporting E2 ABI evidence. The caller still owns the packed
action union and the conditions that select these messages; the original
full-turn transfer differential, upstream route/fuel integration, E3 replay,
and Step-6 promotion remain open.

Evidence:

- `tests/satisfy_orders_raw_failure_probe.c`
- `compat/vc152/satisfy-orders-raw-failure-e2-native/`
- `runs/vmctl-vc152syoe22/SYOFALL.TXT`
- `runs/vmctl-vc152syoe22/SYOFALSTA.TXT`
- `runs/vmctl-vc152syoe22/SYOFACL.LOG`
- `runs/vmctl-vc152syoe22/SYOFALLINK.LOG`
- `reconstructed/src/satisfy_orders.c`
- `reconstructed/include/core.h`
- `/Users/jdrake/Projects/stars-decompile/turn3.c`
- `/Users/jdrake/Projects/stars-decompile/scripts/ghidra_scripts/includes/enums.h`
