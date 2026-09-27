#include <stdio.h>
#include "core.h"

int main(void)
{
    StarsMdTargetToken token;
    token.grobj = 1U; token.overlap_23 = (StarsU16)(7U << 12);
    if (!stars_is_target_of_md_target(&token, 1) ||
        !stars_is_target_of_md_target(&token, 2) ||
        !stars_is_target_of_md_target(&token, 4) ||
        !stars_is_target_of_md_target(&token, 5) ||
        !stars_is_target_of_md_target(&token, 7)) return 1;
    token.grobj = 0U; token.overlap_23 = (StarsU16)(6U << 12);
    if (stars_is_target_of_md_target(&token, 2) ||
        !stars_is_target_of_md_target(&token, 5) ||
        stars_is_target_of_md_target(&token, 4) ||
        stars_is_target_of_md_target(&token, 9)) return 2;
    puts("MD_TARGET_SOURCE=PASS");
    return 0;
}
