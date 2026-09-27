#include "core.h"
#include <string.h>

#define BATTLE_PLANS_WM_ERASE_BACKGROUND 0x0014U
#define BATTLE_PLANS_WM_CONTROL_COLOR 0x0019U
#define BATTLE_PLANS_WM_INIT_DIALOG 0x0110U
#define BATTLE_PLANS_WM_COMMAND 0x0111U
#define BATTLE_PLANS_ID_OK 1U
#define BATTLE_PLANS_ID_CANCEL 2U
#define BATTLE_PLANS_ID_NEW 0x041cU
#define BATTLE_PLANS_ID_DELETE 0x0817U
#define BATTLE_PLANS_ID_PLAN_LIST 0x041eU
#define BATTLE_PLANS_ID_MANUAL 0x041dU
#define BATTLE_PLANS_ID_ATTACKER_TACTIC 0x041fU
#define BATTLE_PLANS_ID_FRIENDLY_TACTIC 0x0420U
#define BATTLE_PLANS_ID_TARGET_TACTIC 0x0421U
#define BATTLE_PLANS_ID_TARGET_PLAYER 0x0422U
#define BATTLE_PLANS_ID_HELP 0x0076U
#define BATTLE_PLANS_ID_FIRST_TACTIC 0x0190U
#define BATTLE_PLANS_ID_LAST_TACTIC 0x0197U
#define BATTLE_PLANS_ID_FIRST_TARGET 0x0078U
#define BATTLE_PLANS_ID_LAST_TARGET 0x007bU
#define BATTLE_PLANS_FLAG_NETWORK_GAME 0x0004U
#define BATTLE_PLANS_WORD_MANUAL 0x8000U
#define BATTLE_PLANS_WORD_VIEW_MASK 0x4000U
#define BATTLE_PLANS_WORD_INDEX_MASK 0x00f0U
#define BATTLE_PLANS_WORD_TARGET_MASK 0x1f00U
#define BATTLE_PLANS_WORD_ATTACKER_MASK 0x000fU
#define BATTLE_PLANS_WORD_FRIENDLY_MASK 0x00f0U
#define BATTLE_PLANS_WORD_TARGET_TACTIC_MASK 0x0f00U
#define BATTLE_PLANS_PLAN_BYTES 0x24U

static StarsU16 battle_plans_word0(const StarsBattlePlan *plan)
{
    return plan->words[0];
}

static StarsU16 battle_plans_word1(const StarsBattlePlan *plan)
{
    return plan->words[1];
}

static void battle_plans_load(StarsBattlePlansInput *input, StarsI16 index)
{
    if (!input->plans || index < 0 || index >= input->plan_count) return;
    input->edit_plan = input->plans[index];
    input->edit_plan_index = index;
    input->dirty = 0;
}

static void battle_plans_store(StarsBattlePlansInput *input)
{
    if (!input->plans || input->edit_plan_index < 0 ||
            input->edit_plan_index >= input->plan_count) return;
    input->plans[input->edit_plan_index] = input->edit_plan;
}

static void battle_plans_commit(StarsBattlePlansInput *input)
{
    if (!input->dirty) return;
    battle_plans_store(input);
    if (input->ops && input->ops->log_change)
        input->ops->log_change(input->context, &input->edit_plan);
    input->dirty = 0;
}

static void battle_plans_enable_edit_controls(StarsBattlePlansInput *input)
{
    StarsI16 enabled = (StarsI16)(input->edit_plan_index > 0);
    if (!input->ops || !input->ops->enable_control) return;
    input->ops->enable_control(input->context, input->delete_control, enabled);
    input->ops->enable_control(input->context, input->new_control, enabled);
}

static void battle_plans_refresh_controls(StarsBattlePlansInput *input)
{
    StarsU16 word0 = battle_plans_word0(&input->edit_plan);
    StarsU16 word1 = battle_plans_word1(&input->edit_plan);
    StarsI16 target;
    if (!input->ops) return;
    if (input->ops->set_choice) {
        input->ops->set_choice(input->context, input->list_attackers,
            (StarsI16)(word1 & 0x000fU));
        input->ops->set_choice(input->context, input->list_friendly_tactics,
            (StarsI16)((word1 >> 4) & 0x000fU));
        input->ops->set_choice(input->context, input->list_tactics,
            (StarsI16)((word0 >> 8) & 0x000fU));
    }
    if (input->ops->set_check)
        input->ops->set_check(input->context, input->check_manual,
            (StarsI16)((word0 & BATTLE_PLANS_WORD_MANUAL) != 0));
    target = (StarsI16)((word1 & BATTLE_PLANS_WORD_TARGET_MASK) >> 8);
    if (!(input->game_flags & BATTLE_PLANS_FLAG_NETWORK_GAME) &&
            target >= input->player + 4)
        --target;
    if (input->game_flags & BATTLE_PLANS_FLAG_NETWORK_GAME) target = 0;
    if (input->ops->set_choice)
        input->ops->set_choice(input->context, input->list_target_players,
            target);
    battle_plans_enable_edit_controls(input);
}

static void battle_plans_fill_lists(StarsBattlePlansInput *input)
{
    StarsI16 index;
    if (!input->ops) return;
    if (input->ops->reset_list)
        input->ops->reset_list(input->context, input->list_plans);
    if (input->ops->add_list_item)
        for (index = 0; index < input->plan_count; ++index)
            input->ops->add_list_item(input->context, input->list_plans,
                index, input->plans[index].name);
    if (input->ops->select_list_item)
        input->ops->select_list_item(input->context, input->list_plans,
            input->edit_plan_index);
}

/* Genuine semantic reconstruction of BattlePlansDlg.  A battle plan is a
 * fixed 36-byte record: the low control bits select tactics, the high bits
 * carry manual/view/target state, and the remaining words are the plan name
 * and saved battle-order data.  The dialog edits a working copy, commits it
 * only when leaving the current plan or dialog, and records one change-log
 * entry per committed edit. */
StarsI16 stars_battle_plans_dialog(StarsU16 hwnd, StarsU16 message,
    StarsU16 wparam, StarsI32 lparam, StarsBattlePlansInput *input)
{
    StarsI16 selection;
    StarsI16 target;
    StarsU16 word;
    (void)lparam;
    if (!input || !input->plans || !input->ops) return 0;
    input->hwnd = hwnd;
    if (message == BATTLE_PLANS_WM_ERASE_BACKGROUND) return 1;
    if (message == BATTLE_PLANS_WM_CONTROL_COLOR) {
        if ((StarsU16)lparam != input->list_plans &&
                input->ops->set_choice)
            input->ops->set_choice(input->context, input->list_plans,
                (StarsI16)input->button_face_brush);
        return (StarsI16)input->button_face_brush;
    }
    if (message == BATTLE_PLANS_WM_INIT_DIALOG) {
        if (input->ops->sticky_dialog)
            input->ops->sticky_dialog(input->context, hwnd);
        input->edit_plan_index = 0;
        if (input->selected_object_kind == 2)
            input->edit_plan_index = input->selected_plan;
        if (input->edit_plan_index < 0 ||
                input->edit_plan_index >= input->plan_count)
            input->edit_plan_index = 0;
        battle_plans_load(input, input->edit_plan_index);
        battle_plans_fill_lists(input);
        battle_plans_refresh_controls(input);
        if (input->game_flags & BATTLE_PLANS_FLAG_NETWORK_GAME) {
            if (input->ops->enable_control)
                input->ops->enable_control(input->context,
                    input->list_target_players, 0);
        }
        if (input->ops->advance_tutor)
            input->ops->advance_tutor(input->context, 0);
        return 1;
    }
    if (message != BATTLE_PLANS_WM_COMMAND) return 0;
    if (wparam == BATTLE_PLANS_ID_OK || wparam == BATTLE_PLANS_ID_CANCEL) {
        battle_plans_commit(input);
        if (input->ops->sticky_dialog)
            input->ops->sticky_dialog(input->context, hwnd);
        if (input->ops->refresh_battle_dropdown &&
                input->selected_object_kind == 2)
            input->ops->refresh_battle_dropdown(input->context,
                input->selected_plan + 1);
        input->edit_plan_index = -1;
        return 1;
    }
    if (wparam == BATTLE_PLANS_ID_MANUAL) {
        word = input->ops->get_check ?
            (StarsU16)input->ops->get_check(input->context,
                input->check_manual) : 0U;
        input->edit_plan.words[0] = (StarsU16)((word ?
            (input->edit_plan.words[0] | BATTLE_PLANS_WORD_MANUAL) :
            (input->edit_plan.words[0] & (StarsU16)~BATTLE_PLANS_WORD_MANUAL)));
        input->dirty = 1;
        return 0;
    }
    if (wparam == BATTLE_PLANS_ID_DELETE) {
        battle_plans_commit(input);
        if (input->ops->delete_plan && input->ops->delete_plan(input->context,
                input->edit_plan_index, 1)) {
            --input->plan_count;
            input->edit_plan_index = input->plan_count - 1;
            battle_plans_load(input, input->edit_plan_index);
            battle_plans_fill_lists(input);
            battle_plans_refresh_controls(input);
        }
        return 0;
    }
    if (wparam == BATTLE_PLANS_ID_NEW) {
        if (input->plan_count >= 15) return 0;
        battle_plans_commit(input);
        input->edit_plan_index = input->plan_count;
        memset(&input->edit_plan, 0, sizeof input->edit_plan);
        if (input->ops->new_plan_name &&
                input->ops->new_plan_name(input->context,
                    &input->edit_plan)) {
            ++input->plan_count;
            input->edit_plan.words[0] = (StarsU16)(
                (input->edit_plan.words[0] & (StarsU16)~BATTLE_PLANS_WORD_INDEX_MASK) |
                ((StarsU16)input->edit_plan_index << 4));
            battle_plans_store(input);
            battle_plans_fill_lists(input);
            battle_plans_refresh_controls(input);
            input->dirty = 1;
        }
        return 0;
    }
    if (wparam == BATTLE_PLANS_ID_PLAN_LIST) {
        selection = input->ops->get_list_selection ?
            input->ops->get_list_selection(input->context,
                input->list_plans) : input->edit_plan_index;
        if (selection != input->edit_plan_index && selection >= 0 &&
                selection < input->plan_count) {
            battle_plans_commit(input);
            battle_plans_load(input, selection);
            battle_plans_refresh_controls(input);
        }
        return 0;
    }
    if (wparam == BATTLE_PLANS_ID_ATTACKER_TACTIC ||
            wparam == BATTLE_PLANS_ID_FRIENDLY_TACTIC ||
            wparam == BATTLE_PLANS_ID_TARGET_TACTIC ||
            wparam == BATTLE_PLANS_ID_TARGET_PLAYER) {
        selection = input->ops->get_choice ?
            input->ops->get_choice(input->context, wparam) : 0;
        if (wparam == BATTLE_PLANS_ID_ATTACKER_TACTIC) {
            input->edit_plan.words[1] = (StarsU16)((input->edit_plan.words[1] &
                (StarsU16)~BATTLE_PLANS_WORD_ATTACKER_MASK) | (selection & 0xf));
        } else if (wparam == BATTLE_PLANS_ID_FRIENDLY_TACTIC) {
            input->edit_plan.words[1] = (StarsU16)((input->edit_plan.words[1] &
                (StarsU16)~BATTLE_PLANS_WORD_FRIENDLY_MASK) | ((selection & 0xf) << 4));
        } else if (wparam == BATTLE_PLANS_ID_TARGET_TACTIC) {
            input->edit_plan.words[0] = (StarsU16)((input->edit_plan.words[0] &
                (StarsU16)~BATTLE_PLANS_WORD_TARGET_TACTIC_MASK) | ((selection & 0xf) << 8));
        } else {
            target = selection;
            if (!(input->game_flags & BATTLE_PLANS_FLAG_NETWORK_GAME) &&
                    target > input->player + 3)
                ++target;
            input->edit_plan.words[1] = (StarsU16)((input->edit_plan.words[1] &
                (StarsU16)~BATTLE_PLANS_WORD_TARGET_MASK) | ((target & 0x1f) << 8));
        }
        input->dirty = 1;
        return 0;
    }
    if (wparam == BATTLE_PLANS_ID_HELP) {
        if (input->ops->win_help)
            input->ops->win_help(input->context, hwnd, input->help_file);
        return 1;
    }
    return 0;
}
