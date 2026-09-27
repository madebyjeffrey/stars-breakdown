#include "core.h"

#define VCR_VIEW_CLEAR 0x2000U
#define VCR_TUTOR_ENABLED 0x0800U
#define VCR_SMALL_BOARD 0x161
#define VCR_SMALL_SQUARE 0x20
#define VCR_LARGE_BOARD 0x2a1
#define VCR_LARGE_SQUARE 0x40
#define VCR_TOK_BYTES 0x1dU
#define VCR_DP_BYTES 4U
#define VCR_ALLOC_TAG 6U
#define VCR_BOARD_INITIAL_STEP 30000
#define VCR_MISSING_RECORD_ALERT 0x10

static const StarsBattleVcrRecord *vcr_find(
    const StarsBattleVcrInput *input)
{
    StarsU16 i;
    for (i = 0; i < input->record_count; ++i)
        if (input->records[i].battle_id == input->battle_id)
            return &input->records[i];
    return (const StarsBattleVcrRecord *)0;
}

static void vcr_release(StarsBattleVcrInput *input)
{
    if (input->token_workspace && input->free_memory)
        input->free_memory(input->context, input->token_workspace,
            VCR_ALLOC_TAG);
    if (input->damage_workspace && input->free_memory)
        input->free_memory(input->context, input->damage_workspace,
            VCR_ALLOC_TAG);
    input->token_workspace = (void *)0;
    input->damage_workspace = (void *)0;
}

/* Genuine semantic reconstruction of BattleVCR, NE 30:0000-028C.
 * The routine selects the compact or expanded battle board from the current
 * view mode, searches the linked battle-log records for the requested battle,
 * allocates the token and per-token damage workspaces, prepares the VCR board,
 * optionally advances the tutor, and opens the dialog.  Missing records and
 * allocation failures follow the original cleanup/error path; all successful
 * and failed paths release the temporary workspaces before returning. */
void stars_battle_vcr(StarsBattleVcrInput *input)
{
    const StarsBattleVcrRecord *record;
    StarsI16 step;

    if (!input) return;
    input->current_vcr_step = -1;
    if ((StarsU16)(input->view_flags >> 14) < 2U) {
        input->board_width = VCR_SMALL_BOARD;
        input->board_square = VCR_SMALL_SQUARE;
    } else {
        input->board_width = VCR_LARGE_BOARD;
        input->board_square = VCR_LARGE_SQUARE;
    }
    record = input->records ? vcr_find(input) :
        (const StarsBattleVcrRecord *)0;
    if (!record) {
        input->view_flags = (StarsU16)(input->view_flags & VCR_VIEW_CLEAR);
        if (input->alert) input->alert(input->context,
            VCR_MISSING_RECORD_ALERT);
        return;
    }
    input->current_step = (StarsI16)record->next_record;
    if (input->allocate) {
        input->token_workspace = input->allocate(input->context,
            (StarsU16)(record->token_count * VCR_TOK_BYTES), VCR_ALLOC_TAG);
        input->damage_workspace = input->allocate(input->context,
            (StarsU16)(record->token_count * VCR_DP_BYTES), VCR_ALLOC_TAG);
    }
    if (!input->token_workspace || !input->damage_workspace) {
        if (input->alert) input->alert(input->context,
            VCR_MISSING_RECORD_ALERT);
        vcr_release(input);
        return;
    }
    step = input->set_board ? input->set_board(input->context,
        VCR_BOARD_INITIAL_STEP) : 0;
    input->current_vcr_step = (StarsI16)(step - 1);
    input->current_vcr_round = input->round;
    if (input->set_board) (void)input->set_board(input->context, -1);
    if ((input->view_flags & VCR_TUTOR_ENABLED) && input->advance_tutor)
        input->advance_tutor(input->context);
    if (input->show_dialog) input->show_dialog(input->context);
    vcr_release(input);
}
