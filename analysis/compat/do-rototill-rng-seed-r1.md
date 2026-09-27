# Rototill original main RNG seed, retained turn r1

The original selector-3 M2 turn-1 output contains four newly named owner-1
starbase designs in slots 17–20. Their names are `Evil Spawn`, `Evil Spawn`,
`Silver Tower`, and `Potato`. The original compressed-string pool maps those
to IDs `0x43e`, `0x43e`, `0x434`, and `0x438`. Source
`DoRototillAiTurn` calls `IroEnsureAi` before `IdTargetScout`;
`IroEnsureAi` calls `EnsureAiStarbaseDesigns`, whose creation order is
slots 18, 20, 17, 19. Thus the four accepted `Random(13)` values are
**12, 6, 12, 2**. The same-hull pairs have different names, so no
duplicate-name retry occurs.

[`tests/test_rototill_rng_seed.py`](../../tests/test_rototill_rng_seed.py)
decodes those names from the retained original file and searches all 16,384
effective low-14-bit `Randomize2` tick residues through current-source
`stars_randomize2`/`stars_random`. Exactly one residue matches: **6844**,
with initial seed words **(31, 587)**. The next source draw is
`Random(100)=31`. The four design-name calls precede the retained scout
route, so this is the scout wormhole-chance draw. It is above the 5% branch
threshold and does not enter the wormhole search callback.

The [selector-3 coordinator](do-rototill-coordinator-e4-r1.md) now seeds
the current RNG with residue 6844 and runs source `stars_iro_ensure_ai`
with Rototill's zero-length requirement list. Its starbase-design service
creates all four names in original order and consumes the four `Random(13)`
draws. The subsequent live scout callback observes `Random(100)=31`, no
wormhole callback, and the original planet-26, warp-5 packed route.
The player design count reaches five, matching the original turn-1 HST/M2.
The shared `stars_iro_ensure_ai` minimum-tech search was corrected to start
at field zero, as the original does; the [source probe](../../tests/iro_ensure_ai_source_probe.c)
passes 3/3 including the zero-requirement branch.
The effective low bits do not establish the complete original 32-bit
`GetTickCount()` value, but `Randomize2` uses only these effective bits for
the initial seed words in this case. Later turn RNG calls and complete
serialized output remain to compare. No original executable run was made.
