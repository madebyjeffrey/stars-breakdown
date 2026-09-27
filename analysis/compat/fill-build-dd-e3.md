# FillBuildDD complete population/control integration — E3

This checkpoint closes the final Step-5 evidence unit for `FillBuildDD`.
The focused matrix covers the full callback boundary rather than only one
list branch: normal visible/hidden designs, queued and already-built edit
state, an empty design list, the 32-row resize clamp, normal and starbase
special-part limits, and enemy-design population that skips the current
player and composes player/design names.

It also verifies the complete event order around the callback adapter:
reset, add-string calls, count retrieval, resize, and end notification. The
strict host C89 probe reports
`FILL_BUILD_DD_E3_CHECKS=14 FAILURES=0 RESULT=PASS`.

The corrected VC++ 1.52c/Win98 package in
`runs/vmctl-vc152fbde2/` reports `COMPILE=PASS`, `LINK=PASS`, and the same
14 checks. The process exited 0, the post-run window inventory showed no
Program Error dialog, and the native screen capture was preserved. The first
package attempt is retained in `runs/vmctl-vc152fbde/`; it exposed and
preserved the no-CRT `_printf` linker diagnostic, which was fixed by guarding
the host-only print before the passing rerun. The VC1.52c compile retains one
pre-existing signed/unsigned comparison warning in the enemy-design loop.

The raw original listing establishes the mode-specific limits, control
enablement, queue/cExist edit rule, special/enemy branches, 32-row clamp,
resize formula, and terminating notification. E1, E2, and E3 are now all
closed; only Step-6 ledger promotion remains.
