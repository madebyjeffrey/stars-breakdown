#include "core.h"

#define SCANNER_DIRECTION_COUNT 8
#define SCANNER_COORDINATE_OFFSET 1000
#define SCANNER_MAP_TILE 400
#define SCANNER_MAP_MARGIN 400
#define SCANNER_OBJECT_CLASS 0x21
#define SCANNER_NO_PLANET (-1)

static StarsI32 scanner_distance_squared(StarsPoint a, StarsPoint b)
{
    StarsI32 dx = (StarsI32)a.x - b.x;
    StarsI32 dy = (StarsI32)a.y - b.y;
    return dx * dx + dy * dy;
}

static StarsI16 scanner_random(const StarsIdGetBestScannerDestInput *input,
    StarsI16 limit)
{
    StarsI16 value;
    if (!input->random_value || limit <= 0)
        return 0;
    value = input->random_value(input->context, limit);
    return value < 0 || value >= limit ? 0 : value;
}

static StarsPoint scanner_direction_point(StarsI16 direction,
    StarsPoint source, StarsI16 map_edge)
{
    StarsI32 x = (StarsI32)source.x - SCANNER_COORDINATE_OFFSET;
    StarsI32 y = (StarsI32)source.y - SCANNER_COORDINATE_OFFSET;
    StarsI32 span = map_edge;
    StarsPoint result;
    switch (direction) {
    case 0: x = span; break;
    case 1:
        if (y < span - x) { x += y; y = 0; }
        else { y -= span - x; x = span; }
        break;
    case 2: y = 0; break;
    case 3:
        if (y < x) { x -= y; y = 0; }
        else { y -= x; x = 0; }
        break;
    case 4: x = 0; break;
    case 5:
        if (y < span - x) { y += x; x = 0; }
        else { x -= span - y; y = span; }
        break;
    case 6: y = span; break;
    case 7:
        if (y < x) { y += span - x; x = span; }
        else { x += span - y; y = span; }
        break;
    default: result.x = -1; result.y = -1; return result;
    }
    result.x = (StarsI16)(x + SCANNER_COORDINATE_OFFSET);
    result.y = (StarsI16)(y + SCANNER_COORDINATE_OFFSET);
    return result;
}

/* Genuine semantic reconstruction of IdGetBestScannerDest, NE 22:282A-2BC0.
 *
 * The scanner chooses a point on one of eight map-edge directions, jitters it
 * by a random amount related to the fleet's warp reach, asks the nearest-object
 * service for a planet, and rejects friendly or too-near results.  The public
 * interface keeps the original nearest-object callback and the map/warp inputs
 * explicit; the callback's object-class filtering corresponds to the original
 * FFindNearestObject(..., 0x21, ... ) call. */
StarsI16 stars_id_get_best_scanner_dest(StarsI16 direction,
    const StarsIdGetBestScannerDestInput *input)
{
    StarsPoint point;
    StarsU16 planet_id = 0;
    StarsU16 index;
    StarsI16 jitter;
    StarsI16 edge;
    StarsI32 minimum_distance;
    StarsI32 actual_distance;

    if (!input || !input->find_object ||
            direction < 0 || direction >= SCANNER_DIRECTION_COUNT)
        return SCANNER_NO_PLANET;
    edge = (StarsI16)(input->map_size * SCANNER_MAP_TILE +
        SCANNER_MAP_MARGIN);
    point = scanner_direction_point(direction, input->source_point, edge);
    jitter = scanner_random(input, (StarsI16)(input->warp_distance + 3));
    if (point.x == 0 || point.x == edge + SCANNER_COORDINATE_OFFSET)
        point.y = (StarsI16)(point.y + jitter -
            scanner_random(input, (StarsI16)(input->warp_distance + 3)));
    else
        point.x = (StarsI16)(point.x + jitter -
            scanner_random(input, (StarsI16)(input->warp_distance + 3)));
    point.x = (StarsI16)(point.x < 0 ? 0 : point.x);
    point.y = (StarsI16)(point.y < 0 ? 0 : point.y);
    if (point.x > edge + SCANNER_COORDINATE_OFFSET)
        point.x = (StarsI16)(edge + SCANNER_COORDINATE_OFFSET);
    if (point.y > edge + SCANNER_COORDINATE_OFFSET)
        point.y = (StarsI16)(edge + SCANNER_COORDINATE_OFFSET);
    if (!input->find_object(input->context, point, &planet_id))
        return SCANNER_NO_PLANET;
    for (index = 0; index < input->planet_count; ++index)
        if (input->planets[index].id == planet_id)
            break;
    if (index == input->planet_count || input->planets[index].owner == 0)
        return SCANNER_NO_PLANET;
    minimum_distance = (StarsI32)(input->warp_distance + 3);
    minimum_distance *= minimum_distance;
    actual_distance = scanner_distance_squared(point,
        input->planets[index].point);
    return actual_distance >= minimum_distance ? (StarsI16)planet_id :
        SCANNER_NO_PLANET;
}
