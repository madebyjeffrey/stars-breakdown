# FAttack native DGROUP hull-table far-pointer path — E43

E43 adds `stars_lpshdef_from_tok_globals`, a second production lookup path that
uses the recovered DGROUP tables instead of a player/design callback. The
original symbols place `rglpshdef` at NE `0025:00FE` and `rglpshdefSB` at
`0025:014C`; each is a 16-entry array of four-byte far pointers, stored as
offset word then segment word. A token design below `0x10` selects the first
table; otherwise it selects the second and subtracts `0x10`. The design record
stride is `0x93` bytes. The final segment:offset is passed to an explicit
resolver because host C pointers cannot dereference arbitrary segmented guest
addresses.

`stars_attack_bind_cool_battle_global_hull` composes that lookup with the
existing `_hul`/`_hs` decoder. The battle integration fixture writes realistic
little-endian far-pointer entries into raw DGROUP bytes, then maps only the
selected addresses to 123-byte hull records. The alternate case reads player
1's table entry at `0x0150` (`1234:2000`) and adds `2 * 0x93` to obtain
`1234:2126`; the normal case reads player 3's entry at `0x010A`
(`2345:1000`) and adds `5 * 0x93` to obtain `2345:12DF`. Both continue through
the actual attack callback, raw part lookup, damage, and battle lifecycle.

This proves the reconstructed raw-global offset/segment selection and native
compiler behavior. It does not claim to link the whole game executable's
DGROUP symbols or remove the final address resolver service.

## Validation

- Strict-C89 host and fresh `C:\FCBATE43` VC++ 1.52c/Win98 integration report
  pass 44/44. Compile/link pass, process exits 0, no compiler warnings/errors,
  and no Program Error dialog.
- Full host suite passes 523 tests, 153 expected skips, 2,241 layout
  assertions, and 846 declarations. Baseline and diff checks pass.
- Packet: `compat/vc152/f-do-cool-battle-global-table-e43/`; native evidence:
  `runs/vmctl-fcbate43/`.

This closes **0 named Step-5 units**. The resolver must still be bound to the
real native per-player table far pointers and hull segment memory; full
`FLookupPart` services, damage acceptance, original differential, full battle
replay, and Step-6 promotion remain open.
