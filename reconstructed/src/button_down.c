#include "core.h"

/* NE 14:0C3A-0DA3. Decode the scanner toolbar button state from the packed
 * scan flags and mine-mode selector. */
StarsI16 stars_is_button_down(const StarsButtonDownInput *input, StarsI16 button)
{
    if (!input) return 0;
    if (button >= 0 && button <= 5)
        return (StarsI16)((input->scan_flags & 0x000fU) == (StarsU16)button);
    if (button == 6) return (input->scan_flags & 0x0010U) != 0U;
    if (button == 7) return (input->scan_flags & 0x0020U) != 0U;
    if (button == 8) return (input->scan_flags & 0x0040U) != 0U &&
        input->mine_mode == 0x000fU;
    if (button == 9) return (input->scan_flags & 0x0080U) != 0U;
    if (button == 10) return (input->scan_flags & 0x0100U) != 0U;
    if (button == 11) return (input->scan_flags & 0x0400U) != 0U;
    if (button == 12) return (input->scan_flags & 0x0200U) != 0U;
    if (button == 14) return (input->scan_flags & 0x0800U) != 0U;
    if (button == 17) return (input->scan_flags & 0x1000U) != 0U;
    return 0;
}
