#include "core.h"

static void hull_draw_line(StarsDrawHullInput *input, StarsI16 *row,
    StarsU16 label, StarsI32 first, StarsI32 second, StarsI16 color)
{
    if (input->ops && input->ops->draw_line)
        input->ops->draw_line(input->context, input->hdc, *row, label,
            first, second, color);
    ++*row;
}

/* Genuine C reconstruction of DrawBuildSelHull, NE 1A:451E-53D9.  The
 * callback is the presentation boundary; cost discounts, mode gates, VCR
 * damage handling, and the optional scan/defense rows stay in ordinary C. */
void stars_draw_build_sel_hull(StarsDrawHullInput *input)
{
    StarsI16 owned_dc;
    StarsI16 row;
    StarsI16 index;
    StarsI32 cost;
    StarsI32 remaining_shield;
    StarsI32 popup_value;
    StarsRect rect;

    if (!input || input->mode == 3)
        return;

    owned_dc = 0;
    if (input->hdc == 0) {
        owned_dc = 1;
        if (input->ops && input->ops->get_dc)
            input->hdc = input->ops->get_dc(input->context, input->window);
    }
    if (input->hdc == 0)
        return;

    if (input->draw_rect.right > input->draw_rect.left &&
        input->draw_rect.bottom > input->draw_rect.top)
        rect = input->draw_rect;
    else
        rect = input->client_rect;
    if (input->ops && input->ops->fill_background)
        input->ops->fill_background(input->context, input->hdc, &rect);

    if (!input->has_hull)
        goto cleanup;

    row = input->draw_y;
    hull_draw_line(input, &row, STARS_HULL_ROW_TITLE, input->hull_id, 0, 0);

    if (input->mode == 1) {
        remaining_shield = input->vcr_shield - input->vcr_damage;
        if (remaining_shield < 0)
            remaining_shield = 0;
        hull_draw_line(input, &row, STARS_HULL_ROW_ARMOR,
            remaining_shield, input->vcr_shield, 0);
    }
    else {
        for (index = 0; index < 4; ++index) {
            cost = input->mineral_cost[index];
            if (input->starbase_mode && !input->race_can_discount &&
                !input->race_special)
                cost -= cost / 5;
            if (input->starbase_mode)
                cost -= cost / 2;
            hull_draw_line(input, &row, STARS_HULL_ROW_COST, cost, index, 0);
        }
        hull_draw_line(input, &row, STARS_HULL_ROW_WEIGHT,
            input->empty_weight, 0, 0);
        hull_draw_line(input, &row, STARS_HULL_ROW_ARMOR,
            input->armor, input->shield, 0);
        hull_draw_line(input, &row, STARS_HULL_ROW_POWER,
            input->power, 0, 0);

        if ((input->tech_flags & 0x4000) != 0) {
            hull_draw_line(input, &row, STARS_HULL_ROW_CLOAK_JAMMER,
                input->cloak, input->jammer, 0);
            hull_draw_line(input, &row, STARS_HULL_ROW_SPEED,
                input->speed, 0, 0);
            if (!input->starbase_mode) {
                hull_draw_line(input, &row, STARS_HULL_ROW_SCANNER,
                    input->scanner_range, input->scanner_width, 0);
            }
            else if (input->race_special) {
                hull_draw_line(input, &row, STARS_HULL_ROW_SCANNER,
                    input->scanner_height, 0, 0);
            }
        }
    }

    if (input->popup_window && input->popup_kind == 0x0b &&
        input->popup_max != 0 && input->popup_distance != 0) {
        popup_value = input->popup_distance;
        if (popup_value < 1)
            popup_value = 1;
        hull_draw_line(input, &row, STARS_HULL_ROW_POPUP,
            popup_value, input->popup_units, 0x7f);
    }

cleanup:
    if (input->ops && input->ops->restore)
        input->ops->restore(input->context, input->window, input->hdc);
    if (owned_dc && input->ops && input->ops->release_dc)
        input->ops->release_dc(input->context, input->window, input->hdc);
}
