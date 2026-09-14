# Reconstructed source

Step 2 generates storage-view headers and effective declarations in `include/`. They are reproducible with `python3 tools/debug_headers.py`; see `analysis/debug/README.md` for their representation, limitations and checks.

Step 5 adds 59 reviewed routines in `src/`, compiled as a host C89 semantic library.
Build with `python3 tools/build_core.py`. Their original addresses, behavior,
machine-code comparisons, and limits are recorded in
[the reconstruction report](../analysis/reconstruction/core.md).

No complete application implementation exists yet. The core API in `include/core.h`
uses explicit state and memory instead of the original far-call ABI. Header
compilation alone is not counted as compiled function reconstruction.

The [VC++ 1.52c compatibility fixture](../compat/vc152/README.md) packages eleven
utility routines and a separate native recovered-header check for the user's
Win98 SE / 86Box VM. The first returned Win16 utility build passed all 563
checks. The revised full layout runner is pending after C2057 errors in the
original compile-time offset assertions; guest instructions accompany
`ftp-share/VC152B/`. See the [returned-build review](../analysis/compat/vc152-return1.md).

Try the reconstructed race generator without Windows:

```sh
python3 tools/race_demo.py --seed 0
```

It prints a synthetic race using reconstructed code and verified original static
string/template data. See [the random-race report](../analysis/reconstruction/random-race.md).

Use `--record-roundtrip` to encode and decode a controlled BOF/player/checksum
stream. This reports byte/checksum differences without changing them; see
[player-record transformations](../analysis/reconstruction/player-records.md)
for the explicit I/O boundary and legacy-name limits.

The demo's header now runs reconstructed WriteBOF; use `--tick 0` to supply a
controlled Windows tick value. [File streams](../analysis/reconstruction/file-streams.md)
documents open/retry/close and transfer routines, their error callbacks, and
additional observations through the unchanged original save/load callers.

[File creation and alerts](../analysis/reconstruction/file-creation.md) adds
filename rules, native recovery around stream creation, error classification,
and dialog/log selection. Message expansion and actual log output remain
explicit integration callbacks.
