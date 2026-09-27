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

static const ExpectedControl page_one[27] = {
    {0xffffU, 10, 10, 48, 10, 11, 891U},
    {1030U, 64, 8, 150, 12, 0, 0U},
    {1000U, 24, 40, 44, 12, 4, 420U},
    {1001U, 24, 52, 44, 12, 5, 505U},
    {1002U, 24, 64, 44, 12, 6, 609U},
    {1003U, 24, 76, 44, 12, 5, 491U},
    {1004U, 24, 88, 44, 12, 4, 393U},
    {1005U, 24, 124, 44, 12, 6, 622U},
    {1006U, 24, 136, 44, 12, 6, 617U},
    {1007U, 24, 148, 44, 12, 5, 495U},
    {1008U, 24, 160, 44, 12, 6, 584U},
    {1009U, 96, 40, 48, 12, 5, 502U},
    {1010U, 96, 52, 48, 12, 8, 817U},
    {1011U, 96, 64, 48, 12, 7, 716U},
    {1012U, 96, 76, 48, 12, 7, 727U},
    {1016U, 90, 94, 140, 12, 28, 2563U},
    {1017U, 90, 107, 140, 12, 21, 1931U},
    {1018U, 90, 120, 140, 12, 21, 1824U},
    {1019U, 90, 133, 140, 12, 17, 1529U},
    {1020U, 90, 146, 140, 12, 32, 3029U},
    {1021U, 90, 159, 140, 12, 21, 1953U},
    {1050U, 90, 172, 140, 12, 16, 1515U},
    {118U, 10, 190, 40, 14, 5, 431U},
    {2U, 60, 190, 40, 14, 6, 582U},
    {1070U, 110, 190, 40, 14, 7, 499U},
    {1071U, 160, 190, 40, 14, 7, 547U},
    {1072U, 210, 190, 40, 14, 7, 647U}
};

static const ExpectedControl page_two[5] = {
    {1072U, 210, 190, 40, 14, 7, 647U},
    {118U, 10, 190, 40, 14, 5, 431U},
    {2U, 60, 190, 40, 14, 6, 582U},
    {1070U, 110, 190, 40, 14, 7, 499U},
    {1071U, 160, 190, 40, 14, 7, 547U}
};

static const ExpectedControl page_three[13] = {
    {0xffffU, 22, 10, 200, 12, 34, 3194U},
    {291U, 10, 26, 12, 12, 0, 0U},
    {292U, 10, 42, 12, 12, 0, 0U},
    {293U, 10, 58, 12, 12, 0, 0U},
    {294U, 10, 74, 12, 12, 0, 0U},
    {295U, 10, 90, 12, 12, 0, 0U},
    {296U, 10, 106, 12, 12, 0, 0U},
    {297U, 10, 122, 12, 12, 0, 0U},
    {118U, 10, 190, 40, 14, 5, 431U},
    {2U, 60, 190, 40, 14, 6, 582U},
    {1070U, 110, 190, 40, 14, 7, 499U},
    {1071U, 160, 190, 40, 14, 7, 547U},
    {1072U, 210, 190, 40, 14, 7, 647U}
};

static StarsI16 check(StarsI16 condition, StarsI16 *checks)
{
    ++*checks;
    return condition ? 0 : 1;
}

static StarsI16 check_page(const StarsNewGameWizardResourcePage *page,
    StarsU16 resource_id, StarsI16 count, const ExpectedControl *expected,
    StarsI16 *checks)
{
    StarsI16 failures;
    StarsI16 i;
    const StarsNewGameWizardResourceControl *actual;

    failures = 0;
    failures += check(page->resource_id == resource_id, checks);
    failures += check(page->width == 261, checks);
    failures += check(page->height == 210, checks);
    failures += check(page->font_size == 8, checks);
    failures += check(page->control_count == count, checks);
    for (i = 0; i < count; ++i) {
        actual = &page->controls[i];
        failures += check(actual->id == expected[i].id, checks);
        failures += check(actual->x == expected[i].x, checks);
        failures += check(actual->y == expected[i].y, checks);
        failures += check(actual->width == expected[i].width, checks);
        failures += check(actual->height == expected[i].height, checks);
        failures += check(actual->text_length == expected[i].text_length,
            checks);
        failures += check(actual->text_checksum == expected[i].text_checksum,
            checks);
    }
    for (i = count; i < 27; ++i) {
        actual = &page->controls[i];
        failures += check(actual->id == 0 && actual->x == 0 && actual->y == 0 &&
            actual->width == 0 && actual->height == 0 &&
            actual->text_length == 0 && actual->text_checksum == 0U,
            checks);
    }
    return failures;
}

int main(void)
{
    StarsNewGameWizardResourceContract contract;
    StarsI16 checks;
    StarsI16 failures;
    FILE *report;

    memset(&contract, 0, sizeof contract);
    checks = 0;
    failures = 0;
    stars_new_game_wizard_resource_contract(&contract);
    failures += check_page(&contract.pages[0], 0x0186U, 27, page_one,
        &checks);
    failures += check_page(&contract.pages[1], 0x0187U, 5, page_two,
        &checks);
    failures += check_page(&contract.pages[2], 0x0188U, 13, page_three,
        &checks);
    report = fopen("C:\\NWRES\\NWRESE2.TXT", "w");
    if (report) {
        fprintf(report,
            "NEW_GAME_WIZARD_RESOURCE_E2_CHECKS=%d FAILURES=%d RESULT=%s\n",
            (int)checks, (int)failures,
            failures == 0 ? "PASS" : "FAIL");
        fclose(report);
    }
#ifndef TARGET_NATIVE
    printf("NEW_GAME_WIZARD_RESOURCE_E2_CHECKS=%d FAILURES=%d RESULT=%s\n",
        (int)checks, (int)failures, failures == 0 ? "PASS" : "FAIL");
#endif
    return failures == 0 ? 0 : 1;
}
