# DoCyberFreighter native E5 gate/progress replay

The reviewed `DoCyberFreighter` source was exercised in a fresh VC++ 1.52c /
Win98 packet. The matrix covers the corrected owned-planet mineral-rate and
stock gate, pickup versus drop-off transfer direction, carrying/drop-off
progress-bit advancement, owned-planet rollback when both destinations are
absent, enemy ordinary-planet starbase fallback, special-planet carrying
behavior, open-space short-circuiting, and the exact `0x11b0` movement plus
packed PLORD first-order replacement.

The strict-C89 host probe reports
`DO_CYBER_FREIGHTER_E5_CHECKS=26 FAILURES=0 RESULT=PASS`. The fresh native
packet in `C:\CYBFE5` reports the same result; compile and link pass, the
process exits 0, and the final guest window inventory contains no Program
Error dialog.

This is stronger native support for the DoCyberFreighter source boundary. It
does not close the broader Cyber AI-turn units: complete packet/freighter
interaction, all raw fleet-cargo predicates, complete AI-table integration,
and original-turn differential evidence remain open. No authoritative TODO6
counter is promoted by this checkpoint.

Evidence:

- `tests/do_cyber_freighter_e5_probe.c`
- `compat/vc152/do-cyber-freighter-e5-native-r1/`
- `runs/vmctl-vc152cybfe5/CYBFE5.TXT`
- `runs/vmctl-vc152cybfe5/CYBFE5STA-r2.TXT`
- `runs/vmctl-vc152cybfe5/CYBFE5CL-r2.LOG`
- `runs/vmctl-vc152cybfe5/CYBFE5LINK-r2.LOG`
- `runs/vmctl-vc152cybfe5/CYBFE5-r2.MAP`
- `reconstructed/src/do_cyber_freighter.c`
- `reconstructed/src/ai_plord.c`
- `analysis/decompiler/export/raw/10a8_37b0_DoCyberFreighter.c`
