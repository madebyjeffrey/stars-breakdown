# FAttack / FLookupPart absolute live-table addresses E45

Date: 2026-09-22

> **Superseded by E46.** E45's numeric offsets/strides matched the original
> instruction immediates, but its selector/anchor interpretation was wrong.
> Every branch executes `MOV DX,CS`; `0x10e0` is a synthetic Ghidra selector
> for NE29, not the part-table selector. The 66/66 native report therefore
> checked a self-consistent but incorrect segment model. Use E46 for current
> source-fidelity evidence.

E45 resolves the four nonliteral `FLookupPart` table bases using CodeView
global records and `reconstructed/include/sglobals.h`:

| Group | Symbol address | Original expression | Absolute family base | Stride |
|---|---:|---|---:|---:|
| `0x0100` special | `0025:12EC` | `&ptStickyRaceDlg + item*0x36` | `0x12EC` | `0x36` |
| `0x0200` planetary | `0025:4B98` | `rgbCur + 0x1F2 + item*0x38` | `0x4D8A` | `0x38` |
| `0x0400` message | `0025:484C` | `szMsgTitle + 0x26 + item*0x8F` | `0x4872` | `0x8F` |
| `0x8000` ZIP | `0025:16B8` | `(ushort *)&hwndZipOrderDlg + item*0x1B` | `0x16B8` | `0x36` bytes |

All use DGROUP selector `0x10e0`. The ZIP expression is word-pointer
arithmetic: each `item*0x1b` unit is scaled by two bytes. The previous E44
model/test incorrectly recorded `0x1b` bytes and is superseded; the passing
E44 report was only self-consistency evidence. Corrected expected selected
offsets include special item 2=`0x1358`, planetary item 15=`0x50D2`, message
item 4=`0x4AAE`, and ZIP item 14=`0x19AC`.

`StarsLookupPartInput` now reports `table_base_kind`, the exact
`table_base_offset`, selected `table_offset`, and stride. All sixteen valid
groups, plus the unsupported-group control, are exercised. The strict-C89
host report is:

```text
LOOKUP_PART_TABLE_CHECKS=66 FAILURES=0 RESULT=PASS
LOOKUP_PART_CHECKS=8 FAILURES=0 RESULT=PASS
```

Fresh VC++ 1.52c / Win98 packet `compat/vc152/f-lookup-part-anchor-native-e47/`
builds and links. The process exits 0, `LKTRES.TXT` reports 66 checks with zero
failures, and the guest window inventory contains no Program Error. Evidence
is in `runs/vmctl-faptae47/`. The full host suite passes 523 tests with 153
expected skips, 2,241 layout assertions, and 846 declarations;
`baseline.py --check` and `git diff --check` pass.

Two earlier `_printf` link failures were preserved in separate attempts
(`runs/vmctl-faptae44/` and `runs/vmctl-faptae45/`). E44's 50/50 packet return
is also retained but superseded because its ZIP expected stride was wrong.

This checkpoint closes **0 named Step-5 units**. Its selector/anchor
interpretation was incorrect and is retained only as an audit trail. E46
reconciles the pointers against raw instructions. The actual caller CS/table
memory view, complete race/hidden/`TechStatus` service binding, full FAttack
damage acceptance, original differential, battle replay, and Step-6 promotion
remain open.

## Evidence

- Raw procedure: `analysis/decompiler/export/raw/1008_524e_FLookupPart.c`
- CodeView/global source: `analysis/debug/globals.json`,
  `reconstructed/include/sglobals.h`
- Source: `reconstructed/src/lookup_part.c`,
  `reconstructed/include/common.h`
- Probe: `tests/lookup_part_table_probe.c`
- Native packet and return: `compat/vc152/f-lookup-part-anchor-native-e47/`,
  `runs/vmctl-faptae47/`
- Superseded E44 notes and return: `analysis/compat/f-attack-live-table-anchors-e44.{md,json}`,
  `runs/vmctl-faptae46/`
