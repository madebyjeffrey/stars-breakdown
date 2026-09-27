#include <stdio.h>
#include "core.h"

int main(void)
{
    StarsDesignValueInput input;
    input.token_count = 3U; input.hull_dp = 1000UL;
    input.displacement_dp = 0UL; input.has_displacement = 0U;
    if (stars_design_value_from_token(&input) != 3000) return 1;
    input.displacement_dp = 128UL; input.has_displacement = 1U;
    if (stars_design_value_from_token(&input) != 2998) return 2;
    input.token_count = 0U; input.displacement_dp = 1UL;
    if (stars_design_value_from_token(&input) != 0) return 3;
    puts("DESIGN_VALUE_SOURCE=PASS");
    return 0;
}
