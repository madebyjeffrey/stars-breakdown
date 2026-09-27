/* Semantic reconstruction of the Win16 application entry point. */
#include <string.h>
#include "core.h"

static void winmain_alert(const StarsWinMainOps *ops, void *context)
{
    if (ops && ops->alert_ids) ops->alert_ids(context, 0x0018U, 0x0010);
}

static char winmain_lower(char ch)
{
    if (ch >= 'A' && ch <= 'Z') return (char)(ch + ('a' - 'A'));
    return ch;
}

static int winmain_digit(char ch)
{
    return ch >= '0' && ch <= '9';
}

static const StarsU8 *winmain_copy_token(char *output, unsigned capacity,
    const StarsU8 *input)
{
    unsigned length;

    length = 0U;
    while (*input != '\0' && *input != ' ') {
        if (length + 1U < capacity) output[length] = (char)*input;
        ++length;
        ++input;
    }
    if (capacity != 0U) {
        if (length >= capacity) output[capacity - 1U] = '\0';
        else output[length] = '\0';
    }
    return input;
}

static void winmain_parse_options(StarsWinMainState *state,
    const StarsU8 *command_line, const StarsWinMainOps *ops, void *context)
{
    const StarsU8 *cursor;
    const StarsU8 *end;
    const StarsU8 *digits;
    unsigned value;
    unsigned consumed;
    char option;
    char mode;

    cursor = command_line ? command_line : (const StarsU8 *)"";
    while (*cursor != '\0') {
        while (*cursor == ' ') ++cursor;
        if (*cursor == '\0') break;
        if (*cursor == '-' || *cursor == '/') {
            ++cursor;
            option = winmain_lower((char)*cursor);
            if (option == 'a') {
                state->ini_flags = (StarsU16)((state->ini_flags & 0xfbffU) |
                    0x0400U);
            } else if (option == 'b') {
                ++cursor;
                while (*cursor == ' ') ++cursor;
                end = winmain_copy_token(state->base, sizeof(state->base),
                    cursor);
                if (ops && ops->set_up_batch_processing &&
                    ops->set_up_batch_processing(context)) {
                    state->ini_flags = (StarsU16)((state->ini_flags & 0xfdf4U) |
                        0x020bU);
                }
                cursor = end - 1;
            } else if (option == 'c') {
                state->ini_flags = (StarsU16)((state->ini_flags & 0xfffdU) |
                    (state->base[0] != '\0' ? 2U : 0U));
            } else if (option == 'd') {
                ++cursor;
                consumed = 0U;
                while (*cursor != '\0' && *cursor != ' ') {
                    mode = winmain_lower((char)*cursor);
                    if (mode == 'f') {
                        state->ini_flags = (StarsU16)((state->ini_flags & 0xf7ffU) |
                            0x0800U);
                    } else if (mode == 'm') {
                        state->ini_flags = (StarsU16)((state->ini_flags & 0xdfffU) |
                            0x2000U);
                    } else if (mode == 'p') {
                        state->ini_flags = (StarsU16)((state->ini_flags & 0xefffU) |
                            0x1000U);
                    }
                    ++cursor;
                    consumed = 1U;
                }
                if (consumed != 0U) --cursor;
                else --cursor;
            } else if (option == 'g') {
                digits = cursor + 1;
                value = 0U;
                while (winmain_digit((char)*digits)) {
                    value = value * 10U + (unsigned)(*digits - '0');
                    ++digits;
                    if (value >= 1000U) {
                        value = 1000U;
                        while (winmain_digit((char)*digits)) ++digits;
                        break;
                    }
                }
                state->ini_flags = (StarsU16)((state->ini_flags & 0xfff7U) |
                    8U);
                if (value != 0U) state->turn_generation_count =
                    (StarsI16)(value - 1U);
                if (digits != cursor + 1) cursor = digits - 1;
                else --cursor;
            } else if (option == 'h') {
                state->global_low_flags = (StarsU16)((state->global_low_flags &
                    0xfdffU) | 0x0200U);
            } else if (option == 'l') {
                state->ini_flags = (StarsU16)((state->ini_flags & 0x7fffU) |
                    0x8000U);
            } else if (option == 'p') {
                ++cursor;
                while (*cursor == ' ') ++cursor;
                end = winmain_copy_token(state->pass_last,
                    sizeof(state->pass_last), cursor);
                if (ops && ops->salt_from_string)
                    state->salt_last = ops->salt_from_string(context,
                        (const StarsU8 *)state->pass_last);
                cursor = end - 1;
            } else if (option == 't') {
                state->ini_flags = (StarsU16)((state->ini_flags & 0xffefU) |
                    0x0010U);
            } else if (option == 'v') {
                state->ini_flags = (StarsU16)((state->ini_flags & 0xbfffU) |
                    0x4000U);
            } else if (option == 'w') {
                state->ini_flags = (StarsU16)((state->ini_flags & 0xfffbU) |
                    4U);
            } else if (option == 'x') {
                state->global_high_flags = (StarsU16)((state->global_high_flags &
                    0xffbfU) | 0x0040U);
            }
            ++cursor;
        } else {
            cursor = winmain_copy_token(state->base, sizeof(state->base), cursor);
            state->ini_flags = (StarsU16)((state->ini_flags & 0xfffcU) | 3U);
        }
    }
}

static StarsI16 winmain_dispatch_messages(StarsWinMainState *state,
    const StarsWinMainOps *ops, void *context)
{
    StarsWinMainMessage message;
    StarsI16 translated;
    StarsI16 handled;
    StarsI16 result;

    if (!ops || !ops->get_message) return 0;
    message.hwnd = 0U;
    message.message = 0U;
    message.wparam = 0U;
    message.lparam = 0UL;
    result = ops->get_message(context, &message);
    while (result != 0) {
        if (state->title != 0U) {
            translated = ops->translate_accelerator ?
                ops->translate_accelerator(context, state->frame,
                    state->accel_title, &message) : 0;
            if (!translated) {
                if (ops->translate_message) ops->translate_message(context,
                    &message);
                if (ops->dispatch_message) ops->dispatch_message(context,
                    &message);
            }
        } else {
            /* The original checks iconic state here, but both branches enter
             * the same frame-accelerator path.  Retain the USER query. */
            if (ops->is_iconic) (void)ops->is_iconic(context, state->frame);
            translated = ops->translate_accelerator ?
                ops->translate_accelerator(context, state->frame,
                    state->accel_frame, &message) : 0;
            if (!translated) {
                if (ops->translate_message) ops->translate_message(context,
                    &message);
                handled = 0;
                if (message.message == 0x0100U || message.message == 0x0101U) {
                    if (ops->handle_key) handled = ops->handle_key(context,
                        message.hwnd, message.message, message.wparam,
                        message.lparam);
                } else if (message.message == 0x0102U) {
                    if (ops->handle_char) handled = ops->handle_char(context,
                        message.hwnd, message.wparam, message.lparam);
                }
                if (!handled && ops->dispatch_message)
                    ops->dispatch_message(context, &message);
                else if (handled) {
                    result = ops->get_message(context, &message);
                    continue;
                }
            }
        }
        result = ops->get_message(context, &message);
    }
    if (ops->free_stuff) ops->free_stuff(context);
    return message.wparam;
}

/* NE 4:0000-06A3.  This is the actual startup sequence and command-line
 * parser, with Win16 services represented by typed callbacks. */
StarsI16 stars_win_main(StarsWinMainState *state, StarsU16 instance,
    StarsU16 previous_instance, const StarsU8 *command_line,
    StarsI16 show_command, const StarsWinMainOps *ops, void *context)
{
    StarsU32 initial_salt;
    StarsU32 tick;
    StarsI16 message_result;

    if (!state || !ops) return 0;
    initial_salt = state->salt_last;
    memset(state, 0, sizeof(*state));
    state->instance = instance;
    state->salt_last = initial_salt;
    state->vtimer_auto_generate = 1U;

    if (previous_instance == 0U) {
        if (!ops->init_mdi || !ops->init_mdi(context)) {
            winmain_alert(ops, context);
            return 0;
        }
    }
    tick = ops->get_tick ? ops->get_tick(context) : 0UL;
    if (ops->randomize2) ops->randomize2(context, tick);
    if (!ops->create_stuff || !ops->create_stuff(context)) return 0;
    if (!ops->get_system_colors || !ops->get_system_colors(context)) {
        winmain_alert(ops, context);
        return 0;
    }
    if (!ops->init_instance || !ops->init_instance(context, show_command, state)) {
        winmain_alert(ops, context);
        return 0;
    }
    winmain_parse_options(state, command_line, ops, context);
    if (ops->post_message)
        ops->post_message(context, state->frame, 0x0464U, 0U, 0UL);
    message_result = winmain_dispatch_messages(state, ops, context);
    return message_result;
}
