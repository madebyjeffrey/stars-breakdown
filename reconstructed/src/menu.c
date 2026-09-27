/* Semantic reconstruction of InitializeMenu. */
#include "core.h"

static void copy_mru(StarsU8 *out, const StarsU8 *text)
{
    StarsU16 i;
    for (i = 0U; i < 255U && text[i]; ++i) out[i] = text[i];
    out[i] = 0;
}

/* NE 5:5376-56BC. Menu handles and USER operations are explicit callbacks;
 * the original enable/check masks and MRU command identifiers are retained. */
void stars_initialize_menu(StarsInitializeMenuState *state,
    const StarsInitializeMenuOps *ops, void *context)
{
    StarsU16 root;
    StarsU16 submenu;
    StarsU16 i;
    StarsU16 flags;
    StarsU16 menu;
    StarsI16 count;
    StarsU8 work[259];
    StarsU16 salt_invalid;

    if (!state || !ops) return;
    if (!state->menu && ops->get_menu)
        state->menu = ops->get_menu(context, state->frame);
    if (!ops->get_sub_menu) return;
    root = ops->get_sub_menu(context, state->frame, 0U);
    for (i = 0x10ccU; i < 0x10d5U; ++i)
        if (ops->delete_menu) ops->delete_menu(context, root, i, 0U);
    for (i = 0U; i < 9U; ++i) {
        if (!state->mru[i] || !state->mru[i][0]) break;
        work[0] = '&'; work[1] = (StarsU8)('1' + i); work[2] = ' ';
        copy_mru(work + 3U, state->mru[i]);
        if (ops->insert_menu)
            ops->insert_menu(context, root, (StarsU16)(i + 9U),
                0x0400U, (StarsU16)(i + 0x10ccU), work);
    }
    flags = (!state->base || !state->base[0] ||
        (state->game_flags & 4U)) ? 3U : 0U;
    if (ops->enable_menu) ops->enable_menu(context, state->menu, 0x6aU,
        flags);
    flags = (!state->base || !state->base[0]) ? 3U : 0U;
    if (ops->enable_menu) ops->enable_menu(context, state->menu, 0x69U,
        flags);
    salt_invalid = ((StarsI16)state->salt_high < 0 ||
        (state->salt_high == 0U && state->salt_low == 0U));
    flags = (!state->base || !state->base[0] ||
        ((state->game_flags & 4U) && salt_invalid)) ? 3U : 0U;
    if (ops->enable_menu) ops->enable_menu(context, state->menu, 0x10eU,
        flags);
    flags = (!state->base || !state->base[0] ||
        (state->game_flags & 4U)) ? 3U : 0U;
    if (ops->enable_menu) ops->enable_menu(context, state->menu, 0x7deU,
        flags);
    if (ops->enable_menu) ops->enable_menu(context, state->menu, 0xedbU,
        flags);

    submenu = ops->get_sub_menu(context, state->frame, 1U);
    if (ops->check_menu) ops->check_menu(context, submenu, 0xb3U,
        (state->color_flags & 0x8000U) ? 8U : 0U);
    if (ops->check_menu) ops->check_menu(context, submenu, 0x98dU,
        (state->scan_flags & 0x2000U) ? 8U : 0U);
    if (!state->scanner) {
        if (ops->get_menu) menu = ops->get_menu(context, state->frame);
        else menu = state->menu;
        if (ops->enable_menu) ops->enable_menu(context, menu, 1U, 0x403U);
    } else {
        if (ops->get_menu) menu = ops->get_menu(context, state->frame);
        else menu = state->menu;
        if (ops->enable_menu) ops->enable_menu(context, menu, 1U, 0x400U);
        submenu = ops->get_sub_menu(context, state->frame, 1U);
        submenu = ops->get_sub_menu(context, submenu, 3U);
        if (ops->check_menu) ops->check_menu(context, submenu,
            (StarsU16)(state->scan_zoom + 4U), 0x408U);
        count = ops->menu_item_count ?
            ops->menu_item_count(context, submenu) : 0;
        for (i = 0U; i < (StarsU16)count; ++i)
            if (ops->enable_menu) ops->enable_menu(context, submenu, i,
                0x400U);
        submenu = ops->get_sub_menu(context, state->frame, 1U);
        submenu = ops->get_sub_menu(context, submenu, 4U);
        if (ops->check_menu) ops->check_menu(context, submenu,
            state->window_layout, 0x408U);
    }
    if (ops->draw_menu_bar) ops->draw_menu_bar(context, state->frame);
}
