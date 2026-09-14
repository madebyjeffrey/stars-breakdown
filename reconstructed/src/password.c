/* NE 9:58D8-5CB9 and NE 30:3A3E-3AC1. */
#include "runtime_internal.h"

StarsU32 stars_password_salt(const StarsU8 *text)
{
    StarsU32 result = 0;
    StarsI32 ch;
    if (!*text) return 0;
    while (*text) {
        ch = *text < 128U ? *text : (StarsI32)*text - 256L;
        result += (StarsU32)ch;
        ++text;
        if (*text) {
            ch = *text < 128U ? *text : (StarsI32)*text - 256L;
            result *= (StarsU32)ch;
            ++text;
        }
    }
    return result ? result : 1U;
}

void stars_delay(StarsI16 count, StarsTickCount timer, void *context)
{
    StarsU32 start = timer(context), now;
    StarsU32 end = start + (StarsU32)(StarsI32)count;
    do { now = timer(context); } while (now >= start && now < end);
}

StarsI16 stars_check_password(StarsRuntime *r)
{
    StarsU8 *d = r->data;
    StarsU32 salt = stars_dword(d, 0x35cU);
    StarsU16 owner;
    if (!salt || salt == stars_dword(d, 0x360U) || stars_word(d, 0x1442U)) return 1;
    if (d[0x2548U] && stars_password_salt(d + 0x2548U) == salt) return 1;
    if (stars_word(d, 0x229aU) & 0x4000U) return 0;
    owner = stars_word(d, 0x352U);
    if (!owner) owner = stars_word(d, 0x258cU);
    return r->platform->dialog(r->context, r, 0x8cU, owner, stars_password_dialog);
}

StarsI16 stars_password_dialog(StarsRuntime *r, StarsU16 hwnd, StarsU16 message,
    StarsU16 wparam, StarsU32 lparam)
{
    StarsU8 *d = r->data;
    StarsRect rect;
    StarsU8 text[62];
    StarsU16 failures, child;
    if (message == 0x14U) {
        stars_window(r, "GetClientRect", hwnd, 0, 0, 0, &rect, 0);
        stars_window(r, "FillRect", wparam, stars_word(d, 0x10U), 0, 0, &rect, 0);
        return 1;
    }
    if (message == 0x19U && (StarsU16)(lparam >> 16) == 6U) {
        stars_window(r, "SetBkColor", wparam, 0, 0, stars_dword(d, 0x22baU), 0, 0);
        return stars_signed_word(stars_word(d, 0x10U));
    }
    if (message == 0x110U) {
        stars_send_item(r, hwnd, 0x10cU, 0x415U, 15U, 0);
        child = stars_control(r, hwnd, 0x7e2U);
        stars_window(r, "SetWindowText", child, 0, 0, 0,
            d + stars_game_string(r, 0x35f), 0);
        return 1;
    }
    if (message == 0x111U) {
        if (wparam == 1U || wparam == 2U) {
            if (wparam == 1U) {
                stars_window(r, "GetDlgItemText", hwnd, 0x10cU, 60U, 0, text, 0);
                if (stars_password_salt(text) != stars_dword(d, 0x35cU)) {
                    failures = (StarsU16)(stars_word(d, 0x140eU) + 1U);
                    stars_put_word(d, 0x140eU, failures);
                    /* CMP/JC in the original: the failure counter is unsigned. */
                    stars_delay(failures < 10U ? 1000 : failures < 100U ? 5000 : 10000,
                        r->platform->timer_count, r->context);
                    stars_runtime_alert(r, stars_format_id(r, 0xea, 0), 16);
                    child = stars_control(r, hwnd, 0x10cU);
                    stars_window(r, "SetFocus", child, 0, 0, 0, 0, 0);
                    stars_send_item(r, hwnd, 0x10cU, 0x401U, 0, 0xffff0000UL);
                    return 0;
                }
                if (!(stars_word(d, 0x7ceU) & 0x200U)) {
                    stars_put_dword(d, 0x360U, stars_dword(d, 0x35cU));
                    strcpy((char *)d + 0x22e0U, (const char *)text);
                }
            }
            stars_window(r, "EndDialog", hwnd, (StarsU16)(wparam == 1U), 0, 0, 0, 0);
            return 1;
        }
        if (wparam == 0x76U) {
            stars_window(r, "WinHelp", hwnd, 1, 0, 0x441U, d + stars_word(d, 0x14aU), 0);
            return 1;
        }
    }
    return 0;
}
