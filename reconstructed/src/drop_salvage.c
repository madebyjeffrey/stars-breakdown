#include "core.h"

#define SALVAGE_FLAG_ON_MAP 0x4000U
#define SALVAGE_VALUE_MASK 0x3fffU
#define SALVAGE_NEW_VALUE 0x03ffU
#define SALVAGE_FULL_VALUE 3000U
#define SALVAGE_VALUE_SCALE 10U
#define SALVAGE_CHUNK_UNITS 30000UL
#define SALVAGE_RESOURCE_COUNT 3
#define SALVAGE_RANDOM_LIMIT 10

static StarsU32 salvage_add(StarsU32 left, StarsU32 right)
{
    return left + right;
}

static StarsU32 salvage_total(const StarsU32 minerals[3])
{
    StarsI16 index;
    StarsU32 total = 0UL;
    for (index = 0; index < SALVAGE_RESOURCE_COUNT; ++index)
        total = salvage_add(total, minerals[index]);
    return total;
}

static void salvage_set_new_thing(StarsSalvageThing *thing,
    const StarsPoint *point)
{
    thing->x = point->x;
    thing->y = point->y;
    thing->flags = (StarsU16)(thing->flags & 0xc3ffU);
    thing->flags = (StarsU16)((thing->flags & 0xfc00U) |
        SALVAGE_NEW_VALUE);
    thing->value = 0U;
}

/* Genuine semantic reconstruction of DropSalvage, NE 31:24DC-2952.
 * Salvage is represented as mineral cargo spread across map objects.  The
 * first object is reused when one already exists at the point; each object
 * can hold 3,000 scaled value units, so a full object is filled and a new
 * object is allocated for the remainder.  The original also refuses to add
 * salvage on a plan coordinate and synthesizes a small nonzero cargo load
 * when its caller supplies an empty load. */
void stars_drop_salvage(StarsDropSalvageInput *input)
{
    StarsI16 index;
    StarsU32 total;
    StarsU32 capacity;
    StarsU32 amount;
    StarsU32 added_value;
    StarsSalvageThing *current;

    if (!input || !input->thing) return;
    for (index = 0; index < input->plan_count; ++index)
        if (input->plan_points[index].x == input->point.x &&
                input->plan_points[index].y == input->point.y)
            return;
    total = salvage_total(input->minerals);
    while (total == 0UL) {
        for (index = 0; index < SALVAGE_RESOURCE_COUNT; ++index) {
            input->minerals[index] = input->random ?
                (StarsU32)input->random(input->context,
                    SALVAGE_RANDOM_LIMIT) : 0UL;
        }
        total = salvage_total(input->minerals);
    }
    current = *input->thing;
    if (!current) {
        if (!input->new_thing) return;
        current = input->new_thing(input->context, input->player, 1);
        if (!current) return;
        salvage_set_new_thing(current, &input->point);
    } else {
        for (index = 0; index < SALVAGE_RESOURCE_COUNT; ++index) {
            input->minerals[index] = salvage_add(input->minerals[index],
                current->minerals[index]);
            current->minerals[index] = 0UL;
        }
        current->value = 0;
        total = salvage_total(input->minerals);
    }
    current->flags = (StarsU16)((current->flags & (StarsU16)~SALVAGE_FLAG_ON_MAP) |
        SALVAGE_FLAG_ON_MAP);
    do {
        for (index = 0; index < SALVAGE_RESOURCE_COUNT; ++index) {
            capacity = (StarsU32)(current->value & SALVAGE_VALUE_MASK) *
                SALVAGE_VALUE_SCALE;
            if (capacity >= SALVAGE_CHUNK_UNITS ||
                    capacity + input->minerals[index] >= SALVAGE_CHUNK_UNITS) {
                amount = SALVAGE_CHUNK_UNITS - capacity;
                current->value = SALVAGE_FULL_VALUE;
                current->minerals[index] += amount;
                input->minerals[index] -= amount;
                total -= amount;
                if (!input->new_thing) return;
                current = input->new_thing(input->context, input->player, 1);
                if (!current) return;
                salvage_set_new_thing(current, &input->point);
                current->flags = (StarsU16)((current->flags &
                    (StarsU16)~SALVAGE_FLAG_ON_MAP) | SALVAGE_FLAG_ON_MAP);
            } else {
                added_value = (input->minerals[index] + 9UL) /
                    SALVAGE_VALUE_SCALE;
                current->value = (StarsU16)((current->value + added_value) &
                    SALVAGE_VALUE_MASK);
                current->minerals[index] += input->minerals[index];
                total -= input->minerals[index];
                input->minerals[index] = 0UL;
            }
            if (total == 0UL) break;
        }
    } while (total != 0UL);
    *input->thing = current;
}
