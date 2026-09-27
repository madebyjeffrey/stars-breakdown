# EstMineralsMined packed/integration checkpoint

The reconstructed mineral-estimation procedure was checked against a raw
packed `_planet` byte view in a fresh VC1.52c/Win98 harness. The adapter uses
the recovered fields at player offset 2, percentage-level offsets 6–8,
concentration offsets 9–11, and `rgwtMin[3]` at offset 28. It loads those
fields into the typed input, runs the exact reconstructed source, and writes
the changed percentage and fixed-point mineral weights back to the same raw
layout.

The native report is
`EST_MINERALS_MINED_INTEGRATION_CHECKS=13 FAILURES=0 RESULT=PASS`; compile and
link also pass in fresh `C:\EMMINT4`. The post-run Win98 window list contains
no Program Error dialog. The original planet surface showed the Minerals On
Hand rows without an error dialog, which is recorded as the consumer/UI
integration boundary.

Evidence:

- `runs/vmctl-vc152emmint/EMIR.TXT`
- `runs/vmctl-vc152emmint/EMISTA.TXT`
- `runs/vmctl-vc152emmint/EMICL.LOG`
- `runs/vmctl-vc152emmint/EMILINK.LOG`
- `runs/vmctl-vc152emmint/native-screen.bmp`
- `runs/vmctl-vc152emmint2/EMISTA.TXT`
- `runs/vmctl-vc152emmint2/EMICL.LOG`
- `runs/vmctl-vc152emmint2/EMILINK.LOG`
- `runs/vmctl-vc152emmint2/EMIR.TXT`
- `runs/vmctl-vc152emmint2/EMI-CONSOLE.TXT`
- `runs/vmctl-vc152emmint2/native-screen.bmp`
- `runs/vmctl-vc152current/planet-frame-current.bmp`
- `runs/vmctl-vc152emmint4/EMISTA-MATRIX.TXT`
- `runs/vmctl-vc152emmint4/EMICL-MATRIX.LOG`
- `runs/vmctl-vc152emmint4/EMILINK-MATRIX.LOG`
- `runs/vmctl-vc152emmint4/EMIR-MATRIX.TXT`
- `runs/vmctl-vc152emmint4/native-screen-matrix.bmp`
- `tests/est_minerals_mined_integration_probe.c`
- `compat/vc152/est-minerals-mined-integration/`
- `analysis/decompiler/export/raw/1028_5362_EstMineralsMined.c`

The expanded native matrix covers invalid mining context, operating-mine
lookup and efficiency, both random-rounding outcomes, concentration-floor
handling, special-race fleet recursion and consumption, raw `rgwtMin`
write-back, the null-ops guard, and a packed fleet-order eligibility case.
That case binds the recovered 124-byte `_fleet` fields for player, planet,
flags, coordinate, and order task, and verifies the original consumed-bit
clear (`overlap_4 & 0xdfff`) while the 56-byte planet record receives the
mineral write-back.

The fresh `C:\EMMINT4` matrix closes the populated mining-turn differential
against the original instruction path. Two ordinary-race turns cover all
three minerals with efficiency factors 5/10 and 7/10, including the
pre-efficiency depletion product, partial percentage levels, concentration
boundaries, and fixed-point weights. A special-race turn covers the planet
mine contribution plus two fleet contributions and verifies sequential
percentage mutation and both consumed fleet orders. During this matrix the
source was corrected to preserve two original details that the earlier edge
cases did not expose: depletion uses the pre-efficiency product, and each
fully consumed concentration level resets the percentage cap to `0x100` and
writes zero before continuing. The expected vectors are derived from
`1028_5362_EstMineralsMined.c`, not from the reconstruction itself.

This closes the remaining Phase 2 Step-5 E1 evidence unit. It does not claim
whole-game turn-generation completion outside this procedure's acceptance
boundary.

Evidence for the prior expansion is preserved in
`runs/vmctl-vc152emmint3/`; the closing matrix is preserved in
`runs/vmctl-vc152emmint4/`.
