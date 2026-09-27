#include "core.h"

/* Typed semantic reconstruction of PopupMenu, NE 25:136c-1938. */

static void copy_menu_label(StarsU8 *destination, StarsU16 capacity,
    const StarsU8 *source)
{
    StarsU16 used;

    used = 0;
    if (!source || capacity == 0)
        return;
    while (*source != 0 && (StarsU16)(used + 1) < capacity) {
        destination[used] = *source++;
        ++used;
        if (destination[used - 1] == '&' &&
                (StarsU16)(used + 1) < capacity)
            destination[used++] = '&';
    }
    destination[used] = 0;
}

static StarsU16 menu_item_flags(StarsU32 checked)
{
    return checked ? 8 : 0;
}

static StarsI16 menu_is_separator(const StarsU8 *label)
{
    return label && label[0] == (StarsU8)-1 && label[1] == 0;
}

static const StarsU8 *menu_text(const StarsPopupMenuOps *ops,
    StarsI16 source_kind, StarsI16 id, const StarsU8 *fallback)
{
    StarsPopupMenuText text;
    const StarsU8 *resolved;

    if (!ops->resolve_text)
        return fallback;
    text.source_kind = source_kind;
    text.id = id;
    resolved = ops->resolve_text(ops->context, &text);
    return resolved ? resolved : fallback;
}

StarsI16 stars_popup_menu(StarsU16 hwnd, StarsI16 x, StarsI16 y,
    StarsI16 c_string, const StarsU32 *ids, const StarsU8 *const *labels,
    StarsI16 checked, StarsI16 right_button, const StarsPopupMenuOps *ops)
{
    StarsPoint point;
    StarsU16 menu;
    StarsU16 submenu;
    StarsU16 flags;
    StarsU16 i;
    StarsU16 j;
    StarsU32 command;
    StarsU32 checked_value;
    const StarsU8 *label;
    StarsU8 escaped[128];
    StarsI16 selected;
    StarsI16 source_kind;

    selected = -1;
    if (!ops)
        return selected;
    point.x = x;
    point.y = y;
    if (ops->client_to_screen)
        ops->client_to_screen(ops->context, hwnd, &point);
    menu = ops->create_menu ? ops->create_menu(ops->context) : 0;
    submenu = 0;
    for (i = 0; i < (StarsU16)c_string; ++i) {
        if (!ids || (checked == -2 && labels)) {
            if (!labels)
                continue;
            label = labels[i];
            if (!label) {
                submenu = ops->create_menu ?
                    ops->create_menu(ops->context) : 0;
                checked_value = ids ? ids[i + 1] : 0;
                j = (StarsU16)(i + 2);
                while (j < (StarsU16)c_string && labels[j]) {
                    label = labels[j];
                    if (menu_is_separator(label)) {
                        if (ops->append_menu)
                            ops->append_menu(ops->context, submenu, 0x800,
                                0, (const StarsU8 *)0);
                    } else {
                        copy_menu_label(escaped, sizeof escaped, label);
                        checked_value = ids ? ids[j] :
                            (j == (StarsU16)checked);
                        if (ops->append_menu)
                            ops->append_menu(ops->context, submenu,
                                menu_item_flags(checked_value),
                                (StarsU16)(j + 15000), escaped);
                    }
                    ++j;
                }
                if (ops->append_menu)
                    ops->append_menu(ops->context, menu, menu_item_flags(
                        checked_value) | 0x10, submenu, labels[i + 1]);
                i = (StarsU16)(j - 1);
            } else if (menu_is_separator(label)) {
                if (ops->append_menu)
                    ops->append_menu(ops->context, menu, 0x800, 0,
                        (const StarsU8 *)0);
            } else {
                copy_menu_label(escaped, sizeof escaped, label);
                if (checked == -2)
                    checked_value = ids ? ids[i] : 0;
                else
                    checked_value = i == (StarsU16)checked;
                if (ops->append_menu)
                    ops->append_menu(ops->context, menu,
                        menu_item_flags(checked_value),
                        (StarsU16)(i + 15000), escaped);
            }
        } else if (ids) {
            if (ids[i] == 0xffffffffUL) {
                if (ops->append_menu)
                    ops->append_menu(ops->context, menu, 0x800, 0,
                        (const StarsU8 *)0);
            } else {
                source_kind = 0;
                if ((ids[i] >> 16) & 0x1000)
                    source_kind = 4;
                else if ((ids[i] >> 16) & 0x4000)
                    source_kind = 3;
                else if ((ids[i] >> 16) & 0x2000)
                    source_kind = 2;
                else if ((ids[i] >> 16) & 0x8000)
                    source_kind = 1;
                label = menu_text(ops, source_kind,
                    (StarsI16)(ids[i] & 0xffff), (const StarsU8 *)0);
                copy_menu_label(escaped, sizeof escaped, label);
                flags = i == (StarsU16)checked ? 8 : 0;
                if (ops->append_menu)
                    ops->append_menu(ops->context, menu, flags,
                        (StarsU16)(i + 15000), escaped);
            }
        }
    }
    flags = right_button ? 2 : 0;
    if (ops->track_popup)
        ops->track_popup(ops->context, menu, flags, point.x, point.y, hwnd);
    if (ops->destroy_menu)
        ops->destroy_menu(ops->context, menu);
    if (submenu && ops->destroy_menu)
        ops->destroy_menu(ops->context, submenu);
    command = 0;
    if (ops->peek_command && ops->peek_command(ops->context, hwnd,
            0x111, 0x111, 2, &command) && command > 14999 &&
            command < 0x3afc)
        selected = (StarsI16)(command - 15000);
    return selected;
}
