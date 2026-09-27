#include "core.h"

#define TRANSFER_SUPPLY_COUNT 5
#define TRANSFER_FLEET_TAG 0x8000U
#define TRANSFER_FULL_MINERAL_EVENT 0x42
#define TRANSFER_FULL_FUEL_EVENT 0x43
#define TRANSFER_RECIPIENT_MINERAL_EVENT 0x44
#define TRANSFER_RECIPIENT_FUEL_EVENT 0x45
#define TRANSFER_PARTIAL_MINERAL_EVENT 0x46
#define TRANSFER_PARTIAL_FUEL_EVENT 0x47
#define TRANSFER_PARTIAL_RECIPIENT_MINERAL_EVENT 0x48
#define TRANSFER_PARTIAL_RECIPIENT_FUEL_EVENT 0x49
#define TRANSFER_EMPTY_MINERAL_EVENT 0x4a
#define TRANSFER_EMPTY_FUEL_EVENT 0x4b

static StarsU16 transfer_tag(StarsU16 id, StarsU8 group)
{
    if (group == 2U) id = (StarsU16)(id | TRANSFER_FLEET_TAG);
    return id;
}

static StarsI16 transfer_event(StarsI16 mineral_event,
    StarsI16 fuel_event, StarsI16 supply)
{
    return supply == 4 ? fuel_event : mineral_event;
}

static void transfer_message(StarsTransferToOthersInput *input,
    StarsI16 recipient, StarsI16 event, StarsU16 source_tag,
    StarsU16 destination_tag, StarsI16 supply, StarsI32 requested,
    StarsI32 accepted)
{
    StarsTransferMessage message;
    message.recipient = recipient;
    message.event = event;
    message.source_tag = source_tag;
    message.destination_tag = destination_tag;
    message.supply = supply;
    message.requested = requested;
    message.accepted = accepted;
    input->send_message(input->context, &message);
}

/* NE 24:3088-34E2. Complete queued transfers into their destination objects.
 * The low and high nibbles of the original transfer record identify the
 * source and destination object kinds; this representation gives them names
 * instead of exposing the packed byte.  A destination may accept less than
 * requested because of capacity.  In that case the original sends the
 * partial-transfer events, and uses the distinct empty-transfer events when
 * none of the request was accepted. */
void stars_transfer_to_others(StarsTransferToOthersInput *input)
{
    StarsU16 record_index;
    StarsI16 supply;
    StarsI16 source_owner;
    StarsI16 destination_owner;
    StarsI32 requested;
    StarsI32 accepted;
    StarsI16 event;
    StarsTransferFullRecord *record;

    if (!input || !input->records || !input->lookup_owner ||
            !input->apply_cargo || !input->send_message)
        return;
    for (record_index = 0; record_index < input->record_count; ++record_index) {
        record = &input->records[record_index];
        source_owner = -1;
        destination_owner = -1;
        if (!input->lookup_owner(input->context,
                (StarsI16)record->destination_group, record->destination_id,
                &destination_owner))
            continue;
        if (record->source_group == 1U &&
                !input->lookup_owner(input->context, 1, record->source_id,
                    &source_owner))
            continue;
        if (record->source_group != 1U)
            source_owner = (StarsI16)((record->source_id >> 9) & 0x000fU);
        for (supply = 0; supply < TRANSFER_SUPPLY_COUNT; ++supply) {
            requested = (StarsI32)record->quantities[supply];
            if (requested == 0L) continue;
            accepted = input->apply_cargo(input->context,
                (StarsI16)record->destination_group, record->destination_id,
                supply, requested);
            if (accepted == requested) {
                event = transfer_event(TRANSFER_FULL_MINERAL_EVENT,
                    TRANSFER_FULL_FUEL_EVENT, supply);
                transfer_message(input, source_owner, event,
                    transfer_tag(record->source_id, record->source_group),
                    transfer_tag(record->destination_id,
                        record->destination_group), supply, requested, accepted);
                event = transfer_event(TRANSFER_RECIPIENT_MINERAL_EVENT,
                    TRANSFER_RECIPIENT_FUEL_EVENT, supply);
                transfer_message(input, destination_owner, event,
                    transfer_tag(record->source_id, record->source_group),
                    transfer_tag(record->destination_id,
                        record->destination_group), supply, requested, accepted);
            } else {
                event = transfer_event(TRANSFER_PARTIAL_MINERAL_EVENT,
                    TRANSFER_PARTIAL_FUEL_EVENT, supply);
                if (accepted == 0L)
                    event = transfer_event(TRANSFER_EMPTY_MINERAL_EVENT,
                        TRANSFER_EMPTY_FUEL_EVENT, supply);
                transfer_message(input, source_owner, event,
                    transfer_tag(record->source_id, record->source_group),
                    transfer_tag(record->destination_id,
                        record->destination_group), supply, requested, accepted);
                event = transfer_event(TRANSFER_PARTIAL_RECIPIENT_MINERAL_EVENT,
                    TRANSFER_PARTIAL_RECIPIENT_FUEL_EVENT, supply);
                transfer_message(input, destination_owner, event,
                    transfer_tag(record->source_id, record->source_group),
                    transfer_tag(record->destination_id,
                        record->destination_group), supply, requested, accepted);
            }
        }
    }
}
