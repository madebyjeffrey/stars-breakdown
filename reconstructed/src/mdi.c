/* Semantic reconstruction of the Win16 MDI class-registration bootstrap. */
#include "core.h"

static StarsMdiClassSpec mdi_class(StarsU16 style, StarsU16 proc_offset,
    StarsU16 proc_selector, StarsU16 background, StarsU16 name_offset,
    StarsU16 instance, StarsU16 cursor)
{
    StarsMdiClassSpec spec;

    spec.style = style;
    spec.window_proc.offset = proc_offset;
    spec.window_proc.selector = proc_selector;
    spec.class_extra = 0U;
    spec.window_extra = 0U;
    spec.instance = instance;
    spec.icon = 0U;
    spec.cursor = cursor;
    spec.background = background;
    spec.menu_name.offset = 0U;
    spec.menu_name.selector = 0U;
    spec.class_name.offset = name_offset;
    spec.class_name.selector = 0x1120U;
    return spec;
}

static StarsI16 register_mdi_class(const StarsInitMdiOps *ops, void *context,
    StarsMdiClassSpec *spec)
{
    if (!ops || !ops->register_class) return 0;
    return ops->register_class(context, spec) != 0U;
}

/* NE 5:0000-038B. Register the exact Win16 class sequence used by the MDI
 * application.  The callback receives a typed class record rather than a
 * byte/emission wrapper, so callers can supply USER32/Win16 services or a
 * native VC1.52 test harness without changing the recovered semantics. */
StarsI16 stars_init_mdi_app(StarsU16 h_instance,
    const StarsInitMdiOps *ops, void *context)
{
    StarsMdiClassSpec spec;
    StarsU16 cursor;
    StarsU16 brush;

    if (!ops || !ops->register_class) return 0;
    cursor = ops->load_cursor ?
        ops->load_cursor(context, 0U, 0x7f00U) : 0U;

    spec = mdi_class(0x000bU, 0x06d6U, 0x1020U, 0x000dU,
                     0x01d6U, h_instance, cursor);
    spec.menu_name.offset = 0x0364U;
    spec.menu_name.selector = 0x1120U;
    if (!register_mdi_class(ops, context, &spec)) return 0;

    brush = ops->get_stock_object ? ops->get_stock_object(context, 1U) : 0U;
    spec = mdi_class(0x020bU, 0x5c92U, 0x1030U, brush, 0x0202U,
                     h_instance, cursor);
    if (!register_mdi_class(ops, context, &spec)) return 0;

    brush = ops->get_stock_object ? ops->get_stock_object(context, 4U) : 0U;
    spec = mdi_class(0x020bU, 0x0032U, 0x1058U, brush, 0x01e2U,
                     h_instance, cursor);
    if (!register_mdi_class(ops, context, &spec)) return 0;

    brush = ops->get_stock_object ? ops->get_stock_object(context, 1U) : 0U;
    spec = mdi_class(0x020bU, 0x0000U, 0x1028U, brush, 0x01ecU,
                     h_instance, cursor);
    if (!register_mdi_class(ops, context, &spec)) return 0;

    brush = ops->get_stock_object ? ops->get_stock_object(context, 1U) : 0U;
    spec = mdi_class(0x0208U, 0x001eU, 0x1068U, brush, 0x0242U,
                     h_instance, cursor);
    if (!register_mdi_class(ops, context, &spec)) return 0;

    brush = ops->get_stock_object ? ops->get_stock_object(context, 1U) : 0U;
    spec = mdi_class(0x0200U, 0x0000U, 0x1048U, brush, 0x01f6U,
                     h_instance, cursor);
    if (!register_mdi_class(ops, context, &spec)) return 0;

    brush = ops->get_stock_object ? ops->get_stock_object(context, 0U) : 0U;
    spec = mdi_class(0x0a00U, 0x0000U, 0x10c0U, brush, 0x0210U,
                     h_instance, cursor);
    if (!register_mdi_class(ops, context, &spec)) return 0;

    brush = ops->get_stock_object ? ops->get_stock_object(context, 0U) : 0U;
    spec = mdi_class(0x0a00U, 0x19e4U, 0x1068U, brush, 0x024aU,
                     h_instance, cursor);
    if (!register_mdi_class(ops, context, &spec)) return 0;

    brush = ops->get_stock_object ? ops->get_stock_object(context, 1U) : 0U;
    spec = mdi_class(0x0200U, 0x2876U, 0x10d8U, brush, 0x021cU,
                     h_instance, cursor);
    if (!register_mdi_class(ops, context, &spec)) return 0;

    cursor = ops->load_cursor ?
        ops->load_cursor(context, 0U, 0x7f00U) : 0U;
    brush = ops->get_stock_object ? ops->get_stock_object(context, 4U) : 0U;
    spec = mdi_class(0U, 0x9126U, 0x1020U, brush, 0x022aU,
                     h_instance, cursor);
    if (!register_mdi_class(ops, context, &spec)) return 0;

    cursor = ops->load_cursor ?
        ops->load_cursor(context, 0U, 0x7f00U) : 0U;
    brush = ops->get_stock_object ? ops->get_stock_object(context, 1U) : 0U;
    spec = mdi_class(0x000bU, 0x0018U, 0x1108U, brush, 0x0236U,
                     h_instance, cursor);
    if (!register_mdi_class(ops, context, &spec)) return 0;

    return 1;
}

static StarsI16 command_game_ready(const StarsCommandState *state)
{
    return state && state->game_present && state->player >= 0;
}

static StarsI16 command_confirm(const StarsCommandOps *ops, void *context,
    StarsU16 tutor_active)
{
    if (!tutor_active || !ops || !ops->confirm_end_tutor) return 1;
    return ops->confirm_end_tutor(context) != 0;
}

static void command_open_or_new(StarsCommandState *state, StarsU16 hwnd,
    StarsU16 race_only, StarsU16 open, const StarsCommandOps *ops,
    void *context)
{
    StarsI16 result;

    if (!command_confirm(ops, context, state->tutor_active)) return;
    if (open) {
        result = ops && ops->open_game ?
            ops->open_game(context, hwnd, race_only) : 0;
    }
    else {
        result = ops && ops->new_game ?
            ops->new_game(context, hwnd, race_only) : 0;
    }
    if (result == 0) return;
    state->game_present = 1U;
    if (ops && ops->initialize_menu) ops->initialize_menu(context);
    if (ops && ops->create_children) ops->create_children(context);
    if (open && state->timer_id == 0U && ops && ops->post_message)
        ops->post_message(context, hwnd, 0xfa1U);
    if (open && state->player == 0 && ops && ops->start_tutor)
        ops->start_tutor(context);
}

static void command_report(StarsCommandState *state, StarsU16 hwnd,
    StarsU16 command, const StarsCommandOps *ops, void *context)
{
    StarsU16 kind;

    if (!command_game_ready(state)) return;
    if (command == 0x8feU) {
        if (state->report_kind == 0U) kind = 0x8fdU;
        else if (state->report_kind == 1U) kind = 0x8ffU;
        else if (state->report_kind == 2U) kind = 0x900U;
        else kind = 0x901U;
    }
    else kind = command;
    if (ops && ops->select_report) ops->select_report(context, kind);
    state->report_kind = kind == 0x8fdU ? 1U :
        (kind == 0x8ffU ? 2U : (kind == 0x900U ? 3U : 4U));
    (void)hwnd;
}

/* Semantic reconstruction of NE 5:2F7A-5375.  CommandHandler has no
 * independent computation: it validates global state, chooses a service,
 * updates a small amount of menu state, and returns.  This implementation
 * makes each of those original service boundaries explicit and keeps the
 * command IDs in one readable dispatch table. */
void stars_mdi_command_handler(StarsCommandState *state, StarsU16 hwnd,
    StarsU16 command, const StarsCommandOps *ops, void *context)
{
    StarsU16 i;

    if (!state) return;
    if (command > 14999U && command < 0x3afcU) {
        if (ops && ops->set_menu_selection)
            ops->set_menu_selection(context, (StarsU16)(command + 0xc568U));
        return;
    }
    if (command == 0x53U || command == 0x54U || command == 0x55U) {
        if (ops && ops->dump) ops->dump(context, (StarsU16)(command - 0x53U));
        return;
    }
    if (command == 0x5fU || command == 0x60U) {
        if (state->player < 0) return;
        if (ops && ops->dialog) ops->dialog(context, 0x66U, hwnd);
        return;
    }
    if (command == 99U) {
        if (ops && ops->dialog) ops->dialog(context, 0x5aU, hwnd);
        return;
    }
    if (command == 0x67U || command == 0x68U) {
        if (state->selected_object_kind == 2U && !state->order_edit_focus &&
            ops && ops->delete_waypoint)
            ops->delete_waypoint(context, (StarsU16)(command == 0x67U));
        if (ops && ops->post_message) ops->post_message(context, hwnd, 0x111U);
        return;
    }
    if (command == 0x69U || command == 21000U || command == 0x5209U) {
        if (!command_confirm(ops, context, state->tutor_active)) return;
        if (state->player < 0) return;
        if (state->tutor_active && ops && ops->advance_tutor)
            ops->advance_tutor(context);
        if (ops && ops->show_tutor && state->tutor_active)
            ops->show_tutor(context, 0U);
        if (ops && ops->ensure_ais) ops->ensure_ais(context);
        if (ops && ops->generate_turn) ops->generate_turn(context);
        state->dirty = 0U;
        return;
    }
    if (command == 0x6dU || command == 0xed9U) {
        command_open_or_new(state, hwnd, 0U, 1U, ops, context);
        return;
    }
    if (command == 0x6eU || command == 0xed8U) {
        command_open_or_new(state, hwnd, 0U, 0U, ops, context);
        return;
    }
    if (command == 0x71U) {
        if (!command_confirm(ops, context, state->tutor_active)) return;
        if (ops && ops->write_ini) ops->write_ini(context);
        if (ops && ops->destroy_game) ops->destroy_game(context);
        state->game_present = 0U;
        state->player = -1;
        state->dirty = 0U;
        if (ops && ops->initialize_menu) ops->initialize_menu(context);
        if (ops && ops->close_frame) ops->close_frame(context, hwnd);
        return;
    }
    if (command == 0x7dU || command == 0x89U) {
        if (!command_game_ready(state)) return;
        if (ops && ops->dialog) ops->dialog(context, 0x7dU, hwnd);
        return;
    }
    if (command == 0x7eU || command == 0x87U) {
        if (!command_game_ready(state)) return;
        if (ops && ops->dialog) ops->dialog(context, 0x7fU, hwnd);
        return;
    }
    if (command == 0x81U || command == 0x9cU || command == 0x9dU) {
        if ((command == 0x9cU || command == 0x9dU) &&
            !command_game_ready(state)) return;
        if (ops && ops->dialog) ops->dialog(context, 0x81U, hwnd);
        return;
    }
    if (command >= 0x82U && command <= 0x84U) {
        if (state->player < 0) return;
        state->layout = (StarsU16)(command - 0x82U);
        if (ops && ops->invalidate_frame) ops->invalidate_frame(context, 1U);
        if (ops && ops->ensure_tile_size)
            ops->ensure_tile_size(context, (StarsU16)(state->layout == 2U));
        if (ops && ops->refit) ops->refit(context);
        return;
    }
    if (command == 0x88U || command == 0x100U) {
        if (!command_game_ready(state)) return;
        state->browser_window = (StarsU16)!state->browser_window;
        if (ops && ops->dialog)
            ops->dialog(context, state->browser_window ? 0x80U : 0U, hwnd);
        return;
    }
    if (command == 0x8aU || command == 0x101U) {
        if (ops && ops->help) ops->help(context, 3U);
        return;
    }
    if (command == 0x9eU || command == 0x9fU) {
        if (!command_game_ready(state)) return;
        if (ops && ops->new_game) ops->new_game(context, hwnd, 1U);
        return;
    }
    if (command == 0xb3U) {
        state->scan_flags ^= 0x8000U;
        if (ops && ops->toggle_toolbar)
            ops->toggle_toolbar(context, (StarsU16)(state->scan_flags & 0x8000U));
        if (ops && ops->refit) ops->refit(context);
        return;
    }
    if (command == 0xd5U) {
        if (state->player >= 0 && ops && ops->dialog)
            ops->dialog(context, 0xd6U, hwnd);
        return;
    }
    if (command >= 0xfaU && command <= 0xfdU) {
        if (state->title_window && ops && ops->post_message)
            ops->post_message(context, state->title_window,
                (StarsU16)(command - 0xfaU));
        return;
    }
    if (command == 0x10eU) {
        if (!command_game_ready(state)) return;
        if (state->overlap_mode && !state->salt_valid && !state->salt_current)
            return;
        if (ops && ops->dialog) ops->dialog(context, 0x8dU, hwnd);
        return;
    }
    if (command == 0x428U) command = 0xedbU;
    if (command == 0x7d9U || command == 0x7deU) {
        if (!command_game_ready(state) || state->overlap_mode) return;
        if (ops && ops->dialog) ops->dialog(context, 0x7d8U, hwnd);
        return;
    }
    if (command == 0x7dbU || command == 0x7dcU) {
        if (!command_game_ready(state)) return;
        if (ops && ops->dialog) ops->dialog(context, 0x7ddU, hwnd);
        return;
    }
    if (command >= 0x8fdU && command <= 0x901U) {
        command_report(state, hwnd, command, ops, context);
        return;
    }
    if (command == 0x98dU) {
        state->scan_flags ^= 0x2000U;
        if (ops && ops->toggle_scanner_grid)
            ops->toggle_scanner_grid(context, (StarsU16)(state->scan_flags & 0x2000U));
        return;
    }
    if (command == 0x9c1U || command == 0x9c2U) {
        if (ops && ops->help)
            ops->help(context, command == 0x9c2U ? 0x1195U : 0x32caU);
        return;
    }
    if (command == 0x9c4U || command == 0x9c5U) {
        if (state->tutor_active) {
            if (ops && ops->show_tutor) ops->show_tutor(context, 1U);
        }
        else if (ops && ops->start_tutor) ops->start_tutor(context);
        return;
    }
    if (command == 0xee2U) {
        if (ops && ops->close_frame) ops->close_frame(context, hwnd);
        return;
    }
    if (command >= 0xf3dU && command <= 0xf45U) {
        if (!state->scanner_window) {
            if (ops && ops->dialog) ops->dialog(context, 0x0fU, hwnd);
            return;
        }
        i = (StarsU16)(command - 0xf41U);
        state->scan_zoom = i;
        if (ops && ops->set_scan_zoom) ops->set_scan_zoom(context, i, command);
        if (ops && ops->invalidate_frame) ops->invalidate_frame(context, 1U);
        return;
    }
    if (command == 0xfa1U) {
        if (!state->scanner_window) return;
        if (ops && ops->restore_selection) ops->restore_selection(context, 1U);
        if (ops && ops->refit) ops->refit(context);
        if (ops && ops->invalidate_frame) ops->invalidate_frame(context, 1U);
        return;
    }
    if (command == 0x1068U || command == 0x1069U) {
        if (state->player >= 0 && ops && ops->find_dialog)
            ops->find_dialog(context, command);
        return;
    }
    if (command >= 0x10ccU && command <= 0x10d4U) {
        if (command_game_ready(state) && ops && ops->load_mru_game)
            ops->load_mru_game(context, (StarsU16)(command - 0x10ccU));
        return;
    }
    if (command == 0x6aU || command == 0x6cU ||
        command == 0xedaU || command == 0xedbU || command == 0x7daU) {
        if (!state->scanner_window) return;
        if (state->player < 0) return;
        if (!state->new_turn_available) {
            if (ops && ops->post_message) ops->post_message(context, hwnd, command);
            return;
        }
        if (ops && ops->generate_turn) ops->generate_turn(context);
        state->dirty = 0U;
        return;
    }
}
