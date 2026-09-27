#include "core.h"

enum {
    RESEARCH_SPIN_TOP = 1,
    RESEARCH_SPIN_BOTTOM = -1,
    RESEARCH_LARGE_STEP = 10,
    RESEARCH_SMALL_STEP = 1,
    RESEARCH_PERCENT_MIN = 0,
    RESEARCH_PERCENT_MAX = 100,
    RESEARCH_FKB_LARGE_STEP = 4,
    RESEARCH_RACE_FLAG_SPECIAL_NOTE = 4,
    RESEARCH_RACE_FLAG_TECH_NOTE = 0x0c,
    RESEARCH_NOTE_NONE = 0,
    RESEARCH_NOTE_STANDARD = 0x144,
    RESEARCH_NOTE_ALTERNATE = 0x14c
};

static StarsI16 point_in_bounds(const StarsFTrackResearchDlgInput *input,
    StarsI16 left, StarsI16 top, StarsI16 right, StarsI16 bottom)
{
    return (StarsI16)(input->x >= left && input->x < right &&
        input->y >= top && input->y < bottom);
}

/* NE 1C:1A8C-1D4E.  Handle the research dialog's three interactive regions:
 * the two spin buttons, the future-technology list, and the technology note.
 * A spin step is deliberately exposed as a callback because FTrackBtn owns
 * the Win16 mouse-repeat state; this function retains the original 1/10-point
 * increments, 0..100 clamp, projection update, and redraw ordering. */
StarsI16 stars_f_track_research_dlg(StarsFTrackResearchDlgInput *input)
{
    StarsI16 direction;
    StarsI16 step;
    StarsI16 index;
    StarsI16 note_string;
    StarsI16 race_flag;
    StarsI16 changed = 0;

    if (!input)
        return 0;
    direction = 0;
    if (point_in_bounds(input, input->spin_top_left,
            input->spin_top_top, input->spin_top_right,
            input->spin_top_bottom))
        direction = RESEARCH_SPIN_TOP;
    else if (point_in_bounds(input, input->spin_bottom_left,
            input->spin_bottom_top, input->spin_bottom_right,
            input->spin_bottom_bottom))
        direction = RESEARCH_SPIN_BOTTOM;
    if (direction != 0 && input->track_step && input->research_percent) {
        step = (input->fkb & RESEARCH_FKB_LARGE_STEP) != 0 ? RESEARCH_LARGE_STEP :
            RESEARCH_SMALL_STEP;
        while (input->track_step(input->context, direction, step) != 0) {
            *input->research_percent = (StarsI16)(*input->research_percent +
                direction * step);
            if (*input->research_percent < RESEARCH_PERCENT_MIN)
                *input->research_percent = RESEARCH_PERCENT_MIN;
            if (*input->research_percent > RESEARCH_PERCENT_MAX)
                *input->research_percent = RESEARCH_PERCENT_MAX;
            if (input->research_budget && input->project_spending)
                *input->research_budget = input->project_spending(
                    input->context, *input->research_percent);
            if (input->redraw) input->redraw(input->context);
            changed = 1;
        }
        return changed;
    }

    if (input->y >= input->future_top && input->font_height > 0 &&
            input->y < input->future_top +
                input->future_count * input->font_height) {
        index = (StarsI16)((input->y - input->future_top) /
            input->font_height);
        if (index >= 0 && index < input->future_count &&
                input->show_future_tech)
            input->show_future_tech(input->context, index);
        return 1;
    }

    if (input->note_top != -1 && input->y > input->note_top &&
            input->y < input->note_top + input->font_height * 3 &&
            input->x > input->resource_left &&
            input->x <= input->resource_right && input->show_tech_note) {
        race_flag = input->race_flag ? input->race_flag(input->context,
            RESEARCH_RACE_FLAG_SPECIAL_NOTE) : 0;
        note_string = (race_flag == 0) ? RESEARCH_NOTE_STANDARD :
            RESEARCH_NOTE_ALTERNATE;
        if (input->race_flag && input->y >= input->note_top +
                (input->font_height * 3) / 2 &&
                input->race_flag(input->context,
                    RESEARCH_RACE_FLAG_TECH_NOTE) != 0)
            note_string = RESEARCH_NOTE_ALTERNATE;
        input->show_tech_note(input->context, note_string);
        return 1;
    }
    (void)note_string;
    return 0;
}
