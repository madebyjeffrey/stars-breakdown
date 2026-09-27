#include "core.h"
#include <stdio.h>
#include <string.h>

typedef struct TransferCargoProbe {
    StarsFleetBalanceFleet snapshots[2];
    StarsFleetBalanceFleet new_fleets[2];
    StarsI16 dialog_result;
    StarsI16 balance_calls;
    StarsI16 deletes;
    StarsI16 cancels;
    char order[24];
    StarsI16 order_count;
} TransferCargoProbe;

static void probe_event(TransferCargoProbe *probe, char event)
{
    if (probe->order_count < 23) {
        probe->order[probe->order_count] = event;
        ++probe->order_count;
        probe->order[probe->order_count] = '\0';
    }
}

static void clear_fleet(StarsFleetBalanceFleet *fleet, StarsI16 player)
{
    StarsI16 index;
    fleet->player = player;
    fleet->flags = 0U;
    for (index = 0; index < STARS_FLEET_BALANCE_DESIGNS; ++index) {
        fleet->ship_count[index] = 0;
        fleet->ship_status[index] = 0U;
    }
    for (index = 0; index < STARS_FLEET_BALANCE_RESOURCES; ++index)
        fleet->cargo[index] = 0L;
}

static StarsI16 probe_lookup(void *opaque, StarsI16 id,
    StarsFleetBalanceFleet *out)
{
    TransferCargoProbe *probe = (TransferCargoProbe *)opaque;
    if (id < 0 || id > 1 || !out)
        return 0;
    *out = probe->snapshots[id];
    return 1;
}

static StarsI16 probe_weight(void *opaque, StarsI16 player, StarsI16 design,
    StarsI16 stat)
{
    (void)opaque;
    (void)player;
    (void)design;
    return stat == 1 ? 10 : 4;
}

static StarsI16 prepare(void *opaque, StarsTransferObject *object,
    StarsI16 mode)
{
    (void)mode;
    (void)object;
    probe_event((TransferCargoProbe *)opaque, 'P');
    return 1;
}

static StarsI16 split(void *opaque, StarsTransferObject *source,
    StarsTransferObject *result)
{
    TransferCargoProbe *probe = (TransferCargoProbe *)opaque;
    (void)source;
    probe_event(probe, 'S');
    result->object_type = 4;
    result->object_id = 1;
    result->has_cargo = 1;
    return 1;
}

static StarsI16 enumerate_hulls(void *opaque,
    const StarsTransferObject *object, StarsI16 *hulls, StarsI16 capacity)
{
    (void)object;
    (void)capacity;
    probe_event((TransferCargoProbe *)opaque, 'H');
    hulls[0] = 0;
    return 1;
}

static StarsI16 show_dialog(void *opaque)
{
    TransferCargoProbe *probe = (TransferCargoProbe *)opaque;
    probe_event(probe, 'D');
    return probe->dialog_result;
}

static void balance(void *opaque, StarsTransferObject *first,
    StarsTransferObject *second)
{
    TransferCargoProbe *probe = (TransferCargoProbe *)opaque;
    StarsFleetTransferCargoBalanceInput input;
    (void)first;
    (void)second;
    probe_event(probe, 'B');
    ++probe->balance_calls;
    input.new_fleets[0] = &probe->new_fleets[0];
    input.new_fleets[1] = &probe->new_fleets[1];
    input.fleet_ids[0] = 0;
    input.fleet_ids[1] = 1;
    input.player = 3;
    input.lookup_fleet = probe_lookup;
    input.max_weight = probe_weight;
    input.context = probe;
    stars_fleet_transfer_cargo_balance(&input);
}

static void delete_fleet(void *opaque, StarsTransferObject *object)
{
    TransferCargoProbe *probe = (TransferCargoProbe *)opaque;
    (void)object;
    probe_event(probe, 'X');
    ++probe->deletes;
}

static void cancel_memory(void *opaque)
{
    TransferCargoProbe *probe = (TransferCargoProbe *)opaque;
    probe_event(probe, 'C');
    ++probe->cancels;
}

static void refresh(void *opaque, StarsI16 type)
{
    (void)type;
    probe_event((TransferCargoProbe *)opaque, 'R');
}

static void invalidate(void *opaque, StarsI16 window)
{
    (void)window;
    probe_event((TransferCargoProbe *)opaque, 'I');
}

static void advance(void *opaque)
{
    probe_event((TransferCargoProbe *)opaque, 'T');
}

static void install(StarsTransferOps *ops, TransferCargoProbe *probe)
{
    memset(ops, 0, sizeof *ops);
    ops->prepare_object = prepare;
    ops->jettison = (StarsTransferJettison)0;
    ops->split_fleet = split;
    ops->enumerate_hulls = enumerate_hulls;
    ops->show_dialog = show_dialog;
    ops->balance = balance;
    ops->delete_fleet = delete_fleet;
    ops->cancel_memory = cancel_memory;
    ops->refresh_selection = refresh;
    ops->invalidate_window = invalidate;
    ops->advance_tutor = advance;
    ops->context = probe;
}

static void setup_fleets(TransferCargoProbe *probe)
{
    StarsI16 index;
    clear_fleet(&probe->snapshots[0], 3);
    clear_fleet(&probe->snapshots[1], 3);
    clear_fleet(&probe->new_fleets[0], 3);
    clear_fleet(&probe->new_fleets[1], 3);
    probe->snapshots[0].ship_count[0] = 10;
    probe->snapshots[0].ship_status[0] = 20U;
    probe->snapshots[1].ship_count[0] = 10;
    probe->snapshots[1].ship_status[0] = 50U;
    probe->new_fleets[0].ship_count[0] = 8;
    probe->new_fleets[1].ship_count[0] = 12;
    probe->new_fleets[0].cargo[0] = 20L;
    probe->new_fleets[0].cargo[1] = 15L;
    probe->new_fleets[0].cargo[4] = 30L;
    probe->new_fleets[1].cargo[0] = 4L;
    probe->new_fleets[1].cargo[1] = 5L;
    probe->new_fleets[1].cargo[4] = 7L;
    for (index = 0; index < 24; ++index)
        probe->order[index] = '\0';
    probe->order_count = 0;
    probe->balance_calls = 0;
    probe->deletes = 0;
    probe->cancels = 0;
}

static StarsI16 expect(StarsI16 condition)
{
    return condition ? 0 : 1;
}

int main(void)
{
    TransferCargoProbe probe;
    StarsTransferInput transfer;
    StarsTransferOps ops;
    StarsI32 before_resources[STARS_FLEET_BALANCE_RESOURCES];
    StarsI16 resource;
    StarsI16 failures;

    setup_fleets(&probe);
    probe.dialog_result = 1;
    for (resource = 0; resource < STARS_FLEET_BALANCE_RESOURCES; ++resource)
        before_resources[resource] = probe.new_fleets[0].cargo[resource] +
            probe.new_fleets[1].cargo[resource];
    transfer.first.object_type = 1;
    transfer.first.object_id = 0;
    transfer.first.has_cargo = 1;
    transfer.first.is_selected_planet = 1;
    transfer.second.object_type = 4;
    transfer.second.object_id = 1;
    transfer.second.has_cargo = 1;
    transfer.second.is_selected_planet = 0;
    transfer.mode = 1;
    transfer.dialog_result = 0;
    transfer.scanner_mode = 5;
    transfer.scanner_bits = 1;
    install(&ops, &probe);
    transfer.ops = &ops;
    stars_transfer_stuff(&transfer);
    failures = 0;
    failures += expect(probe.order[0] == 'P' && probe.order[1] == 'S' &&
        probe.order[2] == 'H' && probe.order[3] == 'T' &&
        probe.order[4] == 'D' && probe.order[5] == 'B');
    failures += expect(probe.order[6] == 'R' && probe.order[7] == 'I' &&
        probe.order[8] == 'I' && probe.order[9] == '\0');
    failures += expect(probe.balance_calls == 1 && probe.deletes == 0 &&
        probe.cancels == 0 && transfer.mode == -1);
    for (resource = 0; resource < STARS_FLEET_BALANCE_RESOURCES; ++resource)
        failures += expect(probe.new_fleets[0].cargo[resource] +
            probe.new_fleets[1].cargo[resource] == before_resources[resource]);
    failures += expect(probe.new_fleets[0].cargo[4] < 30L &&
        probe.new_fleets[1].cargo[4] > 7L);
    failures += expect((probe.new_fleets[0].ship_status[0] &
        STARS_FLEET_BALANCE_STATUS_MASK) != 20U);

    setup_fleets(&probe);
    probe.dialog_result = 0;
    transfer.second.object_type = 4;
    transfer.second.object_id = 1;
    transfer.second.has_cargo = 1;
    transfer.mode = 1;
    transfer.scanner_bits = 0;
    transfer.ops = &ops;
    stars_transfer_stuff(&transfer);
    failures += expect(probe.balance_calls == 0 && probe.deletes == 1 &&
        probe.cancels == 1 && transfer.mode == -1);
    failures += expect(probe.order[4] == 'D' && probe.order[5] == 'X' &&
        probe.order[6] == 'C' && probe.order[7] == '\0');

#ifndef TARGET_NATIVE
    printf("TRANSFER_STUFF_CARGO_E2_CHECKS=12 FAILURES=%d RESULT=%s\n",
        (int)failures, failures == 0 ? "PASS" : "FAIL");
#endif
    return failures == 0 ? 0 : 1;
}
