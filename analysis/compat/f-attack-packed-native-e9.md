# FAttack native packed lookup and typed-scaling support

## Result

The current production `lookup_part.c` and `f_attack.c` sources were compiled
and linked under VC++ 1.52c in a fresh `C:\FAPNE9` guest directory. The native
packet ran three executables with no Program Error dialog:

```text
LOOKUP_PART_TABLE_CHECKS=34 FAILURES=0 RESULT=PASS
F_ATTACK_PACKED_PART_CHECKS=13 FAILURES=0 RESULT=PASS
F_ATTACK_CHECKS=34 FAILURES=0 RESULT=PASS
```

The final compile and link logs are warning-free. The packet covers the eleven
fixed `FLookupPart` table families, the resource-relative control case, the
raw `_hs` group/item/count resolver, component identity and table offsets, and
the current readable FAttack beam/torpedo/scaling matrix.

## Step-5 classification

This closes the native lookup/resolver subcheck and provides native support for
the current typed scaling matrix, but it does not close `FAttack` E1. The raw
complex-beam path still contains target hull/resource/shield/jam/capacity
formulas not yet represented by the readable source. Complete raw
battle-part scaling, broader original damage differentials, native/full battle
replay, and final ledger promotion remain open. `FAttack` remains at `0/3`
named Step-5 units and the active `5/6` gate.

## Evidence

- `compat/vc152/f-attack-packed-native-e9/`
- `runs/vmctl-vc152fapne9/FAPNATSTA-final3.TXT`
- `runs/vmctl-vc152fapne9/LOOKTABR.TXT`
- `runs/vmctl-vc152fapne9/FAPARTR.TXT`
- `runs/vmctl-vc152fapne9/SCALR.TXT`
- `runs/vmctl-vc152fapne9/LOOKCL-final2.LOG`
- `runs/vmctl-vc152fapne9/FAPCL-final2.LOG`
- `runs/vmctl-vc152fapne9/SCALCL-final.LOG`
