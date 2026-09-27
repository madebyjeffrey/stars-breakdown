#include <stdio.h>
#include "core.h"

int main(void)
{
    StarsButtonDownInput input;
    input.scan_flags = 0x1292U; input.mine_mode = 0x000fU;
    if (!stars_is_button_down(&input, 2) || !stars_is_button_down(&input, 6) ||
        !stars_is_button_down(&input, 9) || !stars_is_button_down(&input, 12) ||
        !stars_is_button_down(&input, 17) || stars_is_button_down(&input, 8)) return 1;
    input.scan_flags = (StarsU16)(input.scan_flags | 0x0040U);
    if (!stars_is_button_down(&input, 8)) return 2;
    input.mine_mode = 0U;
    if (stars_is_button_down(&input, 8)) return 3;
    if (stars_is_button_down(&input, 13) || stars_is_button_down(&input, 15) ||
        stars_is_button_down(&input, 16)) return 4;
    puts("BUTTON_DOWN_SOURCE=PASS");
    return 0;
}
