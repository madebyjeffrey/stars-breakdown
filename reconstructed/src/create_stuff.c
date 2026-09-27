/* Semantic reconstruction of the startup resource allocator FCreateStuff. */
#include <string.h>
#include "core.h"

static void create_stuff_alert(const StarsCreateStuffOps *o, void *cx)
{ if (o->alert_ids) o->alert_ids(cx, 0x0017U, 0x0010); }

static StarsU16 create_stuff_brush(const StarsCreateStuffOps *o, void *cx,
    StarsU32 color)
{ return o->get_brush ? o->get_brush(cx, color) : 0U; }

/* NE 1:0014-0AB2. Resource identifiers are the original DGROUP offsets for
 * string/resource names; the platform adapter resolves them as Win16 names. */
StarsI16 stars_create_stuff(StarsCreateStuffState *s, StarsU16 instance,
    const StarsCreateStuffOps *o, void *cx)
{
    static const StarsU32 brush_colors[34] = {
        0x0000ff00UL, 0x0000ffffUL, 0x00ff0000UL, 0x000000ffUL,
        0x0000ffffUL, 0x000000ffUL, 0x007f0000UL, 0x00007f00UL,
        0x0000007fUL, 0x007f007fUL, 0x009fffffUL,
        0x00ff0000UL, 0x00007f00UL, 0x0000ffffUL, 0x00ffffffUL,
        0x000000ffUL, 0x007f0000UL, 0x00ff0000UL, 0x0000007fUL,
        0x000000ffUL, 0x00007f00UL, 0x0000ff00UL,
        0x00ff0000UL, 0x007f0000UL, 0x0000ff00UL, 0x00007f00UL,
        0x0000ffffUL, 0x00007f7fUL, 0x000000ffUL, 0x0000007fUL,
        0x0000ffffUL, 0x00007f7fUL, 0x00c0c0c0UL, 0x00808080UL};
    static const StarsU32 pen_colors[11] = {
        0x0000ff00UL, 0x00007f00UL, 0x00ff0000UL, 0x000000ffUL,
        0x00007f7f7fUL, 0x0000007fUL, 0x007f0000UL, 0x0000ffffUL,
        0x00007f7fUL, 0x007f007fUL, 0x00000000UL};
    static const StarsU16 bitmap_ids[8] = {
        0x0632U, 0x0058U, 0x063dU, 0x00f9U, 0x0077U, 0x0086U,
        0x00c7U, 0x0000U};
    static const StarsU16 cursor_ids[11] = {
        0x0603U, 0x060eU, 0x0619U, 0x0625U, 0x007aU, 0x0079U,
        0x0102U, 0x0104U, 0x0107U, 0x0108U, 0x0109U};
    static const StarsU16 proc_ids[8] = {1U,2U,3U,4U,5U,6U,7U,8U};
    StarsI16 width;
    StarsI16 height;
    StarsU16 i;
    StarsU16 bitmap;
    StarsU16 initial_display_flags;
    StarsI16 failed;

    if (!s || !o) return 0;
    initial_display_flags = s->display_flags;
    memset(s, 0, sizeof(*s));
    s->display_flags = initial_display_flags;
    width = o->get_system_metric ? o->get_system_metric(cx, 0U) : 0;
    height = o->get_system_metric ? o->get_system_metric(cx, 1U) : 0;
    if (width < 800 || height < 600) s->display_flags &= 0x3fffU;
    else if (width < 0x0400 || height < 0x0300)
        s->display_flags = (StarsU16)((s->display_flags & 0x3fffU) | 0x4000U);
    else if (width < 0x0457 || height < 0x0378)
        s->display_flags = (StarsU16)((s->display_flags & 0x3fffU) | 0x8000U);
    else s->display_flags = (StarsU16)((s->display_flags & 0x3fffU) | 0xc000U);
    s->display_flags = (StarsU16)(s->display_flags & 0xfb7fU);
    if (o->copy_player_defaults) o->copy_player_defaults(cx,
        s->player_defaults, (StarsU16)sizeof(s->player_defaults));
    if (o->create_region) {
        s->regions[0] = o->create_region(cx, -10, -10, 2000, 2000);
        s->regions[1] = o->create_region(cx, 0, 0, 10, 10);
    }
    for (i = 0; i < 34U; ++i)
        s->brushes[i] = create_stuff_brush(o, cx, brush_colors[i]);
    for (i = 0; i < 11U; ++i)
        s->pens[i] = o->create_pen ? o->create_pen(cx,
            i == 4U ? 2 : 0, 1, pen_colors[i]) : 0U;
    if (o->load_bitmap && o->create_pattern_brush) {
        bitmap = o->load_bitmap(cx, instance, 0x05e6U);
        s->pattern_brushes[0] = o->create_pattern_brush(cx, bitmap);
        if (o->delete_object) o->delete_object(cx, bitmap);
        for (i = 0; i < 3U; ++i) {
            bitmap = o->load_bitmap(cx, instance, (StarsU16)(0x01ccU + i));
            s->pattern_brushes[i + 1U] = o->create_pattern_brush(cx, bitmap);
            if (o->delete_object) o->delete_object(cx, bitmap);
        }
        bitmap = o->load_bitmap(cx, instance, 0x05f2U);
        s->pattern_brushes[4] = o->create_pattern_brush(cx, bitmap);
        if (o->delete_object) o->delete_object(cx, bitmap);
        bitmap = o->load_bitmap(cx, instance, 0x05fbU);
        s->pattern_brushes[5] = o->create_pattern_brush(cx, bitmap);
        if (o->delete_object) o->delete_object(cx, bitmap);
    }
    if (o->load_cursor)
        for (i = 0; i < 11U; ++i)
            s->cursors[i] = o->load_cursor(cx, instance, cursor_ids[i]);
    if (o->load_bitmap)
        for (i = 0; i < 4U; ++i)
            s->bitmaps[i] = o->load_bitmap(cx, instance, bitmap_ids[i]);
    if (o->load_dib) {
        s->dib_planets = o->load_dib(cx, 0x0070U);
        s->dib_things = o->load_dib(cx, 0x0057U);
        s->dib_toolbar = o->load_dib(cx, 0x00b2U);
        for (i = 0; i < 5U; ++i) {
            s->dib_ships[i] = o->load_dib(cx, (StarsU16)(0x0228U + i));
            s->dib_ships_t[i] = o->load_dib(cx, (StarsU16)(0x022dU + i));
        }
        for (i = 0; i < 7U; ++i)
            s->dib_inventory[i] = o->load_dib(cx, (StarsU16)(500U + i));
        s->dib_races = o->load_dib(cx, 0x0085U);
        s->dib_races_t = o->load_dib(cx, 0x0050U);
        s->dib_races_x = o->load_dib(cx, 0x004fU);
        s->dib_plaque = o->load_dib(cx, 0x0437U);
    }
    if (o->palette_from_dib) s->palette_ships = o->palette_from_dib(cx,
        s->dib_ships[3]);
    if (o->load_bitmap)
        for (i = 4; i < 7U; ++i)
            s->bitmaps[i] = o->load_bitmap(cx, instance, bitmap_ids[i]);
    if (o->load_icon) {
        s->icons[0] = o->load_icon(cx, instance, 0x064eU);
        s->icons[1] = o->load_icon(cx, instance, 0x0657U);
        s->icons[2] = o->load_icon(cx, instance, 0x065fU);
        for (i = 0; i < 7U; ++i)
            s->icons[i + 3U] = o->load_icon(cx, instance,
                (StarsU16)(0x0667U + i * 9U));
    }
    if (o->allocate_far) {
        s->log_buffer = o->allocate_far(cx, 32000U, 4U);
        s->message_buffer = o->allocate_far(cx, 0xffc8U, 2U);
        s->misc_buffer = o->allocate_far(cx, 0x0800U, 9U);
        s->planet_buffer = o->allocate_far(cx, 0x0800U, 9U);
        s->fleet_buffer = o->allocate_far(cx, 0x0800U, 9U);
    }
    if (o->make_proc_instance)
        for (i = 0; i < 8U; ++i)
            s->proc_instances[i] = o->make_proc_instance(cx, proc_ids[i], instance);
    if (o->get_disk_serial) o->get_disk_serial(cx);
    failed = (s->dib_planets == 0U || s->dib_things == 0U ||
        s->dib_toolbar == 0U);
    for (i = 0; i < 5U; ++i)
        if (s->dib_ships[i] == 0U || s->dib_ships_t[i] == 0U) failed = 1;
    for (i = 0; i < 7U; ++i) if (s->dib_inventory[i] == 0U) failed = 1;
    if (s->bitmaps[0] == 0U || s->bitmaps[1] == 0U || s->bitmaps[2] == 0U ||
        s->bitmaps[4] == 0U || s->dib_races == 0U || s->dib_races_t == 0U ||
        s->dib_races_x == 0U || s->bitmaps[6] == 0U || s->bitmaps[5] == 0U ||
        s->icons[1] == 0U || s->icons[0] == 0U || s->icons[2] == 0U) failed = 1;
    if (failed) { create_stuff_alert(o, cx); return 0; }
    return 1;
}
