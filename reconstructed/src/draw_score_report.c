#include "core.h"
#include <string.h>

static StarsI16 score_extent(const StarsDrawScoreReportInput *input,
    StarsU16 dc, const StarsU8 *text, StarsU16 length)
{
    if (!input->text_extent || !text)
        return 0;
    return input->text_extent(input->context, dc, text, length);
}

static StarsI16 score_string(const StarsDrawScoreReportInput *input,
    StarsI16 id, StarsU8 *work, StarsU16 capacity)
{
    StarsI16 length;

    work[0] = 0;
    if (!input->get_string)
        return 0;
    length = input->get_string(input->context, id, work, capacity);
    if (length < 0)
        length = 0;
    if ((StarsU16)length >= capacity)
        length = (StarsI16)(capacity - 1U);
    work[length] = 0;
    return length;
}

static StarsI16 score_dead(const StarsDrawScorePlayerState *player)
{
    return (StarsI16)(((player->detection_flags & 0x0100U) != 0U) &&
        player->dead_flags != 0U);
}

static StarsI32 score_value(const StarsDrawScorePlayerState *player,
    StarsI16 row)
{
    StarsU16 raw;

    if ((player->score_flags & 0x0020U) == 0U)
        return -1;
    switch (row) {
    case 0: return player->planet_count;
    case 1: return player->starbase_count;
    case 2:
    case 3:
    case 4:
        raw = player->ship_counts[row - 2];
        return (StarsI32)(((StarsU32)(raw & 0x1fffU)) <<
            ((raw >> 13) * 2));
    case 5: return player->tech_levels;
    case 6: return player->resources;
    case 7: return player->score;
    default: return player->overlap_value;
    }
}

static void score_color(const StarsDrawScoreReportInput *input, StarsU16 dc,
    StarsU32 color)
{
    if (input->set_text_color)
        input->set_text_color(input->context, dc, color);
}

/* NE 22:1E0C-2494. Draw the score report's player headings, separators, and
 * nine ranked value rows. Score records are typed host input; USER/GDI and
 * the original string/formatting helpers are explicit callbacks. */
void stars_draw_score_report(StarsU16 hdc,
    const StarsDrawScoreReportInput *input)
{
    StarsU8 work[128];
    StarsU8 digit[2];
    StarsI16 digit_width;
    StarsI16 row;
    StarsI16 pass;
    StarsI16 player_index;
    StarsI16 length;
    StarsI16 name_width;
    StarsI16 x;
    StarsI16 y;
    StarsI32 max_value;
    StarsI32 value;
    StarsI16 text_id;
    const StarsU8 *name;
    const StarsDrawScorePlayerState *player;

    if (!input)
        return;
    y = 0x58;
    if (input->set_background_mode)
        input->set_background_mode(input->context, hdc, 1);
    if (input->select_object)
        input->select_object(input->context, hdc, input->font_arial8_small);
    digit[0] = (StarsU8)'8';
    digit[1] = 0;
    digit_width = score_extent(input, hdc, digit, 1);
    if (input->select_object)
        input->select_object(input->context, hdc, input->font_arial8_large);
    x = (StarsI16)((digit_width * 5 - (input->line_height * 3) / 2) / 2 +
        input->score_x);
    for (player_index = 0; player_index < input->player_count; ++player_index) {
        name = input->player_name ? input->player_name(input->context,
            player_index) : 0;
        name_width = name ? score_extent(input, hdc, name,
            (StarsU16)strlen((const char *)name)) : 0;
        player = input->players ? &input->players[player_index] : 0;
        if (!player || !score_dead(player)) {
            if (!player || (player->score_flags & 0x4000U) == 0U)
                score_color(input, hdc, 0);
            else
                score_color(input, hdc, 0x00ff0000UL);
        } else {
            score_color(input, hdc, 0x007f7f7fUL);
        }
        if (input->text_out && name)
            input->text_out(input->context, hdc,
                (StarsI16)(x - name_width + (input->line_height * 3) / 2),
                (StarsI16)((y - name_width) - input->line_height / 2 - 4),
                name, (StarsU16)strlen((const char *)name));
        x = (StarsI16)(x + digit_width * 5);
    }
    if (input->select_object)
        input->select_object(input->context, hdc, input->font_arial8_small);
    if (input->select_object)
        input->select_object(input->context, hdc, input->brush_button_shadow);
    text_id = 0x1b3;
    x = (StarsI16)(input->score_x + 4);
    for (player_index = 0; player_index <= input->player_count;
            ++player_index) {
        if (input->pat_blt)
            input->pat_blt(input->context, hdc, x,
                (StarsI16)(y - input->line_height / 4), 1,
                (StarsI16)((input->line_height * 0x1b) / 2 + 1),
                0xf00021UL);
        x = (StarsI16)(x + digit_width * 5);
    }
    score_color(input, hdc, 0);
    for (row = 0; row < 9; ++row) {
        if (input->pat_blt)
            input->pat_blt(input->context, hdc,
                (StarsI16)(input->score_x + 4),
                (StarsI16)(y - input->line_height / 4),
                (StarsI16)(input->player_count * digit_width * 5), 1,
                0xf00021UL);
        length = score_string(input, text_id, work,
            (StarsU16)sizeof(work));
        if (input->right_text)
            input->right_text(input->context, hdc, input->score_x, y, work,
                (StarsU16)length, 0);
        x = (StarsI16)(digit_width * 5 + input->score_x + 2);
        max_value = 0;
        for (pass = 0; pass < 2; ++pass) {
            for (player_index = 0; player_index < input->player_count;
                    ++player_index) {
                player = input->players ? &input->players[player_index] : 0;
                value = player ? score_value(player, row) : -1;
                if (pass == 0) {
                    if (max_value < value)
                        max_value = value;
                } else if (value >= 0 && player && !score_dead(player)) {
                    if (row == 8)
                        score_color(input, hdc,
                            value == 1 ? 0x00ff0000UL : 0);
                    else
                        score_color(input, hdc,
                            (StarsI32)max_value == value ?
                            0x00ff0000UL : 0);
                    if (input->format_long)
                        length = input->format_long(input->context, value,
                            work, (StarsU16)sizeof(work));
                    else {
                        work[0] = 0;
                        length = 0;
                    }
                    if (input->right_text)
                        input->right_text(input->context,
                            hdc, x, y, work, (StarsU16)length, 0);
                }
                if (pass == 1)
                    x = (StarsI16)(x + digit_width * 5);
            }
            if (pass == 1)
                x = (StarsI16)(x + digit_width * 5);
        }
        y = (StarsI16)(y + (input->line_height * 3) / 2);
        text_id = (StarsI16)(text_id + 1);
    }
    if (input->pat_blt)
        input->pat_blt(input->context, hdc,
            (StarsI16)(input->score_x + 4),
            (StarsI16)(y - input->line_height / 4),
            (StarsI16)(input->player_count * digit_width * 5), 1,
            0xf00021UL);
}
