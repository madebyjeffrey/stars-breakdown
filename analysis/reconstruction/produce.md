# Produce reconstruction

`Produce` (`0018:0000-0c92`, 3,218 bytes) is the production phase. The
reconstruction uses typed planet and production-item records and makes the
shared services explicit: mineral mining, resource calculation and planet
bonuses, dead-owner adjustment, capacity checks, terraform restrictions,
production-item building, object creation, queue removal, population and
research updates, messages, and random events.

Production formulas and object-specific effects remain callbacks into the
planet, production, and research subsystems. The queue loop keeps unfinished
items in order while allowing an item to consume multiple build units when
resources permit. The source is readable C with named policy constants, not
assembly or emitted-byte replay.

The source probe covers mining, two production units, queue removal,
population update, research update, and random-event dispatch. It passes
strict host C89 checks and the native VC++ 1.52c Win98 build/link gate. The
executable report is `CHECKS=1`, `FAILURES=0`, `RESULT=PASS` in
`runs/vmctl-vc152af/produce/PRRES.TXT`.
