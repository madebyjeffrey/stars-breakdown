#include "core.h"

#define ZIP_INFO_HEADER_ID 0x0267
#define ZIP_INFO_LINE_FIRST 0x023f
#define ZIP_INFO_LINE_LAST 0x023c
#define ZIP_INFO_PAIR_ONE_BOLD 0x0238
#define ZIP_INFO_PAIR_ONE_NORMAL 0x0239
#define ZIP_INFO_PAIR_TWO_BOLD 0x023a
#define ZIP_INFO_PAIR_TWO_NORMAL 0x023b
#define ZIP_INFO_PAIR_THREE_BOLD 0x0236
#define ZIP_INFO_PAIR_THREE_NORMAL 0x0237
#define ZIP_INFO_PAIR_FOUR_BOLD 0x0234
#define ZIP_INFO_PAIR_FOUR_NORMAL 0x0235

static StarsI16 zip_info_text(const StarsZipOrderInfoInput *input,
    StarsU16 dc, StarsI16 *x, StarsI16 y, StarsI16 id, StarsI16 print)
{
    StarsU8 text[128];
    StarsI16 length = 0;
    text[0] = 0;
    if (input->ops->get_string)
        length = input->ops->get_string(input->context, id, text,
            (StarsU16)sizeof text);
    if (input->ops->stream_text)
        input->ops->stream_text(input->context, dc, x, y, text, 0, print);
    return length;
}

static void zip_info_pair(const StarsZipOrderInfoInput *input, StarsU16 dc,
    StarsI16 *x, StarsI16 y, StarsI16 bold_id, StarsI16 normal_id,
    StarsI16 print, StarsI16 *maximum)
{
    (void)zip_info_text(input, dc, x, y, bold_id, print);
    if (input->ops->select_font)
        input->ops->select_font(input->context, dc, input->font_normal);
    (void)zip_info_text(input, dc, x, y, normal_id, print);
    if (*maximum < *x)
        *maximum = *x;
}

/* NE 25:2D66-3110. Render the zip-order popup's fixed resource rows and
 * return the exact content extent used by its caller for window sizing. */
StarsPoint stars_pt_display_zip_ord_info(StarsU16 dc, StarsI16 x_center,
    StarsI16 print, const StarsZipOrderInfoInput *input)
{
    StarsPoint result;
    StarsI16 x = 4;
    StarsI16 y = 4;
    StarsI16 maximum = 4;
    StarsI16 length;
    StarsI16 id;
    StarsU8 header[128];

    result.x = 0; result.y = 0;
    if (!input || !input->ops)
        return result;
    if (input->ops->select_font)
        input->ops->select_font(input->context, dc, input->font_bold);
    if (print && input->ops->get_string && input->ops->center_text) {
        length = input->ops->get_string(input->context, ZIP_INFO_HEADER_ID,
            header, (StarsU16)sizeof header);
        input->ops->center_text(input->context, dc, x_center, y, header,
            length);
    }
    y = (StarsI16)(y + input->arial8_height);
    if (input->ops->select_font)
        input->ops->select_font(input->context, dc, input->font_normal);
    for (id = ZIP_INFO_LINE_FIRST; id >= ZIP_INFO_LINE_LAST; --id) {
        x = 4;
        (void)zip_info_text(input, dc, &x, y, id, print);
        if (maximum < x) maximum = x;
        y = (StarsI16)(y + input->arial8_height);
    }
    y = (StarsI16)(y + input->arial8_height);
    x = 4;
    if (input->ops->select_font)
        input->ops->select_font(input->context, dc, input->font_bold);
    zip_info_pair(input, dc, &x, y, ZIP_INFO_PAIR_ONE_BOLD,
        ZIP_INFO_PAIR_ONE_NORMAL, print, &maximum);
    y = (StarsI16)(y + input->arial8_height + input->arial8_height / 2);
    x = 4;
    if (input->ops->select_font)
        input->ops->select_font(input->context, dc, input->font_bold);
    zip_info_pair(input, dc, &x, y, ZIP_INFO_PAIR_TWO_BOLD,
        ZIP_INFO_PAIR_TWO_NORMAL, print, &maximum);
    y = (StarsI16)(y + input->arial8_height + input->arial8_height / 2);
    x = 4;
    if (input->ops->select_font)
        input->ops->select_font(input->context, dc, input->font_bold);
    zip_info_pair(input, dc, &x, y, ZIP_INFO_PAIR_THREE_BOLD,
        ZIP_INFO_PAIR_THREE_NORMAL, print, &maximum);
    y = (StarsI16)(y + input->arial8_height + input->arial8_height / 2);
    x = 4;
    if (input->ops->select_font)
        input->ops->select_font(input->context, dc, input->font_bold);
    zip_info_pair(input, dc, &x, y, ZIP_INFO_PAIR_FOUR_BOLD,
        ZIP_INFO_PAIR_FOUR_NORMAL, print, &maximum);
    result.x = (StarsI16)(maximum + 4);
    result.y = (StarsI16)(y + input->arial8_height + 4);
    return result;
}
