# FTutorTaskDone reconstruction

`reconstructed/src/tutor_task_done.c` is the typed C reconstruction of the
21,667-byte tutor rule dispatcher at NE `0020:0fbc–645f`.  It makes the
original `FCheck*` families explicit through typed check kinds and preserves
the ordered decision chains for tutorial turns 0, 1, and 2, including
selection fallbacks, fleet waypoint checks, queue/research/cargo/colonize
checks, scanner/template checks, builder and zip checks, tutor help IDs,
bold-step changes,
visible/highlight flags, and the advance flag for tutorial turns 0 through 36.

The host and native probes cover representative turn-0/1/2/3/6/8/10/11/18/26/36
branches, including ordered check failures, state flags, and final advancement.
The VC1.52 build and native probe both pass.
