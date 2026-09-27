#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Probe {
    int title_id;
    int create;
    int font_message;
    int frame;
    int center;
    int arrow;
    int survey;
    int end;
    int survey_rect;
    int create_dc;
    int delete_dc;
    int planet;
    int lookup;
    int details;
    int mode_sets;
    int color_sets;
    int order;
    int survey_order;
    int details_order;
    int end_order;
    StarsU16 combo;
    StarsI16 owner_known;
    StarsI16 detail_rows;
    StarsI16 turns;
    StarsI16 environment;
    StarsI16 concentration;
    StarsI32 minerals;
} Probe;

static void get_text(void *v, StarsU16 id, StarsU8 *out, StarsU16 n)
{
    Probe *p = (Probe *)v;
    const char *text;

    p->title_id = id == 0x04bdU ? 1 : p->title_id;
    text = id == 0xffffU ? "BUTTON" : "Survey the selected object";
    if (n != 0) {
        strncpy((char *)out, text, n - 1U);
        out[n - 1U] = 0;
    }
}

static StarsU16 create_window(void *v, const StarsU8 *class_name,
    const StarsU8 *title, StarsU32 style, StarsI16 x, StarsI16 y,
    StarsI16 w, StarsI16 h, StarsU16 parent, StarsU16 menu,
    StarsU16 instance)
{
    Probe *p = (Probe *)v;

    if (strcmp((const char *)class_name, "BUTTON") == 0 &&
            strcmp((const char *)title, "Survey the selected object") == 0 &&
            style == 0x40000003UL && x == 100 && y == 100 && w == 0x96 &&
            h == 8 && parent == 9 && menu == 0 && instance == 3)
        ++p->create;
    p->combo = 42;
    return p->combo;
}

static StarsI32 send_message(void *v, StarsU16 window, StarsU16 message,
    StarsU16 wparam, StarsU32 lparam)
{
    Probe *p = (Probe *)v;
    (void)lparam;
    if (window == 42 && message == 0x30 && wparam == 17)
        ++p->font_message;
    return 0L;
}

static void set_title_bar(void *v, StarsU16 hwnd)
{
    (void)v;
    (void)hwnd;
}

static StarsU16 begin_paint(void *v, StarsU16 hwnd, StarsRect *r)
{
    (void)v;
    (void)hwnd;
    r->left = 0;
    r->top = 0;
    r->right = 120;
    r->bottom = 80;
    return 7;
}

static void get_client_rect(void *v, StarsU16 hwnd, StarsRect *r)
{
    (void)v;
    (void)hwnd;
    r->left = 0;
    r->top = 0;
    r->right = 120;
    r->bottom = 80;
}

static void draw_frame(void *v, StarsU16 dc, const StarsRect *r,
    StarsU16 kind)
{
    Probe *p = (Probe *)v;
    if (dc == 7 && r->left == 4 && r->top == 4 && r->right == 116 &&
            r->bottom == 12 && kind == 0)
        ++p->frame;
    ++p->order;
}

static StarsU32 set_color(void *v, StarsU16 dc, StarsU32 color)
{
    Probe *p = (Probe *)v;
    (void)dc;
    (void)color;
    ++p->color_sets;
    return 55UL;
}

static StarsI32 text_extent(void *v, StarsU16 dc, const StarsU8 *s,
    StarsI16 n)
{
    (void)v;
    (void)dc;
    (void)s;
    return (StarsI32)n * 8L;
}

static void center_text(void *v, StarsU16 dc, const StarsRect *r,
    const StarsU8 *s, StarsI16 n)
{
    Probe *p = (Probe *)v;
    (void)dc;
    (void)r;
    (void)s;
    (void)n;
    ++p->center;
    ++p->order;
}

static void draw_arrow(void *v, StarsU16 dc, const StarsRect *r,
    StarsI16 on)
{
    Probe *p = (Probe *)v;
    (void)dc;
    (void)r;
    if (on == 1)
        ++p->arrow;
    ++p->order;
}

static StarsU16 survey_create_dc(void *v, StarsU16 hdc)
{
    Probe *p = (Probe *)v;
    (void)hdc;
    ++p->create_dc;
    return 19;
}

static void survey_delete_dc(void *v, StarsU16 hdc)
{
    Probe *p = (Probe *)v;
    (void)hdc;
    ++p->delete_dc;
}

static void survey_color(void *v, StarsU16 hdc, StarsU32 color)
{
    Probe *p = (Probe *)v;
    (void)hdc;
    (void)color;
    ++p->color_sets;
}

static void survey_mode(void *v, StarsU16 hdc, StarsU16 mode)
{
    Probe *p = (Probe *)v;
    (void)hdc;
    (void)mode;
    ++p->mode_sets;
}

static StarsU16 survey_get_mode(void *v, StarsU16 hdc)
{
    (void)v;
    (void)hdc;
    return 7;
}

static StarsU32 survey_get_color(void *v, StarsU16 hdc)
{
    (void)v;
    (void)hdc;
    return 0x12345678UL;
}

static StarsI16 draw_planet(void *v, StarsU16 dc,
    StarsDrawMineSurveyRect *r, StarsI16 id)
{
    Probe *p = (Probe *)v;
    (void)dc;
    (void)r;
    if (id == 5)
        ++p->planet;
    return 1;
}

static StarsI16 lookup_planet(void *v, StarsI16 id)
{
    Probe *p = (Probe *)v;
    if (id == 5)
        ++p->lookup;
    return 1;
}

static void planet_details(void *v, StarsU16 dc,
    StarsDrawMineSurveyRect *r, StarsI16 id,
    const StarsDrawMineSurveyPlanetSummary *summary)
{
    Probe *p = (Probe *)v;
    (void)dc;
    (void)r;
    if (id == 5) {
        ++p->details;
        p->details_order = ++p->order;
        p->owner_known = summary->owner_known;
        p->detail_rows = summary->detailed_rows;
        p->turns = summary->turns_since_survey;
        p->environment = summary->environment[0];
        p->concentration = summary->mineral_concentration[0];
        p->minerals = summary->minerals[0];
    }
}

static StarsU16 get_survey_background_mode(void *v, StarsU16 hdc)
{
    return survey_get_mode(v, hdc);
}

static void draw_survey(void *v, StarsU16 dc, const StarsRect *r)
{
    Probe *p = (Probe *)v;
    StarsDrawMineSurveyInput input;
    StarsDrawMineSurveyOps ops;
    StarsDrawMineSurveyPlanetInfo planet;

    ++p->survey;
    p->survey_order = ++p->order;
    p->survey_rect = dc == 7 && r->left == 0 && r->top == 12 &&
        r->right == 120 && r->bottom == 80;
    (void)memset(&input, 0, sizeof input);
    (void)memset(&ops, 0, sizeof ops);
    (void)memset(&planet, 0, sizeof planet);
    ops.create_compatible_dc = survey_create_dc;
    ops.delete_dc = survey_delete_dc;
    ops.set_background = survey_color;
    ops.set_text_color = survey_color;
    ops.get_background_mode = get_survey_background_mode;
    ops.get_background_color = survey_get_color;
    ops.get_text_color = survey_get_color;
    ops.set_background_mode = survey_mode;
    ops.draw_planet_panel = draw_planet;
    ops.lookup_planet = lookup_planet;
    ops.draw_planet_details = planet_details;
    ops.context = p;
    planet.owner = 3;
    planet.survey_detail = 3;
    planet.survey_turn = 11;
    planet.current_turn = 17;
    planet.environment[0] = 70;
    planet.mineral_concentration[0] = 82;
    planet.minerals[0] = 1234L;
    input.hdc = dc;
    input.rect.left = r->left;
    input.rect.top = r->top;
    input.rect.right = r->right;
    input.rect.bottom = r->bottom;
    input.selected_kind = 1;
    input.selected_planet = 5;
    input.planet_info = &planet;
    input.ops = &ops;
    stars_draw_mine_survey(&input);
}

static void end_paint(void *v, StarsU16 hwnd, StarsRect *r)
{
    Probe *p = (Probe *)v;
    (void)hwnd;
    (void)r;
    p->end_order = ++p->order;
    ++p->end;
}

static StarsI16 other_at_selection(void *v)
{
    (void)v;
    return 1;
}

int main(void)
{
    StarsMineWndOps wnd_ops;
    StarsMineWndInput wnd_input;
    Probe probe;
    int failures = 0;

    (void)memset(&wnd_ops, 0, sizeof wnd_ops);
    (void)memset(&wnd_input, 0, sizeof wnd_input);
    (void)memset(&probe, 0, sizeof probe);
    wnd_ops.get_text = get_text;
    wnd_ops.create_window = create_window;
    wnd_ops.send_message = send_message;
    wnd_ops.set_title_bar = set_title_bar;
    wnd_ops.begin_paint = begin_paint;
    wnd_ops.get_client_rect = get_client_rect;
    wnd_ops.draw_frame = draw_frame;
    wnd_ops.set_text_color = set_color;
    wnd_ops.set_background_color = set_color;
    wnd_ops.text_extent = text_extent;
    wnd_ops.center_text = center_text;
    wnd_ops.draw_arrow = draw_arrow;
    wnd_ops.draw_survey = draw_survey;
    wnd_ops.other_at_selection = other_at_selection;
    wnd_ops.end_paint = end_paint;
    wnd_input.instance = 3;
    wnd_input.font = 17;
    wnd_input.font_height = 8;
    wnd_input.text_color = 1;
    wnd_input.background_color = 2;
    wnd_input.button_face_brush = 6;
    wnd_input.button_class = (const StarsU8 *)"BUTTON";
    wnd_input.title = (const StarsU8 *)"Mineral Scanner";
    wnd_input.ops = &wnd_ops;
    wnd_input.context = &probe;
    if (stars_mine_wnd_proc(&wnd_input, 9, 1, 0, 0) != 0)
        ++failures;
    if (stars_mine_wnd_proc(&wnd_input, 9, 0x000f, 0, 0) != 0)
        ++failures;
    if (probe.title_id != 1 || probe.create != 1 || probe.font_message != 1)
        ++failures;
    if (probe.frame != 1 || probe.center != 1 || probe.arrow != 1 ||
            probe.survey != 1 || probe.survey_rect != 1 || probe.end != 1)
        ++failures;
    if (probe.create_dc != 1 || probe.delete_dc != 1 || probe.planet != 1 ||
            probe.lookup != 1 || probe.details != 1)
        ++failures;
    if (probe.owner_known != 1 || probe.detail_rows != 1 || probe.turns != 6)
        ++failures;
    if (probe.environment != 70 || probe.concentration != 82 ||
            probe.minerals != 1234L)
        ++failures;
    if (probe.details_order <= probe.survey_order ||
            probe.end_order <= probe.details_order)
        ++failures;
    if (probe.mode_sets != 2 || probe.color_sets < 6)
        ++failures;
#ifndef TARGET_NATIVE
    if (failures == 0)
        puts("DRAW_MINE_SURVEY_INTEGRATION_CHECKS=12 FAILURES=0 RESULT=PASS");
    else
        printf("DRAW_MINE_SURVEY_INTEGRATION_CHECKS=12 FAILURES=%d RESULT=FAIL\n",
            failures);
#endif
#ifdef TARGET_NATIVE
    {
        FILE *report = fopen("DMSINT.TXT", "w");
        if (!report)
            return 7;
        fprintf(report,
            "DRAW_MINE_SURVEY_INTEGRATION_CHECKS=12 FAILURES=%d RESULT=%s\n",
            failures, failures == 0 ? "PASS" : "FAIL");
        fclose(report);
    }
#endif
    return failures != 0;
}
