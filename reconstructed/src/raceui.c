#include "ui_internal.h"

StarsI16 stars_signed_byte(StarsU8 value)
{ return value < 128U ? (StarsI16)value : (StarsI16)((int)value - 256); }
StarsRect stars_race_rect(StarsRuntime *r, StarsU16 i)
{ return stars_read_rect(r->data, (StarsU16)(stars_word(r->data, 0x48acU) + i * 8U)); }
void stars_set_race_rect(StarsRuntime *r, StarsU16 i, StarsRect rect)
{ stars_write_rect(r->data, (StarsU16)(stars_word(r->data, 0x48acU) + i * 8U), &rect); }

void stars_race_title(StarsRuntime *r, StarsU16 hwnd, StarsI16 step)
{
    char format[52];
    strcpy(format, (char *)r->data + stars_game_string(r,
        stars_signed_word((StarsU16)(stars_word(r->data, 0x5320U) + 0x10eU))));
    sprintf((char *)r->data + 0x57a4U, format, (int)step);
    stars_window(r, "SetWindowText", hwnd, 0, 0, 0, r->data + 0x57a4U, 0);
}

void stars_invalidate_advantage(StarsRuntime *r, StarsU16 hwnd)
{
    StarsU16 allocation, dc, font, old;
    StarsU8 metrics[31];
    StarsRect rect;
    StarsU16 height, width;
    allocation = (StarsU16)stars_window(r, "LocalAlloc", 0, 0x40U, 50U, 0, 0, 0);
    dc = (StarsU16)stars_window(r, "GetDC", hwnd, 0, 0, 0, 0, 0);
    stars_put_word(r->data, allocation, 0xffe8U);
    strcpy((char *)r->data + allocation + 18U, (char *)r->data + 0x6c6U);
    font = (StarsU16)stars_window(r, "CreateFontIndirect", 0, 0, 0, 0, r->data + allocation, 0);
    old = stars_select(r, dc, font);
    stars_window(r, "GetTextMetrics", dc, 0, 0, 0, metrics, 0);
    height = (StarsU16)(stars_word(metrics, 0) + stars_word(metrics, 8));
    width = (StarsU16)(stars_text_extent(r, dc, r->data + 0x135eU, 6) + 8U);
    stars_window(r, "GetClientRect", hwnd, 0, 0, 0, &rect, 0);
    rect.left = (StarsU16)(rect.right - width); rect.bottom = (StarsU16)(rect.top + height + 4U);
    stars_select(r, dc, old); stars_window(r, "DeleteObject", font, 0, 0, 0, 0, 0);
    stars_window(r, "LocalFree", allocation, 0, 0, 0, 0, 0);
    stars_window(r, "ReleaseDC", hwnd, dc, 0, 0, 0, 0);
    stars_window(r, "InvalidateRect", hwnd, 1, 0, 0, &rect, 0);
}

void stars_draw_advantage(StarsRuntime *r, StarsU16 dc, const StarsRect *p, StarsU16 player)
{
    StarsU16 allocation, font, old, mode, height, width, length, top = r->scratch_top, label;
    StarsU8 metrics[31];
    StarsRect rect;
    StarsU32 foreground, background;
    StarsI16 points;
    StarsU8 *d = r->data;
    allocation = (StarsU16)stars_window(r, "LocalAlloc", 0, 0x40U, 50U, 0, 0, 0);
    stars_put_word(d, allocation, 0xffe8U);
    stars_copy_game_string(d, r->strings, 0x538, (StarsU16)(allocation + 18U));
    font = (StarsU16)stars_window(r, "CreateFontIndirect", 0, 0, 0, 0, d + allocation, 0);
    old = stars_select(r, dc, font);
    stars_window(r, "GetTextMetrics", dc, 0, 0, 0, metrics, 0);
    height = (StarsU16)(stars_word(metrics, 0) + stars_word(metrics, 8));
    width = (StarsU16)(stars_text_extent(r, dc, d + 0x1365U, 6) + 8U);
    rect = stars_rect((StarsU16)(p->right - width), p->top, p->right, (StarsU16)(p->top + height + 4U));
    points = stars_race_advantage_points(d, player ? player : 0x4f9aU);
    mode = (StarsU16)stars_window(r, "SetBkMode", dc, 2, 0, 0, 0, 0);
    foreground = stars_window(r, "SetTextColor", dc, 0, 0, points < 0 ? 127U : 0, 0, 0);
    background = stars_window(r, "SetBkColor", dc, 0, 0, stars_dword(d, 0x22baU), 0, 0);
    sprintf((char *)d + 0x57a4U, (char *)d + stars_word(d, 0x1aaU), (int)points);
    stars_rect_center_text(r, dc, &rect, 0x57a4U, -1);
    stars_window(r, "SetTextColor", dc, 0, 0, 0, 0, 0);
    stars_select(r, dc, stars_word(d, 0x26b6U));
    length = (StarsU16)stars_copy_game_string(d, r->strings, 0x530, 0x57a4U);
    width = (StarsU16)stars_text_extent(r, dc, d + 0x57a4U, length);
    label = stars_scratch(r, 32U);
    stars_copy_game_string(d, r->strings, 0x52f, label);
    stars_right_text(r, dc, stars_signed_word(rect.left), 3, label, (StarsI16)strlen((char *)d + label), 0);
    stars_right_text(r, dc, stars_signed_word(rect.left),
        stars_signed_word((StarsU16)(stars_word(d, 0x23faU) + 3U)), 0x57a4U, (StarsI16)length, 0);
    rect.left = (StarsU16)(rect.left - width - 4U);
    stars_pat_blt(r, dc, (StarsU16)(rect.left - 1U), 0, 1, (StarsU16)(rect.bottom + 1U), 0x42U);
    stars_pat_blt(r, dc, (StarsU16)(rect.left - 4U), 0, 2, (StarsU16)(rect.bottom + 4U), 0x42U);
    stars_pat_blt(r, dc, rect.left, rect.bottom, (StarsU16)(rect.right - rect.left), 1, 0x42U);
    stars_pat_blt(r, dc, (StarsU16)(rect.left - 2U), (StarsU16)(rect.bottom + 2U),
        (StarsU16)(rect.right - rect.left + 2U), 2, 0x42U);
    stars_window(r, "SetBkColor", dc, 0, 0, background, 0, 0);
    stars_window(r, "SetTextColor", dc, 0, 0, foreground, 0, 0);
    stars_window(r, "SetBkMode", dc, mode, 0, 0, 0, 0);
    stars_select(r, dc, old); stars_window(r, "DeleteObject", font, 0, 0, 0, 0, 0);
    stars_window(r, "LocalFree", allocation, 0, 0, 0, 0, 0); r->scratch_top = top;
}
