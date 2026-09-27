#include "core.h"
#include <string.h>

static void planet_put_word(StarsU8 *record, StarsU16 offset, StarsU16 value)
{
    record[offset] = (StarsU8)value;
    record[(StarsU16)(offset + 1U)] = (StarsU8)(value >> 8);
}

static void planet_put_long(StarsU8 *record, StarsU16 offset, StarsU32 value)
{
    planet_put_word(record, offset, (StarsU16)value);
    planet_put_word(record, (StarsU16)(offset + 2U),
        (StarsU16)(value >> 16));
}

static void planet_write(const StarsLogChangePlanetOps *ops, void *context,
    StarsU16 type, StarsU16 length, const StarsU8 *record)
{
    if (ops && ops->write_memory)
        ops->write_memory(context, type, length, record);
}

/* This is the same compact transfer encoding used by LogMakeValidXfer. It is
 * kept local so the planet logger can be tested without binding to DGROUP's
 * in-memory log and cursor globals. */
static void planet_flush_xfer(const StarsLogXfer *first,
    const StarsLogXfer *second, const StarsLogChangePlanetOps *ops,
    void *context)
{
    StarsU8 record[26];
    StarsU32 total[5];
    StarsU32 magnitude;
    StarsU32 maximum = 0U;
    StarsU16 mask = 0U;
    StarsU16 type;
    StarsU16 length;
    StarsU16 i;

    for (i = 0; i < 5U; ++i) {
        total[i] = first->item[i] + second->item[i];
        if (total[i] != 0U) mask = (StarsU16)(mask | (StarsU16)(1U << i));
        magnitude = total[i];
        if ((StarsI32)total[i] < 0)
            magnitude = (StarsU32)(0U - magnitude);
        if (magnitude > maximum) maximum = magnitude;
    }
    if (mask == 0U) return;
    planet_put_word(record, 0U, first->id);
    planet_put_word(record, 2U, second->id);
    record[4] = (StarsU8)((first->grobj & 0x0fU) |
        ((second->grobj & 0x0fU) << 4));
    record[5] = (StarsU8)mask;
    if (maximum < 0x80U) {
        type = 1U;
        length = 6U;
        for (i = 0; i < 5U; ++i)
            if (total[i] != 0U) record[length++] = (StarsU8)total[i];
    } else if (maximum < 0x8000U) {
        type = 2U;
        length = 6U;
        for (i = 0; i < 5U; ++i) if (total[i] != 0U) {
            planet_put_word(record, length, (StarsU16)total[i]);
            length = (StarsU16)(length + 2U);
        }
    } else {
        type = 0x19U;
        length = 6U;
        for (i = 0; i < 5U; ++i) if (total[i] != 0U) {
            planet_put_long(record, length, total[i]);
            length = (StarsU16)(length + 4U);
        }
    }
    planet_write(ops, context, type, length, record);
}

/* NE 10:9420-9907. The optional old snapshot is used by the original caller
 * to flush a pending mineral transfer when a planet is removed. */
void stars_log_change_planet(const StarsLogPlanet *old_planet,
    const StarsLogPlanet *new_planet, StarsU16 log_flags,
    StarsLogChangePlanetState *state, const StarsLogChangePlanetOps *ops,
    void *context)
{
    StarsLogXfer current;
    StarsU8 record[1022];
    StarsU32 packed;
    StarsU16 i;
    StarsU16 production_bytes;
    int changed;

    if (!state || !ops) return;
    if ((log_flags & 0x0002U) != 0U) return;
    if (!old_planet) {
        if (state->valid_xfer) {
            memset(&current, 0, sizeof current);
            current.id = (StarsU16)-1;
            current.grobj = 4U;
            for (i = 0; i < 5U; ++i)
                current.item[i] = (StarsU32)(0U - state->xfer.item[i]);
            planet_flush_xfer(&state->xfer, &current, ops, context);
            state->valid_xfer = 0U;
        }
        return;
    }
    if (!new_planet) return;

    memset(&current, 0, sizeof current);
    current.id = (StarsU16)old_planet->object_id;
    current.grobj = 1U;
    changed = 0;
    for (i = 0; i < 4U; ++i) {
        current.item[i] = new_planet->minerals[i] -
            old_planet->minerals[i];
        if (current.item[i] != 0U) changed = 1;
    }
    if (changed) {
        if (!state->valid_xfer) {
            state->xfer = current;
            state->valid_xfer = 1U;
        } else {
            planet_flush_xfer(&state->xfer, &current, ops, context);
            state->valid_xfer = 0U;
        }
    }

    if (!new_planet->has_production && old_planet->has_production) {
        planet_put_word(record, 0U, (StarsU16)new_planet->object_id);
        planet_write(ops, context, 0x1dU, 2U, record);
    } else if (new_planet->has_production &&
            (!old_planet->has_production ||
             old_planet->production_count != new_planet->production_count ||
             (new_planet->production_count != 0U &&
              (!old_planet->production || !new_planet->production ||
               memcmp(old_planet->production, new_planet->production,
                   (size_t)new_planet->production_count * 4U) != 0)))) {
        if (ops->previous_production &&
                ops->previous_production(context, old_planet->object_id) != 0)
            return;
        if (new_planet->production_count > 255U) return;
        production_bytes = (StarsU16)(new_planet->production_count * 4U);
        planet_put_word(record, 0U, (StarsU16)new_planet->object_id);
        if (production_bytes && new_planet->production)
            memcpy(record + 2, new_planet->production, production_bytes);
        planet_write(ops, context, 0x1dU,
            (StarsU16)(production_bytes + 2U), record);
    }

    packed = ((new_planet->overlap_20 >> 23) & 1U) |
        ((new_planet->overlap_44 & 0x03ffU) << 1) |
        (((new_planet->overlap_44 >> 10) & 0x000fU) << 11) |
        (((StarsU32)new_planet->overlap_48 & 0x03ffU) << 15);
    if ((((old_planet->overlap_20 >> 23) ^
            (new_planet->overlap_20 >> 23)) & 1U) != 0U ||
            ((old_planet->overlap_44 ^ new_planet->overlap_44) & 0x3fffU) != 0U ||
            ((old_planet->overlap_48 ^ new_planet->overlap_48) & 0x03ffU) != 0U) {
        planet_put_word(record, 0U, (StarsU16)new_planet->object_id);
        planet_put_long(record, 2U, packed);
        planet_write(ops, context, 0x23U, 6U, record);
    }
}
