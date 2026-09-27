#include "core.h"

enum {
    FINISH_DELETE_CURRENT = 1000,
    FINISH_MESSAGE_TERMINATOR_BYTES = 1
};

static StarsU16 finish_abs_length(StarsI16 length)
{
    if (length < 0)
        return (StarsU16)(-(StarsI32)length);
    return (StarsU16)length;
}

static void finish_copy(StarsU8 *destination, const StarsU8 *source,
    StarsU16 count)
{
    StarsU16 i;
    for (i = 0; i < count; ++i)
        destination[i] = source[i];
}

static StarsI16 finish_payload_equal(const StarsFinishPlayerMessage *entry,
    StarsI16 stored_length, StarsI16 recipient, const StarsU8 *payload)
{
    StarsU16 i;
    StarsU16 count;
    if (entry->stored_length != stored_length ||
            entry->recipient != recipient)
        return 0;
    count = finish_abs_length(stored_length);
    for (i = 0; i < count; ++i)
        if (entry->payload[i] != payload[i]) return 0;
    return 1;
}

static void finish_delete(StarsFinishPlrMsgEntryInput *input)
{
    StarsU16 i;
    StarsI16 index = input->current;

    if (index < 0 || (StarsU16)index >= input->count)
        return;
    /* The original linked list unlinks one node.  The array adapter keeps
     * the same logical order by shifting the following nodes left. */
    for (i = (StarsU16)index; i + 1U < input->count; ++i)
        input->entries[i] = input->entries[i + 1U];
    --input->count;
    if (input->current > 0)
        --input->current;
}

/* Genuine semantic reconstruction of FFinishPlrMsgEntry, NE 7:9BD6-9EB8.
 * The dialog has already copied the edit control into editor_text.  The
 * original routine then either removes the selected linked-list node or
 * replaces its twelve-byte header/payload with a compressed message.  The
 * adapter exposes the same storage as an ordered array and keeps the Win16
 * allocation, dropdown, and compression services as typed callbacks. */
StarsI16 stars_finish_plr_msg_entry(StarsFinishPlrMsgEntryInput *input)
{
    StarsU8 plain[STARS_FINISH_MESSAGE_PAYLOAD_CAPACITY];
    StarsU8 compressed[STARS_FINISH_MESSAGE_PAYLOAD_CAPACITY];
    StarsU8 *payload;
    StarsU16 payload_bytes;
    StarsU16 compressed_length;
    StarsI16 stored_length;
    StarsI16 recipient;
    StarsI16 is_new;
    StarsI16 changed;
    StarsI16 i;

    if (!input || !input->entries || input->capacity == 0U ||
            input->count > input->capacity)
        return 0;

    /* 1000 is the dialog's delete sentinel, not a selection displacement. */
    if (input->direction == FINISH_DELETE_CURRENT) {
        input->direction = 0;
        if (input->current >= 0 &&
                (StarsU16)input->current < input->count) {
            if (input->mark_dirty) input->mark_dirty(input->context);
            finish_delete(input);
        }
        if (input->direction == -1 && input->current > 0)
            --input->current;
        else if (input->direction == 1 &&
                (StarsU16)input->current < input->count)
            ++input->current;
        return 0;
    }

    if (input->editor_length > STARS_FINISH_MESSAGE_TEXT_LIMIT)
        return 0;
    if (!input->editor_text && input->editor_length != 0U)
        return 0;

    /* An empty editor deletes the current node, then applies the requested
     * previous/next movement.  This ordering is observable at list ends. */
    if (input->editor_length == 0U) {
        if (input->current >= 0 &&
                (StarsU16)input->current < input->count) {
            if (input->mark_dirty) input->mark_dirty(input->context);
            finish_delete(input);
        }
        if (input->direction == -1 && input->current > 0)
            --input->current;
        else if (input->direction == 1 &&
                (StarsU16)input->current < input->count)
            ++input->current;
        return 0;
    }

    payload = plain;
    payload_bytes = (StarsU16)(input->editor_length +
        FINISH_MESSAGE_TERMINATOR_BYTES);
    finish_copy(plain, input->editor_text, input->editor_length);
    plain[input->editor_length] = 0;
    stored_length = (StarsI16)(-(StarsI32)payload_bytes);

    /* FCompressUserString returns nonzero only when its encoded form fits;
     * the fallback is deliberately retained as negative length plus NUL. */
    compressed_length = 0U;
    if (input->compress && input->compress(input->context,
            input->editor_text, input->editor_length, compressed,
            &compressed_length) != 0 && compressed_length <=
            STARS_FINISH_MESSAGE_PAYLOAD_CAPACITY) {
        payload = compressed;
        payload_bytes = compressed_length;
        stored_length = (StarsI16)compressed_length;
    }

    recipient = input->recipient ? input->recipient(input->context) : 0;
    is_new = (input->current < 0 ||
        (StarsU16)input->current >= input->count);
    if (is_new) {
        if (input->count >= input->capacity)
            return 0;
        changed = 1;
        i = (StarsI16)input->count;
    } else {
        i = input->current;
        changed = (StarsI16)!finish_payload_equal(&input->entries[i],
            stored_length, recipient, payload);
    }
    if (changed && input->mark_dirty)
        input->mark_dirty(input->context);

    input->entries[i].sender = input->sender;
    input->entries[i].recipient = recipient;
    input->entries[i].message_id = input->message_id;
    input->entries[i].stored_length = stored_length;
    finish_copy(input->entries[i].payload, payload, payload_bytes);
    if (is_new) {
        ++input->count;
        input->current = i;
    }
    input->current = (StarsI16)(input->current + input->direction);
    if (input->current < 0)
        input->current = 0;
    return 1;
}
