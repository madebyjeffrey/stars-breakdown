#include "core.h"

#define CHEATER_MIN_TURN 10
#define CHEATER_FLEET_DESTROY_ROLL 12
#define CHEATER_FLEET_CARGO_ROLL 11
#define CHEATER_FLEET_CARGO_MIN_PERCENT 10
#define CHEATER_PLANET_PACKET_ROLL 8
#define CHEATER_PLANET_PACKET_MAX_PERCENT 31
#define CHEATER_PLANET_MINERAL_ROLL 15
#define CHEATER_PLANET_MINERAL_MAX_PERCENT 41
#define CHEATER_MAX_CARGO_LOSS 30000L
#define CHEATER_FLEET_MARK 0x0400

static StarsI32 cheater_percent_loss(StarsU32 amount, StarsI16 percent)
{
    StarsU32 loss;
    loss = (StarsU32)(((StarsI32)amount * percent) / 100L);
    if (loss == 0UL) loss = 1UL;
    return (StarsI32)loss;
}

/* NE 31:192A-1E16. Apply the game's anti-cheating penalties to fleets and
 * planets owned by players already marked dead.  The original uses compact
 * packed records; this adapter exposes the same three cargo holds and three
 * mineral stocks as ordinary 32-bit values.  Each affected object produces
 * the same user-visible message category as the original: fleet destruction
 * (0x104), fleet cargo loss (0x105), planetary mine-count loss (0x106), or a
 * single mineral-category loss (0x107). */
void stars_spank_the_cheaters(StarsSpankCheatersInput *input)
{
    StarsU8 dead[16];
    StarsI16 index;
    StarsI16 mineral;
    StarsI16 percent;
    StarsI32 loss;
    StarsI16 any_dead;
    StarsCheaterFleet *fleet;
    StarsCheaterPlanet *planet;

    if (!input || !input->dead_players || !input->random ||
            !input->send_message || input->player_count < 0)
        return;
    any_dead = 0;
    for (index = 0; index < input->player_count && index < 16; ++index) {
        dead[index] = input->dead_players[index] ? 1U : 0U;
        if (dead[index]) any_dead = 1;
    }
    if (!any_dead || input->turn <= CHEATER_MIN_TURN - 1)
        return;

    for (index = 0; input->fleets && index < (StarsI16)input->fleet_count;
            ++index) {
        fleet = &input->fleets[index];
        if (fleet->owner < 0 || fleet->owner >= 16 ||
                !dead[fleet->owner] || (fleet->flags & CHEATER_FLEET_MARK))
            continue;
        if (input->random(input->context, CHEATER_FLEET_DESTROY_ROLL) == 0) {
            fleet->flags = (StarsU16)(fleet->flags | CHEATER_FLEET_MARK);
            input->send_message(input->context, fleet->owner, 0x0104, -5,
                fleet->id, 0L, 0);
            continue;
        }
        percent = -1;
        for (mineral = 0; mineral < 3; ++mineral) {
            if (fleet->cargo[mineral] == 0UL) continue;
            if (percent < 0)
                percent = (StarsI16)(input->random(input->context,
                    CHEATER_FLEET_CARGO_ROLL) + CHEATER_FLEET_CARGO_MIN_PERCENT);
            loss = cheater_percent_loss(fleet->cargo[mineral], percent);
            fleet->cargo[mineral] -= (StarsU32)loss;
        }
        if (percent >= 0)
            input->send_message(input->context, fleet->owner, 0x0105, -5,
                fleet->id, (StarsI32)percent, 0);
    }

    for (index = 0; input->planets && index < (StarsI16)input->planet_count;
            ++index) {
        planet = &input->planets[index];
        if (planet->owner < 0 || planet->owner >= 16 ||
                !dead[planet->owner])
            continue;
        if (planet->mine_count != 0UL &&
                input->random(input->context, CHEATER_PLANET_PACKET_ROLL) == 0) {
            percent = (StarsI16)(input->random(input->context,
                CHEATER_PLANET_PACKET_MAX_PERCENT) + 5);
            loss = cheater_percent_loss(planet->mine_count, percent);
            planet->mine_count -= (StarsU32)loss;
            input->send_message(input->context, planet->owner, 0x0106, -5,
                planet->id, loss, 0);
            continue;
        }
        if (input->random(input->context, CHEATER_PLANET_MINERAL_ROLL) != 0)
            continue;
        mineral = input->random(input->context, 3);
        percent = (StarsI16)(input->random(input->context,
            CHEATER_PLANET_MINERAL_MAX_PERCENT) + 5);
        loss = (StarsI32)(((StarsI32)planet->mineral_stock[mineral] * percent) /
            100L);
        if (loss > 0L) {
            if (loss > CHEATER_MAX_CARGO_LOSS) loss = CHEATER_MAX_CARGO_LOSS;
            planet->mineral_stock[mineral] -= (StarsU32)loss;
            input->send_message(input->context, planet->owner, 0x0107, -5,
                planet->id, loss, (StarsI16)(mineral + 1));
        }
    }
}
