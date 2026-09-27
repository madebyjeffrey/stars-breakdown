#include "core.h"

/* NE 34:589C-5BB7. Rebuild one report's row-index cache, then order the
 * selected indices through the report comparator.  The caller supplies the
 * report data and comparator because the original routine reads those from
 * far globals and qsort dispatch state. */
void stars_sort_report_cache(const StarsSortReportCacheInput *input,
    StarsSortReportCacheState *state)
{
    StarsI16 count = 0;
    StarsI16 i;
    StarsI16 j;
    StarsI16 value;
    if (!input || !state) return;
    if (input->current_sort_column != input->sort_column) {
        state->previous_sort_column = input->current_sort_column;
        state->previous_subsort = input->current_subsort;
        state->previous_ascending = input->current_ascending;
        state->cached = 0;
    }
    if (input->report_window != 0U || !input->cached) {
        if (input->report == 0) {
            for (i = 0; i < input->planet_count && count < 1025; ++i)
                if (input->planets[i].player == input->player &&
                        (input->planets[i].status & 0x00ffU) == 7U)
                    state->indices[count++] = i;
        } else if (input->report == 1) {
            for (i = 0; i < input->fleet_count && count < 1025; ++i) {
                if (!input->fleets[i].present) break;
                if (input->fleets[i].player == input->player)
                    state->indices[count++] = i;
            }
        } else if (input->report == 2) {
            state->battle_cached = 0U;
            for (i = 0; i < input->fleet_count && count < 1020; ++i) {
                if (!input->fleets[i].present) break;
                if (input->fleets[i].player != input->player)
                    state->indices[count++] = i;
            }
        } else if (input->report == 3) {
            if (input->battle_count) count = input->battle_count((void *)input);
            if (count < 0) count = 0;
            if (count > 1025) count = 1025;
            for (i = 0; i < count; ++i) state->indices[i] = i;
        } else return;
        state->rows = count;
        if (input->compare) {
            for (i = 1; i < count; ++i) {
                value = state->indices[i];
                j = i;
                while (j > 0 && input->compare((void *)input,
                        state->indices[j - 1], value, input->sort_column) > 0) {
                    state->indices[j] = state->indices[j - 1];
                    --j;
                }
                state->indices[j] = value;
            }
        }
        state->cached = 1;
    }
}
