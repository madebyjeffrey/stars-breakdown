# FAttack / FLookupPart live-table anchor checkpoint E44

Date: 2026-09-22

> **Superseded by E45.** E44's native 50/50 report validated the model as
> written, but the ZIP branch incorrectly treated `(ushort *) + item*0x1b` as
> a `0x1b`-byte stride. The original pointer arithmetic scales by two, so the
> byte stride is `0x36`. E45 uses the CodeView global addresses and corrected
> stride; this checkpoint is retained as history, not original-fidelity proof.

The raw `FLookupPart` branches show four table families whose address is
relative to a live DGROUP symbol rather than a recovered fixed offset:

| Group | Original base expression | Selector | Record stride |
|---|---|---:|---:|
| `0x0100` special | `ptStickyRaceDlg + item*0x36` | `0x10e0` | `0x36` |
| `0x0200` planetary | `rgbCur + 0x1f2 + item*0x38` | `0x10e0` | `0x38` |
| `0x0400` message | `szMsgTitle + 0x26 + item*0x8f` | `0x10e0` | `0x8f` |
| `0x8000` ZIP | `(ushort *)&hwndZipOrderDlg + item*0x1b` | `0x10e0` | `0x36` bytes |

`StarsLookupPartInput` now identifies those base symbols separately from fixed
offset tables. `table_offset` is documented and tested as the anchor-relative
displacement for these families. Absolute DGROUP offsets are intentionally not
asserted until the runtime globals are bound. The other twelve family tests
retain their fixed offsets, and the group `0x0020` current-CS selector remains
separate.

The host strict-C89 report is:

```text
LOOKUP_PART_TABLE_CHECKS=50 FAILURES=0 RESULT=PASS
LOOKUP_PART_CHECKS=8 FAILURES=0 RESULT=PASS
```

A fresh VC++ 1.52c / Win98 harness builds the production `lookup_part.c` with
the expanded probe. `BLDSTAT.TXT` records compile/link pass; the process exits
0; `LKTRES.TXT` reports 50 checks, zero failures; and `windows` shows no
Program Error. The suite passes 523 tests with 153 expected skips, 2,241
layout assertions, and 846 declarations. `baseline.py --check` and
`git diff --check` pass.

The first two native attempts failed to link because the GUI test referenced
`_printf`; both failures and their logs remain in `runs/vmctl-faptae44/` and
`runs/vmctl-faptae45/`. The passing fresh run is retained in
`runs/vmctl-faptae46/`, packet source in
`compat/vc152/f-lookup-part-anchor-native-e46/`.

This checkpoint closes **0 named Step-5 units**. Its earlier `0x1b` byte
stride claim was incorrect; see E45 for the corrected original-derived model.

## Evidence

- Raw branch expressions: `analysis/decompiler/export/raw/1008_524e_FLookupPart.c`
- Source and public descriptor: `reconstructed/src/lookup_part.c`,
  `reconstructed/include/common.h`
- Probe: `tests/lookup_part_table_probe.c`
- Native packet: `compat/vc152/f-lookup-part-anchor-native-e46/`
- Native build/result: `runs/vmctl-faptae46/` (historical; superseded)
- Preserved failed attempts: `runs/vmctl-faptae44/`,
  `runs/vmctl-faptae45/`
