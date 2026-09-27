#include "core.h"

static StarsU16 production_item_type(StarsU32 packed)
{
    return (StarsU16)((packed >> 17) & 7UL);
}

static StarsU16 production_item_number(StarsU32 packed)
{
    return (StarsU16)((packed >> 10) & 0x7fUL);
}

/* Genuine C reconstruction of InitializeProductionDlg, NE 1B:3430-35DC. */
void stars_initialize_production_dlg(StarsU16 dialog,
    const StarsInitializeProductionDlgInput *input)
{
    StarsU8 title[88];
    const StarsU8 *name;
    const StarsU8 *format;
    StarsU16 source_list;
    StarsU16 planet_list;
    StarsU16 research_control;
    StarsI16 selected;
    StarsI16 index;
    StarsU32 item;

    if (!input || !input->global_queue || !input->selected_planet ||
        !input->planet_name || !input->compressed_string ||
        !input->format_title || !input->fill_source ||
        !input->fill_planet || !input->set_window_text ||
        !input->get_dialog_item || !input->send_message)
        return;

    selected = -1;
    name = input->planet_name(input->context, input->selected_planet->id);
    format = input->compressed_string(input->context, 0x033eU);
    input->format_title(input->context, title, format, name);
    input->set_window_text(input->context, dialog, title);

    source_list = input->get_dialog_item(input->context, dialog, 0x0416U);
    input->fill_source(input->context, source_list, -1);
    source_list = input->get_dialog_item(input->context, dialog, 0x0416U);
    input->send_message(input->context, source_list, 0x0407U, 0, 0UL);

    for (index = 0; index < (StarsI16)input->global_queue->item_count;
        ++index) {
        item = input->global_queue->items[index];
        if (production_item_type(item) != 1U ||
            production_item_number(item) > 6U)
            selected = index;
    }

    planet_list = input->get_dialog_item(input->context, dialog, 0x0417U);
    input->fill_planet(input->context, planet_list, input->global_queue,
        (const StarsProductionPlanet *)0);
    planet_list = input->get_dialog_item(input->context, dialog, 0x0417U);
    input->send_message(input->context, planet_list, 0x0407U,
        (StarsU16)(selected + 1), 0UL);

    research_control = input->get_dialog_item(input->context, dialog, 0x008bU);
    input->send_message(input->context, research_control, 0x0401U,
        (StarsU16)((input->selected_planet->flags >> 23) & 1UL), 0UL);
}
