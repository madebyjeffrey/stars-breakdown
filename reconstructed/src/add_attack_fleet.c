#include "core.h"

#define ATTACK_FLEET_CAPACITY_GATE 100
#define ATTACK_FLEET_HIGH_PRESSURE 90
#define ATTACK_FLEET_NORMAL_PRESSURE 60
#define ATTACK_FLEET_BATTLE_ROLL 0x33
#define ATTACK_FLEET_STARBASE_ROLL 0x1a
#define ATTACK_FLEET_OPTIONAL_LARGE_ROLL 0x4b
#define ATTACK_FLEET_OPTIONAL_SMALL_ROLL 0x32
#define ATTACK_FLEET_HIDDEN 0x0200

/* NE 22:4EBA-51A0. Choose the next AI attack design after a capacity-based
 * probability gate.  Battle designs enqueue their two guaranteed weapons
 * and then independently consider the two optional components.  The original
 * routine receives an attack-strength argument but does not read it; the
 * queue entries therefore depend only on capacity pressure, the selected
 * designs, and the random rolls.  The return value identifies which branch
 * supplied the queued item (zero means the capacity gate rejected the turn).
 */
StarsI16 stars_add_attack_fleet(const StarsAttackFleetInput *input)
{
    StarsI16 first_roll;
    StarsI16 second_roll;
    StarsI16 mine_room;
    StarsI16 factory_room;
    StarsI16 threshold;
    StarsU16 index;
    StarsU16 design_base;

    if (!input || !input->random || !input->add_to_queue)
        return 0;
    first_roll = input->random(input->context, 100);
    mine_room = input->planet ? (StarsI16)(input->planet->max_mines -
        input->planet->operating_mines) : 0;
    factory_room = input->planet ? (StarsI16)(input->planet->max_factories -
        input->planet->operating_factories) : 0;
    if (mine_room < 0) mine_room = 0;
    if (factory_room < 0) factory_room = 0;
    second_roll = input->random(input->context, 100);
    threshold = (mine_room < ATTACK_FLEET_CAPACITY_GATE ||
            factory_room < ATTACK_FLEET_CAPACITY_GATE) ?
        ATTACK_FLEET_HIGH_PRESSURE : ATTACK_FLEET_NORMAL_PRESSURE;
    if (second_roll < threshold)
        return 0;
    if (input->best_battle < 0 || first_roll < ATTACK_FLEET_BATTLE_ROLL) {
        if (input->best_starbase_defender < 0 ||
                first_roll < ATTACK_FLEET_STARBASE_ROLL) {
            if (input->best_destroyer < 0)
                return 0;
            input->add_to_queue(input->context, input->best_destroyer,
                1, 2, 1);
            return 3;
        }
        input->add_to_queue(input->context, input->best_starbase_defender,
            1, 2, 1);
        return 2;
    }
    design_base = (StarsU16)(input->best_battle * 4 + 6);
    if (input->battle_design_flags &&
            (StarsU16)(design_base + 1U) < input->battle_design_count) {
        for (index = 0; index < 2U; ++index)
            if ((input->battle_design_flags[design_base + index] &
                    ATTACK_FLEET_HIDDEN) == 0U)
                input->add_to_queue(input->context,
                    (StarsI16)(design_base + index), 2, 2, 1);
        if ((StarsU16)(design_base + 2U) < input->battle_design_count &&
                (input->battle_design_flags[design_base + 2U] &
                    ATTACK_FLEET_HIDDEN) == 0U &&
                input->random(input->context, 100) <
                    ATTACK_FLEET_OPTIONAL_LARGE_ROLL)
            input->add_to_queue(input->context,
                (StarsI16)(design_base + 2U), 1, 2, 1);
        if ((StarsU16)(design_base + 3U) < input->battle_design_count &&
                (input->battle_design_flags[design_base + 3U] &
                    ATTACK_FLEET_HIDDEN) == 0U &&
                input->random(input->context, 100) <
                    ATTACK_FLEET_OPTIONAL_SMALL_ROLL)
            input->add_to_queue(input->context,
                (StarsI16)(design_base + 3U), 1, 2, 1);
    }
    return 1;
}
