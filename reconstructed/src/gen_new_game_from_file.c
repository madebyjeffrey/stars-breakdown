#include "core.h"
#include <string.h>

#define GAME_FILE_OPTION_EXTRA_FUEL 0x0001U
#define GAME_FILE_OPTION_SLOW_TECH 0x0002U
#define GAME_FILE_OPTION_BBS_PLAY 0x0020U
#define GAME_FILE_OPTION_NO_RANDOM 0x0080U
#define GAME_FILE_OPTION_AI_BAND 0x0010U
#define GAME_FILE_OPTION_VISIBLE_SCORES 0x0040U
#define GAME_FILE_OPTION_CLUMPING 0x0100U
#define GAME_FILE_ERROR_NAME 5
#define GAME_FILE_ERROR_MAP 6
#define GAME_FILE_ERROR_OPTIONS 7
#define GAME_FILE_ERROR_PLAYERS 8
#define GAME_FILE_ERROR_RACE 9
#define GAME_FILE_ERROR_EOF 12

static void game_file_error(const StarsGameFileOps *ops, StarsI16 code)
{
    if (ops && ops->error) ops->error(ops->context, code);
}

static void game_file_clear(StarsGameFileState *state)
{
    StarsI16 i;
    StarsI16 j;
    memset(state, 0, sizeof(*state));
    for (i = 0; i < STARS_GAME_FILE_MAX_PLAYERS; ++i) {
        state->players[i].computer_race = -1;
        state->players[i].difficulty = -1;
        state->players[i].bitmap_id = -1;
        for (j = 0; j < STARS_GAME_FILE_PATH_SIZE; ++j)
            state->players[i].race_file[j] = 0;
    }
}

static StarsI16 game_file_line(const StarsU8 *data, StarsU16 length,
    StarsU16 *offset, StarsU8 *line, StarsU16 capacity)
{
    StarsU16 start;
    StarsU16 count;
    StarsU16 copy_count;

    if (*offset >= length) return 0;
    start = *offset;
    while (*offset < length && data[*offset] != '\r' &&
            data[*offset] != '\n') ++*offset;
    count = (StarsU16)(*offset - start);
    copy_count = count < (StarsU16)(capacity - 1U) ? count :
        (StarsU16)(capacity - 1U);
    if (capacity != 0U) {
        memcpy(line, data + start, copy_count);
        line[copy_count] = 0;
    }
    if (*offset < length && data[*offset] == '\r') ++*offset;
    if (*offset < length && data[*offset] == '\n') ++*offset;
    return count <= (StarsU16)(capacity - 1U) ? 1 : -1;
}

static const StarsU8 *game_file_skip_spaces(const StarsU8 *text)
{
    while (*text == ' ') ++text;
    return text;
}

static StarsI16 game_file_parse_numbers(const StarsU8 *text,
    StarsU32 values[4], StarsI16 maximum)
{
    StarsI16 count;
    StarsU32 value;
    StarsI16 have_digit;

    count = 0;
    while (*text != 0) {
        while (*text == ' ') ++text;
        if (*text == 0) break;
        if (*text < '0' || *text > '9') return -1;
        value = 0UL;
        have_digit = 0;
        while (*text >= '0' && *text <= '9') {
            value = value * 10UL + (StarsU32)(*text - '0');
            have_digit = 1;
            ++text;
        }
        if (!have_digit) return -1;
        if (count < maximum) values[count] = value;
        ++count;
        while (*text == ' ') ++text;
        if (*text != 0 && (*text < '0' || *text > '9')) return -1;
    }
    return count > maximum ? maximum : count;
}

static StarsI16 game_file_valid_range(StarsU32 value, StarsU32 low,
    StarsU32 high)
{
    return value >= low && value <= high;
}

static void game_file_copy(StarsU8 *destination, StarsU16 capacity,
    const StarsU8 *source)
{
    StarsU16 length;
    if (capacity == 0U) return;
    length = (StarsU16)strlen((const char *)source);
    if (length >= capacity) length = (StarsU16)(capacity - 1U);
    memcpy(destination, source, length);
    destination[length] = 0;
}

static StarsI16 game_file_same_name(const StarsGameFilePlayer *players,
    StarsI16 first, StarsI16 second)
{
    return strcmp((const char *)players[first].name,
        (const char *)players[second].name) == 0;
}

static void game_file_make_names(StarsGameFileState *state,
    const StarsGameFileOps *ops)
{
    StarsI16 i;
    StarsI16 j;
    StarsI16 candidate;
    StarsU8 name[STARS_GAME_FILE_NAME_SIZE];

    for (i = 0; i < state->player_count; ++i) {
        if (state->players[i].name[0] == 0) {
            if (ops && ops->get_name)
                ops->get_name(ops->context, i, state->players[i].name,
                    STARS_GAME_FILE_NAME_SIZE);
            if (state->players[i].name[0] == 0) {
                name[0] = 'P'; name[1] = 'l'; name[2] = 'a'; name[3] = 'y';
                name[4] = 'e'; name[5] = 'r';
                name[6] = (StarsU8)('1' + (i / 10));
                name[7] = (StarsU8)('0' + (i % 10)); name[8] = 0;
                game_file_copy(state->players[i].name,
                    STARS_GAME_FILE_NAME_SIZE, name);
            }
        }
    }
    for (i = 1; i < state->player_count; ++i) {
        for (j = 0; j < i; ++j) {
            if (!game_file_same_name(state->players, i, j)) continue;
            candidate = ops && ops->random_value ?
                ops->random_value(ops->context, 24) : 0;
            if (ops && ops->get_name) {
                ops->get_name(ops->context, (StarsI16)(candidate + 24),
                    name, STARS_GAME_FILE_NAME_SIZE);
                if (name[0] != 0)
                    game_file_copy(state->players[i].name,
                        STARS_GAME_FILE_NAME_SIZE, name);
            }
            if (game_file_same_name(state->players, i, j)) {
                state->players[i].name[0] = 'P';
                state->players[i].name[1] = (StarsU8)('1' + i);
                state->players[i].name[2] = 0;
            }
            if (ops && ops->set_name)
                ops->set_name(ops->context, i, state->players[i].name,
                    STARS_GAME_FILE_NAME_SIZE);
        }
    }
}

static void game_file_make_bitmaps(StarsGameFileState *state,
    const StarsGameFileOps *ops)
{
    StarsI16 i;
    StarsI16 j;
    StarsI16 value;
    StarsI16 used;

    for (i = 0; i < state->player_count; ++i) {
        if (state->players[i].bitmap_id < 0) continue;
        for (j = 0; j < i; ++j) {
            if (state->players[j].bitmap_id == state->players[i].bitmap_id) {
                state->players[i].bitmap_id = -1;
                break;
            }
        }
    }
    for (i = 0; i < state->player_count; ++i) {
        if (state->players[i].bitmap_id >= 0) continue;
        value = ops && ops->random_value ?
            ops->random_value(ops->context, 32) : i;
        for (used = 0; used < 32; ++used) {
            for (j = 0; j < state->player_count; ++j)
                if (state->players[j].bitmap_id == value) break;
            if (j == state->player_count) break;
            value = (StarsI16)((value + 1) & 31);
        }
        state->players[i].bitmap_id = value;
    }
}

static StarsI16 game_file_read_victory(const StarsU8 *line,
    StarsI16 index, StarsGameFileState *state)
{
    StarsU32 values[7];
    StarsI16 count;
    StarsU32 enabled;
    StarsU32 target;

    count = game_file_parse_numbers(line, values, 4);
    if (count < 1) return 0;
    enabled = values[0];
    if (index == 7) {
        if (!game_file_valid_range(enabled, 0, 7)) return 0;
        if (count < 2) return 0;
        target = values[1];
        if (enabled != 0U && !game_file_valid_range(target, 30, 500))
            return 0;
        state->victory_value[8] = (StarsI16)enabled;
        state->victory_value[9] = enabled ?
            (StarsI16)((target - 30U) / 10U) : 0;
        state->victory_enabled[8] = enabled ? 1 : 0;
        state->victory_enabled[9] = enabled ? 1 : 0;
        return 1;
    }
    if (!game_file_valid_range(enabled, 0, 1)) return 0;
    if (enabled == 0U) {
        state->victory_enabled[index == 1 ? 1 : (StarsI16)(index + 0)] = 0;
        if (index == 1) state->victory_enabled[2] = 0;
        return 1;
    }
    if (count < 2) return 0;
    target = values[1];
    if (index == 0 && !game_file_valid_range(target, 20, 100)) return 0;
    if (index == 1 && !game_file_valid_range(target, 8, 26)) return 0;
    if (index == 2 && !game_file_valid_range(target, 1000, 20000)) return 0;
    if (index == 3 && !game_file_valid_range(target, 20, 300)) return 0;
    if (index == 4 && !game_file_valid_range(target, 10, 500)) return 0;
    if (index == 5 && !game_file_valid_range(target, 10, 300)) return 0;
    if (index == 6 && !game_file_valid_range(target, 30, 900)) return 0;
    if (index == 1) {
        state->victory_enabled[1] = 1;
        state->victory_enabled[2] = 1;
        state->victory_value[1] = (StarsI16)(target - 8U);
        state->victory_value[2] = (StarsI16)(target - 2U);
    } else {
        state->victory_enabled[index] = 1;
        state->victory_value[index] = index == 0 ?
            (StarsI16)((target - 20U) / 5U) : index == 2 ?
            (StarsI16)((target - 1000U) / 1000U) : index == 3 ?
            (StarsI16)((target - 20U) / 10U) : index == 4 ?
            (StarsI16)((target - 10U) / 10U) : index == 5 ?
            (StarsI16)((target - 10U) / 10U) :
            (StarsI16)((target - 30U) / 10U);
    }
    return 1;
}

StarsI16 stars_gen_new_game_from_file(const StarsU8 *data, StarsU16 length,
    StarsGameFileState *state, const StarsGameFileOps *ops)
{
    StarsU8 line[256];
    StarsU32 values[7];
    StarsU16 offset;
    StarsI16 count;
    StarsI16 i;
    StarsI16 raw_race;
    StarsI16 raw_difficulty;
    const StarsU8 *name;

    if (!data || !state || length > 15999U) {
        game_file_error(ops, GAME_FILE_ERROR_NAME);
        return 0;
    }
    game_file_clear(state);
    offset = 0;
    count = game_file_line(data, length, &offset, line, sizeof line);
    name = game_file_skip_spaces(line);
    if (count <= 0 || strlen((const char *)name) > 31U) {
        game_file_error(ops, GAME_FILE_ERROR_NAME);
        return 0;
    }
    game_file_copy(state->game_name, STARS_GAME_FILE_NAME_SIZE, name);

    count = game_file_line(data, length, &offset, line, sizeof line);
    if (count < 0) { game_file_error(ops, GAME_FILE_ERROR_MAP); return 0; }
    if (count > 0) {
        count = game_file_parse_numbers(line, values, 4);
        if (count < 0 || (count > 0 && !game_file_valid_range(values[0], 0, 4)) ||
            (count > 1 && !game_file_valid_range(values[1], 0, 3)) ||
            (count > 2 && !game_file_valid_range(values[2], 0, 3))) {
            game_file_error(ops, GAME_FILE_ERROR_MAP); return 0;
        }
        if (count > 0) state->map_size = (StarsI16)values[0];
        if (count > 1) state->density = (StarsI16)values[1];
        if (count > 2) state->start_distance = (StarsI16)values[2];
        if (count > 3) state->random_seed = values[3];
    }
    count = game_file_line(data, length, &offset, line, sizeof line);
    if (count < 0 || (count > 0 && (count = game_file_parse_numbers(line,
        values, 7)) < 0)) { game_file_error(ops, GAME_FILE_ERROR_OPTIONS); return 0; }
    if (count > 0) {
        if (count > 0 && values[0] > 1U) goto bad_options;
        if (count > 1 && values[1] > 1U) goto bad_options;
        if (count > 2 && values[2] > 1U) goto bad_options;
        if (count > 3 && values[3] > 1U) goto bad_options;
        if (count > 4 && values[4] > 1U) goto bad_options;
        if (count > 5 && values[5] > 1U) goto bad_options;
        if (count > 6 && values[6] > 1U) goto bad_options;
        if (count > 0 && values[0]) state->options |= GAME_FILE_OPTION_EXTRA_FUEL;
        if (count > 1 && values[1]) state->options |= GAME_FILE_OPTION_SLOW_TECH;
        if (count > 2 && values[2]) state->options |= GAME_FILE_OPTION_BBS_PLAY;
        if (count > 3 && values[3]) state->options |= GAME_FILE_OPTION_NO_RANDOM;
        if (count > 4 && values[4]) state->options |= GAME_FILE_OPTION_AI_BAND;
        if (count > 5 && values[5]) state->options |= GAME_FILE_OPTION_VISIBLE_SCORES;
        if (count > 6 && values[6]) state->options |= GAME_FILE_OPTION_CLUMPING;
    }
    count = game_file_line(data, length, &offset, line, sizeof line);
    if (count <= 0 || game_file_parse_numbers(line, values, 1) != 1 ||
            values[0] < 1U || values[0] > STARS_GAME_FILE_MAX_PLAYERS) {
        game_file_error(ops, GAME_FILE_ERROR_PLAYERS); return 0;
    }
    state->player_count = (StarsI16)values[0];
    for (i = 0; i < state->player_count; ++i) {
        count = game_file_line(data, length, &offset, line, sizeof line);
        if (count <= 0) { game_file_error(ops, GAME_FILE_ERROR_RACE); return 0; }
        if (line[0] == '#') {
            count = game_file_parse_numbers(line + 1, values, 2);
            if (count < 2 || values[0] > 6U || values[1] > 4U) {
                game_file_error(ops, GAME_FILE_ERROR_RACE); return 0;
            }
            raw_race = (StarsI16)values[0];
            raw_difficulty = (StarsI16)values[1];
            state->players[i].computer = 1;
            state->players[i].computer_race = raw_race ?
                (StarsI16)(raw_race - 1) : -1;
            state->players[i].difficulty = raw_difficulty ?
                (StarsI16)(raw_difficulty - 1) : -1;
            if (ops && ops->choose_computer_race)
                state->players[i].bitmap_id = ops->choose_computer_race(
                    ops->context, i, state->players[i].computer_race,
                    state->players[i].difficulty);
        } else {
            game_file_copy(state->players[i].race_file,
                STARS_GAME_FILE_PATH_SIZE, game_file_skip_spaces(line));
            if (ops && ops->load_race && !ops->load_race(ops->context, i,
                    state->players[i].race_file)) {
                game_file_error(ops, GAME_FILE_ERROR_RACE); return 0;
            }
        }
    }
    for (i = 0; i < 8; ++i) {
        count = game_file_line(data, length, &offset, line, sizeof line);
        if (count <= 0 || !game_file_read_victory(line, i, state)) {
            game_file_error(ops, (StarsI16)(GAME_FILE_ERROR_RACE +
                state->player_count + i)); return 0;
        }
    }
    count = game_file_line(data, length, &offset, line, sizeof line);
    if (count <= 0) { game_file_error(ops, GAME_FILE_ERROR_EOF); return 0; }
    name = game_file_skip_spaces(line);
    game_file_copy(state->base_name, STARS_GAME_FILE_BASE_SIZE, name);
    i = (StarsI16)strlen((const char *)state->base_name);
    if (i > 3 && state->base_name[i - 3] == '.' &&
            state->base_name[i - 2] == 'x' && state->base_name[i - 1] == 'y')
        state->base_name[i - 3] = 0;
    for (i = 0; i < state->player_count; ++i)
        if (ops && ops->advantage_points && !state->players[i].computer &&
                ops->advantage_points(ops->context, i) < 0 &&
                ops->reset_player)
            ops->reset_player(ops->context, i);
    game_file_make_names(state, ops);
    game_file_make_bitmaps(state, ops);
    if (ops && ops->generate_world) ops->generate_world(ops->context, state);
    return 1;

bad_options:
    game_file_error(ops, GAME_FILE_ERROR_OPTIONS);
    return 0;
}
