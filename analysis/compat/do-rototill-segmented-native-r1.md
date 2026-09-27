# Rototill segmented complete-turn native build diagnostics

RT-E4.d is still open. The complete selector-3 host coordinator matches the
retained original turn, but the Win16 build has not linked or run. This log
records successive compiler packets, each in a fresh guest directory.
Stages 2–8 returned `COMPILE=FAIL`; stage 9 returned `COMPILE=PASS`.
A batch process exit of zero does not override its build status.

| Packet / guest directory | First fatal VC1.52c diagnostic | What it established |
|---|---|---|
| [stage 2](../../runs/rototill-segmented-stage2-r1/COCL.LOG), `C:\RTE4N2` | `C1050`, `turn_validate_player_logs`: code segment too large | Moving `complete_input` and five large trace buffers out of the structure cleared the original oversized-structure error. |
| [stage 3](../../runs/rototill-segmented-stage3-r1/COCL.LOG), `C:\RTE4N3` | `C1126`, `turn_seed_planet_desirability`: automatic allocation exceeds 32K | Function code segments cleared `C1050`; a 64 KB local scratch array became the next boundary. |
| [stage 4](../../runs/rototill-segmented-stage4-r1/COCL.LOG), `C:\RTE4N4` | `C1126`, `turin_binding_original_main`: automatic allocation exceeds 32K | Bounding selector-3 scratch arrays cleared the preceding stack error. |
| [stage 5](../../runs/rototill-segmented-stage5-r1/COCL.LOG), `C:\RTE4N5` | `C1126`, coordinator `main`: automatic allocation exceeds 32K | Removing the unused Turin standalone entry point exposed the coordinator trace on the stack. |
| [stage 6](../../runs/rototill-segmented-stage6-r1/COCL.LOG), `C:\RTE4N6` | `C1027`, `DGROUP` data allocation exceeds 64K | Heap-allocating the coordinator trace cleared the stack limit; retained static fixture data now prevents compilation. |
| [stage 7](../../runs/rototill-segmented-stage7-r1/COCL.LOG), `C:\RTE4N7` | `C1027`, `DGROUP` data allocation exceeds 64K | Changing the harness to `/AL` large model alone did not distribute its static fixture arrays. |
| [stage 8](../../runs/rototill-segmented-stage8-r1/COCL.LOG), `C:\RTE4N8` | `C1015`, too many segments | A distinct named data segment for every retained array exceeded the compiler's segment count. |
| [stage 9](../../runs/rototill-segmented-stage9-r1/COCL.LOG), `C:\RTE4N9` | **`COMPILE=PASS`** | Grouping 204 fixture arrays into eight named data segments cleared the compiler limit. The [object file](../../runs/rototill-segmented-stage9-r1/COORD.OBJ) is retained. |

The source transformation is reproducible through
[the staging script](../../tools/rototill_segmented_stage.py). Each packet's
`MANIFEST.JSON` identifies the exact source and package hashes. The returned
build statuses and logs are under the corresponding `runs/` directory. The
guest [agent log](../../runs/rototill-segmented-stage6-r1/AGENT.LOG) was
retrieved after stage 6; it is a cumulative session log. The earlier stage 2
directory contains an earlier cumulative snapshot.

The transformed stage-2 host `TurinTrace` measured 61,648 bytes, down from
195,488 bytes after the first source cleanup. This is a **host** size, not a
Win16 ABI measurement. The full selector-3 host regression passed after the
buffer split. During the later transformed-host regression, AddressSanitizer
found a pre-existing undersized scratch array in
`rototill_preview_output_rng_tail`: the array was 0x1400 bytes while the
record writer reaches offset 0x4b98. The shared source now allocates 0x4ba8
bytes. The original host regression passed after that correction, and the
regenerated segmented coordinator passed the selector-3 test under
AddressSanitizer. That correction is a test-harness memory fix; it does not
constitute native evidence.

Stage 9 used VC1.52c's `/AL` model for this harness. The fixture's original
64 KB DGROUP image had no nonzero bytes after offset 8817; the staged copy
retains its first 9000 bytes. The full transformed selector-3 host regression
passed under AddressSanitizer (one test, 45.493 seconds), including the
complete original-output comparison. This tests the retained host path, not
Win16 pointer behavior.

The [first link](../../runs/rototill-segmented-stage9-r1/LINK.LOG) was missing
the Win16 stub. After supplying the same `WINSTUB.EXE` used by earlier native
packets, the [second link](../../runs/rototill-segmented-stage9-r1/LINK2.LOG)
still returned **`LINK=FAIL`** with 185 unresolved symbols. The reproducible
[source audit](../../tools/rototill_native_link_audit.py) maps 184 to
[86 candidate reconstructed source modules](do-rototill-native-link-audit-r1.json);
`printf` is the remaining C-runtime symbol. The candidates are a starting
list, not proof that those modules compile or satisfy the full link. The
[stage-9 agent log](../../runs/rototill-segmented-stage9-r1/AGENT.LOG) is
retained.

The next native task is to compile and link the real source modules in the
same memory model, then execute the complete coordinator and return its state
and output files. RT-E4.d cannot close before that run; RT-E4.e still needs
the same-input native/original differential.
