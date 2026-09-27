# ExecuteReportClick E1 resource GlobalPD overlap correction

Original `1108:7CD6` resource-cell branch writes `GlobalPD.grPopup=12`,
`idPlanet=sel.pl.id`, then `iPlanetVar=CResourcesAtPlanet`. It initializes
`iPlanVal` from `part.hs.overlap_2`, the same union word just written as
`iPlanetVar`, before subtracting the research share when applicable
(`analysis/decompiler/export/raw/1108_7cd6_ExecuteReportClick.c:252-263`).
The resource popup consumer reads the first union word as the planet ID for
its title (`10c0_3378_PtDisplayResourceInfo.c:38`).

The reconstructed dispatcher had incorrectly written an unrelated
caller-supplied `globalpd_resource_part` at raw union offset `+2`, erasing
the planet ID. It now writes the selected planet ID at `+2`, resources at
`+4`, and resources minus research share at `+6`. The typed `resource_part`
view is the alias of the resource word, not a separate input. The existing
legacy ops field remains in the header but is ignored by this original
branch. The probe deliberately seeds it with a different value and checks
the complete first long-word overlay as well as the three words.

The strict-C89 probe and fresh VC++ 1.52c/Win98 `C:\GPDE3` replay pass
`EXECUTE_REPORT_CLICK_GLOBALPD_CHECKS=20 FAILURES=0 RESULT=PASS` over fleet,
starbase, defense, and resource popup cases. Build status is
`COMPILE=PASS LINK=PASS`, process exit is zero, and the post-run window list
has no Program Error. Package:
`compat/vc152/execute-report-click-resource-union-e1/`; returned logs,
map, object, executable, report, and agent log: `runs/vmctl-gpde3/`.
The full host suite passes 540 tests (153 expected skips), including the
new dedicated regression runner.

This corrects one original-instruction-derived union value, not the named
ER-E1 boundary. Exact live fleet/starbase far-pointer and defense-part
values across report clicks, live USER/GDI, and all caller/layout branches
remain. No Step-5 unit or promotion closes.
