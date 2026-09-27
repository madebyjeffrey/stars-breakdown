#include "core.h"
#include <stdio.h>
#include <string.h>

static void probe_advance(void *context)
{
    int *count;
    count = (int *)context;
    *count += 1;
}

static StarsU32 probe_item(StarsU16 number, StarsU16 item_class,
    StarsU16 quantity)
{
    return ((StarsU32)quantity & 0x3ffUL) |
        ((StarsU32)number << 10) | ((StarsU32)item_class << 17);
}

int main(void)
{
    StarsProductionList available;
    StarsProductionList queue;
    StarsInitProductionInput input;
    StarsU32 ships[2];
    int advanced;
    int index;
    StarsU32 bases[1];

    memset(&available, 0, sizeof(available));
    memset(&queue, 0, sizeof(queue));
    advanced = 0;
    ships[0] = probe_item(3, 1, 12);
    queue.item_count = 1;
    queue.items[0] = probe_item(3, 1, 4);
    memset(&input, 0, sizeof(input));
    input.available = &available;
    input.queue = &queue;
    input.ship_items = ships;
    input.ship_item_count = 1;
    input.factories_current = 2;
    input.factories_max = 5;
    input.mines_current = 1;
    input.mines_max = 3;
    input.defenses_current = 0;
    input.defenses_max = 1;
    input.starbase_present = 1;
    input.starbase_cargo_capacity = 100;
    input.warp_gate_count = 1;
    input.terraform_count = 2;
    input.research_mask = 0x03;
    input.tutorial_player = 0;
    input.game_flags = 0x0800;
    input.advance_tutor = probe_advance;
    input.context = &advanced;
    stars_init_production(&input);
    if (available.item_count != 17 || queue.item_count != 1 ||
            (queue.items[0] & 0x3ffUL) != 4 || advanced != 1)
        return 1;
    if (available.items[0] != probe_item(3, 1, 8)) return 1;
    for (index = 0; index < 4; ++index)
        if (available.items[index + 1] != probe_item(
                (StarsU16)(14 + index), 1, 0x3ff)) return 1;
    if (available.items[5] != probe_item(7, 1, 3) ||
            available.items[6] != probe_item(8, 1, 2) ||
            available.items[7] != probe_item(9, 1, 1) ||
            available.items[8] != probe_item(11, 1, 0x3ff) ||
            available.items[9] != probe_item(12, 1, 2)) return 1;
    for (index = 0; index < 7; ++index)
        if (available.items[index + 10] != probe_item(
                (StarsU16)index, 1, 0x3ff)) return 1;

    /* Queue order, duplicate entries, progress bits, unavailable items and
     * finite versus unlimited capacities must survive reconciliation. */
    memset(&queue, 0, sizeof queue);
    queue.flags[0] = 0x31;
    queue.flags[1] = 0x52;
    queue.flags[2] = 0x73;
    ships[0] = probe_item(3, 2, 3);
    ships[1] = probe_item(4, 2, 0x3ff);
    input.ship_item_count = 2;
    queue.item_count = 5;
    queue.items[0] = probe_item(4, 2, 1) | 0xabc00000UL;
    queue.items[1] = probe_item(3, 2, 2);
    queue.items[2] = probe_item(30, 2, 1);
    queue.items[3] = probe_item(3, 2, 2);
    queue.items[4] = probe_item(4, 2, 1);
    stars_init_production(&input);
    if (queue.item_count != 4 ||
            queue.items[0] != (probe_item(4, 2, 1) | 0xabc00000UL) ||
            queue.items[1] != probe_item(3, 2, 2) ||
            queue.items[2] != probe_item(3, 2, 1) ||
            queue.items[3] != probe_item(4, 2, 1) ||
            (available.items[0] & 0x3ffUL) != 0 ||
            (available.items[1] & 0x3ffUL) != 0x3ffUL ||
            queue.flags[0] != 0x31 || queue.flags[1] != 0x52 ||
            queue.flags[2] != 0x73)
        return 1;
    memset(&input, 0, sizeof input);
    memset(&queue, 0, sizeof queue);
    input.available = &available;
    input.queue = &queue;
    bases[0] = probe_item(18, 2, 1);
    input.starbase_items = bases;
    input.starbase_item_count = 1;
    input.planetary_part_14_lookup = 1;
    input.scanner_type = 31;
    input.race_stat = 8;
    input.advance_tutor = probe_advance;
    input.context = &advanced;
    advanced = 0;
    stars_init_production(&input);
    if (advanced != 0 || available.item_count != 7 ||
            available.items[0] != bases[0] ||
            available.items[1] != probe_item(13, 1, 1) ||
            available.items[2] != probe_item(11, 1, 0x3ff)) return 1;
    for (index = 0; index < 4; ++index)
        if (available.items[index + 3] != probe_item(
                (StarsU16)(index + 3), 1, 0x3ff)) return 1;
    input.race_stat = 3;
    input.planetary_part_14_lookup = 2;
    stars_init_production(&input);
    if (available.item_count != 8 ||
            available.items[2] != probe_item(27, 1, 1) ||
            available.items[6] != probe_item(3, 1, 0x3ff) ||
            available.items[7] != probe_item(6, 1, 0x3ff)) return 1;
    puts("INIT_PRODUCTION_SOURCE=PASS");
    return 0;
}
