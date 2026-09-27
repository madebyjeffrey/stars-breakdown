#include "core.h"

static StarsI16 first_engine(const StarsIdealWarpDesign *design,
    StarsU16 *engine_id)
{
    StarsU16 i;
    for (i = 0; i < design->engine_count && i < 10U; ++i) {
        if (design->engine_flags[i] == 1U) {
            *engine_id = design->engine_ids[i];
            return 1;
        }
    }
    return 0;
}

/* Genuine C reconstruction of IFindIdealWarp, NE 0B:A76E-A9F3. */
StarsI16 stars_find_ideal_warp(const StarsFindIdealWarpInput *input,
    StarsI16 ignore_scoops)
{
    StarsI16 ideal;
    StarsI16 warp;
    StarsI16 i;
    StarsI16 id;
    StarsU16 engine_id;
    const StarsIdealWarpEngine *engine;

    if (!input)
        return 10;
    ideal = 10;
    for (i = 0; i < 16; ++i) {
        if (input->ship_counts[i] > 0) {
            if (!first_engine(&input->designs[i], &engine_id))
                return 0;
            engine = input->lookup_engine ?
                input->lookup_engine(input->context, engine_id) : 0;
            if (!engine)
                return 0;
            id = (StarsI16)engine_id;
            for (warp = ideal; warp > 0; --warp) {
                if (engine->fuel_used[warp] < 0x79) {
                    if (engine->fuel_used[warp] > 0 && ignore_scoops == 0 &&
                        id != 0x0e && id != 0x0f) {
                        if (warp < 5 || engine->fuel_used[warp - 1] != 0) {
                            if (warp < 6 || engine->fuel_used[warp - 2] != 0) {
                                if (warp > 6 &&
                                    engine->fuel_used[warp - 3] == 0)
                                    warp = (StarsI16)(warp - 3);
                            }
                            else {
                                warp = (StarsI16)(warp - 2);
                            }
                        }
                        else {
                            --warp;
                        }
                    }
                    if (warp == 10 && id != 7 && id != 8 && id != 9 &&
                        id != 0x0e && id != 0x0f)
                        warp = 9;
                    ideal = warp;
                    break;
                }
            }
            if (warp == 0)
                ideal = 0;
        }
    }
    return ideal;
}
