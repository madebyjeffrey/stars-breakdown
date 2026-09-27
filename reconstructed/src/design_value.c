#include "core.h"

/* NE 30:07A8-08D8. Convert a token's hull design value to a displacement-
 * adjusted value.  The bit masks and integer operation order retain the
 * original fixed-point arithmetic before the final subtraction. */
StarsI32 stars_design_value_from_token(const StarsDesignValueInput *input)
{
    StarsU32 value;
    StarsI32 scale;
    StarsU32 adjusted;
    if (!input) return 0;
    value = input->hull_dp * input->token_count;
    if (!input->has_displacement || input->displacement_dp == 0UL)
        return (StarsI32)value;
    scale = (StarsI32)((input->token_count *
        (input->displacement_dp & 0xffff007fUL)) / 100UL);
    if (scale < 1) scale = 1;
    adjusted = ((input->hull_dp * (input->displacement_dp >> 7)) / 10UL);
    adjusted = (adjusted * (StarsU32)scale) / 50UL;
    return (StarsI32)(value - adjusted);
}
