# FillBuildDD localized labels/resources — E2

This checkpoint closes the localized-label/resource evidence unit for
`FillBuildDD`. The readable source uses the recovered normal and starbase
compressed-string ID tables and applies them to both original label modes
(`md == 3` and `md == 4`).

The strict host C89 probe records every requested resource ID and verifies the
exact raw order: 13 normal IDs beginning at `0x52` and 8 starbase IDs
beginning at `0x7c`. It also checks both modes, the reset/add/list-count/end
message sequence, and the three control-disable calls made for a nonzero
mode. The host report is
`FILL_BUILD_DD_LABELS_E2_CHECKS=58 FAILURES=0 RESULT=PASS`.

The fresh VC++ 1.52c/Win98 package in `runs/vmctl-vc152fbdl/` reports
`COMPILE=PASS`, `LINK=PASS`, and the same 58 checks. The process exited 0,
the post-run window inventory showed no Program Error dialog, and the native
screen capture was preserved. The VC1.52c compile retains one pre-existing
signed/unsigned comparison warning in the enemy-design loop; it does not
affect the build or report.

The raw original listing uses the normal table at data offset `0x52` and the
starbase table at `0x7c`, calling `PszGetCompressedString` and adding each
returned label in order for both modes. E3 complete population, queue state,
resize, and caller/control integration, followed by Step-6 promotion, remain
open.
