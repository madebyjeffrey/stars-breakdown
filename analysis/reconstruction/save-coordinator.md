# Save coordinators and battle serialization

The save procedures are being represented as semantic C coordinators rather
than as wrappers around precomputed bytes.  The source keeps the ordering and
record-size decisions that are observable at the file boundary; the original
far-pointer arrays and heap services are explicit callbacks until their full
layouts are integrated.

## FWriteDataFile

[`reconstructed/src/write_data_file.c`](../../reconstructed/src/write_data_file.c)
implements the save sequence for `FWriteDataFile` (NE 15:5964-704D).  It sets
the visible-plan/fleet view, runs the player-view preparation, marks messages
and lost planets, then tries append mode before falling back to a fresh game or
player file.  Once open, the order is battles, eligible player snapshots,
game-only salt, messages, planets, the sixteen-slot and ten-slot design banks,
fleets, battle plans, the final turn marker, and close/reset of the visible
view.  Alert ids are named in the public interface, including the distinct
append failure.

The host probe covers game-file creation, player-file append, append fallback,
visibility reset, player/design/fleet predicates, salt placement, and the
three save-order/error paths. The current source compiled and linked
warning-free with VC1.52c under the Win98 StarDev environment, and the native
probe passed all 3 checks (`runs/vmctl-vc152ew/fwrite-data-file-r1/WDFRES.TXT`).

## WriteBattles

[`reconstructed/src/write_battles.c`](../../reconstructed/src/write_battles.c)
implements `WriteBattles` (NE 15:709C-7A6A).  A viewer receives only blocks
whose visibility mask contains that player.  Participant records mark newly
visible players and their referenced designs or fleets, and a block's planet
is marked before writing.  A block below the 10-bit length limit remains one
type-`0x1f` record.  Larger blocks retain their 14-byte prefix in the primary
record, use at most 34 participant records there, and continue with at most 35
29-byte participant records per type-`0x27` continuation.

Both implementations have native VC1.52c probes under the Win98 `StarDev`
agent.  The probes exercise append fallback, viewer filtering, chunk sizes,
and the no-op/current-log paths.

The current `WriteBattles` source compiled and linked warning-free with the
VC1.52c toolchain, and its native probe passed all 3 checks
(`runs/vmctl-vc152ew/write-battles-r1/WBRES.TXT`).
