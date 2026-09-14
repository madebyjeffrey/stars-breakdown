/* Reviewed stream control flow. OS calls, error presentation and nonlocal
 * transfer use explicit integration callbacks; see file-streams.md. */
#include "core.h"

static StarsU16 stream_word(const StarsU8 *data, StarsU16 at)
{
    return (StarsU16)(data[at] | ((StarsU16)data[at + 1U] << 8));
}

static void stream_put_word(StarsU8 *data, StarsU16 at, StarsU16 value)
{
    data[at] = (StarsU8)value; data[at + 1U] = (StarsU8)(value >> 8);
}

static StarsI16 stream_signed(StarsU16 word)
{
    return word < 32768U ? (StarsI16)word : (StarsI16)((StarsI32)word - 65536L);
}

/* NE 15:52AE-53CC. Unsigned absolute deadlines deliberately retain the
 * original tick-wrap behavior and busy polling; there is no sleep call. */
void stars_stream_open(StarsU8 *data, StarsU16 name, StarsI16 mode,
    const StarsStreamApi *api, void *context)
{
    StarsU32 deadline = 0, now;
    StarsU16 error, flags = (StarsU16)mode;
    StarsI16 handle;
    for (;;) {
        handle = api->open(context, data + name, (StarsU16)(flags & 0xbfffU), &error);
        stream_put_word(data, 0xa22U, (StarsU16)handle);
        if (handle != -1) return;
        if (!(data[0x7cbU] & 2U) || error == 2U) break;
        now = api->ticks(context);
        if (!deadline) deadline = now + 4000U;
        if (deadline <= now) break;
        now += 500U;
        while (api->ticks(context) < now) { }
    }
    if (!(flags & 0x4000U)) api->file_error(context, 4);
    api->abort(context, stream_word(data, 0x6eU), -1);
}

/* NE 15:53CC-53F4. Close errors are ignored; handle is then invalidated. */
void stars_stream_close(StarsU8 *data, const StarsStreamApi *api, void *context)
{
    StarsI16 handle = stream_signed(stream_word(data, 0xa22U));
    if (handle != -1) {
        api->close(context, handle);
        stream_put_word(data, 0xa22U, 0xffffU);
    }
}

/* NE 15:53F4-547E. Far-memory copying uses forward words then one odd byte,
 * preserving the CRT's effects for overlapping contiguous spans. */
void stars_stream_read(StarsU8 *data, StarsU8 *output, StarsU16 count,
    const StarsStreamApi *api, void *context)
{
    StarsU16 source, selector, i, word;
    const StarsU8 *input;
    if (!count) return;
    source = stream_word(data, 0x99cU);
    selector = stream_word(data, 0x99eU);
    if (source || selector) {
        input = api->memory(context, selector, source);
        for (i = 0; i < count / 2U; ++i) {
            word = (StarsU16)(input[i * 2U] | ((StarsU16)input[i * 2U + 1U] << 8));
            output[i * 2U] = (StarsU8)word;
            output[i * 2U + 1U] = (StarsU8)(word >> 8);
        }
        if (count & 1U) output[count - 1U] = input[count - 1U];
        /* Reload after copying: the destination can alias the pointer slot. */
        stream_put_word(data, 0x99cU, (StarsU16)(stream_word(data, 0x99cU) + count));
    } else if (api->read(context, stream_signed(stream_word(data, 0xa22U)), output, count) != count) {
        api->file_error(context, 3);
        api->abort(context, stream_word(data, 0x6eU), -1);
    }
}

/* NE 15:9554-95BC. Any non-exact result is a failure; no retry is attempted. */
void stars_stream_write(StarsU8 *data, const StarsU8 *input, StarsU16 count,
    const StarsStreamApi *api, void *context)
{
    if (!count) return;
    if (api->write(context, stream_signed(stream_word(data, 0xa22U)), input, count) != count) {
        api->write_error(context);
        api->abort(context, stream_word(data, 0x6eU), -1);
    }
}
