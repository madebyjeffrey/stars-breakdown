#include "core.h"

#define STARS_SCAN_SHOW_COLORS 0x2000U
#define STARS_SCAN_FLEET_MARK 0x0800U
#define STARS_SCAN_MAX_COUNT 999UL
#define STARS_SCAN_DIGIT_WIDTH 4
#define STARS_SCAN_DIGIT_HEIGHT 7
#define STARS_SCAN_DIGIT_STEP 2
#define STARS_SCAN_HUNDREDS_OFFSET -6
#define STARS_SCAN_TENS_OFFSET -3
#define STARS_SCAN_SINGLE_OFFSET -1
#define STARS_SCAN_ROP_COLOR 0x220326UL
#define STARS_SCAN_ROP_NUMBER 0xee0086UL
#define STARS_SCAN_BACKGROUND 0UL
#define STARS_SCAN_TEXT 0x00ffffffUL

static StarsU32 scan_fleet_total(StarsDrawScanFleetCountInput *input,
    StarsI16 *owner)
{
    StarsU16 index;
    StarsU32 total;
    StarsU32 visible;
    StarsShipsScanFleet *fleet;

    *owner = (input->scan.scan_flags & STARS_SCAN_SHOW_COLORS) != 0U ?
        (StarsI16)-1 : (StarsI16)-2;
    total = 0UL;
    for (index = 0; index < input->fleet_count; ++index) {
        fleet = &input->fleets[index];
        visible = stars_c_ships_scan_vis(fleet, &input->scan);
        if (visible != 0UL) {
            total += visible;
            if (*owner != (StarsI16)-2 && fleet->player != *owner)
                *owner = *owner == (StarsI16)-1 ? fleet->player :
                    (StarsI16)-2;
        }
        fleet->overlap_flags = (StarsU16)
            ((fleet->overlap_flags & (StarsU16)~STARS_SCAN_FLEET_MARK) |
             STARS_SCAN_FLEET_MARK);
    }
    return total;
}

static void scan_draw_digit(const StarsDrawScanFleetCountInput *input,
    StarsU16 hdc, StarsU16 hdc_mem, StarsI16 x, StarsI16 y,
    StarsU16 digit, StarsU32 color)
{
    StarsI16 source_x;

    source_x = (StarsI16)(digit * STARS_SCAN_DIGIT_WIDTH);
    if (color != STARS_SCAN_TEXT) {
        input->bit_blt(input->context, hdc, x, y, STARS_SCAN_DIGIT_WIDTH,
            STARS_SCAN_DIGIT_HEIGHT, hdc_mem, source_x, 0,
            STARS_SCAN_ROP_COLOR);
        input->set_color(input->context, hdc, color);
    }
    input->bit_blt(input->context, hdc, x, y, STARS_SCAN_DIGIT_WIDTH,
        STARS_SCAN_DIGIT_HEIGHT, hdc_mem, source_x, 0,
        STARS_SCAN_ROP_NUMBER);
    if (color != STARS_SCAN_TEXT)
        input->set_color(input->context, hdc, STARS_SCAN_TEXT);
}

/* Genuine C reconstruction of DrawScanFleetCount, NE 0C:47D2-4BF3. */
void stars_draw_scan_fleet_count(StarsI16 x, StarsI16 y, StarsU16 hdc,
    StarsU16 hdc_mem, StarsDrawScanFleetCountInput *input)
{
    StarsU32 total;
    StarsU32 color;
    StarsU16 old_bitmap;
    StarsU16 hundreds;
    StarsU16 tens;
    StarsU16 ones;
    StarsI16 owner;
    StarsI16 digit_x;
    StarsI16 digit_y;
    StarsI16 has_hundreds;

    if (!input || !input->fleets || !input->select_object ||
        !input->set_color || !input->bit_blt)
        return;
    total = scan_fleet_total(input, &owner);
    if (total == 0UL)
        return;
    if (total > STARS_SCAN_MAX_COUNT)
        total = STARS_SCAN_MAX_COUNT;
    hundreds = (StarsU16)(total / 100UL);
    tens = (StarsU16)((total % 100UL) / 10UL);
    ones = (StarsU16)(total % 10UL);
    has_hundreds = hundreds != 0U;
    old_bitmap = input->select_object(input->context, hdc_mem,
        input->numbers_bitmap);
    input->set_color(input->context, hdc, STARS_SCAN_BACKGROUND);
    input->set_color(input->context, hdc, STARS_SCAN_TEXT);
    color = STARS_SCAN_TEXT;
    if (owner >= 0 && owner != input->scan.local_player &&
        input->player_color != 0)
        color = input->player_color(input->context, owner);
    digit_y = (StarsI16)(y - STARS_SCAN_DIGIT_HEIGHT);
    digit_x = (StarsI16)(x + STARS_SCAN_SINGLE_OFFSET);
    if (has_hundreds) {
        scan_draw_digit(input, hdc, hdc_mem,
            (StarsI16)(x + STARS_SCAN_HUNDREDS_OFFSET), digit_y,
            hundreds, color);
        digit_x = (StarsI16)(x + STARS_SCAN_DIGIT_STEP);
    }
    if (tens != 0U || has_hundreds) {
        scan_draw_digit(input, hdc, hdc_mem,
            (StarsI16)(digit_x + STARS_SCAN_TENS_OFFSET), digit_y,
            tens, color);
        digit_x = (StarsI16)(digit_x + STARS_SCAN_DIGIT_STEP);
    }
    scan_draw_digit(input, hdc, hdc_mem, digit_x, digit_y, ones, color);
    input->select_object(input->context, hdc_mem, old_bitmap);
}
