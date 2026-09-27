# Rototill resource and cost services under VC1.52c

The [r4 probe](../../tests/do_rototill_integrated_production_native_probe.c)
replays the retained selector-3 planet-24 queue and Produce boundary. Its
player-1 race stats, race bits, environment, and starting planet fields come
from the original turn-zero HST player/planet records in
[`runs/rototill-original-turn-r1/input/CLEAN.HST`](../../runs/rototill-original-turn-r1/input/CLEAN.HST).
The retained race center `62/33/61` matches planet 24's environment, so the
desirability adapter checks those bytes and returns 100. The native code then
uses production `stars_calc_planet_max_pop` and
`stars_cresources_at_planet` to calculate the 32-resource budget from 225
colonists and 10 factories. It uses production
`stars_get_production_costs` with retained race stat and bit callbacks to
calculate `0/0/3/10` for the factory queue item and again for Produce.
These calculations replace r3's fixed resource and cost callback outputs.

The strict-C89 host [report](../../runs/rototill-production-turn-e1-r4/HOSTRES.TXT)
is PASS. The fresh [Win16 package](../../compat/vc152/rototill-production-turn-e1-r4/)
was transferred to `C:\RTPTR4` using STARSAGT 0.4.2 after `info`, `ping`,
and `windows` confirmed the guest. VC1.52c
[build status](../../runs/rototill-production-turn-e1-r4/BLDSTAT.TXT)
is `COMPILE=PASS`, `LINK=PASS`; the process exited 0 and the returned
[report](../../runs/rototill-production-turn-e1-r4/RTE1RES.TXT)
says `PRODUCTION_PATH=PASS FAILURES=0`. The probe checks queue selection,
initialization, fill, finalization, source resource and cost calls, two
completed factories, one item at 89% progress, final `539/965/498`
minerals, and four research resources. The
[return directory](../../runs/rototill-production-turn-e1-r4/) preserves
all compiler/link logs, objects, executable, map, report, agent log, and
[hash manifest](../../runs/rototill-production-turn-e1-r4/MANIFEST.JSON).
The only compiler warnings are the existing `utilgen.c(422)` integral
conversion warnings, outside this exercised path.

**RT-E1.b is complete** for the integrated native queue/resource boundary.
The native probe begins after the AI transfer and host mining stages and
does not call the entire `DoRototillAiTurn` personality with live global
queue pointers. Parent RT-E1 therefore remains open under its full
[EVIDENCE.md](../../EVIDENCE.md#ai-personalities) rule until the complete
native selector-3 turn and same-input differential in RT-E4.d/e establish
that wider caller and file outcome. No new original `STARS.EXE` run was used.
