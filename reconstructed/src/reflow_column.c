#include "core.h"

/* NE 0A:5B80-5DD1. Recompute one tile column in the selected layout. */
void stars_reflow_column(StarsReflowColumnInput *input, StarsI16 column,
    StarsI16 tile, StarsI16 redraw)
{
    StarsReflowTile *tiles;
    StarsI16 count;
    StarsI16 i;
    StarsI16 y;
    StarsI16 height;
    StarsU16 object_flags;
    StarsRect rect;

    if (!input)
        return;
    if (input->selected_object == 2) {
        tiles = input->rows;
        count = 7;
    } else {
        tiles = input->columns;
        count = 6;
    }
    y = 4;
    if (tile == -1) {
        for (i = 0; i < count; ++i)
            if ((tiles[i].column_flags & 7U) == (StarsU16)column)
                break;
        if (i == count)
            return;
        tile = i;
    } else {
        for (i = 0; i < tile; ++i) {
            if ((tiles[i].column_flags & 7U) == (StarsU16)column) {
                height = (tiles[i].column_flags & 0x80U) == 0U ?
                    (StarsI16)(input->arial8_height + 3) : tiles[i].height;
                y = (StarsI16)(y + height + 4);
            }
        }
    }
    if (redraw && input->fill_rect) {
        rect.left = (StarsI16)(column * 0xc6 + 4);
        rect.top = y;
        rect.right = (StarsI16)(column * 0xc6 + 0xc3);
        rect.bottom = 0;
        input->fill_rect(input->context, input->hdc, &rect,
            input->button_brush);
    }
    object_flags = 0;
    while (tile < count &&
            (tiles[tile].column_flags & 7U) == (StarsU16)column) {
        tiles[tile].top = y;
        tiles[tile].column_flags = (StarsU16)(
            (tiles[tile].column_flags & (StarsU16)~0x800U) | 0x800U);
        object_flags = (StarsU16)(object_flags | tiles[tile].object_flags);
        height = (tiles[tile].column_flags & 0x80U) == 0U ?
            (StarsI16)(input->arial8_height + 3) : tiles[tile].height;
        y = (StarsI16)(y + height + 4);
        ++tile;
    }
    if (redraw && input->draw_plan_ship) {
        input->draw_plan_ship(input->context, input->hdc,
            object_flags);
        if (input->release_dc)
            input->release_dc(input->context, input->hwnd_planet,
                input->hdc);
    }
}
