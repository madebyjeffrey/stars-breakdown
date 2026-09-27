#include "core.h"

static StarsI16 signed_byte(StarsU16 value)
{
    value &= 0xffU;
    return value < 128U ? (StarsI16)value : (StarsI16)value - 256;
}

/* NE 11:9166-928C. Rebuild the fleet-component list from the selected
 * component records. The formatter callback is the original WSPRINTF/string
 * service; the fixed-point scaling and marker choice remain ordinary C. */
void stars_fill_fleet_comp_lb(StarsFillFleetCompInput *input)
{
    StarsU16 i;
    StarsU16 low;
    StarsU16 high;
    StarsU16 marker;
    StarsI16 value;
    StarsI16 scaled;
    StarsU8 text[128];

    if (!input || !input->send_message || !input->format_entry) return;
    input->send_message(input->context, input->hwnd_list, 0x0405U,
        (const StarsU8 *)0);
    for (i = 0; i < 16U; ++i) {
        if (input->components[i].quantity <= 0) continue;
        low = input->components[i].packed_mass & 0x007fU;
        high = input->components[i].packed_mass >> 7;
        scaled = (StarsI16)((low * high + 250U) / 500U);
        if (scaled == 0) {
            marker = (StarsU16)'Q';
            value = 0x20;
        } else {
            marker = (StarsU16)'P';
            value = signed_byte((StarsU16)scaled);
        }
        input->format_entry(input->context, i, marker, value,
            input->components[i].quantity, text, 128U);
        input->send_message(input->context, input->hwnd_list, 0x0401U, text);
    }
}
