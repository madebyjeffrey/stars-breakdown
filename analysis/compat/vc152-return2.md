# VC++ compatibility check completed: RETURN2

Reviewed 2026-09-14. The user returned `ftp-share/RETURN2/Vc152b/` after building
package 2 in VC++ 1.52c / Windows 98 SE / 86Box. All 23 outgoing files match
their returned copies byte-for-byte. The original return and outgoing package
remain intact. [Recorded evidence](vc152-return2.json) includes allowlisted
hashes, logs, both executable debug summaries and the completion assessment.

| Check | Result |
|---|---|
| Utility compile/link | PASS / PASS |
| Layout compile/link | PASS / PASS |
| Utility execution | 563 checks, zero failures, native medium model |
| Layout execution | 1,491 checks, zero failures, native medium model |
| Full offset checklist | All 1,469 field-offset checks executed |
| Size declarations | All 772 type-size assertions and primitive checks compiled |
| Source/transfer integrity | All 23 package inputs unchanged |

The **early step-5 VC++ compatibility milestone is complete**. The eleven
utility adapters, calling-convention examples and recovered storage headers
have passed this selected Win16 experiment. This does not complete step 5 or
establish full-game compatibility. U002 now concerns exact original toolchain
provenance/options rather than availability or the initial native layout check.

`PROBE.EXE` is 55,316 bytes; SHA-256
`3b1653abee0dd5413c7b4c48eff4658cbcc4658c10d82649dd279b9763332999`.
`LAYOUT.EXE` is 131,732 bytes; SHA-256
`b7aa3a639bffc88cbec902ce5af83c3916a0d1878df02b1702905033f88b735a`.
Both are NE executables with NB09 debug data. The layout executable has five
procedure records, 457 type records and source mappings covering
`SOFFSETS.INC`, `layout.c` and `probe.c`. The utility compiler records still
match the original's `8.00c` records; the same five arithmetic CRT modules
still match all 273 compared bytes without masking. See
[RETURN1](vc152-return1.md) for that comparison's method and limits.

## Status display observation

The user's screenshot shows excessive spacing and incomplete visible status
lines at the point where BUILD runs `TYPE BLDSTAT.TXT`. Other direct batch
messages appear normally. The returned status file is **87 bytes of ASCII**:
no BOM, no NUL bytes and six ordinary CRLF line breaks. Its complete contents
are:

```text
STARS_VC152_BUILD=2
ROOT=c:\msvc
COMPILE=PASS
LINK=PASS
LAYOUT=PASS
LAYLINK=PASS
```

The two runtime reports and other returned logs are also ASCII/CRLF. Therefore
the returned files are not UTF-16. The precise cause of the console rendering
seen in the screenshot has not been established; no encoding conversion is
applied to the retained evidence.

The revised batch script prints its compile/link statuses with direct `ECHO`
commands instead of displaying the status file with `TYPE`. It prints each
executable's run instruction only when that link stage succeeded, so the user
does not need to open the text file before deciding which program to run.
The complete status file is still written for evidence. This bypasses the
affected display path; its appearance in that VM can be observed during the
next ordinary build without repeating the completed compatibility experiment.

`ftp-share/VC152C/` (package 2A) contains this display revision. All C sources,
recovered headers, seed data, compiler/linker inputs and test expectations
are byte-identical to validated package 2. Only BUILD.BAT, README.TXT and the
transfer manifest differ. **No new compilation or return is requested.**

Thirteen focused local tests pass: package/host probes, generated-header
recovery, and return assessment. Negative cases reject failed link stages,
incorrect counts, host-only reports, altered inputs, missing executables and
duplicate report results. This tests the review logic; it does not pretend to
reproduce the user's console rendering.

Reproduce the evidence review:

```sh
python3 -m tools.review_vc152 ftp-share/RETURN2/Vc152b --package ftp-share/VC152B --out analysis/compat/vc152-return2.json
python3 -m unittest tests.test_vc152_review tests.test_vc152_package tests.test_debug_recovery -v
```
