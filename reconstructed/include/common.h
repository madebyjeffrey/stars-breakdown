#ifndef STARS_COMMON_H
#define STARS_COMMON_H
#include "core.h"
/* Near pointers are offsets in the complete DGROUP image. */
StarsU16 stars_number_int(StarsU8 *, StarsI16, StarsU16);
StarsU16 stars_number_long(StarsU8 *, StarsI32, StarsU16);
StarsU16 stars_number_scaled(StarsU8 *, StarsI32, StarsU16);
StarsU16 stars_number_commas(StarsU8 *, StarsU16, StarsI32);
void stars_get_true_hull_cost(const StarsU8 *, StarsU16 *);
StarsI32 stars_long_from_serial_char(StarsI16);
double stars_get_distance(StarsI16, StarsI16, StarsI16, StarsI16);
StarsI16 stars_compare_fleet_point(const StarsU8 *, const StarsU8 *);
StarsI16 stars_compare_fleet_point2(const StarsU8 *, const StarsU8 *);
StarsI16 stars_intersect_circle_line(StarsI16, StarsI16, StarsI16, StarsI16,
    StarsI16, StarsI16, StarsU32, StarsI16, StarsI16 *, StarsI16 *);
/* Lookup helpers use a host byte span for the original far arrays.  The
 * returned value is an element index, or -1 for the original null pointer. */
StarsI16 stars_thing_index_from_id(const StarsU8 *, StarsU16, StarsU16,
    StarsI16);
StarsI16 stars_planet_index_from_id(const StarsU8 *, StarsU16, StarsU16,
    StarsU16, StarsI16);
StarsI16 stars_fleet_pointer_index(const StarsU32 *, StarsU16, StarsU32);
typedef StarsI16 (*StarsUnlinkFile)(void *, const StarsU8 *);
StarsI16 stars_clear_file(const StarsU8 *, const StarsU8 *,
    const StarsU8 *const *, StarsU16, StarsI16, StarsU8 *, StarsU16,
    StarsUnlinkFile, void *);
typedef struct StarsShieldComponent {
    StarsU16 kind;
    StarsU16 packed_count_type;
    StarsU16 damage_points;
} StarsShieldComponent;
StarsU32 stars_shield_power(const StarsShieldComponent *, StarsU16,
    StarsI16);
StarsI16 stars_primary_ship_design(const StarsU16 *, const StarsU16 *,
    StarsU16, StarsI16 *);
StarsU16 stars_fleet_code(StarsU16, StarsI16, StarsI16);
StarsI16 stars_stargate_index(const StarsU8 *, const StarsU8 *, StarsU16,
    StarsU16);
StarsI16 stars_fleet_can_jumpgate(const StarsU16 *, const StarsU8 *,
    StarsU16, StarsU16);
StarsU32 stars_fleet_weight(const StarsU16 *, const StarsU32 *,
    const StarsU32 *);
StarsI16 stars_match_target(const StarsU16 *, const StarsU8 *, StarsU16,
    StarsI16, StarsI16);
StarsI16 stars_fleet_index_from_grouped_id(const StarsU16 *, StarsU16,
    const StarsU16 *, StarsU16, StarsI16, StarsI16);
StarsI16 stars_lookup_object_index(StarsI16, StarsI16, const StarsU16 *,
    StarsU16, const StarsU16 *, StarsU16, const StarsU16 *, StarsU16);
StarsI16 stars_selected_planet_index(StarsI16, StarsI16, const StarsU16 *,
    StarsU16);
StarsI16 stars_selected_fleet_index(StarsI16, StarsU16, const StarsU16 *,
    StarsU16, const StarsU16 *, StarsU16);
StarsI16 stars_lookup_thing_record(StarsI16, StarsU8 *, StarsU16, StarsU16,
    StarsU8 *, StarsI16);
typedef StarsI16 (*StarsPartLookup)(void *, StarsU16, StarsU16);
StarsI16 stars_can_build_design(StarsI16, const StarsShieldComponent *,
    StarsU16, StarsPartLookup, void *);
StarsI16 stars_find_similar_design(StarsU16, const StarsU16 *,
    const StarsU16 *, StarsU16, const StarsU16 *, const StarsU16 *,
    const StarsU16 *, const StarsU16 *, const StarsU16 *, StarsU16,
    StarsU16);
void stars_decorate_hull_name(const StarsU8 *, StarsU16, const StarsU16 *,
    StarsU16, StarsU16, StarsU16, StarsU16, StarsU8 *, StarsU16);
typedef void (*StarsLogOutput)(void *, StarsI16, const StarsU8 *);
StarsI16 stars_turn_log(StarsI16, StarsI16, StarsI16, const StarsU8 *,
    StarsU8 *, StarsU16, StarsLogOutput, void *);
typedef void (*StarsMarkDesign)(void *, StarsI16, StarsU16, StarsU16);
void stars_mark_planet(StarsU8 *, StarsU8 *, StarsU16, StarsI16, StarsU16,
    StarsMarkDesign, void *);
void stars_mark_fleet(StarsU8 *, StarsU8 *, StarsU16, StarsU8 *, StarsU16,
    StarsU16);
void stars_bound_points(StarsU8 *, StarsU16, StarsU16, StarsI16);
StarsU16 stars_pack_long(StarsU32);

/* Resolved component values used by the original LComputePower procedure.
 * The host adapter removes the original far lookup table while preserving
 * the component-kind and packed-count arithmetic. */
typedef struct StarsPowerComponent {
    StarsU16 kind;
    StarsU16 packed_count_type;
    StarsU16 fuel_max;
    StarsU16 cargo_max;
    StarsU16 damage_points;
    StarsU16 flags;
} StarsPowerComponent;
StarsI32 stars_compute_design_power(const StarsPowerComponent *, StarsU16,
    StarsI16);
typedef struct StarsPowerDesign {
    StarsU16 hidden_flags;
    StarsU16 component_count;
    const StarsPowerComponent *components;
    StarsI16 speed;
    StarsI32 power;
} StarsPowerDesign;
void stars_compute_design_powers(StarsPowerDesign *, StarsU16);
typedef struct StarsRamScoopDesign {
    StarsU16 ship_count;
    StarsU16 scoop_count;
    StarsU8 fuel_used[11];
} StarsRamScoopDesign;
StarsI32 stars_fuel_gain_from_ram_scoops(const StarsRamScoopDesign *,
    StarsU16, StarsI16, StarsI32);
typedef struct StarsScannerDesign {
    StarsU16 ship_count;
    StarsI16 scanner_range;
    StarsI16 planet_range;
    StarsI16 detection_percent;
    StarsI16 steal_flags;
} StarsScannerDesign;
StarsI16 stars_get_fleet_scanner_range(const StarsScannerDesign *, StarsU16,
    StarsI16 *, StarsI16 *, StarsI16 *);
StarsI16 stars_get_cached_fleet_scanner_range(const StarsScannerDesign *,
    StarsU16, StarsI16 *, StarsI16 *, StarsI16 *);
StarsI16 stars_get_planet_scanner_range(StarsI16, StarsI16, StarsI32,
    StarsI16, StarsI16, StarsI16, StarsI16 *);
typedef struct StarsScannerComponent {
    StarsU16 kind;
    StarsU16 packed_count_type;
    StarsI16 range;
    StarsI16 abilities;
} StarsScannerComponent;
StarsI16 stars_get_design_scanner_range(const StarsScannerComponent *,
    StarsU16, StarsI32, StarsI32, StarsI16, const StarsU8 *, StarsI16 *,
    StarsI16 *, StarsI16 *);
StarsI32 stars_fleet_design_damage(const StarsU8 *, StarsU16,
    const StarsI16 *, StarsU16);
StarsI16 stars_lookup_orbiting_transfer(StarsI16, StarsI16,
    const StarsU8 *, StarsU16, StarsU16, const StarsU8 *, StarsU16, StarsU16,
    StarsI16, StarsI16, StarsI16, StarsU8 *);
typedef StarsI16 (*StarsStargateDamageCheck)(void *, StarsI16, StarsI16,
    StarsI16, StarsI16, StarsI16 *);
StarsI16 stars_can_fleet_use_stargates(StarsI16, StarsI16, StarsI16,
    StarsI16, StarsI16, StarsI16, StarsI16, StarsI16, StarsI16, StarsI16,
    StarsI16, const StarsI32 *, StarsI16, const StarsU16 *, const StarsI16 *,
    StarsU16, StarsStargateDamageCheck, void *);
typedef struct StarsCostComponent {
    StarsU16 kind;
    StarsU16 packed_count_type;
    StarsU16 ore_cost[3];
    StarsU16 resource_cost;
    StarsU16 weight_empty;
    StarsU16 cargo_max;
} StarsCostComponent;
typedef struct StarsDesignCost {
    StarsI16 damage_points;
    StarsU16 ore_cost[3];
    StarsU16 resource_cost;
    StarsU16 weight_empty;
    StarsU16 hull_weight_empty;
} StarsDesignCost;
void stars_update_design_cost(StarsDesignCost *, const StarsCostComponent *,
    StarsU16, StarsI16);
typedef struct StarsPlanetProduction {
    StarsU16 count;
    StarsU16 capacity;
    StarsU8 *records;
} StarsPlanetProduction;
typedef struct StarsPlanetState {
    StarsU8 bytes[52];
    StarsPlanetProduction *production;
} StarsPlanetState;
StarsI16 stars_duplicate_planet(StarsPlanetState *, const StarsPlanetState *);
typedef void (*StarsPlanetLookupHook)(void *, StarsPlanetState *, StarsPlanetState *);
StarsI16 stars_lookup_planet(StarsPlanetState **, StarsU16, StarsI16,
    StarsPlanetState *, StarsPlanetLookupHook, void *);
typedef struct StarsFleetOrders {
    StarsU16 count;
    StarsU16 capacity;
    StarsU8 *records;
} StarsFleetOrders;
typedef struct StarsFleetState {
    StarsU8 bytes[124];
    StarsFleetOrders *orders;
} StarsFleetState;
StarsI16 stars_duplicate_fleet(const StarsFleetState *, StarsFleetState *);
/* NE 23:94DB-9556 and 23:9632-9A2B.  A player-transfer creates a destination
 * design bank by matching equivalent active designs, allocating the first
 * hidden slot for each unmatched design, and materializing the source record
 * plus its fleet counts/status.  Design records retain the original 0x93-byte
 * stride and packed offsets. */
#define STARS_TRANSFER_DESIGN_COUNT 16U
#define STARS_TRANSFER_DESIGN_BYTES 0x93U
StarsI16 stars_materialize_player_transfer_designs(
    const StarsU8 *, const StarsI16 *, const StarsI16 *,
    StarsU8 *, StarsI16 *, StarsI16 *, StarsU16, StarsU16 *, StarsI16 *);
typedef void (*StarsFleetDeleteHook)(void *, StarsFleetState *, StarsI16,
    StarsI16);
StarsI16 stars_delete_fleet(StarsFleetState **, StarsU16 *, StarsI16,
    StarsI16, StarsI16, StarsFleetDeleteHook, void *);
typedef struct StarsFleetTable {
    StarsFleetState **items;
    StarsU16 count;
    StarsU16 capacity;
    StarsU16 player_counts[16];
} StarsFleetTable;
StarsI16 stars_new_fleet(StarsFleetTable *, StarsI16, StarsI16, StarsI16,
    StarsI16, StarsFleetState **);
typedef void (*StarsFleetSplitHook)(void *, StarsU16);
StarsI16 stars_split_fleet(StarsFleetTable *, const StarsFleetState *,
    StarsFleetState **, StarsFleetSplitHook, void *);
typedef struct StarsNearestPoint {
    StarsI16 x;
    StarsI16 y;
} StarsNearestPoint;
typedef struct StarsNearestResult {
    StarsI16 x;
    StarsI16 y;
    StarsI16 kind;
    StarsI16 planet_index;
    StarsI16 fleet_index;
    StarsI16 waypoint_index;
    StarsI16 thing_index;
} StarsNearestResult;
StarsI16 stars_find_nearest_object(StarsI16, StarsI16, StarsU16,
    const StarsNearestPoint *, StarsU16, const StarsNearestPoint *, StarsU16,
    const StarsNearestPoint *, StarsU16, const StarsNearestPoint *, StarsU16,
    StarsNearestResult *);
StarsI16 stars_lookup_fleet(StarsFleetState **, StarsU16, StarsI16,
    StarsFleetState *, StarsI16);
typedef struct StarsBombComponent {
    StarsU16 kind;
    StarsU16 packed_damage_type;
    StarsU16 defense_power;
    StarsU16 fuel_max;
} StarsBombComponent;
typedef struct StarsBombDesignSlot {
    const StarsBombComponent *components;
    StarsU16 component_count;
} StarsBombDesignSlot;
typedef struct StarsBombFleet {
    StarsI16 player;
    StarsI16 planet;
    StarsI16 next_index;
    StarsU16 flags;
    StarsU16 component_counts[16];
} StarsBombFleet;
typedef struct StarsBombDamage {
    StarsI32 people;
    StarsI32 people_min;
    StarsI32 people_smart;
    StarsI32 building;
    StarsI32 terraforming;
    StarsI16 multiple;
} StarsBombDamage;
StarsI16 stars_calc_fleet_bomb_damage(StarsBombFleet *, StarsU16, StarsI16,
    const StarsBombDesignSlot *, StarsI32, double, double, StarsBombDamage *);
typedef void (*StarsFleetCargoBalance)(void *, StarsFleetState *,
    StarsFleetState *);
StarsI16 stars_split_all_fleet(StarsFleetTable *, StarsFleetState *,
    StarsFleetCargoBalance, void *, StarsU16 *);
typedef struct StarsEtaPoint {
    StarsI16 x;
    StarsI16 y;
    StarsU16 warp_code;
} StarsEtaPoint;
typedef StarsI16 (*StarsEtaGateCheck)(void *, const StarsEtaPoint *,
    const StarsEtaPoint *);
StarsI16 stars_calculate_eta(const StarsEtaPoint *, StarsU16, StarsU16,
    StarsEtaGateCheck, void *, StarsI16 *, StarsI16 *);
typedef struct StarsSelectableFleet {
    StarsI16 id;
    StarsI16 player;
    StarsI16 x;
    StarsI16 y;
} StarsSelectableFleet;
typedef struct StarsSelectableThing {
    StarsI16 x;
    StarsI16 y;
    StarsI16 selectable;
} StarsSelectableThing;
typedef void (*StarsSelectObject)(void *, StarsI16);
StarsI16 stars_select_ours_at_object(StarsI16, StarsI16, StarsI16,
    const StarsNearestPoint *, StarsU16, const StarsSelectableFleet *,
    StarsU16, const StarsSelectableThing *, StarsU16, StarsI16,
    StarsSelectObject, StarsSelectObject, StarsSelectObject, void *);
typedef struct StarsScorePlanet {
    StarsI16 owner;
    StarsI32 mineral_value;
    StarsI32 resources;
    /* True when the planet has a starbase whose hull has cargo capacity. */
    StarsI16 score_starbase;
} StarsScorePlanet;
typedef struct StarsScoreDesign {
    StarsI32 power;
    StarsI16 disabled;
} StarsScoreDesign;
typedef struct StarsScoreFleet {
    StarsI16 owner;
    StarsU16 flags;
    StarsU16 component_counts[16];
} StarsScoreFleet;
typedef struct StarsScoreSummary {
    StarsI32 score;
    StarsI32 planet_count;
    StarsI32 resources;
    StarsI32 starbase_count;
    /* Sum of the six race technology levels; zero for a dead player. */
    StarsI32 technology_levels;
    /* Uncapped active-fleet counts by power class; the score bonus is capped
     * separately by the number of owned planets. */
    StarsI32 fleet_counts[3];
} StarsScoreSummary;
StarsI32 stars_calc_player_score(StarsI16, const StarsScorePlanet *, StarsU16,
    const StarsI16 *, StarsI16, const StarsScoreDesign *, StarsU16,
    const StarsScoreFleet *, StarsU16, StarsI16, StarsScoreSummary *);
/* Pack the score summary into the 24-byte _scorex body. The caller supplies
 * the existing metadata words that UpdatePlayerScores maintains. */
void stars_pack_score_history_record(const StarsScoreSummary *, StarsU16,
    StarsU16, StarsU8 *);
StarsI16 stars_merge_all_fleet(StarsFleetTable *, StarsFleetState *,
    const StarsI16 *, StarsU16, StarsU16 *);
typedef struct StarsWaypointRecord {
    StarsI16 x;
    StarsI16 y;
    StarsU16 type_flags;
    StarsI16 target_id;
} StarsWaypointRecord;
typedef struct StarsWaypointFleet {
    StarsI16 id;
    StarsI16 player;
    StarsI16 x;
    StarsI16 y;
    StarsI32 weight;
    StarsU16 flags;
} StarsWaypointFleet;
typedef struct StarsWaypointThing {
    StarsI16 id;
    StarsI16 x;
    StarsI16 y;
    StarsU16 flags;
} StarsWaypointThing;
StarsI16 stars_validate_waypoints(StarsWaypointFleet *, StarsU16,
    StarsWaypointRecord *, StarsU16, const StarsWaypointThing *, StarsU16,
    StarsI16, StarsI16);
typedef struct StarsPopulationState {
    StarsI16 owner;
    StarsI16 desirability;
    StarsI32 population;
    StarsU8 fraction;
} StarsPopulationState;
StarsI32 stars_change_population(StarsPopulationState *, StarsI32,
    StarsI16, StarsI16, StarsI16);
const StarsU8 *stars_huldef_sb_from_id(const StarsU8 *, StarsI16);
const StarsU8 *stars_huldef_from_id(const StarsU8 *, const StarsU8 *, StarsI16);
const StarsU8 *stars_engine_from_id(const StarsU8 *, StarsI16);
const StarsU8 *stars_scanner_from_id(const StarsU8 *, StarsI16);
const StarsU8 *stars_shdef_t(const StarsU8 *);
const StarsU8 *stars_shdef_sbt(const StarsU8 *);
const StarsU8 *stars_planetary_from_id(const StarsU8 *, StarsI16);
typedef StarsI16 (*StarsLookupPart)(void *, const StarsU8 *);
StarsI16 stars_lookup_part_x(StarsU8 *, StarsU16, StarsU16,
    StarsLookupPart, void *);
StarsI16 stars_lookup_best_planetary_scanner(StarsU8 *, StarsLookupPart,
    void *);
typedef StarsU32 (*StarsPartResolve)(void *, StarsU16, StarsU16);
typedef StarsI16 (*StarsPartRaceStat)(void *, StarsI16, StarsI16);
typedef StarsI16 (*StarsPartRaceGrbit)(void *, StarsI16, StarsI16);
typedef StarsI16 (*StarsPartHidden)(void *, StarsU16, StarsU16);
typedef StarsI16 (*StarsPartTechStatus)(void *, StarsU32);
typedef StarsI16 (*StarsPartArmorDp)(void *, StarsU32);
typedef struct StarsLookupPartInput {
    StarsU16 group;
    StarsU16 item;
    StarsI16 player;
    StarsU32 component;
    StarsU16 table_segment;
    /* All sixteen original FLookupPart families store current CS in the
     * selected far pointer. `table_segment` is zero in that case. */
    StarsU16 table_segment_is_current_cs;
    /* Literal family base and selected record offsets from original CS. */
    StarsU16 table_base_offset;
    StarsU16 table_offset;
    StarsU16 table_stride;
    StarsPartResolve resolve;
    StarsPartRaceStat race_stat;
    StarsPartRaceGrbit race_grbit;
    StarsPartHidden should_hide;
    StarsPartTechStatus tech_status;
    StarsPartArmorDp armor_dp;
    void *context;
} StarsLookupPartInput;
StarsI16 stars_lookup_part(StarsLookupPartInput *);
/* Resolve a successful current-CS part address into a caller-supplied
 * original segment image. The returned record is borrowed from that image. */
const StarsU8 *stars_lookup_part_record(const StarsLookupPartInput *,
    const StarsU8 *, StarsU16);
/* Original group-0x20 `_torp` record field `dp` (record offset 54). */
StarsI16 stars_lookup_torpedo_dp(StarsU16);
/* Original group-0x20 `_torp` record fields `init`/`dRangeMax`. */
StarsI16 stars_lookup_torpedo_init(StarsU16);
StarsI16 stars_lookup_torpedo_range(StarsU16);
/* Original group-0x10 `_beam` record field `dp` (record offset 54). */
StarsI16 stars_lookup_beam_dp(StarsU16);
/* Original group-0x10 `_beam` record field `init` (record offset 56). */
StarsI16 stars_lookup_beam_init(StarsU16);
/* Original group-0x10 `_beam` record field `dRangeMax` (offset 52). */
StarsI16 stars_lookup_beam_range(StarsU16);
StarsI16 stars_tech_status(const StarsI16 *, const StarsI16 *, StarsU16);
StarsI16 stars_planets_in_circle(StarsI16, StarsI16, StarsU32,
    const StarsNearestPoint *, StarsU16);
StarsI16 stars_pct_wormhole_moves(StarsU16);
StarsI16 stars_free_thing(StarsU8 *, StarsU16 *, StarsU16);
StarsI16 stars_new_thing(StarsU8 *, StarsU16 *, StarsU16, StarsI16, StarsI16,
    StarsU16 *);
StarsI16 stars_give_trader_part(StarsU16 *, StarsU16, StarsU16 *);
typedef struct StarsWormholePoint {
    StarsI16 x;
    StarsI16 y;
    StarsU16 identity;
    StarsU16 flags;
} StarsWormholePoint;
StarsI16 stars_validate_wormhole_position(StarsI16, StarsI16, StarsU16,
    StarsI16, const StarsWormholePoint *, StarsU16,
    const StarsNearestPoint *, StarsU16, const StarsNearestPoint *, StarsU16);
typedef struct StarsThingGaugeValues {
    StarsI32 total;
    StarsI32 values[3];
} StarsThingGaugeValues;
typedef void (*StarsThingGaugeDraw)(void *, StarsI16, const StarsI32 *,
    StarsI32, StarsI16);
StarsI16 stars_draw_thing_gauge(const StarsThingGaugeValues *, StarsI16,
    StarsThingGaugeDraw, void *);
typedef struct StarsFleetLinkNode {
    StarsI16 x;
    StarsI16 y;
    StarsI16 dead;
    StarsI16 next_index;
    StarsI16 previous_index;
} StarsFleetLinkNode;
void stars_link_fleets(StarsFleetLinkNode *, StarsU16);
void stars_calc_pct_survive(StarsI16, StarsU16, StarsU16, StarsI16,
    float *, float *);
#endif
