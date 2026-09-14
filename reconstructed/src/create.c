/* Reviewed victory-setting dependencies of GenNewGameFromFile.
 * Explicit DGROUP preserves the original distinction between an argument game
 * record and the global game at offset 0x70. */
#include "core.h"

static StarsU16 victory_address(StarsU16 game, StarsI16 index)
{
    return (StarsU16)(game + 20U + (StarsU16)index);
}

static StarsI16 signed_word(StarsU16 value)
{
    return value & 0x8000U ? (StarsI16)(-1 - (StarsI16)(0xffffU - value)) : (StarsI16)value;
}

/* NE 16:B5BC-B60B. Any nonzero enabled value sets the high bit. */
void stars_set_victory_check(StarsU8 *data, StarsU16 game, StarsI16 index, StarsI16 enabled)
{
    StarsU16 address = victory_address(game, index);
    data[address] = (StarsU8)((data[address] & 127U) | (enabled ? 128U : 0U));
}

/* NE 16:B60C-B644. */
StarsI16 stars_get_victory_check(const StarsU8 *data, StarsU16 game, StarsI16 index)
{
    return (data[victory_address(game, index)] & 128U) ? 1 : 0;
}

/* NE 16:B710-B83F, including the ten-entry switch table at B81F.
 * Index 8 and out-of-range indices use GLOBAL condition flags, omitting index 2. */
StarsI16 stars_get_victory_value(const StarsU8 *data, StarsU16 game, StarsI16 index, StarsI16 raw)
{
    StarsU16 value = data[victory_address(game, index)] & 127U;
    StarsU16 count, i;
    if (raw) return (StarsI16)value;
    switch (index) {
    case 0: value = (StarsU16)(value * 5U + 20U); break;
    case 1: value = (StarsU16)(value + 8U); break;
    case 2: value = (StarsU16)(value + 2U); break;
    case 3: value = (StarsU16)(value * 1000U + 1000U); break;
    case 4: value = (StarsU16)(value * 10U + 20U); break;
    case 5:
    case 6: value = (StarsU16)(value * 10U + 10U); break;
    case 7:
    case 9: value = (StarsU16)(value * 10U + 30U); break;
    default:
        count = 0;
        for (i = 0; i < 8U; ++i)
            if (i != 2U && (data[0x84U + i] & 128U)) ++count;
        if (count < value) value = count;
        break;
    }
    return signed_word(value);
}

/* NE 16:B644-B70F. Table at CS:B5A8-B5BC; valid field indices only.
 * A negative value clamps to zero. Index 8 also clamps to checked conditions. */
StarsI16 stars_set_victory_value(StarsU8 *data, StarsU16 game, StarsI16 index, StarsI16 value)
{
    static const StarsI16 maximum[10] = {16, 18, 4, 19, 28, 49, 29, 87, 7, 47};
    StarsU16 address = victory_address(game, index);
    StarsI16 effective;
    if (value < 0) value = 0;
    else if (value > maximum[index]) value = maximum[index];
    data[address] = (StarsU8)((data[address] & 128U) | (StarsU8)value);
    if (index == 8) {
        effective = stars_get_victory_value(data, game, 8, 0);
        if (effective != value) {
            value = effective;
            data[address] = (StarsU8)((data[address] & 128U) | (StarsU8)value);
        }
    }
    return value;
}

/* NE 16:00C0-0136. The original copies from CS, not DGROUP. */
void stars_init_battle_plan(StarsU8 *data, StarsU16 output, StarsI16 template_index, StarsI16 plan_id)
{
    static const StarsU8 templates[180] = {
#include "battle_plan_templates.inc"
    };
    StarsU16 i, flags;
    for (i = 0; i < 36U; ++i)
        data[(StarsU16)(output + i)] = templates[template_index * 36 + i];
    data[output] = (StarsU8)((data[output] & 240U) | ((StarsU16)plan_id & 15U));
    /* Inspect global fSinglePlr AFTER the copy, retaining possible aliasing. */
    if ((data[0x80U] & 4U) && template_index == 0) {
        flags = (StarsU16)(data[(StarsU16)(output + 2U)] |
                          ((StarsU16)data[(StarsU16)(output + 3U)] << 8));
        flags = (StarsU16)((flags & 0xe0ffU) | 0x300U);
        data[(StarsU16)(output + 2U)] = (StarsU8)flags;
        data[(StarsU16)(output + 3U)] = (StarsU8)(flags >> 8);
    }
}

/* NE 16:B570-B5A7. Four 192-byte difficulty variants per computer race.
 * No bounds check: only the offset wraps; the CS selector remains unchanged. */
StarsU32 stars_computer_player_pointer(StarsU16 code_selector, StarsI16 race, StarsI16 level)
{
    StarsU16 offset = (StarsU16)(0xa370U + (StarsU16)race * 0x300U + (StarsU16)level * 0xc0U);
    return ((StarsU32)code_selector << 16) | offset;
}

/* NE 16:74F8-7507. Original contains only frame/register setup and return. */
void stars_init_new_game3(void)
{
}
