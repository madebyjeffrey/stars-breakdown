#include "core.h"
#include <string.h>

/* NE 26:694C-6A51. Increment the numeric design-name suffix when it is a
 * recognized digit, otherwise append the standard design-name resource, then
 * pass the result through the screen-width fitter. */
void stars_make_new_name(StarsU8 *name, StarsU16 capacity,
    const StarsMakeNewNameOps *ops, void *context)
{
    StarsU16 length;
    StarsU8 digit;
    if (!name || capacity == 0U) return;
    length = (StarsU16)strlen((const char *)name);
    if (length < 28U && length >= 3U && name[length - 1U] == ')' &&
            name[length - 3U] == '(') {
        digit = name[length - 2U];
        if (ops && ops->digit_flag && ops->digit_flag(context, digit)) {
            name[length - 2U] = digit == '9' ? '0' : (StarsU8)(digit + 1U);
        } else if (ops && ops->append_default) {
            ops->append_default(context, name, capacity);
        }
    } else if (length < 28U && ops && ops->append_default) {
        ops->append_default(context, name, capacity);
    }
    if (ops && ops->fit) ops->fit(context, name, 0xa0U);
}
