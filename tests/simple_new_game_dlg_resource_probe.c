#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct ExpectedControl {
    StarsU16 id;
    StarsI16 x;
    StarsI16 y;
    StarsI16 width;
    StarsI16 height;
    StarsI16 text_length;
    StarsU16 text_checksum;
} ExpectedControl;

static const ExpectedControl expected[16] = {
    {200U, 16, 18, 60, 12, 4, 402U},
    {201U, 16, 32, 60, 12, 8, 817U},
    {202U, 16, 46, 60, 12, 6, 598U},
    {203U, 16, 60, 60, 12, 6, 632U},
    {1000U, 16, 90, 60, 12, 4, 420U},
    {1001U, 16, 104, 60, 12, 5, 505U},
    {1002U, 16, 118, 60, 12, 6, 609U},
    {1003U, 16, 132, 60, 12, 5, 491U},
    {1004U, 16, 146, 60, 12, 4, 393U},
    {2074U, 110, 18, 112, 76, 0, 0U},
    {210U, 110, 32, 112, 12, 18, 1550U},
    {211U, 110, 404, 112, 12, 17, 1376U},
    {212U, 8, 170, 76, 12, 15, 1407U},
    {1072U, 102, 170, 36, 12, 2, 186U},
    {2U, 148, 170, 36, 12, 6, 582U},
    {118U, 194, 170, 36, 12, 5, 431U}
};

static StarsI16 check(StarsI16 condition, StarsI16 *checks)
{
    ++*checks;
    return condition ? 0 : 1;
}

int main(void)
{
    StarsSimpleNewGameResourceContract contract;
    StarsSimpleNewGameResourceControl *actual;
    StarsI16 checks;
    StarsI16 failures;
    StarsI16 i;
    FILE *report;

    memset(&contract, 0, sizeof contract);
    checks = 0;
    failures = 0;
    stars_simple_new_game_resource_contract(&contract);
    failures += check(contract.resource_id == 0x00d1U, &checks);
    failures += check(contract.width == 240, &checks);
    failures += check(contract.height == 190, &checks);
    failures += check(contract.font_size == 8, &checks);
    failures += check(contract.control_count == 16, &checks);
    for (i = 0; i < 16; ++i) {
        actual = &contract.controls[i];
        failures += check(actual->id == expected[i].id, &checks);
        failures += check(actual->x == expected[i].x, &checks);
        failures += check(actual->y == expected[i].y, &checks);
        failures += check(actual->width == expected[i].width, &checks);
        failures += check(actual->height == expected[i].height, &checks);
        failures += check(actual->text_length == expected[i].text_length,
            &checks);
        failures += check(actual->text_checksum == expected[i].text_checksum,
            &checks);
    }
    report = fopen("C:\\SNGDR\\SNGDRE2.TXT", "w");
    if (report) {
        fprintf(report,
            "SIMPLE_NEW_GAME_DLG_RESOURCE_E2_CHECKS=%d FAILURES=%d RESULT=%s\n",
            (int)checks, (int)failures,
            failures == 0 ? "PASS" : "FAIL");
        fclose(report);
    }
#ifndef TARGET_NATIVE
    printf("SIMPLE_NEW_GAME_DLG_RESOURCE_E2_CHECKS=%d FAILURES=%d RESULT=%s\n",
        (int)checks, (int)failures, failures == 0 ? "PASS" : "FAIL");
#endif
    return failures == 0 ? 0 : 1;
}
