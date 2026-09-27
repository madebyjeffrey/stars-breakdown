# SatisfyOrders fuel and optimal-fuel raw message E2 native replay

The production source now exposes a caller-resolved fuel-frame builder for the
route engine's raw `FSendPlrMsg` branches. It preserves signed optimal-fuel
adjustments as normal load/unload frames (`0x002b`/`0x002d`) and the recovered
special fuel messages: `0x0126` failed load, `0x003d` fuel-capacity warning,
and `0x003c` insufficient-fuel warning.

The strict-C89 host probe reports
`SATISFY_ORDERS_RAW_FUEL_CHECKS=40 FAILURES=0 RESULT=PASS`. Fresh VC++
1.52c/Win98 validation in `C:\SYOE21` reports the same result; compile/link
pass, the process exits 0, and the final guest window inventory contains no
Program Error dialog. The matrix checks all eight payload words, signed
optimal deltas, fuel stat/required values, target coordinates, and the
special warning layouts.

This is supporting E2 ABI evidence. The route engine still owns the upstream
fuel-use calculation and pass-2/pass-4 conditions; original full-turn
differential, remaining permission/failure families, E3 comparison, and
Step-6 promotion remain open.

Evidence:

- `tests/satisfy_orders_raw_fuel_probe.c`
- `compat/vc152/satisfy-orders-raw-fuel-e2-native/`
- `runs/vmctl-vc152syoe21/SYOFUEL.TXT`
- `runs/vmctl-vc152syoe21/SYORESSTA.TXT`
- `runs/vmctl-vc152syoe21/SYORESCL.LOG`
- `runs/vmctl-vc152syoe21/SYORESLINK.LOG`
- `reconstructed/src/satisfy_orders.c`
- `reconstructed/include/core.h`
- `/Users/jdrake/Projects/stars-decompile/turn3.c`
- `/Users/jdrake/Projects/stars-decompile/scripts/ghidra_scripts/includes/enums.h`
