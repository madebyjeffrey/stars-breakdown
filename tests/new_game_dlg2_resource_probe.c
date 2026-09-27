#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct ExpectedControl {
    StarsU16 id;
    StarsI16 x;
    StarsI16 y;
    StarsI16 width;
    StarsI16 height;
    StarsI16 text_length;
    StarsU16 text_checksum;
} ExpectedControl;

static const ExpectedControl expected[5] = {
    {1072U, 210, 190, 40, 14, 7, 647U},
    {118U, 10, 190, 40, 14, 5, 431U},
    {2U, 60, 190, 40, 14, 6, 582U},
    {1070U, 110, 190, 40, 14, 7, 499U},
    {1071U, 160, 190, 40, 14, 7, 547U}
};

static StarsI16 check(StarsI16 condition, StarsI16 *checks)
{
    ++*checks;
    return condition ? 0 : 1;
}

int main(void)
{
    StarsNewGameDlg2ResourceContract contract;
    StarsNewGameDlg2ResourceControl *actual;
    StarsI16 checks;
    StarsI16 failures;
    StarsI16 i;
    FILE *report;

    memset(&contract, 0, sizeof contract);
    checks = 0;
    failures = 0;
    stars_new_game_dlg2_resource_contract(&contract);
    failures += check(contract.resource_id == 0x0187U, &checks);
    failures += check(contract.width == 261, &checks);
    failures += check(contract.height == 210, &checks);
    failures += check(contract.font_size == 8, &checks);
    failures += check(contract.control_count == 5, &checks);
    for (i = 0; i < 5; ++i) {
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
    report = fopen("C:\\ND2RES\\NWD2RE2.TXT", "w");
    if (report) {
        fprintf(report,
            "NEW_GAME_DLG2_RESOURCE_E2_CHECKS=%d FAILURES=%d RESULT=%s\n",
            (int)checks, (int)failures,
            failures == 0 ? "PASS" : "FAIL");
        fclose(report);
    }
#ifndef TARGET_NATIVE
    printf("NEW_GAME_DLG2_RESOURCE_E2_CHECKS=%d FAILURES=%d RESULT=%s\n",
        (int)checks, (int)failures, failures == 0 ? "PASS" : "FAIL");
#endif
    return failures == 0 ? 0 : 1;
}
