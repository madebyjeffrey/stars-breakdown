# SatisfyOrders raw merge message E2 native replay

The production `SatisfyOrders` source now binds merge outcomes to the original
three-payload `FSendPlrMsg2` ABI. A successful same-owner merge emits `0x00f7`
with the destination fleet marker, the source `WFromLpfl` word, and the
destination id. An enemy destination emits `0x00f6` with the source fleet
identity, and an absent or inactive destination emits `0x00f5` with the same
source identity. All three messages are sent to the source fleet owner.

The strict-C89 host probe reports
`SATISFY_ORDERS_RAW_MERGE_CHECKS=12 FAILURES=0 RESULT=PASS`. Fresh VC++ 1.52c/
Win98 validation in `C:\SYOE15` reports the same result; compile/link pass, the
process exits 0, and the final guest window inventory contains no Program Error
dialog.

The payload mapping is supported directly by the recovered original source:
the success branch uses `lpflDest->id | 0x8000`, `WFromLpfl(lpfl)`, and
`lpflDest->id`; the enemy and invalid branches use `lpfl->id | 0x8000`,
`lpfl->id`, and zero.

This is supporting E2 evidence, not closure of the named SatisfyOrders E2
unit. Ordinary resource-transfer layouts, the remaining shortage/colonist/
technology message families, the original transfer differential, E3 full-turn
comparison, and Step-6 promotion remain open.

Evidence:

- `tests/satisfy_orders_raw_merge_probe.c`
- `compat/vc152/satisfy-orders-raw-merge-e2-native/`
- `runs/vmctl-vc152syoe15/SYOMERGE.TXT`
- `runs/vmctl-vc152syoe15/SYOMERGESTA.TXT`
- `runs/vmctl-vc152syoe15/SYOMERGECL.LOG`
- `runs/vmctl-vc152syoe15/SYOMERGELINK.LOG`
- `runs/vmctl-vc152syoe15/SYOMERGE.MAP`
- `reconstructed/src/satisfy_orders.c`
- `reconstructed/include/core.h`
- `analysis/decompiler/export/raw/10b0_6798_SatisfyOrders.c`
- `/Users/jdrake/Projects/stars-decompile/turn3.c`
