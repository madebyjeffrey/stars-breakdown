# FAttack raw torpedo shield/hull allocation — E21

E21 implements the raw allocation branch traced in E20. A typed allocation
result now holds the two packet terms, allocated count, and resulting damage.
The source preserves the raw order: use total shield and hull capacities
(including the `_dv` reduction), apply the item 8–11 power doubling only
when shield capacity is zero, test `hits * power` against hull capacity, and
when required scan allocations from ship count to packet budget. Each scan
uses the raw ceiling division, shield `/8` term and zero floor, hull `/2`
term, and threshold comparison. The resulting terms supply the separate
shield-only and hull `FDamageTok` calls.

The actual packed `_hs` group/item/count is restored onto the resolver's local
section after lookup. This keeps item-dependent flags and power behavior tied
to the selected slot even if the lookup callback only returns semantic part
data. The raw decompiler calls the word at offset 54 `wtFuelMax` because it
inferred the overlapping `_hul` view; the recovered 60-byte `_torp` record
calls that same offset `dp` (`STARS_OFFSET_12aa_dp=54`). The resolver still
must supply this raw group-0x20 record word; E21's callback supplies only a
fixture value.

## Host evidence

`F_ATTACK_RAW_TORPEDO_DISPATCH_CHECKS=9 FAILURES=0 RESULT=PASS` under strict
C89 covers: item 8 and item 7 damage classes via packed lookup; a two-of-three
hit fallback split; the capacity-scan branch allocating one packet; special
item power doubling with no shields; distinct shield/hull callback arguments;
packet-count mutation; and hit-record semantics. Focused regressions pass:
FAttack source 34, target/RNG 5, and packed raw damage 7.

The full suite after E21 source changes passes: 519 tests, 153 expected skips,
2,241 generated layout assertions, and 846 function declarations.

## Native status and remaining scope

The fresh packet at `compat/vc152/f-attack-complex-native-e21/` contains 43
files: 42 ASCII/CRLF text files and the DOS stub. Packaged sources pass
strict-C89 syntax checks against the packet header. It is prepared for
`C:\FAPNE21`; guest compilation/execution evidence is not recorded.

The same-target/retarget residual loop controlled by `lVar19`/`lVar16` is
still absent, and the actual packed-part resolver has not yet been shown to
populate `raw_torpedo_power` from the group-0x20 record word at offset 54.
Beam damage-class lookup,
broader original differentials, full native battle replay, and Step-6
promotion remain open. This checkpoint closes zero named Step-5 units; TODO6
remains 120/152 closed, 32 open, 10 promotion units, and 42 practical open
work items.
