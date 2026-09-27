#include <stdio.h>
#include <string.h>
#include "core.h"

static void initialize(StarsHtMineWindowInput *input)
{
    memset(input, 0, sizeof *input);
    input->client_left = 0;
    input->client_top = 0;
    input->client_right = 100;
    input->client_bottom = 200;
    input->font_height = 8;
    input->selection.planet = 4;
    input->planet_found = 1;
    input->planet_owner = 2;
    input->player = 2;
}

int main(void)
{
    StarsHtMineWindowInput input;
    int failures = 0;

    initialize(&input);
    input.selection.grobj = 4;
    input.selection.grobj_full = 1;
    input.x = 10;
    input.y = 4;
    if (stars_ht_mine_window(&input) != 12) ++failures;

    initialize(&input);
    input.selection.grobj = 2;
    input.x = 25;
    input.y = 90;
    if (stars_ht_mine_window(&input) != 10) ++failures;

    initialize(&input);
    input.selection.grobj = 8;
    input.selected_thing_kind = 0;
    input.x = 20;
    input.y = 30;
    if (stars_ht_mine_window(&input) != 14) ++failures;

    initialize(&input);
    input.selection.grobj = 1;
    input.planet_owner = 4;
    input.planet_flags = 0x0200;
    input.x = 95;
    input.y = 4;
    if (stars_ht_mine_window(&input) != 13) ++failures;

#ifndef TARGET_NATIVE
    printf("HT_MINE_WINDOW_CHECKS=4 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
#endif
    return failures != 0;
}
