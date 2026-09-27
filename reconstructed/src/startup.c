/* Startup resource ownership and shared brush cache. Win16 USER/GDI/KERNEL
 * services use the same explicit adapter as the recovered race component. */
#include "startup.h"
#include "runtime_internal.h"

/* NE 4:07AA-08D0. A valid player serial is compared against every other
 * dead player. Matching serial words plus a nonzero 11-byte configuration
 * difference identify the other player; serial validation is an explicit host
 * boundary because the original FValidSerialLong reads its own DGROUP table.
 */
StarsI16 stars_player_also_cheater(const StarsU8 *serials,
    const StarsU8 *players, StarsI16 player, StarsI16 player_count,
    StarsSerialValidator validate_serial, void *context)
{
    StarsU16 i;
    StarsU32 serial;
    const StarsU8 *current;
    const StarsU8 *candidate;

    if (!serials || !players || player < 0 || player_count < 0 ||
        !validate_serial)
        return -1;
    current = serials + (StarsU16)(player * 0x10);
    serial = stars_dword(current, 0);
    if (!validate_serial(context, serial)) return -1;
    for (i = 0; i < (StarsU16)player_count; ++i) {
        if (i == (StarsU16)player) continue;
        if (((stars_word(players, (StarsU16)(i * 0xc0U + 0x54U)) >> 2) & 1U) == 0U)
            continue;
        candidate = serials + (StarsU16)(i * 0x10U);
        if (stars_dword(candidate, 0) != serial) continue;
        if (memcmp(current + 4, candidate + 4, 11U) != 0)
            return (StarsI16)i;
    }
    return -1;
}

/* NE 4:15DE-1659. Scanner characters are forwarded as WM_CHAR only for
 * +/-/v/V, and only when the message edit control does not own focus. */
StarsI16 stars_handle_char(StarsU16 scanner, StarsU16 message,
    StarsU16 message_edit, StarsU16 focus, StarsU16 character,
    StarsU32 lparam, StarsGetFocus get_focus, StarsSendCharacter send,
    void *context)
{
    if (!scanner) return 0;
    if (character != 0x002bU && character != 0x002dU &&
        character != 0x0076U && character != 0x0056U)
        return 0;
    focus = get_focus ? get_focus(context) : focus;
    if (message && focus == message_edit) return 0;
    if (send) send(context, scanner, 0x0102U, character, lparam);
    return send ? 1 : 0;
}

/* NE 4:151E-15DC. OrderInfoDlg handles erase-background, control-color,
 * and the two dialog command IDs; all USER/GDI calls are host callbacks. */
StarsI16 stars_order_info_dialog(StarsU16 hwnd, StarsU16 message,
    StarsU16 wparam, StarsU32 lparam, StarsU16 button_face,
    StarsU32 cr_button_face, StarsGetClientRect get_rect,
    StarsFillRect fill_rect, StarsSetBackgroundColor set_background,
    StarsEndDialog end_dialog, void *context)
{
    StarsRect rect;

    if (message == 0x0014U) {
        if (get_rect) get_rect(context, hwnd, &rect);
        if (fill_rect) fill_rect(context, wparam, &rect, button_face);
        return 1;
    }
    if (message == 0x0019U) {
        if ((StarsI16)(lparam >> 16) == 6) {
            if (set_background) set_background(context, wparam, cr_button_face);
            return (StarsI16)button_face;
        }
    } else if (message == 0x0111U && (wparam == 1U || wparam == 2U)) {
        if (end_dialog) end_dialog(context, hwnd, 1);
        return 1;
    }
    return 0;
}

/* NE 4:165A-1C3B. Keyboard routing is kept separate from the Win16
 * boundary: the supplied image retains the original window handles,
 * selection record, scan flags and order pointer, while callbacks represent
 * USER, toolbar, report and fleet services. */
StarsI16 stars_handle_key(StarsU8 *data, StarsU16 hwnd, StarsU16 message,
    StarsU16 key, StarsU32 lparam, const StarsHandleKeyOps *ops,
    void *context)
{
    StarsU16 active;
    StarsU16 focus;
    StarsU16 parent;
    StarsU16 button;
    StarsU16 i;
    StarsU16 order_index;
    StarsU16 order_offset;
    StarsU16 order_record;
    StarsI16 key_state;
    StarsI16 x;
    StarsI16 y;
    StarsI16 row;
    StarsU32 order_value;

    (void)hwnd;
    if (!data) return 0;
    if (message == 0x0100U) {
        if (key == 0x001bU && stars_word(data, 0x018eU) != 0U) {
            active = ops && ops->get_active_window ?
                ops->get_active_window(context) : 0;
            if (active == stars_word(data, 0x018eU)) {
                if (ops && ops->destroy_window)
                    ops->destroy_window(context, stars_word(data, 0x018eU));
                return 1;
            }
        }
        if (key == 0x001bU && stars_word(data, 0x0b7eU) != 0U) {
            if (ops && ops->send_message)
                ops->send_message(context, stars_word(data, 0x0b7eU),
                    0x0202U, 0, 0);
            return 1;
        }
        if (key == 0x001bU && stars_word(data, 0x15c4U) != 0U) {
            if (ops && ops->destroy_window)
                ops->destroy_window(context, stars_word(data, 0x15c4U));
            return 1;
        }
    } else if (message == 0x0101U && stars_word(data, 0x019aU) != 0U &&
               (key == 0x001bU || key == 0x000dU)) {
        focus = ops && ops->get_focus ? ops->get_focus(context) : 0;
        parent = ops && ops->get_parent ?
            ops->get_parent(context, focus) : 0;
        if (parent == stars_word(data, 0x019aU) ||
            (ops && ops->get_parent &&
             ops->get_parent(context, parent) == stars_word(data, 0x019aU))) {
            if (ops && ops->terminate_toolbar_focus)
                ops->terminate_toolbar_focus(context, key == 0x001bU);
        }
    }
    if (key == 0x0010U && stars_word(data, 0x0190U) != 0U) {
        x = 0;
        y = 0;
        if (ops && ops->get_cursor_pos)
            ops->get_cursor_pos(context, &x, &y);
        active = ops && ops->window_from_point ?
            ops->window_from_point(context, x, y) : 0;
        if (active == stars_word(data, 0x0190U) && ops &&
            ops->send_message)
            ops->send_message(context, active, 0x0020U, active, 0);
    }
    if (message != 0x0100U) return 0;
    if (key != 8U && key != 0x2eU && key != 0x28U && key != 0x26U &&
        key != 0x24U && key != 0x23U &&
        (key < 0x30U || key > 0x39U) && key != 0xbcu && key != 0xbeU &&
        key != 0xdbU && key != 0xddU)
        return 0;

    focus = ops && ops->get_focus ? ops->get_focus(context) : 0;
    if (stars_word(data, 0x0198U) != 0U) {
        if (stars_word(data, 0x019aU) != 0U) {
            if (focus == stars_word(data, 0x019aU)) return 0;
            parent = ops && ops->get_parent ?
                ops->get_parent(context, focus) : 0;
            if (parent == stars_word(data, 0x019aU)) return 0;
            if (ops && ops->get_parent &&
                ops->get_parent(context, parent) == stars_word(data, 0x019aU))
                return 0;
        }
        for (i = 0; i < 3U; ++i) {
            if (focus == stars_word(data, (StarsU16)(0x5986U + i * 2U)))
                return 0;
        }
        if (focus == stars_word(data, 0x51baU) ||
            focus == stars_word(data, 0x525cU) ||
            focus == stars_word(data, 0x5924U) ||
            focus == stars_word(data, 0x51fcU) ||
            focus == stars_word(data, 0x0874U) ||
            focus == stars_word(data, 0x531aU) ||
            focus == stars_word(data, 0x530eU)) return 0;
        if (stars_word(data, 0x018eU) != 0U && ops &&
            ops->get_dialog_item &&
            focus == ops->get_dialog_item(context, stars_word(data, 0x018eU),
                                          0x010bU)) return 0;
    }
    if (key > 0x2fU && key < 0x3aU) {
        if (key > 0x30U && key < 0x37U) {
            button = (StarsU16)(key - 0x31U);
            if (button != (StarsU16)(stars_word(data, 0x0588U) & 0x000fU)) {
                if (ops && ops->execute_button)
                    ops->execute_button(context, button, 1);
                if (ops && ops->invalidate_rect)
                    ops->invalidate_rect(context, stars_word(data, 0x019aU));
            }
            return 1;
        }
        if (key == 0x30U) {
            key_state = ops && ops->get_key_state ?
                ops->get_key_state(context, 0x10U) : 0;
            button = key_state < 0 ? 0x11U : 0x0bU;
        } else if (key == 0x37U) button = 7U;
        else if (key == 0x38U) button = 8U;
        else button = 9U;
        key_state = ops && ops->is_button_down ?
            ops->is_button_down(context, button) : 0;
        if (ops && ops->execute_button)
            ops->execute_button(context, button, key_state == 0);
        if (ops && ops->invalidate_rect)
            ops->invalidate_rect(context, stars_word(data, 0x019aU));
        return 1;
    }
    if (key != 8U) {
        if (key == 0x23U || key == 0x24U || key == 0x26U || key == 0x28U) {
            if (focus != stars_word(data, 0x5316U)) {
                if (ops && ops->send_message)
                    ops->send_message(context, stars_word(data, 0x0198U),
                        0x0100U, key, lparam);
                return 1;
            }
            return 0;
        }
        if (key != 0x2eU) {
            if (key == 0xbcu || key == 0xbeU) {
                if (stars_word(data, 0x495aU) == 2U &&
                    (stars_signed_word(stars_word(data, 0x4960U)) > 0 ||
                     stars_signed_word(stars_word(data, 0x49d4U)) > 1)) {
                    order_index = stars_word(data, 0x4960U);
                    if (stars_signed_word(order_index) < 1) order_index = 1U;
                    order_offset = stars_word(data, 0x49d6U);
                    order_record = (StarsU16)(order_offset +
                        order_index * 0x12U + 10U);
                    order_value = stars_dword(data, order_record);
                    button = (StarsU16)((order_value >> 4) & 0x000fU);
                    if (key == 0xbcu) --button;
                    else ++button;
                    if (button < 0x0cU) {
                        order_value = (order_value & 0xff0fU) |
                            ((StarsU32)button << 4);
                        stars_put_dword(data, order_record, order_value);
                        if (ops && ops->lookup_fleet)
                            ops->lookup_fleet(context, -1, data + 0x4972U);
                        if (ops && ops->draw_plan_ship)
                            ops->draw_plan_ship(context, 0, 0x4220U);
                    }
                }
                return 1;
            }
            if (key != 0xdbU && key != 0xddU) return 0;
            row = key == 0xdbU ? -2 : -1;
            if (ops && ops->execute_report_click)
                ops->execute_report_click(context, 0, 0, 2, 0, row);
            return 1;
        }
    }
    if (stars_word(data, 0x495aU) == 2U &&
        ops && ops->delete_current_waypoint)
        ops->delete_current_waypoint(context, 8);
    return 1;
}

/* NE 4:06A4-07A9. The batch loader keeps the original far-pointer slots in
 * the image; the allocation and stream operations are explicit host calls so
 * the source does not hide them behind emitted instructions. */
StarsI16 stars_set_up_batch_processing(StarsU8 *data,
    const StarsBatchOps *ops, void *context)
{
    StarsI16 handle;
    StarsU16 count;
    StarsU16 copied;
    StarsU16 batch_low;
    StarsU16 batch_selector;
    StarsU16 end_low;
    StarsU32 batch_far;
    StarsU8 *batch;
    StarsI16 success;

    success = 0;
    if (!data || !ops || !ops->open || !ops->file_length ||
        !ops->allocate || !ops->memory || !ops->read || !ops->close)
        return 0;
    stars_put_word(data, 0x006eU, 1U);
    stars_put_word(data, 0x0a22U, 0xffffU);
    handle = ops->open(context, data + 0x56a2U, 0x20U);
    stars_put_word(data, 0x0a22U, (StarsU16)handle);
    if (handle != (StarsI16)-1) {
        count = ops->file_length(context, handle);
        batch_far = ops->allocate(context, count, 9);
        stars_put_dword(data, 0x019eU, batch_far);
        batch = ops->memory(context, batch_far);
        if (batch && ops->read(context, handle, batch, count) == count) {
            batch_low = (StarsU16)batch_far;
            batch_selector = (StarsU16)(batch_far >> 16);
            end_low = (StarsU16)(batch_low + count);
            stars_put_word(data, 0x51b0U, end_low);
            stars_put_word(data, 0x51b2U, batch_selector);
            copied = 0;
            while (copied < count && batch[copied] != 0x0aU) {
                data[0x56a2U + copied] = batch[copied];
                ++copied;
            }
            stars_put_dword(data, 0x019eU,
                (StarsU32)(StarsU16)(batch_low + copied + 1U) |
                ((StarsU32)batch_selector << 16));
            if (copied == 0U)
                data[0x56a1U] = 0;
            else
                data[0x56a2U + copied - 1U] = 0;
            success = 1;
        }
    }
    stars_put_word(data, 0x006eU, 0U);
    if (stars_word(data, 0x0a22U) != 0xffffU)
        ops->close(context, handle);
    if (!success) data[0x56a2U] = 0;
    return success;
}

/* NE 1:0D70-0EA9. Initialization retains the original flag clearing,
 * persisted frame placement, accelerator resources and two ShowWindow calls;
 * the Win16 entry points and the larger INI reader stay at the host boundary. */
StarsI16 stars_init_instance(StarsU8 *data, StarsI16 command_show,
    const StarsInitInstanceOps *ops, void *context)
{
    StarsU16 flags;
    StarsU16 hwnd;
    StarsU16 accelerator;
    StarsU16 accelerator_title;
    StarsU16 show;

    if (!data || !ops || !ops->create_window ||
        !ops->load_accelerator || !ops->show_window)
        return 0;
    flags = stars_word(data, 0x229aU);
    stars_put_word(data, 0x229aU, (StarsU16)(flags & 0xfe1aU));
    stars_put_word(data, 0x22a0U, 0xffffU);
    if (ops->read_ini_settings) ops->read_ini_settings(context);
    hwnd = ops->create_window(context, data + 0x01d6U, data + 0x0731U,
        0x00cf0000UL, stars_signed_word(stars_word(data, 0x2290U)),
        stars_signed_word(stars_word(data, 0x2292U)),
        stars_signed_word(stars_word(data, 0x2294U)),
        stars_signed_word(stars_word(data, 0x2296U)),
        stars_word(data, 0x5310U));
    stars_put_word(data, 0x258cU, hwnd);
    if (!hwnd) return 0;
    accelerator = ops->load_accelerator(context, stars_word(data, 0x5310U),
        0x0074U);
    stars_put_word(data, 0x23e6U, accelerator);
    if (!accelerator) return 0;
    accelerator_title = ops->load_accelerator(context,
        stars_word(data, 0x5310U), 0x0438U);
    stars_put_word(data, 0x4a58U, accelerator_title);
    if (!accelerator_title) return 0;
    if (command_show == 1) {
        flags = stars_word(data, 0x2298U);
        show = (flags & 3U) ? 3U : 1U;
    } else show = (StarsU16)command_show;
    ops->show_window(context, hwnd, show);
    ops->show_window(context, hwnd, 0);
    return 1;
}

/* NE 4:1252-151B. About's Win16 painting and dialog operations are exposed
 * through StarsAboutOps; the state machine, timer transitions, message
 * selection and ten-line scrolling window remain in ordinary C. */
StarsI16 stars_about(StarsU8 *data, StarsU16 hwnd, StarsU16 message,
    StarsU16 wparam, StarsU32 lparam, const StarsAboutOps *ops,
    void *context)
{
    StarsRect rect;
    StarsU16 item;
    StarsU16 dc;
    StarsU16 i;
    StarsI16 first;
    StarsI16 partial;
    StarsI16 line;
    StarsU16 height;
    const StarsU8 *text;

    if (!data) return 0;
    if (message == 0x0014U) {
        if (ops && ops->get_client_rect) ops->get_client_rect(context, hwnd, &rect);
        if (ops && ops->fill_rect)
            ops->fill_rect(context, wparam, &rect, stars_word(data, 0x0010U));
        return 1;
    }
    if (message == 0x0019U) {
        if ((StarsI16)(lparam >> 16) == 6) {
            if (ops && ops->set_bk_color)
                ops->set_bk_color(context, wparam, stars_dword(data, 0x22baU));
            return (StarsI16)stars_word(data, 0x0010U);
        }
        return 0;
    }
    if (message == 0x0110U) {
        stars_put_word(data, 0x5994U, 0xfff5U);
        stars_put_word(data, 0x51acU, 0);
        if (ops && ops->get_dialog_item)
            item = ops->get_dialog_item(context, hwnd, 0x0401U);
        else item = 0;
        text = ops && ops->version ? ops->version(context) : 0;
        if (ops && ops->set_window_text) ops->set_window_text(context, item, text);
        if (ops && ops->set_timer)
            stars_put_word(data, 0x01a2U,
                ops->set_timer(context, hwnd, 0x000eU, 0x32U));
        return 1;
    }
    if (message == 0x0111U) {
        if (wparam == 1U || wparam == 2U) {
            if (ops && ops->kill_timer)
                ops->kill_timer(context, hwnd, stars_word(data, 0x01a2U));
            stars_put_word(data, 0x01a2U, 0);
            if (ops && ops->end_dialog) ops->end_dialog(context, hwnd, 1);
            return 1;
        }
        if (wparam == 0x0076U && ops && ops->show_order)
            ops->show_order(context, hwnd, 0x0061U, 0);
        return 0;
    }
    if (message != 0x0113U) return 0;
    if (ops && ops->get_dialog_item)
        item = ops->get_dialog_item(context, hwnd, 0x041fU);
    else item = 0;
    partial = (StarsI16)(stars_word(data, 0x51acU) + 2);
    height = stars_word(data, 0x23faU);
    if ((StarsU16)partial >= height) {
        partial = 0;
        first = (StarsI16)(stars_word(data, 0x5994U) + 1U);
        if (first > 0x4e) first = -11;
        stars_put_word(data, 0x5994U, (StarsU16)first);
    } else first = (StarsI16)stars_word(data, 0x5994U);
    stars_put_word(data, 0x51acU, (StarsU16)partial);
    if (ops && ops->get_client_rect) ops->get_client_rect(context, item, &rect);
    else { rect.left = 0; rect.top = 0; rect.right = 0; rect.bottom = 0; }
    dc = ops && ops->get_dc ? ops->get_dc(context, item) : 0;
    if (ops && ops->select_object)
        ops->select_object(context, dc, stars_word(data, 0x26b6U));
    if (ops && ops->set_bk_mode) ops->set_bk_mode(context, dc, 2);
    if (ops && ops->set_bk_color)
        ops->set_bk_color(context, dc, stars_dword(data, 0x22baU));
    if (ops && ops->set_text_color)
        ops->set_text_color(context, dc, stars_dword(data, 0x25f6U));
    if (ops && ops->intersect_clip)
        ops->intersect_clip(context, dc, 0, 0, rect.right, rect.bottom);
    rect.top = (StarsU16)(rect.top - (StarsU16)partial);
    rect.bottom = (StarsU16)(rect.top + height);
    for (i = 0; i < 10U; ++i) {
        line = (StarsI16)(first + (StarsI16)i);
        if (line < 0) {
            if (ops && ops->offset_rect)
                ops->offset_rect(context, &rect, 0, (StarsI16)height);
            continue;
        }
        if (line > 0x4c) break;
        text = ops && ops->tutorial_string ?
            ops->tutorial_string(context, (StarsI16)(line + 0x277)) : 0;
        if (ops && ops->draw_centered)
            ops->draw_centered(context, dc, &rect, text, -1);
        if (ops && ops->offset_rect)
            ops->offset_rect(context, &rect, 0, (StarsI16)height);
    }
    rect.bottom = 1000;
    if (ops && ops->fill_rect)
        ops->fill_rect(context, dc, &rect, stars_word(data, 0x0010U));
    if (ops && ops->select_clip) ops->select_clip(context, dc, 0);
    if (ops && ops->release_dc) ops->release_dc(context, hwnd, dc);
    return 0;
}

/* NE 9:4448-4530, HbrGet. Last unused cache slot wins. A full cache returns
 * an uncached brush; reference counts are bytes and wrap from 255 to zero. */
StarsU16 stars_brush_get(StarsRuntime *r, StarsU32 color)
{
    StarsI16 i, empty = -1, count = stars_signed_word(stars_word(r->data, 0x13acU));
    StarsU16 brush;
    StarsU8 *d = r->data;
    for (i = 0; i < count; ++i) {
        if (!d[0x2518U + i]) empty = i;
        else if (stars_dword(d, (StarsU16)(0x2612U + i * 4U)) == color) {
            ++d[0x2518U + i];
            return stars_word(d, (StarsU16)(0x52c4U + i * 2U));
        }
    }
    brush = (StarsU16)stars_window(r, "CreateSolidBrush", 0, 0, 0, color, 0, 0);
    if (!brush) return 0;
    if (empty == -1) {
        if (count >= 32) return brush;
        empty = count;
        stars_put_word(d, 0x13acU, (StarsU16)(count + 1));
    }
    d[0x2518U + empty] = 1;
    stars_put_word(d, (StarsU16)(0x52c4U + empty * 2U), brush);
    stars_put_dword(d, (StarsU16)(0x2612U + empty * 4U), color);
    return brush;
}

/* NE 9:4532-459f, FreeHbr. Zero/untracked handles still reach DeleteObject. */
void stars_brush_free(StarsRuntime *r, StarsU16 brush)
{
    StarsI16 i, count = stars_signed_word(stars_word(r->data, 0x13acU));
    for (i = 0; i < count; ++i) {
        if (r->data[0x2518U + i] && stars_word(r->data, (StarsU16)(0x52c4U + i * 2U)) == brush) {
            --r->data[0x2518U + i];
            if (r->data[0x2518U + i]) return;
            break;
        }
    }
    stars_window(r, "DeleteObject", brush, 0, 0, 0, 0, 0);
}

/* FCreateStuff slice, NE 1:023d-02af (exclusive): rghbrPlanetAttr[3][2].
 * COLORREF uses low-byte red. Each environment has a dark/bright pair.
 * This is startup-only; FreeStuff releases all six cached references. */
void stars_create_planet_brushes(StarsRuntime *r)
{
    static const StarsU32 colors[6] = {
        0x7f0000UL, 0xff0000UL, 0x00007fUL, 0x0000ffUL,
        0x007f00UL, 0x00ff00UL};
    StarsU16 i;
    for (i = 0; i < 6; ++i)
        stars_put_word(r->data, (StarsU16)(0x5680U + i * 2U),
            stars_brush_get(r, colors[i]));
}

/* NE 4:08d2-0bad, FGetSystemColors. Returns success even if brush creation
 * fails. The two DIB palette edits store COLORREF's RGB bytes in BGR order. */
StarsI16 stars_system_colors(StarsRuntime *r)
{
    static const StarsU16 release[] = {0x10,0x14,0x16,0x12,0x1a,0x18,0x1c,0x1e};
    static const StarsU16 colors[][3] = {
        {15,0x22ba,0x10}, {20,0x3eec,0x14}, {16,0x23fe,0x16}, {18,0x25f6,0x12},
        {6,0,0x1c}, {1,0,0x1e}, {5,0x2574,0x18}, {8,0x48a6,0x1a}};
    static const StarsU16 metrics[][2] = {{4,0x505a}, {32,0x23ec}, {33,0x23ee}};
    static const StarsU16 palettes[][2] = {{0xc82,0x40c}, {0x598c,0x41c}};
    StarsU8 *d = r->data, *pixel;
    StarsU32 color, pointer;
    StarsU16 i, h, dc, bits, planes, offset;
    for (i = 0; i < 8; ++i)
        if (stars_word(d, release[i])) stars_brush_free(r, stars_word(d, release[i]));
    for (i = 0; i < 8; ++i) {
        color = stars_window(r, "GetSysColor", 0, colors[i][0], 0, 0, 0, 0);
        if (colors[i][1]) stars_put_dword(d, colors[i][1], color);
        stars_put_word(d, colors[i][2], stars_brush_get(r, color));
    }
    for (i = 0; i < 3; ++i)
        stars_put_word(d, metrics[i][1], (StarsU16)stars_window(r, "GetSystemMetrics", 0, metrics[i][0], 0, 0, 0, 0));
    for (i = 0; i < 2; ++i) {
        h = stars_word(d, palettes[i][0]);
        if (!h) continue;
        pointer = stars_window(r, "GlobalLock", h, 0, 0, 0, 0, 0);
        offset = (StarsU16)((StarsU16)pointer + palettes[i][1]);
        pixel = (StarsU8 *)r->platform->memory(r->context, (StarsU16)(pointer >> 16), offset);
        color = stars_dword(d, 0x22baU);
        pixel[2] = (StarsU8)color;
        pixel[1] = (StarsU8)(color >> 8);
        pixel[0] = (StarsU8)(color >> 16);
        stars_window(r, "GlobalUnlock", h, 0, 0, 0, 0, 0);
    }
    dc = (StarsU16)stars_window(r, "GetDC", 0, 0, 0, 0, 0, 0);
    bits = (StarsU16)stars_window(r, "GetDeviceCaps", dc, 12, 0, 0, 0, 0);
    planes = (StarsU16)stars_window(r, "GetDeviceCaps", dc, 14, 0, 0, 0, 0);
    stars_put_word(d, 0x5312U, (StarsU16)((StarsU32)bits * planes));
    stars_window(r, "ReleaseDC", 0, dc, 0, 0, 0, 0);
    return 1;
}

static void font_height(StarsRuntime *r, StarsU16 lf, StarsU16 dc, StarsU16 points)
{
    StarsU16 dpi = (StarsU16)stars_window(r, "GetDeviceCaps", dc, 90, 0, 0, 0, 0);
    StarsU16 height = (StarsU16)stars_window(r, "MulDiv", 0, points, dpi, 72, 0, 0);
    stars_put_word(r->data, lf, (StarsU16)(0U - height));
}
static void font_create(StarsRuntime *r, StarsU16 lf, StarsU16 name, StarsU16 out)
{
    strcpy((char *)r->data + lf + 18U, (char *)r->data + name);
    stars_put_word(r->data, out, (StarsU16)stars_window(r, "CreateFontIndirect", 0, 0, 0, 0, r->data + lf, 0));
}

/* NE 1:0ab2-0d6f, FCreateFonts. All nine font requests, selected-object
 * restoration, four metric reads and LocalFree are retained. Failures are
 * not checked by the original. LOGFONT and TEXTMETRIC retain Win16 layouts. */
StarsI16 stars_create_fonts(StarsRuntime *r, StarsU16 dc)
{
    static const StarsU16 measure[][2] = {{0x26b4,0x23fa}, {0x26be,0x23f6}, {0x26c0,0x23f8}, {0x4840,0x530a}};
    StarsU8 *d = r->data, metrics[31];
    StarsU16 lf, i, old;
    lf = (StarsU16)stars_window(r, "LocalAlloc", 0, 0x40U, 50, 0, 0, 0);
    for (i = 0; i < 4; ++i)
        if (!d[0x6a6U + i * 32U]) stars_copy_game_string(d, r->strings, (StarsI16)(0x537U + i), (StarsU16)(0x6a6U + i * 32U));
    font_height(r, lf, dc, 10);
    for (i = 0; i < 2; ++i) font_create(r, lf, (StarsU16)(0x6a6U + i * 32U), (StarsU16)(0x4840U + i * 2U));
    strcpy((char *)d + lf + 18U, (char *)d + 0x6a6U);
    font_height(r, lf, dc, 6);
    stars_put_word(d, 0x26c0U, (StarsU16)stars_window(r, "CreateFontIndirect", 0, 0, 0, 0, d + lf, 0));
    font_height(r, lf, dc, 7);
    stars_put_word(d, 0x26beU, (StarsU16)stars_window(r, "CreateFontIndirect", 0, 0, 0, 0, d + lf, 0));
    font_height(r, lf, dc, 8);
    for (i = 0; i < 4; ++i) font_create(r, lf, (StarsU16)(0x6a6U + i * 32U), (StarsU16)(0x26b4U + i * 2U));
    strcpy((char *)d + lf + 18U, (char *)d + 0x6c6U);
    stars_put_word(d, (StarsU16)(lf + 4U), 3150);
    stars_put_word(d, 0x26bcU, (StarsU16)stars_window(r, "CreateFontIndirect", 0, 0, 0, 0, d + lf, 0));
    old = (StarsU16)stars_window(r, "SelectObject", dc, stars_word(d, 0x26b4U), 0, 0, 0, 0);
    for (i = 0; i < 4; ++i) {
        if (i) stars_window(r, "SelectObject", dc, stars_word(d, measure[i][0]), 0, 0, 0, 0);
        stars_window(r, "GetTextMetrics", dc, 0, 0, 0, metrics, 0);
        stars_put_word(d, measure[i][1], (StarsU16)(stars_word(metrics, 0) + stars_word(metrics, 8)));
        if (!i) stars_put_word(d, 0x2610U, (StarsU16)stars_window(r, "GetTextExtent", dc, 10, 0, 0, d + 0x726U, 0));
    }
    stars_window(r, "SelectObject", dc, old, 0, 0, 0, 0);
    stars_window(r, "LocalFree", lf, 0, 0, 0, 0, 0);
    return 1;
}

/* NE 1:0eaa-101e, InitTiles. Convert line counts into pixel heights for the
 * two fixed tile arrays, resetting positions at each new low-three-bit group. */
void stars_init_tiles(StarsU8 *d)
{
    StarsU16 pass, i, base, count, group, y, p, flags, height;
    for (pass = 0; pass < 2; ++pass) {
        base = pass ? 0x90eU : 0x7fcU; count = pass ? 7U : 6U;
        group = 0; y = 4;
        for (i = 0; i < count; ++i) {
            p = (StarsU16)(base + i * 16U);
            flags = stars_word(d, (StarsU16)(p + 10U));
            if ((flags & 7U) != group) { y = 4; group = (StarsU16)(flags & 7U); }
            height = (StarsU16)(stars_word(d, (StarsU16)(p + 2U)) +
                (StarsU32)stars_word(d, p) * stars_word(d, 0x23faU));
            stars_put_word(d, (StarsU16)(p + 2U), height);
            stars_put_word(d, p, y);
            stars_put_word(d, (StarsU16)(p + 10U), (StarsU16)(flags & 0xe7ffU));
            y = (StarsU16)(y + ((flags & 0x80U) ? height + 4U : stars_word(d, 0x23faU) + 7U));
        }
    }
}

static void release_word(StarsRuntime *r, const char *op, StarsU16 offset)
{ stars_window(r, op, stars_word(r->data, offset), 0, 0, 0, 0, 0); }
static void release_bitmap(StarsRuntime *r, StarsU16 offset)
{
    release_word(r, "GlobalUnlock", offset);
    release_word(r, "FreeResource", offset);
}

/* NE 4:0bae-1210, FreeStuff. Destruction order is observable and intentional.
 * Only lpLog/lpMsg are cleared. Other handles and heap heads remain stale;
 * this is terminal cleanup, not an idempotent reset or a generic destructor.
 */
void stars_free_stuff(StarsMemory *m)
{
    /* Button/window colours, RGB, purple, tooltip; then mineral brushes. */
    static const StarsU16 conditional_brushes[] = {
        0x10,0x14,0x16,0x12,0x1a,0x18,0x1c,0x1e,0x24,0x26,0x28,0x2692,0x275a};
    /* Fake combo, CE, edit, list, host timer, browser callbacks. */
    static const StarsU16 instances[] = {0x2544,0x2748,0x2538,0x2402,0x51fe,0x51d0};
    static const StarsU16 cursors[] = {
        0x4a66,0x4b7a,0x5304,0x57a2,0x258a,0x3ee4,0x23f0,0x26c6,0x51ce,0x48ae,0x525e};
    /* Scanner, numbers, scan ship, unknown planet. */
    static const StarsU16 bitmaps[] = {0x4a5a,0x597a,0x4b7e,0x3efa};
    /* Planets, things, toolbar, races, transparent races, extra races. */
    static const StarsU16 dibs[] = {0x4952,0x242e,0x598c,0x5476,0x2606,0x2604};
    /* Ship, starbase, blue, enemy, selection, radar. */
    static const StarsU16 brushes[] = {0x5318,0x590c,0x599a,0x5996,0x2430,0x255a};
    static const StarsU16 greys_yellows[] = {0x20,0x22,0x22c6,0x519c};
    /* Ship, dark green/purple, starbase, enemy, mass path, radar. */
    static const StarsU16 pens[] = {0x65a2,0x5358,0x26c2,0x5980,0x568c,0x599e,0x5936};
    StarsRuntime *r = m->runtime;
    StarsU8 *d = r->data;
    StarsU16 i, cursor;
    for (i = 0; i < 13; ++i)
        if (stars_word(d, conditional_brushes[i])) stars_brush_free(r, stars_word(d, conditional_brushes[i]));
    for (i = 0; i < 5; ++i)
        if (stars_word(d, (StarsU16)(0x257cU + i * 2U))) stars_brush_free(r, stars_word(d, (StarsU16)(0x257cU + i * 2U)));
    for (i = 0; i < 6; ++i) stars_brush_free(r, stars_word(d, (StarsU16)(0x5680U + i * 2U)));
    for (i = 0; i < 8; ++i) stars_brush_free(r, stars_word(d, (StarsU16)(0x5692U + i * 2U)));
    for (i = 0; i < 6; ++i)
        stars_window(r, "FreeProcInstance", 0, 0, 0, stars_dword(d, instances[i]), 0, 0);
    if (stars_dword(d, 0x2aU)) stars_window(r, "FreeProcInstance", 0, 0, 0, stars_dword(d, 0x2aU), 0, 0);
    release_word(r, "DeleteObject", 0x525aU); /* hrgnHuge */
    release_word(r, "DeleteObject", 0x22beU); /* hrgnScratch */
    cursor = (StarsU16)stars_window(r, "LoadCursor", 0, 0, 0, 0x7f00U, 0, 0);
    stars_window(r, "SetCursor", cursor, 0, 0, 0, 0, 0);
    for (i = 0; i < 11; ++i) release_word(r, "DestroyCursor", cursors[i]);
    for (i = 0; i < 4; ++i) release_word(r, "DeleteObject", bitmaps[i]);
    for (i = 0; i < 3; ++i) release_word(r, "DestroyIcon", (StarsU16)(0x356U + i * 2U));
    for (i = 0; i < 7; ++i) release_word(r, "DestroyIcon", (StarsU16)(0x24e6U + i * 2U));
    for (i = 0; i < 6; ++i) release_bitmap(r, dibs[i]);
    release_word(r, "DeleteObject", 0x24e2U); /* build background */
    release_word(r, "DeleteObject", 0x260aU); /* message bitmap */
    release_word(r, "DeleteObject", 0x5992U); /* monochrome bitmap */
    release_word(r, "FreeResource", 0xc82U); /* plaque: no unlock here */
    for (i = 0; i < 5; ++i) {
        release_bitmap(r, (StarsU16)(0x4b6eU + i * 2U));
        release_bitmap(r, (StarsU16)(0x5160U + i * 2U));
    }
    for (i = 0; i < 7; ++i) release_bitmap(r, (StarsU16)(0x258eU + i * 2U));
    stars_free(m, stars_dword(d, 0x9a0U), 4); stars_put_dword(d, 0x9a0U, 0);
    stars_free(m, stars_dword(d, 0xaf8U), 2); stars_put_dword(d, 0xaf8U, 0);
    release_word(r, "DeleteObject", 0x5314U); /* main palette */
    if (stars_word(d, 0x42eU)) release_word(r, "DeleteObject", 0x42eU);
    for (i = 0; i < 6; ++i) stars_brush_free(r, stars_word(d, brushes[i]));
    if (stars_word(d, 0x568eU)) stars_brush_free(r, stars_word(d, 0x568eU));
    for (i = 0; i < 4; ++i) stars_brush_free(r, stars_word(d, greys_yellows[i]));
    release_word(r, "DeleteObject", 0x4b78U); /* 50% screen brush */
    for (i = 0; i < 3; ++i) release_word(r, "DeleteObject", (StarsU16)(0x591eU + i * 2U));
    release_word(r, "DeleteObject", 0x520aU); /* cargo */
    release_word(r, "DeleteObject", 0x5262U); /* dock */
    for (i = 0; i < 7; ++i) release_word(r, "DeleteObject", pens[i]);
    if (stars_word(d, 0x567eU)) release_word(r, "DeleteObject", 0x567eU);
    release_word(r, "DeleteObject", 0x5690U); /* dark blue pen */
    release_word(r, "DeleteObject", 0x26c4U); /* yellow pen */
    release_word(r, "DeleteObject", 0x23c8U); /* dark yellow pen */
    for (i = 0; i < 2; ++i) release_word(r, "DeleteObject", (StarsU16)(0x4840U + i * 2U));
    for (i = 0; i < 5; ++i) release_word(r, "DeleteObject", (StarsU16)(0x26b4U + i * 2U));
    release_word(r, "DeleteObject", 0x26c0U);
    release_word(r, "DeleteObject", 0x26beU);
    for (i = 0; i < 12; ++i) stars_heap_free(m, stars_dword(d, (StarsU16)(0xd34U + i * 4U)));
}
