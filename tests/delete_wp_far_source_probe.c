#include <stdio.h>
#include <string.h>
#include "core.h"

static void fill(StarsU8 *records, StarsU16 count)
{
    StarsU16 i;
    StarsU16 j;
    for (i = 0; i < count; ++i)
        for (j = 0; j < 18U; ++j)
            records[i * 18U + j] = (StarsU8)(i * 20U + j);
}

static int ordinary_delete(void)
{
    StarsDeleteWpFarInput input;
    StarsU8 records[72];
    StarsU8 iord_mac;
    fill(records, 4U);
    iord_mac = 4U;
    input.cord = 4U;
    input.iord_mac = &iord_mac;
    input.records = records;
    stars_delete_wp_far(&input, 1U, 0U);
    return input.cord == 3U && iord_mac == 3U && records[18] == 40U &&
        records[36] == 60U;
}

static int recycle_delete(void)
{
    StarsDeleteWpFarInput input;
    StarsU8 records[72];
    StarsU8 iord_mac;
    StarsU8 saved[18];
    fill(records, 4U);
    memcpy(saved, records + 18, sizeof(saved));
    iord_mac = 4U;
    input.cord = 4U;
    input.iord_mac = &iord_mac;
    input.records = records;
    stars_delete_wp_far(&input, 1U, 1U);
    return input.cord == 4U && iord_mac == 4U && records[18] == 40U &&
        memcmp(records + 54, saved, sizeof(saved)) == 0;
}

static int recycle_fallback(void)
{
    StarsDeleteWpFarInput input;
    StarsU8 records[54];
    StarsU8 iord_mac;
    fill(records, 3U);
    memcpy(records + 36, records, 4U);
    iord_mac = 3U;
    input.cord = 3U;
    input.iord_mac = &iord_mac;
    input.records = records;
    stars_delete_wp_far(&input, 0U, 1U);
    return input.cord == 2U && iord_mac == 2U && records[0] == 20U;
}

static int recycle_last_waypoint(void)
{
    StarsDeleteWpFarInput input;
    StarsU8 records[36];
    StarsU8 original[36];
    StarsU8 iord_mac = 2U;
    fill(records, 2U);
    memcpy(original, records, sizeof original);
    input.cord = 2U;
    input.iord_mac = &iord_mac;
    input.records = records;
    stars_delete_wp_far(&input, 1U, 1U);
    return input.cord == 1U && iord_mac == 1U &&
        memcmp(records, original, sizeof records) == 0;
}

int main(void)
{
    if (!ordinary_delete() || !recycle_delete() || !recycle_fallback() ||
            !recycle_last_waypoint())
        return 1;
    puts("DELETE_WP_FAR_SOURCE=PASS");
    return 0;
}
