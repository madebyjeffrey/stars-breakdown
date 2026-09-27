#include <string.h>
#include "core.h"

#define PRODUCE_MESSAGE_NO_QUEUE 0x3f
#define PRODUCE_MESSAGE_QUEUE_DONE 0x3e
#define PRODUCE_MESSAGE_TERRAFORM_BLOCKED 0x129
#define PRODUCE_MESSAGE_CAPACITY 0x12a
#define PRODUCE_MESSAGE_PLANET_UNINHABITABLE 0xb9
#define PRODUCE_SPECIAL_TERRAFORM 12
#define PRODUCE_SPECIAL_FACTORY 7
#define PRODUCE_SPECIAL_MINE 8
#define PRODUCE_SPECIAL_DEFENSE 9

static void produce_remove_item(const StarsProduceOps *ops,
    StarsProducePlanet *planet, StarsI16 index)
{
    StarsI16 move;

    if (ops->remove_item) {
        ops->remove_item(ops->context, planet, index);
        return;
    }
    for (move = index; move + 1 < (StarsI16)planet->item_count; ++move)
        planet->items[move] = planet->items[move + 1];
    if (planet->item_count > 0)
        --planet->item_count;
}

static void produce_message(const StarsProduceOps *ops,
    const StarsProducePlanet *planet, StarsI16 code, StarsI16 value)
{
    if (ops->send_message && planet->owner >= 0)
        ops->send_message(ops->context, planet->owner, code, planet->id,
            planet->id, value);
}

static StarsI32 produce_resource_total(const StarsProduceOps *ops,
    const StarsProducePlanet *planet)
{
    StarsI32 value;
    StarsI32 extra;

    value = ops->resources_at_planet ?
        ops->resources_at_planet(ops->context, planet, planet->owner) :
        planet->resources;
    if (value <= 0)
        return 0;
    extra = ops->planet_extra ? ops->planet_extra(ops->context, planet->id) : 0;
    if (extra > 0)
        value += (value * extra) / (value + extra);
    return value;
}

static StarsI16 produce_capacity(const StarsProduceOps *ops,
    const StarsProducePlanet *planet, StarsI16 type)
{
    if (type == PRODUCE_SPECIAL_TERRAFORM && ops->terraform_capacity)
        return ops->terraform_capacity(ops->context, planet);
    if (ops->max_capacity)
        return ops->max_capacity(ops->context, planet, type, 1);
    if (type == PRODUCE_SPECIAL_FACTORY) return planet->factory_count;
    if (type == PRODUCE_SPECIAL_MINE) return planet->mine_count;
    if (type == PRODUCE_SPECIAL_DEFENSE) return planet->defense_count;
    return 32767;
}

static StarsI16 produce_one_item(const StarsProduceOps *ops,
    StarsProducePlanet *planet, StarsProduceItem *item, StarsI32 *resources)
{
    StarsI16 capacity;
    StarsI16 built;
    StarsI16 object_kind;
    StarsI32 minerals[3];

    if (!item->active || item->quantity <= 0)
        return 0;
    capacity = produce_capacity(ops, planet, item->type);
    if (item->special && capacity < item->quantity) {
        produce_message(ops, planet, PRODUCE_MESSAGE_CAPACITY, capacity);
        if (capacity <= 0)
            return 0;
    }
    if (item->special && item->type == PRODUCE_SPECIAL_TERRAFORM &&
            capacity <= 0) {
        produce_message(ops, planet, PRODUCE_MESSAGE_TERRAFORM_BLOCKED, 0);
        return 0;
    }
    minerals[0] = 0; minerals[1] = 0; minerals[2] = 0;
    built = ops->build_item ? ops->build_item(ops->context, planet, item,
        resources, item->partial) : item->quantity;
    if (built <= 0)
        return 0;
    if (built > item->quantity)
        built = item->quantity;
    object_kind = item->object_kind;
    if (ops->build_object && ops->build_object(ops->context, planet,
            object_kind, item->type, built, minerals) == 0)
        return 0;
    item->quantity = item->quantity - built;
    item->partial = 0;
    return built;
}

static StarsI16 produce_one_item_with_resources(const StarsProduceOps *ops,
    StarsProducePlanet *planet, StarsProduceItem *item,
    StarsProduceItem *partial, StarsI32 resources[4], StarsI16 *status)
{
    StarsI16 capacity;

    if (!item->active || item->quantity <= 0)
        return 0;
    capacity = produce_capacity(ops, planet, item->type);
    if (item->special && capacity < item->quantity) {
        produce_message(ops, planet, PRODUCE_MESSAGE_CAPACITY, capacity);
        if (capacity <= 0)
            return 0;
    }
    if (item->special && item->type == PRODUCE_SPECIAL_TERRAFORM &&
            capacity <= 0) {
        produce_message(ops, planet, PRODUCE_MESSAGE_TERRAFORM_BLOCKED, 0);
        return 0;
    }
    if (!ops->build_item_with_resources)
        return 0;
    return ops->build_item_with_resources(ops->context, planet, item,
        partial, resources, status);
}

static StarsI16 produce_insert_partial(const StarsProduceOps *ops,
    StarsProducePlanet *planet, StarsU16 index,
    const StarsProduceItem *partial)
{
    StarsU16 move;

    if (ops->insert_partial)
        return ops->insert_partial(ops->context, planet, index, partial);
    if (!planet->items || index > planet->item_count ||
            planet->item_count >= planet->item_capacity)
        return 0;
    for (move = planet->item_count; move > index; --move)
        planet->items[move] = planet->items[move - 1U];
    planet->items[index] = *partial;
    ++planet->item_count;
    return 1;
}

void stars_produce(StarsProduceInput *input)
{
    const StarsProduceOps *ops;
    StarsProducePlanet *planet;
    StarsI16 index;
    StarsI16 item_index;
    StarsI16 built;
    StarsI16 built_any;
    StarsI16 build_status;
    StarsI16 inserted;
    StarsI16 resource_index;
    StarsI32 resources;
    StarsI32 research_spent;
    StarsI32 resource_bank[4];
    StarsProduceItem partial_item;

    if (!input || !input->planets || !input->ops)
        return;
    ops = input->ops;
    if (ops->mine_minerals)
        ops->mine_minerals(ops->context);
    for (index = 0; index < (StarsI16)input->planet_count; ++index) {
        planet = &input->planets[index];
        planet->last_year_resources = 0;
        /* Original Produce sends the queue-empty message for an owned
         * planet whose PLPROD pointer is null, then credits its resources. */
        if (!planet->items) {
            if (planet->owner >= 0) {
                produce_message(ops, planet, PRODUCE_MESSAGE_NO_QUEUE, 0);
                planet->last_year_resources = produce_resource_total(ops,
                    planet);
            }
            continue;
        }
        if (planet->owner < 0) {
            continue;
        }
        resources = produce_resource_total(ops, planet);
        /* Produce scales the base resource pool to four-fifths when the
         * owning player carries the original fCheater flag. */
        if (planet->cheater)
            resources = (resources * 4) / 5;
        research_spent = 0;
        /* The original skips this share on planets marked by bit 23. */
        if (!planet->research_exempt && planet->research_percent > 0 &&
                resources > 0) {
            research_spent = (resources * planet->research_percent) / 100;
            if (research_spent > resources)
                research_spent = resources;
            resources -= research_spent;
        }
        planet->last_year_resources += research_spent;
        built_any = 0;
        for (item_index = 0; item_index < (StarsI16)planet->item_count; ) {
            if (!planet->items[item_index].active ||
                    planet->items[item_index].quantity <= 0) {
                produce_remove_item(ops, planet, item_index);
                continue;
            }
            if (resources <= 0)
                break;
            if (ops->build_item_with_resources) {
                if (!ops->insert_partial &&
                        planet->items[item_index].object_kind == 1 &&
                        planet->items[item_index].type < 7 &&
                        planet->item_count >= planet->item_capacity)
                    break;
                for (resource_index = 0; resource_index < 3;
                        ++resource_index)
                    resource_bank[resource_index] =
                        planet->mineral[resource_index];
                resource_bank[3] = resources;
                memset(&partial_item, 0, sizeof partial_item);
                build_status = 0;
                built = produce_one_item_with_resources(ops, planet,
                    &planet->items[item_index], &partial_item,
                    resource_bank, &build_status);
                for (resource_index = 0; resource_index < 3;
                        ++resource_index)
                    planet->mineral[resource_index] =
                        resource_bank[resource_index];
                resources = resource_bank[3];
                if (built > 0)
                    built_any = 1;
                if (partial_item.quantity > 0 && build_status > 4) {
                    if (planet->items[item_index].partial) {
                        planet->items[item_index] = partial_item;
                        ++item_index;
                        if (!built_any && resources > 0)
                            break;
                        continue;
                    }
                    inserted = produce_insert_partial(ops, planet,
                        (StarsU16)item_index, &partial_item);
                    if (inserted) {
                        if (planet->items[item_index + 1].quantity <= 0)
                            produce_remove_item(ops,
                                planet, (StarsI16)(item_index + 1));
                        continue;
                    }
                }
                if (planet->items[item_index].quantity <= 0) {
                    produce_remove_item(ops, planet, item_index);
                    continue;
                }
                if (built > 0)
                    continue;
                ++item_index;
                if (!built_any && resources > 0)
                    break;
                continue;
            }
            built = produce_one_item(ops, planet, &planet->items[item_index],
                &resources);
            if (built > 0)
                built_any = 1;
            if (planet->items[item_index].quantity <= 0) {
                produce_remove_item(ops, planet, item_index);
                continue;
            }
            if (built > 0)
                continue;
            ++item_index;
            if (!built_any && resources > 0)
                break;
        }
        if (built_any && planet->item_count == 0)
            produce_message(ops, planet, PRODUCE_MESSAGE_QUEUE_DONE, 0);
        if (resources > 0)
            planet->last_year_resources += resources;
    }
    if (ops->update_populations) {
        for (index = 0; index < (StarsI16)input->planet_count; ++index)
            ops->update_populations(ops->context, &input->planets[index]);
    }
    if (ops->update_research)
        ops->update_research(ops->context, 1);
    if ((input->game_flags & 0x0080) == 0 && ops->random_events)
        ops->random_events(ops->context);
}
