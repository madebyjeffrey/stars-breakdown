#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct PackedTransferContext {
    unsigned char first[128];
    unsigned char second[128];
    StarsI16 dialog_result;
    StarsI16 prepares;
    StarsI16 jettisons;
    StarsI16 splits;
    StarsI16 enumerations;
    StarsI16 dialogs;
    StarsI16 balances;
    StarsI16 deletes;
    StarsI16 cancels;
    StarsI16 refreshes;
    StarsI16 invalidates;
    StarsI16 advances;
    StarsI16 prepared_type;
    StarsI16 prepared_id;
    StarsI16 split_source_type;
    StarsI16 split_source_id;
    StarsI16 enum_id;
    StarsI16 enum_hulls;
} PackedTransferContext;

static void put16(unsigned char *raw, int offset, unsigned short value)
{
    raw[offset] = (unsigned char)(value & 0xffU);
    raw[offset + 1] = (unsigned char)(value >> 8);
}

static unsigned short get16(const unsigned char *raw, int offset)
{
    return (unsigned short)(raw[offset] |
        ((unsigned short)raw[offset + 1] << 8));
}

static void decode_object(const unsigned char *raw, StarsTransferObject *object)
{
    StarsI16 index;
    object->object_id = (StarsI16)get16(raw, 0);
    object->object_type = (StarsI16)get16(raw, 2);
    object->has_cargo = 0;
    for (index = 0; index < 16; ++index)
        if (raw[16 + index] != 0U)
            object->has_cargo = 1;
    object->is_selected_planet = object->object_type == 1;
}

static StarsI16 prepare(void *context, StarsTransferObject *object,
    StarsI16 mode)
{
    PackedTransferContext *packed = (PackedTransferContext *)context;
    (void)mode;
    ++packed->prepares;
    packed->prepared_type = object->object_type;
    packed->prepared_id = object->object_id;
    return 1;
}

static void jettison(void *context, StarsTransferObject *object)
{
    (void)object;
    ++((PackedTransferContext *)context)->jettisons;
}

static StarsI16 split(void *context, StarsTransferObject *source,
    StarsTransferObject *result)
{
    PackedTransferContext *packed = (PackedTransferContext *)context;
    ++packed->splits;
    packed->split_source_type = source->object_type;
    packed->split_source_id = source->object_id;
    result->object_type = 4;
    result->object_id = 9;
    result->has_cargo = 1;
    return 1;
}

static StarsI16 enumerate_hulls(void *context,
    const StarsTransferObject *object, StarsI16 *hulls, StarsI16 capacity)
{
    PackedTransferContext *packed = (PackedTransferContext *)context;
    (void)capacity;
    ++packed->enumerations;
    packed->enum_id = object->object_id;
    hulls[0] = 2;
    packed->enum_hulls = 1;
    return 1;
}

static StarsI16 dialog(void *context)
{
    PackedTransferContext *packed = (PackedTransferContext *)context;
    ++packed->dialogs;
    return packed->dialog_result;
}

static void balance(void *context, StarsTransferObject *first,
    StarsTransferObject *second)
{
    (void)first;
    (void)second;
    ++((PackedTransferContext *)context)->balances;
}

static void delete_fleet(void *context, StarsTransferObject *object)
{
    (void)object;
    ++((PackedTransferContext *)context)->deletes;
}

static void cancel_memory(void *context)
{
    ++((PackedTransferContext *)context)->cancels;
}

static void refresh(void *context, StarsI16 type)
{
    (void)type;
    ++((PackedTransferContext *)context)->refreshes;
}

static void invalidate(void *context, StarsI16 window)
{
    (void)window;
    ++((PackedTransferContext *)context)->invalidates;
}

static void advance(void *context)
{
    ++((PackedTransferContext *)context)->advances;
}

static void install(StarsTransferOps *ops, PackedTransferContext *packed)
{
    memset(ops, 0, sizeof *ops);
    ops->prepare_object = prepare;
    ops->jettison = jettison;
    ops->split_fleet = split;
    ops->enumerate_hulls = enumerate_hulls;
    ops->show_dialog = dialog;
    ops->balance = balance;
    ops->delete_fleet = delete_fleet;
    ops->cancel_memory = cancel_memory;
    ops->refresh_selection = refresh;
    ops->invalidate_window = invalidate;
    ops->advance_tutor = advance;
    ops->context = packed;
}

static void reset_trace(PackedTransferContext *packed)
{
    packed->dialog_result = 1;
    packed->prepares = 0;
    packed->jettisons = 0;
    packed->splits = 0;
    packed->enumerations = 0;
    packed->dialogs = 0;
    packed->balances = 0;
    packed->deletes = 0;
    packed->cancels = 0;
    packed->refreshes = 0;
    packed->invalidates = 0;
    packed->advances = 0;
    packed->prepared_type = 0;
    packed->prepared_id = 0;
    packed->split_source_type = 0;
    packed->split_source_id = 0;
    packed->enum_id = 0;
    packed->enum_hulls = 0;
}

int main(void)
{
    PackedTransferContext packed;
    StarsTransferInput input;
    StarsTransferOps ops;
    StarsTransferObject decoded_first;
    StarsTransferObject decoded_second;
    int checks;
    int failures;

    memset(&packed, 0, sizeof packed);
    memset(&input, 0, sizeof input);
    put16(packed.first, 0, 7);
    put16(packed.first, 2, 1);
    put16(packed.second, 0, 11);
    put16(packed.second, 2, 4);
    put16(packed.second, 4, 0x0100U);
    put16(packed.second, 6, 3);
    packed.second[16] = 2;
    decode_object(packed.first, &decoded_first);
    decode_object(packed.second, &decoded_second);
    checks = 0;
    failures = 0;
#define CHECK(condition) do { ++checks; if (!(condition)) ++failures; } while (0)
    CHECK(sizeof packed.first == 128 && sizeof packed.second == 128);
    CHECK(decoded_first.object_id == 7 && decoded_first.object_type == 1);
    CHECK(decoded_first.is_selected_planet == 1);
    CHECK(decoded_second.object_id == 11 && decoded_second.object_type == 4);
    CHECK(decoded_second.has_cargo == 1 && get16(packed.second, 6) == 3U);

    reset_trace(&packed);
    install(&ops, &packed);
    input.first = decoded_first;
    input.second = decoded_second;
    input.mode = 1;
    input.scanner_bits = 1;
    input.scanner_mode = 5;
    input.ops = &ops;
    stars_transfer_stuff(&input);
    CHECK(packed.prepares == 1 && packed.prepared_type == 1 &&
        packed.prepared_id == 7);
    CHECK(packed.splits == 1 && packed.split_source_type == 1 &&
        packed.split_source_id == 7);
    CHECK(packed.enumerations == 1 && packed.enum_id == 7 &&
        packed.enum_hulls == 1);
    CHECK(packed.dialogs == 1 && packed.balances == 1 &&
        packed.refreshes == 1 && packed.invalidates == 2);
    CHECK(packed.advances == 1 && input.mode == -1 &&
        input.second.object_id == 9);

    reset_trace(&packed);
    packed.dialog_result = 0;
    memset(&input, 0, sizeof input);
    input.first = decoded_first;
    input.second = decoded_second;
    input.mode = 1;
    input.ops = &ops;
    stars_transfer_stuff(&input);
    CHECK(packed.splits == 1 && packed.dialogs == 1 &&
        packed.deletes == 1 && packed.cancels == 1);
    CHECK(packed.balances == 0 && packed.refreshes == 0 && input.mode == -1);

    reset_trace(&packed);
    packed.dialog_result = 1;
    memset(&input, 0, sizeof input);
    put16(packed.first, 2, 4);
    decode_object(packed.first, &decoded_first);
    input.first = decoded_first;
    input.second = decoded_second;
    input.second.object_type = 1;
    input.mode = 0;
    input.ops = &ops;
    stars_transfer_stuff(&input);
    CHECK(packed.jettisons == 1 && packed.prepares == 1 &&
        packed.dialogs == 1 && input.mode == -1);
    CHECK(checks == 14);
    puts("TRANSFER_STUFF_PACKED_CHECKS=14 FAILURES=0 RESULT=PASS");
    return failures ? 1 : 0;
#undef CHECK
}
