#include "core.h"
#include <string.h>

/* NE 24:8B70-8CBB. Temporarily use the requesting player's record while
 * evaluating remote terraforming, preserve the original planet-player fields,
 * and restore the complete 0xC0-byte player record before returning. */
StarsI16 stars_best_remote_terraform(StarsBestRemoteTerraformInput *input,
    StarsI16 player, StarsI16 help)
{
    StarsU8 saved[0xc0];
    StarsU8 *target;
    StarsU8 *source;
    StarsI16 result;
    StarsI16 i;

    if (!input || !input->planet || !input->players ||
        input->planet->player < 0 || player < 0 ||
        input->planet->player >= input->player_count ||
        player >= input->player_count || !input->best_terraform)
        return 0;
    target = input->players + (StarsU16)(input->planet->player * 0xc0);
    source = input->players + (StarsU16)(player * 0xc0);
    memcpy(saved, target, sizeof(saved));
    memcpy(target, source, sizeof(saved));
    for (i = 0; i < 3; ++i) {
        target[0x10 + i] = saved[0x10 + i];
        target[0x13 + i] = saved[0x13 + i];
        target[0x16 + i] = saved[0x16 + i];
    }
    result = input->best_terraform(input->context, input->planet, help);
    memcpy(target, saved, sizeof(saved));
    return result;
}
