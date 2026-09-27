#include "core.h"

/* NE 0A:587E-5B80. Switch between the two tile-size layouts and reflow both
 * object columns after applying the original per-row geometry adjustments. */
void stars_ensure_tile_size(StarsEnsureTileSizeInput *input)
{
    StarsI16 old_object;
    StarsI16 direction;
    StarsI16 i;
    StarsI16 delta;

    if (!input || !input->selected_object)
        return;
    old_object = *input->selected_object;
    if (input->small_tiles == input->tile_flag) {
        *input->selected_object = old_object;
        return;
    }
    input->tile_flag = (StarsI16)(input->small_tiles & 1);
    direction = input->small_tiles == 0 ? 1 : -1;
    for (i = 0; i < 6; ++i) {
        delta = 0;
        if (input->columns[i].kind == 0x40)
            delta = (StarsI16)((input->arial8_height + 2) * 2);
        if (input->columns[i].kind == 4)
            delta = (StarsI16)((input->arial8_height + 4) * 2);
        if (input->columns[i].kind == 0x80)
            delta = 10;
        input->columns[i].size = (StarsI16)(input->columns[i].size +
            delta * direction);
    }
    for (i = 0; i < 7; ++i) {
        delta = 0;
        if (input->rows[i].kind == 1)
            delta = (StarsI16)(input->arial8_height * 4 + 2);
        if (input->rows[i].kind == 0x200)
            delta = (StarsI16)(input->arial8_height * 3 + 8);
        if (input->rows[i].kind == 0x20)
            delta = (StarsI16)(input->arial8_height + 9);
        if (input->rows[i].kind == 4)
            delta = (StarsI16)((input->arial8_height + 4) * 2);
        if (input->rows[i].kind == 0x80)
            delta = 10;
        if (input->rows[i].kind == 0x100)
            delta = 2;
        if (input->rows[i].kind == 0x40)
            delta = 6;
        input->rows[i].size = (StarsI16)(input->rows[i].size +
            delta * direction);
    }
    if (input->reflow) {
        *input->selected_object = 1;
        for (i = 0; i < 4; ++i)
            input->reflow(input->context, i, -1, 0);
        *input->selected_object = 2;
        for (i = 0; i < 4; ++i)
            input->reflow(input->context, i, -1, 0);
    }
    *input->selected_object = old_object;
}
