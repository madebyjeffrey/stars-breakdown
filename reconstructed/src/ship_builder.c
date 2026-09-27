#include "core.h"

/* NE 26:008C-015F. Prepare the slot dialog geometry, run the dialog, and
 * refresh planet production when the selected object is a planet. */
StarsI16 stars_ship_builder(StarsShipBuilderInput *input)
{
    void *proc_instance;
    void *selected_production;

    if (!input) return 0;
    input->dialog_size.y = (StarsI16)(input->dialog_size.y +
        (((input->display_flags >> 14) & 1U) != 0U ?
            input->dialog_font_height * 3 : 0));
    input->ship_definition = input->nth_valid_shdef ?
        input->nth_valid_shdef(input->context, 0U) : (void *)0;
    proc_instance = input->make_proc_instance ?
        input->make_proc_instance(input->context, input->instance) : (void *)0;
    if (input->dialog_box)
        input->dialog_box(input->context, input->instance, 0x005cU,
            input->frame_window, proc_instance);
    if (input->free_proc_instance)
        input->free_proc_instance(input->context, proc_instance);
    selected_production = (void *)input->selected_production;
    if (input->selected_object_type == 1U && selected_production &&
        input->fill_production)
        input->fill_production(input->context, input->production_list,
            selected_production, (const void *)0);
    return 0;
}
