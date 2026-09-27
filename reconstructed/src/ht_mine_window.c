#include "core.h"

enum {
    MINE_HIT_NONE = 0,
    MINE_HIT_PLANET = 1,
    MINE_HIT_FLEET = 2,
    MINE_HIT_THING = 8,
    MINE_HIT_OTHER = 9,
    MINE_HIT_FLEET_PANEL = 10,
    MINE_HIT_PLANET_PANEL = 11,
    MINE_HIT_PLANET_OWNER = 12,
    MINE_HIT_STARBASE = 13,
    MINE_HIT_SPECIAL_THING = 14,
    MINE_THING_MINES = 2,
    MINE_THING_WORMHOLE = 3,
    MINE_THING_SPECIAL = 0,
    MINE_SELECTION_OBJECT = 4,
    MINE_OBJECT_PLANET_BIT = 1,
    MINE_OBJECT_FLEET_BIT = 2,
    MINE_PLANET_STARBASE_BIT = 0x0200
};

static StarsI16 between(StarsI16 value, StarsI16 low, StarsI16 high)
{
    return (StarsI16)(value >= low && value < high);
}

/* NE 06:37AC-3B4D. Convert the scanner's combined object selection into a
 * concrete panel kind, then return the mine-window command represented by the
 * mouse coordinate. The numeric results are retained because MineClick uses
 * them as dispatch tokens; the names above make that dispatch readable. */
StarsI16 stars_ht_mine_window(const StarsHtMineWindowInput *input)
{
    StarsI16 object_kind;
    StarsI16 content_top;
    StarsI16 title_top;
    StarsI16 row_height;
    StarsI16 detail_top;
    StarsI16 thing_kind;

    if (!input) return MINE_HIT_NONE;
    object_kind = input->selection.grobj;
    if (object_kind == MINE_SELECTION_OBJECT) {
        if ((input->selection.grobj_full & MINE_OBJECT_PLANET_BIT) != 0U)
            object_kind = MINE_HIT_PLANET;
        else if ((input->selection.grobj_full & MINE_OBJECT_FLEET_BIT) != 0U)
            object_kind = MINE_HIT_FLEET;
        else
            object_kind = MINE_HIT_NONE;
    }

    content_top = (StarsI16)(input->client_top + input->font_height * 2);
    if (input->y < (StarsI16)(content_top - 5) && input->y > 4 &&
            input->x < (StarsI16)(input->client_right - 4) &&
            input->x > (StarsI16)(input->client_right - content_top) &&
            input->other_at_selection &&
            input->other_at_selection(input->context))
        return MINE_HIT_OTHER;

    thing_kind = (StarsI16)input->selected_thing_kind;
    if (object_kind == MINE_HIT_FLEET ||
            (object_kind == MINE_HIT_THING && thing_kind != MINE_THING_MINES &&
                thing_kind != MINE_THING_WORMHOLE)) {
        if (between(input->x, (StarsI16)(input->client_left + 0x19),
                (StarsI16)(input->client_left + 0x39)) &&
                between(input->y, (StarsI16)(content_top + 0x47),
                (StarsI16)(content_top + 0x67)))
            return MINE_HIT_FLEET_PANEL;
        if (object_kind == MINE_HIT_THING && thing_kind == MINE_THING_SPECIAL &&
                between(input->x, (StarsI16)(input->client_left + 9),
                    (StarsI16)(input->client_left + 0x49)) &&
                between(input->y, (StarsI16)(content_top + 9),
                    (StarsI16)(content_top + 0x49)))
            return MINE_HIT_SPECIAL_THING;
        if (object_kind == MINE_HIT_THING) return MINE_HIT_NONE;
        if (between(input->x, (StarsI16)(input->client_left + 9),
                (StarsI16)(input->client_left + 0x49)) &&
                between(input->y, (StarsI16)(content_top + 9),
                (StarsI16)(content_top + 0x49)))
            return MINE_HIT_PLANET_PANEL;
        if (input->x >= (StarsI16)(input->client_right + 0x5c) &&
                input->y >= (StarsI16)(content_top + 4) &&
                input->y < (StarsI16)(content_top + 7 +
                    input->font_height))
            return MINE_HIT_PLANET_PANEL;
        return MINE_HIT_NONE;
    }
    if (object_kind == MINE_HIT_THING) return MINE_HIT_NONE;

    if (input->selection.planet == -1 || !input->planet_found)
        return MINE_HIT_NONE;

    title_top = (StarsI16)(content_top - 4);
    row_height = (StarsI16)(((input->client_bottom - title_top -
        input->font_height * 4 - 2) / 6 + 1) & (StarsI16)~1);
    if (row_height < 1) row_height = 1;
    if (input->y < title_top) {
        if (input->y <= (StarsI16)(input->client_top - 4))
            return MINE_HIT_NONE;
        if (input->x < (StarsI16)(input->client_right - 0x18)) {
            if (input->planet_owner == input->player ||
                    input->x <= (StarsI16)((input->client_right * 3) / 5) ||
                    input->y < (StarsI16)(title_top - input->font_height) ||
                    input->planet_owner == -1)
                return MINE_HIT_PLANET_OWNER;
            return MINE_HIT_FLEET_PANEL;
        }
        return (input->planet_flags & MINE_PLANET_STARBASE_BIT) != 0U ?
            MINE_HIT_STARBASE : MINE_HIT_NONE;
    }
    if (input->y < (StarsI16)(title_top + row_height * 3))
        return (StarsI16)((input->y - title_top) / row_height + 6);

    detail_top = (StarsI16)(title_top + row_height * 3 +
        (input->font_height / 2) + 1);
    if (input->y < (StarsI16)(detail_top + row_height * 3))
        return (StarsI16)((input->y - detail_top) / row_height + 1);
    return 5;
}
