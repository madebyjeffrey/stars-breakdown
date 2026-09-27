# MineClick packed-record E1 checkpoint

This packet closes the named Step-5 packed-record boundary for `MineClick`.
It does not claim the remaining localized popup, derived mining arithmetic, or
complete minefield caller integration.

`reconstructed/src/mine_click.c` now accepts optional raw Win16 `_planet`
(56-byte), `_fleet` (124-byte), and `_thing` (18-byte) records. It decodes
the selected planet identity/owner/detail and environment bounds, fleet
identity/owner/planet/ship-count fields, and thing identity/owner/kind/point
fields into a typed popup payload. The existing callback-only typed path is
preserved when no raw records are supplied.

The strict host probe reports:

```text
MINE_CLICK_CHECKS=30 FAILURES=0 RESULT=PASS
```

A fresh VC++ 1.52c / Win98 package in `C:\MCLE2` compiles and links the exact
current source and produces the same native report. The process exits 0, the
build status is `COMPILE=PASS` / `LINK=PASS`, redirected console output is
empty, and the post-run window inventory contains no Program Error dialog.

This closes MineClick E1 and moves it from `0/3` to `1/3` named Step-5
evidence units. E2 localized popup construction/derived arithmetic and E3
complete minefield caller integration remain.

Evidence: `tests/mine_click_source_probe.c`,
`reconstructed/src/mine_click.c`, `compat/vc152/mine-click-e2/`, and
`runs/vmctl-vc152mcle2/`.
