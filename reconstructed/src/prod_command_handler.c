#include "core.h"

#define PROD_CMD_CLOSE 1
#define PROD_CMD_HELP 0x76
#define PROD_CMD_RESEARCH 0x8b
#define PROD_CMD_ADD 0x418
#define PROD_CMD_CLEAR 0x42d
#define PROD_CMD_NEXT 0x42e
#define PROD_CMD_PREVIOUS 0x42f
#define PROD_CMD_MOVE_UP 0x439
#define PROD_CMD_MOVE_DOWN 0x43a
#define PROD_CMD_IMPORT 0x816
#define PROD_ITEM_COUNT_MASK 0x3ffUL
#define PROD_MAX_ITEMS 128

static StarsU16 prod_item_type(StarsU32 item)
{ return (StarsU16)((item >> 17) & 7UL); }

static StarsU16 prod_item_number(StarsU32 item)
{ return (StarsU16)((item >> 10) & 0x7fUL); }

static StarsU32 prod_item_set_count(StarsU32 item, StarsU16 count)
{ return (item & ~PROD_ITEM_COUNT_MASK) | (StarsU32)(count & 0x3ff); }

static void prod_refresh(StarsProdCommandInput *input)
{
    const StarsProdCommandOps *ops;

    ops = input->ops;
    if (ops->fill_queue)
        ops->fill_queue(ops->context, input->dialog, input->queue);
    if (ops->draw)
        ops->draw(ops->context, input->dialog);
    if (ops->invalidate)
        ops->invalidate(ops->context, input->dialog);
}

static StarsU16 prod_step(const StarsProdCommandInput *input)
{
    if (input->shift_down && input->control_down) return 1020;
    if (input->shift_down) return 100;
    if (input->control_down) return 10;
    return 1;
}

static void prod_remove(StarsProductionList *queue, StarsI16 index)
{
    StarsI16 move;

    if (index < 0 || index >= (StarsI16)queue->item_count)
        return;
    for (move = index; move + 1 < (StarsI16)queue->item_count; ++move)
        queue->items[move] = queue->items[move + 1];
    --queue->item_count;
}

static void prod_swap(StarsProductionList *queue, StarsI16 first,
    StarsI16 second)
{
    StarsU32 item;

    if (first < 0 || second < 0 || first >= (StarsI16)queue->item_count ||
            second >= (StarsI16)queue->item_count)
        return;
    item = queue->items[first];
    queue->items[first] = queue->items[second];
    queue->items[second] = item;
}

static void prod_add(StarsProductionList *queue, StarsU32 item,
    StarsI16 index)
{
    StarsI16 move;

    if (queue->item_count >= PROD_MAX_ITEMS)
        return;
    if (index < 0) index = (StarsI16)queue->item_count;
    if (index > (StarsI16)queue->item_count)
        index = (StarsI16)queue->item_count;
    for (move = (StarsI16)queue->item_count; move > index; --move)
        queue->items[move] = queue->items[move - 1];
    queue->items[index] = item;
    ++queue->item_count;
}

void stars_prod_command_handler(StarsProdCommandInput *input)
{
    const StarsProdCommandOps *ops;
    StarsI16 selected;
    StarsI16 next;
    StarsU16 step;
    StarsU16 count;

    if (!input || !input->queue || !input->ops)
        return;
    ops = input->ops;
    if (input->message == PROD_CMD_CLOSE) {
        if (ops->sticky) ops->sticky(ops->context, input->dialog);
        if (ops->finish) ops->finish(ops->context, 1);
        if (ops->end_dialog) ops->end_dialog(ops->context, input->dialog, 1);
        return;
    }
    if (input->message == 2) {
        if (ops->sticky) ops->sticky(ops->context, input->dialog);
        if (ops->finish) ops->finish(ops->context, 0);
        if (ops->end_dialog) ops->end_dialog(ops->context, input->dialog, 0);
        return;
    }
    if (input->message == PROD_CMD_HELP) {
        if (ops->help) ops->help(ops->context, input->dialog, 0x423);
        return;
    }
    if (input->message == PROD_CMD_RESEARCH) {
        if (ops->toggle_research)
            ops->toggle_research(ops->context, (StarsI16)(input->parameter & 1));
        prod_refresh(input);
        return;
    }
    if (input->message == PROD_CMD_NEXT || input->message == PROD_CMD_PREVIOUS) {
        if (ops->navigate)
            ops->navigate(ops->context,
                input->message == PROD_CMD_NEXT ? 1 : -1,
                input->shift_down);
        if (ops->initialize) ops->initialize(ops->context);
        prod_refresh(input);
        return;
    }
    if (input->message == PROD_CMD_MOVE_UP || input->message == PROD_CMD_MOVE_DOWN) {
        selected = input->selected_index;
        next = input->message == PROD_CMD_MOVE_UP ? selected - 1 : selected + 1;
        prod_swap(input->queue, selected, next);
        if (ops->select_item) ops->select_item(ops->context, next, 1);
        prod_refresh(input);
        return;
    }
    if (input->message == PROD_CMD_CLEAR) {
        input->queue->item_count = 0;
        prod_refresh(input);
        return;
    }
    if (input->message == PROD_CMD_IMPORT) {
        input->queue->item_count = 0;
        if (input->catalog) {
            for (selected = 0; selected < (StarsI16)input->catalog->item_count &&
                    selected < PROD_MAX_ITEMS; ++selected)
                if (prod_item_type(input->catalog->items[selected]) != 1 ||
                        prod_item_number(input->catalog->items[selected]) > 6)
                    prod_add(input->queue, input->catalog->items[selected], -1);
        }
        prod_refresh(input);
        return;
    }
    selected = input->selected_index;
    if (input->message == PROD_CMD_ADD && input->catalog &&
            input->parameter < input->catalog->item_count) {
        prod_add(input->queue, input->catalog->items[input->parameter], selected);
        if (ops->select_item)
            ops->select_item(ops->context, selected + 1, 1);
        prod_refresh(input);
        return;
    }
    if (selected < 0 || selected >= (StarsI16)input->queue->item_count)
        return;
    step = prod_step(input);
    count = (StarsU16)(input->queue->items[selected] & PROD_ITEM_COUNT_MASK);
    if (input->parameter == 0) {
        if (count > step) count = (StarsU16)(count - step);
        else count = 0;
    } else {
        count = count + step > 0x3fc ? 0x3fc : (StarsU16)(count + step);
    }
    input->queue->items[selected] = prod_item_set_count(
        input->queue->items[selected], count);
    if (count == 0)
        prod_remove(input->queue, selected);
    prod_refresh(input);
}
