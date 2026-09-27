/* Semantic reconstruction of NE 15:709C-7A6A (WriteBattles).
 *
 * A battle is not written as one unbounded blob.  The original filters each
 * block through the viewer mask, marks the players and objects referenced by
 * the visible participants, and then emits a primary record followed by
 * continuation records whenever the 10-bit record-length field would be
 * exceeded.
 */
#include "core.h"
#include <string.h>

#define BATTLE_FIRST_EVENTS 34U
#define BATTLE_CONT_EVENTS 35U
#define BATTLE_CONT_BYTES (BATTLE_CONT_EVENTS * STARS_BATTLE_EVENT_BYTES)

static StarsI16 write_battle_chunk(const StarsWriteBattlesOps *ops,
    void *context, StarsU16 type, const StarsU8 *data, StarsU16 size)
{
    if (size >= STARS_BATTLE_RECORD_LIMIT) return 0;
    if (ops && ops->write_record)
        ops->write_record(context, type, size, data);
    return 1;
}

static StarsI16 write_battle_block(const StarsWriteBattleBlock *block,
    const StarsWriteBattlesOps *ops, void *context)
{
    StarsU8 chunk[STARS_BATTLE_RECORD_LIMIT - 1U];
    StarsU16 event_count;
    StarsU16 prefix_size;
    StarsU16 offset;
    StarsU16 remaining;
    StarsU16 count;
    StarsU16 chunk_size;

    if (!block || !block->data || block->size < STARS_BATTLE_PREFIX_BYTES ||
            block->event_count > 0xffffU / STARS_BATTLE_EVENT_BYTES)
        return 0;
    prefix_size = STARS_BATTLE_PREFIX_BYTES;
    event_count = block->event_count;
    if ((StarsU32)prefix_size + (StarsU32)event_count *
            STARS_BATTLE_EVENT_BYTES > block->size)
        return 0;
    if (block->size < STARS_BATTLE_RECORD_LIMIT)
        return write_battle_chunk(ops, context, STARS_BATTLE_PRIMARY,
            block->data, block->size);

    /* The first record retains the 14-byte battle prefix and at most 34
     * participant records, leaving it below the 1024-byte wire limit. */
    count = event_count < BATTLE_FIRST_EVENTS ? event_count : BATTLE_FIRST_EVENTS;
    chunk_size = (StarsU16)(prefix_size + count * STARS_BATTLE_EVENT_BYTES);
    memcpy(chunk, block->data, chunk_size);
    if (!write_battle_chunk(ops, context, STARS_BATTLE_PRIMARY, chunk,
            chunk_size)) return 0;
    offset = chunk_size;
    remaining = (StarsU16)(event_count - count);
    while (remaining != 0U) {
        count = remaining < BATTLE_CONT_EVENTS ? remaining : BATTLE_CONT_EVENTS;
        chunk_size = (StarsU16)(count * STARS_BATTLE_EVENT_BYTES);
        memcpy(chunk, block->data + offset, chunk_size);
        if (!write_battle_chunk(ops, context, STARS_BATTLE_CONTINUATION,
                chunk, chunk_size)) return 0;
        offset = (StarsU16)(offset + chunk_size);
        remaining = (StarsU16)(remaining - count);
    }
    /* Any bytes after the participant array are a variable battle tail. */
    remaining = (StarsU16)(block->size - offset);
    while (remaining != 0U) {
        count = remaining < (STARS_BATTLE_RECORD_LIMIT - 1U) ? remaining :
            (STARS_BATTLE_RECORD_LIMIT - 1U);
        memcpy(chunk, block->data + offset, count);
        if (!write_battle_chunk(ops, context, STARS_BATTLE_CONTINUATION,
                chunk, count)) return 0;
        offset = (StarsU16)(offset + count);
        remaining = (StarsU16)(remaining - count);
    }
    return 1;
}

/* NE 15:709C-7A6A.  Playback/current-log checks are represented by
 * log_is_current.  A normal viewer receives only blocks whose visibility bit
 * names that player; the callbacks then apply the same discovered-player,
 * discovered-design, and discovered-fleet side effects before serialization.
 */
StarsI16 stars_write_battles(const StarsWriteBattlesInput *input,
    const StarsWriteBattlesOps *ops, void *context)
{
    StarsU16 i;
    StarsU16 j;
    const StarsWriteBattleBlock *block;
    const StarsWriteBattleEvent *event;
    if (!input || input->player < -1 || input->player > 31)
        return 0;
    if (input->player == -1 || input->log_is_current)
        return 1;
    if (!input->blocks && input->block_count != 0U) return 0;
    for (i = 0U; i < input->block_count; ++i) {
        block = &input->blocks[i];
        if ((block->visibility & (1UL << input->player)) == 0UL) continue;
        for (j = 0U; j < block->event_count; ++j) {
            event = &block->events[j];
            if (event->player == (StarsU16)input->player) continue;
            if (ops && ops->mark_player)
                ops->mark_player(context, event->player);
            if (event->is_design) {
                if (ops && ops->mark_design)
                    ops->mark_design(context, event->player,
                        event->design_slot);
            } else if (ops && ops->mark_fleet) {
                ops->mark_fleet(context, event->player, event->object_id);
            }
        }
        if (block->planet_id != 0xffffU && ops && ops->mark_planet)
            ops->mark_planet(context, block->planet_id, input->player);
        if (!write_battle_block(block, ops, context)) return 0;
    }
    return 1;
}
