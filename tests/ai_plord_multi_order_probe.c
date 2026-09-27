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

static void seed(StarsU8 *plord, StarsU16 slot,
    const StarsMoveOrder *order)
{
    memcpy(plord + STARS_AI_PLORD_HEADER_SIZE +
        slot * STARS_AI_PLORD_ORDER_SIZE, order, sizeof(*order));
}

int ai_plord_multi_order_probe(void)
{
    StarsU8 plord[STARS_AI_PLORD_HEADER_SIZE +
        4U * STARS_AI_PLORD_ORDER_SIZE];
    StarsU8 before[sizeof(plord)];
    StarsMoveOrder first;
    StarsMoveOrder second;
    StarsMoveOrder third;
    StarsMoveOrder replacement;
    StarsMoveOrder expected;
    StarsI16 failures;

    failures = 0;
    memset(plord, 0xa5, sizeof(plord));
    fill_order(&first, 10, 11, 1, 0x1101U, 10U);
    fill_order(&second, 20, 21, 2, 0x1102U, 20U);
    fill_order(&third, 30, 31, 3, 0x1103U, 30U);
    fill_order(&replacement, 40, 41, 4, 0x1104U, 40U);
    seed(plord, 0U, &first);
    seed(plord, 1U, &second);
    plord[3] = 2U;

    /* Append to a two-order queue without disturbing either earlier order. */
    failures += !stars_ai_move_order_to_plord(plord, 4U, &third, 1);
    failures += plord[3] != 3U;
    failures += memcmp(plord + STARS_AI_PLORD_HEADER_SIZE, &first,
        sizeof(first)) != 0 ||
        memcmp(plord + STARS_AI_PLORD_HEADER_SIZE + STARS_AI_PLORD_ORDER_SIZE,
        &second, sizeof(second)) != 0 ||
        memcmp(plord + STARS_AI_PLORD_HEADER_SIZE +
        2U * STARS_AI_PLORD_ORDER_SIZE, &third, sizeof(third)) != 0;

    /* Appending at the existing last endpoint replaces that record in place. */
    fill_order(&replacement, 30, 31, 7, 0x1170U, 70U);
    failures += !stars_ai_move_order_to_plord(plord, 4U, &replacement, 1);
    failures += plord[3] != 3U;
    failures += memcmp(plord + STARS_AI_PLORD_HEADER_SIZE, &first,
        sizeof(first)) != 0 ||
        memcmp(plord + STARS_AI_PLORD_HEADER_SIZE + STARS_AI_PLORD_ORDER_SIZE,
        &second, sizeof(second)) != 0 ||
        memcmp(plord + STARS_AI_PLORD_HEADER_SIZE +
        2U * STARS_AI_PLORD_ORDER_SIZE, &replacement,
        sizeof(replacement)) != 0;

    /* Non-append to a matching first endpoint truncates the queue to one. */
    fill_order(&replacement, 10, 11, 8, 0x1180U, 80U);
    failures += !stars_ai_move_order_to_plord(plord, 4U, &replacement, 0);
    failures += plord[3] != 1U;
    failures += memcmp(plord + STARS_AI_PLORD_HEADER_SIZE, &replacement,
        sizeof(replacement)) != 0;

    /* Non-append to a different endpoint keeps the first, replaces slot two. */
    seed(plord, 0U, &first);
    seed(plord, 1U, &second);
    seed(plord, 2U, &third);
    plord[3] = 3U;
    fill_order(&replacement, 50, 51, 9, 0x1190U, 90U);
    failures += !stars_ai_move_order_to_plord(plord, 4U, &replacement, 0);
    failures += plord[3] != 2U;
    failures += memcmp(plord + STARS_AI_PLORD_HEADER_SIZE, &first,
        sizeof(first)) != 0 ||
        memcmp(plord + STARS_AI_PLORD_HEADER_SIZE + STARS_AI_PLORD_ORDER_SIZE,
        &replacement, sizeof(replacement)) != 0;

    /* A full four-record allocation refuses a further append transactionally. */
    seed(plord, 0U, &first);
    seed(plord, 1U, &second);
    seed(plord, 2U, &third);
    seed(plord, 3U, &replacement);
    plord[3] = 4U;
    memcpy(before, plord, sizeof(plord));
    fill_order(&expected, 60, 61, 10, 0x11a0U, 100U);
    failures += stars_ai_move_order_to_plord(plord, 4U, &expected, 1) != 0;
    failures += memcmp(plord, before, sizeof(plord)) != 0;

#ifndef PLORD_MULTI_NATIVE
    printf("AI_PLORD_MULTI_ORDER_CHECKS=15 FAILURES=%d RESULT=%s\n",
        (int)failures, failures == 0 ? "PASS" : "FAIL");
#endif
    return failures == 0 ? 0 : 1;
}

#ifndef PLORD_MULTI_NATIVE
int main(void)
{
    return ai_plord_multi_order_probe();
}
#endif
