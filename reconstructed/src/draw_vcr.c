#include "core.h"

#define DRAW_VCR_LABEL_BYTES 80
#define DRAW_VCR_GRID_ORIGIN 11
#define DRAW_VCR_CENTER_DIVISOR 2
#define DRAW_VCR_CELL_GAP 3

static void draw_vcr_center(const StarsDrawVcrInput *input, StarsU16 square,
    StarsI16 *x, StarsI16 *y)
{
    *x = (StarsI16)((square & 0x0fU) *
        (input->board_square_size + DRAW_VCR_CELL_GAP) +
        input->board_square_size / DRAW_VCR_CENTER_DIVISOR +
        DRAW_VCR_GRID_ORIGIN);
    *y = (StarsI16)((square >> 4) *
        (input->board_square_size + DRAW_VCR_CELL_GAP) +
        input->board_square_size / DRAW_VCR_CENTER_DIVISOR +
        DRAW_VCR_GRID_ORIGIN);
}

/* Genuine semantic reconstruction of DrawVCR, NE 30:1C62-3A3E.
 * The renderer walks the requested token interval, obtains the current armor,
 * displacement and surviving-ship values from GetVCRStats, formats the
 * player/design label, places the fleet bitmap at the encoded board square,
 * draws the associated statistics, and invokes the attack overlay renderer.
 * The callbacks isolate GDI/font/string resources while leaving the ordering
 * and empty-token filtering visible as ordinary C. */
void stars_draw_vcr(StarsU16 hdc, StarsI16 start, StarsI16 end,
    StarsDrawVcrInput *input)
{
    StarsI16 i;
    StarsI16 stop;
    StarsI16 x;
    StarsI16 y;
    StarsI16 ships;
    StarsI32 armor;
    StarsU16 displacement;
    StarsU8 label[DRAW_VCR_LABEL_BYTES];
    const StarsDrawVcrToken *token;
    if (!input || !input->tokens || !input->board_square_size) return;
    if (start < 0) start = 0;
    if (end < 0 || end > (StarsI16)input->token_count)
        stop = (StarsI16)input->token_count;
    else stop = end;
    if (start > stop) return;
    for (i = start; i < stop; ++i) {
        token = &input->tokens[i];
        if (token->ship_count <= 0) continue;
        armor = 0; displacement = 0; ships = 0;
        if (input->ops.get_stats)
            input->ops.get_stats(input->context, token, &armor,
                &displacement, &ships);
        if (input->ops.format_label)
            input->ops.format_label(input->context, token, label,
                DRAW_VCR_LABEL_BYTES);
        draw_vcr_center(input, token->battle_square, &x, &y);
        if (input->ops.draw_fleet)
            input->ops.draw_fleet(input->context, hdc, token);
        if (input->ops.draw_text) {
            input->ops.draw_text(input->context, hdc, x, y, label);
            input->ops.draw_text(input->context, hdc, (StarsI16)(x + 1),
                (StarsI16)(y + 1), (const StarsU8 *)&armor);
            input->ops.draw_text(input->context, hdc, (StarsI16)(x + 2),
                (StarsI16)(y + 2), (const StarsU8 *)&displacement);
            input->ops.draw_text(input->context, hdc, (StarsI16)(x + 3),
                (StarsI16)(y + 3), (const StarsU8 *)&ships);
        }
    }
    if (input->ops.draw_attack) input->ops.draw_attack(input->context, hdc);
}
