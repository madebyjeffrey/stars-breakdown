/* Typed semantic reconstruction of TitleWndProc.  The older title.c module
 * retains the shared title helpers; this file contains the window dispatcher. */
#include <string.h>
#include "core.h"

static StarsU32 title_default(const StarsTitleOps *ops, void *cx,
    StarsU16 hwnd, StarsU16 msg, StarsU16 wp, StarsU32 lp)
{ return ops->default_proc ? ops->default_proc(cx, hwnd, msg, wp, lp) : 0UL; }

static void title_destroy_resources(StarsTitleState *s, const StarsTitleOps *o,
    void *cx)
{
    if (s->dib_title != 0U) {
        if (o->unlock_resource) o->unlock_resource(cx, s->dib_title);
        if (o->free_resource) o->free_resource(cx, s->dib_title);
        s->dib_title = 0U;
    }
}

static void title_close_window(StarsTitleState *s, StarsU16 hwnd,
    const StarsTitleOps *o, void *cx)
{
    if (s->freeing_title == 0U) {
        s->freeing_title = 1U;
        if (o->destroy_window)
            o->destroy_window(cx, s->title != 0U ? s->title : hwnd);
        s->title = 0U;
    }
}

static void title_create_buttons(StarsTitleState *s, StarsU16 hwnd,
    const StarsTitleOps *o, void *cx)
{
    StarsRect r;
    StarsI16 width, height, button_width, button_height, gap, x, i;
    const StarsU8 *text;

    if (!o->get_client_rect || !o->create_window) return;
    o->get_client_rect(cx, hwnd, &r);
    width = (StarsI16)r.right; height = (StarsI16)r.bottom;
    button_width = (StarsI16)(height / 8);
    if (button_width < 120) button_width = 120;
    if (width < 650) button_width = (StarsI16)(button_width + button_width / 6);
    gap = (StarsI16)((width - button_width * 4) / 4);
    x = (StarsI16)((width - button_width * 4) / 8);
    button_height = height < 500 ? (StarsI16)(s->dy_arial8 * 2) :
        (StarsI16)((s->dy_arial8 * 5) / 2);
    for (i = 0; i < 4; ++i) {
        text = o->get_string ? o->get_string(cx, (StarsU16)(0x01dfU + i)) :
            (const StarsU8 *)"";
        s->buttons[i] = o->create_window(cx, (const StarsU8 *)"BUTTON", text,
            0x50000000UL, x, (StarsI16)(width - button_height -
            (s->dy_arial8 * 5) / 2), button_width, button_height, hwnd,
            (StarsU16)i, s->instance);
        if (i == 2 && (s->base_name == 0 || s->base_name[0] == '\0' ||
            !o->access || o->access(cx, s->base_name, 0) == -1))
            if (o->enable_window) o->enable_window(cx, s->buttons[2], 0);
        if (height < 500 && o->send_message)
            o->send_message(cx, s->buttons[i], 0x0030U, s->font_arial8, 0UL);
        x = (StarsI16)(x + button_width + gap);
    }
}

static void title_paint(StarsTitleState *s, StarsU16 hwnd,
    const StarsTitleOps *o, void *cx)
{
    StarsRect paint, client, text_rect, button_rect;
    StarsU16 dc, font, old_font, metric_x, metric_y;
    StarsI16 top, length;
    void *memory;
    const StarsU8 *text;

    if (!o->begin_paint || !o->end_paint) return;
    if (o->is_iconic && o->is_iconic(cx, hwnd)) {
        dc = o->begin_paint(cx, hwnd, &paint);
        if (o->draw_icon) o->draw_icon(cx, dc, 2, 2, s->icon_stars);
        o->end_paint(cx, hwnd, &paint);
        return;
    }
    memory = o->local_alloc ? o->local_alloc(cx, 0x40U, 0x32U) : 0;
    dc = o->begin_paint(cx, hwnd, &paint);
    if (o->select_object) (void)o->select_object(cx, dc, s->button_face);
    if (o->get_client_rect) o->get_client_rect(cx, hwnd, &client);
    else memset(&client, 0, sizeof(client));
    if (s->screen_colors < 8U || s->dib_title == 0U) {
        if (s->screen_colors >= 8U && o->set_text_color)
            o->set_text_color(cx, dc, 0x02ff7fffUL);
        if (o->draw_stars) o->draw_stars(cx, dc, &client);
        font = o->create_font ? o->create_font(cx,
            (StarsI16)(-(StarsI16)client.bottom / 3),
            (const StarsU8 *)"Arial") : 0U;
        if (o->set_text_color) o->set_text_color(cx, dc, 0x009b009bUL);
        if (font != 0U && o->select_object)
            old_font = o->select_object(cx, dc, font);
        if (o->set_bk_mode) o->set_bk_mode(cx, dc, 1);
        text = o->get_string ? o->get_string(cx, 0x0441U) :
            (const StarsU8 *)"Stars!";
        text_rect = client;
        text_rect.bottom = (StarsU16)((client.bottom * 3) / 4);
        if (o->draw_centered) o->draw_centered(cx, dc, &text_rect, text, 6);
        if (font != 0U && o->select_object) (void)o->select_object(cx, dc, old_font);
        if (font != 0U && o->delete_object) o->delete_object(cx, font);
    } else {
        if (o->select_palette) (void)o->select_palette(cx, dc, s->palette_splash, 0);
        if (o->realize_palette) (void)o->realize_palette(cx, dc);
        metric_y = o->system_metric ? (StarsU16)o->system_metric(cx, 1) : 0U;
        metric_x = o->system_metric ? (StarsU16)o->system_metric(cx, 0) : 0U;
        if (o->dib_blt) o->dib_blt(cx, dc, 0, 0, (StarsI16)metric_x,
            (StarsI16)metric_y, s->dib_title, 0, 0, 800, 600, 0xcc000020UL);
    }
    if (o->select_object) (void)o->select_object(cx, dc, s->font_arial10);
    if (o->set_bk_mode) o->set_bk_mode(cx, dc, 1);
    if (o->version) o->version(cx, s->work, (StarsU16)sizeof(s->work));
    text_rect = client;
    if (o->get_window_rect) o->get_window_rect(cx, s->buttons[0], &button_rect);
    else memset(&button_rect, 0, sizeof(button_rect));
    top = (StarsI16)(button_rect.top - (s->dy_arial8 * 9) / 2);
    text_rect.top = (StarsU16)top;
    text_rect.bottom = (StarsU16)(top + (s->dy_arial8 * 3) / 2);
    length = (StarsI16)strlen(s->work);
    if (o->draw_centered) o->draw_centered(cx, dc, &text_rect,
        (const StarsU8 *)s->work, length);
    o->end_paint(cx, hwnd, &paint);
    if (o->local_free && memory) o->local_free(cx, memory);
}

StarsU32 stars_title_wnd_proc(StarsTitleState *s, StarsU16 hwnd,
    StarsU16 msg, StarsU16 wp, StarsU32 lp, const StarsTitleOps *o, void *cx)
{
    StarsI16 opened;
    StarsU16 dc, old_palette, changed;

    if (!s || !o) return 0UL;
    if (msg == 1U) {
        if (s->screen_colors > 7U && o->load_big_resource && s->dib_title == 0U) {
            s->dib_title = o->load_big_resource(cx, 0x01c1U);
            if (s->palette_splash == 0U && s->dib_title != 0U &&
                o->palette_from_dib)
                s->palette_splash = o->palette_from_dib(cx, s->dib_title);
        }
        title_create_buttons(s, hwnd, o, cx);
        return 0UL;
    }
    if (msg == 2U) {
        title_destroy_resources(s, o, cx);
        if (s->freeing_title == 0U) {
            if ((s->global_high_flags & 0x0040U) == 0U) {
                if (o->write_ini) o->write_ini(cx);
                if (o->post_quit) o->post_quit(cx, s->exit_value);
            } else if (o->exit_windows) o->exit_windows(cx, s->exit_value);
        }
        return title_default(o, cx, hwnd, msg, wp, lp);
    }
    if (msg == 0x000fU) { title_paint(s, hwnd, o, cx); return 0UL; }
    if (msg == 0x0111U) {
        if (wp == 0U) {
            if (o->new_game_wizard) o->new_game_wizard(cx, hwnd, 0U);
            if (!s->planet_present && s->game_lid == 0UL) {
                if (o->set_focus) o->set_focus(cx, hwnd);
            } else {
                title_close_window(s, hwnd, o, cx);
                if (o->show_window) o->show_window(cx, s->frame, 5U);
            }
        } else if (wp == 3U) {
            if ((s->global_high_flags & 0x0040U) == 0U) {
                if (o->write_ini) o->write_ini(cx);
                if (o->post_quit) o->post_quit(cx, s->exit_value);
            } else if (o->exit_windows) o->exit_windows(cx, s->exit_value);
            return 0UL;
        } else if (wp == 1U || wp == 2U) {
            if (wp == 2U) s->game_flags = (StarsU16)(s->game_flags | 1U);
            opened = o->open_game ? o->open_game(cx, hwnd, 0) : 0;
            if (opened < 1) {
                if (o->set_focus) o->set_focus(cx, hwnd);
            } else {
                title_close_window(s, hwnd, o, cx);
                if (s->id_player != -1 && o->show_window)
                    o->show_window(cx, s->frame, 5U);
                if (o->initialize_menu) o->initialize_menu(cx, 0U);
                if (o->post_message) o->post_message(cx, s->frame, 0x0111U,
                    0x0fa1U, 0UL);
                if ((s->game_flags & 8U) != 0U && s->id_player == 0 &&
                    o->start_tutor) o->start_tutor(cx, 0);
            }
            s->game_flags = (StarsU16)(s->game_flags & 0xfffeU);
        } else return 0UL;
        return 0UL;
    }
    if (msg == 0x0311U && wp == hwnd) return 0UL;
    if ((msg == 0x030fU || msg == 0x0311U) && s->screen_colors > 7U) {
        if (!o->get_dc) return 0UL;
        dc = o->get_dc(cx, hwnd);
        old_palette = o->select_palette ? o->select_palette(cx, dc,
            s->palette_splash, 0) : 0U;
        changed = o->realize_palette ? o->realize_palette(cx, dc) : 0U;
        if (o->select_palette) (void)o->select_palette(cx, dc, old_palette, 0);
        if (o->release_dc) o->release_dc(cx, hwnd, dc);
        if (changed != 0U) {
            if (o->invalidate_rect) o->invalidate_rect(cx, hwnd, 1);
            return 1UL;
        }
        return 0UL;
    }
    return title_default(o, cx, hwnd, msg, wp, lp);
}
