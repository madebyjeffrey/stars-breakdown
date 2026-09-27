#include "core.h"

#define FLEET_BITMAP_WHITE_TEXT 0x00ffffffUL
#define FLEET_BITMAP_FRAME_ROP 0xf00021UL
#define FLEET_BITMAP_MARKER_ROP 0xff0062UL
#define FLEET_BITMAP_DIB_ROP 0xcc0020UL

enum {
    FLEET_BITMAP_FULL_SIZE = 0x40,
    FLEET_BITMAP_SMALL_SIZE = 0x20,
    FLEET_BITMAP_FULL_RACE_SIZE = 0x10,
    FLEET_BITMAP_SMALL_RACE_SIZE = 8,
    FLEET_BITMAP_FRAME_SIZE = 0x44,
    FLEET_BITMAP_FRAME_INNER = 0x42,
    FLEET_BITMAP_FRAME_EDGE = 2,
    FLEET_BITMAP_FULL_MARKER_STEP = 8,
    FLEET_BITMAP_SMALL_MARKER_STEP = 5,
    FLEET_BITMAP_FULL_MARKER_THICKNESS = 2,
    FLEET_BITMAP_SMALL_MARKER_THICKNESS = 1,
    FLEET_BITMAP_SPRITE_COLUMNS = 0x20,
    FLEET_BITMAP_SPRITE_GROUP_SIZE = 0x94,
    FLEET_BITMAP_SPRITE_GROUP_SHIFT = 5,
    FLEET_BITMAP_QUADRANT_COUNT = 4,
    FLEET_BITMAP_RACE_COLUMN_MASK = 7,
    FLEET_BITMAP_RACE_ROW_MASK = 0x1f,
    FLEET_BITMAP_MAX_MARKERS = 4,
    FLEET_BITMAP_MARKER_MIN_COUNT = 2
};

static StarsI16 clamp_marker_count(StarsI16 differences)
{
    if (differences > FLEET_BITMAP_MAX_MARKERS)
        differences = FLEET_BITMAP_MAX_MARKERS;
    return (StarsI16)(differences - 1);
}

static void draw_marker_pair(const StarsDrawFleetBitmapInput *input,
    StarsI16 marker, StarsI16 size, StarsI16 step)
{
    StarsI16 x;
    StarsI16 y;

    if ((marker & 1) == ((marker & 2) == 2))
        x = (StarsI16)(input->x + 2);
    else
        x = (StarsI16)(input->x + size - 2 - step);
    if ((marker & 2) == 0)
        y = (StarsI16)(input->y + 2);
    else
        y = (StarsI16)(input->y + size - 2 - step);
    if (input->ops.pattern_blt) {
        input->ops.pattern_blt(input->context, input->hdc, x,
            (StarsI16)(y + (step - 1) / 2), step,
            size == FLEET_BITMAP_FULL_SIZE ?
                FLEET_BITMAP_FULL_MARKER_THICKNESS :
                FLEET_BITMAP_SMALL_MARKER_THICKNESS, FLEET_BITMAP_MARKER_ROP);
        input->ops.pattern_blt(input->context, input->hdc,
            (StarsI16)(x + (step - 1) / 2), y,
            size == FLEET_BITMAP_FULL_SIZE ?
                FLEET_BITMAP_FULL_MARKER_THICKNESS :
                FLEET_BITMAP_SMALL_MARKER_THICKNESS, step,
            FLEET_BITMAP_MARKER_ROP);
    }
}

static void draw_frame(const StarsDrawFleetBitmapInput *input)
{
    StarsU16 old_brush;

    if (!input->ops.pattern_blt || !input->ops.select_object)
        return;
    old_brush = input->ops.select_object(input->context, input->hdc,
        input->brush_shadow);
    input->ops.pattern_blt(input->context, input->hdc, input->x, input->y,
        FLEET_BITMAP_FRAME_SIZE, 2, FLEET_BITMAP_FRAME_ROP);
    input->ops.pattern_blt(input->context, input->hdc, input->x, input->y,
        2, FLEET_BITMAP_FRAME_SIZE, FLEET_BITMAP_FRAME_ROP);
    input->ops.select_object(input->context, input->hdc,
        input->brush_highlight);
    input->ops.pattern_blt(input->context, input->hdc,
        (StarsI16)(input->x + 2),
        (StarsI16)(input->y + 0x42), 0x42, 1, FLEET_BITMAP_FRAME_ROP);
    input->ops.pattern_blt(input->context, input->hdc,
        (StarsI16)(input->x + 3),
        (StarsI16)(input->y + 0x42), 0x41, 1, FLEET_BITMAP_FRAME_ROP);
    input->ops.pattern_blt(input->context, input->hdc,
        (StarsI16)(input->x + 0x42), (StarsI16)(input->y + 2), 1, 0x40,
        FLEET_BITMAP_FRAME_ROP);
    input->ops.pattern_blt(input->context, input->hdc,
        (StarsI16)(input->x + 0x43), (StarsI16)(input->y + 1), 1, 0x41,
        FLEET_BITMAP_FRAME_ROP);
    input->ops.select_object(input->context, input->hdc, old_brush);
}

/* NE 0B:490E-4DF4. Paint a fleet sprite, its optional race badge, and either
 * the small corner difference markers or the full-size count labels. */
void stars_draw_fleet_bitmap(StarsDrawFleetBitmapInput *input)
{
    StarsI16 bitmap_index;
    StarsI16 differences;
    StarsU16 sprite_group;
    StarsU16 sprite_number;
    StarsI16 size;
    StarsI16 race_size;
    StarsI16 marker_step;
    StarsI16 marker_count;
    StarsI16 marker;
    StarsI16 src_x;
    StarsI16 src_y;
    StarsI16 race_src_x;
    StarsI16 race_src_y;
    StarsI16 text_length;
    StarsI32 text_width;
    StarsU8 text[32];

    if (!input)
        return;
    bitmap_index = input->bitmap_index;
    differences = input->differences;
    if (input->draw_frame)
        draw_frame(input);
    if (bitmap_index < 0 && input->ops.lookup_primary)
        bitmap_index = input->ops.lookup_primary(input->context, input->fleet,
            &differences);
    sprite_number = (StarsU16)(bitmap_index % FLEET_BITMAP_SPRITE_GROUP_SIZE);
    if (input->ops.select_palette)
        input->ops.select_palette(input->context, input->hdc, input->palette, 0);
    if (input->ops.realize_palette)
        input->ops.realize_palette(input->context, input->hdc);

    if (input->shrink == 0) {
        size = FLEET_BITMAP_FULL_SIZE;
        race_size = FLEET_BITMAP_FULL_RACE_SIZE;
        marker_step = FLEET_BITMAP_FULL_MARKER_STEP;
        sprite_group = (StarsU16)(sprite_number >>
            FLEET_BITMAP_SPRITE_GROUP_SHIFT);
        src_x = (StarsI16)(((sprite_number & 0x1fU) >> 2) * size);
        src_y = (StarsI16)((FLEET_BITMAP_QUADRANT_COUNT - 1 -
            (sprite_number & 3U)) * size);
        if (input->ops.draw_dib)
            input->ops.draw_dib(input->context, input->hdc, input->x, input->y,
                size, size, input->ship_dibs[sprite_group], src_x, src_y,
                size, size, FLEET_BITMAP_DIB_ROP);
        if (input->race_bitmap >= 0 && input->ops.draw_dib) {
            race_src_x = (StarsI16)((input->race_bitmap &
                FLEET_BITMAP_RACE_COLUMN_MASK) * race_size);
            race_src_y = (StarsI16)((FLEET_BITMAP_QUADRANT_COUNT - 1 -
                ((input->race_bitmap & FLEET_BITMAP_RACE_ROW_MASK) >> 3)) *
                race_size);
            input->ops.draw_dib(input->context, input->hdc, input->x,
                (StarsI16)(input->y + 0x30), race_size, race_size,
                input->race_dib_full, race_src_x, race_src_y, race_size,
                race_size, FLEET_BITMAP_DIB_ROP);
        }
    } else {
        size = FLEET_BITMAP_SMALL_SIZE;
        race_size = FLEET_BITMAP_SMALL_RACE_SIZE;
        marker_step = FLEET_BITMAP_SMALL_MARKER_STEP;
        sprite_group = (StarsU16)(sprite_number >>
            FLEET_BITMAP_SPRITE_GROUP_SHIFT);
        src_x = (StarsI16)(((sprite_number & 0x1fU) >> 2) * size);
        src_y = (StarsI16)((FLEET_BITMAP_QUADRANT_COUNT - 1 -
            (sprite_number & 3U)) * size);
        if (input->ops.draw_dib)
            input->ops.draw_dib(input->context, input->hdc, input->x, input->y,
                size, size, input->ship_dibs_small[sprite_group], src_x, src_y,
                size, size, FLEET_BITMAP_DIB_ROP);
        if (input->race_bitmap >= 0 && input->ops.draw_dib) {
            race_src_x = (StarsI16)((input->race_bitmap &
                FLEET_BITMAP_RACE_COLUMN_MASK) * race_size);
            race_src_y = (StarsI16)((FLEET_BITMAP_QUADRANT_COUNT - 1 -
                ((input->race_bitmap & FLEET_BITMAP_RACE_ROW_MASK) >> 3)) *
                race_size);
            input->ops.draw_dib(input->context, input->hdc, input->x,
                (StarsI16)(input->y + 0x18), race_size, race_size,
                input->race_dib_small, race_src_x, race_src_y, race_size,
                race_size, FLEET_BITMAP_DIB_ROP);
        }
    }

    if (input->ship_count == 0 || input->shrink != 0) {
        marker_count = clamp_marker_count(differences);
        for (marker = 0; marker < marker_count; ++marker)
            draw_marker_pair(input, marker, size, marker_step);
        return;
    }

    if (!input->ops.select_object)
        return;
    (void)input->ops.select_object(input->context, input->hdc,
        input->font_arial7);
    if (input->ops.set_text_color)
        input->ops.set_text_color(input->context, input->hdc,
            FLEET_BITMAP_WHITE_TEXT);
    if (differences > FLEET_BITMAP_MARKER_MIN_COUNT - 1 &&
            input->ops.format_count && input->ops.text_out) {
        text_length = input->ops.format_count(input->context, text,
            (StarsU16)sizeof text, differences);
        input->ops.text_out(input->context, input->hdc,
            (StarsI16)(input->x + 1), (StarsI16)(input->y + 1), text,
            text_length);
    }
    if (input->ship_count > FLEET_BITMAP_MARKER_MIN_COUNT - 1 &&
            input->ops.format_count && input->ops.text_out) {
        text_length = input->ops.format_count(input->context, text,
            (StarsU16)sizeof text, input->ship_count);
        text_width = input->ops.text_extent ?
            input->ops.text_extent(input->context, input->hdc, text,
                text_length) : 0;
        input->ops.text_out(input->context, input->hdc,
            (StarsI16)(input->x + size - 1 - (StarsI16)text_width),
            (StarsI16)(input->y + 1), text, text_length);
    }
}
