#include "core.h"
#include <string.h>

#define ESTIMATE_PROD_MAX_TURNS 100
#define ESTIMATE_PROD_RESOURCE_SHIFT 23
#define ESTIMATE_PROD_ITEM_MASK 0x3fcU
#define ESTIMATE_PROD_SPECIAL_KIND 3
#define ESTIMATE_PROD_SPECIAL_GROUP 1

static StarsI16 estimate_prod_is_special(const StarsEstimateProdItem *item)
{
    return (StarsI16)(((item->packed >> 10) & 0x7fUL) ==
        ESTIMATE_PROD_SPECIAL_KIND &&
        ((item->packed >> 17) & 7UL) == ESTIMATE_PROD_SPECIAL_GROUP);
}

static StarsI32 estimate_prod_subtract_research(
    const StarsEstimateProdPlanet *planet, StarsI32 *resources)
{
    StarsI32 reduction;

    if ((planet->flags & (1UL << ESTIMATE_PROD_RESOURCE_SHIFT)) != 0U)
        return 0;
    reduction = (StarsI32)(((StarsI32)*resources *
        planet->research_percent) / 100L);
    *resources -= reduction;
    return reduction;
}

/* Genuine source reconstruction of EstimateItemProdSched, NE 1B:4F40-548F.
 * Each simulated year refreshes minerals/resources, feeds the partial queue
 * item and then the production queue to the build service, records the
 * requested item's first/last completion turns, and advances population only
 * after the queue has consumed that turn's resources. */
void stars_estimate_item_prod_schedule(StarsEstimateProdInput *input)
{
    StarsI32 mined[3], budget[4];
    StarsI16 turn, queue_index, phase, alchemy, result, lane;
    StarsI32 research_reduction;
    StarsU16 kind, group;
    StarsEstimateProdItem partial_item;
    StarsEstimateProdItem queue[255];
    StarsEstimateProdItem *item;
    StarsEstimateProdPlanet planet;

    if (!input || !input->planet || !input->first_turn || !input->last_turn)
        return;
    *input->first_turn = *input->last_turn = 0;
    if (input->queue_count > 255U || (input->queue_count && !input->queue))
        return;
    planet = *input->planet;
    /* Original owns a simulation copy of the queue and starts with no
     * partial item. Neither the caller's queue nor planet is modified. */
    if (input->queue_count)
        memcpy(queue, input->queue, input->queue_count * sizeof queue[0]);
    memset(&partial_item, 0, sizeof partial_item);
    for (turn = 1; turn <= ESTIMATE_PROD_MAX_TURNS; ++turn) {
        mined[0] = mined[1] = mined[2] = 0;
        if (input->estimate_mined)
            input->estimate_mined(input->context, &planet, mined);
        for (lane = 0; lane < 3; ++lane) {
            planet.minerals[lane] += mined[lane];
            budget[lane] = planet.minerals[lane];
        }
        budget[3] = input->resources_at_planet ?
            input->resources_at_planet(input->context, &planet) : 0;
        research_reduction = estimate_prod_subtract_research(&planet, &budget[3]);
        planet.resources = budget[3];
        alchemy = 0;
        for (queue_index = -1;
                queue_index < (StarsI16)input->queue_count; ++queue_index) {
            item = queue_index < 0 ? &partial_item : &queue[queue_index];
            if (item->quantity == 0U) continue;
            if (estimate_prod_is_special(item)) {
                if (queue_index < (StarsI16)input->queue_count - 1) {
                    if (queue_index == input->item_index) {
                        *input->first_turn = *input->last_turn = -1;
                        return;
                    }
                    alchemy = 1;
                    continue;
                }
                item->quantity = ESTIMATE_PROD_ITEM_MASK;
                item->packed = (item->packed & ~1023UL) | ESTIMATE_PROD_ITEM_MASK;
            }
            phase = 0;
            if (input->build_item_mutable) {
                result = input->build_item_mutable(input->context, &planet,
                    item, queue_index < 0 ? 0 : &partial_item,
                    budget, alchemy, &phase);
                planet.resources = budget[3];
            } else {
                /* Legacy callback exposes mineral inputs read-only. Its
                 * resource result is published through planet.resources. */
                result = input->build_item ? input->build_item(input->context,
                    &planet, item, queue_index < 0 ? 0 : &partial_item,
                    budget, alchemy, &phase) : 0;
                budget[3] = planet.resources;
            }
            if (queue_index == input->item_index) {
                if (result > 0 && *input->first_turn == 0)
                    *input->first_turn = turn;
                if (phase == 2) {
                    *input->last_turn = *input->first_turn ? (StarsI16)(turn - 1) : 0;
                    return;
                }
                if (phase == 0 || phase == 1) {
                    *input->last_turn = turn;
                    return;
                }
            }
            alchemy = 0;
            kind = (StarsU16)((item->packed >> 10) & 127UL);
            group = (StarsU16)((item->packed >> 17) & 7UL);
            if (group == 1U) {
                if (kind == 0U || kind == 8U)
                    planet.mines = (StarsU16)((planet.mines + result) & 4095U);
                else if (kind == 1U || kind == 7U)
                    planet.factories = (StarsU16)((planet.factories + result) & 4095U);
            }
            if (phase > 4) break;
        }
        if (input->item_index < 0) {
            *input->first_turn = (StarsI16)budget[3];
            if (input->item_index == -1)
                *input->first_turn = (StarsI16)(*input->first_turn + research_reduction);
            return;
        }
        for (lane = 0; lane < 3; ++lane) planet.minerals[lane] = budget[lane];
        if (input->change_population)
            input->change_population(input->context, &planet, 1);
    }
    if (*input->first_turn == 0) *input->first_turn = ESTIMATE_PROD_MAX_TURNS;
    *input->last_turn = ESTIMATE_PROD_MAX_TURNS;
}
