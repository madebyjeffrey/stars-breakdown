/* Genuine C reconstructions of the two tooltip-control proxy procedures. */
#include "core.h"

static StarsI32 fake_control_proc(StarsU32 real_proc, StarsU16 hwnd,
    StarsU16 message, StarsU16 wparam, StarsI32 lparam,
    const StarsFakeProcOps *ops, void *context)
{
    if (message == 0x0200U) {
        if (ops && ops->toolbar)
            ops->toolbar(context, hwnd, message, wparam, lparam);
    } else if (message == 0x0201U || message == 0x0203U) {
        if (ops && ops->show_tooltip) ops->show_tooltip(context, -1);
    }
    if (!ops || !ops->call_window) return 0L;
    return ops->call_window(context, real_proc, hwnd, message, wparam, lparam);
}

/* NE 14:1D72-1DF0. */
StarsI32 stars_fake_combo_proc(StarsU32 real_proc, StarsU16 hwnd,
    StarsU16 message, StarsU16 wparam, StarsI32 lparam,
    const StarsFakeProcOps *ops, void *context)
{
    return fake_control_proc(real_proc, hwnd, message, wparam, lparam,
        ops, context);
}

/* NE 14:1DF0-1E6D. */
StarsI32 stars_fake_ce_proc(StarsU32 real_proc, StarsU16 hwnd,
    StarsU16 message, StarsU16 wparam, StarsI32 lparam,
    const StarsFakeProcOps *ops, void *context)
{
    return fake_control_proc(real_proc, hwnd, message, wparam, lparam,
        ops, context);
}

/* NE 14:0BB2.  Negative values are synthetic toolbar IDs; the -3 case uses
 * the Arial height to select the two vertical layouts. */
StarsI16 stars_dx_of_btn(StarsI16 button, StarsI16 arial_height)
{
    if (button >= 0)
        return (button == 0x0d || button == 0x0f) ? 0x0b : 0x1d;
    if (button == -1) return 6;
    if (button == -2) return 2;
    if (button == -3) return arial_height < 0x10 ? 0x3c : 0x46;
    return 0;
}

/* NE 14:06F0.  The original toolbar table is supplied explicitly so the
 * reconstruction exposes the data dependency instead of hiding it in code. */
void stars_draw_toolbar(StarsU16 hdc, StarsU16 palette, const StarsI16 *buttons,
    StarsU16 button_count, StarsI16 arial_height,
    const StarsDrawToolbarOps *ops, void *context)
{
    StarsU16 index;
    StarsI16 x;
    StarsI16 button;
    StarsPoint point;

    if (!buttons || !ops)
        return;
    if (ops->select_palette)
        ops->select_palette(context, hdc, palette);
    if (ops->realize_palette)
        ops->realize_palette(context, hdc);
    x = 4;
    for (index = 0; index < button_count; ++index) {
        button = buttons[index];
        if (button >= 0 && ops->button_down && ops->draw_button) {
            point.x = x;
            point.y = 4;
            ops->draw_button(context, hdc, point, button,
                ops->button_down(context, button));
        }
        x = (StarsI16)(x + stars_dx_of_btn(button, arial_height));
    }
}

/* NE 14:0B12.  The original table lives in the toolbar code segment; the
 * reconstructed interface receives that 29-entry table explicitly. */
StarsI16 stars_itb_from_ppt(StarsPoint *point, const StarsI16 *buttons,
    StarsU16 button_count, StarsI16 arial_height)
{
    StarsU16 index;
    StarsI16 x;
    StarsI16 width;

    x = 4;
    if (point->x < 4 || point->y < 4 || point->y >= 0x20)
        return -1;
    for (index = 0; index < button_count; ++index) {
        width = stars_dx_of_btn(buttons[index], arial_height);
        if (point->x < x + width) {
            point->x = x;
            point->y = 4;
            return buttons[index];
        }
        x = (StarsI16)(x + width);
    }
    return -1;
}
