#include "core.h"

#define INIT_PROD_QUANTITY_MASK 0x3ffUL
#define INIT_PROD_NUMBER_SHIFT 10
#define INIT_PROD_CLASS_SHIFT 17
#define INIT_PROD_CLASS_STANDARD 1
#define INIT_PROD_NUMBER_FACTORY 7
#define INIT_PROD_NUMBER_MINE 8
#define INIT_PROD_NUMBER_DEFENSE 9
#define INIT_PROD_NUMBER_ALCHEMY 11
#define INIT_PROD_NUMBER_TERRAFORM 12
#define INIT_PROD_NUMBER_RESEARCH_BASE 0
#define INIT_PROD_MAX_ITEMS 128
#define INIT_PROD_MAX_QUANTITY 0x3fc

static StarsU32 init_prod_pack(StarsU16 number, StarsU16 item_class,
    StarsU16 quantity)
{
    return ((StarsU32)quantity & INIT_PROD_QUANTITY_MASK) |
        ((StarsU32)number << INIT_PROD_NUMBER_SHIFT) |
        ((StarsU32)item_class << INIT_PROD_CLASS_SHIFT);
}

static StarsU16 init_prod_number(StarsU32 item)
{
    return (StarsU16)((item >> INIT_PROD_NUMBER_SHIFT) & 0x7fUL);
}

static StarsU16 init_prod_class(StarsU32 item)
{
    return (StarsU16)((item >> INIT_PROD_CLASS_SHIFT) & 7UL);
}

static StarsU16 init_prod_quantity(StarsU32 item)
{
    return (StarsU16)(item & INIT_PROD_QUANTITY_MASK);
}

static StarsU32 init_prod_set_quantity(StarsU32 item, StarsU16 quantity)
{
    return (item & ~INIT_PROD_QUANTITY_MASK) |
        ((StarsU32)quantity & INIT_PROD_QUANTITY_MASK);
}

static StarsI16 init_prod_find(const StarsProductionList *list,
    StarsU32 item)
{
    StarsI16 index;
    StarsU16 number;
    StarsU16 item_class;

    number = init_prod_number(item);
    item_class = init_prod_class(item);
    for (index = 0; index < (StarsI16)list->item_count; ++index)
        if (init_prod_number(list->items[index]) == number &&
                init_prod_class(list->items[index]) == item_class)
            return index;
    return -1;
}

static void init_prod_add(StarsProductionList *list, StarsU32 item)
{
    if (list->item_count < INIT_PROD_MAX_ITEMS)
        list->items[list->item_count++] = item;
}

static void init_prod_add_capacity(StarsProductionList *list,
    StarsU16 current, StarsU16 maximum, StarsU16 number)
{
    StarsI16 difference;

    difference = (StarsI16)maximum - (StarsI16)current;
    if (difference > 0) {
        if (difference > INIT_PROD_MAX_QUANTITY)
            difference = INIT_PROD_MAX_QUANTITY;
        init_prod_add(list, init_prod_pack(number, INIT_PROD_CLASS_STANDARD,
            (StarsU16)difference));
    }
}

/* Reconcile in queue order. Duplicate orders remain separate and consume
 * the remaining finite availability; quantity 0x3ff denotes unlimited stock.
 * Keep progress/flag bits from the queued word, not the catalog entry. */
static void init_prod_reconcile(StarsProductionList *available,
    StarsProductionList *queue)
{
    StarsProductionList old_queue;
    StarsI16 index;
    StarsI16 found;
    StarsU16 quantity;
    StarsU16 remaining;
    StarsU32 item;

    old_queue = *queue;
    queue->item_count = 0;
    for (index = 0; index < (StarsI16)old_queue.item_count; ++index) {
        item = old_queue.items[index];
        found = init_prod_find(available, item);
        if (found < 0) continue;
        remaining = init_prod_quantity(available->items[found]);
        quantity = init_prod_quantity(item);
        if (quantity > remaining) quantity = remaining;
        queue->items[queue->item_count++] = init_prod_set_quantity(item,
            quantity);
        if (remaining != 0x3ffU)
            available->items[found] = init_prod_set_quantity(
                available->items[found], (StarsU16)(remaining - quantity));
    }
}

void stars_init_production(StarsInitProductionInput *input)
{
    StarsU16 index;
    StarsU16 research;

    if (!input || !input->available || !input->queue)
        return;
    input->available->item_count = 0;
    for (index = 0; index < input->ship_item_count; ++index)
        init_prod_add(input->available, input->ship_items[index]);

    for (index = 0; index < input->starbase_item_count; ++index)
        init_prod_add(input->available, input->starbase_items[index]);
    if (input->planetary_part_14_lookup == 1)
        init_prod_add(input->available, init_prod_pack(13, 1, 1));
    /* The legacy warp_gate_count input represents positive IWarpMAFromLppl:
     * all four packet choices are offered, not a single gate item. */
    if (input->warp_gate_count != 0)
        for (index = 14; index < 18; ++index)
            init_prod_add(input->available, init_prod_pack(index, 1, 0x3ff));
    init_prod_add_capacity(input->available, input->factories_current,
        input->factories_max, INIT_PROD_NUMBER_FACTORY);
    init_prod_add_capacity(input->available, input->mines_current,
        input->mines_max, INIT_PROD_NUMBER_MINE);
    if (input->defenses_max > input->defenses_current)
        init_prod_add(input->available, init_prod_pack(INIT_PROD_NUMBER_DEFENSE,
            1, (StarsU16)(input->defenses_max - input->defenses_current)));
    init_prod_add(input->available, init_prod_pack(INIT_PROD_NUMBER_ALCHEMY,
        1, 0x3ff));
    if (input->scanner_type == 31 && input->race_stat != 8)
        init_prod_add(input->available, init_prod_pack(27, 1, 1));
    if ((StarsI16)input->terraform_count > 0)
        init_prod_add(input->available, init_prod_pack(INIT_PROD_NUMBER_TERRAFORM,
            INIT_PROD_CLASS_STANDARD, input->terraform_count));

    /* Original automatic planetary choices depend on primary race. */
    for (research = 0; research < 7; ++research) {
        if (input->race_stat == 8 && research < 3) continue;
        if (input->race_stat == 3 && (research == 4 || research == 5)) continue;
        init_prod_add(input->available, init_prod_pack(
            INIT_PROD_NUMBER_RESEARCH_BASE + research, 1, 0x3ff));
    }

    init_prod_reconcile(input->available, input->queue);
    if ((input->game_flags & 0x0800U) != 0 &&
            input->tutorial_player == 0 && input->advance_tutor)
        input->advance_tutor(input->context);
}
