#include <stdio.h>
#include <string.h>
#include "core.h"

int main(void)
{
    StarsFleetWp0Input input;
    memset(&input, 0, sizeof input);
    input.cord = 1U;
    if (!stars_fdest_is_wp0(&input)) return 1;
    input.cord = 2U;
    if (stars_fdest_is_wp0(&input)) return 2;
    input.order_flags = 0x1001U;
    input.order_words[2] = 0x6000U;
    if (!stars_fdest_is_wp0(&input)) return 3;
    input.order_words[2] = 0x5000U;
    if (stars_fdest_is_wp0(&input)) return 4;
    input.order_flags = 0x1102U;
    if (!stars_fdest_is_wp0(&input)) return 5;
    input.order_flags = 0x1004U;
    if (!stars_fdest_is_wp0(&input)) return 6;
    input.order_flags = 0x1003U;
    if (stars_fdest_is_wp0(&input)) return 7;
    puts("FLEET_WP0_SOURCE=PASS");
    return 0;
}
