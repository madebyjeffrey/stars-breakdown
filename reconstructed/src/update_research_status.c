#include "core.h"

#define RESEARCH_TECH_COUNT 6
#define RESEARCH_MAX_LEVEL 25
#define RESEARCH_DEAD_AFTER_9 0x0002
#define RESEARCH_DEAD_AFTER_9B 0x0004
#define RESEARCH_HALF_COST 0x0002
#define RESEARCH_AUTO_MODE 6
#define RESEARCH_CHOOSE_LOWEST 7

#define RESEARCH_LEVEL_MESSAGE 0x0050
#define RESEARCH_LEVEL_MESSAGE_SPECIAL 0x0136
#define RESEARCH_POOL_MESSAGE 0x0159
#define RESEARCH_UNLOCK_NORMAL 0x005f
#define RESEARCH_UNLOCK_LARGE 0x0145
#define RESEARCH_UNLOCK_SMALL 0x0157
#define RESEARCH_UNLOCK_CLOAK 0x00d0
#define RESEARCH_UNLOCK_SCANNER 0x0078

static StarsU32 research_half_up(StarsU32 value)
{
    return (value + 1UL) / 2UL;
}

static void research_add(StarsU32 *destination, StarsU32 amount)
{
    if (destination)
        *destination += amount;
}

static StarsU16 research_lowest_technology(const StarsResearchPlayer *player)
{
    StarsU16 result;
    StarsU16 technology;
    if (!player)
        return 0;
    result = 0;
    for (technology = 1; technology < RESEARCH_TECH_COUNT; ++technology)
        if (player->technology_level[technology] <
                player->technology_level[result])
            result = technology;
    return result;
}

/* A level-up can expose several components.  The original walks each of the
 * sixteen technology-group bits and then each component in that group.  The
 * callback deliberately owns the component catalogue; this routine only
 * expresses the unlock policy and message selection. */
static void research_report_unlocks(StarsResearchInput *input,
    StarsResearchPlayer *player, StarsU16 technology, StarsU8 level,
    StarsU16 race_grbit)
{
    StarsU32 group_mask;
    StarsU16 group_index;
    StarsU16 part_index;
    StarsU8 requirements[RESEARCH_TECH_COUNT];
    StarsI16 found;
    StarsI16 message;
    StarsU16 argument;

    if (!input || !player || !input->ops || !input->ops->lookup_part)
        return;
    group_index = 0;
    for (group_mask = 1UL; group_mask != 0UL; group_mask <<= 1,
            ++group_index) {
        part_index = 0;
        for (;;) {
            found = input->ops->lookup_part(input->context,
                (StarsU16)group_mask, part_index, requirements);
            if (found == 0)
                break;
            if (found == 1 && requirements[technology] == level) {
                /* The alternate racial component families are not announced
                 * for the three duplicate slots in this technology group. */
                if ((group_mask == 0x2000UL && race_grbit != 0 &&
                        (part_index == 8 || part_index == 12 ||
                         part_index == 16))) {
                    ++part_index;
                    continue;
                }
                if (group_mask == 0x0400UL) {
                    message = RESEARCH_UNLOCK_CLOAK;
                    argument = (StarsU16)-3;
                } else if (group_mask == 0x4000UL) {
                    message = RESEARCH_UNLOCK_SCANNER;
                    argument = (StarsU16)-3;
                } else if (group_mask == 0x8000UL && part_index > 8 &&
                        part_index < 14) {
                    message = RESEARCH_UNLOCK_LARGE;
                    argument = (StarsU16)((group_index << 8) |
                        0xc000U | part_index);
                } else if (group_mask == 0x8000UL && part_index < 9) {
                    message = RESEARCH_UNLOCK_SMALL;
                    argument = (StarsU16)((group_index << 8) |
                        0xc000U | part_index);
                } else {
                    message = RESEARCH_UNLOCK_NORMAL;
                    argument = (StarsU16)((group_index << 8) |
                        0xc000U | part_index);
                }
                if (input->ops->send_message)
                    input->ops->send_message(input->context, player->id,
                        message, (StarsI16)argument, (StarsI16)technology,
                        (StarsI16)group_mask, (StarsI16)part_index);
            }
            ++part_index;
        }
    }
}

/* Apply one player's stored research.  The return value says whether the
 * caller should make another pass: completing a selected technology can move
 * leftover points to a new selection, and the original routine immediately
 * spends those points too. */
static StarsI16 research_player(StarsResearchInput *input,
    StarsResearchPlayer *player, StarsI16 original_use_pool,
    StarsU32 pool[RESEARCH_TECH_COUNT])
{
    StarsI16 race_grbit;
    StarsI16 selection_changed;
    StarsI16 repeat;
    StarsI16 technology;
    StarsI16 selected;
    StarsI16 next_level;
    StarsI16 message;
    StarsU32 available;
    StarsU32 cost;
    StarsU32 last_year_share;
    StarsU32 other_share;
    StarsU16 part_index;
    StarsI16 completed;

    if (!input || !player)
        return 0;
    race_grbit = input->ops && input->ops->race_grbit ?
        input->ops->race_grbit(input->context, player->id, 4) : 0;
    selected = (StarsI16)(player->current_technology & 0x0fU);
    if (selected >= RESEARCH_TECH_COUNT)
        selected = 0;
    repeat = 1;
    while (repeat) {
        repeat = 0;
        for (technology = 0; technology < RESEARCH_TECH_COUNT;
                ++technology) {
            available = player->research[technology];
            if ((input->game_flags & RESEARCH_HALF_COST) != 0)
                available <<= 1;
            selection_changed = 0;

            /* Racial research sharing uses last year's total only on the
             * currently selected technology.  Trait 1 also seeds the other
             * technologies with three twentieths of that total. */
            if (technology == selected && original_use_pool &&
                    race_grbit < 2) {
                if (race_grbit == 0) {
                    research_add(&pool[technology],
                        player->research_last_year);
                    available += player->research_last_year;
                } else {
                    last_year_share = research_half_up(
                        player->research_last_year);
                    available += last_year_share;
                    research_add(&pool[technology], last_year_share);
                    race_grbit = 2;
                    for (part_index = 0; part_index < RESEARCH_TECH_COUNT;
                            ++part_index) {
                        if (part_index == (StarsU16)technology)
                            continue;
                        other_share = (player->research_last_year * 3UL +
                            19UL) / 20UL;
                        if ((input->game_flags & RESEARCH_HALF_COST) != 0)
                            other_share = research_half_up(other_share);
                        research_add(&player->research[part_index],
                            other_share);
                        research_add(&pool[part_index], other_share);
                    }
                }
            }

            completed = 0;
            for (;;) {
                if (player->technology_level[technology] >
                        RESEARCH_MAX_LEVEL ||
                        (((player->dead_flags & RESEARCH_DEAD_AFTER_9) != 0 ||
                          (player->dead_flags & RESEARCH_DEAD_AFTER_9B) != 0) &&
                         player->technology_level[technology] > 9))
                    break;
                next_level = (StarsI16)(player->technology_level[technology] +
                    1);
                cost = input->ops && input->ops->level_cost ?
                    input->ops->level_cost(input->context, technology,
                        next_level, player->id) : 0UL;
                if (available < cost)
                    break;
                available -= cost;
                completed = 1;
                player->technology_level[technology] = (StarsU8)next_level;
                if (next_level == 26 && player->current_mode ==
                        RESEARCH_AUTO_MODE)
                    player->current_mode = RESEARCH_CHOOSE_LOWEST;
                selected = (StarsI16)(player->current_technology & 0x0fU);
                if (technology == selected && player->current_mode !=
                        RESEARCH_AUTO_MODE) {
                    if (player->current_mode == RESEARCH_CHOOSE_LOWEST)
                        selected = (StarsI16)research_lowest_technology(player);
                    else
                        selected = player->current_mode;
                    if (selected >= RESEARCH_TECH_COUNT)
                        selected = 0;
                    selection_changed = 1;
                }
                message = race_grbit == 0 ? RESEARCH_LEVEL_MESSAGE :
                    RESEARCH_LEVEL_MESSAGE_SPECIAL;
                if (input->ops && input->ops->send_message)
                    input->ops->send_message(input->context, player->id,
                        message, -2, next_level, technology, selected);
                research_report_unlocks(input, player, (StarsU16)technology,
                    (StarsU8)next_level, race_grbit);
                if (!((!original_use_pool && !selection_changed &&
                        player->current_mode != RESEARCH_CHOOSE_LOWEST) ||
                        player->current_mode == RESEARCH_AUTO_MODE ||
                        technology != selected))
                    break;
            }

            /* A completed level leaves its remainder in the newly selected
             * technology.  The old 16.16 code clears the completed slot first
             * and rounds here when the doubled-cost game option is active. */
            if (player->current_mode == RESEARCH_CHOOSE_LOWEST) {
                selected = (StarsI16)research_lowest_technology(player);
                player->current_technology = (StarsU8)((
                    player->current_technology & 0xf0U) | selected);
                player->research[technology] = 0UL;
                if ((input->game_flags & RESEARCH_HALF_COST) != 0)
                    available = research_half_up(available);
                player->research[selected] += available;
                available = 0UL;
            } else if (selection_changed) {
                player->current_mode = RESEARCH_AUTO_MODE;
                player->current_technology = (StarsU8)((
                    player->current_technology & 0xf0U) |
                    (StarsU8)selected);
                player->research[technology] = 0UL;
                if ((input->game_flags & RESEARCH_HALF_COST) != 0)
                    available = research_half_up(available);
                player->research[selected] += available;
                available = 0UL;
            } else {
                if ((input->game_flags & RESEARCH_HALF_COST) != 0)
                    available = research_half_up(available);
                player->research[technology] = available;
            }
            if (selection_changed || completed)
                repeat = 1;
        }
        original_use_pool = 0;
    }
    return repeat;
}

/* NE 18:80FE-8B6F. Spend each player's research, report every completed
 * technology and newly available component, then distribute qualifying
 * players' pooled research evenly among other qualifying players. */
void stars_update_research_status(StarsResearchInput *input)
{
    StarsU32 pool[RESEARCH_TECH_COUNT];
    StarsU16 player_index;
    StarsU16 technology;
    StarsU16 alive_count;
    StarsI16 use_pool;
    StarsI16 race_stat;
    StarsU32 share;
    StarsI16 distributed;

    if (!input || !input->players)
        return;
    use_pool = input->use_pool;
    for (technology = 0; technology < RESEARCH_TECH_COUNT; ++technology)
        pool[technology] = 0UL;
    alive_count = 0;
    for (player_index = 0; player_index < input->player_count;
            ++player_index) {
        if (input->players[player_index].dead_flags == 0)
            ++alive_count;
        research_player(input, &input->players[player_index], use_pool, pool);
    }

    /* The original accumulates the shared amounts separately from player
     * research.  The public adapter receives those amounts through the same
     * callback boundary, so a caller can provide the exact pool values when
     * it needs to exercise redistribution. */
    distributed = 0;
    if (use_pool && alive_count > 1 && input->ops &&
            input->ops->race_stat) {
        for (player_index = 0; player_index < input->player_count;
                ++player_index) {
            race_stat = input->ops->race_stat(input->context,
                input->players[player_index].id, 0x0e);
            if (race_stat != 1)
                continue;
            for (technology = 0; technology < RESEARCH_TECH_COUNT;
                    ++technology) {
                share = pool[technology] / alive_count / 2UL;
                if (share <= 1UL)
                    continue;
                distributed = 1;
                if (input->ops->send_pool_message)
                    input->ops->send_pool_message(input->context,
                        input->players[player_index].id, RESEARCH_POOL_MESSAGE,
                        (StarsI16)technology, (StarsI16)share);
                if ((input->game_flags & RESEARCH_HALF_COST) != 0)
                    share = research_half_up(share);
                input->players[player_index].research[technology] += share;
            }
        }
    }
    if (distributed) {
        input->use_pool = 0;
        stars_update_research_status(input);
    }
}
