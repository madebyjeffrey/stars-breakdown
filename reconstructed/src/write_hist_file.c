#include "core.h"

static void history_put_word(StarsU8 *data, StarsU16 value)
{
    data[0] = (StarsU8)value;
    data[1] = (StarsU8)(value >> 8);
}

/* NE 10:D29E-D6E0. Serialize the historical snapshot in the original
 * header, planet, message, player, design, score, AI-data, and terminator
 * order. */
StarsI16 stars_write_hist_file(const StarsWriteHistFileInput *input)
{
    StarsU8 header[4];
    StarsU16 i;
    StarsU16 j;
    StarsU16 baseline;
    StarsU16 chunk;
    StarsU32 cursor;
    const StarsHistoryPlayer *player;

    if (!input || !input->create_file || !input->write_record ||
            !input->close_stream)
        return 0;
    if (!input->create_file(input->context, input->player)) {
        if (input->alert) input->alert(input->context, 0x12U, 0x10);
        return 0;
    }
    history_put_word(header, input->planet_count);
    history_put_word(header + 2, input->players ?
        (StarsU16)(input->players[input->player].fleet_count & 0x0fffU) : 0U);
    input->write_record(input->context, 0x20U, 4U, header);
    for (i = 0; i < input->planet_count; ++i)
        if (input->write_planet && input->planets)
            input->write_planet(input->context,
                input->planets[i].owner_flags < 3U ? 0x0fU : 0x0eU,
                input->planets[i].record);
    input->write_record(input->context, 0x21U, input->message_bits_count,
        input->message_bits);
    for (i = 0; i < input->game_player_count; ++i) {
        player = &input->players[i];
        if (i != (StarsU16)input->player && player->detection_flags != 0U &&
                input->write_player)
            input->write_player(input->context, player->record);
    }
    for (i = 0; i < input->game_player_count; ++i) {
        player = &input->players[i];
        if (i != (StarsU16)input->player &&
                (player->detection_flags & 0x0100U) != 0U &&
                input->write_design) {
            for (j = 0; j < 16U; ++j)
                if ((player->designs[j].flags & 0x0200UL) == 0U)
                    input->write_design(input->context,
                        player->designs[j].record);
        }
    }
    for (i = 0; i < input->game_player_count; ++i) {
        player = &input->players[i];
        if (i != (StarsU16)input->player &&
                (player->detection_flags & 0x0100U) != 0U &&
                input->write_design) {
            for (j = 0; j < 10U; ++j)
                if ((player->enemy_designs[j].flags & 0x0200UL) == 0U)
                    input->write_design(input->context,
                        player->enemy_designs[j].record);
        }
    }
    baseline = input->game_turn < 0x65U ? 0U :
        (StarsU16)(input->game_turn - 100U);
    if (input->write_score) {
        for (i = 0; i < input->game_player_count; ++i) {
            player = &input->players[i];
            for (j = 0; j < player->score_count; ++j)
                if (player->scores[j].turn >= baseline)
                    input->write_score(input->context,
                        player->scores[j].record);
        }
    }
    cursor = 0;
    while (cursor < input->ai_size) {
        chunk = (StarsU16)((input->ai_size - cursor) > 0x03ffUL ?
            0x03ffU : input->ai_size - cursor);
        input->write_record(input->context, 0x29U, chunk,
            input->ai_data ? input->ai_data + cursor : 0);
        cursor += chunk;
    }
    input->write_record(input->context, 0U, 0U, 0);
    input->close_stream(input->context);
    return 1;
}
