#include "title.h"
#include "ui_internal.h"

/* NE 1:1020-124c. The original accepts a minus sign anywhere in each four
 * character field, preserves upper flags, and defaults only the main frame
 * to maximized. Width/height occupy RECT.right/bottom in this saved format. */
void stars_ini_window_rect(StarsRuntime *r, StarsU16 section, StarsU16 file, StarsI16 id, StarsU16 out)
{
    StarsU16 top = r->scratch_top, key = stars_scratch(r, 16U), n, flags, i, j, value;
    StarsU16 fields[4];
    StarsU8 *d = r->data, c;
    StarsI16 negative;
    StarsProfileString profile;
    stars_copy_game_string(d, r->strings, id, key);
    profile.section = d + section; profile.key = d + key;
    profile.fallback = d + 0x738U; profile.file = d + file;
    n = (StarsU16)stars_window(r, "GetPrivateProfileString", 0, 20, 0, 0, d + 0x57a4U, &profile);
    if (n != 17 || (d[0x57a4U] != 'M' && d[0x57a4U] != 'R' && d[0x57a4U] != 'I')) goto fallback;
    for (i = 0; i < 4; ++i) {
        value = 0; negative = 0;
        for (j = 0; j < 4; ++j) {
            c = d[0x57a5U + i * 4U + j];
            if (c == '-') negative = 1;
            else if (c < '0' || c > '9') goto fallback;
            else value = (StarsU16)(value * 10U + c - '0');
        }
        fields[i] = negative ? (StarsU16)(0U - value) : value;
    }
    flags = (StarsU16)(4U | (d[0x57a4U] == 'M' ? 1U : d[0x57a4U] == 'I' ? 2U : 0U));
    goto store;
fallback:
    fields[0] = fields[2] = 32768U; fields[1] = fields[3] = 0;
    flags = id == 0x94 ? 1U : 0U;
store:
    for (i = 0; i < 4; ++i) stars_put_word(d, (StarsU16)(out + i * 2U), fields[i]);
    stars_put_word(d, (StarsU16)(out + 8U), (StarsU16)((stars_word(d, (StarsU16)(out + 8U)) & 0xfff8U) | flags));
    r->scratch_top = top;
}

/* NE 8:5fe2-627f. CS:5f7e..5fdf are literal tables, not player data despite
 * the raw decompiler's misleading labels. This routine does not clear the
 * background; the window class supplies its black background brush. */
void stars_draw_stars(StarsRuntime *r, StarsU16 dc, const StarsRect *source, StarsU32 stack_seed)
{
    static const StarsI32 small_density[5] = {28000L,28000L,63000L,95000L,73000L};
    static const StarsU32 small_color[5] = {0x7f7f7fUL,0xffffffUL,0xffUL,0xff00UL,0xff0000UL};
    static const StarsI32 large_density[4] = {80000L,210000L,310000L,260000L};
    static const StarsU32 large_color[4] = {0x7f7f7fUL,0x7fUL,0x7f00UL,0x7f0000UL};
    static const StarsU32 cross_color[4] = {0xc0c0c0UL,0xffUL,0xff00UL,0xff0000UL};
    StarsRect area = *source, star;
    StarsSeeds seeds;
    StarsI16 width, height, count, i, pass, layer;
    StarsI32 pixels;
    stars_push_random(r->data, 0x000b0011UL, stack_seed);
    stars_window(r, "InflateRect", 0, 65533U, 65533U, 0, &area, 0);
    width = stars_signed_word((StarsU16)(area.right - area.left));
    height = stars_signed_word((StarsU16)(area.bottom - area.top));
    pixels = (StarsI32)width * height;
    seeds.first = stars_dword(r->data, 0x1370U); seeds.second = stars_dword(r->data, 0x1374U);
    for (layer = 0; layer < 2; ++layer) {
        for (pass = 0; pass < (layer ? 4 : 5); ++pass) {
            count = stars_signed_word((StarsU16)(pixels / (layer ? large_density[pass] : small_density[pass]) + layer));
            for (i = 0; i < count; ++i) {
                star.left = (StarsU16)(stars_random(&seeds, width) + area.left);
                star.top = (StarsU16)(stars_random(&seeds, height) + area.top);
                stars_put_dword(r->data, 0x1370U, seeds.first); stars_put_dword(r->data, 0x1374U, seeds.second);
                star.right = (StarsU16)(star.left + (layer ? 3U : 1U));
                star.bottom = (StarsU16)(star.top + (layer ? 3U : 1U));
                stars_window(r, "SetBkColor", dc, 0, 0, layer ? large_color[pass] : small_color[pass], 0, 0);
                stars_window(r, "ExtTextOut", dc, 0, 0, 0x20000UL, 0, &star);
                if (layer) {
                    stars_window(r, "SetBkColor", dc, 0, 0, cross_color[pass], 0, 0);
                    stars_window(r, "InflateRect", 0, 65535U, 0, 0, &star, 0);
                    stars_window(r, "ExtTextOut", dc, 0, 0, 0x20000UL, 0, &star);
                    stars_window(r, "InflateRect", 0, 1, 65535U, 0, &star, 0);
                    stars_window(r, "ExtTextOut", dc, 0, 0, 0x20000UL, 0, &star);
                }
            }
        }
    }
    stars_pop_random(r->data);
}

static StarsI16 product_divide(StarsU16 a, StarsU16 b, StarsI16 divisor)
{ return (StarsI16)(stars_signed_word((StarsU16)((StarsU32)a * b)) / divisor); }

/* TitleWndProc WM_CREATE, NE 5:9135-92e3. */
void stars_title_create(StarsRuntime *r, StarsU16 hwnd)
{
    StarsU8 *d = r->data;
    StarsRect client;
    StarsCreateWindow request;
    StarsI16 width, height, x, gap;
    StarsU16 i, button, text;
    if (stars_signed_word(stars_word(d, 0x5312U)) > 7) {
        stars_put_word(d, 0x430U, stars_load_big_resource(r, 449));
        if (!stars_word(d, 0x42eU) && stars_word(d, 0x430U))
            stars_put_word(d, 0x42eU, stars_palette_from_dib(r, stars_word(d, 0x430U)));
    }
    stars_window(r, "GetClientRect", hwnd, 0, 0, 0, &client, 0);
    width = stars_half((StarsU16)stars_half((StarsU16)stars_half(client.right)));
    if (width < 120) width = 120;
    if (stars_signed_word(client.bottom) < 650) width = (StarsI16)(width + width / 6);
    gap = stars_half((StarsU16)stars_half((StarsU16)(client.right - width * 4U)));
    x = stars_half((StarsU16)gap);
    height = stars_signed_word(client.bottom) <= 500 ? stars_signed_word((StarsU16)(stars_word(d, 0x23faU) * 2U)) :
        product_divide(stars_word(d, 0x23faU), 5, 2);
    request.class_name = d + 0x43aU; request.instance = stars_word(d, 0x5310U);
    for (i = 0; i < 4; ++i) {
        text = stars_game_string(r, (StarsI16)(0x1dfU + i));
        request.text = d + text; request.menu = i;
        request.geometry = stars_rect((StarsU16)x,
            (StarsU16)(client.bottom - height - product_divide(stars_word(d, 0x23faU), 5, 2)),
            (StarsU16)width, (StarsU16)height);
        button = (StarsU16)stars_window(r, "CreateWindow", hwnd, 0, 0, 0x50000000UL, &request, 0);
        stars_put_word(d, (StarsU16)(0x432U + i * 2U), button);
        if (i == 2 && (!d[0x56a2U] || r->platform->access(r->context, d + 0x56a2U, 0) == -1))
            stars_window(r, "EnableWindow", button, 0, 0, 0, 0, 0);
        if (stars_signed_word(client.bottom) < 500)
            stars_window(r, "SendMessage", button, 0x30U, stars_word(d, 0x26b6U), 0, 0, 0);
        x = stars_signed_word((StarsU16)(x + width + gap));
    }
}

/* TitleWndProc WM_PAINT, NE 5:956e-97e2. */
void stars_title_paint(StarsRuntime *r, StarsU16 hwnd, StarsU32 stack_seed)
{
    StarsU8 *d = r->data, paint[32];
    StarsRect client, button, logo;
    StarsU16 dc, lf, font, old;
    StarsI16 width, height;
    if (stars_window(r, "IsIconic", hwnd, 0, 0, 0, 0, 0)) {
        dc = (StarsU16)stars_window(r, "BeginPaint", hwnd, 0, 0, 0, paint, 0);
        stars_window(r, "DrawIcon", dc, 2, 2, stars_word(d, 0x356U), 0, 0);
        stars_window(r, "EndPaint", hwnd, 0, 0, 0, paint, 0);
        return;
    }
    lf = (StarsU16)stars_window(r, "LocalAlloc", 0, 0x40U, 50, 0, 0, 0);
    dc = (StarsU16)stars_window(r, "BeginPaint", hwnd, 0, 0, 0, paint, 0);
    stars_select(r, dc, stars_word(d, 0x10U));
    stars_window(r, "GetClientRect", hwnd, 0, 0, 0, &client, 0);
    if (stars_signed_word(stars_word(d, 0x5312U)) >= 8)
        stars_window(r, "SetTextColor", dc, 0, 0, 0x02ff7fffUL, 0, 0);
    if (stars_signed_word(stars_word(d, 0x5312U)) < 8 || !stars_word(d, 0x430U)) {
        stars_draw_stars(r, dc, &client, stack_seed);
        stars_put_word(d, lf, (StarsU16)(stars_signed_word((StarsU16)(0U - client.bottom)) / 3));
        strcpy((char *)d + lf + 18U, (char *)d + 0x706U);
        font = (StarsU16)stars_window(r, "CreateFontIndirect", 0, 0, 0, 0, d + lf, 0);
        stars_window(r, "SetTextColor", dc, 0, 0, 0x009b009bUL, 0, 0);
        if (font) {
            old = stars_select(r, dc, font);
            stars_window(r, "SetBkMode", dc, 1, 0, 0, 0, 0);
            logo = client; logo.bottom = (StarsU16)product_divide(logo.bottom, 3, 4);
            stars_rect_center_text(r, dc, &logo, 0x441U, 6);
            stars_select(r, dc, old); stars_window(r, "DeleteObject", font, 0, 0, 0, 0, 0);
        }
    } else {
        stars_window(r, "SelectPalette", dc, stars_word(d, 0x42eU), 0, 0, 0, 0);
        stars_window(r, "RealizePalette", dc, 0, 0, 0, 0, 0);
        height = (StarsI16)stars_window(r, "GetSystemMetrics", 0, 1, 0, 0, 0, 0);
        width = (StarsI16)stars_window(r, "GetSystemMetrics", 0, 0, 0, 0, 0, 0);
        stars_dib_blt(r, dc, 0, 0, width, height, stars_word(d, 0x430U), 0, 0, 800, 600, 0x00cc0020UL);
    }
    stars_select(r, dc, stars_word(d, 0x4842U));
    stars_window(r, "SetBkMode", dc, 1, 0, 0, 0, 0);
    stars_version_string(d, r->strings);
    stars_window(r, "GetWindowRect", stars_word(d, 0x432U), 0, 0, 0, &button, 0);
    client.top = (StarsU16)(button.top - product_divide(stars_word(d, 0x23faU), 9, 2));
    client.bottom = (StarsU16)(client.top + product_divide(stars_word(d, 0x23faU), 3, 2));
    stars_rect_center_text(r, dc, &client, 0x57a4U, (StarsI16)strlen((char *)d + 0x57a4U));
    stars_window(r, "EndPaint", hwnd, 0, 0, 0, paint, 0);
    stars_window(r, "LocalFree", lf, 0, 0, 0, 0, 0);
}

StarsI16 stars_title_palette(StarsRuntime *r, StarsU16 hwnd, StarsU16 msg, StarsU16 wp)
{
    StarsU16 dc, old, changed;
    if (msg == 0x311U && wp == hwnd) return 0;
    if (stars_signed_word(stars_word(r->data, 0x5312U)) < 8) return 0;
    dc = (StarsU16)stars_window(r, "GetDC", hwnd, 0, 0, 0, 0, 0);
    old = (StarsU16)stars_window(r, "SelectPalette", dc, stars_word(r->data, 0x42eU), 0, 0, 0, 0);
    changed = (StarsU16)stars_window(r, "RealizePalette", dc, 0, 0, 0, 0, 0);
    stars_window(r, "SelectPalette", dc, old, 0, 0, 0, 0);
    stars_window(r, "ReleaseDC", hwnd, dc, 0, 0, 0, 0);
    if (changed) stars_window(r, "InvalidateRect", hwnd, 1, 0, 0, 0, 0);
    return changed ? 1 : 0;
}

void stars_title_release(StarsRuntime *r)
{
    StarsU16 dib = stars_word(r->data, 0x430U);
    if (dib) {
        stars_window(r, "GlobalUnlock", dib, 0, 0, 0, 0, 0);
        stars_window(r, "FreeResource", dib, 0, 0, 0, 0, 0);
        stars_put_word(r->data, 0x430U, 0);
    }
}

void stars_title_read_fonts(StarsRuntime *r, const StarsU8 *file)
{
    StarsU16 top = r->scratch_top, section = stars_scratch(r, 80), key = stars_scratch(r, 80), i, n;
    StarsProfileString profile;
    stars_copy_game_string(r->data, r->strings, 0xc4, section);
    profile.section = r->data + section; profile.file = file;
    profile.key = r->data + key; profile.fallback = r->data + 0x749U;
    for (i = 0; i < 4; ++i) {
        stars_copy_game_string(r->data, r->strings, (StarsI16)(0xc5U + i), key);
        stars_window(r, "GetPrivateProfileString", 0, 80, 0, 0, r->data + 0x57a4U, &profile);
        n = (StarsU16)strlen((char *)r->data + 0x57a4U);
        if (n >= 5 && n < 32) strcpy((char *)r->data + 0x6a6U + i * 32U, (char *)r->data + 0x57a4U);
    }
    r->scratch_top = top;
}
