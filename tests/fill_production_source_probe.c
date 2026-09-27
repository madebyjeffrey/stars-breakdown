#include <stdio.h>
#include <string.h>
#include "core.h"

static void name(void *context, StarsU32 packed, StarsU8 *output)
{
    (void)context;
    if (((packed >> 10) & 0x7fU) == 0U) strcpy((char *)output, "Scout");
    else if (((packed >> 10) & 0x7fU) == 1U) strcpy((char *)output, "Miner");
    else strcpy((char *)output, "Other");
}
static void append_suffix(void *context, StarsU8 *output)
{ (void)context; strcat((char *)output, " unit"); }
static void send_message(void *context, StarsU16 hwnd, StarsU16 message,
        const StarsU8 *value)
{
    StarsU16 *count = (StarsU16 *)context;
    (void)hwnd;
    if (message == 0x405U) {
        if (value != 0) *count = 99U;
    } else if (message == 0x401U) {
        if (value[0] == '*' || value[0] == '#' || value[0] == 'I' ||
                value[0] == ' ')
            *count = (StarsU16)(*count + 1U);
    }
}
static StarsU32 packed(StarsU16 kind, StarsU16 item)
{ return ((StarsU32)kind << 17) | ((StarsU32)item << 10) | 1UL; }

int main(void)
{
    StarsProductionItem items[4];
    StarsFillProdSourceInput input;
    StarsU16 count = 0;
    /* The source probe checks callback sequencing and marker values below. */
    items[0].packed = 0UL;
    items[1].packed = packed(2U, 0U);
    items[2].packed = packed(2U, 0x10U);
    items[3].packed = packed(0U, 1U);
    input.hwnd = 7U;
    input.items = items;
    input.item_count = 4U;
    input.name = name;
    input.append_item_suffix = append_suffix;
    input.send_message = send_message;
    input.context = &count;
    stars_fill_production_source_list(&input);
    if (count != 3U) return 1;
    puts("FILL_PRODUCTION_SOURCE=PASS");
    return 0;
}
