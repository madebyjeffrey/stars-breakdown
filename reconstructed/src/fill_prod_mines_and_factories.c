#include "core.h"

enum {
    /* Production records use a seven-bit item kind beginning at bit 10 and a
     * three-bit record group beginning at bit 17.  The low ten bits are the
     * requested quantity. */
    PRODUCTION_GROUP_SHIFT = 17,
    PRODUCTION_GROUP_MASK = 7,
    PRODUCTION_KIND_SHIFT = 10,
    PRODUCTION_KIND_MASK = 0x7f,
    PRODUCTION_QUANTITY_MASK = 0x03ff,
    PRODUCTION_PLANET_GROUP = 1,

    PRODUCTION_FACTORY = 7,
    PRODUCTION_MINE = 8,
    PRODUCTION_TERRAFORM = 11,
    PRODUCTION_STARBASE = 12,

    /* These are the values used by the original AI's research and race
     * tables.  They are named here so callers do not have to repeat packed
     * shifts or unexplained literals. */
    PRODUCTION_STARBASE_RESEARCH_TYPE = 5,
    PRODUCTION_TERRAFORM_TECH_LEVEL = 0x1a,
    PRODUCTION_TERRAFORM_TURN = 100,
    PRODUCTION_STARBASE_RESOURCE_DIVISOR = 70,
    PRODUCTION_STARBASE_ROUNDING = 69,
    PRODUCTION_GAME_FLAG_LIMIT_FACTORY_RESOURCE = 0x0800
};

static StarsU16 production_group(StarsU32 item)
{
    return (StarsU16)((item >> PRODUCTION_GROUP_SHIFT) &
        PRODUCTION_GROUP_MASK);
}

static StarsU16 production_kind(StarsU32 item)
{
    return (StarsU16)((item >> PRODUCTION_KIND_SHIFT) &
        PRODUCTION_KIND_MASK);
}

static StarsU16 production_quantity(StarsU32 item)
{
    return (StarsU16)(item & PRODUCTION_QUANTITY_MASK);
}

static StarsI16 positive_capacity(StarsI16 maximum, StarsI16 operating,
    StarsU16 queued)
{
    StarsI16 result = (StarsI16)(maximum - operating - (StarsI16)queued);
    return result > 0 ? result : 0;
}

/* Return the largest integral quantity affordable in every resource whose
 * per-item cost is positive.  A zero cost dimension is deliberately ignored:
 * GetProductionCosts uses zero for dimensions that do not constrain an item. */
static StarsI16 affordable_quantity(const StarsI32 budget[4],
    const StarsI32 cost[4], StarsI16 limit)
{
    StarsI16 result = limit;
    StarsI16 index;
    StarsI32 candidate;
    StarsI16 constrained = 0;

    if (result < 1) return 0;
    for (index = 0; index < STARS_PRODUCTION_RESOURCE_COUNT; ++index) {
        if (cost[index] <= 0L) continue;
        constrained = 1;
        if (budget[index] < cost[index]) return 0;
        candidate = budget[index] / cost[index];
        if (candidate < (StarsI32)result)
            result = (StarsI16)candidate;
    }
    return constrained ? result : 0;
}

static void subtract_cost(StarsI32 budget[4], const StarsI32 cost[4],
    StarsI16 quantity)
{
    StarsI16 index;
    for (index = 0; index < STARS_PRODUCTION_RESOURCE_COUNT; ++index)
        budget[index] = (StarsI32)((StarsU32)budget[index] -
            (StarsU32)(cost[index] * (StarsI32)quantity));
}

static void copy_cost(StarsI32 destination[4], const StarsI32 source[4])
{
    StarsI16 index;
    for (index = 0; index < STARS_PRODUCTION_RESOURCE_COUNT; ++index)
        destination[index] = source[index];
}

static StarsI16 all_terraforming_technology(const StarsU8 technology[6])
{
    StarsI16 index;
    for (index = 0; index < 6; ++index)
        if (technology[index] != PRODUCTION_TERRAFORM_TECH_LEVEL)
            return 0;
    return 1;
}

static StarsI16 blocking_order_kind(StarsU32 order)
{
    StarsU16 kind = production_kind(order);
    /* The shortage path leaves an existing ship order or terraform order
     * alone.  This mirrors the original test of order kinds 3 and 11. */
    return (StarsI16)(kind == 3U || kind == PRODUCTION_TERRAFORM);
}

/* NE 16:2D72-37AF.  FillProductionQueue calls this routine after the normal
 * resource and queued-cost services have run.  Its semantic job is to turn
 * the remaining four-resource budget into mine, factory, and (late-game)
 * terraforming orders while respecting the planet's operable population.
 *
 * The original ABI obtains far pointers to the planet queue and global
 * production table.  The adapter below presents those records as packed
 * 32-bit values and resolves item costs through get_costs.  This keeps the
 * original data representation at one clearly marked boundary while making
 * the policy readable: a queued starbase can take priority, otherwise the AI
 * accounts for existing mine/factory work, spends the residual budget, and
 * finally queues terraforming only after every technology field reaches the
 * original sentinel value. */
StarsI16 stars_fill_prod_mines_and_factories(
    StarsFillProdMinesAndFactoriesInput *input)
{
    StarsI32 budget[STARS_PRODUCTION_RESOURCE_COUNT];
    StarsI32 mine_cost[STARS_PRODUCTION_RESOURCE_COUNT];
    StarsI32 factory_cost[STARS_PRODUCTION_RESOURCE_COUNT];
    StarsI32 terraform_cost[STARS_PRODUCTION_RESOURCE_COUNT];
    StarsI32 item_cost[STARS_PRODUCTION_RESOURCE_COUNT];
    StarsU16 queued_mines = 0U;
    StarsU16 queued_factories = 0U;
    StarsU16 queued_starbase = 0U;
    StarsU16 index;
    StarsU16 kind;
    StarsI16 mine_limit;
    StarsI16 factory_limit;
    StarsI16 mines_to_add;
    StarsI16 factories_to_add;
    StarsI16 terraform_to_add;
    StarsI16 shortage_mode = 0;
    StarsI16 starbase_to_add;
    StarsU32 item;
    StarsU32 order;

    if (!input || !input->add_item) return 0;

    for (index = 0; index < STARS_PRODUCTION_RESOURCE_COUNT; ++index) {
        budget[index] = (StarsI32)((StarsU32)input->available[index] -
            (StarsU32)input->queued_cost[index]);
        mine_cost[index] = 0L;
        factory_cost[index] = 0L;
        terraform_cost[index] = 0L;
    }

    /* Original AI4 returns before reading either queue when the signed
     * resource balance is negative (rgResLeft[3]'s high word is negative).
     * Mineral shortage handling below does not replace this early exit. */
    if (budget[3] < 0L) return 0;

    /* The first pass counts work already present in the planet-local queue.
     * The original starts after that queue's header; the adapter receives only
     * its item words, so no pointer arithmetic is needed here. */
    for (index = 0; index < input->planet_queue_count; ++index) {
        item = input->planet_queue_items ? input->planet_queue_items[index] : 0UL;
        if (production_group(item) != PRODUCTION_PLANET_GROUP) continue;
        kind = production_kind(item);
        if (kind == PRODUCTION_MINE)
            queued_mines = (StarsU16)(queued_mines + production_quantity(item));
        else if (kind == PRODUCTION_FACTORY)
            queued_factories = (StarsU16)(queued_factories +
                production_quantity(item));
        else if (kind == PRODUCTION_STARBASE)
            queued_starbase = (StarsU16)(queued_starbase +
                production_quantity(item));
    }

    /* The global list supplies the cost record associated with each kind.  A
     * later duplicate simply replaces the earlier cost, matching the way the
     * original reverse scan leaves the last matching table entry selected. */
    for (index = 0; index < input->global_queue_count; ++index) {
        item = input->global_queue_items ? input->global_queue_items[index] : 0UL;
        if (production_group(item) != PRODUCTION_PLANET_GROUP ||
                !input->get_costs) continue;
        kind = production_kind(item);
        if (kind != PRODUCTION_MINE && kind != PRODUCTION_FACTORY &&
                kind != PRODUCTION_TERRAFORM) continue;
        input->get_costs(input->context, item, item_cost);
        if (kind == PRODUCTION_MINE)
            copy_cost(mine_cost, item_cost);
        else if (kind == PRODUCTION_FACTORY)
            copy_cost(factory_cost, item_cost);
        else
            copy_cost(terraform_cost, item_cost);
    }

    /* A queued starbase is an exceptional priority for the research setting
     * represented by type five.  The +69 then /70 is the original ceiling
     * division, and the existing queue count remains the hard cap. */
    if (queued_starbase != 0U &&
            input->research_type == PRODUCTION_STARBASE_RESEARCH_TYPE) {
        starbase_to_add = (StarsI16)((budget[3] +
            PRODUCTION_STARBASE_ROUNDING) / PRODUCTION_STARBASE_RESOURCE_DIVISOR);
        if (starbase_to_add > (StarsI16)queued_starbase)
            starbase_to_add = (StarsI16)queued_starbase;
        if (starbase_to_add > 0) {
            input->add_item(input->context, PRODUCTION_STARBASE,
                (StarsU16)starbase_to_add, 1, 0);
            return 1;
        }
    }

    /* Negative or exhausted paired budgets put the planner in shortage mode.
     * An already selected ship/terraform order wins over AI rebalancing. */
    if (budget[0] < 0L || budget[1] < 0L || budget[2] < 0L || budget[3] < 0L ||
            (budget[0] == 0L && budget[1] <= 0L) ||
            (budget[2] == 0L && budget[3] <= 0L))
        shortage_mode = 1;
    order = input->current_order;
    if (shortage_mode && blocking_order_kind(order)) return 0;

    mine_limit = positive_capacity(input->max_mines,
        input->mines_operating, queued_mines);
    factory_limit = positive_capacity(input->max_factories,
        input->factories_operating, queued_factories);

    /* Factories are evaluated first because their cost is the residual budget
     * used by the original routine's first production division. */
    factories_to_add = affordable_quantity(budget, factory_cost, factory_limit);
    if (factories_to_add > 0) {
        input->add_item(input->context, PRODUCTION_FACTORY,
            (StarsU16)factories_to_add, 1, 1);
        subtract_cost(budget, factory_cost, factories_to_add);
    }

    mines_to_add = affordable_quantity(budget, mine_cost, mine_limit);
    if (mines_to_add > 0) {
        input->add_item(input->context, PRODUCTION_MINE,
            (StarsU16)mines_to_add, 1, 0);
        subtract_cost(budget, mine_cost, mines_to_add);
    }

    /* Terraforming is a late-game fill: the original requires all six tech
     * bytes to be 0x1a, waits past turn 100, and rounds the residual quotient
     * upward by one item.  Limit it by what the remaining budget can actually
     * buy, preventing the decompiler's signed division artifacts from making
     * an unbounded queue. */
    terraform_to_add = 0;
    if (input->turn > PRODUCTION_TERRAFORM_TURN &&
            all_terraforming_technology(input->technology)) {
        terraform_to_add = affordable_quantity(budget, terraform_cost, 0x7fff);
        if (terraform_to_add > 0 && terraform_to_add < 0x7fff)
            terraform_to_add = (StarsI16)(terraform_to_add + 1);
        if (terraform_to_add > 0) {
            input->add_item(input->context, PRODUCTION_TERRAFORM,
                (StarsU16)terraform_to_add, 1,
                shortage_mode ? 0 : 1);
            subtract_cost(budget, terraform_cost, terraform_to_add);
        }
    }

    return (StarsI16)(factories_to_add > 0 || mines_to_add > 0 ||
        terraform_to_add > 0);
}
