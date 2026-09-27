# FAttack design-to-hull normal/alternate table branches — E42

E42 extends E41's real `FDoCoolBattle` → `FAttack` design/hull integration to
exercise both branches of the original `LpshdefFromTok` selector in the same
strict-C89 host and VC++ 1.52c/Win98 packet. The battle callback attacks with
player 1/design `0x12`, verifying the alternate hull table at index 2 and the
selected hull's raw `_hs` group `0x10`, item 7, count 2 through lookup/damage.
A second direct adapter case uses player 3/design 5 and verifies the normal
table at index 5, selector `0x1357`, and the zero-slot hull path.

The original field mapping remains `_tok.iplr` byte 2 and `_tok.ishdef` byte 4.
For the resolved 123-byte `_hul`, sixteen inline four-byte `_hs` entries begin
at byte 58 and `chs` is byte 122. A `chs` count beyond sixteen is rejected.

## Validation

- Strict-C89 integration report:
  `F_DO_COOL_BATTLE_ATTACK_INTEGRATION_CHECKS=44 FAILURES=0 RESULT=PASS`.
- Fresh `C:\FCBATE42` VC++ 1.52c compile/link pass; Win98 report is 44/44,
  process exit 0, no compiler warnings/errors, and no Program Error dialog.
- Full host suite passes 523 tests, 153 expected skips, 2,241 layout
  assertions, and 846 declarations. Baseline and diff checks pass.
- Packet: `compat/vc152/f-do-cool-battle-design-hull-e42/`; native evidence:
  `runs/vmctl-fcbate42/`.

This closes **0 named Step-5 units**. The branch selection and record decoding
are now covered through the reconstructed adapter, but exact native table
globals/far pointers, complete part services, full damage acceptance, original
differential, battle replay, and Step-6 promotion remain.
