#include "ui_internal.h"

static StarsI16 multiply_divide(StarsRuntime *r, StarsI16 a, StarsI16 b, StarsI16 c)
{ return stars_signed_word((StarsU16)stars_window(r, "MulDiv", 0,
    (StarsU16)a, (StarsU16)b, (StarsU16)c, 0, 0)); }

StarsI16 stars_race_hit_test(StarsRuntime *r, StarsPoint point)
{
    StarsI16 i, count = stars_signed_word(stars_word(r->data, 0x4b94U));
    StarsRect rect;
    if (stars_word(r->data, 0x5320U)) return -1;
    for (i = 0; i < count; ++i) {
        rect = stars_race_rect(r, (StarsU16)i);
        if (stars_point_in_rect(&rect, point)) {
            /* For indices 15/16 this also reads the byte after rgEnvVarMax,
             * namely growth. Do not substitute a three-element bounds check. */
            if (stars_word(r->data, 0x2602U) == 4U && stars_signed_byte(r->data[0x4fb0U + i / 5]) < 0) return -1;
            return i;
        }
    }
    return -1;
}

StarsI16 stars_track_race2(StarsRuntime *r, StarsU16 hwnd, StarsPoint point, StarsU16 keyboard)
{
    StarsI16 hit = stars_race_hit_test(r, point), column, environment, half, center, low, high, delta, widen;
    StarsU16 type, text = 0;
    StarsRect rect;
    StarsButtonTrack track;
    StarsU8 *d = r->data;
    if (hit < 0) return 0;
    column = (StarsI16)(hit % 5); environment = (StarsI16)(hit / 5); rect = stars_race_rect(r, (StarsU16)hit);
    if (column == 1 && hit < 15) {
        stars_window(r, "SetCapture", hwnd, 0, 0, 0, 0, 0);
        stars_window(r, "SetCursor", stars_word(d, 0x5304U), 0, 0, 0, 0, 0);
        half = (StarsI16)((stars_signed_byte(d[0x4fb0U + environment]) - stars_signed_byte(d[0x4fadU + environment])) / 2);
        while (stars_get_mouse_move(r, &point)) {
            if (stars_signed_word(point.x) < stars_signed_word(rect.left)) point.x = rect.left;
            if (stars_signed_word(rect.right) < stars_signed_word(point.x)) point.x = rect.right;
            center = multiply_divide(r, stars_signed_word((StarsU16)(point.x - rect.left)), 100,
                stars_signed_word((StarsU16)(rect.right - rect.left)));
            if (center > 100 - half) center = (StarsI16)(100 - half);
            if (center < half) center = half;
            if (stars_signed_byte(d[0x4fb0U + environment]) != center + half) {
                d[0x4fadU + environment] = (StarsU8)(center - half);
                d[0x4fb0U + environment] = (StarsU8)(center + half);
                d[0x4faaU + environment] = (StarsU8)(stars_signed_byte(d[0x4fadU + environment]) +
                    (stars_signed_byte(d[0x4fb0U + environment]) - stars_signed_byte(d[0x4fadU + environment])) / 2);
                stars_draw_race2(r, hwnd, 0, (StarsU16)(1U << environment));
            }
        }
        stars_window(r, "ReleaseCapture", 0, 0, 0, 0, 0, 0);
        return 1;
    }
    delta = 0; widen = 0;
    if (hit == 15) { delta = 1; type = 0xa0U; }
    else if (hit == 16) { delta = -1; type = 0xa1U; }
    else if (column == 0) { delta = -1; type = 2; }
    else if (column == 2) { delta = 1; type = 3; }
    else if (column == 3) { widen = 1; type = 8; text = stars_word(d, 0x1304U); }
    else { widen = -1; type = 8; text = stars_word(d, 0x1306U); }
    /* High unused flag bits and initial tick bytes are irrelevant to a newly
     * initialized track, but initialize the host local to avoid C UB. */
    memset(&track, 0, sizeof(track));
    stars_init_button_track(r, &track, hwnd, 0, &rect, type, 80U, 0, 0, text);
    if (keyboard & 4U) { widen *= 10; delta *= 10; }
    while (stars_track_button(r, &track)) {
        if (hit >= 15) {
            center = stars_signed_byte((StarsU8)(d[0x4fb3U] + delta));
            if (center < 1) center = 1; if (center > 20) center = 20;
            if ((StarsU8)center != d[0x4fb3U]) { d[0x4fb3U] = (StarsU8)center; stars_draw_race2(r, hwnd, track.dc, 8); }
        } else {
            low = stars_signed_byte((StarsU8)(d[0x4fadU + environment] - widen + delta));
            high = stars_signed_byte((StarsU8)(d[0x4fb0U + environment] + widen + delta));
            if (high > 100) { low = stars_signed_byte((StarsU8)(low - high + 100)); high = 100; }
            if (low < 0) { high = high - low < 101 ? stars_signed_byte((StarsU8)(high - low)) : 100; low = 0; }
            if (high - low < 20) { half = (StarsI16)((20 - high + low) / 2); low -= half; high += half; }
            if (stars_signed_byte(d[0x4fadU + environment]) != low || stars_signed_byte(d[0x4fb0U + environment]) != high) {
                d[0x4fadU + environment] = (StarsU8)low; d[0x4fb0U + environment] = (StarsU8)high;
                d[0x4faaU + environment] = (StarsU8)(low + (high - low) / 2);
                stars_draw_race2(r, hwnd, track.dc, (StarsU16)(1U << environment));
            }
        }
    }
    if (hit < 15) d[0x4faaU + environment] = (StarsU8)(stars_signed_byte(d[0x4fadU + environment]) +
        (stars_signed_byte(d[0x4fb0U + environment]) - stars_signed_byte(d[0x4fadU + environment])) / 2);
    return 1;
}
StarsI16 stars_track_race3(StarsRuntime *r, StarsU16 hwnd, StarsPoint point, StarsU16 keyboard)
{
    StarsI16 hit = stars_race_hit_test(r, point), row, delta, old, value;
    StarsButtonTrack track;
    StarsRect rect;
    if (hit < 0) return 0;
    row = (StarsI16)(hit / 2); delta = (hit & 1) ? -1 : 1;
    rect = stars_race_rect(r, (StarsU16)hit); memset(&track, 0, sizeof(track));
    stars_init_button_track(r, &track, hwnd, 0, &rect, (hit & 1) ? 0xa1U : 0xa0U, 80U, 0, 0, 0);
    if (keyboard & 4U) delta *= 3;
    while (stars_track_button(r, &track)) {
        /* Verified CS:2A9C table = 0,1,2,3,4,5,6 for the seven rows. */
        old = stars_get_race_stat(r->data, 0x4f9aU, row);
        value = stars_set_race_stat(r->data, 0x4f9aU, row, (StarsI16)(old + delta));
        if (value != old) stars_draw_race3(r, hwnd, track.dc, row);
    }
    return 1;
}

void stars_draw_race2(StarsRuntime *r, StarsU16 hwnd, StarsU16 dc, StarsU16 draw)
{
    StarsU8 *d = r->data;
    StarsU16 own = !dc, disabled = stars_word(d, 0x5320U) ? 4U : 0;
    StarsU16 mode, i, row, column, immune, length, width, height, top = r->scratch_top, local;
    StarsRect client, rect, right, bottom, first, growth;
    StarsI16 center, low, high, x, span, dy = stars_signed_word(stars_word(d, 0x23faU));
    StarsU32 chance = 1, sum, odds;
    char format[100];
    if (!dc) dc = (StarsU16)stars_window(r, "GetDC", hwnd, 0, 0, 0, 0, 0);
    stars_window(r, "GetClientRect", hwnd, 0, 0, 0, &client, 0);
    stars_draw_advantage(r, dc, &client, 0);
    mode = (StarsU16)stars_window(r, "SetBkMode", dc, 1, 0, 0, 0, 0);
    stars_select(r, dc, stars_word(d, 0x26b6U));
    right = stars_race_rect(r, 2); first = stars_race_rect(r, 0); growth = stars_race_rect(r, 15);
    center = stars_signed_word((StarsU16)(stars_signed_word((StarsU16)(client.right - right.right)) / 2 + right.right));
    local = stars_scratch(r, 32U);
    for (row = 0; row < 3; ++row) if (draw & (1U << row)) {
        rect = stars_race_rect(r, (StarsU16)(row * 5U + 2U)); bottom = stars_race_rect(r, (StarsU16)(row * 5U + 4U));
        if (draw != 65535U) {
            stars_select(r, dc, stars_word(d, 0x10U)); stars_pat_blt(r, dc, right.right, rect.top,
                (StarsU16)(client.right - right.right), (StarsU16)(bottom.bottom - rect.top), 0xf00021UL);
        }
        if (stars_signed_byte(d[0x4fb0U + row]) < 0) {
            length = (StarsU16)stars_copy_game_string(d, r->strings, 0x534, local);
            stars_center_text(r, dc, center, stars_signed_word((StarsU16)(rect.top + 1U + dy)), local, (StarsI16)length);
        } else {
            stars_environment_string(r, (StarsI16)row, stars_signed_byte(d[0x4fadU + row]));
            stars_center_text(r, dc, center, stars_signed_word((StarsU16)(rect.top + 1U)), 0x57a4U, 0);
            length = (StarsU16)stars_copy_game_string(d, r->strings, 0x535, local);
            stars_center_text(r, dc, center, stars_signed_word((StarsU16)(rect.top + 1U + dy)), local, (StarsI16)length);
            stars_environment_string(r, (StarsI16)row, stars_signed_byte(d[0x4fb0U + row]));
            stars_center_text(r, dc, center, stars_signed_word((StarsU16)(rect.top + 1U + dy * 2)), 0x57a4U, 0);
        }
        if (draw == 65535U) {
            rect = stars_race_rect(r, (StarsU16)(row * 5U));
            stars_right_text(r, dc, stars_signed_word((StarsU16)(rect.left - 4U)),
                stars_signed_word((StarsU16)(rect.top + dy / 4)), stars_word(d, (StarsU16)(0x47eU + row * 2U)), 0, 0);
        }
    }
    for (i = 0; i < 15; ++i) {
        row = (StarsU16)(i / 5U); column = (StarsU16)(i % 5U); rect = stars_race_rect(r, i);
        immune = stars_signed_byte(d[0x4fb0U + row]) < 0 ? 4U : 0;
        if (column == 0 || column == 2) {
            if (draw & 0xfff0U) stars_draw_button(r, dc, &rect, (StarsU16)((column ? 3U : 2U) | immune | disabled), 0, 0);
        } else if (column == 1) {
            if (draw & (1U << row)) {
                stars_pat_blt(r, dc, rect.left, rect.top, (StarsU16)(rect.right - rect.left), (StarsU16)(rect.bottom - rect.top), 0x42U);
                if (!immune) {
                    low = stars_signed_byte(d[0x4fadU + row]); high = stars_signed_byte(d[0x4fb0U + row]);
                    width = (StarsU16)(rect.right - rect.left - 2U); height = (StarsU16)(rect.bottom - rect.top - 2U);
                    stars_select(r, dc, stars_word(d, (StarsU16)(0x5680U + row * 4U)));
                    x = multiply_divide(r, low, stars_signed_word(width), 100);
                    span = multiply_divide(r, (StarsI16)(high - low), stars_signed_word(width), 100);
                    stars_pat_blt(r, dc, (StarsU16)(rect.left + x + 1U), (StarsU16)(rect.top + 1U), (StarsU16)span, height, 0xf00021UL);
                }
            }
        } else if (draw & 0xfff0U) stars_draw_button(r, dc, &rect, (StarsU16)(8U | immune | disabled), 0, column == 3 ? 0x133bU : 0x1345U);
    }
    if (draw & 8U) {
        if (draw == 65535U) {
            length = (StarsU16)stars_copy_game_string(d, r->strings, 0xf5, 0x57a4U);
            stars_text_out(r, dc, first.left, (StarsU16)(growth.top + 3U), d + 0x57a4U, length);
            stars_draw_button(r, dc, &growth, (StarsU16)(disabled | 0xa0U), 0, 0);
            rect = stars_race_rect(r, 16); stars_draw_button(r, dc, &rect, (StarsU16)(disabled | 0xa1U), 0, 0);
        } else {
            width = (StarsU16)stars_text_extent(r, dc, d + 0x134fU, 3);
            stars_select(r, dc, stars_word(d, 0x10U));
            stars_pat_blt(r, dc, (StarsU16)(growth.left - 4U - width), (StarsU16)(growth.top + 3U), width, (StarsU16)dy, 0xf00021UL);
        }
        length = (StarsU16)sprintf((char *)d + 0x57a4U, (char *)d + stars_word(d, 0x1b8U), (int)stars_signed_byte(d[0x4fb3U]));
        stars_right_text(r, dc, stars_signed_word((StarsU16)(growth.left - 4U)),
            stars_signed_word((StarsU16)(growth.top + 3U)), 0x57a4U, (StarsI16)length, 0);
    }
    if (draw & 7U) {
        for (row = 0; row < 3; ++row) {
            low = stars_signed_byte(d[0x4fadU + row]); high = stars_signed_byte(d[0x4fb0U + row]);
            if (high < 0 || high - low == 100) chance *= 100U;
            else if (row == 2) chance *= (StarsU32)(StarsI32)(high - low);
            else {
                sum = 0;
                for (x = low; x <= high; ++x) sum += (StarsU32)(StarsI32)(x < 10 ? x : x < 90 ? 10 : 100 - x);
                chance = chance * sum / 9U;
            }
        }
        if (!chance || (chance & 0x80000000UL)) chance = 1;
        odds = (1000000UL + (chance >> 1)) / chance;
        width = chance == 1000000UL ? 0 : (StarsU16)odds;
        if (width != stars_word(d, 0x2608U)) {
            stars_put_word(d, 0x2608U, width);
            if (odds == 1) length = (StarsU16)stars_copy_game_string(d, r->strings, chance == 1000000UL ? 0x188 : 0x189, 0x57a4U);
            else {
                strcpy(format, (char *)d + stars_game_string(r, 0x187));
                length = (StarsU16)sprintf((char *)d + 0x57a4U, format, (long)odds);
            }
            rect = stars_rect(first.left, (StarsU16)(growth.top + dy + 8U), (StarsU16)(growth.right + 20U),
                (StarsU16)(growth.top + dy * 4 + 8U));
            stars_select(r, dc, stars_word(d, 0x10U));
            stars_pat_blt(r, dc, rect.left, rect.top, (StarsU16)(rect.right - rect.left), (StarsU16)(rect.bottom - rect.top), 0xf00021UL);
            stars_window(r, "DrawText", dc, length, 0x10U, 0, d + 0x57a4U, &rect);
        }
    }
    stars_window(r, "SetBkMode", dc, mode, 0, 0, 0, 0);
    if (own) stars_window(r, "ReleaseDC", hwnd, dc, 0, 0, 0, 0);
    r->scratch_top = top;
}

void stars_draw_race3(StarsRuntime *r, StarsU16 hwnd, StarsU16 dc, StarsI16 draw)
{
    /* CS:2A8C/2A94/2A9C are inline byte tables, not DGROUP arrays. */
    static const StarsI16 rows[7] = {4,3,3,3,3,3,3};
    static const StarsI16 digits[7] = {-2,2,2,2,-2,2,2};
    StarsU8 *d = r->data;
    StarsU16 own = !dc, disabled = stars_word(d, 0x5320U) ? 4U : 0, mode, y, length, i, child, count = 0;
    StarsI16 alternate = stars_get_race_stat(d, 0x4f9aU, 14) == 8, id = 0xf6, label, digit_width, unit_width, x, width;
    StarsI16 dy = stars_signed_word(stars_word(d, 0x23faU));
    StarsU32 background;
    StarsRect client, rect, second;
    if (!dc) dc = (StarsU16)stars_window(r, "GetDC", hwnd, 0, 0, 0, 0, 0);
    stars_window(r, "GetClientRect", hwnd, 0, 0, 0, &client, 0);
    stars_draw_advantage(r, dc, &client, 0);
    mode = (StarsU16)stars_window(r, "SetBkMode", dc, 2, 0, 0, 0, 0);
    background = stars_window(r, "SetBkColor", dc, 0, 0, stars_dword(d, 0x22baU), 0, 0);
    stars_window(r, "SetTextColor", dc, 0, 0, stars_dword(d, 0x48a6U), 0, 0);
    stars_select(r, dc, stars_word(d, 0x26b6U)); y = (StarsU16)(dy * 3 + 6);
    digit_width = stars_signed_word((StarsU16)stars_text_extent(r, dc, d + 0x1353U, 1));
    unit_width = stars_signed_word((StarsU16)stars_text_extent(r, dc, d + 0x1355U, 2));
    for (i = 0; i < 7; ++i) {
        if (i == 1 && alternate) { stars_window(r, "SetTextColor", dc, 0, 0, stars_dword(d, 0x23feU), 0, 0); disabled = 4; }
        if (i == 4) {
            if (draw == -1) { child = stars_control(r, hwnd, 0x123U); stars_window_position(r, child, 6, y,
                (StarsU16)(client.right - 12U), (StarsU16)(dy * 3 / 2), 4); }
            y = (StarsU16)(y + dy * 5 / 2);
        }
        label = alternate && id == 0xf6 ? 0x104 : id; ++id;
        length = (StarsU16)stars_copy_game_string(d, r->strings, label, 0x57a4U);
        if (draw == -1) stars_text_out(r, dc, 6, y, d + 0x57a4U, length);
        x = stars_signed_word((StarsU16)(stars_text_extent(r, dc, d + 0x57a4U, length) + 6U));
        width = (StarsI16)((digits[i] < 0 ? -digits[i] : digits[i]) * digit_width);
        sprintf((char *)d + 0x57a4U, (char *)d + stars_word(d, 0x1aaU), (int)stars_get_race_stat(d, 0x4f9aU, (StarsI16)i));
        if (digits[i] < 0 && (i || !alternate)) {
            width += unit_width; strcat((char *)d + 0x57a4U, (char *)d + (i ? 0x135bU : 0x1358U));
        }
        x += width;
        if (draw == -1 || draw == (StarsI16)i) stars_right_text(r, dc, x, stars_signed_word(y), 0x57a4U, 0, width);
        rect = stars_rect((StarsU16)(x + 4), (StarsU16)(y - 3U), (StarsU16)(x + 19), (StarsU16)(y + stars_half((StarsU16)dy)));
        second = rect; stars_offset_rect(&second, 0, stars_signed_word((StarsU16)(rect.bottom - rect.top - 1U)));
        stars_set_race_rect(r, count, rect); stars_set_race_rect(r, (StarsU16)(count + 1U), second);
        if (draw == -1) {
            stars_draw_button(r, dc, &rect, (StarsU16)(disabled | 0xa0U), 0, 0);
            stars_draw_button(r, dc, &second, (StarsU16)(disabled | 0xa1U), 0, 0);
            label = alternate && id == 0xf7 ? 0x105 : id;
            length = (StarsU16)stars_copy_game_string(d, r->strings, label, 0x57a4U);
            stars_text_out(r, dc, (StarsU16)(rect.right + 4U), y, d + 0x57a4U, length);
        }
        ++id; count += 2U; y = (StarsU16)(y + rows[i] * dy / 2);
    }
    stars_put_word(d, 0x4b94U, alternate ? 2U : count);
    stars_window(r, "SetBkColor", dc, 0, 0, background, 0, 0);
    stars_window(r, "SetBkMode", dc, mode, 0, 0, 0, 0);
    if (own) stars_window(r, "ReleaseDC", hwnd, dc, 0, 0, 0, 0);
}
