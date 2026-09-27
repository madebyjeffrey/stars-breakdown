#include "core.h"

#define VCR_TOKEN_FOCUS 0x0008U
#define VCR_TARGET_CLASS_MASK 0x001fU
#define VCR_TARGET_CLASS_SHIFT 5
#define VCR_TARGET_CLASS_CLEAR 4U

static void vcr_reset(StarsSetVcrBoardInput *input)
{
    StarsU16 i;
    for (i = 0; i < input->token_count; ++i)
        input->tokens[i] = input->initial_tokens[i];
    input->current_step = -1;
    input->round = 0;
    input->focus_token = 0;
    input->focus_square = input->token_count ? input->tokens[0].battle_square : 0;
}

static void vcr_new_round(StarsSetVcrBoardInput *input, StarsI16 round)
{
    StarsU16 i;
    input->round = round;
    for (i = 0; i < input->token_count; ++i) {
        if (input->tokens[i].flags & VCR_TOKEN_FOCUS &&
                input->regenerate_shield)
            input->regenerate_shield(input->context, &input->tokens[i]);
        input->tokens[i].flags = (StarsU16)(input->tokens[i].flags &
            (StarsU16)~VCR_TOKEN_FOCUS);
    }
}

/* Genuine semantic reconstruction of SetVCRBoard, NE 30:08D8-0E90.
 * A backward seek (or the first seek) restores the initial token snapshot and
 * replays records from the beginning.  Forward seeks apply each recorded
 * casualty/shield/movement update, refresh shields on a new round, encode the
 * current initiative class, clear targets for class four, and expose the
 * focused token/square for the VCR controls. */
StarsI16 stars_set_vcr_board(StarsI16 requested_step,
    StarsSetVcrBoardInput *input)
{
    StarsI16 step;
    const StarsVcrStepRecord *record;
    StarsVcrTokenState *token;
    if (!input || !input->tokens || !input->initial_tokens) return requested_step;
    if (requested_step == input->current_step && input->current_step != -1)
        return requested_step;
    if (requested_step < input->current_step || input->current_step == -1)
        vcr_reset(input);
    while (input->current_step < requested_step &&
            (StarsU16)(input->current_step + 1) < input->step_count) {
        step = (StarsI16)(input->current_step + 1);
        record = &input->steps[step];
        if (record->token_index >= input->token_count) break;
        token = &input->tokens[record->token_index];
        token->ship_count = (StarsI16)(token->ship_count - record->ship_loss);
        token->shield = (StarsI16)(token->shield - record->shield_damage);
        token->battle_square = record->destination_square;
        token->flags = (StarsU16)((token->flags & 0xfc1fU) |
            ((record->packed_flags >> 4) & VCR_TARGET_CLASS_MASK) <<
            VCR_TARGET_CLASS_SHIFT);
        if (((token->flags >> VCR_TARGET_CLASS_SHIFT) &
                VCR_TARGET_CLASS_MASK) == VCR_TARGET_CLASS_CLEAR)
            token->target = 0;
        if ((StarsI16)record->round > input->round)
            vcr_new_round(input, (StarsI16)record->round);
        input->focus_token = (StarsI16)record->token_index;
        input->focus_square = token->battle_square;
        input->current_step = step;
    }
    if (input->enable_buttons)
        input->enable_buttons(input->context, input->current_step,
            (StarsI16)input->step_count);
    return input->current_step;
}
