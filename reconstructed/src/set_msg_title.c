#include "core.h"

enum {
    SET_MSG_TITLE_VISIBLE_CHILD = 5,
    SET_MSG_TITLE_HIDDEN_CHILD = 0,
    SET_MSG_TITLE_OBJECT_MASK = 0x3fff,
    SET_MSG_TITLE_ID_MASK = 0x7fff,
    SET_MSG_TITLE_POSITION_DROPDOWN_X = 0x1e,
    SET_MSG_TITLE_POSITION_DROPDOWN_WIDTH_REDUCTION = 0x54,
    SET_MSG_TITLE_POSITION_BUTTON_X = 0x32,
    SET_MSG_TITLE_POSITION_EDIT_Y_EXTRA = 3,
    SET_MSG_TITLE_POSITION_EDIT_WIDTH_REDUCTION = 0,
    SET_MSG_TITLE_POSITION_FLAGS_EDIT = 0x44,
    SET_MSG_TITLE_POSITION_FLAGS_DROPDOWN = 0x44,
    SET_MSG_TITLE_POSITION_FLAGS_BUTTON = 0x45
};

#define SET_MSG_TITLE_FILTER_MASK 0xc000U

static void set_msg_show(const StarsSetMsgTitleInput *input, StarsU16 control,
    StarsI16 shown)
{
    if (input->ops->show_control)
        input->ops->show_control(input->context, control, shown);
}

static void set_msg_enable(const StarsSetMsgTitleInput *input, StarsU16 control,
    StarsI16 enabled)
{
    if (input->ops->enable_control)
        input->ops->enable_control(input->context, control, enabled);
}

static StarsI16 set_msg_filtered(const StarsSetMsgTitleInput *input,
    StarsI16 message_id)
{
    StarsU16 id;
    if (message_id < 0) return 0;
    id = (StarsU16)message_id;
    if (!input->filtered_bits || (id >> 3) >= input->filtered_bit_bytes)
        return 0;
    return (StarsI16)((input->filtered_bits[id >> 3] >> (id & 7U)) & 1U);
}

static StarsI16 set_msg_object_from_goto(const StarsSetMsgTitleInput *input,
    const StarsSetMsgIncomingView *view, StarsI16 *object, StarsI16 *id,
    StarsI16 *x, StarsI16 *y)
{
    StarsI16 signed_goto;
    *object = 0; *id = 0; *x = 0; *y = 0;
    if (view->goto_word == STARS_SET_MSG_TITLE_GOTO_NONE)
        return 0;
    *id = (StarsI16)(view->goto_word & SET_MSG_TITLE_ID_MASK);
    if (view->goto_word == STARS_SET_MSG_TITLE_GOTO_SPECIAL) {
        *object = STARS_SET_MSG_TITLE_OBJECT_SPECIAL;
    } else if (view->goto_word == STARS_SET_MSG_TITLE_GOTO_PLANETARY) {
        *object = STARS_SET_MSG_TITLE_OBJECT_PLANETARY;
    } else if (view->goto_word == STARS_SET_MSG_TITLE_GOTO_THING) {
        *object = STARS_SET_MSG_TITLE_OBJECT_THING;
    } else if (view->goto_word == STARS_SET_MSG_TITLE_GOTO_OWNER) {
        *object = STARS_SET_MSG_TITLE_OBJECT_OWNER;
    } else if (view->goto_word == STARS_SET_MSG_TITLE_GOTO_POINT) {
        *object = STARS_SET_MSG_TITLE_OBJECT_POINT; *x = view->parameter0;
    } else if (view->goto_word == STARS_SET_MSG_TITLE_GOTO_HOME) {
        *object = STARS_SET_MSG_TITLE_OBJECT_HOME;
    } else if ((view->goto_word & SET_MSG_TITLE_FILTER_MASK) ==
            SET_MSG_TITLE_FILTER_MASK) {
        *object = STARS_SET_MSG_TITLE_OBJECT_BATTLE;
    } else if ((view->goto_word & 0x4000U) == 0U) {
        signed_goto = (StarsI16)view->goto_word;
        if (signed_goto < 0) {
            if (input->ops->fleet_exists && input->ops->fleet_exists(
                    input->context, *id))
                *object = STARS_SET_MSG_TITLE_OBJECT_FLEET;
        } else {
            *object = STARS_SET_MSG_TITLE_OBJECT_PLANET;
        }
    } else {
        *id = (StarsI16)(view->goto_word & SET_MSG_TITLE_OBJECT_MASK);
        if (*id == STARS_SET_MSG_TITLE_GOTO_BATTLE_ID) {
            *object = STARS_SET_MSG_TITLE_OBJECT_MASS;
        } else {
            *object = STARS_SET_MSG_TITLE_OBJECT_THING;
            *x = view->parameter0; *y = view->parameter1;
        }
    }
    return (StarsI16)(*object != 0);
}

/* Genuine semantic reconstruction of SetMsgTitle, NE 7:7218-7808.  The
 * message window has two layouts.  In outgoing mode this routine restores
 * the selected recipient and editor text and enables message-navigation
 * controls; in incoming mode it formats the turn/count title and determines
 * whether the current message has a navigable object target. */
void stars_set_msg_title(StarsSetMsgTitleInput *input)
{
    const StarsSetMsgTitleOps *ops;
    StarsSetMsgOutgoingView outgoing;
    StarsSetMsgIncomingView incoming;
    StarsI16 object, object_id, object_x, object_y;
    StarsI16 message_id;
    StarsI16 target_enabled;
    StarsI16 shown;
    StarsU16 i;
    StarsU8 title[128];
    StarsU8 editor[STARS_FINISH_MESSAGE_PAYLOAD_CAPACITY];

    if (!input || !input->ops || input->hwnd == 0 || input->ai_active)
        return;
    ops = input->ops;
    shown = input->send_mode ? SET_MSG_TITLE_VISIBLE_CHILD :
        SET_MSG_TITLE_HIDDEN_CHILD;
    set_msg_show(input, input->edit_control, shown);
    set_msg_show(input, input->dropdown_control, shown);
    set_msg_show(input, input->controls[STARS_SET_MSG_TITLE_BUTTON_DROPDOWN],
        shown);
    if (input->send_mode) set_msg_show(input, input->scroll_control, 0);

    if (input->send_mode) {
        if (ops->format_title) ops->format_title(input->context,
            STARS_SET_MSG_TITLE_OUTGOING_COUNT, input->turn, input->dirty,
            input->current_outgoing + 1, input->outgoing_count, title,
            (StarsU16)sizeof title);
        if (ops->set_control_text) ops->set_control_text(input->context,
            input->controls[STARS_SET_MSG_TITLE_BUTTON_ACTION], title);
        set_msg_enable(input, input->controls[STARS_SET_MSG_TITLE_BUTTON_PREVIOUS],
            input->current_outgoing > 0);
        set_msg_enable(input, input->controls[STARS_SET_MSG_TITLE_BUTTON_ACTION], 1);
        set_msg_enable(input, input->controls[STARS_SET_MSG_TITLE_BUTTON_NEXT], 1);
        if (ops->set_position) {
            ops->set_position(input->context, input->dropdown_control,
                (StarsI16)(input->message_text_rect.left +
                SET_MSG_TITLE_POSITION_DROPDOWN_X), input->message_text_rect.top,
                (StarsI16)(input->message_text_rect.right -
                input->message_text_rect.left -
                SET_MSG_TITLE_POSITION_DROPDOWN_WIDTH_REDUCTION),
                (StarsI16)(input->message_text_rect.bottom -
                input->message_text_rect.top),
                SET_MSG_TITLE_POSITION_FLAGS_DROPDOWN);
            ops->set_position(input->context,
                input->controls[STARS_SET_MSG_TITLE_BUTTON_DROPDOWN],
                (StarsI16)(input->message_text_rect.right -
                SET_MSG_TITLE_POSITION_BUTTON_X), input->message_text_rect.top,
                0, 0, SET_MSG_TITLE_POSITION_FLAGS_BUTTON);
            ops->set_position(input->context, input->edit_control,
                input->message_text_rect.left,
                (StarsI16)(input->message_text_rect.top +
                input->ship_dropdown_height + SET_MSG_TITLE_POSITION_EDIT_Y_EXTRA),
                (StarsI16)(input->message_text_rect.right -
                input->message_text_rect.left),
                (StarsI16)(input->message_text_rect.bottom -
                input->message_text_rect.top), SET_MSG_TITLE_POSITION_FLAGS_EDIT);
        }
        if (!ops->get_outgoing || !ops->get_outgoing(input->context,
                input->current_outgoing, &outgoing)) {
            if (ops->set_recipient)
                ops->set_recipient(input->context, input->incoming_recipient);
            editor[0] = 0;
        } else {
            if (ops->set_recipient)
                ops->set_recipient(input->context, outgoing.recipient);
            if (outgoing.stored_length < 0) {
                for (i = 0U; i < STARS_FINISH_MESSAGE_PAYLOAD_CAPACITY - 1U &&
                        outgoing.payload && outgoing.payload[i] != 0U; ++i)
                    editor[i] = outgoing.payload[i];
                editor[i] = 0;
            } else if (ops->decode_text) {
                ops->decode_text(input->context, outgoing.payload,
                    outgoing.stored_length, editor, (StarsU16)sizeof editor);
            } else editor[0] = 0;
        }
        if (ops->set_editor) ops->set_editor(input->context, editor);
    } else {
        if (ops->format_title) ops->format_title(input->context,
            input->incoming_count == 0 ? STARS_SET_MSG_TITLE_INCOMING_EMPTY :
            STARS_SET_MSG_TITLE_INCOMING_LIST, input->turn, input->dirty,
            input->current_message + 1, input->incoming_count, title,
            (StarsU16)sizeof title);
        if (ops->set_title) ops->set_title(input->context, title);
        set_msg_enable(input, input->controls[STARS_SET_MSG_TITLE_BUTTON_PREVIOUS],
            input->current_message > 0);
        set_msg_enable(input, input->controls[STARS_SET_MSG_TITLE_BUTTON_NEXT],
            input->current_message >= 0 &&
            input->current_message + 1 < input->incoming_count);
        target_enabled = 0;
        if (input->current_message >= 0 && input->current_message <
                input->incoming_count && ops->get_incoming) {
            if (ops->get_incoming(input->context, input->current_message,
                    &incoming) && set_msg_object_from_goto(input, &incoming,
                    &object, &object_id, &object_x, &object_y)) {
                target_enabled = 1;
                if (input->message_view_filtered == 0 && ops->message_id) {
                    message_id = ops->message_id(input->context,
                        input->current_message);
                    if (set_msg_filtered(input, message_id))
                        target_enabled = 0;
                }
                if (target_enabled && ops->set_object)
                    ops->set_object(input->context, object, object_id,
                        object_x, object_y);
            }
        }
        set_msg_enable(input, input->controls[STARS_SET_MSG_TITLE_BUTTON_ACTION],
            target_enabled || input->current_message >= input->incoming_count);
        if (ops->invalidate) ops->invalidate(input->context);
    }
}
