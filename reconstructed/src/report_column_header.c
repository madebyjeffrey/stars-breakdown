#include "core.h"

static StarsI32 report_width(const StarsReportColumnHeaderInput *input,
    const StarsU8 *text, StarsI16 length)
{
    if (!input->text_extent)
        return 0;
    return (StarsI32)input->text_extent(input->context, input->hdc,
        text, (StarsU16)length);
}

/* NE 22:305E-3398.  Calculate the even pixel width reserved for one report
 * column, including the original per-report and per-column minimums. */
StarsI16 stars_report_column_header(const StarsReportColumnHeaderInput *input,
    StarsI16 report, StarsI16 column, StarsU8 *text)
{
    StarsU8 digit[2];
    StarsU8 temporary[42];
    StarsI16 length;
    StarsI32 digit_width;
    StarsI32 width;
    StarsI32 minimum;
    StarsU16 string_id;

    if (!input || !text || report < 0 || report > 3) {
        if (text)
            text[0] = 0;
        return 0;
    }
    digit[0] = (StarsU8)'8';
    digit[1] = 0;
    digit_width = report_width(input, digit, 1);
    minimum = 0;
    if (report == 0) {
        string_id = (StarsU16)(column + 0x459);
        length = input->get_string ? input->get_string(input->context,
            string_id, text, 42U) : 0;
        width = report_width(input, text, length);
        switch (column) {
        case 0:
            width *= 2;
            break;
        case 1:
        case 0x0d:
        case 0x0e:
            minimum = digit_width * 0x0f;
            break;
        case 3:
            minimum = digit_width * 4 + 2;
            break;
        case 4:
            if (input->get_string)
                length = input->get_string(input->context, 0x468U,
                    temporary, (StarsU16)sizeof(temporary));
            minimum = report_width(input, temporary, length);
            break;
        case 5:
            width = width * 3 + 0x14;
            break;
        case 6:
        case 7:
            minimum = digit_width * 5;
            break;
        case 9:
            minimum = digit_width * 0x0e;
            break;
        case 10:
        case 11:
            minimum = digit_width * 0x0b;
            break;
        case 12:
            if (input->get_string)
                length = input->get_string(input->context, 0x46aU,
                    temporary, (StarsU16)sizeof(temporary));
            minimum = report_width(input, temporary, length);
            break;
        default:
            break;
        }
    } else if (report == 1) {
        string_id = (StarsU16)(column + 0x472);
        length = input->get_string ? input->get_string(input->context,
            string_id, text, 42U) : 0;
        width = report_width(input, text, length);
        switch (column) {
        case 0:
        case 1:
        case 3:
        case 8:
        case 10:
        case 11:
            width *= 2;
            break;
        case 2:
            width = width * 2 + width / 2;
            break;
        case 4:
            width = width * 2 - digit_width;
            break;
        case 5:
            width = width * 4 + width / 2;
            break;
        case 6:
            minimum = digit_width * 5;
            break;
        case 7:
            minimum = digit_width * 0x13;
            break;
        default:
            break;
        }
    } else if (report == 2) {
        string_id = (StarsU16)(column + 0x47e);
        length = input->get_string ? input->get_string(input->context,
            string_id, text, 42U) : 0;
        width = report_width(input, text, length);
        if (column == 0)
            width *= 3;
        else if (column == 1 || column == 5)
            width *= 2;
        else if (column == 2)
            width = width * 2 + width / 2;
    } else {
        string_id = (StarsU16)(column + 0x48a);
        length = input->get_string ? input->get_string(input->context,
            string_id, text, 42U) : 0;
        width = report_width(input, text, length);
        if (column == 0)
            width = width * 2 + width / 2;
    }
    if (width < minimum)
        width = minimum;
    return (StarsI16)(((width + 6) / 2) * 2);
}
