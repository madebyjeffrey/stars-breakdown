#include "core.h"
#include <stdio.h>
#include <string.h>

int main(void)
{
    struct { StarsMessageResetState state; StarsU8 canary[2]; } box;
    StarsMessageResetState *state = &box.state;
    int failures = 0;
    memset(&box, 0xa5, sizeof box);
    stars_reset_messages(state);
    if (state->message_memory != 0U || state->current_message != -1 ||
        state->message_count != 0U || state->send_current != 0 ||
        state->incoming_list != 0 || state->incoming_segment != 0U ||
        state->outgoing_list != 0 || state->outgoing_segment != 0U ||
        state->incoming_count != 0U || state->outgoing_count != 0U)
        ++failures;
    if (state->sent[0] != 0U || state->sent[48] != 0U ||
        state->filtered[0] != 0U || state->filtered[48] != 0U)
        ++failures;
    if (box.canary[0] != 0xa5U || box.canary[1] != 0xa5U)
        ++failures;
    printf("MESSAGES_RESET_CHECKS=3 FAILURES=%d RESULT=%s\n", failures,
        failures ? "FAIL" : "PASS");
    return failures != 0;
}
