/* Startup resource ownership and shared brush cache. Win16 USER/GDI/KERNEL
 * services use the same explicit adapter as the recovered race component. */
#include "startup.h"
#include "runtime_internal.h"

/* NE 9:4448-4530, HbrGet. Last unused cache slot wins. A full cache returns
 * an uncached brush; reference counts are bytes and wrap from 255 to zero. */
StarsU16 stars_brush_get(StarsRuntime *r, StarsU32 color)
{
    StarsI16 i, empty = -1, count = stars_signed_word(stars_word(r->data, 0x13acU));
    StarsU16 brush;
    StarsU8 *d = r->data;
    for (i = 0; i < count; ++i) {
        if (!d[0x2518U + i]) empty = i;
        else if (stars_dword(d, (StarsU16)(0x2612U + i * 4U)) == color) {
            ++d[0x2518U + i];
            return stars_word(d, (StarsU16)(0x52c4U + i * 2U));
        }
    }
    brush = (StarsU16)stars_window(r, "CreateSolidBrush", 0, 0, 0, color, 0, 0);
    if (!brush) return 0;
    if (empty == -1) {
        if (count >= 32) return brush;
        empty = count;
        stars_put_word(d, 0x13acU, (StarsU16)(count + 1));
    }
    d[0x2518U + empty] = 1;
    stars_put_word(d, (StarsU16)(0x52c4U + empty * 2U), brush);
    stars_put_dword(d, (StarsU16)(0x2612U + empty * 4U), color);
    return brush;
}

/* NE 9:4532-459f, FreeHbr. Zero/untracked handles still reach DeleteObject. */
void stars_brush_free(StarsRuntime *r, StarsU16 brush)
{
    StarsI16 i, count = stars_signed_word(stars_word(r->data, 0x13acU));
    for (i = 0; i < count; ++i) {
        if (r->data[0x2518U + i] && stars_word(r->data, (StarsU16)(0x52c4U + i * 2U)) == brush) {
            --r->data[0x2518U + i];
            if (r->data[0x2518U + i]) return;
            break;
        }
    }
    stars_window(r, "DeleteObject", brush, 0, 0, 0, 0, 0);
}

/* FCreateStuff slice, NE 1:023d-02af (exclusive): rghbrPlanetAttr[3][2].
 * COLORREF uses low-byte red. Each environment has a dark/bright pair.
 * This is startup-only; FreeStuff releases all six cached references. */
void stars_create_planet_brushes(StarsRuntime *r)
{
    static const StarsU32 colors[6] = {
        0x7f0000UL, 0xff0000UL, 0x00007fUL, 0x0000ffUL,
        0x007f00UL, 0x00ff00UL};
    StarsU16 i;
    for (i = 0; i < 6; ++i)
        stars_put_word(r->data, (StarsU16)(0x5680U + i * 2U),
            stars_brush_get(r, colors[i]));
}

/* NE 4:08d2-0bad, FGetSystemColors. Returns success even if brush creation
 * fails. The two DIB palette edits store COLORREF's RGB bytes in BGR order. */
StarsI16 stars_system_colors(StarsRuntime *r)
{
    static const StarsU16 release[] = {0x10,0x14,0x16,0x12,0x1a,0x18,0x1c,0x1e};
    static const StarsU16 colors[][3] = {
        {15,0x22ba,0x10}, {20,0x3eec,0x14}, {16,0x23fe,0x16}, {18,0x25f6,0x12},
        {6,0,0x1c}, {1,0,0x1e}, {5,0x2574,0x18}, {8,0x48a6,0x1a}};
    static const StarsU16 metrics[][2] = {{4,0x505a}, {32,0x23ec}, {33,0x23ee}};
    static const StarsU16 palettes[][2] = {{0xc82,0x40c}, {0x598c,0x41c}};
    StarsU8 *d = r->data, *pixel;
    StarsU32 color, pointer;
    StarsU16 i, h, dc, bits, planes, offset;
    for (i = 0; i < 8; ++i)
        if (stars_word(d, release[i])) stars_brush_free(r, stars_word(d, release[i]));
    for (i = 0; i < 8; ++i) {
        color = stars_window(r, "GetSysColor", 0, colors[i][0], 0, 0, 0, 0);
        if (colors[i][1]) stars_put_dword(d, colors[i][1], color);
        stars_put_word(d, colors[i][2], stars_brush_get(r, color));
    }
    for (i = 0; i < 3; ++i)
        stars_put_word(d, metrics[i][1], (StarsU16)stars_window(r, "GetSystemMetrics", 0, metrics[i][0], 0, 0, 0, 0));
    for (i = 0; i < 2; ++i) {
        h = stars_word(d, palettes[i][0]);
        if (!h) continue;
        pointer = stars_window(r, "GlobalLock", h, 0, 0, 0, 0, 0);
        offset = (StarsU16)((StarsU16)pointer + palettes[i][1]);
        pixel = (StarsU8 *)r->platform->memory(r->context, (StarsU16)(pointer >> 16), offset);
        color = stars_dword(d, 0x22baU);
        pixel[2] = (StarsU8)color;
        pixel[1] = (StarsU8)(color >> 8);
        pixel[0] = (StarsU8)(color >> 16);
        stars_window(r, "GlobalUnlock", h, 0, 0, 0, 0, 0);
    }
    dc = (StarsU16)stars_window(r, "GetDC", 0, 0, 0, 0, 0, 0);
    bits = (StarsU16)stars_window(r, "GetDeviceCaps", dc, 12, 0, 0, 0, 0);
    planes = (StarsU16)stars_window(r, "GetDeviceCaps", dc, 14, 0, 0, 0, 0);
    stars_put_word(d, 0x5312U, (StarsU16)((StarsU32)bits * planes));
    stars_window(r, "ReleaseDC", 0, dc, 0, 0, 0, 0);
    return 1;
}

static void font_height(StarsRuntime *r, StarsU16 lf, StarsU16 dc, StarsU16 points)
{
    StarsU16 dpi = (StarsU16)stars_window(r, "GetDeviceCaps", dc, 90, 0, 0, 0, 0);
    StarsU16 height = (StarsU16)stars_window(r, "MulDiv", 0, points, dpi, 72, 0, 0);
    stars_put_word(r->data, lf, (StarsU16)(0U - height));
}
static void font_create(StarsRuntime *r, StarsU16 lf, StarsU16 name, StarsU16 out)
{
    strcpy((char *)r->data + lf + 18U, (char *)r->data + name);
    stars_put_word(r->data, out, (StarsU16)stars_window(r, "CreateFontIndirect", 0, 0, 0, 0, r->data + lf, 0));
}

/* NE 1:0ab2-0d6f, FCreateFonts. All nine font requests, selected-object
 * restoration, four metric reads and LocalFree are retained. Failures are
 * not checked by the original. LOGFONT and TEXTMETRIC retain Win16 layouts. */
StarsI16 stars_create_fonts(StarsRuntime *r, StarsU16 dc)
{
    static const StarsU16 measure[][2] = {{0x26b4,0x23fa}, {0x26be,0x23f6}, {0x26c0,0x23f8}, {0x4840,0x530a}};
    StarsU8 *d = r->data, metrics[31];
    StarsU16 lf, i, old;
    lf = (StarsU16)stars_window(r, "LocalAlloc", 0, 0x40U, 50, 0, 0, 0);
    for (i = 0; i < 4; ++i)
        if (!d[0x6a6U + i * 32U]) stars_copy_game_string(d, r->strings, (StarsI16)(0x537U + i), (StarsU16)(0x6a6U + i * 32U));
    font_height(r, lf, dc, 10);
    for (i = 0; i < 2; ++i) font_create(r, lf, (StarsU16)(0x6a6U + i * 32U), (StarsU16)(0x4840U + i * 2U));
    strcpy((char *)d + lf + 18U, (char *)d + 0x6a6U);
    font_height(r, lf, dc, 6);
    stars_put_word(d, 0x26c0U, (StarsU16)stars_window(r, "CreateFontIndirect", 0, 0, 0, 0, d + lf, 0));
    font_height(r, lf, dc, 7);
    stars_put_word(d, 0x26beU, (StarsU16)stars_window(r, "CreateFontIndirect", 0, 0, 0, 0, d + lf, 0));
    font_height(r, lf, dc, 8);
    for (i = 0; i < 4; ++i) font_create(r, lf, (StarsU16)(0x6a6U + i * 32U), (StarsU16)(0x26b4U + i * 2U));
    strcpy((char *)d + lf + 18U, (char *)d + 0x6c6U);
    stars_put_word(d, (StarsU16)(lf + 4U), 3150);
    stars_put_word(d, 0x26bcU, (StarsU16)stars_window(r, "CreateFontIndirect", 0, 0, 0, 0, d + lf, 0));
    old = (StarsU16)stars_window(r, "SelectObject", dc, stars_word(d, 0x26b4U), 0, 0, 0, 0);
    for (i = 0; i < 4; ++i) {
        if (i) stars_window(r, "SelectObject", dc, stars_word(d, measure[i][0]), 0, 0, 0, 0);
        stars_window(r, "GetTextMetrics", dc, 0, 0, 0, metrics, 0);
        stars_put_word(d, measure[i][1], (StarsU16)(stars_word(metrics, 0) + stars_word(metrics, 8)));
        if (!i) stars_put_word(d, 0x2610U, (StarsU16)stars_window(r, "GetTextExtent", dc, 10, 0, 0, d + 0x726U, 0));
    }
    stars_window(r, "SelectObject", dc, old, 0, 0, 0, 0);
    stars_window(r, "LocalFree", lf, 0, 0, 0, 0, 0);
    return 1;
}

/* NE 1:0eaa-101e, InitTiles. Convert line counts into pixel heights for the
 * two fixed tile arrays, resetting positions at each new low-three-bit group. */
void stars_init_tiles(StarsU8 *d)
{
    StarsU16 pass, i, base, count, group, y, p, flags, height;
    for (pass = 0; pass < 2; ++pass) {
        base = pass ? 0x90eU : 0x7fcU; count = pass ? 7U : 6U;
        group = 0; y = 4;
        for (i = 0; i < count; ++i) {
            p = (StarsU16)(base + i * 16U);
            flags = stars_word(d, (StarsU16)(p + 10U));
            if ((flags & 7U) != group) { y = 4; group = (StarsU16)(flags & 7U); }
            height = (StarsU16)(stars_word(d, (StarsU16)(p + 2U)) +
                (StarsU32)stars_word(d, p) * stars_word(d, 0x23faU));
            stars_put_word(d, (StarsU16)(p + 2U), height);
            stars_put_word(d, p, y);
            stars_put_word(d, (StarsU16)(p + 10U), (StarsU16)(flags & 0xe7ffU));
            y = (StarsU16)(y + ((flags & 0x80U) ? height + 4U : stars_word(d, 0x23faU) + 7U));
        }
    }
}

static void release_word(StarsRuntime *r, const char *op, StarsU16 offset)
{ stars_window(r, op, stars_word(r->data, offset), 0, 0, 0, 0, 0); }
static void release_bitmap(StarsRuntime *r, StarsU16 offset)
{
    release_word(r, "GlobalUnlock", offset);
    release_word(r, "FreeResource", offset);
}

/* NE 4:0bae-1210, FreeStuff. Destruction order is observable and intentional.
 * Only lpLog/lpMsg are cleared. Other handles and heap heads remain stale;
 * this is terminal cleanup, not an idempotent reset or a generic destructor.
 */
void stars_free_stuff(StarsMemory *m)
{
    /* Button/window colours, RGB, purple, tooltip; then mineral brushes. */
    static const StarsU16 conditional_brushes[] = {
        0x10,0x14,0x16,0x12,0x1a,0x18,0x1c,0x1e,0x24,0x26,0x28,0x2692,0x275a};
    /* Fake combo, CE, edit, list, host timer, browser callbacks. */
    static const StarsU16 instances[] = {0x2544,0x2748,0x2538,0x2402,0x51fe,0x51d0};
    static const StarsU16 cursors[] = {
        0x4a66,0x4b7a,0x5304,0x57a2,0x258a,0x3ee4,0x23f0,0x26c6,0x51ce,0x48ae,0x525e};
    /* Scanner, numbers, scan ship, unknown planet. */
    static const StarsU16 bitmaps[] = {0x4a5a,0x597a,0x4b7e,0x3efa};
    /* Planets, things, toolbar, races, transparent races, extra races. */
    static const StarsU16 dibs[] = {0x4952,0x242e,0x598c,0x5476,0x2606,0x2604};
    /* Ship, starbase, blue, enemy, selection, radar. */
    static const StarsU16 brushes[] = {0x5318,0x590c,0x599a,0x5996,0x2430,0x255a};
    static const StarsU16 greys_yellows[] = {0x20,0x22,0x22c6,0x519c};
    /* Ship, dark green/purple, starbase, enemy, mass path, radar. */
    static const StarsU16 pens[] = {0x65a2,0x5358,0x26c2,0x5980,0x568c,0x599e,0x5936};
    StarsRuntime *r = m->runtime;
    StarsU8 *d = r->data;
    StarsU16 i, cursor;
    for (i = 0; i < 13; ++i)
        if (stars_word(d, conditional_brushes[i])) stars_brush_free(r, stars_word(d, conditional_brushes[i]));
    for (i = 0; i < 5; ++i)
        if (stars_word(d, (StarsU16)(0x257cU + i * 2U))) stars_brush_free(r, stars_word(d, (StarsU16)(0x257cU + i * 2U)));
    for (i = 0; i < 6; ++i) stars_brush_free(r, stars_word(d, (StarsU16)(0x5680U + i * 2U)));
    for (i = 0; i < 8; ++i) stars_brush_free(r, stars_word(d, (StarsU16)(0x5692U + i * 2U)));
    for (i = 0; i < 6; ++i)
        stars_window(r, "FreeProcInstance", 0, 0, 0, stars_dword(d, instances[i]), 0, 0);
    if (stars_dword(d, 0x2aU)) stars_window(r, "FreeProcInstance", 0, 0, 0, stars_dword(d, 0x2aU), 0, 0);
    release_word(r, "DeleteObject", 0x525aU); /* hrgnHuge */
    release_word(r, "DeleteObject", 0x22beU); /* hrgnScratch */
    cursor = (StarsU16)stars_window(r, "LoadCursor", 0, 0, 0, 0x7f00U, 0, 0);
    stars_window(r, "SetCursor", cursor, 0, 0, 0, 0, 0);
    for (i = 0; i < 11; ++i) release_word(r, "DestroyCursor", cursors[i]);
    for (i = 0; i < 4; ++i) release_word(r, "DeleteObject", bitmaps[i]);
    for (i = 0; i < 3; ++i) release_word(r, "DestroyIcon", (StarsU16)(0x356U + i * 2U));
    for (i = 0; i < 7; ++i) release_word(r, "DestroyIcon", (StarsU16)(0x24e6U + i * 2U));
    for (i = 0; i < 6; ++i) release_bitmap(r, dibs[i]);
    release_word(r, "DeleteObject", 0x24e2U); /* build background */
    release_word(r, "DeleteObject", 0x260aU); /* message bitmap */
    release_word(r, "DeleteObject", 0x5992U); /* monochrome bitmap */
    release_word(r, "FreeResource", 0xc82U); /* plaque: no unlock here */
    for (i = 0; i < 5; ++i) {
        release_bitmap(r, (StarsU16)(0x4b6eU + i * 2U));
        release_bitmap(r, (StarsU16)(0x5160U + i * 2U));
    }
    for (i = 0; i < 7; ++i) release_bitmap(r, (StarsU16)(0x258eU + i * 2U));
    stars_free(m, stars_dword(d, 0x9a0U), 4); stars_put_dword(d, 0x9a0U, 0);
    stars_free(m, stars_dword(d, 0xaf8U), 2); stars_put_dword(d, 0xaf8U, 0);
    release_word(r, "DeleteObject", 0x5314U); /* main palette */
    if (stars_word(d, 0x42eU)) release_word(r, "DeleteObject", 0x42eU);
    for (i = 0; i < 6; ++i) stars_brush_free(r, stars_word(d, brushes[i]));
    if (stars_word(d, 0x568eU)) stars_brush_free(r, stars_word(d, 0x568eU));
    for (i = 0; i < 4; ++i) stars_brush_free(r, stars_word(d, greys_yellows[i]));
    release_word(r, "DeleteObject", 0x4b78U); /* 50% screen brush */
    for (i = 0; i < 3; ++i) release_word(r, "DeleteObject", (StarsU16)(0x591eU + i * 2U));
    release_word(r, "DeleteObject", 0x520aU); /* cargo */
    release_word(r, "DeleteObject", 0x5262U); /* dock */
    for (i = 0; i < 7; ++i) release_word(r, "DeleteObject", pens[i]);
    if (stars_word(d, 0x567eU)) release_word(r, "DeleteObject", 0x567eU);
    release_word(r, "DeleteObject", 0x5690U); /* dark blue pen */
    release_word(r, "DeleteObject", 0x26c4U); /* yellow pen */
    release_word(r, "DeleteObject", 0x23c8U); /* dark yellow pen */
    for (i = 0; i < 2; ++i) release_word(r, "DeleteObject", (StarsU16)(0x4840U + i * 2U));
    for (i = 0; i < 5; ++i) release_word(r, "DeleteObject", (StarsU16)(0x26b4U + i * 2U));
    release_word(r, "DeleteObject", 0x26c0U);
    release_word(r, "DeleteObject", 0x26beU);
    for (i = 0; i < 12; ++i) stars_heap_free(m, stars_dword(d, (StarsU16)(0xd34U + i * 4U)));
}
