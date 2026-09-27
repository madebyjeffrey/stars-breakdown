#include "core.h"
#include <string.h>

/* The create.c flags and fixed-point constants below are named here so the
 * world generator reads as game policy rather than as a table of unexplained
 * byte-sized literals.  Their values are the packed values used by the
 * original Win16 records. */
#define GENERATE_WORLD_FIXED_MINERALS 0x0001U
#define GENERATE_WORLD_SCRIPTED_WORLDS 0x0008U
#define GENERATE_WORLD_ENHANCED_MINERALS 0x0020U
#define GENERATE_WORLD_NO_SPECIAL_WORLDS 0x0080U
#define GENERATE_WORLD_SCANNED_FLAG 0x0200U
#define GENERATE_WORLD_HOMEWORLD_FLAG 0x0400U
#define GENERATE_WORLD_POINT_OFFSET 0x03f2L
#define GENERATE_WORLD_EDGE_MARGIN 0x0013L
#define GENERATE_WORLD_INVERSE_BIAS 0x0960L
#define GENERATE_WORLD_MAX_PLANETS 999
#define GENERATE_WORLD_SPECIAL_ID_LIMIT 1000
#define GENERATE_WORLD_BASE_DENSITY_DIVISOR 5000L
#define GENERATE_WORLD_SPECIAL_ENVIRONMENT_PLANET 5
#define GENERATE_WORLD_SPECIAL_ENVIRONMENT_PLANET_2 11
#define GENERATE_WORLD_SPECIAL_ENVIRONMENT_RETRY_LIMIT 0x61

static StarsU16 generate_world_read16(const StarsU8 *raw, StarsU16 offset)
{
    return (StarsU16)(raw[offset] |
        ((StarsU16)raw[(StarsU16)(offset + 1U)] << 8));
}

static StarsI16 generate_world_signed16(StarsU16 value)
{
    return value < 32768U ? (StarsI16)value :
        (StarsI16)((StarsI32)value - 65536L);
}

static StarsU32 generate_world_signed_quotient(StarsU32 value,
    StarsU32 divisor)
{
    if (value & 0x80000000UL)
        return (StarsU32)0 - (((StarsU32)0 - value) / divisor);
    return value / divisor;
}

static StarsU32 generate_world_advance_seed(StarsU32 seed,
    StarsU32 divisor, StarsU32 multiplier, StarsU32 correction,
    StarsU32 modulus)
{
    StarsU32 quotient;
    StarsU32 next;
    quotient = generate_world_signed_quotient(seed, divisor);
    next = (seed - quotient * divisor) * multiplier -
        quotient * correction;
    if (next & 0x80000000UL)
        next += modulus;
    return next;
}

static StarsI16 generate_world_random_seed(StarsSeeds *seeds,
    StarsI16 limit)
{
    StarsU32 difference;
    seeds->first = generate_world_advance_seed(seeds->first, 53668UL,
        40014UL, 12211UL, 2147483563UL);
    seeds->second = generate_world_advance_seed(seeds->second, 52774UL,
        40692UL, 3791UL, 2147483399UL);
    difference = seeds->first - seeds->second;
    if (!difference || (difference & 0x80000000UL))
        difference += 2147483562UL;
    if (limit <= 0)
        return 0;
    return (StarsI16)(difference % (StarsU32)limit);
}

StarsI16 stars_generate_world_read_game_state(const StarsU8 *raw,
    const StarsSeeds *seeds, StarsGenerateWorldGameState *state)
{
    if (!raw || !seeds || !state)
        return 0;
    state->game_id = (StarsU32)raw[0] |
        ((StarsU32)raw[1] << 8) |
        ((StarsU32)raw[2] << 16) |
        ((StarsU32)raw[3] << 24);
    state->map_size = generate_world_signed16(generate_world_read16(raw, 4));
    state->density = generate_world_signed16(generate_world_read16(raw, 6));
    state->player_count = generate_world_signed16(
        generate_world_read16(raw, 8));
    state->planet_max = generate_world_signed16(
        generate_world_read16(raw, 10));
    state->start_distance = generate_world_signed16(
        generate_world_read16(raw, 12));
    state->dirty = generate_world_read16(raw, 14);
    state->generation_flags = generate_world_read16(raw, 16);
    state->turn = generate_world_signed16(generate_world_read16(raw, 18));
    state->random = *seeds;
    return 1;
}

StarsI16 stars_generate_world_random_from_state(void *context,
    StarsI16 limit)
{
    StarsGenerateWorldGameState *state;
    state = (StarsGenerateWorldGameState *)context;
    if (!state)
        return 0;
    return generate_world_random_seed(&state->random, limit);
}

static StarsI16 generate_world_random(const StarsGenerateWorldInput *input,
    StarsI16 limit)
{
    if (!input->random_value || limit <= 0)
        return 0;
    return input->random_value(input->context, limit);
}

static StarsI32 generate_world_distance(StarsPoint a, StarsPoint b)
{
    StarsI32 dx;
    StarsI32 dy;

    dx = (StarsI32)a.x - (StarsI32)b.x;
    dy = (StarsI32)a.y - (StarsI32)b.y;
    return dx * dx + dy * dy;
}

static void generate_world_sort(StarsPoint *points, StarsI16 count)
{
    StarsI16 i;
    StarsI16 j;
    StarsPoint point;

    /* This is the ordered effect of the original ICompLong qsort pass. */
    for (i = 1; i < count; ++i) {
        point = points[i];
        j = i;
        while (j > 0 && (points[j - 1].x > point.x ||
                (points[j - 1].x == point.x && points[j - 1].y > point.y))) {
            points[j] = points[j - 1];
            --j;
        }
        points[j] = point;
    }
}

static void generate_world_smooth(const StarsGenerateWorldInput *input,
    StarsPoint *points, StarsI16 count)
{
    StarsI16 i;
    StarsI16 j;
    StarsI16 selected;
    StarsI16 nearest;
    StarsI32 nearest_distance;
    StarsI32 distance;
    StarsI32 x;
    StarsI32 y;

    for (i = 0; i < count; ++i) {
        selected = generate_world_random(input, count);
        nearest = -1;
        nearest_distance = 0x7fffffffL;
        for (j = 0; j < count; ++j) {
            if (j == selected)
                continue;
            distance = generate_world_distance(points[selected], points[j]);
            if (distance < nearest_distance) {
                nearest_distance = distance;
                nearest = j;
            }
        }
        if (nearest < 0 || nearest_distance <= 0x90L)
            continue;

        x = points[selected].x;
        y = points[selected].y;
        if (nearest_distance < 0x145L) {
            x = (x * 4 + points[nearest].x) / 5;
            y = (y * 4 + points[nearest].y) / 5;
        } else if (nearest_distance < 0x272L) {
            x = (x * 2 + points[nearest].x) / 3;
            y = (y * 2 + points[nearest].y) / 3;
        } else if (nearest_distance < 0x641L) {
            x = (x + points[nearest].x) / 2;
            y = (y + points[nearest].y) / 2;
        } else {
            x = (points[nearest].x * 2 + x) / 3;
            y = (points[nearest].y * 2 + y) / 3;
        }
        points[selected].x = (StarsI16)x;
        points[selected].y = (StarsI16)y;
    }
}

static void generate_world_assign_point_ids(const StarsGenerateWorldInput *input,
    StarsGenerateWorldLayout *layout)
{
    StarsU8 used[125];
    StarsI16 i;
    StarsI16 candidate;
    StarsI16 id_limit;

    memset(used, 0, sizeof used);
    /* The original special map mode admits the terminal ID 999 when a
     * collision walk reaches it; ordinary maps wrap after 998. */
    id_limit = (input->world_flags & STARS_GENERATE_WORLD_SPECIAL_ID_MODE) != 0U ?
        GENERATE_WORLD_SPECIAL_ID_LIMIT : GENERATE_WORLD_MAX_PLANETS;
    for (i = 0; i < layout->point_count; ++i) {
        candidate = generate_world_random(input, GENERATE_WORLD_MAX_PLANETS);
        while ((used[candidate >> 3] &
                (StarsU8)(1U << (candidate & 7))) != 0U) {
            ++candidate;
            if (candidate >= id_limit)
                candidate = 0;
        }
        used[candidate >> 3] = (StarsU8)(used[candidate >> 3] |
            (StarsU8)(1U << (candidate & 7)));
        layout->point_ids[i] = candidate;
    }
}

StarsI16 stars_generate_world_layout(StarsGenerateWorldInput *input)
{
    StarsGenerateWorldLayout *layout;
    StarsI32 galaxy;
    StarsI32 density_count;
    StarsI32 requested;
    StarsI32 minimum_distance_squared;
    StarsI32 x_limit;
    StarsI32 dy;
    StarsI32 distance;
    StarsI16 i;
    StarsI16 j;
    StarsI16 removed;
    StarsI16 target_removed;
    StarsI16 random_index;
    StarsI16 compacted;

    if (!input || !input->layout)
        return 0;
    layout = input->layout;
    layout->point_count = 0;
    layout->candidate_count = 0;
    layout->rejected_count = 0;
    layout->galaxy_extent = 0;
    layout->galaxy_inverse = 0;

    galaxy = (StarsI32)input->map_size * 400L + 400L;
    if (galaxy <= 0 || input->minimum_distance < 0)
        return 0;
    layout->galaxy_extent = (StarsI16)galaxy;
    layout->galaxy_inverse = (StarsI16)((StarsI32)input->map_size * 400L +
        GENERATE_WORLD_INVERSE_BIAS);

    density_count = galaxy * galaxy / GENERATE_WORLD_BASE_DENSITY_DIVISOR;
    density_count += (density_count / 4L) * ((StarsI32)input->density - 1L);
    if (input->density > 2)
        density_count += density_count / 4L;
    if (density_count > GENERATE_WORLD_MAX_PLANETS)
        density_count = GENERATE_WORLD_MAX_PLANETS;
    if (density_count < 1L)
        density_count = 1L;

    requested = density_count + density_count / 7L;
    if (requested > GENERATE_WORLD_MAX_PLANETS)
        requested = GENERATE_WORLD_MAX_PLANETS;
    layout->candidate_count = (StarsI16)requested;
    minimum_distance_squared = (StarsI32)input->minimum_distance *
        (StarsI32)input->minimum_distance;
    x_limit = galaxy - GENERATE_WORLD_EDGE_MARGIN;

    for (i = 0; i < (StarsI16)requested; ++i) {
        layout->points[i].x = (StarsI16)(generate_world_random(input,
            (StarsI16)x_limit) + GENERATE_WORLD_POINT_OFFSET);
        layout->points[i].y = (StarsI16)(generate_world_random(input,
            (StarsI16)x_limit) + GENERATE_WORLD_POINT_OFFSET);
    }
    generate_world_sort(layout->points, (StarsI16)requested);

    removed = 0;
    for (i = 0; i < (StarsI16)requested; ++i) {
        if (layout->points[i].y < 0)
            continue;
        x_limit = (StarsI32)layout->points[i].x + input->minimum_distance;
        for (j = (StarsI16)(i + 1); j < (StarsI16)requested; ++j) {
            if (layout->points[j].x > x_limit)
                break;
            dy = (StarsI32)layout->points[i].y - layout->points[j].y;
            if (dy < 0)
                dy = -dy;
            if (dy <= input->minimum_distance) {
                distance = generate_world_distance(layout->points[i],
                    layout->points[j]);
                if (distance <= minimum_distance_squared) {
                    layout->points[j].y = (StarsI16)-100;
                    ++removed;
                }
            }
        }
    }

    target_removed = (StarsI16)(requested - density_count);
    while (removed < target_removed) {
        random_index = generate_world_random(input, (StarsI16)requested);
        if (layout->points[random_index].y >= 0) {
            layout->points[random_index].y = (StarsI16)-100;
            ++removed;
        }
    }

    compacted = 0;
    for (i = 0; i < (StarsI16)requested; ++i) {
        if (layout->points[i].y >= 0) {
            layout->points[compacted] = layout->points[i];
            ++compacted;
        }
    }
    layout->point_count = compacted;
    layout->rejected_count = removed;

    if (input->smooth_layout)
        generate_world_smooth(input, layout->points, layout->point_count);
    generate_world_sort(layout->points, layout->point_count);
    generate_world_assign_point_ids(input, layout);
    return 1;
}

static StarsI16 generate_world_planet_random(
    const StarsGenerateWorldPlanetInput *input, StarsI16 limit)
{
    if (!input->random_value || limit <= 0)
        return 0;
    return input->random_value(input->context, limit);
}

StarsI16 stars_generate_world_init_planets(StarsGenerateWorldPlanetInput *input)
{
    StarsI16 i;
    StarsI16 mineral;
    StarsI16 pass;
    StarsI16 random_value;
    StarsI16 random_mineral;
    StarsI16 concentration;
    StarsGenerateWorldPlanet *planet;

    if (!input || !input->planets || input->planet_count < 1 ||
        input->planet_count > GENERATE_WORLD_MAX_PLANETS)
        return 0;

    for (i = 0; i < input->planet_count; ++i) {
        planet = &input->planets[i];
        planet->id = i;
        planet->owner = (StarsI16)-1;
        planet->flags = 7U;
        planet->mine_count = 0;
        planet->factory_count = 0;
        planet->defense_count = 0;
        planet->production_slot = 0;
        planet->special_world = 0;
        if ((input->world_flags & GENERATE_WORLD_NO_SPECIAL_WORLDS) == 0U &&
            generate_world_planet_random(input, 3) == 0)
            planet->special_world = 1;

        planet->environment[0] = (StarsI16)(
            generate_world_planet_random(input, 0x5a) + 1 +
            generate_world_planet_random(input, 10));
        planet->environment[1] = (StarsI16)(
            generate_world_planet_random(input, 0x5a) + 1 +
            generate_world_planet_random(input, 10));
        planet->environment[2] = (StarsI16)(
            generate_world_planet_random(input, 99) + 1);
        planet->environment_original[0] = planet->environment[0];
        planet->environment_original[1] = planet->environment[1];
        planet->environment_original[2] = planet->environment[2];

        if ((input->world_flags & GENERATE_WORLD_SCRIPTED_WORLDS) != 0U) {
            if (i == GENERATE_WORLD_SPECIAL_ENVIRONMENT_PLANET) {
                for (mineral = 0; mineral < 3; ++mineral) {
                    planet->environment[mineral] -= 5;
                    planet->environment_original[mineral] =
                        planet->environment[mineral];
                }
            } else if (i == GENERATE_WORLD_SPECIAL_ENVIRONMENT_PLANET_2) {
                planet->environment[0] += 20;
                planet->environment_original[0] = planet->environment[0];
            }
        }

        for (mineral = 0; mineral < 3; ++mineral) {
            if ((input->world_flags & GENERATE_WORLD_FIXED_MINERALS) == 0U) {
                random_value = generate_world_planet_random(input, 0x2d);
                random_mineral = generate_world_planet_random(input, 0x2d);
                concentration = (StarsI16)(random_value + random_mineral + 0x1f);
                if (planet->environment[2] > 'Y') {
                    concentration = (StarsI16)(concentration +
                        generate_world_planet_random(input,
                            (StarsI16)(99 - concentration)) / 2);
                }
                planet->mineral_concentration[mineral] = concentration;
            } else {
                planet->mineral_concentration[mineral] = 100;
            }
            planet->mineral_rate[mineral] = 0;
            if ((input->world_flags & GENERATE_WORLD_ENHANCED_MINERALS) != 0U &&
                planet->mineral_concentration[mineral] < 40)
                planet->mineral_concentration[mineral] += 5;
        }

        if ((input->world_flags & GENERATE_WORLD_FIXED_MINERALS) == 0U)
            random_value = generate_world_planet_random(input, 0x1b);
        else
            random_value = 100;
        if (random_value < 0x12) {
            if (random_value < 9) {
                for (pass = (StarsI16)(random_value + 1);
                     pass < 0x10; pass = (StarsI16)(pass << 1)) {
                    concentration = (StarsI16)(
                        generate_world_planet_random(input, 0x1e) + 1);
                    random_mineral = generate_world_planet_random(input, 3);
                    planet->mineral_concentration[random_mineral] =
                        concentration;
                }
            } else {
                concentration = (StarsI16)(
                    generate_world_planet_random(input, 0x1e) + 1);
                random_mineral = generate_world_planet_random(input, 3);
                planet->mineral_concentration[random_mineral] = concentration;
            }
        }
    }

    /* The original initializes the first planet's fixed-point mineral
     * baseline after the per-planet pass.  Keep that observable phase in the
     * model instead of folding it into the random concentration loop. */
    for (mineral = 0; mineral < 3; ++mineral) {
        concentration = input->planets[0].mineral_concentration[mineral];
        input->planets[0].mineral_rate[mineral] =
            (StarsI32)generate_world_planet_random(input,
                (StarsI16)(concentration * 10)) + 10L;
        if (input->planets[0].mineral_rate[mineral] < 200L)
            input->planets[0].mineral_rate[mineral] +=
                (StarsI32)generate_world_planet_random(input, 150) + 155L;
        if ((input->world_flags & GENERATE_WORLD_ENHANCED_MINERALS) != 0U)
            input->planets[0].mineral_rate[mineral] +=
                input->planets[0].mineral_rate[mineral] / 4L;
    }
    return 1;
}

static StarsI16 generate_world_start_random(
    const StarsGenerateWorldStartsInput *input, StarsI16 limit)
{
    if (!input->random_value || limit <= 0)
        return 0;
    return input->random_value(input->context, limit);
}

static StarsI16 generate_world_start_in_box(StarsPoint point,
    StarsI16 lower, StarsI16 upper)
{
    return point.x >= lower && point.x <= upper &&
        point.y >= lower && point.y <= upper;
}

static StarsI32 generate_world_start_distance(StarsPoint a, StarsPoint b)
{
    StarsI32 dx;
    StarsI32 dy;

    dx = (StarsI32)a.x - (StarsI32)b.x;
    dy = (StarsI32)a.y - (StarsI32)b.y;
    return dx * dx + dy * dy;
}

static StarsI16 generate_world_start_is_usable(
    const StarsGenerateWorldStartsInput *input, StarsI16 candidate,
    const StarsI16 *selected, StarsI16 count, StarsI32 minimum,
    StarsI32 maximum, StarsI16 lower, StarsI16 upper)
{
    StarsI16 i;
    StarsI16 close_enough;
    StarsI32 distance;

    if (!generate_world_start_in_box(input->layout->points[candidate],
            lower, upper))
        return 0;
    close_enough = 0;
    for (i = 0; i < count; ++i) {
        distance = generate_world_start_distance(
            input->layout->points[candidate],
            input->layout->points[selected[i]]);
        if (distance < 1L || distance < minimum)
            return 0;
        if (distance <= maximum)
            close_enough = 1;
    }
    return count == 0 || close_enough;
}

StarsI16 stars_generate_world_assign_starts(
    StarsGenerateWorldStartsInput *input)
{
    StarsI16 selected[16];
    StarsI16 player;
    StarsI16 attempt;
    StarsI16 candidate;
    StarsI16 fallback;
    StarsI16 lower;
    StarsI16 upper;
    StarsI16 center;
    StarsPoint center_point;
    StarsI16 found;
    StarsI16 swap_index;
    StarsI16 swap_value;
    StarsI32 base_distance;
    StarsI32 available_distance;
    StarsI32 target_distance;
    StarsI32 minimum_distance;
    StarsI32 maximum_distance;

    if (!input || !input->layout || !input->start_planets ||
        input->layout->point_count < 1 ||
        input->layout->point_count > GENERATE_WORLD_MAX_PLANETS ||
        input->player_count < 1 || input->player_count > 16 ||
        input->player_count > input->layout->point_count ||
        input->start_distance < 0)
        return 0;

    base_distance = (StarsI32)input->layout->galaxy_extent * 6L;
    available_distance =
        (StarsI32)input->layout->galaxy_extent *
        (StarsI32)input->layout->galaxy_extent / input->player_count -
        base_distance;
    if (available_distance < 0L)
        available_distance = 0L;
    target_distance = base_distance +
        available_distance * 9L / 10L * input->start_distance / 3L;
    minimum_distance = target_distance * 9L / 10L;
    maximum_distance = target_distance * 7L / 6L;

    center = (StarsI16)(input->layout->galaxy_extent / 2L + 1000L);
    center_point.x = center;
    center_point.y = center;
    if (input->player_count < 5) {
        if (input->player_count < 3) {
            lower = (StarsI16)((StarsI32)input->layout->galaxy_extent *
                3L / 20L + 1000L);
            upper = (StarsI16)((StarsI32)input->layout->galaxy_extent *
                17L / 20L + 1000L);
        } else {
            lower = (StarsI16)((StarsI32)input->layout->galaxy_extent /
                10L + 1000L);
            upper = (StarsI16)((StarsI32)input->layout->galaxy_extent *
                9L / 10L + 1000L);
        }
    } else {
        lower = (StarsI16)((StarsI32)input->layout->galaxy_extent /
            20L + 1000L);
        upper = (StarsI16)((StarsI32)input->layout->galaxy_extent *
            19L / 20L + 1000L);
    }

select_starts:
    found = 0;
    for (attempt = 0; attempt < 50; ++attempt) {
        candidate = generate_world_start_random(input,
            input->layout->point_count);
        if (!generate_world_start_in_box(input->layout->points[candidate],
                (StarsI16)((StarsI32)input->layout->galaxy_extent / 4L +
                    1000L),
                (StarsI16)((StarsI32)input->layout->galaxy_extent * 3L /
                    4L + 1000L))) {
            if (!found || generate_world_start_distance(
                    input->layout->points[candidate],
                    center_point) <
                    generate_world_start_distance(
                    input->layout->points[selected[0]],
                    center_point)) {
                selected[0] = candidate;
                found = 1;
            }
        } else {
            selected[0] = candidate;
            found = 1;
            break;
        }
    }
    if (!found)
        return 0;

    for (player = 1; player < input->player_count; ++player) {
        found = 0;
        for (attempt = 0; attempt < 50; ++attempt) {
            candidate = generate_world_start_random(input,
                input->layout->point_count);
            if (generate_world_start_is_usable(input, candidate, selected,
                    player, minimum_distance, maximum_distance, lower, upper)) {
                selected[player] = candidate;
                found = 1;
                break;
            }
        }
        if (!found) {
            fallback = (StarsI16)(selected[player - 1] + 1);
            for (attempt = 0; attempt < input->layout->point_count;
                 ++attempt) {
                candidate = (StarsI16)(fallback + attempt);
                if (candidate >= input->layout->point_count)
                    candidate = (StarsI16)(candidate -
                        input->layout->point_count);
                if (generate_world_start_is_usable(input, candidate, selected,
                        player, minimum_distance, maximum_distance, lower, upper)) {
                    selected[player] = candidate;
                    found = 1;
                    break;
                }
            }
        }
        if (!found)
        {
            /* The original widens the acceptable band and restarts the
             * complete selection when the sequential fallback cycles back
             * to its first candidate. */
            minimum_distance -= target_distance / 35L;
            maximum_distance += target_distance / 35L;
            goto select_starts;
        }
    }

    for (player = 0; player < input->player_count; ++player) {
        swap_index = (StarsI16)(player + generate_world_start_random(input,
            (StarsI16)(input->player_count - player)));
        swap_value = selected[swap_index];
        selected[swap_index] = selected[player];
        selected[player] = swap_value;
        input->start_planets[player] = selected[player];
    }
    return 1;
}

StarsI16 stars_generate_world_init_players(
    StarsGenerateWorldPlayerInput *input)
{
    StarsI16 player_index;
    StarsI16 stat_index;
    StarsI16 race_type;
    StarsI16 ceiling;
    StarsI16 relation;
    StarsGenerateWorldPlayer *player;

    if (!input || !input->players || !input->race_stat ||
        !input->race_flag || input->player_count < 1 ||
        input->player_count > 16)
        return 0;

    for (player_index = 0; player_index < input->player_count;
         ++player_index) {
        player = &input->players[player_index];
        player->id = player_index;
        player->homeworld = -1;
        player->messages_sent = 0;
        player->is_dead = 0;
        player->ships_created = 0;
        player->salt = 0L;
        player->research_percent = 0;
        player->resources_last_year = 0L;
        player->score = 0;
        for (relation = 0; relation < input->player_count; ++relation)
            player->relations[relation] = 0;
        if (input->randomize_race &&
            input->race_flag(input->context, player_index, 0x1e) != 0)
            input->randomize_race(input->context, player_index);

        for (stat_index = 0; stat_index < 6; ++stat_index) {
            player->race_stats[stat_index] = 0;
            player->tech[stat_index] = 0;
        }
        race_type = input->race_stat(input->context, player_index, 0x0e);
        player->race_type = race_type;
        switch (race_type) {
        case 1:
            player->tech[4] = 5;
            break;
        case 2:
            player->tech[1] = 6;
            player->tech[2] = 1;
            player->tech[0] = 1;
            break;
        case 3:
            player->tech[5] = 6;
            player->tech[3] = 2;
            player->tech[0] = 1;
            player->tech[1] = 1;
            player->tech[2] = 1;
            break;
        case 5:
            player->tech[2] = 2;
            player->tech[5] = 2;
            break;
        case 6:
            player->tech[0] = 4;
            break;
        case 7:
            player->tech[2] = 5;
            player->tech[3] = 5;
            break;
        case 8:
            player->tech[0] = 1;
            break;
        case 9:
            for (stat_index = 0; stat_index < 6; ++stat_index)
                player->race_stats[stat_index] = 3;
            break;
        default:
            break;
        }

        if (input->race_flag(input->context, player_index, 0x1d) != 0) {
            ceiling = race_type == 9 ? 4 : 3;
            for (stat_index = 0; stat_index < 6; ++stat_index) {
                if (player->race_stats[stat_index] < ceiling &&
                    input->race_stat(input->context, player_index,
                        (StarsI16)(stat_index + 8)) == 0)
                    player->race_stats[stat_index] = ceiling;
            }
        }
        if (input->race_flag(input->context, player_index, 8) != 0)
            ++player->tech[2];
        if (input->race_flag(input->context, player_index, 0) != 0 &&
            (input->world_flags & 8U) == 0U)
            ++player->tech[2];
        if (input->race_flag(input->context, player_index, 9) == 0)
            player->research_percent = 15;
        player->messages_sent = 4;
    }
    return 1;
}

static StarsI16 generate_world_homeworld_random(
    const StarsGenerateWorldHomeworldInput *input, StarsI16 limit)
{
    if (!input->random_value || limit <= 0)
        return 0;
    return input->random_value(input->context, limit);
}

StarsI16 stars_generate_world_assign_homeworlds(
    StarsGenerateWorldHomeworldInput *input)
{
    StarsI16 player_index;
    StarsI16 mineral;
    StarsI16 environment;
    StarsI16 planet_index;
    StarsI16 value;
    StarsI16 true_max_growth;
    StarsI16 advantage_points;
    StarsI16 homeworld_stat;
    StarsI16 minimum_mineral;
    StarsI16 concentration_delta;
    StarsI32 advantage_rate_bonus;
    StarsGenerateWorldPlanet *planet;
    StarsGenerateWorldPlayer *player;

    if (!input || !input->start_planets || !input->planets ||
        !input->players || !input->race_flag || input->player_count < 1 ||
        input->player_count > 16 || input->planet_count < 1 ||
        input->planet_count > GENERATE_WORLD_MAX_PLANETS || input->mineral_floor < 0 ||
        input->mineral_floor > 100)
        return 0;

    for (player_index = 0; player_index < input->player_count;
         ++player_index) {
        planet_index = input->start_planets[player_index];
        if (planet_index < 0 || planet_index >= input->planet_count)
            return 0;
        planet = &input->planets[planet_index];
        player = &input->players[player_index];
        planet->owner = player_index;
        planet->flags = (StarsU16)(planet->flags |
            GENERATE_WORLD_SCANNED_FLAG | GENERATE_WORLD_HOMEWORLD_FLAG);
        /* These are the packed cMines, cFactories, and cDefenses fields
         * initialized by the original homeworld path. */
        planet->mine_count = 10;
        planet->factory_count = 10;
        planet->defense_count = 10;
        planet->production_slot = 0;
        planet->population = input->race_flag(input->context,
            player_index, 0x0b) != 0 ? 175L : 250L;
        for (mineral = 0; mineral < 3; ++mineral) {
            planet->mineral_rate[mineral] =
                input->planets[0].mineral_rate[mineral];
            if (planet->mineral_concentration[mineral] < input->mineral_floor)
                planet->mineral_concentration[mineral] = input->mineral_floor;
        }
        /* The enhanced-mineral game option applies the same fixed-point
         * population adjustment as GenerateWorld's rgwtMin[3] path. */
        if ((input->world_flags & GENERATE_WORLD_ENHANCED_MINERALS) != 0U &&
                input->true_max_growth) {
            true_max_growth = input->true_max_growth(input->context,
                player_index);
            planet->population = (planet->population *
                ((StarsI32)true_max_growth * 2L + 10L)) / 10L;
        }
        if (input->advantage_points && input->race_stat) {
            advantage_points = input->advantage_points(input->context,
                player_index);
            if (advantage_points >= 0x33)
                advantage_points = 0x32;
            homeworld_stat = input->race_stat(input->context, player_index, 7);
            if (homeworld_stat == 0) {
                /* The default homeworld advantage is distributed as a
                 * common mineral-rate increment plus one extra increment on
                 * the currently weakest mineral. */
                advantage_rate_bonus = ((StarsI32)advantage_points * 10L) /
                    4L;
                minimum_mineral = 0;
                for (mineral = 1; mineral < 3; ++mineral) {
                    if (planet->mineral_rate[mineral] <
                            planet->mineral_rate[minimum_mineral])
                        minimum_mineral = mineral;
                }
                for (mineral = 0; mineral < 3; ++mineral)
                    planet->mineral_rate[mineral] += advantage_rate_bonus;
                planet->mineral_rate[minimum_mineral] +=
                    advantage_rate_bonus;
            } else if (homeworld_stat == 2)
                planet->mine_count = (StarsI16)(planet->mine_count +
                    advantage_points / 2);
            else if (homeworld_stat == 3)
                planet->factory_count = (StarsI16)(planet->factory_count +
                    (advantage_points + 5) / 10);
            else if (homeworld_stat == 4)
                planet->defense_count = (StarsI16)(planet->defense_count +
                    (advantage_points + 5) / 10);
            else if (homeworld_stat == 1) {
                concentration_delta = (advantage_points < 1 ||
                    advantage_points > 2) ? advantage_points / 2 : 1;
                minimum_mineral = 0;
                for (mineral = 1; mineral < 3; ++mineral) {
                    if (planet->mineral_concentration[mineral] <
                            planet->mineral_concentration[minimum_mineral])
                        minimum_mineral = mineral;
                }
                planet->mineral_concentration[minimum_mineral] =
                    (StarsI16)(planet->mineral_concentration[minimum_mineral] +
                    concentration_delta);
                concentration_delta = (StarsI16)((concentration_delta + 1) / 2);
                for (mineral = 0; mineral < 3; ++mineral)
                    planet->mineral_concentration[mineral] = (StarsI16)(
                        planet->mineral_concentration[mineral] +
                        concentration_delta);
            } else if (homeworld_stat == 8) {
                planet->mine_count = 0;
                planet->factory_count = 0;
                planet->defense_count = 0;
            }
        }
        for (environment = 0; environment < 3; ++environment) {
            if (player->environment_max[environment] < 0) {
                value = (StarsI16)(generate_world_homeworld_random(input,
                    99) + 1);
            } else {
                value = (StarsI16)(player->environment_min[environment] +
                    (player->environment_max[environment] -
                    player->environment_min[environment]) / 2);
            }
            planet->environment[environment] = value;
            planet->environment_original[environment] = value;
        }
        player->homeworld = planet_index;
    }
    return 1;
}

static StarsI16 generate_world_startup_ship(
    StarsGenerateWorldStartupShipsInput *input, StarsI16 player,
    StarsI16 design, StarsI16 add_design)
{
    StarsI16 result;

    result = input->create_ship(input->context, player,
        input->players[player].homeworld, design, add_design);
    ++input->players[player].ships_created;
    return result;
}

#define GENERATE_WORLD_PART_ENGINE 1U
#define GENERATE_WORLD_PART_LASER 2U
#define GENERATE_WORLD_PART_TORPEDO 4U
#define GENERATE_WORLD_PART_SHIELD 8U
#define GENERATE_WORLD_PART_ARMOR 0x10U
#define GENERATE_WORLD_PART_SCANNER 0x20U
#define GENERATE_WORLD_PART_MINELAYER 0x40U
#define GENERATE_WORLD_PART_SPECIAL 0x80U
#define GENERATE_WORLD_RACE8_SCANNER 31

static void generate_world_repair_designs(
    StarsGenerateWorldStartupShipsInput *input, StarsI16 player_index)
{
    StarsGenerateWorldDesign *designs;
    StarsU16 design_count;
    StarsU16 design_index;
    StarsU16 part_index;
    StarsU16 part_count;
    StarsU16 candidate_count;
    StarsU16 candidate_index;
    StarsU16 group;
    StarsU16 subtype;
    StarsU16 current;
    StarsU16 candidates[5];
    StarsGenerateWorldDesignPart *part;

    designs = input->designs;
    design_count = input->design_count;
    if (input->get_design_bank)
        designs = input->get_design_bank(input->context, player_index,
            &design_count);
    if (!designs || !input->part_available)
        return;
    for (design_index = 0; design_index < design_count;
            ++design_index) {
        part_count = designs[design_index].part_count;
        if (part_count > 16)
            part_count = 16;
        for (part_index = 0; part_index < part_count; ++part_index) {
            part = &designs[design_index].parts[part_index];
            group = part->group;
            subtype = part->subtype;
            current = part->part;
            candidate_count = 0;
            if (group == GENERATE_WORLD_PART_ENGINE && subtype == 1U) {
                if (input->environment_special == -1 || current != 0x0fU ||
                        input->environment_special > 'T')
                    candidates[candidate_count++] = 10U;
                candidates[candidate_count++] = 5U;
                candidates[candidate_count++] = 4U;
                candidates[candidate_count++] = 2U;
                candidates[candidate_count++] = 3U;
            } else if (group == GENERATE_WORLD_PART_LASER &&
                    (subtype == 0U || subtype == 1U)) {
                candidates[0] = 4U;
                candidates[1] = 2U;
                candidate_count = 2;
            } else if ((group == GENERATE_WORLD_PART_TORPEDO ||
                    group == GENERATE_WORLD_PART_SHIELD) &&
                    (subtype == 0U || subtype == 1U)) {
                candidates[0] = 2U;
                candidates[1] = 1U;
                candidate_count = 2;
            } else if (group == GENERATE_WORLD_PART_ARMOR &&
                    (subtype == 0U || subtype == 1U)) {
                candidates[0] = 3U;
                candidates[1] = 1U;
                candidate_count = 2;
            } else if ((group == GENERATE_WORLD_PART_SCANNER ||
                    group == GENERATE_WORLD_PART_MINELAYER) &&
                    subtype == 0U) {
                candidates[0] = 1U;
                candidate_count = 1;
            } else if (group == GENERATE_WORLD_PART_SPECIAL &&
                    (subtype == 0U || subtype == 1U)) {
                candidates[0] = 2U;
                candidates[1] = 0U;
                candidate_count = 2;
            }
            for (candidate_index = 0; candidate_index < candidate_count;
                    ++candidate_index) {
                if (input->part_available(input->context, group, subtype,
                        candidates[candidate_index])) {
                    part->part = candidates[candidate_index];
                    break;
                }
            }
        }
    }
}

StarsI16 stars_generate_world_create_startup_ships(
    StarsGenerateWorldStartupShipsInput *input)
{
    StarsI16 player_index;
    StarsI16 race_type;
    StarsI16 design;
    StarsI16 special_ships;
    StarsGenerateWorldPlayer *player;

    if (!input || !input->players || !input->race_flag ||
        !input->create_ship || input->player_count < 1 ||
        input->player_count > 16 || input->map_size < 0)
        return 0;

    for (player_index = 0; player_index < input->player_count;
         ++player_index) {
        player = &input->players[player_index];
        if (player->homeworld < 0)
            return 0;
        player->ships_created = 0;
        race_type = player->race_type;
        if (race_type == 6) {
            generate_world_startup_ship(input, player_index, 4, 1);
            if (input->set_starbase)
                input->set_starbase(input->context, player_index,
                    player->homeworld);
        } else if (race_type == 2) {
            generate_world_startup_ship(input, player_index, 3, 1);
            if (player->tech[3] > 2) {
                generate_world_startup_ship(input, player_index, 7, 1);
                generate_world_startup_ship(input, player_index, 13, 1);
            }
        } else if (race_type == 9) {
            generate_world_startup_ship(input, player_index, 3, 1);
            generate_world_startup_ship(input, player_index, 4, 1);
        } else if (race_type == 1) {
            design = player->tech[0] < 2 ? 2 : 5;
            generate_world_startup_ship(input, player_index, design, 1);
            if (input->race_flag(input->context, player_index, 9) == 0)
                generate_world_startup_ship(input, player_index, 1, 1);
        } else {
            generate_world_startup_ship(input, player_index, 2, 1);
        }

        if (race_type == 0) {
            design = generate_world_startup_ship(input, player_index, 12, 1);
            generate_world_startup_ship(input, player_index, design, 0);
            generate_world_startup_ship(input, player_index, design, 0);
        } else if (race_type == 7) {
            generate_world_startup_ship(input, player_index, 11, 1);
        } else if (race_type == 8) {
            generate_world_startup_ship(input, player_index, 10, 1);
        } else {
            generate_world_startup_ship(input, player_index, 9, 1);
        }

        if (race_type == 5) {
            generate_world_startup_ship(input, player_index, 16, 1);
            generate_world_startup_ship(input, player_index, 18, 1);
        } else if (race_type == 3) {
            generate_world_startup_ship(input, player_index, 17, 1);
        } else if (race_type == 7) {
            generate_world_startup_ship(input, player_index, 7, 1);
            generate_world_startup_ship(input, player_index, 8, 1);
        }

        /* GenerateWorld creates a nearby special planet and its design-0
         * startup ship for races 7 and 6 on non-tiny maps.  Keep the
         * planet-selection/mutation service behind a typed boundary, but
         * retain its ordering and ship-count effect here. */
        if ((race_type == 7 || race_type == 6) && input->map_size > 0 &&
                input->create_special_startup) {
            special_ships = input->create_special_startup(input->context,
                player_index, player->homeworld);
            if (special_ships < 0)
                return 0;
            player->ships_created = (StarsI16)(player->ships_created +
                special_ships);
        }

        if (race_type == 9) {
            design = player->tech[3] < 4 ? 6 : 8;
            generate_world_startup_ship(input, player_index, design, 1);
            generate_world_startup_ship(input, player_index, 7, 1);
            generate_world_startup_ship(input, player_index, 14, 1);
        }
        if (input->race_flag(input->context, player_index, 9) == 0 &&
            input->race_flag(input->context, player_index, 2) != 0) {
            design = generate_world_startup_ship(input, player_index, 15, 1);
            generate_world_startup_ship(input, player_index, design, 0);
        }
        generate_world_repair_designs(input, player_index);
        if (race_type == 8 && input->set_scanner)
            input->set_scanner(input->context, player_index,
                player->homeworld, GENERATE_WORLD_RACE8_SCANNER);
    }
    return 1;
}

static StarsI16 generate_world_special_random(
    const StarsGenerateWorldSpecialPlanetInput *input, StarsI16 limit)
{
    if (!input->random_value || limit <= 0)
        return 0;
    return input->random_value(input->context, limit);
}

static StarsI32 generate_world_special_distance(StarsPoint a, StarsPoint b)
{
    StarsI32 dx;
    StarsI32 dy;

    dx = (StarsI32)a.x - (StarsI32)b.x;
    dy = (StarsI32)a.y - (StarsI32)b.y;
    return dx * dx + dy * dy;
}

StarsI16 stars_generate_world_create_special_planet(
    StarsGenerateWorldSpecialPlanetInput *input)
{
    StarsI16 candidate;
    StarsI16 selected;
    StarsI16 fallback;
    StarsI16 candidate_count;
    StarsI16 attempt;
    StarsI16 environment;
    StarsI16 mineral;
    StarsI16 value;
    StarsI32 lower_distance;
    StarsI32 upper_distance;
    StarsI32 distance;
    StarsI32 fallback_distance;
    StarsGenerateWorldPlanet *home;
    StarsGenerateWorldPlanet *planet;

    if (!input || !input->layout || !input->planets ||
        !input->desirability || !input->create_ship ||
        !input->special_planet || input->planet_count < 1 ||
        input->planet_count > GENERATE_WORLD_MAX_PLANETS || input->player < 0 ||
        input->homeworld < 0 || input->homeworld >= input->planet_count)
        return 0;

    lower_distance = (StarsI32)input->layout->galaxy_extent * 15L / 100L;
    lower_distance *= lower_distance;
    upper_distance = (StarsI32)input->layout->galaxy_extent * 23L / 100L;
    upper_distance *= upper_distance;
    selected = -1;
    fallback = -1;
    fallback_distance = 10000000L;
    candidate_count = 0;
    for (candidate = 0; candidate < input->planet_count; ++candidate) {
        if (candidate == input->homeworld ||
            input->planets[candidate].owner != -1)
            continue;
        distance = generate_world_special_distance(
            input->layout->points[input->homeworld],
            input->layout->points[candidate]);
        if (distance < lower_distance || distance > upper_distance) {
            if (distance < fallback_distance) {
                fallback = candidate;
                fallback_distance = distance;
            }
        } else {
            ++candidate_count;
            if (generate_world_special_random(input, candidate_count) == 0)
                selected = candidate;
        }
    }
    if (selected < 0)
        selected = fallback;
    if (selected < 0)
        return 0;

    home = &input->planets[input->homeworld];
    planet = &input->planets[selected];
    planet->owner = input->player;
    planet->flags = (StarsU16)(planet->flags | GENERATE_WORLD_SCANNED_FLAG);
    for (attempt = 0; attempt < 100; ++attempt) {
        if (input->desirability(input->context, planet, input->player) > 9)
            break;
        for (environment = 0; environment < 3; ++environment) {
            value = (StarsI16)(generate_world_special_random(input,
                GENERATE_WORLD_SPECIAL_ENVIRONMENT_RETRY_LIMIT) + 2);
            planet->environment[environment] = value;
            planet->environment_original[environment] = value;
        }
    }
    if (attempt == 100) {
        for (environment = 0; environment < 3; ++environment) {
            planet->environment[environment] = home->environment[environment];
            planet->environment_original[environment] =
                home->environment_original[environment];
        }
    }
    /* The original divides the homeworld's population between the two
     * planets using the fourth fixed-point resource slot. */
    planet->population = home->population * 2L / 5L;
    home->population = home->population * 4L / 5L;
    planet->mine_count = 10;
    planet->factory_count = 4;
    planet->defense_count = 0;
    planet->production_slot = 1;
    for (mineral = 0; mineral < 3; ++mineral) {
        planet->mineral_rate[mineral] =
            (StarsI32)(generate_world_special_random(input, 200) + 100);
    }
    input->create_ship(input->context, input->player, selected, 0, 0);
    *input->special_planet = selected;
    return 1;
}

StarsI16 stars_generate_world_write_files(StarsGenerateWorldFileInput *input)
{
    StarsI16 i;
    StarsI16 player;
    StarsI16 previous_x;
    StarsI16 player_count;
    StarsU32 packed;

    if (!input || !input->layout || !input->base_name ||
        !input->game_state || !input->open_file ||
        !input->write_record || !input->close_stream ||
        !input->write_data_file || input->layout->point_count < 1 ||
        input->layout->point_count > GENERATE_WORLD_MAX_PLANETS || input->player_count < 0 ||
        input->player_count > 16)
        return 0;
    if (!input->open_file(input->context, input->base_name))
        return 0;
    if (!input->write_record(input->context, 7, 0x40,
            input->game_state, 0)) {
        input->close_stream(input->context);
        return 0;
    }

    previous_x = 1000;
    for (i = 0; i < input->layout->point_count; ++i) {
        packed = ((StarsU32)input->layout->points[i].y & 0x3ffUL) << 10;
        packed |= ((StarsU32)input->layout->point_ids[i] & 0x3ffUL) << 22;
        packed |= ((StarsU32)(input->layout->points[i].x - previous_x) &
            0x3ffUL);
        if (!input->write_record(input->context, 0, 0, &packed, 4)) {
            input->close_stream(input->context);
            return 0;
        }
        previous_x = input->layout->points[i].x;
    }
    player_count = input->player_count;
    if (!input->write_record(input->context, 0, 2, &player_count, 0)) {
        input->close_stream(input->context);
        return 0;
    }
    input->close_stream(input->context);
    for (player = -1; player < input->player_count; ++player) {
        if (!input->write_data_file(input->context, input->base_name,
                player, 0))
            return 0;
    }
    return 1;
}

StarsI16 stars_generate_world_finalize(
    StarsGenerateWorldFinalizeInput *input)
{
    if (!input)
        return 0;
    if (input->batch_mode)
        return 1;
    if (!input->set_player || !input->create_windows)
        return 0;
    if (!input->hosted_game) {
        if (!input->reset_message_log)
            return 0;
        input->set_player(input->context, -1);
        input->reset_message_log(input->context);
        input->create_windows(input->context);
        return 1;
    }
    if (!input->destroy_game || !input->format_load_name ||
        !input->load_game || !input->send_message ||
        !input->base_name || !input->load_name ||
        input->load_name_capacity < 1 || input->selected_player < 0)
        return 0;
    input->destroy_game(input->context);
    if (!input->format_load_name(input->context,
            (StarsI16)(input->selected_player + 1), input->load_name,
            input->load_name_capacity))
        return 0;
    if (!input->load_game(input->context, input->base_name,
            input->load_name))
        return 0;
    input->set_player(input->context, input->selected_player);
    input->create_windows(input->context);
    input->send_message(input->context, 0x111, (StarsI16)0xfa1, 0);
    return 1;
}

StarsI16 stars_generate_world_init_battle_plans(
    StarsGenerateWorldBattlePlansInput *input)
{
    StarsI16 player;
    StarsI16 plan;
    StarsI16 mineral;
    StarsU8 *player_plans;

    if (!input || !input->plan_storage || !input->plan_counts ||
        !input->init_plan || input->player_count < 1 ||
        input->player_count > 16 || input->plan_size < 1 ||
        input->plan_stride < (StarsI16)(input->plan_size * 5))
        return 0;
    for (player = 0; player < input->player_count; ++player) {
        player_plans = input->plan_storage +
            (StarsI32)player * input->plan_stride;
        input->plan_counts[player] = 5;
        for (plan = 0; plan < 5; ++plan)
            input->init_plan(input->context,
                player_plans + plan * input->plan_size,
                plan, player);
    }
    /* GenerateWorld discards the provisional mineral rates on the first
     * unowned planet before the generated game is written. */
    if (input->planets && input->planet_count > 0 &&
            input->planets[0].owner == -1) {
        for (mineral = 0; mineral < 3; ++mineral)
            input->planets[0].mineral_rate[mineral] = 0L;
    }
    return 1;
}

static StarsI16 generate_world_wormhole_random(
    const StarsGenerateWorldWormholesInput *input, StarsI16 limit)
{
    if (!input->random_value || limit <= 0)
        return 0;
    return input->random_value(input->context, limit);
}

StarsI16 stars_generate_world_create_wormholes(
    StarsGenerateWorldWormholesInput *input)
{
    StarsI16 pair;
    StarsI16 endpoint;
    StarsI16 attempt;
    StarsI16 score;
    StarsI16 best_score;
    StarsI16 first;
    StarsI16 second;
    StarsPoint point;
    StarsPoint best_point;

    if (!input || !input->random_value || !input->create_wormhole ||
        !input->set_point || !input->validate_wormhole ||
        !input->link_wormholes || input->pair_count < 0 ||
        input->pair_count > 100 || input->galaxy_extent <= 0 ||
        input->max_attempts < 1)
        return 0;
    for (pair = 0; pair < input->pair_count; ++pair) {
        first = input->create_wormhole(input->context,
            generate_world_wormhole_random(input, 3));
        second = input->create_wormhole(input->context,
            generate_world_wormhole_random(input, 3));
        if (first < 0 || second < 0 ||
            !input->link_wormholes(input->context, first, second))
            return 0;
        for (endpoint = 0; endpoint < 2; ++endpoint) {
            best_score = 0x10;
            for (attempt = 0; attempt < input->max_attempts; ++attempt) {
                point.x = (StarsI16)(generate_world_wormhole_random(input,
                    input->galaxy_extent) + 1000);
                point.y = (StarsI16)(generate_world_wormhole_random(input,
                    input->galaxy_extent) + 1000);
                input->set_point(input->context,
                    endpoint == 0 ? first : second, point);
                score = input->validate_wormhole(input->context,
                    endpoint == 0 ? first : second);
                if (score == 0)
                    break;
                if (score < best_score) {
                    best_score = score;
                    best_point = point;
                }
            }
            if (score != 0) {
                if (best_score == 0x10)
                    return 0;
                input->set_point(input->context,
                    endpoint == 0 ? first : second, best_point);
            }
        }
    }
    return 1;
}

StarsI16 stars_generate_world_finalize_player_status(
    StarsGenerateWorldPlayerStatusInput *input)
{
    StarsI16 player;
    StarsI16 recipient;
    StarsI16 selected;
    StarsI16 eligible_count;
    StarsI16 single_player_mode;
    StarsGenerateWorldPlayer *current;

    if (!input || !input->players || !input->send_message ||
        !input->selected_player || !input->single_player_mode ||
        input->player_count < 1 || input->player_count > 16)
        return 0;
    for (player = 0; player < input->player_count; ++player) {
        current = &input->players[player];
        if (!current->is_dead)
            continue;
        input->send_message(input->context, player, 0x117, -1, 0);
        for (recipient = 0; recipient < input->player_count; ++recipient) {
            if (recipient != player && !input->players[recipient].is_computer)
                input->send_message(input->context, recipient, 0x182,
                    player, 0);
        }
    }
    selected = -1;
    eligible_count = 0;
    for (player = 0; player < input->player_count; ++player) {
        current = &input->players[player];
        if (!current->is_computer || current->computer_type == 7) {
            ++eligible_count;
            if (selected < 0)
                selected = player;
        } else {
            current->salt = 0x094dabeeL;
        }
    }
    single_player_mode = eligible_count == 1 ? 1 : 0;
    if (single_player_mode) {
        for (player = 0; player < input->player_count; ++player) {
            for (recipient = 0; recipient < input->player_count;
                 ++recipient) {
                if (player != recipient)
                    input->players[player].relations[recipient] = 2;
            }
        }
    }
    *input->selected_player = selected;
    *input->single_player_mode = single_player_mode;
    return 1;
}

StarsI16 stars_generate_world_pipeline(StarsGenerateWorldPipelineInput *input)
{
    StarsI16 index;

    if (!input || !input->layout || !input->planets || !input->starts ||
        !input->players || !input->homeworlds || !input->startup_ships ||
        !input->battle_plans || !input->wormholes ||
        !input->player_status || !input->files || !input->finalize ||
        input->special_planet_count < 0)
        return 0;

    if (!stars_generate_world_layout(input->layout) ||
        !stars_generate_world_init_planets(input->planets))
        return 0;

    /* The original keeps coordinates in the shared point table while the
     * typed model exposes them on each planet for later semantic callbacks.
     * Materialize that relationship at the same phase boundary, after the
     * layout has been compacted and before starts/homeworlds are consumed. */
    if (input->planets->planet_count > input->layout->layout->point_count)
        return 0;
    for (index = 0; index < input->planets->planet_count; ++index)
        input->planets->planets[index].point =
            input->layout->layout->points[index];

    if (!stars_generate_world_assign_starts(input->starts) ||
        !stars_generate_world_init_players(input->players) ||
        !stars_generate_world_assign_homeworlds(input->homeworlds) ||
        !stars_generate_world_create_startup_ships(input->startup_ships))
        return 0;

    if (input->special_planet_count > 0 && !input->special_planets)
        return 0;
    for (index = 0; index < input->special_planet_count; ++index) {
        if (!stars_generate_world_create_special_planet(
                &input->special_planets[index]))
            return 0;
    }

    if (!stars_generate_world_init_battle_plans(input->battle_plans) ||
        !stars_generate_world_create_wormholes(input->wormholes) ||
        !stars_generate_world_finalize_player_status(input->player_status) ||
        !stars_generate_world_write_files(input->files) ||
        !stars_generate_world_finalize(input->finalize))
        return 0;
    return 1;
}
