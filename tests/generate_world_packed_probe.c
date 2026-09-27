#include "core.h"
#include <stdio.h>
#include <string.h>

static void put16(StarsU8 *raw, StarsU16 offset, StarsU16 value)
{
    raw[offset] = (StarsU8)value;
    raw[(StarsU16)(offset + 1U)] = (StarsU8)(value >> 8);
}

static void put32(StarsU8 *raw, StarsU16 offset, StarsU32 value)
{
    raw[offset] = (StarsU8)value;
    raw[(StarsU16)(offset + 1U)] = (StarsU8)(value >> 8);
    raw[(StarsU16)(offset + 2U)] = (StarsU8)(value >> 16);
    raw[(StarsU16)(offset + 3U)] = (StarsU8)(value >> 24);
}

static StarsI16 check(StarsI16 condition, StarsI16 *checks)
{
    ++*checks;
    return condition ? 0 : 1;
}

int main(void)
{
    StarsU8 raw[64];
    StarsSeeds seeds;
    StarsSeeds expected;
    StarsGenerateWorldGameState state;
    StarsGenerateWorldInput input;
    StarsGenerateWorldLayout layout;
    StarsI16 checks;
    StarsI16 failures;
    StarsI16 expected_value;
    StarsI16 actual_value;

    memset(raw, 0, sizeof raw);
    put32(raw, 0, 0x89abcdefUL);
    put16(raw, 4, 3U);
    put16(raw, 6, 2U);
    put16(raw, 8, 4U);
    put16(raw, 10, 17U);
    put16(raw, 12, 23U);
    put16(raw, 14, 0xa55aU);
    put16(raw, 16, 0x0121U);
    put16(raw, 18, 0xfffeU);

    stars_randomize2(&seeds, 0x12345678UL);
    checks = 0;
    failures = 0;
    failures += check(stars_generate_world_read_game_state(raw, &seeds,
        &state), &checks);
    failures += check(state.game_id == 0x89abcdefUL, &checks);
    failures += check(state.map_size == 3 && state.density == 2, &checks);
    failures += check(state.player_count == 4 && state.planet_max == 17,
        &checks);
    failures += check(state.start_distance == 23, &checks);
    failures += check(state.dirty == 0xa55aU &&
        state.generation_flags == 0x0121U, &checks);
    failures += check(state.turn == -2, &checks);
    failures += check(state.random.first == seeds.first &&
        state.random.second == seeds.second, &checks);

    expected = state.random;
    expected_value = stars_random(&expected, 997);
    actual_value = stars_generate_world_random_from_state(&state, 997);
    failures += check(actual_value == expected_value &&
        state.random.first == expected.first &&
        state.random.second == expected.second, &checks);
    expected_value = stars_random(&expected, 1);
    actual_value = stars_generate_world_random_from_state(&state, 1);
    failures += check(actual_value == expected_value &&
        state.random.first == expected.first &&
        state.random.second == expected.second, &checks);
    failures += check(stars_generate_world_random_from_state(&state, 0) == 0,
        &checks);
    failures += check(stars_generate_world_random_from_state(&state, -1) == 0,
        &checks);

    memset(&input, 0, sizeof input);
    memset(&layout, 0, sizeof layout);
    input.map_size = state.map_size;
    input.density = state.density;
    input.minimum_distance = state.start_distance;
    input.world_flags = state.generation_flags;
    input.random_value = stars_generate_world_random_from_state;
    input.context = &state;
    input.layout = &layout;
    failures += check(stars_generate_world_layout(&input), &checks);
    failures += check(layout.galaxy_extent == 1600 &&
        layout.galaxy_inverse == 0x0e10, &checks);
    failures += check(layout.point_count > 0 && layout.point_count <= 999,
        &checks);
    failures += check(state.random.first != seeds.first ||
        state.random.second != seeds.second, &checks);

    printf("GENERATE_WORLD_E1_CHECKS=%d FAILURES=%d RESULT=%s\n",
        (int)checks, (int)failures, failures == 0 ? "PASS" : "FAIL");
    return failures == 0 ? 0 : 1;
}
