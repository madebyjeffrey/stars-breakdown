# VC++ 1.52c compatibility experiment

This is the early step-5 compiler integration fixture for the user's Visual
C++ 1.52c environment in an 86Box Windows 98 SE VM. The guest instructions are
in [README.TXT](README.TXT). Native compilation and execution are **pending**;
local C89 checks cannot establish a successful Win16 build.

Generate a fresh transfer folder and ZIP with:

```sh
python3 tools/package_vc152.py
```

The default outputs are `ftp-share/VC152/` and `ftp-share/VC152.ZIP`. The exporter
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
`STARS_NATIVE16`. A failure here does not prevent building/running the utility
probe. The layout object is not linked: generated extern declarations are
metadata checks, not implemented game routines. The checks cover recovered
storage views and pointer representations, not the original source spelling
or compiler bit-field allocation.

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
results do not change the pending native-build status.

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

The host executable writes `MODE=HOST_ONLY`; it omits native pointer sizes and
Win16 calling-convention validation. `MANIFEST.TXT` states the expected native
and host check counts. A mutation check proves an incorrect expected vector
causes a failing report. Guest build results remain independent: COMPILE, LINK,
LAYOUT, then the runtime report. A partial build must not be reported as a pass.

The sources and batch script use DOS 8.3 names and ASCII; export produces CRLF
and renames local quoted include filenames without changing reconstructed
logic. Exact pre/post hashes are retained in the transfer manifest. ZIP content
is deterministic and contains neither host paths nor Mac metadata.

On return, preserve the raw guest files and hashes before interpreting them.
Check `MODE=WIN16_MEDIUM`, the expected check count, zero failures, successful
layout compilation, an NE executable and retained debug information. Compare
the actual emitted evidence with the original before updating U002 or U011.
