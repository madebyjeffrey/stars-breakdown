# VC++ 1.52c returned build: experiment 1

Reviewed 2026-09-14. The user built package 1 in their VC++ 1.52c / Windows
98 SE / 86Box environment and returned `ftp-share/RETURN1/Vc152/`.
All 19 outgoing files match their returned copies byte-for-byte. The returned
folder is preserved. [Machine-readable evidence](vc152-return1.json) records
allowlisted artifact hashes, logs, compiler records and comparisons; FTP
client logs and server configuration are outside this review.

The utility executable **passed all 563 native checks**, with zero failures.
The full recovered-header compile **failed**, so the early compatibility
milestone remains open pending the revised layout experiment.

| Stage | Observed result |
|---|---|
| Utility compilation and linking | PASS |
| Utility runtime | `MODE=WIN16_MEDIUM`, `CHECKS=563`, `FAILURES=0`, `RESULT=PASS` |
| Recovered headers | C2057: expected constant expression, starting at `STYPES.H:7321`; stopped after 100 errors |
| User's `OUTPUT.TXT` | Only the existing-results guard from a second BUILD invocation; no compiler errors in this file |
| Build status schema | Version digit missing due to adjacent `1>` being interpreted as redirection; other statuses intact |

## What the successful build establishes

The eleven utility adapters run under actual 16-bit Microsoft C with the
113 original-x86 cases and their 288 return/state assertions. The remaining
checks cover platform/storage widths and repeated calling-convention tests.
The report records `_MSC_VER=800`, `_M_I86=1`, `_M_I86MM=1`, 2-byte `int`,
4-byte `long`, 2-byte default/near data pointers, 4-byte far/huge data pointers,
and 4-byte default function pointers.

`PROBE.EXE` is a 55,304-byte Windows NE executable with a 24,768-byte NB09
debug block. SHA-256:
`3b20ff1e7e5beea74e9f1554b1865c0072c684f732ab4d7c11c878d67372354d`.
The debug block contains four directly compiled objects, 53 runtime modules,
34 procedure records and 59 type records. This is a small experiment, not a
complete replacement application.

The four `S_COMPILE` records are byte-for-byte identical to all 40 game-object
compiler records: `8.00c`, near ambient data, far ambient code, and matching
recorded machine/floating-package flags. The actual linker banner is
`5.60.339 Dec 5 1994`; both the probe and original NE headers record `5.60`.
The original's full linker build number is not established by that header.

The new build reproduces library index **256** for `PROBE.OBJ`, `ABI.OBJ`,
`VECTEST.OBJ` and `UTILGEN.OBJ`. All 53 CRT modules use index 1, resolving to
`MLIBCEW.LIB`; the library table also lists `LIBW.LIB`. This reproduces U011's
previously unexplained direct-object pattern in a controlled build. Treat 256
as an observed direct-object convention here, not a reference to a nonexistent
256th library. Its universal/documented meaning is not claimed.

Five shared CRT modules match without relocation masking or normalization:

| Module | Bytes compared | Result |
|---|---:|---|
| `lmul.asm` | 50 | Exact |
| `lshl.asm` | 11 | Exact |
| `uldiv.asm` | 95 | Exact |
| `ulrem.asm` | 106 | Exact |
| `ulshr.asm` | 11 | Exact |

The comparison uses each OMF module's recorded segment span and CodeView
segment map, with the original executable pinned by its recorded SHA-256.
These 273 matching bytes support arithmetic-runtime compatibility; they do
not certify the entire CRT or floating-point behavior.

The emitted ABI examples also agree with their declarations: far cdecl ends
in `RETF`, far pascal ends in `RETF 8`, and the same-module near cdecl routine
ends in `RET`. The wrapper performs a near call and removes eight argument
bytes. Procedure types record conventions 1, 3 and 0 respectively. The runtime
test separately exercised mixed-width arguments, indirect pascal calls and
32-bit returns. Exact instruction bytes and file locations are in the JSON.

This is strong evidence that the user's environment is suitable for ongoing
Win16 reconstruction experiments. Matching metadata does not uniquely identify
the original IDE/maintenance release or prove the original optimization and
packing flags. U002 remains open at that narrower scope.

## Fix and next experiment

The C2057 diagnostics point to `offsetof` expressions used as file-scope array
bounds. They do not establish incorrect storage layouts. The evidence does not
isolate whether nesting, the compiler's macro implementation, or another aspect
of its constant-expression handling is responsible. Package 2 therefore avoids
requiring these expressions to be constant, while retaining the actual offset
measurements and expected values.

The header generator now emits one shared `soffsets.inc` checklist containing
all 1,469 field offsets. Ordinary consumers still assert them at compile time.
The native runner opts into `STARS_RUNTIME_OFFSETS`, retains all 772 type-size
assertions and primitive checks, and evaluates every offset in function context.
The storage declarations, field positions and expected offsets are unchanged.
Deferral alone is not validation: the runner must execute successfully.

Package 2 builds two independent executables. `PROBE.EXE` should again pass
563 checks; `LAYOUT.EXE` should pass 1,491 checks and write `LAYRES.TXT`.
Both remain native package-2 experiments pending the user's returned results.
The batch status version uses redirection before the command to preserve its
digit, and the existing-results guard remains intact.

Ten focused tests pass locally: header regeneration/C89 compilation, all 552
host utility checks, all 1,480 host layout checks, package integrity, and two
negative controls. Changing a utility expected value causes a failed report;
moving a planet owner field from offset 2 to 3 also causes a failed report.
The native pointer/ABI checks are not simulated by these host passes.

The revised transfer package is `ftp-share/VC152B/` and `ftp-share/VC152B.ZIP`.
Build it in a fresh VM folder, run both executables, and return the whole build
directory under `ftp-share/RETURN2/`. Full guest instructions accompany it.

Reproduce the review:

```sh
python3 -m tools.review_vc152 ftp-share/RETURN1/Vc152 --package ftp-share/VC152 --out analysis/compat/vc152-return1.json
python3 -m unittest tests.test_vc152_package tests.test_debug_recovery -v
```
