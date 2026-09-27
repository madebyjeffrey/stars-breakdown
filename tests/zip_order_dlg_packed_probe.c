#include "core.h"
#include <stdio.h>
#include <string.h>

#define ZIP_RECORDS 4
#define ZIP_RECORD_SIZE 24
#define ZIP_WORDS 5
#define ZIP_NAME 10
#define ZIP_VALID 23
#define PLORD_WORDS 12

typedef struct PackedZipContext {
    StarsU8 zip[ZIP_RECORDS][ZIP_RECORD_SIZE];
    StarsU8 plord[32];
    StarsI16 copies;
    StarsI16 deletes;
    StarsI16 renames;
    StarsI16 sets;
    StarsI16 dialogs;
    StarsI16 last_result;
    StarsU8 last_text[64];
} PackedZipContext;

static void put16(StarsU8 *raw, StarsU16 offset, StarsU16 value)
{
    raw[offset] = (StarsU8)value;
    raw[offset + 1U] = (StarsU8)(value >> 8);
}

static StarsU16 get16(const StarsU8 *raw, StarsU16 offset)
{
    return (StarsU16)(raw[offset] |
        ((StarsU16)raw[offset + 1U] << 8));
}

static void seed(PackedZipContext *packed)
{
    StarsI16 index;
    StarsI16 word;
    memset(packed, 0, sizeof *packed);
    packed->zip[0][ZIP_VALID] = 1;
    packed->zip[0][ZIP_NAME] = 'O';
    packed->zip[0][ZIP_NAME + 1] = 'l';
    packed->zip[0][ZIP_NAME + 2] = 'd';
    packed->zip[0][ZIP_NAME + 3] = 0;
    for (word = 0; word < ZIP_WORDS; ++word)
        put16(packed->zip[0], (StarsU16)(word * 2),
            (StarsU16)(0x1000U + (StarsU16)word));
    for (index = 0; index < ZIP_WORDS; ++index)
        put16(packed->plord, (StarsU16)(PLORD_WORDS + index * 2),
            (StarsU16)(0x2200U + (StarsU16)index));
}

static void decode(const PackedZipContext *packed, StarsZipOrderDlgState *state)
{
    StarsI16 index;
    StarsI16 word;
    memset(state, 0, sizeof *state);
    for (index = 0; index < ZIP_RECORDS; ++index) {
        state->valid[index] = packed->zip[index][ZIP_VALID] != 0U;
        for (word = 0; word < ZIP_WORDS; ++word)
            state->words[index][word] = get16(packed->zip[index],
                (StarsU16)(word * 2));
        for (word = 0; word < 63 &&
                packed->zip[index][ZIP_NAME + word] != 0U; ++word)
            state->names[index][word] = packed->zip[index][ZIP_NAME + word];
        state->names[index][word] = 0;
    }
}

static StarsU16 item(void *context, StarsU16 hwnd, StarsU16 id)
{
    (void)context;
    (void)hwnd;
    return (StarsU16)(id + 100U);
}

static const StarsU8 *compressed(void *context, StarsU16 id)
{
    (void)context;
    (void)id;
    return (const StarsU8 *)"Zip Order";
}

static void show(void *context, StarsU16 hwnd, StarsI16 visible)
{
    (void)context; (void)hwnd; (void)visible;
}

static void radio(void *context, StarsU16 hwnd, StarsU16 first,
    StarsU16 last, StarsU16 checked)
{
    (void)context; (void)hwnd; (void)first; (void)last; (void)checked;
}

static void enable(void *context, StarsU16 hwnd, StarsI16 slot)
{
    (void)context; (void)hwnd; (void)slot;
}

static void invalidate(void *context, StarsU16 hwnd)
{
    (void)context; (void)hwnd;
}

static void set_text(void *context, StarsU16 item_handle,
    const StarsU8 *text)
{
    PackedZipContext *packed = (PackedZipContext *)context;
    StarsI16 slot;
    StarsI16 in;
    StarsI16 out;
    ++packed->sets;
    slot = (StarsI16)item_handle - 100 - 0x431;
    if (slot < 0 || slot >= ZIP_RECORDS)
        return;
    in = 0;
    out = 0;
    while (text && text[in] != 0U && out < 12) {
        packed->last_text[out] = text[in++];
        if (packed->last_text[out] == '&' && text[in] == '&')
            ++in;
        ++out;
    }
    packed->last_text[out] = 0;
    for (in = 0; in <= out; ++in)
        packed->zip[slot][ZIP_NAME + in] = packed->last_text[in];
}

static StarsI16 rename_order(void *context, StarsI16 slot, StarsU8 *text,
    StarsU16 is_rename)
{
    PackedZipContext *packed = (PackedZipContext *)context;
    (void)slot;
    (void)is_rename;
    ++packed->renames;
    text[0] = 'C';
    text[1] = '&';
    text[2] = 'D';
    text[3] = 0;
    return 1;
}

static void format_default(void *context, StarsU8 *text, StarsI16 number)
{
    (void)context;
    text[0] = 'Z';
    text[1] = 'i';
    text[2] = 'p';
    text[3] = ' ';
    text[4] = (StarsU8)('0' + number);
    text[5] = 0;
}

static void copy_order(void *context, StarsI16 slot)
{
    PackedZipContext *packed = (PackedZipContext *)context;
    StarsI16 word;
    ++packed->copies;
    packed->zip[slot][ZIP_VALID] = 1;
    for (word = 0; word < ZIP_WORDS; ++word)
        put16(packed->zip[slot], (StarsU16)(word * 2),
            get16(packed->plord, (StarsU16)(PLORD_WORDS + word * 2)));
}

static void delete_order(void *context, StarsI16 slot)
{
    PackedZipContext *packed = (PackedZipContext *)context;
    ++packed->deletes;
    packed->zip[slot][ZIP_VALID] = 0;
}

static void sticky(void *context, StarsU16 hwnd, StarsPoint *point,
    StarsI16 save)
{
    (void)context; (void)hwnd; (void)point; (void)save;
}

static void end_dialog(void *context, StarsU16 hwnd, StarsI16 result)
{
    PackedZipContext *packed = (PackedZipContext *)context;
    (void)hwnd;
    ++packed->dialogs;
    packed->last_result = result;
}

static void tutor(void *context)
{
    (void)context;
}

static void install(StarsZipOrderDlgOps *ops, PackedZipContext *packed)
{
    memset(ops, 0, sizeof *ops);
    ops->get_dialog_item = item;
    ops->compressed_string = compressed;
    ops->show_window = show;
    ops->check_radio_button = radio;
    ops->enable_buttons = enable;
    ops->invalidate = invalidate;
    ops->set_window_text = set_text;
    ops->rename_dialog = rename_order;
    ops->format_default = format_default;
    ops->copy_order = copy_order;
    ops->delete_order = delete_order;
    ops->sticky_position = sticky;
    ops->end_dialog = end_dialog;
    ops->advance_tutor = tutor;
    ops->context = packed;
}

int main(void)
{
    PackedZipContext packed;
    StarsZipOrderDlgState state;
    StarsZipOrderDlgOps ops;
    int checks;
    int failures;

    seed(&packed);
    decode(&packed, &state);
    install(&ops, &packed);
    state.tutor_active = 0;
    checks = 0;
    failures = 0;
#define CHECK(condition) do { ++checks; if (!(condition)) { ++failures; fprintf(stderr, "CHECK %d FAILED\\n", checks); } } while (0)
    CHECK(sizeof packed.zip[0] == 24 && sizeof packed.plord == 32);
    CHECK(state.valid[0] == 1 && state.names[0][0] == 'O' &&
        state.words[0][0] == 0x1000U);
    CHECK(stars_zip_order_dlg(&state, 10, 272, 0, 0, &ops) == 1 &&
        state.selected == 0 && state.frame_window == 10);
    CHECK(packed.sets >= 4 && packed.zip[0][ZIP_VALID] != 0U);

    CHECK(stars_zip_order_dlg(&state, 10, 273, 0x433, 0, &ops) == 0 &&
        state.selected == 2);
    CHECK(stars_zip_order_dlg(&state, 10, 273, 0x816, 0, &ops) == 1 &&
        state.valid[2] != 0U && packed.copies == 1);
    CHECK(packed.zip[2][ZIP_VALID] != 0U &&
        get16(packed.zip[2], 0) == 0x2200U &&
        get16(packed.zip[2], 8) == 0x2204U &&
        strcmp((const char *)&packed.zip[2][ZIP_NAME], "C&D") == 0);
    CHECK(strcmp((const char *)packed.last_text, "C&D") == 0);

    CHECK(stars_zip_order_dlg(&state, 10, 273, 0x817, 0, &ops) == 0 &&
        state.valid[2] == 0 && packed.deletes == 1 &&
        packed.zip[2][ZIP_VALID] == 0U);
    (void)stars_zip_order_dlg(&state, 10, 273, 0x431, 0, &ops);
    CHECK(stars_zip_order_dlg(&state, 10, 273, 0x41b, 0, &ops) == 1 &&
        packed.renames == 2 && strcmp((const char *)&packed.zip[0][ZIP_NAME],
        "C&D") == 0);
    CHECK(stars_zip_order_dlg(&state, 10, 273, 2, 0, &ops) == 1 &&
        state.frame_window == 0 && packed.dialogs == 1 &&
        packed.last_result == 0);
    if (checks != 11)
        ++failures;
    if (failures == 0)
        puts("ZIP_ORDER_DLG_PACKED_CHECKS=11 FAILURES=0 RESULT=PASS");
    else
        puts("ZIP_ORDER_DLG_PACKED_CHECKS=11 FAILURES=1 RESULT=FAIL");
    return failures ? 1 : 0;
#undef CHECK
}
