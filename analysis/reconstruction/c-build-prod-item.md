# CBuildProdItem reconstruction

`CBuildProdItem` (`0018:0c92-19b1`, 3,359 bytes) computes and applies one
production queue item's build work. The typed reconstruction resolves
production costs, capacity and terraform restrictions, resource-limited
build counts, alchemy fallback, object creation, partial-production output,
resource consumption, status reporting, and the optional build message.

Cost calculation, capacity rules, population/terraform checks, object
creation, race abilities, and messaging are explicit callbacks into the
surrounding subsystems. The implementation uses named status constants and
ordinary C source; it does not replay assembly or emitted bytes.

The source probe covers multi-unit production, resource deduction, object
creation, status output, and notification. It passes strict host C89 checks
and the native VC++ 1.52c Win98 build/link gate. The executable report is
`CHECKS=1`, `FAILURES=0`, `RESULT=PASS` in
`runs/vmctl-vc152ag/c-build-prod-item/CBRES.TXT`.

## Finite completion status correction (2026-09-25)

Original turn2.c:740-759 reports status 0 after a finite production item
completes and its remaining quantity becomes zero. Status 2 belongs to the
separate auto-build capacity case. Corrected the source constant and old probe
expectations. The corrected probe and SO-E3 ETA integration pass on the host;
the historical native report above does not validate this corrected status.
No new native run was performed.
