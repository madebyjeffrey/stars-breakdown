#include "core.h"

enum {
    EXECUTE_POPUP_MINES = 8,
    EXECUTE_POPUP_SHIPS = 13,
    EXECUTE_POPUP_ENEMY_SHIPS = 15,
    EXECUTE_POPUP_ZOOM = 16,
    EXECUTE_SCAN_MINE = 0x0040,
    EXECUTE_SCAN_SHIP = 0x0200,
    EXECUTE_SCAN_ENEMY_SHIP = 0x0800,
    EXECUTE_TUTOR_ACTIVE = 0x0800,
    EXECUTE_HIDDEN_SHIP = 0x0200,
    EXECUTE_ALL_MINES = 0x000f,
    EXECUTE_ALL_ENEMY_SHIPS = 0x00ff,
    EXECUTE_ZOOM_COMMAND_BASE = 0x0f3d
};

static StarsI16 execute_popup(StarsExecuteButtonInput *input,
    StarsU16 kind, StarsU16 current, StarsU16 available)
{
    StarsPoint point;

    point.x = 0;
    point.y = 0;
    if (input->ops.get_cursor_pos)
        input->ops.get_cursor_pos(input->context, input->toolbar_window,
            &point);
    if (input->ops.screen_to_client)
        input->ops.screen_to_client(input->context, input->toolbar_window,
            &point);
    if (!input->ops.popup_menu)
        return -1;
    return input->ops.popup_menu(input->context, kind, point.x, point.y,
        current, available, -1);
}

static StarsU16 execute_visible_ship_mask(
    const StarsExecuteButtonInput *input)
{
    StarsU16 mask = 0U;
    StarsU16 index;

    for (index = 0U; index < input->ship_definition_count && index < 16U;
            ++index)
        if (!input->ship_definition_flags ||
                (input->ship_definition_flags[index] & EXECUTE_HIDDEN_SHIP) == 0U)
            mask = (StarsU16)(mask | (StarsU16)(1U << index));
    return mask;
}

static StarsU16 execute_toggle_visible_ship(
    const StarsExecuteButtonInput *input, StarsI16 row)
{
    StarsU16 index;
    StarsI16 remaining = (StarsI16)(row - 4);

    for (index = 0U; index < input->ship_definition_count && index < 16U;
            ++index) {
        if (input->ship_definition_flags &&
                (input->ship_definition_flags[index] & EXECUTE_HIDDEN_SHIP) != 0U)
            continue;
        if (remaining-- == 0)
            return (StarsU16)(1U << index);
    }
    return 0U;
}

static void execute_common(StarsExecuteButtonInput *input, StarsI16 button)
{
    if (button != 6 && input->ops.invalidate)
        input->ops.invalidate(input->context, input->scanner_window);
    if ((input->game_bits & EXECUTE_TUTOR_ACTIVE) != 0U &&
            input->ops.advance_tutor)
        input->ops.advance_tutor(input->context);
}

/* Genuine semantic reconstruction of ExecuteButton, NE 14:0DB6-1680.
 * Buttons 0..5 select a scanner direction, the ordinary buttons toggle
 * scanner layers, and buttons 8, 13, and 15 open localized filter menus.
 * Menu construction is deliberately a callback: it preserves the original
 * UI boundary while this function documents the resulting mask transitions,
 * hidden ship-definition filtering, redraw rules, and tutor notification. */
void stars_execute_button(StarsExecuteButtonInput *input, StarsI16 button,
    StarsI16 pressed)
{
    StarsI16 selection;
    StarsU16 mask;
    StarsU16 visible;
    StarsU16 bit;

    if (!input)
        return;
    input->game_bits2 = (StarsU16)((input->game_bits2 & 0xffbfU) | 0x0040U);

    if (button >= 0 && button <= 5) {
        if (!pressed)
            return;
        input->grbit_scan = (StarsU16)((input->grbit_scan & 0x3ff0U) |
            (StarsU16)button);
        execute_common(input, button);
        return;
    }

    switch (button) {
    case 6: bit = 0x0010U; break;
    case 7: bit = 0x0020U; break;
    case 9: bit = 0x0080U; break;
    case 10: bit = 0x0100U; break;
    case 11: bit = 0x0400U; break;
    case 12: bit = 0x0200U; break;
    case 14: bit = 0x0800U; break;
    case 17: bit = 0x1000U; break;
    default: bit = 0U; break;
    }
    if (bit != 0U) {
        if (pressed)
            input->grbit_scan = (StarsU16)(input->grbit_scan | bit);
        else
            input->grbit_scan = (StarsU16)(input->grbit_scan & (StarsU16)~bit);
        execute_common(input, button);
        return;
    }

    if (button == EXECUTE_POPUP_MINES) {
        if ((input->grbit_scan & EXECUTE_SCAN_MINE) == 0U)
            input->grbit_scan_mines = 0U;
        selection = execute_popup(input, EXECUTE_POPUP_MINES,
            input->grbit_scan_mines, EXECUTE_ALL_MINES);
        if (selection < 0)
            return;
        if (selection == 0)
            input->grbit_scan_mines = EXECUTE_ALL_MINES;
        else if (selection == 1)
            input->grbit_scan_mines = 0U;
        else if (selection >= 3 && selection < 7)
            input->grbit_scan_mines = (StarsU16)(input->grbit_scan_mines ^
                (StarsU16)(1U << (selection - 3)));
        else
            return;
        if (input->grbit_scan_mines == 0U)
            input->grbit_scan = (StarsU16)(input->grbit_scan & 0xffbfU);
        else
            input->grbit_scan = (StarsU16)(input->grbit_scan | EXECUTE_SCAN_MINE);
        if (input->ops.invalidate)
            input->ops.invalidate(input->context, input->toolbar_window);
        execute_common(input, button);
        return;
    }

    if (button == EXECUTE_POPUP_SHIPS ||
            button == EXECUTE_POPUP_ENEMY_SHIPS) {
        if (button == EXECUTE_POPUP_SHIPS) {
            visible = execute_visible_ship_mask(input);
            mask = input->grbit_scan_ship;
        } else {
            visible = EXECUTE_ALL_ENEMY_SHIPS;
            mask = input->grbit_scan_enemy_ship;
        }
        selection = execute_popup(input, (StarsU16)button, mask, visible);
        if (selection < 0)
            return;
        if (selection == 0)
            mask = visible;
        else if (selection == 1)
            mask = (StarsU16)(mask ^ visible);
        else if (selection < 4)
            mask = 0U;
        else if (button == EXECUTE_POPUP_SHIPS)
            mask = (StarsU16)(mask ^ execute_toggle_visible_ship(input,
                selection));
        else if (selection < 4 + 8)
            mask = (StarsU16)(mask ^ (StarsU16)(1U << (selection - 4)));
        else
            return;

        if (button == EXECUTE_POPUP_SHIPS)
            input->grbit_scan_ship = mask;
        else
            input->grbit_scan_enemy_ship = mask;
        bit = button == EXECUTE_POPUP_SHIPS ? EXECUTE_SCAN_SHIP :
            EXECUTE_SCAN_ENEMY_SHIP;
        if ((input->grbit_scan & bit) == 0U && mask != 0U) {
            input->grbit_scan = (StarsU16)(input->grbit_scan | bit);
            if (input->ops.invalidate)
                input->ops.invalidate(input->context, input->toolbar_window);
        }
        if ((input->grbit_scan & bit) == 0U)
            return;
        execute_common(input, button);
        return;
    }

    if (button == EXECUTE_POPUP_ZOOM) {
        selection = execute_popup(input, EXECUTE_POPUP_ZOOM,
            (StarsU16)(input->scan_zoom + 4), 9U);
        if (selection >= 0 && input->ops.command_handler)
            input->ops.command_handler(input->context,
                (StarsU16)(selection + EXECUTE_ZOOM_COMMAND_BASE));
    }
}
