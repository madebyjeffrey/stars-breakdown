#include <stdio.h>
#include <string.h>
#include "core.h"

static StarsI16 text_compare(void *context, const StarsU8 *left,
    const StarsU8 *right)
{
    (void)context;
    return (StarsI16)strcmp((const char *)left, (const char *)right);
}

static void row_init(StarsICompReportRow *row)
{
    memset(row, 0, sizeof(*row));
    row->valid[0] = 1;
    row->valid[1] = 1;
    row->valid[2] = 1;
}

static StarsI16 compare(const StarsICompReportRow *left,
    const StarsICompReportRow *right, StarsI16 column, StarsI16 ascending,
    StarsI16 previous)
{
    StarsICompReportInput input;
    memset(&input, 0, sizeof(input));
    input.report_kind = 2;
    input.sort_column = column;
    input.ascending = ascending;
    input.previous_sort_column = previous;
    input.previous_ascending = 1;
    input.left = left;
    input.right = right;
    input.compare_text = text_compare;
    return stars_i_comp_report(&input);
}

int main(void)
{
    StarsICompReportRow left, right;
    row_init(&left);
    row_init(&right);
    left.value[1] = 4;
    right.value[1] = 9;
    if (compare(&left, &right, 1, 1, -1) >= 0) return 1;
    if (compare(&left, &right, 1, 0, -1) <= 0) return 2;
    left.value[1] = right.value[1];
    left.value[2] = 3;
    right.value[2] = 7;
    if (compare(&left, &right, 1, 1, 2) >= 0) return 3;
    left.text[0] = (const StarsU8 *)"Alpha";
    right.text[0] = (const StarsU8 *)"Beta";
    if (compare(&left, &right, 0, 1, -1) >= 0) return 4;
    left.valid[0] = 0;
    if (compare(&left, &right, 0, 1, -1) >= 0) return 5;
    puts("I_COMP_REPORT_SOURCE=PASS");
    return 0;
}
