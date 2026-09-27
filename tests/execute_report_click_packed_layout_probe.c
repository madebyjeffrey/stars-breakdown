#include <stdio.h>
#include <string.h>
#include "core.h"

static void put16(StarsU8 *raw, StarsU16 offset, StarsU16 value)
{
    raw[offset] = (StarsU8)(value & 0xffU);
    raw[offset + 1] = (StarsU8)(value >> 8);
}

static void put32(StarsU8 *raw, StarsU16 offset, StarsU32 value)
{
    put16(raw, offset, (StarsU16)(value & 0xffffUL));
    put16(raw, (StarsU16)(offset + 2), (StarsU16)(value >> 16));
}

static StarsU16 get16(const StarsU8 *raw, StarsU16 offset)
{
    return (StarsU16)(raw[offset] | ((StarsU16)raw[offset + 1] << 8));
}

static StarsU32 get32(const StarsU8 *raw, StarsU16 offset)
{
    return (StarsU32)get16(raw, offset) |
        ((StarsU32)get16(raw, (StarsU16)(offset + 2)) << 16);
}

int main(void)
{
    static StarsU8 fleet[124];
    static StarsU8 planet[56];
    static StarsU8 battle[14];
    static StarsU8 popup[22];
    StarsU16 failures;
    StarsU16 index;

    failures = 0;
    if (sizeof fleet != 124 || sizeof planet != 56 || sizeof battle != 14 ||
            sizeof popup != 22)
        ++failures;
    memset(fleet, 0, sizeof fleet);
    put16(fleet, 0, 40);
    put16(fleet, 6, 7);
    put16(fleet, 8, 10);
    put16(fleet, 10, 20);
    if (get16(fleet, 0) != 40 || get16(fleet, 6) != 7 ||
            get16(fleet, 8) != 10 || get16(fleet, 10) != 20)
        ++failures;
    memset(planet, 0, sizeof planet);
    put16(planet, 0, 7);
    put16(planet, 4, (StarsU16)(1U << 9));
    planet[9] = 80;
    planet[10] = 70;
    planet[11] = 60;
    put32(planet, 20, (5UL << 20) | (3UL << 8));
    put32(planet, 24, 1UL);
    for (index = 0; index < 4; ++index) {
        put16(planet, (StarsU16)(28 + index * 4),
            (StarsU16)(12 + index));
        put16(planet, (StarsU16)(30 + index * 4),
            (StarsU16)(8 + index));
    }
    if (get16(planet, 0) != 7 ||
            ((get16(planet, 4) >> 9) & 1U) != 1U ||
            planet[9] != 80 || planet[11] != 60 ||
            get32(planet, 20) != ((5UL << 20) | (3UL << 8)) ||
            (get32(planet, 24) & 0x0fffUL) != 1UL)
        ++failures;
    if (get16(planet, 28) != 12 || get16(planet, 30) != 8 ||
            get16(planet, 40) != 15 || get16(planet, 42) != 11)
        ++failures;
    memset(battle, 0, sizeof battle);
    put16(battle, 0, 2);
    put16(battle, 10, 30);
    put16(battle, 12, 40);
    if (get16(battle, 0) != 2 || get16(battle, 10) != 30 ||
            get16(battle, 12) != 40)
        ++failures;
    memset(popup, 0, sizeof popup);
    put16(popup, 0, 8);
    put16(popup, 2, 7);
    put16(popup, 4, 9);
    put16(popup, 6, 5);
    put16(popup, 8, 9);
    put16(popup, 10, 1);
    if (get16(popup, 0) != 8 || get16(popup, 2) != 7 ||
            get16(popup, 4) != 9 || get16(popup, 6) != 5 ||
            get16(popup, 8) != 9 || get16(popup, 10) != 1)
        ++failures;
    put16(popup, 0, 1);
    put16(popup, 2, 2);
    put16(popup, 10, 33);
    put16(popup, 12, 13);
    put16(popup, 14, 60);
    put16(popup, 18, 77);
    if (get16(popup, 0) != 1 || get16(popup, 2) != 2 ||
            get16(popup, 10) != 33 || get16(popup, 12) != 13 ||
            get16(popup, 14) != 60 || get16(popup, 18) != 77)
        ++failures;
    if (failures != 0) {
        puts("EXECUTE_REPORT_CLICK_PACKED_LAYOUT_CHECKS=16 FAILURES>0 RESULT=FAIL");
        return 1;
    }
    puts("EXECUTE_REPORT_CLICK_PACKED_LAYOUT_CHECKS=16 FAILURES=0 RESULT=PASS");
    return 0;
}
