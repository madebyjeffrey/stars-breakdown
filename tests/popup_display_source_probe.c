#include "core.h"
#include <stdio.h>

typedef struct PopupProbe {
    int font_calls;
    int wrap_calls;
    StarsU16 hdc;
    StarsU16 font;
    StarsI16 width;
    StarsI16 print;
    const StarsU8 *text;
} PopupProbe;

static void select_font(void *context, StarsU16 hdc, StarsU16 font)
{
    PopupProbe *probe = (PopupProbe *)context;
    ++probe->font_calls;
    probe->hdc = hdc;
    probe->font = font;
}

static void wrap_text(void *context, StarsU16 hdc, StarsI16 *text_origin,
    StarsI16 *line_metrics, const StarsU8 *text, StarsI16 first,
    StarsI16 count, StarsI16 width, StarsI16 *right, StarsI16 unused,
    StarsI16 print)
{
    PopupProbe *probe = (PopupProbe *)context;
    (void)text_origin; (void)line_metrics; (void)first; (void)count;
    (void)right;
    (void)unused;
    ++probe->wrap_calls;
    probe->hdc = hdc;
    probe->text = text;
    probe->width = width;
    probe->print = print;
    *right = 17;
}

int main(void)
{
    static const StarsU8 text[] = "popup";
    StarsPopupTextOps ops;
    PopupProbe probe;
    StarsPoint point;
    int failures = 0;
    ops.select_font = select_font;
    ops.wrap_text = wrap_text;
    probe.font_calls = probe.wrap_calls = 0;
    point = stars_display_string(3, 20, 1, text, 9, 6, &ops, &probe);
    if (point.x != 21 || point.y != 10 || probe.font_calls != 1 ||
        probe.wrap_calls != 1 || probe.hdc != 3 || probe.font != 9 ||
        probe.width != 12 || probe.print != 1 || probe.text != text)
        ++failures;
    point = stars_display_string(3, 20, 0, text, 9, 6, 0, &probe);
    if (point.x != 8 || point.y != 10)
        ++failures;
    printf("POPUP_DISPLAY_CHECKS=2 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures != 0;
}
