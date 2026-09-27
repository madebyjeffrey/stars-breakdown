#include "core.h"

#define WIZARD_PACKED_GROUP_SHIFT 3
#define WIZARD_PACKED_GROUP_MASK 0x00f8U
#define WIZARD_PACKED_RELATION_EXCLUDED 0x0200U
#define WIZARD_PACKED_AI_LEVEL_MASK 0x1c00U
#define WIZARD_PACKED_AI_ID_MASK 0xe000U
#define WIZARD_PACKED_PLAYER_SIZE 192U
#define WIZARD_PACKED_DEAD_MARKER 0x0010U

void stars_new_game_wizard_resource_contract(
    StarsNewGameWizardResourceContract *contract)
{
    static const StarsNewGameWizardResourcePage pages[3] = {
        {
            0x0186U, 261, 210, 8, 27,
            {
                {0xffffU, 10, 10, 48, 10, 11, 891U},
                {1030U, 64, 8, 150, 12, 0, 0U},
                {1000U, 24, 40, 44, 12, 4, 420U},
                {1001U, 24, 52, 44, 12, 5, 505U},
                {1002U, 24, 64, 44, 12, 6, 609U},
                {1003U, 24, 76, 44, 12, 5, 491U},
                {1004U, 24, 88, 44, 12, 4, 393U},
                {1005U, 24, 124, 44, 12, 6, 622U},
                {1006U, 24, 136, 44, 12, 6, 617U},
                {1007U, 24, 148, 44, 12, 5, 495U},
                {1008U, 24, 160, 44, 12, 6, 584U},
                {1009U, 96, 40, 48, 12, 5, 502U},
                {1010U, 96, 52, 48, 12, 8, 817U},
                {1011U, 96, 64, 48, 12, 7, 716U},
                {1012U, 96, 76, 48, 12, 7, 727U},
                {1016U, 90, 94, 140, 12, 28, 2563U},
                {1017U, 90, 107, 140, 12, 21, 1931U},
                {1018U, 90, 120, 140, 12, 21, 1824U},
                {1019U, 90, 133, 140, 12, 17, 1529U},
                {1020U, 90, 146, 140, 12, 32, 3029U},
                {1021U, 90, 159, 140, 12, 21, 1953U},
                {1050U, 90, 172, 140, 12, 16, 1515U},
                {118U, 10, 190, 40, 14, 5, 431U},
                {2U, 60, 190, 40, 14, 6, 582U},
                {1070U, 110, 190, 40, 14, 7, 499U},
                {1071U, 160, 190, 40, 14, 7, 547U},
                {1072U, 210, 190, 40, 14, 7, 647U}
            }
        },
        {
            0x0187U, 261, 210, 8, 5,
            {
                {1072U, 210, 190, 40, 14, 7, 647U},
                {118U, 10, 190, 40, 14, 5, 431U},
                {2U, 60, 190, 40, 14, 6, 582U},
                {1070U, 110, 190, 40, 14, 7, 499U},
                {1071U, 160, 190, 40, 14, 7, 547U}
            }
        },
        {
            0x0188U, 261, 210, 8, 13,
            {
                {0xffffU, 22, 10, 200, 12, 34, 3194U},
                {291U, 10, 26, 12, 12, 0, 0U},
                {292U, 10, 42, 12, 12, 0, 0U},
                {293U, 10, 58, 12, 12, 0, 0U},
                {294U, 10, 74, 12, 12, 0, 0U},
                {295U, 10, 90, 12, 12, 0, 0U},
                {296U, 10, 106, 12, 12, 0, 0U},
                {297U, 10, 122, 12, 12, 0, 0U},
                {118U, 10, 190, 40, 14, 5, 431U},
                {2U, 60, 190, 40, 14, 6, 582U},
                {1070U, 110, 190, 40, 14, 7, 499U},
                {1071U, 160, 190, 40, 14, 7, 547U},
                {1072U, 210, 190, 40, 14, 7, 647U}
            }
        }
    };
    StarsI16 page;
    StarsI16 control;

    if (!contract)
        return;
    for (page = 0; page < 3; ++page) {
        contract->pages[page].resource_id = pages[page].resource_id;
        contract->pages[page].width = pages[page].width;
        contract->pages[page].height = pages[page].height;
        contract->pages[page].font_size = pages[page].font_size;
        contract->pages[page].control_count = pages[page].control_count;
        for (control = 0; control < 27; ++control)
            contract->pages[page].controls[control] =
                pages[page].controls[control];
    }
}

static void wizard_put16(StarsU8 *raw, StarsU16 offset, StarsU16 value)
{
    raw[offset] = (StarsU8)value;
    raw[(StarsU16)(offset + 1U)] = (StarsU8)(value >> 8);
}

static StarsU16 wizard_get16(const StarsU8 *raw, StarsU16 offset)
{
    return (StarsU16)(raw[offset] | ((StarsU16)raw[(StarsU16)(offset + 1U)] << 8));
}

static void wizard_copy_name(StarsU8 *raw, StarsU16 offset,
    const StarsU8 *name)
{
    StarsI16 i;

    for (i = 0; i < 32; ++i)
        raw[(StarsU16)(offset + (StarsU16)i)] = name[i];
}

static void wizard_sync_packed_player(StarsNewGameWizardInput *input,
    StarsI16 index)
{
    StarsU8 *raw;
    StarsU16 flags;
    StarsU16 stride;

    if (!input->packed_players || !input->players || index < 0 || index >= 16)
        return;
    stride = input->packed_player_stride;
    if (stride == 0U)
        stride = WIZARD_PACKED_PLAYER_SIZE;
    raw = input->packed_players + (StarsU16)(index * stride);
    flags = wizard_get16(raw, 6U);
    if (input->players[index].packed_flags_valid)
        flags = input->players[index].packed_flags;
    else {
        flags = (StarsU16)((flags & (StarsU16)~WIZARD_PACKED_GROUP_MASK) |
            ((StarsU16)(input->players[index].relation_group & 0x1fU) <<
            WIZARD_PACKED_GROUP_SHIFT));
        if (input->players[index].dead)
            flags |= WIZARD_PACKED_RELATION_EXCLUDED;
    }
    wizard_put16(raw, 6U, flags);
    raw[12] = (StarsU8)input->players[index].salt;
    raw[13] = (StarsU8)(input->players[index].salt >> 8);
    raw[14] = (StarsU8)(input->players[index].salt >> 16);
    raw[15] = (StarsU8)(input->players[index].salt >> 24);
    flags = wizard_get16(raw, 84U);
    if (input->players[index].dead)
        flags |= WIZARD_PACKED_DEAD_MARKER;
    else
        flags &= (StarsU16)~WIZARD_PACKED_DEAD_MARKER;
    wizard_put16(raw, 84U, flags);
    wizard_copy_name(raw, 128U, input->players[index].name);
    wizard_copy_name(raw, 160U, input->players[index].display_name);
}

/* Typed semantic reconstruction of NewGameWizard, NE 10:6022-6e44.
 * Win16 dialog procedures, packed _player records, localized names, and the
 * actual world generator are callback-owned; this routine retains the wizard
 * result protocol, retry/restart behavior, player-type decoding, relation
 * group repair, and cleanup boundaries. */

static void wizard_set_bits(StarsNewGameWizardInput *input, StarsU16 bits)
{
    if (input->game_bits)
        *input->game_bits = bits;
    if (input->ops->set_game_bits)
        input->ops->set_game_bits(input->ops->context, bits);
}

static void wizard_restore(StarsNewGameWizardInput *input,
    StarsU16 saved_bits)
{
    wizard_set_bits(input, (StarsU16)((input->game_bits ?
        *input->game_bits : saved_bits) & 0xff7fU) |
        (StarsU16)(saved_bits & 0x0080U));
}

static StarsI16 wizard_dialog(StarsNewGameWizardInput *input,
    StarsU16 stage, StarsI16 step)
{
    if (!input->ops->dialog)
        return 0;
    return input->ops->dialog(input->ops->context, stage, input->hwnd,
        input->read_only, step);
}

static StarsI16 wizard_random(StarsNewGameWizardInput *input, StarsI16 limit)
{
    if (!input->ops->random_value || limit <= 0)
        return 0;
    return input->ops->random_value(input->ops->context, limit);
}

static void wizard_copy_read_only(StarsNewGameWizardInput *input)
{
    StarsI16 i;

    if (!input->current_players || !input->players || !input->player_types)
        return;
    for (i = 0; i < input->current_player_count && i < 16; ++i) {
        if (input->current_players[i].active &&
                !input->current_players[i].dead) {
            input->player_types[i] = (StarsU8)((i << 2) | 2);
            input->players[i] = input->current_players[i];
        } else {
            input->player_types[i] = 0x19U;
        }
    }
}

static StarsI16 wizard_run_pages(StarsNewGameWizardInput *input,
    StarsI16 *step)
{
    StarsI16 result;

    for (;;) {
        result = wizard_dialog(input, 1, *step);
        if (result == 0)
            return 0;
        if (input->ops->init_players)
            input->ops->init_players(input->ops->context, *step,
                input->ai_level);
        if (result == 3)
            return result;
        for (;;) {
            result = wizard_dialog(input, 2, *step);
            if (result == 0)
                return 0;
            if (*step < 1)
                *step = 1;
            if (input->ops->init_players)
                input->ops->init_players(input->ops->context, *step,
                    input->ai_level);
            if (result == 1)
                break;
            if (result == 3)
                return result;
            result = wizard_dialog(input, 3, *step);
            if (result == 0)
                return 0;
            if (*step < 2)
                *step = 2;
            if (result != 1)
                return result;
        }
    }
}

static void wizard_assign_relation_groups(StarsNewGameWizardInput *input,
    StarsI16 count)
{
    StarsI16 groups[16];
    StarsI16 i;
    StarsI16 j;
    StarsI16 selected;
    StarsI16 candidate;

    for (i = 0; i < count; ++i) {
        if (input->players[i].packed_flags_valid) {
            if ((input->players[i].packed_flags &
                    WIZARD_PACKED_RELATION_EXCLUDED) != 0U)
                groups[i] = -1;
            else
                groups[i] = (StarsI16)((input->players[i].packed_flags &
                    WIZARD_PACKED_GROUP_MASK) >> WIZARD_PACKED_GROUP_SHIFT);
        } else if (input->players[i].dead)
            groups[i] = -1;
        else
            groups[i] = (StarsI16)(input->players[i].relation_group & 0x1fU);
    }
    for (i = 1; i < count; ++i) {
        if (groups[i] < 0)
            continue;
        for (j = 0; j < i && groups[j] != groups[i]; ++j)
            ;
        if (j < i) {
            selected = wizard_random(input, 2) == 0 ? j : i;
            groups[selected] = -1;
        }
    }
    for (i = 0; i < count; ++i) {
        if (groups[i] >= 0)
            continue;
        candidate = wizard_random(input, 0x20);
        for (;;) {
            for (j = 0; j < count &&
                    (j == i || groups[j] != candidate); ++j)
                ;
            if (j == count)
                break;
            candidate = (StarsI16)((candidate + 1) & 0x1f);
        }
        groups[i] = candidate;
    }
    for (i = 0; i < count; ++i) {
        input->players[i].relation_group = (StarsU8)(groups[i] & 0x1f);
        if (input->players[i].packed_flags_valid)
            input->players[i].packed_flags =
                (StarsU16)((input->players[i].packed_flags &
                    (StarsU16)~WIZARD_PACKED_GROUP_MASK) |
                ((StarsU16)(groups[i] & 0x1f) << WIZARD_PACKED_GROUP_SHIFT));
    }
}

static StarsI16 wizard_commit_players(StarsNewGameWizardInput *input)
{
    StarsI16 i;
    StarsI16 count;
    StarsI16 kind;
    StarsI16 source;
    StarsI16 race;
    StarsI16 ai_level;
    StarsI16 randomized_race;
    StarsU8 type;

    if (!input->player_types || !input->players)
        return 0;
    count = 0;
    while (count < 16 && input->player_types[count] != 0) {
        type = input->player_types[count];
        kind = (StarsI16)(type & 3U);
        source = (StarsI16)(type >> 2);
        if (kind == 1) {
            randomized_race = source >= 7;
            race = randomized_race ? wizard_random(input, 7) : source;
            input->players[count].race = race;
            input->players[count].ai_level = 0;
            if (randomized_race)
                input->players[count].salt = -1L;
            if (randomized_race && input->players[count].packed_flags_valid)
                input->players[count].packed_flags =
                    (StarsU16)((input->players[count].packed_flags &
                    (StarsU16)~(WIZARD_PACKED_AI_LEVEL_MASK |
                    WIZARD_PACKED_AI_ID_MASK)) |
                    WIZARD_PACKED_RELATION_EXCLUDED | WIZARD_PACKED_AI_ID_MASK);
            if (input->ops->configure_computer)
                input->ops->configure_computer(input->ops->context, count,
                    race, 0, randomized_race);
        } else if (kind == 2) {
            if (source >= 0 && source < 16 && input->temporary_players) {
                /* The raw path copies the complete temporary player record;
                 * the callback may then mirror that operation into the
                 * packed Win16 player storage. */
                input->players[count] = input->temporary_players[source];
                if (input->ops->copy_player)
                    input->ops->copy_player(input->ops->context, count,
                        source);
            }
        } else if (kind == 3) {
            race = (StarsI16)((type >> 2) & 7U);
            ai_level = (StarsI16)(type >> 5);
            if (race > 5)
                race = wizard_random(input, 6);
            if (ai_level > 3)
                ai_level = wizard_random(input, 4);
            input->players[count].race = race;
            input->players[count].ai_level = ai_level;
            if (input->players[count].packed_flags_valid)
                input->players[count].packed_flags =
                    (StarsU16)((input->players[count].packed_flags &
                    (StarsU16)~(WIZARD_PACKED_RELATION_EXCLUDED |
                    WIZARD_PACKED_AI_LEVEL_MASK | WIZARD_PACKED_AI_ID_MASK)) |
                    WIZARD_PACKED_RELATION_EXCLUDED |
                    ((StarsU16)(ai_level & 7) << 10) |
                    ((StarsU16)(race & 7) << 13));
            if (input->ops->configure_computer)
                input->ops->configure_computer(input->ops->context, count,
                    race, ai_level, 0);
        }
        if (input->ops->finalize_player)
            input->ops->finalize_player(input->ops->context, count, type,
                kind, source);
        ++count;
    }
    input->game_player_count = count;
    if (input->ops->set_player_count)
        input->ops->set_player_count(input->ops->context, count);
    for (i = 0; i < count; ++i) {
        if (input->ops->advantage_points &&
                (!input->players[i].packed_flags_valid ||
                 (input->players[i].packed_flags &
                  WIZARD_PACKED_RELATION_EXCLUDED) == 0U) &&
                input->ops->advantage_points(input->ops->context, i) < 0 &&
                input->ops->finalize_player) {
            if (input->default_player)
                input->players[i] = *input->default_player;
            /* CAdvantagePoints failure copies the default record and then
             * sets the raw fDead bit before callback-owned finalization. */
            input->players[i].dead = 1;
            input->ops->finalize_player(input->ops->context, i, 0xffU, -1,
                -1);
        }
        if (input->ops->ensure_player_name)
            input->ops->ensure_player_name(input->ops->context, i);
        if (input->ops->ensure_display_name)
            input->ops->ensure_display_name(input->ops->context, i);
        wizard_sync_packed_player(input, i);
    }
    if (input->relation_groups_valid)
        wizard_assign_relation_groups(input, count);
    if (input->ops->assign_relations)
        input->ops->assign_relations(input->ops->context, count);
    return count != 0;
}

StarsI16 stars_new_game_wizard(StarsNewGameWizardInput *input)
{
    StarsI16 result;
    StarsI16 step;
    StarsU16 saved_bits;

    if (!input || !input->ops)
        return -1;
    saved_bits = input->game_bits ? *input->game_bits : input->saved_game_bits;
    wizard_set_bits(input, (StarsU16)(saved_bits | 0x0080U));
    if (input->read_only) {
        wizard_copy_read_only(input);
        step = 0;
        result = wizard_run_pages(input, &step);
        if (result == 0) {
            wizard_restore(input, saved_bits);
            return 0;
        }
    } else {
        result = wizard_dialog(input, 0, 0);
        if (result == 2) {
            wizard_restore(input, saved_bits);
            return 0;
        }
        if (result == 0xd4) {
            if (input->ops->start_tutor)
                input->ops->start_tutor(input->ops->context);
            wizard_restore(input, saved_bits);
            return 0;
        }
        if (result == 0x430 || result == 0xd3) {
            if (input->ops->prepare_simple)
                input->ops->prepare_simple(input->ops->context, result);
            if (input->ops->init_players)
                input->ops->init_players(input->ops->context, 0,
                    input->ai_level);
            if (result == 0x430)
                input->simple_game_name_mode = 1;
            else {
                step = 0;
                result = wizard_run_pages(input, &step);
                if (result == 0) {
                    wizard_restore(input, saved_bits);
                    return 0;
                }
            }
        } else {
            step = 0;
            result = wizard_run_pages(input, &step);
            if (result == 0) {
                wizard_restore(input, saved_bits);
                return 0;
            }
        }
    }
    if (input->read_only) {
        wizard_restore(input, saved_bits);
        return 0;
    }
    for (;;) {
        if (!input->ops->get_game_name ||
                !input->ops->get_game_name(input->ops->context,
                    input->game_name, (StarsU16)sizeof input->game_name)) {
            if (input->simple_game_name_mode) {
                wizard_restore(input, saved_bits);
                return 0;
            }
            step = 0;
            result = wizard_run_pages(input, &step);
            if (result == 0) {
                wizard_restore(input, saved_bits);
                return 0;
            }
            continue;
        }
        if (input->ops->save_destroy_restore)
            input->ops->save_destroy_restore(input->ops->context);
        if (!wizard_commit_players(input)) {
            wizard_restore(input, saved_bits);
            return 0;
        }
        if (input->ops->close_title)
            input->ops->close_title(input->ops->context);
        if (input->ops->generate_world)
            input->ops->generate_world(input->ops->context);
        if (input->host_player == -1 && input->ops->bring_up_host)
            input->ops->bring_up_host(input->ops->context);
        wizard_restore(input, saved_bits);
        return 1;
    }
}
