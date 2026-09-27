#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct WriteLog {
    StarsU16 kinds[4];
    StarsU16 lengths[4];
    StarsU8 bytes[8];
    StarsU16 calls;
} WriteLog;

static void write_record(void *context, StarsU16 kind, StarsU16 length,
    const void *data)
{
    WriteLog *log = (WriteLog *)context;
    log->kinds[log->calls] = kind;
    log->lengths[log->calls] = length;
    if (kind == 0x000cU) memcpy(log->bytes, data, length);
    ++log->calls;
}

int main(void)
{
    StarsU8 messages[12];
    StarsOutgoingPlayerMessage outgoing[3];
    WriteLog log;
    int failures = 0;
    memset(&log, 0, sizeof log);
    messages[0] = 0x12U; messages[1] = 7U; messages[2] = 0U;
    messages[3] = 0x34U; messages[4] = 0x12U; messages[5] = 0x56U;
    messages[6] = 0x02U; messages[7] = 0xffU; messages[8] = 1U;
    messages[9] = 0x78U; messages[10] = 0x56U; messages[11] = 0U;
    outgoing[0].from = 1; outgoing[0].to = 0; outgoing[0].compressed_length = -4;
    outgoing[0].data = 0;
    outgoing[1].from = 2; outgoing[1].to = 0; outgoing[1].compressed_length = 2;
    outgoing[1].data = 0;
    outgoing[2].from = 3; outgoing[2].to = 3; outgoing[2].compressed_length = 3;
    outgoing[2].data = 0;
    stars_write_player_messages(messages, 12U, 2, outgoing, 3U,
        write_record, &log);
    if (log.calls != 3U || log.kinds[0] != 0x000cU || log.lengths[0] != 5U ||
        log.bytes[0] != 7U || log.bytes[1] != 0U || log.bytes[2] != 0x34U ||
        log.bytes[3] != 0x12U || log.bytes[4] != 0x56U ||
        log.kinds[1] != 0x0028U || log.lengths[1] != 0x0010U ||
        log.kinds[2] != 0x0028U || log.lengths[2] != 0x000fU)
        ++failures;
    memset(&log, 0, sizeof log);
    stars_write_player_messages(messages, 12U, -1, outgoing, 3U,
        write_record, &log);
    if (log.calls != 0U) ++failures;
    printf("MESSAGES_WRITE_CHECKS=2 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures != 0;
}
