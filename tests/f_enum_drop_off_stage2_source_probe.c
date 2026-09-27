#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct DropoffState {
    StarsPoint points[4];
    int calls;
} DropoffState;

static void get_point(void *context, StarsU16 id, StarsPoint *point)
{
    DropoffState *state = (DropoffState *)context;
    ++state->calls;
    *point = state->points[id];
}

static void put_word(StarsU8 *bytes, StarsU16 offset, StarsU16 value)
{
    bytes[offset] = (StarsU8)value;
    bytes[offset + 1U] = (StarsU8)(value >> 8);
}

static void put_long(StarsU8 *bytes, StarsU16 offset, StarsU32 value)
{
    bytes[offset] = (StarsU8)value;
    bytes[offset + 1U] = (StarsU8)(value >> 8);
    bytes[offset + 2U] = (StarsU8)(value >> 16);
    bytes[offset + 3U] = (StarsU8)(value >> 24);
}

int main(void)
{
    static const StarsDropoffOps ops = { get_point };
    StarsU8 source[56];
    StarsU8 test[56];
    StarsU8 ai_data[32];
    DropoffState state;
    StarsI16 result;
    int failures = 0;

    memset(source, 0, sizeof source);
    memset(test, 0, sizeof test);
    memset(ai_data, 0, sizeof ai_data);
    memset(&state, 0, sizeof state);
    put_word(source, 0, 0);
    put_word(test, 0, 1);
    put_word(test, 2, 4);
    put_long(test, 42, (StarsU32)-1L);
    put_long(test, 31, 500U);
    state.points[0].x = 0;
    state.points[0].y = 0;
    state.points[1].x = 400;
    state.points[1].y = 0;
    put_word(ai_data, 5 * 2 + 2 + 1 * 2, 2U << 3);
    result = stars_f_enum_drop_off_stage2(source, test, 5, 4, ai_data,
        &ops, &state);
    if (result != 1 || state.calls != 2)
        ++failures;

    put_long(test, 42, 0);
    put_long(test, 31, 900U);
    put_word(ai_data, 5 * 2 + 2 + 1 * 2, 1U << 3);
    result = stars_f_enum_drop_off_stage2(source, test, 5, 4, ai_data,
        &ops, &state);
    if (result != 0 || state.calls != 2)
        ++failures;

    put_long(test, 42, (StarsU32)-1L);
    put_word(ai_data, 5 * 2 + 2 + 1 * 2, 3U << 3);
    result = stars_f_enum_drop_off_stage2(source, test, 5, 4, ai_data,
        &ops, &state);
    if (result != 0)
        ++failures;
    printf("F_ENUM_DROPOFF2_CHECKS=3 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures != 0;
}
