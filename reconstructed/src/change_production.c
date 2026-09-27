#include "core.h"

/* Genuine C reconstruction of ChangeProduction, NE 1B:0000-015D. */
StarsI16 stars_change_production(StarsChangeProductionInput *input)
{
    StarsI16 result;
    const StarsU8 *message;

    if (!input)
        return 0;
    input->current_environment = input->product_buffer;
    if (input->exception != 0) {
        if (input->global_product != 0) {
            if (input->ops && input->ops->free_product)
                input->ops->free_product(input->context, input->global_product);
            input->global_product = 0;
        }
        if (input->dialog_window != 0) {
            if (input->ops && input->ops->end_dialog)
                input->ops->end_dialog(input->context, input->dialog_window, 0);
            input->dialog_window = 0;
        }
        input->dialog_up = 0;
        message = input->ops && input->ops->format_ids ?
            input->ops->format_ids(input->context, 0x20U) : 0;
        if (input->ops && input->ops->alert)
            input->ops->alert(input->context, message, 0x10);
        input->current_environment = input->saved_environment;
        return 0;
    }
    if (input->clear == 0) {
        if (input->ops && input->ops->initialize)
            input->ops->initialize(input->context, input->product_buffer,
                input->product_buffer_size);
        input->dialog_up = 1;
        if (input->ops && input->ops->dialog)
            result = input->ops->dialog(input->context, input->product_buffer,
                input->product_buffer_size);
        else
            result = 0;
        if (input->ops && input->ops->release_dialog_proc)
            input->ops->release_dialog_proc(input->context);
        input->dialog_window = 0;
        input->dialog_up = 0;
    }
    else {
        result = 1;
    }
    if (input->ops && input->ops->finish)
        input->ops->finish(input->context, result);
    if (result != 0 && input->selected_object == 1 && input->ops &&
        input->ops->draw_ship)
        input->ops->draw_ship(input->context, 0, 8);
    input->current_environment = input->saved_environment;
    return 1;
}
