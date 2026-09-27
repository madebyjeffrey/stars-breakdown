#include "core.h"

/* NE 16:A210-A31F. Track one of the paired race-value controls and redraw it
 * after each accepted increment/decrement.  USER/button state is explicit in
 * callbacks; the hit-to-value and keyboard-acceleration rules remain here. */
StarsI16 stars_track_new_game_dlg3(StarsU16 hwnd, StarsPoint point,
    StarsI16 keyboard, const StarsTrackNewGameDlg3Ops *ops, void *context)
{
    StarsI16 hit;
    StarsI16 vc;
    StarsI16 delta;
    StarsU16 message;
    StarsU16 hdc = 0U;
    StarsI16 old_value;
    StarsI16 new_value;
    if (!ops || !ops->hit_test || !ops->track_button ||
        !ops->get_value || !ops->set_value)
        return 0;
    hit = ops->hit_test(context, point);
    if (hit < 0) return 0;
    vc = (StarsI16)((StarsU16)hit >> 1);
    if (((StarsU16)hit & 1U) == 0U) {
        delta = 1;
        message = 0x00a0U;
    } else {
        delta = -1;
        message = 0x00a1U;
    }
    if (ops->init_track)
        ops->init_track(context, hwnd, (StarsU16)hit, message, 0x0050U, 0U);
    if ((keyboard & 0x000c) != 0)
        delta = (StarsI16)(delta * 5);
    while (ops->track_button(context, hwnd, &hdc) != 0) {
        old_value = ops->get_value(context, vc, 1);
        new_value = ops->set_value(context, vc,
            (StarsI16)(old_value + delta));
        if (new_value != old_value && ops->draw)
            ops->draw(context, hwnd, hdc, vc);
    }
    return 1;
}
