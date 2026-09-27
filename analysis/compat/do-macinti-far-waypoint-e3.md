# DoMacintiAiTurn MA-E3: distant second-waypoint truncation

Status: **supporting evidence only; MA-E3 remains open**.

The original first fleet sweep at `10a0:1a7d–1b58` tests every fleet,
including foreign fleets, before the owner-specific action logic. It requires
`cord > 1` and order 1's high-nibble task kind (bits 8–11 of its kind word)
to equal 8. It subtracts the second order's point (`PLORD` offsets 22/24)
from the fleet point (record offsets 8/10) as two 16-bit words, sign-extends
both differences, squares and sums them in 32 bits, then truncates when the
signed result exceeds 40000. The decompiler's apparent six-unit offset is
not present in the instruction sequence. The selection call is followed by
setting selected `cord` to 1, selected `PLORD.iordMac` (header byte 3) to 1,
and a fleet lookup/refresh before subsequent fleet decisions.

The bound Macinti path now performs that predicate and selected-record
mutation through `select_fleet`/`refresh_fleet` callbacks. Its preflight
requires a complete second 18-byte order and callbacks whenever a distant
second waypoint could be truncated. It rebinds the selected packed view for
the remainder of the first sweep. The host adapter requires the refresh
callback to publish any selected-buffer changes to the live record used by
later passes; this identity relationship still needs native confirmation.

The strict-C89 `tests/do_macinti_far_waypoint_e3_probe.c` passes **8/8**:
foreign-fleet truncation, 201-versus-200 threshold, 16-bit subtraction
wrap, signed-32-bit sum overflow, non-kind-8 exclusion, missing-callback
preflight, short-PLORD preflight, and selected-buffer rejection without
mutation. The earlier E1 and E2 probes pass 31/31 and 12/12; the complete
host suite passes **570 tests**, 153 expected skips, zero failures.
Baseline and diff checks are clean.

The fresh combined DOS-8.3/CRLF VC1.52c packet at
`compat/vc152/macinti-planet-table-e1-r16/` targets `C:\MAE1N16` and
includes this, first-transport, buddy/miner and expanded order probes
(31/13/12/10/16/8/14). It is **staged,
not compiled or run in Win98**; STARSAGT did not connect to the latest
local listener. MA-E3 still requires complete
multi-order PLORD mutation and route sequencing, a native return, and an
action-bearing original-executable comparison. The 8/8 check does not close
the named evidence unit or the procedure promotion.
