# Cyber freighter movement to packed PLORD integration

The reviewed `DoCyberFreighter` movement boundary now has a focused host
integration through the shared packed-order adapter. The probe exercises the
owned-planet supply branch, target lookup, ideal-warp forwarding, movement
success flagging, and replacement of the existing first order with the
constructed Cyber freighter order.

The strict host C89 probe reports:

`AI_CYBER_FREIGHTER_PLORD_INTEGRATION_CHECKS=8 FAILURES=0 RESULT=PASS`

The packed record preserves target `8`, point `(20,20)`, and exact movement
flags `0x11b0` (planet movement plus the supplied warp nibble). The fleet's
`0x8000` moving flag is also set after the movement callback succeeds.

This is a host-only movement-to-record checkpoint and closes no authoritative
Step-5 unit. The complete Cyber freighter source behavior, packet/freighter
interaction, repeated-turn progress semantics, native replay, and
original-turn differential remain open.

Evidence:

- `reconstructed/src/do_cyber_freighter.c`
- `reconstructed/src/ai_plord.c`
- `tests/ai_cyber_freighter_plord_integration_probe.c`
- `tests/do_cyber_freighter_source_probe.c`
