#include "core.h"
#include <stdio.h>
#include <string.h>

static void fill_order(StarsMoveOrder *order, StarsI16 x, StarsI16 y,
    StarsI16 id, StarsU16 flags, StarsU8 marker)
{
    StarsU16 i;

    memset(order, 0, sizeof(*order));
    order->point.x = x;
    order->point.y = y;
    order->id = id;
    order->flags = flags;
    for (i = 0; i < sizeof(order->reserved); ++i)
        order->reserved[i] = (StarsU8)(marker + i);
}

int main(void)
{
    StarsU8 plord[STARS_AI_PLORD_HEADER_SIZE + 3U *
        STARS_AI_PLORD_ORDER_SIZE];
    StarsU8 before[sizeof(plord)];
    StarsMoveOrder first;
    StarsMoveOrder second;
    StarsMoveOrder replacement;
    StarsI16 failures;

    memset(plord, 0xa5, sizeof(plord));
    plord[3] = 1;
    fill_order(&first, 10, 20, 4, 0x1102U, 1);
    fill_order(&second, 30, 40, 8, 0x1140U, 20);
    fill_order(&replacement, 10, 20, 9, 0x1161U, 40);
    memcpy(plord + 4, &first, sizeof(first));
    failures = !stars_ai_move_order_to_plord(plord, 3, &first, 0);
    failures += plord[3] != 1U;
    failures += memcmp(plord + 4, &first, sizeof(first)) != 0;

    failures += !stars_ai_move_order_to_plord(plord, 3, &second, 1);
    failures += plord[3] != 2U;
    failures += memcmp(plord + 4 + STARS_AI_PLORD_ORDER_SIZE,
        &second, sizeof(second)) != 0;

    memcpy(before, plord, sizeof(plord));
    replacement.point = second.point;
    failures += !stars_ai_move_order_to_plord(plord, 3, &replacement, 1);
    failures += plord[3] != 2U;
    failures += memcmp(plord + 4 + STARS_AI_PLORD_ORDER_SIZE,
        &replacement, sizeof(replacement)) != 0;
    failures += memcmp(plord + 4, before + 4, STARS_AI_PLORD_ORDER_SIZE) != 0;
    plord[3] = 3U;
    memcpy(before, plord, sizeof(plord));
    failures += stars_ai_move_order_to_plord(plord, 3, &first, 1) != 0;
    failures += memcmp(plord, before, sizeof(plord)) != 0;

    printf("AI_PLORD_MOVE_ORDER_CHECKS=12 FAILURES=%d RESULT=%s\n",
        (int)failures, failures == 0 ? "PASS" : "FAIL");
    return failures == 0 ? 0 : 1;
}
