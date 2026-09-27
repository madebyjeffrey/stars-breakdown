#include "core.h"

/* Genuine C reconstruction of FillZipProdLB, NE 1B:5E58-5FF4. */
void stars_fill_zip_prod_lb(StarsU16 dialog,
    const StarsZipProductionQueue *queue, const StarsFillZipProdInput *input)
{
    StarsU16 list;
    StarsRect rect;
    StarsU8 template_text[16];
    StarsU8 item_text[40];
    StarsU8 work[40];
    StarsI16 index;
    StarsU16 item;
    StarsU16 code;
    StarsU16 kind;
    const StarsU8 *message;

    if (!queue || !input || !input->get_dialog_item ||
        !input->get_client_rect || !input->invalidate_rect ||
        !input->send_message)
        return;
    list = input->get_dialog_item(input->context, dialog, 0x0417U);
    rect.left = rect.top = rect.right = rect.bottom = 0;
    input->get_client_rect(input->context, dialog, &rect);
    rect.top = (StarsU16)input->static_y;
    rect.bottom = (StarsU16)(input->static_y + input->line_height);
    input->invalidate_rect(input->context, dialog, &rect, 1);
    input->send_message(input->context, list, 0x0405U, 0, (const StarsU8 *)0);
    if (queue->valid == 0U || queue->item_count == 0U) {
        message = input->compressed_string ?
            input->compressed_string(input->context, 0x04c4U) :
            (const StarsU8 *)0;
        input->send_message(input->context, list, 0x0401U, 0, message);
        return;
    }
    if (input->get_string)
        input->get_string(input->context, 0x04c5U, template_text,
            (StarsU16)sizeof template_text);
    for (index = 0; index < (StarsI16)queue->item_count; ++index) {
        item = queue->items[index];
        code = item & 0x003fU;
        kind = (item >> 6) & 0x03ffU;
        if (input->get_string)
            input->get_string(input->context, (StarsU16)(0x007eU + code),
                item_text, (StarsU16)sizeof item_text);
        if (kind == 1U || code == 3U) {
            if (input->copy_string)
                input->copy_string(input->context, work, item_text);
        } else if (input->format_string) {
            input->format_string(input->context, work, template_text,
                item_text, kind);
        }
        input->send_message(input->context, list, 0x0401U, 0, work);
    }
}
