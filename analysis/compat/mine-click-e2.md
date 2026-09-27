# MineClick E2 popup-resolution checkpoint

This packet closes the named Step-5 E2 boundary for `MineClick`: the typed
popup payload now carries the popup kind, localized resource identity, and the
derived mineral/terraform values that the original `GlobalPD` overlay prepares
before calling `Popup`.

`reconstructed/src/mine_click.c` maps the recovered hit families to their
popup kinds, asks the host-owned localization service for the resource ID, and
invokes typed mineral and terraforming resolvers before the final popup
callback. The payload preserves the selected planet variable, player bounds,
terraform bounds, and three mineral outputs. The existing callback-only path
is unchanged when no raw packed records are supplied.

The strict host probe reports:

```text
MINE_CLICK_CHECKS=36 FAILURES=0 RESULT=PASS
```

A fresh VC++ 1.52c / Win98 package in `C:\MCLE3` compiles and links the exact
current source and produces the same native report. The process exits 0, the
build status is `COMPILE=PASS` / `LINK=PASS`, redirected console output is
empty, and the post-run window inventory contains no Program Error dialog.

This closes MineClick E2 and moves it from `1/3` to `2/3` named Step-5
evidence units. E3 complete minefield caller integration and the final review
promotion remain open.

Evidence: `tests/mine_click_source_probe.c`,
`reconstructed/src/mine_click.c`, `compat/vc152/mine-click-e3/`, and
`runs/vmctl-vc152mcle3/`.
