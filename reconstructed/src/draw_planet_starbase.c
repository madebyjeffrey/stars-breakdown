#include "core.h"

#define STARBASE_TILE_SELECTED 0x0200U
#define STARBASE_TILE_RESET 0x0800U
#define STARBASE_REFERENCE_ACTION 13
#define STARBASE_REFERENCE_DETAILS 14
#define STARBASE_REFERENCE_GAUGE 15
#define STARBASE_REFERENCE_WARP 16
#define STARBASE_EMPTY_LEFT (-5)
#define STARBASE_EMPTY_TOP (-5)
#define STARBASE_EMPTY_RIGHT (-6)
#define STARBASE_EMPTY_BOTTOM (-6)

#define STARBASE_PLANET_SIZE 56U
#define STARBASE_DESIGN_SIZE 147U
#define STARBASE_HULL_SIZE 143U
#define STARBASE_FLAG_WORD 4U
#define STARBASE_WORD 44U
#define STARBASE_HULL_ID 0U
#define STARBASE_PLANET_PLAYER 2U
#define STARBASE_HULL_CLASS 8U
#define STARBASE_CARGO_MAX 52U
#define STARBASE_DEFENSE 56U

static StarsU16 starbase_read16(const StarsU8 *raw, StarsU16 offset)
{
    return (StarsU16)(raw[offset] | ((StarsU16)raw[offset + 1U] << 8));
}

/* Decode the direct _planet, _shdef, and _huldef fields consumed by the
 * original DrawPlanetStarbase.  The caller supplies the same table slices
 * that the Win16 far-pointer lookups traverse; no host pointer layout is
 * assumed. */
StarsI16 stars_decode_planet_starbase(
    const StarsPlanetStarbasePackedInput *input, StarsPlanetStarbaseData *data)
{
    StarsU16 flags;
    StarsU16 starbase_low;
    StarsU16 starbase_high;
    StarsU16 design_slot;
    StarsU16 hull_id;
    StarsU16 hull_slot;
    StarsU16 index;
    const StarsU8 *design;
    const StarsU8 *hull;

    if (!input || !data || !input->planet ||
            !input->starbase_designs || !input->starbase_hulls)
        return 0;
    data->has_starbase = 0;
    data->hull_id = -1;
    data->starbase_design = -1;
    data->cargo_capacity = 0;
    data->defense = 0;
    data->mass_warp_packed = 0;
    data->planet_id = (StarsI16)starbase_read16(input->planet, 0U);
    data->player_id = (StarsI16)starbase_read16(input->planet,
        STARBASE_PLANET_PLAYER);
    data->destination_id = -1;
    data->hull_class[0] = 0;
    flags = starbase_read16(input->planet, STARBASE_FLAG_WORD);
    data->has_starbase = (StarsI16)((flags >> 9) & 1U);
    if (!data->has_starbase)
        return 1;
    if (input->design_count == 0U || input->hull_count == 0U)
        return 0;
    starbase_low = starbase_read16(input->planet, STARBASE_WORD);
    starbase_high = starbase_read16(input->planet, STARBASE_WORD + 2U);
    design_slot = (StarsU16)(starbase_low & 0x000fU);
    if (design_slot >= input->design_count)
        return 0;
    design = input->starbase_designs +
        (StarsU16)(design_slot * STARBASE_DESIGN_SIZE);
    hull_id = starbase_read16(design, STARBASE_HULL_ID);
    if (hull_id >= 0x20U)
        hull_slot = (StarsU16)(hull_id - 0x20U);
    else
        hull_slot = hull_id;
    if (hull_slot >= input->hull_count)
        return 0;
    hull = input->starbase_hulls +
        (StarsU16)(hull_slot * STARBASE_HULL_SIZE);
    data->hull_id = (StarsI16)hull_id;
    data->starbase_design = (StarsI16)design_slot;
    for (index = 0U; index < 31U; ++index)
        data->hull_class[index] = design[STARBASE_HULL_CLASS + index];
    data->hull_class[31] = 0;
    data->cargo_capacity = starbase_read16(hull, STARBASE_CARGO_MAX);
    data->defense = starbase_read16(hull, STARBASE_DEFENSE);
    data->mass_warp_packed = starbase_low;
    if ((starbase_high & 0x03ffU) != 0U)
        data->destination_id = (StarsI16)((starbase_high & 0x03ffU) - 1U);
    return 1;
}

static void starbase_empty_rect(StarsRect *rect)
{
    rect->left = STARBASE_EMPTY_LEFT;
    rect->top = STARBASE_EMPTY_TOP;
    rect->right = STARBASE_EMPTY_RIGHT;
    rect->bottom = STARBASE_EMPTY_BOTTOM;
}

static void starbase_set_empty_references(StarsPlanetStarbaseInput *input)
{
    StarsI16 index;

    if (!input->reference_rects)
        return;
    for (index = STARBASE_REFERENCE_DETAILS;
         index <= STARBASE_REFERENCE_WARP; ++index)
        starbase_empty_rect(&input->reference_rects[index]);
}

static StarsI16 starbase_get_text(StarsPlanetStarbaseInput *input,
    StarsU16 id, StarsU8 *buffer, StarsU16 capacity)
{
    if (!input->ops->get_text)
        return 0;
    return input->ops->get_text(input->ops->context, id, buffer, capacity);
}

static void starbase_text(StarsPlanetStarbaseInput *input, StarsI16 x,
    StarsI16 y, const StarsU8 *text, StarsI16 length)
{
    if (input->ops->text_out)
        input->ops->text_out(input->ops->context, input->hdc, x, y, text,
            length);
}

static void starbase_right(StarsPlanetStarbaseInput *input, StarsI16 right,
    StarsI16 y, const StarsU8 *text, StarsI16 length, StarsI16 offset)
{
    if (input->ops->right_text)
        input->ops->right_text(input->ops->context, input->hdc, right, y,
            text, length, offset);
}

static StarsI16 starbase_value(StarsPlanetStarbaseInput *input,
    StarsU8 *buffer, StarsU16 capacity, StarsI32 value, StarsI16 mode)
{
    StarsI16 length;

    if (value == 0) {
        length = starbase_get_text(input, 0x560, buffer, capacity);
    } else if (mode == 0 && value == 0xffffL) {
        length = starbase_get_text(input, 0x2ed, buffer, capacity);
    } else if (input->ops->format_value) {
        length = input->ops->format_value(input->ops->context, buffer,
            capacity, value, mode);
    } else {
        buffer[0] = 0;
        length = 0;
    }
    return length;
}

static void starbase_draw_value(StarsPlanetStarbaseInput *input,
    StarsU8 *buffer, StarsU16 capacity, StarsI32 value, StarsI16 mode,
    StarsI16 right, StarsI16 y, StarsI16 offset)
{
    StarsI16 length;

    length = starbase_value(input, buffer, capacity, value, mode);
    starbase_right(input, right, y, buffer, length, offset);
}

/* Genuine semantic reconstruction of DrawPlanetStarbase, NE 0A:22CC-2AFA.
 * It preserves the common tile frame, detail-row ordering, cached reference
 * rectangles, sentinel rectangles for hidden controls, and the warp action
 * geometry. Localization, packed design lookup, and GDI calls remain
 * explicit services at the boundary. */
void stars_draw_planet_starbase(StarsPlanetStarbaseInput *input)
{
    StarsRect rect;
    StarsRect inner;
    StarsRect reference;
    StarsI16 left;
    StarsI16 right;
    StarsI16 y;
    StarsI16 line;
    StarsI16 length;
    StarsI16 warp;
    StarsI16 warp_sign;
    StarsI16 gauge_value;
    StarsI16 gauge_best;
    StarsI16 button_kind;
    StarsI16 button_enabled;
    StarsI16 width;
    StarsI16 index;
    StarsI32 shield;
    StarsI32 mass_warp;
    StarsU8 buffer[96];
    const StarsU8 *title;

    if (!input || !input->tile || !input->data || !input->ops ||
        !input->ops->draw_tile)
        return;
    if ((input->tile->flags & STARBASE_TILE_RESET) != 0U) {
        if (input->reference_rects)
            starbase_empty_rect(&input->reference_rects[
                STARBASE_REFERENCE_ACTION]);
        starbase_set_empty_references(input);
        input->tile->flags = (StarsU16)(input->tile->flags &
            (StarsU16)~STARBASE_TILE_RESET);
    }
    title = input->data->has_starbase && input->data->hull_class[0] != 0 ?
        input->data->hull_class :
        (input->ops->compressed_string ?
            input->ops->compressed_string(input->ops->context, 0x343) :
            (const StarsU8 *)0);
    if (!input->ops->draw_tile(input->ops->context, input->hdc, input->tile,
            title, &rect))
        return;
    left = (StarsI16)(rect.left + 4);
    right = (StarsI16)(rect.right - 4);
    line = input->line_height > 0 ? input->line_height : 1;
    inner.left = (StarsU16)(rect.left + 2);
    inner.top = rect.top;
    inner.right = (StarsU16)(rect.right - 2);
    inner.bottom = (StarsU16)(rect.bottom - 2);
    if (input->ops->pat_blt)
        input->ops->pat_blt(input->ops->context, input->hdc, inner.left,
            inner.top, (StarsI16)(inner.right - inner.left),
            (StarsI16)(inner.bottom - inner.top), 0xf00021UL);
    if (!input->data->has_starbase) {
        starbase_set_empty_references(input);
        return;
    }
    reference.left = left;
    reference.top = rect.top;
    reference.right = right;
    reference.bottom = (StarsU16)(rect.top + line * 4);
    if (input->reference_rects)
        input->reference_rects[STARBASE_REFERENCE_DETAILS] = reference;
    if (input->ops->set_reference)
        input->ops->set_reference(input->ops->context,
            STARBASE_REFERENCE_DETAILS, &reference);
    if (input->ops->select_font)
        input->ops->select_font(input->ops->context, input->hdc,
            input->bold_font);
    length = starbase_get_text(input, 0x344, buffer, (StarsU16)sizeof buffer);
    starbase_text(input, left, rect.top, buffer, length);
    if (input->ops->select_font)
        input->ops->select_font(input->ops->context, input->hdc,
            input->normal_font);
    width = input->ops->text_extent ? input->ops->text_extent(
        input->ops->context, input->hdc, buffer, length) : 0;
    starbase_draw_value(input, buffer, (StarsU16)sizeof buffer,
        input->data->cargo_capacity, 0, right, rect.top, width);
    y = (StarsI16)(rect.top + line);
    if (input->ops->select_font)
        input->ops->select_font(input->ops->context, input->hdc,
            input->bold_font);
    length = starbase_get_text(input, 0x345, buffer, (StarsU16)sizeof buffer);
    starbase_text(input, left, y, buffer, length);
    starbase_draw_value(input, buffer, (StarsU16)sizeof buffer,
        input->data->defense, 1, right, y, width);
    y = (StarsI16)(y + line);
    if (input->ops->select_font)
        input->ops->select_font(input->ops->context, input->hdc,
            input->normal_font);
    length = starbase_get_text(input, 0x346, buffer, (StarsU16)sizeof buffer);
    starbase_text(input, left, y, buffer, length);
    shield = input->ops->shield_value ? input->ops->shield_value(
        input->ops->context, input->data->hull_id, input->data->player_id) : 0;
    starbase_draw_value(input, buffer, (StarsU16)sizeof buffer, shield, 1,
        right, y, width);
    y = (StarsI16)(y + line);
    length = starbase_get_text(input, 0x347, buffer, (StarsU16)sizeof buffer);
    starbase_text(input, left, y, buffer, length);
    mass_warp = (StarsI32)(input->data->mass_warp_packed >> 4);
    if (mass_warp != 0 && mass_warp < 5L)
        mass_warp = 5L;
    starbase_draw_value(input, buffer, (StarsU16)sizeof buffer,
        mass_warp / 5L, 1, right, y, width);
    if (mass_warp != 0 && input->ops->set_text_color)
        input->ops->set_text_color(input->ops->context, input->hdc,
            input->highlight_color);
    y = (StarsI16)(y + line);
    if (input->ops->set_text_color && mass_warp != 0)
        input->ops->set_text_color(input->ops->context, input->hdc, 0);
    if (input->ops->pat_blt)
        input->ops->pat_blt(input->ops->context, input->hdc, rect.left, y,
            (StarsI16)(rect.right - rect.left), 1, 0xf00021UL);
    if (input->ops->select_font)
        input->ops->select_font(input->ops->context, input->hdc,
            input->bold_font);
    reference.left = left;
    reference.top = (StarsU16)(y + 1);
    reference.right = right;
    reference.bottom = (StarsU16)(y + line + 1);
    if (input->reference_rects)
        input->reference_rects[STARBASE_REFERENCE_WARP] = reference;
    if (input->ops->set_reference)
        input->ops->set_reference(input->ops->context,
            STARBASE_REFERENCE_WARP, &reference);
    length = starbase_get_text(input, 0x349, buffer, (StarsU16)sizeof buffer);
    starbase_text(input, left, (StarsI16)(y + 1), buffer, length);
    y = (StarsI16)(y + line + 1);
    if (input->ops->select_font)
        input->ops->select_font(input->ops->context, input->hdc,
            input->bold_font);
    length = starbase_get_text(input, 0x475, buffer,
        (StarsU16)sizeof buffer);
    starbase_text(input, left, y, buffer, length);
    y = (StarsI16)(y + line);
    length = input->ops->get_planet_name && input->data->destination_id >= 0 ?
        input->ops->get_planet_name(input->ops->context,
            input->data->destination_id, buffer, (StarsU16)sizeof buffer) : 0;
    if (length <= 0) {
        length = starbase_get_text(input, 0x560, buffer,
            (StarsU16)sizeof buffer);
    }
    starbase_right(input, right, (StarsI16)(y - line), buffer, length, width);
    warp = input->ops->warp_value ? input->ops->warp_value(
        input->ops->context, input->data->planet_id, &warp_sign) : 0;
    button_kind = warp == 0 ? 0x0c : 8;
    button_enabled = input->button_enabled;
    reference.left = left;
    reference.top = y;
    reference.right = (StarsU16)(left + (right - left) / 3);
    reference.bottom = (StarsU16)(y + line + 6);
    if (input->reference_rects)
        input->reference_rects[STARBASE_REFERENCE_ACTION] = reference;
    if (input->ops->set_reference)
        input->ops->set_reference(input->ops->context,
            STARBASE_REFERENCE_ACTION, &reference);
    if (input->ops->draw_button)
        input->ops->draw_button(input->ops->context, input->hdc, &reference,
            button_kind, button_enabled);
    if (warp < 1) {
        if (input->reference_rects) {
            starbase_empty_rect(&input->reference_rects[
                STARBASE_REFERENCE_GAUGE]);
            starbase_empty_rect(&input->reference_rects[STARBASE_REFERENCE_WARP]);
        }
        return;
    }
    reference.left = (StarsU16)(reference.right + 4);
    reference.top = (StarsU16)(y + 3);
    reference.right = right;
    reference.bottom = (StarsU16)(y + line + 3);
    if (input->reference_rects)
        input->reference_rects[STARBASE_REFERENCE_GAUGE] = reference;
    if (input->ops->set_reference)
        input->ops->set_reference(input->ops->context,
            STARBASE_REFERENCE_GAUGE, &reference);
    gauge_value = warp_sign ? (StarsI16)-warp : warp;
    gauge_best = (StarsI16)(((input->data->mass_warp_packed >> 10) &
        0x000fU) + 4U);
    if (input->ops->draw_gauge)
        input->ops->draw_gauge(input->ops->context, input->hdc, &reference,
            gauge_value, gauge_best);
    (void)index;
}
