#include "core.h"

/* Genuine C reconstruction of FinishProduction, NE 1B:1090-1204. */
void stars_finish_production(StarsFinishProductionInput *input)
{
    StarsU32 bit;
    StarsU8 *old_global;

    if (!input)
        return;
    if (input->write == 0) {
        bit = (input->game_flags >> 5) & 1UL;
        input->planet_bits = (input->planet_bits & ~0x00800000UL) | (bit << 23);
        if (input->ops && input->ops->free_product)
            input->ops->free_product(input->context, input->global_product);
    }
    else {
        if (input->ops && input->ops->free_product)
            input->ops->free_product(input->context, input->selected_product);
        old_global = input->global_product;
        if (old_global != 0 && input->global_count == 0) {
            if (input->ops && input->ops->free_product)
                input->ops->free_product(input->context, old_global);
            input->global_product = 0;
        }
        input->selected_product = input->global_product;
        input->global_product = 0;
        if (input->ops && input->ops->lookup_planet) {
            input->ops->lookup_planet(input->context, -1, input->selected_product);
            input->ops->lookup_planet(input->context, input->planet_id,
                input->selected_product);
        }
        if (!input->ai_active && input->ops) {
            if (input->ops->fill_list)
                input->ops->fill_list(input->context);
            if (input->ops->draw_ship)
                input->ops->draw_ship(input->context, 0, 0x40U);
        }
    }
    input->global_product = 0;
    if ((input->game_flags & 0x0800U) != 0U && input->player == 0) {
        input->tutor_flags = (StarsU16)(input->tutor_flags | 0x0400U);
        if (input->ops && input->ops->advance_tutor)
            input->ops->advance_tutor(input->context);
    }
}
