# FAttack unsigned initiative-wrap correction

The raw `FAttack` instruction path stores `tok.initBase + part.dp` in a
16-bit unsigned temporary and only caps values above `0x3f`. It does not
clamp a negative signed part delta to zero. The typed source now models that
wrap-before-cap rule explicitly.

The focused strict-C89 source probe adds the boundary case
`init_base=0`, `init_delta=-1`, and `input.init=63`; the wrapped `0xffff`
value is capped to 63 and dispatches the attack. Existing source, packed
token/hull, and battle-chain probes remain green:

```text
F_ATTACK_CHECKS=34 FAILURES=0 RESULT=PASS
F_ATTACK_INTEGRATION_CHECKS=2 FAILURES=0 RESULT=PASS
F_DO_COOL_BATTLE_ATTACK_INTEGRATION_CHECKS=18 FAILURES=0 RESULT=PASS
```

This is a host source-fidelity correction. It closes no Phase-2 Step-5
evidence unit: exact packed `FLookupPart` binding, complete battle-part
scaling, broader original damage differentials, and native battle-chain
replay remain open. The Win98 guest-agent endpoint is still disconnected, so
the native rerun is pending reconnection.

Evidence:

- `analysis/decompiler/export/raw/10f0_69ac_FAttack.c`
- `analysis/debug/types.json` (`_beam`/`_torp` `dp` fields are signed 16-bit records)
- `reconstructed/src/f_attack.c`
- `reconstructed/include/core.h`
- `tests/f_attack_source_probe.c`
