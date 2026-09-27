# FillBuildDD packed design records — E1

This checkpoint closes the packed-design-record evidence unit for
`FillBuildDD`. The source now accepts a direct flat `_shdef` view while
retaining the typed design array as a host-adapter fallback.

The adapter uses the recovered 147-byte record stride, checks `fFree` at
offset 123 with the original visibility polarity, copies the class name from
offset 8, and reads the packed `cExist` dword at offset 131 for the first
edit-button decision. The probe initializes all neighboring records as free,
then supplies two visible packed records while deliberately conflicting the
typed shadow records; it also verifies that the raw record bytes are not
modified.

The strict host C89 probe reports
`FILL_BUILD_DD_PACKED_E1_CHECKS=8 FAILURES=0 RESULT=PASS`. The fresh VC++
1.52c/Win98 run in `runs/vmctl-vc152fbdp/` reports `COMPILE=PASS`,
`LINK=PASS`, and the same eight checks. The process exited 0 and the post-run
window inventory showed no Program Error dialog. The VC1.52c compile retains
one pre-existing signed/unsigned comparison warning in the enemy-design loop;
it does not affect the build or report.

The raw original listing and the existing original Win98 Ship & Starbase
Designer observation establish the surrounding behavior boundary. E2
localized labels/resources, E3 complete population/control integration, and
Step-6 promotion remain open.
