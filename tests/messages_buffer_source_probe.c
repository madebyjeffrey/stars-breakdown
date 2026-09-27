#include "core.h"
#include <stdio.h>

int main(void)
{
    StarsU8 records[16];
    int failures = 0;
    /* Two five-byte records with distinct player, message and object fields. */
    records[0] = 0x02U; records[1] = 7U; records[2] = 0U;
    records[3] = 0x34U; records[4] = 0x12U;
    records[5] = 0x03U; records[6] = 9U; records[7] = 0U;
    records[8] = 0x78U; records[9] = 0x56U;
    if (!stars_find_player_message(records, 10U, 2, 7, 0x1234) ||
        stars_find_player_message(records, 10U, 2, 9, 0x1234) ||
        stars_remove_player_message(records, 10U, 2, 7, 0x1234) != 1 ||
        (records[1] & 0xffU) != 0xffU || records[2] != 0U ||
        stars_find_player_message(records, 10U, 2, 7, 0x1234))
        ++failures;
    if (stars_remove_player_message(records, 10U, 2, 7, 0x1234) != 0 ||
        stars_remove_player_message(records, 10U, 3, 9, 0x5678) != 1)
        ++failures;
    printf("MESSAGES_BUFFER_CHECKS=2 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures != 0;
}
