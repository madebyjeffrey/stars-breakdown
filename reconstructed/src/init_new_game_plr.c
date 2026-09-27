#include "core.h"

/* The values in vrgplrTypeNew are UI/AI race-template identifiers, not
 * arbitrary flags.  Keeping their names here makes the difficulty bands
 * readable while preserving the original byte values. */
#define AI_EASY_COLONIAL 0x0b
#define AI_EASY_SILENT 0x0f
#define AI_EASY_MILITARY 0x07
#define AI_EASY_RANDOM 0x1b
#define AI_NORMAL_COLONIAL 0x27
#define AI_NORMAL_SILENT 0x23
#define AI_NORMAL_MILITARY 0x2b
#define AI_NORMAL_TRADER 0x2f
#define AI_NORMAL_RESEARCHER 0x33
#define AI_NORMAL_RANDOM 0x9b
#define AI_HARD_COLONIAL 'S'
#define AI_HARD_SILENT 'G'
#define AI_HARD_MILITARY 'C'
#define AI_HARD_TRADER 'W'
#define AI_HARD_RESEARCHER '['
#define AI_HARD_RANDOM 0x9b
#define AI_EXPERT_COLONIAL 'c'
#define AI_EXPERT_TRADER 'w'
#define AI_EXPERT_SILENT 's'
#define AI_EXPERT_RANDOM '{'

static StarsI16 init_new_game_random(const StarsInitNewGamePlrInput *input,
    StarsI16 limit)
{
    if (input->random_value) return input->random_value(input->context, limit);
    return 0;
}

static void set_ai_type(StarsInitNewGamePlrInput *input, StarsI16 index,
    StarsU8 value)
{
    if (input->player_types && index >= 0)
        input->player_types[(StarsU16)index] = value;
}

static void assign_easy_types(StarsInitNewGamePlrInput *input,
    StarsI16 count)
{
    StarsI16 i;
    for (i = 1; i < count; ++i) {
        if (i < (count + 1) / 3 + 1)
            set_ai_type(input, i, AI_EASY_COLONIAL);
        else if (i < ((count + 1) * 2) / 3 + 1)
            set_ai_type(input, i, AI_EASY_SILENT);
        else if (i < ((count + 1) * 5) / 6 + 1)
            set_ai_type(input, i, AI_EASY_MILITARY);
        else
            set_ai_type(input, i, AI_EASY_RANDOM);
    }
}

static void assign_normal_types(StarsInitNewGamePlrInput *input,
    StarsI16 count, StarsU8 first, StarsU8 second, StarsU8 third,
    StarsU8 fourth, StarsU8 fifth, StarsU8 sixth)
{
    StarsI16 i;
    for (i = 1; i < count; ++i) {
        if (i < ((count + 5) * 2) / 7 + 1)
            set_ai_type(input, i, first);
        else if (i < ((count - 1) * 3 + 6) / 7 + 1)
            set_ai_type(input, i, second);
        else if (i < ((count - 1) * 4 + 6) / 7 + 1)
            set_ai_type(input, i, third);
        else if (i < ((count - 1) * 5 + 6) / 7 + 1)
            set_ai_type(input, i, fourth);
        else if (i < ((count - 1) * 6 + 6) / 7 + 1)
            set_ai_type(input, i, fifth);
        else
            set_ai_type(input, i, sixth);
    }
}

static void assign_expert_types(StarsInitNewGamePlrInput *input,
    StarsI16 count)
{
    StarsI16 i;
    for (i = 1; i < count; ++i) {
        if (i < (count + 1) / 3 + 1)
            set_ai_type(input, i, AI_EXPERT_COLONIAL);
        else if (i < ((count - 1) * 6 + 11) / 12 + 1)
            set_ai_type(input, i, AI_EXPERT_TRADER);
        else if (i < ((count - 1) * 5 + 5) / 6 + 1)
            set_ai_type(input, i, AI_EXPERT_SILENT);
        else
            set_ai_type(input, i, AI_EXPERT_RANDOM);
    }
}

static void shuffle_ai_types(StarsInitNewGamePlrInput *input,
    StarsI16 count)
{
    StarsI16 i;
    StarsI16 offset;
    StarsU8 saved;

    if (!input->player_types) return;
    for (i = 1; i < count - 1; ++i) {
        offset = init_new_game_random(input, count - i - 1);
        saved = input->player_types[(StarsU16)i];
        input->player_types[(StarsU16)i] =
            input->player_types[(StarsU16)(i + offset)];
        input->player_types[(StarsU16)(i + offset)] = saved;
    }
}

/* NE 16:6e44.  Selects the number of computer players from map size and AI
 * level, seeds the corresponding template bands, and shuffles computer
 * templates while leaving player zero untouched. */
void stars_init_new_game_plr(StarsInitNewGamePlrInput *input)
{
    StarsI16 count;
    StarsI16 random_value;

    if (!input || !input->player_count) return;
    if (input->step_max_so_far >= 2 || input->read_only != 0U)
        return;
    if (input->set_victory_value)
        (void)input->set_victory_value(input->context, 9,
            input->map_size << 1);
    if (input->step_max_so_far != 0) return;

    count = *input->player_count;
    switch (input->map_size) {
    case 0:
        random_value = input->ai_level == 3 ?
            init_new_game_random(input, 3) : 1;
        count = (input->ai_level == 3 && random_value == 0) ? 3 : 2;
        break;
    case 1:
        if (input->ai_level == 3 &&
                init_new_game_random(input, 4) == 0)
            count = 5;
        else if (input->ai_level < 2 ||
                init_new_game_random(input, 6 - input->ai_level) != 0)
            count = 3;
        else
            count = 4;
        break;
    case 2:
        if (input->ai_level == 3 &&
                init_new_game_random(input, 10) == 0)
            count = 9;
        else if (input->ai_level == 3 &&
                init_new_game_random(input, 10) == 0)
            count = 5;
        else if (input->ai_level < 2 ||
                init_new_game_random(input, 7 - input->ai_level) != 0)
            count = (input->ai_level < 2 ||
                init_new_game_random(input, 7 - input->ai_level) != 0) ? 7 : 6;
        else
            count = 8;
        break;
    case 3:
        if (input->ai_level == 3 &&
                init_new_game_random(input, 10) == 0)
            count = init_new_game_random(input, 2) + 14;
        else if (input->ai_level == 3 &&
                init_new_game_random(input, 10) == 0)
            count = 10 - init_new_game_random(input, 2);
        else if (input->ai_level < 2 ||
                init_new_game_random(input, 7 - input->ai_level) != 0)
            count = (input->ai_level < 2 ||
                init_new_game_random(input, 7 - input->ai_level) != 0) ? 12 : 11;
        else
            count = 13;
        break;
    case 4:
        if (input->ai_level == 3 &&
                init_new_game_random(input, 10) == 0)
            count = 13 - init_new_game_random(input, 3);
        else if (input->ai_level < 2 ||
                init_new_game_random(input, 9 - input->ai_level) != 0)
            count = (input->ai_level < 2 ||
                init_new_game_random(input, 7 - input->ai_level) != 0) ? 16 : 15;
        else
            count = 14;
        break;
    default:
        break;
    }
    *input->player_count = count;

    if (input->ai_level == 0)
        assign_easy_types(input, count);
    else if (input->ai_level == 1)
        assign_normal_types(input, count, AI_NORMAL_COLONIAL,
            AI_NORMAL_SILENT, AI_NORMAL_MILITARY, AI_NORMAL_TRADER,
            AI_NORMAL_RESEARCHER, AI_NORMAL_RANDOM);
    else if (input->ai_level == 2)
        assign_normal_types(input, count, AI_HARD_COLONIAL,
            AI_HARD_SILENT, AI_HARD_MILITARY, AI_HARD_TRADER,
            AI_HARD_RESEARCHER, AI_HARD_RANDOM);
    else if (input->ai_level == 3)
        assign_expert_types(input, count);
    shuffle_ai_types(input, count);
}
