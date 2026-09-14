#include "runtime_internal.h"
#include <assert.h>
#include <setjmp.h>

typedef struct StarsRecovery {
    jmp_buf jump;
    StarsU16 token;
    struct StarsRecovery *previous;
} StarsRecovery;

StarsU16 stars_word(const StarsU8 *d, StarsU16 at)
{ return (StarsU16)(d[at] | ((StarsU16)d[(StarsU16)(at + 1U)] << 8)); }
StarsU32 stars_dword(const StarsU8 *d, StarsU16 at)
{ return stars_word(d, at) | ((StarsU32)stars_word(d, (StarsU16)(at + 2U)) << 16); }
void stars_put_word(StarsU8 *d, StarsU16 at, StarsU16 value)
{ d[at] = (StarsU8)value; d[(StarsU16)(at + 1U)] = (StarsU8)(value >> 8); }
void stars_put_dword(StarsU8 *d, StarsU16 at, StarsU32 value)
{ stars_put_word(d, at, (StarsU16)value); stars_put_word(d, (StarsU16)(at + 2U), (StarsU16)(value >> 16)); }
StarsI16 stars_signed_word(StarsU16 value)
{ return value < 32768U ? (StarsI16)value : (StarsI16)((StarsI32)value - 65536L); }

StarsU16 stars_scratch(StarsRuntime *r, StarsU16 count)
{
    assert(r->scratch_top >= r->scratch_bottom);
    assert(count <= r->scratch_top - r->scratch_bottom);
    r->scratch_top = (StarsU16)(r->scratch_top - count);
    return r->scratch_top;
}

StarsU32 stars_window(StarsRuntime *r, const char *op, StarsU16 hwnd,
    StarsU16 a, StarsU16 b, StarsU32 value, void *p, void *q)
{
    StarsWindowCall call;
    call.operation = op; call.window = hwnd; call.a = a; call.b = b;
    call.value = value; call.p = p; call.q = q;
    return r->platform->window(r->context, &call);
}
StarsU16 stars_control(StarsRuntime *r, StarsU16 hwnd, StarsU16 id)
{ return (StarsU16)stars_window(r, "GetDlgItem", hwnd, id, 0, 0, 0, 0); }
StarsU32 stars_send_item(StarsRuntime *r, StarsU16 hwnd, StarsU16 id,
    StarsU16 msg, StarsU16 wp, StarsU32 lp)
{ return stars_window(r, "SendDlgItemMessage", hwnd, id, msg, lp, &wp, 0); }
StarsU16 stars_game_string(StarsRuntime *r, StarsI16 id)
{ return stars_get_compressed_string(r->data, r->strings, id); }
StarsI16 stars_runtime_alert(StarsRuntime *r, StarsU16 text, StarsI16 type)
{ return stars_alert_string(r->data, text, type, &r->alerts, r); }

static StarsI16 runtime_open(void *p, const StarsU8 *name, StarsU16 mode, StarsU16 *error)
{ StarsRuntime *r = (StarsRuntime *)p; return r->platform->open(r->context, name, mode, error); }
static StarsI16 runtime_close(void *p, StarsI16 handle)
{ StarsRuntime *r = (StarsRuntime *)p; return r->platform->close(r->context, handle); }
static StarsU16 runtime_read(void *p, StarsI16 h, StarsU8 *out, StarsU16 n)
{ StarsRuntime *r = (StarsRuntime *)p; return r->platform->read(r->context, h, out, n); }
static StarsU16 runtime_write(void *p, StarsI16 h, const StarsU8 *in, StarsU16 n)
{ StarsRuntime *r = (StarsRuntime *)p; return r->platform->write(r->context, h, in, n); }
static StarsU32 runtime_ticks(void *p)
{ StarsRuntime *r = (StarsRuntime *)p; return r->platform->ticks(r->context); }
static const StarsU8 *runtime_memory(void *p, StarsU16 seg, StarsU16 off)
{ StarsRuntime *r = (StarsRuntime *)p; return r->platform->memory(r->context, seg, off); }
static void runtime_file_error(void *p, StarsI16 id)
{ StarsRuntime *r = (StarsRuntime *)p; stars_file_error(r->data, id, &r->alerts, r); }
static void runtime_write_error(void *p)
{ StarsRuntime *r = (StarsRuntime *)p; stars_runtime_alert(r, stars_format_id(r, 22, 0), 16); }
static void runtime_abort(void *p, StarsU16 token, StarsI16 value)
{ stars_runtime_abort((StarsRuntime *)p, token, value); }
void stars_runtime_abort(StarsRuntime *r, StarsU16 token, StarsI16 value)
{
    StarsRecovery *scope;
    for (scope = (StarsRecovery *)r->recovery; scope; scope = scope->previous)
        if (scope->token == token) { r->recovery = scope; longjmp(scope->jump, value); }
    r->platform->abort(r->context, token, value);
}
static StarsU16 runtime_focus(void *p)
{ return (StarsU16)stars_window((StarsRuntime *)p, "GetFocus", 0, 0, 0, 0, 0, 0); }
static StarsI16 runtime_message(void *p, StarsU16 owner, const StarsU8 *text,
    const StarsU8 *caption, StarsI16 type)
{ return stars_signed_word((StarsU16)stars_window((StarsRuntime *)p, "MessageBox",
    owner, (StarsU16)type, 0, 0, (void *)text, (void *)caption)); }
static void runtime_output(void *p, StarsI16 channel, const StarsU8 *text)
{ stars_output_string((StarsRuntime *)p, channel, text); }
static StarsU16 runtime_format(void *p, StarsI16 id)
{ return stars_format_id((StarsRuntime *)p, id, 0); }

void stars_runtime_init(StarsRuntime *r, StarsU8 *data, const StarsU8 *strings,
    const StarsPlatform *platform, void *context, StarsU16 bottom, StarsU16 top)
{
    memset(r, 0, sizeof(*r));
    r->data = data; r->strings = strings; r->platform = platform; r->context = context;
    r->scratch_bottom = bottom; r->scratch_top = top;
    r->segments[3] = strings; r->segments[37] = data;
    r->stream.open = runtime_open; r->stream.close = runtime_close;
    r->stream.read = runtime_read; r->stream.write = runtime_write;
    r->stream.ticks = runtime_ticks; r->stream.memory = runtime_memory;
    r->stream.file_error = runtime_file_error; r->stream.write_error = runtime_write_error;
    r->stream.abort = runtime_abort;
    r->alerts.focus = runtime_focus; r->alerts.message = runtime_message;
    r->alerts.output = runtime_output; r->alerts.format_id = runtime_format;
}
void stars_runtime_read(void *p, StarsU8 *out, StarsU16 n)
{ StarsRuntime *r = (StarsRuntime *)p; stars_stream_read(r->data, out, n, &r->stream, r); }
void stars_runtime_write(void *p, const StarsU8 *in, StarsU16 n)
{ StarsRuntime *r = (StarsRuntime *)p; stars_stream_write(r->data, in, n, &r->stream, r); }

/* NE 29:58D4-5AB7. Save dialog buffers are represented separately from the
 * 72-byte Win16 OPENFILENAME; the platform maps its pointer fields. */
StarsI16 stars_save_race(StarsRuntime *r, StarsU16 suggestion, StarsU16 player)
{
    StarsU16 top = r->scratch_top, file = stars_scratch(r, 256U);
    StarsU16 filter = stars_scratch(r, 257U), env, i, checksum;
    StarsU8 title[256], directory[256], eof[2];
    StarsSaveChoice choice;
    StarsU8 *d = r->data;
    StarsI16 result;
    if (suggestion) strcpy((char *)d + file, (char *)d + suggestion);
    else d[file] = 0;
    directory[0] = 0;
    stars_copy_game_string(d, r->strings, 0x531, filter);
    for (i = 0; d[filter + i]; ++i) if (d[filter + i] == '|') d[filter + i] = 0;
    memset(&choice, 0, sizeof(choice));
    choice.owner = stars_word(d, 0x48aaU); choice.flags = 0x8806U;
    choice.filter_index = 1; choice.file = d + file; choice.file_capacity = 256;
    choice.title = title; choice.title_capacity = 256;
    choice.directory = directory; choice.filter = d + filter; choice.extension = d + 0x136cU;
    result = r->platform->save_choice(r->context, &choice);
    if (!result) { r->scratch_top = top; return 0; }
    env = stars_scratch(r, 32U);
    result = stars_create_file(d, 5, -1, file, env, &r->stream, r);
    if (!result) {
        stars_runtime_alert(r, stars_format_id(r, 0x10b, 0), 16);
        r->scratch_top = top;
        return 0;
    }
    stars_write_player_record(d, player, 0, stars_runtime_write, r);
    checksum = stars_race_checksum(d, player);
    eof[0] = (StarsU8)checksum; eof[1] = (StarsU8)(checksum >> 8);
    stars_write_record(d, 0, 2, eof, stars_runtime_write, r);
    stars_stream_close(d, &r->stream, r);
    strcpy((char *)d + 0x1318U, (char *)choice.file + choice.file_offset);
    r->scratch_top = top;
    return 1;
}

/* NE 5:5DA8-5FFC. The local player is only published after all checks pass.
 * Volatile is necessary for locals assigned between setjmp and a failed read. */
StarsI16 stars_was_race_file(StarsRuntime *r, StarsU16 file, StarsI16 check)
{
    StarsRecovery scope;
    StarsU8 *d = r->data;
    StarsU16 top = r->scratch_top, player = stars_scratch(r, 192U);
    StarsU16 silent = stars_word(d, 0x74cU), old_env = stars_word(d, 0x6eU), version;
    StarsU32 salt;
    volatile StarsI16 error = -1, result = 0;
    scope.token = stars_scratch(r, 32U); scope.previous = (StarsRecovery *)r->recovery;
    r->recovery = &scope;
    stars_put_word(d, 0x74cU, 1);
    stars_put_word(d, 0x6eU, scope.token);
    if (!setjmp(scope.jump)) {
        stars_stream_open(d, file, 0x20, &r->stream, r);
        stars_read_record(d, stars_runtime_read, r);
        version = stars_word(d, 0x4ba0U);
        if ((stars_word(d, 0x2696U) >> 10) != 8U || (version >> 12) != 2U ||
                ((version >> 5) & 127U) < 49U || ((version >> 5) & 127U) >= 84U) {
            error = 13; result = -1;
        } else {
            stars_put_word(d, 0x750U, version);
            if (d[0x4ba6U] == 5U) {
                stars_read_record(d, stars_runtime_read, r);
                if ((stars_word(d, 0x2696U) >> 10) == 6U) {
                    error = 3;
                    stars_read_player_record(d, r->strings, player, 0x4b98U);
                    stars_read_record(d, stars_runtime_read, r);
                    if (!(stars_word(d, 0x2696U) >> 10) &&
                            stars_word(d, 0x4b98U) == stars_race_checksum(d, player)) {
                        salt = stars_dword(d, 0x35cU);
                        stars_put_dword(d, 0x35cU, stars_dword(d, (StarsU16)(player + 12U)));
                        if (!check || stars_check_password(r)) {
                            stars_put_dword(d, 0x35cU, salt);
                            if (stars_dword(d, (StarsU16)(player + 12U)))
                                strcpy((char *)d + 0x1308U, (char *)d + 0x22e0U);
                            else d[0x1308U] = 0;
                            memcpy(d + 0x4f9aU, d + player, 192U);
                            strcpy((char *)d + 0x1318U, (char *)d + file);
                            result = 1;
                        } else { stars_put_dword(d, 0x35cU, salt); result = -1; }
                    }
                }
            }
        }
    }
    stars_stream_close(d, &r->stream, r);
    stars_put_word(d, 0x6eU, old_env); stars_put_word(d, 0x74cU, silent);
    r->recovery = scope.previous; r->scratch_top = top;
    if (result != 1 && !silent && error != -1) {
        strcpy((char *)d + 0x57a4U, (char *)d + file);
        stars_runtime_alert(r, stars_format_id(r, error, 0), 16);
    }
    return result;
}
