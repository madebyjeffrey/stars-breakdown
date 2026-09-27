# FAttack packed/integration checkpoint

The reconstructed attack procedure was exercised through recovered packed
Win16 battle storage in a fresh VC1.52c/Win98 harness. The adapter binds the
29-byte `_tok` view, including the token id, owner, battle coordinate,
shield-points word, ship-count word, packed target word, and active flags. It
also binds the 123-byte `_hul` view's hull-DP word and the six-byte battle
record.

The attack callback decodes those raw fields into the typed `FAttack` and
`FDamageTok` inputs, applies an 80-point beam through the shield and casualty
paths, writes the resulting shield and ship count back to `_tok`, and checks
the damage record and FAttack hit metadata. A second direct damage call checks
the shield-only branch and confirms the hull-DP field remains intact.

Host and native reports are:

`F_ATTACK_INTEGRATION_CHECKS=2 FAILURES=0 RESULT=PASS`

The fresh native package compiled and linked with VC++ 1.52c in `StarDev` at
`C:\FAINT`. The post-run window list contained no Program Error dialog. The
original battle-report surface is preserved as the consumer boundary in
`runs/vmctl-vc152current/report-battles.bmp`; it does not claim a populated
original-versus-reconstructed damage replay.

This advances `FAttack` to Phase 2 step 5/6, not promotion. Exact packed part
lookup, complete battle-part scaling, broader original damage differentials,
and full battle replay remain open.

Evidence:

- `tests/f_attack_integration_probe.c`
- `compat/vc152/f-attack-integration/`
- `runs/vmctl-vc152faint/FAISTA3.TXT`
- `runs/vmctl-vc152faint/FAICL3.LOG`
- `runs/vmctl-vc152faint/FAILINK3.LOG`
- `runs/vmctl-vc152faint/FAIR.TXT`
- `runs/vmctl-vc152faint/native-screen.bmp`
- `runs/vmctl-vc152current/report-battles.bmp`
