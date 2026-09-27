# SO-E2 current-source v4 repricing closure

This closes the final SO-E2 gate: the exact original high-offer case now runs
through the reconstructed packed task-5 path and the VC1.52c guest build.

## Bound original inputs

The [v4 original control/action oracle](satisfy-orders-reprice-v4-bitc-original-oracle.json)
pins the shared H2, original executable and HST hashes, task-0/task-5 order
pair, owner-1 fleet 0, and the slot-0 hull-9 SHDEF with eleven components.
The owner input is player 1 technology `[0,0,1,0,5,0]`, race stat 1, and race
bits `0x80003045` with bit C set. The exact part records are extracted from
the pinned original executable's NE segment 2 into
[`satisfy_orders_reprice_v4_bitc_parts.h`](../../tests/fixtures/satisfy_orders_reprice_v4_bitc_parts.h);
the fixture manifest records each part's offset, technology, cost lanes, and
hash. The task-5 planet starts at 469/893/376 minerals and 250 colonists.

## Corrected original flag mapping

The decompiled original sets `gd.grBits2` bit 3 immediately before its
task-5 `UpdateShdefCost` calls (`analysis/decompiler/export/raw/10b0_6798_SatisfyOrders.c`,
around lines 1162–1164) and clears it after cost calculation (around line
1442). `GetTruePartCost` tests that same bit (`analysis/decompiler/export/raw/1050_cd00_GetTruePartCost.c`,
around line 109). The reconstruction had checked `0x0800`; it now checks
`0x0008`, and packed task 5 sets the transient bit for repricing and clears it
after the cost pass. Failure while repricing also clears it. The regression
probe checks both that the bit suppresses technology-premium doubling and
that the task-5 call clears it afterward.

## Host and native result

`tests/satisfy_orders_reprice_v4_live_probe.c` binds the exact part segment,
SHDEF, race, technology, planet, packed fleet/order, part lookup, repricing,
technology discovery and message callbacks. Strict-C89 host test
[`test_satisfy_orders_reprice_v4_live.py`](../../tests/test_satisfy_orders_reprice_v4_live.py)
passes. The matching [VC1.52c native report](../../runs/vmctl-soe2s/SOE2SRES.TXT)
also passes **74/74**:

```text
V4_LIVE_TASK5_CHECKS=74 FAILURES=0 RESULT=PASS LOOKUPS=12 REPRICE=4 COST=1930,494,681,261 EVENTS=KUUUUMTMRD MESSAGES=90,317 RECIPIENTS=1,0 MINERALS=864,1437,584 TECH=1 RESEARCH0=0 FLEET_DEAD=1
```

This verifies twelve original part lookups, four live repricing calls, the
original callback order, the `0x5A` and `0x13D` messages and recipients,
technology result, planet mutation, and fleet retirement. The isolated
task-5 mineral return is 395/544/208 from the pinned starting stockpile. The
original full-turn action/control difference is 396/543/209; that one-unit
per-lane gap is outside the isolated task-5 accounting and remains part of
SO-E3's surrounding mining/RNG comparison.

The reproducible [native run manifest](../../runs/vmctl-soe2s/MANIFEST.JSON)
retains the compiler and linker logs, status, map, object, executable, report,
input packet manifest, and SHA-256 hashes. The fresh DOS 8.3 / CRLF packet is
[`satisfy-orders-reprice-v4-e2-native-r1`](../../compat/vc152/satisfy-orders-reprice-v4-e2-native-r1/).
The standalone report executable ran under `C:\SOE2S`; Stars! itself was not
launched in this verification. The attached `C:\SOTC\CLEAN.X2` error dialog
is from the earlier invalid candidate and is not part of this isolated
current-source native run or the pinned `C:\SVC92*` original pair.

All **27** `test_satisfy_orders*.py` tests pass. SO-E2 is complete; the
separate full-turn differential remains open as SO-E3.
