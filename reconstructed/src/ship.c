/* Genuine C reconstruction of the numeric-only edit control procedure. */
#include "core.h"
#include <string.h>

static StarsI16 fleet_has_hull_mask(const StarsI16 *ship_counts,
    StarsU16 player, StarsGetHullType get_hull_type, void *context,
    StarsU32 mask)
{
    StarsU16 index;
    StarsU16 hull_type;

    if (!ship_counts || !get_hull_type)
        return 0;
    for (index = 0; index < 16U; ++index) {
        if (ship_counts[index] != 0) {
            hull_type = get_hull_type(context, player, index);
            if ((1UL << hull_type & mask) != 0UL)
                return 1;
        }
    }
    return 0;
}

/* NE 17:227C and 17:2322.  Both routines scan the fleet's sixteen design
 * counts, testing the corresponding player's hull-type bit. */
StarsI16 stars_f_colonizer(const StarsI16 *ship_counts, StarsU16 player,
    StarsGetHullType get_hull_type, void *context)
{
    return fleet_has_hull_mask(ship_counts, player, get_hull_type, context,
        0xc000UL);
}

StarsI16 stars_f_scout(const StarsI16 *ship_counts, StarsU16 player,
    StarsGetHullType get_hull_type, void *context)
{
    return fleet_has_hull_mask(ship_counts, player, get_hull_type, context,
        0x0070UL);
}

StarsI32 stars_fake_edit_proc(StarsU16 hwnd, StarsU16 message,
    StarsU16 character, StarsI32 lparam, StarsU32 real_proc,
    StarsFakeCallWindowProc call_window, void *context)
{
    if (message == 0x0102U &&
        !((character >= 0x30U && character <= 0x39U) || character == 8U))
        return 0;
    if (!call_window)
        return 0;
    return call_window(context, real_proc, hwnd, message, character, lparam);
}

/* NE 11:9354.  The view flag selects the number of leading order rows; a
 * second list-box message keeps the selected row visible when necessary. */
void stars_set_orders_lb_sel(StarsU16 window, StarsI16 selection,
    StarsU16 view_flags, const StarsSetOrdersLbSelOps *ops, void *context)
{
    StarsI16 first_visible;

    if (!ops || !ops->send_message || !ops->update_window ||
        !ops->update_dropdowns)
        return;
    ops->send_message(context, window, 0x0407U, (StarsU16)selection, 0U);
    first_visible = (StarsI16)(((view_flags >> 3) & 1U) ? 1 : 2);
    if (first_visible < selection)
        ops->send_message(context, window, 0x0418U,
            (StarsU16)(selection - first_visible), 0U);
    ops->update_window(context, window);
    ops->update_dropdowns(context, 0);
}

/* NE 11:928C. Refill the orders list from the 18-byte order records and pass
 * the packed location fields to the name resolver before selecting the row. */
void stars_fill_orders_lb(StarsU16 listbox, const StarsU8 *order_list,
    StarsU16 order_count, StarsI16 active_row, StarsI16 selected_object,
    const StarsFillOrdersOps *ops, void *context)
{
    StarsU16 index;
    StarsMoveOrder order;
    const StarsU8 *name;
    StarsI16 warp;

    if (!ops || !ops->clear_list || !ops->get_location_name ||
        !ops->add_text || !order_list)
        return;
    ops->clear_list(context, listbox);
    for (index = 0; index < order_count; ++index) {
        memcpy(&order, order_list + 4U + index * 0x12U, sizeof(order));
        warp = (StarsI16)((order.flags >> 8) & 0x000fU);
        name = ops->get_location_name(context, warp, order.id,
            order.point.x, order.point.y);
        ops->add_text(context, listbox, name);
    }
    if (ops->select_row)
        ops->select_row(context, active_row);
    if (selected_object == 2 && ops->draw_plan_ship)
        ops->draw_plan_ship(context, 0, 0x0120U);
}

/* NE 11:9A36. Reset the battle dropdown, add its title and the current
 * player's battle names, then restore the requested selection. */
void stars_fill_battle_dropdown(StarsU16 dropdown, StarsI16 player,
    StarsU16 battle_count, StarsI16 selection,
    const StarsFillBattleDropdownOps *ops, void *context)
{
    StarsU16 index;
    const StarsU8 *text;

    if (!ops || !ops->clear_list || !ops->get_text || !ops->add_text)
        return;
    ops->clear_list(context, dropdown);
    text = ops->get_text(context, player, -1);
    ops->add_text(context, dropdown, text);
    for (index = 0; index < battle_count; ++index) {
        text = ops->get_text(context, player, (StarsI16)index);
        ops->add_text(context, dropdown, text);
    }
    if (ops->select_row)
        ops->select_row(context, selection);
}

static const StarsU8 *nth_valid_design(const StarsU8 *designs,
    StarsU16 count, StarsI16 n)
{
    StarsU16 index;
    StarsU16 flags;
    const StarsU8 *design;

    for (index = 0; index < count; ++index) {
        design = designs + index * 0x93U;
        flags = (StarsU16)(design[0x7b] |
            ((StarsU16)design[0x7c] << 8));
        if ((flags & 0x0200U) == 0U) {
            if (n == 0)
                return design;
            --n;
        }
    }
    return 0;
}

/* NE 26:5C06. Select the nth unreserved design from the normal or starbase
 * table; the original normal table has sixteen entries and the starbase table
 * has ten, but the counts remain explicit for the host adapter. */
const StarsU8 *stars_nth_valid_shdef(const StarsU8 *normal_designs,
    StarsU16 normal_count, StarsI16 n, const StarsU8 *starbase_designs,
    StarsU16 starbase_count, StarsI16 starbase_mode)
{
    return starbase_mode ? nth_valid_design(starbase_designs, starbase_count, n) :
        nth_valid_design(normal_designs, normal_count, n);
}

/* NE 11:C280. Clear the starbase design flag on matching planets, then run
 * both queue cleanup callbacks against the same original planet record. */
void stars_destroy_all_ishdef_sb(StarsU8 *planets, StarsU16 count,
    StarsI16 ishdef_sb, StarsI16 player, const StarsDestroyIshdefSbOps *ops,
    void *context)
{
    StarsU16 index;
    StarsU8 *planet;
    StarsU16 flags;
    StarsU16 design;

    if (!planets)
        return;
    for (index = 0; index < count; ++index) {
        planet = planets + (StarsU16)(index * 0x38U);
        flags = (StarsU16)(planet[4] | ((StarsU16)planet[5] << 8));
        design = (StarsU16)(planet[0x2c] |
            ((StarsU16)planet[0x2d] << 8));
        if ((StarsI16)(planet[2] | ((StarsU16)planet[3] << 8)) == player &&
            (flags & 0x0200U) != 0U && (design & 0x000fU) ==
            (StarsU16)ishdef_sb) {
            flags = (StarsU16)(flags & 0xfdffU);
            planet[4] = (StarsU8)flags;
            planet[5] = (StarsU8)(flags >> 8);
            if (ops && ops->kill_ships)
                ops->kill_ships(context, planet);
            if (ops && ops->kill_mass_packets)
                ops->kill_mass_packets(context, planet);
        }
    }
}
