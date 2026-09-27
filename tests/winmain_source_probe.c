#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct ProbeContext {
    int init_mdi_ok;
    int colors_ok;
    int create_calls;
    int mdi_calls;
    int colors_calls;
    int init_calls;
    int batch_calls;
    int alert_calls;
    int post_calls;
    int free_calls;
    int translate_calls;
    int translated;
    int dispatch_calls;
    int key_calls;
    int char_calls;
    int iconic_calls;
    int message_index;
    int message_count;
    int title_enabled;
    StarsWinMainMessage messages[4];
} ProbeContext;

static StarsI16 init_mdi(void *opaque)
{ ProbeContext *c = (ProbeContext *)opaque; ++c->mdi_calls; return (StarsI16)c->init_mdi_ok; }
static StarsU32 tick(void *opaque)
{ (void)opaque; return 0x12345678UL; }
static void randomize2(void *opaque, StarsU32 value)
{ (void)opaque; (void)value; }
static StarsI16 create_stuff(void *opaque)
{ ProbeContext *c = (ProbeContext *)opaque; ++c->create_calls; return 1; }
static StarsI16 colors(void *opaque)
{ ProbeContext *c = (ProbeContext *)opaque; ++c->colors_calls; return (StarsI16)c->colors_ok; }
static StarsI16 init_instance(void *opaque, StarsI16 show, StarsWinMainState *state)
{ ProbeContext *c = (ProbeContext *)opaque; (void)show; ++c->init_calls;
    state->frame = 0x20U; state->title = c->title_enabled ? 0x30U : 0U;
    state->accel_frame = 0x40U; state->accel_title = 0x50U; return 1; }
static StarsI16 batch(void *opaque)
{ ProbeContext *c = (ProbeContext *)opaque; ++c->batch_calls; return 1; }
static StarsU32 salt(void *opaque, const StarsU8 *text)
{ const StarsU8 *p = text; (void)opaque;
    while (*p != '\0') ++p;
    return 0x6000UL + (StarsU32)(p - text); }
static void alert(void *opaque, StarsU16 id, StarsI16 flags)
{ ProbeContext *c = (ProbeContext *)opaque; (void)id; (void)flags; ++c->alert_calls; }
static void post(void *opaque, StarsU16 hwnd, StarsU16 message,
    StarsU16 wparam, StarsU32 lparam)
{ ProbeContext *c = (ProbeContext *)opaque; (void)hwnd; (void)message;
    (void)wparam; (void)lparam; ++c->post_calls; }
static StarsI16 get_message(void *opaque, StarsWinMainMessage *message)
{ ProbeContext *c = (ProbeContext *)opaque;
    if (c->message_index >= c->message_count) return 0;
    *message = c->messages[c->message_index++]; return 1; }
static StarsI16 iconic(void *opaque, StarsU16 hwnd)
{ ProbeContext *c = (ProbeContext *)opaque; (void)hwnd; ++c->iconic_calls; return 0; }
static StarsI16 translate(void *opaque, StarsU16 hwnd, StarsU16 accel,
    const StarsWinMainMessage *message)
{ ProbeContext *c = (ProbeContext *)opaque; (void)hwnd; (void)accel; (void)message;
    ++c->translate_calls; return (StarsI16)c->translated; }
static void translate_message(void *opaque, const StarsWinMainMessage *message)
{ (void)opaque; (void)message; }
static StarsI16 handle_key(void *opaque, StarsU16 hwnd, StarsU16 message,
    StarsU16 wparam, StarsU32 lparam)
{ ProbeContext *c = (ProbeContext *)opaque; (void)hwnd; (void)message;
    (void)wparam; (void)lparam; ++c->key_calls; return 1; }
static StarsI16 handle_char(void *opaque, StarsU16 hwnd, StarsU16 wparam,
    StarsU32 lparam)
{ ProbeContext *c = (ProbeContext *)opaque; (void)hwnd; (void)wparam;
    (void)lparam; ++c->char_calls; return 1; }
static void dispatch(void *opaque, const StarsWinMainMessage *message)
{ ProbeContext *c = (ProbeContext *)opaque; (void)message; ++c->dispatch_calls; }
static void free_stuff(void *opaque)
{ ProbeContext *c = (ProbeContext *)opaque; ++c->free_calls; }

static void reset(ProbeContext *context, StarsWinMainOps *ops)
{
    memset(context, 0, sizeof(*context)); memset(ops, 0, sizeof(*ops));
    context->init_mdi_ok = 1; context->colors_ok = 1;
    context->title_enabled = 1;
    ops->init_mdi = init_mdi; ops->get_tick = tick; ops->randomize2 = randomize2;
    ops->create_stuff = create_stuff; ops->get_system_colors = colors;
    ops->init_instance = init_instance; ops->set_up_batch_processing = batch;
    ops->salt_from_string = salt; ops->alert_ids = alert; ops->post_message = post;
    ops->get_message = get_message; ops->is_iconic = iconic;
    ops->translate_accelerator = translate; ops->translate_message = translate_message;
    ops->handle_key = handle_key; ops->handle_char = handle_char;
    ops->dispatch_message = dispatch; ops->free_stuff = free_stuff;
}

static int startup_case(void)
{
    ProbeContext context; StarsWinMainOps ops; StarsWinMainState state;
    StarsI16 result;
    reset(&context, &ops); memset(&state, 0, sizeof(state));
    context.message_count = 2;
    context.messages[0].hwnd = 7U; context.messages[0].message = 0x111U;
    context.messages[1].hwnd = 8U; context.messages[1].message = 0x222U;
    context.messages[1].wparam = 0x1234U;
    result = stars_win_main(&state, 1U, 0U,
        (const StarsU8 *)"/a /b BASE /c /dFMP /g12 /h /l /p PASS /t /v /w /x",
        1, &ops, &context);
    return result == 0x1234 && state.ini_flags == 0xfe1fU &&
        state.global_low_flags == 0x0200U && state.global_high_flags == 0x0040U &&
        state.turn_generation_count == 11 && strcmp(state.base, "BASE") == 0 &&
        strcmp(state.pass_last, "PASS") == 0 && state.salt_last == 0x6004UL &&
        state.vtimer_auto_generate == 1U && context.mdi_calls == 1 &&
        context.batch_calls == 1 && context.post_calls == 1 &&
        context.dispatch_calls == 2 && context.free_calls == 1 ? 0 : 1;
}

static int init_failure_case(void)
{
    ProbeContext context; StarsWinMainOps ops; StarsWinMainState state;
    reset(&context, &ops); context.init_mdi_ok = 0; memset(&state, 0, sizeof(state));
    if (stars_win_main(&state, 1U, 0U, (const StarsU8 *)"", 1, &ops, &context) != 0)
        return 1;
    return context.mdi_calls == 1 && context.alert_calls == 1 &&
        context.create_calls == 0 && state.vtimer_auto_generate == 1U ? 0 : 1;
}

static int previous_instance_case(void)
{
    ProbeContext context; StarsWinMainOps ops; StarsWinMainState state;
    reset(&context, &ops); memset(&state, 0, sizeof(state));
    context.message_count = 1; context.messages[0].wparam = 9U;
    if (stars_win_main(&state, 1U, 2U, (const StarsU8 *)"GAME", 3,
        &ops, &context) != 9) return 1;
    return context.mdi_calls == 0 && context.create_calls == 1 &&
        strcmp(state.base, "GAME") == 0 && state.ini_flags == 3U ? 0 : 1;
}

static int fallback_key_case(void)
{
    ProbeContext context; StarsWinMainOps ops; StarsWinMainState state;
    reset(&context, &ops); memset(&state, 0, sizeof(state));
    context.title_enabled = 0;
    context.message_count = 1; context.messages[0].message = 0x0100U;
    context.messages[0].wparam = 0x0055U; context.messages[0].lparam = 4UL;
    if (stars_win_main(&state, 1U, 2U, (const StarsU8 *)"", 3,
        &ops, &context) != 0x0055) return 1;
    return context.iconic_calls == 1 && context.key_calls == 1 &&
        context.dispatch_calls == 0 && context.free_calls == 1 ? 0 : 1;
}

int main(void)
{
    int failures;
    failures = startup_case() + init_failure_case() + previous_instance_case() +
        fallback_key_case();
    printf("WINMAIN_CHECKS=4 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures;
}
