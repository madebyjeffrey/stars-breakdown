#include "core.h"

#define TECH_LEARN_DEAD 0x0008
#define TECH_LEARN_RACE_STAT 0x0e
#define TECH_LEARN_TRADER_MESSAGE_BASE 0x2f
#define TECH_LEARN_RESEARCH_MESSAGE 0xfffe

/* NE 31:9918-9C0D. Occasionally award an available technology-trader part;
 * otherwise select a battle technology that can advance and add its next
 * level's cost to research. A completed event sets the player's event bit
 * and returns a signed technology number: negative for a trader part and
 * positive for research. */
StarsI16 stars_itech_learn_a_tech(StarsTechLearnInput *input)
{
    StarsI16 attempt;
    StarsI16 technology;
    StarsI16 message;
    StarsI16 next_level;
    StarsI32 cost;
    StarsU32 mask;
    StarsU16 part_id;

    if (!input || !input->player || !input->random)
        return 0;
    if ((input->player->dead_flags & TECH_LEARN_DEAD) != 0 ||
            input->random(input->context, 100) <= 0x31)
        return 0;
    for (attempt = 0; attempt < 13; ++attempt) {
        technology = input->random(input->context, 13);
        if (technology < 0 || technology >= 13 ||
                input->trader_chance[technology] == 0 ||
                (input->player->trader_flags & ((StarsU32)1UL << technology)) != 0 ||
                input->random(input->context, 100) >=
                    input->trader_chance[technology])
            continue;
        mask = (StarsU32)1UL << technology;
        part_id = 0;
        message = input->give_trader_part ?
            input->give_trader_part(input->context, mask, input->player_id,
                &part_id) : 0;
        if (input->message_id != -1) {
            if (input->send_trader_message)
                input->send_trader_message(input->context, input->player_id,
                    (StarsI16)(message + TECH_LEARN_TRADER_MESSAGE_BASE),
                    part_id, input->x, input->y);
        } else if (input->goto_id)
            *input->goto_id = part_id;
        if (input->mark_event)
            input->mark_event(input->context, input->player_id);
        return (StarsI16)(-(technology + 1));
    }
    for (attempt = 0; attempt < 6; ++attempt) {
        technology = input->random(input->context, 6);
        if (technology < 0 || technology >= 6 ||
                input->player->tech_level[technology] >=
                    input->battle_threshold[technology])
            continue;
        next_level = (StarsI16)(input->player->tech_level[technology] + 1);
        cost = input->tech_level_cost ? input->tech_level_cost(input->context,
            technology, next_level, input->player_id) : 0L;
        if (input->half_cost)
            cost >>= 1;
        input->player->research[technology] += (StarsU32)cost;
        if (input->message_id != -1) {
            if (input->send_research_message)
                input->send_research_message(input->context, input->player_id,
                    input->message_id, input->x, input->y, technology,
                    input->half_cost ? (StarsI32)(cost << 1) : cost);
        } else if (input->goto_id)
            *input->goto_id = 0xfffe;
        if (input->mark_event)
            input->mark_event(input->context, input->player_id);
        return (StarsI16)(technology + 1);
    }
    return 0;
}
