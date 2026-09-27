#include "core.h"
#include <stdio.h>
#include <string.h>

int main(void)
{
    StarsPlayerMessage records[3];
    StarsU16 det[4];
    int failures = 0;
    records[0].from = 1; records[0].to = 0;
    records[1].from = 2; records[1].to = 3;
    records[2].from = 3; records[2].to = 3;
    det[0] = det[1] = det[2] = det[3] = 0x0200U;
    stars_mark_players_that_sent_messages(2, records, 3U, det, 4U);
    if (det[1] != 0x0303U || det[2] != 0x0303U || det[3] != 0x0303U)
        ++failures;
    memset(det, 0, sizeof det);
    stars_mark_players_that_sent_messages(-1, records, 3U, det, 4U);
    if (det[1] != 0U || det[2] != 0U || det[3] != 0U)
        ++failures;
    printf("MESSAGES_PLAYERS_CHECKS=2 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures != 0;
}
