# First function-byte matching experiment

2026-09-14: VC++ 1.52c reproduced the instruction bytes of all three selected
functions from the supplied, hash-verified executable. This establishes a
working code-generation match for these functions, not a matching whole EXE.

| Function | Original bytes | Result | Address handling |
| --- | ---: | --- | --- |
| GetRaceStat | 34 | Exact | No address adjustment required |
| SetRaceStat | 97 | Exact after placement | Four 16-bit references to the two code-segment tables |
| Random | 439 | Exact after placement | Eight 16-bit global references and nine 16:16 runtime calls |

All **570 function bytes** match after resolving those **60 address bytes**;
the remaining **510 bytes** match without address adjustment. The intervening
32 bytes of stat limits also match exactly. There is no instruction masking,
stack-offset normalization, inline assembly or replacement of generated code.
Raw object addresses differ because this tiny module has a different link layout.
For the original reference, real NE relocations are loaded into the same synthetic
selector space used by the disassembly exports. For the candidate, named OMF
fixups resolve globals, table placement and runtime callees into that layout.

## Reproducible source and flags

[PROBE.C](../../compat/bytematch/PROBE.C) uses ordinary 16-bit C, a near PLAYER
pointer, signed-byte statistics at offset 62, code-segment limit tables, and
original-shaped long arithmetic. Its small PLAYER prefix is sufficient for
these accesses; it is not the complete original type declaration.

[BUILD.BAT](../../compat/bytematch/BUILD.BAT) uses:

```
/c /AM /GA /GEs /Zp1 /Od /Gs /W3 /Fc
```

The successful compile emits C4135 at the final Random return: a 32-bit unsigned
remainder becomes a 16-bit int. This expected narrowing matches the original AX
return. It is retained in the captured log, not ignored as an unknown diagnostic.

The first compile without `/Gs` inserted stack-check calls. `/Gs` produced the
original SUB SP prologues. This demonstrates a compatible setting; an equivalent
original source pragma or other build context has not been excluded.

GetRaceStat and SetRaceStat then matched. Random initially had the correct
439-byte body and operation ordering but different local stack offsets. A finite
24-order declaration experiment found one match in that source context:

```
long s2;
long s1;
long k;
long z;
```

The resulting slots match the debug evidence: z at BP-6, s1 at BP-10, k at BP-14,
s2 at BP-18, with a 22-byte local frame including temporary/reserved storage.
Separate `/Zi` and `/Z7` controls on the initial source did not correct its offsets.
A fresh build of the matching candidate under the filename PROBE.C repeated all
three matches; this is not merely a match inferred from the assembly listing.

The candidate names its two external globals lSeed1/lSeed2 and maps them explicitly
to original lRandSeed1/lRandSeed2 at DGROUP:1370/1374. Renaming them to the original
spellings in a control changed local allocation despite unchanged arithmetic and
function length. The internal compiler mechanism is not isolated. Consequently,
this is not a claim to have recovered the original source text or original debug
records. A full-module reconstruction may change the required source context.

## Comparison and independent controls

[compare_byte_match.py](../../tools/compare_byte_match.py) reads actual object
LEDATA, FIXUPP and compiler backpatches. It verifies the original binary hash,
reads the original NE bytes, and compares every resolved byte. Public entries are
cross-checked with listing extents. The table contents are checked separately.

The compiler's object format exposed a target-thread decoding bug in our existing
OMF parser: target-thread method selection uses two bits, unlike frame threads.
That was corrected according to the [TIS OMF 1.1 specification](https://openwatcom.org/ftp/devel/docs/omf.pdf),
which also defines additive BAKPAT records. The probe adapter accepts only the
specific bounded record forms needed here; it is not a general 32-bit OMF linker.

All 13 tests in `tests.test_byte_match` and `tests.test_runtime_library` pass.
The negative controls change a getter opcode and a seed relocation identity;
both are detected as mismatches. Truncated/corrupt objects are rejected. Existing
runtime-library audit tests also pass after the thread fix. The fixture retains
our own compiler-generated object and listing; no compiler binary is committed.

Full results, hashes, fixup sites, flags, diagnostics and all 24 trial outcomes:
[byte-match.json](byte-match.json). Local returned object/listing/logs:
`runs/byte-match/final2`. A compact regression fixture is retained in
[byte-match-probe.json](../../tests/fixtures/byte-match-probe.json).

## Meaning for the project

The compiler can reproduce the original code generation for these three functions,
including the RNG's long-arithmetic helper calls and unusual unoptimized branches.
That is stronger evidence than behavioral agreement, and a practical basis for
incrementally pursuing more matches.

The 130-function semantic reconstruction remains unchanged. Its other 127 functions
have not acquired byte-match credit from this experiment. Full module boundaries,
original type/source context, link ordering, DGROUP layout, debug information,
resources and complete executable bytes remain separate work. No new Step 5
whole-function credit or whole-executable match is claimed.
