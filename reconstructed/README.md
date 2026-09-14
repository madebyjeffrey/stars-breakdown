# Reconstructed source

Step 2 generates storage-view headers and effective declarations in `include/`. They are reproducible with `python3 tools/debug_headers.py`; see `analysis/debug/README.md` for their representation, limitations and checks.

Step 5 adds 54 reviewed routines in `src/`, compiled as a host C89 semantic library.
Build with `python3 tools/build_core.py`. Their original addresses, behavior,
machine-code comparisons, and limits are recorded in
[the reconstruction report](../analysis/reconstruction/core.md).

No complete application implementation exists yet. The core API in `include/core.h`
uses explicit state and memory instead of the original far-call ABI. Header
compilation alone is not counted as compiled function reconstruction.

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
