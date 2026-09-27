#include "core.h"

static StarsI16 tutor_check(const StarsTutorTaskOps *ops, StarsU16 kind,
    StarsI16 a, StarsI16 b, StarsI16 c, StarsI16 d, StarsI16 e, StarsI16 f)
{
    StarsI16 args[6];
    args[0] = a; args[1] = b; args[2] = c;
    args[3] = d; args[4] = e; args[5] = f;
    if (!ops || !ops->check)
        return 0;
    return ops->check(ops->context, kind, args);
}

static StarsI16 tutor_value(const StarsTutorTaskOps *ops, StarsU16 kind,
    StarsI16 a, StarsI16 b)
{
    if (!ops || !ops->value)
        return 0;
    return ops->value(ops->context, kind, a, b);
}

static void tutor_set_advance(StarsTutorTaskState *state)
{ state->flags = (StarsU16)(state->flags | 0x0008U); }

static void tutor_clear_visible(StarsTutorTaskState *state)
{ state->flags = (StarsU16)(state->flags & (StarsU16)~0x0400U); }

static StarsI16 tutor_turn_zero(StarsTutorTaskState *state,
    const StarsTutorTaskOps *ops)
{
    StarsI16 selected;

    if (state->idt == 0) {
        state->idt_bold = 5;
        return tutor_check(ops, STARS_TUTOR_CHECK_MESSAGES, 9999, -1, 0,
            0, 0, 0);
    }
    if (state->idt == 8) {
        state->idt_bold = 0x0b;
        selected = tutor_check(ops, STARS_TUTOR_CHECK_SELECTION, 2, 0, 0,
            0, 0, 0);
        if (!selected) return 0;
        state->idt_bold = 0x0f;
        return tutor_check(ops, STARS_TUTOR_CHECK_FLEET_WP, 0, 1, 1,
            0x0c, 0, -1);
    }
    if (state->idt == 0x10) {
        state->idt_bold = 0x12;
        selected = tutor_check(ops, STARS_TUTOR_CHECK_SELECTION, 2, 1, 0,
            0, 0, 0);
        if (!selected) { state->idh = 0x1771; return 0; }
        state->idt_bold = 0x15;
        return tutor_check(ops, STARS_TUTOR_CHECK_FLEET_WP, 1, 1, 1,
            0x10, 0, -1);
    }
    if (state->idt == 0x18) {
        selected = tutor_check(ops, STARS_TUTOR_CHECK_SELECTION, 2, 4, 0,
            0, 0, 0);
        if (!selected) {
            if (tutor_check(ops, STARS_TUTOR_CHECK_SELECTION, 2, 3, 0,
                    0, 0, 0)) state->idt_bold = 0x1d;
            else if (tutor_check(ops, STARS_TUTOR_CHECK_SELECTION, 2, 2, 0,
                    0, 0, 0)) state->idt_bold = 0x1b;
            else state->idt_bold = 0x19;
            return 0;
        }
        state->idt_bold = 0x1f;
        return tutor_check(ops, STARS_TUTOR_CHECK_FLEET_WP, 4, 1, 1,
            0x0f, 0, -1);
    }
    if (state->idt == 0x20) {
        selected = tutor_check(ops, STARS_TUTOR_CHECK_SELECTION, 2, 4, 0,
            0, 0, 0);
        if (selected && (state->flags & 0x0600U) == 0U) {
            state->idt_bold = 0x20;
            return 0;
        }
        selected = tutor_check(ops, STARS_TUTOR_CHECK_SELECTION, 2, 5, 0,
            0, 0, 0);
        if (selected && (state->flags & 0x0600U) == 0U) {
            state->idt_bold = 0x22;
            return 0;
        }
        if (tutor_check(ops, STARS_TUTOR_CHECK_RESEARCH, 1, 6, 0x0f,
                0, 0, 0)) {
            tutor_set_advance(state);
            state->idt_bold = 0x27;
            return 1;
        }
        state->idt_bold = state->pct_res == -1 ? 0x25 : 0x26;
        return 0;
    }
    return 0;
}

static StarsI16 tutor_turn_one(StarsTutorTaskState *state,
    const StarsTutorTaskOps *ops)
{
    if (state->idt != 0x28)
        return 1;
    if (!tutor_check(ops, STARS_TUTOR_CHECK_QUEUE, 0x0d, 0, 1, 7,
            0x14, 0)) {
        state->idt_bold = tutor_value(ops, STARS_TUTOR_VALUE_PRODUCT_COUNT,
            0, 0) == 0 ? 0x2a : 0x2b;
        return 0;
    }
    state->idt_bold = 0x2f;
    tutor_set_advance(state);
    return 1;
}

static StarsI16 tutor_turn_two(StarsTutorTaskState *state,
    const StarsTutorTaskOps *ops)
{
    StarsI16 selected;

    if (state->idt == 0x30) {
        if (tutor_check(ops, STARS_TUTOR_CHECK_FLEET_WP, 1, 1, 1,
                0x15, 0, -1)) return 1;
        state->idt_bold = tutor_check(ops, STARS_TUTOR_CHECK_SELECTION,
            2, 0, 0, 0, 0, 0) ? 0x32 : 0x30;
        if (!tutor_check(ops, STARS_TUTOR_CHECK_FLEET_WP, 0, 1, 1, 9, 0, -1)) return 0;
        state->idt_bold = 0x33;
        if (!tutor_check(ops, STARS_TUTOR_CHECK_FLEET_WP, 0, 2, 1, 3, 0, -1)) return 0;
        state->idt_bold = 0x34;
        if (!tutor_check(ops, STARS_TUTOR_CHECK_FLEET_WP, 0, 3, 1, 8, 0, -1)) return 0;
        state->idt_bold = 0x35;
        if (!tutor_check(ops, STARS_TUTOR_CHECK_FLEET_WP, 0, 4, 1, 5, 0, -1)) return 0;
        state->idt_bold = 0x36;
        if (!tutor_check(ops, STARS_TUTOR_CHECK_FLEET_WP, 0, 5, 1, 2, 0, -1)) return 0;
        state->idt_bold = 0x37;
        return tutor_check(ops, STARS_TUTOR_CHECK_SELECTION, 2, 1, 0, 0, 0, 0);
    }
    if (state->idt == 0x38) {
        if (tutor_check(ops, STARS_TUTOR_CHECK_FLEET_WP, 4, 1, 1,
                0x0e, 0, -1)) return 1;
        state->idt_bold = 0x39;
        if (!tutor_check(ops, STARS_TUTOR_CHECK_FLEET_WP, 1, 1, 1, 0x15, 0, -1)) return 0;
        state->idt_bold = 0x3a;
        if (!tutor_check(ops, STARS_TUTOR_CHECK_FLEET_WP, 1, 2, 1, 0x13, 0, -1)) return 0;
        state->idt_bold = 0x3b;
        if (!tutor_check(ops, STARS_TUTOR_CHECK_FLEET_WP, 1, 3, 1, 0x14, 0, -1)) return 0;
        state->idt_bold = 0x3c;
        if (!tutor_check(ops, STARS_TUTOR_CHECK_FLEET_WP, 1, 4, 1, 7, 0, -1)) return 0;
        state->idt_bold = tutor_check(ops, STARS_TUTOR_CHECK_MESSAGES, 2, -1, 0,
            0, 0, 0) ? 0x3f : 0x3d;
        return tutor_check(ops, STARS_TUTOR_CHECK_SELECTION, 2, 4, 0, 0, 0, 0);
    }
    if (state->idt == 0x40) {
        if (tutor_check(ops, STARS_TUTOR_CHECK_FLEET_WP, 5, 1, 1, 0x0c, 3, -1)) return 1;
        state->idt_bold = 0x40;
        if (!tutor_check(ops, STARS_TUTOR_CHECK_FLEET_WP, 4, 1, 1, 0x0e, 0, -1)) return 0;
        state->idt_bold = 0x41;
        if (!tutor_check(ops, STARS_TUTOR_CHECK_FLEET_WP, 4, 2, 1, 0x11, 0, -1)) return 0;
        state->idt_bold = 0x42;
        if (!tutor_check(ops, STARS_TUTOR_CHECK_FLEET_WP, 4, 3, 1, 0x12, 0, -1)) return 0;
        state->idt_bold = 0x43;
        if (!tutor_check(ops, STARS_TUTOR_CHECK_FLEET_WP, 4, 4, 1, 0x17, 0, -1)) return 0;
        state->idt_bold = 0x44;
        if (!tutor_check(ops, STARS_TUTOR_CHECK_FLEET_WP, 4, 5, 1, 0x16, 0, -1)) return 0;
        state->idt_bold = tutor_check(ops, STARS_TUTOR_CHECK_MESSAGES, 4, -1, 0,
            0, 0, 0) ? 0x47 : 0x45;
        return tutor_check(ops, STARS_TUTOR_CHECK_SUMMARY, 1, 0x0c, 0, 0, 0, 0);
    }
    if (state->idt == 0x48) {
        if (tutor_check(ops, STARS_TUTOR_CHECK_COLONIZE_WP, 2, 0x10, -1, 0, 0, 0)) return 1;
        if (tutor_check(ops, STARS_TUTOR_CHECK_FLEET_WP, 5, 1, 1, 0x0c, 3, -1)) {
            tutor_clear_visible(state);
            return 1;
        }
        if (!tutor_check(ops, STARS_TUTOR_CHECK_FLEET_WP, 5, 1, 1, 0x0c, 0, -1)) {
            state->idt_bold = tutor_check(ops, STARS_TUTOR_CHECK_SELECTION, 2, 5, 0, 0, 0, 0) ? 0x4d : 0x4c;
        }
        else state->idt_bold = 0x4f;
        return 0;
    }
    if (state->idt == 0x50) {
        if (tutor_check(ops, STARS_TUTOR_CHECK_COLONIZE_WP, 2, 0x10, -1, 0, 0, 0)) return 1;
        if (tutor_check(ops, STARS_TUTOR_CHECK_SUMMARY, 1, 0x10, 0, 0, 0, 0)) return 1;
        if (!tutor_check(ops, STARS_TUTOR_CHECK_SUMMARY, 1, 0x0f, 0, 0, 0, 0)) return 0;
        if (!tutor_check(ops, STARS_TUTOR_CHECK_MESSAGES, 9999, -1, 0, 0, 0, 0))
            state->idt_bold = (state->flags & 0x0400U) == 0U ? 0x53 : 0x54;
        else state->idt_bold = 0x56;
        return 0;
    }
    if (state->idt == 0x58) {
        if (!tutor_check(ops, STARS_TUTOR_CHECK_CARGO, 2, 0, 0, 0, 0x19, 0)) {
            selected = tutor_check(ops, STARS_TUTOR_CHECK_SELECTION, 2, 2, 0, 0, 0, 0);
            if (!selected) state->idt_bold = 0x59;
            else state->idt_bold = state->xfer_dialog == -1 ? 0x5a : 0x5b;
            if (state->xfer_dialog == -1) state->idh = 0x5ea;
            return 0;
        }
        if (tutor_check(ops, STARS_TUTOR_CHECK_COLONIZE_WP, 2, 0x10, -1, 0, 0, 0)) {
            tutor_set_advance(state);
            tutor_clear_visible(state);
            return 1;
        }
        if (!tutor_check(ops, STARS_TUTOR_CHECK_FLEET_WP, 2, 1, 1, 0x10, 0, -1))
            state->idt_bold = tutor_check(ops, STARS_TUTOR_CHECK_SELECTION, 2, 2, 0, 0, 0, 0) ? 0x5c : 0x59;
        else state->idt_bold = 0x5d;
        return 0;
    }
    return 1;
}

static StarsI16 tutor_turn_three(StarsTutorTaskState *state,
    const StarsTutorTaskOps *ops)
{
    if (state->idt == 0x60) {
        if (!tutor_check(ops, STARS_TUTOR_CHECK_MESSAGES, -1, 0x36, 1,
                0, 0, 0)) { state->idt_bold = 0x61; return 0; }
        if (tutor_check(ops, STARS_TUTOR_CHECK_QUEUE, 0x0d, 0, 1, 1,
                0x1e, 0)) return 1;
        if (!tutor_check(ops, STARS_TUTOR_CHECK_MESSAGES, 1, -1, 0, 0, 0, 0) ||
            !tutor_check(ops, STARS_TUTOR_CHECK_SELECTION, 1, 0x0d, 0, 0, 0, 0))
            state->idt_bold = 0x62;
        else state->idt_bold = tutor_value(ops,
            STARS_TUTOR_VALUE_PRODUCT_COUNT, 0, 0) == 0 ? 0x65 : 0x66;
        return 0;
    }
    if (state->idt == 0x68) {
        if (!tutor_check(ops, STARS_TUTOR_CHECK_QUEUE, 0x10, 0, 1, 7, 3, 1) ||
            !tutor_check(ops, STARS_TUTOR_CHECK_QUEUE, 0x10, 1, 1, 8, 3, 1)) {
            if (!tutor_check(ops, STARS_TUTOR_CHECK_MESSAGES, 3, -1, 0, 0, 0, 0) ||
                !tutor_check(ops, STARS_TUTOR_CHECK_SELECTION, 1, 0x10, 0, 0, 0, 0))
                state->idt_bold = 0x68;
            else state->idt_bold = tutor_value(ops,
                STARS_TUTOR_VALUE_PRODUCT_COUNT, 0, 0) == 0 ? 0x6a : 0x6b;
            return 0;
        }
        if (!tutor_check(ops, STARS_TUTOR_CHECK_CARGO, 3, 0, 0, 0,
                0xd2, 0)) {
            state->idt_bold = tutor_check(ops, STARS_TUTOR_CHECK_SELECTION,
                2, 3, 0, 0, 0, 0) ? 0x6f : 0x6d;
            return 0;
        }
        return 1;
    }
    if (state->idt == 0x70) {
        if (!tutor_check(ops, STARS_TUTOR_CHECK_XFER_WP, 3, 1, 0x10,
                -1, 0, 0)) {
            if (!tutor_check(ops, STARS_TUTOR_CHECK_FLEET_WP, 3, 1, 1,
                    0x10, 1, -1))
                state->idt_bold = tutor_check(ops, STARS_TUTOR_CHECK_FLEET_WP,
                    3, 1, 1, 0x10, -1, -1) ? 0x71 : 0x70;
            else state->idt_bold = 0x72;
            return 0;
        }
        if (tutor_value(ops, STARS_TUTOR_VALUE_FLEET_CORD, 0, 0) < 6)
            return 1;
        if (!tutor_check(ops, STARS_TUTOR_CHECK_SELECTION, 2, 0, 0, 0, 0, 0))
            state->idt_bold = tutor_check(ops, STARS_TUTOR_CHECK_SUMMARY,
                1, 9, 0, 0, 0, 0) ? 0x76 : 0x73;
        else return 1;
        return 0;
    }
    if (state->idt == 0x78) {
        if (tutor_value(ops, STARS_TUTOR_VALUE_FLEET_CORD, 0, 0) == 6) {
            state->idt_bold = tutor_check(ops, STARS_TUTOR_CHECK_SUMMARY,
                1, 9, 0, 0, 0, 0) ? 0x7b : 0x79;
            return 0;
        }
        state->idt_bold = 0x7f;
        state->flags = (StarsU16)((state->flags & (StarsU16)~0x0408U) | 8U);
        return 1;
    }
    return 1;
}

static StarsI16 tutor_turn_four(StarsTutorTaskState *state,
    const StarsTutorTaskOps *ops)
{
    StarsI16 count;

    if (state->idt == 0x80) {
        if (tutor_value(ops, STARS_TUTOR_VALUE_FLEET_CORD, 4, 0) == 6) {
            if (!tutor_check(ops, STARS_TUTOR_CHECK_SELECTION, 2, 4, 0, 0, 0, 0)) {
                if (!tutor_check(ops, STARS_TUTOR_CHECK_SUMMARY, 1, 0x0e, 0, 0, 0, 0) &&
                    !tutor_check(ops, STARS_TUTOR_CHECK_SUMMARY, 2, 4, 0, 0, 0, 0))
                    state->idt_bold = 0x80;
                else state->idt_bold = 0x82;
            }
            else state->idt_bold = tutor_check(ops, STARS_TUTOR_CHECK_SUMMARY,
                1, 0x0e, 0, 0, 0, 0) ? 0x84 : 0x83;
            return 0;
        }
        count = tutor_value(ops, STARS_TUTOR_VALUE_PRODUCT_COUNT, 0x0d, 0);
        if (count == 3) return 1;
        if (!tutor_check(ops, STARS_TUTOR_CHECK_SELECTION, 1, 0x0d, 0, 0, 0, 0))
            state->idt_bold = tutor_check(ops, STARS_TUTOR_CHECK_SUMMARY,
                1, 0x15, 0, 0, 0, 0) ? 0x87 : 0x85;
        return 0;
    }
    if (state->idt == 0x88) {
        count = tutor_value(ops, STARS_TUTOR_VALUE_PRODUCT_COUNT, 0x0d, 0);
        if (count == 3 && tutor_check(ops, STARS_TUTOR_CHECK_QUEUE,
                0x0d, 1, 2, 2, 1, 0)) {
            state->idt_bold = 0x8f;
            tutor_set_advance(state);
            return 1;
        }
        state->idt_bold = tutor_value(ops, STARS_TUTOR_VALUE_PRODUCT_COUNT,
            -1, 0) == 0 ? 0x88 : 0x89;
        return 0;
    }
    return 1;
}

static StarsI16 tutor_turn_five(StarsTutorTaskState *state,
    const StarsTutorTaskOps *ops)
{
    if (state->idt == 0x90) {
        if (!tutor_check(ops, STARS_TUTOR_CHECK_CARGO, 2, 0, 0, 0,
                0x19, 0)) {
            state->idt_bold = tutor_check(ops, STARS_TUTOR_CHECK_SELECTION,
                2, 2, 0, 0, 0, 0) ?
                (state->xfer_dialog == -1 ? 0x92 : 0x94) : 0x90;
            return 0;
        }
        if (!tutor_check(ops, STARS_TUTOR_CHECK_FLEET_WP, 2, 1, 1,
                0x0e, -1, -1)) {
            state->idt_bold = tutor_check(ops, STARS_TUTOR_CHECK_SCANNER,
                3, -1, 0, 0, 0, 0) ? 0x97 : 0x96;
            return 0;
        }
        return 1;
    }
    if (state->idt == 0x98) {
        if (!tutor_check(ops, STARS_TUTOR_CHECK_COLONIZE_WP, 2, 0x0e,
                -1, 0, 0, 0)) { state->idt_bold = 0x98; return 0; }
        if (!tutor_check(ops, STARS_TUTOR_CHECK_FLEET_WP, 3, 1, 1,
                0x0d, -1, -1)) {
            if (!tutor_check(ops, STARS_TUTOR_CHECK_SCANNER, 0, -1,
                    0, 0, 0, 0)) state->idt_bold = 0x99;
            else state->idt_bold = tutor_check(ops, STARS_TUTOR_CHECK_SELECTION,
                2, 3, 0, 0, 0, 0) ? 0x9b : 0x9a;
            return 0;
        }
        if (tutor_value(ops, STARS_TUTOR_VALUE_FLEET_CORD, 0, 0) == 5) {
            if (!tutor_check(ops, STARS_TUTOR_CHECK_MESSAGES, 9999, -1,
                    0, 0, 0, 0) || !tutor_check(ops,
                    STARS_TUTOR_CHECK_SELECTION, 1, 0x10, 0, 0, 0, 0))
                state->idt_bold = 0x9c;
            else state->idt_bold = 0x9e;
            return 0;
        }
        state->idt_bold = 0x9f;
        state->flags = (StarsU16)(state->flags & (StarsU16)~0x0400U);
        tutor_set_advance(state);
        return 1;
    }
    return 1;
}

static StarsI16 tutor_turn_six(StarsTutorTaskState *state,
    const StarsTutorTaskOps *ops)
{
    if (state->idt == 0xa0) {
        if (!tutor_check(ops, STARS_TUTOR_CHECK_XFER_WP, 3, 1, 0x0c,
                -1, 0, 0)) {
            if (!tutor_check(ops, STARS_TUTOR_CHECK_FLEET_WP, 3, 1, 1,
                    0x0c, 1, -1))
                state->idt_bold = tutor_check(ops, STARS_TUTOR_CHECK_FLEET_WP,
                    3, 1, 1, 0x0c, -1, -1) ? 0xa3 : 0xa0;
            else state->idt_bold = 0xa4;
            return 0;
        }
        state->idt_bold = 0xa6;
        return tutor_check(ops, STARS_TUTOR_CHECK_FLEET_WP, 3, 2, 1,
            0x0d, -1, -1);
    }
    if (state->idt == 0xa8) {
        state->flags = (StarsU16)(state->flags | 0x0020U);
        if (!tutor_check(ops, STARS_TUTOR_CHECK_XFER_WP, 3, 2, 0x0d,
                -1, 0, 0)) {
            state->flags = (StarsU16)(state->flags & (StarsU16)~0x0020U);
            state->idt_bold = 0xaa;
            return 0;
        }
        state->flags = (StarsU16)(state->flags & (StarsU16)~0x0020U);
        state->idh = 0x5ee;
        state->idt_bold = 0xab;
        if (!tutor_value(ops, STARS_TUTOR_VALUE_FLEET_PRESENT, 3, 0))
            return 0;
        state->idh = 0x423;
        if (tutor_value(ops, STARS_TUTOR_VALUE_PRODUCT_COUNT, 0x0d, 0) == 3 &&
            tutor_check(ops, STARS_TUTOR_CHECK_QUEUE, 0x0d, 1, 2, 2, 1, 0)) {
            state->idt_bold = 0xaf;
            tutor_set_advance(state);
            return 1;
        }
        state->idt_bold = tutor_check(ops, STARS_TUTOR_CHECK_SELECTION,
            1, 0x0d, 0, 0, 0, 0) ? 0xae : 0xad;
        return 0;
    }
    return 1;
}

static StarsI16 tutor_turn_seven(StarsTutorTaskState *state,
    const StarsTutorTaskOps *ops)
{
    if (state->idt == 0xb0) {
        if (!tutor_check(ops, STARS_TUTOR_CHECK_CARGO, 6, 0, 0, 0,
                0x19, 0)) return 0;
        if (!tutor_check(ops, STARS_TUTOR_CHECK_COLONIZE_WP, 6, 0x12,
                -1, 0, 0, 0)) {
            state->idt_bold = tutor_check(ops, STARS_TUTOR_CHECK_SCANNER,
                3, -1, 0, 0, 0, 0) ? 0xb3 : 0xb1;
            return 0;
        }
        if (tutor_value(ops, STARS_TUTOR_VALUE_PRODUCT_COUNT, 0x0d, 0) < 3 ||
            !tutor_check(ops, STARS_TUTOR_CHECK_QUEUE, 0x0d, 1, 2, 6, 3, 0)) {
            state->idt_bold = 0xb5;
            return 0;
        }
        if (!tutor_check(ops, STARS_TUTOR_CHECK_SCANNER, 0, -1, 0, 0, 0, 0)) {
            state->idt_bold = 0xb6;
            return 0;
        }
        return 1;
    }
    if (state->idt == 0xb8) {
        if (!tutor_check(ops, STARS_TUTOR_CHECK_MESSAGES, -1, 0x38, 1, 0, 0, 0)) {
            state->idt_bold = tutor_check(ops, STARS_TUTOR_CHECK_MESSAGES, 3,
                -1, 0, 0, 0, 0) ? 0xba : 0xb8;
            return 0;
        }
        if (!tutor_check(ops, STARS_TUTOR_CHECK_QUEUE, 0x10, 0, 1, 1, 10, 1) ||
            !tutor_check(ops, STARS_TUTOR_CHECK_QUEUE, 0x10, 1, 1, 0, 10, 1)) {
            if (!tutor_check(ops, STARS_TUTOR_CHECK_SELECTION, 1, 0x10, 0, 0, 0, 0))
                state->idt_bold = 0xbb;
            else state->idt_bold = tutor_check(ops, STARS_TUTOR_CHECK_MESSAGES,
                4, -1, 0, 0, 0, 0) ? 0xbc : 0xbb;
            return 0;
        }
        state->idt_bold = 0xbe;
        tutor_clear_visible(state);
        return tutor_check(ops, STARS_TUTOR_CHECK_MESSAGES, 9999, -1, 0, 0, 0, 0);
    }
    if (state->idt == 0xc0) {
        if ((state->flags & 0x0600U) == 0U) {
            if (!tutor_check(ops, STARS_TUTOR_CHECK_SUMMARY, 2, 0x200,
                    0, 0, 0, 0)) { state->idt_bold = 0xc0; return 0; }
            state->flags = (StarsU16)(state->flags | 0x0400U);
        }
        state->idh = 0x5e3;
        if (tutor_value(ops, STARS_TUTOR_VALUE_PRODUCT_COUNT, 0x0d, 0) < 4 ||
            !tutor_check(ops, STARS_TUTOR_CHECK_QUEUE, 0x0d, 2, 2, 0, 2, 0)) {
            state->idt_bold = 0xc4;
            return 0;
        }
        state->idt_bold = 0xc6;
        tutor_set_advance(state);
        return 1;
    }
    return 1;
}

static StarsI16 tutor_turn_eight(StarsTutorTaskState *state,
    const StarsTutorTaskOps *ops)
{
    if (state->idt == 0xd0) {
        state->flags = (StarsU16)(state->flags | 0x0020U);
        if (!tutor_check(ops, STARS_TUTOR_CHECK_COLONIZE_WP, 7, 0x11,
                -1, 0, 0, 0)) {
            state->flags = (StarsU16)(state->flags & (StarsU16)~0x0020U);
            tutor_check(ops, STARS_TUTOR_CHECK_COLONIZE_WP, 7, 8, -1, 0, 0, 0);
            state->idt_bold = 0xd1;
            return 0;
        }
        state->flags = (StarsU16)(state->flags & (StarsU16)~0x0020U);
        if (!tutor_check(ops, STARS_TUTOR_CHECK_FLEET_WP, 8, 1, 1, 9, -1, -1)) {
            state->idt_bold = tutor_check(ops, STARS_TUTOR_CHECK_SELECTION,
                2, 8, 0, 0, 0, 0) ? 0xd6 : 0xd4;
            return 0;
        }
        tutor_clear_visible(state);
        return 1;
    }
    if (state->idt == 0xd8) {
        state->flags = (StarsU16)(state->flags & (StarsU16)~0x0020U);
        if (!tutor_check(ops, STARS_TUTOR_CHECK_MESSAGES, -1, 0x2d, 1, 0, 0, 0)) {
            tutor_clear_visible(state); state->idt_bold = 0xd8; return 0;
        }
        state->flags = (StarsU16)(state->flags | 0x0020U);
        if (!tutor_check(ops, STARS_TUTOR_CHECK_RESEARCH, 1, 6, 0x1e, 0, 0, 0)) {
            if (!tutor_check(ops, STARS_TUTOR_CHECK_MESSAGES, 9999, -1, 0, 0, 0, 0)) {
                state->idt_bold = 0xd9; tutor_clear_visible(state); return 0;
            }
            if ((state->flags & 0x0600U) == 0U)
                state->idt_bold = tutor_check(ops, STARS_TUTOR_CHECK_SUMMARY,
                    1, 5, 0, 0, 0, 0) ? 0xdb : 0xd9;
            else state->idt_bold = 0xdd;
            if (state->pct_res == -1) return 0;
            tutor_clear_visible(state); return 1;
        }
        state->flags = (StarsU16)(state->flags & (StarsU16)~0x0020U);
        return 1;
    }
    if (state->idt == 0xe0) {
        if (state->pct_res == -1) {
            if (!tutor_check(ops, STARS_TUTOR_CHECK_RESEARCH, 1, 6, 0x1e, 0, 0, 0)) {
                state->idt_bold = 0xe4; return 0;
            }
            state->idt_bold = 0xe6; tutor_set_advance(state); return 1;
        }
        state->idt_bold = (state->flags & 0x0400U) == 0U ? 0xe2 : 0xe4;
        return 0;
    }
    return 1;
}

static StarsI16 tutor_turn_nine(StarsTutorTaskState *state,
    const StarsTutorTaskOps *ops)
{
    if (state->idt == 0xe8) {
        state->flags = (StarsU16)(state->flags | 0x0020U);
        if (!tutor_check(ops, STARS_TUTOR_CHECK_RESEARCH, 1, 3, 0x1e, 0, 0, 0)) {
            state->idt_bold = state->pct_res == -1 ? 0xe8 : 0xec;
            state->flags = (StarsU16)(state->flags & (StarsU16)~0x0020U);
            return 0;
        }
        state->flags = (StarsU16)(state->flags & (StarsU16)~0x0020U);
        state->idt_bold = 0xee;
        return tutor_check(ops, STARS_TUTOR_CHECK_MESSAGES, -1, 0x7d, 1, 0, 0, 0);
    }
    if (state->idt == 0xf0) {
        if (!tutor_check(ops, STARS_TUTOR_CHECK_COLONIZE_WP, 9, 2, -1, 0, 0, 0)) {
            if (!tutor_check(ops, STARS_TUTOR_CHECK_SELECTION, 2, 9, 0, 0, 0, 0))
                state->idt_bold = tutor_check(ops, STARS_TUTOR_CHECK_SUMMARY,
                    1, 2, 0, 0, 0, 0) ? 0xf1 : 0xf0;
            else state->idt_bold = tutor_check(ops, STARS_TUTOR_CHECK_CARGO,
                9, 0, 0, 0, 0x19, 0) ? 0xf3 : 0xf2;
            return 0;
        }
        state->flags = (StarsU16)(state->flags | 0x0020U);
        if (!tutor_check(ops, STARS_TUTOR_CHECK_FLEET_WP, 0, 1, 1, 4, -1, -1)) {
            tutor_check(ops, STARS_TUTOR_CHECK_FLEET_WP, 0, 1, 1, 2, -1, -1);
            state->idt_bold = 0xf5;
            state->flags = (StarsU16)(state->flags & (StarsU16)~0x0020U);
            return 0;
        }
        state->idt_bold = 0xf7;
        state->flags = (StarsU16)((state->flags & (StarsU16)~0x0028U) | 8U);
        return 1;
    }
    return 1;
}

static StarsI16 tutor_turn_ten(StarsTutorTaskState *state,
    const StarsTutorTaskOps *ops)
{
    if (state->idt == 0xf8) {
        if (!tutor_check(ops, STARS_TUTOR_CHECK_MESSAGES, -1, 0x59, 1, 0, 0, 0)) {
            state->idt_bold = 0xf8; tutor_clear_visible(state); return 0;
        }
        if (!tutor_check(ops, STARS_TUTOR_CHECK_QUEUE, 0x0e, 0, 1, 1, 3, 1) ||
            !tutor_check(ops, STARS_TUTOR_CHECK_QUEUE, 0x0e, 1, 1, 0, 3, 1)) {
            if (!tutor_check(ops, STARS_TUTOR_CHECK_SELECTION, 1, 0x0e, 0, 0, 0, 0))
                state->idt_bold = 0xf9;
            else state->idt_bold = tutor_value(ops, STARS_TUTOR_VALUE_PRODUCT_COUNT,
                -1, 0) == 0 ? 0xfa : 0xfb;
            return 0;
        }
        if (!tutor_check(ops, STARS_TUTOR_CHECK_TEMPLATE, 0, 0, 0, 0, 0, 0)) {
            if (tutor_value(ops, STARS_TUTOR_VALUE_PRODUCT_COUNT, -1, 0) == 0) {
                state->idt_bold = 0xfe; return 0;
            }
            if (state->pct_res == -1) { state->idt_bold = 0xff; return 0; }
        }
        return 1;
    }
    if (state->idt == 0x100) {
        if (!tutor_check(ops, STARS_TUTOR_CHECK_TEMPLATE, 0, 0, 0, 0, 0, 0)) {
            state->idt_bold = 0x100; return 0;
        }
        if ((state->flags & 0x0400U) == 0U &&
            tutor_value(ops, STARS_TUTOR_VALUE_PRODUCT_COUNT, -1, 0) != 0) {
            state->idt_bold = 0x101; return 0;
        }
        state->idh = 0x423;
        if (tutor_value(ops, STARS_TUTOR_VALUE_PRODUCT_COUNT, 0x0d, 0) < 3 ||
            !tutor_check(ops, STARS_TUTOR_CHECK_QUEUE, 0x0d, 1, 2, 2, 1, 0)) {
            state->idt_bold = 0x104; return 0;
        }
        if (tutor_value(ops, STARS_TUTOR_VALUE_PRODUCT_COUNT, 0x0d, 0) < 4 ||
            !tutor_check(ops, STARS_TUTOR_CHECK_QUEUE, 0x0d, 2, 2, 3, 1, 0)) {
            state->idt_bold = 0x106; return 0;
        }
        state->idt_bold = 0x107; tutor_set_advance(state); return 1;
    }
    return 1;
}

static StarsI16 tutor_product_count(const StarsTutorTaskOps *ops,
    StarsI16 planet)
{ return tutor_value(ops, STARS_TUTOR_VALUE_PRODUCT_COUNT, planet, 0); }

static StarsI16 tutor_fleet_cord(const StarsTutorTaskOps *ops)
{ return tutor_value(ops, STARS_TUTOR_VALUE_FLEET_CORD, 0, 0); }

static StarsI16 tutor_fleet_present(const StarsTutorTaskOps *ops,
    StarsI16 fleet)
{ return tutor_value(ops, STARS_TUTOR_VALUE_FLEET_PRESENT, fleet, 0); }

static void tutor_set_hidden(StarsTutorTaskState *state)
{ state->flags = (StarsU16)(state->flags | 0x0020U); }

static void tutor_clear_hidden(StarsTutorTaskState *state)
{ state->flags = (StarsU16)(state->flags & (StarsU16)~0x0020U); }

static StarsI16 tutor_turn_late(StarsTutorTaskState *state,
    const StarsTutorTaskOps *ops)
{
    StarsI16 selected;
    StarsI16 dispatch_turn;

    /* The decompiler's shared labels combine the final two tutorial-turn
     * blocks: turn 18 shares the turn-17 label and turns 20/21 share the
     * following label.  Keep that control-flow fact local to the dispatcher;
     * the public state still carries the original tutorial turn. */
    dispatch_turn = state->turn;
    if (state->turn == 18) dispatch_turn = 17;
    else if (state->turn == 19 || state->turn == 20)
        dispatch_turn = (StarsI16)(state->turn - 1);
    else if (state->turn >= 21)
        dispatch_turn = (StarsI16)(state->turn - 2);

    switch (dispatch_turn) {
    case 11:
        if (state->idt == 0x108) {
            if (!tutor_check(ops, STARS_TUTOR_CHECK_FLEET_WP, 0, 1, 1,
                    10, -1, -1)) {
                state->idt_bold = tutor_check(ops,
                    STARS_TUTOR_CHECK_SELECTION, 2, 0, 0, 0, 0, 0) ?
                    0x109 : 0x108;
                return 0;
            }
            if (!tutor_check(ops, STARS_TUTOR_CHECK_FLEET_WP, 1, 1, 1,
                    0x0d, 5, -1)) {
                state->idt_bold = tutor_check(ops,
                    STARS_TUTOR_CHECK_SELECTION, 2, 1, 0, 0, 0, 0) ?
                    0x10c : 0x10a;
                return 0;
            }
            if (!tutor_check(ops, STARS_TUTOR_CHECK_FLEET_WP, 4, 1, 1,
                    0x0d, -1, -1)) {
                state->idt_bold = 0x10d;
                return 0;
            }
            if (!tutor_check(ops, STARS_TUTOR_CHECK_FLEET_WP, 8, 1, 2,
                    0x200, -1, -1)) {
                state->idt_bold = 0x10f;
                return 0;
            }
            return 1;
        }
        if (state->idt == 0x110) {
            if (!tutor_check(ops, STARS_TUTOR_CHECK_COLONIZE_WP, 2, 4,
                    -1, 0, 0, 0)) {
                state->idt_bold = tutor_check(ops,
                    STARS_TUTOR_CHECK_SELECTION, 2, 2, 0, 0, 0, 0) ?
                    0x112 : 0x110;
                return 0;
            }
            if (!tutor_check(ops, STARS_TUTOR_CHECK_CARGO, 0x0b, 0, 0,
                    0, 0xd2, 0)) {
                state->idt_bold = tutor_check(ops,
                    STARS_TUTOR_CHECK_SELECTION, 2, 0x0b, 0, 0, 0, 0) ?
                    0x114 : 0x113;
                return 0;
            }
            if (!tutor_check(ops, STARS_TUTOR_CHECK_FLEET_WP, 0x0b, 1, 1,
                    5, -1, -1)) {
                state->idt_bold = 0x114;
                return 0;
            }
            if (!tutor_check(ops, STARS_TUTOR_CHECK_FLEET_WP, 0x0b, 1, 1,
                    5, 1, -1)) {
                state->idt_bold = 0x115;
                return 0;
            }
            if (tutor_check(ops, STARS_TUTOR_CHECK_XFER_WP, 0x0b, 1, 5,
                    -1, 0, 0)) {
                tutor_clear_visible(state);
                return 1;
            }
            selected = tutor_check(ops, STARS_TUTOR_CHECK_SELECTION,
                2, 0x0b, 0, 0, 0, 0);
            state->idt_bold = selected ? 0x117 : 0x116;
            return 0;
        }
        if (state->idt == 0x118) {
            if (tutor_product_count(ops, 0x0d) < 2 ||
                !tutor_check(ops, STARS_TUTOR_CHECK_QUEUE, 0x0d, 0, 1,
                    8, 0x46, 0)) {
                state->idt_bold = 0x118;
                return 0;
            }
            if (!tutor_check(ops, STARS_TUTOR_CHECK_RESEARCH, 3, 5,
                    0x1e, 0, 0, 0)) {
                state->idt_bold = state->pct_res == -1 ? 0x119 : 0x11a;
                return 0;
            }
            if (tutor_check(ops, STARS_TUTOR_CHECK_SELECTION, 2, 8,
                    0, 0, 0, 0)) return 1;
            if (!state->browser) {
                state->idt_bold = 0x11b;
                return 0;
            }
            if ((state->flags & 0x0400U) == 0U) {
                state->idt_bold = 0x11c;
                state->flags = (StarsU16)(state->flags | 0x0400U);
                return 0;
            }
            state->idt_bold = tutor_check(ops, STARS_TUTOR_CHECK_MESSAGES,
                0x0d, -1, 0, 0, 0, 0) ? 0x11e : 0x11d;
            return 0;
        }
        if (state->idt == 0x120) {
            if ((state->flags & 0x0600U) == 0U) {
                state->idt_bold = tutor_value(ops,
                    STARS_TUTOR_VALUE_FLEET_PRESENT, 0, 0) ? 0x122 : 0x121;
                return 0;
            }
            state->idt_bold = tutor_check(ops, STARS_TUTOR_CHECK_MESSAGES,
                9999, -1, 0, 0, 0, 0) ? 0x127 : 0x123;
            tutor_set_advance(state);
            return 1;
        }
        return 1;

    case 12:
        if (state->idt == 0x128) {
            if (tutor_product_count(ops, 8) < 3) {
                if (!tutor_check(ops, STARS_TUTOR_CHECK_SUMMARY, 2, 3,
                        0, 0, 0, 0)) {
                    if (!tutor_check(ops, STARS_TUTOR_CHECK_SUMMARY, 8, -1,
                            0, 0, 0, 0)) {
                        state->idt_bold = !tutor_check(ops,
                            STARS_TUTOR_CHECK_MESSAGES, 1, -1, 0, 0, 0, 0) &&
                            !tutor_check(ops, STARS_TUTOR_CHECK_SELECTION,
                                2, 8, 0, 0, 0, 0) ? 0x129 : 300;
                    } else state->idt_bold = 0x12f;
                    return 0;
                }
            }
            return 1;
        }
        if (state->idt == 0x130) {
            if (!tutor_check(ops, STARS_TUTOR_CHECK_MESSAGES, 5, -1,
                    0, 0, 0, 0)) { state->idt_bold = 0x132; return 0; }
            if (!tutor_check(ops, STARS_TUTOR_CHECK_MESSAGES, 6, -1,
                    0, 0, 0, 0)) { state->idt_bold = 0x134; return 0; }
            state->idt_bold = 0x136;
            return tutor_check(ops, STARS_TUTOR_CHECK_SELECTION, 1, 0x12,
                0, 0, 0, 0);
        }
        if (state->idt == 0x138) {
            if (tutor_product_count(ops, 0x0d) >= 3 &&
                tutor_check(ops, STARS_TUTOR_CHECK_QUEUE, 0x0d, 1, 2,
                    3, 2, 0)) {
                state->idt_bold = 0x13f;
                tutor_set_advance(state);
                return 1;
            }
            if (tutor_product_count(ops, 8) < 3 ||
                !tutor_check(ops, STARS_TUTOR_CHECK_QUEUE, 8, 0, 1,
                    0x0c, 2, 1)) {
                state->idt_bold = tutor_check(ops, STARS_TUTOR_CHECK_SELECTION,
                    1, 8, 0, 0, 0, 0) ? 0x13c : 0x139;
                return 0;
            }
            state->idt_bold = 0x13e;
            return 0;
        }
        return 1;

    case 13:
        if (state->idt == 0x140) {
            if (tutor_check(ops, STARS_TUTOR_CHECK_XFER_WP, 0, 1, 8,
                    -1, 0, 0)) {
                if (!tutor_check(ops, STARS_TUTOR_CHECK_MESSAGES, 3, -1,
                        0, 0, 0, 0)) { state->idt_bold = 0x142; return 0; }
                if (!tutor_check(ops, STARS_TUTOR_CHECK_RESEARCH, 5, 2,
                        0x1e, 0, 0, 0)) { state->idt_bold = 0x144; return 0; }
                if ((state->flags & 0x0600U) == 0U) {
                    state->idt_bold = state->browser ? 0x147 : 0x145;
                    if (state->browser) state->flags =
                        (StarsU16)(state->flags | 0x0400U);
                    return 0;
                }
                state->idt_bold = 0x147;
                return state->slot_dialog;
            }
            state->idt_bold = tutor_check(ops, STARS_TUTOR_CHECK_CARGO,
                0, 0, 0, 0, 0xd2, 0) ? 0x141 : 0x140;
            tutor_clear_visible(state);
            return 0;
        }
        if (state->idt == 0x148) {
            if (!tutor_check(ops, STARS_TUTOR_CHECK_SHIP_BUILDER,
                    4, -1, 0, 0, 0, 0)) {
                if (!tutor_check(ops, STARS_TUTOR_CHECK_SHIP_BUILDER,
                        1, 7, 0, 0, 0, 0)) {
                    state->idt_bold = tutor_check(ops,
                        STARS_TUTOR_CHECK_SHIP_BUILDER, 1, -1, 0, 0, 0, 0) ?
                        0x149 : 0x148;
                } else state->idt_bold = 0x14a;
                return 0;
            }
            state->idh = 0xbdf;
            if (!tutor_check(ops, STARS_TUTOR_CHECK_BUILDER_PART, 0,
                    0x103, 1, 0, 0, 0)) { state->idt_bold = 0x14c; return 0; }
            if (!tutor_check(ops, STARS_TUTOR_CHECK_BUILDER_PART, 1,
                    0x101, 1, 0, 0, 0)) { state->idt_bold = 0x14d; return 0; }
            state->idt_bold = tutor_check(ops, STARS_TUTOR_CHECK_SHIP_BUILDER,
                4, 8, 0, 0, 0, 0) ? 0x14f : 0x14e;
            return 0;
        }
        if (state->idt == 0x150) {
            if (state->slot_dialog) {
                state->idt_bold = tutor_check(ops,
                    STARS_TUTOR_CHECK_SHIP_BUILDER, 4, -1, 0, 0, 0, 0) ?
                    0x151 : 0x152;
                return 0;
            }
            if (tutor_product_count(ops, 0x0d) < 3 ||
                !tutor_check(ops, STARS_TUTOR_CHECK_QUEUE, 0x0d, 1, 2,
                    6, 1, 0)) { state->idt_bold = 0x153; return 0; }
            if (tutor_product_count(ops, 0x0d) < 4 ||
                !tutor_check(ops, STARS_TUTOR_CHECK_QUEUE, 0x0d, 0, 1,
                    8, 100, 0)) {
                state->idt_bold = tutor_product_count(ops, -1) ? 0x156 : 0x155;
                tutor_clear_visible(state);
                return 0;
            }
            return 1;
        }
        if (state->idt == 0x158) {
            if ((state->flags & 0x0600U) == 0U) { state->idt_bold = 0x158; return 0; }
            if (!tutor_check(ops, STARS_TUTOR_CHECK_MESSAGES, 9999, -1,
                    0, 0, 0, 0)) { state->idt_bold = 0x159; return 0; }
            state->idt_bold = 0x15e;
            if (!tutor_check(ops, STARS_TUTOR_CHECK_FLEET_WP, 8, 1, 1,
                    0x0d, -1, -1)) return 0;
            tutor_set_advance(state);
            return 1;
        }
        return 1;

    case 14:
        if (state->idt == 0x160) {
            if (tutor_product_count(ops, 2) < 3 ||
                !tutor_check(ops, STARS_TUTOR_CHECK_QUEUE, 2, 0, 1, 4, 2, 1) ||
                !tutor_check(ops, STARS_TUTOR_CHECK_TEMPLATE, 1, 0, 0, 0, 0, 0)) {
                if (!tutor_check(ops, STARS_TUTOR_CHECK_SELECTION, 1, 2, 0, 0, 0, 0))
                    state->idt_bold = tutor_check(ops, STARS_TUTOR_CHECK_SELECTION,
                        1, 0x11, 0, 0, 0, 0) ? 0x162 : 0x160;
                else state->idt_bold = 0x164;
                return 0;
            }
            state->idt_bold = 0x166; tutor_set_advance(state); return 1;
        }
        return 1;

    case 15:
        if (state->idt == 0x168) {
            if (!tutor_check(ops, STARS_TUTOR_CHECK_FLEET_WP, 8, 1, 1,
                    0x0b, -1, -1)) { state->idt_bold = 0x168; return 0; }
            if (!tutor_check(ops, STARS_TUTOR_CHECK_CARGO, 6, 0, 0, 0,
                    0xd2, 0)) { state->idt_bold = 0x169; tutor_clear_visible(state); return 0; }
            if ((state->flags & 0x0600U) == 0U) {
                state->idt_bold = 0x16b;
                if (tutor_value(ops, STARS_TUTOR_VALUE_REPORT_SORT, 0, 0) == 4)
                    state->idt_bold = 0x16c;
                return 0;
            }
            state->idt_bold = 0x16f;
            return tutor_check(ops, STARS_TUTOR_CHECK_XFER_WP, 6, 1, 5, -1, 0, 0);
        }
        if (state->idt == 0x170) {
            if ((state->flags & 0x0600U) == 0U) {
                state->idt_bold = state->pct_res == -1 ? 0x170 : 0x173;
                if (state->pct_res != -1) state->flags =
                    (StarsU16)(state->flags | 0x0400U);
                return 0;
            }
            if (!tutor_check(ops, STARS_TUTOR_CHECK_FLEET_WP, 0x0b, 1, 1,
                    0x0d, -1, -1)) { state->idt_bold = 0x174; return 0; }
            state->idt_bold = 0x176; tutor_set_advance(state); return 1;
        }
        return 1;

    case 16:
        if (state->idt == 0x178) {
            if (!tutor_check(ops, STARS_TUTOR_CHECK_FLEET_WP, 4, 1, 1,
                    5, -1, -1)) { state->idt_bold = 0x178; return 0; }
            tutor_set_hidden(state);
            if (tutor_fleet_cord(ops) == 1) {
                state->idt_bold = tutor_check(ops, STARS_TUTOR_CHECK_SELECTION,
                    2, 7, 0, 0, 0, 0) ? 0x17a : 0x179;
                return 0;
            }
            tutor_clear_hidden(state);
            if (!tutor_check(ops, STARS_TUTOR_CHECK_FLEET_WP, 7, 1, 1,
                    0x0c, -1, -1)) { state->idt_bold = 0x17a; return 0; }
            tutor_clear_hidden(state);
            if (!tutor_check(ops, STARS_TUTOR_CHECK_FLEET_WP, 7, 1, 2,
                    5, 4, -1)) { state->idt_bold = 0x17a; return 0; }
            if (tutor_product_count(ops, 0x0d) < 2 ||
                !tutor_check(ops, STARS_TUTOR_CHECK_QUEUE, 0x0d, 0, 1,
                    1, 0x3c, 0) || !tutor_check(ops, STARS_TUTOR_CHECK_QUEUE,
                    0x0d, 1, 1, 0, 0x3c, 0)) {
                state->idt_bold = tutor_product_count(ops, -1) ? 0x17d : 0x17c;
                return 0;
            }
            if (!tutor_check(ops, STARS_TUTOR_CHECK_RESEARCH, 2, 3,
                    0x1e, 0, 0, 0)) { state->idt_bold = 0x17e; return 0; }
            state->idt_bold = 0x17f; tutor_set_advance(state); return 1;
        }
        return 1;

    case 17:
        if (state->idt == 0x180) {
            if (!tutor_check(ops, STARS_TUTOR_CHECK_FLEET_WP, 0, 1, 1,
                    0x0d, -1, -1)) {
                state->idt_bold = tutor_check(ops, STARS_TUTOR_CHECK_MESSAGES,
                    9999, -1, 0, 0, 0, 0) ? 0x181 : 0x180;
                return 0;
            }
            state->idt_bold = 0x183; tutor_set_advance(state); return 1;
        }
        if (state->idt == 0x188) {
            if (tutor_product_count(ops, 0x0d) < 3 ||
                !tutor_check(ops, STARS_TUTOR_CHECK_QUEUE, 0x0d, 0, 2, 3, 1, 0)) {
                state->idt_bold = 0x188; return 0;
            }
            if (!tutor_check(ops, STARS_TUTOR_CHECK_MESSAGES, 5, -1, 0, 0, 0, 0)) {
                tutor_clear_visible(state); state->idt_bold = 0x189; return 0;
            }
            if ((state->flags & 0x0600U) == 0U) {
                if (!tutor_check(ops, STARS_TUTOR_CHECK_SELECTION, 1, 0x10, 0, 0, 0, 0))
                    state->idt_bold = 0x189;
                else state->idt_bold = state->pct_res == -1 ? 0x18b : 0x18c;
                return 0;
            }
            if (state->pct_res == -1) { state->idt_bold = 399; tutor_set_advance(state); return 1; }
            state->idt_bold = 0x18e; return 0;
        }
        return 1;

    case 18:
        if (state->idt == 0x190) {
            if (!tutor_check(ops, STARS_TUTOR_CHECK_CARGO, 0x0b, 0, 0, 0, 0xd2, 0)) {
                state->idt_bold = 0x190; return 0;
            }
            if (!tutor_check(ops, STARS_TUTOR_CHECK_XFER_WP, 0x0b, 1, 5, -1, 0, 0)) {
                state->idt_bold = 0x191; return 0;
            }
            tutor_set_hidden(state);
            if (!tutor_check(ops, STARS_TUTOR_CHECK_XFER_WP, 0x0b, 2, 0x0d, -1, 0, 0)) {
                tutor_clear_hidden(state); state->idt_bold = 0x192; return 0;
            }
            tutor_clear_hidden(state); state->idh = 0x5ee; state->idt_bold = 0x193;
            if (!tutor_fleet_present(ops, 1)) return 0;
            if (!tutor_check(ops, STARS_TUTOR_CHECK_CARGO, 1, 0, 0, 0, 0xd2, 0)) {
                state->idt_bold = tutor_check(ops, STARS_TUTOR_CHECK_SELECTION,
                    2, 1, 0, 0, 0, 0) ? 0x197 : 0x196; return 0;
            }
            return 1;
        }
        if (state->idt == 0x198) {
            if (!tutor_check(ops, STARS_TUTOR_CHECK_XFER_WP, 1, 1, 2, -1, 0, 0)) {
                state->idt_bold = 0x198; return 0;
            }
            tutor_set_hidden(state);
            if (!tutor_check(ops, STARS_TUTOR_CHECK_XFER_WP, 1, 2, 0x0d, -1, 0, 0)) {
                tutor_clear_hidden(state); state->idt_bold = 0x199; return 0;
            }
            tutor_clear_hidden(state); state->idh = 0x5ee; state->idt_bold = 0x19a;
            if (!tutor_fleet_present(ops, 1)) return 0;
            if (tutor_product_count(ops, 0x10) < 3 ||
                !tutor_check(ops, STARS_TUTOR_CHECK_QUEUE, 0x10, 2, 1, 5, 1, 1)) {
                state->idt_bold = 0x19c; return 0;
            }
            if (!tutor_check(ops, STARS_TUTOR_CHECK_FLEET_WP, 6, 1, 1, 0x0d, -1, -1)) {
                state->idt_bold = 0x19d; return 0;
            }
            state->idt_bold = 0x19e; tutor_set_advance(state); return 1;
        }
        return 1;

    case 19:
        if (state->idt == 0x1a0) {
            if (!tutor_check(ops, STARS_TUTOR_CHECK_FLEET_WP, 8, 1, 1, 6, -1, -1)) {
                state->idt_bold = 0x1a0; return 0;
            }
            if (!tutor_check(ops, STARS_TUTOR_CHECK_RESEARCH, 3, 1, 0x1e, 0, 0, 0)) {
                state->idt_bold = 0x1a1; return 0;
            }
            if (state->slot_dialog || state->player_designs == 1) {
                state->idt_bold = state->slot_dialog ? 0x1a5 : 0x1a2;
                return 0;
            }
            if (tutor_product_count(ops, 0x0d) < 4 ||
                !tutor_check(ops, STARS_TUTOR_CHECK_QUEUE, 0x0d, 1, 2, 0x11, 1, 0)) {
                state->idt_bold = 0x1a6; return 0;
            }
            state->idt_bold = 0x1a7; tutor_set_advance(state); return 1;
        }
        if (state->idt == 0x1a8) {
            if (!tutor_check(ops, STARS_TUTOR_CHECK_CARGO, 0, 0, 0, 0, 0xd2, 0)) {
                state->idt_bold = 0x1a8; return 0;
            }
            if (!tutor_check(ops, STARS_TUTOR_CHECK_XFER_WP, 0, 1, 5, -1, 0, 0)) {
                state->idt_bold = 0x1a9; return 0;
            }
            if (!tutor_check(ops, STARS_TUTOR_CHECK_ZIP, 0, 0, 0x209, 0, 0, 0)) {
                state->idt_bold = state->zip_dialog ? 0x1ac : 0x1ab; return 0;
            }
            tutor_set_hidden(state);
            if (!tutor_check(ops, STARS_TUTOR_CHECK_XFER_WP, 0, 2, 0x0d, -1, 0, 0)) {
                tutor_clear_hidden(state); state->idt_bold = 0x1ae; return 0;
            }
            tutor_clear_hidden(state); state->idh = 0x5ee; state->idt_bold = 0x1af;
            return tutor_fleet_present(ops, 0);
        }
        if (state->idt == 0x1b0) {
            if (!tutor_check(ops, STARS_TUTOR_CHECK_MESSAGES, 9999, -1, 0, 0, 0, 0)) {
                state->idt_bold = 0x1b0; return 0;
            }
            if ((state->flags & 0x0600U) == 0U) { state->idt_bold = 0x1b3; return 0; }
            state->idt_bold = 0x1b7; tutor_set_advance(state); return 1;
        }
        return 1;

    case 20:
        if (state->idt == 0x1b8) {
            if (!tutor_check(ops, STARS_TUTOR_CHECK_SELECTION, 2, 3, 0, 0, 0, 0)) {
                state->idt_bold = 0x1b8; return 0;
            }
            state->idh = 0x5ed; state->idt_bold = 0x1bd; return 0;
        }
        if (state->idt == 0x1c0) {
            if (tutor_product_count(ops, 0x0d) < 4 ||
                !tutor_check(ops, STARS_TUTOR_CHECK_QUEUE, 0x0d, 1, 2, 3, 1, 0)) {
                state->idt_bold = 0x1c0; return 0;
            }
            if (!tutor_check(ops, STARS_TUTOR_CHECK_RESEARCH, 1, 2, 0x1e, 0, 0, 0)) {
                state->idt_bold = 0x1c1; return 0;
            }
            if (!tutor_check(ops, STARS_TUTOR_CHECK_MESSAGES, 9999, -1, 0, 0, 0, 0)) {
                state->idt_bold = 0x1c2; return 0;
            }
            if (!state->slot_dialog || state->player_designs < 8) {
                state->idt_bold = 0x1c2; return 0;
            }
            state->idh = 0xbdf;
            if (!tutor_check(ops, STARS_TUTOR_CHECK_BUILDER_PART, 0, 0x104, 1, 0, 0, 0)) {
                state->idt_bold = 0x1c4; return 0;
            }
            state->idt_bold = tutor_check(ops, STARS_TUTOR_CHECK_BUILDER_PART,
                2, 0x301, 1, 0, 0, 0) ? 0x1c6 : 0x1c5;
            return 0;
        }
        if (state->idt == 0x1c8) {
            if (!tutor_check(ops, STARS_TUTOR_CHECK_FLEET_WP, 4, 1, 2, 0x204, -1, -1)) {
                state->idt_bold = !tutor_check(ops, STARS_TUTOR_CHECK_SELECTION,
                    2, 4, 0, 0, 0, 0) ?
                    (!tutor_check(ops, STARS_TUTOR_CHECK_SUMMARY, 2, 0x204, 0, 0, 0, 0) ? 0x1c8 : 0x1ca) : 0x1cb;
                return 0;
            }
            state->idt_bold = 0x1cd; tutor_set_advance(state); return 1;
        }
        return 1;

    case 21:
        if (state->idt == 0x1d0) {
            if (tutor_check(ops, STARS_TUTOR_CHECK_CARGO, 6, 0, 0, 0, 0xd2, 0)) return 1;
            if (!tutor_check(ops, STARS_TUTOR_CHECK_MESSAGES, 1, -1, 0, 0, 0, 0) &&
                !tutor_check(ops, STARS_TUTOR_CHECK_SELECTION, 2, 4, 0, 0, 0, 0)) {
                tutor_clear_visible(state); state->idt_bold = 0x1d0; return 0;
            }
            if (!tutor_check(ops, STARS_TUTOR_CHECK_MESSAGES, 2, -1, 0, 0, 0, 0) &&
                !tutor_check(ops, STARS_TUTOR_CHECK_SELECTION, 2, 6, 0, 0, 0, 0)) {
                state->idt_bold = 0x1d4; return 0;
            }
            state->idt_bold = 0x1d5; return 0;
        }
        if (state->idt == 0x1d8) {
            if (!tutor_check(ops, STARS_TUTOR_CHECK_CARGO, 6, 0, 0, 0, 0xd2, 0) ||
                !tutor_check(ops, STARS_TUTOR_CHECK_FLEET_WP, 6, 1, 1, 0x11, -1, -1) ||
                !tutor_check(ops, STARS_TUTOR_CHECK_FLEET_WP, 6, 1, 1, 0x11, 1, -1)) {
                state->idt_bold = 0x1d8; return 0;
            }
            if (!tutor_check(ops, STARS_TUTOR_CHECK_XFER_WP, 6, 1, 0x11, -1, 0, 0)) {
                state->idh = 0x5f0; state->idt_bold = 0x1da; return 0;
            }
            state->idt_bold = 0x1db;
            if (!tutor_fleet_present(ops, 2)) tutor_clear_visible(state);
            return 0;
        }
        if (state->idt == 0x1e0) {
            if (!tutor_check(ops, STARS_TUTOR_CHECK_FLEET_WP, 7, 0, 1, 0x0d, 6, -1)) {
                state->idt_bold = 0x1e0; return 0;
            }
            if (tutor_product_count(ops, 0x0d) < 3 ||
                !tutor_check(ops, STARS_TUTOR_CHECK_QUEUE, 0x0d, 0, 2, 6, 1, 0)) {
                state->idt_bold = 0x1e1; tutor_clear_visible(state); return 0;
            }
            if (!tutor_check(ops, STARS_TUTOR_CHECK_MESSAGES, 0x0e, -1, 0, 0, 0, 0)) {
                state->idt_bold = 0x1e2; return 0;
            }
            state->idt_bold = 0x1e7; tutor_set_advance(state); return 1;
        }
        return 1;

    case 22:
        if (state->idt == 0x1e8) {
            if (!tutor_check(ops, STARS_TUTOR_CHECK_FLEET_WP, 4, 1, 1, 5, -1, -1)) {
                state->idt_bold = 0x1e8; return 0;
            }
            tutor_set_hidden(state);
            if (!tutor_check(ops, STARS_TUTOR_CHECK_FLEET_WP, 2, 1, 2, 5, 4, -1)) {
                tutor_clear_hidden(state);
                tutor_check(ops, STARS_TUTOR_CHECK_FLEET_WP, 2, 1, 1, 0x0c, -1, -1);
                state->idt_bold = 0x1e9; return 0;
            }
            tutor_clear_hidden(state);
            if (!tutor_check(ops, STARS_TUTOR_CHECK_MESSAGES, 9999, -1, 0, 0, 0, 0)) {
                state->idt_bold = 0x1ea; return 0;
            }
            state->idt_bold = 0x1ec; tutor_set_advance(state); return 1;
        }
        return 1;

    case 23:
        if (state->idt == 0x1f0) {
            if (!tutor_check(ops, STARS_TUTOR_CHECK_MESSAGES, 3, -1, 0, 0, 0, 0)) {
                state->idt_bold = 0x1f0; return 0;
            }
            if (!state->slot_dialog) { state->idh = 0x3e9; state->idt_bold = 499; return 0; }
            if (!tutor_check(ops, STARS_TUTOR_CHECK_SCANNER, 3, -1, 0, 0, 0, 0)) {
                state->idt_bold = 0x1f1; return 0;
            }
            state->idt_bold = tutor_check(ops, STARS_TUTOR_CHECK_SHIP_BUILDER,
                4, -1, 0, 0, 0, 0) ? 0x1f5 : 500;
            return 0;
        }
        if (state->idt == 0x1f8) {
            if (tutor_product_count(ops, 0x0d) < 3 ||
                !tutor_check(ops, STARS_TUTOR_CHECK_QUEUE, 0x0d, 0, 2, 2, 3, 0)) {
                state->idt_bold = 0x1f8; return 0;
            }
            if (!tutor_check(ops, STARS_TUTOR_CHECK_MESSAGES, 4, -1, 0, 0, 0, 0)) {
                state->idt_bold = 0x1f9; return 0;
            }
            if (tutor_product_count(ops, 0x11) == 3 &&
                tutor_check(ops, STARS_TUTOR_CHECK_QUEUE, 0x11, 2, 1, 5, 2, 1)) {
                if (!tutor_check(ops, STARS_TUTOR_CHECK_MESSAGES, 6, -1, 0, 0, 0, 0) ||
                    !tutor_check(ops, STARS_TUTOR_CHECK_RESEARCH, 2, 3, 0x1e, 0, 0, 0)) {
                    state->idt_bold = 0x1fc; return 0;
                }
                if (!tutor_check(ops, STARS_TUTOR_CHECK_MESSAGES, 9999, -1, 0, 0, 0, 0)) {
                    state->idt_bold = 0x1fd; return 0;
                }
                state->idt_bold = 0x1ff; tutor_set_advance(state); return 1;
            }
            state->idt_bold = tutor_check(ops, STARS_TUTOR_CHECK_SELECTION,
                1, 0x11, 0, 0, 0, 0) ? 0x1fb : 0x1f9;
            return 0;
        }
        return 1;

    case 24:
        if (state->idt == 0x200) {
            if (!tutor_check(ops, STARS_TUTOR_CHECK_MESSAGES, 4, -1, 0, 0, 0, 0)) {
                state->idt_bold = 0x200; return 0;
            }
            if (!tutor_check(ops, STARS_TUTOR_CHECK_COLONIZE_WP, 9, 0, -1, 0, 0, 0)) {
                state->idt_bold = tutor_check(ops, STARS_TUTOR_CHECK_SELECTION,
                    2, 9, 0, 0, 0, 0) ? 0x202 : 0x201; return 0;
            }
            tutor_set_hidden(state);
            if (!tutor_check(ops, STARS_TUTOR_CHECK_COLONIZE_WP, 9, 1, -1, 0, 0, 0)) {
                tutor_clear_hidden(state); tutor_check(ops, STARS_TUTOR_CHECK_COLONIZE_WP,
                    9, 0, -1, 0, 0, 0); state->idt_bold = 0x204; return 0;
            }
            if (!tutor_check(ops, STARS_TUTOR_CHECK_COLONIZE_WP, 10, 0x17, -1, 0, 0, 0)) {
                tutor_clear_hidden(state); tutor_check(ops, STARS_TUTOR_CHECK_COLONIZE_WP,
                    10, 0, -1, 0, 0, 0); state->idt_bold = 0x205; return 0;
            }
            tutor_clear_hidden(state);
            state->idt_bold = 0x206;
            return tutor_check(ops, STARS_TUTOR_CHECK_MESSAGES, 9999, -1, 0, 0, 0, 0);
        }
        if (state->idt == 0x208) {
            if (tutor_product_count(ops, 0x0d) < 4 ||
                !tutor_check(ops, STARS_TUTOR_CHECK_QUEUE, 0x0d, 1, 2, 3, 3, 0)) {
                state->idt_bold = 0x209; return 0;
            }
            tutor_set_hidden(state);
            if (!tutor_check(ops, STARS_TUTOR_CHECK_FLEET_WP, 4, 1, 2, 0x205, -1, -1)) {
                tutor_clear_hidden(state); tutor_check(ops, STARS_TUTOR_CHECK_COLONIZE_WP,
                    4, 5, -1, 0, 0, 0);
                state->idt_bold = tutor_check(ops, STARS_TUTOR_CHECK_SUMMARY,
                    2, 0x205, 0, 0, 0, 0) ? 0x20d : 0x20b; return 0;
            }
            state->idt_bold = 0x20f; state->flags = (StarsU16)
                ((state->flags & (StarsU16)~0x0028U) | 8U); return 1;
        }
        return 1;

    case 25:
        if (state->idt == 0x210) {
            tutor_set_hidden(state);
            if (!tutor_check(ops, STARS_TUTOR_CHECK_FLEET_WP, 4, 0, 2, 0x205, -1, -1)) {
                tutor_clear_hidden(state);
                state->idt_bold = tutor_check(ops, STARS_TUTOR_CHECK_SELECTION,
                    2, 4, 0, 0, 0, 0) ? 0x212 : 0x210;
                if (state->idt_bold == 0x212) state->idh = 0x5ee;
                return 0;
            }
            tutor_clear_hidden(state);
            if (!tutor_check(ops, STARS_TUTOR_CHECK_MESSAGES, 1, -1, 0, 0, 0, 0)) {
                state->idt_bold = 0x214; return 0;
            }
            if (!tutor_check(ops, STARS_TUTOR_CHECK_CARGO, 0x0c, 0, 0, 0, 0x276, 0)) {
                state->idt_bold = tutor_check(ops, STARS_TUTOR_CHECK_SELECTION,
                    2, 0x0c, 0, 0, 0, 0) ? 0x217 : 0x214; return 0;
            }
            return 1;
        }
        if (state->idt == 0x218) {
            if (!tutor_fleet_present(ops, 0x0c)) {
                if (!tutor_check(ops, STARS_TUTOR_CHECK_MESSAGES, 8, -1, 0, 0, 0, 0))
                    { state->idt_bold = 0x21a; return 0; }
                if (!tutor_check(ops, STARS_TUTOR_CHECK_MESSAGES, 9999, -1, 0, 0, 0, 0))
                    { state->idt_bold = 0x21c; return 0; }
                state->idh = 0x1771; state->idt_bold = 0x21e;
                return state->slot_dialog || state->player_designs == 9;
            }
            state->idt_bold = 0x219; return 0;
        }
        if (state->idt == 0x220) {
            if (!state->slot_dialog && state->player_designs > 8) {
                if (tutor_product_count(ops, 0x0d) < 3 ||
                    !tutor_check(ops, STARS_TUTOR_CHECK_QUEUE, 0x0d, 0, 2, 8, 10, 0)) {
                    state->idt_bold = 0x226; return 0;
                }
                state->idt_bold = 0x227; tutor_set_advance(state); return 1;
            }
            if (!tutor_check(ops, STARS_TUTOR_CHECK_SHIP_BUILDER, 4, -1, 0, 0, 0, 0)) {
                state->idt_bold = state->player_designs < 9 ? 0x221 : 0x225; return 0;
            }
            state->idt_bold = state->player_designs >= 9 ? 0x225 : 0x222;
            return 0;
        }
        return 1;

    case 26:
        if (state->idt == 0x228) {
            if (!tutor_check(ops, STARS_TUTOR_CHECK_FLEET_WP, 4, 1, 1, 5, -1, -1)) {
                state->idt_bold = tutor_check(ops, STARS_TUTOR_CHECK_SELECTION,
                    2, 4, 0, 0, 0, 0) ? 0x22a : 0x228; return 0;
            }
            if (!tutor_check(ops, STARS_TUTOR_CHECK_MESSAGES, 6, -1, 0, 0, 0, 0)) {
                state->idt_bold = 0x22b; return 0;
            }
            if (!tutor_check(ops, STARS_TUTOR_CHECK_FLEET_WP, 0x0c, 1, 1, 10, -1, -1)) {
                state->idt_bold = tutor_check(ops, STARS_TUTOR_CHECK_SELECTION,
                    2, 0x0c, 0, 0, 0, 0) ? 0x22c : 0x22b; return 0;
            }
            if (!tutor_check(ops, STARS_TUTOR_CHECK_MESSAGES, 9999, -1, 0, 0, 0, 0)) {
                state->idt_bold = 0x22d; return 0;
            }
            state->idt_bold = 0x22f; tutor_set_advance(state); return 1;
        }
        return 1;

    case 27:
        if (state->idt == 0x230) {
            tutor_set_hidden(state);
            if (!tutor_check(ops, STARS_TUTOR_CHECK_FLEET_WP, 3, 1, 1, 0x0d, -1, 5)) {
                tutor_clear_hidden(state);
                state->idt_bold = tutor_check(ops, STARS_TUTOR_CHECK_SELECTION,
                    2, 3, 0, 0, 0, 0) ? 0x232 : 0x230; return 0;
            }
            tutor_clear_hidden(state);
            if (!tutor_check(ops, STARS_TUTOR_CHECK_MESSAGES, 6, -1, 0, 0, 0, 0)) {
                state->idt_bold = 0x234; return 0;
            }
            if (!tutor_check(ops, STARS_TUTOR_CHECK_FLEET_WP, 0x0d, 1, 1, 10, -1, -1)) {
                state->idt_bold = 0x235; return 0;
            }
            if (!tutor_check(ops, STARS_TUTOR_CHECK_PLANET_ROUTE, 0x0d, 10, 0, 0, 0, 0)) {
                state->idt_bold = 0x237; return 0;
            }
            return 1;
        }
        if (state->idt == 0x238) {
            if (!tutor_check(ops, STARS_TUTOR_CHECK_MESSAGES, 0x11, -1, 0, 0, 0, 0)) {
                state->idt_bold = 0x239; return 0;
            }
            if (!tutor_check(ops, STARS_TUTOR_CHECK_RESEARCH, 3, 0, 0x1e, 0, 0, 0)) {
                state->idt_bold = 0x23a; return 0;
            }
            if (!tutor_check(ops, STARS_TUTOR_CHECK_MESSAGES, 9999, -1, 0, 0, 0, 0)) {
                state->idt_bold = 0x23b; return 0;
            }
            tutor_set_hidden(state);
            if (!tutor_check(ops, STARS_TUTOR_CHECK_FLEET_WP, 6, 0, 1, 0x11, 5, -1)) {
                tutor_clear_hidden(state);
                state->idt_bold = tutor_check(ops, STARS_TUTOR_CHECK_SELECTION,
                    2, 6, 0, 0, 0, 0) ? 0x23e : 0x23c; return 0;
            }
            tutor_clear_hidden(state); return 1;
        }
        if (state->idt == 0x240) {
            if (!state->slot_dialog) {
                if (state->player_designs < 10) { state->idh = 0x1771; state->idt_bold = 0x241; return 0; }
                if (tutor_product_count(ops, 0x0d) < 3 ||
                    !tutor_check(ops, STARS_TUTOR_CHECK_QUEUE, 0x0d, 0, 2, 9, 10, 0)) {
                    state->idt_bold = 0x246; return 0;
                }
                state->idt_bold = 0x247; tutor_set_advance(state); return 1;
            }
            if (!tutor_check(ops, STARS_TUTOR_CHECK_SHIP_BUILDER, 4, -1, 0, 0, 0, 0)) {
                state->idt_bold = 0x242; return 0;
            }
            state->idt_bold = state->player_designs >= 10 ? 0x245 : 0x243;
            return 0;
        }
        return 1;

    case 28:
        if (state->idt == 0x248) {
            if (!tutor_check(ops, STARS_TUTOR_CHECK_MESSAGES, 5, -1, 0, 0, 0, 0)) {
                state->idt_bold = 0x24a; return 0;
            }
            if (!tutor_check(ops, STARS_TUTOR_CHECK_MESSAGES, 9999, -1, 0, 0, 0, 0)) {
                state->idt_bold = tutor_check(ops, STARS_TUTOR_CHECK_SELECTION,
                    2, 0x0e, 0, 0, 0, 0) ? 0x24c : 0x24a; return 0;
            }
            state->idt_bold = 0x24e; tutor_set_advance(state); return 1;
        }
        return 1;

    case 29:
        if (state->idt == 0x250) {
            if (!tutor_check(ops, STARS_TUTOR_CHECK_MESSAGES, 6, -1, 0, 0, 0, 0) ||
                !tutor_check(ops, STARS_TUTOR_CHECK_SELECTION, 1, 5, 0, 0, 0, 0)) {
                state->idt_bold = 0x250; return 0;
            }
            if (tutor_product_count(ops, 5) < 4 ||
                !tutor_check(ops, STARS_TUTOR_CHECK_QUEUE, 5, 0, 1, 8, 100, 1)) {
                state->idt_bold = 0x252; return 0;
            }
            if (!tutor_check(ops, STARS_TUTOR_CHECK_MESSAGES, 9999, -1, 0, 0, 0, 0)) {
                state->idt_bold = 0x254; return 0;
            }
            state->idt_bold = 0x256; tutor_set_advance(state); return 1;
        }
        return 1;

    case 30:
        if (state->idt == 0x258) {
            if (!tutor_check(ops, STARS_TUTOR_CHECK_MESSAGES, 2, -1, 0, 0, 0, 0)) {
                state->idt_bold = 0x258; return 0;
            }
            if ((state->flags & 0x0600U) == 0U) { state->idt_bold = 0x25a; return 0; }
            if (!tutor_check(ops, STARS_TUTOR_CHECK_MESSAGES, 0x12, -1, 0, 0, 0, 0)) {
                state->idt_bold = tutor_check(ops, STARS_TUTOR_CHECK_SELECTION,
                    2, 0x0c, 0, 0, 0, 0) ? 0x25c : 0x258; return 0;
            }
            state->idt_bold = 0x25e;
            if (!tutor_check(ops, STARS_TUTOR_CHECK_MESSAGES, 9999, -1, 0, 0, 0, 0)) return 0;
            tutor_set_advance(state); return 1;
        }
        return 1;

    case 31:
        if (state->idt == 0x260) {
            if (!tutor_check(ops, STARS_TUTOR_CHECK_MESSAGES, 10, -1, 0, 0, 0, 0)) {
                state->idt_bold = 0x260; return 0;
            }
            if (tutor_product_count(ops, 0x0d) < 3 ||
                !tutor_check(ops, STARS_TUTOR_CHECK_QUEUE, 0x0d, 0, 2, 9, 10, 0)) {
                state->idt_bold = tutor_check(ops, STARS_TUTOR_CHECK_SELECTION,
                    1, 0x0d, 0, 0, 0, 0) ? 0x261 : 0x260; return 0;
            }
            if (!tutor_check(ops, STARS_TUTOR_CHECK_MESSAGES, 9999, -1, 0, 0, 0, 0)) {
                state->idt_bold = 0x263; return 0;
            }
            state->idt_bold = 0x264; tutor_set_advance(state); return 1;
        }
        return 1;

    case 32:
        if (state->idt == 0x268) {
            state->idt_bold = 0x26b;
            if (!tutor_check(ops, STARS_TUTOR_CHECK_MESSAGES, 9999, -1, 0, 0, 0, 0)) return 0;
            tutor_set_advance(state); return 1;
        }
        return 1;

    case 33:
        if (state->idt == 0x270) {
            if (!tutor_check(ops, STARS_TUTOR_CHECK_MESSAGES, 9999, -1, 0, 0, 0, 0)) {
                state->idt_bold = 0x270; return 0;
            }
            state->idt_bold = 0x274; tutor_set_advance(state); return 1;
        }
        return 1;

    case 34:
        if (state->idt == 0x278) {
            if (!tutor_check(ops, STARS_TUTOR_CHECK_MESSAGES, 9999, -1, 0, 0, 0, 0)) {
                state->idt_bold = 0x27c; return 0;
            }
            state->idt_bold = 0x27d; tutor_set_advance(state); return 1;
        }
        return 1;
    default:
        return 1;
    }
}

/* Typed semantic reconstruction of FTutorTaskDone, NE 20:0FBC-645F.  The
 * original dispatcher is retained as named check families and ordered C
 * decision chains rather than as assembly or emitted-byte replay. */
StarsI16 stars_tutor_task_done(StarsTutorTaskState *state,
    const StarsTutorTaskOps *ops)
{
    if (!state)
        return 0;
    switch (state->turn) {
    case 0: return tutor_turn_zero(state, ops);
    case 1: return tutor_turn_one(state, ops);
    case 2: return tutor_turn_two(state, ops);
    case 3: return tutor_turn_three(state, ops);
    case 4: return tutor_turn_four(state, ops);
    case 5: return tutor_turn_five(state, ops);
    case 6: return tutor_turn_six(state, ops);
    case 7: return tutor_turn_seven(state, ops);
    case 8: return tutor_turn_eight(state, ops);
    case 9: return tutor_turn_nine(state, ops);
    case 10: return tutor_turn_ten(state, ops);
    default: return tutor_turn_late(state, ops);
    }
}
