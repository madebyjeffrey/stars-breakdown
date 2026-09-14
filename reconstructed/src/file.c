/* Filename creation and recovery scope; see file-creation.md. */
#include "core.h"
#include <setjmp.h>
#include <stdio.h>
#include <string.h>

/* NE 15:8CFE-8E15. Truncates szBase in place before writing szWork. */
void stars_set_file_name(StarsU8 *data, StarsU16 type, StarsI16 player)
{
    char *base = (char *)data + 0x56a2U, *work = (char *)data + 0x57a4U;
    char *dot = strrchr(base, '.'), *slash;
    StarsU16 number_word;
    StarsI16 number;
    int length, letter;
    if (dot) {
        slash = strrchr(base, '\\');
        if (!slash || slash < dot) *dot = 0;
    }
    length = sprintf(work, (const char *)data + 0xa0cU, base);
    if (type == 2U) strcat(work, (const char *)data + 0xa13U);
    else if (type == 1U || type == 3U || type == 4U) {
        letter = type == 1U ? 'x' : type == 4U ? 'h' : 'm';
        number_word = (StarsU16)((StarsU16)player + 1U);
        number = number_word < 32768U ? (StarsI16)number_word : (StarsI16)((StarsI32)number_word - 65536L);
        sprintf(work + length, (const char *)data + 0xa17U, letter, (int)number);
    } else strcat(work, (const char *)data + 0xa10U);
}

typedef struct StarsCreateScope {
    jmp_buf recovery;
    StarsU8 *data;
    StarsU16 environment;
    const StarsStreamApi *outer;
    void *context;
    StarsStreamApi api;
} StarsCreateScope;

static StarsI16 create_open(void *opaque, const StarsU8 *name, StarsU16 mode, StarsU16 *error)
{
    StarsCreateScope *scope = (StarsCreateScope *)opaque;
    return scope->outer->open(scope->context, name, mode, error);
}
static StarsU16 create_write(void *opaque, StarsI16 handle, const StarsU8 *input, StarsU16 count)
{
    StarsCreateScope *scope = (StarsCreateScope *)opaque;
    return scope->outer->write(scope->context, handle, input, count);
}
static StarsU32 create_ticks(void *opaque)
{
    StarsCreateScope *scope = (StarsCreateScope *)opaque;
    return scope->outer->ticks(scope->context);
}
static void create_file_error(void *opaque, StarsI16 id)
{
    StarsCreateScope *scope = (StarsCreateScope *)opaque;
    scope->outer->file_error(scope->context, id);
}
static void create_write_error(void *opaque)
{
    StarsCreateScope *scope = (StarsCreateScope *)opaque;
    scope->outer->write_error(scope->context);
}
static void create_abort(void *opaque, StarsU16 environment, StarsI16 value)
{
    StarsCreateScope *scope = (StarsCreateScope *)opaque;
    if (environment == scope->environment) longjmp(scope->recovery, value);
    scope->outer->abort(scope->context, environment, value);
}
static void create_transfer(void *opaque, const StarsU8 *input, StarsU16 count)
{
    StarsCreateScope *scope = (StarsCreateScope *)opaque;
    stars_stream_write(scope->data, input, count, &scope->api, scope);
}

/* NE 15:8E16-8EA4. Retains an open handle after a header write failure. */
StarsI16 stars_create_file(StarsU8 *data, StarsU16 type, StarsI16 player,
    StarsU16 forced_name, StarsU16 environment, const StarsStreamApi *api, void *context)
{
    StarsCreateScope scope;
    StarsU16 saved;
    StarsI16 signed_type;
    if (!forced_name) {
        stars_set_file_name(data, type, player);
        forced_name = 0x57a4U;
    }
    saved = (StarsU16)(data[0x6eU] | ((StarsU16)data[0x6fU] << 8));
    scope.data = data; scope.environment = environment;
    scope.outer = api; scope.context = context;
    memset(&scope.api, 0, sizeof(scope.api));
    scope.api.open = create_open; scope.api.write = create_write;
    scope.api.ticks = create_ticks; scope.api.file_error = create_file_error;
    scope.api.write_error = create_write_error; scope.api.abort = create_abort;
    data[0x6eU] = (StarsU8)environment; data[0x6fU] = (StarsU8)(environment >> 8);
    if (setjmp(scope.recovery)) {
        data[0x6eU] = (StarsU8)saved; data[0x6fU] = (StarsU8)(saved >> 8);
        return 0;
    }
    stars_stream_open(data, forced_name, 0x1012, &scope.api, &scope);
    signed_type = type < 32768U ? (StarsI16)type : (StarsI16)((StarsI32)type - 65536L);
    stars_write_bof(data, player, signed_type, 0, create_ticks, create_transfer, &scope);
    data[0x6eU] = (StarsU8)saved; data[0x6fU] = (StarsU8)(saved >> 8);
    return 1;
}
