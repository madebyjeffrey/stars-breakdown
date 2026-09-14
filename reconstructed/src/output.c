#include "runtime_internal.h"

/* NE 4:1212-1252. This build formats 2, 60 and 'j' here, despite the
 * release label; the constants are read from instructions, not inferred. */
StarsU16 stars_version_string(StarsU8 *data, const StarsU8 *strings)
{
    StarsU16 format = stars_get_compressed_string(data, strings, 0x22d);
    sprintf((char *)data + 0x57a4U, (const char *)data + format, 2, 60, 'j');
    return 0x57a4U;
}

/* NE 9:1F64-1FF9. Logging is best effort, with its own handle; it must not
 * replace the game's active stream handle or invoke stream-error recovery. */
void stars_output_file_string(StarsRuntime *r, const StarsU8 *name, const StarsU8 *text)
{
    StarsU16 mode = 2, error;
    StarsI16 handle;
    if (r->platform->access(r->context, name, 0) == -1) mode |= 0x1000U;
    handle = r->platform->open(r->context, name, mode, &error);
    if (handle != -1) {
        r->platform->seek(r->context, handle, 0, 2);
        r->platform->write(r->context, handle, text, (StarsU16)strlen((const char *)text));
        r->platform->close(r->context, handle);
    }
}

/* NE 8:7FE6-80C2. Original filename and line buffers have 256 bytes. */
void stars_output_string(StarsRuntime *r, StarsI16 channel, const StarsU8 *text)
{
    StarsU8 name[256], line[256], date[100], time[102];
    StarsU8 *d = r->data;
    StarsU16 extension = stars_word(d, (StarsU16)(0x552U + (StarsU16)channel * 2U));
    sprintf((char *)name, (const char *)d + 0x564U, d + 0x56a2U, d + extension);
    if (r->platform->access(r->context, name, 0) == -1) {
        sprintf((char *)line, (const char *)d + 0x56aU,
            d + stars_version_string(d, r->strings));
        stars_output_file_string(r, name, line);
    }
    r->platform->date(r->context, date);
    r->platform->time(r->context, time);
    sprintf((char *)line, (const char *)d + 0x578U, date, time, text);
    stars_output_file_string(r, name, line);
}
