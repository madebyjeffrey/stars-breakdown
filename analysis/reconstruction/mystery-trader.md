# MysteryTrader reconstruction

`reconstructed/src/mystery_trader.c` is a typed semantic reconstruction of mystery-trader events, preserving event selection, target/resource effects, and resulting reports/state changes.

Validation: `tests/mystery_trader_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ew/MTSTA.TXT` and `runs/vmctl-vc152ew/MTRES.TXT`.
