/* C-only semantic probe for InitMDIApp; no Python or callback trampolines. */
#include <stdio.h>
#include "../reconstructed/include/core.h"

static unsigned registrations;
static unsigned cursor_calls;
static unsigned stock_calls;
static unsigned failure_at;
static const StarsU16 styles[11] = {
    0x000bU, 0x020bU, 0x020bU, 0x020bU, 0x0208U, 0x0200U,
    0x0a00U, 0x0a00U, 0x0200U, 0x0000U, 0x000bU
};
static const StarsU16 proc_offsets[11] = {
    0x06d6U, 0x5c92U, 0x0032U, 0x0000U, 0x001eU, 0x0000U,
    0x0000U, 0x19e4U, 0x2876U, 0x9126U, 0x0018U
};
static const StarsU16 proc_selectors[11] = {
    0x1020U, 0x1030U, 0x1058U, 0x1028U, 0x1068U, 0x1048U,
    0x10c0U, 0x1068U, 0x10d8U, 0x1020U, 0x1108U
};
static const StarsU16 class_names[11] = {
    0x01d6U, 0x0202U, 0x01e2U, 0x01ecU, 0x0242U, 0x01f6U,
    0x0210U, 0x024aU, 0x021cU, 0x022aU, 0x0236U
};

static StarsU16 load_cursor(void *context, StarsU16 instance,
    StarsU16 resource)
{
    (void)context;
    if (instance != 0U || resource != 0x7f00U) return 0U;
    ++cursor_calls;
    return (StarsU16)(0x7000U + cursor_calls);
}

static StarsU16 get_stock(void *context, StarsU16 stock)
{
    (void)context;
    ++stock_calls;
    return (StarsU16)(0x5000U + stock);
}

static StarsU16 register_class(void *context, const StarsMdiClassSpec *spec)
{
    unsigned i;
    (void)context;
    i = registrations;
    if (i >= 11U || spec->style != styles[i] ||
        spec->window_proc.offset != proc_offsets[i] ||
        spec->window_proc.selector != proc_selectors[i] ||
        spec->class_name.offset != class_names[i] ||
        spec->class_name.selector != 0x1120U || spec->instance != 0x1234U)
        return 0U;
    if (i == 0U && (spec->menu_name.offset != 0x0364U ||
                    spec->menu_name.selector != 0x1120U ||
                    spec->background != 0x000dU)) return 0U;
    if (i != 0U && (spec->menu_name.offset != 0U ||
                    spec->menu_name.selector != 0U)) return 0U;
    ++registrations;
    return failure_at == registrations ? 0U : 1U;
}

static int run_probe(unsigned fail)
{
    StarsInitMdiOps ops;
    StarsI16 result;

    registrations = 0U;
    cursor_calls = 0U;
    stock_calls = 0U;
    failure_at = fail;
    ops.load_cursor = load_cursor;
    ops.get_stock_object = get_stock;
    ops.register_class = register_class;
    result = stars_init_mdi_app(0x1234U, &ops, 0);
    if (fail == 0U)
        return result == 1 && registrations == 11U && cursor_calls == 3U &&
               stock_calls == 10U ? 0 : 1;
    return result == 0 && registrations == fail ? 0 : 1;
}

int main(void)
{
    unsigned fail;
    if (run_probe(0U) != 0) return 1;
    for (fail = 1U; fail <= 11U; ++fail)
        if (run_probe(fail) != 0) return 2;
    printf("MDI_SOURCE_CHECKS=12 FAILURES=0 RESULT=PASS\n");
    return 0;
}
