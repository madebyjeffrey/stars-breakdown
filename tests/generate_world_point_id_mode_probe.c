#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct IdRandom {
    StarsI32 calls;
} IdRandom;

static StarsI16 id_random(void *opaque, StarsI16 limit)
{
    IdRandom *random;
    StarsI32 call;

    random = (IdRandom *)opaque;
    call = random->calls++;
    if (limit <= 0)
        return 0;
    if (call < 292)
        return (StarsI16)(call % limit);
    if (call < 310)
        return (StarsI16)((call - 292) % limit);
    return (StarsI16)(998 % limit);
}

static StarsI16 run_mode(StarsU16 world_flags, StarsI16 expected_second)
{
    StarsGenerateWorldInput input;
    StarsGenerateWorldLayout layout;
    IdRandom random;

    memset(&input, 0, sizeof input);
    memset(&layout, 0, sizeof layout);
    memset(&random, 0, sizeof random);
    input.map_size = 1;
    input.density = 1;
    input.minimum_distance = 0;
    input.world_flags = world_flags;
    input.random_value = id_random;
    input.context = &random;
    input.layout = &layout;
    if (!stars_generate_world_layout(&input))
        return 0;
    if (layout.point_count < 3 || layout.point_ids[0] != 998 ||
            layout.point_ids[1] != expected_second)
        return 0;
    return 1;
}

int main(void)
{
    StarsI16 failures;

    failures = !run_mode(0, 0);
    failures += !run_mode(STARS_GENERATE_WORLD_SPECIAL_ID_MODE, 999);
    printf("GENERATE_WORLD_POINT_ID_MODE_CHECKS=2 FAILURES=%d RESULT=%s\n",
        (int)failures, failures == 0 ? "PASS" : "FAIL");
    return failures == 0 ? 0 : 1;
}
