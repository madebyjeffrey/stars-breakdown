#include <string.h>
#include "core.h"

#define BUILD_STATUS_NONE 0
#define BUILD_STATUS_COMPLETE 0
#define BUILD_STATUS_ALCHEMY 3
#define BUILD_STATUS_BLOCKED 4
#define BUILD_STATUS_RESOURCES 5
#define BUILD_STATUS_IDLE 6
#define BUILD_STATUS_SPECIAL 7
#define BUILD_MESSAGE_BUILT 0x8c
#define BUILD_TYPE_MINE 0
#define BUILD_TYPE_FACTORY 1
#define BUILD_TYPE_DEFENSE 2
#define BUILD_TYPE_TERRAFORM 4

static StarsI32 build_cost_at_percent(StarsI32 cost, StarsI16 percent)
{
    StarsI32 quotient;
    StarsI32 remainder;

    if (cost <= 0 || percent <= 0)
        return 0;
    if (percent > 100)
        percent = 100;
    quotient = cost / 100L;
    remainder = cost % 100L;
    return quotient * percent + (remainder * percent) / 100L;
}

static StarsI16 build_has_separate_partial(const StarsProduceItem *item)
{
    /* Raw CBuildProdItem uses iStack20 for object-kind 1 and item types < 7. */
    return (StarsI16)(item->object_kind == 1 && item->type >= 0 &&
        item->type < 7);
}

static StarsI16 build_max_progress(const StarsI32 resources[4],
    const StarsI32 costs[4], const StarsI32 paid[4], StarsI16 old_progress)
{
    StarsI16 index;
    StarsI16 progress;
    StarsI32 available;
    StarsI32 lower;
    StarsI32 next;
    StarsI32 candidate;

    progress = 100;
    for (index = 0; index < 4; ++index) {
        if (costs[index] <= 0)
            continue;
        available = resources[index] + paid[index];
        if (available < 0)
            available = 0;
        if (available >= costs[index])
            continue;
        lower = (available * 100L) / costs[index];
        next = ((available + 1L) * 100L) / costs[index];
        candidate = lower;
        if (candidate <= next - 1L)
            candidate = next - 1L;
        if (candidate < progress)
            progress = (StarsI16)candidate;
    }
    if (progress < old_progress)
        progress = old_progress;
    if (progress > 100)
        progress = 100;
    return progress;
}

static StarsI16 build_capacity(const StarsBuildProductionOps *ops,
    const StarsProducePlanet *planet, const StarsProduceItem *item)
{
    if (ops->capacity)
        return ops->capacity(ops->context, planet, item->type, 1);
    return item->quantity;
}

StarsI16 stars_build_production_item(StarsBuildProductionInput *input)
{
    const StarsBuildProductionOps *ops;
    StarsI32 costs[4];
    StarsI32 minerals[3];
    StarsI32 possible;
    StarsI32 built;
    StarsI32 needed[4];
    StarsI32 paid[4];
    StarsI32 updated_paid[4];
    StarsI32 original_resources[4];
    StarsI16 old_progress;
    StarsI16 progress;
    StarsI16 new_progress;
    StarsI16 can_build;
    StarsI16 index;
    StarsI16 capacity;
    StarsI16 status;

    if (!input || !input->planet || !input->item || !input->ops)
        return 0;
    ops = input->ops;
    for (index = 0; index < 4; ++index)
        original_resources[index] = input->resources[index];
    if (ops->costs && !ops->costs(ops->context, input->planet, input->item,
            costs))
        return 0;
    else if (!ops->costs)
        for (index = 0; index < 4; ++index)
            costs[index] = input->item->cost[index];
    if (input->item->special == BUILD_TYPE_TERRAFORM && ops->terraform &&
            ops->terraform(ops->context, input->planet) <= 0) {
        status = BUILD_STATUS_BLOCKED;
        if (input->status) *input->status = status;
        return 0;
    }
    capacity = build_capacity(ops, input->planet, input->item);
    if (capacity <= 0) {
        status = BUILD_STATUS_BLOCKED;
        if (input->status) *input->status = status;
        return 0;
    }
    possible = capacity;
    if (possible > input->item->quantity)
        possible = input->item->quantity;
    if (possible < 0)
        possible = 0;
    built = 0;
    progress = input->item->progress;
    if (progress < 0) progress = 0;
    if (progress > 100) progress = 100;
    old_progress = progress;
    if (input->partial)
        memset(input->partial, 0, sizeof *input->partial);
    while (built < possible) {
        can_build = 1;
        for (index = 0; index < 4; ++index) {
            paid[index] = build_cost_at_percent(costs[index], progress);
            needed[index] = costs[index] - paid[index];
            if (needed[index] > input->resources[index])
                can_build = 0;
        }
        if (!can_build) {
            new_progress = build_max_progress(input->resources, costs,
                paid, progress);
            if (new_progress > progress) {
                for (index = 0; index < 4; ++index) {
                    updated_paid[index] = build_cost_at_percent(
                        costs[index], new_progress);
                    input->resources[index] -= updated_paid[index] -
                        paid[index];
                }
                progress = new_progress;
            }
            break;
        }
        for (index = 0; index < 4; ++index)
            input->resources[index] -= needed[index];
        minerals[0] = 0; minerals[1] = 0; minerals[2] = 0;
        if (ops->build_object && !ops->build_object(ops->context,
                input->planet, input->item->object_kind, input->item->type,
                1, minerals)) {
            for (index = 0; index < 4; ++index)
                input->resources[index] = original_resources[index];
            break;
        }
        ++built;
        --input->item->quantity;
        input->item->progress = 0;
        progress = 0;
    }
    if (build_has_separate_partial(input->item) && input->partial &&
            progress > 0 && input->item->quantity > 0) {
        *input->partial = *input->item;
        input->partial->quantity = 1;
        input->partial->partial = 1;
        input->partial->progress = progress;
        input->partial->active = 1;
    } else if (!build_has_separate_partial(input->item)) {
        input->item->progress = progress;
    }
    if (built == 0)
        status = progress != old_progress ? BUILD_STATUS_IDLE :
            BUILD_STATUS_SPECIAL;
    else if (input->item->quantity > 0)
        status = BUILD_STATUS_RESOURCES;
    else
        status = BUILD_STATUS_COMPLETE;
    if (input->alchemy)
        status = BUILD_STATUS_ALCHEMY;
    if (!input->calculate_only && built > 0 && ops->send_message)
        ops->send_message(ops->context, input->planet->owner,
            BUILD_MESSAGE_BUILT, input->planet->id, (StarsI16)built, status);
    if (input->status)
        *input->status = status;
    return (StarsI16)built;
}
