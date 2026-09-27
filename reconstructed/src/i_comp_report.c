#include "core.h"

static StarsI16 compare_signed(StarsI32 left, StarsI32 right)
{
    if (left < right) return -1;
    if (left > right) return 1;
    return 0;
}

static StarsI16 compare_column(const StarsICompReportInput *input,
    StarsI16 column)
{
    const StarsICompReportRow *left;
    const StarsICompReportRow *right;
    StarsI16 result;

    left = input->left;
    right = input->right;
    if (left->valid[column] == 0 && right->valid[column] == 0) return 0;
    if (left->valid[column] == 0) return -1;
    if (right->valid[column] == 0) return 1;
    if (left->text[column] && right->text[column] && input->compare_text) {
        result = input->compare_text(input->context, left->text[column],
            right->text[column]);
        result = compare_signed(result, 0);
    } else {
        result = compare_signed(left->value[column], right->value[column]);
    }
    if (result == 0)
        result = compare_signed(left->tie_value[column],
            right->tie_value[column]);
    return result;
}

/* NE 34:5BB8-74D4.  ICompReport is the qsort comparator shared by the
 * planet, fleet, and battle report views.  Each view resolves its packed
 * records through the row adapter above; this function preserves the
 * original signed ordering, missing-record ordering, descending inversion,
 * and one-level previous-column tie break. */
StarsI16 stars_i_comp_report(const StarsICompReportInput *input)
{
    StarsI16 result;

    if (!input || !input->left || !input->right) return 0;
    if (input->sort_column < 0 ||
        input->sort_column >= STARS_ICOMP_REPORT_COLUMNS) return 0;
    result = compare_column(input, input->sort_column);
    if (result == 0 && input->previous_sort_column >= 0 &&
            input->previous_sort_column < STARS_ICOMP_REPORT_COLUMNS &&
            input->previous_sort_column != input->sort_column) {
        StarsICompReportInput previous = *input;
        previous.sort_column = input->previous_sort_column;
        previous.subsort = input->previous_subsort;
        previous.ascending = input->previous_ascending;
        previous.previous_sort_column = -1;
        result = compare_column(&previous, previous.sort_column);
        if (previous.ascending == 0) result = (StarsI16)-result;
    }
    if (input->ascending == 0) result = (StarsI16)-result;
    return result;
}
