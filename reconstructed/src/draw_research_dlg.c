#include "core.h"
#include <string.h>

#define RESEARCH_TITLE_STRING 0x01bcU
#define RESEARCH_HEADER_STRING 0x0044U
#define RESEARCH_SECTION_STRING 0x0045U
#define RESEARCH_COST_FORMAT_STRING 0x053fU
#define RESEARCH_NEXT_LEVEL_STRING 0x0046U
#define RESEARCH_REMAINING_STRING 0x004cU
#define RESEARCH_TOTAL_STRING 0x004eU
#define RESEARCH_TOTAL_RESEARCH_STRING 0x0047U
#define RESEARCH_LAST_YEAR_STRING 0x004fU
#define RESEARCH_PERCENT_STRING 0x0050U
#define RESEARCH_BUDGET_STRING 0x0051U
#define RESEARCH_NOTE_STRING 0x050aU
#define RESEARCH_NO_COST_STRING 0x034aU
#define RESEARCH_NO_BUDGET_STRING 0x034bU
#define RESEARCH_RATIO_STRING 0x034cU
#define RESEARCH_TECH_NAME_BASE 0x0054U
#define RESEARCH_TECH_LINES 9
#define RESEARCH_FLAG_DRAW_TECH 0x0004U
#define RESEARCH_FLAG_COST 0x0300U
#define RESEARCH_FLAG_COST_FILL 0x0200U
#define RESEARCH_FLAG_TOTAL 0x0400U
#define RESEARCH_FLAG_TOTAL_RESEARCH 0x0800U
#define RESEARCH_FLAG_LAST_YEAR 0x1000U
#define RESEARCH_FLAG_PERCENT 0x2000U
#define RESEARCH_FLAG_SPIN 0x4000U
#define RESEARCH_FLAG_INVALIDATE 0x0004U
#define RESEARCH_TEXT_BLACK 0U
#define RESEARCH_TEXT_CURRENT 0x007f00UL
#define RESEARCH_TEXT_FUTURE 0x007f0000UL

static StarsU16 research_string(const StarsDrawResearchInput *input,
    StarsU16 id, StarsU8 *buffer, StarsU16 capacity)
{
    if (!input->ops->get_string) return 0;
    return input->ops->get_string(input->context, id, buffer, capacity);
}

static void research_text(const StarsDrawResearchInput *input, StarsU16 hdc,
    StarsI16 x, StarsI16 y, const StarsU8 *text, StarsU16 length)
{
    if (input->ops->text_out) input->ops->text_out(input->context, hdc,
        x, y, text, length);
}

static void research_right(const StarsDrawResearchInput *input, StarsU16 hdc,
    StarsI16 x, StarsI16 y, const StarsU8 *text, StarsU16 length,
    StarsI16 offset)
{
    if (input->ops->right_text) input->ops->right_text(input->context, hdc,
        x, y, text, length, offset);
}

static StarsU32 research_difference(StarsU32 cost, StarsU32 saved,
    StarsU16 double_cost)
{
    if (double_cost) cost <<= 1;
    if (cost <= saved) return 0U;
    return cost - saved;
}

static StarsU32 research_ratio(StarsU32 remaining, StarsU32 budget)
{
    if (remaining == 0U) return 1U;
    if (budget == 0U) return 0U;
    return (StarsU32)(((remaining * 100UL) + budget - 1UL) / budget);
}

static void research_draw_technology_list(StarsDrawResearchInput *input,
    StarsU16 hdc, StarsRect *rect, StarsI16 saved_technology)
{
    StarsU8 name[64];
    StarsU8 requirements[6];
    const StarsU8 *part_name;
    StarsI16 level;
    StarsI16 item;
    StarsI16 group;
    StarsI16 result;

    input->future_tech_count = 0;
    for (level = 1; level <= RESEARCH_TECH_LINES; ++level) {
        if (level == 1) {
            if (input->ops->set_text_color)
                input->ops->set_text_color(input->context, hdc,
                    RESEARCH_TEXT_CURRENT);
        } else if (level >= 2 && level <= 4) {
            if (input->ops->set_text_color)
                input->ops->set_text_color(input->context, hdc,
                    RESEARCH_TEXT_FUTURE);
        } else if (input->ops->set_text_color) {
            input->ops->set_text_color(input->context, hdc,
                RESEARCH_TEXT_BLACK);
        }
        group = level;
        item = 0;
        for (;;) {
            part_name = (const StarsU8 *)0;
            result = input->ops->lookup_part ? input->ops->lookup_part(
                input->context, (StarsU16)(1U << (level - 1)),
                (StarsU16)item, requirements, &part_name) : 0;
            if (result == 0) break;
            if (result > 0 && part_name && input->future_tech_count < 8) {
                (void)research_string(input, (StarsU16)(RESEARCH_TECH_NAME_BASE +
                    (StarsU16)group), name, (StarsU16)sizeof name);
                (void)name;
                if (rect->bottom >= rect->top + input->dy_arial8) {
                    ++rect->top;
                    research_text(input, hdc, (StarsI16)(rect->left + 8),
                        rect->top, part_name,
                        (StarsU16)strlen((const char *)part_name));
                    input->future_tech_ids[input->future_tech_count++] =
                        (StarsU16)((level << 8) | (item & 0xff));
                    rect->top = (StarsI16)(rect->top + input->dy_arial8);
                }
            }
            ++item;
        }
    }
    input->current_technology = saved_technology;
    if (input->ops->set_text_color)
        input->ops->set_text_color(input->context, hdc, RESEARCH_TEXT_BLACK);
}

/* NE 28:090A-0D5C. Draw the research dialog in independent header, current
 * research, projected-cost, history, and technology-note phases.  The packed
 * player and USER/GDI records are represented by the typed input and service
 * callbacks, while the original flag gates, cost arithmetic, row spacing,
 * invalidation, and spin-button geometry remain in this source. */
void stars_draw_research_dialog(StarsDrawResearchInput *input)
{
    const StarsDrawResearchOps *ops;
    StarsU16 hdc;
    StarsI16 acquired;
    StarsI16 title_width;
    StarsI16 saved_technology;
    StarsI16 level;
    StarsRect rect;
    StarsU8 text[96];
    StarsU8 format[96];
    StarsU32 cost;
    StarsU32 remaining;
    StarsU32 ratio;
    StarsU16 length;

    if (!input || !input->ops) return;
    ops = input->ops;
    acquired = 0;
    hdc = input->hdc;
    if (hdc == 0U && ops->get_dc) {
        hdc = ops->get_dc(input->context, input->hwnd);
        acquired = 1;
    }
    if (hdc == 0U) return;
    if (ops->select_font) ops->select_font(input->context, hdc, input->font);
    if (ops->set_text_color) ops->set_text_color(input->context, hdc,
        RESEARCH_TEXT_BLACK);
    if (ops->set_bk_color) ops->set_bk_color(input->context, hdc,
        input->button_face);

    rect = input->paint_rect;
    if ((input->draw_flags & 0xff) != 0) {
        length = research_string(input, RESEARCH_TITLE_STRING, text,
            (StarsU16)sizeof text);
        title_width = ops->text_extent ? ops->text_extent(input->context,
            hdc, text, length) : 0;
        if (ops->set_rect) ops->set_rect(input->context, &rect, 8,
            input->dy_arial8, (StarsI16)(input->dx_res_left - 8),
            (StarsI16)(input->dy_arial8 * 13));
        if ((input->draw_flags & 3) != 0) {
            if (ops->draw_frame) ops->draw_frame(input->context, hdc, &rect,
                -1);
            length = research_string(input, RESEARCH_HEADER_STRING, text,
                (StarsU16)sizeof text);
            research_text(input, hdc, (StarsI16)(rect.left + 8),
                (StarsI16)(rect.top - input->dy_arial8 / 2), text, length);
            length = research_string(input, RESEARCH_TITLE_STRING, text,
                (StarsU16)sizeof text);
            research_text(input, hdc, (StarsI16)(rect.right - 8 - title_width),
                rect.top, text, length);
            rect.top = (StarsI16)(rect.top + input->dy_arial8);
            if (ops->draw_frame) research_draw_technology_list(input, hdc,
                &rect, input->current_technology);
        }
        if ((input->draw_flags & RESEARCH_FLAG_INVALIDATE) == 0 ||
                (input->draw_flags & 3) != 0) {
            if (ops->draw_frame) ops->draw_frame(input->context, hdc, &rect,
                -1);
        } else if (ops->invalidate_rect) {
            if (ops->set_rect) ops->set_rect(input->context, &rect,
                (StarsI16)(rect.left - 6), (StarsI16)(rect.top - 6),
                (StarsI16)(rect.right + 6), (StarsI16)(rect.bottom + 6));
            ops->invalidate_rect(input->context, input->hwnd, &rect, 1);
        }
    }

    if (ops->set_rect) ops->set_rect(input->context, &rect,
        (StarsI16)(input->dx_res_left + 8), input->dy_arial8,
        (StarsI16)(input->dx_res_left + input->dx_res_right - 8),
        (StarsI16)(input->dy_arial8 * 8 + input->dy_arial8));
    if ((input->draw_flags & RESEARCH_FLAG_COST) != 0) {
        level = input->technology_level[input->current_technology];
        if (level < 0x1a && ops->level_cost)
            cost = ops->level_cost(input->context, input->current_technology,
                (StarsI16)(level + 1), input->player);
        else cost = (StarsU32)-1L;
        remaining = (cost == (StarsU32)-1L) ? cost :
            research_difference(cost,
                input->research[input->current_technology],
                (StarsU16)(input->game_flags & 2U));
        if ((input->draw_flags & RESEARCH_FLAG_COST_FILL) == 0 ||
                (input->draw_flags & 0x0f) != 0) {
            if (ops->draw_frame) ops->draw_frame(input->context, hdc, &rect,
                -1);
            length = research_string(input, RESEARCH_NEXT_LEVEL_STRING, text,
                (StarsU16)sizeof text);
            research_text(input, hdc, (StarsI16)(rect.left + 8), rect.top,
                text, length);
        } else if (ops->fill_rect) {
            ops->fill_rect(input->context, hdc, rect.left, rect.top,
                rect.right, rect.bottom, input->button_face);
        }
        ratio = (remaining == (StarsU32)-1L) ? 0U :
            research_ratio(remaining, input->resource_budget);
        (void)ratio;
        if (remaining == (StarsU32)-1L)
            length = research_string(input, RESEARCH_NO_COST_STRING, text,
                (StarsU16)sizeof text);
        else {
            length = research_string(input, RESEARCH_COST_FORMAT_STRING,
                format, (StarsU16)sizeof format);
            (void)format;
            length = research_string(input, RESEARCH_REMAINING_STRING, text,
                (StarsU16)sizeof text);
        }
        research_right(input, hdc,
            (StarsI16)(rect.right - input->dx_res_string_right), rect.top,
            text, length, 0);
    }
    if ((input->draw_flags & RESEARCH_FLAG_TOTAL) != 0) {
        length = research_string(input, RESEARCH_TOTAL_STRING, text,
            (StarsU16)sizeof text);
        research_right(input, hdc, rect.right, (StarsI16)(rect.top +
            input->dy_arial8), text, length, 0);
    }
    if ((input->draw_flags & RESEARCH_FLAG_TOTAL_RESEARCH) != 0) {
        length = research_string(input, RESEARCH_TOTAL_RESEARCH_STRING, text,
            (StarsU16)sizeof text);
        research_right(input, hdc, rect.right, (StarsI16)(rect.top +
            input->dy_arial8 * 2), text, length, 0);
    }
    if ((input->draw_flags & RESEARCH_FLAG_LAST_YEAR) != 0) {
        length = research_string(input, RESEARCH_LAST_YEAR_STRING, text,
            (StarsU16)sizeof text);
        research_right(input, hdc, rect.right, (StarsI16)(rect.top +
            input->dy_arial8 * 3), text, length, 0);
    }
    if ((input->draw_flags & RESEARCH_FLAG_PERCENT) != 0) {
        length = research_string(input, RESEARCH_PERCENT_STRING, text,
            (StarsU16)sizeof text);
        research_right(input, hdc, rect.right, (StarsI16)(rect.top +
            input->dy_arial8 * 4), text, length, 0);
    }
    saved_technology = input->current_technology;
    if (ops->set_text_color) ops->set_text_color(input->context, hdc,
        input->window_text);
    input->current_technology = saved_technology;
    if ((input->draw_flags & RESEARCH_FLAG_SPIN) != 0 && ops->draw_button) {
        input->spin_top.left = (StarsU16)(rect.right - 19);
        input->spin_top.top = (StarsU16)(rect.top - 4);
        input->spin_top.right = (StarsU16)(input->spin_top.left + 19);
        input->spin_top.bottom = (StarsU16)(rect.top + input->dy_arial8 / 2);
        input->spin_bottom = input->spin_top;
        input->spin_bottom.top = (StarsU16)(input->spin_top.bottom - 1);
        ops->draw_button(input->context, hdc, &input->spin_top, 0xa0U);
        ops->draw_button(input->context, hdc, &input->spin_bottom, 0xa1U);
    }
    if (ops->set_text_color) ops->set_text_color(input->context, hdc,
        RESEARCH_TEXT_BLACK);
    if (ops->select_font) ops->select_font(input->context, hdc, input->font);
    if (acquired && ops->release_dc)
        ops->release_dc(input->context, input->hwnd, hdc);
}
