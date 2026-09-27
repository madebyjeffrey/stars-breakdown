#include "core.h"
#include <string.h>

static StarsU16 delete_plan_word(const StarsU8 *data)
{
    return (StarsU16)(data[0] | ((StarsU16)data[1] << 8));
}

static void delete_plan_set_word(StarsU8 *data, StarsU16 value)
{
    data[0] = (StarsU8)value;
    data[1] = (StarsU8)(value >> 8);
}

/* Genuine C reconstruction of FDeleteBattlePlan, NE 31:1706-1929. */
StarsI16 stars_delete_battle_plan(StarsI16 plan, StarsI16 warn,
    StarsDeleteBattlePlanInput *input)
{
    StarsI16 warned;
    StarsI16 index;
    StarsI16 current_plan;
    StarsU8 old_count;
    StarsU8 *destination;
    const StarsU8 *source;
    StarsU16 word;
    StarsDeleteBattleFleet *fleet;
    const StarsU8 *message;

    if (!input || !input->fleet_at || !input->plan_count || !input->plans)
        return 0;
    warned = 0;
restart_scan:
    for (index = 0; index < input->fleet_count; ++index) {
        fleet = input->fleet_at(input->context, index);
        if (!fleet)
            break;
        if (fleet->player < input->current_player)
            continue;
        if (fleet->player > input->current_player)
            break;
        current_plan = (StarsI16)fleet->plan;
        if ((StarsU16)plan <= (StarsU16)current_plan) {
            if ((StarsU16)plan < (StarsU16)current_plan) {
                if (warn == 0)
                    fleet->plan = (StarsU8)(current_plan - 1);
                else
                    warned = 1;
            } else if (warn != 0) {
                message = input->format_ids ?
                    input->format_ids(input->context, 0x035bU) :
                    (const StarsU8 *)0;
                if (input->alert &&
                    input->alert(input->context, message, 0x31) == 2)
                    return 0;
                warn = 0;
                goto restart_scan;
            } else {
                fleet->plan = (StarsU8)(current_plan - 1);
            }
        }
    }
    if (warn != 0 && warned != 0) {
        warn = 0;
        goto restart_scan;
    }
    old_count = *input->plan_count;
    *input->plan_count = (StarsU8)(old_count - 1U);
    for (index = plan; (StarsU16)index < (StarsU16)*input->plan_count;
        ++index) {
        source = input->plans + (StarsU16)(index + 1) * 0x24U;
        destination = input->plans + (StarsU16)index * 0x24U;
        memcpy(destination, source, 0x24U);
        word = delete_plan_word(destination);
        word = (StarsU16)((word & 0xff0fU) |
            (((StarsU16)index & 0x000fU) << 4));
        delete_plan_set_word(destination, word);
    }
    return 1;
}
