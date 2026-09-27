/* Genuine C reconstruction of the turn-wide mineral estimation loop. */
#include "core.h"
#include <stdio.h>
#include <string.h>

void stars_mine_minerals(const StarsU8 *planets, StarsU16 planet_count,
    StarsEstimateMineralsMined estimate, void *context)
{
    StarsU16 index;
    StarsI32 scratch[3];
    for (index = 0; index < planet_count; ++index) {
        if (estimate)
            estimate(context, planets + (StarsU16)(index * 56U), scratch,
                (StarsI16)-1, 1);
    }
}

/* NE 24:7638.  Thing records are 18 bytes; active mine-field state is the
 * word at offset 0x0E, and records with any high three overlap bits set are
 * intentionally left untouched. */
void stars_unmark_mine_fields(StarsU8 *things, StarsU16 thing_count)
{
    StarsU16 index;
    StarsU16 offset;
    StarsU16 flags;
    for (index = 0; index < thing_count; ++index) {
        offset = (StarsU16)(index * 0x12U);
        flags = (StarsU16)(things[offset] | ((StarsU16)things[offset + 1U] << 8));
        if ((flags & 0xe000U) == 0U) {
            things[offset + 0x0eU] = 0;
            things[offset + 0x0fU] = 0;
        }
    }
}

/* NE 24:6DCA. Preserve the source directory, choose the configured backup
 * name format, create that directory, and append the original suffix. */
void stars_create_backup_dir(StarsU8 *path, const StarsU8 *game_file,
    StarsU16 backup_dirs, StarsU16 turn, const StarsU8 *single_name,
    const StarsU8 *short_format, const StarsU8 *long_format,
    const StarsU8 *suffix, StarsMakeDirectory make_directory, void *context)
{
    StarsU8 *leaf;

    if (!path || !game_file)
        return;
    strcpy((char *)path, (const char *)game_file);
    leaf = (StarsU8 *)strrchr((char *)path, '\\');
    leaf = leaf ? leaf + 1 : path;
    *leaf = 0;
    if (backup_dirs < 2U)
        strcpy((char *)leaf, (const char *)single_name);
    else if (backup_dirs < 100U)
        sprintf((char *)leaf, (const char *)short_format,
            (unsigned)(turn % backup_dirs));
    else
        sprintf((char *)leaf, (const char *)long_format,
            (unsigned)(turn % backup_dirs));
    if (make_directory)
        make_directory(context, path);
    if (suffix)
        strcat((char *)path, (const char *)suffix);
}

static StarsU32 turn2_long_at(const StarsU8 *bytes, StarsU16 offset)
{
    return (StarsU32)bytes[offset] |
        ((StarsU32)bytes[offset + 1U] << 8) |
        ((StarsU32)bytes[offset + 2U] << 16) |
        ((StarsU32)bytes[offset + 3U] << 24);
}

static void turn2_put_long(StarsU8 *bytes, StarsU16 offset, StarsU32 value)
{
    bytes[offset] = (StarsU8)value;
    bytes[offset + 1U] = (StarsU8)(value >> 8);
    bytes[offset + 2U] = (StarsU8)(value >> 16);
    bytes[offset + 3U] = (StarsU8)(value >> 24);
}

/* NE 24:2FD6.  Hull records are 0x93 bytes; offsets 0x7F and 0x83 are the
 * two 32-bit accumulated ship counters updated by the signed delta. */
void stars_create_ship(StarsI16 *fleet_counts, StarsU8 *hull_records,
    StarsU16 hull_count, StarsI16 hull_index, StarsI16 ship_delta)
{
    StarsU16 offset;
    StarsI32 delta;
    StarsI32 old_value;
    StarsI16 index;
    if (hull_index < 0 || (StarsU16)hull_index >= hull_count)
        return;
    index = hull_index;
    fleet_counts[index] = (StarsI16)(fleet_counts[index] + ship_delta);
    offset = (StarsU16)index * 0x93U;
    delta = (StarsI32)ship_delta;
    old_value = (StarsI32)turn2_long_at(hull_records, (StarsU16)(offset + 0x83U));
    turn2_put_long(hull_records, (StarsU16)(offset + 0x83U),
        (StarsU32)(old_value + delta));
    old_value = (StarsI32)turn2_long_at(hull_records, (StarsU16)(offset + 0x7fU));
    turn2_put_long(hull_records, (StarsU16)(offset + 0x7fU),
        (StarsU32)(old_value + delta));
}

/* NE 22:2CE2.  The original list is a far-pointer array terminated by a
 * null pointer pair; the host adapter represents each valid planet as a
 * pointer and retains the explicit list bound. */
void stars_fill_production_queue(const StarsU8 *const *planets,
    StarsU16 planet_limit, const StarsProductionQueueOps *ops, void *context)
{
    StarsU16 index;
    const StarsU8 *planet;
    StarsU8 production_scratch[256];
    for (index = 0; index < planet_limit; ++index) {
        planet = planets[index];
        if (!planet)
            break;
        if (ops && ops->change_selection)
            ops->change_selection(context, 1,
                (StarsI16)(planet[0] | ((StarsU16)planet[1] << 8)));
        if (ops && ops->initialize)
            ops->initialize(context, production_scratch,
                (StarsU16)sizeof production_scratch);
        if (ops && ops->fill && ops->finish)
            ops->finish(context, ops->fill(context, planet));
    }
}
