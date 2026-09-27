#include <stdio.h>
#include "core.h"

int main(void)
{
    StarsBattleDataRecord first_records[3];
    StarsBattleDataRecord second_records[3];
    StarsBattleDataBlock first, second;
    StarsBattleDataInput input;
    first_records[0].id=10U; first_records[0].cb_data=14U;
    first_records[1].id=11U; first_records[1].cb_data=16U;
    first_records[2].id=0xffffU; first_records[2].cb_data=0U;
    second_records[0].id=12U; second_records[0].cb_data=14U;
    second_records[1].id=0xffffU; second_records[1].cb_data=0U;
    second.ib_top=0x11U; second.records=second_records; second.record_count=2U; second.next=0;
    first.ib_top=0x20U; first.records=first_records; first.record_count=3U; first.next=&second;
    input.first=&first;
    if (stars_btl_data_get(&input, 0)->id != 10U) return 1;
    if (stars_btl_data_get(&input, 1)->id != 11U) return 2;
    if (stars_btl_data_get(&input, 2)->id != 12U) return 3;
    if (stars_btl_data_get(&input, -1)->id != 10U) return 4;
    second.ib_top=0x10U;
    if (stars_btl_data_get(&input, 2) != 0) return 5;
    second.ib_top=0x11U; first_records[1].cb_data=0U;
    if (stars_btl_data_get(&input, 1) != 0) return 6;
    puts("BTL_DATA_SOURCE=PASS");
    return 0;
}
