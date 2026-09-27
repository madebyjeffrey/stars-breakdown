#include "core.h"
#include <string.h>

enum {
    REPORT_FIRST_X = 2,
    REPORT_FIRST_Y = 2,
    REPORT_TITLE_LEFT_MARGIN = 3,
    REPORT_TITLE_TOP_MARGIN = 2,
    REPORT_TITLE_HEIGHT_EXTRA = 4,
    REPORT_CELL_LEFT_MARGIN = 2,
    REPORT_CELL_TOP_MARGIN = 2,
    REPORT_CELL_RIGHT_MARGIN = 3,
    REPORT_CELL_BOTTOM_MARGIN = 3,
    REPORT_BACKGROUND_MODE_TRANSPARENT = 1
};

#define REPORT_LINE_ROP 0x00f00021UL

static StarsI16 report_visible(const StarsReportViewState *view,
    StarsI16 field)
{
    if (field < 0 || field >= 32)
        return 0;
    return (StarsI16)((view->visible_fields & (1UL << field)) != 0UL);
}

static StarsI16 report_field_visible(const StarsReportViewState *view,
    StarsI16 field)
{
    return (StarsI16)(report_visible(view, field) &&
        (field == 0 || view->field_first <= (StarsU16)field));
}

/* Genuine semantic reconstruction of DrawReport, NE 22:0BAE-0F65.
 * The routine draws the report header only when its top edge is exposed, then
 * walks the visible row window.  The report state owns the original 32-bit
 * field mask and cached half-widths; UI drawing and report-specific item
 * rendering remain explicit callbacks. */
void stars_draw_report(StarsU16 hwnd, StarsU16 hdc,
    const StarsRect *paint_rect,
    StarsDrawReportInput *input)
{
    StarsI16 field;
    StarsI16 row;
    StarsI16 row_end;
    StarsI16 x;
    StarsI16 y;
    StarsI16 width;
    StarsI16 length;
    StarsI16 paint_top;
    StarsI16 paint_bottom;
    StarsU8 text[42];
    StarsRect rect;

    (void)hwnd;
    if (!input || !input->view || !paint_rect || !input->ops)
        return;
    paint_top = (StarsI16)paint_rect->top;
    paint_bottom = (StarsI16)paint_rect->bottom;
    if (input->ops->select_object)
        input->ops->select_object(input->context, hdc, input->font);
    if (input->ops->set_background_mode)
        input->ops->set_background_mode(input->context, hdc,
            REPORT_BACKGROUND_MODE_TRANSPARENT);

    x = REPORT_FIRST_X;
    y = REPORT_FIRST_Y;
    if (paint_top <= (StarsI16)(input->arial8_height + 6)) {
        for (field = 0; field < (StarsI16)input->view->fields; ++field) {
            if (!report_field_visible(input->view, field))
                continue;
            text[0] = 0;
            width = input->ops->header_width ?
                input->ops->header_width(input->context,
                    input->view->report_kind, field, text, hdc) : 0;
            input->view->column_width[field] = (StarsU8)(width / 2);
            rect.left = (StarsU16)x;
            rect.top = (StarsU16)y;
            rect.right = (StarsU16)(x + width - 1);
            rect.bottom = (StarsU16)(input->arial8_height + 6);
            if (input->fill_cells && input->ops->fill_rect)
                input->ops->fill_rect(input->context, hdc, &rect,
                    input->button_face_brush);
            length = (StarsI16)strlen((const char *)text);
            if (field == 0) {
                if (input->ops->text_out)
                    input->ops->text_out(input->context, hdc,
                        (StarsI16)(rect.left + REPORT_TITLE_LEFT_MARGIN),
                        (StarsI16)(rect.top + REPORT_TITLE_TOP_MARGIN),
                        text, length);
            } else if (input->ops->center_text) {
                input->ops->center_text(input->context, hdc,
                    (StarsI16)((rect.right - rect.left) / 2 + rect.left),
                    (StarsI16)(rect.top + REPORT_TITLE_TOP_MARGIN), text, 0);
            }
            if (input->ops->draw_frame)
                input->ops->draw_frame(input->context, hdc, &rect, 0);
            x = (StarsI16)(x + width);
        }
    }

    row_end = (StarsI16)(input->view->row_first +
        input->view->rows_visible);
    if (input->view->row_count < row_end)
        row_end = input->view->row_count;
    y = (StarsI16)(input->arial8_height + REPORT_TITLE_HEIGHT_EXTRA);
    for (row = input->view->row_first; row < row_end; ++row) {
        y = (StarsI16)(y + input->arial8_height + REPORT_TITLE_HEIGHT_EXTRA);
        x = REPORT_FIRST_X;
        if (paint_top <= y && y <= paint_bottom) {
            if (input->ops->select_object)
                input->ops->select_object(input->context, hdc,
                    input->shadow_brush);
            if (input->ops->pat_blt)
                input->ops->pat_blt(input->context, hdc, x, y, 1,
                    (StarsI16)(input->arial8_height + 4), REPORT_LINE_ROP);
        }
        for (field = 0; field < (StarsI16)input->view->fields; ++field) {
            if (!report_field_visible(input->view, field))
                continue;
            width = (StarsI16)(input->view->column_width[field] * 2);
            if (paint_top - (input->arial8_height + 4) <= y &&
                    y <= paint_bottom) {
                rect.left = (StarsU16)(x + REPORT_CELL_LEFT_MARGIN);
                rect.top = (StarsU16)(y + REPORT_CELL_TOP_MARGIN);
                rect.right = (StarsU16)(x + width -
                    REPORT_CELL_RIGHT_MARGIN);
                rect.bottom = (StarsU16)(y + input->arial8_height +
                    REPORT_CELL_BOTTOM_MARGIN);
                if (input->ops->select_object)
                    input->ops->select_object(input->context, hdc,
                        input->shadow_brush);
                if (input->ops->pat_blt) {
                    input->ops->pat_blt(input->context, hdc,
                        (StarsI16)(x + width - 1), y, 1,
                        (StarsI16)(input->arial8_height + 4), REPORT_LINE_ROP);
                    input->ops->pat_blt(input->context, hdc, x,
                        (StarsI16)(y + input->arial8_height + 4), width, 1,
                        REPORT_LINE_ROP);
                }
                if (input->fill_cells && input->ops->fill_rect)
                    input->ops->fill_rect(input->context, hdc, &rect,
                        input->button_face_brush);
                if (input->ops->draw_item)
                    input->ops->draw_item(input->context, hdc, &rect,
                        input->view->report_kind, row, field);
            }
            x = (StarsI16)(x + width);
        }
    }
}
