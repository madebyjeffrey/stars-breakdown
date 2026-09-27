#include "core.h"

#define TUTOR_TEXT_CONTROL 2U
#define TUTOR_TEXT_BORDER_ROP 0xf00021UL

static void tutor_text_expand(StarsRect *rect, StarsI16 dx, StarsI16 dy)
{
    rect->left = (StarsU16)(rect->left - (StarsU16)dx);
    rect->right = (StarsU16)(rect->right + (StarsU16)dx);
    rect->top = (StarsU16)(rect->top - (StarsU16)dy);
    rect->bottom = (StarsU16)(rect->bottom + (StarsU16)dy);
}

/* NE 32:03C0-06B4.  Paint the inset tutor text panel, decode up to eight
 * tutor strings, add paragraph spacing from the resource metadata, and
 * temporarily reverse the colors for the bold tutor line. */
void stars_draw_tutor_text(const StarsDrawTutorInput *input)
{
    StarsRect paint;
    StarsRect panel;
    StarsRect control;
    StarsPoint point;
    StarsU16 hdc;
    StarsU16 control_hwnd;
    StarsI16 x;
    StarsI16 y;
    StarsI16 line;
    StarsI16 length;
    StarsI16 new_line;
    StarsU8 text[256];

    if (!input || !input->ops)
        return;
    paint.left = paint.top = paint.right = paint.bottom = 0U;
    panel.left = panel.top = panel.right = panel.bottom = 0U;
    control.left = control.top = control.right = control.bottom = 0U;
    hdc = input->ops->begin_paint ? input->ops->begin_paint(input->context,
        input->hwnd, &paint) : 0U;
    if (input->ops->set_background_color)
        input->ops->set_background_color(input->context, hdc,
            input->button_face_color);
    if (input->ops->select_object)
        (void)input->ops->select_object(input->context, hdc,
            input->arial8_font);
    if (input->ops->get_window_rect)
        input->ops->get_window_rect(input->context, input->hwnd, &panel);
    control_hwnd = input->ops->get_dialog_item ?
        input->ops->get_dialog_item(input->context, input->hwnd,
            TUTOR_TEXT_CONTROL) : 0U;
    if (input->ops->get_window_rect)
        input->ops->get_window_rect(input->context, control_hwnd, &control);
    panel.bottom = control.top;
    if (input->ops->screen_to_client) {
        point.x = panel.left; point.y = panel.top;
        input->ops->screen_to_client(input->context, input->hwnd, &point);
        panel.left = point.x; panel.top = point.y;
        point.x = panel.right; point.y = panel.bottom;
        input->ops->screen_to_client(input->context, input->hwnd, &point);
        panel.right = point.x; panel.bottom = point.y;
    }
    panel.top = (StarsU16)(panel.top + input->arial8_height * 2);
    panel.bottom = (StarsU16)(panel.bottom -
        (input->arial8_height * 2) / 3);
    panel.left = (StarsU16)(panel.left +
        (input->arial8_height * 2) / 3);
    panel.right = (StarsU16)(panel.right -
        (input->arial8_height * 2) / 3);
    if (input->ops->select_object)
        (void)input->ops->select_object(input->context, hdc,
            input->button_shadow_brush);
    if (input->ops->pat_blt) {
        input->ops->pat_blt(input->context, hdc, panel.left, panel.top,
            (StarsI16)(panel.right - panel.left), 1,
            TUTOR_TEXT_BORDER_ROP);
        input->ops->pat_blt(input->context, hdc, panel.left, panel.top, 1,
            (StarsI16)(panel.bottom - panel.top), TUTOR_TEXT_BORDER_ROP);
    }
    if (input->ops->select_object)
        (void)input->ops->select_object(input->context, hdc,
            input->button_highlight_brush);
    if (input->ops->pat_blt) {
        input->ops->pat_blt(input->context, hdc, panel.left,
            (StarsI16)(panel.bottom - 1U),
            (StarsI16)(panel.right - panel.left), 1,
            TUTOR_TEXT_BORDER_ROP);
        input->ops->pat_blt(input->context, hdc,
            (StarsI16)(panel.right - 1U), panel.top, 1,
            (StarsI16)(panel.bottom - panel.top), TUTOR_TEXT_BORDER_ROP);
    }
    tutor_text_expand(&panel, (StarsI16)(-input->arial8_height / 2),
        (StarsI16)(-input->arial8_height / 2));
    y = (StarsI16)panel.top;
    if (input->ops->fill_rect)
        input->ops->fill_rect(input->context, hdc, &panel,
            input->button_face_brush);
    if (input->ops->set_text_color)
        input->ops->set_text_color(input->context, hdc,
            input->button_text_color);
    for (line = 0; line < 8; ++line) {
        if (!input->ops->get_string)
            break;
        length = input->ops->get_string(input->context, text,
            (StarsI16)(input->idt + line));
        if (length == 1)
            break;
        new_line = input->ops->has_break ? input->ops->has_break(
            input->context, text[0]) : 0;
        x = (StarsI16)panel.left;
        if (new_line && line != 0)
            y = (StarsI16)(y + input->arial8_height / 2);
        if (input->idt + line == input->idt_bold) {
            if (input->ops->set_text_color)
                input->ops->set_text_color(input->context, hdc,
                    input->button_face_color);
            if (input->ops->set_background_color)
                input->ops->set_background_color(input->context, hdc,
                    input->button_text_color);
        }
        if (input->ops->wrap_text)
            input->ops->wrap_text(input->context, hdc, &x, &y, text,
                length, (StarsI16)panel.left,
                (StarsI16)(panel.right - panel.left), 0, new_line, 1);
        if (input->idt + line == input->idt_bold) {
            if (input->ops->set_text_color)
                input->ops->set_text_color(input->context, hdc,
                    input->button_text_color);
            if (input->ops->set_background_color)
                input->ops->set_background_color(input->context, hdc,
                    input->button_face_color);
        }
    }
    if (input->ops->end_paint)
        input->ops->end_paint(input->context, input->hwnd, &paint);
}
