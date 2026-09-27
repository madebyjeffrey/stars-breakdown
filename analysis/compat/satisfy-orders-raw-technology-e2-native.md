# SatisfyOrders raw technology message E2 native replay

The production `SatisfyOrders` source now preserves the raw technology-message
handoff after signed technology-result binding. The binding supplies the raw
recipient and caller-resolved eight-word `FSendPlrMsg` frame through
`StarsSatisfyTechnologyOutcome`; the order resolver forwards that frame through
the existing `raw_message` callback whenever a nonzero signed result selects a
message code.

The strict-C89 host probe reports
`SATISFY_ORDERS_RAW_TECHNOLOGY_CHECKS=16 FAILURES=0 RESULT=PASS`. Fresh VC++
1.52c/Win98 validation in `C:\SYOE18` reports the same result; compile/link
pass, the process exits 0, and the final guest window inventory contains no
Program Error dialog. The matrix checks signed negative point binding (`0x013c`)
and positive planet binding (`0x013f`), exact recipient, all eight forwarded
payload words, result id, amount, and order completion.

After the ordinary resource-frame mapping was added, the host regression probe
initially observed a duplicate raw callback for technology orders. The
production helper now suppresses the ordinary resource frame when
`technology_flag` is set, leaving the dedicated technology frame as the only
raw transport event. All eight affected host raw-order probes pass again
(141/141 checks across the nine raw-order probes), and a fresh rebuild/replay in `C:\SYOE18` reports the same
16/16 technology result with clean compile/link status and no Program Error
dialog. The rerun is preserved separately from the original evidence.

This milestone validates the raw ABI handoff and its interaction with the
already validated production technology-table/result binding. It intentionally
does not claim that the reduced typed order model has independently derived
every original payload field; the live caller remains responsible for resolving
that frame from planet/design state. Full original technology-message field
semantics, foreign/fuel/permission resource-message families, the original
transfer differential, E3 full-turn comparison, and Step-6 promotion remain
open.

Evidence:

- `tests/satisfy_orders_raw_technology_probe.c`
- `compat/vc152/satisfy-orders-raw-technology-e2-native/`
- `runs/vmctl-vc152syoe18/SYOTECH.TXT`
- `runs/vmctl-vc152syoe18/SYOTECHSTA.TXT`
- `runs/vmctl-vc152syoe18/SYOTECHCL.LOG`
- `runs/vmctl-vc152syoe18/SYOTECHLINK.LOG`
- `runs/vmctl-vc152syoe18/SYOTECH.MAP`
- `runs/vmctl-vc152syoe18-r2/SYOTECH.TXT`
- `runs/vmctl-vc152syoe18-r2/SYOTECHSTA.TXT`
- `runs/vmctl-vc152syoe18-r2/SYOTECHCL.LOG`
- `runs/vmctl-vc152syoe18-r2/SYOTECHLINK.LOG`
- `reconstructed/src/satisfy_orders.c`
- `reconstructed/include/core.h`
- `analysis/compat/satisfy-orders-technology-binding-e2-native.md`
- `analysis/decompiler/export/raw/10b0_6798_SatisfyOrders.c`
