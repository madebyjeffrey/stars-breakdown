#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    const char *text;
    char output[20];
    StarsU16 message;
    StarsU16 message_value;
    StarsU16 message_window;
    StarsU16 focused;
    StarsU16 invalidated[2];
    unsigned invalidated_count;
} Trace;

static void get_text(void *context, StarsU16 window, StarsU8 *text, StarsU16 size)
{
    Trace *t = (Trace *)context;
    (void)window;
    if (size) {
        strncpy((char *)text, t->text, (size_t)size - 1U);
        text[size - 1U] = 0;
    }
}
static void set_text(void *context, StarsU16 window, const StarsU8 *text)
{
    Trace *t = (Trace *)context;
    (void)window;
    strcpy(t->output, (const char *)text);
}
static void send_message(void *context, StarsU16 window, StarsU16 message,
    StarsU16 value, StarsU32 extra)
{
    Trace *t = (Trace *)context;
    (void)extra;
    t->message_window = window; t->message = message; t->message_value = value;
}
static void invalidate(void *context, StarsU16 window)
{
    Trace *t = (Trace *)context;
    if (t->invalidated_count < 2U)
        t->invalidated[t->invalidated_count++] = window;
}
static void focus(void *context, StarsU16 window)
{ ((Trace *)context)->focused = window; }

static void reset(StarsTerminateToolbarFocusInput *input, Trace *trace)
{
    memset(trace, 0, sizeof(*trace));
    input->cancel = 0U; input->radar = 10U; input->toolbar = 11U;
    input->scanner = 12U; input->frame = 13U; input->grbits2 = 0x1234U;
    input->scan_flags = 0U; input->radar_view = 50;
    input->get_window_text = get_text; input->set_window_text = set_text;
    input->send_message = send_message; input->invalidate_rect = invalidate;
    input->set_focus = focus; input->context = trace;
}

int main(void)
{
    StarsTerminateToolbarFocusInput input; Trace trace;
    reset(&input, &trace); trace.text = "37%";
    stars_terminate_toolbar_focus(&input);
    if (input.grbits2 != 0x1274U || input.radar_view != 37 ||
        strcmp(trace.output, "37%") != 0 || trace.message != 0x040eU ||
        trace.message_value != 6U || trace.invalidated_count != 2U ||
        trace.invalidated[0] != 11U || trace.invalidated[1] != 12U ||
        trace.focused != 13U) return 1;
    reset(&input, &trace); trace.text = "101";
    stars_terminate_toolbar_focus(&input);
    if (input.radar_view != 100 || strcmp(trace.output, "100%") != 0 ||
        trace.message_value != 0U) return 2;
    reset(&input, &trace); input.cancel = 1U; input.radar_view = 1;
    input.scan_flags = 0x0020U;
    stars_terminate_toolbar_focus(&input);
    if (input.radar_view != 2 || trace.invalidated_count != 1U ||
        trace.invalidated[0] != 12U || strcmp(trace.output, "2%") != 0) return 3;
    reset(&input, &trace); trace.text = "12x";
    stars_terminate_toolbar_focus(&input);
    if (input.radar_view != 2 || strcmp(trace.output, "2%") != 0) return 4;
    puts("TERMINATE_TOOLBAR_SOURCE=PASS");
    return 0;
}
