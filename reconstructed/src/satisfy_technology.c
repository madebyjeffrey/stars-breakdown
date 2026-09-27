#include "core.h"

/* NE 23:8B2D-8CED and NE 31:9918-9C0D.  Keep the signed result from
 * ITechLearnATech bound to the message/result numbering used by SatisfyOrders:
 * negative results identify trader parts, positive results identify the next
 * battle-technology level, and zero means that no technology was learned. */
StarsI16 stars_bind_satisfy_technology(StarsTechLearnInput *input,
    StarsI16 negative_message, StarsI16 positive_message,
    StarsSatisfyTechnologyOutcome *outcome)
{
    StarsI16 result;

    if (!outcome)
        return 0;
    result = stars_itech_learn_a_tech(input);
    outcome->result = result;
    outcome->message_code = 0;
    outcome->result_id = 0;
    if (result < 0) {
        outcome->message_code = negative_message;
        outcome->result_id = (StarsI16)(-(result + 1));
    } else if (result > 0) {
        outcome->message_code = positive_message;
        outcome->result_id = (StarsI16)(result - 1);
    }
    return result;
}
