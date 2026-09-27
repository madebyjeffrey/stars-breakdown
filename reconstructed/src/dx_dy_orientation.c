#include <math.h>
#include "core.h"

/* NE 12:987C-9974. Convert a vector to the original eight-way orientation,
 * then select the scan-size and anchor values from iScanZoom. */
void stars_get_dx_dy_orientation(StarsI16 dx, StarsI16 dy,
    const StarsDxDyOrientationInput *input, StarsPoint *point,
    StarsPoint *point_delta)
{
    StarsU16 orientation = 0U;
    StarsI16 scale;
    double angle;
    double sector;
    if (!input || !point || !point_delta) return;
    if (dx != 0 || dy != 0) {
        angle = atan2((double)dy, (double)dx);
        sector = (angle + 3.14159265358979323846) *
            4.0 / 3.14159265358979323846 + 0.5;
        orientation = (StarsU16)((StarsI32)sector & 7L);
        orientation = (StarsU16)((9U - orientation) & 7U);
    }
    scale = input->scan_zoom < 0 ? 7 : 9;
    point_delta->x = scale;
    point_delta->y = scale;
    point->x = input->scan_zoom < 0 ? 0 : 7;
    point->y = (StarsI16)(orientation * (StarsU16)scale);
}
