# Player record serialization and legacy names

Five reviewed functions add 2,180 original procedure bytes. These are host C89
semantic adapters, with explicit stream-transfer boundaries and documented input
limits. They do not implement the file dialog, password prompt or file-error UI.

| Original | NE range | Source / host entry |
|---|---|---|
| ReadRt | 15:5168–524D | records.c / stars_read_record |
| WriteRt | 15:947C–9553 | records.c / stars_write_record |
| ReadRtPlr | 15:05E2–080F | records.c / stars_read_player_record |
| WriteRtPlr | 15:551C–574D | records.c / stars_write_player_record |
| PszPlayerName | 8:11F2–145C | strings.c / stars_player_name |

Addresses are NE ordinals and half-open offsets. Signatures and local-variable
records are in `analysis/debug/procedures.json` and `analysis/functions.json`.
All five use original far C calls with caller cleanup. ReadRtPlr and WriteRtPlr's
input and output pointers are resolved through the supplied data image; WriteRt
accepts a host pointer in place of its original far source pointer.

## Record framing and stream state

A two-byte little-endian header encodes length in bits 0–9 and record type in
bits 10–15. The supported body length is 0–1023 bytes. The shared current body is
DGROUP:4B98; the read header is at 2696. Writing constructs a **local** header and
does not change the current read header.

ReadRt transfers the header, transfers the body only when length is nonzero,
then processes the record type. WriteRt first memmoves the complete supplied
body into shared scratch, processes the exact signed type argument, then emits
a header and body in separate calls. Its header masks type to six bits; the
earlier type comparisons use the original full word. For example, type 64 is
encrypted even though its emitted header identifies type zero.

Type 8 is a beginning-of-file record (BOF). Its body initializes the existing
file XOR generator from the game id at +4, turn at +10, salt/player word at +12,
and the bit at +14 corresponding to bit 12. Salt uses arithmetic right shift
by five; the player field sign-extends the low five bits. These are the original
signed operations, not ordinary unsigned extraction. Type zero is left plain
and does not advance the XOR stream. Other types pass the body through the
reconstructed XorFileBuf. Zero-length BOFs still initialize seeds from whatever
body bytes remain in scratch; no special clearing or minimum-length check is
invented. ReadRt does not itself update the file-version global.

Host transfer callbacks represent the RgFromStream/RgToStream boundaries. They
must transfer the requested count, or abort nonlocally on failure. They are not
Windows APIs and must not silently return a short transfer. WriteRt invokes its
body callback even for zero length, matching its call to RgToStream; that
original callee skips the zero-length Windows API operation.

## Player payload layout

WriteRtPlr first tests bit zero at player+84. If set, it forces the low three
bits of the word at player+6 to seven, mutating the supplied player. It then
copies all 192 player bytes into either supplied storage or a local 266-byte
buffer. A supplied buffer must accommodate that initial copy even when the
serialized payload is short.

If the low three bits at player+6 are seven, the body consists of:

1. The first 112 bytes of the player.
2. One relationship-count byte.
3. That many relationship bytes from player+112, omitting trailing zero entries
   among the sixteen original relationships.
4. Encoded singular and plural names.

Otherwise only the first eight player bytes precede the two names. The rest of
the copied temporary is not automatically emitted. Each name begins with one
byte: a nonzero value is the compressed byte count; zero means an ordinary
NUL-terminated string follows. Compression gets capacity 31. An empty name or
compression failure uses the raw representation, including its terminator.
WriteRtPlr finally calls WriteRt with type 6 and the constructed byte count.

ReadRtPlr clears all 192 destination bytes first. It copies either the 112-byte
prefix and relationship list or the eight-byte prefix, then decodes the singular
name. For file minor versions below 55, it generates the plural name through
PszPlayerName; newer formats read a second encoded name. The version is extracted
from DGROUP:0750, bits 5–11. At exit it clears bit 3 at player+84. It does not call
BoundsCheckPlayer, validate the record header or check the race checksum.

## Name formatting

PszPlayerName selects global player zero plus index*192 when its supplied player
pointer is zero. It assembles text in a 50-byte local and copies it to shared
szWork at DGROUP:57A4. A blank singular name uses embedded `Player %d` with the
16-bit wrapped value index+1, appending the original possessive suffix when not
plural. It uses singular grammar strings (` has`, ` is`) for that placeholder.

For a supplied name it optionally prefixes `the `, capitalizing only that prefix
when requested, chooses the explicit plural when available, and strips trailing
ASCII spaces. All-space text becomes `No Name`. Automatic pluralization appends
`s` unless the trimmed name ends in lowercase `s` or `se`. Named text uses the
other grammar strings (` have`, ` are`). These rules are deliberately literal;
modern grammar or Unicode libraries would change observed behavior.

## Original edge cases and limits

* **Ignored codec failure:** ReadRtPlr passes decoded capacity 32, ignores a false
  return, advances past the indicated compressed bytes, and proceeds with the
  next name. A failed decode leaves the relevant cleared field unchanged.
* **Terminator outside the field:** capacity counts decoded bytes, not the NUL.
  Exactly 32 decoded singular characters put their terminator in the next field;
  32 decoded plural characters put it at player+192. Tests verify that extra
  write in a complete synthetic memory image. This extends U015's caller audit.
* **Saved-register-dependent plural:** for a trimmed single lowercase `e`, no
  explicit plural and no article, the original checks the byte before its local
  buffer. That byte is the high byte of saved SI. Original runs with SI=1234
  produce `es`; SI=7300 produces `e`. The portable adapter excludes this case
  rather than claiming its guarded C expression is universally equivalent.
  U019 tracks how to represent the calling-context dependency for integration.
* **Checksum canonicalization:** bytes after a name's NUL are not serialized,
  and the reader clears the destination. Setting a single unused name-tail
  byte to 55h in a synthetic source record changes the original whole-player
  checksum after roundtrip by 55h. Clearing flag bit 3 can also change it.
  FSaveRace computes its checksum from the source, while FWasRaceFile compares
  against the decoded player. Higher-level initialization/canonicalization
  therefore needs auditing (U020); these transforms do not silently repair it.

Ordinary source names must terminate within their 32-byte fields. Raw reader
strings must fit their destination fields; relationship counts must be at most
16. For the explicitly tested 32-decoded-character case, the surrounding data
image must also make player+192 writable. All buffers are contiguous, remain
inside the supplied 64 KiB image, and avoid code, original private stack, codec
lookup tables, shared scratch, and incompatible overlapping records. The reader
clears its destination before reading input, so source/destination overlap is
outside its supported domain. Writer storage is either zero or disjoint from
the player and shared record scratch. PszPlayerName's assembled text must fit
its 50-byte local and use the verified original string/format tables.

## Differential validation boundary

`tests/test_record_machine.py` compares application data below E000, excluding
only the original private stack. It covers all 64 record types plus signed/type-
mask edges, lengths including 0 and 1023, odd bodies, overlapping WriteRt sources,
signed BOF fields, storage and relationship variants, compressed/raw/empty names,
legacy versions, all name-formatting branches, and a BOF/player/checksum sequence
with XOR state carried across records. The name-capacity and saved-SI edge tests
are separate and retain the observed original effects.

`tools/record_machine.py` loads original NE segments 15 and 8 in addition to the
existing core. All game and CRT callees execute unchanged. Three Windows import
boundaries are explicitly simulated: successful _lread, successful _lwrite and
wsprintf for the verified `Player %d` format. The stream is synthetic memory;
no live user files are opened. Other imports remain unmapped. This is **not**
validation of short-I/O failures, dialogs, Windows handles or protected-mode
selector behavior. Existing core tests still use a base harness with no import
substitutions. RgFromStream, RgToStream, FWasRaceFile, FSaveRace and WriteBOF are
not counted as reconstructed functions in this checkpoint.

## Runnable record example

```sh
python3 tools/race_demo.py --seed 0 --record-roundtrip
```

This generates a synthetic race using reconstructed code, encodes a controlled
BOF/player/checksum sequence, reads it back, and reports byte equality and the
checksum result. It runs without Windows. BOF game id, turn and salt are fixed
fixture inputs; WriteBOF's clock path has not been reconstructed. Seed 0 produces
a 144-byte stream whose restored player bytes and checksum match. Other inputs
report differences instead of normalizing them away. The tool writes no files.
