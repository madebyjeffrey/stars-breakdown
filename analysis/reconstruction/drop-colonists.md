# DropColonists reconstruction

`DropColonists` (`0018:34e2-444b`, 3,945 bytes) resolves colonist-drop
records at their destination planets. The typed reconstruction makes the
important policy explicit: destination lookup, survival adjustment, private
drop rejection, over-capacity abandonment, uninhabited-planet claiming,
production initialization, research notification, population/growth update,
arrival messaging, and drop-record clearing.

Survival, race abilities, planet claiming, research, production setup, and
messages are callback services. This keeps the source independent of the
original packed records while retaining the decision order and state changes.
It is ordinary C source with named message and race constants, not assembly or
emitted-byte replay.

The source probe covers survival filtering, abandoned-planet claiming,
production initialization, research notification, population arrival, and
record clearing. It passes strict host C89 checks and the native VC++ 1.52c
Win98 build/link gate. The executable report is `CHECKS=1`, `FAILURES=0`,
`RESULT=PASS` in
`runs/vmctl-vc152ah/drop-colonists/DCRES.TXT`.
