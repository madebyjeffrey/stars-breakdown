#include <string.h>
#include "core.h"

#define SHIP_PLANET_PLANET_NAME_FLAG 0x8000U

enum {
    SHIP_PLANET_NO_PLANET = -1,
    SHIP_PLANET_TILE_RESET_BIT = 0x0800,
    SHIP_PLANET_TILE_HIDE_BIT = 0x1000,
    SHIP_PLANET_GRID_BIT = 8,
    SHIP_PLANET_THING_KIND_SHIFT = 13,
    SHIP_PLANET_THING_KIND = 1,
    SHIP_PLANET_LABEL_NONE = 0x0377,
    SHIP_PLANET_LABEL_PLANET = 0x0376,
    SHIP_PLANET_LABEL_THING = 0x0375,
    SHIP_PLANET_BUTTON_OFFSET = 4,
    SHIP_PLANET_BUTTON_COUNT = 3,
    SHIP_PLANET_BUTTON_WIDTH_ADJUST = 0x10,
    SHIP_PLANET_BUTTON_SHOW = 5,
    SHIP_PLANET_BUTTON_POSITION_FLAGS = 0x14
};

static void hide_buttons(const StarsDrawShipPlanetInput *input)
{
    if (input->ops.show_window) {
        input->ops.show_window(input->context, input->button_planet, 0);
        input->ops.show_window(input->context, input->button_thing, 0);
    }
}

static StarsI16 has_matching_thing(const StarsDrawShipPlanetInput *input)
{
    StarsU16 index;
    const StarsDrawShipPlanetThing *thing;

    for (index = 0; index < input->thing_count; ++index) {
        thing = &input->things[index];
        if ((thing->flags >> SHIP_PLANET_THING_KIND_SHIFT) ==
                SHIP_PLANET_THING_KIND &&
                thing->point.x == input->object_x &&
                thing->point.y == input->object_y)
            return 1;
    }
    return 0;
}

/* NE 0B:17B6-1A53. Draw the selected planet/thing tile and position the two
 * contextual buttons, enabling them according to ownership and occupancy. */
void stars_draw_ship_planet(StarsDrawShipPlanetInput *input)
{
    StarsU8 title[128];
    StarsRect rect;
    StarsI16 title_id;
    StarsI16 left;
    StarsI16 right;
    StarsI16 top;
    StarsI16 width;
    StarsI16 height;
    StarsI16 button;
    StarsI16 owned;

    if (!input || !input->ops.draw_tile)
        return;
    if (input->planet_id == SHIP_PLANET_NO_PLANET) {
        title_id = SHIP_PLANET_LABEL_NONE;
        if (input->ops.get_text)
            input->ops.get_text(input->context, title_id, title,
                (StarsU16)sizeof title);
    } else if (input->ops.get_planet_name) {
        input->ops.get_planet_name(input->context,
            (StarsI16)(input->planet_id | SHIP_PLANET_PLANET_NAME_FLAG),
            title, (StarsU16)sizeof title);
    } else {
        title[0] = '\0';
    }
    if (input->tile_flags & SHIP_PLANET_TILE_RESET_BIT) {
        hide_buttons(input);
        input->tile_flags = (StarsU16)(input->tile_flags &
            (StarsU16)~SHIP_PLANET_TILE_RESET_BIT);
    }
    rect = input->tile_rect;
    if (!input->ops.draw_tile(input->context, input->hdc, input->tile_flags,
            title, &rect)) {
        hide_buttons(input);
        return;
    }
    left = (StarsI16)rect.left + SHIP_PLANET_BUTTON_OFFSET;
    right = (StarsI16)rect.right - SHIP_PLANET_BUTTON_OFFSET;
    top = (StarsI16)rect.top +
        (input->grid_bit == 0 ? 4 : 1);
    width = (StarsI16)((right - left - SHIP_PLANET_BUTTON_WIDTH_ADJUST) /
        SHIP_PLANET_BUTTON_COUNT);
    height = (StarsI16)(input->arial8_height * 3 / 2);
    owned = input->planet_id != SHIP_PLANET_NO_PLANET &&
        input->selected_planet_player == input->current_player;
    if (input->ops.enable_window)
        input->ops.enable_window(input->context, input->button_planet, owned);
    title_id = input->planet_id == SHIP_PLANET_NO_PLANET ?
        SHIP_PLANET_LABEL_THING : SHIP_PLANET_LABEL_PLANET;
    if (input->ops.get_text && input->ops.set_window_text) {
        input->ops.get_text(input->context, title_id, title,
            (StarsU16)sizeof title);
        input->ops.set_window_text(input->context, input->button_thing, title);
    }
    if (input->ops.enable_window) {
        owned = input->planet_id == SHIP_PLANET_NO_PLANET ?
            (StarsI16)!has_matching_thing(input) : 1;
        input->ops.enable_window(input->context, input->button_thing, owned);
    }
    if ((input->tile_flags & SHIP_PLANET_TILE_HIDE_BIT) == 0U &&
            input->ops.set_window_pos && input->ops.show_window) {
        for (button = 0; button < 2; ++button) {
            input->ops.set_window_pos(input->context,
                button == 0 ? input->button_planet : input->button_thing,
                left, top, width, height, SHIP_PLANET_BUTTON_POSITION_FLAGS);
            input->ops.show_window(input->context,
                button == 0 ? input->button_planet : input->button_thing,
                SHIP_PLANET_BUTTON_SHOW);
            left = (StarsI16)(left + width * 2 + 0x10);
        }
    }
}
