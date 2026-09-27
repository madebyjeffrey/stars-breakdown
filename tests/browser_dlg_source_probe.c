#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct Trace {
    StarsI16 stickies;
    StarsI16 menus;
    StarsI16 checks;
    StarsI16 rects;
    StarsI16 fills;
    StarsI16 colors;
    StarsI16 positions;
    StarsI16 gets;
    StarsI16 selects;
    StarsI16 releases;
    StarsI16 text_calls;
    StarsI16 extents;
    StarsI16 creates;
    StarsI16 strings;
    StarsI16 resets;
    StarsI16 lookups;
    StarsI16 hides;
    StarsI16 invalidates;
    StarsI16 ends;
    StarsI16 tutors;
    StarsI16 defaults;
    StarsU16 combo_selection;
    StarsI16 last_group;
    StarsI16 last_item;
    StarsU16 first_string;
    StarsU16 last_string;
    StarsU8 first_string_text[32];
    StarsU8 last_string_text[32];
    StarsU32 string_payload_sum;
    StarsI16 packed_mode;
    StarsI16 packed_lookups;
    StarsU8 packed_part[8];
    StarsU8 packed_component[52];
    StarsU32 packed_offset;
} Trace;

/* The original tables are in the executable's code segment.  Their records
 * are laid out as _compart values even though the table strides include the
 * original compiler's surrounding record spacing. */
typedef struct OriginalComponent {
    StarsU16 id;
    StarsU16 tech[3];
    StarsU8 name[32];
    StarsU16 mass;
    StarsU16 resource_cost;
    StarsU16 ore[3];
    StarsU16 bitmap;
} OriginalComponent;

static const OriginalComponent original_group_8[12] = {
    {1U, {0U, 0U, 0U}, "Tritanium", 60U, 10U, {5U, 0U, 0U}, 64U},
    {2U, {0U, 768U, 0U}, "Crobmnium", 56U, 13U, {6U, 0U, 0U}, 65U},
    {3U, {0U, 0U, 1024U}, "Carbonic Armor", 25U, 15U, {0U, 0U, 5U}, 70U},
    {4U, {0U, 1536U, 0U}, "Strobnium", 54U, 18U, {8U, 0U, 0U}, 68U},
    {5U, {0U, 0U, 1792U}, "Organic Armor", 15U, 20U, {0U, 0U, 6U}, 71U},
    {6U, {0U, 2304U, 0U}, "Kelarium", 50U, 25U, {9U, 1U, 0U}, 67U},
    {7U, {4U, 2560U, 0U}, "Fielded Kelarium", 50U, 28U, {10U, 0U, 2U}, 78U},
    {8U, {0U, 2560U, 3U}, "Depleted Neutronium", 50U, 28U, {10U, 0U, 2U}, 79U},
    {9U, {0U, 3072U, 0U}, "Neutronium", 45U, 30U, {11U, 2U, 1U}, 69U},
    {10U, {14U, 3584U, 1550U}, "Mega Poly Shell", 20U, 65U, {18U, 6U, 6U}, 110U},
    {11U, {0U, 4096U, 0U}, "Valanium", 40U, 50U, {15U, 0U, 0U}, 66U},
    {12U, {0U, 6144U, 0U}, "Superlatanium", 30U, 100U, {25U, 0U, 0U}, 77U}
};

static const OriginalComponent original_group_10[24] = {
    {1U, {0U, 0U, 0U}, "Laser", 1U, 5U, {0U, 6U, 0U}, 28U},
    {2U, {768U, 0U, 0U}, "X-Ray Laser", 1U, 6U, {0U, 6U, 0U}, 29U},
    {3U, {1280U, 0U, 0U}, "Mini Gun", 3U, 10U, {0U, 16U, 0U}, 20U},
    {4U, {1536U, 0U, 0U}, "Yakimora Light Phaser", 1U, 7U, {0U, 8U, 0U}, 19U},
    {5U, {1792U, 0U, 0U}, "Blackjack", 10U, 7U, {0U, 16U, 0U}, 14U},
    {6U, {2048U, 0U, 0U}, "Phaser Bazooka", 2U, 11U, {0U, 8U, 0U}, 21U},
    {7U, {2309U, 0U, 0U}, "Pulsed Sapper", 1U, 12U, {0U, 0U, 4U}, 17U},
    {8U, {2560U, 0U, 0U}, "Colloidal Phaser", 2U, 18U, {0U, 14U, 0U}, 192U},
    {9U, {2816U, 0U, 0U}, "Gatling Gun", 3U, 13U, {0U, 20U, 0U}, 26U},
    {10U, {3072U, 0U, 0U}, "Mini Blaster", 1U, 9U, {0U, 10U, 0U}, 24U},
    {11U, {3328U, 0U, 0U}, "Bludgeon", 10U, 9U, {0U, 22U, 0U}, 15U},
    {12U, {3584U, 0U, 0U}, "Mark IV Blaster", 2U, 15U, {0U, 12U, 0U}, 25U},
    {13U, {3848U, 0U, 0U}, "Phased Sapper", 1U, 16U, {0U, 0U, 6U}, 18U},
    {14U, {4096U, 0U, 0U}, "Heavy Blaster", 2U, 25U, {0U, 20U, 0U}, 193U},
    {15U, {4352U, 0U, 0U}, "Gatling Neutrino Cannon", 3U, 17U, {0U, 28U, 0U}, 30U},
    {16U, {4608U, 0U, 0U}, "Myopic Disruptor", 1U, 12U, {0U, 14U, 0U}, 194U},
    {17U, {4864U, 0U, 0U}, "Blunderbuss", 10U, 13U, {0U, 30U, 0U}, 13U},
    {18U, {5120U, 0U, 0U}, "Disruptor", 2U, 20U, {0U, 16U, 0U}, 27U},
    {19U, {5397U, 0U, 3088U}, "Multi Contained Munition", 8U, 40U, {6U, 40U, 6U}, 111U},
    {20U, {5387U, 0U, 0U}, "Syncro Sapper", 1U, 21U, {0U, 0U, 8U}, 16U},
    {21U, {5632U, 0U, 0U}, "Mega Disruptor", 2U, 33U, {0U, 30U, 0U}, 195U},
    {22U, {5888U, 0U, 0U}, "Big Mutha Cannon", 3U, 23U, {0U, 36U, 0U}, 31U},
    {23U, {6144U, 0U, 0U}, "Streaming Pulverizer", 1U, 16U, {0U, 20U, 0U}, 22U},
    {24U, {6656U, 0U, 0U}, "Anti-Matter Pulverizer", 2U, 27U, {0U, 22U, 0U}, 23U}
};

static void copy_original_component(Trace *trace, StarsU16 group,
    StarsU16 item)
{
    const OriginalComponent *component;

    component = (const OriginalComponent *)0;
    if (group == 8U && item < 12U)
        component = &original_group_8[item];
    else if (group == 0x10U && item < 24U)
        component = &original_group_10[item];
    if (component)
        memcpy(trace->packed_component, component, sizeof *component);
}

/* CS:1EB5 in the recovered BrowserDlg code.  Entry 0 is the sentinel used
 * by the original scan; normal component-group navigation cycles entries 1
 * through 16. */
static const StarsU16 browser_groups[17] = {
    0x19ffU, 0x0008U, 0x0010U, 0x0040U, 0x0800U, 0x0001U,
    0x1000U, 0x0100U, 0x0080U, 0x0200U, 0x8000U, 0x0002U,
    0x0004U, 0x4000U, 0x0400U, 0x2000U, 0x0020U
};

static const char *original_browser_strings[17] = {
    "All", "Armor", "Beam Weapons", "Bombs", "Electrical", "Engines",
    "Mechanical", "Mine Layers", "Mining Robots", "Orbital", "Planetary",
    "Scanners", "Shields", "Ship Hulls", "Starbase Hulls", "Terraforming",
    "Torpedoes"
};

static void sticky(void *context, StarsU16 hwnd, StarsPoint *point,
    StarsI16 save)
{
    (void)hwnd; (void)point; (void)save; ++((Trace *)context)->stickies;
}
static StarsU16 submenu(void *context, StarsU16 hwnd, StarsI16 position)
{
    (void)hwnd; (void)position; ++((Trace *)context)->menus; return 9;
}
static void check_menu(void *context, StarsU16 menu, StarsU16 id,
    StarsU16 flags)
{
    (void)menu; (void)id; (void)flags; ++((Trace *)context)->checks;
}
static void get_client(void *context, StarsU16 hwnd, StarsRect *rect)
{
    (void)hwnd; ++((Trace *)context)->rects;
    rect->left = 0; rect->top = 0; rect->right = 100; rect->bottom = 80;
}
static void fill(void *context, StarsU16 dc, const StarsRect *rect,
    StarsU16 brush)
{
    (void)dc; (void)rect; (void)brush; ++((Trace *)context)->fills;
}
static void set_color(void *context, StarsU16 dc, StarsU32 color)
{
    (void)dc; (void)color; ++((Trace *)context)->colors;
}
static StarsU16 metric(void *context, StarsU16 index)
{
    (void)context; return index == 7 ? 4 : (index == 8 ? 5 : 6);
}
static void set_pos(void *context, StarsU16 hwnd, StarsI16 x, StarsI16 y,
    StarsI16 width, StarsI16 height, StarsU16 flags)
{
    (void)hwnd; (void)x; (void)y; (void)width; (void)height; (void)flags;
    ++((Trace *)context)->positions;
}
static StarsU16 get_dc(void *context, StarsU16 hwnd)
{
    (void)hwnd; ++((Trace *)context)->gets; return 3;
}
static void select_object(void *context, StarsU16 dc, StarsU16 object)
{
    (void)dc; (void)object; ++((Trace *)context)->selects;
}
static void release_dc(void *context, StarsU16 hwnd, StarsU16 dc)
{
    (void)hwnd; (void)dc; ++((Trace *)context)->releases;
}
static StarsU16 get_item(void *context, StarsU16 hwnd, StarsU16 id)
{
    (void)context; (void)hwnd; return id == 0x10b ? 20 : (StarsU16)(id + 100);
}
static StarsI16 get_text(void *context, StarsU16 hwnd, StarsU16 id,
    StarsU8 *text, StarsU16 capacity)
{
    const char *value = "Hull";
    StarsI16 i;
    (void)hwnd; (void)id; ++((Trace *)context)->text_calls;
    if (capacity == 0) return 0;
    for (i = 0; i < 4 && (StarsU16)i + 1 < capacity; ++i)
        text[i] = (StarsU8)value[i];
    text[i] = 0;
    return 4;
}
static StarsI32 text_extent(void *context, StarsU16 dc, const StarsU8 *text,
    StarsI16 length)
{
    (void)dc; (void)text; ++((Trace *)context)->extents; return length * 8;
}
static StarsU16 create_window(void *context, const StarsU8 *class_name,
    StarsU32 style, StarsI16 x, StarsI16 y, StarsI16 width, StarsI16 height,
    StarsU16 parent)
{
    (void)class_name; (void)style; (void)x; (void)y; (void)width;
    (void)height; (void)parent; ++((Trace *)context)->creates; return 90;
}
static const StarsU8 *compressed(void *context, StarsU16 id)
{
    Trace *trace = (Trace *)context;
    const char *value;
    StarsI16 i;

    if (id >= 0x43fU && id <= 0x44fU)
        value = original_browser_strings[id - 0x43fU];
    else
        value = "Part";
    if (trace->strings == 0)
    {
        trace->first_string = id;
        strcpy((char *)trace->first_string_text, value);
    }
    trace->last_string = id;
    strcpy((char *)trace->last_string_text, value);
    for (i = 0; value[i] != '\0'; ++i)
        trace->string_payload_sum += (StarsU8)value[i];
    return (const StarsU8 *)value;
}
static StarsI32 send_message(void *context, StarsU16 hwnd, StarsU16 message,
    StarsU16 wparam, StarsU32 lparam)
{
    Trace *trace = (Trace *)context;
    (void)hwnd; (void)wparam; (void)lparam;
    if (message == 0x407) return trace->combo_selection;
    if (message == 0x403) { ++trace->strings; return 0; }
    if (message == 0x40e) { ++trace->resets; return 0; }
    return 0;
}
static StarsI16 lookup(void *context, StarsU16 group, StarsU16 item_word)
{
    Trace *trace = (Trace *)context;
    StarsU16 item;
    if (trace->packed_mode) {
        item = (StarsU16)(item_word & 0xffU);
        trace->packed_part[0] = (StarsU8)group;
        trace->packed_part[1] = (StarsU8)(group >> 8);
        trace->packed_part[2] = (StarsU8)item_word;
        trace->packed_part[3] = (StarsU8)(item_word >> 8);
        trace->packed_lookups++;
        trace->packed_offset = 0;
        if (group == 8U) {
            if (item > 11U) return 0;
            trace->packed_offset = 0x4e0UL +
                (StarsU32)item * 0x36UL;
        } else if (group == 0x10U) {
            if (item > 23U) return 0;
            trace->packed_offset = 0x2450UL +
                (StarsU32)item * 0x3cUL;
        } else {
            return 0;
        }
        trace->packed_part[4] = (StarsU8)trace->packed_offset;
        trace->packed_part[5] = (StarsU8)(trace->packed_offset >> 8);
        trace->packed_part[6] = (StarsU8)(trace->packed_offset >> 16);
        trace->packed_part[7] = (StarsU8)(trace->packed_offset >> 24);
        copy_original_component(trace, group, item);
        return 1;
    }
    trace->last_group = (StarsI16)group;
    trace->last_item = (StarsI16)(item_word & 0xffU);
    ++trace->lookups;
    return (item_word & 0xffU) == 1U ? 1 : 0;
}
static StarsI16 should_hide(void *context, StarsU16 group, StarsU16 item,
    StarsU16 result)
{
    (void)group; (void)item; (void)result; ++((Trace *)context)->hides;
    return 0;
}
static StarsU16 group_from_selection(void *context, StarsI16 selection)
{
    StarsI16 index = selection >> 1;
    (void)context;
    if (index < 1 || index > 16) index = 1;
    return browser_groups[index];
}
static StarsU16 step_group(void *context, StarsU16 group, StarsI16 direction)
{
    StarsI16 index;
    (void)context;
    for (index = 1; index <= 16; ++index)
        if (browser_groups[index] == group)
            break;
    if (index > 16) index = 1;
    index += direction;
    if (index > 16) index = 1;
    if (index < 1) index = 16;
    return browser_groups[index];
}
static StarsI16 button_checked(void *context, StarsU16 hwnd, StarsU16 id)
{
    (void)context; (void)hwnd; (void)id; return 1;
}
static void invalidate(void *context, StarsU16 hwnd)
{
    (void)hwnd; ++((Trace *)context)->invalidates;
}
static void end_dialog(void *context, StarsU16 hwnd, StarsI16 result)
{
    (void)hwnd; (void)result; ++((Trace *)context)->ends;
}
static void advance(void *context)
{
    ++((Trace *)context)->tutors;
}
static StarsI32 default_proc(void *context, StarsU16 hwnd, StarsU16 message,
    StarsU16 wparam, StarsU32 lparam)
{
    (void)hwnd; (void)message; (void)wparam; (void)lparam;
    ++((Trace *)context)->defaults; return 77;
}

static void install(StarsBrowserDlgOps *ops)
{
    memset(ops, 0, sizeof *ops);
    ops->sticky_position = sticky;
    ops->get_submenu = submenu;
    ops->check_menu_item = check_menu;
    ops->get_client_rect = get_client;
    ops->fill_rect = fill;
    ops->set_background_color = set_color;
    ops->get_system_metrics = metric;
    ops->set_window_pos = set_pos;
    ops->get_dc = get_dc;
    ops->select_object = select_object;
    ops->release_dc = release_dc;
    ops->get_dialog_item = get_item;
    ops->get_item_text = get_text;
    ops->get_text_extent = text_extent;
    ops->create_window = create_window;
    ops->get_compressed_string = compressed;
    ops->send_message = send_message;
    ops->lookup_part = lookup;
    ops->should_hide = should_hide;
    ops->group_from_selection = group_from_selection;
    ops->step_group = step_group;
    ops->button_checked = button_checked;
    ops->invalidate = invalidate;
    ops->end_dialog = end_dialog;
    ops->advance_tutor = advance;
    ops->default_proc = default_proc;
    ops->begin_paint = (StarsBrowserDlgBeginPaint)0;
    ops->end_paint = (StarsBrowserDlgEndPaint)0;
}

int main(void)
{
    StarsBrowserDlgState state;
    StarsBrowserDlgOps ops;
    StarsBrowserDlgInput input;
    Trace trace;
    StarsI16 checks;
    StarsI16 failures;

    memset(&state, 0, sizeof state);
    memset(&trace, 0, sizeof trace);
    install(&ops);
    state.arial8_height = 12;
    state.arial10_height = 10;
    state.font_arial8 = 2;
    state.button_face_brush = 3;
    state.button_face_color = 0x1234UL;
    state.frame_window = 8;
    state.game_bits = 0x0800;
    state.browser_valid = 1;
    state.item_word = 0x1201;
    state.hull_group = 8;
    trace.combo_selection = 1;
    input.state = &state;
    input.ops = &ops;
    input.context = &trace;
    checks = 0;
    failures = 0;
#define CHECK(condition) do { ++checks; if (!(condition)) ++failures; } while (0)
    CHECK(stars_browser_dlg(&input, 20, 272, 0, 0) == 1);
    CHECK(state.browser_window == 20 && state.browser_child_window == 90);
    CHECK(state.item_word == 0x1201);
    CHECK(browser_groups[1] == 0x0008U && browser_groups[16] == 0x0020U);
    CHECK(trace.strings == 17 && trace.resets == 1 && trace.tutors == 1);
    CHECK(trace.first_string == 0x43fU && trace.last_string == 0x44fU);
    CHECK(strcmp((const char *)trace.first_string_text, "All") == 0 &&
        strcmp((const char *)trace.last_string_text, "Torpedoes") == 0 &&
        trace.string_payload_sum == 15229UL);
    CHECK(trace.gets == 1 && trace.releases == 1 && trace.extents == 1);
    CHECK(stars_browser_dlg(&input, 20, 15, 0, 0) == 1);
    CHECK(stars_browser_dlg(&input, 20, 20, 4, 0) == 1);
    CHECK(trace.fills == 1 && trace.rects == 1);
    CHECK(stars_browser_dlg(&input, 20, 25, 4, 1170) == 3);
    CHECK(trace.colors == 1);
    CHECK(stars_browser_dlg(&input, 20, 273, 0x10b, 1UL << 16) == 0);
    CHECK(trace.invalidates == 1 && trace.last_group == 8 &&
        state.item_word == 0x1200);
    CHECK(stars_browser_dlg(&input, 20, 273, 0x42f, 0) == 0);
    CHECK(state.component_valid == 1 && trace.invalidates == 2 &&
        state.item_word == 0x1201);
    CHECK(stars_browser_dlg(&input, 20, 273, 0x42e, 0) == 0);
    CHECK(trace.invalidates == 3 && state.item_word == 0x1200);
    trace.combo_selection = 0;
    state.hull_group = browser_groups[16];
    state.item_word = 0x12ff;
    CHECK(stars_browser_dlg(&input, 20, 273, 0x42f, 0) == 0);
    CHECK(state.hull_group == browser_groups[1] &&
        state.item_word == 0x1201);
    CHECK(step_group(0, browser_groups[1], -1) == browser_groups[16]);
    trace.packed_mode = 1;
    trace.combo_selection = 1;
    state.browser_valid = 1;
    state.hull_group = 8;
    state.item_word = 0x120a;
    CHECK(stars_browser_dlg(&input, 20, 272, 0, 0) == 1);
    CHECK(trace.packed_part[0] == 8U &&
        ((StarsU16)trace.packed_part[2] |
            ((StarsU16)trace.packed_part[3] << 8)) == 0x120aU &&
        trace.packed_offset == 0x6fcUL);
    CHECK(sizeof(OriginalComponent) == 52U);
    CHECK(((StarsU16)trace.packed_component[0] |
            ((StarsU16)trace.packed_component[1] << 8)) == 11U &&
        trace.packed_component[8] == 'V' &&
        trace.packed_component[15] == 'm' &&
        ((StarsU16)trace.packed_component[40] |
            ((StarsU16)trace.packed_component[41] << 8)) == 40U &&
        ((StarsU16)trace.packed_component[42] |
            ((StarsU16)trace.packed_component[43] << 8)) == 50U &&
        ((StarsU16)trace.packed_component[44] |
            ((StarsU16)trace.packed_component[45] << 8)) == 15U &&
        ((StarsU16)trace.packed_component[50] |
            ((StarsU16)trace.packed_component[51] << 8)) == 66U);
    CHECK(stars_browser_dlg(&input, 20, 273, 0x42f, 0) == 0);
    CHECK(state.component_valid == 1 && trace.packed_part[0] == 8U &&
        ((StarsU16)trace.packed_part[2] |
            ((StarsU16)trace.packed_part[3] << 8)) == 0x120bU &&
        trace.packed_offset == 0x732UL);
    CHECK(((StarsU16)trace.packed_component[0] |
            ((StarsU16)trace.packed_component[1] << 8)) == 12U &&
        trace.packed_component[8] == 'S' &&
        ((StarsU16)trace.packed_component[40] |
            ((StarsU16)trace.packed_component[41] << 8)) == 30U &&
        ((StarsU16)trace.packed_component[42] |
            ((StarsU16)trace.packed_component[43] << 8)) == 100U &&
        ((StarsU16)trace.packed_component[44] |
            ((StarsU16)trace.packed_component[45] << 8)) == 25U &&
        ((StarsU16)trace.packed_component[48] |
            ((StarsU16)trace.packed_component[49] << 8)) == 0U &&
        ((StarsU16)trace.packed_component[50] |
            ((StarsU16)trace.packed_component[51] << 8)) == 77U);
    trace.combo_selection = 2;
    CHECK(stars_browser_dlg(&input, 20, 273, 0x10b, 1UL << 16) == 0);
    CHECK(state.hull_group == 0x10U && trace.packed_part[0] == 0x10U &&
        trace.packed_offset == 0x2450UL &&
        ((StarsU16)trace.packed_component[0] |
            ((StarsU16)trace.packed_component[1] << 8)) == 1U &&
        trace.packed_component[8] == 'L' &&
        ((StarsU16)trace.packed_component[40] |
            ((StarsU16)trace.packed_component[41] << 8)) == 1U &&
        ((StarsU16)trace.packed_component[42] |
            ((StarsU16)trace.packed_component[43] << 8)) == 5U &&
        ((StarsU16)trace.packed_component[46] |
            ((StarsU16)trace.packed_component[47] << 8)) == 6U &&
        ((StarsU16)trace.packed_component[50] |
            ((StarsU16)trace.packed_component[51] << 8)) == 28U);
    CHECK(lookup(&trace, 0x10U, 0x1212U) == 1 &&
        trace.packed_offset == 0x2888UL &&
        ((StarsU16)trace.packed_component[0] |
            ((StarsU16)trace.packed_component[1] << 8)) == 19U &&
        trace.packed_component[8] == 'M' &&
        ((StarsU16)trace.packed_component[42] |
            ((StarsU16)trace.packed_component[43] << 8)) == 40U &&
        ((StarsU16)trace.packed_component[48] |
            ((StarsU16)trace.packed_component[49] << 8)) == 6U &&
        ((StarsU16)trace.packed_component[50] |
            ((StarsU16)trace.packed_component[51] << 8)) == 111U);
    CHECK(stars_browser_dlg(&input, 20, 273, 2, 0) == 1);
    CHECK(trace.ends == 1 && trace.menus == 1 && trace.checks == 1);
    CHECK(stars_browser_dlg(&input, 20, 2, 0, 0) == 77);
    CHECK(trace.defaults == 1 && trace.stickies >= 3);
#undef CHECK
#ifdef TARGET_NATIVE
    {
        FILE *report = fopen("BRDR.TXT", "w");
        if (!report)
            return 7;
        fprintf(report,
            "BROWSER_DLG_CHECKS=%d FAILURES=%d RESULT=%s\n",
            checks, failures, failures == 0 ? "PASS" : "FAIL");
        fclose(report);
    }
#else
    if (failures == 0)
        printf("BROWSER_DLG_CHECKS=%d FAILURES=0 RESULT=PASS\n", checks);
    else
        printf("BROWSER_DLG_CHECKS=%d FAILURES=%d RESULT=FAIL\n",
            checks, failures);
#endif
    return failures == 0 ? 0 : 1;
}
