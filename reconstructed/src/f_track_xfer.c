#include "core.h"

/* Typed semantic reconstruction of FTrackXfer, NE 11:5a16-5f98. */

static StarsI32 track_step(StarsI16 fkb)
{
    if (fkb & 8)
        return (fkb & 4) ? 1000 : 100;
    return (fkb & 4) ? 10 : 1;
}

static void track_drag(StarsTrackXferInput *input, StarsI16 button)
{
    StarsPoint point;
    StarsI32 available;
    StarsI32 projected;
    StarsI32 current;
    StarsI32 delta;
    StarsI32 width;
    StarsI32 numerator;

    if (!input->ops->set_capture || !input->ops->mouse_move ||
            !input->drag_rect_valid ||
            input->drag_rect.right - input->drag_rect.left <= 2)
        return;
    width = (StarsI32)input->drag_rect.right -
        (StarsI32)input->drag_rect.left - 2L;
    input->ops->set_capture(input->ops->context, input->hwnd);
    point.x = -1;
    point.y = -1;
    while (input->ops->mouse_move(input->ops->context, &point)) {
        if (input->ops->quantity)
            available = input->ops->quantity(input->ops->context,
                input->cargo_kind, input->source_side);
        else
            available = input->total_cargo;
        numerator = (StarsI32)point.x -
            (StarsI32)input->drag_rect.left;
        projected = available * numerator / width;
        current = input->ops->change_cargo ?
            input->ops->change_cargo(input->ops->context, input->cargo_kind,
                0) : input->other_cargo;
        delta = projected - current;
        if (input->source_side)
            delta = -delta;
        if (input->ops->supply && input->ops->supply(input->ops->context,
                input->cargo_kind, delta) != 0)
            if (input->ops->draw_dialog)
                input->ops->draw_dialog(input->ops->context, input->hwnd,
                    input->cargo_kind);
    }
    if (input->ops->release_capture)
        input->ops->release_capture(input->ops->context, input->hwnd);
    (void)button;
}

StarsI16 stars_track_xfer(StarsTrackXferInput *input)
{
    StarsI16 button;
    StarsI16 kind;
    StarsI32 step;
    StarsI32 delta;
    StarsI32 max_change;

    if (!input || !input->ops)
        return 0;
    button = -1;
    if (input->ops->hit_test)
        for (button = 0; button < input->button_count; ++button)
            if (input->ops->hit_test(input->ops->context, button,
                    &input->point))
                break;
    if (button == input->button_count)
        return 0;
    kind = input->cargo_kind;
    if (!input->source_mode) {
        if (kind < 5 && !(input->object_kind == 8 &&
                (kind == 3 || kind == 4)) && input->owner_matches)
            track_drag(input, button);
    } else {
        step = track_step(input->fkb);
        while (input->ops->track_button && input->ops->track_button(
                input->ops->context, button)) {
            if (input->md_xfer_dialog) {
                delta = step;
                max_change = input->other_cargo;
                if (max_change < delta)
                    delta = max_change;
                if (delta &&
                        ((input->ops->dialog_transfer &&
                          input->ops->dialog_transfer(input->ops->context,
                              kind, delta) != 0) ||
                         (!input->ops->dialog_transfer && input->ops->supply &&
                          input->ops->supply(input->ops->context, kind,
                              delta) != 0)))
                    if (input->ops->draw_dialog)
                        input->ops->draw_dialog(input->ops->context,
                            input->hdc, input->button_index);
            } else if (kind >= 0 && kind < 5) {
                delta = input->source_side ? -step : step;
                if (input->ops->supply && input->ops->supply(
                        input->ops->context, kind, delta) != 0)
                    if (input->ops->draw_dialog)
                        input->ops->draw_dialog(input->ops->context,
                            input->hdc, kind);
            }
        }
    }
    if (input->ops->update_buttons)
        input->ops->update_buttons(input->ops->context);
    if (input->ops->draw_dialog)
        input->ops->draw_dialog(input->ops->context, input->hdc, -2);
    return 1;
}
