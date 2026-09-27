#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct CreateContext {
    StarsI16 width;
    StarsI16 height;
    StarsU16 fail_dib;
    int defaults;
    int regions;
    int brushes;
    int pens;
    int bitmaps;
    int cursors;
    int dibs;
    int icons;
    int allocs;
    int procs;
    int disk;
    int alerts;
    StarsI16 first_left;
    StarsI16 first_top;
} CreateContext;

static StarsI16 metric(void *p, StarsU16 id)
{ CreateContext *c = (CreateContext *)p; return id == 0U ? c->width : c->height; }
static void defaults(void *p, StarsU8 *data, StarsU16 count)
{ CreateContext *c = (CreateContext *)p; ++c->defaults; if (count) data[0] = 0x5a; }
static StarsU16 region(void *p, StarsI16 left, StarsI16 top, StarsI16 right, StarsI16 bottom)
{ CreateContext *c = (CreateContext *)p; if (++c->regions == 1) { c->first_left = left; c->first_top = top; }
    (void)right; (void)bottom; return (StarsU16)(0x100U + c->regions); }
static StarsU16 brush(void *p, StarsU32 color)
{ CreateContext *c = (CreateContext *)p; (void)color; ++c->brushes; return (StarsU16)(0x200U + c->brushes); }
static StarsU16 pen(void *p, StarsI16 style, StarsI16 width, StarsU32 color)
{ CreateContext *c = (CreateContext *)p; (void)style; (void)width; (void)color; ++c->pens; return (StarsU16)(0x300U + c->pens); }
static StarsU16 bitmap(void *p, StarsU16 instance, StarsU16 id)
{ CreateContext *c = (CreateContext *)p; (void)instance; (void)id; ++c->bitmaps; return (StarsU16)(0x400U + c->bitmaps); }
static StarsU16 pattern(void *p, StarsU16 bitmap_handle)
{ (void)p; return (StarsU16)(bitmap_handle + 1U); }
static void delete_object(void *p, StarsU16 object)
{ (void)p; (void)object; }
static StarsU16 cursor(void *p, StarsU16 instance, StarsU16 id)
{ CreateContext *c = (CreateContext *)p; (void)instance; (void)id; ++c->cursors; return (StarsU16)(0x500U + c->cursors); }
static StarsU16 dib(void *p, StarsU16 id)
{ CreateContext *c = (CreateContext *)p; ++c->dibs; return id == c->fail_dib ? 0U : (StarsU16)(0x600U + c->dibs); }
static StarsU16 palette(void *p, StarsU16 dib_handle)
{ (void)p; return (StarsU16)(dib_handle + 1U); }
static StarsU16 icon(void *p, StarsU16 instance, StarsU16 id)
{ CreateContext *c = (CreateContext *)p; (void)instance; (void)id; ++c->icons; return (StarsU16)(0x700U + c->icons); }
static StarsU32 allocate(void *p, StarsU16 size, StarsU16 flags)
{ CreateContext *c = (CreateContext *)p; (void)size; (void)flags; ++c->allocs; return 0x10000UL + (StarsU32)c->allocs; }
static StarsU32 make_proc(void *p, StarsU16 id, StarsU16 instance)
{ CreateContext *c = (CreateContext *)p; (void)id; (void)instance; ++c->procs; return 0x20000UL + (StarsU32)c->procs; }
static void disk(void *p)
{ CreateContext *c = (CreateContext *)p; ++c->disk; }
static void alert(void *p, StarsU16 id, StarsI16 flags)
{ CreateContext *c = (CreateContext *)p; (void)id; (void)flags; ++c->alerts; }

static void setup(CreateContext *c, StarsCreateStuffOps *o)
{
    memset(c, 0, sizeof(*c)); memset(o, 0, sizeof(*o)); c->width = 800; c->height = 600;
    o->get_system_metric = metric; o->copy_player_defaults = defaults;
    o->create_region = region; o->get_brush = brush; o->create_pen = pen;
    o->load_bitmap = bitmap; o->create_pattern_brush = pattern;
    o->delete_object = delete_object; o->load_cursor = cursor; o->load_dib = dib;
    o->palette_from_dib = palette; o->load_icon = icon; o->allocate_far = allocate;
    o->make_proc_instance = make_proc; o->get_disk_serial = disk; o->alert_ids = alert;
}

static int success_case(void)
{
    CreateContext c; StarsCreateStuffOps o; StarsCreateStuffState s;
    setup(&c, &o); memset(&s, 0, sizeof(s));
    if (!stars_create_stuff(&s, 0x1234U, &o, &c)) return 1;
    return s.display_flags == 0x4000U && s.player_defaults[0] == 0x5a &&
        s.regions[0] == 0x101U && c.first_left == -10 && c.first_top == -10 &&
        c.defaults == 1 && c.regions == 2 && c.brushes == 34 && c.pens == 11 &&
        c.cursors == 11 && c.dibs == 24 && c.icons == 10 && c.allocs == 5 &&
        c.procs == 8 && c.disk == 1 && c.alerts == 0 ? 0 : 1;
}

static int failure_case(void)
{
    CreateContext c; StarsCreateStuffOps o; StarsCreateStuffState s;
    setup(&c, &o); c.fail_dib = 0x0228U; memset(&s, 0, sizeof(s));
    if (stars_create_stuff(&s, 1U, &o, &c) != 0) return 1;
    return c.alerts == 1 && s.dib_ships[0] == 0U ? 0 : 1;
}

static int resolution_case(void)
{
    CreateContext c; StarsCreateStuffOps o; StarsCreateStuffState s;
    setup(&c, &o); c.width = 1200; c.height = 900; memset(&s, 0, sizeof(s));
    if (!stars_create_stuff(&s, 1U, &o, &c)) return 1;
    return s.display_flags == 0xc000U ? 0 : 1;
}

int main(void)
{
    int failures = success_case() + failure_case() + resolution_case();
    printf("CREATE_STUFF_CHECKS=3 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures;
}
