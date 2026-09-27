#include <stdio.h>
#include "core.h"

int main(void)
{
    StarsDxDyOrientationInput input;
    StarsPoint point, delta;
    input.scan_zoom = 1;
    stars_get_dx_dy_orientation(0, 0, &input, &point, &delta);
    if (point.x != 7 || point.y != 0 || delta.x != 9 || delta.y != 9)
        return 1;
    stars_get_dx_dy_orientation(1, 0, &input, &point, &delta);
    if (point.x != 7 || point.y != 45) return 2;
    stars_get_dx_dy_orientation(0, -1, &input, &point, &delta);
    if (point.y != 63) return 3;
    input.scan_zoom = -1;
    stars_get_dx_dy_orientation(-1, 0, &input, &point, &delta);
    if (point.x != 0 || point.y != 7 || delta.x != 7 || delta.y != 7)
        return 4;
    puts("DX_DY_ORIENTATION_SOURCE=PASS");
    return 0;
}
