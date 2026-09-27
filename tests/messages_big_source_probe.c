#include "core.h"
#include <stdio.h>
#include <string.h>

static StarsU16 count(void *context, StarsI16 message)
{
    (void)context;
    return message == 7 ? 2U : 1U;
}

int main(void)
{
    StarsMessageOps ops;
    StarsMessageBig big;
    StarsU8 records[16];
    int failures = 0;

    memset(&ops, 0, sizeof ops);
    memset(&big, 0, sizeof big);
    ops.parameter_count = count;
    /* Record 0: message 3, goto 0x1234, one byte parameter 0x56. */
    records[0] = 3U; records[1] = 0U;
    records[2] = 0x34U; records[3] = 0x12U; records[4] = 0x56U;
    /* Record 1: message 7, goto 0x5678, byte 0x78, word 0x9abc. */
    records[5] = 7U; records[6] = 0x04U;
    records[7] = 0x78U; records[8] = 0x56U;
    records[9] = 0x78U; records[10] = 0xbcU; records[11] = 0x9aU;
    if (!stars_get_message_big(records, 12U, 2U, 0, &ops, 0, &big) ||
        big.message != 3 || big.goto_id != 0x1234 || big.parameters[0] != 0x56)
        ++failures;
    if (!stars_get_message_big(records, 12U, 2U, 1, &ops, 0, &big) ||
        big.message != 7 || big.goto_id != 0x5678 ||
        big.parameters[0] != 0x78 || big.parameters[1] != (StarsI16)0x9abc)
        ++failures;
    if (stars_get_message_big(records, 12U, 2U, -1, &ops, 0, &big) != 0 ||
        stars_get_message_big(records, 12U, 2U, 2, &ops, 0, &big) != 0)
        ++failures;
    printf("MESSAGES_BIG_CHECKS=3 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures != 0;
}
