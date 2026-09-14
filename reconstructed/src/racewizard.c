/* Dialog orchestration and six original Windows-message procedures. */
#include "ui_internal.h"

static void sticky(StarsRuntime *r, StarsU16 hwnd, StarsI16 init)
{
    StarsPoint point;
    point.x = stars_word(r->data, 0x12ecU); point.y = stars_word(r->data, 0x12eeU);
    stars_sticky_dialog(r, hwnd, &point, init);
    stars_put_word(r->data, 0x12ecU, point.x); stars_put_word(r->data, 0x12eeU, point.y);
}
static int navigation(StarsRuntime *r, StarsU16 command)
{
    int i;
    for (i = 0; i < 4; ++i) if (command == stars_word(r->data, (StarsU16)(0x12e2U + i * 2U))) return i;
    return -1;
}
static void disable(StarsRuntime *r, StarsU16 hwnd, StarsU16 id)
{ StarsU16 control = stars_control(r, hwnd, id); stars_window(r, "EnableWindow", control, 0, 0, 0, 0, 0); }
static void help(StarsRuntime *r, StarsU16 hwnd, StarsU16 topic)
{ stars_window(r, "WinHelp", hwnd, 1, 0, topic, r->data + stars_word(r->data, 0x14aU), 0); }
static StarsU32 send(StarsRuntime *r, StarsU16 control, StarsU16 message, StarsU16 wp, const StarsU8 *text)
{ return stars_window(r, "SendMessage", control, message, wp, 0, (void *)text, 0); }
static void set_text(StarsRuntime *r, StarsU16 hwnd, StarsU16 id, StarsU16 text)
{ stars_window(r, "SetDlgItemText", hwnd, id, 0, 0, r->data + text, 0); }
static void get_text(StarsRuntime *r, StarsU16 hwnd, StarsU16 id, StarsU16 text, StarsU16 size)
{ stars_window(r, "GetDlgItemText", hwnd, id, size, 0, r->data + text, 0); }
static void radio(StarsRuntime *r, StarsU16 hwnd, StarsU16 first, StarsU16 last, StarsU16 choice)
{ stars_window(r, "CheckRadioButton", hwnd, first, last, choice, 0, 0); }
static StarsU16 checked_race(StarsRuntime *r, StarsU16 hwnd)
{
    StarsU16 id;
    for (id = 0x10fU; id < 0x117U; ++id)
        if (stars_window(r, "IsDlgButtonChecked", hwnd, id, 0, 0, 0, 0)) break;
    return id;
}
static StarsI16 dialog_background(StarsRuntime *r, StarsU16 hwnd, StarsU16 msg, StarsU16 wp,
    StarsU32 lp, StarsU16 first, StarsU16 end, StarsU16 extra)
{
    StarsRect rect;
    StarsU16 i;
    if (msg == 0x14U) {
        stars_window(r, "GetClientRect", hwnd, 0, 0, 0, &rect, 0);
        stars_fill_rect(r, wp, &rect, stars_word(r->data, 0x10U)); return 1;
    }
    if (msg == 0x19U) {
        for (i = first; i < end; ++i) if (stars_control(r, hwnd, i) == (StarsU16)lp) break;
        if (i < end || (extra && stars_control(r, hwnd, extra) == (StarsU16)lp) || (StarsU16)(lp >> 16) == 6U) {
            stars_window(r, "SetBkColor", wp, 0, 0, stars_dword(r->data, 0x22baU), 0, 0);
            return stars_signed_word(stars_word(r->data, 0x10U));
        }
    }
    return 0;
}
static StarsRect controls_frame(StarsRuntime *r, StarsU16 hwnd, StarsU16 first, StarsU16 last)
{
    StarsRect a, b;
    StarsU16 child = stars_control(r, hwnd, first);
    stars_window(r, "GetWindowRect", child, 0, 0, 0, &a, 0); stars_screen_point(r, hwnd, &a.left);
    child = stars_control(r, hwnd, last);
    stars_window(r, "GetWindowRect", child, 0, 0, 0, &b, 0); stars_screen_point(r, hwnd, &b.right);
    a.right = b.right; a.bottom = b.bottom; return a;
}
static void frame_label(StarsRuntime *r, StarsU16 dc, StarsRect *rect, StarsI16 id)
{
    StarsU16 length = (StarsU16)stars_copy_game_string(r->data, r->strings, id, 0x57a4U);
    stars_text_out(r, dc, (StarsU16)(rect->left + 8U),
        (StarsU16)(rect->top - stars_half(stars_word(r->data, 0x23faU))), r->data + 0x57a4U, length);
}
static void finish_dialog(StarsRuntime *r, StarsU16 hwnd, int result)
{ sticky(r, hwnd, 0); stars_window(r, "EndDialog", hwnd, (StarsU16)result, 0, 0, 0, 0); }

StarsI16 stars_race_wizard(StarsRuntime *r, StarsU16 owner, StarsI16 read_only, StarsI16 dont_write)
{
    static const StarsU16 resources[6] = {0x92,0x95,0x96,0x93,0x94,0x97};
    static StarsDialogProc const procedures[6] = {stars_race_dialog1,stars_race_dialog4,stars_race_dialog5,
        stars_race_dialog2,stars_race_dialog3,stars_race_dialog6};
    StarsU16 top = r->scratch_top, rects = stars_scratch(r, 146U);
    StarsI16 result, points;
    int page = 0;
    StarsU8 *d = r->data;
    (void)dont_write; /* No load of BP+0A exists in this executable. */
    stars_put_word(d, 0x48acU, rects); stars_put_word(d, 0x5320U, (StarsU16)read_only);
    stars_put_word(d, 0x48aaU, owner);
    for (;;) {
        stars_put_word(d, 0x2602U, (StarsU16)(page + 1));
        result = r->platform->dialog(r->context, r, resources[page], stars_word(d, 0x48aaU), procedures[page]);
        if (!result) { r->scratch_top = top; return 0; }
        if (result == 1 && page > 0) { --page; continue; }
        if (result != 3 && page < 5) { ++page; continue; }
        if (stars_word(d, 0x5320U)) { r->scratch_top = top; return 0; }
        points = stars_race_advantage_points(d, 0x4f9aU);
        if (points < 0) {
            sprintf((char *)d + 0x57a4U, (char *)d + stars_game_string(r, 0x515),
                (int)stars_signed_word((StarsU16)(0U - (StarsU16)points)));
            stars_runtime_alert(r, 0x57a4U, 16);
            /* Original switches on the global, allowing a modal callback to
             * change which page is resumed. */
            result = stars_signed_word(stars_word(d, 0x2602U)); page = result >= 2 && result <= 6 ? result - 1 : 0;
            continue;
        }
        stars_put_dword(d, 0x35cU, stars_password_salt(d + 0x1308U));
        stars_put_dword(d, 0x360U, 0xfffffffbUL);
        if (stars_check_password(r) && stars_save_race(r, d[0x1318U] ? 0x1318U : 0x1328U, 0x4f9aU)) {
            r->scratch_top = top; return 1;
        }
        page = 0;
    }
}

/* NE 5:3243-3269, reached by CommandHandler(hwnd, 0x81). Existing password
 * and file-name buffers are deliberately retained, just as in the caller. */
StarsI16 stars_new_race(StarsRuntime *r, StarsU16 owner)
{
    memcpy(r->data + 0x4f9aU, r->data + 0xda2U, 192U);
    return stars_race_wizard(r, owner, 0, 0);
}

StarsI16 stars_race_dialog1(StarsRuntime *r, StarsU16 hwnd, StarsU16 msg, StarsU16 wp, StarsU32 lp)
{
    StarsU8 *d = r->data, paint[32], unused[32];
    StarsU16 i, id, child, dc, player, icon, value, flags, name;
    StarsRect rect, frame, spin;
    StarsPoint point;
    StarsButtonTrack track;
    StarsI16 dy = stars_signed_word(stars_word(d, 0x23faU)), direction;
    int nav;
    if (msg == 0xfU) {
        dc = (StarsU16)stars_window(r, "BeginPaint", hwnd, 0, 0, 0, paint, 0);
        id = checked_race(r, hwnd); player = id < 0x116U ? (StarsU16)(0xda2U + (id - 0x10fU) * 192U) : 0x4f9aU;
        stars_window(r, "GetClientRect", hwnd, 0, 0, 0, &rect, 0); stars_draw_advantage(r, dc, &rect, player);
        frame = controls_frame(r, hwnd, 0x10fU, 0x116U); stars_expand_rect(&frame, dy, stars_half((StarsU16)dy));
        stars_draw_frame(r, dc, &frame, -1); stars_select(r, dc, stars_word(d, 0x26b6U));
        stars_window(r, "SetBkColor", dc, 0, 0, stars_dword(d, 0x22baU), 0, 0); frame_label(r, dc, &frame, 0x533);
        child = stars_control(r, hwnd, 0x81aU); stars_window(r, "GetWindowRect", child, 0, 0, 0, &frame, 0);
        stars_screen_point(r, hwnd, &frame.right); icon = (StarsU16)((stars_word(d, 0x4fa0U) >> 3) & 31U);
        stars_window(r, "SelectPalette", dc, stars_word(d, 0x5314U), 0, 0, 0, 0);
        stars_window(r, "RealizePalette", dc, 0, 0, 0, 0, 0);
        stars_dib_blt(r, dc, stars_signed_word((StarsU16)(frame.right + 32U)),
            stars_signed_word((StarsU16)(frame.bottom - 32U)), 32, 32, stars_word(d, 0x5476U),
            (StarsI16)((icon & 7U) * 32U), (StarsI16)((3U - icon / 8U) * 32U), 32, 32, 0xcc0020UL);
        if (!stars_word(d, 0x5320U)) {
            spin = stars_rect((StarsU16)(frame.right + 34U), (StarsU16)(frame.bottom + 3U),
                (StarsU16)(frame.right + 48U), (StarsU16)(frame.bottom + 17U));
            stars_write_rect(d, 0x5926U, &spin); stars_offset_rect(&spin, 14, 0); stars_write_rect(d, 0x592eU, &spin);
            for (i = 0; i < 2; ++i) { spin = stars_read_rect(d, (StarsU16)(0x5926U + i * 8U));
                stars_draw_button(r, dc, &spin, i ? 0x23U : 0x22U, 0, 0); }
        }
        stars_window(r, "EndPaint", hwnd, 0, 0, 0, paint, 0); return 1;
    }
    if (msg == 0x14U || msg == 0x19U) return dialog_background(r, hwnd, msg, wp, lp, 0x10fU, 0x117U, 0);
    if (msg == 0x110U) {
        icon = (StarsU16)((stars_word(d, 0x4fa0U) >> 3) & 31U);
        stars_race_title(r, hwnd, stars_signed_word(stars_word(d, 0x2602U)));
        set_text(r, hwnd, 0x10cU, 0x501aU); set_text(r, hwnd, 0x81bU, 0x503aU);
        if (!d[0x501aU]) { get_text(r, hwnd, 0x10fU, 0x501aU, 16); set_text(r, hwnd, 0x10cU, 0x501aU); }
        sticky(r, hwnd, 1);
        if (!(stars_word(d, 0x80U) & 8U) || stars_word(d, 0x18cU) || !stars_word(d, 0x5320U)) {
            for (i = 0; i < 7; ++i) {
                flags = stars_word(d, (StarsU16)(0xda8U + i * 192U));
                stars_put_word(d, 0x4fa0U, (StarsU16)((stars_word(d, 0x4fa0U) & 0xff07U) | (flags & 0xf8U)));
                if (!memcmp(d + 0x4f9aU, d + 0xda2U + i * 192U, 128U)) break;
            }
            stars_put_word(d, 0x4fa0U, (StarsU16)((stars_word(d, 0x4fa0U) & 0xff07U) | (icon << 3)));
        } else i = 0;
        radio(r, hwnd, 0x10fU, 0x116U, (StarsU16)(i + 0x10fU));
        stars_send_item(r, hwnd, 0x10cU, 0x415U, 15, 0); stars_send_item(r, hwnd, 0x81bU, 0x415U, 15, 0);
        stars_send_item(r, hwnd, 0x10dU, 0x415U, 16, 0);
        child = stars_control(r, hwnd, 0x81aU);
        for (i = 0x106U; i < 0x10bU; ++i) send(r, child, 0x403U, 0, d + stars_game_string(r, (StarsI16)i));
        send(r, child, 0x40eU, (StarsU16)stars_get_race_stat(d, 0x4f9aU, 7), 0);
        if (stars_dword(d, 0x4fa6U)) set_text(r, hwnd, 0x10dU, 0x1308U);
        if (stars_word(d, 0x5320U)) {
            for (i = 0x10cU; i < 0x10eU; ++i) disable(r, hwnd, i);
            disable(r, hwnd, 0x81bU); for (i = 0x10fU; i < 0x117U; ++i) disable(r, hwnd, i);
            stars_window(r, "EnableWindow", child, 0, 0, 0, 0, 0);
        }
        return 1;
    }
    if (msg == 0x111U) {
        if (wp == 0x76U) { help(r, hwnd, 0x3ffU); return 1; }
        nav = navigation(r, wp);
        if (nav >= 0) {
            if (wp != 2U) {
                icon = (StarsU16)((stars_word(d, 0x4fa0U) >> 3) & 31U); id = checked_race(r, hwnd);
                if (id < 0x116U) memcpy(d + 0x4f9aU, d + 0xda2U + (id - 0x10fU) * 192U, 192U);
                /* The original does NOT clear name tails here. Preserve U020. */
                get_text(r, hwnd, 0x10cU, 0x501aU, 32); get_text(r, hwnd, 0x81bU, 0x503aU, 32);
                get_text(r, hwnd, 0x10dU, 0x1308U, 16); child = stars_control(r, hwnd, 0x81aU);
                value = (StarsU16)send(r, child, 0x407U, 0, 0);
                stars_set_race_stat(d, 0x4f9aU, 7, stars_signed_word(value));
                stars_put_dword(d, 0x4fa6U, stars_password_salt(d + 0x1308U));
                stars_put_word(d, 0x4fa0U, (StarsU16)((stars_word(d, 0x4fa0U) & 0xff07U) | (icon << 3)));
            }
            finish_dialog(r, hwnd, nav); return 1;
        }
        if (!(lp >> 16) && wp > 0x10eU && wp < 0x117U) {
            memset(d + 0x501aU, 0, 32); get_text(r, hwnd, 0x10cU, 0x501aU, 32);
            memset(d + 0x503aU, 0, 32); get_text(r, hwnd, 0x81bU, 0x503aU, 32);
            stars_window(r, "GetDlgItemText", hwnd, wp, 32, 0, unused, 0);
            for (i = 0; i < 7; ++i) if (!strcmp((char *)d + 0x501aU, (char *)d + stars_game_string(r, (StarsI16)(i + 0x567U)))) break;
            if (i < 7 && wp < 0x116U) {
                memset(d + 0x501aU, 0, 32); stars_copy_game_string(d, r->strings, (StarsI16)(wp + 0x458U), 0x501aU);
                set_text(r, hwnd, 0x10cU, 0x501aU); memset(d + 0x503aU, 0, 32);
                name = stars_player_name(d, r->strings, 0, 1, 1, 0, 0, 0x4f9aU);
                strcpy((char *)d + 0x503aU, (char *)d + name); set_text(r, hwnd, 0x81bU, 0x503aU);
            }
            player = wp < 0x116U ? (StarsU16)(0xda2U + (wp - 0x10fU) * 192U) : 0x4f9aU;
            stars_invalidate_advantage(r, hwnd); value = (StarsU16)stars_get_race_stat(d, player, 7);
            child = stars_control(r, hwnd, 0x81aU); send(r, child, 0x40eU, value, 0);
            child = stars_control(r, hwnd, 0x42fU); stars_window(r, "EnableWindow", child, (StarsU16)(wp != 0x115U), 0, 0, 0, 0);
            stars_put_word(d, 0x4fa0U, (StarsU16)((stars_word(d, 0x4fa0U) & 0xff07U) | (stars_word(d, (StarsU16)(player + 6U)) & 0xf8U)));
            child = stars_control(r, hwnd, 0x81aU); stars_window(r, "GetWindowRect", child, 0, 0, 0, &rect, 0);
            stars_screen_point(r, hwnd, &rect.right); rect.left = (StarsU16)(rect.right + 32U);
            rect.top = (StarsU16)(rect.bottom - 32U); rect.right = (StarsU16)(rect.right + 64U);
            stars_window(r, "InvalidateRect", hwnd, 1, 0, 0, &rect, 0);
        }
    } else if ((msg == 0x201U || msg == 0x203U) && !stars_word(d, 0x5320U)) {
        point.x = (StarsU16)lp; point.y = (StarsU16)(lp >> 16);
        rect = stars_read_rect(d, 0x5926U); spin = stars_read_rect(d, 0x592eU);
        if (!stars_point_in_rect(&rect, point) && !stars_point_in_rect(&spin, point)) return 0;
        direction = stars_point_in_rect(&rect, point) ? -1 : 1; if (direction == 1) rect = spin;
        icon = (StarsU16)((stars_word(d, 0x4fa0U) >> 3) & 31U);
        dc = (StarsU16)stars_window(r, "GetDC", hwnd, 0, 0, 0, 0, 0);
        stars_window(r, "SelectPalette", dc, stars_word(d, 0x5314U), 0, 0, 0, 0);
        stars_window(r, "RealizePalette", dc, 0, 0, 0, 0, 0); memset(&track, 0, sizeof(track));
        stars_init_button_track(r, &track, hwnd, 0, &rect, direction < 0 ? 0x22U : 0x23U, 80, 0, 0, 0);
        while (stars_track_button(r, &track)) {
            icon = (StarsU16)((icon + 32U + direction) % 32U); spin = stars_read_rect(d, 0x5926U);
            stars_dib_blt(r, dc, stars_signed_word((StarsU16)(spin.left - 2U)), stars_signed_word((StarsU16)(spin.top - 35U)),
                32, 32, stars_word(d, 0x5476U), (StarsI16)((icon & 7U) * 32U), (StarsI16)((3U - icon / 8U) * 32U), 32, 32, 0xcc0020UL);
        }
        stars_put_word(d, 0x4fa0U, (StarsU16)((stars_word(d, 0x4fa0U) & 0xff07U) | (icon << 3)));
        stars_window(r, "ReleaseDC", hwnd, dc, 0, 0, 0, 0); return 1;
    }
    return 0;
}

/* Paint-only shared frame construction; each page retains its own label,
 * geometry, invalidation rectangle and control behavior. */
static void paint_traits(StarsRuntime *r, StarsU16 hwnd, StarsU16 page)
{
    StarsU8 *d = r->data, paint[32];
    StarsU16 dc, child, length, i, text, top = r->scratch_top;
    StarsI16 dy = stars_signed_word(stars_word(d, 0x23faU)), id;
    StarsRect rect, frame;
    dc = (StarsU16)stars_window(r, "BeginPaint", hwnd, 0, 0, 0, paint, 0);
    stars_window(r, "GetClientRect", hwnd, 0, 0, 0, &rect, 0); stars_draw_advantage(r, dc, &rect, 0);
    stars_select(r, dc, stars_word(d, 0x26b6U)); stars_window(r, "SetBkColor", dc, 0, 0, stars_dword(d, 0x22baU), 0, 0);
    if (page == 4) {
        frame = controls_frame(r, hwnd, 0x10fU, 0x118U); stars_expand_rect(&frame, (StarsI16)(dy + 2), stars_half((StarsU16)dy));
        frame.top = (StarsU16)(frame.top - 4U); stars_draw_frame(r, dc, &frame, -1); frame_label(r, dc, &frame, 0x112);
        stars_window(r, "GetClientRect", hwnd, 0, 0, 0, &rect, 0);
        rect.top = (StarsU16)(frame.bottom + 12U); rect.left += 12U; rect.right -= 12U;
        child = stars_control(r, hwnd, 0x76U); stars_window(r, "GetWindowRect", child, 0, 0, 0, &frame, 0);
        stars_screen_point(r, hwnd, &frame.left); rect.bottom = (StarsU16)(frame.top - 6U);
        stars_draw_frame(r, dc, &rect, -1); frame_label(r, dc, &rect, 0x113);
        id = (StarsI16)(stars_get_race_stat(d, 0x4f9aU, 14) * 3 + 0x114);
        text = stars_scratch(r, 600U); length = 0;
        for (i = 0; i < 3; ++i) length = (StarsU16)(length + stars_copy_game_string(d, r->strings, id++, (StarsU16)(text + length)));
        stars_expand_rect(&rect, (StarsI16)(-2 - dy), (StarsI16)-stars_half((StarsU16)dy)); rect.top += 4U;
        stars_window(r, "DrawText", dc, length, 0x10U, 0, d + text, &rect); stars_write_rect(d, 0x23beU, &rect);
    } else if (page == 5) {
        child = stars_control(r, hwnd, 0x130U); stars_window(r, "GetWindowRect", child, 0, 0, 0, &frame, 0);
        stars_screen_point(r, hwnd, &frame.right); stars_window(r, "GetClientRect", hwnd, 0, 0, 0, &rect, 0);
        rect.top = (StarsU16)(frame.bottom + 12U); rect.left += 12U; rect.right -= 12U;
        child = stars_control(r, hwnd, 0x76U); stars_window(r, "GetWindowRect", child, 0, 0, 0, &frame, 0);
        stars_screen_point(r, hwnd, &frame.left); rect.bottom = (StarsU16)(frame.top - 12U);
        stars_draw_frame(r, dc, &rect, -1); frame = rect; frame.top = (StarsU16)(frame.top - stars_half((StarsU16)dy));
        stars_write_rect(d, 0x23beU, &frame); frame_label(r, dc, &rect, (StarsI16)(stars_word(d, 0x5998U) + 0x132U));
        length = (StarsU16)stars_copy_game_string(d, r->strings, (StarsI16)(stars_word(d, 0x5998U) + 0x140U), 0x57a4U);
        stars_expand_rect(&rect, (StarsI16)(-2 - dy), (StarsI16)-stars_half((StarsU16)dy)); rect.top += 4U;
        stars_window(r, "DrawText", dc, length, 0x10U, 0, d + 0x57a4U, &rect);
    } else {
        for (i = 0; i < 6; ++i) {
            frame = controls_frame(r, hwnd, (StarsU16)(0x10fU + i * 3U), (StarsU16)(0x111U + i * 3U));
            stars_expand_rect(&frame, dy, stars_half((StarsU16)dy)); stars_draw_frame(r, dc, &frame, -1);
            length = (StarsU16)stars_copy_game_string(d, r->strings, (StarsI16)(0x54U + i), 0x57a4U);
            length = (StarsU16)(length + stars_copy_game_string(d, r->strings, 0x52e, (StarsU16)(0x57a4U + length)));
            stars_text_out(r, dc, (StarsU16)(frame.left + 8U), (StarsU16)(frame.top - stars_half((StarsU16)dy)), d + 0x57a4U, length);
        }
    }
    stars_window(r, "EndPaint", hwnd, 0, 0, 0, paint, 0); r->scratch_top = top;
}
StarsI16 stars_race_dialog4(StarsRuntime *r, StarsU16 hwnd, StarsU16 msg, StarsU16 wp, StarsU32 lp)
{
    StarsU8 *d = r->data; StarsU16 i; StarsRect rect; int nav;
    if (msg == 0xfU) { paint_traits(r, hwnd, 4); return 1; }
    if (msg == 0x14U || msg == 0x19U) return dialog_background(r, hwnd, msg, wp, lp, 0x10fU, 0x119U, 0);
    if (msg == 0x110U) {
        stars_race_title(r, hwnd, stars_signed_word(stars_word(d, 0x2602U)));
        radio(r, hwnd, 0x10fU, 0x118U, (StarsU16)(stars_get_race_stat(d, 0x4f9aU, 14) + 0x10fU));
        if (stars_word(d, 0x5320U)) for (i = 0x10fU; i < 0x119U; ++i) disable(r, hwnd, i);
        sticky(r, hwnd, 1); return 1;
    }
    if (msg == 0x111U) {
        if (wp == 0x76U) { help(r, hwnd, 0x408U); return 1; }
        nav = navigation(r, wp); if (nav >= 0) { finish_dialog(r, hwnd, nav); return 1; }
        if (!(lp >> 16) && wp > 0x10eU && wp < 0x119U) {
            stars_set_race_stat(d, 0x4f9aU, 14, (StarsI16)(wp - 0x10fU));
            if (stars_get_race_stat(d, 0x4f9aU, 14) == 8) {
                for (i = 1; i <= 6; ++i) stars_set_race_stat(d, 0x4f9aU, (StarsI16)i, i == 5 ? 5 : 10);
                stars_set_race_trait(d, 0x4f9aU, 31, 0);
            }
            stars_invalidate_advantage(r, hwnd); rect = stars_read_rect(d, 0x23beU);
            stars_window(r, "InvalidateRect", hwnd, 0, 0, 0, &rect, 0);
        }
    }
    return 0;
}
StarsI16 stars_race_dialog5(StarsRuntime *r, StarsU16 hwnd, StarsU16 msg, StarsU16 wp, StarsU32 lp)
{
    StarsU8 *d = r->data; StarsU16 i, child, value; StarsRect rect; int nav;
    if (msg == 0xfU) { paint_traits(r, hwnd, 5); return 1; }
    if (msg == 0x14U || msg == 0x19U) return dialog_background(r, hwnd, msg, wp, lp, 0x123U, 0x131U, 0);
    if (msg == 0x110U) {
        stars_race_title(r, hwnd, stars_signed_word(stars_word(d, 0x2602U))); stars_put_word(d, 0x5998U, 0);
        for (i = 0; i < 14; ++i) {
            child = stars_control(r, hwnd, (StarsU16)(i + 0x123U));
            stars_window(r, "SetWindowText", child, 0, 0, 0, d + stars_game_string(r, (StarsI16)(i + 0x132U)), 0);
            send(r, child, 0x401U, (StarsU16)stars_get_race_trait(d, 0x4f9aU, (StarsI16)i), 0);
            if (stars_word(d, 0x5320U)) stars_window(r, "EnableWindow", child, 0, 0, 0, 0, 0);
        }
        sticky(r, hwnd, 1); return 1;
    }
    if (msg == 0x111U) {
        if (wp == 0x76U) { help(r, hwnd, 0x411U); return 1; }
        nav = navigation(r, wp); if (nav >= 0) { finish_dialog(r, hwnd, nav); return 1; }
        if (wp > 0x122U && wp < 0x131U) {
            child = stars_control(r, hwnd, wp); value = (StarsU16)send(r, child, 0x400U, 0, 0);
            stars_put_word(d, 0x5998U, (StarsU16)(wp - 0x123U));
            stars_set_race_trait(d, 0x4f9aU, (StarsI16)(wp - 0x123U), stars_signed_word(value));
            stars_invalidate_advantage(r, hwnd); rect = stars_read_rect(d, 0x23beU);
            stars_window(r, "InvalidateRect", hwnd, 0, 0, 0, &rect, 0);
        }
    }
    return 0;
}
StarsI16 stars_race_dialog6(StarsRuntime *r, StarsU16 hwnd, StarsU16 msg, StarsU16 wp, StarsU32 lp)
{
    StarsU8 *d = r->data; StarsU16 i, child, value; int nav;
    if (msg == 0xfU) { paint_traits(r, hwnd, 6); return 1; }
    if (msg == 0x14U || msg == 0x19U) return dialog_background(r, hwnd, msg, wp, lp, 0x10fU, 0x121U, 0x123U);
    if (msg == 0x110U) {
        stars_race_title(r, hwnd, stars_signed_word(stars_word(d, 0x2602U)));
        for (i = 0; i < 6; ++i) radio(r, hwnd, (StarsU16)(0x10fU + i * 3U), (StarsU16)(0x111U + i * 3U),
            (StarsU16)(0x10fU + i * 3U + stars_get_race_stat(d, 0x4f9aU, (StarsI16)(i + 8U))));
        if (stars_word(d, 0x5320U)) for (i = 0x10fU; i < 0x121U; ++i) disable(r, hwnd, i);
        value = (StarsU16)(3 + (stars_get_race_stat(d, 0x4f9aU, 14) == 9));
        sprintf((char *)d + 0x57a4U, (char *)d + stars_game_string(r, 0x4d5), (int)value);
        child = stars_control(r, hwnd, 0x123U); stars_window(r, "SetWindowText", child, 0, 0, 0, d + 0x57a4U, 0);
        child = stars_control(r, hwnd, 0x123U); send(r, child, 0x401U, (StarsU16)stars_get_race_trait(d, 0x4f9aU, 29), 0);
        if (stars_word(d, 0x5320U)) disable(r, hwnd, 0x123U);
        sticky(r, hwnd, 1); return 1;
    }
    if (msg == 0x111U) {
        if (wp == 0x76U) { help(r, hwnd, 0x421U); return 1; }
        nav = navigation(r, wp); if (nav >= 0) { finish_dialog(r, hwnd, nav); return 1; }
        if (!(lp >> 16) && wp > 0x10eU && wp < 0x121U) {
            i = (StarsU16)(wp - 0x10fU); stars_set_race_stat(d, 0x4f9aU, (StarsI16)(i / 3U + 8U), (StarsI16)(i % 3U));
            stars_invalidate_advantage(r, hwnd);
        } else if (wp == 0x123U) {
            child = stars_control(r, hwnd, 0x123U); value = (StarsU16)send(r, child, 0x400U, 0, 0);
            stars_set_race_trait(d, 0x4f9aU, 29, stars_signed_word(value)); stars_invalidate_advantage(r, hwnd);
        }
    }
    return 0;
}

StarsI16 stars_race_dialog2(StarsRuntime *r, StarsU16 hwnd, StarsU16 msg, StarsU16 wp, StarsU32 lp)
{
    StarsU8 *d = r->data, paint[32];
    StarsU16 dc, old, i, child, length, value, text, top = r->scratch_top;
    StarsI16 dy = stars_signed_word(stars_word(d, 0x23faU)), left, width, size, y, label_width;
    StarsU32 extent1, extent2;
    StarsRect client, rect, copy;
    StarsPoint point;
    int nav;
    if (msg == 0xfU) {
        dc = (StarsU16)stars_window(r, "BeginPaint", hwnd, 0, 0, 0, paint, 0);
        stars_put_word(d, 0x2608U, 65534U); stars_draw_race2(r, hwnd, dc, 65535U);
        stars_window(r, "EndPaint", hwnd, 0, 0, 0, paint, 0); return 1;
    }
    if (msg == 0x14U || msg == 0x19U) return dialog_background(r, hwnd, msg, wp, lp, 0x123U, 0x126U, 0);
    if (msg == 0x20U) {
        stars_window(r, "GetCursorPos", 0, 0, 0, 0, &point, 0); stars_screen_point(r, hwnd, &point);
        if (stars_race_hit_test(r, point) >= 0) { stars_window(r, "SetCursor", stars_word(d, 0x525eU), 0, 0, 0, 0, 0); return 1; }
    } else if (msg == 0x110U) {
        stars_race_title(r, hwnd, stars_signed_word(stars_word(d, 0x2602U))); stars_put_word(d, 0x2608U, 65535U);
        dc = (StarsU16)stars_window(r, "GetDC", hwnd, 0, 0, 0, 0, 0);
        stars_window(r, "GetClientRect", hwnd, 0, 0, 0, &client, 0); old = stars_select(r, dc, stars_word(d, 0x26b6U));
        text = stars_scratch(r, 20U); length = (StarsU16)stars_copy_game_string(d, r->strings, 0x52d, text);
        left = stars_signed_word((StarsU16)(stars_text_extent(r, dc, d + text, length) + 10U));
        label_width = stars_signed_word((StarsU16)(stars_text_extent(r, dc, d + 0x1331U, 5) + 10U));
        width = stars_signed_word((StarsU16)(client.right - left - label_width)); size = (StarsI16)(dy * 3 / 2); y = (StarsI16)(dy * 3);
        stars_set_race_rect(r, 0, stars_rect((StarsU16)left, (StarsU16)y, (StarsU16)(left + size), (StarsU16)(y + size)));
        stars_set_race_rect(r, 1, stars_rect((StarsU16)(left + size + 6), (StarsU16)y, (StarsU16)(left + width - size - 6), (StarsU16)(y + size)));
        stars_set_race_rect(r, 2, stars_rect((StarsU16)(left + width - size), (StarsU16)y, (StarsU16)(left + width), (StarsU16)(y + size)));
        stars_set_race_rect(r, 3, stars_rect((StarsU16)left, (StarsU16)(y + size + 4), (StarsU16)(left + size * 3), (StarsU16)(y + size * 2 + 4)));
        stars_set_race_rect(r, 4, stars_rect((StarsU16)(left + width - size * 3), (StarsU16)(y + size + 4), (StarsU16)(left + width), (StarsU16)(y + size * 2 + 4)));
        for (i = 0; i < 5; ++i) {
            rect = stars_race_rect(r, i); copy = rect; stars_offset_rect(&copy, 0, (StarsI16)(size * 3)); stars_set_race_rect(r, (StarsU16)(i + 5U), copy);
            copy = rect; stars_offset_rect(&copy, 0, (StarsI16)(size * 6)); stars_set_race_rect(r, (StarsU16)(i + 10U), copy);
        }
        for (i = 0; i < 3; ++i) {
            child = stars_control(r, hwnd, (StarsU16)(i + 0x123U)); rect = stars_race_rect(r, (StarsU16)(i * 5U + 3U));
            stars_window_position(r, child, (StarsU16)(left + size * 3 + 6), rect.top, (StarsU16)(width - size * 6 - 12), (StarsU16)size, 4);
            stars_window(r, "CheckDlgButton", hwnd, (StarsU16)(i + 0x123U), (StarsU16)(stars_signed_byte(d[0x4fb0U + i]) < 0), 0, 0, 0);
        }
        length = (StarsU16)stars_copy_game_string(d, r->strings, 0xf5, 0x57a4U);
        extent1 = stars_text_extent(r, dc, d + 0x1337U, 3); extent2 = stars_text_extent(r, dc, d + 0x57a4U, length);
        rect.left = (StarsU16)(left + (StarsU16)extent1 + (StarsU16)extent2 + 4U); rect.top = (StarsU16)(size * 9 + y - 3);
        rect.right = (StarsU16)(rect.left + 15U); rect.bottom = (StarsU16)(rect.top + stars_half((StarsU16)dy) + 3);
        stars_set_race_rect(r, 15, rect); stars_offset_rect(&rect, 0, stars_signed_word((StarsU16)(rect.bottom - rect.top - 1U)));
        stars_set_race_rect(r, 16, rect); stars_put_word(d, 0x4b94U, 17);
        stars_select(r, dc, old); stars_window(r, "ReleaseDC", hwnd, dc, 0, 0, 0, 0);
        if (stars_word(d, 0x5320U)) for (i = 0x123U; i < 0x126U; ++i) disable(r, hwnd, i);
        sticky(r, hwnd, 1); r->scratch_top = top; return 1;
    } else if (msg == 0x111U) {
        if (wp == 0x76U) { help(r, hwnd, 0x41dU); return 1; }
        nav = navigation(r, wp); if (nav >= 0) { finish_dialog(r, hwnd, nav); return 1; }
        if (wp > 0x122U && wp < 0x126U) {
            child = stars_control(r, hwnd, wp); value = (StarsU16)send(r, child, 0x400U, 0, 0); i = (StarsU16)(wp - 0x123U);
            if (value == 1U) { d[0x4faaU + i] = 255; d[0x4fb0U + i] = 255; d[0x4fadU + i] = 255; }
            else { d[0x4fadU + i] = 20; d[0x4fb0U + i] = 80; d[0x4faaU + i] = 50; }
            stars_draw_race2(r, hwnd, 0, (StarsU16)(0xff00U | (1U << i)));
        }
    } else if (msg == 0x201U || msg == 0x203U) {
        point.x = (StarsU16)lp; point.y = (StarsU16)(lp >> 16); return stars_track_race2(r, hwnd, point, wp);
    }
    return 0;
}

StarsI16 stars_race_dialog3(StarsRuntime *r, StarsU16 hwnd, StarsU16 msg, StarsU16 wp, StarsU32 lp)
{
    StarsU8 *d = r->data, paint[32]; StarsU16 dc, child, value; StarsPoint point; int nav;
    if (msg == 0xfU) {
        dc = (StarsU16)stars_window(r, "BeginPaint", hwnd, 0, 0, 0, paint, 0); stars_draw_race3(r, hwnd, dc, -1);
        stars_window(r, "EndPaint", hwnd, 0, 0, 0, paint, 0); return 1;
    }
    if (msg == 0x14U || msg == 0x19U) return dialog_background(r, hwnd, msg, wp, lp, 0x123U, 0x124U, 0);
    if (msg == 0x20U) {
        stars_window(r, "GetCursorPos", 0, 0, 0, 0, &point, 0); stars_screen_point(r, hwnd, &point);
        if (stars_race_hit_test(r, point) >= 0) { stars_window(r, "SetCursor", stars_word(d, 0x525eU), 0, 0, 0, 0, 0); return 1; }
    } else if (msg == 0x110U) {
        stars_race_title(r, hwnd, stars_signed_word(stars_word(d, 0x2602U))); child = stars_control(r, hwnd, 0x123U);
        send(r, child, 0x401U, (StarsU16)stars_get_race_trait(d, 0x4f9aU, 31), 0);
        if (stars_word(d, 0x5320U) || stars_get_race_stat(d, 0x4f9aU, 14) == 8) disable(r, hwnd, 0x123U);
        sticky(r, hwnd, 1); return 1;
    } else if (msg == 0x111U) {
        if (wp == 0x76U) { help(r, hwnd, 0x420U); return 1; }
        nav = navigation(r, wp); if (nav >= 0) { finish_dialog(r, hwnd, nav); return 1; }
        if (wp == 0x123U) {
            child = stars_control(r, hwnd, 0x123U); value = (StarsU16)send(r, child, 0x400U, 0, 0);
            stars_set_race_trait(d, 0x4f9aU, 31, stars_signed_word(value)); stars_draw_race3(r, hwnd, 0, 99);
        }
    } else if (msg == 0x201U || msg == 0x203U) {
        point.x = (StarsU16)lp; point.y = (StarsU16)(lp >> 16); return stars_track_race3(r, hwnd, point, wp);
    }
    return 0;
}
