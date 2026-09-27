# SatisfyOrders foreign-colonist typed fallback regression replay

The E2 consolidation exposed a compatibility regression in the current
production source: the new raw `FSendPlrMsg2` foreign-fleet-colonist branch
returned without invoking the legacy typed `send_message` callback when no
raw callback was installed. That broke the existing 32-check source probe.

The production source now selects the exact raw frame when `raw_message2` is
present, and otherwise preserves the typed callback fallback. Host strict-C89
validation passes both the original source probe
(`SATISFY_ORDERS_CHECKS=32 FAILURES=0`) and the raw foreign-colonist probe
(`SATISFY_ORDERS_RAW_FOREIGN_COLONIST_CHECKS=12 FAILURES=0`).

A fresh packet was replayed in Win98 directory `C:\SYOE24` using VC++ 1.52c.
The build reports `COMPILE=PASS` and `LINK=PASS`; the native report is
`SATISFY_ORDERS_RAW_FOREIGN_COLONIST_CHECKS=12 FAILURES=0 RESULT=PASS`, the
process exited 0, and the final guest window list contained no Program Error
dialog. The earlier `C:\SYOE23` attempt is retained as diagnostic evidence:
it used a copied packet whose wrapper selected the resource probe, so it is
not the authoritative result.

Evidence:

- `reconstructed/src/satisfy_orders.c`
- `tests/satisfy_orders_source_probe.c`
- `tests/satisfy_orders_raw_foreign_colonist_probe.c`
- `compat/vc152/satisfy-orders-raw-foreign-colonist-e2-native-r4/`
- `runs/vmctl-vc152syoe24/SYORESSTA.TXT`
- `runs/vmctl-vc152syoe24/SYORESCL.LOG`
- `runs/vmctl-vc152syoe24/SYORESLINK.LOG`
- `runs/vmctl-vc152syoe24/SYOFOR.TXT`
- `runs/vmctl-vc152syoe24/SYORES.MAP`
