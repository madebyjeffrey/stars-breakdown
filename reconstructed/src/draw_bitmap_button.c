#include "core.h"

enum {
    DRAW_BUTTON_ICON_SMALL_WIDTH = 7,
    DRAW_BUTTON_ICON_WIDTH = 0x18,
    DRAW_BUTTON_HEIGHT = 0x1c,
    DRAW_BUTTON_INNER_OFFSET = 2,
    DRAW_BUTTON_ICON_HEIGHT = 0x17,
    DRAW_BUTTON_ICON_STRIDE = 0x18,
    DRAW_BUTTON_ICON_ROP = 0x00cc0020UL,
    DRAW_BUTTON_FRAME_ROP = 0x00f00021UL
};

static void draw_button_pat(const StarsDrawBitmapButtonInput *input,
    StarsU16 brush, StarsI16 x, StarsI16 y, StarsI16 width, StarsI16 height)
{
    (void)brush;
    if (input->ops.pat_blt)
        input->ops.pat_blt(input->context, input->hdc, x, y, width, height,
            DRAW_BUTTON_FRAME_ROP);
}

/* Genuine semantic reconstruction of DrawBitmapButton, NE 14:078C-0B12.
 * The toolbar bitmap is painted inside a classic Win16 three-dimensional
 * button.  A released button has a highlight on its top/left and a shadow on
 * its bottom/right; a pressed button swaps those edges, fills the pressed
 * inset, and shifts the icon by the pressed depth.  Narrow synthetic buttons
 * use the seven-pixel radar glyph while ordinary buttons use a 24-pixel
 * glyph. */
void stars_draw_bitmap_button(const StarsDrawBitmapButtonInput *input)
{
    StarsI16 width;
    StarsI16 icon_width;
    StarsU16 upper_brush;
    StarsU16 lower_brush;
    StarsU16 old_brush;
    StarsI16 x;
    StarsI16 y;

    if (!input) return;
    width = stars_dx_of_btn(input->button, input->arial8_height);
    icon_width = width < DRAW_BUTTON_ICON_WIDTH ?
        DRAW_BUTTON_ICON_SMALL_WIDTH : DRAW_BUTTON_ICON_WIDTH;
    if (input->pressed == 0) {
        upper_brush = input->button_highlight_brush;
        lower_brush = input->button_shadow_brush;
    } else {
        upper_brush = input->button_shadow_brush;
        lower_brush = input->button_highlight_brush;
    }

    if (input->ops.select_object)
        input->ops.select_object(input->context, input->hdc, upper_brush);
    draw_button_pat(input, upper_brush, (StarsI16)(input->point.x + 2),
        input->point.y, (StarsI16)(width - 4), 1);
    draw_button_pat(input, upper_brush, input->point.x,
        (StarsI16)(input->point.y + 2), 1, 0x18);
    draw_button_pat(input, upper_brush, (StarsI16)(input->point.x + 1),
        (StarsI16)(input->point.y + 1), 1, 1);
    draw_button_pat(input, upper_brush, (StarsI16)(input->point.x + 1),
        (StarsI16)(input->point.y + 0x1a), 1, 1);

    if (input->ops.select_object)
        input->ops.select_object(input->context, input->hdc, lower_brush);
    draw_button_pat(input, lower_brush, (StarsI16)(input->point.x + 2),
        (StarsI16)(input->point.y + 0x1b), (StarsI16)(width - 4), 1);
    draw_button_pat(input, lower_brush, (StarsI16)(input->point.x + width - 1),
        (StarsI16)(input->point.y + 2), 1, 0x18);
    draw_button_pat(input, lower_brush, (StarsI16)(input->point.x + width - 2),
        (StarsI16)(input->point.y + 1), 1, 1);
    draw_button_pat(input, lower_brush, (StarsI16)(input->point.x + width - 2),
        (StarsI16)(input->point.y + 0x1a), 1, 1);

    if (input->ops.select_object)
        old_brush = input->ops.select_object(input->context, input->hdc,
            input->button_face_brush);
    else old_brush = 0;
    draw_button_pat(input, input->button_face_brush,
        (StarsI16)(input->point.x + 2),
        (StarsI16)(input->point.y + 1), (StarsI16)(width - 4), 1);
    draw_button_pat(input, input->button_face_brush,
        (StarsI16)(input->point.x + 1),
        (StarsI16)(input->point.y + 2), 1, 0x18);
    if (input->pressed == 0) {
        draw_button_pat(input, input->button_face_brush,
            (StarsI16)(input->point.x + 2),
            (StarsI16)(input->point.y + 0x19), (StarsI16)(width - 4), 2);
        draw_button_pat(input, input->button_face_brush,
            (StarsI16)(input->point.x + width - 3),
            (StarsI16)(input->point.y + 2), 2, 0x18);
    } else {
        draw_button_pat(input, input->button_face_brush,
            (StarsI16)(input->point.x + 2),
            (StarsI16)(input->point.y + 2), (StarsI16)input->pressed,
            input->pressed);
        draw_button_pat(input, input->button_face_brush,
            (StarsI16)(input->point.x + 2),
            (StarsI16)(input->point.y + 2), input->pressed, 0x18);
        if (input->pressed == 1) {
            draw_button_pat(input, input->button_face_brush,
                (StarsI16)(input->point.x + 2),
                (StarsI16)(input->point.y + 0x1a),
                (StarsI16)(width - 4), 1);
            draw_button_pat(input, input->button_face_brush,
                (StarsI16)(input->point.x + width - 2),
                (StarsI16)(input->point.y + 2), 1, 0x18);
        }
    }

    if (input->ops.select_object)
        input->ops.select_object(input->context, input->hdc, old_brush);
    x = (StarsI16)(input->point.x + 2 + input->pressed);
    y = (StarsI16)(input->point.y + 2 + input->pressed);
    if (input->ops.dib_blt)
        input->ops.dib_blt(input->context, input->hdc, x, y, icon_width,
            DRAW_BUTTON_ICON_HEIGHT, input->toolbar_bitmap,
            (StarsI16)(input->button * DRAW_BUTTON_ICON_STRIDE), 0,
            icon_width, DRAW_BUTTON_ICON_HEIGHT, DRAW_BUTTON_ICON_ROP);
    if (input->pressed > 1) {
        if (input->ops.select_object)
            input->ops.select_object(input->context, input->hdc,
                input->button_highlight_brush);
        draw_button_pat(input, input->button_highlight_brush,
            (StarsI16)(input->point.x + width - 2),
            (StarsI16)(input->point.y + 0x1a), 1, 1);
    }
}
