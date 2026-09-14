/* Reviewed alert selection, with explicit logging/formatting dependencies. */
#include "core.h"
#include <stdio.h>

/* NE 9:2160-221D. The verified "Error: %s" result must fit 258 bytes. */
StarsI16 stars_alert_string(StarsU8 *data, StarsU16 text, StarsI16 type,
    const StarsAlertApi *api, void *context)
{
    StarsU16 flags = (StarsU16)(data[0x229aU] | ((StarsU16)data[0x229bU] << 8));
    StarsU16 owner;
    char buffer[258];
    if ((flags & 0x4000U) || ((flags & 0x8000U) && (flags & 8U))) {
        sprintf(buffer, (const char *)data + 0x137bU, (const char *)data + text);
        api->output(context, (data[0x229bU] & 64U) ? 7 : 6, (const StarsU8 *)buffer);
        return 6;
    }
    owner = api->focus(context);
    return api->message(context, owner, data + text, data + 0x1385U, type);
}

/* NE 15:4A10-4A5F. Records the id even when presentation is suppressed. */
void stars_file_error(StarsU8 *data, StarsI16 id, const StarsAlertApi *api, void *context)
{
    StarsU16 text;
    data[0x74eU] = (StarsU8)id; data[0x74fU] = (StarsU8)((StarsU16)id >> 8);
    if (!(data[0x74cU] | data[0x74dU]) && !(data[0x7caU] & 2U)) {
        text = api->format_id(context, id);
        stars_alert_string(data, text, 16, api, context);
    }
}

/* NE 15:524E-52AD. */
StarsI16 stars_bad_file_error(StarsI16 id)
{
    switch (id) {
    case 0: case 1: case 2: case 3: case 22: case 28: case 29: return 1;
    default: return 0;
    }
}
