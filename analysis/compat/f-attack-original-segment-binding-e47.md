# FAttack / FLookupPart original-segment record binding E47

Date: 2026-09-22

`stars_lookup_part_record` maps `FLookupPart`'s validated current-CS record
address into a caller-provided segment image. It rejects a non-current-CS
selector, unsupported family, out-of-range item, inconsistent family
base/stride/address, and any span that does not contain the entire record.
Returned pointers are borrowed from the caller's image; the helper does not
pretend Ghidra's synthetic selector is a runtime selector.

The strict-C89 host test extracts the **unrelocated** NE segment-2 bytes from
the original `target/stars.exe` through `tools.ne_layout.NE.raw[1]`. The input
binary SHA-256 is
`ccab6682b55dae14dbf104e491cfbd3b32a6280022020032ae0615500949ba59` and the
segment is 25,256 bytes. The 129 checks cover:

- one original record address in each of the sixteen part-table families;
- all 24 beam records and 12 torpedo records at field offsets 52, 54, and 56
  (`dRangeMax`, `dp`, and `init`);
- a truncated record span and inconsistent selector/base/offset/stride
  metadata.

```text
LOOKUP_PART_MEMORY_CHECKS=129 FAILURES=0 RESULT=PASS
```

The packed-FAttack host regression also passes with the corrected E46
current-CS model. `PYTHONMALLOC=malloc python3 -m unittest discover -s tests
-q` passes 524 tests with 153 expected skips, 2,241 layout assertions, and 846
declarations; baseline and diff checks pass. Fresh VC++ 1.52c packet E50 also
compiles/links and passes the same 129
checks in Win98. The returned `SEG2.BIN` matches the package byte-for-byte.
E49's failed linker attempt is preserved; it omitted the explicit map-file
argument in the response file, which E50 corrects. Native artifacts are in
`compat/vc152/f-lookup-part-memory-native-e50/` and
`runs/vmctl-faptae50/`; failed E49 artifacts are in `runs/vmctl-faptae49/`.

This checkpoint closes **0 named Step-5 units**. It maps addresses to the
original static executable bytes on both host and native target, but is not a
live Win98 Stars! process CS observation. Next make packed FAttack consume the
resolved raw weapon record and validate the complete callback.
Complete race/hidden/technology services, attack acceptance, differential,
full battle replay, and promotion remain open.

## Sources

- `reconstructed/src/lookup_part.c`
- `reconstructed/include/common.h`
- `tests/lookup_part_memory_probe.c`
- `tests/test_lookup_part_memory.py`
- `tools/package_lookup_part_memory.py`
- `analysis/decompiler/export/raw/1008_524e.asm`
- Original input identity: `analysis/input_inventory.json`
- Static NE segment generation and limitations: `tools/ne_layout.py`,
  `analysis/ne/README.md`
