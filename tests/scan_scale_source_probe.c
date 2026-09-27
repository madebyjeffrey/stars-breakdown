#include "core.h"
#include <stdio.h>

int main(void)
{
    int failures = 0;
    StarsI16 zoom;
    const StarsI16 values[] = {1, 2, 3, 4, -4, -3, -2, -1, 0, 5};
    const StarsI16 forward[] = {8, 10, 14, 28, 1, 2, 3, 5, 7, 7};
    const StarsI16 reverse[] = {5, 4, 3, 1, 28, 18, 14, 9, 7, 7};

    for (zoom = 0; zoom < 10; ++zoom) {
        if (stars_pt_to_scan(7, values[zoom]) != forward[zoom])
            ++failures;
        if (stars_scan_to_pt(7, values[zoom]) != reverse[zoom])
            ++failures;
    }
    if (stars_pt_to_scan(7, 1) != 8)
        ++failures;
    if (stars_scan_to_pt(7, 1) != 5)
        ++failures;

    printf("SCAN_SCALE_CHECKS=22 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures != 0;
}
