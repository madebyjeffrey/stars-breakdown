# SatisfyOrders raw player-transfer message E2 native replay

The production `SatisfyOrders` source now exposes an explicit `raw_message2`
service boundary for the original three-payload `FSendPlrMsg2` ABI. The
typed fleet view carries a caller-resolved `WFromLpfl` word, while the order
view carries the newly allocated target fleet identity and its corresponding
`WFromLpfl` word. This keeps the raw identity bits visible without pretending
that the reduced typed record can reconstruct the full design table.

The boundary implements the six raw player-transfer outcomes found in the
original `10b0:6798` routine: created source/recipient messages `0x014d` and
`0x014e`, no-design source/recipient messages `0x014a` and `0x014b`, no-ship
message `0x0149`, blocked message `0x014c`, and invalid-target message
`0x0148`. The probe checks the recipient, message code, and all three payloads,
including `0x8000` fleet identity and `0x30` player identity packing.

The strict-C89 host probe reports
`SATISFY_ORDERS_RAW_TRANSFER_CHECKS=21 FAILURES=0 RESULT=PASS`. The fresh
VC++ 1.52c/Win98 packet in `C:\SYOE12` reports the same result. Compile and
link pass, the process exits 0, and the final guest window inventory contains
no Program Error dialog. The response file includes the Win16 definition file
so the stack, heap, and stub directives are applied.

This is a supporting E2 milestone, not closure of the named SatisfyOrders E2
unit. Ordinary eight-payload `FSendPlrMsg` layouts, the remaining special
message families, the full original transfer differential, E3 full-turn
comparison, and Step-6 promotion remain open.

Evidence:

- `tests/satisfy_orders_raw_transfer_probe.c`
- `compat/vc152/satisfy-orders-raw-transfer-e2-native/`
- `runs/vmctl-vc152syoe12/SYORAW.TXT`
- `runs/vmctl-vc152syoe12/SYORAWSTA.TXT`
- `runs/vmctl-vc152syoe12/SYORAWCL.LOG`
- `runs/vmctl-vc152syoe12/SYORAWLINK.LOG`
- `runs/vmctl-vc152syoe12/SYORAW.MAP`
- `reconstructed/src/satisfy_orders.c`
- `reconstructed/include/core.h`
- `analysis/decompiler/export/raw/10b0_6798_SatisfyOrders.c`
- `/Users/jdrake/Projects/stars-decompile/util.c`
