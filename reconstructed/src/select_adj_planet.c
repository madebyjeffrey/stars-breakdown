#include "core.h"

static StarsI16 adjacent_find_planet(const StarsAdjacentPlanet *planets,
    StarsU16 count, StarsI16 id)
{
    StarsU16 index;
    for (index = 0; index < count; ++index)
        if (planets[index].id == id)
            return (StarsI16)index;
    return (StarsI16)count;
}

/* Genuine C reconstruction of SelectAdjPlanet, NE 0A:44DA-4702. */
void stars_select_adj_planet(StarsI16 increment, StarsI16 planet_id,
    StarsSelectAdjacentInput *input, const StarsSelectAdjacentOps *ops)
{
    StarsI16 index;
    StarsI16 count;
    StarsAdjacentPlanet *planet;
    StarsScanSelection selection;

    if (!input || !ops || input->planet_count == 0U || planet_id == -1 ||
        !input->planets || !input->planet_points)
        return;
    if (increment != 0)
        planet_id = input->selected_planet;
    if (!input->report_cached && ops->invalidate_report)
        ops->invalidate_report(ops->context, 0, 1);
    index = adjacent_find_planet(input->planets, input->planet_count,
        planet_id);
    if (index == (StarsI16)input->planet_count ||
        (input->planets[index].visibility_flags & 0x00ffU) != 7U) {
        selection.x = input->planet_points[planet_id].x;
        selection.y = input->planet_points[planet_id].y;
        selection.grobj = 0x0081;
        if (ops->change_scan)
            ops->change_scan(ops->context, &selection, 0);
    } else {
        if (increment != 0) {
            count = 0;
            while (count < (StarsI16)input->player_planet_count &&
                input->player_planet_ids[count] != planet_id)
                ++count;
            count = (StarsI16)(count + increment);
            if (count < (StarsI16)input->player_planet_count) {
                if (count < 0)
                    count = (StarsI16)(input->player_planet_count - 1U);
            } else {
                count = 0;
            }
            planet_id = input->player_planet_ids[count];
            index = adjacent_find_planet(input->planets,
                input->planet_count, planet_id);
        }
        planet = &input->planets[index];
        if (planet->owner != input->current_player)
            return;
        selection.x = input->planet_points[planet_id].x;
        selection.y = input->planet_points[planet_id].y;
        selection.grobj = 0x0081;
        if (ops->change_scan)
            ops->change_scan(ops->context, &selection, 0);
        if (increment != 0) {
            if (ops->redraw_scan)
                ops->redraw_scan(ops->context, 0, 0);
            if (ops->change_main)
                ops->change_main(ops->context, 1, planet_id);
            if (ops->redraw_scan)
                ops->redraw_scan(ops->context, 0, 1);
        }
    }
    if (ops->center_scan)
        ops->center_scan(ops->context,
            input->planet_points[planet_id], 1);
    if (ops->draw_scroll_bar)
        ops->draw_scroll_bar(ops->context);
    if (ops->invalidate_mine)
        ops->invalidate_mine(ops->context, input->mine_window, 1);
    if (ops->set_mineral_title)
        ops->set_mineral_title(ops->context);
}
