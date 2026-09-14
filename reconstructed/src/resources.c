#include "title.h"
#include "runtime_internal.h"

/* NE 9:5310-53b4. Huge pointers advance their selector by eight when the
 * offset wraps; an ordinary far-pointer increment loses data above 64 KiB. */
StarsI16 stars_read_big_block(StarsRuntime *r, StarsU16 file, StarsU32 pointer, StarsU32 size)
{
    StarsU16 count, selector;
    StarsU32 offset;
    while (size) {
        count = size > 30000UL ? 30000U : (StarsU16)size;
        if ((StarsU16)stars_window(r, "_lread", file, count, 0, pointer, 0, 0) != count) return 0;
        size -= count;
        offset = (StarsU32)(StarsU16)pointer + count;
        selector = (StarsU16)((pointer >> 16) + ((offset >> 16) * 8U));
        pointer = ((StarsU32)selector << 16) | (StarsU16)offset;
    }
    return 1;
}

/* NE 9:5220-530f. Success returns an allocated resource that is still locked.
 * Failure closes an opened file and frees the allocation, in that order. */
StarsU16 stars_load_big_resource(StarsRuntime *r, StarsU16 id)
{
    StarsU16 instance = stars_word(r->data, 0x5310U), resource, handle, file;
    StarsU32 pointer, size;
    resource = (StarsU16)stars_window(r, "FindResource", instance, id, 2, 0, 0, 0);
    if (!resource) return 0;
    handle = (StarsU16)stars_window(r, "AllocResource", instance, resource, 0, 0, 0, 0);
    if (!handle) return 0;
    file = (StarsU16)stars_window(r, "AccessResource", instance, resource, 0, 0, 0, 0);
    if (file != 65535U) {
        pointer = stars_window(r, "LockResource", handle, 0, 0, 0, 0, 0);
        if (pointer) {
            size = stars_window(r, "SizeofResource", instance, resource, 0, 0, 0, 0);
            if (stars_read_big_block(r, file, pointer, size)) {
                stars_window(r, "_lclose", file, 0, 0, 0, 0, 0);
                return handle;
            }
        }
        stars_window(r, "_lclose", file, 0, 0, 0, 0, 0);
    }
    stars_window(r, "FreeResource", handle, 0, 0, 0, 0, 0);
    return 0;
}

/* NE 9:4b50-4c9b. Preserve the fixed +40 palette offset, signed color-count
 * comparison, RGB/BGR swap and reserved palette bytes. LocalAlloc failure is
 * unchecked by the original; valid inputs require that allocation to succeed. */
StarsU16 stars_palette_from_dib(StarsRuntime *r, StarsU16 dib)
{
    StarsU32 pointer;
    const StarsU8 *header;
    StarsU16 count, local, palette, i;
    StarsU8 temp;
    pointer = stars_window(r, "LockResource", dib, 0, 0, 0, 0, 0);
    if (!pointer) return 0;
    header = r->platform->memory(r->context, (StarsU16)(pointer >> 16), (StarsU16)pointer);
    count = stars_dib_num_colors(header);
    if (stars_signed_word(count) > 256) {
        stars_window(r, "GlobalUnlock", dib, 0, 0, 0, 0, 0);
        return 0;
    }
    local = (StarsU16)stars_window(r, "LocalAlloc", 0, 0x40U, (StarsU16)(count * 4U + 8U), 0, 0, 0);
    stars_put_word(r->data, local, 0x300U);
    stars_put_word(r->data, (StarsU16)(local + 2U), count);
    memcpy(r->data + local + 4U, header + 40U, (StarsU16)(count * 4U));
    for (i = 0; stars_signed_word(i) < stars_signed_word(count); ++i) {
        temp = r->data[local + 4U + i * 4U];
        r->data[local + 4U + i * 4U] = r->data[local + 6U + i * 4U];
        r->data[local + 6U + i * 4U] = temp;
    }
    palette = (StarsU16)stars_window(r, "CreatePalette", 0, 0, 0, 0, r->data + local, 0);
    stars_window(r, "LocalFree", local, 0, 0, 0, 0, 0);
    stars_window(r, "GlobalUnlock", dib, 0, 0, 0, 0, 0);
    return palette;
}
