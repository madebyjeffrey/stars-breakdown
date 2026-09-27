/* Shared numeric/geometry helpers; widths and aliasing follow the x86 code. */
#include "common.h"
#include "runtime_internal.h"

static StarsU16 scorex_word(const StarsU8 *score, StarsU16 offset)
{
    return (StarsU16)(score[offset] |
        ((StarsU16)score[offset + 1U] << 8));
}

static StarsU32 scorex_long(const StarsU8 *score, StarsU16 offset)
{
    return (StarsU32)scorex_word(score, offset) |
        ((StarsU32)scorex_word(score, (StarsU16)(offset + 2U)) << 16);
}

/* NE 34:2F94.  Score records retain the original packed component-count
 * words; values 2..4 mask 13 bits and shift by their encoded two-bit step. */
StarsI32 stars_fetch_score_x_val(const StarsU8 *score, StarsI16 value)
{
    StarsU16 packed;
    StarsU16 shift;

    if (!score)
        return 0;
    switch (value) {
    case 0: return (StarsI32)(StarsI16)scorex_word(score, 0x0cU);
    case 1: return (StarsI32)(StarsI16)scorex_word(score, 0x0eU);
    case 2:
    case 3:
    case 4:
        packed = scorex_word(score, (StarsU16)(0x10U +
            (StarsU16)(value - 2) * 2U));
        shift = (StarsU16)(((packed >> 13) & 7U) << 1);
        return (StarsI32)((StarsU32)(packed & 0x1fffU) << shift);
    case 5: return (StarsI32)(StarsI16)scorex_word(score, 0x16U);
    case 6: return (StarsI32)scorex_long(score, 0x04U);
    case 7: return (StarsI32)scorex_long(score, 0x08U);
    default: return 0;
    }
}

/* NE 20:0374-03BF. Show or hide the tutor window and mirror the low show
 * bit in tutor's flags; the callback is the USER::ShowWindow boundary. */
void stars_show_tutor(StarsU16 *hwnd, StarsU16 *tutor_flags, StarsI16 show,
    StarsInitShowWindow show_window, void *context)
{
    if (!hwnd || !tutor_flags || *hwnd == 0U) return;
    if (show_window) show_window(context, *hwnd, show == 0 ? 0U : 5U);
    *tutor_flags = (StarsU16)((*tutor_flags & 0xfffeU) |
        ((StarsU16)show & 1U));
}

/* NE 20:0C02-0C8F. Tear down an active tutor session, preserving the
 * close-versus-restore branch and the original tick-seeded Randomize2 call.
 * data is the DGROUP image; tutor points at its 44-byte tutor record. */
void stars_end_tutor(StarsU8 *data, StarsU8 *tutor, StarsI16 close,
    const StarsEndTutorOps *ops, void *context)
{
    StarsU16 tutor_flags;
    StarsU16 hwnd;
    StarsU32 tick;
    if (!data || !tutor || (stars_word(data, 0x7caU) & 0x0800U) == 0U)
        return;
    stars_put_word(data, 0x7caU,
        (StarsU16)(stars_word(data, 0x7caU) & 0xf7ffU));
    hwnd = (StarsU16)(tutor[16] | ((StarsU16)tutor[17] << 8));
    if (hwnd && ops && ops->destroy_window)
        ops->destroy_window(context, hwnd);
    stars_put_word(data, (StarsU16)(0x70U + 16U),
        (StarsU16)(stars_word(data, (StarsU16)(0x70U + 16U)) & 0xfff7U));
    if (close == 0) {
        tutor_flags = stars_word(tutor, 0);
        stars_put_word(tutor, 0,
            (StarsU16)((tutor_flags & 0xdfffU) | 0x2000U));
    } else if (ops && ops->restore_game) {
        ops->restore_game(context);
    }
    memset(tutor, 0, 44U);
    if (ops && ops->get_tick && ops->randomize2) {
        tick = ops->get_tick(context);
        ops->randomize2(context, tick);
    }
}

/* NE 20:0F36-0F93. Ask before ending an early-turn tutor session; response 6
 * is the original affirmative result. */
StarsI16 stars_ask_kill_tutor(StarsU16 turn, const StarsU8 *warning,
    StarsTutorConfirmEnd confirm, StarsTutorEndCallback end, void *context)
{
    if (turn < 0x1eU && (!confirm || confirm(context, warning) != 6))
        return 0;
    if (end) end(context, 1);
    return 1;
}

/* NE 31:3E68-3EC1. Resolve the token's hull under its player context,
 * compute initiative, restore idPlayer to -1, and write initBase/pctBC. */
void stars_check_initiative(StarsU8 *token,
    const StarsCheckInitiativeOps *ops, void *context)
{
    const StarsU8 *hull;
    StarsU8 pct_bc = 0;
    StarsI16 initiative;
    if (!token || !ops || !ops->lookup_hull || !ops->init_hull) return;
    hull = ops->lookup_hull(context, token);
    if (!hull) return;
    if (ops->set_player) ops->set_player(context, (StarsI16)token[2]);
    initiative = ops->init_hull(context, hull, &pct_bc);
    token[6] = (StarsU8)initiative;
    if (ops->set_player) ops->set_player(context, -1);
    token[12] = pct_bc;
}

/* NE 20:3020-302F. The original procedure has no executable body beyond its
 * standard far-call prologue/epilogue. */
void stars_ensure_cas_hdefs(StarsI16 current)
{
    (void)current;
}

/* NE 10:A108-A130. rt is an ABI parameter retained for the host entry; the
 * original state transition uses the current memory-log header and cursor. */
void stars_cancel_mem_rt(StarsU8 *data, StarsI16 rt)
{
    StarsU16 header;
    StarsU16 cursor;
    (void)rt;
    if (!data) return;
    header = stars_word(data, 0x5260U);
    cursor = stars_word(data, 0x09a4U);
    stars_put_word(data, 0x09a4U,
        (StarsU16)(cursor - (StarsU16)((header & 0x03ffU) + 2U)));
    stars_put_word(data, 0x5260U, (StarsU16)(header & 0x03ffU));
}

/* NE 10:A228-A25E. The title callback represents SetMsgTitle(hwndMessage);
 * the original only invokes it after a real dirty-state transition and when
 * the AI flag is clear. */
void stars_dirty_game(StarsU8 *data, StarsI16 dirty,
    StarsSetMessageTitle set_title, void *context)
{
    if (!data || (StarsU16)dirty == stars_word(data, 0x7eU)) return;
    stars_put_word(data, 0x7eU, (StarsU16)dirty);
    if (stars_word(data, 0x1442U) == 0U && set_title)
        set_title(context, stars_word(data, 0x198U));
}

/* NE 10:A130-A227. Append a typed memory-log record unless playback is
 * active, preserving the previous header and both log cursors. */
void stars_write_mem_rt(StarsU8 *data, StarsU8 *log, StarsI16 record_type,
    StarsI16 count, const StarsU8 *input, const StarsWriteMemRtOps *ops,
    void *context)
{
    StarsU16 old_header;
    StarsU16 cursor;
    StarsU16 header;
    StarsI32 end;
    if (!data) return;
    old_header = stars_word(data, 0x5260U);
    if (stars_word(data, 0x09a8U) == 0U) {
        cursor = stars_word(data, 0x09a4U);
        end = (StarsI32)cursor + (StarsI32)count + 2L;
        if (end > 32000L && ops && ops->log_overflow)
            ops->log_overflow(context);
        stars_dirty_game(data, 1, ops ? ops->set_message_title : 0, context);
        stars_put_word(data, 0x09a6U, cursor);
        header = (StarsU16)((StarsU16)count & 0x03ffU);
        header = (StarsU16)(header | ((StarsU16)record_type << 10));
        if (log) {
            log[cursor] = (StarsU8)header;
            log[(StarsU16)(cursor + 1U)] = (StarsU8)(header >> 8);
            if (count > 0 && input)
                memcpy(log + (StarsU16)(cursor + 2U), input,
                    (size_t)(StarsU16)count);
        }
        stars_put_word(data, 0x09a4U,
            (StarsU16)(cursor + (StarsU16)count + 2U));
        if (record_type == 0) old_header = header;
    }
    stars_put_word(data, 0x5260U, old_header);
}

/* NE 11:6004-6033. LGetFleetStat(1) is an explicit host boundary; the
 * following subtraction is the original unsigned 32-bit fuel-free result,
 * including borrow across the two words. */
StarsU32 stars_get_fuel_free(const StarsU8 *fleet,
    StarsGetFleetStat get_stat, void *context)
{
    StarsU32 statistic, minimum;
    if (!fleet || !get_stat) return 0;
    statistic = get_stat(context, fleet, 1U);
    minimum = (StarsU32)fleet[0x5cU] |
        ((StarsU32)fleet[0x5dU] << 8) |
        ((StarsU32)fleet[0x5eU] << 16) |
        ((StarsU32)fleet[0x5fU] << 24);
    return statistic - minimum;
}

/* NE 11:5F98-6003. The four capacity words are accumulated as four
 * little-endian dwords, exactly as the original low-word ADD/high-word ADC
 * loop does, before querying LGetFleetStat(2). */
StarsU32 stars_get_cargo_free(const StarsU8 *fleet,
    StarsGetFleetStat get_stat, void *context)
{
    StarsU32 capacity = 0;
    StarsU16 i;
    StarsU32 value;
    if (!fleet || !get_stat) return 0;
    for (i = 0; i < 4U; ++i) {
        const StarsU8 *entry = fleet + 0x4cU + i * 4U;
        value = (StarsU32)entry[0] | ((StarsU32)entry[1] << 8) |
            ((StarsU32)entry[2] << 16) | ((StarsU32)entry[3] << 24);
        capacity += value;
    }
    return get_stat(context, fleet, 2U) - capacity;
}

/* NE 31:20F6-2129. The hull definition identifier is the first word of the
 * supplied ship-definition record. */
StarsI16 stars_fuel_tanker(const StarsU8 *ship_definition)
{
    StarsU16 hull;
    if (!ship_definition) return 0;
    hull = (StarsU16)ship_definition[0] |
        ((StarsU16)ship_definition[1] << 8);
    return (StarsI16)(hull == 0x19U || hull == 0x1aU);
}

/* NE 12:744E-748F. Coordinates are read and written as 16-bit values; the
 * callback represents PtToScan while the data image supplies xScanTop,
 * yScanTop, and dGalInv at their recovered DGROUP offsets. */
void stars_logical_to_scan(StarsU8 *data, StarsPtToScan pt_to_scan,
    void *context)
{
    StarsU16 x, y, x_top, y_top, galactic_inverse;
    if (!data || !pt_to_scan) return;
    x = (StarsU16)(data[0] | ((StarsU16)data[1] << 8));
    y = (StarsU16)(data[2] | ((StarsU16)data[3] << 8));
    x_top = (StarsU16)(data[0x2572U] |
        ((StarsU16)data[0x2573U] << 8));
    y_top = (StarsU16)(data[0x2570U] |
        ((StarsU16)data[0x2571U] << 8));
    galactic_inverse = (StarsU16)(data[0xf2U] |
        ((StarsU16)data[0xf3U] << 8));
    x = (StarsU16)(x - x_top);
    x = (StarsU16)pt_to_scan(context, (StarsI16)x);
    y = (StarsU16)(galactic_inverse - y);
    y = (StarsU16)(y - y_top);
    y = (StarsU16)pt_to_scan(context, (StarsI16)y);
    data[0] = (StarsU8)x;
    data[1] = (StarsU8)(x >> 8);
    data[2] = (StarsU8)y;
    data[3] = (StarsU8)(y >> 8);
}

/* NE 12:7490-7502. ScanToPt is explicit at the host boundary; additions,
 * subtraction, and the final signed comparisons retain 16-bit coordinates. */
void stars_scan_to_logical(StarsU8 *data, StarsScanToPt scan_to_pt,
    void *context)
{
    StarsU16 x, y, x_top, y_top, galactic_inverse, galactic;
    StarsI16 x_limit;
    if (!data || !scan_to_pt) return;
    x = (StarsU16)(data[0] | ((StarsU16)data[1] << 8));
    y = (StarsU16)(data[2] | ((StarsU16)data[3] << 8));
    x_top = (StarsU16)(data[0x2572U] |
        ((StarsU16)data[0x2573U] << 8));
    y_top = (StarsU16)(data[0x2570U] |
        ((StarsU16)data[0x2571U] << 8));
    galactic_inverse = (StarsU16)(data[0xf2U] |
        ((StarsU16)data[0xf3U] << 8));
    galactic = (StarsU16)(data[0xf0U] |
        ((StarsU16)data[0xf1U] << 8));
    x = (StarsU16)((StarsU16)scan_to_pt(context, (StarsI16)x) + x_top);
    y = (StarsU16)((StarsU16)scan_to_pt(context, (StarsI16)y) + y_top);
    y = (StarsU16)(galactic_inverse - y);
    x_limit = (StarsI16)(StarsU16)(galactic + 1000U);
    if ((StarsI16)x > x_limit) x = (StarsU16)x_limit;
    if ((StarsI16)y < 1000) y = 1000U;
    data[0] = (StarsU8)x;
    data[1] = (StarsU8)(x >> 8);
    data[2] = (StarsU8)y;
    data[3] = (StarsU8)(y >> 8);
}

/* NE 7:8412-8443. The 18-byte message-big record is supplied as a raw host
 * span; FGetNMsgbig remains an explicit callback while this wrapper retains
 * its failure sentinel mutation and return value. */
StarsI16 stars_idm_get_message_n(StarsI16 message, StarsGetMessage get_message,
    void *context)
{
    StarsU8 record[18];
    StarsU16 value;
    if (!get_message) return -1;
    memset(record, 0, sizeof record);
    if (!get_message(context, message, record)) {
        record[0] = 0xffU;
        record[1] = 0xffU;
    }
    value = (StarsU16)(record[0] | ((StarsU16)record[1] << 8));
    return (StarsI16)value;
}

/* NE 7:8580-85CB. A failed lookup returns the supplied message buffer after
 * clearing its first byte; a successful lookup forwards the 16-bit parameter
 * span beginning at record offset four to PszFormatMessage. */
const StarsU8 *stars_psz_get_message_n(StarsI16 message,
    StarsGetMessage get_message, StarsFormatMessage format_message,
    StarsU8 *message_buffer, void *context)
{
    StarsU8 record[18];
    StarsU16 value;
    if (!message_buffer || !get_message) return message_buffer;
    memset(record, 0, sizeof record);
    if (!get_message(context, message, record)) {
        message_buffer[0] = 0;
        return message_buffer;
    }
    value = (StarsU16)(record[0] | ((StarsU16)record[1] << 8));
    if (!format_message) return message_buffer;
    return format_message(context, (StarsI16)value, record + 4U);
}

static StarsU16 current_fleet_count(const StarsU8 *data)
{
    StarsU16 player = (StarsU16)(data[0x18cU] |
        ((StarsU16)data[0x18dU] << 8));
    StarsU16 offset = (StarsU16)(0x59a2U + player * 0xc0U + 4U);
    return (StarsU16)((data[offset] | ((StarsU16)data[(StarsU16)(offset + 1U)] << 8)) &
        0x0fffU);
}

/* NE 11:245C-24AD. Splitting is unavailable at the fleet-count sentinel;
 * otherwise the boat count must be a nonnegative value of at least two. */
StarsI16 stars_can_split(const StarsU8 *data, StarsI32 boats)
{
    if (!data || current_fleet_count(data) == 0x0200U) return 0;
    return (StarsI16)(boats >= 2L);
}

/* NE 11:24AE-2521. The total after removing one boat must fit the original
 * signed 32-bit range tested by the high/low-word compare, and boats must be
 * at least two. */
StarsI16 stars_can_split_all(const StarsU8 *data, StarsI32 boats)
{
    StarsI32 total;
    if (!data) return 0;
    total = boats - 1L + (StarsI32)current_fleet_count(data);
    return (StarsI16)(total >= 0L && total <= 0x200L && boats >= 2L);
}

/* NE 11:2522-263F. A merge is allowed only when another fleet shares the
 * target's owner and position and the resulting ship total remains below
 * that player's fleet-count limit. The original scans the far-pointer list
 * until its first null entry; present is the typed host equivalent. */
StarsI16 stars_can_merge(const StarsMergeFleet *target,
    const StarsMergeFleet *fleets, StarsU16 fleet_count,
    StarsI16 player_fleet_count)
{
    StarsU16 i;
    StarsU16 j;
    StarsU16 matching;
    StarsU32 total;

    if (!target || !fleets) return 1;
    matching = 0U;
    total = 0UL;
    for (i = 0U; i < fleet_count; ++i) {
        if (!fleets[i].present) break;
        if (fleets[i].player == target->player &&
            fleets[i].x == target->x && fleets[i].y == target->y) {
            ++matching;
            for (j = 0U; j < 16U; ++j)
                total += (StarsU32)fleets[i].ship_counts[j];
        }
    }
    if (matching == 1U) return 0;
    if (total > 0xffffUL ||
        0x7ffeL - ((StarsI32)player_fleet_count - 1L) < (StarsI32)total)
        return 0;
    return 1;
}

/* NE 11:400E-40F1. Count earlier fleets at the selected point, excluding
 * the selected fleet itself, then refresh the dropdown and ship preview.
 * The bounded typed array replaces rglpfl and the two callbacks are the
 * original USER::SendMessage and DrawPlanShip boundaries. */
void stars_set_fleet_dropdown_sel(StarsU16 window, StarsU16 id,
    StarsI16 selected_group, StarsU16 selected_fleet, StarsI16 selected_x,
    StarsI16 selected_y, const StarsDropDownFleet *fleets,
    StarsU16 fleet_count, const StarsSetFleetDropDownOps *ops, void *context)
{
    StarsU16 i;
    StarsU16 same_point;
    StarsU16 excluded;

    same_point = 0U;
    excluded = selected_group == 2 ? selected_fleet : 0xffffU;
    if (fleets) {
        for (i = 0U; i < fleet_count && fleets[i].id != id; ++i) {
            if (fleets[i].x == selected_x && fleets[i].y == selected_y &&
                fleets[i].id != excluded)
                ++same_point;
        }
    }
    if (ops && ops->send_message)
        ops->send_message(context, window, 0x40eU, same_point, 0UL);
    if (ops && ops->draw_plan_ship)
        ops->draw_plan_ship(context, 0U, 0x4004U);
}

/* NE 11:41B2-44B5. Start with the selected hull's fuel or cargo capacity,
 * then add the relevant installed component capacities. The component's
 * high byte is its multiplicity and its low byte is the subtype. */
StarsI16 stars_wt_max_ship_design_stat(const StarsShipDesignStat *design,
    const StarsHullStat *hull, StarsI16 statistic)
{
    StarsU16 i;
    StarsU16 subtype;
    StarsU16 count;
    StarsU16 result;

    if (!design || !hull) return 0;
    if (statistic == 1)
        result = hull->fuel_max;
    else if (statistic == 2)
        result = hull->cargo_max;
    else
        return 0;
    for (i = 0U; i < design->component_count; ++i) {
        const StarsShipComponentStat *component = &design->components[i];
        subtype = (StarsU16)(component->packed_count_type & 0xffU);
        count = (StarsU16)(component->packed_count_type >> 8);
        if (component->group == 0x1000U) {
            if (statistic == 1 && subtype == 5U)
                result = (StarsU16)(result + count * 250U);
            else if (statistic == 1 && subtype == 6U)
                result = (StarsU16)(result + count * 500U);
            else if (statistic == 2 && subtype == 2U)
                result = (StarsU16)(result + count * 50U);
            else if (statistic == 2 && subtype == 3U)
                result = (StarsU16)(result + count * 100U);
            else if (statistic == 2 && subtype == 4U)
                result = (StarsU16)(result + count * 250U);
        } else if (statistic == 1 && component->group == 0x0800U &&
                   subtype == 0x10U) {
            result = (StarsU16)(result + count * 200U);
        }
    }
    return (StarsI16)result;
}

/* NE 11:40F2-41B1. A fleet with the active-stat flag set contributes each
 * nonzero design count multiplied by that design's maximum fuel or cargo.
 * The other fleet states retain the original fixed sentinel result. */
StarsI32 stars_get_fleet_stat(const StarsFleetStatEntry *entries,
    StarsU16 entry_count, StarsU16 fleet_flags, StarsI16 statistic)
{
    StarsU16 i;
    StarsI32 total;

    if ((fleet_flags & 0xffU) != 7U) return 32000L;
    if (!entries) return 0L;
    total = 0L;
    for (i = 0U; i < entry_count; ++i) {
        StarsI16 per_design;
        if (entries[i].ship_count == 0) continue;
        per_design = stars_wt_max_ship_design_stat(entries[i].design,
            entries[i].hull, statistic);
        total += (StarsI32)entries[i].ship_count * (StarsI32)per_design;
    }
    return total;
}

/* NE 11:4DF4-4FA9. Validate a jettison record against the selected fleet's
 * coordinates, then subtract its selected mineral amounts from the planet.
 * The callback replaces the two FLookupFleet calls; the record exposes the
 * byte and long amount views that the original two record kinds use. */
StarsI16 stars_enum_calc_jettison(const StarsJettisonRecord *record,
    StarsI16 record_type, StarsU32 *planet_minerals, StarsU16 selected_fleet,
    StarsLookupJettisonFleet lookup_fleet, void *context)
{
    StarsI16 selected_x;
    StarsI16 selected_y;
    StarsI16 source_x;
    StarsI16 source_y;
    StarsU16 selected_count;
    StarsU16 mask;
    StarsU16 i;

    if (!record || !planet_minerals || !lookup_fleet ||
        (record_type != 1 && record_type != 2)) return 1;
    if ((record->flags & 0x0fU) != 2U ||
        ((record->flags >> 4) & 0x0fU) != 4U)
        return 1;
    if (!lookup_fleet(context, selected_fleet, &selected_x, &selected_y) ||
        !lookup_fleet(context, record->fleet_id, &source_x, &source_y) ||
        selected_x != source_x || selected_y != source_y)
        return 1;

    selected_count = 0U;
    mask = record->mineral_mask;
    for (i = 0U; i < 5U; ++i) {
        if ((mask & 1U) != 0U) {
            StarsI32 amount;
            amount = record_type == 1 ?
                (StarsI32)record->byte_amounts[selected_count] :
                record->long_amounts[selected_count];
            planet_minerals[i] -= (StarsU32)amount;
            ++selected_count;
        }
        mask = (StarsU16)(mask >> 1);
    }
    return 1;
}

/* NE 11:64CC-66A7. Probe the endpoint in the direction of travel, clamp the
 * requested quantity to what it can release, apply the opposite endpoint,
 * and roll back the source by the amount actually accepted. */
StarsI32 stars_xfer_supply(StarsI16 supply, StarsI32 quantity,
    const StarsTransferEndpoint *endpoints, StarsChangeCargo change_cargo,
    void *context)
{
    StarsU16 direction;
    StarsI32 available;
    StarsI32 result;

    if (quantity == 0 || !endpoints || !change_cargo) return 0L;
    direction = quantity > 0L ? 1U : 0U;
    if (quantity < 0L)
        quantity = (StarsI32)(0UL - (StarsU32)quantity);
    available = change_cargo(context, endpoints[direction].group,
        endpoints[direction].id, supply, 0L, endpoints[direction].object);
    if (available < quantity) quantity = available;
    if (quantity == 0L) return 0L;
    result = change_cargo(context, endpoints[1U - direction].group,
        endpoints[1U - direction].id, supply, quantity,
        endpoints[1U - direction].object);
    if (result != 0L)
        change_cargo(context, endpoints[direction].group,
            endpoints[direction].id, supply, (StarsI32)(0L - result),
            endpoints[direction].object);
    return result;
}

static StarsU32 cargo_magnitude(StarsI32 value)
{
    if (value < 0) return (StarsU32)(0UL - (StarsU32)value);
    return (StarsU32)value;
}

static StarsI32 cargo_clamp_at_zero(StarsU32 current, StarsI32 change)
{
    if (change < 0 && cargo_magnitude(change) > current)
        return (StarsI32)(0UL - current);
    return change;
}

static StarsU32 cargo_add(StarsU32 current, StarsI32 change)
{
    return current + (StarsU32)change;
}

/* NE 11:6034-64CB. Change a mineral, packet cargo, or fleet supply value,
 * preserving the original zero-query, lower-bound, capacity, and object
 * lookup/store behavior.  The callbacks are host boundaries for the three
 * FLookup* families and the fleet free-capacity helpers; all state mutation
 * itself is ordinary typed C. */
StarsI32 stars_change_cargo(StarsI16 object_group, StarsU16 id,
    StarsI16 supply, StarsI32 change, StarsCargoState *object,
    const StarsChgCargoOps *ops, void *context)
{
    StarsCargoState temporary;
    StarsCargoState *state;
    StarsU32 free_capacity;
    StarsI16 should_store;

    state = object;
    should_store = 0;
    if (!state) {
        if (object_group == 4) {
            memset(&temporary, 0, sizeof temporary);
            state = &temporary;
        } else if (ops && ops->lookup) {
            state = ops->lookup(context, object_group, id);
        }
        if (!state) return change;
        should_store = 1;
    }

    if (object_group == 1 || object_group == 4) {
        if (supply < 5) {
            if (supply == 4) return 0L;
            if (change == 0L) return (StarsI32)state->minerals[supply];
            change = cargo_clamp_at_zero(state->minerals[supply], change);
            state->minerals[supply] = cargo_add(state->minerals[supply], change);
        }
        if (should_store && object_group == 1 && change != 0L &&
            ops && ops->store)
            ops->store(context, object_group, id, state);
        return change;
    }

    if (object_group == 8) {
        if (supply > 2) return 0L;
        if (change == 0L) return (StarsI32)state->cargo[supply];
        change = cargo_clamp_at_zero(state->cargo[supply], change);
        free_capacity = (StarsU32)(state->thing_capacity & 0x3fffU) * 10UL;
        free_capacity -= (StarsU32)state->cargo[0];
        free_capacity -= (StarsU32)state->cargo[1];
        free_capacity -= (StarsU32)state->cargo[2];
        if (change > 0L && (StarsU32)change > free_capacity)
            change = (StarsI32)free_capacity;
        state->cargo[supply] = (StarsU16)(state->cargo[supply] +
            (StarsU16)change);
        if (should_store && change != 0L && ops && ops->store)
            ops->store(context, object_group, id, state);
        return change;
    }

    if (supply < 5) {
        if (change == 0L) return (StarsI32)state->minerals[supply];
        change = cargo_clamp_at_zero(state->minerals[supply], change);
        if (supply == 3 && (state->fleet_flags & 0x00ffU) != 7U)
            change = 0L;
        free_capacity = 0UL;
        if (ops) {
            if (supply == 4 && ops->fuel_free)
                free_capacity = ops->fuel_free(context, state);
            else if (supply != 4 && ops->cargo_free)
                free_capacity = ops->cargo_free(context, state);
        }
        if (change > 0L && (StarsU32)change >= free_capacity)
            change = (StarsI32)free_capacity;
        state->minerals[supply] = cargo_add(state->minerals[supply], change);
    }
    if (should_store && change != 0L && ops && ops->store)
        ops->store(context, object_group, id, state);
    return change;
}

/* NE 11:66A8-6BE9. Refresh the disabled bit on active transfer buttons.
 * The availability callback represents either the ship-count word used by
 * the ship-transfer dialog or ChgCargo/Get*Free for the cargo dialog. */
void stars_update_xfer_buttons(StarsI16 dialog_mode, StarsI16 valid_hulls,
    StarsTransferButton *buttons, StarsU16 button_count,
    StarsTransferAvailability availability, void *context)
{
    StarsI16 maximum_supply;
    StarsU16 i;

    if (!buttons) return;
    maximum_supply = dialog_mode == 1 ? (StarsI16)(valid_hulls * 2) : 4;
    for (i = 0U; i < button_count; ++i) {
        StarsU16 side;
        StarsI32 amount;
        if ((buttons[i].flags & 1U) == 0U ||
            (dialog_mode != 1 && buttons[i].supply > maximum_supply))
            continue;
        side = (StarsU16)((buttons[i].flags >> 2) & 3U);
        amount = availability ? availability(context, side,
            buttons[i].supply, dialog_mode) : 0L;
        if (amount == 0L)
            buttons[i].button_state = (StarsU16)(buttons[i].button_state | 4U);
        else
            buttons[i].button_state = (StarsU16)(buttons[i].button_state & 0xfffbU);
    }
}

/* NE 11:6908-6BEA. Acquire a DC only when the caller supplies zero, paint
 * the divider/buttons for a negative supply request, then dispatch each
 * transfer side by dialog mode and object group. */
void stars_draw_xfer_dlg(StarsU16 window, StarsU16 hdc,
    const StarsRect *whole, StarsI16 supply, StarsI16 dialog_mode,
    StarsI16 arial_height,
    const StarsTransferButton *buttons, StarsU16 button_count,
    const StarsTransferSide *sides, const StarsDrawXferOps *ops,
    void *context)
{
    StarsI16 acquired;
    StarsU16 i;
    StarsRect side_rects[2];

    if (!whole || !ops) return;
    acquired = hdc == 0U;
    if (acquired) {
        if (!ops->acquire_dc) return;
        hdc = ops->acquire_dc(context, window);
    }
    if (supply < 0) {
        if (ops->paint_transfer)
            ops->paint_transfer(context, hdc, (StarsU16)(whole->right >> 1),
                0U, 1U, whole->bottom, 0x42U);
        if (buttons && ops->draw_button) {
            for (i = 0U; i < button_count; ++i)
                if ((buttons[i].flags & 1U) != 0U)
                    ops->draw_button(context, hdc, &buttons[i].rect,
                        buttons[i].button_state);
        }
        if (supply == -2) goto done;
    }
    stars_get_xfer_left_right_rects(whole, &side_rects[0], &side_rects[1],
        arial_height);
    if (sides) {
        for (i = 0U; i < 2U; ++i) {
            if (dialog_mode == 1) {
                if (ops->draw_ships)
                    ops->draw_ships(context, hdc, &side_rects[i],
                        sides[i].state, supply);
            } else if (sides[i].group == 2) {
                if (ops->draw_cargo)
                    ops->draw_cargo(context, hdc, &side_rects[i],
                        sides[i].state, supply);
            } else if (sides[i].group == 1 || sides[i].group == 4) {
                if (ops->draw_planet)
                    ops->draw_planet(context, hdc, &side_rects[i],
                        sides[i].state, supply);
            } else if (sides[i].group == 8 && ops->draw_thing) {
                ops->draw_thing(context, hdc, &side_rects[i],
                    sides[i].state, supply);
            }
        }
    }
done:
    if (acquired && ops->release_dc)
        ops->release_dc(context, window, hdc);
}

/* NE 11:6BEA-7087. Build the central transfer-button rows, add the
 * non-ship side controls when required, then run the same availability pass
 * as UpdateXferBtns. side_groups[0..1] replaces pxfer[].grobj and the map
 * replaces rgXferValidHulls. */
StarsI16 stars_setup_xfer_buttons(const StarsRect *whole,
    StarsI16 dialog_mode, StarsI16 valid_hulls, StarsI16 arial_height,
    const StarsI16 *side_groups, const StarsU16 *valid_hull_map,
    StarsTransferButton *buttons, StarsU16 button_capacity,
    StarsTransferAvailability availability, void *context)
{
    StarsI16 row_y;
    StarsI16 side_offset;
    StarsI16 row_count;
    StarsI16 has_thing;
    StarsI16 side;
    StarsU16 midpoint;
    StarsU16 i;
    StarsU16 button_count;
    StarsRect side_rects[2];
    StarsRect rect;

    if (!whole || !buttons || button_capacity == 0U) return 0;
    midpoint = (StarsU16)(whole->right >> 1);
    has_thing = side_groups && side_groups[1] == 8;
    row_y = (StarsI16)(arial_height + 10);
    row_count = dialog_mode == 1 ? valid_hulls : 5;
    if (row_count < 0) row_count = 0;
    if (dialog_mode != 1)
        row_y = (StarsI16)(row_y + (arial_height + 6) * 2);
    button_count = 0U;
    for (i = 0U; i < (StarsU16)row_count; ++i) {
        if (i == 4U && dialog_mode != 1)
            row_y = (StarsI16)(row_y - (arial_height + 6) * 6);
        rect.left = (StarsU16)(midpoint - (StarsU16)(arial_height + 3) + 1U);
        rect.top = (StarsU16)row_y;
        rect.right = (StarsU16)(midpoint + 1U);
        rect.bottom = (StarsU16)(row_y + arial_height + 3);
        for (side = 0; side < 2; ++side) {
            if (button_count >= button_capacity) return 0;
            buttons[button_count].rect = rect;
            buttons[button_count].button_state = (StarsU16)(side == 0 ? 2 : 3);
            if (has_thing && dialog_mode != 1 && i >= 4U) {
                buttons[button_count].flags = 0U;
                buttons[button_count].rect.bottom = (StarsU16)-100;
            } else {
                buttons[button_count].flags = 1U;
            }
            buttons[button_count].flags = (StarsU16)(
                (buttons[button_count].flags & 0xfff3U) |
                ((StarsU16)side << 2));
            buttons[button_count].supply = (StarsI16)(
                dialog_mode == 1 && valid_hull_map ?
                valid_hull_map[i] : i);
            ++button_count;
            rect.left = (StarsU16)(rect.left + (StarsU16)(arial_height + 2));
            rect.right = (StarsU16)(rect.right + (StarsU16)(arial_height + 2));
        }
        row_y = (StarsI16)(row_y + arial_height + 6);
    }

    if (dialog_mode != 1) {
        side_offset = 0x4b;
        stars_get_xfer_left_right_rects(whole, &side_rects[0],
            &side_rects[1], arial_height);
        if (side_groups && side_groups[0] == 1) {
            if (side_groups[1] == 1) goto refresh;
            side = 1;
        } else {
            side = 0;
        }
        for (; side < 2; ++side) {
            rect.left = (StarsU16)(side_rects[side].left + side_offset + 10);
            rect.top = (StarsU16)(side_rects[side].top + arial_height + 6);
            rect.right = (StarsU16)(side_rects[side].right - 4U);
            rect.bottom = (StarsU16)(side_rects[side].top + arial_height * 2 + 6);
            for (i = 0U; i < 5U; ++i) {
                if (i == 0U)
                    rect.top = (StarsU16)(rect.top + (arial_height + 6) * 2);
                else if (i == 4U)
                    rect.top = (StarsU16)(rect.top - (arial_height + 6) * 6);
                if (button_count >= button_capacity) return 0;
                buttons[button_count].rect = rect;
                buttons[button_count].button_state = 0U;
                buttons[button_count].flags = (StarsU16)(side << 2);
                buttons[button_count].supply = (StarsI16)(i + 0x80U);
                ++button_count;
                rect.top = (StarsU16)(rect.top + arial_height + 6);
            }
            if (side_groups && side_groups[1] == 1) break;
            side_offset = 0x4b;
        }
    }
refresh:
    stars_update_xfer_buttons(dialog_mode, valid_hulls, buttons, button_count,
        availability, context);
    return 1;
}

/* NE 11:7088-72DD. Render a thing transfer side, retaining the original
 * title/label geometry and the rule that supply rows 3 and 4 have no gauge.
 * Text, frame, and gauge operations are explicit host/UI boundaries. */
void stars_draw_thing_xfer_side(StarsU16 hdc, const StarsRect *area,
    const StarsCargoState *thing, StarsU16 thing_id, StarsI16 supply,
    StarsI16 arial_height, StarsU16 text_color, StarsU16 background_color,
    const StarsDrawThingXferOps *ops, void *context)
{
    StarsRect title;
    StarsRect gauge;
    const char *text;
    StarsI16 y;
    StarsI16 i;

    if (!area || !thing || !ops) return;
    title.left = area->left;
    title.top = area->top;
    title.right = area->right;
    title.bottom = (StarsU16)(title.top + title.right - title.left);
    if (ops->set_text_color)
        ops->set_text_color(context, hdc, text_color);
    if (ops->set_background_color)
        ops->set_background_color(context, hdc, background_color);
    if (ops->select_font) ops->select_font(context, hdc, 1U);
    if (supply == -1 && ops->draw_frame)
        ops->draw_frame(context, hdc, &title, 0U);
    stars_expand_rect(&title, -1, -1);
    title.bottom = (StarsU16)(title.top + arial_height + 2);
    if (supply == -1) {
        if (ops->draw_frame) ops->draw_frame(context, hdc, &title, 0U);
        if (ops->thing_name && ops->centered_text) {
            text = ops->thing_name(context, thing_id);
            ops->centered_text(context, hdc, &title, text, 0);
        }
    }
    if (supply == -1 && ops->label && ops->right_text) {
        y = (StarsI16)(title.bottom + arial_height + 9);
        for (i = -1; i < 3; ++i) {
            text = ops->label(context, (StarsI16)(i == -1 ? 0x1b2 : i + 0x1ae));
            ops->right_text(context, hdc, (StarsI16)(area->left + 0x4b), y,
                text, 0, 0);
            y = (StarsI16)(y + arial_height + 6);
        }
    }
    if (supply == 3 || supply == 4 || !ops->draw_gauge) return;
    gauge.left = (StarsU16)(area->left + 4 + 0x4b + 6);
    gauge.right = (StarsU16)(area->right - 4);
    gauge.top = (StarsU16)(title.bottom + arial_height + 9);
    gauge.bottom = (StarsU16)(gauge.top + arial_height);
    ops->draw_gauge(context, hdc, &gauge, thing, 5);
    for (i = 0; i < 3; ++i) {
        stars_offset_rect(&gauge, 0, (StarsI16)(arial_height + 6));
        if (supply == -1 || supply == i)
            ops->draw_gauge(context, hdc, &gauge, thing, i);
    }
}

/* NE 11:72DE-7725. Render the cargo side for a fleet. A fleet owned by the
 * current player uses gauges; an opposing fleet uses the original five
 * mineral boxes. The drawing and string operations remain typed callbacks. */
void stars_draw_fleet_cargo_xfer_side(StarsU16 hdc, const StarsRect *area,
    const StarsCargoState *fleet, StarsU16 fleet_id, StarsU16 current_player,
    StarsI16 supply, StarsI16 arial_height, StarsU16 text_color,
    StarsU16 background_color, const StarsDrawFleetCargoXferOps *ops,
    void *context)
{
    StarsRect title;
    StarsRect gauge;
    const char *text;
    StarsI16 y;
    StarsI16 i;
    StarsI16 same_owner;
    StarsI16 mineral;

    if (!area || !fleet || !ops) return;
    same_owner = fleet->owner == current_player;
    title.left = area->left;
    title.top = area->top;
    title.right = area->right;
    title.bottom = (StarsU16)(title.top + title.right - title.left);
    if (ops->set_text_color)
        ops->set_text_color(context, hdc, text_color);
    if (ops->set_background_color)
        ops->set_background_color(context, hdc, background_color);
    if (ops->select_font) ops->select_font(context, hdc, 1U);
    if (supply == -1 && ops->draw_frame)
        ops->draw_frame(context, hdc, &title, 0U);
    stars_expand_rect(&title, -1, -1);
    title.bottom = (StarsU16)(title.top + arial_height + 2);
    if (supply == -1) {
        if (ops->draw_frame) ops->draw_frame(context, hdc, &title, 0U);
        if (ops->fleet_name && ops->centered_text) {
            text = ops->fleet_name(context, fleet_id);
            ops->centered_text(context, hdc, &title, text, 0);
        }
        if (ops->label && ops->right_text) {
            y = (StarsI16)(title.bottom + 3);
            for (i = 0; i < 6; ++i) {
                text = ops->label(context, (StarsI16)(i + 0x1ac));
                ops->right_text(context, hdc, (StarsI16)(area->left + 0x4b),
                    y, text, 0, 0);
                y = (StarsI16)(y + arial_height + 6);
            }
        }
    }
    y = (StarsI16)(title.bottom + 3);
    if (same_owner) {
        gauge.left = (StarsU16)(area->left + 4 + 0x4b + 6);
        gauge.top = (StarsU16)y;
        gauge.right = (StarsU16)(area->right - 4);
        gauge.bottom = (StarsU16)(y + arial_height);
        if ((supply == -1 || supply == 4) && ops->draw_gauge)
            ops->draw_gauge(context, hdc, &gauge, fleet, 4);
        if (supply != 4 && ops->draw_gauge) {
            stars_offset_rect(&gauge, 0, (StarsI16)(arial_height + 6));
            ops->draw_gauge(context, hdc, &gauge, fleet, 5);
            for (i = 0; i < 4; ++i) {
                stars_offset_rect(&gauge, 0, (StarsI16)(arial_height + 6));
                if (supply == -1 || supply == i)
                    ops->draw_gauge(context, hdc, &gauge, fleet, i);
            }
        }
        return;
    }
    if (!ops->draw_mineral) return;
    gauge.left = (StarsU16)(area->left + 4 + 0x4b + 6);
    gauge.right = (StarsU16)(gauge.left + 0x44);
    gauge.top = (StarsU16)(title.bottom + 5);
    gauge.bottom = (StarsU16)(y + arial_height + 1);
    for (i = 0; i < 6; ++i) {
        if (i == 1) {
            stars_offset_rect(&gauge, 0, (StarsI16)(arial_height + 6));
        } else {
            mineral = i == 0 ? 4 : (StarsI16)(i - 2);
            if (supply == -1 || supply == mineral)
                ops->draw_mineral(context, hdc, &gauge, fleet, mineral,
                    supply == mineral);
            stars_offset_rect(&gauge, 0, (StarsI16)(arial_height + 6));
        }
    }
}

/* NE 11:7726-79C8. Render the ship-transfer side as a title followed by the
 * valid hull rows, preserving mapped hull labels/counts and selected-row
 * framing. */
void stars_draw_fleet_ships_xfer_side(StarsU16 hdc, const StarsRect *area,
    const StarsCargoState *fleet, StarsU16 fleet_id,
    const StarsU16 *valid_hull_map, StarsU16 valid_hull_count,
    StarsI16 supply, StarsI16 arial_height, StarsU16 text_color,
    StarsU16 background_color, const StarsDrawFleetShipsXferOps *ops,
    void *context)
{
    StarsRect title;
    StarsRect row;
    const char *text;
    StarsI16 y;
    StarsU16 i;
    StarsU16 hull;

    if (!area || !fleet || !ops) return;
    title = *area;
    if (ops->set_text_color)
        ops->set_text_color(context, hdc, text_color);
    if (ops->set_background_color)
        ops->set_background_color(context, hdc, background_color);
    if (ops->select_font) ops->select_font(context, hdc, 1U);
    if (supply == -1 && ops->draw_frame)
        ops->draw_frame(context, hdc, &title, 0U);
    stars_expand_rect(&title, -1, -1);
    title.bottom = (StarsU16)(title.top + arial_height + 2);
    if (supply == -1) {
        if (ops->draw_frame) ops->draw_frame(context, hdc, &title, 0U);
        if (ops->fleet_name && ops->centered_text) {
            text = ops->fleet_name(context, fleet_id);
            ops->centered_text(context, hdc, &title, text, 0);
        }
    }
    row.left = (StarsU16)(area->right - 0x44 - 6 - 2);
    row.right = (StarsU16)(row.left + 0x44 + 4);
    row.top = (StarsU16)(title.bottom + 2);
    row.bottom = (StarsU16)(title.bottom + 3 + arial_height + 1);
    y = (StarsI16)(title.bottom + 3);
    for (i = 0U; i < valid_hull_count; ++i) {
        hull = valid_hull_map ? valid_hull_map[i] : i;
        if (supply == -1 && ops->ship_label && ops->right_text) {
            text = ops->ship_label(context, (StarsI16)hull);
            ops->right_text(context, hdc, (StarsI16)(row.left - 6), y,
                text, 0, 0);
        }
        if (supply == -1 || supply == (StarsI16)i) {
            if (ops->draw_frame)
                ops->draw_frame(context, hdc, &row,
                    (StarsU16)(supply == (StarsI16)i));
            if (ops->right_text && ops->format_count) {
                text = ops->format_count(context, fleet->ship_counts[hull & 15U]);
                ops->right_text(context, hdc, (StarsI16)(row.right - 2), y,
                    text, 0, 0);
            }
            if (supply == (StarsI16)i) return;
        }
        stars_offset_rect(&row, 0, (StarsI16)(arial_height + 6));
        y = (StarsI16)(y + arial_height + 6);
    }
}

/* NE 11:79CA-7A98. Render a planet transfer side with its unknown-planet
 * title case, mineral labels, and four displayed mineral rows. */
void stars_draw_planet_xfer_side(StarsU16 hdc, const StarsRect *area,
    const StarsCargoState *planet, StarsI16 planet_id, StarsI16 supply,
    StarsI16 arial_height, StarsU16 text_color, StarsU16 background_color,
    StarsU16 dx_max_mineral, const StarsDrawPlanetXferOps *ops, void *context)
{
    StarsRect title;
    StarsRect row;
    const char *text;
    StarsI16 y;
    StarsI16 i;
    StarsI16 selected;

    if (!area || !planet || !ops) return;
    title = *area;
    title.bottom = (StarsU16)(title.top + title.right - title.left);
    if (ops->set_text_color)
        ops->set_text_color(context, hdc, text_color);
    if (ops->set_background_color)
        ops->set_background_color(context, hdc, background_color);
    if (ops->select_font) ops->select_font(context, hdc, 1U);
    if (supply == -1 && ops->draw_frame)
        ops->draw_frame(context, hdc, &title, 0U);
    stars_expand_rect(&title, -1, -1);
    title.bottom = (StarsU16)(title.top + arial_height + 2);
    if (supply == -1) {
        if (ops->draw_frame) ops->draw_frame(context, hdc, &title, 0U);
        if (ops->centered_text) {
            if (planet_id == -1 && ops->unknown_name)
                text = ops->unknown_name(context, 0x362);
            else if (ops->planet_name)
                text = ops->planet_name(context, planet_id);
            else
                text = 0;
            ops->centered_text(context, hdc, &title, text, 0);
        }
        if (ops->label && ops->right_text) {
            y = (StarsI16)(title.bottom + 3);
            for (i = 0; i < 6; ++i) {
                if (i > 1) {
                    text = ops->label(context, (StarsI16)(i + 0x1ac));
                    ops->right_text(context, hdc,
                        (StarsI16)(area->left + 4 + 0x4b), y, text, 0, 0);
                }
                y = (StarsI16)(y + arial_height + 6);
            }
        }
    }
    row.left = (StarsU16)(area->left + 4 + 0x4f);
    row.right = (StarsU16)(area->left + 4 + 0x51 + dx_max_mineral + 0x0e);
    row.top = (StarsU16)(title.bottom + 2);
    row.bottom = (StarsU16)(title.bottom + 4 + arial_height);
    y = (StarsI16)(title.bottom + 3);
    for (i = 0; i < 5; ++i) {
        if (i == 0) {
            stars_offset_rect(&row, 0, (StarsI16)((arial_height + 6) * 2));
            y = (StarsI16)(y + (arial_height + 6) * 2);
        } else if (i == 4) {
            stars_offset_rect(&row, 0, (StarsI16)((arial_height + 6) * -6));
            y = (StarsI16)(y - (arial_height + 6) * 6);
        }
        if (i != 4 && (supply == -1 || supply == i)) {
            selected = supply == i;
            if (ops->draw_frame)
                ops->draw_frame(context, hdc, &row, (StarsU16)selected);
            if (ops->draw_value)
                ops->draw_value(context, hdc, &row,
                    (StarsI32)planet->minerals[i], selected);
            if (supply == i) return;
        }
        stars_offset_rect(&row, 0, (StarsI16)(arial_height + 6));
        y = (StarsI16)(y + arial_height + 6);
    }
}

/* NE 11:6B46-6BEA. Split a transfer dialog rectangle at its midpoint,
 * inset both halves for the labels, and make room for the Arial line height
 * on the outside edges. */
void stars_get_xfer_left_right_rects(const StarsRect *whole,
    StarsRect *left, StarsRect *right, StarsI16 arial_height)
{
    StarsI16 inset;
    StarsU16 midpoint;

    if (!whole || !left || !right) return;
    inset = (StarsI16)(-7 - arial_height);
    midpoint = (StarsU16)(whole->right >> 1);
    left->left = 0U;
    left->top = 0U;
    left->right = midpoint;
    left->bottom = whole->bottom;
    stars_expand_rect(left, inset, -4);
    left->left = (StarsU16)(left->left - (StarsU16)(arial_height + 1));

    right->left = midpoint;
    right->top = 0U;
    right->right = whole->right;
    right->bottom = whole->bottom;
    stars_expand_rect(right, inset, -4);
    right->right = (StarsU16)(right->right + (StarsU16)(arial_height + 1));
}

/* NE 25:0148-01BE. The host supplies the already-resolved hull definition
 * and the original 16-entry popup visibility mask table. */
StarsI16 stars_is_popup_hull_type(StarsU16 plan_max,
    const StarsU8 *hull_definition, StarsU16 visibility,
    const StarsU16 *popup_masks)
{
    StarsU16 flags, index;
    if (plan_max == 0U || plan_max == 0xffU) return 1;
    if (!hull_definition || !popup_masks) return 0;
    flags = (StarsU16)(hull_definition[0x7bU] |
        ((StarsU16)hull_definition[0x7cU] << 8));
    index = (StarsU16)((flags >> 10) & 0x0fU);
    return (StarsI16)(popup_masks[index] & visibility);
}

/* NE 10:60E6-6127. FrameRect is the sole host/UI boundary; inflate is an
 * ABI parameter present in the original signature but unused by its body. */
void stars_handle_focus_state(StarsU16 item_state, StarsU16 hdc,
    const StarsRect *rect, StarsU16 brush, StarsI16 inflate,
    StarsFrameRect frame_rect, void *context)
{
    (void)inflate;
    if ((item_state & 0x0010U) != 0U && rect && frame_rect)
        frame_rect(context, hdc, rect, brush);
}

/* NE 17:0832-087F. The zip-order record is 24 bytes and its validity byte is
 * at offset 23; the two USER control operations remain host callbacks. */
void stars_enable_zip_buttons(StarsU16 hwnd, StarsI16 selection,
    const StarsU8 *zip_table, StarsGetDialogItem get_item,
    StarsEnableWindow enable, void *context)
{
    StarsU16 valid, control;
    const StarsU8 *record;
    if (!zip_table || !get_item || !enable || selection < 0) return;
    record = zip_table + (StarsU16)selection * 0x18U;
    valid = record[0x17U] ? 1U : 0U;
    control = get_item(context, hwnd, 0x817U);
    enable(context, control, valid);
    control = get_item(context, hwnd, 0x41bU);
    enable(context, control, valid);
}

/* NE 24:3064-3086. The four event procedures are host callbacks; their
 * original order is the complete body. */
void stars_random_events(StarsRandomEvent meteor, StarsRandomEvent climate,
    StarsRandomEvent minerals, StarsRandomEvent trader, void *context)
{
    if (meteor) meteor(context);
    if (climate) climate(context);
    if (minerals) minerals(context);
    if (trader) trader(context);
}

/* NE 19:6110-6151. Planet owner is word offset two; the starbase flag is
 * bit nine of the word at offset four. */
StarsI16 stars_enum_our_starbase(const StarsU8 *planet, StarsU16 player)
{
    StarsU16 owner, flags;
    if (!planet) return 0;
    owner = (StarsU16)(planet[2] | ((StarsU16)planet[3] << 8));
    flags = (StarsU16)(planet[4] | ((StarsU16)planet[5] << 8));
    return (StarsI16)(owner == player && (flags & 0x0200U) != 0U);
}

static StarsU16 record_owner(const StarsU8 *record)
{
    return (StarsU16)(record[2] | ((StarsU16)record[3] << 8));
}

/* NE 18:325C-3285. The source record parameter is unused; the test record's
 * owner word is compared against the current player. */
StarsI16 stars_enum_calc_enemy_fleet(const StarsU8 *fleet, StarsU16 player)
{
    return (StarsI16)(fleet && record_owner(fleet) != player);
}

/* NE 22:45A6-45DC. Unowned planets use owner -1 and are not enemy targets. */
StarsI16 stars_enum_calc_enemy_planet(const StarsU8 *planet, StarsU16 player)
{
    StarsU16 owner;
    if (!planet) return 0;
    owner = record_owner(planet);
    return (StarsI16)(owner != player && owner != 0xffffU);
}

/* NE 26:6716-6756. The original scans exactly 21 word entries beginning at
 * DGROUP:0C52 and returns the first matching index, with zero also denoting
 * no match. */
StarsI16 stars_empty_bmp_from_grhst(const StarsU8 *data, StarsI16 grhst)
{
    StarsU16 i;
    StarsU16 offset;
    StarsU16 value;
    if (!data) return 0;
    for (i = 0; i <= 20U; ++i) {
        offset = (StarsU16)(0x0c52U + i * 2U);
        value = (StarsU16)(data[offset] |
            ((StarsU16)data[(StarsU16)(offset + 1U)] << 8));
        if (value == (StarsU16)grhst) return (StarsI16)i;
    }
    return 0;
}

/* NE 32:6DA4-6DF3. The original compares signed bytes from the current
 * player's record: the low nibble is the current technology and the
 * arithmetic high-nibble shift is the next technology. A failed check sets
 * tutor.idh at DGROUP:5212 to the research-dialog error code. */
StarsI16 stars_check_research(StarsU8 *data, StarsI16 tech,
    StarsI16 tech_next, StarsI16 pct)
{
    StarsI16 current_tech;
    StarsI16 current_pct;

    if (!data) return 0;
    current_tech = (StarsI16)(signed char)data[0x59dbU];
    current_pct = (StarsI16)(signed char)data[0x59daU];
    if (((current_tech & 0x000f) == tech) &&
        ((current_tech >> 4) == tech_next) &&
        (current_pct == pct)) return 1;
    data[0x5212U] = 0x2eU;
    data[0x5213U] = 0x04U;
    return 0;
}

/* NE 19:60C0-610F. Selection is the host image of sel.fl; fleet is the
 * source fleet record. The two original global calls remain explicit hooks,
 * while the order-state mask update is performed on the supplied image. */
void stars_clear_ai_current_task(StarsU8 *selection, const StarsU8 *fleet,
    StarsI16 change_selection, StarsChangeMainObject change_main_object,
    StarsLookupFleet lookup_fleet, void *context)
{
    StarsU16 overlap;
    if (!selection) return;
    if (change_selection && fleet && change_main_object) {
        overlap = (StarsU16)(fleet[0] | ((StarsU16)fleet[1] << 8));
        change_main_object(context, 2, overlap);
    }
    selection[0x0aU] &= 0xf0U;
    selection[0x0bU] &= 0xffU;
    if (lookup_fleet) lookup_fleet(context, -1, selection);
}

/* NE 16:A320-A36F. CchGetString and the USER string setter are explicit host
 * boundaries. The temporary format text is kept local, while szWork remains
 * at its original DGROUP offset so callers observe the same formatted text. */
void stars_set_ngw_title(StarsU8 *data, StarsU16 hwnd, StarsI16 step,
    StarsGetString get_string, StarsFormatTitle format_title,
    StarsSetWindowText set_window_text, void *context)
{
    StarsU8 format[50];
    StarsU16 string_id;
    if (!data || !get_string || !format_title || !set_window_text) return;
    string_id = (StarsU16)(data[0x5320U] |
        ((StarsU16)data[0x5321U] << 8));
    string_id = (StarsU16)(string_id + 0x0110U);
    memset(format, 0, sizeof format);
    get_string(context, string_id, format);
    format_title(context, data + 0x57a4U, format, step);
    set_window_text(context, hwnd, data + 0x57a4U);
}

/* NE 32:6DF4-6F85. The host supplies the resolved fleet and PLORD images;
 * the order record layout is the recovered 18-byte _order type. TutorError
 * and FCheckSelection remain callbacks because both depend on wider tutor/UI
 * state, but the validator preserves their ordering and tutor.idh lifetime. */
StarsI16 stars_check_fleet_wp(StarsU8 *data, const StarsU8 *fleet,
    const StarsU8 *orders, StarsU16 ifl, StarsI16 order_index,
    StarsU16 object_group, StarsI16 object_id, StarsU16 task_group,
    StarsU16 warp,
    StarsTutorError tutor_error, StarsCheckSelection check_selection,
    void *context)
{
    StarsU16 saved_idh;
    StarsU16 current_idh;
    StarsU16 cord;
    const StarsU8 *order;
    StarsU16 packed;
    StarsI16 success = 0;

    if (!data) return 0;
    saved_idh = (StarsU16)(data[0x5212U] |
        ((StarsU16)data[0x5213U] << 8));
    current_idh = 0x05f6U;
    data[0x5212U] = 0xf6U;
    data[0x5213U] = 0x05U;
    if (fleet && orders) {
        cord = (StarsU16)(fleet[0x62U] |
            ((StarsU16)fleet[0x63U] << 8));
        if ((StarsI32)order_index + 1L <= (StarsI32)(StarsI16)cord) {
            current_idh = 0x0bf6U;
            data[0x5212U] = 0xf6U;
            data[0x5213U] = 0x0bU;
            order = orders + 4U + (StarsU16)order_index * 0x12U;
            packed = (StarsU16)(order[6] | ((StarsU16)order[7] << 8));
            if (((StarsU16)object_id & 0x7fffU) == 0x7fffU ||
                ((((packed >> 8) & 0x000fU) == object_group) &&
                 ((StarsI16)(order[4] | ((StarsU16)order[5] << 8)) == object_id))) {
                if ((packed & 0x000fU) == task_group || task_group == 0xffffU) {
                    current_idh = 0x05eeU;
                    data[0x5212U] = 0xeeU;
                    data[0x5213U] = 0x05U;
                    success = (StarsI16)(warp == 0xffffU ||
                        (((packed >> 4) & 0x000fU) == warp));
                } else {
                    current_idh = 0x05efU;
                    data[0x5212U] = 0xefU;
                    data[0x5213U] = 0x05U;
                    if ((packed & 0x000fU) != 0U && tutor_error)
                        tutor_error(context, (StarsI16)(task_group == 0U ?
                            0x01ed : 0x01ee));
                }
            } else {
                if (tutor_error) tutor_error(context, 0x01ec);
                current_idh = 0x0bf7U;
                data[0x5212U] = 0xf7U;
                data[0x5213U] = 0x0bU;
            }
        }
    }
    if (!success && check_selection && check_selection(context, 2, ifl) == 0) {
        current_idh = (StarsU16)(data[0x5212U] |
            ((StarsU16)data[0x5213U] << 8));
    }
    (void)current_idh;
    data[0x5212U] = (StarsU8)saved_idh;
    data[0x5213U] = (StarsU8)(saved_idh >> 8);
    return success;
}

/* NE 32:6F86-6FF2. The planet resolver is represented by the supplied host
 * record; a successful route check restores the caller's tutor code, while a
 * mismatch intentionally leaves the route-specific code visible. */
StarsI16 stars_check_planet_route(StarsU8 *data, const StarsU8 *planet,
    StarsI16 route)
{
    StarsU16 saved_idh;
    StarsU16 route_word;
    if (!data || !planet) return 0;
    saved_idh = (StarsU16)(data[0x5212U] |
        ((StarsU16)data[0x5213U] << 8));
    data[0x5212U] = 0xfbU;
    data[0x5213U] = 0x05U;
    route_word = (StarsU16)(planet[0x30U] |
        ((StarsU16)planet[0x31U] << 8));
    if ((route_word & 0x03ffU) != (StarsU16)(route + 1)) return 0;
    data[0x5212U] = (StarsU8)saved_idh;
    data[0x5213U] = (StarsU8)(saved_idh >> 8);
    return 1;
}

/* NE 32:6FF4-70BF. This is the original laying waypoint composition around
 * FCheckFleetWP: the ID high bit selects object group 1 or 2, task group six
 * is required, and the order's two-byte years field follows the point data. */
StarsI16 stars_check_laying_wp(StarsU8 *data, const StarsU8 *fleet,
    const StarsU8 *orders, StarsU16 ifl, StarsI16 order_index,
    StarsI16 id, StarsI16 years, StarsTutorError tutor_error,
    StarsCheckSelection check_selection, void *context)
{
    StarsU16 saved_idh;
    StarsU16 group;
    const StarsU8 *order;
    StarsI16 result;

    if (!data) return 0;
    saved_idh = (StarsU16)(data[0x5212U] |
        ((StarsU16)data[0x5213U] << 8));
    if (!fleet || !orders) return 0;
    group = (id & (StarsI16)0x8000U) ? 2U : 1U;
    result = stars_check_fleet_wp(data, fleet, orders, ifl, order_index,
        group, (StarsI16)((StarsU16)id & 0x7fffU), 6U, 0xffffU,
        tutor_error, check_selection, context);
    if (!result) return 0;
    order = orders + 4U + (StarsU16)order_index * 0x12U;
    if ((StarsI16)(order[8] | ((StarsU16)order[9] << 8)) != years) return 0;
    data[0x5212U] = (StarsU8)saved_idh;
    data[0x5213U] = (StarsU8)(saved_idh >> 8);
    return 1;
}

/* NE 32:690A-69E0. A null resolved fleet follows the original successful
 * sentinel path; an unnamed fleet only accepts the wildcard string ID. For a
 * named fleet, CchGetString and far case-insensitive comparison are explicit
 * host boundaries and the original tutor code is restored on equality. */
StarsI16 stars_check_fleet_name(StarsU8 *data, const StarsU8 *fleet_name,
    StarsI16 string_id, StarsGetString get_string,
    StarsCompareText compare_text, StarsTutorError tutor_error, void *context)
{
    StarsU16 saved_idh;
    StarsU8 expected[34];

    if (!data) return 0;
    saved_idh = (StarsU16)(data[0x5212U] |
        ((StarsU16)data[0x5213U] << 8));
    data[0x5212U] = 0xeeU;
    data[0x5213U] = 0x0bU;
    if (!fleet_name) return 1;
    if (string_id == -1) return 1;
    if (!get_string || !compare_text) return 0;
    memset(expected, 0, sizeof expected);
    get_string(context, (StarsU16)string_id, expected);
    if (compare_text(context, expected, fleet_name) == 0) {
        data[0x5212U] = (StarsU8)saved_idh;
        data[0x5213U] = (StarsU8)(saved_idh >> 8);
        return 1;
    }
    if (tutor_error) tutor_error(context, 0x04cf);
    return 0;
}

/* NE 32:685C-6909. Scan mode uses nibble matching below mode six and a
 * subset test for larger modes; tutor mode bypasses both scan constraints. */
StarsI16 stars_check_scanner(StarsU8 *data, StarsI16 mode, StarsI16 zoom)
{
    StarsU16 tutor_flags;
    StarsU16 scan_flags;
    StarsI16 scan_zoom;

    if (!data) return 0;
    tutor_flags = (StarsU16)(data[0x520cU] |
        ((StarsU16)data[0x520dU] << 8));
    if ((tutor_flags & 0x0200U) != 0U) return 1;
    scan_flags = (StarsU16)(data[0x0588U] |
        ((StarsU16)data[0x0589U] << 8));
    if (mode != -1 && scan_flags != (StarsU16)mode) {
        if (mode < 6) {
            if ((scan_flags & 0x000fU) != (StarsU16)mode) {
                data[0x5212U] = 0xb8U;
                data[0x5213U] = 0x36U;
                return 0;
            }
        } else if ((scan_flags & (StarsU16)mode) != (StarsU16)mode) {
            data[0x5212U] = 0xb8U;
            data[0x5213U] = 0x36U;
            return 0;
        }
    }
    scan_zoom = (StarsI16)(data[0x05a0U] |
        ((StarsU16)data[0x05a1U] << 8));
    if (zoom == -1 || zoom == scan_zoom) return 1;
    data[0x5212U] = 0xc6U;
    data[0x5213U] = 0x36U;
    return 0;
}

/* NE 32:67AE-685B. The tutor flags retain the original progress transition:
 * bits 6..8 advance by 0x40 on a repeated error, and a new/fully advanced
 * error clears them before formatting and alerting the message. */
void stars_tutor_error(StarsU8 *data, StarsI16 error_id,
    StarsFormatIds format_ids, StarsAlertText alert_text, void *context)
{
    StarsU16 flags;
    StarsU16 old_progress;
    StarsU16 next_progress;
    StarsI16 same_error;
    const StarsU8 *text;

    if (!data) return;
    flags = (StarsU16)(data[0x520cU] |
        ((StarsU16)data[0x520dU] << 8));
    if ((flags & 0x0020U) != 0U) {
        data[0x5214U] = 0xffU;
        data[0x5215U] = 0xffU;
        return;
    }
    same_error = (StarsI16)((StarsI16)(data[0x5214U] |
        ((StarsU16)data[0x5215U] << 8)) == error_id);
    old_progress = (StarsU16)(flags >> 6);
    next_progress = (StarsU16)((flags + 0x0040U) & 0x01c0U);
    flags = (StarsU16)((flags & 0xfe3fU) | next_progress);
    data[0x520cU] = (StarsU8)flags;
    data[0x520dU] = (StarsU8)(flags >> 8);
    if (same_error && (old_progress & 7U) < 3U) return;
    flags = (StarsU16)(flags & 0xfe3fU);
    data[0x520cU] = (StarsU8)flags;
    data[0x520dU] = (StarsU8)(flags >> 8);
    data[0x5214U] = (StarsU8)error_id;
    data[0x5215U] = (StarsU8)((StarsU16)error_id >> 8);
    if (!format_ids || !alert_text) return;
    text = format_ids(context, error_id);
    alert_text(context, text, 0x10);
}

/* NE 32:666E-67AD. Each template occupies 0x1a bytes: two header bytes and
 * twelve bytes of production-queue words. Tutor mode installs the template;
 * normal mode compares it against the existing ZIP-production record. */
StarsI16 stars_check_template(StarsU8 *data, const StarsU8 *templates,
    StarsI16 template_index)
{
    const StarsU8 *source;
    StarsU16 i;
    StarsU16 flags;
    StarsU16 count;

    if (!data || !templates) return 0;
    data[0x5212U] = 0x2dU;
    data[0x5213U] = 0x0cU;
    source = templates + (StarsU16)template_index * 0x1aU;
    flags = (StarsU16)(data[0x520cU] |
        ((StarsU16)data[0x520dU] << 8));
    if ((flags & 0x0200U) != 0U) {
        data[0x2303U] = 1U;
        for (i = 0; i < 13U; ++i) {
            data[(StarsU16)(0x2304U + i * 2U)] = source[i * 2U];
            data[(StarsU16)(0x2305U + i * 2U)] = source[i * 2U + 1U];
        }
        flags = (StarsU16)(data[0x07ceU] |
            ((StarsU16)data[0x07cfU] << 8));
        flags = (StarsU16)((flags & 0xffdfU) | 0x0020U);
        data[0x07ceU] = (StarsU8)flags;
        data[0x07cfU] = (StarsU8)(flags >> 8);
        return 1;
    }
    if (!data[0x2303U] || data[0x2304U] != source[0] ||
        data[0x2305U] != source[1]) return 0;
    count = data[0x2305U];
    for (i = 0; i < count; ++i) {
        if ((StarsU16)(data[(StarsU16)(0x2306U + i * 2U)] |
            ((StarsU16)data[(StarsU16)(0x2307U + i * 2U)] << 8)) !=
            (StarsU16)(source[2U + i * 2U] |
            ((StarsU16)source[3U + i * 2U] << 8))) return 0;
    }
    flags = (StarsU16)(data[0x07ceU] |
        ((StarsU16)data[0x07cfU] << 8));
    flags = (StarsU16)((flags & 0xffdfU) | 0x0020U);
    data[0x07ceU] = (StarsU8)flags;
    data[0x07cfU] = (StarsU8)(flags >> 8);
    return 1;
}

/* NE 32:69E2-6AF3. The host supplies the records already resolved from the
 * selected fleet/thing indices; the selection image retains the original
 * grobj, planet, fleet, thing, and sentinel fields. */
StarsI16 stars_check_summary(StarsU8 *data, StarsU16 object_group,
    StarsI16 id, const StarsU8 *fleet, const StarsU8 *thing)
{
    StarsU16 global_flags;
    StarsU16 selected_group;
    StarsU16 selected_planet;
    StarsI16 selected_fleet;
    StarsI16 result = 0;

    if (!data) return 0;
    global_flags = (StarsU16)(data[0x07caU] |
        ((StarsU16)data[0x07cbU] << 8));
    if ((global_flags & 0x0002U) != 0U) return 1;
    selected_group = (StarsU16)(data[0x4966U] |
        ((StarsU16)data[0x4967U] << 8));
    selected_planet = (StarsU16)(data[0x496aU] |
        ((StarsU16)data[0x496bU] << 8));
    selected_fleet = (StarsI16)(data[0x496cU] |
        ((StarsU16)data[0x496dU] << 8));
    if (object_group == 1U) {
        result = (StarsI16)(selected_group == 1U &&
            selected_planet == (StarsU16)id);
    } else if (object_group == 2U) {
        result = (StarsI16)(selected_group == 2U && selected_fleet != -1 &&
            fleet && (StarsU16)(fleet[0] | ((StarsU16)fleet[1] << 8)) ==
            (StarsU16)id);
    } else if (object_group == 8U) {
        result = (StarsI16)(selected_group == 8U &&
            (id == -1 || (thing &&
            (StarsU16)(thing[0] | ((StarsU16)thing[1] << 8)) ==
            (StarsU16)id)));
    }
    if (!result) {
        data[0x5212U] = 0xd2U;
        data[0x5213U] = 0x36U;
    }
    return result;
}

/* NE 32:6C48-6DA2. Message traversal and filter-group mutation are explicit
 * callbacks; the message filter bitset is supplied as its original byte
 * table. The tutor-id restoration follows each successful path exactly. */
StarsI16 stars_check_messages(StarsU8 *data, StarsI16 message,
    StarsI16 message_id, StarsI16 filter, const StarsU8 *filter_bits,
    StarsMessageNext message_next, StarsMessageId message_lookup,
    StarsSetFilteringGroups set_filtering_groups, void *context)
{
    StarsU16 saved_idh;
    StarsU16 global_flags;
    StarsU16 tutor_flags;
    StarsI16 current_message;
    StarsI16 result;
    StarsI16 bit_set;

    if (!data) return 0;
    saved_idh = (StarsU16)(data[0x5212U] |
        ((StarsU16)data[0x5213U] << 8));
    global_flags = (StarsU16)(data[0x07caU] |
        ((StarsU16)data[0x07cbU] << 8));
    if ((global_flags & 0x0002U) != 0U) {
        data[0x5212U] = (StarsU8)saved_idh;
        data[0x5213U] = (StarsU8)(saved_idh >> 8);
        return 1;
    }
    data[0x5212U] = 0xb1U;
    data[0x5213U] = 0x36U;
    current_message = (StarsI16)(data[0x0b00U] |
        ((StarsU16)data[0x0b01U] << 8));
    if (message == 9999 && message_next &&
        message_next(context, 0) != -1) return 0;
    if (message != 9999 && message != -1 && message > current_message) return 0;
    if (message_id == -1) {
        data[0x5212U] = (StarsU8)saved_idh;
        data[0x5213U] = (StarsU8)(saved_idh >> 8);
        return 1;
    }
    bit_set = (StarsI16)(filter_bits &&
        (filter_bits[(StarsU16)message_id >> 3] &
        (StarsU8)(1U << ((StarsU16)message_id & 7U))) != 0U);
    tutor_flags = (StarsU16)(data[0x520cU] |
        ((StarsU16)data[0x520dU] << 8));
    if (filter && (tutor_flags & 0x0200U) != 0U && !bit_set) {
        if (set_filtering_groups)
            set_filtering_groups(context, message_id, 1);
        data[0x5212U] = (StarsU8)saved_idh;
        data[0x5213U] = (StarsU8)(saved_idh >> 8);
        return 1;
    }
    if (filter && !bit_set) return 0;
    if (!filter && message_lookup &&
        message_lookup(context, current_message) != message_id) return 0;
    result = 1;
    if (result) {
        data[0x5212U] = (StarsU8)saved_idh;
        data[0x5213U] = (StarsU8)(saved_idh >> 8);
    }
    return result;
}

/* NE 32:6AF4-6C47. The selection image supplies the message and selected
 * object state; a resolved fleet record is used only for the cross planet /
 * fleet consistency check. Intermediate tutor codes are restored on return. */
StarsI16 stars_check_selection(StarsU8 *data, StarsU16 object_group,
    StarsI16 id, const StarsU8 *fleet)
{
    StarsU16 saved_idh;
    StarsU16 tutor_flags;
    StarsU16 global_flags;
    StarsU16 message_object;
    StarsI16 message_id;
    StarsU16 selected_group;
    StarsU16 selected_planet;
    StarsI16 selected_fleet;
    StarsI16 result = 0;
    StarsU16 compare_group;

    if (!data) return 0;
    saved_idh = (StarsU16)(data[0x5212U] |
        ((StarsU16)data[0x5213U] << 8));
    tutor_flags = (StarsU16)(data[0x520cU] |
        ((StarsU16)data[0x520dU] << 8));
    global_flags = (StarsU16)(data[0x07caU] |
        ((StarsU16)data[0x07cbU] << 8));
    if ((tutor_flags & 0x0200U) != 0U ||
        (global_flags & 0x0002U) != 0U) return 1;
    message_object = (StarsU16)(data[0x3ee2U] |
        ((StarsU16)data[0x3ee3U] << 8));
    message_id = (StarsI16)(data[0x3ee6U] |
        ((StarsU16)data[0x3ee7U] << 8));
    compare_group = object_group == 2U ? 2U : 1U;
    if (message_object == compare_group && id == message_id) {
        data[0x5212U] = 0xb1U;
        data[0x5213U] = 0x36U;
    } else {
        selected_group = (StarsU16)(data[0x495aU] |
            ((StarsU16)data[0x495bU] << 8));
        selected_planet = (StarsU16)(data[0x4978U] |
            ((StarsU16)data[0x4979U] << 8));
        selected_fleet = (StarsI16)(data[0x4972U] |
            ((StarsU16)data[0x4973U] << 8));
        if (object_group == 1U && selected_group == 2U &&
            selected_planet == (StarsU16)id) {
            data[0x5212U] = 0xeaU;
            data[0x5213U] = 0x05U;
        } else if (object_group == 2U && selected_group == 1U && fleet &&
            (StarsU16)(fleet[6] | ((StarsU16)fleet[7] << 8)) ==
            (StarsU16)selected_planet) {
            data[0x5212U] = 0xe6U;
            data[0x5213U] = 0x05U;
        } else {
            data[0x5212U] = 0xf6U;
            data[0x5213U] = 0x05U;
        }
    }
    selected_group = (StarsU16)(data[0x495aU] |
        ((StarsU16)data[0x495bU] << 8));
    selected_planet = (StarsU16)(data[0x49eeU] |
        ((StarsU16)data[0x49efU] << 8));
    selected_fleet = (StarsI16)(data[0x4972U] |
        ((StarsU16)data[0x4973U] << 8));
    if (object_group == 1U) {
        result = (StarsI16)(selected_group == 1U &&
            selected_planet == (StarsU16)id);
    } else if (object_group == 2U) {
        result = (StarsI16)(selected_group == 2U &&
            (StarsU16)selected_fleet == (StarsU16)id);
    }
    data[0x5212U] = (StarsU8)saved_idh;
    data[0x5213U] = (StarsU8)(saved_idh >> 8);
    return result;
}

/* NE 32:6460-6638. ZIP records are 24 bytes, with five packed item actions,
 * a 13-byte name field, and a validity byte at offset 23. Tutor mode copies
 * goal action high nibbles into the record; normal mode validates them and
 * compares the localized name through explicit host callbacks. */
StarsI16 stars_check_zip(StarsU8 *data, StarsI16 zip_index,
    const StarsU8 *goal, StarsI16 string_id, StarsGetString get_string,
    StarsCompareText compare_text, StarsTutorError tutor_error, void *context)
{
    StarsU16 flags;
    StarsU16 i;
    StarsU16 offset;
    StarsU16 zip_word;
    StarsU16 goal_word;
    StarsU8 text[34];
    StarsU8 *record;

    if (!data || !goal || zip_index < 0) return 0;
    record = data + 0x5264U + (StarsU16)zip_index * 0x18U;
    flags = (StarsU16)(data[0x520cU] |
        ((StarsU16)data[0x520dU] << 8));
    data[0x5212U] = 0x4aU;
    data[0x5213U] = 0x04U;
    if ((flags & 0x0200U) != 0U) {
        record[0x17U] = 1U;
        for (i = 0; i < 5U; ++i) {
            offset = (StarsU16)(i * 2U);
            zip_word = (StarsU16)(record[offset] |
                ((StarsU16)record[(StarsU16)(offset + 1U)] << 8));
            goal_word = (StarsU16)(goal[offset] |
                ((StarsU16)goal[(StarsU16)(offset + 1U)] << 8));
            zip_word = (StarsU16)((zip_word & 0x0fffU) |
                (goal_word & 0xf000U));
            record[offset] = (StarsU8)zip_word;
            record[(StarsU16)(offset + 1U)] = (StarsU8)(zip_word >> 8);
        }
        if (!get_string) return 1;
        memset(text, 0, sizeof text);
        get_string(context, (StarsU16)string_id, text);
        strcpy((char *)record + 10, (const char *)text);
        return 1;
    }
    if (!record[0x17U]) {
        if ((data[0x16b8U] | ((StarsU16)data[0x16b9U] << 8)) == 0U) {
            data[0x5212U] = 0xf0U;
            data[0x5213U] = 0x05U;
        }
        return 0;
    }
    data[0x5212U] = 0xefU;
    data[0x5213U] = 0x05U;
    for (i = 0; i < 5U; ++i) {
        offset = (StarsU16)(i * 2U);
        zip_word = (StarsU16)(record[offset] |
            ((StarsU16)record[(StarsU16)(offset + 1U)] << 8));
        goal_word = (StarsU16)(goal[offset] |
            ((StarsU16)goal[(StarsU16)(offset + 1U)] << 8));
        if ((zip_word >> 12) != 0U &&
            (zip_word >> 12) != (goal_word >> 12)) return 0;
    }
    if (!get_string || !compare_text) return 0;
    memset(text, 0, sizeof text);
    get_string(context, (StarsU16)string_id, text);
    if (compare_text(context, text, record + 10U) == 0) {
        return 1;
    }
    if (tutor_error) tutor_error(context, 0x0208);
    return 0;
}

static StarsI32 cargo_word(const StarsU8 *fleet, StarsU16 offset)
{
    StarsU32 value = (StarsU32)fleet[offset] |
        ((StarsU32)fleet[(StarsU16)(offset + 1U)] << 8) |
        ((StarsU32)fleet[(StarsU16)(offset + 2U)] << 16) |
        ((StarsU32)fleet[(StarsU16)(offset + 3U)] << 24);
    return value <= 0x7fffffffUL ? (StarsI32)value :
        -1L - (StarsI32)(~value);
}

/* NE 32:7664-77D7. The fleet's four minimum-weight longs are compared with
 * sign-extended 16-bit requirements; zero requirements reject nonzero cargo
 * and raise the original 0x01ef tutor error. */
StarsI16 stars_check_cargo(StarsU8 *data, const StarsU8 *fleet,
    StarsI16 minimum1, StarsI16 minimum2, StarsI16 minimum3,
    StarsI16 colonists, StarsTutorError tutor_error,
    StarsCheckSelection check_selection, void *context)
{
    StarsU16 saved_idh;
    StarsI16 result = 0;
    StarsI32 expected;
    StarsU16 fleet_id;
    StarsI16 selection_result;

    if (!data) return 0;
    saved_idh = (StarsU16)(data[0x5212U] |
        ((StarsU16)data[0x5213U] << 8));
    if (fleet) {
        data[0x5212U] = 0x33U;
        data[0x5213U] = 0x04U;
        if ((minimum1 == 0 && cargo_word(fleet, 76U) != 0L) ||
            (minimum2 == 0 && cargo_word(fleet, 80U) != 0L) ||
            (minimum3 == 0 && cargo_word(fleet, 84U) != 0L) ||
            (colonists == 0 && cargo_word(fleet, 88U) != 0L)) {
            if (tutor_error) tutor_error(context, 0x01ef);
        } else {
            expected = (StarsI32)minimum1;
            if (cargo_word(fleet, 76U) == expected) {
                expected = (StarsI32)minimum2;
                if (cargo_word(fleet, 80U) == expected) {
                    expected = (StarsI32)minimum3;
                    if (cargo_word(fleet, 84U) == expected) {
                        expected = (StarsI32)colonists;
                        if (cargo_word(fleet, 88U) == expected) result = 1;
                    }
                }
            }
        }
        if (!result && check_selection) {
            fleet_id = (StarsU16)(fleet[0] | ((StarsU16)fleet[1] << 8));
            selection_result = check_selection(context, 2, fleet_id);
            (void)selection_result;
        }
    }
    data[0x5212U] = (StarsU8)saved_idh;
    data[0x5213U] = (StarsU8)(saved_idh >> 8);
    return result;
}

/* NE 32:7964-79F4. The dialog/control/message operations remain host
 * callbacks; the validator preserves category and ship wildcards and only
 * restores the caller's tutor code on success. */
StarsI16 stars_check_ship_builder(StarsU8 *data, StarsU16 slot_dialog,
    StarsI16 build_mode, StarsI16 category, StarsI16 ship,
    StarsGetDialogItem get_dialog_item, StarsSendBuilderMessage send_message,
    void *context)
{
    StarsU16 saved_idh;
    StarsU16 control;
    StarsI32 selected_ship;

    if (!data) return 0;
    saved_idh = (StarsU16)(data[0x5212U] |
        ((StarsU16)data[0x5213U] << 8));
    data[0x5212U] = 0x2aU;
    data[0x5213U] = 0x04U;
    if (slot_dialog == 0U ||
        (category != -1 && category != build_mode) ||
        !get_dialog_item || !send_message) return 0;
    control = get_dialog_item(context, slot_dialog, 0x081aU);
    selected_ship = send_message(context, control, 0x0407U);
    if (ship == -1 || selected_ship == (StarsI32)ship) {
        data[0x5212U] = (StarsU8)saved_idh;
        data[0x5213U] = (StarsU8)(saved_idh >> 8);
        return 1;
    }
    return 0;
}

/* NE 32:77D8-7963. built_slots is the resolved hull's four-byte slot array;
 * goal_part is the two-word _hs record. The count is the high byte of the
 * packed second word, while the low byte and grhst are compared on success. */
StarsI16 stars_check_builder_part(StarsU8 *data, StarsU16 slot_dialog,
    StarsI16 build_mode, StarsI16 slot, const StarsU8 *built_slots,
    const StarsU8 *goal_part, StarsU16 initial_count,
    StarsTutorError tutor_error, void *context)
{
    StarsU16 saved_idh;
    StarsU16 built_word;
    StarsU16 goal_word;
    StarsU16 built_count;
    StarsU16 goal_count;
    StarsU16 built_grhst;
    StarsU16 goal_grhst;
    StarsU16 effective_count;
    const StarsU8 *built;

    if (!data) return 0;
    saved_idh = (StarsU16)(data[0x5212U] |
        ((StarsU16)data[0x5213U] << 8));
    if (slot_dialog == 0U) {
        data[0x5212U] = 0x2aU;
        data[0x5213U] = 0x04U;
        return 0;
    }
    data[0x5212U] = 0xe0U;
    data[0x5213U] = 0x0bU;
    if (build_mode != 4 || !built_slots || !goal_part || slot < 0) return 0;
    built = built_slots + (StarsU16)slot * 4U;
    built_grhst = (StarsU16)(built[0] | ((StarsU16)built[1] << 8));
    built_word = (StarsU16)(built[2] | ((StarsU16)built[3] << 8));
    goal_grhst = (StarsU16)(goal_part[0] |
        ((StarsU16)goal_part[1] << 8));
    goal_word = (StarsU16)(goal_part[2] |
        ((StarsU16)goal_part[3] << 8));
    built_count = (StarsU16)(built_word >> 8);
    goal_count = (StarsU16)(goal_word >> 8);
    if (goal_count == 0U && built_count == 0U) return 1;
    effective_count = initial_count;
    if (effective_count < goal_count) {
        if (built_count < effective_count) goto count_error;
        effective_count = goal_count;
    } else if (built_count < goal_count) goto count_error;
    if (built_count > effective_count) goto count_error;
    if (built_count != goal_count) goto count_mismatch;
    if (goal_grhst == built_grhst &&
        (goal_word & 0x00ffU) == (built_word & 0x00ffU)) {
        data[0x5212U] = (StarsU8)saved_idh;
        data[0x5213U] = (StarsU8)(saved_idh >> 8);
        return 1;
    }
    if (tutor_error) tutor_error(context, 0x01f6);
count_mismatch:
    data[0x5212U] = 0xdfU;
    data[0x5213U] = 0x0bU;
    return 0;
count_error:
    if (tutor_error) tutor_error(context, 0x01f5);
    data[0x5212U] = 0xdfU;
    data[0x5213U] = 0x0bU;
    return 0;
}

/* NE 32:7442-7608. A production record is two little-endian dwords; the
 * packed value's object group, item, and quantity fields are decoded with the
 * original 17-, 10-, and zero-bit shifts. */
StarsI16 stars_check_queue(StarsU8 *data, StarsI16 game_turn,
    StarsI16 planet_id, StarsI16 production_index, StarsU16 object_group,
    StarsU16 item, StarsU16 quantity, StarsU16 no_research,
    const StarsU8 *planet, const StarsU8 *queue, StarsU16 queue_count,
    StarsTutorError tutor_error, StarsCheckSelection check_selection,
    void *context)
{
    StarsU16 saved_idh;
    StarsU32 packed;
    StarsU32 research_word;
    StarsU16 decoded_group;
    StarsU16 decoded_item;
    StarsU16 decoded_quantity;
    StarsI16 result = 0;

    if (!data) return 0;
    saved_idh = (StarsU16)(data[0x5212U] |
        ((StarsU16)data[0x5213U] << 8));
    if (game_turn < 2) {
        data[0x5212U] = 0xe3U;
        data[0x5213U] = 0x05U;
    } else {
        data[0x5212U] = 0x23U;
        data[0x5213U] = 0x04U;
    }
    if (planet && queue && production_index >= 0 &&
        (StarsU16)production_index < queue_count) {
        queue += (StarsU16)production_index * 8U;
        packed = (StarsU32)queue[0] | ((StarsU32)queue[1] << 8) |
            ((StarsU32)queue[2] << 16) | ((StarsU32)queue[3] << 24);
        decoded_group = (StarsU16)((packed >> 17) & 7U);
        decoded_item = (StarsU16)((packed >> 10) & 0x7fU);
        decoded_quantity = (StarsU16)(packed & 0x03ffU);
        if (decoded_group == object_group && decoded_item == item) {
            if (decoded_quantity == quantity) {
                research_word = (StarsU32)planet[0x24U] |
                    ((StarsU32)planet[0x25U] << 8) |
                    ((StarsU32)planet[0x26U] << 16) |
                    ((StarsU32)planet[0x27U] << 24);
                if (no_research == 0xffffU ||
                    ((research_word >> 23) & 1U) == (StarsU32)no_research) {
                    result = 1;
                } else if (tutor_error) {
                    tutor_error(context, 0x0519);
                }
            } else if (tutor_error) {
                tutor_error(context, 0x01f0);
            }
        } else if (tutor_error) {
            tutor_error(context, 0x01eb);
        }
    }
    if (!result && check_selection)
        check_selection(context, 1, (StarsU16)planet_id);
    data[0x5212U] = (StarsU8)saved_idh;
    data[0x5213U] = (StarsU8)(saved_idh >> 8);
    return result;
}

/* NE 32:7280-7441. The fleet waypoint check supplies the prerequisite order
 * validation; the transfer order's five packed actions then compare against
 * goal actions, with low-12-bit equality required only for groups 4 and 8. */
StarsI16 stars_check_xfer_wp(StarsU8 *data, const StarsU8 *fleet,
    const StarsU8 *orders, StarsU16 ifl, StarsI16 order_index,
    StarsI16 object_id, StarsU16 warp, const StarsU8 *goal,
    StarsTutorError tutor_error, StarsCheckSelection check_selection,
    void *context)
{
    StarsU16 saved_idh;
    StarsU16 object_group;
    const StarsU8 *order;
    StarsU16 source_word;
    StarsU16 goal_word;
    StarsU16 i;
    StarsI16 result;

    if (!data) return 0;
    saved_idh = (StarsU16)(data[0x5212U] |
        ((StarsU16)data[0x5213U] << 8));
    object_group = ((StarsU16)object_id & 0x8000U) ? 2U : 1U;
    object_id = (StarsI16)((StarsU16)object_id & 0x7fffU);
    result = 0;
    if (fleet && orders && goal) {
        result = stars_check_fleet_wp(data, fleet, orders, ifl, order_index,
            object_group, object_id, 1U, warp, tutor_error, NULL, context);
        if (result) {
            order = orders + 4U + (StarsU16)order_index * 0x12U + 8U;
            data[0x5212U] = 0xefU;
            data[0x5213U] = 0x05U;
            result = 1;
            for (i = 0; i < 5U; ++i) {
                source_word = (StarsU16)(order[i * 2U] |
                    ((StarsU16)order[i * 2U + 1U] << 8));
                goal_word = (StarsU16)(goal[i * 2U] |
                    ((StarsU16)goal[i * 2U + 1U] << 8));
                if ((source_word >> 12) != (goal_word >> 12)) {
                    if ((source_word >> 12) != 0U && tutor_error)
                        tutor_error(context, 0x0268);
                    result = 0;
                    break;
                }
                if (((source_word >> 12) == 4U ||
                     (source_word >> 12) == 8U) &&
                    (source_word & 0x0fffU) != (goal_word & 0x0fffU)) {
                    result = 0;
                    break;
                }
            }
        }
    }
    if (!result && check_selection)
        check_selection(context, 2, ifl);
    data[0x5212U] = (StarsU8)saved_idh;
    data[0x5213U] = (StarsU8)(saved_idh >> 8);
    return result;
}

/* NE 32:70C0-71AB. Colonizer fleets consume a cargo requirement derived from
 * all sixteen cargo-count bytes; other fleets use the waypoint task-group-2
 * predicate. The resolved cargo and tutor operations remain host callbacks. */
StarsI16 stars_check_colonize_wp(StarsU8 *data, const StarsU8 *fleet,
    const StarsU8 *orders, StarsU16 ifl, StarsI16 object_id, StarsU16 warp,
    StarsColonizeCargoCheck cargo_check, StarsTutorError tutor_error,
    void *context)
{
    StarsU16 saved_idh;
    StarsU16 planet_id;
    StarsU16 i;
    StarsI16 cargo_total = 0;
    StarsI16 result;

    if (!data) return 0;
    saved_idh = (StarsU16)(data[0x5212U] |
        ((StarsU16)data[0x5213U] << 8));
    if (!fleet) return 0;
    data[0x5212U] = 0xf2U;
    data[0x5213U] = 0x05U;
    planet_id = (StarsU16)(fleet[6] | ((StarsU16)fleet[7] << 8));
    for (i = 0; i < 16U; ++i)
        cargo_total = (StarsI16)(cargo_total + fleet[(StarsU16)(12U + i)]);
    if (planet_id == 0x000dU) {
        result = cargo_check ? cargo_check(context, fleet,
            (StarsI16)(cargo_total * 25)) : 0;
    } else {
        result = stars_check_fleet_wp(data, fleet, orders, ifl, 1,
            1U, object_id, 2U, warp, tutor_error, NULL, context);
    }
    if (result) {
        data[0x5212U] = (StarsU8)saved_idh;
        data[0x5213U] = (StarsU8)(saved_idh >> 8);
        return 1;
    }
    return 0;
}

/* NE 32:71AC-727F. Patrol uses task group seven and checks the order distance
 * field after the fleet-waypoint prerequisite; iPlan is retained as an ABI
 * parameter because the original body does not read it. */
StarsI16 stars_check_patrol_wp(StarsU8 *data, const StarsU8 *fleet,
    const StarsU8 *orders, StarsU16 ifl, StarsI16 order_index,
    StarsI16 object_id, StarsU16 warp, StarsU16 plan, StarsU16 distance,
    StarsTutorError tutor_error, void *context)
{
    StarsU16 saved_idh;
    StarsU16 object_group;
    const StarsU8 *order;
    StarsI16 result;

    (void)plan;
    if (!data) return 0;
    saved_idh = (StarsU16)(data[0x5212U] |
        ((StarsU16)data[0x5213U] << 8));
    if (!fleet || !orders) return 0;
    data[0x5212U] = 0x17U;
    data[0x5213U] = 0x0cU;
    object_group = ((StarsU16)object_id & 0x8000U) ? 2U : 1U;
    object_id = (StarsI16)((StarsU16)object_id & 0x7fffU);
    result = stars_check_fleet_wp(data, fleet, orders, ifl, order_index,
        object_group, object_id, 7U, warp, tutor_error, NULL, context);
    if (!result) return 0;
    order = orders + 4U + (StarsU16)order_index * 0x12U;
    if (distance != 0xffffU &&
        (StarsU16)(order[10] | ((StarsU16)order[11] << 8)) != distance) {
        data[0x5212U] = 0xefU;
        data[0x5213U] = 0x05U;
        return 0;
    }
    data[0x5212U] = (StarsU8)saved_idh;
    data[0x5213U] = (StarsU8)(saved_idh >> 8);
    return 1;
}

/* NE 32:760A-7663. The original routine accepts the full battle-plan
 * validation tuple, but this body only bounds-checks the plan index against
 * the low byte of rgcbtlplan[0]. A valid index restores tutor.idh; a failed
 * index deliberately leaves the diagnostic code installed. */
StarsI16 stars_check_battle_plan(StarsU8 *data, StarsI16 plan_index,
    StarsU16 target_mode, StarsU16 spread, StarsU16 bomb, StarsU16 dump,
    StarsU16 unarmed_mode, StarsU16 scout_mode, StarsU16 war_mode,
    StarsU16 bomber_mode)
{
    StarsU16 saved_idh;
    StarsU16 plan_count;

    (void)target_mode;
    (void)spread;
    (void)bomb;
    (void)dump;
    (void)unarmed_mode;
    (void)scout_mode;
    (void)war_mode;
    (void)bomber_mode;
    if (!data) return 0;
    saved_idh = (StarsU16)(data[0x5212U] |
        ((StarsU16)data[0x5213U] << 8));
    data[0x5212U] = 0x21U;
    data[0x5213U] = 0x0cU;
    plan_count = (StarsU16)data[0x4830U];
    if (plan_index < 0 || (StarsU16)plan_index > plan_count)
        return 0;
    data[0x5212U] = (StarsU8)saved_idh;
    data[0x5213U] = (StarsU8)(saved_idh >> 8);
    return 1;
}

/* NE 32:81DA-82A6. Merge dialogs are valid on the two tutorial turns only:
 * turn 0x17 needs the first pair selected and the second pair empty, while
 * turn 0x1b needs the inverse. The original callback is kept at the host
 * boundary for the unsupported-turn diagnostic. */
StarsI16 stars_ok_merge_dialog(StarsU8 *data, const StarsU8 *merge,
    StarsTutorError tutor_error, void *context)
{
    StarsU16 turn;
    StarsI16 valid;

    if (!data || !merge) return 0;
    turn = stars_word(data, 0x0082U);
    valid = 0;
    if (turn == 0x0017U) {
        valid = (merge[0] != 0xffU || merge[1] != 0xffU) &&
            (merge[4] == 0xffU && merge[5] == 0xffU &&
             merge[6] == 0xffU && merge[7] == 0xffU);
    } else if (turn == 0x001bU) {
        valid = (merge[0] == 0xffU && merge[1] == 0xffU &&
                 merge[2] == 0xffU && merge[3] == 0xffU) &&
            (merge[4] != 0xffU || merge[5] != 0xffU ||
             merge[6] != 0xffU || merge[7] != 0xffU);
    } else {
        if (tutor_error) tutor_error(context, 0x0204);
        return 0;
    }
    if (valid) return 1;
    if (tutor_error) tutor_error(context, 0x0202);
    stars_put_word(data, 0x5212U, 0x0453U);
    return 0;
}

/* NE 33:5A14-5B7D. Tutor strings use one 64-entry index page per group.
 * Each entry is a compressed-byte count; the preceding entry bytes select a
 * nibble-aligned stream position, and runs of 0xf mean "keep accumulating"
 * before indexing the resource-character table. The last-read id is an
 * explicit host argument because the original iLastTutGet is a DGROUP global.
 */
StarsI16 stars_cch_tutor_string(StarsU8 *output, StarsI16 idt,
    StarsI16 last_id, const StarsU8 *base, const StarsU8 *env_offsets,
    const StarsU8 *encoded, const StarsU8 *resource_chars)
{
    StarsU16 raw_id;
    StarsU16 page;
    StarsU16 entry;
    StarsU16 prefix;
    StarsU16 count;
    StarsU16 run;
    StarsU16 i;
    StarsU8 high_nibble;
    const StarsU8 *stream;
    StarsU8 nibble;
    StarsU8 *cursor;

    if (!output || !base || !env_offsets || !encoded || !resource_chars)
        return 0;
    if (idt == last_id) {
        cursor = output;
        while (*cursor) ++cursor;
        return (StarsI16)(cursor - output);
    }
    if (idt < 0) return 0;
    raw_id = (StarsU16)idt;
    page = (StarsU16)(raw_id >> 6);
    entry = (StarsU16)(raw_id & 0x003fU);
    prefix = 0;
    for (i = 0; i < entry; ++i)
        prefix = (StarsU16)(prefix + base[(StarsU16)(page * 64U + i)]);
    stream = encoded + (StarsU16)(env_offsets[page * 2U] |
        ((StarsU16)env_offsets[page * 2U + 1U] << 8));
    stream += (StarsU16)(prefix >> 1);
    high_nibble = (StarsU8)((prefix & 1U) == 0U);
    count = base[(StarsU16)(page * 64U + entry)];
    run = 0;
    cursor = output;
    for (i = 0; i < count; ++i) {
        if (high_nibble) {
            nibble = (StarsU8)(*stream >> 4);
        } else {
            nibble = (StarsU8)(*stream & 0x0fU);
            ++stream;
        }
        high_nibble = (StarsU8)!high_nibble;
        run = (StarsU16)(run + nibble);
        if (nibble != 0x0fU) {
            *cursor++ = resource_chars[run];
            run = 0;
        }
    }
    *cursor = 0;
    return (StarsI16)(cursor - output);
}

/* NE 32:79F6-81D9. This is the tutorial ship-builder action dispatcher.
 * The original dialog checks are represented by explicit callbacks; the
 * supplied data image retains game.turn, tutor.idt, player design counts,
 * overlap flags, starbase mode, and the current design's class/bitmap. */
StarsI16 stars_tutorial_enabled_ship_builder(StarsU8 *data,
    StarsI16 action, StarsI16 ship_count, StarsU16 overlap_flags,
    StarsU16 starbase_mode, StarsU16 design_bitmap, const StarsU8 *expected_class,
    const StarsU8 *current_class, StarsTutorialShipCheck check_ship,
    StarsTutorialPartCheck check_part, StarsCompareText compare_class,
    StarsTutorError tutor_error, void *context)
{
    StarsU16 turn;
    StarsU16 tutor_id;
    StarsU16 design_count;
    StarsI16 result;

    if (!data) return 0;
    turn = stars_word(data, 0x0082U);
    tutor_id = stars_word(data, 0x520eU);
    design_count = data[0x59a3U];
    if (action == 0) {
        if (tutor_error) tutor_error(context, 0x01f1);
        return 0;
    }
    if (action == 1) {
        if (turn == 0x000dU && tutor_id == 0x0148U) {
            if (design_count == 7U) {
                if (tutor_error) tutor_error(context, 499);
                return 0;
            }
            result = check_ship ? check_ship(context, 1, 7) : 0;
            if (result) return 1;
            if (tutor_error) tutor_error(context, 0x01fe);
            return 0;
        }
        if (turn == 0x0014U && tutor_id == 0x01a0U) {
            if ((overlap_flags >> 12) == 2U) {
                if (tutor_error) tutor_error(context, 499);
                return 0;
            }
            result = check_ship ? check_ship(context, 0, 0) : 0;
            if (result && starbase_mode) return 1;
            if (tutor_error) tutor_error(context, 500);
            return 0;
        }
        if (turn == 0x0016U && tutor_id == 0x01c0U) {
            if (design_count == 8U) {
                if (tutor_error) tutor_error(context, 499);
                return 0;
            }
            result = check_ship ? check_ship(context, 1, 3) : 0;
            if (result) return 1;
            if (tutor_error) tutor_error(context, 0x01fe);
            return 0;
        }
        if (turn == 0x001bU && tutor_id == 0x0220U) {
            if (design_count == 9U) {
                if (tutor_error) tutor_error(context, 499);
                return 0;
            }
            result = check_ship ? check_ship(context, 1, 4) : 0;
            if (result) return 1;
            if (tutor_error) tutor_error(context, 0x01fe);
            return 0;
        }
        if (turn == 0x001dU && tutor_id == 0x0240U) {
            if (design_count == 10U) {
                if (tutor_error) tutor_error(context, 499);
                return 0;
            }
            result = check_ship ? check_ship(context, 1, 8) : 0;
            if (result) return 1;
            if (tutor_error) tutor_error(context, 0x01fe);
            return 0;
        }
        if (tutor_error) tutor_error(context, 0x01f2);
        return 0;
    }
    if (action == 2) {
        if (turn == 0x0019U && tutor_id == 0x01f0U) {
            result = check_ship ? check_ship(context, 0, 2) : 0;
            if (result) return result;
            if (tutor_error) tutor_error(context, 0x01ff);
            return 0;
        }
        if (tutor_error) tutor_error(context, 0x01f2);
        return 0;
    }
    if (action == 4) {
        if (tutor_error) tutor_error(context, 0x01f7);
        return 0;
    }
    if (action != 3) {
        if (tutor_error) tutor_error(context, 0x01f2);
        return 0;
    }

    if (turn == 0x000dU) {
        if (tutor_id != 0x0150U) {
            if (tutor_error) tutor_error(context, 0x01f8);
            return 0;
        }
        if (!check_part || !check_part(context, 0, 1, 0x0103, 1) ||
            !check_part(context, 1, 2, 0x0101, 1) ||
            !check_part(context, 2, 0x0080, 0x0102, 1) ||
            !check_part(context, 3, 0x0080, 0x0102, 1)) {
            if (tutor_error) tutor_error(context, 0x01fb);
            return 0;
        }
        return 1;
    }
    if (turn == 0x0014U) {
        if (tutor_id != 0x01a0U) {
            if (tutor_error) tutor_error(context, 0x01f8);
            return 0;
        }
        if (!check_part || !check_part(context, 0, 0x0200, 0x0100, 1)) {
            if (tutor_error) tutor_error(context, 0x0203);
            return 0;
        }
        if (!compare_class || !expected_class || !current_class ||
            compare_class(context, expected_class, current_class) != 0) {
            if (tutor_error) tutor_error(context, 0x0200);
            return 0;
        }
        if (design_bitmap != 0x0089U) {
            if (tutor_error) tutor_error(context, 0x01f9);
            return 0;
        }
        return 1;
    }
    if (turn == 0x0016U) {
        if (tutor_id != 0x01c0U) {
            if (tutor_error) tutor_error(context, 0x01f8);
            return 0;
        }
        if (!check_part || !check_part(context, 0, 1, 0x0104, 1) ||
            !check_part(context, 2, 0x0100, 0x0301, 3)) {
            if (tutor_error) tutor_error(context, 0x01fc);
            return 0;
        }
        if (!compare_class || !expected_class || !current_class ||
            compare_class(context, expected_class, current_class) != 0) {
            if (tutor_error) tutor_error(context, 0x0201);
            return 0;
        }
        return 1;
    }
    if (turn == 0x0019U) {
        if (!check_part || !check_part(context, 0, 1, 0x0104, 1) ||
            !check_part(context, 1, 0x1000, 0x0100, 1)) {
            if (tutor_error) tutor_error(context, 0x01fb);
            return 0;
        }
        return 1;
    }
    if (turn == 0x001bU) {
        if (tutor_id != 0x0220U) {
            if (tutor_error) tutor_error(context, 0x01f8);
            return 0;
        }
        if (!check_part || !check_part(context, 0, 1, 0x010a, 1) ||
            !check_part(context, 1, 0x0010, 0x0103, 1) ||
            !check_part(context, 2, 0x0010, 0x0103, 1) ||
            !check_part(context, 3, 0x0010, 0x0103, 1) ||
            !check_part(context, 4, 8, 0x0202, 2) ||
            !check_part(context, 5, 0x1000, 0x0105, 1) ||
            !check_part(context, 6, 0x0800, 0x0105, 1)) {
            if (tutor_error) tutor_error(context, 0x01fa);
            return 0;
        }
        if (design_bitmap != 0x0019U) {
            if (tutor_error) tutor_error(context, 0x01f9);
            return 0;
        }
        return 1;
    }
    if (turn == 0x001dU) {
        if (tutor_id != 0x0240U) {
            if (tutor_error) tutor_error(context, 0x01f8);
            return 0;
        }
        if (!check_part || !check_part(context, 0, 1, 0x020a, 2) ||
            !check_part(context, 1, 0x0040, 0x0401, 4) ||
            !check_part(context, 2, 0x0040, 0x0401, 4) ||
            !check_part(context, 3, 0x1000, 0x0105, 1)) {
            if (tutor_error) tutor_error(context, 0x01fd);
            return 0;
        }
        return 1;
    }
    if (tutor_error) tutor_error(context, 0x01f2);
    (void)ship_count;
    return 0;
}
#include <math.h>
#include <stdlib.h>
#include <string.h>

static StarsI32 signed_long(StarsU32 n)
{ return n <= 0x7fffffffUL ? (StarsI32)n : -1L - (StarsI32)(~n); }

/* NE 15:8ADC-8CFD. The player image is supplied with its original 0xC0
 * stride; marking a referenced ship design remains an explicit host callback
 * because the original table is a far pointer. */
void stars_mark_planet(StarsU8 *planet, StarsU8 *players, StarsU16 player_stride,
    StarsI16 mark_player, StarsU16 detail, StarsMarkDesign mark_design,
    void *context)
{
    StarsU16 flags = stars_word(planet, 4U);
    StarsI16 owner = (StarsI16)stars_word(planet, 2U);
    StarsU8 *marked_player;
    if (!(flags & 0x0100U)) {
        flags = (StarsU16)((flags & 0xfeffU) | 0x0100U);
        flags &= 0xff00U;
        if (players && mark_player >= 0)
            stars_put_word(players, (StarsU16)(mark_player * player_stride + 2U),
                (StarsU16)(stars_word(players,
                    (StarsU16)(mark_player * player_stride + 2U)) + 1U));
    }
    if ((flags & 0xffU) < (detail & 0xffU))
        flags = (StarsU16)((flags & 0xff00U) | (detail & 0xffU));
    stars_put_word(planet, 4U, flags);
    if (owner < 0 || !players) return;
    marked_player = players + (StarsU16)(owner * player_stride);
    if (!(stars_word(marked_player, 6U) & 0x0100U)) {
        StarsU16 player_flags = stars_word(marked_player, 6U);
        player_flags = (StarsU16)((player_flags & 0xfeffU) | 0x0100U);
        player_flags = (StarsU16)((player_flags & 0xfff8U) | 3U);
        stars_put_word(marked_player, 6U, player_flags);
    }
    if (detail != 2U && (flags & 0x0200U) && mark_design)
        mark_design(context, owner, (StarsU16)(stars_word(planet, 44U) & 0x000fU),
            stars_word(marked_player, 4U));
}

/* NE 15:885E-89B7. Design records are supplied as a contiguous host table;
 * their original 0x93-byte stride and component presence tests are retained. */
void stars_mark_fleet(StarsU8 *fleet, StarsU8 *players, StarsU16 player_stride,
    StarsU8 *designs, StarsU16 design_stride, StarsU16 detail)
{
    StarsU16 flags = stars_word(fleet, 4U);
    StarsI16 owner = (StarsI16)stars_word(fleet, 2U);
    StarsU16 i;
    if (!(flags & 0x0100U)) {
        StarsU8 *player;
        flags = (StarsU16)((flags & 0xfeffU) | 0x0100U);
        flags &= 0xff00U;
        stars_put_word(fleet, 118U, (StarsU16)((stars_word(fleet, 118U) & 0xffefU) | 0x0010U));
        if (players && owner >= 0) {
            player = players + (StarsU16)(owner * player_stride);
            stars_put_word(player, 4U, (StarsU16)((stars_word(player, 4U) & 0xf000U) | 1U));
        }
        if (designs) {
            for (i = 0; i < 16U; ++i) {
                if (fleet[12U + i]) {
                    StarsU16 offset = (StarsU16)(i * design_stride + 0x7bU);
                    stars_put_word(designs, offset,
                        (StarsU16)(stars_word(designs, offset) | 0x0100U));
                }
            }
        }
    }
    if ((flags & 0xffU) < (detail & 0xffU))
        flags = (StarsU16)((flags & 0xff00U) | (detail & 0xffU));
    stars_put_word(fleet, 4U, flags);
}

/* NE 9:2274-22b5, PszFromInt. */
StarsU16 stars_number_int(StarsU8 *d, StarsI16 n, StarsU16 count)
{
    StarsU16 length = (StarsU16)sprintf((char *)d + 0x590eU,
        (char *)d + stars_word(d, 0x1aaU), (int)n);
    if (count) stars_put_word(d, count, length);
    return 0x590eU;
}

/* NE 9:22b6-22fc, PszFromLong. The original tests *pcch, not pcch. Thus a
 * zero initial length remains zero, and offset zero is actually dereferenced. */
StarsU16 stars_number_long(StarsU8 *d, StarsI32 n, StarsU16 count)
{
    StarsU16 length = (StarsU16)sprintf((char *)d + 0x590eU,
        (char *)d + stars_word(d, 0x1b0U), (long)n);
    if (stars_word(d, count)) stars_put_word(d, count, length);
    return 0x590eU;
}

/* NE 9:22fe-243f, PszFromLongK. Appends over the formatter's terminator without
 * writing another NUL. Consumers use the returned count. Positive rounding
 * near INT32_MAX can wrap negative before the signed division. */
StarsU16 stars_number_scaled(StarsU8 *d, StarsI32 n, StarsU16 count)
{
    int thousands = n >= 10000L, millions = n >= 1000000L;
    StarsU16 at;
    if (millions) {
        n = signed_long((StarsU32)n + 500000UL) / 1000000L;
        if (n > 999L) n = 999;
    } else if (thousands) {
        n = signed_long((StarsU32)n + 500UL) / 1000L;
        if (n > 999L) n = 999;
    }
    stars_number_int(d, stars_signed_word((StarsU16)n), count);
    if (millions || thousands) {
        at = stars_word(d, count);
        stars_put_word(d, count, (StarsU16)(at + 1U));
        d[(StarsU16)(0x590eU + at)] = (StarsU8)(millions ? 'M' : 'k');
    }
    return 0x590eU;
}

/* NE 9:2440-2532, CommaFormatLong. The first group includes the minus sign. */
StarsU16 stars_number_commas(StarsU8 *d, StarsU16 out, StarsI32 n)
{
    char buffer[18];
    int length, group, i = 0;
    StarsU16 begin = out;
    length = sprintf(buffer, (char *)d + stars_word(d, 0x1b0U), (long)n);
    group = length % 3;
    if (!group) group = 3;
    else if (group == 1 && n < 0) group = 4;
    while (group-- > 0) d[out++] = (StarsU8)buffer[i++];
    while (buffer[i]) {
        d[out++] = ',';
        for (group = 0; group < 3; ++group) d[out++] = (StarsU8)buffer[i++];
    }
    d[out] = 0;
    return (StarsU16)(out - begin);
}

/* NE 8:5DBA-5E0E, GetTrueHullCost. iPlayer is an unused original
 * parameter; the hull record contains the three ore words and resource word
 * at the fixed CodeView offsets. */
void stars_get_true_hull_cost(const StarsU8 *hull, StarsU16 *cost)
{
    cost[0] = stars_word(hull, 44U);
    cost[1] = stars_word(hull, 46U);
    cost[2] = stars_word(hull, 48U);
    cost[3] = stars_word(hull, 42U);
}

/* NE 8:6280-62F8, LongFromSerialCh. The result is a sign-extended
 * 16-bit value, returned in the original 32-bit long representation. */
StarsI32 stars_long_from_serial_char(StarsI16 ch)
{
    StarsU16 value;
    if (ch < 'A' || ch > 'Z')
        value = (StarsU16)((StarsU16)ch - 0x16U);
    else
        value = (StarsU16)((StarsU16)ch - (StarsU16)'A');
    if (value & 0x8000U || (value < 1U && value < 0x20U))
        value = (StarsU16)(value ^ 0x15U);
    return (StarsI32)stars_signed_word(value);
}

static StarsI16 compare_signed_word_pair(StarsU16 low1, StarsI16 high1,
    StarsU16 low2, StarsI16 high2)
{
    if (high1 < high2 || (high1 == high2 && low1 < low2)) return -1;
    if (high1 > high2 || (high1 == high2 && low1 > low2)) return 1;
    return 0;
}

/* NE 8:1F0C-1FA2, ICompFleetPoint. The point is stored as a signed high
 * word and unsigned low word at record offsets 10 and 8. */
StarsI16 stars_compare_fleet_point(const StarsU8 *first, const StarsU8 *second)
{
    return compare_signed_word_pair(stars_word(first, 8U),
        stars_signed_word(stars_word(first, 10U)), stars_word(second, 8U),
        stars_signed_word(stars_word(second, 10U)));
}

/* NE 8:1FA2-2032, ICompFleetPoint2. The first operand is a point at
 * offsets 0/2; the second retains the fleet-record offsets above. */
StarsI16 stars_compare_fleet_point2(const StarsU8 *first, const StarsU8 *second)
{
    return compare_signed_word_pair(stars_word(first, 0U),
        stars_signed_word(stars_word(first, 2U)), stars_word(second, 8U),
        stars_signed_word(stars_word(second, 10U)));
}

/* NE 8:1B2-21E, LpthFromId. The original returns a far pointer; an element
 * index is the equivalent host representation and retains the first-match
 * walk through the contiguous thing records. */
StarsI16 stars_thing_index_from_id(const StarsU8 *things, StarsU16 count,
    StarsU16 stride, StarsI16 id)
{
    StarsU16 i;
    for (i = 0; i < count; ++i) {
        if (stars_signed_word(things[(StarsU16)(i * stride)] |
                ((StarsU16)things[(StarsU16)(i * stride + 1U)] << 8)) == id)
            return (StarsI16)i;
    }
    return -1;
}

/* NE 8:21E-2F6, LpplFromId. With all planet slots populated the original
 * uses the ID as the direct index; otherwise it binary-searches the sorted
 * ID field at offset zero. */
StarsI16 stars_planet_index_from_id(const StarsU8 *planets, StarsU16 count,
    StarsU16 maximum, StarsU16 stride, StarsI16 id)
{
    StarsI16 lo, hi, mid, current;
    if (id < 0 || (StarsU16)id >= maximum) return -1;
    if (count == maximum) return id;
    lo = -1;
    hi = (StarsI16)count;
    while (hi > lo + 1) {
        mid = (StarsI16)((lo + hi) / 2);
        current = stars_signed_word(planets[(StarsU16)(mid * stride)] |
            ((StarsU16)planets[(StarsU16)(mid * stride + 1U)] << 8));
        if (current < id) lo = mid;
        else if (current > id) hi = mid;
        else return mid;
    }
    return -1;
}

/* NE 8:2CE8-2D44, IflFromLpfl. Far pointers are represented by their raw
 * selector:offset DWORD values so equality includes both original words. */
StarsI16 stars_fleet_pointer_index(const StarsU32 *pointers, StarsU16 count,
    StarsU32 target)
{
    StarsU16 i;
    for (i = 0; i < count; ++i)
        if (pointers[i] == target) return (StarsI16)i;
    return -1;
}

/* NE 8:7F6A-7FE5, ClearFile. The unlink operation is an explicit host
 * service; the filename construction retains the original last-dot rule. */
StarsI16 stars_clear_file(const StarsU8 *base, const StarsU8 *extension,
    const StarsU8 *const *suffixes, StarsU16 suffix_count, StarsI16 type,
    StarsU8 *output, StarsU16 capacity, StarsUnlinkFile unlink_file,
    void *context)
{
    const StarsU8 *dot;
    size_t length;
    if (type < 0 || (StarsU16)type >= suffix_count || !unlink_file)
        return -1;
    length = strlen((const char *)base);
    if (length >= capacity) return -1;
    strcpy((char *)output, (const char *)base);
    dot = (const StarsU8 *)strrchr((char *)output, '.');
    if (dot) {
        output[(StarsU16)(dot - output) + 1U] = 0;
    } else {
        if (length + strlen((const char *)extension) + 1U > capacity)
            return -1;
        strcat((char *)output, (const char *)extension);
    }
    if (strlen((const char *)output) +
            strlen((const char *)suffixes[(StarsU16)type]) + 1U > capacity)
        return -1;
    strcat((char *)output, (const char *)suffixes[(StarsU16)type]);
    return unlink_file(context, output);
}

/* NE 8:F24-10F9, DpShieldOfShdef. Each component retains the original
 * four-byte shield slot: the high packed byte is quantity and the low byte
 * selects the special case. damage_points is the resolved part's dp field. */
StarsU32 stars_shield_power(const StarsShieldComponent *components,
    StarsU16 count, StarsI16 race_bonus)
{
    StarsU32 total = 0;
    StarsU16 i, quantity, subtype;
    for (i = 0; i < count; ++i) {
        quantity = (StarsU16)(components[i].packed_count_type >> 8);
        subtype = (StarsU16)(components[i].packed_count_type & 0xffU);
        if (components[i].kind == 4U && quantity != 0U)
            total += (StarsU32)components[i].damage_points * quantity;
        else if (components[i].kind == 8U && quantity != 0U && subtype == 6U)
            total += (StarsU32)quantity * 50U;
        else if (components[i].kind == 8U && subtype == 9U)
            total += (StarsU32)quantity * 100U;
    }
    if (total > 0xffffUL) total = 0xffffUL;
    if (race_bonus) total += (total * 2U) / 5U;
    return total;
}

/* NE 8:3E1C-3F00, IshdefPrimaryFromLpfl. The caller supplies the fleet's
 * sixteen ship counts and the corresponding hull design type words; the
 * global player design table is represented by the latter array. */
StarsI16 stars_primary_ship_design(const StarsU16 *counts,
    const StarsU16 *design_types, StarsU16 count, StarsI16 *different)
{
    StarsU16 i;
    StarsI16 active = 0, best = 0x10, largest = 0;
    for (i = 0; i < count; ++i) {
        if (counts[i] != 0U) {
            ++active;
            if (counts[i] > (StarsU16)largest) {
                largest = (StarsI16)counts[i];
                best = (StarsI16)i;
                if (design_types && (design_types[i] == 0x19U ||
                        design_types[i] == 0x1aU))
                    --largest;
            }
        }
    }
    if (different) *different = active;
    return best;
}

/* NE 8:2B10-2B5E, WFromLpfl. The low nine bits carry the fleet ID; the
 * primary design occupies the next field and multiple design types set bit
 * 13. */
StarsU16 stars_fleet_code(StarsU16 fleet_id, StarsI16 primary,
    StarsI16 different)
{
    StarsU16 code = (StarsU16)(((StarsU16)primary << 9) |
        (fleet_id & 0x01ffU));
    if (different > 1) code = (StarsU16)(code | 0x2000U);
    return code;
}

/* NE 8:10FA-11F2, IStargateFromLppl. The planet's selected design is chosen
 * from its low four-bit design field; each hull slot is four bytes with a
 * kind word followed by a quantity/type word. */
StarsI16 stars_stargate_index(const StarsU8 *planet, const StarsU8 *designs,
    StarsU16 design_stride, StarsU16 design_count)
{
    StarsU16 design, slots, i, kind, packed;
    if ((stars_word(planet, 4U) & 0x0200U) == 0) return -1;
    design = (StarsU16)(stars_word(planet, 44U) & 0x000fU);
    if (design >= design_count) return -1;
    slots = designs[(StarsU16)(design * design_stride + 0x7aU)];
    for (i = 0; i < slots; ++i) {
        kind = stars_word(designs, (StarsU16)(design * design_stride +
            0x3aU + i * 4U));
        packed = stars_word(designs, (StarsU16)(design * design_stride +
            0x3cU + i * 4U));
        if (kind == 0x0200U && (packed >> 8) != 0U &&
                (packed & 0xffU) < 7U)
            return (StarsI16)(packed & 0xffU);
    }
    return -1;
}

/* NE 8:7960-7A67, FFleetCanJumpgate. Every nonempty hull must expose a
 * quantity-bearing component kind 0x1000/type 9. */
StarsI16 stars_fleet_can_jumpgate(const StarsU16 *ship_counts,
    const StarsU8 *designs, StarsU16 design_stride, StarsU16 design_count)
{
    StarsU16 design, slots, i, kind, packed;
    for (design = 0; design < design_count; ++design) {
        if (ship_counts[design] == 0U) continue;
        slots = designs[(StarsU16)(design * design_stride + 0x7aU)];
        for (i = 0; i < slots; ++i) {
            kind = stars_word(designs, (StarsU16)(design * design_stride +
                0x3aU + i * 4U));
            packed = stars_word(designs, (StarsU16)(design * design_stride +
                0x3cU + i * 4U));
            if (kind == 0x1000U && (packed >> 8) != 0U &&
                    (packed & 0xffU) == 9U) break;
        }
        if (i == slots) return 0;
    }
    return 1;
}

/* NE 8:7A68-7B3F, WtFromLpfl. The caller supplies the already-resolved
 * per-design cargo weights and the fleet's four raw mineral words. */
StarsU32 stars_fleet_weight(const StarsU16 *ship_counts,
    const StarsU32 *design_weights, const StarsU32 *minerals)
{
    StarsU16 i;
    StarsU32 total = 0;
    for (i = 0; i < 16U; ++i)
        total += (StarsU32)ship_counts[i] * design_weights[i];
    for (i = 0; i < 4U; ++i) total += minerals[i];
    return total;
}

/* NE 8:6612-68C6, FMatchTarget. Hull classes are the resolved low-nibble
 * values from the original hull-definition records. */
StarsI16 stars_match_target(const StarsU16 *ship_counts,
    const StarsU8 *hull_classes, StarsU16 count, StarsI16 mode,
    StarsI16 exact)
{
    StarsU16 i;
    StarsI16 found = 0;
    if (mode == 4) {
        for (i = 0; i < count; ++i)
            if (ship_counts[i] && (hull_classes[i] == 1U ||
                    hull_classes[i] == 5U)) return 1;
        return 0;
    }
    if (mode == 6 || mode == 7) {
        for (i = 0; i < count; ++i)
            if (ship_counts[i] && ((mode == 6 && hull_classes[i] == 7U) ||
                    (mode == 7 && hull_classes[i] == 1U))) return 1;
        return 0;
    }
    if (mode != 3 && mode != 5)
        return exact ? 0 : 1;
    for (i = 0; i < count; ++i)
        if (ship_counts[i] && hull_classes[i] > 1U &&
                hull_classes[i] < 5U) found = 1;
    return mode == 3 ? found : !found;
}

/* NE 8:2078-2160, LpflFromId. The high four ID bits select a player's fleet
 * group; the low thirteen bits are searched in the globally sorted pointer
 * table. */
StarsI16 stars_fleet_index_from_grouped_id(const StarsU16 *fleet_ids,
    StarsU16 fleet_count, const StarsU16 *player_counts, StarsU16 player_count,
    StarsI16 player_group, StarsI16 id)
{
    StarsU16 i, start = 0;
    StarsI16 lo, hi, mid, target;
    if (player_group < 0 || (StarsU16)player_group >= player_count)
        return -1;
    for (i = 0; i < (StarsU16)player_group; ++i) start += player_counts[i];
    target = (StarsI16)((StarsU16)id & 0x1fffU);
    lo = (StarsI16)start;
    hi = (StarsI16)fleet_count;
    while (lo < hi) {
        mid = (StarsI16)((lo + hi) / 2);
        if ((StarsI16)fleet_ids[(StarsU16)mid] < target) lo = (StarsI16)(mid + 1);
        else if ((StarsI16)fleet_ids[(StarsU16)mid] > target) hi = mid;
        else return mid;
    }
    return -1;
}

/* NE 8:24A0-24FA, FLookupObject. The original dispatches by grobj and lets
 * each specialized lookup decide whether the ID exists. */
StarsI16 stars_lookup_object_index(StarsI16 grobj, StarsI16 id,
    const StarsU16 *planet_ids, StarsU16 planet_count,
    const StarsU16 *fleet_ids, StarsU16 fleet_count,
    const StarsU16 *thing_ids, StarsU16 thing_count)
{
    const StarsU16 *ids;
    StarsU16 count, i;
    if (grobj == 1) { ids = planet_ids; count = planet_count; }
    else if (grobj == 2) { ids = fleet_ids; count = fleet_count; }
    else { ids = thing_ids; count = thing_count; }
    for (i = 0; i < count; ++i)
        if (stars_signed_word(ids[i]) == id) return (StarsI16)i;
    return -1;
}

/* NE 8:0000-0031, FLookupSelPlanet. A selected scan object is a planet only
 * when grobj is one; the actual record operation is delegated to lookup. */
StarsI16 stars_selected_planet_index(StarsI16 grobj, StarsI16 id,
    const StarsU16 *planet_ids, StarsU16 count)
{
    StarsU16 i;
    if (grobj != 1) return -1;
    for (i = 0; i < count; ++i)
        if (stars_signed_word(planet_ids[i]) == id) return (StarsI16)i;
    return -1;
}

/* NE 8:2032-2078, FLookupSelShip. The scan index resolves to a fleet ID,
 * after which the normal fleet lookup searches the sorted fleet table. */
StarsI16 stars_selected_fleet_index(StarsI16 grobj, StarsU16 scan_index,
    const StarsU16 *selected_fleet_ids, StarsU16 selected_count,
    const StarsU16 *fleet_ids, StarsU16 fleet_count)
{
    StarsU16 i;
    if (grobj != 2 || scan_index >= selected_count) return -1;
    for (i = 0; i < fleet_count; ++i)
        if (fleet_ids[i] == selected_fleet_ids[scan_index]) return (StarsI16)i;
    return -1;
}

/* NE 8:07FE-08F1, FLookupThing. The thing record is eighteen bytes in the
 * original. Negative IDs update the stored record from the caller; ordinary
 * IDs copy the stored record into the caller. */
StarsI16 stars_lookup_thing_record(StarsI16 id, StarsU8 *things,
    StarsU16 count, StarsU16 stride, StarsU8 *record, StarsI16 update)
{
    StarsU16 i, wanted;
    if (count == 0U) return 0;
    wanted = id < 0 && record ? stars_word(record, 0U) : (StarsU16)id;
    for (i = 0; i < count; ++i) {
        StarsU8 *found = things + (StarsU16)(i * stride);
        if (stars_word(found, 0U) != wanted) continue;
        if (record) {
            if (update || id < 0) memcpy(found, record, 18U);
            else memcpy(record, found, 18U);
        }
        return 1;
    }
    return 0;
}

/* NE 8:7B40-7C5E, FCanBuildShdef. Part lookup is explicit because the
 * original temporarily changes idPlayer while resolving the hull and each
 * nonempty component. */
StarsI16 stars_can_build_design(StarsI16 hull_id,
    const StarsShieldComponent *components, StarsU16 count,
    StarsPartLookup lookup, void *context)
{
    StarsU16 hull_kind, hull_type, i, quantity;
    if (!lookup) return 0;
    if (hull_id < 0x20) {
        hull_kind = 0x4000U;
        hull_type = (StarsU16)hull_id & 0xffU;
    } else {
        hull_kind = 0x0400U;
        hull_type = (StarsU16)(hull_id - 0x20) & 0xffU;
    }
    if (lookup(context, hull_kind, hull_type) != 1) return 0;
    for (i = 0; i < count; ++i) {
        quantity = (StarsU16)(components[i].packed_count_type >> 8);
        if (quantity && lookup(context, components[i].kind,
                components[i].packed_count_type) != 1) return 0;
    }
    return 1;
}

/* NE 8:7C5E-7E43, IshFindSimilarDesign. candidate_stride is the number of
 * component slots reserved for each of the candidate designs. */
StarsI16 stars_find_similar_design(StarsU16 target_hull,
    const StarsU16 *target_kinds, const StarsU16 *target_packed,
    StarsU16 target_count, const StarsU16 *candidate_hulls,
    const StarsU16 *candidate_flags, const StarsU16 *candidate_counts,
    const StarsU16 *candidate_kinds, const StarsU16 *candidate_packed,
    StarsU16 candidate_count, StarsU16 candidate_stride)
{
    StarsU16 candidate, slot, target_value, candidate_value;
    int matches;
    for (candidate = 0; candidate < candidate_count; ++candidate) {
        if ((candidate_flags[candidate] & 0x0200U) != 0U ||
                (candidate_flags[candidate] & 0x8000U) == 0U ||
                candidate_hulls[candidate] != target_hull ||
                candidate_counts[candidate] != target_count)
            continue;
        matches = 1;
        for (slot = 0; slot < target_count; ++slot) {
            target_value = target_packed[slot];
            candidate_value = candidate_packed[candidate * candidate_stride + slot];
            if ((target_value >> 8) != (candidate_value >> 8)) {
                matches = 0;
                break;
            }
            if ((target_value >> 8) != 0U &&
                    (target_value & 0xffU) != (candidate_value & 0xffU)) {
                matches = 0;
                break;
            }
            if ((target_value >> 8) != 0U &&
                    target_kinds[slot] != candidate_kinds[
                        candidate * candidate_stride + slot]) {
                matches = 0;
                break;
            }
        }
        if (matches) return (StarsI16)candidate;
    }
    return -1;
}

static void roman_small(StarsI16 value, char *out)
{
    static const StarsI16 values[5] = {10, 9, 5, 4, 1};
    static const char *digits[5] = {"X", "IX", "V", "IV", "I"};
    StarsU16 i, at = 0;
    while (value > 0) {
        for (i = 0; i < 5U; ++i) {
            if (value >= values[i]) {
                const char *p = digits[i];
                while (*p) out[at++] = *p++;
                value = (StarsI16)(value - values[i]);
                break;
            }
        }
    }
    out[at] = 0;
}

/* NE 8:5E0E-5F7E, DecorateHullName. Names are fixed-width host records;
 * flags bit 9 marks a hidden design. */
void stars_decorate_hull_name(const StarsU8 *names, StarsU16 stride,
    const StarsU16 *flags, StarsU16 player, StarsU16 current_player,
    StarsU16 hull, StarsU16 count, StarsU8 *output, StarsU16 capacity)
{
    StarsU16 i, duplicates = 0, ordinal = 1, length;
    char roman[8];
    const StarsU8 *name;
    if (hull >= count || (flags[hull] & 0x0200U) != 0U || capacity == 0U) {
        if (capacity) output[0] = 0;
        return;
    }
    name = names + hull * stride;
    length = 0;
    while (length < 28U && length + 1U < capacity && name[length]) {
        output[length] = name[length];
        ++length;
    }
    output[length] = 0;
    if (player == current_player) return;
    for (i = 0; i < count; ++i) {
        if (i == hull || (flags[i] & 0x0200U) != 0U) continue;
        if (strcmp((const char *)name, (const char *)(names + i * stride)) == 0) {
            ++duplicates;
            if (i < hull) ++ordinal;
        }
    }
    if (!duplicates) return;
    roman_small((StarsI16)ordinal, roman);
    if (length + 4U < capacity) {
        output[length++] = ' '; output[length++] = '(';
        strcpy((char *)output + length, roman);
        length = (StarsU16)(length + strlen(roman));
        if (length + 2U < capacity) {
            output[length++] = ')'; output[length] = 0;
        }
    }
}

/* NE 8:80C2-8126, TurnLog. Format resolution is supplied by the caller;
 * OutputSz's original channel is retained as the callback's channel. */
StarsI16 stars_turn_log(StarsI16 logging_enabled, StarsI16 string_id,
    StarsI16 turn, const StarsU8 *format, StarsU8 *output, StarsU16 capacity,
    StarsLogOutput emit, void *context)
{
    int length;
    (void)string_id;
    if (logging_enabled >= 0 || !format || !output || capacity == 0U || !emit)
        return 0;
    length = sprintf((char *)output, (const char *)format, (int)turn + 0x961);
    if (length < 0 || (StarsU16)length >= capacity) return 0;
    emit(context, 6, output);
    return 1;
}

/* NE 9:2f70-3093, BoundPoints. Reads point zero even when count <= zero;
 * one-pixel expansion wraps at the signed 16-bit coordinate boundaries. */
void stars_bound_points(StarsU8 *d, StarsU16 rect, StarsU16 points, StarsI16 count)
{
    StarsI16 x, y, left, top, right, bottom, i;
    left = right = stars_signed_word(stars_word(d, points));
    top = bottom = stars_signed_word(stars_word(d, (StarsU16)(points + 2U)));
    for (i = 1; i < count; ++i) {
        x = stars_signed_word(stars_word(d, (StarsU16)(points + (StarsU16)i * 4U)));
        y = stars_signed_word(stars_word(d, (StarsU16)(points + (StarsU16)i * 4U + 2U)));
        if (x <= left) left = x;
        if (x >= right) right = x;
        if (y <= top) top = y;
        if (y >= bottom) bottom = y;
    }
    stars_put_word(d, (StarsU16)(rect + 2U), (StarsU16)((StarsU16)top - 1U));
    stars_put_word(d, (StarsU16)(rect + 6U), (StarsU16)((StarsU16)bottom + 1U));
    stars_put_word(d, rect, (StarsU16)((StarsU16)left - 1U));
    stars_put_word(d, (StarsU16)(rect + 4U), (StarsU16)((StarsU16)right + 1U));
}

/* NE 8:4ba2-4c00, WPackLong. Logical shifts, including negative input bit
 * patterns. The exponent can exceed three bits and truncates on return. */
StarsU16 stars_pack_long(StarsU32 n)
{
    StarsU16 exponent = 0;
    while (n & 0xffffe000UL) { n >>= 2; ++exponent; }
    return (StarsU16)(((StarsU32)exponent << 13) | n);
}

static StarsI32 power_product(StarsI32 left, StarsI32 right)
{
    /* Every input is a signed 16-bit value in the original records.  Keeping
     * the operation in the host's 32-bit core type matches __aFlmul for the
     * values used by the game and makes the intended truncation explicit. */
    return (StarsI32)((StarsU32)left * (StarsU32)right);
}

static StarsI32 power_quotient(StarsI32 value, StarsI32 divisor)
{
    StarsU32 bits;
    if (divisor == 0) return 0;
    bits = (StarsU32)value;
    if (value < 0)
        return (StarsI32)((StarsU32)0 - ((StarsU32)0 - bits) /
            (StarsU32)divisor);
    return (StarsI32)(bits / (StarsU32)divisor);
}

/* NE 8:0B32-0E4E, LComputePower. The original resolves each component with
 * FLookupPart; the caller supplies those resolved values here. Kind 0x10 is
 * cargo capacity, 0x20 is fuel-tanker capacity, 0x40 is engine power, and
 * 0x800/0xC-0xD is the multiplicative weapon modifier. */
StarsI32 stars_compute_design_power(const StarsPowerComponent *components,
    StarsU16 count, StarsI16 speed)
{
    StarsU16 i, quantity, subtype, repetitions;
    StarsI32 cargo_power = 0, fuel_power = 0, engine_power = 0;
    StarsI32 multiplier = 1000, contribution, scale;
    for (i = 0; i < count; ++i) {
        quantity = (StarsU16)(components[i].packed_count_type >> 8);
        subtype = (StarsU16)(components[i].packed_count_type & 0xffU);
        if (quantity == 0U) continue;
        if (components[i].kind == 0x10U) {
            contribution = power_product((StarsI32)components[i].fuel_max,
                (StarsI32)quantity);
            contribution = power_product(contribution,
                (StarsI32)components[i].cargo_max + 3L);
            contribution = power_quotient(contribution, 4L);
            if ((components[i].flags & 1U) != 0U)
                contribution = power_quotient(contribution, 3L);
            cargo_power += contribution;
        } else if (components[i].kind == 0x20U) {
            contribution = power_product((StarsI32)components[i].fuel_max,
                (StarsI32)quantity);
            contribution = power_product(contribution,
                (StarsI32)components[i].cargo_max - 2L);
            fuel_power += power_quotient(contribution, 2L);
        } else if (components[i].kind == 0x40U) {
            contribution = (StarsI32)((StarsU32)(components[i].fuel_max +
                components[i].damage_points) * (StarsU32)quantity * 2UL);
            engine_power = (StarsI32)((StarsU32)engine_power +
                (StarsU32)contribution);
        } else if (components[i].kind == 0x800U &&
                   (subtype == 0x0cU || subtype == 0x0dU)) {
            repetitions = quantity;
            while (repetitions-- != 0U) {
                scale = (StarsI32)components[i].damage_points + 100L;
                multiplier = power_quotient(power_product(multiplier, scale),
                    100L);
            }
        }
    }
    if (multiplier != 1000L) {
        scale = power_quotient(multiplier, 10L);
        if (scale > 255L) scale = 255L;
        cargo_power = power_quotient(power_product(cargo_power, scale), 100L);
    }
    contribution = power_quotient(power_product(cargo_power,
        (StarsI32)speed - 4L), 10L);
    return cargo_power + contribution + engine_power + fuel_power;
}

/* NE 8:0E4E-0F24, ComputeShdefPowers. Designs carrying bit 9 are excluded
 * exactly as the original hidden/free definitions are. */
void stars_compute_design_powers(StarsPowerDesign *designs, StarsU16 count)
{
    StarsU16 i;
    for (i = 0; i < count; ++i) {
        if ((designs[i].hidden_flags & 0x0200U) == 0U)
            designs[i].power = stars_compute_design_power(
                designs[i].components, designs[i].component_count,
                designs[i].speed);
    }
}

/* NE 8:56B8-58A6, LCalcFuelGainFromRamScoops. A design's scoop count is
 * resolved from its first component; engine use flags are supplied by the
 * host adapter so the calculation remains ordinary C. At most four turns'
 * worth of consecutive zero-use entries contribute. */
StarsI32 stars_fuel_gain_from_ram_scoops(const StarsRamScoopDesign *designs,
    StarsU16 count, StarsI16 warp, StarsI32 travel)
{
    StarsU16 i, step;
    StarsI32 gain, total = 0;
    if (warp >= 11 || warp < 0) return 0;
    for (i = 0; i < count; ++i) {
        if (designs[i].ship_count == 0U) continue;
        gain = 0;
        if (warp < 10 && designs[i].fuel_used[warp] == 0U) {
            gain = designs[i].scoop_count;
            for (step = 1; step < 4U && (StarsI16)(warp + (StarsI16)step) < 11;
                 ++step) {
                if (designs[i].fuel_used[warp + step] != 0U) break;
                gain += (StarsI32)designs[i].scoop_count * (StarsI32)(step + 1U);
            }
        }
        total = (StarsI32)((StarsU32)total +
            (StarsU32)gain * (StarsU32)designs[i].ship_count);
    }
    return (StarsI32)((StarsU32)total * (StarsU32)travel);
}

/* NE 8:4FB8-50D0, GetFleetScannerRange. The per-design values are the
 * resolved result of GetShdefScannerRange. */
StarsI16 stars_get_fleet_scanner_range(const StarsScannerDesign *designs,
    StarsU16 count, StarsI16 *planet_range, StarsI16 *detection_percent,
    StarsI16 *steal_flags)
{
    StarsU16 i;
    StarsI16 range = -1, best_planet = 0, detect = 100;
    if (detection_percent) *detection_percent = 100;
    for (i = 0; i < count; ++i) {
        if (designs[i].ship_count == 0U) continue;
        if (designs[i].scanner_range > range)
            range = designs[i].scanner_range;
        if (designs[i].planet_range > best_planet)
            best_planet = designs[i].planet_range;
        if (designs[i].detection_percent < detect)
            detect = designs[i].detection_percent;
        if (steal_flags)
            *steal_flags = (StarsI16)((StarsU16)*steal_flags |
                (StarsU16)designs[i].steal_flags);
    }
    if (planet_range) *planet_range = best_planet;
    if (detection_percent) *detection_percent = detect;
    return range;
}

/* NE 8:4E02-4FB8, GetCachedFleetScannerRange. Cached scanner range 0x7FF
 * means "not applicable" and is omitted from the maximum. */
StarsI16 stars_get_cached_fleet_scanner_range(
    const StarsScannerDesign *designs, StarsU16 count,
    StarsI16 *planet_range, StarsI16 *detection_percent, StarsI16 *steal_flags)
{
    StarsU16 i;
    StarsI16 range = -1, best_planet = 0, detect = 100;
    for (i = 0; i < count; ++i) {
        if (designs[i].ship_count == 0U) continue;
        if (designs[i].scanner_range != 0x07ff &&
                designs[i].scanner_range > range)
            range = designs[i].scanner_range;
        if (designs[i].planet_range > best_planet)
            best_planet = designs[i].planet_range;
        if (designs[i].detection_percent < detect)
            detect = designs[i].detection_percent;
        if (steal_flags)
            *steal_flags = (StarsI16)((StarsU16)*steal_flags |
                (StarsU16)designs[i].steal_flags);
    }
    if (planet_range) *planet_range = best_planet;
    if (detection_percent) *detection_percent = detect;
    return range;
}

/* NE 8:4C02-4E02, GetPlanetScannerRange. scanner_stat and trait_flag are
 * resolved race values; mineral_value and planetary_scanner_dp are the
 * resolved planet/scanner records. */
StarsI16 stars_get_planet_scanner_range(StarsI16 scanner_stat,
    StarsI16 trait_flag, StarsI32 mineral_value, StarsI16 scanner_dp,
    StarsI16 blocked, StarsI16 deep_enabled, StarsI16 *deep)
{
    double range;
    StarsI16 result;
    if (deep) *deep = 0;
    if (scanner_stat == 8) {
        range = sqrt((double)mineral_value * 10.0);
        result = (StarsI16)range;
        if (!trait_flag) {
            if (deep && deep_enabled) *deep = (StarsI16)(result / 2);
        } else {
            result = (StarsI16)((StarsI32)result * 0x584L / 1000L);
            if (deep) *deep = 0;
        }
        return result;
    }
    if (blocked) return 0;
    if (deep && scanner_dp < 0) *deep = (StarsI16)(-scanner_dp / 2);
    result = scanner_dp < 0 ? (StarsI16)-scanner_dp : scanner_dp;
    if (trait_flag) result = (StarsI16)(result << 1);
    return result;
}

static StarsI32 scanner_fourth_power(StarsI16 value)
{
    StarsI32 square = power_product((StarsI32)value, (StarsI32)value);
    return power_product(square, square);
}

/* NE 8:50D0-56B8, GetShdefScannerRange. Each component has already been
 * resolved through LpscannerFromId. base_scan_power and base_plan_power are
 * the optional fourth-power terms established by the original scanner-tech
 * branch (use zero when no built-in scanner is present). */
StarsI16 stars_get_design_scanner_range(const StarsScannerComponent *parts,
    StarsU16 count, StarsI32 base_scan_power, StarsI32 base_plan_power,
    StarsI16 trait_flag, const StarsU8 *detection_table,
    StarsI16 *planet_range, StarsI16 *detection_percent, StarsI16 *steal_flags)
{
    StarsU16 i, quantity, subtype;
    StarsI16 i_range, plan_range, detection_index = 0;
    StarsI16 has_scanner = (StarsI16)(base_scan_power > 0);
    StarsI32 scan_power = base_scan_power, plan_power = base_plan_power;
    StarsI16 steal = 0;
    double root;
    if (detection_percent) *detection_percent = 100;
    for (i = 0; i < count; ++i) {
        quantity = (StarsU16)(parts[i].packed_count_type >> 8);
        subtype = (StarsU16)(parts[i].packed_count_type & 0xffU);
        if (quantity == 0U) continue;
        if (parts[i].kind == 2U) {
            has_scanner = 1;
            scan_power = (StarsI32)((StarsU32)scan_power +
                (StarsU32)quantity * (StarsU32)scanner_fourth_power(
                    parts[i].range));
            if (subtype == 6U) i_range = 45;
            else if (subtype == 5U) {
                i_range = 0;
                steal = (StarsI16)(steal | 1);
            } else if (subtype == 0x0eU) {
                i_range = 120;
                steal = 3;
            } else if (parts[i].abilities < 1) {
                continue;
            } else if (parts[i].abilities == 1) i_range = 50;
            else if (parts[i].abilities == 2) i_range = 100;
            else i_range = 200;
            plan_power = (StarsI32)((StarsU32)plan_power +
                (StarsU32)quantity * (StarsU32)scanner_fourth_power(i_range));
        } else {
            if (parts[i].kind == 8U && subtype == 9U) {
                i_range = 80; plan_range = 40;
            } else if (parts[i].kind == 0x10U && subtype == 0x12U) {
                i_range = 150; plan_range = 75;
            } else if (parts[i].kind == 4U && subtype == 6U) {
                i_range = 50; plan_range = 25;
            } else {
                if (parts[i].kind == 0x800U && subtype == 0x0fU)
                    detection_index = (StarsI16)(detection_index + quantity);
                continue;
            }
            scan_power = (StarsI32)((StarsU32)scan_power +
                (StarsU32)quantity * (StarsU32)scanner_fourth_power(i_range));
            plan_power = (StarsI32)((StarsU32)plan_power +
                (StarsU32)quantity * (StarsU32)scanner_fourth_power(plan_range));
        }
    }
    if (planet_range) {
        root = sqrt(sqrt((double)(StarsU32)plan_power));
        *planet_range = (StarsI16)root;
    }
    if (steal_flags) *steal_flags = steal;
    if (!has_scanner) return -1;
    root = sqrt(sqrt((double)(StarsU32)scan_power));
    i_range = (StarsI16)root;
    if (trait_flag) i_range = (StarsI16)(i_range << 1);
    if (detection_percent) {
        if (detection_index > 17) detection_index = 17;
        *detection_percent = detection_table ? detection_table[
            (StarsU16)detection_index] : 100;
    }
    return i_range;
}

/* NE 8:0746-07FE, DpOfLpflIshdef. The fleet byte span retains rgcsh at
 * offset 0x0C: sixteen counts followed by sixteen packed hull/part words.
 * design_damage is the resolved hull damage value at hull offset 0x38. */
StarsI32 stars_fleet_design_damage(const StarsU8 *fleet, StarsU16 design,
    const StarsI16 *design_damage, StarsU16 design_count)
{
    StarsU16 count, packed, part_count;
    StarsI32 per_ship;
    if (!fleet || !design_damage || design >= 16U || design >= design_count)
        return 0;
    count = stars_word(fleet, (StarsU16)(0x0cU + design * 2U));
    packed = stars_word(fleet, (StarsU16)(0x0cU + (design + 16U) * 2U));
    part_count = (StarsU16)(packed & 0x007fU);
    per_ship = (StarsI32)part_count * (StarsI32)design_damage[design];
    per_ship = power_quotient(per_ship, 10L);
    per_ship = power_product(per_ship, (StarsI32)(packed >> 7));
    return power_quotient(power_product((StarsI32)count, per_ship), 5000L);
}

/* NE 8:24FA-26DE, FLookupOrbitingXfer. Fleet records are 124-byte spans and
 * thing records are 18-byte spans; the transfer record is the original
 * 128-byte union with its copied payload beginning at offset four. */
StarsI16 stars_lookup_orbiting_transfer(StarsI16 planet_id, StarsI16 nth,
    const StarsU8 *fleets, StarsU16 fleet_count, StarsU16 fleet_stride,
    const StarsU8 *things, StarsU16 thing_count, StarsU16 thing_stride,
    StarsI16 id_skip, StarsI16 selected_x, StarsI16 selected_y,
    StarsU8 *transfer)
{
    StarsU16 i;
    const StarsU8 *record;
    for (i = 0; i < fleet_count; ++i) {
        record = fleets + i * fleet_stride;
        if (stars_signed_word(stars_word(record, 6U)) != planet_id ||
                stars_signed_word(stars_word(record, 0U)) == id_skip ||
                (id_skip != -1 &&
                 (stars_signed_word(stars_word(record, 8U)) != selected_x ||
                  stars_signed_word(stars_word(record, 10U)) != selected_y)))
            continue;
        if (nth == 0) {
            if (transfer) {
                memcpy(transfer + 4U, record, 124U);
                stars_put_word(transfer, 2U, 2U);
                stars_put_word(transfer, 0U, stars_word(record, 0U));
            }
            return 1;
        }
        --nth;
    }
    for (i = 0; i < thing_count; ++i) {
        record = things + i * thing_stride;
        if (((stars_word(record, 0U) >> 13) & 7U) != 1U ||
                stars_signed_word(stars_word(record, 2U)) != selected_x ||
                stars_signed_word(stars_word(record, 4U)) != selected_y)
            continue;
        if (nth == 0) {
            if (transfer) {
                memcpy(transfer + 4U, record, 18U);
                stars_put_word(transfer, 2U, 8U);
                stars_put_word(transfer, 0U, stars_word(record, 0U));
            }
            return 1;
        }
        --nth;
    }
    return 0;
}

/* NE 8:75E2-7960, FCanFleetUseStargates. The nearest-object search and the
 * original MdCalcStargateDamage call are explicit inputs; this preserves the
 * eligibility state machine and its 1/3/5/7 result codes. */
StarsI16 stars_can_fleet_use_stargates(StarsI16 fleet_owner,
    StarsI16 destination_valid, StarsI16 destination_owner,
    StarsI16 destination_current_turn, StarsI16 destination_gate,
    StarsI16 source_valid, StarsI16 source_owner, StarsI16 source_current_turn,
    StarsI16 source_gate, StarsI16 fleet_can_jumpgate, StarsI16 race_stat,
    const StarsI32 *minerals, StarsI16 distance, const StarsU16 *ship_counts,
    const StarsI16 *hull_damage, StarsU16 design_count,
    StarsStargateDamageCheck damage_check, void *context)
{
    StarsI16 destination_foreign = 0, source_gate_to_use;
    StarsI16 mineral_flag = 0, damage_flag = 0, damage;
    StarsI16 status;
    StarsU16 i;
    if (!destination_valid) return 0;
    if (destination_owner != fleet_owner) {
        if (destination_owner == -1 && destination_current_turn) return 0;
        destination_foreign = 1;
    } else if (destination_gate < 0) {
        return 0;
    }
    if (source_valid && source_owner == fleet_owner && source_gate >= 0) {
        if (destination_foreign) return -1;
        source_gate_to_use = source_gate;
    } else {
        if (!fleet_can_jumpgate) {
            if (!source_valid ||
                    (source_owner == -1 && source_current_turn)) return 0;
            if (source_owner == fleet_owner) return 0;
            return -1;
        }
        if (destination_foreign) return -1;
        source_gate_to_use = destination_gate;
    }
    if (race_stat != 7 && minerals) {
        for (i = 0; i < 4U; ++i) {
            if (minerals[i] > 0) { mineral_flag = 1; break; }
        }
    }
    if (!damage_check || !ship_counts || !hull_damage) return 0;
    for (i = 0; i < design_count; ++i) {
        if (ship_counts[i] == 0U) continue;
        damage = 0;
        status = damage_check(context, source_gate_to_use, destination_gate,
            distance, hull_damage[i], &damage);
        if (status <= 0) return 0;
        if (status == 1 && damage > 0) damage_flag = 1;
    }
    return (StarsI16)(1 + (damage_flag ? 2 : 0) +
        (mineral_flag ? 4 : 0));
}

/* NE 8:47B0-4BA1, UpdateShdefCost. Component costs are already resolved by
 * GetTruePartCost; the final fields retain the original low-word stores. */
void stars_update_design_cost(StarsDesignCost *design,
    const StarsCostComponent *components, StarsU16 count,
    StarsI16 half_cargo_race)
{
    StarsU32 ore[3], resource, empty;
    StarsU16 i, quantity, subtype;
    if (!design) return;
    ore[0] = design->ore_cost[0];
    ore[1] = design->ore_cost[1];
    ore[2] = design->ore_cost[2];
    resource = design->resource_cost;
    empty = design->weight_empty;
    for (i = 0; i < count; ++i) {
        quantity = (StarsU16)(components[i].packed_count_type >> 8);
        subtype = (StarsU16)(components[i].packed_count_type & 0xffU);
        if (quantity == 0U) continue;
        ore[0] += (StarsU32)quantity * components[i].ore_cost[0];
        ore[1] += (StarsU32)quantity * components[i].ore_cost[1];
        ore[2] += (StarsU32)quantity * components[i].ore_cost[2];
        resource += (StarsU32)quantity * components[i].resource_cost;
        empty += (StarsU32)quantity * design->hull_weight_empty;
        if (components[i].kind == 4U &&
                (subtype == 3U || subtype == 6U)) {
            design->damage_points = (StarsI16)(design->damage_points +
                quantity * 0x41U);
        } else if (components[i].kind == 8U) {
            StarsU32 cargo = (StarsU32)quantity * components[i].cargo_max;
            if (half_cargo_race) cargo >>= 1;
            design->damage_points = (StarsI16)(design->damage_points + cargo);
        } else if (components[i].kind == 0x1000U && subtype == 4U) {
            design->damage_points = (StarsI16)(design->damage_points +
                quantity * 0x32U);
        }
    }
    design->ore_cost[0] = (StarsU16)ore[0];
    design->ore_cost[1] = (StarsU16)ore[1];
    design->ore_cost[2] = (StarsU16)ore[2];
    design->resource_cost = (StarsU16)resource;
    design->weight_empty = (StarsU16)empty;
}

/* NE 8:0032-01B2, FDupPlanet. The host representation keeps the original
 * 52-byte scalar record separate from its variable production tail, while
 * preserving the destination tail object across the scalar copy. */
StarsI16 stars_duplicate_planet(StarsPlanetState *destination,
    const StarsPlanetState *source)
{
    StarsPlanetProduction *saved;
    StarsU16 bytes;
    if (!destination || !source) return 1;
    saved = destination->production;
    memcpy(destination->bytes, source->bytes, sizeof(destination->bytes));
    destination->production = saved;
    if (!source->production) {
        if (saved) {
            free(saved->records);
            free(saved);
        }
        destination->production = 0;
        return 1;
    }
    if (!saved) {
        saved = (StarsPlanetProduction *)calloc(1, sizeof(*saved));
        if (!saved) return 1;
        saved->capacity = source->production->capacity;
        saved->records = (StarsU8 *)malloc((size_t)saved->capacity * 4U);
        if (!saved->records && saved->capacity) {
            free(saved);
            return 1;
        }
        destination->production = saved;
    } else if (saved->capacity < source->production->count) {
        StarsU8 *records = (StarsU8 *)realloc(saved->records,
            (size_t)source->production->capacity * 4U);
        if (!records && source->production->capacity) return 1;
        saved->records = records;
        saved->capacity = source->production->capacity;
    }
    bytes = (StarsU16)(source->production->count * 4U);
    if (bytes) memcpy(saved->records, source->production->records, bytes);
    saved->count = source->production->count;
    return 1;
}

/* NE 8:04A6-0746, FLookupPlanet. A positive lookup refreshes a working
 * record; the negative form commits that working record back to the table. */
StarsI16 stars_lookup_planet(StarsPlanetState **planets, StarsU16 count,
    StarsI16 id, StarsPlanetState *working, StarsPlanetLookupHook hook,
    void *context)
{
    StarsI16 requested = id;
    StarsU16 i;
    StarsPlanetState *stored = 0;
    if (count == 0U) return 0;
    if (id < 0) {
        if (!working) return 0;
        requested = stars_signed_word((StarsU16)(working->bytes[0] |
            ((StarsU16)working->bytes[1] << 8)));
        if (requested == -1) {
            if (hook) hook(context, 0, working);
            return 1;
        }
    }
    for (i = 0; i < count; ++i) {
        StarsI16 current = stars_signed_word((StarsU16)(planets[i]->bytes[0] |
            ((StarsU16)planets[i]->bytes[1] << 8)));
        if (current == requested) {
            stored = planets[i];
            break;
        }
    }
    if (!stored || !working) return 0;
    if (id < 0) {
        if (!stars_duplicate_planet(stored, working)) return 0;
        if (hook) hook(context, stored, working);
    } else {
        if (!stars_duplicate_planet(working, stored)) return 0;
        if (hook) hook(context, working, stored);
    }
    return 1;
}

/* NE 8:2332-249F, FDupFleet. The fixed fleet bytes and variable 18-byte
 * order records use the same ownership-preserving copy pattern as planets. */
StarsI16 stars_duplicate_fleet(const StarsFleetState *source,
    StarsFleetState *destination)
{
    StarsFleetOrders *saved;
    StarsU16 bytes;
    if (!source || !destination) return 1;
    saved = destination->orders;
    memcpy(destination->bytes, source->bytes, sizeof(destination->bytes));
    destination->orders = saved;
    if (!source->orders) {
        if (saved) {
            free(saved->records);
            free(saved);
        }
        destination->orders = 0;
        return 1;
    }
    if (!saved) {
        saved = (StarsFleetOrders *)calloc(1, sizeof(*saved));
        if (!saved) return 1;
        saved->capacity = source->orders->capacity;
        saved->records = (StarsU8 *)malloc((size_t)saved->capacity * 18U);
        if (!saved->records && saved->capacity) {
            free(saved);
            return 1;
        }
        destination->orders = saved;
    } else if (saved->capacity < source->orders->count) {
        StarsU8 *records = (StarsU8 *)realloc(saved->records,
            (size_t)source->orders->capacity * 18U);
        if (!records && source->orders->capacity) return 1;
        saved->records = records;
        saved->capacity = source->orders->capacity;
    }
    bytes = (StarsU16)(source->orders->count * 18U);
    if (bytes) memcpy(saved->records, source->orders->records, bytes);
    saved->count = source->orders->count;
    return 1;
}

static StarsU32 transfer_design_long(const StarsU8 *record,
    StarsU16 offset)
{
    return (StarsU32)record[offset] |
        ((StarsU32)record[offset + 1U] << 8) |
        ((StarsU32)record[offset + 2U] << 16) |
        ((StarsU32)record[offset + 3U] << 24);
}

static void transfer_design_put_long(StarsU8 *record, StarsU16 offset,
    StarsU32 value)
{
    record[offset] = (StarsU8)value;
    record[offset + 1U] = (StarsU8)(value >> 8);
    record[offset + 2U] = (StarsU8)(value >> 16);
    record[offset + 3U] = (StarsU8)(value >> 24);
}

/* NE 23:94DB-9A2B.  This is the player-specific half of the created-transfer
 * branch.  The destination bank is represented as two contiguous 16-entry
 * tables so the host path can exercise the exact packed record operation
 * without fabricating the game's far-pointer player object. */
StarsI16 stars_materialize_player_transfer_designs(
    const StarsU8 *source_records, const StarsI16 *source_counts,
    const StarsI16 *source_status, StarsU8 *destination_records,
    StarsI16 *destination_counts, StarsI16 *destination_status,
    StarsU16 destination_count, StarsU16 *active_design_count,
    StarsI16 *mapping)
{
    StarsU16 source;
    StarsU16 candidate;
    StarsU16 slot;
    StarsU16 source_hulls[STARS_TRANSFER_DESIGN_COUNT];
    StarsU16 source_kinds[STARS_TRANSFER_DESIGN_COUNT];
    StarsU16 source_packed[STARS_TRANSFER_DESIGN_COUNT];
    StarsU16 candidate_hulls[STARS_TRANSFER_DESIGN_COUNT];
    StarsU16 candidate_flags[STARS_TRANSFER_DESIGN_COUNT];
    StarsU16 candidate_counts[STARS_TRANSFER_DESIGN_COUNT];
    StarsU16 candidate_kinds[STARS_TRANSFER_DESIGN_COUNT *
        STARS_TRANSFER_DESIGN_COUNT];
    StarsU16 candidate_packed[STARS_TRANSFER_DESIGN_COUNT *
        STARS_TRANSFER_DESIGN_COUNT];
    StarsU16 source_count;
    StarsU16 destination_offset;
    StarsI16 next_slot;
    StarsI16 match;
    StarsU32 flags;
    StarsU32 value;

    if (!source_records || !source_counts || !source_status ||
            !destination_records || !destination_counts ||
            !destination_status || !mapping || destination_count == 0U ||
            destination_count > STARS_TRANSFER_DESIGN_COUNT)
        return 0;

    for (candidate = 0; candidate < destination_count; ++candidate) {
        destination_offset = (StarsU16)(candidate *
            STARS_TRANSFER_DESIGN_BYTES);
        candidate_hulls[candidate] = stars_word(destination_records,
            destination_offset);
        candidate_flags[candidate] = stars_word(destination_records,
            (StarsU16)(destination_offset + 0x7bU));
        candidate_counts[candidate] = destination_records[
            destination_offset + 0x7aU];
        for (slot = 0; slot < STARS_TRANSFER_DESIGN_COUNT; ++slot) {
            candidate_kinds[candidate * STARS_TRANSFER_DESIGN_COUNT + slot] =
                stars_word(destination_records, (StarsU16)(destination_offset +
                    0x3aU + slot * 4U));
            candidate_packed[candidate * STARS_TRANSFER_DESIGN_COUNT + slot] =
                stars_word(destination_records, (StarsU16)(destination_offset +
                    0x3cU + slot * 4U));
        }
    }

    next_slot = -1;
    for (source = 0; source < STARS_TRANSFER_DESIGN_COUNT; ++source) {
        destination_offset = (StarsU16)(source *
            STARS_TRANSFER_DESIGN_BYTES);
        if (source_counts[source] <= 0) {
            mapping[source] = -1;
            continue;
        }
        source_hulls[0] = stars_word(source_records, destination_offset);
        source_count = source_records[destination_offset + 0x7aU];
        if (source_count > STARS_TRANSFER_DESIGN_COUNT)
            return 0;
        for (slot = 0; slot < source_count &&
                slot < STARS_TRANSFER_DESIGN_COUNT; ++slot) {
            source_kinds[slot] = stars_word(source_records,
                (StarsU16)(destination_offset + 0x3aU + slot * 4U));
            source_packed[slot] = stars_word(source_records,
                (StarsU16)(destination_offset + 0x3cU + slot * 4U));
        }
        match = stars_find_similar_design(source_hulls[0], source_kinds,
            source_packed, source_count, candidate_hulls, candidate_flags,
            candidate_counts, candidate_kinds, candidate_packed,
            destination_count, STARS_TRANSFER_DESIGN_COUNT);
        if (match >= 0) {
            mapping[source] = match;
            continue;
        }
        do {
            ++next_slot;
        } while (next_slot < (StarsI16)destination_count &&
            (candidate_flags[next_slot] & 0x0200U) == 0U);
        if (next_slot >= (StarsI16)destination_count)
            return 0;
        mapping[source] = next_slot;
        candidate_flags[next_slot] = (StarsU16)(candidate_flags[next_slot] |
            0x0200U);
    }

    for (source = 0; source < STARS_TRANSFER_DESIGN_COUNT; ++source) {
        if (source_counts[source] <= 0)
            continue;
        destination_offset = (StarsU16)(mapping[source] *
            STARS_TRANSFER_DESIGN_BYTES);
        if ((stars_word(destination_records,
                (StarsU16)(destination_offset + 0x7bU)) & 0x0200U) != 0U) {
            memcpy(destination_records + destination_offset,
                source_records + (StarsU16)(source *
                    STARS_TRANSFER_DESIGN_BYTES),
                STARS_TRANSFER_DESIGN_BYTES);
            flags = transfer_design_long(destination_records,
                (StarsU16)(destination_offset + 0x7bU));
            flags = (flags & 0x83ffUL) |
                ((StarsU32)(mapping[source] & 0x1f) << 10);
            flags = (flags & 0x7fffUL) | 0x8000UL;
            transfer_design_put_long(destination_records,
                (StarsU16)(destination_offset + 0x7bU), flags);
            transfer_design_put_long(destination_records,
                (StarsU16)(destination_offset + 0x7fU), 0UL);
            transfer_design_put_long(destination_records,
                (StarsU16)(destination_offset + 0x83U), 0UL);
            if (active_design_count)
                *active_design_count = (StarsU16)(*active_design_count + 1U);
            candidate_flags[mapping[source]] = (StarsU16)(flags & 0xffffU);
        }
        destination_counts[mapping[source]] = (StarsI16)(
            destination_counts[mapping[source]] + source_counts[source]);
        destination_status[mapping[source]] = source_status[source];
        value = transfer_design_long(destination_records,
            (StarsU16)(destination_offset + 0x7fU));
        transfer_design_put_long(destination_records,
            (StarsU16)(destination_offset + 0x7fU),
            value + (StarsU32)(StarsI32)source_counts[source]);
        value = transfer_design_long(destination_records,
            (StarsU16)(destination_offset + 0x83U));
        transfer_design_put_long(destination_records,
            (StarsU16)(destination_offset + 0x83U),
            value + (StarsU32)(StarsI32)source_counts[source]);
    }
    return 1;
}

/* NE 8:2D44-300C, FDeleteFleet. The table is the sorted rglpfl array; the
 * hook receives the removed record for order/selection/window side effects. */
StarsI16 stars_delete_fleet(StarsFleetState **fleets, StarsU16 *count,
    StarsI16 id, StarsI16 selected_kind, StarsI16 selected_id,
    StarsFleetDeleteHook hook, void *context)
{
    StarsU16 i, n;
    StarsFleetState *removed;
    StarsU16 fleet_id;
    if (!fleets || !count) return 0;
    n = *count;
    for (i = 0; i < n; ++i) {
        fleet_id = (StarsU16)(fleets[i]->bytes[0] |
            ((StarsU16)fleets[i]->bytes[1] << 8));
        if (fleet_id == (StarsU16)id) break;
        if ((StarsI16)fleet_id > id) return 0;
    }
    if (i == n) return 0;
    removed = fleets[i];
    removed->bytes[5] = (StarsU8)(removed->bytes[5] | 0x04U);
    if (hook) hook(context, removed, selected_kind, selected_id);
    for (; i + 1U < n; ++i) fleets[i] = fleets[i + 1U];
    *count = (StarsU16)(n - 1U);
    return 1;
}

/* NE 8:300C-3371, LpflNew. The table owns sorted fleet pointers; this host
 * form initializes the scalar record and its one-entry order list directly. */
StarsI16 stars_new_fleet(StarsFleetTable *table, StarsI16 player,
    StarsI16 planet, StarsI16 x, StarsI16 y, StarsFleetState **result)
{
    StarsFleetState *fleet;
    StarsFleetOrders *orders;
    StarsU16 i, insert, id;
    StarsI16 previous;
    StarsU16 flags;
    if (!table || player < 0 || player >= 16) return 0;
    insert = table->count;
    previous = -1;
    for (i = 0; i < table->count; ++i) {
        id = (StarsU16)(table->items[i]->bytes[0] |
            ((StarsU16)table->items[i]->bytes[1] << 8));
        if ((StarsI16)table->items[i]->bytes[2] > player) {
            insert = i;
            break;
        }
        if ((StarsI16)table->items[i]->bytes[2] == player) {
            if ((StarsI16)(id & 0x01ffU) != (StarsI16)(previous + 1))
                break;
            previous = (StarsI16)(id & 0x01ffU);
        }
    }
    if (table->count == table->capacity) {
        StarsFleetState **items = (StarsFleetState **)realloc(table->items,
            (size_t)(table->capacity + 1U) * sizeof(*items));
        if (!items) return 0;
        table->items = items;
        table->capacity = (StarsU16)(table->capacity + 1U);
    }
    fleet = (StarsFleetState *)calloc(1, sizeof(*fleet));
    orders = (StarsFleetOrders *)calloc(1, sizeof(*orders));
    if (!fleet || !orders) {
        free(fleet);
        free(orders);
        return 0;
    }
    orders->capacity = 3;
    orders->count = 1;
    orders->records = (StarsU8 *)calloc(3U, 18U);
    if (!orders->records) {
        free(orders);
        free(fleet);
        return 0;
    }
    fleet->orders = orders;
    id = (StarsU16)((previous + 1) & 0x01ff);
    fleet->bytes[0] = (StarsU8)(id & 0xffU);
    fleet->bytes[1] = (StarsU8)(((id >> 8) & 1U) | ((player & 0x0fU) << 1));
    fleet->bytes[2] = (StarsU8)player;
    fleet->bytes[6] = (StarsU8)(planet & 0xffU);
    fleet->bytes[7] = (StarsU8)(((StarsU16)planet >> 8) & 0xffU);
    fleet->bytes[8] = (StarsU8)(x & 0xffU);
    fleet->bytes[9] = (StarsU8)(((StarsU16)x >> 8) & 0xffU);
    fleet->bytes[10] = (StarsU8)(y & 0xffU);
    fleet->bytes[11] = (StarsU8)(((StarsU16)y >> 8) & 0xffU);
    fleet->bytes[98] = 1;
    flags = 7;
    fleet->bytes[4] = (StarsU8)flags;
    fleet->bytes[5] = (StarsU8)(flags >> 8);
    orders->records[0] = fleet->bytes[8];
    orders->records[1] = fleet->bytes[9];
    orders->records[2] = fleet->bytes[10];
    orders->records[3] = fleet->bytes[11];
    orders->records[4] = fleet->bytes[6];
    orders->records[5] = fleet->bytes[7];
    orders->records[6] = 0;
    orders->records[7] = (StarsU8)(planet == -1 ? 0x14U : 0x11U);
    for (i = table->count; i > insert; --i) table->items[i] = table->items[i - 1U];
    table->items[insert] = fleet;
    table->count = (StarsU16)(table->count + 1U);
    table->player_counts[player] = (StarsU16)(table->player_counts[player] + 1U);
    if (result) *result = fleet;
    return 1;
}

/* NE 8:3372-34D8, LpflNewSplit. The newly allocated fleet receives the
 * source's route/flags/order payload while retaining its fresh identity. */
StarsI16 stars_split_fleet(StarsFleetTable *table,
    const StarsFleetState *source, StarsFleetState **result,
    StarsFleetSplitHook log_split, void *context)
{
    StarsFleetState *fleet;
    StarsFleetOrders *source_orders, *orders;
    StarsU16 source_flags, destination_flags, bytes;
    StarsI16 player, planet, x, y;
    if (!table || !source) return 0;
    player = (StarsI16)source->bytes[2];
    planet = (StarsI16)(source->bytes[6] |
        ((StarsU16)source->bytes[7] << 8));
    x = stars_signed_word((StarsU16)(source->bytes[8] |
        ((StarsU16)source->bytes[9] << 8)));
    y = stars_signed_word((StarsU16)(source->bytes[10] |
        ((StarsU16)source->bytes[11] << 8)));
    if (!stars_new_fleet(table, player, planet, x, y, &fleet)) return 0;
    source_flags = (StarsU16)(source->bytes[4] |
        ((StarsU16)source->bytes[5] << 8));
    destination_flags = (StarsU16)(fleet->bytes[4] |
        ((StarsU16)fleet->bytes[5] << 8));
    destination_flags = (StarsU16)((destination_flags & 0xfdffU) |
        (source_flags & 0x0200U));
    destination_flags = (StarsU16)((destination_flags & 0xff00U) |
        (source_flags & 0x00ffU));
    fleet->bytes[4] = (StarsU8)destination_flags;
    fleet->bytes[5] = (StarsU8)(destination_flags >> 8);
    fleet->bytes[96] = source->bytes[96];
    fleet->bytes[97] = source->bytes[97];
    fleet->bytes[98] = source->bytes[98];
    source_orders = source->orders;
    orders = fleet->orders;
    if (source_orders) {
        if (orders->capacity < source_orders->count) {
            StarsU8 *records = (StarsU8 *)realloc(orders->records,
                (size_t)source_orders->count * 18U);
            if (!records) return 0;
            orders->records = records;
            orders->capacity = source_orders->count;
        }
        bytes = (StarsU16)(source_orders->count * 18U);
        if (bytes) memcpy(orders->records, source_orders->records, bytes);
        orders->count = source_orders->count;
    } else {
        orders->count = 0;
    }
    if (log_split) log_split(context, (StarsU16)(source->bytes[0] |
        ((StarsU16)source->bytes[1] << 8)));
    if (result) *result = fleet;
    return 1;
}

/* NE 8:4070-47AF, FFindNearestObject. Candidate arrays are the host form of
 * the original planet/fleet/waypoint/thing tables; the result retains scan
 * indices and the selected object kind. */
StarsI16 stars_find_nearest_object(StarsI16 x, StarsI16 y, StarsU16 mask,
    const StarsNearestPoint *planets, StarsU16 planet_count,
    const StarsNearestPoint *fleets, StarsU16 fleet_count,
    const StarsNearestPoint *waypoints, StarsU16 waypoint_count,
    const StarsNearestPoint *things, StarsU16 thing_count,
    StarsNearestResult *result)
{
    StarsI32 best = 0x3b9aca00L;
    StarsI32 distance;
    StarsU16 i;
    StarsI16 found = 0;
    StarsNearestPoint point;
    if (!result) return 0;
    result->x = result->y = -1;
    result->kind = -1;
    result->planet_index = result->fleet_index = -1;
    result->waypoint_index = result->thing_index = -1;
    if (mask & 1U) {
        for (i = 0; i < planet_count; ++i) {
            point = planets[i];
            distance = (StarsI32)(x - point.x) * (x - point.x) +
                (StarsI32)(y - point.y) * (y - point.y);
            if (distance < best) {
                best = distance; found = 1; result->kind = 1;
                result->planet_index = (StarsI16)i;
                result->x = point.x; result->y = point.y;
            }
        }
    }
    if (mask & 2U) {
        for (i = 0; i < fleet_count; ++i) {
            point = fleets[i];
            distance = (StarsI32)(x - point.x) * (x - point.x) +
                (StarsI32)(y - point.y) * (y - point.y);
            if (distance < best) {
                best = distance; found = 1; result->kind = 2;
                result->fleet_index = (StarsI16)i;
                result->planet_index = result->waypoint_index = -1;
                result->thing_index = -1;
                result->x = point.x; result->y = point.y;
            }
        }
    }
    if (mask & 4U) {
        for (i = 0; i < waypoint_count; ++i) {
            point = waypoints[i];
            distance = (StarsI32)(x - point.x) * (x - point.x) +
                (StarsI32)(y - point.y) * (y - point.y);
            if (distance < best) {
                best = distance; found = 1; result->kind = 4;
                result->waypoint_index = (StarsI16)i;
                result->planet_index = result->fleet_index = -1;
                result->thing_index = -1;
                result->x = point.x; result->y = point.y;
            }
        }
    }
    if (mask & 8U) {
        for (i = 0; i < thing_count; ++i) {
            point = things[i];
            distance = (StarsI32)(x - point.x) * (x - point.x) +
                (StarsI32)(y - point.y) * (y - point.y);
            if (distance < best) {
                best = distance; found = 1; result->kind = 8;
                result->thing_index = (StarsI16)i;
                result->planet_index = result->fleet_index = -1;
                result->waypoint_index = -1;
                result->x = point.x; result->y = point.y;
            }
        }
    }
    return found;
}

/* NE 8:2160-2CE8, FLookupFleet.  The original uses the global fleet pointer
 * table and far order pointers.  This host form makes those two tables
 * explicit, preserves negative-id merge versus positive-id lookup, and uses
 * the already reviewed fleet duplication helper for order storage. */
StarsI16 stars_lookup_fleet(StarsFleetState **items, StarsU16 count,
    StarsI16 id_fleet, StarsFleetState *working, StarsI16 working_is_selected)
{
    StarsI16 negative = (StarsI16)(id_fleet < 0);
    StarsU16 id;
    StarsFleetState *target;
    StarsU16 bytes;
    if (!items || !working || count == 0U) return 0;
    if (negative) {
        id = (StarsU16)(working->bytes[0] |
            ((StarsU16)working->bytes[1] << 8));
        if (id >= count) return 0;
        target = items[id];
        if (!target) return 0;
        if (working->orders) {
            if (!target->orders) {
                target->orders = (StarsFleetOrders *)calloc(1,
                    sizeof(StarsFleetOrders));
                if (!target->orders) return 0;
            }
            if (target->orders->capacity < working->orders->count) {
                StarsU8 *records = (StarsU8 *)realloc(target->orders->records,
                    (size_t)working->orders->count * 18U);
                if (!records) return 0;
                target->orders->records = records;
                target->orders->capacity = working->orders->count;
            }
            bytes = (StarsU16)(working->orders->count * 18U);
            if (bytes) memcpy(target->orders->records,
                working->orders->records, bytes);
            target->orders->count = working->orders->count;
        }
        memcpy(target->bytes, working->bytes, sizeof(target->bytes));
        return 1;
    }
    id = (StarsU16)id_fleet;
    if (id >= count) return 0;
    target = items[id];
    if (!target) return 0;
    if (working_is_selected) {
        return stars_duplicate_fleet(target, working);
    }
    memcpy(working->bytes, target->bytes, sizeof(working->bytes));
    if (target->orders) {
        if (!working->orders) {
            working->orders = (StarsFleetOrders *)calloc(1,
                sizeof(StarsFleetOrders));
            if (!working->orders) return 0;
        }
        if (working->orders->capacity < target->orders->count) {
            StarsU8 *records = (StarsU8 *)realloc(working->orders->records,
                (size_t)target->orders->count * 18U);
            if (!records) return 0;
            working->orders->records = records;
            working->orders->capacity = target->orders->count;
        }
        bytes = (StarsU16)(target->orders->count * 18U);
        if (bytes) memcpy(working->orders->records,
            target->orders->records, bytes);
        working->orders->count = target->orders->count;
    }
    return 1;
}

/* NE 8:145C-1BB4, FCalcFleetBombDamage.  Design lookups and the original
 * far-linked fleet list are explicit host tables; the component classes,
 * smart-bomb decay, capped smart result, same-owner filtering, and multiple
 * fleet flag are retained. */
StarsI16 stars_calc_fleet_bomb_damage(StarsBombFleet *fleets,
    StarsU16 fleet_count, StarsI16 first, const StarsBombDesignSlot *slots,
    StarsI32 smart_initial, double smart_base, double smart_denominator,
    StarsBombDamage *damage)
{
    StarsI16 current = first;
    StarsI16 owner;
    StarsI16 processed = 0;
    StarsU16 i, j, count;
    StarsU16 low, high;
    StarsBombFleet *fleet;
    const StarsBombComponent *component;
    double smart;
    if (!fleets || !slots || !damage || first < 0 ||
            (StarsU16)first >= fleet_count) return 0;
    damage->people = damage->people_min = damage->people_smart = 0;
    damage->building = damage->terraforming = 0;
    damage->multiple = 0;
    owner = fleets[first].player;
    smart = (double)smart_initial;
    while (current >= 0 && (StarsU16)current < fleet_count) {
        fleet = &fleets[current];
        if (processed && (fleet->player != owner ||
                fleet->planet != fleets[first].planet || current == first))
            break;
        fleet->flags = (StarsU16)(fleet->flags | 0x1000U);
        ++processed;
        for (i = 0; i < 16; ++i) {
            count = fleet->component_counts[i];
            if (!count) continue;
            for (j = 0; j < slots[i].component_count; ++j) {
                component = &slots[i].components[j];
                low = (StarsU16)(component->packed_damage_type & 0xffU);
                high = (StarsU16)(component->packed_damage_type >> 8);
                if (component->kind == 0x40U) {
                    if (low == 9U) {
                        damage->terraforming += (StarsI32)high * count;
                    } else if (high) {
                        if (!component->defense_power) {
                            StarsU32 steps = (StarsU32)high * count;
                            while (steps--) {
                                smart *= smart_base -
                                    (double)component->fuel_max /
                                    smart_denominator;
                            }
                        } else {
                            damage->people += (StarsI32)high * count *
                                component->fuel_max;
                            damage->building += (StarsI32)high * count *
                                component->defense_power;
                            if (low < 5U)
                                damage->people_min += (StarsI32)high * count * 3;
                        }
                    }
                } else if (component->kind == 0x10U && low == 0x12U) {
                    damage->people += (StarsI32)high * count * 20;
                    damage->building += (StarsI32)high * count * 5;
                    damage->people_min += (StarsI32)high * count * 3;
                } else if (component->kind == 0x1000U && low == 1U) {
                    damage->people_min += (StarsI32)high * count * 20;
                }
            }
        }
        if (fleet->next_index < 0 || (StarsU16)fleet->next_index >= fleet_count)
            break;
        current = fleet->next_index;
        if (fleets[current].flags & 0x0400U) current = -1;
    }
    damage->people_smart = (StarsI32)smart;
    if (damage->people_smart > 1000) damage->people_smart = 1000;
    damage->multiple = (StarsI16)(processed > 1);
    return (StarsI16)(damage->people || damage->people_min ||
        damage->people_smart || damage->building || damage->terraforming);
}

/* NE 8:3A00-3BC8, FFleetSplitAll.  Each extra ship becomes a fresh fleet;
 * the component count is moved one ship at a time and cargo balancing stays
 * an explicit host callback, matching the original FleetTransfer boundary. */
StarsI16 stars_split_all_fleet(StarsFleetTable *table, StarsFleetState *source,
    StarsFleetCargoBalance balance, void *context, StarsU16 *created)
{
    StarsU16 i, count, total = 0;
    StarsFleetState *fresh;
    StarsU8 *raw;
    if (created) *created = 0;
    if (!table || !source) return 0;
    for (i = 0; i < 16; ++i) {
        count = (StarsU16)(source->bytes[12U + i * 2U] |
            ((StarsU16)source->bytes[13U + i * 2U] << 8));
        while (count > 1U) {
            --count;
            raw = source->bytes + 12U + i * 2U;
            raw[0] = (StarsU8)count;
            raw[1] = (StarsU8)(count >> 8);
            fresh = 0;
            if (!stars_split_fleet(table, source, &fresh, 0, 0)) return 0;
            memset(fresh->bytes + 12U, 0, 32U);
            fresh->bytes[12U + i * 2U] = 1;
            if (balance) balance(context, source, fresh);
            ++total;
        }
    }
    if (created) *created = total;
    return (StarsI16)(total > 0U);
}

/* NE 8:3BC8-3E1B, CchGetETA.  The localized text and device color calls are
 * outside the host core; this function returns the ETA and the original
 * special-result code (-3/-2/-1/0/1) through explicit outputs. */
StarsI16 stars_calculate_eta(const StarsEtaPoint *points, StarsU16 count,
    StarsU16 waypoint_count, StarsEtaGateCheck gate, void *context,
    StarsI16 *eta, StarsI16 *special)
{
    StarsU16 i;
    StarsI16 turns = 0;
    StarsI16 status;
    StarsI16 distance;
    StarsI16 speed;
    if (eta) *eta = 0;
    if (special) *special = 0;
    if (!points || count == 0U || waypoint_count + 1U > count) return 0;
    for (i = 0; i < waypoint_count; ++i) {
        distance = (StarsI16)(stars_get_distance(points[i].x, points[i].y,
            points[i + 1U].x, points[i + 1U].y) + 0.5);
        speed = (StarsI16)((points[i].warp_code >> 4) & 0x0fU);
        if (speed < 11) {
            speed = (StarsI16)(speed * speed);
        } else {
            status = gate ? gate(context, &points[i], &points[i + 1U]) : 0;
            if (status < 0) {
                if (special) *special = status == -1 ? -3 :
                    (status == -2 ? -2 : -1);
                return 1;
            }
            if (status == 0) {
                if (special) *special = 0;
                return 1;
            }
            speed = status;
        }
        if (speed < 1) speed = 1;
        turns = (StarsI16)(turns +
            (distance > speed ? (distance + speed - 1) / speed : 1));
    }
    if (eta) *eta = turns;
    return 1;
}

/* NE 8:08F2-0B31, SelectOursAtObject.  Selection/UI side effects are
 * callbacks; the coordinate resolution and own-fleet/planet/thing precedence
 * remain in the host implementation. */
StarsI16 stars_select_ours_at_object(StarsI16 input_x, StarsI16 input_y,
    StarsI16 own_player, const StarsNearestPoint *planets,
    StarsU16 planet_count, const StarsSelectableFleet *fleets,
    StarsU16 fleet_count, const StarsSelectableThing *things,
    StarsU16 thing_count, StarsI16 encoded_planet_count,
    StarsSelectObject select_planet, StarsSelectObject select_fleet,
    StarsSelectObject select_thing, void *context)
{
    StarsI16 x, y, enemy = -1;
    StarsI16 i;
    StarsNearestResult nearest;
    if (input_x == -1) {
        if (input_y & (StarsI16)0x8000U) {
            if (select_fleet) select_fleet(context,
                (StarsI16)(input_y & 0x7fff));
            return 1;
        }
        if (input_y < 0 || (StarsU16)input_y >= planet_count) return 0;
        x = planets[input_y].x;
        y = planets[input_y].y;
    } else {
        x = input_x;
        y = input_y;
    }
    for (i = 0; i < (StarsI16)fleet_count; ++i) {
        if (fleets[i].x == x && fleets[i].y == y) {
            if (fleets[i].player == own_player) {
                if (select_fleet) select_fleet(context, fleets[i].id);
                return 1;
            }
            if (enemy == -1) enemy = fleets[i].id;
        }
    }
    for (i = 0; i < (StarsI16)encoded_planet_count &&
            (StarsU16)i < planet_count; ++i) {
        if (planets[i].x == x && planets[i].y == y) {
            if (select_planet) select_planet(context, i);
            return 1;
        }
    }
    if (stars_find_nearest_object(x, y, 8, 0, 0, 0, 0, 0, 0,
            (const StarsNearestPoint *)things, thing_count, &nearest) &&
            nearest.thing_index >= 0 &&
            things[nearest.thing_index].selectable) {
        if (select_thing) select_thing(context, nearest.thing_index);
    } else if (enemy != -1) {
        if (select_fleet) select_fleet(context, enemy);
    }
    return 1;
}

/* NE 8:58A6-5DB9, CalcPlayerScore.  Planet/resource, race-stat, design
 * power-class, and active-fleet tables are explicit host inputs; the score
 * caps and category bonuses follow the original long arithmetic. */
StarsI32 stars_calc_player_score(StarsI16 player,
    const StarsScorePlanet *planets, StarsU16 planet_count,
    const StarsI16 *race_stats, StarsI16 dead,
    const StarsScoreDesign *designs, StarsU16 design_count,
    const StarsScoreFleet *fleets, StarsU16 fleet_count,
    StarsI16 extra_bonus, StarsScoreSummary *summary)
{
    StarsI32 score = 0, planets_owned = 0, resources = 0;
    StarsI32 starbase_count = 0, technology_levels = 0;
    StarsI32 categories[3] = {0, 0, 0};
    StarsI32 bonus_categories[2];
    StarsI32 design_class[16];
    StarsU16 i, j;
    StarsI16 value;
    if (!planets || !designs || !fleets) return 0;
    for (i = 0; i < planet_count; ++i) {
        if (planets[i].owner == player) {
            value = (StarsI16)((planets[i].mineral_value + 999L) / 1000L);
            if (value > 6) value = 6;
            if (value < 0) value = 0;
            ++planets_owned;
            score += value;
            if (planets[i].score_starbase) ++starbase_count;
            resources += planets[i].resources;
        }
    }
    score += resources / 30L + starbase_count * 3L;
    if (!dead && race_stats) {
        for (i = 0; i < 6; ++i) {
            value = race_stats[i];
            technology_levels += value;
            if (value < 4) score += value;
            else if (value < 7) score += value * 2 - 3;
            else if (value < 10) score += value * 3 - 9;
            else score += value * 4 - 18;
        }
    }
    for (i = 0; i < 16; ++i) design_class[i] = -1;
    for (i = 0; i < design_count && i < 16; ++i) {
        if (designs[i].disabled) continue;
        design_class[i] = designs[i].power < 1 ? 0 :
            (designs[i].power < 2000 ? 1 : 2);
    }
    for (i = 0; i < fleet_count; ++i) {
        if (fleets[i].owner != player || (fleets[i].flags & 0x0400U)) continue;
        for (j = 0; j < 16; ++j)
            if (design_class[j] >= 0)
                categories[design_class[j]] += fleets[i].component_counts[j];
    }
    bonus_categories[0] = categories[0];
    bonus_categories[1] = categories[1];
    if (bonus_categories[1] > planets_owned)
        bonus_categories[1] = planets_owned;
    if (bonus_categories[0] > planets_owned)
        bonus_categories[0] = planets_owned;
    score += bonus_categories[1] * 2L + bonus_categories[0] / 2L +
        extra_bonus;
    if (summary) {
        summary->score = score;
        summary->planet_count = planets_owned;
        summary->resources = resources;
        summary->starbase_count = starbase_count;
        summary->technology_levels = technology_levels;
        summary->fleet_counts[0] = categories[0];
        summary->fleet_counts[1] = categories[1];
        summary->fleet_counts[2] = categories[2];
    }
    return score;
}

/* UpdatePlayerScores copies _score into _scorex after maintaining the two
 * score-history metadata words. Preserve those words while packing the score
 * fields in their original order and compressing rgcsh with WPackLong. */
void stars_pack_score_history_record(const StarsScoreSummary *summary,
    StarsU16 flags, StarsU16 rank, StarsU8 *record)
{
    StarsU16 index;
    StarsU32 value;
    if (!summary || !record) return;
    stars_put_word(record, 0U, flags);
    stars_put_word(record, 2U, rank);
    value = (StarsU32)summary->score;
    record[4U] = (StarsU8)value;
    record[5U] = (StarsU8)(value >> 8);
    record[6U] = (StarsU8)(value >> 16);
    record[7U] = (StarsU8)(value >> 24);
    value = (StarsU32)summary->resources;
    record[8U] = (StarsU8)value;
    record[9U] = (StarsU8)(value >> 8);
    record[10U] = (StarsU8)(value >> 16);
    record[11U] = (StarsU8)(value >> 24);
    stars_put_word(record, 12U, (StarsU16)summary->planet_count);
    stars_put_word(record, 14U, (StarsU16)summary->starbase_count);
    for (index = 0U; index < 3U; ++index)
        stars_put_word(record, (StarsU16)(16U + index * 2U),
            stars_pack_long((StarsU32)summary->fleet_counts[index]));
    stars_put_word(record, 22U, (StarsU16)summary->technology_levels);
}

/* NE 8:34D8-3A00, FFleetMergeAll.  Merge IDs and table ownership are explicit
 * host inputs; component counts and the five-word mineral-weight span are
 * accumulated before foreign source fleets are compacted out of the table. */
StarsI16 stars_merge_all_fleet(StarsFleetTable *table,
    StarsFleetState *destination, const StarsI16 *merge_ids,
    StarsU16 merge_count, StarsU16 *deleted)
{
    StarsU16 i, j, id, amount;
    StarsFleetState *source;
    StarsU16 destination_id;
    if (deleted) *deleted = 0;
    if (!table || !destination || !merge_ids) return 0;
    destination_id = (StarsU16)((destination->bytes[0] |
        ((StarsU16)destination->bytes[1] << 8)) & 0x01ffU);
    for (i = 0; i < merge_count; ++i) {
        if (merge_ids[i] < 0 || (StarsU16)merge_ids[i] >= table->count)
            continue;
        id = (StarsU16)merge_ids[i];
        source = table->items[id];
        if (!source || source == destination) continue;
        for (j = 0; j < 16; ++j) {
            amount = (StarsU16)(source->bytes[12U + j * 2U] |
                ((StarsU16)source->bytes[13U + j * 2U] << 8));
            amount = (StarsU16)(amount +
                (StarsU16)(destination->bytes[12U + j * 2U] |
                    ((StarsU16)destination->bytes[13U + j * 2U] << 8)));
            destination->bytes[12U + j * 2U] = (StarsU8)amount;
            destination->bytes[13U + j * 2U] = (StarsU8)(amount >> 8);
        }
        for (j = 0; j < 20; ++j) {
            amount = (StarsU16)(destination->bytes[100U + j] +
                source->bytes[100U + j]);
            destination->bytes[100U + j] = (StarsU8)amount;
        }
        if (((StarsU16)(source->bytes[0] |
                ((StarsU16)source->bytes[1] << 8)) & 0x01ffU) != destination_id) {
            for (j = id; j + 1U < table->count; ++j)
                table->items[j] = table->items[j + 1U];
            --table->count;
            if (deleted) ++*deleted;
        }
    }
    return (StarsI16)(deleted && *deleted);
}

/* NE 8:68C6-7081, ValidateWaypoints.  Message emission and random tie
 * breaking are integration callbacks in the original; this host form keeps
 * target-type validation, coordinate refresh, fleet replacement, and galactic
 * boundary clamping explicit. */
StarsI16 stars_validate_waypoints(StarsWaypointFleet *fleets,
    StarsU16 fleet_count, StarsWaypointRecord *waypoints,
    StarsU16 waypoint_count, const StarsWaypointThing *things,
    StarsU16 thing_count, StarsI16 galaxy_limit, StarsI16 owner)
{
    StarsU16 i, j;
    StarsI16 changed = 0;
    StarsI16 best = -1;
    StarsI32 best_weight;
    if (!fleets || !waypoints) return 0;
    for (i = 0; i < fleet_count; ++i) {
        if (!(fleets[i].flags & 0x0400U)) {
            if (fleets[i].x < 1000) fleets[i].x = 1000;
            if (fleets[i].y < 1000) fleets[i].y = 1000;
            if (fleets[i].x > galaxy_limit + 1000)
                fleets[i].x = (StarsI16)(galaxy_limit + 1000);
            if (fleets[i].y > galaxy_limit + 1000)
                fleets[i].y = (StarsI16)(galaxy_limit + 1000);
        }
    }
    for (i = 0; i < waypoint_count; ++i) {
        if (((waypoints[i].type_flags >> 8) & 0x0fU) == 8U) {
            for (j = 0; j < thing_count; ++j)
                if (things[j].id == waypoints[i].target_id) break;
            if (j == thing_count || things[j].x != waypoints[i].x ||
                    things[j].y != waypoints[i].y) {
                waypoints[i].type_flags = (StarsU16)((waypoints[i].type_flags &
                    0xf0ffU) | 0x0400U);
                waypoints[i].target_id = (StarsI16)i;
                ++changed;
            } else {
                waypoints[i].x = things[j].x;
                waypoints[i].y = things[j].y;
            }
        } else if (((waypoints[i].type_flags >> 8) & 0x0fU) == 2U) {
            best = -1;
            best_weight = -1;
            for (j = 0; j < fleet_count; ++j) {
                if (fleets[j].x == waypoints[i].x &&
                        fleets[j].y == waypoints[i].y &&
                        fleets[j].id == waypoints[i].target_id &&
                        !(fleets[j].flags & 0x0400U)) {
                    best = (StarsI16)j;
                    break;
                }
            }
            if (best < 0) {
                for (j = 0; j < fleet_count; ++j) {
                    if (fleets[j].x == waypoints[i].x &&
                            fleets[j].y == waypoints[i].y &&
                            fleets[j].player == owner &&
                            !(fleets[j].flags & 0x0400U) &&
                            fleets[j].weight >= best_weight) {
                        best = (StarsI16)j;
                        best_weight = fleets[j].weight;
                    }
                }
            }
            if (best >= 0) {
                waypoints[i].target_id = fleets[best].id;
                waypoints[i].x = fleets[best].x;
                waypoints[i].y = fleets[best].y;
            } else {
                waypoints[i].type_flags = (StarsU16)((waypoints[i].type_flags &
                    0xf0ffU) | 0x0400U);
                ++changed;
            }
        }
    }
    return changed;
}

/* NE 2:510A-51FD, fixed component-table selectors.  Win16 segment bases are
 * explicit host spans; the original record strides and the special-weapon
 * base offset are unchanged. */
const StarsU8 *stars_huldef_sb_from_id(const StarsU8 *shared, StarsI16 id)
{
    return shared && id >= 0 ? shared + 0x26U + (StarsU16)id * 0x8fU : 0;
}

const StarsU8 *stars_huldef_from_id(const StarsU8 *planet_table,
    const StarsU8 *shared, StarsI16 id)
{
    if (id < 0) return 0;
    return id < 0x20 ? (planet_table + (StarsU16)id * 0x8fU) :
        stars_huldef_sb_from_id(shared, (StarsI16)(id - 0x20));
}

const StarsU8 *stars_engine_from_id(const StarsU8 *table, StarsI16 id)
{
    return table && id >= 0 ? table + (StarsU16)id * 0x4eU : 0;
}

const StarsU8 *stars_scanner_from_id(const StarsU8 *table, StarsI16 id)
{
    return table && id >= 0 ? table + 0x768U + (StarsU16)id * 0x38U : 0;
}

const StarsU8 *stars_shdef_t(const StarsU8 *table)
{ return table; }

const StarsU8 *stars_shdef_sbt(const StarsU8 *table)
{ return table; }

const StarsU8 *stars_planetary_from_id(const StarsU8 *table, StarsI16 id)
{
    return table && id >= 0 ? table + (StarsU16)id * 0x1bU : 0;
}

/* NE 2:51FE-524D, FLookupPartX.  The full table search remains a callback
 * boundary; this wrapper retains the exact hs.grhst and low-byte item setup
 * before dispatch. */
StarsI16 stars_lookup_part_x(StarsU8 *part, StarsU16 grhst, StarsU16 item,
    StarsLookupPart lookup, void *context)
{
    if (!part || !lookup) return 0;
    part[0] = (StarsU8)grhst;
    part[1] = (StarsU8)(grhst >> 8);
    part[2] = (StarsU8)item;
    part[3] = 0;
    return lookup(context, part);
}

/* NE 2:60BE-6147, LookupBestPlanetaryScanner. */
StarsI16 stars_lookup_best_planetary_scanner(StarsU8 *part,
    StarsLookupPart lookup, void *context)
{
    StarsI16 result;
    StarsU8 item;
    if (!part || !lookup) return 0;
    part[0] = 0;
    part[1] = 0x80U;
    part[2] = 8U;
    part[3] = 0;
    do {
        result = lookup(context, part);
        item = part[2];
        if (result == 1 || item == 0) break;
        part[2] = (StarsU8)(item - 1U);
    } while (1);
    return result;
}

/* NE 2:6148-62A8, TechStatus. */
StarsI16 stars_tech_status(const StarsI16 *current,
    const StarsI16 *required, StarsU16 current_tech)
{
    StarsU16 i;
    StarsI16 count = 0;
    StarsI16 next = 0;
    StarsI16 one_step = 0;
    if (!current || !required) return 0;
    for (i = 0; i < 6; ++i) {
        if (current[i] < required[i]) {
            ++count;
            if (i == (current_tech & 0x0fU)) {
                if (current[i] + 1 == required[i]) one_step = 1;
                else next = (StarsI16)i + 1;
            }
        }
    }
    if (!count) return 1;
    if (count == 1 && one_step) return 2;
    if (count == 1 && next)
        return (StarsI16)(required[next - 1] - current[next - 1] + 1);
    return 99;
}

/* NE 35:02A0-044E, CPlanetsInCircle.  The original uses the x-sorted planet
 * table as a scan optimization; the host form retains the exact inclusive
 * squared-distance predicate over the supplied table. */
StarsI16 stars_planets_in_circle(StarsI16 x, StarsI16 y, StarsU32 radius_squared,
    const StarsNearestPoint *planets, StarsU16 count)
{
    StarsU16 i;
    StarsI32 dx, dy;
    StarsU32 distance;
    StarsI16 found = 0;
    if (!planets) return 0;
    for (i = 0; i < count; ++i) {
        dx = (StarsI32)planets[i].x - x;
        dy = (StarsI32)planets[i].y - y;
        distance = (StarsU32)(dx * dx + dy * dy);
        if (distance <= radius_squared) ++found;
    }
    return found;
}

/* NE 35:0ADC-0B39, PctWormholeMoves. */
StarsI16 stars_pct_wormhole_moves(StarsU16 packed_destination_x)
{
    StarsI16 result = (StarsI16)(((packed_destination_x >> 2) & 0x03ffU) /
        5U - (2 - (packed_destination_x & 3U)));
    if (result < 0) result = 0;
    else if (result > 6) result = 6;
    return result;
}

/* NE 35:0224-029F, FreeLpth.  Thing records are the original fixed 18-byte
 * span; ownership of the backing table stays with the caller. */
StarsI16 stars_free_thing(StarsU8 *records, StarsU16 *count, StarsU16 index)
{
    if (!records || !count || index >= *count) return 0;
    if (index + 1U < *count)
        memmove(records + index * 18U, records + (index + 1U) * 18U,
            (size_t)(*count - index - 1U) * 18U);
    --*count;
    return 1;
}

/* NE 35:0000-0223, LpthNew.  The host caller supplies the backing span and
 * capacity; original packed owner/type identity, sorted insertion, duplicate
 * low-nine-bit allocation, and 18-byte compaction are preserved. */
StarsI16 stars_new_thing(StarsU8 *records, StarsU16 *count, StarsU16 capacity,
    StarsI16 player, StarsI16 thing_type, StarsU16 *index)
{
    StarsU16 identity, candidate, position = 0;
    if (!records || !count || !index || *count >= 0xfd2U ||
            *count >= capacity || player < 0 || thing_type < 0) return 0;
    identity = (StarsU16)(((StarsU16)thing_type << 13) |
        (((StarsU16)player & 0x0fU) << 9));
    candidate = identity;
    while (position < *count) {
        StarsU16 current = (StarsU16)(records[position * 18U] |
            ((StarsU16)records[position * 18U + 1U] << 8));
        if (current >= candidate) break;
        ++position;
    }
    while (position < *count) {
        StarsU16 current = (StarsU16)(records[position * 18U] |
            ((StarsU16)records[position * 18U + 1U] << 8));
        if (current != candidate) break;
        if ((candidate & 0x01ffU) == 0x01feU) return 0;
        candidate = (StarsU16)((candidate & 0xfe00U) |
            ((candidate + 1U) & 0x01ffU));
        ++position;
    }
    memmove(records + (position + 1U) * 18U,
        records + position * 18U, (size_t)(*count - position) * 18U);
    memset(records + position * 18U, 0, 18U);
    records[position * 18U] = (StarsU8)candidate;
    records[position * 18U + 1U] = (StarsU8)(candidate >> 8);
    ++*count;
    *index = position;
    return 1;
}

/* NE 35:1A96-1B9F, IdmGiveTraderPart. */
StarsI16 stars_give_trader_part(StarsU16 *trader_bits, StarsU16 part_bit,
    StarsU16 *goto_id)
{
    StarsI16 message = 0x10b;
    StarsU16 target = 0xcc04U;
    if (!trader_bits || !goto_id) return 0;
    *trader_bits = (StarsU16)(*trader_bits | part_bit);
    switch (part_bit) {
    case 2: target = 0xcb04U; break;
    case 4: target = 0xc206U; break;
    case 8: target = 0xc309U; break;
    case 0x10: target = 0xc706U; break;
    case 0x20: target = 0xc608U; break;
    case 0x40: target = 0xc507U; break;
    case 0x80: target = 0xc412U; break;
    case 0x100: message = 0x10c; target = 0xce1eU; break;
    case 0x200: target = 0xc008U; break;
    case 0x400: message = 0x10f; target = 0xcf0eU; break;
    case 0x800: target = 0xcc09U; break;
    default: break;
    }
    *goto_id = target;
    return message;
}

/* NE 35:064C-0ADC, IValidateWormholePos.  The Win16 object/planet tables are
 * explicit; thresholds and bit assignments follow the original distance
 * bands. */
StarsI16 stars_validate_wormhole_position(StarsI16 x, StarsI16 y,
    StarsU16 self_identity, StarsI16 map_size,
    const StarsWormholePoint *things, StarsU16 thing_count,
    const StarsNearestPoint *planets, StarsU16 planet_count,
    const StarsNearestPoint *fleets, StarsU16 fleet_count)
{
    StarsU16 i;
    StarsI32 dx, dy;
    StarsU32 distance;
    StarsI16 result = 0;
    StarsI32 limit = (StarsI32)map_size * 400L;
    if (x < 1000 || y < 1000 || x > limit + 0x578L ||
            y > limit + 0x578L) return 0x0f;
    for (i = 0; i < thing_count; ++i)
        if (things[i].x == x && things[i].y == y) return 0x0f;
    for (i = 0; i < planet_count; ++i)
        if (planets[i].x == x && planets[i].y == y) return 0x0f;
    for (i = 0; i < fleet_count; ++i)
        if (fleets[i].x == x && fleets[i].y == y) return 0x0f;
    if (x < 0x3f2 || y < 0x3f2 || x > limit + 0x56eL ||
            y > limit + 0x56eL) result = 4;
    for (i = 0; i < thing_count; ++i) {
        if (((things[i].flags >> 13) & 3U) != 2U) continue;
        dx = (StarsI32)x - things[i].x;
        dy = (StarsI32)y - things[i].y;
        distance = (StarsU32)(dx * dx + dy * dy);
        if (things[i].identity == self_identity) {
            if (distance < 0x1324U)
                result |= distance < 0x19U ? 8 : distance < 100U ? 4 :
                    distance < 900U ? 2 : 1;
        } else if (distance < 900U) {
            result |= distance < 0x10U ? 8 : distance < 0x40U ? 4 :
                distance < 0xe1U ? 2 : 1;
        }
    }
    for (i = 0; i < planet_count; ++i) {
        dx = (StarsI32)x - planets[i].x;
        dy = (StarsI32)y - planets[i].y;
        distance = (StarsU32)(dx * dx + dy * dy);
        if (distance < 0x310U)
            result |= distance < 0x19U ? 8 : distance < 100U ? 4 :
                distance < 400U ? 2 : 1;
    }
    return result;
}

/* NE 35:044E-064B, DrawThingGauge.  Font/GDI text rendering is an explicit
 * callback; mode selection and gauge values retain the original fields. */
StarsI16 stars_draw_thing_gauge(const StarsThingGaugeValues *source,
    StarsI16 mode, StarsThingGaugeDraw draw, void *context)
{
    StarsI32 values[3];
    StarsI16 segments = 1;
    StarsU16 i;
    if (!source) return 0;
    values[0] = values[1] = values[2] = 0;
    if (mode >= 0 && mode <= 2) {
        values[0] = source->values[mode];
    } else if (mode == 3 || mode == 4) {
        values[0] = source->total;
    } else if (mode == 5) {
        segments = 3;
        for (i = 0; i < 3; ++i) values[i] = source->values[i];
    } else {
        values[0] = source->total;
    }
    if (draw) draw(context, segments, values, source->total, mode);
    return 1;
}

/* NE 8:1BB4-1F0C, LinkFleets. Far next/previous pointers become indices in
 * the supplied fleet table; equal-coordinate fleets form circular chains. */
void stars_link_fleets(StarsFleetLinkNode *fleets, StarsU16 count)
{
    StarsI16 sorted[126];
    StarsU16 sorted_count = 0, i, j, insert;
    StarsI16 current, existing, next_index, previous_index;
    if (!fleets) return;
    for (i = 0; i < count; ++i) {
        fleets[i].next_index = (StarsI16)i;
        fleets[i].previous_index = (StarsI16)i;
        if (fleets[i].dead) continue;
        insert = sorted_count;
        for (j = 0; j < sorted_count; ++j) {
            current = sorted[j];
            if (fleets[i].y < fleets[current].y ||
                    (fleets[i].y == fleets[current].y &&
                     fleets[i].x <= fleets[current].x)) {
                insert = j;
                break;
            }
        }
        if (insert < sorted_count && fleets[i].x == fleets[sorted[insert]].x &&
                fleets[i].y == fleets[sorted[insert]].y) {
            existing = sorted[insert];
            next_index = fleets[existing].next_index;
            previous_index = fleets[existing].previous_index;
            fleets[i].next_index = next_index;
            fleets[i].previous_index = previous_index;
            fleets[next_index].previous_index = (StarsI16)i;
            fleets[previous_index].next_index = (StarsI16)i;
        } else {
            for (j = sorted_count; j > insert; --j)
                sorted[j] = sorted[j - 1U];
            sorted[insert] = (StarsI16)i;
            ++sorted_count;
        }
    }
}

/* NE 8:2F6-4A6, CalcPctSurvive.  The original resolves the planet's best
 * defense and operable-defense count through global tables.  This host API
 * accepts those resolved values explicitly, while retaining the original
 * owner/count guards, 0x0fff defense mask, cap, and x87 pow arithmetic. */
void stars_calc_pct_survive(StarsI16 owner, StarsU16 defense_count,
    StarsU16 operable_defenses, StarsI16 defense_damage,
    float *percent, float *percent_smart)
{
    double ordinary, smart;
    StarsU16 count;
    if (percent_smart) *percent_smart = 1.0f;
    if (!percent) return;
    if (owner == -1 || (defense_count & 0x0fffU) == 0U) {
        *percent = 1.0f;
        return;
    }
    count = (StarsU16)(defense_count & 0x0fffU);
    if (operable_defenses < count) count = operable_defenses;
    ordinary = pow(1.0 - (double)defense_damage / 1000.0,
        (double)count);
    *percent = (float)ordinary;
    if (percent_smart) {
        smart = pow(1.0 - (double)defense_damage / 2000.0,
            (double)count);
        *percent_smart = (float)smart;
    }
}
