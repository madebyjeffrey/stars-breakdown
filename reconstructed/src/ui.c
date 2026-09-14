/* Shared drawing and input helpers used by the race dialogs. Coordinates are
 * Win16 signed words. Valid Windows rectangles/text spans are the input domain. */
#include "ui_internal.h"

StarsI16 stars_half(StarsU16 value)
{ return stars_signed_word((StarsU16)((value >> 1) | (value & 0x8000U))); }
StarsRect stars_rect(StarsU16 l, StarsU16 t, StarsU16 rr, StarsU16 b)
{ StarsRect rect; rect.left = l; rect.top = t; rect.right = rr; rect.bottom = b; return rect; }
StarsRect stars_read_rect(const StarsU8 *d, StarsU16 off)
{ return stars_rect(stars_word(d, off), stars_word(d, (StarsU16)(off + 2U)),
    stars_word(d, (StarsU16)(off + 4U)), stars_word(d, (StarsU16)(off + 6U))); }
void stars_write_rect(StarsU8 *d, StarsU16 off, const StarsRect *p)
{
    stars_put_word(d, off, p->left); stars_put_word(d, (StarsU16)(off + 2U), p->top);
    stars_put_word(d, (StarsU16)(off + 4U), p->right); stars_put_word(d, (StarsU16)(off + 6U), p->bottom);
}
int stars_point_in_rect(const StarsRect *p, StarsPoint point)
{
    return stars_signed_word(point.x) >= stars_signed_word(p->left) &&
        stars_signed_word(point.x) < stars_signed_word(p->right) &&
        stars_signed_word(point.y) >= stars_signed_word(p->top) &&
        stars_signed_word(point.y) < stars_signed_word(p->bottom);
}
StarsU32 stars_text_extent(StarsRuntime *r, StarsU16 dc, const StarsU8 *text, StarsU16 n)
{ return stars_window(r, "GetTextExtent", dc, n, 0, 0, (void *)text, 0); }
void stars_text_out(StarsRuntime *r, StarsU16 dc, StarsU16 x, StarsU16 y, const StarsU8 *text, StarsU16 n)
{ stars_window(r, "TextOut", dc, x, y, n, (void *)text, 0); }
void stars_pat_blt(StarsRuntime *r, StarsU16 dc, StarsU16 x, StarsU16 y, StarsU16 w, StarsU16 h, StarsU32 rop)
{ StarsPoint size; size.x = w; size.y = h; stars_window(r, "PatBlt", dc, x, y, rop, &size, 0); }
void stars_window_position(StarsRuntime *r, StarsU16 hwnd, StarsU16 x, StarsU16 y,
    StarsU16 width, StarsU16 height, StarsU16 flags)
{ StarsRect box = stars_rect(x, y, width, height); stars_window(r, "SetWindowPos", hwnd, 0, flags, 0, &box, 0); }
StarsU16 stars_select(StarsRuntime *r, StarsU16 dc, StarsU16 object)
{ return (StarsU16)stars_window(r, "SelectObject", dc, object, 0, 0, 0, 0); }
void stars_fill_rect(StarsRuntime *r, StarsU16 dc, const StarsRect *rect, StarsU16 brush)
{ stars_window(r, "FillRect", dc, brush, 0, 0, (void *)rect, 0); }
void stars_screen_point(StarsRuntime *r, StarsU16 hwnd, void *point)
{ stars_window(r, "ScreenToClient", hwnd, 0, 0, 0, point, 0); }

/* NE 9:3094-31A2. Remembered positions are clamped; a newly centered dialog
 * retains the original negative position when larger than the screen. */
void stars_sticky_dialog(StarsRuntime *r, StarsU16 hwnd, StarsPoint *position, StarsI16 init)
{
    StarsRect rect;
    StarsI16 width, height;
    stars_window(r, "GetWindowRect", hwnd, 0, 0, 0, &rect, 0);
    if (!init) { position->x = rect.left; position->y = rect.top; return; }
    width = stars_signed_word((StarsU16)stars_window(r, "GetSystemMetrics", 0, 0, 0, 0, 0, 0));
    height = stars_signed_word((StarsU16)stars_window(r, "GetSystemMetrics", 0, 1, 0, 0, 0, 0));
    if (position->x == 65535U && position->y == 65535U) {
        rect.left = (StarsU16)stars_half((StarsU16)(width - (rect.right - rect.left)));
        rect.top = (StarsU16)stars_half((StarsU16)(height - (rect.bottom - rect.top)));
    } else {
        stars_offset_rect(&rect, stars_signed_word((StarsU16)(position->x - rect.left)),
            stars_signed_word((StarsU16)(position->y - rect.top)));
        if (width < stars_signed_word(rect.right)) rect.left = (StarsU16)(rect.left + width - rect.right);
        if (height < stars_signed_word(rect.bottom)) rect.top = (StarsU16)(rect.top + height - rect.bottom);
        if (stars_signed_word(rect.left) < 0) rect.left = 0;
        if (stars_signed_word(rect.top) < 0) rect.top = 0;
    }
    stars_window_position(r, hwnd, rect.left, rect.top, 0, 0, 0x15U);
}

void stars_draw_frame(StarsRuntime *r, StarsU16 dc, const StarsRect *p, StarsI16 erase)
{
    StarsRect rect = *p;
    StarsU16 width = (StarsU16)(p->right - p->left), height = (StarsU16)(p->bottom - p->top);
    StarsU16 old = stars_select(r, dc, stars_word(r->data, 0x14U));
    if (erase == -1 || erase == -2) {
        stars_pat_blt(r, dc, rect.left, rect.bottom, (StarsU16)(width + 1U), 1, 0xf00021UL);
        stars_pat_blt(r, dc, rect.right, rect.top, 1, height, 0xf00021UL);
        stars_select(r, dc, stars_word(r->data, 0x16U));
        stars_pat_blt(r, dc, rect.left, rect.top, width, 1, 0xf00021UL);
        stars_pat_blt(r, dc, rect.left, rect.top, 1, height, 0xf00021UL);
        stars_expand_rect(&rect, -1, -1); width = (StarsU16)(width - 2U); height = (StarsU16)(height - 2U);
        stars_select(r, dc, stars_word(r->data, 0x14U));
    }
    stars_pat_blt(r, dc, rect.left, rect.top, width, 1, 0xf00021UL);
    stars_pat_blt(r, dc, rect.left, rect.top, 1, height, 0xf00021UL);
    stars_select(r, dc, stars_word(r->data, 0x16U));
    stars_pat_blt(r, dc, rect.left, rect.bottom, (StarsU16)(width + 1U), 1, 0xf00021UL);
    stars_pat_blt(r, dc, rect.right, rect.top, 1, height, 0xf00021UL);
    if (erase && erase != -2) {
        stars_select(r, dc, stars_word(r->data, 0x10U));
        stars_pat_blt(r, dc, (StarsU16)(rect.left + 1U), (StarsU16)(rect.top + 1U),
            (StarsU16)(width - 1U), (StarsU16)(height - 1U), 0xf00021UL);
    }
    stars_select(r, dc, old);
}

void stars_center_text(StarsRuntime *r, StarsU16 dc, StarsI16 x, StarsI16 y, StarsU16 text, StarsI16 n)
{
    StarsU32 extent;
    if (!n) n = (StarsI16)strlen((char *)r->data + text);
    extent = stars_text_extent(r, dc, r->data + text, (StarsU16)n);
    stars_text_out(r, dc, (StarsU16)(x - stars_half((StarsU16)extent)), (StarsU16)y, r->data + text, (StarsU16)n);
}
void stars_rect_center_text(StarsRuntime *r, StarsU16 dc, const StarsRect *rect, StarsU16 text, StarsI16 n)
{
    StarsU32 extent;
    StarsI16 dx, dy;
    if (n == -1) { stars_fill_rect(r, dc, rect, stars_word(r->data, 0x10U)); n = 0; }
    if (!n) n = (StarsI16)strlen((char *)r->data + text);
    extent = stars_text_extent(r, dc, r->data + text, (StarsU16)n);
    dx = stars_signed_word((StarsU16)(rect->right - rect->left - (StarsU16)extent));
    dy = stars_signed_word((StarsU16)(rect->bottom - rect->top - (StarsU16)(extent >> 16)));
    stars_text_out(r, dc, (StarsU16)(dx / 2 + rect->left), (StarsU16)(dy / 2 + rect->top), r->data + text, (StarsU16)n);
}
void stars_right_text(StarsRuntime *r, StarsU16 dc, StarsI16 x, StarsI16 y, StarsU16 text, StarsI16 n, StarsI16 erase)
{
    StarsI16 width;
    StarsRect rect;
    if (n == -1) n = stars_copy_game_string(r->data, r->strings, 0x263, text);
    else if (!n) n = (StarsI16)strlen((char *)r->data + text);
    width = stars_signed_word((StarsU16)stars_text_extent(r, dc, r->data + text, (StarsU16)n));
    if (width < erase) {
        rect = stars_rect((StarsU16)(x - erase), (StarsU16)y, (StarsU16)(x - width),
            (StarsU16)(y + stars_word(r->data, 0x23faU)));
        stars_fill_rect(r, dc, &rect, stars_word(r->data, 0x10U));
    } else if (erase > 0 && erase < width) {
        rect = stars_rect((StarsU16)(x - erase), (StarsU16)y, (StarsU16)x,
            (StarsU16)(y + stars_word(r->data, 0x23faU)));
        /* value packs options in the high word, length in the low word. */
        stars_window(r, "ExtTextOut", dc, (StarsU16)(x - width), (StarsU16)y,
            0x60000UL | (StarsU16)n, r->data + text, &rect);
        return;
    }
    stars_text_out(r, dc, (StarsU16)(x - width), (StarsU16)y, r->data + text, (StarsU16)n);
}

StarsU16 stars_dib_num_colors(const StarsU8 *header)
{
    StarsU16 depth;
    if (stars_dword(header, 0) == 12U) depth = stars_word(header, 10U);
    else {
        if (stars_dword(header, 32U)) return stars_word(header, 32U);
        depth = stars_word(header, 14U);
    }
    return depth == 1U ? 2U : depth == 4U ? 16U : depth == 8U ? 256U : 0;
}
StarsU16 stars_palette_size(const StarsU8 *header)
{ return (StarsU16)(stars_dib_num_colors(header) * (stars_dword(header, 0) == 12U ? 3U : 4U)); }
StarsI16 stars_dib_blt(StarsRuntime *r, StarsU16 dc, StarsI16 x, StarsI16 y, StarsI16 width,
    StarsI16 height, StarsU16 bitmap, StarsI16 sx, StarsI16 sy, StarsI16 sw, StarsI16 sh, StarsU32 rop)
{
    StarsU32 pointer;
    const StarsU8 *header;
    StarsI16 geometry[8];
    StarsU16 bits_offset;
    if (!bitmap) {
        StarsPoint size;
        size.x = (StarsU16)width; size.y = (StarsU16)height;
        return stars_signed_word((StarsU16)stars_window(r, "PatBlt", dc,
            (StarsU16)x, (StarsU16)y, rop, &size, 0));
    }
    pointer = stars_window(r, "GlobalLock", bitmap, 0, 0, 0, 0, 0);
    if (!pointer) return 0;
    header = r->platform->memory(r->context, (StarsU16)(pointer >> 16), (StarsU16)pointer);
    bits_offset = (StarsU16)(stars_word(header, 0) + stars_palette_size(header));
    geometry[0] = x; geometry[1] = y; geometry[2] = width; geometry[3] = height;
    geometry[4] = sx; geometry[5] = sy; geometry[6] = sw; geometry[7] = sh;
    /* q points to the BITMAPINFO, a gives its near offset to the bits.
     * The native adapter preserves the selector when adding that offset. */
    stars_window(r, "StretchDIBits", dc, bits_offset, 0, rop, geometry, (void *)header);
    stars_window(r, "GlobalUnlock", bitmap, 0, 0, 0, 0, 0);
    return 1;
}

StarsI16 stars_get_mouse_move(StarsRuntime *r, StarsPoint *point)
{
    StarsU8 message[18];
    StarsU16 type;
    for (;;) {
        if (!stars_window(r, "PeekMessage", 0, 0, 0, 1, message, 0)) return 1;
        type = stars_word(message, 2);
        if (type == 0x200U || type == 0x202U) {
            point->x = stars_word(message, 6); point->y = stars_word(message, 8);
            return (StarsI16)(type != 0x202U);
        }
        /* The original consumes unrelated messages without dispatching them. */
    }
}
void stars_init_button_track(StarsRuntime *r, StarsButtonTrack *p, StarsU16 hwnd, StarsU16 dc,
    const StarsRect *rect, StarsU16 type, StarsU16 delay, StarsU16 down, StarsU16 no_redraw, StarsU16 text)
{
    p->window = hwnd; p->flags = (StarsU16)((p->flags & 0xfff7U) | (dc ? 0 : 8U));
    if (!dc) dc = (StarsU16)stars_window(r, "GetDC", hwnd, 0, 0, 0, 0, 0);
    p->dc = dc; p->rect = *rect; p->type = type; p->delay = delay;
    p->flags = (StarsU16)((p->flags & 0xffe0U) | (p->flags & 8U) | 3U |
        ((down & 1U) << 2) | ((no_redraw & 1U) << 4));
    p->text = text;
}
StarsI16 stars_track_button(StarsRuntime *r, StarsButtonTrack *p)
{
    StarsPoint point;
    StarsU16 inside;
    StarsU32 now, deadline;
    if (p->flags & 1U) {
        stars_window(r, "SetCapture", p->window, 0, 0, 0, 0, 0);
        stars_draw_button(r, p->dc, &p->rect, p->type,
            (StarsI16)(((p->flags >> 1) ^ (p->flags >> 2)) & 1U), p->text);
        now = stars_window(r, "GetCurrentTime", 0, 0, 0, 0, 0, 0);
        now += (StarsU32)(StarsI32)stars_signed_word((StarsU16)(p->delay * 3U));
        stars_put_dword(p->ticks, 0, now); p->flags &= 0xfffeU;
    } else {
        point.x = p->rect.left; point.y = p->rect.top;
        for (;;) {
            if (!stars_get_mouse_move(r, &point)) {
                if ((p->flags & 2U) && !(p->flags & 16U)) {
                    p->flags &= 0xfffdU;
                    stars_draw_button(r, p->dc, &p->rect, p->type, (StarsI16)((p->flags >> 2) & 1U), p->text);
                    p->flags |= 2U;
                }
                stars_window(r, "ReleaseCapture", 0, 0, 0, 0, 0, 0);
                if (p->flags & 8U) stars_window(r, "ReleaseDC", p->window, p->dc, 0, 0, 0, 0);
                return 0;
            }
            inside = (StarsU16)stars_point_in_rect(&p->rect, point);
            if (inside != ((p->flags >> 1) & 1U)) {
                p->flags = (StarsU16)((p->flags & 0xfffdU) | (inside << 1));
                stars_draw_button(r, p->dc, &p->rect, p->type,
                    (StarsI16)(((p->flags >> 1) ^ (p->flags >> 2)) & 1U), p->text);
            }
            now = stars_window(r, "GetCurrentTime", 0, 0, 0, 0, 0, 0);
            deadline = stars_dword(p->ticks, 0);
            if ((now ^ 0x80000000UL) >= (deadline ^ 0x80000000UL) && (p->flags & 2U)) break;
        }
        stars_put_dword(p->ticks, 0, now + (StarsU32)(StarsI32)stars_signed_word(p->delay));
    }
    return 1;
}

void stars_draw_button(StarsRuntime *r, StarsU16 dc, const StarsRect *p, StarsU16 type, StarsI16 down, StarsU16 text)
{
    StarsRect rect = *p;
    StarsU8 *d = r->data;
    StarsU16 old = stars_select(r, dc, stars_word(d, 0x1cU)), brush, font, mode;
    StarsU32 color;
    StarsI16 pts[12], w, h, dx, dy, x, y, step, length, t;
    StarsI32 scale;
    int i, count, disabled = (type & 4U) != 0, direction = type & 3U;
    if (!(type & 0x40U)) {
        stars_window(r, "FrameRect", dc, stars_word(d, 0x1cU), 0, 0, &rect, 0);
        stars_expand_rect(&rect, -1, -1);
    }
    w = stars_signed_word((StarsU16)(rect.right - rect.left - 1U));
    h = stars_signed_word((StarsU16)(rect.bottom - rect.top - 1U));
    scale = (StarsI32)stars_signed_word((StarsU16)(p->right - p->left)) - 5L;
    stars_select(r, dc, stars_word(d, down ? 0x16U : 0x14U));
    stars_pat_blt(r, dc, rect.left, rect.top, (StarsU16)w, 1, 0xf00021UL);
    stars_pat_blt(r, dc, rect.left, rect.top, 1, (StarsU16)h, 0xf00021UL);
    stars_select(r, dc, stars_word(d, down ? 0x10U : 0x16U));
    stars_pat_blt(r, dc, rect.left, (StarsU16)(rect.bottom - 1U), (StarsU16)(w + 1), 1, 0xf00021UL);
    stars_pat_blt(r, dc, (StarsU16)(rect.right - 1U), rect.top, 1, (StarsU16)h, 0xf00021UL);
    if (w < 14 || h < 14 || (type & 0xc0U)) stars_expand_rect(&rect, -1, -1);
    else {
        stars_pat_blt(r, dc, (StarsU16)(rect.left + 1U), (StarsU16)(rect.bottom - 2U), (StarsU16)(w - 1), 1, 0xf00021UL);
        stars_pat_blt(r, dc, (StarsU16)(rect.right - 2U), (StarsU16)(rect.top + 1U), 1, (StarsU16)(h - 2), 0xf00021UL);
        rect = stars_rect((StarsU16)(rect.left + 1U), (StarsU16)(rect.top + 1U),
            (StarsU16)(rect.right - 2U), (StarsU16)(rect.bottom - 2U));
    }
    stars_select(r, dc, stars_word(d, 0x10U)); stars_fill_rect(r, dc, &rect, stars_word(d, 0x10U));
    if (!(type & 8U)) {
        count = (type & 0x20U) ? 3 : 5; w = (type & 0x20U) ? 8 : 6; h = (type & 0x20U) ? 4 : 6;
        for (i = 0; i < count * 2; ++i) pts[i] = stars_signed_word(stars_word(d,
            (StarsU16)(((type & 0x20U) ? 0x13a0U : 0x138cU) + i * 2U)));
        if (type & 16U) {
            for (i = 0; i < count; ++i) ++pts[i * 2 + 1];
            h += 2; pts[count * 2] = 0; pts[count * 2 + 1] = 0; ++count;
        }
        for (i = 0; i < count; ++i) {
            if (pts[i * 2] < 0) pts[i * 2] = (StarsI16)(pts[0] * 2 - pts[-pts[i * 2] * 2]);
            else pts[i * 2] = (StarsI16)((StarsI32)pts[i * 2] * scale / 11L);
            pts[i * 2 + 1] = (StarsI16)((StarsI32)pts[i * 2 + 1] * scale / 11L);
        }
        w = (StarsI16)((StarsI32)w * scale / 11L); h = (StarsI16)((StarsI32)h * scale / 11L);
        dx = (StarsI16)(stars_half((StarsU16)(p->right - p->left - w)) + down);
        dy = (StarsI16)(stars_half((StarsU16)(p->bottom - p->top - h)) + down);
        if (direction >= 2) { t = dx; dx = dy; dy = t; }
        for (i = 0; i < count; ++i) {
            if (direction == 1) pts[i * 2 + 1] = (StarsI16)(h - pts[i * 2 + 1]);
            if (direction >= 2) {
                t = (StarsI16)(w - pts[i * 2]); pts[i * 2] = pts[i * 2 + 1]; pts[i * 2 + 1] = t;
                if (direction == 3) pts[i * 2] = (StarsI16)(h - pts[i * 2]);
            }
            pts[i * 2] = stars_signed_word((StarsU16)(pts[i * 2] + p->left + dx));
            pts[i * 2 + 1] = stars_signed_word((StarsU16)(pts[i * 2 + 1] + p->top + dy));
        }
        brush = stars_word(d, disabled ? 0x14U : 0x12U); stars_select(r, dc, brush);
        for (;;) {
            if (direction < 2) {
                step = direction == 0 ? -1 : 1; x = pts[2]; y = pts[3]; length = (StarsI16)(pts[4] - pts[2] + 1);
                if (type & 16U) stars_pat_blt(r, dc, (StarsU16)x, (StarsU16)pts[(count - 1) * 2 + 1], (StarsU16)length, 1, 0xf00021UL);
                for (; length > 0; length -= 2, ++x, y += step)
                    stars_pat_blt(r, dc, (StarsU16)x, (StarsU16)y, (StarsU16)length, 1, 0xf00021UL);
            } else {
                step = direction == 2 ? -1 : 1; x = pts[2]; y = pts[5]; length = (StarsI16)(pts[3] - pts[5] + 1);
                if (type & 16U) stars_pat_blt(r, dc, (StarsU16)pts[(count - 1) * 2], (StarsU16)y, 1, (StarsU16)length, 0xf00021UL);
                for (; length > 0; length -= 2, ++y, x += step)
                    stars_pat_blt(r, dc, (StarsU16)x, (StarsU16)y, 1, (StarsU16)length, 0xf00021UL);
            }
            if (!(type & 0x20U)) {
                rect = stars_rect((StarsU16)(pts[6] < pts[8] ? pts[6] : pts[8]),
                    (StarsU16)(pts[7] < pts[9] ? pts[7] : pts[9]),
                    (StarsU16)((pts[6] > pts[8] ? pts[6] : pts[8]) + 1),
                    (StarsU16)((pts[7] > pts[9] ? pts[7] : pts[9]) + 1));
                stars_fill_rect(r, dc, &rect, brush);
            }
            if (!disabled) break;
            for (i = 0; i < count * 2; ++i) --pts[i];
            disabled = 0; brush = stars_word(d, 0x16U); stars_select(r, dc, brush);
        }
    }
    stars_select(r, dc, old);
    if (text) {
        font = stars_select(r, dc, stars_word(d, 0x26b6U));
        mode = (StarsU16)stars_window(r, "SetBkMode", dc, 1, 0, 0, 0, 0);
        color = stars_window(r, "SetTextColor", dc, 0, 0, stars_dword(d, disabled ? 0x3eecU : 0x25f6U), 0, 0);
        rect = *p; if (down) stars_offset_rect(&rect, 1, 1);
        stars_rect_center_text(r, dc, &rect, text, 0);
        if (disabled) {
            stars_offset_rect(&rect, -1, -1); stars_window(r, "SetTextColor", dc, 0, 0, stars_dword(d, 0x23feU), 0, 0);
            stars_rect_center_text(r, dc, &rect, text, 0);
        }
        stars_window(r, "SetTextColor", dc, 0, 0, color, 0, 0);
        stars_window(r, "SetBkMode", dc, mode, 0, 0, 0, 0); stars_select(r, dc, font);
    }
}
