#include <stdio.h>
#include "core.h"

static void reflow(void *context, StarsI16 column, StarsI16 tile,
        StarsI16 redraw)
{
    StarsI16 *calls = (StarsI16 *)context;
    if (column >= 0 && column < 4 && tile == -1 && redraw == 0)
        *calls = (StarsI16)(*calls + 1);
}
int main(void)
{
    StarsEnsureTileSizeInput input;
    StarsI16 selected = 7;
    StarsI16 calls = 0;
    input.small_tiles = 1; input.tile_flag = 0; input.arial8_height = 8;
    input.selected_object = &selected; input.reflow = reflow;
    input.context = &calls;
    input.columns[0].kind = 0x40; input.columns[0].size = 100;
    input.columns[1].kind = 4; input.columns[1].size = 100;
    input.columns[2].kind = 0x80; input.columns[2].size = 100;
    input.columns[3].kind = 0; input.columns[3].size = 100;
    input.columns[4].kind = 0; input.columns[4].size = 100;
    input.columns[5].kind = 0; input.columns[5].size = 100;
    input.rows[0].kind = 1; input.rows[0].size = 100;
    input.rows[1].kind = 0x200; input.rows[1].size = 100;
    input.rows[2].kind = 0x20; input.rows[2].size = 100;
    input.rows[3].kind = 4; input.rows[3].size = 100;
    input.rows[4].kind = 0x80; input.rows[4].size = 100;
    input.rows[5].kind = 0x100; input.rows[5].size = 100;
    input.rows[6].kind = 0x40; input.rows[6].size = 100;
    stars_ensure_tile_size(&input);
    if (input.tile_flag != 1 || selected != 7 || calls != 8 ||
            input.columns[0].size != 80 || input.columns[1].size != 76 ||
            input.columns[2].size != 90 || input.rows[0].size != 66 ||
            input.rows[1].size != 68 || input.rows[6].size != 94)
        return 1;
    puts("ENSURE_TILE_SIZE_SOURCE=PASS");
    return 0;
}
