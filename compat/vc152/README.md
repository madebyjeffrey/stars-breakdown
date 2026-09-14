# VC++ 1.52c compatibility experiment

This is the early step-5 compiler integration fixture for the user's Visual
C++ 1.52c environment in an 86Box Windows 98 SE VM. The guest instructions are
in [README.TXT](README.TXT). **RETURN2 passed all 563 native utility checks
and all 1,491 native layout checks**, completing the early compatibility
milestone. The offset runner resolves RETURN1's C2057 issue without removing
checks. See the [returned-build review](../../analysis/compat/vc152-return2.md).

Generate a fresh transfer folder and ZIP with:

```sh
python3 tools/package_vc152.py
```

The current outputs are `ftp-share/VC152C/` and `ftp-share/VC152C.ZIP`. This is
package 2A: a status-display/documentation revision of the validated package 2.
All C sources, compiler/linker response files and expectations are unchanged. No repeat VM build
is needed for the completed compatibility milestone. The exporter
uses a fixed allowlist and refuses to overwrite either output. Use `--output`
with a new directory for another package. The share is ignored by Git; canonical
fixture source, original-machine expectations, and the exporter are tracked.
No compiler, Windows libraries, game executable or private runtime data is
distributed. Record returned experiment evidence separately after review.

`PROBE.C`, `ABI.C`, generated `VECTEST.C` and the reviewed `utilgen.c` become a
small Win16 application. It checks integer and pointer widths, packed storage,
near/far calls, cdecl/pascal argument passing and cleanup, indirect calls and
DX:AX returns. Near calls stay within their module's code segment. The selected
eleven reconstructed functions require only small state structures, avoiding
the other host adapters' 64-KiB synthetic DGROUP allocation requirement.

`LAYOUT.C` separately compiles all four generated headers with
`STARS_NATIVE16` and links with `PROBE.C` compiled as `LAYMAIN.OBJ` using
`LAYOUT_PROBE`. This produces `LAYOUT.EXE` and `LAYRES.TXT`; failure here does
not prevent building/running the utility probe. Generated extern declarations
remain unreferenced metadata. `STARS_RUNTIME_OFFSETS` retains all 772 type-size
assertions and primitive checks, and defers the 1,469 field-offset assertions
to the shared generated `soffsets.inc` checklist. Every offset is evaluated
through the compiler's `offsetof` macro in function context. Ordinary header
consumers retain the complete compile-time checks by default. The checks cover
recovered storage views and pointer representations, not the original source
spelling or compiler bit-field allocation.

The build uses `/AM /GA /GEs /Zp1 /Od /Zi /W3`, and explicitly links `MLIBCEW`
and `LIBW` with `/NOD /ALIGN:16 /CO /MAP`. `/AM` is a hypothesis consistent with
the recovered ABI and runtime library identity. Packing and optimization flags
are controlled experiment choices, not proven original settings. The plain-C
recipe follows the medium Windows build described in Microsoft's archived
[KB Q88113](https://jeffpar.github.io/kbarchive/kb/088/Q88113/); the five response
fields follow [KB Q40429](https://jeffpar.github.io/kbarchive/kb/040/Q40429/).

The package preserves object files, executable CodeView records and a linker
map. These will support comparison of compiler records, generated instruction
idioms, calling conventions, linker metadata, runtime helpers and U011's direct
object library-index ambiguity. Even matching compiler records do not identify
the original IDE or uniquely prove a maintenance release.

## Expected results and local validation

On 2026-09-14, 113 cases captured from the original instructions supplied 288
value/state assertions. The exported host C89 probe passed all 552 checks;
the native probe expects 563. Both packaging tests pass, including the full
generated-header host compile, deterministic ZIP/integrity checks, preservation
of prior return artifacts, and deliberate vector mismatch detection. These
results preceded RETURN1's native utility success. Package 2 additionally
passes 1,480 host layout checks; the native layout executable expects 1,491.
Ten focused package/debug-recovery tests pass, including a deliberately moved
field that causes the new runtime layout check to fail. RETURN2 subsequently
passed both native builds and executions; no failed assertion was deleted.
The three additional return-assessment tests reject incomplete/incorrect
evidence, for 13 focused tests in total.

Capture/recheck expectations by executing the pinned original x86 instructions:

```sh
python3 -m tools.vc152_vectors --check
```

Omit `--check` only when deliberately refreshing `vectors.json`. The oracle
requires Unicorn's JIT permissions, as do the existing machine tests. Ordinary
packaging only reads the retained expectations. Each case retains its exact
NE procedure address; the seed getter/setter pair records both addresses.

Run packaging, host C89 execution and generated-header checks with:

```sh
python3 -m unittest tests.test_vc152_package -v
```

The host executables write `MODE=HOST_ONLY`; they omit native pointer sizes and
Win16 calling-convention validation. `MANIFEST.TXT` states the expected native
and host check counts. Mutation checks prove an incorrect expected vector and
an incorrectly padded field cause failing reports. Guest statuses remain
independent: COMPILE/LINK and LAYOUT/LAYLINK, then the two runtime reports.
A partial build must not be reported as a pass.

The sources and batch script use DOS 8.3 names and ASCII; export produces CRLF
and renames local quoted include filenames without changing reconstructed
logic. Exact pre/post hashes are retained in the transfer manifest. ZIP content
is deterministic and contains neither host paths nor Mac metadata.

On return, preserve the raw guest files and hashes before interpreting them.
The read-only reviewer can record an allowlisted return without ingesting FTP
client/server configuration:

```sh
python3 -m tools.review_vc152 ftp-share/RETURN2/Vc152b --package ftp-share/VC152B --out analysis/compat/vc152-return2.json
```

Check `MODE=WIN16_MEDIUM`, the expected check count, zero failures, successful
layout compilation, an NE executable and retained debug information. Compare
the actual emitted evidence with the original before updating uncertainties.
