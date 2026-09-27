#include "core.h"

static StarsU32 dump_cargo_dword(const StarsU8 *data)
{
    return (StarsU32)data[0] | ((StarsU32)data[1] << 8) |
        ((StarsU32)data[2] << 16) | ((StarsU32)data[3] << 24);
}

static void dump_cargo_store_dword(StarsU8 *data, StarsU32 value)
{
    data[0] = (StarsU8)value;
    data[1] = (StarsU8)(value >> 8);
    data[2] = (StarsU8)(value >> 16);
    data[3] = (StarsU8)(value >> 24);
}

/* Genuine C reconstruction of FDumpCargo, NE 31:234A-24DC. */
StarsI16 stars_dump_cargo(StarsDumpCargoInput *input)
{
    StarsI16 index;
    StarsI16 owner;
    const StarsU8 *plan;
    StarsU8 *planet;

    if (!input || !input->battle_plan)
        return 0;
    for (index = 0; index < 3; ++index)
        if (dump_cargo_dword(input->minerals + index * 4) != 0U)
            break;
    if (index >= 3)
        return 0;
    owner = (StarsI16)((input->fleet_flags >> 9) & 0x0fU);
    plan = input->battle_plan(input->context, owner, input->plan_index);
    if (!plan || (dump_cargo_dword(plan) & 0x80000000UL) == 0U)
        return 0;
    if (input->planet == -1) {
        if (input->drop_salvage)
            input->drop_salvage(input->context, input->minerals, owner,
                &input->point);
    } else {
        planet = input->find_planet ?
            input->find_planet(input->context, input->planet) :
            (StarsU8 *)0;
        if (!planet)
            return 0;
        if (input->add_minerals)
            input->add_minerals(input->context, planet, input->minerals);
        else {
            for (index = 0; index < 3; ++index) {
                StarsU32 sum = dump_cargo_dword(planet + index * 4) +
                    dump_cargo_dword(input->minerals + index * 4);
                dump_cargo_store_dword(planet + index * 4, sum);
            }
        }
    }
    for (index = 0; index < 3; ++index)
        dump_cargo_store_dword(input->minerals + index * 4, 0U);
    return 1;
}
