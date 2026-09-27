#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsI16 gets, source_calls, planet_calls, titles, texts, sends;
    StarsU16 ids[8], controls[8], messages[8], wparams[8];
    StarsU32 lparams[8];
    StarsI16 source_fill;
    const StarsProductionList *planet_queue;
    const StarsProductionPlanet *planet_arg;
    StarsU8 title[88];
    StarsU8 window_title[88];
} Trace;

static const StarsU8 *planet_name(void *value, StarsU16 id)
{
    Trace *trace = (Trace *)value;
    (void)trace;
    if (id != 7U) return (const StarsU8 *)0;
    return (const StarsU8 *)"Orion";
}

static const StarsU8 *compressed(void *value, StarsU16 id)
{
    (void)value;
    if (id != 0x033eU) return (const StarsU8 *)0;
    return (const StarsU8 *)"%s Production";
}

static void format_title(void *value, StarsU8 *out, const StarsU8 *fmt,
    const StarsU8 *name)
{
    Trace *trace = (Trace *)value;
    trace->titles++;
    strcpy((char *)out, (const char *)name);
    strcat((char *)out, (const char *)fmt + 2);
    strcpy((char *)trace->title, (const char *)out);
}

static void fill_source(void *value, StarsU16 list, StarsI16 md)
{
    Trace *trace = (Trace *)value;
    trace->source_calls++;
    trace->source_fill = md;
    trace->controls[0] = list;
}

static void fill_planet(void *value, StarsU16 list,
    const StarsProductionList *queue, const StarsProductionPlanet *planet)
{
    Trace *trace = (Trace *)value;
    trace->planet_calls++;
    trace->planet_queue = queue;
    trace->planet_arg = planet;
    trace->controls[1] = list;
}

static void set_window_text(void *value, StarsU16 dialog, const StarsU8 *text)
{
    Trace *trace = (Trace *)value;
    (void)dialog;
    trace->texts++;
    strcpy((char *)trace->window_title, (const char *)text);
}

static StarsU16 get_item(void *value, StarsU16 dialog, StarsU16 id)
{
    Trace *trace = (Trace *)value;
    (void)dialog;
    trace->ids[trace->gets] = id;
    ++trace->gets;
    return (StarsU16)(100U + id);
}

static void send_message(void *value, StarsU16 control, StarsU16 message,
    StarsU16 wparam, StarsU32 lparam)
{
    Trace *trace = (Trace *)value;
    StarsI16 n = trace->sends++;
    trace->controls[n] = control;
    trace->messages[n] = message;
    trace->wparams[n] = wparam;
    trace->lparams[n] = lparam;
}

int main(void)
{
    StarsProductionList queue;
    StarsProductionPlanet planet;
    StarsInitializeProductionDlgInput input;
    Trace trace;

    memset(&queue, 0, sizeof queue);
    memset(&planet, 0, sizeof planet);
    memset(&input, 0, sizeof input);
    memset(&trace, 0, sizeof trace);
    planet.id = 7U;
    planet.flags = 0x00800000UL;
    queue.item_count = 3U;
    queue.items[0] = (1UL << 17) | (5UL << 10);
    queue.items[1] = (2UL << 17) | (1UL << 10);
    queue.items[2] = (1UL << 17) | (7UL << 10);
    input.global_queue = &queue;
    input.selected_planet = &planet;
    input.planet_name = planet_name;
    input.compressed_string = compressed;
    input.format_title = format_title;
    input.fill_source = fill_source;
    input.fill_planet = fill_planet;
    input.set_window_text = set_window_text;
    input.get_dialog_item = get_item;
    input.send_message = send_message;
    input.context = &trace;
    stars_initialize_production_dlg(9U, &input);
    if (trace.titles != 1 || trace.texts != 1 ||
        strcmp((char *)trace.window_title, "Orion Production") != 0 ||
        trace.source_calls != 1 || trace.source_fill != -1 ||
        trace.planet_calls != 1 || trace.planet_queue != &queue ||
        trace.planet_arg != (const StarsProductionPlanet *)0)
        return 1;
    if (trace.sends != 3 || trace.messages[0] != 0x0407U ||
        trace.wparams[0] != 0U || trace.messages[1] != 0x0407U ||
        trace.wparams[1] != 3U || trace.messages[2] != 0x0401U ||
        trace.wparams[2] != 1U)
        return 2;
    puts("INITIALIZE_PRODUCTION_DLG_SOURCE=PASS");
    return 0;
}
