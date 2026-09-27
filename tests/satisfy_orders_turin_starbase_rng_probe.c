#include "core.h"
#include "TURINSTARBASEFX.H"
#include <stdio.h>
#include <string.h>

typedef struct StarbaseRun {
    StarsSeeds seeds;
    StarsAiStarbaseDesign designs[10];
    StarsU8 capabilities[3];
    StarsIroAiState research;
    StarsU8 bitmap_used[4];
    StarsPoint planet_points[32];
    StarsU8 ai_planet_records[32 * 16];
    StarsI16 requested_designs[4];
    StarsI16 requested_kinds[4];
    StarsI16 saved_designs[4];
    StarsI16 saved_bitmaps[4];
    StarsI16 random_limits[8];
    StarsI16 random_values[8];
    char saved_names[4][32];
    StarsI16 create_calls;
    StarsI16 save_calls;
    StarsI16 random_calls;
    StarsI16 wormhole_calls;
    StarsI16 ensure_ai_calls;
    StarsI16 history_calls;
    StarsI16 research_writes;
    StarsI16 research_order;
    StarsI16 latest_starbase;
    StarsU8 written_current_tech;
    StarsI16 written_percent;
    StarsI16 failures;
} StarbaseRun;

static StarsI16 seeded_random(void *context, StarsI16 limit)
{
    StarbaseRun *run = (StarbaseRun *)context;
    StarsI16 index = run->random_calls;
    StarsI16 value = stars_random(&run->seeds, limit);
    if (index >= 8) {
        ++run->failures;
        return 0;
    }
    run->random_limits[index] = limit;
    run->random_values[index] = value;
    ++run->random_calls;
    return value;
}

static StarsI16 get_name(void *context, StarsI16 string_id, char *out,
    StarsU16 capacity)
{
    (void)context;
    if (capacity < 8U)
        return 0;
    sprintf(out, "ID%04X", (unsigned)string_id);
    return 1;
}

static StarsI16 bitmap_in_use(void *context, StarsU16 bitmap, StarsU16 hull)
{
    StarbaseRun *run = (StarbaseRun *)context;
    StarsU16 index;
    if (hull != 12U || bitmap < 0x20U || bitmap >= 0x24U)
        return 0;
    index = (StarsU16)(bitmap - 0x20U);
    return run->bitmap_used[index] != 0U;
}

static StarsI16 name_in_use(void *context, StarsU16 design_class,
    const char *name)
{
    (void)context;
    (void)design_class;
    (void)name;
    return 0;
}

static StarsI16 create_shdef(void *context, StarsI16 ishdef,
    StarsI16 starbase_index, StarsI16 family, StarsAiStarbaseWork *work)
{
    (void)context;
    if (ishdef != -1 || starbase_index < 0 || family < 0)
        return 0;
    work->hull_id = 12U;
    work->flags = 0x0200U;
    work->bitmap_id = 0x20;
    work->name[0] = '\0';
    work->component_count = 0;
    return 1;
}

static void change_shdef(void *context, StarsI16 design,
    const StarsAiStarbaseWork *work)
{
    StarbaseRun *run = (StarbaseRun *)context;
    StarsU16 bitmap;
    if (run->save_calls >= 4) {
        ++run->failures;
        return;
    }
    run->saved_designs[run->save_calls] = design;
    run->saved_bitmaps[run->save_calls] = work->bitmap_id;
    memcpy(run->saved_names[run->save_calls], work->name, 32U);
    bitmap = (StarsU16)work->bitmap_id;
    if (bitmap >= 0x20U && bitmap < 0x24U)
        run->bitmap_used[bitmap - 0x20U] = 1U;
    ++run->save_calls;
}

static void create_starbase(void *context, StarsI16 design, StarsI16 kind)
{
    StarbaseRun *run = (StarbaseRun *)context;
    StarsAiStarbaseWork work;
    StarsPickAiNameOps name_ops;
    StarsI16 index = run->create_calls;
    if (index >= 4) {
        ++run->failures;
        return;
    }
    run->requested_designs[index] = design;
    run->requested_kinds[index] = kind;
    ++run->create_calls;
    name_ops.random_value = seeded_random;
    name_ops.get_name = get_name;
    name_ops.bitmap_in_use = bitmap_in_use;
    name_ops.name_in_use = name_in_use;
    name_ops.format_name = 0;
    if (!stars_f_create_ai_starbase(design, kind, -1, -1, 1, &work,
            create_shdef, change_shdef, &name_ops, run))
        ++run->failures;
}

static void ensure_starbase_designs(void *context)
{
    StarbaseRun *run = (StarbaseRun *)context;
    memcpy(run->capabilities, retained_starbase_capabilities,
        sizeof(run->capabilities));
    stars_ensure_ai_starbase_designs(0, run->capabilities, run->designs,
        create_starbase, run);
}

static StarsI16 latest_starbase(void *context)
{
    (void)context;
    return RETAINED_LATEST_STARBASE_DESIGN;
}

static void validate_starbase_history(void *context)
{
    StarbaseRun *run = (StarbaseRun *)context;
    ++run->history_calls;
}

static void write_research(void *context, StarsU8 current, StarsI16 percent)
{
    StarbaseRun *run = (StarbaseRun *)context;
    ++run->research_writes;
    run->written_current_tech = current;
    run->written_percent = percent;
}

static StarsI16 ensure_ai(void *context, StarsI16 percent,
    StarsI16 *latest)
{
    StarbaseRun *run = (StarbaseRun *)context;
    StarsIroAiOps iro_ops;
    StarsI16 index;
    ++run->ensure_ai_calls;
    memset(&run->research, 0, sizeof(run->research));
    for (index = 0; index < 6; ++index)
        run->research.tech[index] = retained_player_technology[index];
    run->research.current_tech = RETAINED_PLAYER_CURRENT_TECH;
    run->research.percent_research = RETAINED_PLAYER_PERCENT_RESEARCH;
    iro_ops.ensure_designs = ensure_starbase_designs;
    iro_ops.latest_starbase = latest_starbase;
    iro_ops.validate_history = validate_starbase_history;
    iro_ops.write_research = write_research;
    run->research_order = stars_iro_ensure_ai(
        (StarsU8 *)retained_turin_research_requirements,
        (StarsI16)sizeof(retained_turin_research_requirements), latest,
        percent, &run->research, &iro_ops, run);
    if (latest)
        run->latest_starbase = *latest;
    return run->research_order;
}

static void verify_run(StarbaseRun *run, int action)
{
    static const StarsI16 designs[4] = {2, 4, 1, 3};
    static const StarsI16 kinds[4] = {2, 3, 1, 2};
    static const StarsI16 saved[4] = {18, 20, 17, 19};
    static const StarsI16 control_name_ids[4] = {
        0x436, 0x43b, 0x439, 0x432
    };
    static const StarsI16 action_name_ids[4] = {
        0x43b, 0x437, 0x438, 0x43c
    };
    const StarsI16 *name_ids = action ? action_name_ids : control_name_ids;
    StarsI16 index;
    for (index = 0; index < 4; ++index) {
        char expected[32];
        sprintf(expected, "ID%04X", (unsigned)name_ids[index]);
        if (run->requested_designs[index] != designs[index] ||
                run->requested_kinds[index] != kinds[index] ||
                run->saved_designs[index] != saved[index] ||
                run->saved_bitmaps[index] != (StarsI16)(0x20 + index) ||
                strcmp(run->saved_names[index], expected) != 0 ||
                run->random_limits[index] != 13 ||
                run->random_values[index] != name_ids[index] - 0x432)
            ++run->failures;
    }
    if (run->create_calls != 4 || run->save_calls != 4 ||
            run->random_calls != 4)
        ++run->failures;
}

static StarsI16 fleet_point(void *context,
    const StarsTurinTurnFleet *fleet, StarsPoint *out)
{
    StarbaseRun *run = (StarbaseRun *)context;
    if (!fleet || !out || fleet->planet_id < 0 || fleet->planet_id >= 32)
        return 0;
    *out = run->planet_points[(StarsU16)fleet->planet_id];
    return 1;
}

static StarsI16 fleet_has_teeth(void *context,
    const StarsTurinTurnFleet *fleet)
{
    (void)context;
    return fleet && fleet->owner == 1;
}

static StarsI16 ideal_warp(void *context,
    const StarsTurinTurnFleet *fleet, StarsI16 target, StarsPoint point)
{
    (void)context;
    if (!fleet || target != 26 || point.x != 1313 || point.y != 1262)
        return -1;
    return 5;
}

static const StarsU8 *no_wormhole(void *context, StarsPoint source,
    StarsU32 distance)
{
    StarbaseRun *run = (StarbaseRun *)context;
    (void)source;
    (void)distance;
    ++run->wormhole_calls;
    return 0;
}

static void verify_scout(StarbaseRun *run, int action)
{
    StarsTurinScoutAdapter adapter;
    StarsTurinTurnFleet fleet;
    StarsTurinFleetPolicy policy;
    StarsTurinFleetPlan plan;
    StarsI16 planet;
    StarsI16 expected_draw = action ? 4 : 99;
    for (planet = 0; planet < 32; ++planet) {
        run->planet_points[planet].x = (StarsI16)(8000 + planet * 100);
        run->planet_points[planet].y = 8000;
        run->ai_planet_records[(StarsU16)planet * 16U + 15U] = 0U;
    }
    run->planet_points[24].x = 1290;
    run->planet_points[24].y = 1258;
    run->planet_points[26].x = 1313;
    run->planet_points[26].y = 1262;
    run->ai_planet_records[24U * 16U + 15U] = 1U;
    run->ai_planet_records[26U * 16U + 15U] = 0x10U;
    memset(&adapter, 0, sizeof(adapter));
    adapter.planet_points = run->planet_points;
    adapter.ai_planet_records = run->ai_planet_records;
    adapter.ai_planet_records_size = sizeof(run->ai_planet_records);
    adapter.planet_count = 32U;
    adapter.ai_planet_record_stride = 16U;
    adapter.unknown_status_offset = 15U;
    adapter.order_capacity = 2U;
    adapter.status_ready = 1;
    adapter.fleet_point = fleet_point;
    adapter.fleet_has_teeth = fleet_has_teeth;
    adapter.ideal_warp = ideal_warp;
    adapter.find_wormhole = no_wormhole;
    adapter.random_value = seeded_random;
    adapter.context = run;
    memset(&fleet, 0, sizeof(fleet));
    fleet.id = 0;
    fleet.owner = 1;
    fleet.planet_id = 24;
    memset(&policy, 0, sizeof(policy));
    if (!stars_turin_plan_scout_planet(&adapter, &fleet, &policy, 0,
            &plan) || plan.action != STARS_TURIN_FLEET_ACTION_MOVE ||
            plan.target_planet != 26 || plan.order_kind != 0x1150U ||
            run->random_calls != 5 || run->random_limits[4] != 100 ||
            run->random_values[4] != expected_draw ||
            run->wormhole_calls != (action ? 1 : 0))
        ++run->failures;
}

static void run_turin_ensure_ai(StarbaseRun *run)
{
    StarsTurinDroneAiTurnInput input;
    StarsTurinDroneAiTurnOps ops;
    memset(&input, 0, sizeof(input));
    memset(&ops, 0, sizeof(ops));
    input.turn = 0;
    input.player_id = 1;
    input.ops = &ops;
    ops.ensure_ai = ensure_ai;
    ops.context = run;
    if (!stars_do_turin_drone_ai_turn(&input))
        ++run->failures;
    if (run->ensure_ai_calls != 1 || run->history_calls != 1 ||
            run->research_order != RETAINED_TURIN_RESEARCH_ORDER ||
            run->latest_starbase != RETAINED_LATEST_STARBASE_DESIGN ||
            run->research_writes != 1 ||
            run->written_current_tech != RETAINED_OUTPUT_CURRENT_TECH ||
            run->written_percent != RETAINED_OUTPUT_PERCENT_RESEARCH ||
            run->research.current_tech != RETAINED_OUTPUT_CURRENT_TECH ||
            run->research.percent_research != RETAINED_OUTPUT_PERCENT_RESEARCH)
        ++run->failures;
    {
        StarsI16 index;
        for (index = 0; index < 6; ++index)
            if (run->research.tech[index] != retained_player_technology[index])
                ++run->failures;
    }
}

int main(int argc, char **argv)
{
    StarbaseRun run;
    int action;
    StarsU32 tick;
    if (argc != 2 || (strcmp(argv[1], "control") != 0 &&
            strcmp(argv[1], "action") != 0))
        return 2;
    memset(&run, 0, sizeof(run));
    action = strcmp(argv[1], "action") == 0;
    tick = action ? 9953UL : 13747UL;
    stars_randomize2(&run.seeds, tick);
    if ((tick == 13747UL &&
            (run.seeds.first != 19UL || run.seeds.second != 279UL)) ||
            (tick == 9953UL &&
            (run.seeds.first != 443UL || run.seeds.second != 67UL)))
        ++run.failures;
    run_turin_ensure_ai(&run);
    verify_run(&run, action);
    verify_scout(&run, action);
    if (run.failures == 0)
        puts("TURIN_STARBASE_STARTUP=PASS NAMES=4 DRAWS=5 SCOUT=PASS");
    else
        printf("TURIN_STARBASE_STARTUP=FAIL FAILURES=%d\n",
            (int)run.failures);
    return run.failures == 0 ? 0 : 1;
}
