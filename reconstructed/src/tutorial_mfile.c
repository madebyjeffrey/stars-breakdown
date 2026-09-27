#include "core.h"
#include <string.h>

static StarsU16 tutorial_word(const StarsU8 *data, StarsU16 offset)
{
    return (StarsU16)(data[offset] |
        ((StarsU16)data[(StarsU16)(offset + 1U)] << 8));
}

/* NE 10:D058-D29D. Select the tutorial resource for the requested turn,
 * skip to its type-8 record boundary, and copy that record run to the
 * tutorial stream. Resource and stream operations are explicit callbacks so
 * the source retains the original Win16 ownership order without replaying
 * the original far pointers. */
StarsI16 stars_write_tutorial_m_file(const StarsWriteTutorialMFileInput *input)
{
    StarsU16 resource_id;
    StarsU16 resource_type;
    StarsU16 handle;
    const StarsTutorialResource *resource;
    StarsU16 cursor;
    StarsU16 remaining;
    StarsU16 length;
    StarsU16 header;
    StarsU8 filename[30];

    if (!input || !input->find_resource || !input->load_resource ||
            !input->lock_resource || !input->unlock_resource ||
            !input->free_resource || !input->make_filename ||
            !input->open_stream || !input->write_stream ||
            !input->close_stream)
        return 0;
    if (input->turn < 32) {
        resource_id = 0x2713U;
        resource_type = 0x2712U;
        remaining = (StarsU16)input->turn;
    } else {
        resource_id = 0x2715U;
        resource_type = 0x2714U;
        remaining = (StarsU16)(input->turn - 32);
    }
    handle = input->find_resource(input->context, resource_id, resource_type);
    handle = input->load_resource(input->context, handle);
    if (!handle)
        return 0;
    resource = input->lock_resource(input->context, handle);
    if (!resource || !resource->data || resource->size == 0U) {
        input->unlock_resource(input->context, handle);
        input->free_resource(input->context, handle);
        return 0;
    }
    if (remaining >= (StarsU16)resource->data[0]) {
        input->unlock_resource(input->context, handle);
        input->free_resource(input->context, handle);
        return 2;
    }
    cursor = 1U;
    while (remaining != 0U) {
        do {
            if ((StarsU16)(cursor + 2U) > resource->size)
                goto bad_resource;
            header = tutorial_word(resource->data, cursor);
            length = (StarsU16)(header & 0x03ffU);
            if ((StarsU16)(cursor + length + 2U) > resource->size)
                goto bad_resource;
            cursor = (StarsU16)(cursor + length + 2U);
        } while ((tutorial_word(resource->data, cursor) >> 10) != 8U);
        --remaining;
    }
    filename[0] = 0;
    if (!input->make_filename(input->context, filename,
            (StarsU16)sizeof(filename)))
        goto bad_resource;
    if (!input->open_stream(input->context, filename, 0x1012U))
        goto bad_resource;
    do {
        header = tutorial_word(resource->data, cursor);
        length = (StarsU16)(header & 0x03ffU);
        if ((StarsU16)(cursor + length + 2U) > resource->size)
            goto bad_stream;
        input->write_stream(input->context, resource->data + cursor,
            (StarsU16)(length + 2U));
        cursor = (StarsU16)(cursor + length + 2U);
    } while ((tutorial_word(resource->data, cursor) >> 10) != 8U);
    input->close_stream(input->context);
    input->unlock_resource(input->context, handle);
    input->free_resource(input->context, handle);
    return 1;
bad_stream:
    input->close_stream(input->context);
bad_resource:
    input->unlock_resource(input->context, handle);
    input->free_resource(input->context, handle);
    return 0;
}
