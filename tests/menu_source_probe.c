#include <stdio.h>
#include <string.h>
#include "core.h"

static unsigned deletes;
static unsigned inserts;
static unsigned enables;
static unsigned checks;
static unsigned draws;
static unsigned menu_calls;
static unsigned submenu_calls;
static StarsU16 last_enable_id;
static StarsU16 last_enable_flags;
static StarsU16 last_check_id;
static StarsU16 last_check_flags;
static char inserted[32];

static StarsU16 get_menu(void *context, StarsU16 frame)
{ (void)context; ++menu_calls; return (StarsU16)(frame + 10U); }
static StarsU16 get_sub_menu(void *context, StarsU16 menu, StarsU16 position)
{ (void)context; (void)position; ++submenu_calls; return (StarsU16)(menu + 1U); }
static void delete_menu(void *context, StarsU16 menu, StarsU16 id,
    StarsU16 flags)
{ (void)context; (void)menu; (void)id; (void)flags; ++deletes; }
static void insert_menu(void *context, StarsU16 menu, StarsU16 position,
    StarsU16 flags, StarsU16 id, const StarsU8 *text)
{
    (void)context; (void)menu; (void)position; (void)flags; (void)id;
    ++inserts;
    if (inserts == 1U) strcpy(inserted, (const char *)text);
}
static void enable_menu(void *context, StarsU16 menu, StarsU16 id,
    StarsU16 flags)
{ (void)context; (void)menu; ++enables; last_enable_id = id;
    last_enable_flags = flags; }
static void check_menu(void *context, StarsU16 menu, StarsU16 id,
    StarsU16 flags)
{ (void)context; (void)menu; ++checks; last_check_id = id;
    last_check_flags = flags; }
static StarsI16 menu_count(void *context, StarsU16 menu)
{ (void)context; (void)menu; return 3; }
static void draw_menu(void *context, StarsU16 frame)
{ (void)context; (void)frame; ++draws; }
static StarsInitializeMenuOps ops = { get_menu, get_sub_menu, delete_menu,
    insert_menu, enable_menu, check_menu, menu_count, draw_menu };

static void setup(StarsInitializeMenuState *state)
{
    memset(state, 0, sizeof(*state));
    state->frame = 100U;
}
static int empty_case(void)
{
    StarsInitializeMenuState state;
    setup(&state);
    deletes = inserts = enables = checks = draws = 0U;
    stars_initialize_menu(&state, &ops, 0);
    return state.menu == 110U && deletes == 9U && inserts == 0U &&
        enables == 6U && checks == 2U && draws == 1U &&
        last_enable_id == 1U && last_enable_flags == 0x403U ? 0 : 1;
}
static int active_case(void)
{
    StarsInitializeMenuState state;
    setup(&state);
    state.base = (const StarsU8 *)"game.s1";
    state.scanner = 9U; state.scan_flags = 0x2000U;
    state.color_flags = 0x8000U; state.scan_zoom = 2U;
    state.window_layout = 1U; state.salt_low = 1U;
    state.mru[0] = (const StarsU8 *)"Alpha";
    state.mru[1] = (const StarsU8 *)"Beta";
    deletes = inserts = enables = checks = draws = 0U;
    strcpy(inserted, "");
    stars_initialize_menu(&state, &ops, 0);
    return deletes == 9U && inserts == 2U && strcmp(inserted, "&1 Alpha") == 0 &&
        enables == 9U && checks == 4U && draws == 1U &&
        last_check_id == 1U && last_check_flags == 0x408U ? 0 : 1;
}
int main(void)
{
    int failures = empty_case() + active_case();
    printf("MENU_CHECKS=2 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures;
}
