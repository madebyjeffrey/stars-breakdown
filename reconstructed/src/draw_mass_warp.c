#include "core.h"

/* NE 10:2AFA-2C37. Select the gauge colors from the current and best mass
 * warp values, draw the gauge, and center its formatted result. */
void stars_draw_mass_warp_gauge(StarsDrawMassWarpGaugeInput *input)
{
    StarsI16 negative;
    StarsI16 best;
    StarsI16 current;
    StarsI32 maximum;
    StarsI32 gauge_value;
    StarsI32 displayed_value;
    StarsU16 brush;
    StarsU16 old_mode;
    StarsU8 text[64];
    StarsI16 length;

    if (!input || !input->rect || !input->draw_gauge ||
        !input->format_value || !input->center_text)
        return;
    negative = input->best < 0 ? 1 : 0;
    if (input->select_object)
        input->select_object(input->context, input->hdc, input->font_arial8);
    current = input->current < 5 ? 5 : input->current;
    best = input->best < 0 ? (StarsI16)-input->best : input->best;
    maximum = (StarsI32)best - 1L;
    if ((StarsI32)best + negative < current) {
        brush = (StarsI32)current < (StarsI32)best + negative + 3L ?
            input->brush_yellow : input->brush_red;
    } else brush = input->brush_purple;
    gauge_value = (StarsI32)current - 4L;
    gauge_value = input->draw_gauge(input->context, input->hdc, input->rect,
        1, brush, maximum);
    old_mode = input->set_bk_mode ?
        input->set_bk_mode(input->context, input->hdc, 1U) : 0U;
    displayed_value = gauge_value + 4L;
    length = input->format_value(input->context, displayed_value, text,
        (StarsU16)sizeof(text));
    if (input->get_text_extent)
        input->get_text_extent(input->context, input->hdc, text, length);
    input->center_text(input->context, input->hdc, input->rect, text, length);
    if (input->set_bk_mode)
        input->set_bk_mode(input->context, input->hdc, old_mode);
}
