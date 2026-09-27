#include "core.h"
#include <stdio.h>

static StarsI16 check(StarsI16 condition, StarsI16 *checks)
{
    ++*checks;
    return condition ? 0 : 1;
}

int main(void)
{
    static const StarsNewGameDlgResourceControl expected[27] = {
        {0xffffU, 10, 10, 48, 10}, {1030U, 64, 8, 150, 12},
        {1000U, 24, 40, 44, 12}, {1001U, 24, 52, 44, 12},
        {1002U, 24, 64, 44, 12}, {1003U, 24, 76, 44, 12},
        {1004U, 24, 88, 44, 12}, {1005U, 24, 124, 44, 12},
        {1006U, 24, 136, 44, 12}, {1007U, 24, 148, 44, 12},
        {1008U, 24, 160, 44, 12}, {1009U, 96, 40, 48, 12},
        {1010U, 96, 52, 48, 12}, {1011U, 96, 64, 48, 12},
        {1012U, 96, 76, 48, 12}, {1016U, 90, 94, 140, 12},
        {1017U, 90, 107, 140, 12}, {1018U, 90, 120, 140, 12},
        {1019U, 90, 133, 140, 12}, {1020U, 90, 146, 140, 12},
        {1021U, 90, 159, 140, 12}, {1050U, 90, 172, 140, 12},
        {118U, 10, 190, 40, 14}, {2U, 60, 190, 40, 14},
        {1070U, 110, 190, 40, 14}, {1071U, 160, 190, 40, 14},
        {1072U, 210, 190, 40, 14}
    };
    StarsNewGameDlgResourceContract contract;
    StarsI16 checks;
    StarsI16 failures;
    StarsI16 i;
    FILE *report;

    stars_new_game_dlg_resource_contract(&contract);
    checks = 0;
    failures = 0;
    failures += check(contract.resource_id == 0x0186U, &checks);
    failures += check(contract.width == 261 && contract.height == 210,
        &checks);
    failures += check(contract.font_size == 8 && contract.control_count == 27,
        &checks);
    for (i = 0; i < 27; ++i) {
        failures += check(contract.controls[i].id == expected[i].id &&
            contract.controls[i].x == expected[i].x &&
            contract.controls[i].y == expected[i].y &&
            contract.controls[i].width == expected[i].width &&
            contract.controls[i].height == expected[i].height, &checks);
    }
    report = fopen("C:\\NGDR\\NGDPE2.TXT", "w");
    if (report) {
        fprintf(report,
            "NEW_GAME_DLG_RESOURCE_E2_CHECKS=%d FAILURES=%d RESULT=%s\n",
            (int)checks, (int)failures,
            failures == 0 ? "PASS" : "FAIL");
        fclose(report);
    }
#ifndef TARGET_NATIVE
    printf("NEW_GAME_DLG_RESOURCE_E2_CHECKS=%d FAILURES=%d RESULT=%s\n",
        (int)checks, (int)failures, failures == 0 ? "PASS" : "FAIL");
#endif
    return failures == 0 ? 0 : 1;
}
