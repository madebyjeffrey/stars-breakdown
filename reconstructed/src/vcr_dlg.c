#include "core.h"

#define VCRDLG_DESTROY 2U
#define VCRDLG_PAINT 0x000fU
#define VCRDLG_ERASE 0x0014U
#define VCRDLG_INIT 0x0110U
#define VCRDLG_COMMAND 0x0111U
#define VCRDLG_TIMER 0x0113U
#define VCRDLG_LBUTTON 0x0201U
#define VCRDLG_RBUTTON 0x0204U
#define VCRDLG_FIRST_STEP 0x00a1U
#define VCRDLG_LAST_STEP 0x00a5U
#define VCRDLG_OK 1U
#define VCRDLG_CANCEL 2U
#define VCRDLG_HELP 0x0076U
#define VCRDLG_TIMER_ID 0x0a6cU
#define VCRDLG_MIN_SPEED 0
#define VCRDLG_MAX_SPEED 4
#define VCRDLG_CELL_MARGIN 8

static StarsI16 vcrdlg_x(StarsU32 value)
{ return (StarsI16)(value & 0xffffU); }
static StarsI16 vcrdlg_y(StarsU32 value)
{ return (StarsI16)((value >> 16) & 0xffffU); }

static StarsU16 vcrdlg_square(const StarsVcrDlgInput *input,
    StarsI16 x, StarsI16 y)
{
    StarsI16 col;
    StarsI16 row;
    if (x < VCRDLG_CELL_MARGIN || y < VCRDLG_CELL_MARGIN) return 0xffffU;
    col = (StarsI16)((x - VCRDLG_CELL_MARGIN) / (input->board_square + 3));
    row = (StarsI16)((y - VCRDLG_CELL_MARGIN) / (input->board_square + 3));
    if (col < 0 || col > 9 || row < 0 || row > 9) return 0xffffU;
    return (StarsU16)((row << 4) | col);
}

static void vcrdlg_step(StarsVcrDlgInput *input, StarsI16 step)
{
    if (step < -1) step = -1;
    if (step > input->final_step) step = input->final_step;
    input->current_step = input->ops.set_step ?
        input->ops.set_step(input->context, step) : step;
    if (input->ops.draw) input->ops.draw(input->context, -2, -1);
}

/* Genuine semantic reconstruction of VCRDlg, NE 30:0E90-193E.
 * The window procedure owns the VCR lifecycle: it sizes the controls after
 * initialization, paints the board, translates step/play/stop commands into
 * SetVCRBoard transitions, advances playback from the timer, changes speed
 * through the spin controls, selects board squares, opens the token popup,
 * dispatches Help, and stops animation before closing. */
StarsI16 stars_vcr_dlg(StarsU16 hwnd, StarsU16 message, StarsU16 wparam,
    StarsU32 lparam, StarsVcrDlgInput *input)
{
    StarsI16 command;
    StarsU16 square;
    if (!input) return 0;
    if (message == VCRDLG_DESTROY) {
        input->animating = 0;
        if (input->ops.kill_timer) input->ops.kill_timer(input->context);
        if (input->ops.end_dialog) input->ops.end_dialog(input->context);
        return 0;
    }
    if (message == VCRDLG_PAINT) {
        if (input->ops.paint) input->ops.paint(input->context);
        if (input->ops.draw) input->ops.draw(input->context, -1, -1);
        return 1;
    }
    if (message == VCRDLG_ERASE) {
        if (input->ops.fill_background) input->ops.fill_background(
            input->context, hwnd);
        return 1;
    }
    if (message == VCRDLG_INIT) {
        input->hwnd = hwnd;
        if (input->ops.resize) input->ops.resize(input->context, hwnd);
        if (input->ops.refresh_speed) input->ops.refresh_speed(input->context);
        return 1;
    }
    if (message == VCRDLG_TIMER) {
        if (!input->animating) return 0;
        if (input->current_step >= input->final_step) {
            input->animating = 0;
            if (input->ops.kill_timer) input->ops.kill_timer(input->context);
        } else vcrdlg_step(input, (StarsI16)(input->current_step + 1));
        return 0;
    }
    if (message == VCRDLG_COMMAND) {
        command = (StarsI16)(wparam - VCRDLG_FIRST_STEP);
        if (wparam == VCRDLG_OK || wparam == VCRDLG_CANCEL) {
            input->animating = 0;
            if (input->ops.kill_timer) input->ops.kill_timer(input->context);
            if (input->ops.end_dialog) input->ops.end_dialog(input->context);
            return 1;
        }
        if (wparam == VCRDLG_HELP) {
            if (input->ops.help) input->ops.help(input->context);
            return 1;
        }
        if (command < 0 || command > 4) return 0;
        input->animating = 0;
        if (input->ops.kill_timer) input->ops.kill_timer(input->context);
        if (command == 0) vcrdlg_step(input, -1);
        else if (command == 1) vcrdlg_step(input,
            (StarsI16)(input->current_step - 1));
        else if (command == 2) {
            input->animating = 1;
            if (input->ops.set_timer) input->ops.set_timer(input->context,
                input->speed, VCRDLG_TIMER_ID);
        } else if (command == 3) vcrdlg_step(input,
            (StarsI16)(input->current_step + 1));
        else vcrdlg_step(input, input->final_step);
        return 1;
    }
    if (message == VCRDLG_LBUTTON || message == VCRDLG_RBUTTON) {
        square = vcrdlg_square(input, vcrdlg_x(lparam), vcrdlg_y(lparam));
        if (square == 0xffffU) return 0;
        input->focus_square = square;
        if (message == VCRDLG_RBUTTON && input->ops.popup)
            input->focus_token = input->ops.popup(input->context,
                vcrdlg_x(lparam), vcrdlg_y(lparam), square);
        else if (input->ops.focus_square)
            input->ops.focus_square(input->context, square);
        if (input->ops.draw) input->ops.draw(input->context, -2, -1);
        return message == VCRDLG_RBUTTON ? 0 : 1;
    }
    return 0;
}
