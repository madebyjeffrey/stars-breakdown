#include "core.h"
#include <string.h>

/* NE 16:7508-76AA. Prepare the Stars save-file dialog and append the default
 * xy extension at the same position reported by GetSaveFileName. USER and
 * resource lookup are explicit callbacks; the path and filter transformations
 * remain ordinary bounded C string operations. */
StarsI16 stars_get_new_game_name(StarsU8 *suggestion, StarsU16 capacity,
    const StarsGetNewGameNameOps *ops, void *context)
{
    StarsU8 filter[256];
    StarsU8 selected[512];
    StarsU16 extension_offset = 0U;
    StarsU16 i;
    StarsU16 length;
    if (!suggestion || capacity == 0U || !ops || !ops->choose_file)
        return 0;
    selected[0] = 0U;
    strncpy((char *)selected, (const char *)suggestion,
        (size_t)(sizeof selected - 1U));
    selected[sizeof selected - 1U] = 0U;
    memset(filter, 0, sizeof filter);
    if (ops->get_string) ops->get_string(context, 0x316U, filter);
    for (i = 0; i < (StarsU16)sizeof filter && filter[i] != 0U; ++i)
        if (filter[i] == '|') {
            filter[i] = 0U;
            break;
        }
    if (!ops->choose_file(context, selected, filter, selected,
            (StarsU16)sizeof selected, &extension_offset)) return 0;
    length = (StarsU16)strlen((const char *)selected);
    if (extension_offset == 0U) {
        if ((StarsU16)(length + 2U) >= (StarsU16)sizeof selected) return 0;
        selected[length++] = 'x';
        selected[length++] = 'y';
        selected[length] = 0U;
    } else {
        if (extension_offset > (StarsU16)sizeof selected - 3U) return 0;
        selected[extension_offset] = 'x';
        selected[extension_offset + 1U] = 'y';
        selected[extension_offset + 2U] = 0U;
    }
    if ((StarsU16)(strlen((const char *)selected) + 1U) > capacity) return 0;
    strcpy((char *)suggestion, (const char *)selected);
    return 1;
}
