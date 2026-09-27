#include <stdio.h>
#include <string.h>
#include "core.h"

static StarsI16 digit_flag(void *context, StarsU8 digit)
{ (void)context; return digit >= '0' && digit <= '9'; }
static int fit_width_bad;
static void append_default(void *context, StarsU8 *name, StarsU16 capacity)
{
    (void)context;
    if ((StarsU16)(strlen((char *)name) + 5U) < capacity)
        strcat((char *)name, "_new");
}
static void fit(void *context, StarsU8 *name, StarsU16 width)
{ (void)context; (void)name; if (width != 0xa0U) fit_width_bad = 1; }

int main(void)
{
    StarsMakeNewNameOps ops;
    StarsU8 name[64];
    memset(&ops, 0, sizeof ops);
    ops.digit_flag = digit_flag; ops.append_default = append_default; ops.fit = fit;
    strcpy((char *)name, "Design (3)");
    stars_make_new_name(name, (StarsU16)sizeof name, &ops, 0);
    if (strcmp((char *)name, "Design (4)") != 0) return 1;
    strcpy((char *)name, "Design (9)");
    stars_make_new_name(name, (StarsU16)sizeof name, &ops, 0);
    if (strcmp((char *)name, "Design (0)") != 0) return 2;
    strcpy((char *)name, "Design");
    stars_make_new_name(name, (StarsU16)sizeof name, &ops, 0);
    if (strcmp((char *)name, "Design_new") != 0) return 3;
    if (fit_width_bad) return 4;
    puts("MAKE_NEW_NAME_SOURCE=PASS");
    return 0;
}
