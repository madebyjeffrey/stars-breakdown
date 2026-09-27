# Rototill complete-turn VC1.52c compile boundary, r1

RT-E4.d requires the complete retained selector-3 coordinator to execute
under VC++ 1.52c and return build logs, a report, and state. The current
[strict-C89 host coordinator](../../tests/satisfy_orders_rototill_coordinator_e4_probe.c)
passes the complete source turn and original-file comparison, but its
existing test structure has never been a Win16 build.

On 2026-09-27, the passing host regression generated `TURINFX.H` (755,190
bytes). The fresh [compile-only package](../../compat/vc152/rototill-full-turn-r1/MANIFEST.JSON)
copied that fixture, the 527 KB Turin test included by the 141 KB Rototill
test, and current headers into flat DOS names at `C:\RTE4N1`. The guest
agent reported `STARSAGT 0.4.2`, `PONG`, and desktop windows. The wrapper
changed to that directory before invoking VC1.52c. The batch process
exited 0, but the returned [build status](../../runs/rototill-full-turn-r1/BLDSTAT.TXT)
is **`COMPILE=FAIL`**. The preserved [compiler log](../../runs/rototill-full-turn-r1/COCL.LOG)
first reports `TURINB.C(41): C2089 'TurinTrace': 'struct' too large` and
later `coord.c(1719): C1001 internal compiler error` at `msc1.cpp:585`.
The [return manifest](../../runs/rototill-full-turn-r1/MANIFEST.JSON)
pins package and returned-file hashes; the agent log is retained too.
There is no linked or executed native coordinator, so RT-E4.d and the
parent RT-E4 remain open.

The host `TurinTrace` was 261,024 bytes at this failure. An unused
`input_decode_data[65536]` field was then removed from the shared test
structure; the full host regression still passes (one test, 43.693
seconds), and the host structure is now 195,488 bytes. The retained
`Soe3CompleteInput` alone is 57,312 bytes on the host, and its two
static fixture entries total 114,624 bytes. Host sizes are diagnostic,
not Win16 layout measurements. They show that removing unused storage
cannot make the monolithic trace fit a 64 KB Win16 segment.

The next native packet needs a purpose-built coordinator with state split
across separately allocated Win16 segments and fixture views loaded in
bounded parts. It must preserve the existing complete-turn service sequence,
RNG transcript, and all original-output comparisons. Then compile the
source modules in separate objects, link, run, and return packed state and
complete files for RT-E4.d/e. The r1 compiler failure is a concrete sizing
diagnostic, not a passing native result or a reduced acceptance rule.

The subsequent [segmented compile sequence](do-rototill-segmented-native-r1.md)
has cleared the trace, code-segment, automatic-allocation, and `DGROUP`
compiler errors. The stage-9 object compiles; the current barrier is linking
the required reconstructed source modules. RT-E4.d remains open.

## Field-level split audit

The reproducible [layout audit](../../tools/rototill_native_layout_audit.py)
uses Clang's host record-layout output for the *current* coordinator and
retained fixture. Its [result](do-rototill-native-layout-r1.json) measures
`TurinTrace` at 195,488 host bytes, one `Soe3CompleteInput` at 57,312 bytes,
and the two-entry static input fixture at 114,624 bytes. The largest other
trace fields are the 23,332-byte race-data storage, 20,480-byte RNG file
buffer, two 12,288-byte three-file banks, and 8,192-byte AI data bank.

A 48,000-host-byte contiguous partition target yields five groups: the
57,312-byte input alone; 37,332 bytes from `turn_messages` through
`startup_written_percent`; 46,196 bytes from `rng_file_data` through
`append_input_header`; 47,668 bytes from `saved_mutation_stream` through
`basic_speed_fuel_calls`; and 6,980 bytes through `failures`. The input
already exceeds that conservative target, so it needs its own Win16 size
check and may need to be split by view. This is a partition guide, **not**
proof of VC1.52c sizes or a runnable native turn. The next packet must
replace the monolithic trace and duplicate static fixture, then compile and
run the complete selector-3 state path before RT-E4.d can close.
