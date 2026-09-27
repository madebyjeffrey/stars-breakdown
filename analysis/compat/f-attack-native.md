# FAttack native checkpoint

The fresh Win98 guest run is preserved in `runs/vmctl-vc152fat/`.

- `FASTA.TXT`: `COMPILE=PASS`, `LINK=PASS`
- `FACL.LOG`: VC++ 1.52c compiled the native wrapper containing the exact
  reviewed `FA.C` and `PROBE.C` source
- `FALINK.LOG`: segmented link completed with the retained `WINSTUB.EXE`
- `FAR.TXT`: `F_ATTACK_CHECKS=33 FAILURES=0 RESULT=PASS`
- the post-run guest window list showed no Program Error dialog

The transferred `FA.C` is byte-identical to `reconstructed/src/f_attack.c`,
and `PROBE.C` is byte-identical to `tests/f_attack_source_probe.c`. The
package also carries the reviewed `CTorpHit` helper and battle-coordinate
distance boundary needed by the exact source.

The probe covers player-mask and initiative filtering, target-mode selection,
beam broadcast and best-target paths, coordinate-range gating, beam scaling,
cargo adjustment, first-hit and hit-count fields, torpedo hit selection,
shield/hull dispatch, and optional battle-record updates.

This advances `FAttack` from Phase 2 step 3/6 to step 4/6. Packed hull/part
lookup, exact battle-part scaling, exact damage-engine integration, and full
battle-record integration remain open.
