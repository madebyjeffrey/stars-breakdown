# Rototill output RNG tail, r1

The [selector-3 coordinator](../../tests/satisfy_orders_rototill_coordinator_e4_probe.c)
starts with the recovered effective turn-zero `Randomize2` residue 6844.
It consumes the original four starbase-name draws, scout draw, X2/H2 BOF
draws, two-player shuffle, six mining draws, four random-event draws,
postproduction wormhole draws, and two `UpdateGuesses` draws through current
source-backed services. Its retained output fields, including X2/H2, all
four wormholes, the owned planets, and fleet movement, match original.

From the resulting source seed, the preview of the output tail is:

| Site | Source draw |
|---|---:|
| saved turn flag, `Random(8)` | 6 |
| M1 `WriteBof`, `Random(2000)` | 1416 |
| M2 `WriteBof`, `Random(2000)` | 1017 |
| HST `WriteBof`, `Random(2000)` | 1216 |

The retained original turn-1 BOFs have salt residues 285, 1937, and 89
in M1, M2, and HST. Because the source writer uses
`(tick + Random(2000)) & 2047`, those source draws imply tick residues
917, 920, and 921 respectively. These are **inferred tick inputs**, not
independent measurements of the guest clock. With those explicit inputs,
the coordinator calls current-source `stars_write_bof` three times. All
three complete 18-byte BOF records match the retained original, including
file type, player, turn, saved-flag bits, salt, and record header. The source
writer's final seed equals the preview seed.

The focused check is in
[`test_satisfy_orders_turin_binding.py`](../../tests/test_satisfy_orders_turin_binding.py)
and passed on 2026-09-26:

```text
python3 -m unittest tests.test_satisfy_orders_turin_binding -q
Ran 1 test in 34.520s
OK
```

This closes the **output-tail subunit** in the small burndown. It does not
close RT-E2: the entire later main RNG path still needs an original-source
conditional-call audit, and the inferred ticks cannot by themselves prove
that no earlier draw was omitted. Remaining route branches and native/original
turn comparison are separate RT-E2 requirements. No new original Stars!
run was made.
