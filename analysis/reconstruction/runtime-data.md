# Remaining CRT data audit

The installed VC++ 1.52c MLIBCEW.LIB maps six of the seven original CRT records
without code completely for their data/absolute-symbol role. The seventh is
identified, with one historical Windows linker transformation still unresolved.
This adds data evidence to the existing 101-record / 15,192-byte code match; it
does not count library bytes as newly reconstructed game procedures.

| Member | Evidence and result |
| --- | --- |
| malloc.asm | Selected member contributes no loadable bytes. |
| huge.asm | The nine-byte double variant matches __HUGE and __matherr_flag at DGROUP 7200/7208. The eleven-byte alternative does not match the public anchors. |
| wnull.asm | Five absolute local-heap/stack equates match the original symbols; no segment bytes. |
| cmiscdat.asm | 34 bytes at DGROUP 6294, including seven verified code pointers and signal/size globals. |
| seekfast.asm | Two bytes at DGROUP 6292, anchored by __lseekchk. |
| ctype.asm | All 257 initialized classification bytes match at DGROUP 5982. |
| fixups.asm | The 14-byte CDATA record is located at DGROUP 7300. WIN87EM ordinal 1 and __fpsignal pointers match. The __fptaskdata slot is zero, while its absolute symbol is 9876h; the linker transformation is not yet reproduced. |

Across all 108 CRT records, the audit examines 44 nonempty data segments,
2,199 initialized literal bytes and 99 fixups. Ninety-eight fixups are verified;
one is retained as an observed Windows stub slot. Original public symbols and
previously verified code references supply anchors. Private data placement is
constrained to NE data segments and matching literals/pointers; ambiguity is
preserved, with a bounded set of candidate examples in the report.

Four module rows retain unresolved details: fixups.asm as above, and common PAD /
EPAD markers contributed by nmsghdr.asm, crt0fp.asm and crt0msg.asm. Their message
bodies and other anchored data match; repeated FF markers alone do not establish
which occurrence belongs to each common segment. The installed STARTUP/DEFSEGS.INC
confirms common PAD/EPAD declarations, and DOS/NMSGHDR.ASM confirms their marker
values. This explains their role without assigning unproven byte addresses.

The installed STARTUP/WIN/CRT0.ASM explicitly declares __fptaskdata = 9876h to
avoid pulling in the non-Windows emulator. DOS/CRT0DAT.ASM reserves the common
floating-point setup area. Those primary sources explain why this is special,
but do not establish the exact original LINK transformation. Source material is
kept in the local compiler installation; no vendor source is copied into the
reconstruction or transfer package.

`python3 -m tools.review_runtime_data --check` reproduces the retained JSON using
the exact installed-library and original-executable hashes. Negative tests mutate
an initialized ctype byte and a data-pointer displacement; both invalidate their
mapping. See [machine-readable results](runtime-data.json) and
[tests](../../tests/test_runtime_data.py). Full CRT-data/link-order completion
remains open under U002.
