#include "core.h"
#include <string.h>

/* NE 10:5E5E-6022. Initialize the fixed tutorial game record, copy the
 * default and competitor player records, remove stale tutorial files, and
 * hand the completed state to world generation. */
void stars_create_tutor_world(const StarsCreateTutorWorldInput *input)
{
    StarsU16 flags;
    StarsI16 player;

    if (!input || !input->player_count || !input->density || !input->size ||
            !input->start_distance || !input->game_flags ||
            !input->game_lid || !input->victory_conditions ||
            !input->player0 || !input->player1 || !input->default_player ||
            !input->competitor_player)
        return;
    if (input->game_flags) *input->game_flags =
        (StarsU16)((*input->game_flags & 0xff17U) | 0x00e8U);
    *input->game_lid = 0x008cef49UL;
    input->victory_conditions[7] = 0x80U;
    input->victory_conditions[8] = 0x81U;
    if (input->get_string && input->game_name)
        input->get_string(input->context, 0x01e7U, input->game_name, 64U);
    memset(input->player0, 0, 192U);
    memcpy(input->player0, input->default_player, 192U);
    if (input->get_string && input->player0_name)
        input->get_string(input->context, 0x0567U, input->player0_name, 64U);
    if (input->player0_names && input->format_names)
        input->format_names(input->context, input->player0_names, 0x5a22U);
    memset(input->player1, 0, 192U);
    memcpy(input->player1, input->competitor_player, 192U);
    flags = (StarsU16)(input->player1[6] |
        ((StarsU16)input->player1[7] << 8));
    flags = (StarsU16)((flags & 0x01ffU) | 0x2200U);
    input->player1[6] = (StarsU8)flags;
    input->player1[7] = (StarsU8)(flags >> 8);
    if (input->get_string && input->player1_name)
        input->get_string(input->context, 0x056eU, input->player1_name, 64U);
    if (input->player_count) *input->player_count = 2U;
    if (input->density) *input->density = 0U;
    if (input->size) *input->size = 0U;
    if (input->start_distance) *input->start_distance = 1U;
    if (input->randomize) input->randomize(input->context, 0x499602d2UL);
    if (input->unlink_file) {
        for (player = 1; player < 3; ++player) {
            input->unlink_file(input->context, 0x0314U, player);
            input->unlink_file(input->context, 0x0313U, player);
        }
    }
    if (input->generate_world) input->generate_world(input->context, 0);
}
