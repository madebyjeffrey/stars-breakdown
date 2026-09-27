# Rototill retained queue and Produce boundary under VC1.52c

Status: **native boundary passes; RT-E1.b and parent RT-E1 remain open**.

The [probe](../../tests/do_rototill_integrated_production_native_probe.c)
starts from the retained selector-3 planet-24 state after the 25-colonist
transfer: population 225, mineral stocks `538/964/506`, and the AI-phase 0%
research setting. Reconstructed `stars_fill_production_queue` calls the
selection, initialization, fill, and finish adapters. The fill adapter uses
reconstructed `stars_get_resources_available` and
`stars_fill_prod_mines_and_factories`; it verifies a 32-resource budget and
one addition of three factories. After the original host-phase research
reload, reconstructed `stars_produce` and `stars_build_production_item` use
15% research and the post-mining `539/965/506` stocks. They leave two new
factories, one at 89% progress, final stocks `539/965/498`, and four
research resources, matching the retained original turn.

The strict-C89 host compilation and [execution](../../runs/rototill-production-turn-e1-r3/HOSTRES.TXT)
pass. The fresh
[VC1.52c r3 package](../../compat/vc152/rototill-production-turn-e1-r3/)
was transferred to `C:\RTPTR3` through STARSAGT 0.4.2. Its
[build status](../../runs/rototill-production-turn-e1-r3/BLDSTAT.TXT) says
`COMPILE=PASS` and `LINK=PASS`; the Win16 process exited 0, and its
[report](../../runs/rototill-production-turn-e1-r3/RTE1RES.TXT) says
`PRODUCTION_PATH=PASS FAILURES=0`. The
[return](../../runs/rototill-production-turn-e1-r3/) preserves every
compiler log, link log, object, map, executable, agent log, and the
[hash manifest](../../runs/rototill-production-turn-e1-r3/MANIFEST.JSON).
Earlier r1 passed a narrower filler/Produce boundary. The r2 expansion
compiled but failed to link because `ai.c` needed
`stars_distance_squared` and `stars_hull_has_teeth`; its failed
[link log](../../runs/rototill-production-turn-e1-r2/LINK.LOG) is retained.
Fresh r3 adds the corresponding source utility and battle modules.

This is a bounded native checkpoint. The resource and item-cost callbacks
return the values proven by the integrated host turn, and the native probe
begins after the AI transfer and HST mining stages. It does not yet call
`DoRototillAiTurn` with the live global queue and service pointers, nor
derive those two values through the full source resource/cost services in
Win16. RT-E1.b requires that final native binding before closure. The full
selector-3 native coordinator and native/original file differential remain
RT-E4.d/e.
