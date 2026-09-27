#include <stdio.h>
#include <string.h>
#include "core.h"

void stars_stream_open(StarsU8 *data, StarsU16 name, StarsI16 mode,
    const StarsStreamApi *api, void *context)
{ (void)data; (void)name; (void)mode; (void)api; (void)context; }
void stars_stream_write(StarsU8 *data, const StarsU8 *input, StarsU16 count,
    const StarsStreamApi *api, void *context)
{ (void)data; (void)input; (void)count; (void)api; (void)context; }
void stars_write_bof(StarsU8 *data, StarsI16 player, StarsI16 type,
    StarsI16 multi, StarsTickCount ticks, StarsWriteTransfer write,
    void *context)
{
    (void)data; (void)player; (void)type; (void)multi; (void)ticks;
    (void)write; (void)context;
}

static StarsU8 *next_block(void *context, StarsU8 *current, StarsU16 size,
    StarsU16 *offset, StarsU16 *next_size)
{
    (void)current;
    (void)size;
    *offset = 0U;
    *next_size = 64U;
    return (StarsU8 *)context;
}

static StarsU16 word(const StarsU8 *data, StarsU16 at)
{ return (StarsU16)(data[at] | ((StarsU16)data[at + 1U] << 8)); }

int main(void)
{
    StarsU8 block[64];
    StarsU8 next[64];

    memset(block, 0, sizeof block);
    block[0] = 1U;
    block[3] = 0U;
    block[6] = 32U;
    block[14U + 2U] = 2U;
    block[14U + 3U] = 5U;
    block[14U + 4U] = 0xaaU;
    stars_update_battle_records(block, sizeof block, 0U, 0, 0);
    if (word(block, 16U) != 5U || word(block, 18U) != 0x02aaU) return 1;

    memset(next, 0, sizeof next);
    block[0] = 0xffU;
    block[1] = 0xffU;
    next[0] = 1U;
    next[6] = 8U;
    stars_update_battle_records(block, sizeof block, 0U, next_block, next);
    puts("UPDATE_BATTLE_RECORDS_SOURCE=PASS");
    return 0;
}
