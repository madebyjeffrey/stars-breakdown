/* Filename creation and recovery scope; see file-creation.md. */
#include "core.h"
#include <setjmp.h>
#include <stdio.h>
#include <string.h>

static StarsU32 file_long(const StarsU8 *data, StarsU16 offset)
{
    return (StarsU32)data[offset] |
        ((StarsU32)data[(StarsU16)(offset + 1U)] << 8) |
        ((StarsU32)data[(StarsU16)(offset + 2U)] << 16) |
        ((StarsU32)data[(StarsU16)(offset + 3U)] << 24);
}

static StarsU16 file_word(const StarsU8 *data, StarsU16 offset)
{
    return (StarsU16)(data[offset] |
        ((StarsU16)data[(StarsU16)(offset + 1U)] << 8));
}

static void file_put_word(StarsU8 *data, StarsU16 offset, StarsU16 value)
{
    data[offset] = (StarsU8)value;
    data[(StarsU16)(offset + 1U)] = (StarsU8)(value >> 8);
}

/* FWriteDataFile writes each selected player's packed _scorex history as a
 * 24-byte type-0x2d record in player files; the game HST has no score block. */
StarsI16 stars_write_score_records(const StarsWriteScoreRecordsInput *input,
    StarsWriteGameRecord write_record, void *context)
{
    StarsU16 player;
    const StarsU8 *record;
    if (!input || input->player_count > 32U || input->file_player < -1 ||
            input->file_player >= (StarsI16)input->player_count)
        return 0;
    if (input->file_player == -1 || !input->records)
        return 1;
    if (!write_record)
        return 0;
    for (player = 0U; player < input->player_count; ++player) {
        if ((input->visible_players && input->visible_players[player]) ||
                player == (StarsU16)input->file_player ||
                (input->dead_players && input->dead_players[player]) ||
                ((input->game_flags & 0x0040U) != 0U &&
                input->turn > 0x0013U)) {
            record = input->records + player * STARS_SCOREX_RECORD_BYTES;
            write_record(context, 0x2dU, STARS_SCOREX_RECORD_BYTES, record);
        }
    }
    return 1;
}

/* FWriteDataFile's HST thing section starts with a type-0x2b count record,
 * followed by one type-0x2b record for each 18-byte thing. */
StarsI16 stars_write_thing_records(const StarsThing *things, StarsU16 count,
    StarsWriteGameRecord write_record, void *context)
{
    StarsU8 count_record[2];
    StarsU16 index;
    if (count == 0U) return 1;
    if (!things || !write_record) return 0;
    file_put_word(count_record, 0U, count);
    write_record(context, 0x2bU, 2U, count_record);
    for (index = 0U; index < count; ++index)
        write_record(context, 0x2bU, 18U, things[index].raw);
    return 1;
}

/* These two small helpers keep the file-module reconstruction independently
 * linkable for the source-isolation tests while retaining WriteMemRt's state
 * transitions and host callbacks. */
static void file_cancel_mem_rt(StarsU8 *data)
{
    StarsU16 header;
    StarsU16 cursor;
    if (!data) return;
    header = file_word(data, 0x5260U);
    cursor = file_word(data, 0x09a4U);
    file_put_word(data, 0x09a4U,
        (StarsU16)(cursor - (StarsU16)((header & 0x03ffU) + 2U)));
    file_put_word(data, 0x5260U, (StarsU16)(header & 0x03ffU));
}

static void file_write_mem_rt(StarsU8 *data, StarsU8 *log,
    StarsU16 record_type, StarsU16 count, const StarsU8 *input,
    const StarsWriteMemRtOps *ops, void *context)
{
    StarsU16 old_header;
    StarsU16 cursor;
    StarsU16 header;
    StarsI32 end;
    if (!data) return;
    old_header = file_word(data, 0x5260U);
    if (file_word(data, 0x09a8U) == 0U) {
        cursor = file_word(data, 0x09a4U);
        end = (StarsI32)cursor + (StarsI32)count + 2L;
        if (end > 32000L && ops && ops->log_overflow)
            ops->log_overflow(context);
        if (file_word(data, 0x7eU) != 1U) {
            file_put_word(data, 0x7eU, 1U);
            if (file_word(data, 0x1442U) == 0U && ops &&
                    ops->set_message_title)
                ops->set_message_title(context, file_word(data, 0x198U));
        }
        file_put_word(data, 0x09a6U, cursor);
        header = (StarsU16)((count & 0x03ffU) |
            (StarsU16)(record_type << 10));
        if (log) {
            log[cursor] = (StarsU8)header;
            log[(StarsU16)(cursor + 1U)] = (StarsU8)(header >> 8);
            if (count && input)
                memcpy(log + (StarsU16)(cursor + 2U), input, count);
        }
        file_put_word(data, 0x09a4U,
            (StarsU16)(cursor + count + 2U));
        if (record_type == 0U) old_header = header;
    }
    file_put_word(data, 0x5260U, old_header);
}

/* NE 10:99F6-9FA6. Merge a pending transfer delta with a matching prior
 * transfer record, choose the narrowest item representation, and append the
 * resulting type-1, type-2, or type-0x19 log record. */
void stars_log_make_valid_xfer(StarsU8 *data, StarsU8 *log,
    const StarsLogXfer *first, const StarsLogXfer *second,
    const StarsWriteMemRtOps *ops, void *context)
{
    StarsU8 header[2];
    StarsU8 previous[32];
    StarsU8 record[26];
    StarsU32 total[5];
    StarsU32 magnitude;
    StarsU32 max_magnitude = 0;
    StarsU16 previous_type;
    StarsU16 previous_mask;
    StarsU16 mask = 0;
    StarsU16 i;
    StarsU16 previous_index = 0;
    StarsU16 size;
    StarsU16 record_type;
    StarsU32 raw;
    StarsI32 signed_value;
    int matches = 0;
    if (!data || !first || !second) return;
    for (i = 0; i < 5U; ++i) total[i] = 0U;
    previous_type = (StarsU16)(file_word(data, 0x5260U) >> 10);
    if (log && (previous_type == 1U || previous_type == 2U ||
            previous_type == 0x19U) &&
            stars_get_prev_log_rt(data, log, header, previous)) {
        previous_type = (StarsU16)(((StarsU16)header[0] |
            ((StarsU16)header[1] << 8)) >> 10);
        matches = (previous_type == 1U || previous_type == 2U ||
            previous_type == 0x19U) &&
            (file_word(previous, 0) == first->id) &&
            (file_word(previous, 2) == second->id) &&
            ((previous[4] & 0x0fU) == (first->grobj & 0xffU)) &&
            ((previous[4] >> 4) == (second->grobj & 0xffU));
        if (matches) {
            previous_mask = previous[5];
            for (i = 0; i < 5U; ++i) {
                if ((previous_mask & (StarsU16)(1U << i)) != 0U) {
                    if (previous_type == 1U) {
                        signed_value = (StarsI32)(signed char)
                            previous[6U + previous_index];
                    } else if (previous_type == 2U) {
                        signed_value = (StarsI32)(StarsI16)file_word(
                            previous, (StarsU16)(6U + previous_index * 2U));
                    } else {
                        signed_value = (StarsI32)file_long(previous,
                            (StarsU16)(6U + previous_index * 4U));
                    }
                    total[i] = (StarsU32)signed_value;
                    previous_index = (StarsU16)(previous_index + 1U);
                }
            }
            file_cancel_mem_rt(data);
        }
    }
    for (i = 0; i < 5U; ++i) {
        raw = total[i] + first->item[i];
        total[i] = raw;
        signed_value = (StarsI32)raw;
        magnitude = signed_value < 0 ? (StarsU32)(0U - raw) : raw;
        if (magnitude > max_magnitude) max_magnitude = magnitude;
        if (raw != 0U) mask = (StarsU16)(mask | (StarsU16)(1U << i));
    }
    if (mask == 0U) return;
    record[0] = (StarsU8)first->id;
    record[1] = (StarsU8)(first->id >> 8);
    record[2] = (StarsU8)second->id;
    record[3] = (StarsU8)(second->id >> 8);
    record[4] = (StarsU8)((first->grobj & 0x0fU) |
        (StarsU8)((second->grobj & 0x0fU) << 4));
    record[5] = (StarsU8)mask;
    if (max_magnitude < 0x80U) {
        record_type = 1U;
        size = 6U;
        for (i = 0; i < 5U; ++i) if (total[i] != 0U) {
            record[size] = (StarsU8)total[i];
            size = (StarsU16)(size + 1U);
        }
    } else if (max_magnitude < 0x8000U) {
        record_type = 2U;
        size = 6U;
        for (i = 0; i < 5U; ++i) if (total[i] != 0U) {
            record[size] = (StarsU8)total[i];
            record[(StarsU16)(size + 1U)] = (StarsU8)(total[i] >> 8);
            size = (StarsU16)(size + 2U);
        }
    } else {
        record_type = 0x19U;
        size = 6U;
        for (i = 0; i < 5U; ++i) if (total[i] != 0U) {
            record[size] = (StarsU8)total[i];
            record[(StarsU16)(size + 1U)] = (StarsU8)(total[i] >> 8);
            record[(StarsU16)(size + 2U)] = (StarsU8)(total[i] >> 16);
            record[(StarsU16)(size + 3U)] = (StarsU8)(total[i] >> 24);
            size = (StarsU16)(size + 4U);
        }
    }
    file_write_mem_rt(data, log, record_type, size, record, ops, context);
}

static void file_put_long(StarsU8 *data, StarsU16 offset, StarsU32 value)
{
    file_put_word(data, offset, (StarsU16)value);
    file_put_word(data, (StarsU16)(offset + 2U), (StarsU16)(value >> 16));
}

/* NE 15:0006-05E1. Assemble the saved runtime design before merging it
 * into its bank. Hull defaults supply bitmap/slot bounds and base weight;
 * they are not the saved design's initial contents. */
StarsI16 stars_read_shdef(const StarsU8 *record, StarsU16 record_size,
    StarsU8 *design, StarsU16 design_size, StarsI16 player,
    const StarsReadShdefOps *ops, void *context)
{
    StarsU8 hull[143], part[8], candidate[147], decoded[33];
    StarsU8 *target;
    StarsU16 flags, hull_id, bitmap, slot, base, count, name_at;
    StarsU16 name_length, decoded_length, i, at, group, item;
    StarsU16 quantity, old_flags, first_bitmap;
    StarsI16 status;
    StarsU32 weight;
    if (!record || !design || record_size < 7U || !ops ||
            !ops->lookup_hull) return 0;
    flags = file_word(record, 0U);
    hull_id = record[2];
    bitmap = record[3];
    slot = (StarsU16)((flags >> 10) & 31U);
    if (slot >= 16U) slot = (StarsU16)(slot - 16U);
    base = (StarsU16)(slot * 147U);
    if (base + 147U > design_size) return 0;
    memset(candidate, 0, sizeof candidate);
    file_put_word(candidate, 0U, hull_id);
    file_put_word(candidate, 123U, flags);
    candidate[122] = record[6];
    if ((flags & 255U) == 7U) {
        count = record[6];
        if (count > 16U || record_size < 17U + count * 4U) return 0;
        file_put_word(candidate, 56U, file_word(record, 4U));
        memcpy(candidate + 125U, record + 7U, 10U);
        memcpy(candidate + 58U, record + 17U, count * 4U);
        name_at = (StarsU16)(17U + count * 4U);
    } else {
        file_put_word(candidate, 40U, file_word(record, 4U));
        name_at = 6U;
    }
    memset(hull, 0, sizeof hull);
    if (!ops->lookup_hull(context, hull_id, hull, sizeof hull)) return 0;
    first_bitmap = file_word(hull, 50U);
    if (bitmap < first_bitmap || bitmap >= first_bitmap + 4U)
        bitmap = (StarsU16)((bitmap & 3U) | first_bitmap);
    file_put_word(candidate, 50U, bitmap);
    if (name_at >= record_size) return 0;
    name_length = record[name_at++];
    memset(decoded, 0, sizeof decoded);
    if (name_length == 0U) {
        for (i = 0U; name_at + i < record_size && i <= 32U; ++i)
            if (record[name_at + i] == 0U) break;
        if (i > 32U || name_at + i >= record_size) return 0;
        memcpy(candidate + 8U, record + name_at, i + 1U);
    } else {
        if (name_length > 32U || name_at + name_length > record_size ||
                !ops->decode_string) return 0;
        decoded_length = 32U;
        if (!ops->decode_string(context, record + name_at, name_length,
                decoded, &decoded_length) || decoded_length > 32U)
            return 0;
        /* The callback returns decoded characters; the reader terminates. */
        memcpy(candidate + 8U, decoded, decoded_length);
        candidate[8U + decoded_length] = 0U;
    }
    target = design + base;
    old_flags = file_word(target, 123U);
    if ((flags & 255U) == 7U || (old_flags & 0x0200U) != 0U ||
            (old_flags & 255U) < 7U || file_word(target, 0U) != hull_id ||
            file_word(target, 50U) != bitmap)
        memcpy(target, candidate, sizeof candidate);
    if (player >= 0 && ops->update_cost) ops->update_cost(context, target);
    if ((file_word(target, 123U) & 255U) != 7U) return 1;
    count = target[122];
    if (count > 16U || !ops->lookup_part) return 0;
    weight = file_word(hull, 40U);
    for (i = 0U; i < count; ++i) {
        at = (StarsU16)(58U + i * 4U);
        group = file_word(target, at);
        item = target[at + 2U];
        quantity = target[at + 3U];
        if (quantity != 0U) {
            memset(part, 0, sizeof part);
            status = ops->lookup_part(context, group, item, part, sizeof part);
            if (status == 0) return 0;
            if ((group & file_word(hull, at)) == 0U ||
                    (player >= 0 && status > 1 && (flags & 0x8000U) == 0U) ||
                    quantity > hull[at + 3U]) {
                quantity = 0U;
                target[at + 3U] = 0U;
            }
            weight += (StarsU32)file_word(part, 4U) * quantity;
        }
        if (i == 0U && target[at + 3U] == 0U &&
                file_word(hull, 58U) == 1U) {
            file_put_word(target, at, 1U);
            target[at + 2U] = 1U;
            target[at + 3U] = hull[61];
            memset(part, 0, sizeof part);
            if (!ops->lookup_part(context, 1U, 1U, part, sizeof part)) return 0;
            weight += (StarsU32)file_word(part, 4U) * hull[61];
        }
    }
    file_put_word(target, 40U, (StarsU16)weight);
    return 1;
}

/* NE 15:574E-5964. Serialize one runtime ship design into the compact
 * record consumed by FReadShDef.  The two output paths mirror the original:
 * a caller may receive the bytes in its own advancing store pointer, or the
 * record may be sent directly to the runtime-record writer. */
StarsI16 stars_write_rt_shdef(const StarsWriteShdefInput *input,
    StarsU8 *store, StarsU16 store_capacity, StarsU16 *stored,
    const StarsWriteShdefOps *ops, void *context)
{
    StarsU8 record[512];
    StarsU8 compressed[32];
    StarsU16 cursor = 0U;
    StarsU16 components;
    StarsU16 name_length;
    StarsI16 compressed_length = 31;
    StarsU16 length;
    if (!input) return 0;
    memset(record, 0, sizeof record);
    file_put_word(record, 0U, input->flags);
    record[2] = (StarsU8)input->hull_id;
    record[3] = input->bitmap;
    if ((input->flags & 0x00ffU) == 7U) {
        file_put_word(record, 4U, input->design_points);
        record[6] = input->component_count;
        file_put_word(record, 7U, input->turn);
        file_put_long(record, 9U, input->built);
        file_put_long(record, 13U, input->existing);
        cursor = 17U;
        components = input->component_count;
        if (components > 32U) components = 32U;
        if (components != 0U) {
            if (!input->component_data ||
                    (StarsU16)(components * 4U) > input->component_data_size)
                return 0;
            memcpy(record + cursor, input->component_data,
                (size_t)components * 4U);
            cursor = (StarsU16)(cursor + components * 4U);
        }
    } else {
        file_put_word(record, 4U, input->empty_weight);
        cursor = 6U;
    }
    if (!input->name) return 0;
    name_length = (StarsU16)strlen((const char *)input->name);
    if (name_length == 0U || !ops || !ops->compress_name ||
            !ops->compress_name(context, input->name, compressed,
                &compressed_length)) {
        if ((StarsU16)(cursor + name_length + 2U) > (StarsU16)sizeof record)
            return 0;
        record[cursor++] = 0U;
        memcpy(record + cursor, input->name, name_length + 1U);
        cursor = (StarsU16)(cursor + name_length + 1U);
    } else {
        if ((StarsU16)(cursor + (StarsU16)compressed_length + 1U) >
                (StarsU16)sizeof record || compressed_length < 0) return 0;
        record[cursor++] = (StarsU8)compressed_length;
        memcpy(record + cursor, compressed, (size_t)compressed_length);
        cursor = (StarsU16)(cursor + (StarsU16)compressed_length);
    }
    length = cursor;
    if (store) {
        if (length > store_capacity) return 0;
        memcpy(store, record, length);
        if (stored) *stored = length;
    } else if (ops && ops->write_record) {
        ops->write_record(context, 0x1aU, length, record);
    } else {
        return 0;
    }
    return 1;
}

static StarsI16 read_planet_byte(const StarsReadPlanetInput *input,
    StarsU16 *cursor, StarsU8 *value)
{
    if (!input || !cursor || !value || *cursor >= input->payload_size)
        return 0;
    *value = input->payload[*cursor];
    *cursor = (StarsU16)(*cursor + 1U);
    return 1;
}

static StarsI16 read_planet_word(const StarsReadPlanetInput *input,
    StarsU16 *cursor, StarsU16 *value)
{
    StarsU8 low, high;
    if (!read_planet_byte(input, cursor, &low) ||
            !read_planet_byte(input, cursor, &high)) return 0;
    *value = (StarsU16)(low | ((StarsU16)high << 8));
    return 1;
}

static StarsI16 read_planet_long(const StarsReadPlanetInput *input,
    StarsU16 *cursor, StarsU32 *value)
{
    StarsU16 low, high;
    if (!read_planet_word(input, cursor, &low) ||
            !read_planet_word(input, cursor, &high)) return 0;
    *value = (StarsU32)low | ((StarsU32)high << 16);
    return 1;
}

static void read_planet_set_message(const StarsReadPlanetOps *ops, void *context,
    StarsI16 message, const StarsReadPlanetState *state, StarsI16 value)
{
    if (ops && ops->send_message)
        ops->send_message(context, 0, message, state->id, state->id, value);
}

/* NE 15:3206-3A4C. Decode the compact planet payload, preserving the
 * pre-initialized/history distinction and the original four mineral-width
 * encodings.  Static tables and the player-facing notification helpers are
 * callbacks; all record flags and cursor transitions remain ordinary C. */
StarsI16 stars_read_planet(const StarsReadPlanetInput *input,
    StarsReadPlanetState *state, StarsI16 loading_player,
    const StarsReadPlanetOps *ops, void *context)
{
    StarsU16 cursor = 0U;
    StarsU8 bitmap;
    StarsU8 code;
    StarsU16 i;
    StarsU16 low_flags;
    StarsU16 packed;
    StarsI16 notify = 0;
    StarsI16 current;
    StarsI16 value;
    StarsI16 growth;
    StarsU32 long_value;
    StarsU32 encoded;
    if (!input || !state || !input->payload) return 0;
    if (!input->preinitialized) memset(state, 0, sizeof *state);
    current = input->id_player_word;
    state->id = (StarsI16)(current & 0x07ffU);
    if ((current & 0xf800U) == 0xf800U)
        state->player = -1;
    else
        state->player = (StarsI16)((current >> 11) & 0x1fU);
    if (!input->history && loading_player != -1) {
        if (!input->preinitialized) {
            notify = 1;
            state->flags = (StarsU16)(state->flags | 0x0800U);
        } else if ((state->flags & 0x0800U) != 0U) {
            if (input->previous_turn == input->game_turn) notify = 1;
            else state->flags = (StarsU16)(state->flags & 0xf7ffU);
        }
    } else {
        state->flags = (StarsU16)((state->flags & 0xf7ffU) |
            ((input->flags_word >> 15) & 1U) << 11);
    }
    low_flags = (StarsU16)(input->flags_word & 0x007fU);
    if ((state->flags & 0x00ffU) < low_flags)
        state->flags = (StarsU16)((state->flags & 0xff00U) | low_flags);
    state->flags = (StarsU16)((state->flags & 0xfeffU) |
        ((input->flags_word >> 8) & 1U) << 8);
    state->flags = (StarsU16)((state->flags & 0xfdffU) |
        ((input->flags_word >> 9) & 1U) << 9);
    state->flags = (StarsU16)((state->flags & 0xfbffU) |
        ((input->flags_word >> 7) & 1U) << 10);
    if ((state->flags & 0x0200U) != 0U && state->player == -1)
        state->flags = (StarsU16)(state->flags & 0xfdffU);
    if (!input->history) state->turn = input->game_turn;
    if (low_flags > 2U) {
        if (!read_planet_byte(input, &cursor, &bitmap)) return 0;
        for (i = 0; i < 3U; ++i) {
            code = (StarsU8)(bitmap & 3U);
            bitmap = (StarsU8)(bitmap >> 2);
            if (code == 0U) state->mineral_levels[i] = 0U;
            else if (code == 1U) {
                if (!read_planet_byte(input, &cursor,
                        &state->mineral_levels[i])) return 0;
            } else return 0;
        }
        for (i = 0; i < 3U; ++i)
            if (!read_planet_byte(input, &cursor,
                    &state->mineral_concentrations[i])) return 0;
        for (i = 0; i < 3U; ++i) {
            if (!read_planet_byte(input, &cursor, &state->environment[i])) return 0;
            if (state->environment[i] > 100U) return 0;
            state->environment_original[i] = state->environment[i];
        }
        if ((input->flags_word & 0x0400U) != 0U)
            for (i = 0; i < 3U; ++i) {
                if (!read_planet_byte(input, &cursor,
                        &state->environment_original[i])) return 0;
                if (state->environment_original[i] > 100U) return 0;
            }
        if (state->player != -1 &&
                !read_planet_word(input, &cursor, &state->routing_word)) return 0;
        if ((state->flags & 0x00ffU) > 3U &&
                (input->flags_word & 0x2000U) != 0U) {
            if (!read_planet_byte(input, &cursor, &bitmap)) return 0;
            for (i = 0; i < 4U; ++i) {
                code = (StarsU8)(bitmap & 3U);
                bitmap = (StarsU8)(bitmap >> 2);
                if (code == 0U) state->minerals[i] = 0U;
                else if (code == 1U) {
                    if (!read_planet_byte(input, &cursor, &code)) return 0;
                    state->minerals[i] = code;
                } else if (code == 2U) {
                    if (!read_planet_word(input, &cursor, &packed)) return 0;
                    state->minerals[i] = packed;
                } else {
                    if (!read_planet_long(input, &cursor, &long_value)) return 0;
                    state->minerals[i] = long_value;
                }
            }
        }
        if (low_flags > 3U && (input->header_flags >> 10) != 0x0eU) {
            if ((input->flags_word & 0x0800U) == 0U) {
                encoded = (StarsU32)((input->flags_word >> 12) & 1U) << 22;
                /* The FReadPlanet default belongs to overlap_20's second
                 * long: bits 12..16 are the absent-tail sentinel, bit 16 is
                 * always set, and the header's bit 12 maps to runtime bit 22.
                 * Keeping this in [1] lets WritePlanet omit the absent tail
                 * and reproduce unowned HST planet records. */
                state->opaque_20[1] = 0x0001f000UL | encoded;
            } else {
                if (!read_planet_long(input, &cursor, &state->opaque_20[0]) ||
                        !read_planet_long(input, &cursor, &state->opaque_20[1])) return 0;
            }
            if (state->player != -1) {
                if ((state->flags & 0x0200U) != 0U &&
                        !read_planet_long(input, &cursor, &state->opaque_44)) return 0;
                if ((input->flags_word & 0x4000U) != 0U &&
                        !read_planet_word(input, &cursor, &state->opaque_48)) return 0;
            }
            return 1;
        }
    }
    if ((state->flags & 0x0200U) != 0U) {
        if (!read_planet_byte(input, &cursor, &bitmap)) return 0;
        state->opaque_44 = (state->opaque_44 & 0xfffffff0UL) | (bitmap & 0x0fU);
    }
    if (!input->history) {
        if (notify) {
            if (state->player == -1) {
                if ((state->flags & 0x00ffU) < 2U)
                    read_planet_set_message(ops, context, 0x0ad, state, 0);
                else if (ops && ops->race_stat &&
                        ops->race_stat(context, loading_player, 0x0e) == 3) {
                    value = ops->optimal_value ?
                        ops->optimal_value(context, state, loading_player) : 0;
                    read_planet_set_message(ops, context, 0x15d, state, value);
                } else {
                    value = ops && ops->desirability ?
                        ops->desirability(context, state, loading_player) : 0;
                    if (value < 1) {
                        growth = ops && ops->optimal_value ?
                            ops->optimal_value(context, state, loading_player) : 0;
                        if (growth < 1) {
                            value = (StarsI16)(value * 10);
                            notify = 0x0ab;
                        }
                        else {
                            value = (StarsI16)(growth *
                                (ops && ops->true_max_growth ?
                                ops->true_max_growth(context, loading_player) : 0));
                            notify = 0x0ae;
                        }
                    } else {
                        value = (StarsI16)(value *
                            (ops && ops->true_max_growth ?
                            ops->true_max_growth(context, loading_player) : 0));
                        notify = 0x0ac;
                    }
                    growth = value < 0 ? (StarsI16)-value : value;
                    if (ops && ops->send_message)
                        ops->send_message(context, 0, notify, state->id,
                            growth, state->id);
                }
            } else {
                read_planet_set_message(ops, context, 0x0aa, state,
                    (StarsI16)(state->player | 0x30));
            }
        }
    } else {
        if (!read_planet_word(input, &cursor, &state->turn)) return 0;
    }
    (void)current;
    return 1;
}

static StarsI16 write_planet_byte(StarsU8 *record, StarsU16 capacity,
    StarsU16 *cursor, StarsU8 value)
{
    if (!record || !cursor || *cursor >= capacity) return 0;
    record[(*cursor)++] = value;
    return 1;
}

static StarsI16 write_planet_word(StarsU8 *record, StarsU16 capacity,
    StarsU16 *cursor, StarsU16 value)
{
    return write_planet_byte(record, capacity, cursor, (StarsU8)value) &&
        write_planet_byte(record, capacity, cursor, (StarsU8)(value >> 8));
}

static StarsI16 write_planet_long(StarsU8 *record, StarsU16 capacity,
    StarsU16 *cursor, StarsU32 value)
{
    return write_planet_word(record, capacity, cursor, (StarsU16)value) &&
        write_planet_word(record, capacity, cursor, (StarsU16)(value >> 16));
}

/* NE 15:7A6A-81C5. Pack a planet into the variable-width runtime record.
 * Mineral words use the same two-bit width selectors as FReadPlanet; the
 * writer first builds the optional selector span so an all-zero span emits
 * no phantom payload. */
StarsI16 stars_write_planet(const StarsReadPlanetState *state, StarsU16 rt,
    StarsU16 history, StarsU8 *record, StarsU16 capacity, StarsU16 *length,
    StarsWriteGameRecord write_record, void *context)
{
    StarsU16 cursor = 4U;
    StarsU16 flags;
    StarsU16 low_flags;
    StarsU16 i;
    StarsU16 mineral_selector;
    StarsU16 mineral_start;
    StarsU16 code;
    StarsU16 high;
    StarsU32 value;
    const StarsU32 *opaque;
    StarsU8 selector;
    StarsU8 mineral_data[24];
    StarsU16 data_cursor;
    StarsI16 write_opaque_20;
    if (!state || !record || capacity < 4U) return 0;
    memset(record, 0, capacity);
    file_put_word(record, 0U, (StarsU16)((state->id & 0x07ff) |
        (state->player == -1 ? 0xf800U :
        ((StarsU16)state->player << 11))));
    low_flags = (StarsU16)(state->flags & 0x007fU);
    if (rt == 0x0eU && low_flags > 3U)
        low_flags = history ? 3U : 4U;
    flags = (StarsU16)(low_flags |
        (((state->flags >> 8) & 1U) << 8) |
        (((state->flags >> 9) & 1U) << 9) |
        (((state->flags >> 10) & 1U) << 7) |
        (((state->flags >> 11) & 1U) << 15));
    if ((state->opaque_48 & 0x03ffUL) != 0UL) flags |= 0x4000U;
    if (low_flags > 1U) {
        selector = 0U;
        if (capacity <= cursor) return 0;
        mineral_selector = cursor++;
        for (i = 0; i < 3U; ++i) {
            if (state->mineral_levels[i] != 0U) {
                selector = (StarsU8)(selector | (StarsU8)(1U << (i * 2U)));
                if (!write_planet_byte(record, capacity, &cursor,
                        state->mineral_levels[i])) return 0;
            }
        }
        record[mineral_selector] = selector;
        for (i = 0; i < 3U; ++i)
            if (!write_planet_byte(record, capacity, &cursor,
                    state->mineral_concentrations[i])) return 0;
        for (i = 0; i < 3U; ++i) {
            if (!write_planet_byte(record, capacity, &cursor,
                    state->environment[i])) return 0;
            if (state->environment[i] != state->environment_original[i])
                flags |= 0x0400U;
        }
        if ((flags & 0x0400U) != 0U)
            for (i = 0; i < 3U; ++i)
                if (!write_planet_byte(record, capacity, &cursor,
                        state->environment_original[i])) return 0;
        if (state->player != -1 &&
                !write_planet_word(record, capacity, &cursor,
                    state->routing_word)) return 0;
        if (low_flags > 3U) {
            mineral_start = cursor;
            data_cursor = 1U;
            mineral_data[0] = 0U;
            for (i = 0; i < 4U; ++i) {
                if (i == 3U && low_flags <= 6U) continue;
                value = state->minerals[i];
                if (value == 0UL) continue;
                high = (StarsU16)(value >> 16);
                if ((StarsI16)high < 0 ||
                        ((StarsI16)high < 1 && value < 0x100UL)) {
                    code = 1U;
                    mineral_data[data_cursor++] = (StarsU8)value;
                } else if ((StarsI16)high < 1) {
                    code = 2U;
                    mineral_data[data_cursor++] = (StarsU8)value;
                    mineral_data[data_cursor++] = (StarsU8)(value >> 8);
                } else {
                    code = 3U;
                    mineral_data[data_cursor++] = (StarsU8)value;
                    mineral_data[data_cursor++] = (StarsU8)(value >> 8);
                    mineral_data[data_cursor++] = (StarsU8)(value >> 16);
                    mineral_data[data_cursor++] = (StarsU8)(value >> 24);
                }
                mineral_data[0] = (StarsU8)(mineral_data[0] |
                    (StarsU8)(code << (i * 2U)));
            }
            if (mineral_data[0] != 0U) {
                if ((StarsU16)(mineral_start + data_cursor) > capacity) return 0;
                memcpy(record + mineral_start, mineral_data, data_cursor);
                cursor = (StarsU16)(mineral_start + data_cursor);
                flags |= 0x2000U;
            }
            opaque = state->opaque_20;
            flags = (StarsU16)((flags & 0xefffU) |
                (((opaque[1] >> 22) & 1UL) << 12));
            write_opaque_20 = (StarsI16)(
                (state->player != -1 &&
                    ((opaque[0] & 0x00ffUL) != 0UL ||
                     ((opaque[1] >> 23) & 1UL) != 0UL)) ||
                ((opaque[0] >> 8) & 0x0fffUL) != 0UL ||
                ((opaque[0] >> 20) & 0x0fffUL) != 0UL ||
                (opaque[1] & 0x0fffUL) != 0UL ||
                ((opaque[1] >> 12) & 0x001fUL) != 0x001fUL);
            if (rt != 0x0eU && write_opaque_20) {
                flags |= 0x0800U;
                if (!write_planet_long(record, capacity, &cursor,
                        opaque[0]) ||
                        !write_planet_long(record, capacity, &cursor,
                        opaque[1])) return 0;
            }
            if (state->player != -1 && (state->flags & 0x0200U) != 0U) {
                if (!write_planet_long(record, capacity, &cursor,
                        state->opaque_44)) return 0;
            }
            if (state->player != -1 &&
                    (state->opaque_48 & 0x03ffUL) != 0UL &&
                    !write_planet_word(record, capacity, &cursor,
                        state->opaque_48)) return 0;
            file_put_word(record, 2U, flags);
            if (length) *length = cursor;
            if (write_record) write_record(context, 0x0dU, cursor, record);
            return 1;
        }
    }
    if ((state->flags & 0x0200U) != 0U &&
            !write_planet_byte(record, capacity, &cursor,
                (StarsU8)(state->opaque_44 & 0x0fU))) return 0;
    if (history && !write_planet_word(record, capacity, &cursor, state->turn))
        return 0;
    file_put_word(record, 2U, flags);
    if (length) *length = cursor;
    if (write_record) write_record(context, 0x0eU, cursor, record);
    return 1;
}

/* Pack the active production items into original four-byte _prod records.
 * Bit fields come from the recovered _prod layout: cItem (0..9), iItem
 * (10..16), grobj (17..19), and pct (20..26). This typed service handles
 * ordinary queue entries; partial/special items use separate source paths. */
StarsI16 stars_write_production_queue_record(const StarsProduceItem *items,
    StarsU16 item_count, StarsU8 *record, StarsU16 capacity,
    StarsU16 *length, StarsWriteGameRecord write_record, void *context)
{
    StarsU16 index;
    StarsU16 cursor;
    StarsU32 packed;
    const StarsProduceItem *item;
    if (!record || (!items && item_count != 0U) ||
            (StarsU32)item_count * 4UL > capacity || item_count > 255U)
        return 0;
    cursor = 0U;
    for (index = 0U; index < item_count; ++index) {
        item = &items[index];
        if (!item->active || item->partial || item->special ||
                item->quantity < 0 || item->quantity > 1023 ||
                item->type < 0 || item->type > 127 ||
                item->object_kind < 0 || item->object_kind > 7 ||
                item->progress < 0 || item->progress > 127)
            return 0;
        packed = (StarsU32)item->quantity |
            ((StarsU32)item->type << 10) |
            ((StarsU32)item->object_kind << 17) |
            ((StarsU32)item->progress << 20);
        record[cursor++] = (StarsU8)packed;
        record[cursor++] = (StarsU8)(packed >> 8);
        record[cursor++] = (StarsU8)(packed >> 16);
        record[cursor++] = (StarsU8)(packed >> 24);
    }
    if (length) *length = cursor;
    if (item_count != 0U && write_record)
        write_record(context, 0x1cU, cursor, record);
    return 1;
}

static StarsI16 read_fleet_word(const StarsU8 *data, StarsU16 size,
    StarsU16 *cursor, StarsU16 *value)
{
    if (!data || !cursor || !value || (StarsU16)(*cursor + 2U) > size) return 0;
    *value = (StarsU16)(data[*cursor] |
        ((StarsU16)data[(StarsU16)(*cursor + 1U)] << 8));
    *cursor = (StarsU16)(*cursor + 2U);
    return 1;
}

static StarsI16 read_fleet_byte(const StarsU8 *data, StarsU16 size,
    StarsU16 *cursor, StarsU8 *value)
{
    if (!data || !cursor || !value || *cursor >= size) return 0;
    *value = data[*cursor];
    *cursor = (StarsU16)(*cursor + 1U);
    return 1;
}

static StarsI16 read_fleet_long(const StarsU8 *data, StarsU16 size,
    StarsU16 *cursor, StarsU32 *value)
{
    StarsU16 low, high;
    if (!read_fleet_word(data, size, cursor, &low) ||
            !read_fleet_word(data, size, cursor, &high)) return 0;
    *value = (StarsU32)low | ((StarsU32)high << 16);
    return 1;
}

static void read_fleet_failure(const StarsReadFleetOps *ops, void *context)
{
    if (ops && ops->alert) ops->alert(context, 3);
}

/* NE 15:3A4C-40CE. Expand a saved fleet record, including sparse ship and
 * mineral counts, optional order records, and the two fleet-name encodings.
 * The original stream cursor is represented by separate bounded payload spans;
 * this makes every far-memory read explicit without replaying bytes. */
StarsI16 stars_read_fleet(const StarsReadFleetInput *input,
    StarsReadFleetState *state, const StarsReadFleetOps *ops, void *context)
{
    StarsU16 index;
    StarsU16 cursor;
    StarsU16 value;
    StarsU16 code;
    StarsU16 active_ships = 0U;
    StarsU16 ship_index = 0U;
    StarsU16 ship_payload_index = 0U;
    StarsU16 ship_bitmap;
    StarsU16 mineral_cursor = 0U;
    StarsU16 mineral_bitmap;
    StarsU16 order;
    StarsU16 name_length;
    StarsU16 decoded_length;
    StarsU16 bitmap;
    StarsU8 byte_value;
    StarsU8 decoded[32];
    if (!input || !state) return 0;
    memset(state, 0, sizeof *state);
    state->fleet_flags = input->fleet_flags;
    state->planet = input->planet;
    state->x = input->x;
    state->y = input->y;
    ship_bitmap = input->ship_bitmap;
    for (ship_index = 0; ship_index < 16U && ship_bitmap != 0U; ++ship_index) {
        if ((ship_bitmap & 1U) != 0U) {
            if ((input->fleet_flags & 0x0800U) == 0U) {
                if (!input->ship_words) return 0;
                value = input->ship_words[ship_payload_index++];
            } else {
                if (!input->ship_bytes) return 0;
                value = input->ship_bytes[ship_payload_index++];
            }
            state->ship_counts[ship_index] = value;
            if (value != 0U) ++active_ships;
        }
        ship_bitmap = (StarsU16)(ship_bitmap >> 1);
    }
    if (active_ships == 0U) state->fleet_flags =
        (StarsU16)(state->fleet_flags | 0x0400U);
    if ((state->fleet_flags & 0x00ffU) > 3U) {
        mineral_bitmap = input->mineral_bitmap;
        for (index = 0; index < 5U; ++index) {
            code = (StarsU16)(mineral_bitmap & 3U);
            mineral_bitmap = (StarsU16)(mineral_bitmap >> 2);
            if (code == 1U) {
                if (!input->mineral_payload || mineral_cursor >=
                        input->mineral_payload_size) return 0;
                state->minerals[index] = input->mineral_payload[mineral_cursor++];
            } else if (code == 2U) {
                if (!read_fleet_word(input->mineral_payload,
                        input->mineral_payload_size, &mineral_cursor, &value)) return 0;
                state->minerals[index] = value;
            } else if (code == 3U) {
                if (!read_fleet_long(input->mineral_payload,
                        input->mineral_payload_size, &mineral_cursor,
                        &state->minerals[index])) return 0;
            }
        }
    }
    cursor = 0U;
    if ((state->fleet_flags & 0x00ffU) < 7U) {
        if (!read_fleet_long(input->tail_payload, input->tail_payload_size,
                &cursor, &state->overlap_116[0]) ||
                !read_fleet_long(input->tail_payload, input->tail_payload_size,
                &cursor, &state->overlap_44[0])) {
            read_fleet_failure(ops, context);
            return 0;
        }
        return 1;
    }
    if (input->header_type == 0x10U) {
        if (!read_fleet_word(input->tail_payload, input->tail_payload_size,
                &cursor, &value)) return 0;
        bitmap = value;
        for (index = 0; index < 16U && bitmap != 0U; ++index) {
            if ((bitmap & 1U) != 0U) {
                if (!read_fleet_word(input->tail_payload,
                        input->tail_payload_size, &cursor, &value)) return 0;
                if ((value >> 7) > 499U)
                    value = (StarsU16)((value & 0x007fU) | 0xf980U);
                state->ship_counts[(StarsU16)(index + 16U)] = value;
            }
            bitmap = (StarsU16)(bitmap >> 1);
        }
        if (!read_fleet_byte(input->tail_payload, input->tail_payload_size,
                &cursor, &byte_value)) return 0;
        state->design_plan = byte_value;
        if (!read_fleet_byte(input->tail_payload, input->tail_payload_size,
                &cursor, &byte_value)) return 0;
        state->order_count = byte_value > 32U ? 32U : byte_value;
        if (state->order_count > 0U) {
            /* Type-20 compact orders carry eight bytes; keep their data at
             * the normal 18-byte runtime stride and leave the tail clear. */
            if (input->short_record_count > state->order_count) return 0;
            for (order = 0; order < state->order_count; ++order) {
                StarsU16 order_size = order < input->short_record_count ?
                    8U : 18U;
                if ((StarsU16)(cursor + order_size) >
                        input->tail_payload_size) return 0;
                memcpy(state->orders + order * 18U,
                    input->tail_payload + cursor, order_size);
                cursor = (StarsU16)(cursor + order_size);
            }
            for (order = 0; order < state->order_count; ++order) {
                /* fNoAutoTrack is bit 13 of the word at offset 6. The
                 * low byte's bit 5 belongs to iWarp and must survive. */
                StarsU16 offset = (StarsU16)(order * 18U + 7U);
                state->orders[offset] = (StarsU8)(state->orders[offset] & 0xdfU);
            }
        }
        if (state->planet >= 0 && (StarsU16)state->planet >= input->planet_count)
            state->planet = -1;
        if (state->planet >= 0 && input->planet_x && input->planet_y &&
                (state->x != input->planet_x[state->planet] ||
                 state->y != input->planet_y[state->planet])) {
            if (state->order_count != 0U || input->game_turn != 0) {
                read_fleet_failure(ops, context);
                return 0;
            }
            state->x = input->planet_x[state->planet];
            state->y = input->planet_y[state->planet];
        }
    }
    if (input->header_type == 0x15U && input->name_payload &&
            input->name_payload_size != 0U) {
        name_length = input->name_payload[0];
        if (name_length == 0U) {
            name_length = (StarsU16)(input->name_payload_size - 1U);
            if (name_length > 32U) name_length = 32U;
            memcpy(state->name, input->name_payload + 1U, name_length);
        } else if (ops && ops->decompress_name) {
            decoded_length = (StarsU16)sizeof decoded;
            if (ops->decompress_name(context, input->name_payload + 1U,
                    name_length, decoded, &decoded_length) != 0) {
                if (decoded_length > 32U) decoded_length = 32U;
                memcpy(state->name, decoded, decoded_length);
            }
        }
        state->name[32] = 0;
    }
    return 1;
}

static StarsI16 write_fleet_byte(StarsU8 *record, StarsU16 capacity,
    StarsU16 *cursor, StarsU8 value)
{
    if (!record || !cursor || *cursor >= capacity) return 0;
    record[(*cursor)++] = value;
    return 1;
}

static StarsI16 write_fleet_word(StarsU8 *record, StarsU16 capacity,
    StarsU16 *cursor, StarsU16 value)
{
    return write_fleet_byte(record, capacity, cursor, (StarsU8)value) &&
        write_fleet_byte(record, capacity, cursor, (StarsU8)(value >> 8));
}

static StarsI16 write_fleet_long(StarsU8 *record, StarsU16 capacity,
    StarsU16 *cursor, StarsU32 value)
{
    return write_fleet_word(record, capacity, cursor, (StarsU16)value) &&
        write_fleet_word(record, capacity, cursor, (StarsU16)(value >> 16));
}

/* NE 15:81C6-87B4. Serialize a fleet's sparse ship counts and its
 * variable-width mineral or design tail.  The original routine obtains the
 * player-dependent ship weights through a far table; this interface receives
 * that resolved table explicitly, leaving the record format and all width
 * decisions in ordinary C. */
StarsI16 stars_write_fleet(const StarsWriteFleetInput *input,
    StarsU8 *record, StarsU16 capacity, StarsU16 *length,
    const StarsWriteFleetOps *ops, void *context)
{
    StarsU16 cursor = 4U;
    StarsU16 bitmap = 0U;
    StarsU16 extra_bitmap = 0U;
    StarsU16 i;
    StarsU16 low_flags;
    StarsU16 high_word;
    StarsI16 high_signed;
    StarsU16 mineral_selector;
    StarsU32 total;
    StarsU32 value;
    StarsU8 narrow;
    StarsU16 selector;

    if (!input || !record || capacity < 4U) return 0;
    memset(record, 0, capacity);
    low_flags = (StarsU16)(input->fleet_flags & 0x00ffU);
    narrow = 1U;
    for (i = 0; i < 16U; ++i) {
        if (input->ship_counts[i] != 0U) {
            bitmap = (StarsU16)(bitmap | (StarsU16)(1U << i));
            if (input->ship_counts[i] > 0xffU) narrow = 0U;
        }
    }
    file_put_word(record, 0U, (StarsU16)((input->fleet_flags & 0xf7ffU) |
        ((StarsU16)narrow << 11)));
    file_put_word(record, 2U, bitmap);
    if (narrow) {
        for (i = 0; i < 16U; ++i)
            if (input->ship_counts[i] != 0U &&
                    !write_fleet_byte(record, capacity, &cursor,
                    (StarsU8)input->ship_counts[i])) return 0;
    } else {
        for (i = 0; i < 16U; ++i)
            if (input->ship_counts[i] != 0U &&
                    !write_fleet_word(record, capacity, &cursor,
                    input->ship_counts[i])) return 0;
    }

    if (low_flags > 3U) {
        mineral_selector = cursor;
        if (!write_fleet_word(record, capacity, &cursor, 0U)) return 0;
        selector = 0U;
        for (i = 0; i < 5U; ++i) {
            if (low_flags != 7U && (i == 3U || i == 4U)) continue;
            value = input->minerals[i];
            high_word = (StarsU16)(value >> 16);
            high_signed = high_word >= 0x8000U ?
                (StarsI16)((StarsI32)high_word - 65536L) :
                (StarsI16)high_word;
            if (high_signed < 0 ||
                    (high_signed == 0 && (StarsU16)value == 0U)) continue;
            if (high_signed == 0 && (StarsU16)value < 0x100U) {
                selector = (StarsU16)(selector | (StarsU16)(1U << (i * 2U)));
                if (!write_fleet_byte(record, capacity, &cursor,
                        (StarsU8)value)) return 0;
            } else if (high_signed < 1) {
                selector = (StarsU16)(selector | (StarsU16)(2U << (i * 2U)));
                if (!write_fleet_word(record, capacity, &cursor,
                        (StarsU16)value)) return 0;
            } else {
                selector = (StarsU16)(selector | (StarsU16)(3U << (i * 2U)));
                if (!write_fleet_long(record, capacity, &cursor, value)) return 0;
            }
        }
        file_put_word(record, mineral_selector, selector);
    }

    if (low_flags < 7U) {
        total = 0U;
        if (!write_fleet_long(record, capacity, &cursor,
                input->overlap_116[0]) ||
                !write_fleet_long(record, capacity, &cursor,
                input->overlap_116[1])) return 0;
        for (i = 0; i < 16U; ++i)
            if (input->ship_counts[i] != 0U && input->ship_weights)
                total += (StarsU32)input->ship_counts[i] *
                    input->ship_weights[i];
        for (i = 0; i < 4U; ++i) total += input->minerals[i];
        if (!write_fleet_long(record, capacity, &cursor, total)) return 0;
        if (ops && ops->write_record)
            ops->write_record(context, 0x11U, cursor, record);
    } else {
        for (i = 0; i < 16U; ++i)
            if (input->ship_counts[i + 16U] != 0U)
                extra_bitmap = (StarsU16)(extra_bitmap |
                    (StarsU16)(1U << i));
        if (!write_fleet_word(record, capacity, &cursor, extra_bitmap)) return 0;
        for (i = 0; i < 16U; ++i)
            if (input->ship_counts[i + 16U] != 0U &&
                    !write_fleet_word(record, capacity, &cursor,
                    input->ship_counts[i + 16U])) return 0;
        if (!write_fleet_byte(record, capacity, &cursor, input->design_plan) ||
                !write_fleet_byte(record, capacity, &cursor, input->cord)) return 0;
        if (ops && ops->write_record)
            ops->write_record(context, 0x10U, cursor, record);
        if (ops && ops->write_record && input->orders)
            stars_write_orders(input->orders, input->order_count,
                ops->write_record, context);
        if (ops && ops->write_name && input->name && *input->name)
            ops->write_name(context, input->name);
    }
    if (length) *length = cursor;
    return 1;
}

/* NE 15:41AC-4329. Walk the linked battle-history blocks and convert each
 * compact battle entry's ship-count fields from the saved byte form to the
 * in-memory word form.  The block resolver is the explicit replacement for
 * the original far-pointer traversal. */
void stars_update_battle_records(StarsU8 *block, StarsU16 block_size,
    StarsU16 record_offset, StarsNextBattleBlock next_block, void *context)
{
    StarsU16 cursor;
    StarsU16 length;
    StarsU16 subcursor;
    StarsU16 sublength;
    StarsU16 count;
    StarsU16 next_size;
    StarsU16 next_offset;
    StarsU8 *next;

    while (block && record_offset < block_size) {
        cursor = record_offset;
        while ((StarsU16)(cursor + 2U) <= block_size &&
                file_word(block, cursor) == 0xffffU) {
            if (!next_block) return;
            next_offset = record_offset;
            next_size = 0U;
            next = next_block(context, block, block_size, &next_offset,
                &next_size);
            if (!next || next_size < 0x11U) return;
            block = next;
            block_size = next_size;
            record_offset = next_offset;
            cursor = record_offset;
        }
        if ((StarsU16)(cursor + 8U) > block_size) return;
        length = file_word(block, (StarsU16)(cursor + 6U));
        if (length == 0U || (StarsU16)(cursor + length) > block_size) return;
        subcursor = (StarsU16)(cursor +
            (StarsU16)block[(StarsU16)(cursor + 3U)] * 29U + 14U);
        while ((StarsU16)(subcursor + 6U) <= (StarsU16)(cursor + length)) {
            /* The record is packed: the low byte is the old span count and
             * the following byte is the saved per-entry count.  Reading the
             * pair as one aligned C word would lose that distinction on the
             * odd offsets used by the original format. */
            sublength = block[(StarsU16)(subcursor + 2U)];
            if (sublength == 0U) break;
            count = block[(StarsU16)(subcursor + 3U)];
            file_put_word(block, (StarsU16)(subcursor + 2U), count);
            file_put_word(block, (StarsU16)(subcursor + 4U),
                (StarsU16)((file_word(block, (StarsU16)(subcursor + 4U)) &
                    0x00ffU) | ((sublength & 0x00ffU) << 8)));
            subcursor = (StarsU16)(subcursor + sublength * 8U + 6U);
        }
        cursor = (StarsU16)(cursor + length);
        if (cursor == record_offset) return;
        record_offset = cursor;
    }
}

/* NE 15:44B0-484C. Release the current game while preserving the selected
 * object for the next startup and tearing down child windows in the original
 * order.  Heap handles and USER windows are callbacks because they are the
 * two intentionally external parts of the original far-pointer program. */
void stars_destroy_cur_game(StarsDestroyGameState *state,
    const StarsDestroyGameOps *ops, void *context)
{
    StarsU16 i;
    static const StarsU16 heaps[] = { 3U, 5U, 10U, 11U, 6U, 1U, 7U, 0U, 8U };

    if (!state) return;
    if (state->message_entry_open && ops && ops->finish_message_entry)
        ops->finish_message_entry(context);
    if (state->player != -1 && state->dirty && ops && ops->prompt_save)
        (void)ops->prompt_save(context);
    if (ops && ops->reset_heap) ops->reset_heap(context, heaps[0]);
    state->planet_count = 0U;
    if (ops && ops->reset_heap) ops->reset_heap(context, heaps[1]);
    state->fleet_count = 0U;
    if (ops && ops->reset_heap) ops->reset_heap(context, heaps[2]);
    state->thing_count = 0U;
    state->thing_capacity = 0U;
    state->score_count = 0U;
    state->battle_template = 0U;
    state->battle_log = 0U;
    state->battle_current = 0U;
    state->game_flags &= ~0x00001400UL;
    state->game_flags2 &= 0xfbffU;
    for (i = 0; i < 4U; ++i) state->battle_report_cached[i] = 0U;
    if (ops && ops->reset_heap)
        for (i = 3U; i < (StarsU16)(sizeof heaps / sizeof heaps[0]); ++i)
            ops->reset_heap(context, heaps[i]);
    for (i = 0; i < 16U; ++i) {
        state->design_table[i] = 0U;
        state->design_hulls[i] = 0U;
        state->design_costs[i] = 0U;
        state->battle_plan_table[i] = 0U;
        state->battle_plan_counts[i] = 0U;
    }
    if (state->selected_kind && ops && ops->save_selection)
        ops->save_selection(context);
    state->player = -1;
    state->memory_log_current = 0;
    state->memory_log_previous = -1;
    state->message_current = 0;
    state->ai_data_present = 0U;
    if (ops && ops->reset_messages) ops->reset_messages(context);
    state->salt = 0UL;
    state->last_tick = 0UL;
    state->game_lid = 0UL;
    state->player_count = 0U;
    state->planet_capacity = 0U;
    state->dirty = 0U;
    state->turn = 0U;
    state->game_flags &= ~0x00010100UL;
    state->message_entry_open = 0U;
    state->scan_flags = 0U;

    if (state->browser_window && ops && ops->destroy_window)
        ops->destroy_window(context, state->browser_window);
    if (state->report_window && ops && ops->destroy_window)
        ops->destroy_window(context, state->report_window);
    if (state->popup_window && ops && ops->destroy_window)
        ops->destroy_window(context, state->popup_window);
    state->browser_window = 0U;
    state->report_window = 0U;
    state->popup_window = 0U;
    state->active_window = 0U;
    state->orders_visible = 0U;
    state->selected_full_kind = 0U;
    state->selected_kind = 0U;
    state->selected_id = -1;
    state->selected_point.x = 0; state->selected_point.y = 0;
    state->selected_planet = -1;
    state->selected_fleet = -1;
    state->selected_waypoint = -1;
    state->selected_fleet_order = 0U;
    state->selected_planet_production = 0U;
    for (i = 0; i < 3U; ++i) state->control_widths[i] = 0U;
}

/* NE 10:9908-99F6. Accumulate three changed thing words into the pending
 * transfer record and flush it on the next change, unless the game is in the
 * log-suppressed mode represented by gd.grBits bit 1. */
void stars_log_change_thing(StarsU8 *data, StarsU8 *log, StarsU16 *valid,
    StarsLogXfer *pending, const StarsThing *old_thing,
    const StarsThing *new_thing, const StarsWriteMemRtOps *ops,
    void *context)
{
    StarsLogXfer current;
    StarsI16 difference;
    StarsU16 i;
    int changed = 0;
    if (!data || !valid || !pending || !old_thing || !new_thing) return;
    if (((file_word(data, 0x7caU) >> 1) & 1U) != 0U) return;
    memset(&current, 0, sizeof current);
    current.id = file_word(new_thing->raw, 0);
    current.grobj = 8U;
    for (i = 0; i < 3U; ++i) {
        difference = (StarsI16)(StarsU16)(
            file_word(new_thing->raw, (StarsU16)(8U + i * 2U)) -
            file_word(old_thing->raw, (StarsU16)(8U + i * 2U)));
        current.item[i] = (StarsU32)(StarsI32)difference;
        if (difference != 0) changed = 1;
    }
    if (!changed) return;
    if (*valid == 0U) {
        *pending = current;
        *valid = 1U;
    } else {
        stars_log_make_valid_xfer(data, log, pending, &current, ops, context);
        *valid = 0U;
    }
}

/* NE 10:8D5A-8EBE. Replace a fleet name, optionally compress the user
 * string for the log record, and emit type 0x2c with the original five-byte
 * record prefix.  The fleet-name callbacks represent the original heap and
 * far-pointer services; flags bit 11 is the tutor-advance condition. */
void stars_log_change_name(StarsI16 grobj, StarsI16 id, const StarsU8 *name,
    StarsU16 flags, const StarsLogChangeNameOps *ops, void *context)
{
    StarsFleetNameRecord *fleet;
    StarsU8 encoded[33];
    StarsU8 record[38];
    StarsI16 length;
    StarsI16 compressed_length;
    StarsU16 size;
    char *replacement;
    if (!ops || !ops->find_fleet || !ops->write_memory) return;
    fleet = ops->find_fleet(context, id);
    if (!fleet) return;
    if (fleet->name && ops->free_name)
        ops->free_name(context, fleet->name);
    fleet->name = 0;
    memset(encoded, 0, sizeof encoded);
    if (!name || name[0] == 0U) {
        length = 1;
        encoded[0] = 0;
    } else {
        length = (StarsI16)strlen((const char *)name);
        replacement = ops->allocate_name ? ops->allocate_name(context,
            (StarsU16)(length + 1)) : 0;
        if (replacement) {
            strcpy(replacement, (const char *)name);
            fleet->name = replacement;
        }
        compressed_length = length;
        if (ops->compress_name && ops->compress_name(context, name, encoded,
                &compressed_length) != 0) {
            encoded[32] = 0;
            record[4] = (StarsU8)compressed_length;
            length = compressed_length;
        } else {
            memcpy(encoded, name, (size_t)(length + 1));
            record[4] = 0;
            length = (StarsI16)(length + 1);
        }
    }
    record[0] = (StarsU8)id;
    record[1] = (StarsU8)((StarsU16)id >> 8);
    record[2] = (StarsU8)grobj;
    record[3] = (StarsU8)((StarsU16)grobj >> 8);
    if (!name || name[0] == 0U) record[4] = 0;
    memcpy(record + 5, encoded, (size_t)length);
    size = (StarsU16)(length + 5);
    ops->write_memory(context, 0x2cU, size, record);
    if ((flags & 0x0800U) != 0U && ops->advance_tutor)
        ops->advance_tutor(context);
}

/* NE 10:8C28-8D59. Queue a changed ship design in the memory log.  The
 * original calls WriteRtShDef for the variable design tail; write_design is
 * the typed boundary for that already-resolved design serializer, while this
 * routine owns the log header, deletion form, and tutor transition. */
StarsI16 stars_log_change_shdef(const StarsLogChangeShdefInput *input,
    const StarsLogChangeShdefOps *ops, void *context)
{
    StarsU8 record[160];
    StarsU16 header;
    StarsU16 payload_size = 0U;
    if (!input || !ops || !ops->write_memory) return 0;
    if ((input->game_flags & 0x0002U) != 0U) return 1;
    header = (StarsU16)((((input->design_flags >> 10) & 0x001fU) << 8) |
        (((StarsU16)input->player & 0x000fU) << 4));
    if ((input->design_flags & 0x0200U) == 0U) {
        header = (StarsU16)(header | 1U);
        if (input->design_payload) {
            if (ops->write_design)
                payload_size = ops->write_design(context,
                    input->design_payload, record + 2U, 150U);
            else {
                if (input->design_payload_size > 150U) return 0;
                payload_size = input->design_payload_size;
                memcpy(record + 2U, input->design_payload, payload_size);
            }
        }
        if (payload_size > 150U) return 0;
        file_put_word(record, 0U, header);
        ops->write_memory(context, 0x1bU, (StarsU16)(payload_size + 2U),
            record);
    } else {
        file_put_word(record, 0U, header);
        ops->write_memory(context, 0x1bU, 2U, record);
    }
    if ((input->tutor_flags & 0x0800U) != 0U && input->player == 0 &&
            ops->advance_tutor)
        ops->advance_tutor(context);
    return 1;
}

/* NE 10:9FA6-A107. Construct the compact type-0x17 transfer record from the
 * non-empty items in the first transfer descriptor.  The original stores the
 * first and second object kinds as nibbles in one byte, followed by the item
 * presence mask and the selected 16-bit item values. */
void stars_log_make_valid_xferf(const StarsLogXferf *first,
    const StarsLogXferf *second, StarsWriteMemoryLog write_memory,
    void *context)
{
    StarsU8 record[39];
    StarsU16 mask = 0;
    StarsU16 count = 0;
    StarsU16 i;
    StarsU16 size;
    if (!first || !second || !write_memory) return;
    for (i = 0; i < 16U; ++i) {
        if (first->item[i] != 0U) mask = (StarsU16)(mask | (StarsU16)(1U << i));
    }
    if (mask == 0U) return;
    record[0] = (StarsU8)first->id;
    record[1] = (StarsU8)(first->id >> 8);
    record[2] = (StarsU8)second->id;
    record[3] = (StarsU8)(second->id >> 8);
    record[4] = (StarsU8)((first->grobj & 0x0fU) |
        (StarsU8)((second->grobj & 0x0fU) << 4));
    record[5] = (StarsU8)mask;
    record[6] = (StarsU8)(mask >> 8);
    for (i = 0; i < 16U; ++i) {
        if (first->item[i] != 0U) {
            record[7U + count] = (StarsU8)first->item[i];
            record[8U + count] = (StarsU8)(first->item[i] >> 8);
            count = (StarsU16)(count + 2U);
        }
    }
    size = (StarsU16)(7U + count);
    write_memory(context, 0x17U, size, record);
}

/* NE 15:484C-48C3. The original compares against the 25-entry bogi table
 * at DGROUP:0766; the bounded scan preserves the table's real extent while
 * making the host data-image contract explicit. */
StarsI16 stars_file_bogus_long(const StarsU8 *data, StarsU32 serial)
{
    StarsU32 value = serial ^ 0xa5a5a5a5UL;
    StarsU16 i;
    for (i = 0; i < 25U; ++i) {
        StarsU32 entry = file_long(data, (StarsU16)(0x766U + i * 4U));
        if (entry >= value) return (StarsI16)(entry == value);
    }
    return 0;
}

/* NE 15:48C4-4A10. Division and multiplication are unsigned 32-bit
 * operations, matching the original helper calls and wraparound. */
StarsI16 stars_file_valid_serial_long(const StarsU8 *data, StarsU32 serial)
{
    StarsU32 left = serial, rebuilt, remainder;
    StarsU16 i;
    if (stars_file_bogus_long(data, serial)) return 0;
    for (i = 0; i < 4U; ++i) left /= 36U;
    rebuilt = left;
    for (i = 0; i < 4U; ++i) rebuilt *= 36U;
    remainder = serial - rebuilt;
    if (((remainder >> 16) == 0U && remainder < 100U) ||
            remainder > 1500000UL) return 0;
    return (StarsI16)(left == 0x12U || left == 0x16U || left == 2U ||
        left == 4U || left == 6U);
}

/* NE 15:8CFE-8E15. Truncates szBase in place before writing szWork. */
void stars_set_file_name(StarsU8 *data, StarsU16 type, StarsI16 player)
{
    char *base = (char *)data + 0x56a2U, *work = (char *)data + 0x57a4U;
    char *dot = strrchr(base, '.'), *slash;
    StarsU16 number_word;
    StarsI16 number;
    int length, letter;
    if (dot) {
        slash = strrchr(base, '\\');
        if (!slash || slash < dot) *dot = 0;
    }
    length = sprintf(work, (const char *)data + 0xa0cU, base);
    if (type == 2U) strcat(work, (const char *)data + 0xa13U);
    else if (type == 1U || type == 3U || type == 4U) {
        letter = type == 1U ? 'x' : type == 4U ? 'h' : 'm';
        number_word = (StarsU16)((StarsU16)player + 1U);
        number = number_word < 32768U ? (StarsI16)number_word : (StarsI16)((StarsI32)number_word - 65536L);
        sprintf(work + length, (const char *)data + 0xa17U, letter, (int)number);
    } else strcat(work, (const char *)data + 0xa10U);
}

/* NE 15:4A60-4AC1. SetSzWorkFromDt mutates the shared base name exactly as
 * the original does, then records the access result in gd.grBits2 bit 5. */
StarsI16 stars_get_file_status(StarsU8 *data, StarsU16 type, StarsI16 player,
    StarsPathAccess access, void *context)
{
    StarsI16 missing;
    stars_set_file_name(data, type, player);
    missing = access ? (StarsI16)(access(context, data + 0x57a4U, 2) != 0) : 1;
    data[0x7ceU] = (StarsU8)((data[0x7ceU] & 0xdfU) | (missing ? 0x20U : 0));
    return missing;
}

static StarsI16 stars_open_file_failure(const StarsOpenFileInput *input,
    StarsOpenFileState *state, const StarsOpenFileOps *ops, void *context,
    StarsI16 error_id)
{
    if (ops && ops->alert) ops->alert(context, error_id);
    if (state) {
        state->file_error_silent = input->file_error_silent;
        state->opened = 0U;
    }
    if (ops && ops->close_file) ops->close_file(context);
    return 0;
}

/* NE 15:4AC2-4F22. Open a game file, validate its beginning-of-file record,
 * and apply the turn/warp decisions made by the original loader.  The
 * filesystem, record reader and alert are callbacks; the header is a typed
 * view so this adapter does not depend on a host pointer layout. */
StarsI16 stars_open_file(const StarsOpenFileInput *input,
    StarsOpenFileState *state, const StarsOpenFileOps *ops, void *context)
{
    StarsU16 type;
    StarsU16 version_minor;
    StarsU16 saved_gd;
    StarsI16 response;
    if (!input || !state) return 0;
    memset(state, 0, sizeof *state);
    state->game_turn = input->game_turn;
    state->game_flags = input->game_flags;
    state->gd_flags = (StarsU16)(input->gd_flags & 0xff7fU);
    state->file_error_silent = input->file_error_silent;
    type = (StarsU16)(input->type & 0x00ffU);
    (void)input->game_id;
    if (input->data) stars_set_file_name(input->data, type, input->player);
    state->file_error_silent = 1U;
    if (!ops || !ops->open_file ||
            !ops->open_file(context, type, input->player, input->mode))
        return stars_open_file_failure(input, state, ops, context, 4);
    state->opened = 1U;
    state->file_error_silent = input->file_error_silent;
    version_minor = (StarsU16)((input->header_version >> 5) & 0x007fU);
    if (input->header_type != 8U)
        return stars_open_file_failure(input, state, ops, context, 13);
    if ((input->header_version >> 12) != 2U || version_minor <= 0x30U ||
            version_minor >= 0x54U)
        return stars_open_file_failure(input, state, ops, context, 0x2ca);
    if ((input->header_flags & 0x001fU) != (StarsU16)input->player)
        return stars_open_file_failure(input, state, ops, context, 3);
    if (input->game_lid != 0UL && input->header_game_lid != input->game_lid)
        return stars_open_file_failure(input, state, ops, context, 0x1d);
    if (type == 4U) return 1;
    if ((input->type & 0x2000U) != 0U &&
            (input->header_flags & 0x0400U) != 0U) {
        /* The original seeks to and reads the trailing turn header here.
         * Callers that need that branch provide the already-read header. */
    }
    if (state->game_turn == 0U && input->header_turn != 0U) {
        state->game_turn = input->header_turn;
        state->game_flags = (StarsU16)((state->game_flags & 0xf1ffU) |
            (((input->header_flags >> 13) & 7U) << 9));
        return 1;
    }
    if (input->header_turn != state->game_turn)
        return stars_open_file_failure(input, state, ops, context, 0x1c);
    if (type == 2U && (state->gd_flags & 8U) == 0U &&
            (input->header_flags & 0x0200U) != 0U) {
        response = ops && ops->alert ? ops->alert(context, 0x15) : 0;
        if (response == 6) return 1;
    } else if ((input->header_flags & 0x0100U) == 0U &&
            (state->gd_flags & 2U) != 0U && (state->gd_flags & 4U) == 0U) {
        saved_gd = state->gd_flags;
        state->gd_flags = (StarsU16)(saved_gd | 0x0080U);
        return 1;
    } else if (type != 1U || (state->game_flags & 8U) != 0U ||
            ((input->header_flags >> 13) & 7U) ==
            ((state->game_flags >> 9) & 7U)) {
        return 1;
    } else {
        return stars_open_file_failure(input, state, ops, context, 0x1d);
    }
    return stars_open_file_failure(input, state, ops, context, 0x1d);
}

/* NE 15:904A-947B. Open a saved file, validate its BOF, and change one of
 * the mark bits in either that BOF or the selected player's record.  The
 * record writer is deliberately a callback because the original seeks back
 * into the file and rewrites the record in place. */
StarsI16 stars_mark_file(const StarsMarkFileInput *input,
    StarsMarkFileState *state, const StarsMarkFileOps *ops, void *context)
{
    StarsU16 type;
    StarsU16 major;
    StarsU16 minor;
    StarsU16 old_bit;
    StarsU16 new_flags;
    StarsU8 body[16];
    if (!input || !state) return 0;
    memset(state, 0, sizeof *state);
    type = (StarsU16)(input->type & 0x00ffU);
    state->header_flags = input->header_flags;
    state->player_record_flags = input->player_record_flags;
    if (!ops || !ops->open_file ||
            !ops->open_file(context, type, input->player, 0x12U))
        return 0;
    state->file_open = 1U;
    major = (StarsU16)(input->header_version >> 12);
    minor = (StarsU16)((input->header_version >> 5) & 0x007fU);
    if (input->header_type != 8U)
        goto mark_error_type;
    if (major < 2U || (major == 2U && minor < 0x31U))
        goto mark_error_version;
    if (major >= 3U || (major == 2U && minor >= 0x54U))
        goto mark_error_version;
    if (input->game_lid != 0UL &&
            input->header_game_lid != input->game_lid)
        goto mark_error_game;
    state->changed = 0U;
    if (input->mark_mode == 1U || input->mark_mode == 2U ||
            input->mark_mode == 4U) {
        old_bit = (StarsU16)((state->header_flags >>
            (input->mark_mode == 1U ? 9 :
             input->mark_mode == 2U ? 8 : 10)) & 1U);
        if (old_bit != (StarsU16)(input->value & 1)) {
            new_flags = (StarsU16)(state->header_flags &
                (input->mark_mode == 1U ? 0xfdffU :
                 input->mark_mode == 2U ? 0xfeffU : 0xfbffU));
            new_flags = (StarsU16)(new_flags |
                ((StarsU16)(input->value & 1) <<
                (input->mark_mode == 1U ? 9 :
                 input->mark_mode == 2U ? 8 : 10)));
            state->header_flags = new_flags;
            state->changed = 1U;
        }
    } else if (input->mark_mode == 8U) {
        if (input->player_record_type != 6U)
            goto mark_error_game;
        old_bit = (StarsU16)((state->player_record_flags >> 9) & 1U);
        if (old_bit != (StarsU16)(input->value & 1)) {
            if (old_bit != 0U &&
                    (input->player_record_flags >> 13) != 7U)
                goto mark_error_game;
            state->player_record_flags = (StarsU16)(
                state->player_record_flags & 0xfdffU);
            if ((input->value & 1) != 0)
                state->player_record_flags = (StarsU16)(
                    state->player_record_flags | 0xe200U);
            state->changed = 1U;
            if (ops->write_record) {
                memset(body, 0, sizeof body);
                body[0] = (StarsU8)state->player_record_flags;
                body[1] = (StarsU8)(state->player_record_flags >> 8);
                ops->write_record(context, 6U, body, 16U);
            }
            state->header_flags = (StarsU16)(state->header_flags & 0xfeffU);
        }
    }
    if (state->changed && input->mark_mode != 8U && ops->write_record) {
        memset(body, 0, sizeof body);
        body[0] = (StarsU8)state->header_flags;
        body[1] = (StarsU8)(state->header_flags >> 8);
        ops->write_record(context, 8U, body, 16U);
    }
    if ((input->type & 0x2000U) == 0U || state->changed == 0U) {
        if (ops->close_file) ops->close_file(context);
        state->file_open = 0U;
    }
    return 1;
mark_error_type:
    if (ops && ops->alert) ops->alert(context, 13);
    goto mark_failure;
mark_error_version:
    if (ops && ops->alert) ops->alert(context, 0x2ca);
    goto mark_failure;
mark_error_game:
    if (ops && ops->alert) ops->alert(context, 0x1d);
mark_failure:
    if (ops && ops->close_file) ops->close_file(context);
    state->file_open = 0U;
    state->changed = 0U;
    return 0;
}

/* NE 15:4F22-4FB2. FOpenFile supplies the file header and may update the
 * game turn; the host callback preserves that integration boundary. */
StarsI16 stars_new_turn_available(StarsU8 *data, StarsI16 player,
    StarsOpenGameFile open_file, StarsCloseGameFile close_file, void *context)
{
    StarsU16 old_turn = file_word(data, 0x82U);
    StarsU16 old_warp = (StarsU16)(file_word(data, 0x80U) >> 9);
    StarsI16 opened;
    StarsI16 result = 0;
    file_put_word(data, 0x74cU, 1U);
    file_put_word(data, 0x82U, 0U);
    opened = open_file ? open_file(context, 0x2003U, player, 0x20U) : 0;
    if (opened) {
        if (close_file) close_file(context);
        result = (StarsI16)(old_turn < file_word(data, 0x82U));
    }
    file_put_word(data, 0x82U, old_turn);
    file_put_word(data, 0x80U, (StarsU16)((file_word(data, 0x80U) & 0xf1ffU) |
        ((old_warp & 7U) << 9)));
    return result;
}

/* NE 15:4FB2-5167. Header reads for mode 8 remain an explicit callback so
 * the host can provide the same sequential records as ReadRt. */
StarsI16 stars_check_file(StarsU8 *data, StarsU16 type, StarsI16 player,
    StarsU16 mode, StarsOpenGameFile open_file, StarsReadGameRecord read_record,
    StarsCloseGameFile close_file, void *context)
{
    StarsU16 saved_silent = file_word(data, 0x74cU);
    StarsU16 saved_warp = (StarsU16)(file_word(data, 0x80U) >> 9);
    StarsU16 saved_file_flag = (StarsU16)((file_word(data, 0x7caU) >> 3) & 1U);
    StarsI16 opened, result = 0;
    if (type == 2U)
        file_put_word(data, 0x7caU, (StarsU16)((file_word(data, 0x7caU) & 0xfff7U) | 8U));
    file_put_word(data, 0x74cU, 1U);
    opened = open_file ? open_file(context, type, player, 0x20U) : 0;
    if (mode == 1U)
        result = (StarsI16)(opened && !(file_word(data, 0x4bacU) & 0x0200U));
    else if (mode == 2U)
        result = (StarsI16)(opened && (file_word(data, 0x4bacU) & 0x0100U));
    else if (mode == 4U)
        result = (StarsI16)(opened && (file_word(data, 0x4bacU) & 0x0400U));
    else if (mode == 8U && opened) {
        do {
            if (read_record) read_record(context);
            else break;
        } while ((file_word(data, 0x2696U) >> 10) != 6U &&
            data[0x4b98U] != (StarsU8)player);
        result = (StarsI16)((file_word(data, 0x4b9eU) >> 9) & 1U);
    }
    if (opened && close_file) close_file(context);
    if (type == 2U)
        file_put_word(data, 0x7caU, (StarsU16)((file_word(data, 0x7caU) & 0xfff7U) |
            (saved_file_flag << 3)));
    file_put_word(data, 0x74cU, saved_silent);
    file_put_word(data, 0x80U, (StarsU16)((file_word(data, 0x80U) & 0xf1ffU) |
        ((saved_warp & 7U) << 9)));
    return result;
}

/* NE 15:547E-551B. Each order occupies the original 0x12-byte record;
 * the low nibble at offset 6 selects the compact or full stream record. */
void stars_write_orders(const StarsU8 *orders, StarsU16 count,
    StarsWriteGameRecord write_record, void *context)
{
    StarsU16 i;
    if (!orders || !write_record) return;
    for (i = 0; i < count; ++i) {
        const StarsU8 *order = orders + i * 0x12U;
        if ((order[6] & 0x0fU) == 0U)
            write_record(context, 0x14U, 8U, order);
        else
            write_record(context, 0x13U, 0x12U, order);
    }
}

/* NE 15:87B4-885D. The first byte of a type-0x15 record is the compressed
 * length, or zero when the original string is copied literally. */
void stars_write_runtime_string(const StarsU8 *dgroup, const StarsU8 *input,
    StarsWriteGameRecord write_record, void *context)
{
    StarsU8 buffer[36];
    StarsI16 capacity = 31, length;
    if (!input || !*input || !write_record) return;
    if (stars_compress_user_string(dgroup, input, buffer + 1U, &capacity)) {
        buffer[0] = (StarsU8)capacity;
        write_record(context, 0x15U, (StarsU16)(capacity + 1), buffer);
        return;
    }
    strcpy((char *)buffer + 1, (const char *)input);
    buffer[0] = 0;
    length = (StarsI16)(strlen((const char *)input) + 1U);
    write_record(context, 0x15U, (StarsU16)(length + 1), buffer);
}

/* NE 15:704E-709B. The two save helpers remain explicit host boundaries. */
StarsI16 stars_append_file(StarsI16 player, StarsMarkGameFile mark_file,
    StarsWriteBofRecord write_bof, void *context)
{
    StarsI16 marked = mark_file ? mark_file(context, 0x2003U, player, 4U, 1) : 0;
    if (marked && write_bof) write_bof(context, player, 3, 1);
    return (StarsI16)(marked != 0);
}

/* NE 15:89B8-8ADB. The battle-plan header is four bytes; a deleted plan
 * writes only its first word, while a live plan appends its length-prefixed
 * compressed-or-plain name. */
void stars_write_battle_plan(const StarsU8 *plan, const StarsU8 *dgroup,
    StarsI16 log, StarsWriteGameRecord write_rt,
    StarsWriteGameRecord write_mem, void *context)
{
    StarsU8 buffer[38];
    StarsI16 capacity = 31, length;
    StarsU16 size = 4U;
    StarsWriteGameRecord write_record = log ? write_mem : write_rt;
    if (!plan || !write_record) return;
    memcpy(buffer, plan, 4U);
    if (buffer[1] & 0x40U) {
        write_record(context, 0x1eU, 2U, buffer);
        return;
    }
    if (plan[4] && stars_compress_user_string(dgroup, plan + 4U,
            buffer + 5U, &capacity)) {
        buffer[4] = (StarsU8)capacity;
        size = (StarsU16)(5U + capacity);
    } else {
        strcpy((char *)buffer + 5U, (const char *)plan + 4U);
        buffer[4] = 0;
        length = (StarsI16)(strlen((const char *)plan + 4U) + 1U);
        size = (StarsU16)(5U + length);
    }
    write_record(context, 0x1eU, size, buffer);
}

/* NE 10:93D0-9420. Log the changed plan and notify the tutor for player 0
 * when the original tutorial bit is active. */
void stars_log_change_btlplan(StarsU8 *data, const StarsU8 *plan,
    const StarsLogChangeBtlplanOps *ops, void *context)
{
    if (!data || !ops) return;
    if (ops->write_battle_plan)
        ops->write_battle_plan(context, plan, 1);
    if ((file_word(data, 0x7caU) & 0x0800U) != 0U &&
        file_word(data, 0x18cU) == 0U) {
        StarsU16 tutor_flags = (StarsU16)((file_word(data, 0x520cU) &
            0xfffbU) | 4U);
        data[0x520cU] = (StarsU8)tutor_flags;
        data[0x520dU] = (StarsU8)(tutor_flags >> 8);
        if (ops->advance_tutor) ops->advance_tutor(context);
    }
}

/* NE 10:8B0E-8B6B. Split logging is suppressed during playback; otherwise
 * the plan record is written, or the live fleet receives its split flag. */
void stars_log_split_fleet(StarsU8 *data, StarsI16 id,
    const StarsLogSplitFleetOps *ops, void *context)
{
    StarsU16 flags;
    StarsU8 *fleet;
    StarsU8 id_bytes[2];
    if (!data || !ops) return;
    id_bytes[0] = (StarsU8)id;
    id_bytes[1] = (StarsU8)((StarsU16)id >> 8);
    if ((file_word(data, 0x7caU) & 2U) == 0U) {
        if (ops->write_memory)
        ops->write_memory(context, 0x18U, 2U, id_bytes);
        return;
    }
    fleet = ops->find_fleet ? ops->find_fleet(context, id) : 0;
    if (fleet) {
        flags = (StarsU16)(fleet[0x118U] |
            ((StarsU16)fleet[0x119U] << 8));
        flags = (StarsU16)((flags & 0xffdfU) | 0x20U);
        fleet[0x118U] = (StarsU8)flags;
        fleet[0x119U] = (StarsU8)(flags >> 8);
    } else if (ops->write_memory) {
        ops->write_memory(context, 0x18U, 2U, id_bytes);
    }
}

/* NE 10:A25E-A2D5. Extract the previous log header and bounded payload from
 * the supplied log image; the original -1 cursor remains the no-record case. */
StarsI16 stars_get_prev_log_rt(const StarsU8 *data, const StarsU8 *log,
    StarsU8 *header, StarsU8 *payload)
{
    StarsU16 cursor;
    StarsU16 value;
    if (!data || !log || !header || !payload) return 0;
    cursor = (StarsU16)(data[0x9a6U] | ((StarsU16)data[0x9a7U] << 8));
    if (cursor == 0xffffU) return 0;
    value = (StarsU16)(log[cursor] | ((StarsU16)log[(StarsU16)(cursor + 1U)] << 8));
    header[0] = (StarsU8)value;
    header[1] = (StarsU8)(value >> 8);
    value = (StarsU16)(value & 0x03ffU);
    if (value) memcpy(payload, log + (StarsU16)(cursor + 2U), value);
    return 1;
}

/* NE 10:A2D6-A38B. Replay the length-prefixed log image through the host
 * record dispatcher while preserving the original fLogOff state. */
StarsI16 stars_run_log_file(StarsU8 *data, const StarsU8 *log,
    StarsRunLogRecord run_record, void *context)
{
    StarsU16 saved_log_off;
    StarsU16 cursor = 0;
    StarsU16 limit;
    StarsU16 header;
    StarsU16 result = 1;
    if (!data) return 0;
    saved_log_off = file_word(data, 0x9a8U);
    limit = file_word(data, 0x9a4U);
    if (limit) {
        data[0x9a8U] = 1;
        data[0x9a9U] = 0;
        while (cursor < limit) {
            header = (StarsU16)(log ? log[cursor] : 0);
            if (log)
                header = (StarsU16)(header |
                    ((StarsU16)log[(StarsU16)(cursor + 1U)] << 8));
            if (run_record)
                result = (StarsU16)(result & run_record(context,
                    (StarsU16)(header >> 10), (StarsU16)(header & 0x03ffU),
                    log ? log + (StarsU16)(cursor + 2U) : 0));
            cursor = (StarsU16)(cursor + (header & 0x03ffU) + 2U);
        }
        {
            StarsU16 flags = file_word(data, 0x7ceU);
            flags = (StarsU16)(flags & 0xfbffU);
            data[0x7ceU] = (StarsU8)flags;
            data[0x7cfU] = (StarsU8)(flags >> 8);
        }
    }
    data[0x9a8U] = (StarsU8)saved_log_off;
    data[0x9a9U] = (StarsU8)(saved_log_off >> 8);
    return result < 32768U ? (StarsI16)result :
        (StarsI16)((StarsI32)result - 65536L);
}

/* NE 10:D6E0-D782. Enumerate the current log image and stop immediately when
 * the caller's record visitor returns zero. */
void stars_enum_log_rts(const StarsU8 *data, const StarsU8 *log,
    StarsEnumLogRecord visit, void *context, void *pass)
{
    StarsU16 cursor = 0;
    StarsU16 limit;
    StarsU16 header;
    if (!data || !log || !visit) return;
    limit = file_word(data, 0x9a4U);
    while (cursor < limit) {
        header = (StarsU16)(log[cursor] |
            ((StarsU16)log[(StarsU16)(cursor + 1U)] << 8));
        if (!visit(context, (StarsU16)(header >> 10),
            (StarsU16)(header & 0x03ffU),
            log + (StarsU16)(cursor + 2U), pass)) return;
        cursor = (StarsU16)(cursor + (header & 0x03ffU) + 2U);
    }
}

/* NE 10:8B6C-8C27. Build bounded merge groups from the pending fleet list;
 * playback suppresses the memory-log write exactly as in the original. */
void stars_log_merge_fleet(StarsU8 *data, StarsI16 id,
    StarsWriteMemoryLog write_memory, void *context)
{
    StarsI16 group[0x1ff];
    StarsU16 total;
    StarsU16 cursor = 0;
    StarsU16 i;
    StarsU16 count;
    StarsI16 value;
    if (!data || !write_memory || (file_word(data, 0x7caU) & 2U) != 0U)
        return;
    total = file_word(data, 0x2578U);
    while (cursor < total) {
        count = 1;
        group[0] = id;
        for (i = cursor; i < total && count < 0x1ffU; ++i) {
            value = (StarsI16)file_word(data,
                (StarsU16)(0x51bcU + i * 2U));
            if (value != -1 && value != id) group[count++] = value;
        }
        cursor = i;
        write_memory(context, 0x25U, (StarsU16)(count * 2U),
            (const StarsU8 *)group);
    }
}

/* NE 10:9340-93CF. Reuse the previous relation record when it is already a
 * relation log entry, then append the current relation state. */
void stars_log_change_relations(StarsU8 *data, const StarsU8 *log,
    const StarsU8 *relations, const StarsLogChangeRelationsOps *ops,
    void *context)
{
    StarsU8 header[2];
    StarsU8 payload[1024];
    StarsU16 player_count;
    StarsU16 previous;
    if (!data || !ops) return;
    previous = file_word(data, 0x9a6U);
    if (stars_get_prev_log_rt(data, log, header, payload) &&
        (((StarsU16)header[0] | ((StarsU16)header[1] << 8)) >> 10) == 0x26U) {
        data[0x9a4U] = (StarsU8)previous;
        data[0x9a5U] = (StarsU8)(previous >> 8);
    }
    player_count = file_word(data, 0x78U);
    if (ops->write_memory)
        ops->write_memory(context, 0x26U, player_count, relations);
    if ((file_word(data, 0x7caU) & 0x0800U) != 0U &&
        file_word(data, 0x18cU) == 0U) {
        StarsU16 tutor_flags = (StarsU16)((file_word(data, 0x520cU) &
            0xfffbU) | 4U);
        data[0x520cU] = (StarsU8)tutor_flags;
        data[0x520dU] = (StarsU8)(tutor_flags >> 8);
        if (ops->advance_tutor) ops->advance_tutor(context);
    }
}

/* NE 15:40CE-41AB. A battle-plan record begins with the four-byte plan
 * header, followed by a one-byte name encoding length. A zero length denotes
 * a literal NUL-terminated name; otherwise the next 32 bytes are copied to a
 * scratch span before the original user-string decoder expands them. The
 * plan number is then installed in header bits 4..7 without disturbing the
 * player, tactic, delete, or cargo flags. */
void stars_unpack_battle_plan(const StarsU8 *dgroup, const StarsU8 *input,
    StarsU8 *plan, StarsI16 plan_number)
{
    StarsU8 encoded[32];
    StarsU8 decoded[32];
    StarsI16 capacity = 32;
    StarsU16 length;
    if (!dgroup || !input || !plan) return;
    memset(encoded, 0, sizeof encoded);
    memset(decoded, 0, sizeof decoded);
    memcpy(plan, input, 4U);
    length = input[4];
    if (length == 0U) {
        strcpy((char *)plan + 4, (const char *)input + 5);
    } else {
        memcpy(encoded, input + 5, sizeof encoded);
        stars_decompress_user_string(dgroup, encoded, (StarsI16)length,
            decoded, &capacity);
        memcpy(plan + 4, decoded, sizeof decoded);
    }
    plan[0] = (StarsU8)((plan[0] & 0x0fU) |
        (((StarsU16)plan_number & 0x0fU) << 4));
}

/* NE 15:432A-43ED. This is the save-dialog callback's complete message
 * dispatch. EndDialog and WinHelp remain explicit host services; lParam is
 * accepted because it is part of the Pascal callback ABI but is unused by the
 * original body. */
StarsI16 stars_ask_save_dialog(StarsU16 hwnd, StarsU16 message,
    StarsU16 wparam, StarsI32 lparam, const StarsU8 *help_file,
    StarsEndDialog end_dialog, StarsWinHelp win_help, void *context)
{
    StarsI16 result;
    (void)lparam;
    if (message == 2U) return 0;
    if (message == 0x110U) return 1;
    if (message != 0x111U) return 0;
    if (wparam == 0x429U || wparam == 0x42bU || wparam == 0x42aU) {
        if (wparam == 0x42bU) result = 0;
        else if (wparam == 0x42aU) result = (StarsI16)-1;
        else result = 1;
        if (end_dialog) end_dialog(context, hwnd, result);
        return 1;
    }
    if (wparam == 0x76U) {
        if (win_help) win_help(context, hwnd, help_file, 1U, 0x442U);
        return 1;
    }
    return 0;
}

/* NE 15:43EE-44AF. The callback instance and dialog manager are host
 * services, while resource selection, result interpretation, the gd view
 * bit, and the subsequent log/history writes remain in the recovered C
 * control flow. */
void stars_prompt_save_game(StarsU8 *data, const StarsPromptSaveOps *ops,
    void *context)
{
    StarsU16 instance, resource, frame, player;
    StarsI16 result;
    void *procedure;
    if (!data || !ops) return;
    instance = file_word(data, 0x5310U);
    resource = (StarsU16)((file_word(data, 0x80U) & 4U) ? 0x7e9U : 0x42cU);
    frame = file_word(data, 0x258cU);
    player = file_word(data, 0x18cU);
    procedure = ops->make_proc ? ops->make_proc(context, instance) : NULL;
    result = ops->dialog_box ? ops->dialog_box(context, instance, resource,
        frame, procedure) : 0;
    if (ops->free_proc) ops->free_proc(context, procedure);
    if (result == 0) return;
    file_put_word(data, 0x7caU, (StarsU16)((file_word(data, 0x7caU) & 0xffefU) |
        ((result == (StarsI16)-1 ? 1U : 0U) << 4)));
    if (ops->write_log) ops->write_log(context, data + 0x56a2U, (StarsI16)player);
    if (ops->write_history) ops->write_history(context, (StarsI16)player);
}

/* NE 15:95BC-9652. The pass routines own the visibility calculations; this
 * function preserves their order and the three progress notifications. The
 * all-player sentinel updates player-zero's visible-planet count directly,
 * matching the original early path. */
void stars_set_visible_plan_fleet(StarsU8 *data, StarsI16 player,
    const StarsVisiblePlanFleetOps *ops, void *context)
{
    if (!data || !ops) return;
    if (ops->initialize) ops->initialize(context, player);
    if (player == (StarsI16)-1) {
        file_put_word(data, 0x59a4U, file_word(data, 0x7aU));
        return;
    }
    if (ops->progress) ops->progress(context, (StarsI16)-0x39f);
    if (ops->fleets) ops->fleets(context, player);
    if (ops->progress) ops->progress(context, (StarsI16)-0x39f);
    if (ops->planets) ops->planets(context, player);
    if (ops->progress) ops->progress(context, (StarsI16)-0x39f);
    if (ops->things) ops->things(context, player);
    if (ops->finish) ops->finish(context, player);
}

/* NE 15:C41C-C852. Finish visibility by converting the per-player knowledge
 * masks into the compact design/starbase flags consumed by later screens. */
void stars_set_visible_pf_finish(const StarsVisibleFinishInput *input)
{
    StarsU16 player;
    StarsU16 item;
    StarsU16 flags;
    StarsU16 default_level;
    StarsU32 viewer_bit;
    StarsVisibleFinishPlayer *current;
    if (!input || !input->players || input->viewer < 0) return;
    viewer_bit = 1UL << ((StarsU16)input->viewer & 31U);
    default_level = (StarsU16)(input->race_stat == 2 ? 7U : 3U);
    for (player = 0; player < input->player_count; ++player) {
        if ((StarsI16)player == input->viewer) continue;
        current = &input->players[player];
        current->shdef_count = 0U;
        for (item = 0; item < 16U; ++item) {
            flags = current->fleet_flags[item];
            if ((current->fleet_known[item] & viewer_bit) == 0UL) {
                if ((flags & 0x0100U) == 0U) continue;
                current->fleet_flags[item] = (StarsU16)((flags & 0xff00U) |
                    default_level);
            } else {
                current->fleet_flags[item] = (StarsU16)((flags & 0xfeffU) |
                    0x0100U);
                current->fleet_flags[item] = (StarsU16)((
                    current->fleet_flags[item] & 0xff00U) | 7U);
            }
            current->detail_flags = (StarsU16)((current->detail_flags &
                0xfff8U) | 3U);
            current->detail_flags = (StarsU16)((current->detail_flags &
                0xfeffU) | 0x0100U);
            ++current->shdef_count;
        }
        current->fleet_count = (StarsU16)(current->fleet_count & 0x0fffU);
        for (item = 0; item < 10U; ++item) {
            flags = current->starbase_flags[item];
            if ((current->starbase_known[item] & viewer_bit) == 0UL) {
                if ((flags & 0x0100U) == 0U) continue;
                current->starbase_flags[item] = (StarsU16)((flags & 0xff00U) |
                    default_level);
            } else {
                current->starbase_flags[item] = (StarsU16)((flags & 0xfeffU) |
                    0x0100U);
                current->starbase_flags[item] = (StarsU16)((
                    current->starbase_flags[item] & 0xff00U) | 7U);
            }
            current->detail_flags = (StarsU16)((current->detail_flags &
                0xfeffU) | 0x0100U);
            current->detail_flags = (StarsU16)((current->detail_flags &
                0xfff8U) | 3U);
            current->fleet_count = (StarsU16)((current->fleet_count & 0x0fffU) |
                ((current->fleet_count + 0x1000U) & 0xf000U));
        }
    }
}

/* NE 1C:1DBA-1EB6. Combine the player's colonist burden with the level table,
 * apply the race technology modifier, and finally apply the game's doubled
 * research-cost flag. */
StarsI32 stars_get_tech_level_cost(const StarsTechLevelCostInput *input,
    const StarsTechLevelCostOps *ops, void *context)
{
    StarsI16 i;
    StarsI16 stat;
    StarsI32 burden = 0;
    StarsU32 cost;
    StarsU32 quarter;
    if (!input || !input->colonist_counts || !input->level_costs ||
            input->level < 0 || (StarsU16)input->level >= input->level_count)
        return 0;
    for (i = 0; i < 6; ++i)
        burden += (StarsI32)((const signed char *)input->colonist_counts)[i];
    cost = (StarsU32)(burden * 10L) + input->level_costs[input->level];
    stat = ops && ops->get_race_stat ?
        ops->get_race_stat(context, input->player, (StarsI16)(input->tech + 8)) : 1;
    if (stat != 1) {
        if (stat < 1) {
            quarter = cost >> 2;
            cost += cost - quarter;
        } else {
            cost >>= 1;
        }
    }
    if ((input->game_flags & 2U) != 0U) cost <<= 1;
    return (StarsI32)cost;
}

typedef struct StarsCreateScope {
    jmp_buf recovery;
    StarsU8 *data;
    StarsU16 environment;
    const StarsStreamApi *outer;
    void *context;
    StarsStreamApi api;
} StarsCreateScope;

static StarsI16 create_open(void *opaque, const StarsU8 *name, StarsU16 mode, StarsU16 *error)
{
    StarsCreateScope *scope = (StarsCreateScope *)opaque;
    return scope->outer->open(scope->context, name, mode, error);
}
static StarsU16 create_write(void *opaque, StarsI16 handle, const StarsU8 *input, StarsU16 count)
{
    StarsCreateScope *scope = (StarsCreateScope *)opaque;
    return scope->outer->write(scope->context, handle, input, count);
}
static StarsU32 create_ticks(void *opaque)
{
    StarsCreateScope *scope = (StarsCreateScope *)opaque;
    return scope->outer->ticks(scope->context);
}
static void create_file_error(void *opaque, StarsI16 id)
{
    StarsCreateScope *scope = (StarsCreateScope *)opaque;
    scope->outer->file_error(scope->context, id);
}
static void create_write_error(void *opaque)
{
    StarsCreateScope *scope = (StarsCreateScope *)opaque;
    scope->outer->write_error(scope->context);
}
static void create_abort(void *opaque, StarsU16 environment, StarsI16 value)
{
    StarsCreateScope *scope = (StarsCreateScope *)opaque;
    if (environment == scope->environment) longjmp(scope->recovery, value);
    scope->outer->abort(scope->context, environment, value);
}
static void create_transfer(void *opaque, const StarsU8 *input, StarsU16 count)
{
    StarsCreateScope *scope = (StarsCreateScope *)opaque;
    stars_stream_write(scope->data, input, count, &scope->api, scope);
}

/* NE 15:8E16-8EA4. Retains an open handle after a header write failure. */
StarsI16 stars_create_file(StarsU8 *data, StarsU16 type, StarsI16 player,
    StarsU16 forced_name, StarsU16 environment, const StarsStreamApi *api, void *context)
{
    StarsCreateScope scope;
    StarsU16 saved;
    StarsI16 signed_type;
    if (!forced_name) {
        stars_set_file_name(data, type, player);
        forced_name = 0x57a4U;
    }
    saved = (StarsU16)(data[0x6eU] | ((StarsU16)data[0x6fU] << 8));
    scope.data = data; scope.environment = environment;
    scope.outer = api; scope.context = context;
    memset(&scope.api, 0, sizeof(scope.api));
    scope.api.open = create_open; scope.api.write = create_write;
    scope.api.ticks = create_ticks; scope.api.file_error = create_file_error;
    scope.api.write_error = create_write_error; scope.api.abort = create_abort;
    data[0x6eU] = (StarsU8)environment; data[0x6fU] = (StarsU8)(environment >> 8);
    if (setjmp(scope.recovery)) {
        data[0x6eU] = (StarsU8)saved; data[0x6fU] = (StarsU8)(saved >> 8);
        return 0;
    }
    stars_stream_open(data, forced_name, 0x1012, &scope.api, &scope);
    signed_type = type < 32768U ? (StarsI16)type : (StarsI16)((StarsI32)type - 65536L);
    stars_write_bof(data, player, signed_type, 0, create_ticks, create_transfer, &scope);
    data[0x6eU] = (StarsU8)saved; data[0x6fU] = (StarsU8)(saved >> 8);
    return 1;
}
