#include <stdio.h>
#include "core.h"

int main(void)
{
    static const StarsBattleRecord a[] = {{1U, 4U}, {2U, 6U}, {0xffffU, 0U}};
    static const StarsBattleRecord b[] = {{3U, 8U}, {0xffffU, 0U}};
    static const StarsBattleRecord c[] = {{4U, 0U}};
    StarsBattleBlock third;
    StarsBattleBlock second;
    StarsBattleBlock first;
    third.records = c; third.record_count = 1U; third.ib_top = 0x20U;
    third.next = 0;
    second.records = b; second.record_count = 2U; second.ib_top = 0x20U;
    second.next = &third;
    first.records = a; first.record_count = 3U; first.ib_top = 0x20U;
    first.next = &second;
    if (stars_count_battles(&first) != 3) return 1;
    second.ib_top = 0x10U;
    if (stars_count_battles(&first) != 2) return 2;
    first.records = c; first.record_count = 1U; first.next = 0;
    if (stars_count_battles(&first) != 0) return 3;
    puts("BATTLES_SOURCE=PASS");
    return 0;
}
