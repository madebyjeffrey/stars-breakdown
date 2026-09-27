#include "core.h"
#include <stdio.h>

typedef struct BattleDropdownState {
    int cleared;
    int texts;
    int title;
    StarsI16 player;
    StarsI16 last_battle;
    StarsI16 selected;
} BattleDropdownState;

static void clear_list(void *context, StarsU16 dropdown)
{ BattleDropdownState *s = (BattleDropdownState *)context; s->cleared = dropdown; }

static const StarsU8 *get_text(void *context, StarsI16 player, StarsI16 battle)
{
    BattleDropdownState *s = (BattleDropdownState *)context;
    s->player = player;
    if (battle < 0) { ++s->title; return (const StarsU8 *)"Battles"; }
    s->last_battle = battle; return (const StarsU8 *)"Battle";
}

static void add_text(void *context, StarsU16 dropdown, const StarsU8 *text)
{
    BattleDropdownState *s = (BattleDropdownState *)context;
    (void)dropdown; (void)text; ++s->texts;
}

static void select_row(void *context, StarsI16 selection)
{ BattleDropdownState *s = (BattleDropdownState *)context; s->selected = selection; }

int main(void)
{
    StarsFillBattleDropdownOps ops;
    BattleDropdownState state;
    int failures = 0;

    ops.clear_list = clear_list; ops.get_text = get_text;
    ops.add_text = add_text; ops.select_row = select_row;
    state.cleared = 0; state.texts = 0; state.title = 0;
    state.player = 0; state.last_battle = -1; state.selected = -1;
    stars_fill_battle_dropdown(6U, 4, 3U, 2, &ops, &state);
    if (state.cleared != 6 || state.texts != 4 || state.title != 1 ||
        state.player != 4 || state.last_battle != 2 || state.selected != 2)
        ++failures;
    printf("FILL_BATTLE_DROPDOWN_CHECKS=4 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures != 0;
}
