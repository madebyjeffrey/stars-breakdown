#include "core.h"

#define MINEFIELD_MAX_TYPES 3
#define MINEFIELD_MAX_SHIP_TYPES 16
#define MINEFIELD_MESSAGE_HIT 0xc5
#define MINEFIELD_MESSAGE_DAMAGE 0xc6
#define MINEFIELD_MESSAGE_DESTROYED 0xc7

static StarsI32 mine_abs(StarsI32 value)
{
    return value < 0 ? -value : value;
}

static StarsI16 mine_intersects(const StarsMinefieldOps *ops,
    const StarsMinefieldFleet *fleet, const StarsMinefield *field,
    StarsI32 *entry, StarsI32 *span)
{
    StarsI32 dx;
    StarsI32 dy;
    StarsI32 along;

    if (ops->intersect)
        return ops->intersect(ops->context, fleet, field, entry, span);
    dx = (StarsI32)fleet->target_x - fleet->x;
    dy = (StarsI32)fleet->target_y - fleet->y;
    if (dx != 0 && mine_abs((StarsI32)field->y - fleet->y) <= field->radius) {
        along = ((StarsI32)field->x - fleet->x) * dx;
        if (along >= 0 && along <= dx * dx) {
            *entry = mine_abs((StarsI32)field->x - fleet->x);
            *span = field->radius;
            return 1;
        }
    }
    if (dy != 0 && mine_abs((StarsI32)field->x - fleet->x) <= field->radius) {
        along = ((StarsI32)field->y - fleet->y) * dy;
        if (along >= 0 && along <= dy * dy) {
            *entry = mine_abs((StarsI32)field->y - fleet->y);
            *span = field->radius;
            return 1;
        }
    }
    return 0;
}

static StarsI32 mine_total_ships(const StarsMinefieldFleet *fleet)
{
    StarsI16 index;
    StarsI32 total;

    total = 0;
    for (index = 0; index < fleet->engine_count &&
            index < MINEFIELD_MAX_SHIP_TYPES; ++index)
        total += fleet->engine_count_by_type[index];
    return total;
}

static StarsI32 mine_compute_damage(const StarsMinefieldOps *ops,
    StarsMinefieldFleet *fleet, const StarsMinefield *field)
{
    StarsI32 ships;
    StarsI32 shield;
    StarsI32 damage;

    ships = mine_total_ships(fleet);
    damage = field->strength * (ships > 0 ? ships : 1);
    if (ops->shield_value)
        shield = ops->shield_value(ops->context, fleet->owner,
            field->mine_class);
    else
        shield = 0;
    damage -= shield;
    if (damage < 0) damage = 0;
    if (ops->apply_damage)
        damage = ops->apply_damage(ops->context, fleet, field, damage);
    return damage;
}

StarsI16 stars_travel_through_mine_fields(StarsMinefieldInput *input)
{
    const StarsMinefieldOps *ops;
    StarsMinefield *selected;
    StarsI32 selected_entry;
    StarsI32 selected_span;
    StarsI32 entry;
    StarsI32 span;
    StarsI32 damage;
    StarsI32 limit;
    StarsI16 index;
    StarsI16 owner;

    if (!input || !input->fleet || !input->travel || !input->ops)
        return 1;
    ops = input->ops;
    selected = input->hit;
    selected_entry = 0x7fffffffL;
    selected_span = 0;
    if (!selected) {
        for (index = 0; index < (StarsI16)input->field_count; ++index) {
            if (!input->fields[index].active ||
                    input->fields[index].owner == input->fleet->owner)
                continue;
            if (!mine_intersects(ops, input->fleet, &input->fields[index],
                    &entry, &span))
                continue;
            if (entry < selected_entry) {
                selected = &input->fields[index];
                selected_entry = entry;
                selected_span = span;
            }
        }
        if (!selected)
            return 1;
    } else {
        if (mine_intersects(ops, input->fleet, selected, &selected_entry,
                &selected_span) == 0) {
            selected_entry = 0;
            selected_span = 0;
        }
    }
    limit = *input->travel;
    if (selected_entry < limit)
        *input->travel = selected_entry;
    else
        *input->travel = 0;
    damage = mine_compute_damage(ops, input->fleet, selected);
    owner = selected->owner;
    if (ops->send_message) {
        ops->send_message(ops->context, input->fleet->owner,
            damage == 0 ? MINEFIELD_MESSAGE_HIT : MINEFIELD_MESSAGE_DAMAGE,
            input->fleet->id, owner, (StarsI16)selected_entry,
            (StarsI16)selected_span);
    }
    if (ops->transfer_cargo && damage > 0)
        ops->transfer_cargo(ops->context, input->fleet);
    if (damage >= selected->strength && selected->strength > 0) {
        selected->active = 0;
        if (ops->remove_mine)
            ops->remove_mine(ops->context, selected);
        if (ops->send_message)
            ops->send_message(ops->context, input->fleet->owner,
                MINEFIELD_MESSAGE_DESTROYED, input->fleet->id, owner,
                (StarsI16)damage, 0);
    }
    if (input->hit)
        *input->hit = *selected;
    return 0;
}
