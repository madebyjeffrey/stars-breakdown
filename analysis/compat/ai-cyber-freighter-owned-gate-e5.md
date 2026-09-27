# Cyber freighter owned-planet mineral gate evidence

The typed `DoCyberFreighter` view now exposes the raw owned-planet mineral
rate and stock inputs. The source uses the original gate: pickup when the rate
is negative, or when rate is below one and stock is below `2001`; otherwise it
loads the freighter for drop-off.

The strict host C89 probe reports:

`DO_CYBER_FREIGHTER_OWNED_GATE_CHECKS=12 FAILURES=0 RESULT=PASS`

It covers rate `-1`, rate `0` with stock `2000`, rate `0` with stock `2001`,
and positive rate with empty stock, including target kind, movement, and the
corresponding `-1000` pickup or `+1000` supply transfer.

This is host source-fidelity evidence only. Other raw fleet-cargo predicates,
native replay of the corrected mineral gate, and original-turn differential
remain open; no authoritative Step-5 or promotion count changes.

Evidence:

- `reconstructed/include/core.h`
- `reconstructed/src/do_cyber_freighter.c`
- `tests/do_cyber_freighter_owned_gate_probe.c`
- `tests/do_cyber_freighter_source_probe.c`
- `analysis/decompiler/export/raw/10a8_37b0_DoCyberFreighter.c`
