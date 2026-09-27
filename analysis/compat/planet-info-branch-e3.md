# Planet information display branch integration (host E3)

## Result

The strict host C89 probe passes:

```text
PLANET_INFO_BRANCH_CHECKS=9 FAILURES=0 RESULT=PASS
```

## Scope

`tests/planet_info_branch_probe.c` runs both reconstructed planet-information
display procedures against one shared callback harness. It checks the
owned/unowned/foreign population headers, zero-mineral and negative-
desirability branches, foreign-environment disclosure, maximum-population and
growth-warning callbacks, the unknown-player state branch, known player and
planet limit rows, desirability improvement wrapping, environment service
calls, and the signed-difference layout path. Returned layout points and
font/stream/wrap dispatch are also asserted.

## Status

This is host-only E3 integration evidence. It does not close either
authoritative TODO6 Step-5 unit: native VC++ 1.52c replay, packed planet/player
lookup, localized fixed-point formatting, detailed GDI comparison, original
behavior, and promotion remain open. The Win98 guest was left running; its
Close Program dialog prevented a reliable agent handshake, so no native claim
is made.

## Inputs

- `reconstructed/src/pt_display_planet_pop_info.c`
- `reconstructed/src/pt_display_planet_state_info.c`
- `reconstructed/include/core.h`
- `tests/planet_info_branch_probe.c`
