#include "core.h"
#include <string.h>

/* NE 19:3D0A and the personality-specific fallback branches change only the
 * low order-kind nibble of the first packed record. The raw code repeatedly
 * performs (word & 0xfff0) | 5 or | 6 after selecting the fleet. Keep that
 * operation separate from first-order repair so an existing queue count and
 * every other order byte remain untouched. */
StarsI16 stars_ai_set_first_order_kind(StarsU8 *plord, StarsU16 kind)
{
    StarsU16 flags;

    if (!plord || kind > 0x000fU)
        return 0;
    flags = (StarsU16)(plord[10] | ((StarsU16)plord[11] << 8));
    flags = (StarsU16)((flags & 0xfff0U) | kind);
    plord[10] = (StarsU8)flags;
    plord[11] = (StarsU8)(flags >> 8);
    return 1;
}

/* NE 19:3D0A callers use this packed repair when an AI fleet has no first
 * order. PLORD is a four-byte header followed by 18-byte order records: the
 * first record's order-kind word is at header+6, its two repair words are at
 * header+8 and header+10.  The original repair branches write no iordMac
 * byte: preserve both header counts and all unrelated order bytes, including
 * any queued orders after the first. */
StarsI16 stars_ai_repair_first_order(StarsU8 *plord)
{
    if (!plord || !stars_ai_set_first_order_kind(plord, 6U))
        return 0;
    plord[12] = 5U;
    plord[13] = 0U;
    plord[14] = 5U;
    plord[15] = 0U;
    return 1;
}

/* NE 19:3D0A. Mirror FMoveAiFleet's packed copy path after the typed order
 * has been constructed by FColonizeAiFleet, FGotoWormholeAiFleet, or one of
 * the personality-specific target helpers.  The caller supplies the current
 * allocation capacity because the packed allocator is owned by the fleet
 * selection boundary. */
StarsI16 stars_ai_move_order_to_plord(StarsU8 *plord, StarsU16 capacity,
    const StarsMoveOrder *order, StarsI16 append)
{
    StarsU16 count;
    StarsU16 index;
    StarsU16 offset;
    StarsI16 same_endpoint;

    if (!plord || !order || capacity == 0U)
        return 0;
    count = plord[3];
    if (count >= capacity)
        return 0;
    if (append && count != 0U)
        index = (StarsU16)(count - 1U);
    else
        index = 0U;
    same_endpoint = 0;
    if (count != 0U) {
        offset = (StarsU16)(STARS_AI_PLORD_HEADER_SIZE +
            index * STARS_AI_PLORD_ORDER_SIZE);
        same_endpoint = plord[offset] == (StarsU8)order->point.x &&
            plord[(StarsU16)(offset + 1U)] ==
                (StarsU8)((StarsU16)order->point.x >> 8) &&
            plord[(StarsU16)(offset + 2U)] == (StarsU8)order->point.y &&
            plord[(StarsU16)(offset + 3U)] ==
                (StarsU8)((StarsU16)order->point.y >> 8);
    }
    if (same_endpoint) {
        if (count > 1U)
            count = 1U;
    } else {
        ++index;
    }
    if (index >= capacity)
        return 0;
    offset = (StarsU16)(STARS_AI_PLORD_HEADER_SIZE +
        index * STARS_AI_PLORD_ORDER_SIZE);
    memcpy(plord + offset, order, sizeof(*order));
    plord[3] = (StarsU8)(index + 1U);
    return 1;
}
