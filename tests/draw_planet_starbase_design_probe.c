#include <stdio.h>
#include <string.h>
#include "core.h"

static void put16(StarsU8 *raw, StarsU16 offset, StarsU16 value)
{
    raw[offset] = (StarsU8)(value & 0xffU);
    raw[offset + 1U] = (StarsU8)(value >> 8);
}

int main(void)
{
    StarsU8 planet[56];
    StarsU8 designs[16 * 147];
    StarsU8 hulls[4 * 143];
    StarsPlanetStarbasePackedInput input;
    StarsPlanetStarbaseData data;
    int checks;
    int failures;
    StarsU16 design_base;
    StarsU16 hull_base;

    memset(planet, 0, sizeof planet);
    memset(designs, 0, sizeof designs);
    memset(hulls, 0, sizeof hulls);
    design_base = (StarsU16)(3U * 147U);
    hull_base = (StarsU16)(2U * 143U);
    put16(planet, 0U, 7U);
    put16(planet, 4U, 0x0200U);
    put16(planet, 44U, 0x0053U);
    put16(planet, 46U, 6U);
    put16(designs, design_base, 0x0022U);
    put16(hulls, (StarsU16)(hull_base + 52U), 200U);
    put16(hulls, (StarsU16)(hull_base + 56U), 37U);
    input.planet = planet;
    input.starbase_designs = designs;
    input.starbase_hulls = hulls;
    input.design_count = 16U;
    input.hull_count = 4U;
    checks = 0;
    failures = 0;
#define CHECK(condition) do { ++checks; if (!(condition)) ++failures; } while (0)
    CHECK(stars_decode_planet_starbase(&input, &data) == 1);
    CHECK(data.has_starbase == 1 && data.planet_id == 7);
    CHECK(data.starbase_design == 3 && data.hull_id == 0x22);
    CHECK(data.cargo_capacity == 200U && data.defense == 37U);
    CHECK(data.mass_warp_packed == 0x0053U && data.destination_id == 5);
    put16(planet, 4U, 0U);
    CHECK(stars_decode_planet_starbase(&input, &data) == 1 &&
        data.has_starbase == 0 && data.hull_id == -1 &&
        data.destination_id == -1);
    put16(planet, 4U, 0x0200U);
    input.design_count = 3U;
    CHECK(stars_decode_planet_starbase(&input, &data) == 0);
    input.design_count = 16U;
    input.hull_count = 2U;
    CHECK(stars_decode_planet_starbase(&input, &data) == 0);
    input.hull_count = 4U;
    input.planet = (const StarsU8 *)0;
    CHECK(stars_decode_planet_starbase(&input, &data) == 0);
    CHECK(checks == 10);
    printf("DRAW_PLANET_STARBASE_DESIGN_CHECKS=%d FAILURES=%d RESULT=%s\n",
        checks, failures, failures == 0 ? "PASS" : "FAIL");
    return failures ? 1 : 0;
#undef CHECK
}
