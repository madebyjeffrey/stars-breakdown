# Race validation dependencies

Five routines were added to `reconstructed/src/race.c`, covering 1,184 procedure
bytes. These support FWasRaceFile and CAdvantagePoints; neither enclosing routine
is yet reconstructed. All addresses are NE ordinals with half-open offsets.

| Original | NE 29 range | Host entry |
|---|---|---|
| SetRaceStat | 3114–3175 | stars_set_race_stat |
| GetRaceGrbit | 3176–31B8 | stars_get_race_trait |
| SetRaceGrbit | 31B8–3209 | stars_set_race_trait |
| BoundsCheckPlayer | 40AE–440F | stars_bounds_check_player |
| IRaceChecksum | 5888–58D3 | stars_race_checksum |

The existing GetRaceStat supplies signed-byte reads. SetRaceStat clamps a signed
16-bit input using the original minimum/maximum byte tables at CS:30F4–3114,
then writes one byte. The sixteen minima are
`7,5,5,5,5,2,5,0,0,0,0,0,0,0,0,0`; maxima are
`25,15,25,25,25,15,25,6,2,2,2,2,2,2,9,0`.
The setter does not set a correction flag. Its valid indices are 0–15;
out-of-range table reads are outside the host adapter's contract.

Trait flags form a 32-bit word at player+78. Their mask uses the original CRT
shift helper at NE 36:0D50, which clears CH and shifts one bit CL times.
Thus index 256 selects bit zero; counts whose low byte is 32–255 produce zero.
This must not be replaced by an ordinary unchecked C shift. SetRaceGrbit treats
every nonzero enabled argument as true and preserves other bits.

IRaceChecksum XORs all 96 little-endian words in the complete 192-byte player
record. It is not a sum, CRC, or cryptographic integrity check. In FWasRaceFile,
the loaded player record's checksum is compared with the following record's
stored word before the player is accepted. Stream decoding and password-related
control flow remain unreconstructed dependencies; no changes were made to them.

BoundsCheckPlayer mutates the player record in this order:

1. For each environmental dimension, a low endpoint of signed -1 forces center
   and high endpoint to -1. Otherwise it clamps the low endpoint to 0–100,
   caps the high endpoint at 100, raises it to the low endpoint when necessary,
   and recomputes the center as low + (high−low)/2.
2. It caps signed growth byte player+25 at 20. It does **not** impose a lower
   growth bound; that is handled separately in CAdvantagePoints.
3. It clamps all sixteen race-stat bytes to their table limits.

Every repair sets bit 4 in the word at player+84, preserving the other bits.
Already-correct fields are not rewritten solely to set the flag. All byte
comparisons are signed; treating 255 as a large positive number would change
the immunity sentinel and repair behavior.

## Validation and remaining work

`tests/test_race_machine.py` compares original instructions with compiled C89
source, including whole synthetic memory images for mutating functions. Tests
cover all 65,536 trait-index bit patterns; all byte-valued stat inputs plus signed
extrema; checksum contributions across every player byte; random player records;
environmental endpoint/center combinations; and near-offset wrap. No actual user
race records or private runtime settings are used as test inputs.

The APIs require a complete 64 KiB data image, valid word accesses, and no aliasing
with code or the private stack. Native Win16 selector fault behavior remains
outside the host adapter. The original arithmetic helpers execute in the oracle
but are not counted as reconstructed runtime source.

Next dependencies include stream record decoding, ReadRtPlr, and innate
habitability calculations. CAdvantagePoints mutates records through
BoundsCheckPlayer before computing points; a future reconstruction must preserve
that ordering rather than treating it as a pure scoring function.

## Segment-move display audit (U016)

Inspection of pinned Ghidra 12.1.3 `ia.sinc` lines 4010–4013 shows register-form
opcode 8E has reversed operands in its display constructor, while its p-code
body correctly assigns the general register to the segment register. The bug
therefore does not justify reversing the decompiler's execution semantics.

`python3 -m tools.audit_segment_moves` audits original bytes at every exported
instruction boundary. It examined 5,814 register-form segment moves and retained
2,326 display corrections in `segment-moves.json`, keyed by NE address and bytes.
The overlay records the source export SHA-256; raw exports and their manifests
remain unchanged. For NE 16:00DB, `8e d9` is correctly shown as `MOV DS,CX`.
Use this overlay when reading affected instruction text. Tests verify direction,
operand-size handling, retained audit identity, and the battle-plan example.
The original-machine battle-plan tests independently verify the needed behavior.
