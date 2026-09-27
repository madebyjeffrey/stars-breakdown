#include "core.h"
#include "common.h"
#include "TURINFX.H"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct TurinPopulationSummary {
    StarsPopulationState owner1;
    StarsPopulationState planet10;
    StarsI16 desirability;
    StarsI16 growth_rate;
    StarsI16 planet10_desirability;
    StarsI16 planet10_growth_rate;
    StarsI32 maximum_population;
    StarsI32 population_growth;
    StarsI32 planet10_maximum_population;
    StarsI32 planet10_population_growth;
    StarsU32 supply_population_after_transfer;
} TurinPopulationSummary;

typedef struct TurinTrace {
    Soe3CompleteInput complete_input;
    StarsMessageState turn_messages;
    StarsU8 turn_message_buffer[4096];
    StarsU16 turn_message_bytes;
    StarsU16 turn_message_requests;
    StarsU8 turn_written_messages[2][1024];
    StarsU16 turn_written_message_bytes[2];
    StarsI16 turn_message_write_player;
    StarsPlayerMessage turn_outgoing_messages[8];
    StarsU16 turn_outgoing_count;
    StarsU16 turn_mark_message_calls, turn_mark_loss_calls;
    StarsI16 complete_input_loaded;
    StarsVisPFPlayer turn_player1_visibility_players[2];
    StarsVisPFPlayer turn_player0_visibility_players[2];
    StarsU16 turn_player0_visibility_calls;
    StarsVisThingTarget turn_fleet_visibility_things[3][4];
    StarsU16 turn_fleet_visibility_calls[2];
    StarsU16 turn_planet_visibility_calls[2], turn_thing_visibility_calls[2];
    StarsReadPlanetState turn_client_visibility_planets[2][32];
    StarsU16 turn_visibility_fleet_flags[3][10];
    StarsU16 turn_design_write_counts[3];
    StarsU16 turn_plan_write_counts[3];
    StarsU16 turn_prepare_view_calls[2], turn_prepare_view_fleets[2];
    StarsU16 turn_generation_flag_entries, turn_generation_flag_exits;
    StarsU16 turn_generation_prior_flags;
    StarsVisPFPlayer turn_host_visibility_players[2];
    StarsU16 turn_host_visibility_calls;
    StarsU8 turn_host_player_records[2][266];
    StarsU16 turn_host_player_sizes[2], turn_host_player_writes;
    StarsU8 turn_m2_player_record[266];
    StarsU16 turn_m2_player_size, turn_m2_player_writes;
    StarsU8 turn_m1_player_record[266];
    StarsU16 turn_m1_player_size, turn_m1_player_writes;
    StarsU16 turn_player1_visibility_fleets;
    StarsPoint points[TURIN_RETAINED_POINT_COUNT];
    StarsU8 ai_planet_records[TURIN_RETAINED_POINT_COUNT * 16];
    StarsU8 plord[TURIN_RETAINED_FLEET_COUNT][STARS_AI_PLORD_HEADER_SIZE +
        2 * STARS_AI_PLORD_ORDER_SIZE];
    StarsU8 raw_fleets[TURIN_RETAINED_FLEET_COUNT][124];
    StarsFleetState fleet_storage[TURIN_RETAINED_FLEET_COUNT];
    StarsFleetState *fleet_items[TURIN_RETAINED_FLEET_COUNT];
    StarsU16 fleet_count;
    StarsDestroyGameState session;
    StarsU16 ai_destroy_calls, ai_destroy_heaps, ai_reset_messages;
    StarsU16 host_saved_selection_calls, host_saved_selection_kind;
    StarsI16 host_saved_selection_id, host_saved_selection_player;
    StarsU32 host_saved_selection_lid;
    StarsU16 host_saved_ini_flags;
    StarsInvalidateReportRecord fleet_report;
    StarsU16 fleet_report_flags;
    StarsU8 designs[16U * 147U];
    StarsU8 starbase_designs[10U * 147U];
    StarsI16 current_designs_loaded;
    StarsU8 startup_build_shdef[147];
    StarsHullComponent current_hull_components[16][16];
    StarsIdealWarpEngine current_ideal_engines[16];
    StarsU8 input_players[4][192];
    StarsU16 input_player_reads[4];
    StarsU8 scrap_planet[60];
    StarsU8 tech_battle[6];
    StarsU8 tech_trader[12];
    StarsTechLearnPlayer tech_player;
    StarsU16 tech_goto;
    StarsU16 game_flags;
    StarsU16 planet_resource_extra[32];
    StarsU16 ai_production_saved_flags;
    StarsI16 turn_production_commits;
    StarsI16 scrap_messages;
    StarsI16 scrap_message_recipient[2];
    StarsI16 scrap_message_code[2];
    StarsI16 scrap_message_payload[2][8];
    StarsI16 scrap_tech_lookup_calls;
    StarsI16 scrap_tech_mark_calls;
    StarsI16 scrap_tech_learn_calls;
    StarsI16 scrap_tech_random_calls;
    StarsU8 turn_owner0_task5_fleet[124];
    StarsU8 turn_owner0_task5_order[18];
    StarsU8 turn_owner0_task5_planet[60];
    StarsI16 turn_owner0_task5_messages;
    StarsI16 turn_owner0_task5_message_code[2];
    StarsI16 turn_owner0_task5_message_recipient[2];
    StarsI16 turn_owner0_task5_done_calls;
    StarsI16 turn_owner0_task5_result;
    StarsI16 scrap_remove_calls;
    StarsI16 scrap_order_done_calls;
    StarsI16 scrap_loaded;
    StarsI16 scrap_result;
    StarsI16 raw_fleet_present[TURIN_RETAINED_FLEET_COUNT];
    StarsU8 planet_index[TURIN_RETAINED_POINT_COUNT * 4];
    StarsU8 ai_data[0x2000];
    StarsU16 startup_owned_planets[32];
    StarsU16 startup_owned_planet_count;
    StarsU16 startup_prepare_random_calls;
    StarsU16 startup_mine_prepare_calls;
    StarsU16 startup_attack_fleets;
    StarsI16 first_pass_attack_head;
    StarsI16 first_pass_attack_next[TURIN_RETAINED_FLEET_COUNT];
    StarsU16 first_pass_visits;
    StarsU16 host_destroy_calls, host_destroy_heaps;
    StarsU16 startup_power_designs;
    StarsU8 startup_history_records[4][128];
    StarsU16 startup_history_types[4], startup_history_sizes[4];
    StarsU16 startup_history_record_count, startup_history_closed;
    StarsU8 startup_message_filters[49];
    StarsAiTurnPlayer players[2];
    StarsTurinTurnPlanet planet;
    StarsTurinTurnFleet fleets[TURIN_RETAINED_FLEET_COUNT];
    StarsTurinDroneAiTurnInput turin_input;
    StarsTurinDroneAiTurnOps turin_ops;
    StarsTurinScoutAdapter scout;
    StarsDoAiTurnInput ai_input;
    StarsI16 random_calls;
    StarsI16 rototill_seeded_mode;
    StarsI16 random_result;
    StarsI16 seeded_mode;
    StarsI16 seeded_action;
    StarsSeeds seeds;
    StarsSeeds main_rng_last;
    StarsU16 main_rng_count, main_rng_complete;
    StarsI16 random_limits[16];
    StarsI16 random_values[16];
    StarsI16 startup_name_random_calls;
    StarsI16 startup_name_limits[8];
    StarsI16 startup_name_values[8];
    StarsI16 startup_name_count;
    StarsI16 startup_name_designs[4];
    StarsI16 startup_name_bitmaps[4];
    StarsU16 startup_name_ids[4];
    StarsU16 startup_last_name_id;
    char startup_name_text[4][32];
    StarsU16 startup_name_hulls[4];
    StarsU8 startup_created_components[4][64];
    StarsU8 startup_design_changes[4][147];
    StarsU16 startup_design_change_count;
    StarsU8 startup_design_logs[4][160];
    StarsU16 startup_design_log_sizes[4];
    StarsU16 startup_design_log_count;
    StarsU8 startup_memory_records[16][160];
    StarsU16 startup_memory_types[16], startup_memory_sizes[16];
    StarsU16 startup_memory_count, startup_memory_applied;
    StarsU16 startup_log_off, startup_log_flags;
    StarsU8 startup_merge_inputs[2][124];
    StarsAiStarbaseDesign startup_starbase_designs[10];
    StarsU8 startup_capabilities[3];
    StarsIroAiState startup_research;
    StarsI16 startup_ensure_calls;
    StarsI16 startup_history_calls;
    StarsI16 startup_research_writes;
    StarsI16 turn_production_planet_visits;
    StarsI16 turn_research_calls;
    StarsU16 turn_order_research_calls;
    StarsI16 turn_research_pool_count;
    StarsI16 turn_research_pool_messages[12][5];
    StarsI16 startup_research_order;
    StarsI16 startup_latest_design;
    StarsU8 startup_written_current_tech;
    StarsI16 startup_written_percent;
    StarsU8 rng_file_data[0x5000];
    StarsU8 bof_write_records[5][18];
    StarsI16 bof_write_sizes[5];
    StarsI16 bof_writer_calls;
    StarsI16 bof_random_values[2];
    StarsU8 guess_runtime_planets[TURIN_RETAINED_POINT_COUNT][56];
    StarsU32 guess_runtime_populations[2];
    StarsU8 guess_race_data_storage[0x5b22];
    StarsGuessPlanet runtime_guess_planets[TURIN_RETAINED_POINT_COUNT];
    StarsU8 *guess_race_data;
    StarsI16 guess_random_calls;
    StarsI16 guess_random_limits[2];
    StarsI16 guess_random_values[2];
    StarsI16 guess_race_calls;
    StarsI16 guess_survival_calls;
    StarsI16 guess_current_owner;
    StarsI16 guess_survival_part_items[2];
    StarsI16 guess_survival_damage[2];
    StarsI16 guess_survival_operable[2];
    StarsI16 guess_survival_percent[2];
    StarsI16 saved_flag;
    StarsI16 output_bof_random_values[3];
    StarsU32 output_bof_tick_counts[3];
    StarsI16 output_bof_index;
    StarsI16 turn_data_file_create_calls;
    StarsU16 turn_data_file_append_calls, turn_append_mark_calls;
    StarsU8 runtime_log_present[2], append_input_header[18];
    StarsU8 saved_mutation_stream[2592];
    StarsU16 saved_mutation_size, saved_mutation_records;
    StarsU8 generated_log_file[4096];
    StarsU16 generated_log_size, generated_log_closed, generated_log_records, generated_log_loads;
    StarsU32 replay_serial;
    StarsU8 replay_environment[11];
    StarsU8 turn_serials[2][16];
    StarsU16 log_file_version, log_view_flags_high, log_dirty_calls, log_title_updates;
    StarsU16 turn_serial_validation_calls;
    StarsU16 turn_prepared_designs, turn_prepared_fleets;
    StarsU16 turn_battle_output_calls, turn_battle_output_blocks;
    StarsU16 turn_scanner_refreshes, turn_cloak_refreshes;
    StarsU16 startup_design_status_calls;
    StarsU16 startup_ship_maintenance_calls;
    StarsU16 ai_live_allocations, ai_allocation_calls, ai_release_calls;
    StarsU8 generated_history_file[4096];
    StarsU16 generated_history_size;
    StarsU8 active_files[3][4096], backup_files[3][4096], backup_log[4096];
    StarsU16 active_file_sizes[3], backup_file_sizes[3], backup_log_size;
    StarsU16 output_file_open, output_file_index, output_file_prefix_sizes[3];
    StarsU16 output_file_closes[3];
    StarsU16 backup_moves, backup_copies;
    StarsI16 turn_data_file_planet_calls;
    StarsI16 turn_score_record_counts[3];
    StarsScoreDesign turn_score_designs[2][16];
    StarsScoreSummary turn_score_summaries[2];
    StarsU8 turn_score_records[2][24];
    StarsUpdatePlayerScore turn_player_scores[2];
    StarsU16 turn_score_updates;
    StarsU16 turn_score_design_count;
    StarsI16 turn_data_file_close_calls;
    StarsI16 turn_data_turn_marker_calls;
    StarsU16 turn_data_turn_values[3];
    StarsU8 turn_data_turn_marker_bodies[3][2];
    StarsI16 turn_player_planet_write_index;
    StarsI16 turn_player_planet_write_counts[2];
    StarsI16 turn_player_fleet_write_counts[3];
    StarsI16 turn_player_order_write_counts[3];
    StarsI16 turn_production_queue_write_counts[3];
    StarsI16 turn_thing_record_counts[3];
    StarsI16 turn_data_file_visibility_calls;
    StarsI16 turn_player1_visibility_calls;
    StarsI16 turn_visible_planet_mark_calls;
    StarsReadPlanetState turn_visible_planet26;
    StarsI16 turn_visible_planet26_valid;
    StarsReadPlanetState turn_planet_states[TURIN_RETAINED_POINT_COUNT];
    StarsU8 turn_planet_records[TURIN_RETAINED_POINT_COUNT][128];
    StarsU16 turn_planet_record_types[TURIN_RETAINED_POINT_COUNT];
    StarsU16 turn_planet_record_lengths[TURIN_RETAINED_POINT_COUNT];
    StarsI16 turn_planet_write_index;
    StarsI16 turn_planet_write_calls;
    StarsI16 turn_planet_level_divergences;
    StarsI16 turn_planet_stock_divergences;
    StarsI16 startup_upgrade_calls;
    StarsU16 startup_defense_gate_calls;
    StarsU16 final_queue_budget_calls;
    StarsI32 final_queue_available[4], final_queue_cost[4];
    StarsU8 final_queue_packed[260];
    StarsU8 final_queue_planet[56];
    StarsProductionList final_queue_copy, final_queue_catalog;
    StarsU16 final_queue_live, final_queue_saved_flags;
    StarsU16 final_queue_phase, basic_queue_begin_calls, basic_queue_finish_calls;
    StarsI32 basic_queue_available[4], basic_queue_cost[4];
    StarsU16 basic_packet_calls, basic_scanner_calls, basic_terraform_calls;
    StarsU16 basic_keep_calls, basic_keep_fleets, basic_starbase_calls;
    StarsU16 basic_starbase_visits, basic_attack_gate_calls;
    StarsChangeMainSelection ai_selection;
    StarsU16 ai_selection_calls, ai_selection_loads;
    StarsU16 basic_speed_calls, basic_speed_fuel_calls;
    StarsProductionList first_queue_catalog;
    StarsU16 first_queue_init_calls;
    StarsU16 scout_marker_calls, scout_enemy_visits;
    StarsU16 basic_blocked_calls, basic_eta_builds, basic_eta_years;
    StarsI16 basic_eta_first, basic_eta_last;
    StarsI32 basic_blocked_unit_cost, basic_blocked_resources;
    StarsU16 final_queue_selection_calls, final_queue_init_calls;
    StarsU16 final_queue_finish_calls, final_queue_free_calls;
    StarsI16 startup_turn_shuffle_calls;
    StarsI16 startup_turn_order[2];
    StarsI16 turn_load_log_calls;
    StarsI16 turn_hst_planet_read_calls;
    StarsReadPlanetState turn_m1_planet_states[TURIN_M1_PLANET_RECORD_COUNT];
    StarsReadPlanetState turn_m2_planet_states[TURIN_M2_PLANET_INPUT_COUNT];
    StarsI16 turn_m1_planet_read_calls;
    StarsI16 turn_m2_planet_read_calls;
    StarsI16 turn_m2_shdef_read_calls;
    StarsI16 turn_m2_shdef_unresolved_calls;
    StarsI16 turn_m2_fleet_read_calls;
    StarsI16 turn_m1_fleet_read_calls;
    StarsU8 turn_owner0_input_orders[6][STARS_AI_PLORD_ORDER_SIZE];
    StarsI16 turn_validate_calls;
    StarsI16 turn_waypoint_source_calls;
    StarsI16 turn_waypoint_fleets[3];
    StarsI16 turn_waypoint_orders[3];
    StarsI16 turn_used_waypoint_calls;
    StarsI16 turn_used_waypoint_messages;
    StarsI16 turn_used_waypoint_message[10][3];
    StarsI16 turn_unmark_mine_calls;
    StarsI16 turn_pre_move_things_calls;
    StarsI16 turn_thing_decay_calls;
    StarsI16 turn_thing_decay_free_calls;
    StarsI16 turn_spank_cheaters_calls;
    StarsI16 turn_breed_colonists_calls;
    StarsI16 turn_auto_terraform_calls;
    StarsI16 turn_remote_terraform_calls;
    StarsU16 turn_remote_amount_calls;
    StarsU16 turn_remote_fleet_count;
    StarsI16 turn_sweep_for_mines_calls;
    StarsI16 turn_sweep_fleet_power_calls;
    StarsI16 turn_sweep_planet_power_calls;
    StarsI16 turn_sweep_can_attack_calls;
    StarsI16 turn_sweep_message_calls;
    StarsI16 turn_sweep_free_calls;
    StarsI16 turn_sweep_fleet_count;
    StarsI16 turn_sweep_planet_count;
    StarsI16 turn_sweep_thing_count;
    StarsI16 turn_orders_calls[2];
    StarsI16 turn_order_pass_calls;
    StarsI16 turn_order_passes[4];
    StarsI16 turn_battle_calls;
    StarsColonistDrop turn_drops[8];
    StarsTransferFullRecord turn_transfers[8];
    StarsU16 turn_drop_count, turn_transfer_count, turn_drop_calls, turn_transfer_calls;
    StarsU16 turn_order_fleet_visits[4], turn_passive_orders[4];
    StarsU16 turn_battle_links[10], turn_battle_link_flags[10];
    StarsU16 turn_battle_link_count, turn_battle_link_calls;
    StarsU16 turn_battle_eligibility_calls, turn_bombing_calls;
    StarsU16 turn_interaction_calls[2], turn_interaction_things[2];
    StarsU16 turn_battle_id;
    StarsI16 turn_move_fleets_calls;
    StarsI16 turn_produce_calls;
    StarsI16 turn_produce_message_calls;
    StarsI16 turn_produce_message_player[4];
    StarsI16 turn_produce_message_code[4];
    StarsI16 turn_produce_message_planet[4];
    StarsI16 turn_produce_message_object[4];
    StarsI16 turn_produce_message_value[4];
    StarsProducePlanet *turn_produce_planets;
    StarsI32 turn_produce_base_resources;
    StarsI16 turn_produce_resource_calls;
    StarsProduceItem turn_produce_queue[2];
    StarsI16 turn_produce_build_calls;
    StarsI16 turn_seed_queue_calls;
    StarsI16 turn_seed_finish_calls;
    StarsI16 turn_finish_lookup_calls;
    StarsProduceItem *turn_committed_queue;
    StarsU16 turn_committed_queue_count;
    StarsI16 turn_committed_planet;
    StarsI16 turn_queue_prepare_calls;
    StarsU8 turn_ai_queue_records[64 * 4];
    StarsAiProductionQueue turn_ai_queue;
    StarsI16 turn_produce_population_calls;
    StarsI16 turn_produce_random_event_calls;
    StarsI16 turn_update_guesses_calls;
    StarsI16 turn_mark_backup_calls;
    StarsU16 turn_prepare_output_calls;
    StarsU16 turn_task5_visit_ids[2], turn_task5_visit_count;
    StarsI16 turn_write_message_calls;
    StarsI16 turn_write_data_calls;
    StarsI16 pre_scrap_checks_done;
    StarsI16 turn_state_turn;
    StarsI16 turn_state_generated;
    StarsI16 turn_state_output_written;
    StarsOrdersPlanet turn_order_planets[TURIN_RETAINED_POINT_COUNT];
    StarsU8 turn_thing_bytes[TURIN_RETAINED_THING_COUNT * 18U];
    StarsI16 turn_fuel_calls;
    StarsU16 turn_fuel_fleet_count;
    StarsI16 turn_heal_calls;
    StarsU16 turn_heal_fleet_count;
    StarsMoveFleet turn_moving_fleets[TURIN_RETAINED_FLEET_COUNT];
    StarsMoveFleet turn_owner0_moving_fleets[6];
    TurinPopulationSummary population_summary;
    StarsI16 startup_player_tech_calls;
    StarsI16 tech_random_limits[32];
    StarsI16 tech_random_values[32];
    StarsU8 mining_planets[32U * 56U];
    StarsU16 mining_runtime_ids[2];
    StarsU16 mining_visited;
    StarsMineralMinedInput mining_inputs[2];
    StarsMineralMinedOps mining_ops;
    StarsI32 mining_stock[2][3];
    StarsI16 mining_planet_calls;
    StarsI16 mining_planet_ids[2];
    StarsI16 mining_random_calls;
    StarsI16 mining_random_limits[6];
    StarsI16 mining_random_values[6];
    StarsI16 random_event_turn;
    StarsI16 random_event_callback_calls;
    StarsI16 random_event_callback_order[4];
    StarsI16 random_event_random_calls;
    StarsI16 random_event_random_limits[3];
    StarsI16 random_event_random_values[3];
    StarsI16 random_event_trader_skipped;
    StarsMoveThing retained_move_things[TURIN_RETAINED_THING_COUNT];
    StarsI16 wormhole_move_random_calls;
    StarsI16 wormhole_move_random_limits[TURIN_RETAINED_WORMHOLE_DRAW_COUNT];
    StarsI16 wormhole_move_random_values[TURIN_RETAINED_WORMHOLE_DRAW_COUNT];
    StarsI16 wormhole_move_validate_calls;
    StarsI16 wormhole_move_waypoint_calls;
    StarsI16 wormhole_calls;
    StarsI16 load_calls;
    StarsI16 personality_calls;
    StarsI16 log_calls;
    StarsI16 history_calls;
    StarsI16 order_warp_calls;
    StarsI16 ideal_engine_lookups;
    StarsI16 ideal_engine_id;
    StarsI16 fleet_teeth_calls;
    StarsI16 hull_lookup_calls;
    StarsI16 move_order_calls;
    StarsI16 move_fuel_calls;
    StarsI16 merge_calls;
    StarsI16 production_merge_calls;
    StarsI16 production_merge_cargo_calls;
    StarsI16 production_merge_balance_lookup_calls;
    StarsI16 production_merge_lookup_calls;
    StarsI16 production_merge_delete_calls;
    StarsI16 production_merge_invalidate_calls;
    StarsI16 production_merge_deleted_id;
    StarsI16 production_merge_destination_id;
    StarsI16 transport_calls;
    StarsI16 transport_plan_calls;
    StarsI16 supply_calls;
    StarsU16 supply_cargo_free_calls;
    StarsU16 supply_fleet_stat_calls;
    StarsI16 merge_masks[4];
    StarsI16 supply_group[3];
    StarsI16 supply_id[3];
    StarsI16 supply_index[3];
    StarsI32 supply_delta[3];
    StarsCargoState supply_planet;
    StarsCargoState supply_fleet;
    StarsU8 supply_raw_fleet[124];
    StarsI32 supply_fleet_stat_value;
    StarsU32 supply_cargo_free_value;
    StarsI32 fuel_distance;
    StarsI16 failures;
} TurinTrace;

static void turn_audit_main_draw(TurinTrace *trace, StarsSeeds before, StarsI16 limit, StarsI16 value)
{
    StarsSeeds check_seeds = before;
    StarsI16 check_value;
    StarsU16 index = trace->main_rng_count;
    if (!trace->seeded_mode || trace->main_rng_complete) return;
    check_value = stars_random(&check_seeds, limit);
    if (before.first != trace->main_rng_last.first || before.second != trace->main_rng_last.second ||
            check_seeds.first != trace->seeds.first || check_seeds.second != trace->seeds.second ||
            check_value != value || index >= expected_main_rng_count[trace->seeded_action]) {
        ++trace->failures;
    } else if (limit != expected_main_rng[trace->seeded_action][index][0] ||
            value != expected_main_rng[trace->seeded_action][index][1]) {
        fprintf(stderr, "main RNG draw %u differs: limit=%d value=%d\n", index + 1U, limit, value);
        ++trace->failures;
    }
    trace->main_rng_last = trace->seeds;
    ++trace->main_rng_count;
}

static StarsI16 turn_main_random(TurinTrace *trace, StarsI16 limit)
{
    StarsSeeds before = trace->seeds;
    StarsI16 value = stars_random(&trace->seeds, limit);
    turn_audit_main_draw(trace, before, limit, value);
    return value;
}


static void put_u16(StarsU8 *raw, StarsU16 offset, StarsU16 value)
{
    raw[offset] = (StarsU8)value;
    raw[offset + 1U] = (StarsU8)(value >> 8);
}

static void put_u32(StarsU8 *raw, StarsU16 offset, StarsU32 value)
{
    put_u16(raw, offset, (StarsU16)value);
    put_u16(raw, (StarsU16)(offset + 2U), (StarsU16)(value >> 16));
}

static void pack_loaded_planet(StarsU8 runtime_planet[56],
    const StarsReadPlanetState *loaded)
{
    StarsU16 axis;
    memset(runtime_planet, 0, 56U);
    put_u16(runtime_planet, 0U, (StarsU16)loaded->id);
    put_u16(runtime_planet, 2U, (StarsU16)loaded->player);
    put_u16(runtime_planet, 4U, loaded->flags);
    for (axis = 0U; axis < 3U; ++axis) {
        runtime_planet[6U + axis] = loaded->mineral_levels[axis];
        runtime_planet[9U + axis] = loaded->mineral_concentrations[axis];
        runtime_planet[12U + axis] = loaded->environment[axis];
        runtime_planet[15U + axis] = loaded->environment_original[axis];
    }
    put_u16(runtime_planet, 18U, loaded->routing_word);
    put_u32(runtime_planet, 20U, loaded->opaque_20[0]);
    put_u32(runtime_planet, 24U, loaded->opaque_20[1]);
    for (axis = 0U; axis < 4U; ++axis)
        put_u32(runtime_planet, (StarsU16)(28U + axis * 4U), loaded->minerals[axis]);
    put_u32(runtime_planet, 44U, loaded->opaque_44);
    put_u16(runtime_planet, 48U, loaded->opaque_48);
    put_u16(runtime_planet, 50U, loaded->turn);
}

static StarsU16 get_u16(const StarsU8 *raw, StarsU16 offset)
{
    return (StarsU16)(raw[offset] | ((StarsU16)raw[offset + 1U] << 8));
}

static StarsU32 get_u32(const StarsU8 *raw, StarsU16 offset)
{
    return (StarsU32)get_u16(raw, offset) |
        ((StarsU32)get_u16(raw, (StarsU16)(offset + 2U)) << 16);
}

static const StarsU8 *scrap_part_record(StarsU16 group, StarsU16 item)
{
    StarsLookupPartInput lookup;
    memset(&lookup, 0, sizeof lookup);
    lookup.group = group;
    lookup.item = item;
    lookup.player = -1;
    if (stars_lookup_part(&lookup) < 0)
        return (const StarsU8 *)0;
    return stars_lookup_part_record(&lookup, retained_part_segment,
        (StarsU16)sizeof retained_part_segment);
}

static StarsU16 scrap_read_hull(void *opaque, StarsU16 id, StarsU8 *out,
    StarsU16 capacity)
{
    const StarsU8 *record;
    (void)opaque;
    if (capacity < 143U)
        return 0U;
    record = scrap_part_record(id < 32U ? 0x4000U : 0x0400U,
        id < 32U ? id : (StarsU16)(id - 32U));
    if (!record)
        return 0U;
    memcpy(out, record, 143U);
    return 1U;
}

static StarsI16 scrap_part_technology(void *opaque, StarsU16 group,
    StarsU16 subtype, StarsU8 technology[6])
{
    TurinTrace *trace = (TurinTrace *)opaque;
    const StarsU8 *record = scrap_part_record(group, subtype);
    if (!record)
        return 0;
    memcpy(technology, record + 2U, 6U);
    ++trace->scrap_tech_lookup_calls;
    return 1;
}

static StarsU16 turn_message_parameter_count(void *opaque, StarsI16 message)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    if (message < 0 || message >= 387) { ++trace->failures; return 0U; }
    return original_message_counts[message];
}

static StarsU16 turn_message_player_flags(void *opaque, StarsI16 player)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    if (player < 0 || player >= 2) { ++trace->failures; return 0x0200U; }
    return get_u16(trace->complete_input.views[0].players[player], 6U);
}

static void turn_append_message(void *opaque, const StarsU8 *bytes, StarsU16 count)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    if ((StarsU32)trace->turn_message_bytes + count > sizeof trace->turn_message_buffer) {
        ++trace->failures; return;
    }
    memcpy(trace->turn_message_buffer + trace->turn_message_bytes, bytes, count);
    trace->turn_message_bytes = (StarsU16)(trace->turn_message_bytes + count);
}

static void turn_send_message(TurinTrace *trace, StarsI16 player, StarsI16 message,
    StarsI16 object, const StarsI16 *parameters)
{
    StarsMessageOps ops;
    if (!trace->seeded_mode) return;
    memset(&ops, 0, sizeof ops);
    ops.parameter_count = turn_message_parameter_count;
    ops.player_flags = turn_message_player_flags;
    ops.append_message = turn_append_message;
    if (!stars_send_player_message(&trace->turn_messages, player, message,
            object, parameters, &ops, trace)) ++trace->failures;
    if (trace->turn_messages.message_memory != trace->turn_message_bytes)
        ++trace->failures;
    ++trace->turn_message_requests;
}

static void scrap_raw_message(void *opaque, StarsI16 recipient,
    StarsI16 code, const StarsI16 payload[8])
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsI16 index;
    if (trace->scrap_messages >= 2) {
        ++trace->failures;
        return;
    }
    turn_send_message(trace, recipient, code, payload[0], payload + 1);
    index = trace->scrap_messages;
    trace->scrap_message_recipient[index] = recipient;
    trace->scrap_message_code[index] = code;
    for (index = 0; index < 8; ++index)
        trace->scrap_message_payload[trace->scrap_messages][index] =
            payload[index];
    ++trace->scrap_messages;
}

static StarsI16 scrap_tech_random(void *opaque, StarsI16 limit)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsI16 index = trace->scrap_tech_random_calls;
    StarsI16 value = 0;
    ++trace->scrap_tech_random_calls;
    if (trace->seeded_mode)
        value = turn_main_random(trace, limit);
    if (index < 32) {
        trace->tech_random_limits[index] = limit;
        trace->tech_random_values[index] = value;
    }
    if (limit <= 0)
        ++trace->failures;
    /* Nonseeded runs retain the one-call no-learn witness. */
    return value;
}

static StarsI16 scrap_learn_technology(void *opaque, StarsI16 player,
    StarsU16 *message_object)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsTechLearnInput input;
    StarsI16 index;
    StarsI16 result;
    ++trace->scrap_tech_learn_calls;
    if (player != 1 || *message_object != 24U)
        ++trace->failures;
    memset(&input, 0, sizeof input);
    for (index = 0; index < 6; ++index)
        trace->tech_player.tech_level[index] =
            trace->complete_input.views[0].players[player][26U + index];
    input.player = &trace->tech_player;
    input.player_id = player;
    input.message_id = -1;
    input.goto_id = &trace->tech_goto;
    input.random = scrap_tech_random;
    input.context = trace;
    result = stars_itech_learn_a_tech(&input);
    for (index = 0; index < 6; ++index)
        trace->complete_input.views[0].players[player][26U + index] =
            (StarsU8)trace->tech_player.tech_level[index];
    if (result != 0 || *message_object != 24U || trace->tech_goto != 0U)
        ++trace->failures;
    return result;
}

static void scrap_remove_message(void *opaque, StarsI16 player,
    StarsI16 code, StarsU16 object)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    if (player != 1 || code != 0x4e || object != 0x8002U)
        ++trace->failures;
    if (trace->seeded_mode)
        stars_remove_player_message(trace->turn_message_buffer, trace->turn_message_bytes,
            player, code, (StarsI16)object);
    ++trace->scrap_remove_calls;
}

static void scrap_mark_techs(void *opaque, const StarsU8 *design,
    StarsI16 player)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsMarkTechsInput input;
    if (design != trace->designs + 2U * 147U || player != 1)
        ++trace->failures;
    memset(&input, 0, sizeof input);
    memcpy(input.tech_battle, trace->tech_battle,
        sizeof input.tech_battle);
    memcpy(input.tech_trader, trace->tech_trader,
        sizeof input.tech_trader);
    input.lookup = scrap_part_technology;
    input.context = trace;
    if (!stars_mark_techs_seen_shdef(design, 147U, &input))
        ++trace->failures;
    memcpy(trace->tech_battle, input.tech_battle,
        sizeof trace->tech_battle);
    memcpy(trace->tech_trader, input.tech_trader,
        sizeof trace->tech_trader);
    ++trace->scrap_tech_mark_calls;
}

static void scrap_order_done(void *opaque, StarsSatisfyFleet *fleet,
    StarsSatisfyOrder *order, StarsI16 index)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    if (!fleet || fleet->id != 2 || fleet->owner != 1 ||
            !order || order->kind != STARS_SATISFY_ORDER_SCRAP ||
            order->active || index != 0)
        ++trace->failures;
    ++trace->scrap_order_done_calls;
}

/* Materialize the current decoded input record, without retained output data. */
static void pack_input_fleet(StarsU8 raw[124], const Soe3InputFleet *fleet)
{
    StarsU16 i;
    const StarsReadFleetState *state = &fleet->state;
    memset(raw, 0, 124U);
    put_u16(raw, 0U, (StarsU16)(fleet->id_word & 511U));
    put_u16(raw, 2U, (StarsU16)fleet->owner);
    put_u16(raw, 4U, state->fleet_flags);
    put_u16(raw, 6U, (StarsU16)state->planet);
    put_u16(raw, 8U, (StarsU16)state->x);
    put_u16(raw, 10U, (StarsU16)state->y);
    for (i = 0U; i < 32U; ++i)
        put_u16(raw, (StarsU16)(12U + i * 2U), state->ship_counts[i]);
    for (i = 0U; i < 5U; ++i)
        put_u32(raw, (StarsU16)(76U + i * 4U), state->minerals[i]);
    raw[98] = (StarsU8)state->order_count;
}

static StarsI16 retained_m2_shdef_cost_matches(TurinTrace *trace)
{
    return (StarsI16)(get_u16(trace->designs + 2U * 147U, 0x2aU) == 123U &&
        get_u16(trace->designs + 2U * 147U, 0x2cU) == 41U &&
        get_u16(trace->designs + 2U * 147U, 0x2eU) == 0U &&
        get_u16(trace->designs + 2U * 147U, 0x30U) == 12U);
}

static StarsI16 read_retained_m2_shdef_bank(TurinTrace *trace,
    StarsI16 count_turn_inputs)
{
    const Soe3InputView *source;
    StarsU16 slot;
    StarsU16 count = 0U;
    if (!trace || !trace->complete_input_loaded) return 0;
    source = &trace->complete_input.views[2];
    if (!source->player_present[1]) return 0;
    if (!trace->current_designs_loaded) {
        memcpy(trace->designs, source->ship_designs[1], sizeof trace->designs);
        memcpy(trace->starbase_designs, source->starbase_designs[1],
            sizeof trace->starbase_designs);
        trace->current_designs_loaded = 1;
    }
    for (slot = 0U; slot < 32U; ++slot)
        if (source->design_present[1][slot]) ++count;
    if (count != TURIN_M2_SHDEF_INPUT_COUNT) return 0;
    if (count_turn_inputs) trace->turn_m2_shdef_read_calls =
        (StarsI16)(trace->turn_m2_shdef_read_calls + count);
    return retained_m2_shdef_cost_matches(trace);
}

static StarsI16 turn_read_m2_shdef_input(TurinTrace *trace)
{
    return read_retained_m2_shdef_bank(trace, 1);
}

static StarsI16 scrap_load_inputs(TurinTrace *trace)
{
    StarsReadPlanetState planet_state;
    if (trace->turn_m2_shdef_read_calls == 0) {
        if (!read_retained_m2_shdef_bank(trace, 0)) return 0;
    } else if (!retained_m2_shdef_cost_matches(trace)) {
        return 0;
    }
    if (trace->turn_order_pass_calls > 0) {
        planet_state = trace->turn_planet_states[24];
    } else if (trace->turn_m2_planet_read_calls > 0) {
        planet_state = trace->turn_m2_planet_states[0];
    } else {
        if (!trace->complete_input_loaded ||
                trace->complete_input.views[2].planet_count != 1U) return 0;
        planet_state = trace->complete_input.views[2].planets[0];
    }
    if (planet_state.id != 24 || planet_state.player != 1) return 0;
    if (trace->turn_order_pass_calls == 0 && (
            planet_state.flags != 0x0f07U ||
            planet_state.minerals[0] != 532UL ||
            planet_state.minerals[1] != 956UL ||
            planet_state.minerals[2] != 500UL ||
            planet_state.minerals[3] != 250UL))
        return 0;
    put_u16(trace->scrap_planet, 0U, (StarsU16)planet_state.id);
    put_u16(trace->scrap_planet, 2U, (StarsU16)planet_state.player);
    put_u16(trace->scrap_planet, 4U, planet_state.flags);
    put_u32(trace->scrap_planet, 0x1cU, planet_state.minerals[0]);
    put_u32(trace->scrap_planet, 0x20U, planet_state.minerals[1]);
    put_u32(trace->scrap_planet, 0x24U, planet_state.minerals[2]);
    put_u32(trace->scrap_planet, 0x28U, planet_state.minerals[3]);
    return 1;
}

static StarsI16 run_task5_scrap(TurinTrace *trace)
{
    StarsSatisfyOrdersOps ops;
    StarsSatisfyPackedInput input;
    StarsSatisfyScrapPackedState scrap;
    StarsU32 race_bits = get_u32(trace->complete_input.views[0].players[1], 78U);
    memset(&ops, 0, sizeof ops);
    memset(&input, 0, sizeof input);
    memset(&scrap, 0, sizeof scrap);
    ops.context = trace;
    ops.order_done = scrap_order_done;
    ops.raw_message = scrap_raw_message;
    ops.scrap_learn = scrap_learn_technology;
    ops.scrap_remove_message = scrap_remove_message;
    ops.scrap_mark_techs = scrap_mark_techs;
    scrap.planet_record = trace->scrap_planet;
    scrap.planet_size = sizeof trace->scrap_planet;
    scrap.design_records[2] = trace->designs + 2U * 147U;
    scrap.design_size = 147U;
    scrap.fleet_race_reprice = (StarsI16)((race_bits >> 12) & 1UL);
    scrap.planet_race_scrap_bonus = (StarsI16)(
        (race_bits >> 5) & 1UL);
    if (scrap.planet_race_scrap_bonus)
        scrap.planet_resource_extra = &trace->planet_resource_extra[24];
    scrap.game_flags = &trace->game_flags;
    input.fleet_record = trace->raw_fleets[2];
    input.fleet_size = sizeof trace->raw_fleets[2];
    input.order_record = trace->plord[2] + STARS_AI_PLORD_HEADER_SIZE;
    input.order_size = STARS_AI_PLORD_ORDER_SIZE;
    input.pass = 1;
    input.planet_id = 24;
    input.scrap_state = &scrap;
    input.ops = &ops;
    return stars_satisfy_orders_packed(&input);
}

static void turn_owner0_task5_message(void *opaque, StarsI16 recipient,
    StarsI16 code, const StarsI16 payload[8])
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsI16 index = trace->turn_owner0_task5_messages;
    turn_send_message(trace, recipient, code, payload[0], payload + 1);
    if (index < 0 || index >= 2) {
        ++trace->failures;
        return;
    }
    trace->turn_owner0_task5_message_code[index] = code;
    trace->turn_owner0_task5_message_recipient[index] = recipient;
    ++trace->turn_owner0_task5_messages;
}

static void turn_owner0_task5_done(void *opaque, StarsSatisfyFleet *fleet,
    StarsSatisfyOrder *order, StarsI16 index)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    if (!fleet || fleet->id != 0 || fleet->owner != 0 || !order ||
            order->kind != STARS_SATISFY_ORDER_SCRAP || order->active ||
            index != 0)
        ++trace->failures;
    ++trace->turn_owner0_task5_done_calls;
}

static StarsI16 turn_owner0_task5_learn(void *opaque, StarsI16 player,
    StarsU16 *message_object)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsTechLearnInput input;
    StarsI16 index;
    StarsI16 result;
    if (player != 0 || !message_object || *message_object != 10U)
        ++trace->failures;
    memset(&input, 0, sizeof input);
    for (index = 0; index < 6; ++index)
        trace->tech_player.tech_level[index] =
            trace->complete_input.views[0].players[0][26U + index];
    input.player = &trace->tech_player;
    input.player_id = 0;
    input.message_id = -1;
    input.goto_id = &trace->tech_goto;
    input.random = scrap_tech_random;
    input.context = trace;
    ++trace->startup_player_tech_calls;
    result = stars_itech_learn_a_tech(&input);
    for (index = 0; index < 6; ++index)
        trace->complete_input.views[0].players[player][26U + index] =
            (StarsU8)trace->tech_player.tech_level[index];
    if (result != 0 || trace->tech_goto != 0U || *message_object != 10U)
        ++trace->failures;
    return result;
}

static StarsI16 run_owner0_task5_scrap(TurinTrace *trace)
{
    StarsSatisfyOrdersOps ops;
    StarsSatisfyPackedInput input;
    StarsSatisfyScrapPackedState scrap;
    StarsU16 mineral;
    StarsU16 slot;
    Soe3InputFleet *loaded = 0;

    memset(&ops, 0, sizeof ops);
    memset(&input, 0, sizeof input);
    memset(&scrap, 0, sizeof scrap);
    for (slot = 0U; slot < trace->complete_input.views[0].fleet_count; ++slot)
        if (trace->complete_input.views[0].fleets[slot].owner == 0 &&
                (trace->complete_input.views[0].fleets[slot].id_word & 511U) == 0U)
            loaded = &trace->complete_input.views[0].fleets[slot];
    if (!loaded || loaded->state.order_count != 1U) return 0;
    /* Dispatch from the loaded order, independently of the oracle case label. */
    if ((loaded->state.orders[6] & 15U) == 0U) return 1;
    if ((loaded->state.orders[6] & 15U) != 5U) return 0;
    memset(trace->turn_owner0_task5_fleet, 0, sizeof trace->turn_owner0_task5_fleet);
    put_u16(trace->turn_owner0_task5_fleet, 0U, loaded->id_word);
    put_u16(trace->turn_owner0_task5_fleet, 2U, (StarsU16)loaded->owner);
    put_u16(trace->turn_owner0_task5_fleet, 4U, loaded->state.fleet_flags);
    put_u16(trace->turn_owner0_task5_fleet, 6U, (StarsU16)loaded->state.planet);
    put_u16(trace->turn_owner0_task5_fleet, 8U, (StarsU16)loaded->state.x);
    put_u16(trace->turn_owner0_task5_fleet, 10U, (StarsU16)loaded->state.y);
    for (slot = 0U; slot < 32U; ++slot)
        put_u16(trace->turn_owner0_task5_fleet, (StarsU16)(12U + slot * 2U),
            loaded->state.ship_counts[slot]);
    for (slot = 0U; slot < 5U; ++slot)
        put_u32(trace->turn_owner0_task5_fleet, (StarsU16)(76U + slot * 4U),
            loaded->state.minerals[slot]);
    trace->turn_owner0_task5_fleet[96] = (StarsU8)loaded->state.design_plan;
    put_u16(trace->turn_owner0_task5_fleet, 98U, loaded->state.order_count);
    memcpy(trace->turn_owner0_task5_order,
        loaded->state.orders, sizeof trace->turn_owner0_task5_order);
    memset(trace->turn_owner0_task5_planet, 0,
        sizeof trace->turn_owner0_task5_planet);
    put_u16(trace->turn_owner0_task5_planet, 0U,
        (StarsU16)trace->turn_planet_states[10].id);
    put_u16(trace->turn_owner0_task5_planet, 2U,
        (StarsU16)trace->turn_planet_states[10].player);
    put_u16(trace->turn_owner0_task5_planet, 4U,
        trace->turn_planet_states[10].flags);
    for (mineral = 0U; mineral < 4U; ++mineral)
        put_u32(trace->turn_owner0_task5_planet,
            (StarsU16)(0x1cU + mineral * 4U),
            trace->turn_planet_states[10].minerals[mineral]);
    ops.context = trace;
    ops.raw_message = turn_owner0_task5_message;
    ops.order_done = turn_owner0_task5_done;
    ops.scrap_learn = turn_owner0_task5_learn;
    scrap.planet_record = trace->turn_owner0_task5_planet;
    scrap.planet_size = sizeof trace->turn_owner0_task5_planet;
    scrap.design_records[0] = trace->complete_input.views[0].ship_designs[0];
    scrap.fleet_race_reprice = (StarsI16)((get_u32(
        trace->complete_input.views[0].players[0], 78U) >> 12) & 1UL);
    scrap.planet_race_scrap_bonus = (StarsI16)((get_u32(
        trace->complete_input.views[0].players[0], 78U) >> 5) & 1UL);
    scrap.planet_resource_extra = &trace->planet_resource_extra[10];
    scrap.game_flags = &trace->game_flags;
    scrap.design_size = 147U;
    input.fleet_record = trace->turn_owner0_task5_fleet;
    input.fleet_size = sizeof trace->turn_owner0_task5_fleet;
    input.order_record = trace->turn_owner0_task5_order;
    input.order_size = sizeof trace->turn_owner0_task5_order;
    input.pass = 1;
    input.planet_id = 10;
    input.scrap_state = &scrap;
    input.ops = &ops;
    trace->turn_owner0_task5_result = stars_satisfy_orders_packed(&input);
    if (trace->turn_owner0_task5_result != 1 ||
            trace->turn_owner0_task5_messages != 2 ||
            trace->turn_owner0_task5_message_recipient[0] != 0 ||
            trace->turn_owner0_task5_message_code[0] != 0x5a ||
            trace->turn_owner0_task5_message_recipient[1] != 0 ||
            trace->turn_owner0_task5_message_code[1] != 0x141 ||
            trace->turn_owner0_task5_done_calls != 1 ||
            get_u32(trace->turn_owner0_task5_planet, 0x1cU) != 478UL ||
            get_u32(trace->turn_owner0_task5_planet, 0x20U) != 899UL ||
            get_u32(trace->turn_owner0_task5_planet, 0x24U) != 381UL ||
            (get_u16(trace->turn_owner0_task5_fleet, 4U) & 0x0400U) == 0U ||
            (get_u16(trace->turn_owner0_task5_order, 6U) & 0x1000U) != 0U)
        ++trace->failures;
    loaded->state.fleet_flags = get_u16(trace->turn_owner0_task5_fleet, 4U);
    memcpy(loaded->state.orders, trace->turn_owner0_task5_order,
        sizeof trace->turn_owner0_task5_order);
    for (mineral = 0U; mineral < 4U; ++mineral)
        trace->turn_planet_states[10].minerals[mineral] =
            get_u32(trace->turn_owner0_task5_planet,
                (StarsU16)(0x1cU + mineral * 4U));
    return trace->turn_owner0_task5_result;
}

static StarsI16 balance_lookup_merge_fleet(void *, StarsI16,
    StarsFleetBalanceFleet *);
static StarsI16 balance_merge_max_weight(void *, StarsI16, StarsI16,
    StarsI16);

static void merge_cargo(void *opaque, StarsU8 *destination,
    StarsU8 *deleted)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsFleetBalanceFleet current[2];
    StarsFleetTransferCargoBalanceInput input;
    StarsU16 side;
    StarsU16 slot;
    memset(current, 0, sizeof current);
    for (side = 0; side < 2U; ++side) {
        const StarsU8 *raw = side == 0U ? destination : deleted;
        current[side].player = (StarsI16)get_u16(raw, 2U);
        current[side].flags = get_u16(raw, 4U);
        for (slot = 0; slot < 16U; ++slot) {
            current[side].ship_count[slot] =
                (StarsI16)get_u16(raw, (StarsU16)(12U + slot * 2U));
            current[side].ship_status[slot] =
                get_u16(raw, (StarsU16)(44U + slot * 2U));
        }
        for (slot = 0; slot < 5U; ++slot)
            current[side].cargo[slot] = (StarsI32)get_u32(raw,
                (StarsU16)(76U + slot * 4U));
    }
    memset(&input, 0, sizeof input);
    input.new_fleets[0] = &current[0];
    input.new_fleets[1] = &current[1];
    input.fleet_ids[0] = (StarsI16)get_u16(destination, 0U);
    input.fleet_ids[1] = (StarsI16)get_u16(deleted, 0U);
    input.player = 1;
    input.lookup_fleet = balance_lookup_merge_fleet;
    input.max_weight = balance_merge_max_weight;
    input.context = trace;
    stars_fleet_transfer_cargo_balance(&input);
    for (side = 0; side < 2U; ++side) {
        StarsU8 *raw = side == 0U ? destination : deleted;
        for (slot = 0; slot < 16U; ++slot)
            put_u16(raw, (StarsU16)(44U + slot * 2U),
                current[side].ship_status[slot]);
        for (slot = 0; slot < 5U; ++slot)
            put_u32(raw, (StarsU16)(76U + slot * 4U),
                (StarsU32)current[side].cargo[slot]);
    }
    ++trace->production_merge_cargo_calls;
}

static StarsI16 balance_lookup_merge_fleet(void *opaque, StarsI16 fleet_id,
    StarsFleetBalanceFleet *out)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    const StarsU8 *raw;
    StarsU16 slot;
    if (!out || fleet_id < 2 || fleet_id > 3 ||
            !trace->raw_fleet_present[(StarsU16)fleet_id])
        return 0;
    raw = trace->raw_fleets[(StarsU16)fleet_id];
    memset(out, 0, sizeof *out);
    out->player = (StarsI16)get_u16(raw, 2U);
    out->flags = get_u16(raw, 4U);
    for (slot = 0; slot < 16U; ++slot) {
        out->ship_count[slot] =
            (StarsI16)get_u16(raw, (StarsU16)(12U + slot * 2U));
        out->ship_status[slot] =
            get_u16(raw, (StarsU16)(44U + slot * 2U));
    }
    for (slot = 0; slot < 5U; ++slot)
        out->cargo[slot] = (StarsI32)get_u32(raw,
            (StarsU16)(76U + slot * 4U));
    ++trace->production_merge_balance_lookup_calls;
    return out->player == 1;
}

static StarsI16 balance_merge_max_weight(void *opaque, StarsI16 player,
    StarsI16 design_id, StarsI16 statistic)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsShipComponentStat components[16];
    StarsShipDesignStat design;
    StarsHullStat hull;
    StarsU8 hull_record[143];
    StarsU16 count;
    StarsU16 index;
    const StarsU8 *raw;
    if (player != 1 || design_id < 0 || design_id >= 16 ||
            statistic < 1 || statistic > 2) {
        ++trace->failures;
        return 0;
    }
    raw = trace->designs + (StarsU16)design_id * 147U;
    count = raw[122];
    if (count > 16U || !scrap_read_hull(opaque, get_u16(raw, 0U), hull_record,
            (StarsU16)sizeof hull_record)) {
        ++trace->failures; return 0;
    }
    for (index = 0U; index < count; ++index) {
        StarsU16 offset = (StarsU16)(58U + index * 4U);
        components[index].group = get_u16(raw, offset);
        components[index].packed_count_type = get_u16(raw, (StarsU16)(offset + 2U));
    }
    design.component_count = count;
    design.components = components;
    hull.fuel_max = get_u16(hull_record, 54U);
    hull.cargo_max = get_u16(hull_record, 52U);
    return stars_wt_max_ship_design_stat(&design, &hull, statistic);
}

static void lookup_merge_fleet(void *opaque, StarsI16 player,
    StarsU8 *fleet)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsFleetState working;
    StarsU16 id;
    ++trace->production_merge_lookup_calls;
    if (player != -1 || !fleet) {
        ++trace->failures;
        return;
    }
    id = get_u16(fleet, 0U);
    memset(&working, 0, sizeof working);
    memcpy(working.bytes, fleet, sizeof working.bytes);
    if (id >= TURIN_RETAINED_FLEET_COUNT ||
            !stars_lookup_fleet(trace->fleet_items, trace->fleet_count,
                -1, &working, 0) ||
            memcmp(trace->fleet_items[id]->bytes, fleet,
                sizeof working.bytes) != 0)
        ++trace->failures;
}

static void delete_merge_fleet(void *opaque, StarsI16 deleted_id,
    StarsI16 reason, StarsI16 destination_id)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    ++trace->production_merge_delete_calls;
    trace->production_merge_deleted_id = deleted_id;
    trace->production_merge_destination_id = destination_id;
    if (reason != 2 || deleted_id < 0 ||
            deleted_id >= TURIN_RETAINED_FLEET_COUNT ||
            destination_id != 2 ||
            !stars_delete_fleet(trace->fleet_items, &trace->fleet_count,
                deleted_id, reason, destination_id, 0, 0)) {
        ++trace->failures;
        return;
    }
    trace->raw_fleet_present[deleted_id] = 0;
}

static void invalidate_merge_report(void *opaque, StarsI16 player,
    StarsI16 report_kind)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsInvalidateReportInput input;
    StarsInvalidateReportRecord *current = &trace->fleet_report;
    memset(&input, 0, sizeof input);
    input.report = player;
    input.reload = report_kind;
    input.current = &current;
    input.fleet_report = &trace->fleet_report;
    input.ai_mode = 0;
    stars_invalidate_report(&input);
    trace->fleet_report_flags = input.game_view_flags;
    ++trace->production_merge_invalidate_calls;
    if (player != 1 || report_kind != 2 ||
            current != &trace->fleet_report ||
            trace->fleet_report.cached != 0 ||
            (input.game_view_flags & 0x0200U) == 0U)
        ++trace->failures;
}

static StarsI16 read_input_player(TurinTrace *trace, StarsU16 view)
{
    const Soe3InputView *source;
    StarsU16 owner;
    if (view >= 4U) return 0;
    if (!trace->complete_input_loaded) {
        trace->complete_input = soe3_inputs[trace->seeded_action ? 1 : 0];
        trace->complete_input_loaded = 1;
    }
    source = &trace->complete_input.views[view < 2U ? 0U : view - 1U];
    owner = view == 1U || view == 3U ? 1U : 0U;
    if (!source->player_present[owner]) return 0;
    memcpy(trace->input_players[view], source->players[owner], 192U);
    ++trace->input_player_reads[view];
    return 1;
}

static void startup_reset_heap(void *opaque, StarsU16 heap)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    static const StarsU16 order[] = { 3, 5, 10, 11, 6, 1, 7, 0, 8 };
    if (trace->ai_destroy_heaps >= 9U || heap != order[trace->ai_destroy_heaps]) {
        ++trace->failures; return;
    }
    ++trace->ai_destroy_heaps;
    if (heap == 3U) {
        memset(&trace->planet, 0, sizeof trace->planet);
        memset(&trace->supply_planet, 0, sizeof trace->supply_planet);
    } else if (heap == 5U) {
        memset(trace->fleets, 0, sizeof trace->fleets);
        memset(trace->raw_fleets, 0, sizeof trace->raw_fleets);
        memset(trace->raw_fleet_present, 0, sizeof trace->raw_fleet_present);
        memset(trace->fleet_storage, 0, sizeof trace->fleet_storage);
        memset(trace->fleet_items, 0, sizeof trace->fleet_items);
        memset(&trace->supply_fleet, 0, sizeof trace->supply_fleet);
        trace->fleet_count = 0;
    } else if (heap == 7U) {
        memset(trace->designs, 0, sizeof trace->designs);
        memset(trace->starbase_designs, 0, sizeof trace->starbase_designs);
        trace->current_designs_loaded = 0;
    } else if (heap == 0U) {
        memset(trace->plord, 0, sizeof trace->plord);
        memset(&trace->turn_ai_queue, 0, sizeof trace->turn_ai_queue);
    } else if (heap == 6U) {
        if (trace->ai_live_allocations) ++trace->failures;
        memset(trace->ai_planet_records, 0, sizeof trace->ai_planet_records);
        memset(trace->planet_index, 0, sizeof trace->planet_index);
        memset(trace->ai_data, 0, sizeof trace->ai_data);
    }
    /* Other heaps contain no runtime allocations before this fresh-input
     * AI load. Their source reset ordering is still checked above. */
}
static void startup_reset_messages(void *opaque)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    if (trace->log_calls || trace->history_calls || trace->turn_message_requests ||
            trace->session.memory_log_current || trace->session.memory_log_previous != -1)
        ++trace->failures;
    memset(&trace->turn_messages, 0, sizeof trace->turn_messages);
    trace->turn_message_bytes = 0;
    ++trace->ai_reset_messages;
}
static void startup_destroy_game(void *opaque)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsDestroyGameOps ops;
    memset(&ops, 0, sizeof ops);
    if (trace->load_calls || trace->ai_destroy_calls || !trace->ai_input.ai_active ||
            trace->session.dirty || trace->session.selected_kind ||
            trace->session.message_entry_open || trace->session.browser_window ||
            trace->session.report_window || trace->session.popup_window) {
        ++trace->failures; return;
    }
    trace->session.game_flags = (get_u32(trace->rng_file_data, 0x07caU) & 0xffff0000UL) |
        trace->game_flags;
    trace->session.game_flags2 = trace->startup_log_flags;
    ops.reset_heap = startup_reset_heap; ops.reset_messages = startup_reset_messages;
    stars_destroy_cur_game(&trace->session, &ops, trace);
    trace->game_flags = (StarsU16)trace->session.game_flags;
    trace->startup_log_flags = trace->session.game_flags2;
    put_u32(trace->rng_file_data, 0x07caU, trace->session.game_flags);
    put_u16(trace->rng_file_data, 0x07ceU, trace->startup_log_flags);
    memset(&trace->ai_selection, 0, sizeof trace->ai_selection);
    trace->ai_selection.id = trace->session.selected_id;
    trace->ai_selection.planet_id = trace->session.selected_planet;
    trace->ai_selection.active_waypoint = trace->session.selected_waypoint;
    trace->fleet_report.cached = 0;
    ++trace->ai_destroy_calls;
}

static void startup_load_runtime(TurinTrace *);

static StarsI16 load_game(void *opaque, StarsI16 player,
    StarsI16 requested_personality)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    ++trace->load_calls;
    if (trace->ai_destroy_calls != 1U || trace->session.player != -1 ||
            trace->fleet_count || trace->current_designs_loaded) {
        ++trace->failures; return 0;
    }
    startup_load_runtime(trace);
    trace->session.player = player;
    trace->session.player_count = get_u16(trace->complete_input.game, 8U);
    trace->session.game_lid = get_u32(trace->complete_input.game, 0U);
    trace->session.planet_capacity = trace->complete_input.coordinate_count;
    trace->session.planet_count = trace->complete_input.views[2].planet_count;
    trace->session.fleet_count = trace->fleet_count;
    trace->session.thing_count = trace->complete_input.views[2].thing_count;
    trace->session.turn = get_u16(trace->complete_input.game, 18U);
    if (!read_input_player(trace, 3U)) return 0;
    if (!read_retained_m2_shdef_bank(trace, 0)) return 0;
    trace->players[1].personality = get_u16(trace->input_players[3], 6U);
    trace->players[1].dead = (StarsI16)(get_u16(trace->input_players[3], 84U) & 1U);
    return player == 1 && requested_personality == -1;
}

static void *allocate(void *opaque, StarsU32 bytes, StarsU16 tag)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsU16 bit = 0U;
    void *memory = 0;
    if (tag != 6U)
        return 0;
    if (bytes == sizeof(trace->ai_planet_records)) { memory = trace->ai_planet_records; bit = 1U; }
    if (bytes == sizeof(trace->planet_index)) { memory = trace->planet_index; bit = 2U; }
    if (bytes == sizeof(trace->ai_data)) { memory = trace->ai_data; bit = 4U; }
    if (!memory || (trace->ai_live_allocations & bit)) { ++trace->failures; return 0; }
    trace->ai_live_allocations |= bit; ++trace->ai_allocation_calls;
    return memory;
}

static void free_memory(void *opaque, void *memory, StarsU16 tag)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsU16 bit = memory == trace->ai_planet_records ? 1U :
        (memory == trace->planet_index ? 2U : (memory == trace->ai_data ? 4U : 0U));
    if (tag != 6U || !bit || !(trace->ai_live_allocations & bit) ||
            !trace->startup_history_closed || !trace->history_calls || !trace->log_calls) {
        ++trace->failures; return;
    }
    trace->ai_live_allocations &= (StarsU16)~bit; ++trace->ai_release_calls;
}

static void startup_capture_memory_log(void *, StarsU16, StarsU16, const StarsU8 *);

static StarsI16 merge_shdefs(void *opaque, StarsI16 mask)
{
    StarsTurinScoutAdapter *scout = (StarsTurinScoutAdapter *)opaque;
    TurinTrace *trace = (TurinTrace *)scout->context;
    StarsI16 call = trace->merge_calls;

    if (call < 4)
        trace->merge_masks[call] = mask;
    ++trace->merge_calls;
    if (mask == 0x000c) {
        StarsMergeFleetInput input;
        StarsLogXferf first, second;
        StarsU16 slot;
        memcpy(trace->startup_merge_inputs[0], trace->raw_fleets[2], 124U);
        memcpy(trace->startup_merge_inputs[1], trace->raw_fleets[3], 124U);
        memset(&input, 0, sizeof input);
        input.destination = trace->raw_fleets[2];
        input.deleted = trace->raw_fleets[3];
        input.no_delete = 0;
        input.cargo_balance = merge_cargo;
        input.lookup = lookup_merge_fleet;
        input.delete_fleet = delete_merge_fleet;
        input.invalidate_report = invalidate_merge_report;
        input.context = trace;
        stars_merge_two_fleets(&input);
        memset(&first, 0, sizeof first); memset(&second, 0, sizeof second);
        first.id = (StarsU16)(get_u16(trace->startup_merge_inputs[0], 0U) |
            (get_u16(trace->startup_merge_inputs[0], 2U) << 9));
        second.id = (StarsU16)(get_u16(trace->startup_merge_inputs[1], 0U) |
            (get_u16(trace->startup_merge_inputs[1], 2U) << 9));
        first.grobj = 2U; second.grobj = 2U;
        for (slot = 0U; slot < 16U; ++slot)
            first.item[slot] = (StarsU16)(get_u16(trace->raw_fleets[2], (StarsU16)(12U + slot * 2U)) -
                get_u16(trace->startup_merge_inputs[0], (StarsU16)(12U + slot * 2U)));
        stars_log_make_valid_xferf(&first, &second, startup_capture_memory_log, trace);
        for (slot = 0; slot < 16U; ++slot) {
            StarsU16 offset = (StarsU16)(12U + slot * 2U);
            trace->fleets[2].ship_counts[slot] =
                (StarsI16)get_u16(trace->raw_fleets[2], offset);
            trace->fleets[3].ship_counts[slot] =
                (StarsI16)get_u16(trace->raw_fleets[3], offset);
        }
        for (slot = 0; slot < 5U; ++slot) {
            StarsU16 offset = (StarsU16)(76U + slot * 4U);
            trace->fleets[2].current_cargo[slot] =
                (StarsI32)get_u32(trace->raw_fleets[2], offset);
            trace->fleets[3].current_cargo[slot] =
                (StarsI32)get_u32(trace->raw_fleets[3], offset);
        }
        ++trace->production_merge_calls;
        trace->fleets[3].owner = -1;
    }
    return 1;
}

static StarsI32 current_fleet_capacity(void *, const StarsFuelFleet *, StarsI16);

static StarsU32 supply_fleet_stat(void *opaque, const StarsU8 *fleet,
    StarsU16 statistic)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsFuelFleet current;
    StarsU16 slot;
    if (!fleet) {
        ++trace->failures;
        return 0UL;
    }
    ++trace->supply_fleet_stat_calls;
    if (statistic != 2U)
        ++trace->failures;
    memset(&current, 0, sizeof current);
    current.player = 1;
    current.status_flags = get_u16(fleet, 4U);
    for (slot = 0U; slot < 16U; ++slot)
        current.ship_counts[slot] = get_u16(trace->raw_fleets[1],
            (StarsU16)(12U + slot * 2U));
    trace->supply_fleet_stat_value = current_fleet_capacity(trace, &current,
        (StarsI16)statistic);
    return (StarsU32)trace->supply_fleet_stat_value;
}

static StarsU32 supply_cargo_free(void *opaque,
    const StarsCargoState *fleet)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsU16 index;

    if (fleet != &trace->supply_fleet)
        ++trace->failures;
    if (!fleet)
        return 0UL;
    ++trace->supply_cargo_free_calls;
    memset(trace->supply_raw_fleet, 0, sizeof trace->supply_raw_fleet);
    put_u16(trace->supply_raw_fleet, 4U, fleet->fleet_flags);
    for (index = 0U; index < 4U; ++index)
        put_u32(trace->supply_raw_fleet,
            (StarsU16)(0x4cU + index * 4U), fleet->minerals[index]);
    trace->supply_cargo_free_value = stars_get_cargo_free(
        trace->supply_raw_fleet, supply_fleet_stat, trace);
    return trace->supply_cargo_free_value;
}

static StarsI32 change_supply(void *opaque, StarsI16 group, StarsI16 id,
    StarsI16 supply, StarsI32 delta)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsCargoState *object;
    StarsChgCargoOps ops;
    StarsI16 slot;

    slot = trace->supply_calls;
    if (slot < 3) {
        trace->supply_group[slot] = group;
        trace->supply_id[slot] = id;
        trace->supply_index[slot] = supply;
        trace->supply_delta[slot] = delta;
    }
    ++trace->supply_calls;
    if (group == 1 && id == 24)
        object = &trace->supply_planet;
    else if (group == 2 && id == 1)
        object = &trace->supply_fleet;
    else {
        ++trace->failures;
        return 0L;
    }
    memset(&ops, 0, sizeof ops);
    ops.cargo_free = supply_cargo_free;
    {
        StarsI32 result = stars_change_cargo(group, (StarsU16)id, supply, delta,
            object, &ops, trace);
        if (group == 2) {
            StarsU16 lane, index;
            for (lane = 0U; lane < 5U; ++lane)
                put_u32(trace->raw_fleets[id], (StarsU16)(76U + lane * 4U),
                    object->minerals[lane]);
            for (index = 0U; index < trace->complete_input.views[0].fleet_count; ++index) {
                Soe3InputFleet *fleet = &trace->complete_input.views[0].fleets[index];
                if (fleet->owner == 1 && (fleet->id_word & 511U) == (StarsU16)id)
                    for (lane = 0U; lane < 5U; ++lane)
                        fleet->state.minerals[lane] = object->minerals[lane];
            }
        }
        return result;
    }
}

static StarsI16 startup_first_fleet_pass(void *opaque, StarsI16 turn)
{
    StarsTurinScoutAdapter *scout = (StarsTurinScoutAdapter *)opaque;
    TurinTrace *trace = (TurinTrace *)scout->context;
    StarsI16 index, slot;
    StarsI16 hulls[16];
    (void)turn;
    trace->first_pass_attack_head = -1;
    for (slot = 0; slot < 16; ++slot)
        hulls[slot] = (StarsI16)get_u16(trace->designs + slot * 147U, 0U);
    for (index = 0; index < trace->turin_input.fleet_count; ++index) {
        const StarsTurinTurnFleet *fleet = &trace->turin_input.fleets[index];
        StarsI16 destination, planet_index;
        StarsU16 flags;
        if (fleet->id < 0 || fleet->id >= TURIN_RETAINED_FLEET_COUNT ||
                fleet->cord < 1 ||
                fleet->cord > 2 || !fleet->raw_plord) return 0;
        if (!trace->raw_fleet_present[fleet->id]) continue;
        /* Enemy list consumers are not bound in this no-enemy input adapter. */
        if (fleet->owner != trace->session.player) return 0;
        trace->first_pass_attack_next[fleet->id] = -1;
        if (stars_is_turin_drone_ai_attack(fleet->ship_counts, hulls)) {
            trace->first_pass_attack_next[fleet->id] = trace->first_pass_attack_head;
            trace->first_pass_attack_head = fleet->id;
        }
        flags = (StarsU16)(get_u16(trace->raw_fleets[fleet->id], 4U) & 0x7fffU);
        put_u16(trace->raw_fleets[fleet->id], 4U, flags);
        for (slot = 0; slot < (StarsI16)trace->fleet_count; ++slot)
            if (get_u16(trace->fleet_items[slot]->bytes, 0U) == (StarsU16)fleet->id)
                put_u16(trace->fleet_items[slot]->bytes, 4U, flags);
        if ((fleet->ship_counts[2] || fleet->ship_counts[3]) &&
                (get_u16(fleet->raw_plord,
                    (StarsU16)(10 + (fleet->cord - 1) * 18)) & 15U)) return 0;
        if (fleet->ship_counts[1] || fleet->ship_counts[8] || fleet->ship_counts[9]) {
            destination = fleet->cord < 2 ? fleet->planet_id : -1;
            if (fleet->cord > 1 &&
                    ((get_u16(fleet->raw_plord, 28U) >> 8) & 15U) == 1U)
                destination = (StarsI16)get_u16(fleet->raw_plord, 26U);
            if (destination < 0) {
                if (fleet->cord > 1 &&
                        ((get_u16(fleet->raw_plord, 28U) >> 8) & 15U) == 4U)
                    return 0;
            } else {
                for (planet_index = 0; planet_index < trace->turin_input.planet_count;
                        ++planet_index)
                    if (trace->turin_input.planets[planet_index].id == destination) break;
                /* Foreign/missing targets require original drop/clear services. */
                if (destination >= TURIN_RETAINED_POINT_COUNT ||
                        planet_index == trace->turin_input.planet_count ||
                        trace->ai_planet_records[destination * 16 + 3] ||
                        (trace->turin_input.planets[planet_index].owner != -1 &&
                        trace->turin_input.planets[planet_index].owner != fleet->owner))
                    return 0;
            }
        }
        ++trace->first_pass_visits;
    }
    return 1;
}

static StarsI16 plan_turin_fleet(void *opaque,
    const StarsTurinTurnFleet *fleet, const StarsTurinFleetPolicy *policy,
    StarsI16 turn, StarsTurinFleetPlan *plan)
{
    StarsTurinScoutAdapter *scout;
    TurinTrace *trace;
    StarsI16 index;

    scout = (StarsTurinScoutAdapter *)opaque;
    trace = (TurinTrace *)scout->context;
    /* Original second-pass dispatch (ai.c:2079 onward). The bounded route
     * adapter must not turn an unsupported role into a successful scout no-op. */
    if (fleet->cord >= 2)
        return 1;
    if (policy->needs_order_repair) {
        ++trace->failures; return 0;
    }
    if (policy->transport_ships > 0) {
        for (index = 0; index < trace->turin_input.planet_count; ++index) {
            if (trace->turin_input.planets[index].id == fleet->planet_id) {
                ++trace->transport_plan_calls;
                if (!stars_turin_plan_local_transport(fleet,
                        &trace->turin_input.planets[index],
                        trace->turin_input.player_id, plan) ||
                        plan->action != STARS_TURIN_FLEET_ACTION_TRANSPORT) {
                    ++trace->failures; return 0;
                }
                return 1;
            }
        }
    }
    if (policy->transport_ships > 0 || fleet->ship_counts[8] ||
            fleet->ship_counts[9] || fleet->ship_counts[13] ||
            fleet->ship_counts[14] ||
            !(fleet->ship_counts[0] || fleet->ship_counts[10] ||
            fleet->ship_counts[11])) {
        ++trace->failures; return 0;
    }
    /* A high-tech obsolete scout takes the original scrap path, not routing. */
    if (fleet->ship_counts[0] &&
            trace->complete_input.views[2].players[1][29U] > 5U &&
            get_u16(trace->designs, 0U) == 4U) {
        ++trace->failures; return 0;
    }
    scout->enemy_candidate_count = 0U;
    for (index = 0; index < (StarsI16)trace->fleet_count; ++index) {
        StarsU16 id = get_u16(trace->fleet_items[index]->bytes, 0U);
        if (id >= TURIN_RETAINED_FLEET_COUNT || !trace->raw_fleet_present[id]) {
            ++trace->failures; return 0;
        }
        if (stars_enum_calc_enemy_fleet(trace->raw_fleets[id],
                (StarsU16)trace->session.player)) ++scout->enemy_candidate_count;
        ++trace->scout_enemy_visits;
    }
    if (scout->enemy_candidate_count) {
        /* This adapter binds the no-enemy scout path. Eligible combat must
         * not silently take the scout helper's early return. */
        ++trace->failures; return 0;
    }
    if (!stars_turin_plan_scout_planet(scout, fleet, policy, turn, plan) ||
            plan->action != STARS_TURIN_FLEET_ACTION_MOVE) {
        /* Unarmed/no-target fallbacks have additional original behavior. */
        ++trace->failures; return 0;
    }
    return 1;
}

static StarsI16 transfer_turin_supply(void *opaque,
    const StarsTurinTurnFleet *fleet, const StarsTurinFleetPlan *plan)
{
    StarsTurinScoutAdapter *scout = (StarsTurinScoutAdapter *)opaque;
    TurinTrace *trace = (TurinTrace *)scout->context;
    StarsI16 transferred;
    StarsCargoState before = trace->supply_fleet;

    ++trace->transport_calls;
    if (fleet->id != 1 || fleet->planet_id != 24 ||
            plan->action != STARS_TURIN_FLEET_ACTION_TRANSPORT)
        return 0;
    transferred = stars_xfer_ai_supply(1, fleet->planet_id, 2, fleet->id,
        3, STARS_TURIN_LOCAL_SUPPLY_COLONISTS, change_supply, trace);
    if (trace->seeded_mode && transferred != 0) {
        StarsLogXfer first, second;
        StarsU8 *data = (StarsU8 *)calloc(0x5262U, 1U);
        StarsU8 log[28];
        StarsU16 lane, header;
        if (!data) { ++trace->failures; return 0; }
        memset(&first, 0, sizeof first); memset(&second, 0, sizeof second);
        memset(log, 0, sizeof log);
        first.id = (StarsU16)(fleet->id | (trace->turin_input.player_id << 9));
        first.grobj = 2U; second.id = (StarsU16)fleet->planet_id; second.grobj = 1U;
        for (lane = 0U; lane < 5U; ++lane)
            first.item[lane] = trace->supply_fleet.minerals[lane] - before.minerals[lane];
        /* Fresh local memory log: the preceding unified record is an order
         * insertion, so no prior cargo transfer can coalesce here. */
        stars_log_make_valid_xfer(data, log, &first, &second, 0, trace);
        header = get_u16(log, 0U);
        if (get_u16(data, 0x09a4U) != (header & 1023U) + 2U)
            ++trace->failures;
        else
            startup_capture_memory_log(trace, (StarsU16)(header >> 10),
                (StarsU16)(header & 1023U), log + 2U);
        free(data);
    }
    return transferred == STARS_TURIN_LOCAL_SUPPLY_COLONISTS;
}

static StarsI16 startup_prepare_random(void *opaque, StarsI16 limit)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    ++trace->startup_prepare_random_calls;
    return turn_main_random(trace, limit);
}

static void startup_init_planet_list(void *opaque)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    const Soe3InputView *view = &trace->complete_input.views[2];
    StarsU8 raw[32][56];
    const StarsU8 *input[32], *output[32];
    StarsU16 i, count = 0U;
    if (view->planet_count > 32U) { ++trace->failures; return; }
    for (i = 0U; i < view->planet_count; ++i) {
        pack_loaded_planet(raw[i], &view->planets[i]);
        input[i] = raw[i];
    }
    stars_init_random_planet_list(input, view->planet_count, 1,
        (StarsU16)((trace->game_flags >> 11) & 1U), output, &count,
        startup_prepare_random, trace);
    trace->startup_owned_planet_count = count;
    for (i = 0U; i < count; ++i)
        trace->startup_owned_planets[i] = get_u16(output[i], 0U);
    /* This coordinator currently has one Turin planet slot. Fail if the
     * complete source list needs more, rather than silently truncating it. */
    if (count != 1U || trace->planet.id != (StarsI16)trace->startup_owned_planets[0]) {
        ++trace->failures; return;
    }
    trace->turin_input.planet_count = count;
    trace->planet.id = (StarsI16)trace->startup_owned_planets[0];
}

static void startup_increase_minefields(void *opaque)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    Soe3InputView *view = &trace->complete_input.views[2];
    StarsAiMineThing things[4];
    StarsU16 i;
    if (view->thing_count > 4U) { ++trace->failures; return; }
    for (i = 0U; i < view->thing_count; ++i)
        memcpy(things[i].raw, view->things[i], 18U);
    stars_increase_ai_minefield_sizes(things, view->thing_count);
    for (i = 0U; i < view->thing_count; ++i)
        memcpy(view->things[i], things[i].raw, 18U);
    ++trace->startup_mine_prepare_calls;
}

static StarsI32 turn_score_design_power(TurinTrace *, const StarsU8 *);

static void startup_compute_powers(void *opaque)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsU16 player, slot, present;
    for (player = 0U; player < get_u16(trace->complete_input.game, 8U); ++player) {
        StarsU8 *bank;
        if (player >= 2U) { ++trace->failures; return; }
        present = 0U;
        for (slot = 0U; slot < 16U; ++slot)
            present = (StarsU16)(present | trace->complete_input.views[2].design_present[player][slot]);
        if (!present) continue;
        bank = player == 1U ? trace->designs :
            trace->complete_input.views[2].ship_designs[player];
        for (slot = 0U; slot < 16U; ++slot) {
            StarsU8 *raw = bank + slot * 147U;
            if (get_u16(raw, 123U) & 0x0200U) continue;
            put_u32(raw, 135U, (StarsU32)turn_score_design_power(trace, raw));
            ++trace->startup_power_designs;
        }
    }
}

static StarsI16 startup_unexpected_attacker(void *opaque, StarsI16 owner, StarsU16 design)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    (void)owner; (void)design;
    ++trace->failures;
    return -1;
}

static const StarsU8 *startup_unexpected_attack_planet(void *opaque, StarsI16 planet)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    (void)planet;
    ++trace->failures;
    return 0;
}

static void startup_unexpected_attack_mark(void *opaque, StarsI16 planet)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    (void)planet;
    ++trace->failures;
}

static void startup_mark_attacks(void *opaque)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    const StarsU8 *fleets[TURIN_RETAINED_FLEET_COUNT];
    StarsMarkPlanetsUnderAttackOps ops;
    StarsU16 i, count = 0U;
    for (i = 0U; i < TURIN_RETAINED_FLEET_COUNT; ++i)
        if (trace->raw_fleet_present[i]) fleets[count++] = trace->raw_fleets[i];
    ops.hull_category = startup_unexpected_attacker;
    ops.lookup_planet = startup_unexpected_attack_planet;
    ops.mark_planet = startup_unexpected_attack_mark;
    /* All loaded M2 fleets belong to this player. Source rejects them before
     * any attacker lookup. Unexpected enemy evaluation fails explicitly. */
    stars_mark_planets_under_attack(fleets, count, 1, &ops, trace);
    trace->startup_attack_fleets = count;
}

static void run_bof_writer(TurinTrace *, StarsI16);

static StarsI16 save_generated_mutations(TurinTrace *trace)
{
    StarsU16 index, cursor = 0U;
    if (trace->startup_memory_count > 16U) return 0;
    for (index = 0U; index < trace->startup_memory_count; ++index) {
        StarsU16 size = trace->startup_memory_sizes[index];
        StarsU16 type = trace->startup_memory_types[index];
        if (size > 160U || type > 63U ||
                (StarsU32)cursor + size + 2U > sizeof trace->saved_mutation_stream) return 0;
        put_u16(trace->saved_mutation_stream, cursor, (StarsU16)((type << 10) | size));
        memcpy(trace->saved_mutation_stream + cursor + 2U,
            trace->startup_memory_records[index], size);
        cursor = (StarsU16)(cursor + size + 2U);
    }
    trace->saved_mutation_size = cursor;
    trace->saved_mutation_records = trace->startup_memory_count;
    return 1;
}

static StarsI16 turn_validate_replay_serial(void *opaque, StarsU32 serial)
{
    (void)opaque;
    return stars_file_valid_serial_long(original_dgroup, serial);
}

static StarsI16 turn_log_create(void *opaque, StarsI16 player, const StarsU8 *base)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    (void)base;
    if (player != 1 || trace->generated_log_size) { ++trace->failures; return 0; }
    run_bof_writer(trace, 1);
    memcpy(trace->generated_log_file, trace->bof_write_records[0], 18U);
    trace->generated_log_size = 18U;
    return 1;
}

static void turn_log_transfer(void *opaque, const StarsU8 *bytes, StarsU16 count)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    if ((StarsU32)trace->generated_log_size + count > sizeof trace->generated_log_file) {
        ++trace->failures; return;
    }
    if (count) memcpy(trace->generated_log_file + trace->generated_log_size, bytes, count);
    trace->generated_log_size = (StarsU16)(trace->generated_log_size + count);
}

static void turn_log_record(void *opaque, StarsU16 type, StarsU16 size, const void *body)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    if (type == 9U && (size != 17U || memcmp(body,
            expected_log_headers[trace->seeded_action], 17U) != 0)) ++trace->failures;
    stars_write_record(trace->rng_file_data, (StarsI16)type, size,
        size ? (const StarsU8 *)body : trace->saved_mutation_stream, turn_log_transfer, trace);
    ++trace->generated_log_records;
}

static void turn_log_close(void *opaque)
{ ++((TurinTrace *)opaque)->generated_log_closed; }

static void turn_log_title(void *opaque, StarsU16 window)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    (void)window;
    ++trace->log_title_updates;
}

static void turn_set_dirty(TurinTrace *trace, StarsI16 dirty)
{
    StarsU8 data[0x1444U];
    memset(data, 0, sizeof data);
    put_u16(data, 0x007eU, get_u16(trace->complete_input.game, 14U));
    put_u16(data, 0x1442U, (StarsU16)trace->ai_input.ai_active);
    stars_dirty_game(data, dirty, turn_log_title, trace);
    put_u16(trace->complete_input.game, 14U, get_u16(data, 0x007eU));
}

static void turn_log_clean(void *opaque)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    turn_set_dirty(trace, 0);
    ++trace->log_dirty_calls;
}

static StarsI16 turn_write_generated_log(TurinTrace *trace)
{
    StarsWriteLogFileInput input;
    StarsSeeds decode_seeds;
    StarsDiskIdentity drives[2];
    StarsU8 registered_environment[11];
    memset(&input, 0, sizeof input); memset(&decode_seeds, 0, sizeof decode_seeds);
    memset(drives, 0, sizeof drives);
    if (!stars_serial_and_env_from_string(&decode_seeds, replay_environment_ini,
            &trace->replay_serial, registered_environment, replay_environment_shuffle,
            turn_validate_replay_serial, trace)) return 0;
    /* Explicit equivalent replay profile: archived unlabeled Win98 C disk;
     * D unavailable. Disk identity provenance is retained in its audit. */
    drives[0].free_nibble = REPLAY_DISK_CAPACITY_NIBBLE;
    if (stars_get_disk_serial_number(drives, trace->replay_environment) != 11U) return 0;
    input.player = 1; input.current_player = trace->session.player;
    input.player_flags = get_u16(trace->complete_input.views[0].players[1], 6U);
    if (!(input.player_flags & 0x0200U)) { ++trace->failures; return 0; }
    input.serial = trace->replay_serial; input.environment = trace->replay_environment;
    input.log = trace->saved_mutation_stream; input.log_size = trace->saved_mutation_size;
    input.game_flags = &trace->startup_log_flags;
    input.create_file = turn_log_create; input.write_record = turn_log_record;
    input.close_stream = turn_log_close; input.dirty_game = turn_log_clean;
    input.context = trace;
    if (!stars_write_log_file(&input)) return 0;
    return trace->generated_log_closed == 1U &&
        trace->generated_log_records == trace->saved_mutation_records + 2U;
}

static void write_log(void *opaque, StarsI16 player)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    if (trace->session.player != player || !trace->ai_input.ai_active)
        ++trace->failures;
    if (player == 1) {
        if (trace->turn_committed_queue_count != 2U ||
                trace->turn_seed_finish_calls != 1 ||
            (trace->seeded_mode && trace->final_queue_budget_calls != 1) || trace->turn_produce_calls != 0)
            ++trace->failures;
        ++trace->log_calls;
        if (trace->seeded_mode) {
            if (!save_generated_mutations(trace) || !trace->saved_mutation_size) {
                ++trace->failures; return;
            }
            if (!turn_write_generated_log(trace)) { ++trace->failures; return; }
        }
        trace->runtime_log_present[player] = 1U;
    }
}

static StarsI16 startup_history_create(void *opaque, StarsI16 player)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    if (player != 1) { ++trace->failures; return 0; }
    if (trace->seeded_mode) run_bof_writer(trace, 4);
    if (trace->seeded_mode) {
        memcpy(trace->generated_history_file, trace->bof_write_records[1], 18U);
        trace->generated_history_size = 18U;
    }
    return 1;
}

static void startup_history_transfer(void *opaque, const StarsU8 *bytes, StarsU16 count)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    if ((StarsU32)trace->generated_history_size + count > sizeof trace->generated_history_file) {
        ++trace->failures; return;
    }
    if (count) memcpy(trace->generated_history_file + trace->generated_history_size, bytes, count);
    trace->generated_history_size = (StarsU16)(trace->generated_history_size + count);
}

static void startup_history_record(void *opaque, StarsU16 type, StarsU16 size,
    const void *body)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsU16 i = trace->startup_history_record_count;
    if (i >= 4U || size > 128U) { ++trace->failures; return; }
    trace->startup_history_types[i] = type;
    trace->startup_history_sizes[i] = size;
    if (size) memcpy(trace->startup_history_records[i], body, size);
    if (trace->seeded_mode)
        stars_write_record(trace->rng_file_data, (StarsI16)type, size,
            size ? (const StarsU8 *)body : trace->generated_history_file,
            startup_history_transfer, trace);
    ++trace->startup_history_record_count;
}

static void startup_history_planet(void *opaque, StarsU16 type, const void *planet)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsU8 record[128];
    StarsU16 length;
    if (!stars_write_planet((const StarsReadPlanetState *)planet, type, 1U,
            record, sizeof record, &length, 0, trace)) { ++trace->failures; return; }
    startup_history_record(trace, type, length, record);
}

static void startup_history_unexpected(void *opaque, const void *record)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    (void)record;
    ++trace->failures;
}

static void startup_history_close(void *opaque)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    ++trace->startup_history_closed;
    if (trace->seeded_mode && (trace->generated_history_size != expected_history_file_sizes[trace->seeded_action] ||
            memcmp(trace->generated_history_file, expected_history_files[trace->seeded_action],
                trace->generated_history_size))) ++trace->failures;
}

static void write_history(void *opaque, StarsI16 player)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    Soe3InputView *view = &trace->complete_input.views[2];
    StarsWriteHistFileInput input;
    StarsHistoryPlanet planets[32];
    StarsHistoryPlayer players[2];
    StarsU16 i, j;
    if (player != 1 || view->planet_count > 32U || trace->session.player != player ||
            !trace->ai_input.ai_active) { ++trace->failures; return; }
    memset(&input, 0, sizeof input); memset(players, 0, sizeof players);
    for (i = 0U; i < view->planet_count; ++i) {
        planets[i].owner_flags = (StarsU16)(view->planets[i].flags & 255U);
        planets[i].record = &view->planets[i];
    }
    for (i = 0U; i < 2U; ++i) {
        players[i].detection_flags = (StarsU16)(get_u16(view->players[i], 6U) & 0x0107U);
        players[i].record = view->players[i];
        for (j = 0U; j < 16U; ++j) {
            players[i].designs[j].flags = get_u16(view->ship_designs[i] + j * 147U, 123U);
            players[i].designs[j].record = view->ship_designs[i] + j * 147U;
        }
        for (j = 0U; j < 10U; ++j) {
            players[i].enemy_designs[j].flags = get_u16(view->starbase_designs[i] + j * 147U, 123U);
            players[i].enemy_designs[j].record = view->starbase_designs[i] + j * 147U;
        }
    }
    for (i = 0U; i < TURIN_RETAINED_FLEET_COUNT; ++i)
        if (trace->raw_fleet_present[i]) ++players[1].fleet_count;
    input.player = player; input.planet_count = view->planet_count;
    input.game_player_count = get_u16(trace->complete_input.game, 8U);
    input.game_turn = get_u16(trace->complete_input.game, 18U);
    input.planets = planets; input.players = players;
    input.message_bits = trace->startup_message_filters;
    input.message_bits_count = sizeof trace->startup_message_filters;
    input.ai_data = trace->ai_data;
    input.ai_size = trace->ai_data[0] > 2U ? trace->ai_data[0] : 0U;
    input.create_file = startup_history_create; input.write_record = startup_history_record;
    input.write_planet = startup_history_planet; input.write_player = startup_history_unexpected;
    input.write_design = startup_history_unexpected; input.write_score = startup_history_unexpected;
    input.close_stream = startup_history_close; input.context = trace;
    if (input.game_player_count != 2U || !stars_write_hist_file(&input)) ++trace->failures;
    ++trace->history_calls;
}

static void startup_scout_markers(TurinTrace *trace, const StarsTurinTurnFleet *active)
{
    StarsI16 owners[TURIN_RETAINED_POINT_COUNT];
    StarsI16 blocked[TURIN_RETAINED_FLEET_COUNT];
    StarsU8 status[TURIN_RETAINED_POINT_COUNT];
    const Soe3InputView *view = &trace->complete_input.views[2];
    StarsU16 i, id, flags, count = 0;
    StarsI16 mode = (StarsI16)(get_u16(view->players[1], 6U) >> 13);
    StarsPoint point;
    if (trace->scout_marker_calls) return;
    for (i = 0; i < TURIN_RETAINED_POINT_COUNT; ++i) owners[i] = -2;
    for (i = 0; i < view->planet_count; ++i) {
        id = (StarsU16)view->planets[i].id;
        if (id >= TURIN_RETAINED_POINT_COUNT || view->planets[i].player == -1) {
            ++trace->failures; return;
        }
        owners[id] = view->planets[i].player;
    }
    for (i = 0; i < trace->fleet_count; ++i) {
        id = get_u16(trace->fleet_items[i]->bytes, 0U);
        if (id >= TURIN_RETAINED_FLEET_COUNT) { ++trace->failures; return; }
        if (id == (StarsU16)active->id || get_u16(trace->raw_fleets[id], 2U) != 1U ||
                trace->plord[id][3] < 2U) continue;
        flags = get_u16(trace->plord[id], 28U);
        if (((flags >> 8) & 15U) == 1U &&
                ((mode != 0 && mode != 5) || (flags & 15U) == 2U))
            blocked[count++] = (StarsI16)get_u16(trace->plord[id], 26U);
    }
    point.x = (StarsI16)get_u16(trace->raw_fleets[active->id], 8U);
    point.y = (StarsI16)get_u16(trace->raw_fleets[active->id], 10U);
    memset(status, 0, sizeof status);
    /* IdNearestUnknownPlanet first invokes this with a null wormhole output. */
    (void)stars_id_nearest_colonizable_planet(point, active->planet_id, mode,
        (StarsI16)get_u16(trace->complete_input.game, 18U), 0, 1, status,
        owners, trace->points, TURIN_RETAINED_POINT_COUNT, blocked, count, 0, trace, 0);
    for (i = 0; i < TURIN_RETAINED_POINT_COUNT; ++i) {
        trace->ai_planet_records[i*16U+15U] = status[i];
        if (status[i] != retained_ai_planet_status[i]) ++trace->failures;
    }
    ++trace->scout_marker_calls;
}

static StarsI16 fleet_point(void *opaque, const StarsTurinTurnFleet *fleet,
    StarsPoint *out)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    if (fleet->planet_id < 0 || fleet->planet_id >= 32 || !out)
        return 0;
    startup_scout_markers(trace, fleet);
    *out = trace->points[(StarsU16)fleet->planet_id];
    return 1;
}

static const StarsHullComponent *lookup_hull_components(void *opaque,
    StarsU16 owner, StarsU16 design_id, StarsU16 *component_count)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsU8 hull[143];
    StarsU16 slot;
    if (owner != 1U || design_id >= 16U || !component_count)
        return 0;
    ++trace->hull_lookup_calls;
    if (!scrap_read_hull(trace, get_u16(trace->designs + design_id * 147U, 0U),
            hull, sizeof hull) || hull[122] > 16U) {
        ++trace->failures; *component_count = 0U; return 0;
    }
    *component_count = hull[122];
    if (*component_count == 0U)
        return 0;
    for (slot = 0U; slot < *component_count; ++slot) {
        trace->current_hull_components[design_id][slot].attributes =
            get_u16(hull, (StarsU16)(58U + slot * 4U));
        trace->current_hull_components[design_id][slot].packed_damage =
            get_u16(hull, (StarsU16)(60U + slot * 4U));
    }
    return trace->current_hull_components[design_id];
}

static StarsI16 fleet_has_teeth(void *opaque,
    const StarsTurinTurnFleet *fleet)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    if (!fleet)
        return 0;
    ++trace->fleet_teeth_calls;
    return stars_fleet_might_have_teeth(fleet->ship_counts,
        (StarsU16)fleet->owner << 9, lookup_hull_components, trace);
}

static const StarsIdealWarpEngine *lookup_ideal_warp_engine(void *opaque,
    StarsU16 engine_id)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    const StarsU8 *engine;
    StarsU16 warp;
    if (engine_id >= 16U)
        return 0;
    ++trace->ideal_engine_lookups;
    trace->ideal_engine_id = (StarsI16)engine_id;
    engine = scrap_part_record(1U, engine_id);
    if (!engine) { ++trace->failures; return 0; }
    memset(&trace->current_ideal_engines[engine_id], 0,
        sizeof trace->current_ideal_engines[engine_id]);
    for (warp = 0U; warp < 12U; ++warp)
        trace->current_ideal_engines[engine_id].fuel_used[warp] =
            (StarsI16)get_u16(engine, (StarsU16)(54U + warp * 2U));
    /* Both ideal-warp modes read this fuel table; ore_cost is legacy only. */
    return &trace->current_ideal_engines[engine_id];
}

static StarsI16 current_ideal_warp(TurinTrace *trace,
    const StarsI16 *ship_counts, StarsI16 ignore_scoops)
{
    StarsFindIdealWarpInput input;
    StarsU16 ship, slot;
    memset(&input, 0, sizeof input);
    memcpy(input.ship_counts, ship_counts, sizeof input.ship_counts);
    for (ship = 0U; ship < 16U; ++ship) {
        const StarsU8 *design = trace->designs + ship * 147U;
        if (!input.ship_counts[ship]) continue;
        if (design[122] > 16U) { ++trace->failures; return 0; }
        for (slot = 0U; slot < design[122]; ++slot) {
            StarsU16 base = (StarsU16)(58U + slot * 4U);
            if (get_u16(design, base) == 1U && design[base + 3U]) {
                StarsU16 index = input.designs[ship].engine_count;
                if (index >= 10U) { ++trace->failures; return 0; }
                input.designs[ship].engine_flags[index] = 1U;
                input.designs[ship].engine_ids[index] = design[base + 2U];
                ++input.designs[ship].engine_count;
            }
        }
    }
    input.lookup_engine = lookup_ideal_warp_engine;
    input.context = trace;
    return stars_find_ideal_warp(&input, ignore_scoops);
}

static StarsI16 ideal_warp(void *opaque, const StarsTurinTurnFleet *fleet,
    StarsI16 target, StarsPoint point)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    if (target != TURIN_RETAINED_EXPECTED_TARGET ||
            point.x != TURIN_RETAINED_EXPECTED_X ||
            point.y != TURIN_RETAINED_EXPECTED_Y || !fleet)
        ++trace->failures;
    ++trace->order_warp_calls;
    if (!fleet) return 0;
    return current_ideal_warp(trace, fleet->ship_counts, 1);
}

static StarsI16 random_value(void *opaque, StarsI16 limit)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsI16 index = trace->random_calls;
    StarsI16 value;
    if (trace->seeded_mode) {
        value = turn_main_random(trace, limit);
    } else {
        if (limit != 100)
            ++trace->failures;
        value = trace->random_result;
    }
    if (index < 16) {
        trace->random_limits[index] = limit;
        trace->random_values[index] = value;
    }
    ++trace->random_calls;
    return value;
}

static TurinTrace *startup_trace(void *opaque)
{
    StarsTurinScoutAdapter *scout = (StarsTurinScoutAdapter *)opaque;
    return scout ? (TurinTrace *)scout->context : (TurinTrace *)0;
}

static StarsI16 startup_name_random(void *opaque, StarsI16 limit)
{
    TurinTrace *trace = startup_trace(opaque);
    StarsI16 index;
    if (!trace)
        return 0;
    index = trace->startup_name_random_calls;
    if (index >= 8) {
        ++trace->failures;
        return 0;
    }
    trace->startup_name_limits[index] = limit;
    trace->startup_name_values[index] = trace->rototill_seeded_mode ?
        stars_random(&trace->seeds, limit) : turn_main_random(trace, limit);
    ++trace->startup_name_random_calls;
    return trace->startup_name_values[index];
}

static StarsI16 startup_name_get(void *opaque, StarsI16 string_id,
    char *out, StarsU16 capacity)
{
    TurinTrace *trace = startup_trace(opaque);
    static StarsU8 string_data[65536];
    StarsU16 offset;
    size_t length;
    if (!trace || string_id < 0x432 || string_id >= 0x43f)
        return 0;
    string_data[0x1484] = 0xffU;
    offset = stars_get_compressed_string(string_data, original_string_segment,
        string_id);
    length = strlen((const char *)string_data + offset);
    if (length >= capacity) { ++trace->failures; return 0; }
    memset(out, 0, capacity);
    memcpy(out, string_data + offset, length);
    trace->startup_last_name_id = (StarsU16)string_id;
    return 1;
}

static StarsI16 startup_bitmap_in_use(void *opaque, StarsU16 bitmap,
    StarsU16 hull)
{
    TurinTrace *trace = startup_trace(opaque);
    StarsU16 index;
    if (!trace)
        return 0;
    for (index = 0U; index < 10U; ++index) {
        const StarsU8 *raw = trace->starbase_designs + index * 147U;
        if ((get_u16(raw, 123U) & 0x200U) == 0U &&
                get_u16(raw, 0U) == hull && get_u16(raw, 50U) == bitmap)
            return 1;
    }
    for (index = 0U; index < (StarsU16)trace->startup_name_count; ++index)
        if (trace->startup_name_hulls[index] == hull &&
                (StarsU16)trace->startup_name_bitmaps[index] == bitmap)
            return 1;
    return 0;
}

static StarsI16 startup_name_in_use(void *opaque, StarsU16 design_class,
    const char *name)
{
    TurinTrace *trace = startup_trace(opaque);
    StarsU16 slot;
    StarsU16 hull;
    if (!trace) return 0;
    if (design_class < 16U) { ++trace->failures; return 1; }
    hull = get_u16(trace->startup_build_shdef, 0U);
    for (slot = 0U; slot < 10U; ++slot) {
        const StarsU8 *raw = trace->starbase_designs + slot * 147U;
        if ((get_u16(raw, 123U) & 0x200U) == 0U &&
                get_u16(raw, 0U) == hull &&
                strncmp((const char *)raw + 8U, name, 32U) == 0)
            return 1;
    }
    for (slot = 0U; slot < (StarsU16)trace->startup_name_count; ++slot)
        if (trace->startup_name_hulls[slot] == hull &&
                strncmp(trace->startup_name_text[slot], name, 32U) == 0)
            return 1;
    return 0;
}

static StarsI16 startup_part_race(void *opaque, StarsI16 player, StarsI16 stat)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    if (player != 1 || stat != 14) { ++trace->failures; return 0; }
    return (StarsI16)(signed char)trace->complete_input.views[0].players[1][76];
}

static StarsI16 startup_part_race_bit(void *opaque, StarsI16 player, StarsI16 bit)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    if (player != 1 || bit < 0 || bit >= 32) { ++trace->failures; return 0; }
    return (StarsI16)((get_u32(trace->complete_input.views[0].players[1], 78U) >> bit) & 1UL);
}

static StarsU32 startup_part_token(void *opaque, StarsU16 group, StarsU16 item)
{
    (void)opaque;
    return ((StarsU32)group << 16) | item;
}

static StarsI16 startup_part_tech(void *opaque, StarsU32 token)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    const StarsU8 *record = scrap_part_record((StarsU16)(token >> 16), (StarsU16)token);
    StarsI16 current[6], required[6];
    StarsU16 i;
    if (!record) return 0;
    for (i = 0U; i < 6U; ++i) {
        current[i] = trace->complete_input.views[0].players[1][26U + i];
        required[i] = record[2U + i];
    }
    return stars_tech_status(current, required,
        trace->complete_input.views[0].players[1][57]);
}

static StarsI16 startup_lookup_part(void *opaque, StarsU16 group, StarsU16 item,
    StarsAiPart *part)
{
    StarsLookupPartInput input;
    memset(&input, 0, sizeof input);
    input.group = group; input.item = item; input.player = 1;
    input.context = opaque; input.race_stat = startup_part_race;
    input.race_grbit = startup_part_race_bit; input.resolve = startup_part_token;
    input.tech_status = startup_part_tech;
    if (stars_lookup_part(&input) != 1) return 0;
    part->component = stars_lookup_part_record(&input, retained_part_segment,
        (StarsU16)sizeof retained_part_segment);
    part->tech_group = group; part->attributes = item;
    return part->component != 0;
}

static StarsI16 startup_lookup_ai_part(void *opaque, StarsU16 choice, StarsAiPart *part)
{
    StarsI16 result;
    if (choice >= sizeof original_ai_part_counts) return 0;
    result = stars_f_get_ai_part(choice, original_ai_part_counts,
        (StarsU16)(sizeof original_ai_part_choices / sizeof original_ai_part_choices[0]),
        original_ai_part_choices, startup_lookup_part, part, opaque);
    if (!result) fprintf(stderr, "ai choice failed %u\n", choice);
    return result;
}

static StarsI16 startup_lookup_ai_hull(void *opaque, StarsU16 group, StarsU16 id,
    StarsAiShdefWork *work)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsAiPart part;
    if (!startup_lookup_part(opaque, group, id, &part) || group != 0x0400U ||
            part.component[122] > 16U) {
        fprintf(stderr, "ai hull failed %u %u\n", group, id); return 0;
    }
    memset(trace->startup_build_shdef, 0, sizeof trace->startup_build_shdef);
    memcpy(trace->startup_build_shdef, part.component, 123U);
    work->hull_id = get_u16(part.component, 0U);
    work->bitmap_id = (StarsI16)get_u16(part.component, 50U);
    work->part_count = part.component[122];
    return 1;
}

static StarsI16 startup_publish_build(void *opaque, StarsI16 design,
    const StarsAiShdefWork *work)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsU16 i;
    if (design != -1 || work->part_count > 16U) return 0;
    for (i = 0U; i < work->part_count; ++i) {
        put_u16(trace->startup_build_shdef, (StarsU16)(58U + i * 4U), work->part_groups[i]);
        trace->startup_build_shdef[60U + i * 4U] = (StarsU8)work->part_attributes[i];
    }
    put_u16(trace->startup_build_shdef, 123U, work->flags);
    return 1;
}

static StarsI16 startup_create_shdef(void *opaque, StarsI16 ishdef,
    StarsI16 starbase_index, StarsI16 family, StarsAiStarbaseWork *work)
{
    TurinTrace *trace = startup_trace(opaque);
    StarsU16 i;
    if (!trace || ishdef != -1 || starbase_index < 0 || family < 0 ||
            (family != 0 && family != 12)) return 0;
    if (!stars_f_create_ai_shdef(-1, (StarsU16)starbase_index,
            original_sb_templates + family, startup_lookup_ai_hull,
            startup_lookup_ai_part, 0, startup_publish_build, trace)) {
        fprintf(stderr, "startup create failed family=%d hull=%d\n", family, starbase_index);
        return 0;
    }
    memset(work, 0, sizeof *work);
    work->hull_id = get_u16(trace->startup_build_shdef, 0U);
    work->flags = get_u16(trace->startup_build_shdef, 123U);
    work->bitmap_id = (StarsI16)get_u16(trace->startup_build_shdef, 50U);
    work->component_count = trace->startup_build_shdef[122];
    for (i = 0U; i < work->component_count; ++i) {
        work->groups[i] = get_u16(trace->startup_build_shdef, (StarsU16)(58U + i * 4U));
        work->tech_levels[i] = trace->startup_build_shdef[61U + i * 4U];
    }
    return 1;
}

static void startup_change_shdef(void *, StarsI16,
    const StarsAiStarbaseWork *);

static void startup_create_starbase(void *opaque, StarsI16 design,
    StarsI16 kind)
{
    StarsTurinScoutAdapter *scout = (StarsTurinScoutAdapter *)opaque;
    TurinTrace *trace = startup_trace(opaque);
    StarsAiStarbaseWork work;
    StarsPickAiNameOps name_ops;
    if (!trace) {
        return;
    }
    name_ops.random_value = startup_name_random;
    name_ops.get_name = startup_name_get;
    name_ops.bitmap_in_use = startup_bitmap_in_use;
    name_ops.name_in_use = startup_name_in_use;
    name_ops.format_name = 0;
    if (!stars_f_create_ai_starbase(design, kind, -1, -1,
            (StarsI16)(get_u16(trace->complete_input.views[0].players[1], 6U) >> 13), &work,
            startup_create_shdef, startup_change_shdef, &name_ops, scout))
        ++trace->failures;
}

static StarsU16 startup_old_design_flags(void *opaque, StarsI16 slot)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    return get_u16(trace->starbase_designs + slot * 147U, 123U);
}

static void startup_set_design_flags(void *opaque, StarsI16 slot, StarsU16 flags)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    put_u16(trace->starbase_designs + slot * 147U, 123U, flags);
    /* The pinned startup only creates free slots. Replacement logging needs
     * the deleted record as well as the new one. Fail if it becomes eligible. */
    ++trace->failures;
}

static void startup_change_design_count(void *opaque, StarsI16 player, StarsI16 delta)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsU8 *raw = trace->complete_input.views[0].players[player];
    StarsU16 flags = get_u16(raw, 4U);
    put_u16(raw, 4U, (StarsU16)((flags & 0x0fffU) |
        (((StarsU16)((flags >> 12) + delta) & 15U) << 12)));
}

static StarsI16 startup_cost_race(void *opaque, StarsI16 player, StarsI16 stat)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    if (player != 1 || stat < 0 || stat >= 16) { ++trace->failures; return 0; }
    return (StarsI16)(signed char)trace->complete_input.views[0].players[player][62 + stat];
}

static void startup_price_raw_design(void *opaque, StarsU8 *raw)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsShdefRepriceInput input;
    memset(&input, 0, sizeof input);
    input.part_segment = retained_part_segment;
    input.part_segment_size = sizeof retained_part_segment;
    input.player_technology = (const signed char *)trace->complete_input.views[0].players[1] + 26U;
    input.game_flags = &trace->game_flags;
    input.race_stat = startup_cost_race;
    input.race_grbit = startup_part_race_bit;
    input.context = trace;
    input.player = 1;
    if (!stars_initialize_shdef_cost(&input, raw, 147U, raw)) ++trace->failures;
}

static void startup_update_design_cost(void *opaque, StarsAiShdefWork *work)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    put_u16(trace->startup_build_shdef, 123U, work->flags);
    put_u16(trace->startup_build_shdef, 125U, work->turn);
    memset(trace->startup_build_shdef + 127U, 0, 8U);
    startup_price_raw_design(opaque, trace->startup_build_shdef);
}

static StarsI16 startup_compress_name(void *opaque, const StarsU8 *name,
    StarsU8 *out, StarsI16 *size)
{
    (void)opaque;
    return (StarsI16)stars_compress_user_string(original_dgroup, name, out, size);
}

static StarsU16 startup_write_design_payload(void *opaque, const StarsU8 *raw,
    StarsU8 *out, StarsU16 capacity)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsWriteShdefInput input;
    StarsWriteShdefOps ops;
    StarsU16 size = 0U;
    /* Original LogChangeShDef mutates the published record before writing. */
    put_u16(trace->startup_build_shdef, 123U,
        (StarsU16)((get_u16(raw, 123U) & 0xff00U) | 7U));
    memset(&input, 0, sizeof input); memset(&ops, 0, sizeof ops);
    input.flags = get_u16(raw, 123U); input.hull_id = get_u16(raw, 0U);
    input.bitmap = (StarsU8)get_u16(raw, 50U); input.component_count = raw[122];
    input.design_points = get_u16(raw, 56U); input.turn = get_u16(raw, 125U);
    input.built = get_u32(raw, 127U); input.existing = get_u32(raw, 131U);
    input.empty_weight = get_u16(raw, 40U); input.component_data = raw + 58U;
    input.component_data_size = (StarsU16)(raw[122] * 4U); input.name = raw + 8U;
    ops.compress_name = startup_compress_name;
    if (!stars_write_rt_shdef(&input, out, capacity, &size, &ops, trace))
        ++trace->failures;
    return size;
}

static void startup_capture_memory_log(void *opaque, StarsU16 type,
    StarsU16 size, const StarsU8 *record)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsU16 index = trace->startup_memory_count;
    if (index >= 16U || size > 160U) { ++trace->failures; return; }
    trace->startup_memory_types[index] = type;
    trace->startup_memory_sizes[index] = size;
    if (size) memcpy(trace->startup_memory_records[index], record, size);
    ++trace->startup_memory_count;
    if (trace->seeded_mode) turn_set_dirty(trace, 1);
}

static void startup_capture_design_log(void *opaque, StarsU16 type,
    StarsU16 size, const StarsU8 *record)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsU16 index = trace->startup_design_log_count;
    if (type != 27U || index >= 4U || size > 160U) { ++trace->failures; return; }
    memcpy(trace->startup_design_logs[index], record, size);
    trace->startup_design_log_sizes[index] = size;
    ++trace->startup_design_log_count;
    startup_capture_memory_log(trace, type, size, record);
}

static StarsI16 startup_decode_name(void *opaque, const StarsU8 *encoded,
    StarsU16 size, StarsU8 *out, StarsU16 *length)
{
    StarsI16 capacity = 32;
    (void)opaque;
    if (!stars_decompress_user_string(original_dgroup, encoded,
            (StarsI16)size, out, &capacity)) return 0;
    *length = (StarsU16)strlen((const char *)out);
    return 1;
}

static StarsI16 startup_read_part(void *opaque, StarsU16 group, StarsU16 item,
    StarsU8 *out, StarsU16 capacity)
{
    StarsAiPart part;
    if (capacity < 8U || !startup_lookup_part(opaque, group, item, &part)) return 0;
    memset(out, 0, capacity);
    memcpy(out + 4U, part.component + 40U, 2U);
    return 1;
}

static StarsI16 startup_apply_design_log(void *opaque,
    const StarsRunLogDesignChange *change)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsReadShdefOps ops;
    if (change->player != 1U || change->action != 1U ||
            change->slot < 16U || change->slot >= 26U) return 0;
    memset(&ops, 0, sizeof ops);
    ops.lookup_hull = scrap_read_hull; ops.lookup_part = startup_read_part;
    ops.update_cost = startup_price_raw_design; ops.decode_string = startup_decode_name;
    return stars_read_shdef(change->design_payload, change->design_payload_size,
        trace->starbase_designs, sizeof trace->starbase_designs, 1, &ops, trace);
}

static StarsI16 startup_apply_research_log(void *opaque,
    const StarsRunLogResearch *change)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    trace->complete_input.views[0].players[1][56] = change->research_percent;
    trace->complete_input.views[0].players[1][57] = change->technology;
    return 1;
}

static StarsI16 startup_apply_production_log(void *opaque,
    const StarsRunLogProduction *change)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsU16 i;
    if (change->planet_id >= TURIN_RETAINED_POINT_COUNT ||
            trace->turn_planet_states[change->planet_id].id != (StarsI16)change->planet_id ||
            trace->turn_planet_states[change->planet_id].player != 1 ||
            change->entry_count > 2U) return 0;
    trace->turn_committed_planet = (StarsI16)change->planet_id;
    for (i = 0U; i < change->entry_count; ++i) {
        StarsU32 packed = get_u32(change->entries, (StarsU16)(i * 4U));
        StarsProduceItem *item = &trace->turn_produce_queue[i];
        memset(item, 0, sizeof *item);
        item->type = (StarsI16)((packed >> 10) & 127UL);
        item->object_kind = (StarsI16)((packed >> 17) & 7UL);
        item->quantity = (StarsI16)(packed & 1023UL);
        item->progress = (StarsI16)((packed >> 20) & 127UL);
        item->active = 1;
    }
    trace->turn_committed_queue = change->entry_count ? trace->turn_produce_queue : 0;
    trace->turn_committed_queue_count = change->entry_count;
    return 1;
}

static StarsI16 startup_apply_balance_log(void *opaque,
    const StarsRunLogTransferBalance *change)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    TurinTrace *copy;
    StarsMergeFleetInput input;
    StarsU16 side, slot;
    StarsU8 initial[2][124];
    StarsU16 found = 0U;
    for (slot = 0U; slot < trace->complete_input.views[2].fleet_count; ++slot) {
        const Soe3InputFleet *fleet = &trace->complete_input.views[2].fleets[slot];
        StarsU16 id = (StarsU16)(fleet->id_word & 511U);
        if (fleet->owner == 1 && (id == 2U || id == 3U)) {
            pack_input_fleet(initial[id - 2U], fleet);
            found |= (StarsU16)(1U << (id - 2U));
        }
    }
    if (found != 3U) return 0;
    if (change->first_fleet != (StarsU16)(get_u16(initial[0], 0U) |
            (get_u16(initial[0], 2U) << 9)) ||
        change->second_fleet != (StarsU16)(get_u16(initial[1], 0U) |
            (get_u16(initial[1], 2U) << 9))) return 0;
    /* This retained record is a complete merge. A partial transfer needs its
     * own replay adapter rather than silently inheriting merge semantics. */
    for (slot = 0U; slot < 16U; ++slot)
        if (change->delta[slot] != (StarsI16)get_u16(initial[1],
                (StarsU16)(12U + slot * 2U))) return 0;
    copy = (TurinTrace *)malloc(sizeof *copy);
    if (!copy) return 0;
    *copy = *trace;
    for (slot = 0U; slot < TURIN_RETAINED_FLEET_COUNT; ++slot)
        copy->fleet_items[slot] = &copy->fleet_storage[slot];
    copy->fleet_count = TURIN_RETAINED_FLEET_COUNT;
    for (side = 0U; side < 2U; ++side) {
        memcpy(copy->raw_fleets[side + 2U], initial[side], 124U);
        memcpy(copy->fleet_storage[side + 2U].bytes, initial[side], 124U);
        copy->raw_fleet_present[side + 2U] = 1;
    }
    memset(&input, 0, sizeof input);
    input.destination = copy->raw_fleets[2]; input.deleted = copy->raw_fleets[3];
    input.cargo_balance = merge_cargo; input.lookup = lookup_merge_fleet;
    input.delete_fleet = delete_merge_fleet; input.invalidate_report = invalidate_merge_report;
    input.context = copy;
    stars_merge_two_fleets(&input);
    if (copy->failures != trace->failures) { free(copy); return 0; }
    for (side = 0U; side < 2U; ++side) {
        /* Restore the host's merged ship/damage/cargo state from pre-AI
         * inputs, retaining independently logged order changes. */
        memcpy(trace->raw_fleets[side + 2U] + 12U,
            copy->raw_fleets[side + 2U] + 12U, 84U);
        trace->raw_fleet_present[side + 2U] = copy->raw_fleet_present[side + 2U];
        memcpy(trace->fleet_storage[side + 2U].bytes,
            copy->fleet_storage[side + 2U].bytes, 124U);
        for (slot = 0U; slot < 16U; ++slot)
            trace->fleets[side + 2U].ship_counts[slot] = (StarsI16)get_u16(
                trace->raw_fleets[side + 2U], (StarsU16)(12U + slot * 2U));
        if (!trace->raw_fleet_present[side + 2U])
            trace->fleets[side + 2U].owner = -1;
    }
    trace->fleet_count = copy->fleet_count;
    for (slot = 0U; slot < trace->fleet_count; ++slot) {
        StarsU16 id = get_u16(copy->fleet_items[slot]->bytes, 0U);
        if (id >= TURIN_RETAINED_FLEET_COUNT) { free(copy); return 0; }
        trace->fleet_items[slot] = &trace->fleet_storage[id];
    }
    for (side = 0U; side < 2U; ++side) {
        StarsU16 id = (StarsU16)(side + 2U);
        StarsU16 found = 0U;
        StarsU16 world_index;
        const StarsU8 *raw = trace->raw_fleets[id];
        for (world_index = 0U;
                world_index < trace->complete_input.views[0].fleet_count;
                ++world_index) {
            Soe3InputFleet *world_fleet =
                &trace->complete_input.views[0].fleets[world_index];
            if (world_fleet->owner != 1 ||
                    (world_fleet->id_word & 511U) != id) continue;
            if (found++) { free(copy); return 0; }
            world_fleet->state.fleet_flags = (StarsU16)(get_u16(raw, 4U) |
                (trace->raw_fleet_present[id] ? 0U : 0x0400U));
            for (slot = 0U; slot < 32U; ++slot)
                world_fleet->state.ship_counts[slot] = get_u16(raw,
                    (StarsU16)(12U + slot * 2U));
            for (slot = 0U; slot < 5U; ++slot)
                world_fleet->state.minerals[slot] = get_u32(raw,
                    (StarsU16)(76U + slot * 4U));
        }
        if (found != 1U) { free(copy); return 0; }
    }
    free(copy);
    return 1;
}

static StarsI16 startup_apply_cargo_log(void *opaque, const void *payload)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    const StarsRunLogTransfer *change = (const StarsRunLogTransfer *)payload;
    TurinTrace *copy;
    StarsU16 lane, index;
    StarsI32 accepted;
    if (change->source_kind != 2U || change->target_kind != 1U ||
            change->source_id != 513U || change->target_id != 24U) return 0;
    for (lane = 0U; lane < 5U; ++lane)
        if (lane != 3U && change->delta[lane] != 0L) return 0;
    if (change->delta[3] <= 0L || trace->complete_input.views[2].fleet_count <= 1U ||
            trace->complete_input.views[2].planet_count == 0U ||
            trace->complete_input.views[2].fleets[1].id_word != change->source_id ||
            trace->complete_input.views[2].planets[0].id != change->target_id ||
            trace->complete_input.views[2].planets[0].player != 1) return 0;
    copy = (TurinTrace *)malloc(sizeof *copy);
    if (!copy) return 0;
    *copy = *trace;
    copy->supply_fleet.fleet_flags =
        trace->complete_input.views[2].fleets[1].state.fleet_flags;
    for (lane = 0U; lane < 5U; ++lane)
        copy->supply_fleet.minerals[lane] =
            trace->complete_input.views[2].fleets[1].state.minerals[lane];
    for (lane = 0U; lane < 4U; ++lane)
        copy->supply_planet.minerals[lane] =
            trace->complete_input.views[2].planets[0].minerals[lane];
    /* Original FRunLogRecord applies positive deltas to the first object,
     * then their negatives to the second object. Capacity/clamp failures
     * would require message/refund branches absent from this pinned pair. */
    accepted = change_supply(copy, 2, 1, 3, change->delta[3]);
    if (accepted != change->delta[3] ||
            change_supply(copy, 1, 24, 3, -accepted) != -accepted ||
            copy->failures != trace->failures) { free(copy); return 0; }
    trace->supply_fleet = copy->supply_fleet;
    trace->supply_planet = copy->supply_planet;
    memcpy(trace->raw_fleets[1] + 76U, copy->raw_fleets[1] + 76U, 20U);
    for (index = 0U; index < trace->complete_input.views[0].fleet_count; ++index) {
        Soe3InputFleet *fleet = &trace->complete_input.views[0].fleets[index];
        if (fleet->owner == 1 && (fleet->id_word & 511U) == 1U)
            for (lane = 0U; lane < 5U; ++lane)
                fleet->state.minerals[lane] = trace->supply_fleet.minerals[lane];
    }
    trace->turn_planet_states[24].minerals[3] = trace->supply_planet.minerals[3];
    free(copy);
    return 1;
}

static StarsI16 startup_apply_order_log(void *opaque, const StarsRunLogOrder *change)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsU16 id = (StarsU16)(change->fleet_id & 511U);
    StarsU8 *plord;
    StarsU16 count;
    if ((change->fleet_id >> 9) != 1U || id >= TURIN_RETAINED_FLEET_COUNT ||
            change->data_size > 18U) return 0;
    plord = trace->plord[id]; count = plord[3];
    if (change->operation == STARS_LOG_ORDER_INSERT) {
        if (count >= 2U || change->index > count) return 0;
        memmove(plord + 4U + (change->index + 1U) * 18U,
            plord + 4U + change->index * 18U, (count - change->index) * 18U);
        ++count;
    } else if (change->operation != STARS_LOG_ORDER_REPLACE || change->index >= count)
        return 0;
    memset(plord + 4U + change->index * 18U, 0, 18U);
    memcpy(plord + 4U + change->index * 18U, change->data, change->data_size);
    plord[3] = (StarsU8)count;
    trace->raw_fleets[id][98] = (StarsU8)count;
    return 1;
}

typedef struct StartupLogReplay {
    TurinTrace *trace;
    StarsU8 *data;
    StarsRunLogRecordOps ops;
} StartupLogReplay;

static StarsU16 startup_dispatch_memory_record(void *opaque, StarsU16 type,
    StarsU16 size, const StarsU8 *body)
{
    StartupLogReplay *replay = (StartupLogReplay *)opaque;
    TurinTrace *trace = replay->trace;
    StarsRunLogRecordInput input;
    memset(&input, 0, sizeof input);
    input.player = 1U; input.player_count = get_u16(trace->complete_input.game, 8U);
    input.game_flags = trace->game_flags;
    input.record_type = type; input.payload = body; input.payload_size = size;
    if (get_u16(replay->data, 0x09a8U) != 1U ||
            (type != 1U && type != 27U && type != 34U && type != 29U &&
             type != 23U && type != 4U && type != 5U) ||
            !stars_run_log_record(&input, &replay->ops, trace)) {
        ++trace->failures; return 0U;
    }
    ++trace->startup_memory_applied;
    return 1U;
}

typedef struct TurnLogRead {
    TurinTrace *trace;
    StarsU16 cursor;
} TurnLogRead;

static void turn_log_read_transfer(void *opaque, StarsU8 *out, StarsU16 count)
{
    TurnLogRead *reader = (TurnLogRead *)opaque;
    if ((StarsU32)reader->cursor + count > reader->trace->generated_log_size) {
        ++reader->trace->failures; memset(out, 0, count); return;
    }
    if (count) memcpy(out, reader->trace->generated_log_file + reader->cursor, count);
    reader->cursor = (StarsU16)(reader->cursor + count);
}

static StarsI16 turn_log_accept_header(void *opaque, StarsU16 version, StarsU16 flags)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsU8 *player = trace->complete_input.views[0].players[1];
    if (version != 0x2a60U || (flags & 255U) != 1U) return 0;
    /* FLoadLogFile, NE 10:C7A2: host load publishes the BOF bit to
     * gd.grBits high word and the current player's flags. */
    trace->log_file_version = version;
    trace->log_view_flags_high = (StarsU16)((trace->log_view_flags_high & 0xfffbU) |
        (((flags >> 12) & 1U) << 2));
    put_u16(player, 84U, (StarsU16)((get_u16(player, 84U) & 0xfffdU) |
        (((flags >> 12) & 1U) << 1)));
    return 1;
}

static StarsI16 turn_log_accept_serial(void *opaque, const StarsLoadGameRecord *record)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    if (record->length != 17U || get_u32(record->data, 2U) != trace->replay_serial ||
            memcmp(record->data + 6U, trace->replay_environment, 11U) != 0) return 0;
    /* Host generation allocates one 16-byte TURNSERIAL per player. */
    memset(trace->turn_serials[1], 0, 16U);
    memcpy(trace->turn_serials[1], record->data + 2U, 15U);
    return 1;
}

static StarsI16 turn_load_generated_log(TurinTrace *trace)
{
    StarsLoadGameRecord records[18];
    StarsU8 bodies[18][160];
    StarsU8 *data;
    StarsLoadLogFileInput input;
    StarsLoadLogFileState state;
    TurnLogRead reader;
    StarsU16 count = 0U, header, size;
    StarsI16 result;
    if (!trace->generated_log_closed || !trace->runtime_log_present[1]) return 0;
    data = (StarsU8 *)calloc(65536U, 1U);
    if (!data) return 0;
    memset(&input, 0, sizeof input); memset(&reader, 0, sizeof reader);
    reader.trace = trace;
    while (reader.cursor < trace->generated_log_size) {
        if (count >= 18U || (StarsU32)reader.cursor + 2U > trace->generated_log_size) {
            free(data); return 0;
        }
        header = get_u16(trace->generated_log_file, reader.cursor); size = header & 1023U;
        if (size > 160U || (StarsU32)reader.cursor + size + 2U > trace->generated_log_size) {
            free(data); return 0;
        }
        stars_read_record(data, turn_log_read_transfer, &reader);
        records[count].type = (StarsU16)(header >> 10); records[count].length = size;
        memcpy(bodies[count], data + 0x4b98U, size); records[count].data = bodies[count];
        ++count;
    }
    free(data);
    if (count < 3U || records[0].type != 8U || records[count - 1U].type != 0U) return 0;
    input.records = records; input.record_count = count;
    input.game_id = get_u32(trace->complete_input.game, 0U);
    input.game_turn = get_u16(trace->complete_input.bof, 10U);
    input.game_turn_flags = (StarsU16)(((get_u16(trace->complete_input.bof, 14U) >> 13) & 7U) << 9);
    input.log = trace->saved_mutation_stream; input.log_capacity = sizeof trace->saved_mutation_stream;
    input.accept_header = turn_log_accept_header; input.serial = turn_log_accept_serial;
    input.dirty_game = turn_log_clean;
    input.context = trace;
    result = stars_load_log_file(&input, &state);
    if (!result || state.log_length != trace->saved_mutation_size || state.messages != 0U) return 0;
    ++trace->generated_log_loads;
    return 1;
}

static StarsI16 startup_replay_memory_logs(TurinTrace *trace)
{
    StartupLogReplay replay;
    StarsU8 data[0x09aaU];
    StarsU16 i, saved_log_off;
    StarsI16 result;
    if (!turn_load_generated_log(trace)) return 0;
    memset(&replay, 0, sizeof replay); memset(data, 0, sizeof data);
    replay.trace = trace; replay.data = data;
    replay.ops.design = startup_apply_design_log;
    replay.ops.research = startup_apply_research_log;
    replay.ops.production = startup_apply_production_log;
    replay.ops.transfer_balance = startup_apply_balance_log;
    replay.ops.order = startup_apply_order_log;
    replay.ops.transfer = startup_apply_cargo_log;
    for (i = 0U; i < trace->complete_input.views[2].fleet_count; ++i) {
        const Soe3InputFleet *original = &trace->complete_input.views[2].fleets[i];
        StarsU16 id = (StarsU16)(original->id_word & 511U);
        if (id >= TURIN_RETAINED_FLEET_COUNT || original->state.order_count > 2U) return 0;
        memset(trace->plord[id] + 4U, 0, 36U);
        memcpy(trace->plord[id] + 4U, original->state.orders, original->state.order_count * 18U);
        trace->plord[id][3] = (StarsU8)original->state.order_count;
        trace->raw_fleets[id][98] = (StarsU8)original->state.order_count;
    }

    if (!trace->runtime_log_present[1] || !trace->saved_mutation_size ||
            trace->saved_mutation_size > sizeof trace->saved_mutation_stream) return 0;
    put_u16(data, 0x09a4U, trace->saved_mutation_size);
    saved_log_off = trace->startup_log_off;
    put_u16(data, 0x09a8U, saved_log_off);
    put_u16(data, 0x07ceU, trace->startup_log_flags);
    result = stars_run_log_file(data, trace->saved_mutation_stream, startup_dispatch_memory_record, &replay);
    trace->startup_log_off = get_u16(data, 0x09a8U);
    trace->startup_log_flags = get_u16(data, 0x07ceU);
    if (trace->startup_log_off != saved_log_off) {
        ++trace->failures; return 0;
    }
    return result;
}

static void startup_log_design_change(void *opaque, StarsI16 slot,
    const StarsAiShdefWork *work)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsLogChangeShdefInput input;
    StarsLogChangeShdefOps ops;
    (void)work;
    if (slot < 0 || slot >= 10 || trace->startup_design_change_count >= 4U) {
        ++trace->failures; return;
    }
    memset(&input, 0, sizeof input); memset(&ops, 0, sizeof ops);
    input.game_flags = trace->game_flags; input.player = 1;
    input.design_flags = get_u16(trace->startup_build_shdef, 123U);
    input.design_payload = trace->startup_build_shdef; input.design_payload_size = 147U;
    ops.write_design = startup_write_design_payload; ops.write_memory = startup_capture_design_log;
    if (!stars_log_change_shdef(&input, &ops, trace)) ++trace->failures;
    memcpy(trace->starbase_designs + slot * 147U, trace->startup_build_shdef, 147U);
    memcpy(trace->startup_design_changes[trace->startup_design_change_count++],
        trace->startup_build_shdef, 147U);
}

static void startup_change_shdef(void *opaque, StarsI16 design,
    const StarsAiStarbaseWork *work)
{
    TurinTrace *trace = startup_trace(opaque);
    StarsU16 slot;
    StarsI16 index;
    StarsAiShdefWork changed;
    StarsChangeAiShdefOps ops;
    if (!trace || trace->startup_name_count >= 4) {
        if (trace)
            ++trace->failures;
        return;
    }
    index = trace->startup_name_count;
    trace->startup_name_designs[index] = design;
    trace->startup_name_bitmaps[index] = work->bitmap_id;
    trace->startup_name_ids[index] = trace->startup_last_name_id;
    memcpy(trace->startup_name_text[index], work->name, 32U);
    trace->startup_name_hulls[index] = work->hull_id;
    for (slot = 0U; slot < work->component_count; ++slot) {
        StarsU8 *part = trace->startup_created_components[index] + slot * 4U;
        put_u16(part, 0U, work->groups[slot]);
        part[2] = trace->startup_build_shdef[60U + slot * 4U];
        part[3] = (StarsU8)work->tech_levels[slot];
    }
    memcpy(trace->startup_build_shdef + 58U,
        trace->startup_created_components[index], work->component_count * 4U);
    memcpy(trace->startup_build_shdef + 8U, work->name, 32U);
    put_u16(trace->startup_build_shdef, 50U, (StarsU16)work->bitmap_id);
    memset(&changed, 0, sizeof changed);
    changed.flags = work->flags;
    memset(&ops, 0, sizeof ops);
    ops.old_flags = startup_old_design_flags;
    ops.set_flags = startup_set_design_flags;
    ops.change_count = startup_change_design_count;
    ops.update_cost = startup_update_design_cost;
    ops.log_change = startup_log_design_change;
    ops.player_design_count = 1;
    if (!stars_f_change_ai_shdef(&changed, design,
            get_u16(trace->complete_input.game, 18U), &ops, trace)) ++trace->failures;
    ++trace->startup_name_count;
}

static void startup_read_starbase_state(TurinTrace *trace)
{
    StarsU16 slot;
    static const StarsU16 required_slots[3] = {2U, 1U, 3U};
    for (slot = 0U; slot < 3U; ++slot)
        trace->startup_capabilities[slot] = (StarsU8)((get_u16(
            trace->starbase_designs + required_slots[slot] * 147U, 123U) >> 9) & 1U);
    for (slot = 0U; slot < 10U; ++slot) {
        const StarsU8 *raw = trace->starbase_designs + slot * 147U;
        StarsAiStarbaseDesign *design = &trace->startup_starbase_designs[slot];
        design->active = (StarsU8)((get_u16(raw, 123U) >> 9) & 1U);
        design->age = get_u16(raw, 125U);
        design->cost_a = get_u16(raw, 131U);
        design->cost_b = get_u16(raw, 133U);
    }
}

static void startup_ensure_designs(void *opaque)
{
    StarsTurinScoutAdapter *scout = (StarsTurinScoutAdapter *)opaque;
    TurinTrace *trace = startup_trace(opaque);
    if (!trace)
        return;
    startup_read_starbase_state(trace);
    stars_ensure_ai_starbase_designs((StarsI16)get_u16(trace->complete_input.game, 18U),
        trace->startup_capabilities, trace->startup_starbase_designs,
        startup_create_starbase, scout);
}

static StarsI16 startup_latest_starbase(void *opaque)
{
    TurinTrace *trace = startup_trace(opaque);
    if (!trace || !trace->current_designs_loaded) return -1;
    return stars_ishdef_ai_sb_latest(trace->starbase_designs);
}

static void startup_validate_history(void *opaque)
{
    TurinTrace *trace = startup_trace(opaque);
    StarsU16 mode;
    if (!trace) return;
    ++trace->startup_history_calls;
    mode = (StarsU16)(get_u16(trace->complete_input.views[0].players[1], 6U) >> 13);
    if (mode == 4U || mode == 5U) return;
    if (trace->ai_data[0] < 3U)
        put_u16(trace->ai_data, 2U, 0U);
    /* Original ValidateStarbaseHistory enters its planet/transport walk only
     * after turn 19. Do not silently approximate that path on another input. */
    if (get_u16(trace->complete_input.game, 18U) > 19U)
        ++trace->failures;
}

static void startup_write_research(void *opaque, StarsU8 current,
    StarsI16 percent)
{
    TurinTrace *trace = startup_trace(opaque);
    StarsU8 record[2];
    if (!trace)
        return;
    ++trace->startup_research_writes;
    trace->startup_written_current_tech = current;
    trace->startup_written_percent = percent;
    trace->complete_input.views[0].players[1][56] = (StarsU8)percent;
    trace->complete_input.views[0].players[1][57] = current;
    record[0] = (StarsU8)percent; record[1] = current;
    startup_capture_memory_log(trace, 34U, 2U, record);
}

static StarsI16 startup_ensure_ai(void *opaque, StarsI16 percent,
    StarsI16 *latest_starbase)
{
    TurinTrace *trace = startup_trace(opaque);
    StarsIroAiOps iro_ops;
    StarsU8 requirements[TURIN_RETAINED_RESEARCH_REQUIREMENTS_COUNT];
    StarsI16 index;
    if (!trace)
        return (StarsI16)-1;
    ++trace->startup_ensure_calls;
    memset(&trace->startup_research, 0, sizeof(trace->startup_research));
    for (index = 0; index < 6; ++index)
        trace->startup_research.tech[index] =
            trace->complete_input.views[0].players[1][26U + index];
    trace->startup_research.current_tech =
        trace->complete_input.views[0].players[1][57];
    trace->startup_research.percent_research =
        trace->complete_input.views[0].players[1][56];
    memcpy(requirements, retained_turin_research_requirements,
        sizeof(requirements));
    iro_ops.ensure_designs = startup_ensure_designs;
    iro_ops.latest_starbase = startup_latest_starbase;
    iro_ops.validate_history = startup_validate_history;
    iro_ops.write_research = startup_write_research;
    trace->startup_research_order = stars_iro_ensure_ai(requirements,
        TURIN_RETAINED_RESEARCH_REQUIREMENTS_COUNT, latest_starbase,
        percent, &trace->startup_research, &iro_ops, opaque);
    if (latest_starbase)
        trace->startup_latest_design = *latest_starbase;
    return trace->startup_research_order;
}

static void startup_unexpected_recycle(void *opaque, StarsI16 design)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    (void)design;
    ++trace->failures;
}

static StarsI16 startup_check_design_status(void *opaque, StarsI16 first,
    StarsI16 last, StarsI16 period, StarsI16 *latest, StarsU8 *old_designs)
{
    StarsTurinScoutAdapter *scout = (StarsTurinScoutAdapter *)opaque;
    TurinTrace *trace = (TurinTrace *)scout->context;
    StarsAiShdefStatus designs[16];
    StarsU16 i;
    StarsI16 failures = trace->failures;
    if (first < 0 || last >= 16 || first > last) { ++trace->failures; return 0; }
    memset(designs, 0, sizeof designs);
    for (i = 0U; i < 16U; ++i) {
        const StarsU8 *raw = trace->designs + i * 147U;
        designs[i].active = (StarsU8)((get_u16(raw, 123U) >> 9) & 1U);
        designs[i].existing = get_u32(raw, 131U);
        designs[i].turn = get_u16(raw, 125U);
    }
    i = (StarsU16)stars_check_ai_shdef_status(first, last, (StarsU16)period, latest,
        old_designs, (StarsI16)get_u16(trace->complete_input.game, 18U), designs,
        startup_unexpected_recycle, trace);
    if (first == 8 && last == 9)
        trace->turin_input.design_8_9_existing_total = (StarsI16)i;
    ++trace->startup_design_status_calls;
    return trace->failures == failures;
}

static StarsI16 startup_unexpected_ship_create(void *opaque, StarsI16 slot,
    StarsU16 hull, StarsI16 name)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    (void)slot; (void)hull; (void)name; ++trace->failures; return 0;
}

static void startup_unexpected_ship_change(void *opaque, StarsI16 slot,
    const StarsTurinDroneShdef *design)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    (void)slot; (void)design; ++trace->failures;
}

static StarsI16 startup_ensure_ship_designs(void *opaque, StarsI16 research)
{
    StarsTurinScoutAdapter *scout = (StarsTurinScoutAdapter *)opaque;
    TurinTrace *trace = (TurinTrace *)scout->context;
    StarsEnsureTurinDroneShdefsInput input;
    StarsTurinDroneShdef designs[16];
    StarsU16 i;
    StarsI16 failures = trace->failures;
    (void)research;
    memset(&input, 0, sizeof input);
    for (i = 0U; i < 16U; ++i) {
        const StarsU8 *raw = trace->designs + i * 147U;
        StarsU32 existing = get_u32(raw, 131U);
        if (existing > 65535UL) { ++trace->failures; return 0; }
        designs[i].active = get_u16(raw, 123U);
        designs[i].existing = (StarsU16)existing;
        designs[i].hull_id = get_u16(raw, 0U);
    }
    input.designs = designs; input.design_count = 16U;
    memcpy(input.tech, trace->complete_input.views[0].players[1] + 26U, 6U);
    input.random_value = startup_prepare_random;
    input.create_shdef = startup_unexpected_ship_create;
    input.change_shdef = startup_unexpected_ship_change;
    input.context = trace;
    stars_ensure_turin_drone_shdefs(&input);
    ++trace->startup_ship_maintenance_calls;
    return trace->failures == failures;
}

static StarsI16 startup_queue_upgrade(void *opaque, StarsI16 design)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    (void)design;
    if (trace)
        ++trace->failures;
    return 0;
}

static void startup_select_current_planet(TurinTrace *, StarsI16);
static void startup_select_current_object(TurinTrace *, StarsI16, StarsI16);
static void startup_keep_speed(void *, const StarsU8 *, StarsI16, StarsI16,
    const StarsU8 *const *);
static StarsI16 startup_basic_begin(TurinTrace *, StarsI16);
static void startup_basic_finish(TurinTrace *);
static void startup_blocked_queue(TurinTrace *);
static void startup_basic_packet_scanner(TurinTrace *);
static void startup_basic_terraform(TurinTrace *);

static StarsI16 startup_unexpected_defense_lookup(void *opaque, StarsI16 type)
{
    (void)type; ++((TurinTrace *)opaque)->failures; return 0;
}
static void startup_unexpected_defense_add(void *opaque, StarsI16 type,
    StarsI16 quantity)
{
    (void)type; (void)quantity; ++((TurinTrace *)opaque)->failures;
}

static StarsI16 startup_keep_unexpected_transport(void *opaque, const StarsU8 *fleet)
{
    (void)fleet; ++((TurinTrace *)opaque)->failures; return 0;
}
static void startup_basic_prelude(TurinTrace *trace, StarsI16 latest_starbase)
{
    StarsKeepFleetsMovingOps ops;
    StarsU8 fleet_bytes[TURIN_RETAINED_FLEET_COUNT][124];
    const StarsU8 *fleets[TURIN_RETAINED_FLEET_COUNT];
    const StarsU8 *things[32];
    const Soe3InputView *view = &trace->complete_input.views[2];
    StarsU16 i, id, mode, fleet_count = 0;
    StarsI16 before = trace->failures;
    StarsU16 turn = get_u16(trace->complete_input.game, 18U);
    if (trace->basic_keep_calls || view->thing_count > 32U ||
            trace->fleet_count > TURIN_RETAINED_FLEET_COUNT) {
        fprintf(stderr, "prelude guard line=%d\n", __LINE__); ++trace->failures; return;
    }
    for (i = 0; i < view->thing_count; ++i) things[i] = view->things[i];
    for (i = 0; i < trace->fleet_count; ++i) {
        id = get_u16(trace->fleet_items[i]->bytes, 0U);
        if (id >= TURIN_RETAINED_FLEET_COUNT || !trace->raw_fleet_present[id]) {
            fprintf(stderr, "prelude guard line=%d\n", __LINE__); ++trace->failures; return;
        }
        memcpy(fleet_bytes[fleet_count], trace->raw_fleets[id], 124U);
        /* AI order mutation owns the current packed order list. */
        put_u16(fleet_bytes[fleet_count], 98U, trace->plord[id][3]);
        fleets[fleet_count] = fleet_bytes[fleet_count];
        ++fleet_count;
    }
    ops.is_transport = startup_keep_unexpected_transport;
    ops.set_speed = startup_keep_speed;
    stars_keep_fleets_moving(things, view->thing_count, 1, (StarsI16)turn,
        fleets, fleet_count, &ops, trace);
    trace->basic_keep_fleets = fleet_count;
    ++trace->basic_keep_calls;

    /* QueueAiStarbases normal-personality loop: existing valid bases skip
     * before history, selection, allocation or queue mutation. */
    mode = (StarsU16)(get_u16(trace->complete_input.views[0].players[1], 6U) >> 13);
    if (mode == 4U || latest_starbase < 0 || mode == 5U) {
        fprintf(stderr, "prelude guard line=%d\n", __LINE__); ++trace->failures; return;
    }
    for (i = 0; i < view->planet_count; ++i) {
        const StarsReadPlanetState *planet = &view->planets[i];
        ++trace->basic_starbase_visits;
        if (planet->player == 1 && ((planet->flags & 0x0200U) == 0U ||
                (planet->opaque_44 & 15UL) > 9UL)) ++trace->failures;
    }
    ++trace->basic_starbase_calls;
    if (trace->failures != before) fprintf(stderr, "prelude delta=%d fleets=%u planets=%u mode=%u\n", trace->failures - before, fleet_count, view->planet_count, mode);
}
static void startup_basic_attack_gate(TurinTrace *trace)
{
    StarsU16 flags = get_u16(trace->complete_input.game, 16U);
    StarsU16 turn = get_u16(trace->complete_input.game, 18U);
    StarsU16 size = get_u16(trace->complete_input.game, 4U);
    StarsU16 minimum = (StarsU16)(size * 10U + 20U);
    if ((flags & 8U) == 0U && turn >= minimum)
        ++trace->failures;
    ++trace->basic_attack_gate_calls;
}

static StarsI16 startup_handle_tasks(void *opaque, StarsI16 research_order,
    StarsI16 latest_starbase)
{
    TurinTrace *trace = startup_trace(opaque);
    StarsAiUpgradeStarbase starbase;
    StarsUpgradeStarbaseOps upgrade_ops;
    const StarsReadPlanetState *planet;
    StarsU16 slot, mineral, mode;
    if (!trace)
        return 0;
    if (research_order != TURIN_RETAINED_RESEARCH_ORDER ||
            latest_starbase != TURIN_RETAINED_LATEST_STARBASE_DESIGN)
        ++trace->failures;
    if (trace->complete_input.views[2].planet_count != 1U) {
        ++trace->failures; return 0;
    }
    startup_basic_prelude(trace, latest_starbase);
    planet = &trace->complete_input.views[2].planets[0];
    mode = (StarsU16)(get_u16(trace->complete_input.views[0].players[1], 6U) >> 13);
    if (latest_starbase < 0 || !(planet->flags & 0x0200U)) return 1;
    if (mode == 4U && get_u16(trace->complete_input.game, 18U) < 40U) return 1;
    slot = (StarsU16)(planet->opaque_44 & 15U);
    /* Bind the original normal-personality in-family upgrade branch. Other
     * eligible families/recycling paths require their own source adapter. */
    if (mode == 5U || planet->player != 1 || slot != 0U ||
            latest_starbase != 0) {
        ++trace->failures; return 0;
    }
    for (mineral = 0U; mineral < trace->turn_committed_queue_count; ++mineral)
        if (trace->turn_committed_queue[mineral].object_kind == 2 &&
                trace->turn_committed_queue[mineral].type > 15)
            return 1;
    if (get_u16(trace->starbase_designs + (slot + 2U) * 147U, 123U) & 0x0200U)
        return 1;
    memset(&starbase, 0, sizeof starbase);
    starbase.family = slot;
    for (mineral = 0U; mineral < 3U; ++mineral)
        starbase.resource_values[mineral] = (StarsU16)(planet->minerals[mineral] > 65535UL ?
            65535U : planet->minerals[mineral]);
    upgrade_ops.random_value = random_value;
    upgrade_ops.queue_upgrade = startup_queue_upgrade;
    if (!startup_basic_begin(trace, planet->id)) return 0;
    ++trace->startup_upgrade_calls;
    if (stars_f_upgrade_ai_starbase(&starbase, latest_starbase,
            (StarsI16)get_u16(trace->complete_input.game, 18U), mode,
            &upgrade_ops, trace) != 0)
        ++trace->failures;
    startup_basic_packet_scanner(trace);
    /* Current population proves the source defense routine exits before
     * consulting either queue. Catalog arguments are unreachable here. */
    if (trace->supply_planet.minerals[3] >= 1600UL) {
        ++trace->failures; return 0;
    }
    if (stars_f_queue_ai_defenses((StarsI32)trace->supply_planet.minerals[3],
            planet->opaque_20[1] & 4095UL, 0U, 9, 9,
            startup_unexpected_defense_lookup, startup_unexpected_defense_add,
            trace) != 0) ++trace->failures;
    ++trace->startup_defense_gate_calls;
    startup_basic_terraform(trace);
    startup_basic_finish(trace);
    startup_basic_attack_gate(trace);
    startup_blocked_queue(trace);
    return 1;
}

static StarsU32 replay_startup_tick_count(void *opaque)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    if (trace->bof_writer_calls < 0 || trace->bof_writer_calls >= 2) {
        ++trace->failures; return 0UL;
    }
    return replay_startup_ticks[trace->seeded_action][trace->bof_writer_calls];
}

static StarsU32 retained_output_tick_count(void *opaque)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    if (trace->output_bof_index < 0 || trace->output_bof_index >= 3) {
        ++trace->failures;
        return 0UL;
    }
    return trace->output_bof_tick_counts[trace->output_bof_index];
}

static void capture_bof_write(void *opaque, const StarsU8 *bytes,
    StarsU16 count)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsI16 index = trace->bof_writer_calls;
    StarsI16 current_size;
    if (index < 0 || index >= 5) {
        ++trace->failures;
        return;
    }
    current_size = trace->bof_write_sizes[index];
    if ((StarsU16)current_size + count > 18U) {
        ++trace->failures;
        return;
    }
    memcpy(trace->bof_write_records[index] + current_size, bytes, count);
    trace->bof_write_sizes[index] = (StarsI16)(current_size + count);
}

static StarsI16 retained_guess_random(void *opaque, StarsI16 limit)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsI16 index = trace->guess_random_calls;
    StarsI16 expected_limit;
    StarsI16 value;
    if (index < 0 || index >= 2) {
        ++trace->failures;
        return 0;
    }
    expected_limit = trace->seeded_action ?
        retained_guess_limits_action[index] :
        retained_guess_limits_control[index];
    if (limit != expected_limit)
        ++trace->failures;
    trace->guess_random_limits[index] = limit;
    value = turn_main_random(trace, limit);
    trace->guess_random_values[index] = value;
    ++trace->guess_random_calls;
    return value;
}

static StarsI16 retained_guess_race_stat(void *opaque, StarsI16 owner,
    StarsI16 stat_index)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    ++trace->guess_race_calls;
    if (stat_index != 0x000e || owner < 0 || owner > 1) {
        ++trace->failures;
        return 0;
    }
    return (StarsI16)(signed char)trace->complete_input.views[0].players[owner][76];
}

static StarsU32 retained_defense_resolve(void *opaque, StarsU16 group,
    StarsU16 item)
{
    (void)opaque;
    return ((StarsU32)group << 16) | item;
}

static StarsI16 retained_defense_race_stat(void *opaque, StarsI16 player,
    StarsI16 stat)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    if ((player != 0 && player != 1) || stat != 0x0e)
        return 0;
    return (StarsI16)(signed char)trace->complete_input.views[0].players[player][76];
}

static StarsI16 retained_defense_tech_status(void *opaque,
    StarsU32 component)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsLookupPartInput lookup;
    const StarsU8 *record;
    StarsI16 current[6];
    StarsI16 required[6];
    StarsU16 index;
    memset(&lookup, 0, sizeof lookup);
    lookup.group = (StarsU16)(component >> 16);
    lookup.item = (StarsU16)component;
    lookup.player = -1;
    if (stars_lookup_part(&lookup) < 0)
        ++trace->failures;
    record = stars_lookup_part_record(&lookup, retained_part_segment,
        (StarsU16)sizeof retained_part_segment);
    if (!record) {
        ++trace->failures;
        return 0;
    }
    for (index = 0; index < 6U; ++index) {
        current[index] = (StarsI16)
            trace->complete_input.views[0].players[trace->guess_current_owner][26U + index];
        required[index] = (StarsI16)(signed char)record[2U + index];
    }
    return stars_tech_status(current, required,
        trace->guess_current_owner == 1 ? trace->startup_written_current_tech :
            trace->complete_input.views[0].players[trace->guess_current_owner][57]);
}

static StarsI16 retained_defense_lookup(void *opaque, StarsU8 *part)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsLookupPartInput lookup;
    memset(&lookup, 0, sizeof lookup);
    lookup.group = get_u16(part, 0U);
    lookup.item = part[2];
    lookup.player = trace->guess_current_owner;
    lookup.resolve = retained_defense_resolve;
    lookup.race_stat = retained_defense_race_stat;
    lookup.tech_status = retained_defense_tech_status;
    lookup.context = trace;
    return stars_lookup_part(&lookup);
}

static StarsI16 retained_defense_player_stat(void *opaque, StarsI16 player,
    StarsI16 stat)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    if ((player != 0 && player != 1) || stat != 0x0e)
        return 0;
    return (StarsI16)(signed char)trace->complete_input.views[0].players[player][76];
}

static StarsI16 retained_defense_desirability(void *opaque,
    const StarsU8 *planet, StarsI16 player)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    if (!trace->guess_race_data || !planet ||
            player != trace->guess_current_owner) {
        ++trace->failures;
        return 0;
    }
    memcpy(trace->guess_race_data + 12U, planet + 12U, 3U);
    memcpy(trace->guess_race_data + 0x59a2U + (StarsU16)player * 192U,
        trace->complete_input.views[0].players[player], 192U);
    return stars_planet_desirability(trace->guess_race_data, 0U, player);
}

static StarsI16 retained_guess_calc_survive(void *opaque,
    const StarsGuessPlanet *planet)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsI16 index = trace->guess_survival_calls;
    StarsU8 part[8];
    const StarsU8 *defense_record;
    StarsCMaxOperableDefensesOps ops;
    StarsI16 found;
    StarsI16 damage;
    StarsI16 operable;
    float ordinary;
    float smart;
    if (index >= 2 || !planet || planet->survival_flags != 10U ||
            (planet->owner != 0 && planet->owner != 1)) {
        ++trace->failures;
        return 0;
    }
    ++trace->guess_survival_calls;
    trace->guess_current_owner = planet->owner;
    memset(part, 0, sizeof part);
    found = stars_get_best_defense_part(part, retained_defense_lookup, trace);
    if (!found || get_u16(part, 0U) != 0x8000U || part[2] != 9U)
        ++trace->failures;
    trace->guess_survival_part_items[index] = part[2];
    defense_record = retained_part_segment + 0x16b8U +
        (StarsU16)part[2] * 0x36U;
    damage = (StarsI16)get_u32(defense_record, 52U);
    memset(&ops, 0, sizeof ops);
    ops.max_defenses.planet_desirability = retained_defense_desirability;
    ops.max_defenses.get_player_stat = retained_defense_player_stat;
    operable = stars_cmax_operable_defenses(
        trace->guess_runtime_planets[(StarsU16)(planet - trace->runtime_guess_planets)],
        planet->owner, 0, &ops, trace);
    stars_calc_pct_survive(planet->owner, planet->survival_flags,
        (StarsU16)operable, damage, &ordinary, &smart);
    trace->guess_survival_damage[index] = damage;
    trace->guess_survival_operable[index] = operable;
    trace->guess_survival_percent[index] = (StarsI16)(ordinary * 100.0f);
    if (trace->guess_survival_part_items[index] != 9 || damage != 10 ||
            operable != 10 || trace->guess_survival_percent[index] != 90)
        fprintf(stderr, "survive owner=%d item=%d damage=%d operable=%d pct=%d race=%d tech=%d\n",
            planet->owner, part[2], damage, operable,
            trace->guess_survival_percent[index],
            retained_guess_race_stats[planet->owner],
            retained_guess_player_current_tech[planet->owner]);
    return trace->guess_survival_percent[index];
}

static void run_retained_update_guesses(TurinTrace *trace)
{
    StarsUpdateGuessesInput input;
    const StarsU16 *expected_words = trace->seeded_action ?
        retained_guess_words_action : retained_guess_words_control;
    StarsU16 index;
    for (index = 0U; index < TURIN_RETAINED_POINT_COUNT; ++index) {
        const StarsReadPlanetState *planet = &trace->turn_planet_states[index];
        trace->runtime_guess_planets[index].owner = planet->player;
        trace->runtime_guess_planets[index].guess_word = planet->routing_word;
        trace->runtime_guess_planets[index].mineral_value = planet->minerals[3];
        trace->runtime_guess_planets[index].survival_flags =
            (StarsU16)(planet->opaque_20[1] & 0xffffU);
        pack_loaded_planet(trace->guess_runtime_planets[index], planet);
    }
    memset(&input, 0, sizeof input);
    input.planets = trace->runtime_guess_planets;
    input.planet_count = TURIN_RETAINED_POINT_COUNT;
    input.random = retained_guess_random;
    input.race_stat = retained_guess_race_stat;
    input.calc_pct_survive = retained_guess_calc_survive;
    input.context = trace;
    stars_update_guesses(&input);
    for (index = 0U; index < TURIN_RETAINED_POINT_COUNT; ++index)
        if (trace->turn_planet_states[index].minerals[3] == 0UL &&
                trace->runtime_guess_planets[index].guess_word != 0U)
            ++trace->failures;
    if (trace->guess_random_calls != 2 || trace->guess_race_calls != 2 ||
            trace->guess_survival_calls != 2) {
        fprintf(stderr, "guess calls random=%d race=%d survival=%d\n",
            trace->guess_random_calls, trace->guess_race_calls,
            trace->guess_survival_calls);
        ++trace->failures;
    }
    for (index = 0; index < 2; ++index)
        if (trace->guess_survival_part_items[index] != 9 ||
                trace->guess_survival_damage[index] != 10 ||
                trace->guess_survival_operable[index] !=
                    (index == 0 ? 12 : 11) ||
                trace->guess_survival_percent[index] != 90)
            ++trace->failures;
    for (index = 0; index < 2; ++index) {
        if (trace->runtime_guess_planets[index == 0U ? 10U : 24U].guess_word !=
                expected_words[index] ||
                trace->guess_random_limits[index] !=
                    (trace->seeded_action ?
                        retained_guess_limits_action[index] :
                        retained_guess_limits_control[index]) ||
                trace->guess_random_values[index] !=
                    (trace->seeded_action ?
                        retained_guess_values_action[index] :
                        retained_guess_values_control[index])) {
            fprintf(stderr, "guess[%u] word=%04x expected=%04x limit=%d value=%d\n",
                (unsigned)index,
                (unsigned)trace->runtime_guess_planets[index == 0U ? 10U : 24U].guess_word,
                (unsigned)expected_words[index],
                trace->guess_random_limits[index],
                trace->guess_random_values[index]);
            ++trace->failures;
        }
    }
}

static void run_bof_writer(TurinTrace *trace, StarsI16 file_type)
{
    StarsI16 index = trace->bof_writer_calls;
    StarsSeeds before = trace->seeds;
    if (index < 0 || index >= 2) {
        ++trace->failures;
        return;
    }
    memcpy(trace->rng_file_data + 0x0a1cU, trace->complete_input.bof, 4U);
    memcpy(trace->rng_file_data + 0x0070U, trace->complete_input.game, 4U);
    put_u16(trace->rng_file_data, 0x0082U,
        get_u16(trace->complete_input.bof, 10U));
    put_u16(trace->rng_file_data, 0x0080U,
        (StarsU16)(((get_u16(trace->complete_input.bof, 14U) >> 13) & 7U) << 9));
    put_u32(trace->rng_file_data, 0x1370U, trace->seeds.first);
    put_u32(trace->rng_file_data, 0x1374U, trace->seeds.second);
    put_u16(trace->rng_file_data, 0x07caU, trace->game_flags);
    put_u16(trace->rng_file_data, 0x07ceU, trace->startup_log_flags);
    stars_write_bof(trace->rng_file_data, 1, file_type, 0,
        replay_startup_tick_count, capture_bof_write, trace);
    trace->seeds.first = get_u32(trace->rng_file_data, 0x1370U);
    trace->seeds.second = get_u32(trace->rng_file_data, 0x1374U);
    if (trace->bof_write_sizes[index] != 18 ||
            get_u16(trace->bof_write_records[index], 0U) != 0x2010U ||
            (get_u16(trace->bof_write_records[index], 14U) & 31U) != 1U ||
            get_u16(trace->bof_write_records[index], 16U) !=
                (StarsU16)(file_type | 0x0100U))
        ++trace->failures;
    if (memcmp(trace->bof_write_records[index], expected_startup_bofs[trace->seeded_action][index], 18U)) {
        StarsU16 byte;
        for (byte = 0U; byte < 18U; ++byte)
            if (trace->bof_write_records[index][byte] != expected_startup_bofs[trace->seeded_action][index][byte])
                fprintf(stderr, "startup BOF %d byte %u got %u expected %u\n", index, byte,
                    trace->bof_write_records[index][byte], expected_startup_bofs[trace->seeded_action][index][byte]);
        ++trace->failures;
    }
    trace->bof_random_values[index] = (StarsI16)(((get_u16(trace->bof_write_records[index], 14U) >> 5) +
        2048U - (StarsU16)replay_startup_ticks[trace->seeded_action][index]) & 0x07ffU);
    turn_audit_main_draw(trace, before, 2000, trace->bof_random_values[index]);
    ++trace->bof_writer_calls;
}

static StarsI16 seeded_mining_random(void *opaque, StarsI16 limit)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsI16 index = trace->mining_random_calls;
    StarsI16 value;
    if (index < 0 || index >= TURIN_RETAINED_MINE_DRAW_COUNT) {
        ++trace->failures;
        return 0;
    }
    if (limit != 100 || !trace->seeded_mode)
        ++trace->failures;
    trace->mining_random_limits[index] = limit;
    value = turn_main_random(trace, limit);
    trace->mining_random_values[index] = value;
    ++trace->mining_random_calls;
    return value;
}

static StarsI16 turn_score_resource_stat(void *, StarsI16, StarsI16);
static StarsI32 turn_score_max_population(void *, StarsU16, StarsI16);

static void estimate_retained_mining_planet(void *opaque,
    const StarsU8 *planet, StarsI32 output[3], StarsI16 c_mines,
    StarsI16 apply)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsU16 planet_id = get_u16(planet, 0U);
    StarsI16 index;
    StarsI16 mineral;
    StarsMineralMinedInput *input;
    ++trace->mining_visited;
    if ((StarsI16)get_u16(planet, 2U) < 0) {
        StarsMineralMinedInput unowned;
        memset(&unowned, 0, sizeof unowned);
        unowned.player = -1; unowned.c_mines = c_mines;
        unowned.ops = &trace->mining_ops;
        stars_est_minerals_mined(&unowned);
        for (mineral = 0; mineral < 3; ++mineral) output[mineral] = unowned.output[mineral];
        return;
    }
    for (index = 0; index < 2; ++index)
        if (planet_id == trace->mining_runtime_ids[index])
            break;
    if (index >= 2) {
        ++trace->failures;
        return;
    }
    if (trace->mining_planet_calls >= 2)
        ++trace->failures;
    else
        trace->mining_planet_ids[trace->mining_planet_calls] =
            (StarsI16)planet_id;
    ++trace->mining_planet_calls;
    input = &trace->mining_inputs[index];
    if (get_u16(planet, 2U) != (StarsU16)trace->mining_inputs[index].player ||
            c_mines != -1 || apply != 1)
        ++trace->failures;
    input->c_mines = c_mines;
    input->apply = apply;
    stars_est_minerals_mined(input);
    for (mineral = 0; mineral < 3; ++mineral)
        output[mineral] = input->output[mineral];
}

static void run_retained_mining(TurinTrace *trace, StarsI16 action)
{
    const StarsI16 *expected_random = action ?
        retained_mining_values_action : retained_mining_values_control;
    const StarsI32 (*expected_output)[3] = action ?
        retained_mining_outputs_action : retained_mining_outputs_control;
    StarsI16 index;
    StarsI16 mineral;
    StarsU16 offset;
    memset(trace->mining_planets, 0, sizeof trace->mining_planets);
    memset(trace->mining_inputs, 0, sizeof trace->mining_inputs);
    memset(trace->mining_stock, 0, sizeof trace->mining_stock);
    memset(&trace->mining_ops, 0, sizeof trace->mining_ops);
    trace->mining_ops.random = seeded_mining_random;
    trace->mining_ops.context = trace;
    index = 0;
    for (offset = 0U; offset < 32U; ++offset) {
        const StarsReadPlanetState *source = &trace->turn_planet_states[offset];
        StarsMineralMinedInput *input;
        StarsCMaxOperableMinesOps mine_ops;
        StarsU8 *raw = trace->mining_planets + offset * 56U;
        pack_loaded_planet(raw, source);
        if (source->player < 0) continue;
        if (index >= 2) { ++trace->failures; return; }
        trace->mining_runtime_ids[index] = (StarsU16)source->id;
        input = &trace->mining_inputs[index];
        input->player = source->player;
        input->special_race = turn_score_resource_stat(trace, source->player, 14) == 8;
        if (input->special_race) { ++trace->failures; return; }
        input->efficiency_tenths = turn_score_resource_stat(trace, source->player, 4);
        memset(&mine_ops, 0, sizeof mine_ops);
        mine_ops.max_mines.calc_max_population = turn_score_max_population;
        mine_ops.max_mines.get_player_stat = turn_score_resource_stat;
        input->operating_mines = stars_cmines_operating(raw, &mine_ops, trace);
        input->c_mines = -1;
        input->force_concentration_floor = (source->flags >> 10) & 1U;
        input->random_rounding = 1;
        input->mineral_weight = trace->mining_stock[index];
        input->ops = &trace->mining_ops;
        for (mineral = 0; mineral < 3; ++mineral) {
            input->concentration[mineral] = source->mineral_concentrations[mineral];
            input->percent_level[mineral] = source->mineral_levels[mineral];
            trace->mining_stock[index][mineral] = (StarsI32)source->minerals[mineral];
        }
        ++index;
    }
    if (index != 2) { ++trace->failures; return; }
    stars_mine_minerals(trace->mining_planets, 32U,
        estimate_retained_mining_planet, trace);
    if (trace->mining_visited != 32U || trace->mining_planet_calls != 2 ||
            trace->mining_planet_ids[0] != 10 ||
            trace->mining_planet_ids[1] != 24 ||
            trace->mining_random_calls != TURIN_RETAINED_MINE_DRAW_COUNT)
        ++trace->failures;
    for (index = 0; index < TURIN_RETAINED_MINE_DRAW_COUNT; ++index) {
        if (trace->mining_random_limits[index] != 100 ||
                trace->mining_random_values[index] != expected_random[index])
            ++trace->failures;
    }
    for (index = 0; index < 2; ++index)
        for (mineral = 0; mineral < 3; ++mineral)
            if (trace->mining_inputs[index].output[mineral] !=
                    expected_output[index][mineral])
                ++trace->failures;
}

static StarsI16 turn_event_random(void *opaque, StarsI16 limit)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsI16 index = trace->random_event_random_calls;
    StarsI16 value;
    if (index < 0 || index >= 3) {
        ++trace->failures;
        return 0;
    }
    if (limit != retained_random_event_limits[index])
        ++trace->failures;
    value = turn_main_random(trace, limit);
    trace->random_event_random_limits[index] = limit;
    trace->random_event_random_values[index] = value;
    ++trace->random_event_random_calls;
    return value;
}

static void record_random_event_callback(TurinTrace *trace, StarsI16 event_id)
{
    StarsI16 index = trace->random_event_callback_calls;
    if (index < 0 || index >= 4)
        ++trace->failures;
    else
        trace->random_event_callback_order[index] = event_id;
    ++trace->random_event_callback_calls;
}

/* Unexpected nested effects must fail this pinned-pair adapter. Source
 * gate decisions, not retained random values, determine whether they occur. */
static void turn_event_meteor_message(void *opaque, StarsI16 player,
    StarsI16 message, StarsI16 planet, const StarsU32 amount[3])
{ (void)player; (void)message; (void)planet; (void)amount;
  ++((TurinTrace *)opaque)->failures; }
static void turn_event_meteor_toss(void *opaque, StarsMeteorPlanet *planet)
{ (void)planet; ++((TurinTrace *)opaque)->failures; }
static void turn_event_climate_message(void *opaque, StarsI16 player,
    StarsU16 message, StarsU16 a, StarsU16 b, StarsI16 climate)
{ (void)player; (void)message; (void)a; (void)b; (void)climate;
  ++((TurinTrace *)opaque)->failures; }
static void turn_event_climate_toss(void *opaque, StarsClimatePlanet *planet)
{ (void)planet; ++((TurinTrace *)opaque)->failures; }
static void turn_event_mineral_message(void *opaque, StarsI16 player,
    StarsU16 a, StarsU16 b, StarsI16 mineral)
{ (void)player; (void)a; (void)b; (void)mineral;
  ++((TurinTrace *)opaque)->failures; }
static StarsMysteryTraderThing *turn_event_allocate(void *opaque, StarsI16 a, StarsI16 b)
{ (void)a; (void)b; ++((TurinTrace *)opaque)->failures; return 0; }

static void retained_event_meteor(void *opaque)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsMeteorInput input;
    StarsMeteorPlanet planets[32];
    StarsU16 i, axis;
    memset(&input, 0, sizeof input); memset(planets, 0, sizeof planets);
    for (i = 0U; i < 32U; ++i) {
        const StarsReadPlanetState *source = &trace->turn_planet_states[i];
        planets[i].id = source->id; planets[i].owner = source->player;
        planets[i].population = source->minerals[3];
        for (axis = 0U; axis < 3U; ++axis) {
            planets[i].minerals[axis] = source->minerals[axis];
            planets[i].concentration[axis] = source->mineral_concentrations[axis];
            planets[i].environment[axis] = (signed char)source->environment[axis];
            planets[i].original_environment[axis] = (signed char)source->environment_original[axis];
        }
    }
    record_random_event_callback(trace, 1);
    input.turn = trace->random_event_turn;
    input.player_count = (StarsI16)get_u16(trace->complete_input.game, 8U);
    input.planets = planets; input.planet_count = trace->complete_input.views[0].planet_count;
    input.random = turn_event_random; input.send_message = turn_event_meteor_message;
    input.toss_non_auto_build_items = turn_event_meteor_toss; input.context = trace;
    stars_meteor_strike(&input);
}

static void retained_event_climate(void *opaque)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsPlanetaryClimateInput input;
    StarsClimatePlanet planets[32];
    StarsU16 i, axis;
    memset(&input, 0, sizeof input); memset(planets, 0, sizeof planets);
    for (i = 0U; i < 32U; ++i) {
        const StarsReadPlanetState *source = &trace->turn_planet_states[i];
        planets[i].id = source->id; planets[i].player = source->player;
        planets[i].climate_metric = source->minerals[3];
        for (axis = 0U; axis < 3U; ++axis) {
            planets[i].environment[axis] = source->environment[axis];
            planets[i].original_environment[axis] = source->environment_original[axis];
        }
    }
    record_random_event_callback(trace, 2);
    input.turn = trace->random_event_turn; input.planets = planets;
    input.planet_count = trace->complete_input.views[0].planet_count;
    input.random = turn_event_random; input.send_message = turn_event_climate_message;
    input.toss_build_items = turn_event_climate_toss; input.context = trace;
    stars_planetary_climate_change(&input);
}

static void retained_event_minerals(void *opaque)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsDiscoverMineralsInput input;
    StarsDiscoverMineralPlanet planets[32];
    StarsU16 i, axis;
    memset(&input, 0, sizeof input); memset(planets, 0, sizeof planets);
    for (i = 0U; i < 32U; ++i) {
        const StarsReadPlanetState *source = &trace->turn_planet_states[i];
        planets[i].id = (StarsU16)source->id; planets[i].player = source->player;
        for (axis = 0U; axis < 3U; ++axis)
            planets[i].concentration[axis] = source->mineral_concentrations[axis];
    }
    record_random_event_callback(trace, 3);
    input.turn = trace->random_event_turn; input.planets = planets;
    input.planet_count = trace->complete_input.views[0].planet_count;
    input.mineral_size = (StarsI16)get_u16(trace->complete_input.game, 4U);
    input.random = turn_event_random; input.message = turn_event_mineral_message;
    input.context = trace;
    stars_discover_new_minerals(&input);
}

static void retained_event_trader(void *opaque)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsMysteryTraderInput input;
    StarsI16 draws = trace->random_event_random_calls;
    memset(&input, 0, sizeof input);
    record_random_event_callback(trace, 4);
    input.turn = trace->random_event_turn;
    input.player_count = (StarsI16)get_u16(trace->complete_input.game, 8U);
    input.map_size = (StarsI16)get_u16(trace->complete_input.game, 4U);
    input.random = turn_event_random; input.allocate_thing = turn_event_allocate;
    input.context = trace;
    stars_mystery_trader(&input);
    if (input.turn <= 39 && trace->random_event_random_calls == draws)
        ++trace->random_event_trader_skipped;
    else ++trace->failures;
}

static void run_retained_random_events(TurinTrace *trace, StarsI16 action)
{
    const StarsI16 *expected_values = action ?
        retained_random_event_values_action :
        retained_random_event_values_control;
    StarsI16 index;
    trace->random_event_turn = (StarsI16)get_u16(trace->complete_input.game, 18U);
    stars_random_events(retained_event_meteor, retained_event_climate,
        retained_event_minerals, retained_event_trader, trace);
    if (trace->random_event_callback_calls != 4 ||
            trace->random_event_random_calls != 3 ||
            trace->random_event_trader_skipped != 1)
        ++trace->failures;
    for (index = 0; index < 4; ++index)
        if (trace->random_event_callback_order[index] != index + 1)
            ++trace->failures;
    for (index = 0; index < 3; ++index)
        if (trace->random_event_random_limits[index] !=
                    retained_random_event_limits[index] ||
                trace->random_event_random_values[index] !=
                    expected_values[index])
            ++trace->failures;
}

static StarsI16 retained_wormhole_random(void *opaque, StarsI16 limit)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    const StarsI16 *expected_limits = trace->seeded_action ?
        retained_wormhole_draw_limits_action :
        retained_wormhole_draw_limits_control;
    const StarsI16 *expected_values = trace->seeded_action ?
        retained_wormhole_draw_values_action :
        retained_wormhole_draw_values_control;
    StarsI16 index = trace->wormhole_move_random_calls;
    StarsI16 value;
    if (index < 0 || index >= TURIN_RETAINED_WORMHOLE_DRAW_COUNT) {
        ++trace->failures;
        return 0;
    }
    value = turn_main_random(trace, limit);
    trace->wormhole_move_random_limits[index] = limit;
    trace->wormhole_move_random_values[index] = value;
    if (limit != expected_limits[index] ||
            value != expected_values[index])
        ++trace->failures;
    ++trace->wormhole_move_random_calls;
    return value;
}

static StarsI16 retained_wormhole_chance(void *opaque,
    const StarsMoveThing *thing)
{
    (void)opaque;
    return stars_pct_wormhole_moves(thing->packed_destination);
}

static StarsMoveFleet *turn_current_moving_fleet(TurinTrace *, const Soe3InputFleet *);

static StarsI16 retained_wormhole_validate(void *opaque,
    StarsMoveThing *thing)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsWormholePoint others[TURIN_RETAINED_THING_COUNT - 1U];
    StarsU16 current;
    StarsU16 index;
    StarsU16 count = 0;
    StarsNearestPoint fleets[10];
    StarsNearestPoint planets[32];
    StarsU16 fleet_count = 0U;

    current = (StarsU16)(thing - trace->retained_move_things);
    if (current >= TURIN_RETAINED_THING_COUNT) {
        ++trace->failures;
        return 0x0f;
    }
    for (index = 0; index < TURIN_RETAINED_THING_COUNT; ++index) {
        const StarsMoveThing *other;
        if (index == current) continue;
        other = &trace->retained_move_things[index];
        others[count].x = other->x;
        others[count].y = other->y;
        others[count].identity = other->identity;
        others[count].flags = other->identity;
        ++count;
    }
    for (index = 0U; index < trace->complete_input.views[0].fleet_count; ++index) {
        StarsMoveFleet *moving = turn_current_moving_fleet(trace,
            &trace->complete_input.views[0].fleets[index]);
        if (!moving) continue;
        if (fleet_count >= 10U) { ++trace->failures; return 0x0f; }
        fleets[fleet_count].x = moving->x;
        fleets[fleet_count].y = moving->y;
        ++fleet_count;
    }
    if (fleet_count != (trace->seeded_action ? 7U : 8U)) ++trace->failures;
    for (index = 0U; index < trace->complete_input.coordinate_count; ++index) {
        planets[index].x = trace->complete_input.coordinates[index][0];
        planets[index].y = trace->complete_input.coordinates[index][1];
    }
    ++trace->wormhole_move_validate_calls;
    return stars_validate_wormhole_position(thing->x, thing->y,
        thing->partner_identity, (StarsI16)get_u16(trace->complete_input.game, 4U), others,
        count, planets, trace->complete_input.coordinate_count, fleets, fleet_count);
}

static void turn_validate_current_waypoints(TurinTrace *);

static void retained_wormhole_validate_waypoints(void *opaque)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsU16 index;
    /* MoveThings calls validation before returning: publish its latest
     * coordinates before resolving any waypoint target. */
    for (index = 0U; index < trace->complete_input.views[0].thing_count; ++index) {
        StarsU8 *raw = trace->complete_input.views[0].things[index];
        put_u16(raw, 2U, (StarsU16)trace->retained_move_things[index].x);
        put_u16(raw, 4U, (StarsU16)trace->retained_move_things[index].y);
        put_u16(raw, 6U, trace->retained_move_things[index].packed_destination);
        memcpy(trace->turn_thing_bytes + index * 18U, raw, 18U);
    }
    turn_validate_current_waypoints(trace);
    ++trace->wormhole_move_waypoint_calls;
}

static void run_retained_wormhole_movement(TurinTrace *trace)
{
    const StarsU8 (*expected)[18] = trace->seeded_action ?
        retained_wormhole_output_action : retained_wormhole_output_control;
    StarsMoveThingsOps ops;
    StarsMoveThingsInput input;
    StarsU8 output[18];
    StarsU16 index;

    memset(&ops, 0, sizeof ops);
    memset(&input, 0, sizeof input);
    memset(trace->retained_move_things, 0,
        sizeof trace->retained_move_things);
    for (index = 0; index < TURIN_RETAINED_THING_COUNT; ++index) {
        const StarsU8 *raw = trace->turn_thing_bytes + index * 18U;
        StarsU16 identity = get_u16(raw, 0U);
        StarsMoveThing *thing = &trace->retained_move_things[index];
        thing->kind = (StarsI16)(identity >> 13);
        thing->owner = (StarsI16)((identity >> 9) & 0x0fU);
        thing->identity = identity;
        thing->partner_identity = get_u16(raw, 12U);
        thing->x = (StarsI16)get_u16(raw, 2U);
        thing->y = (StarsI16)get_u16(raw, 4U);
        thing->packed_destination = get_u16(raw, 6U);
        thing->active = 1;
        if (thing->kind != 2)
            ++trace->failures;
    }
    ops.random_value = retained_wormhole_random;
    ops.wormhole_chance = retained_wormhole_chance;
    ops.validate_wormhole = retained_wormhole_validate;
    ops.validate_waypoints = retained_wormhole_validate_waypoints;
    ops.context = trace;
    input.things = trace->retained_move_things;
    input.thing_count = TURIN_RETAINED_THING_COUNT;
    input.post_production = 1;
    input.map_size = (StarsI16)get_u16(trace->complete_input.game, 4U);
    input.ops = &ops;
    stars_move_things(&input);
    if (trace->wormhole_move_random_calls !=
            TURIN_RETAINED_WORMHOLE_DRAW_COUNT ||
            trace->wormhole_move_validate_calls !=
                TURIN_RETAINED_THING_COUNT ||
            trace->wormhole_move_waypoint_calls != 1)
        ++trace->failures;
    for (index = 0; index < TURIN_RETAINED_THING_COUNT; ++index) {
        memcpy(output, trace->turn_thing_bytes + index * 18U, sizeof output);
        put_u16(output, 2U,
            (StarsU16)trace->retained_move_things[index].x);
        put_u16(output, 4U,
            (StarsU16)trace->retained_move_things[index].y);
        put_u16(output, 6U,
            trace->retained_move_things[index].packed_destination);
        if (memcmp(output, expected[index], sizeof output) != 0)
            ++trace->failures;
        memcpy(trace->turn_thing_bytes + index * 18U, output, sizeof output);
        memcpy(trace->complete_input.views[0].things[index], output, sizeof output);
    }
}

static void run_retained_preproduction_thing_move(TurinTrace *trace)
{
    StarsMoveThing things[TURIN_RETAINED_THING_COUNT];
    StarsMoveThingsOps ops;
    StarsMoveThingsInput input;
    StarsSeeds seeds_before = trace->seeds;
    StarsI16 calls_before = trace->random_calls;
    StarsU16 index;
    memset(things, 0, sizeof things);
    memset(&ops, 0, sizeof ops);
    memset(&input, 0, sizeof input);
    for (index = 0; index < TURIN_RETAINED_THING_COUNT; ++index) {
        const StarsU8 *raw = trace->complete_input.views[0].things[index];
        StarsU16 identity = get_u16(raw, 0U);
        things[index].kind = (StarsI16)(identity >> 13);
        things[index].owner = (StarsI16)((identity >> 9) & 0x0fU);
        things[index].identity = identity;
        things[index].partner_identity = get_u16(raw, 12U);
        things[index].x = (StarsI16)get_u16(raw, 2U);
        things[index].y = (StarsI16)get_u16(raw, 4U);
        things[index].packed_destination = get_u16(raw, 6U);
        things[index].active = 1;
    }
    ops.random_value = random_value;
    ops.context = trace;
    input.things = things;
    input.thing_count = TURIN_RETAINED_THING_COUNT;
    input.post_production = 0;
    input.map_size = (StarsI16)get_u16(trace->complete_input.game, 4U);
    input.ops = &ops;
    stars_move_things(&input);
    if (trace->random_calls != calls_before ||
            trace->seeds.first != seeds_before.first ||
            trace->seeds.second != seeds_before.second)
        ++trace->failures;
    for (index = 0; index < TURIN_RETAINED_THING_COUNT; ++index)
        if (things[index].x != (StarsI16)get_u16(
                    retained_wormholes[index], 2U) ||
                things[index].y != (StarsI16)get_u16(
                    retained_wormholes[index], 4U) ||
                things[index].packed_destination != get_u16(
                    retained_wormholes[index], 6U))
            ++trace->failures;
}

static void startup_host_reset_heap(void *opaque, StarsU16 heap)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    static const StarsU16 order[] = { 3, 5, 10, 11, 6, 1, 7, 0, 8 };
    if (trace->host_destroy_heaps >= 9U || heap != order[trace->host_destroy_heaps]) {
        ++trace->failures; return;
    }
    ++trace->host_destroy_heaps;
    if (heap == 3U) {
        memset(&trace->planet, 0, sizeof trace->planet);
        memset(&trace->supply_planet, 0, sizeof trace->supply_planet);
    } else if (heap == 5U) {
        memset(trace->raw_fleets, 0, sizeof trace->raw_fleets);
        memset(trace->raw_fleet_present, 0, sizeof trace->raw_fleet_present);
        memset(trace->fleets, 0, sizeof trace->fleets);
        memset(trace->fleet_storage, 0, sizeof trace->fleet_storage);
        memset(trace->fleet_items, 0, sizeof trace->fleet_items);
        memset(&trace->supply_fleet, 0, sizeof trace->supply_fleet);
        trace->fleet_count = 0U;
    } else if (heap == 6U) {
        if (trace->ai_live_allocations) ++trace->failures;
        /* Freed AI bytes remain observation snapshots; remove runtime aliases. */
        trace->turin_input.raw_ai_planet = 0;
        trace->turin_input.raw_ai_planet_size = 0U;
        trace->scout.ai_planet_records = 0;
        trace->scout.ai_planet_records_size = 0U;
    } else if (heap == 7U) {
        memset(trace->designs, 0, sizeof trace->designs);
        memset(trace->starbase_designs, 0, sizeof trace->starbase_designs);
        trace->current_designs_loaded = 0;
    } else if (heap == 0U) {
        memset(trace->plord, 0, sizeof trace->plord);
        memset(trace->turn_produce_queue, 0, sizeof trace->turn_produce_queue);
        trace->turn_committed_queue = 0;
        trace->turn_committed_queue_count = 0U;
        trace->turn_committed_planet = -1;
    }
}

static void startup_host_reset_messages(void *opaque)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    memset(&trace->turn_messages, 0, sizeof trace->turn_messages);
    trace->turn_message_bytes = 0U;
    trace->turn_message_requests = 0U;
    trace->turn_outgoing_count = 0U;
}

static void startup_host_save_selection(void *opaque)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    if (trace->host_saved_selection_calls ||
            trace->session.selected_kind != (StarsU16)trace->ai_selection.grobj ||
            trace->session.selected_id != trace->ai_selection.id ||
            trace->session.game_lid != get_u32(trace->complete_input.game, 0U)) {
        ++trace->failures; return;
    }
    trace->host_saved_selection_kind = trace->session.selected_kind;
    trace->host_saved_selection_id = trace->session.selected_id;
    trace->host_saved_selection_player = trace->session.player;
    trace->host_saved_selection_lid = trace->session.game_lid;
    trace->host_saved_ini_flags = (StarsU16)((trace->host_saved_ini_flags & 0xfe1fU) |
        ((trace->session.selected_kind & 15U) << 5));
    ++trace->host_saved_selection_calls;
}

static void startup_host_destroy(void *opaque, StarsGenerateTurnState *state)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsDestroyGameOps ops;
    (void)state;
    if (trace->host_destroy_calls || trace->ai_input.ai_active ||
            trace->ai_live_allocations || !trace->startup_history_closed ||
            !trace->generated_log_size) { ++trace->failures; return; }
    memset(&ops, 0, sizeof ops);
    ops.reset_heap = startup_host_reset_heap;
    ops.reset_messages = startup_host_reset_messages;
    ops.save_selection = startup_host_save_selection;
    trace->session.game_flags = (get_u32(trace->rng_file_data, 0x07caU) & 0xffff0000UL) |
        trace->game_flags;
    trace->session.game_flags2 = trace->startup_log_flags;
    stars_destroy_cur_game(&trace->session, &ops, trace);
    trace->game_flags = (StarsU16)trace->session.game_flags;
    trace->startup_log_flags = trace->session.game_flags2;
    put_u32(trace->rng_file_data, 0x07caU, trace->session.game_flags);
    put_u16(trace->rng_file_data, 0x07ceU, trace->startup_log_flags);
    memset(&trace->ai_selection, 0, sizeof trace->ai_selection);
    trace->ai_selection.id = trace->session.selected_id;
    trace->ai_selection.planet_id = trace->session.selected_planet;
    trace->ai_selection.active_waypoint = trace->session.selected_waypoint;
    trace->fleet_report.cached = 0;
    ++trace->host_destroy_calls;
}

static StarsI16 startup_turn_load(void *opaque,
    StarsGenerateTurnState *state)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsU16 index;
    const Soe3InputView *host;
    if (trace->host_destroy_calls != 1U || trace->host_destroy_heaps != 9U ||
            trace->session.player != -1 || trace->fleet_count ||
            trace->current_designs_loaded || trace->turn_committed_queue ||
            trace->session.game_lid != 0UL || trace->session.salt != 0UL ||
            trace->session.last_tick != 0UL ||
            trace->session.memory_log_current != 0 ||
            trace->session.memory_log_previous != -1 ||
            trace->session.message_current != 0 ||
            trace->session.selected_kind != 0U ||
            trace->session.selected_id != -1) return 0;
    for (index = 0U; index < trace->complete_input.views[2].fleet_count; ++index) {
        const Soe3InputFleet *fleet = &trace->complete_input.views[2].fleets[index];
        StarsU16 id = (StarsU16)(fleet->id_word & 511U);
        if (fleet->owner != 1 || id >= TURIN_RETAINED_FLEET_COUNT) return 0;
        pack_input_fleet(trace->raw_fleets[id], fleet);
        trace->raw_fleet_present[id] = 1;
        memcpy(trace->fleet_storage[id].bytes, trace->raw_fleets[id], 124U);
        trace->fleet_items[trace->fleet_count++] = &trace->fleet_storage[id];
        if (!stars_turin_fleet_from_packed(trace->raw_fleets[id], 124U,
                (StarsI16)(fleet->state.orders[6] & 15U), trace->plord[id],
                &trace->fleets[id])) return 0;
    }
    if (!read_input_player(trace, 0U) || !read_input_player(trace, 1U))
        return 0;
    host = &trace->complete_input.views[0];
    memset(trace->planet_resource_extra, 0, sizeof trace->planet_resource_extra);
    trace->turn_drop_count = 0U; trace->turn_transfer_count = 0U;
    memset(&trace->turn_messages, 0, sizeof trace->turn_messages);
    trace->turn_message_bytes = 0U; trace->turn_message_requests = 0U;
    trace->turn_outgoing_count = 0U;
    state->player_count = (StarsI16)get_u16(
        trace->complete_input.game, 8U);
    state->turn = (StarsI16)get_u16(
        trace->complete_input.bof, 10U);
    if (host->planet_count != TURIN_RETAINED_POINT_COUNT) return 0;
    trace->session.player_count = (StarsU16)state->player_count;
    trace->session.game_lid = get_u32(trace->complete_input.game, 0U);
    trace->session.planet_capacity = trace->complete_input.coordinate_count;
    trace->session.planet_count = host->planet_count;
    trace->session.fleet_count = host->fleet_count;
    trace->session.thing_count = host->thing_count;
    trace->session.turn = (StarsU16)state->turn;
    for (index = 0U; index < TURIN_RETAINED_POINT_COUNT; ++index) {
        if (host->planets[index].id != (StarsI16)index) return 0;
        trace->turn_planet_states[index] = host->planets[index];
        trace->turn_order_planets[index].id = (StarsU16)host->planets[index].id;
        trace->turn_order_planets[index].player = host->planets[index].player;
        trace->turn_order_planets[index].turn = host->planets[index].turn;
        trace->turn_order_planets[index].flags = host->planets[index].flags;
        ++trace->turn_hst_planet_read_calls;
    }
    if (trace->turn_planet_states[10].player != 0 ||
            trace->turn_planet_states[24].player != 1 ||
            trace->turn_planet_states[10].routing_word != 62U ||
            trace->turn_planet_states[24].routing_word != 62U ||
            trace->turn_planet_states[10].minerals[3] != 250UL ||
            trace->turn_planet_states[24].minerals[3] != 250UL)
        ++trace->failures;
    return 1;
}

static const StarsU8 *find_wormhole(void *opaque, StarsPoint source,
    StarsU32 distance)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    const StarsU8 *things[TURIN_RETAINED_THING_COUNT];
    StarsI16 index;
    ++trace->wormhole_calls;
    for (index = 0; (StarsU16)index < TURIN_RETAINED_THING_COUNT; ++index)
        things[(StarsU16)index] = trace->complete_input.views[0].things[(StarsU16)index];
    return stars_lpth_worm_find(source, distance, 1, things,
        TURIN_RETAINED_THING_COUNT, 0, random_value, trace);
}

static StarsI16 visit_fleet(void *opaque,
    const StarsTurinTurnFleet *fleet, const StarsTurinFleetPolicy *policy,
    const StarsU8 potency[4])
{
    TurinTrace *trace = startup_trace(opaque);
    const Soe3InputFleet *original = 0;
    StarsLogFleet before, after;
    StarsLogChangeFleetState state;
    StarsLogChangeFleetOps ops;
    StarsU16 i;
    (void)policy; (void)potency;
    if (!trace || !fleet || !fleet->raw_plord) return 0;
    for (i = 0U; i < trace->complete_input.views[2].fleet_count; ++i)
        if ((trace->complete_input.views[2].fleets[i].id_word & 511U) == (StarsU16)fleet->id)
            original = &trace->complete_input.views[2].fleets[i];
    if (!original) return 0;
    memset(&before, 0, sizeof before);
    before.object_id = original->id_word;
    /* This hook logs the order mutation only. Other fleet mutations have
     * their own snapshots and log boundaries. */
    before.order_count = original->state.order_count;
    before.orders = original->state.orders;
    after = before;
    after.order_count = fleet->raw_plord[3]; after.orders = fleet->raw_plord + 4U;
    memset(&state, 0, sizeof state); memset(&ops, 0, sizeof ops);
    ops.write_memory = startup_capture_memory_log;
    stars_log_change_fleet(&before, &after, trace->game_flags, &state, &ops, trace);
    return 1;
}

static void run_turin_personality(void *opaque, StarsI16 player,
    void *planet_table)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    if (trace->session.player != player || !trace->ai_input.ai_active)
        ++trace->failures;
    ++trace->personality_calls;
    trace->turin_input.player_id = player;
    trace->turin_input.raw_ai_planet = (StarsU8 *)planet_table;
    trace->turin_input.raw_ai_planet_size = sizeof(trace->ai_planet_records);
    if (!stars_do_turin_drone_ai_turn(&trace->turin_input))
        ++trace->failures;
}

static StarsI32 turn_fuel_capacity(void *, const StarsFuelFleet *);
static StarsI32 move_current_capacity(TurinTrace *trace, StarsI16 owner,
    StarsU16 id, const StarsReadFleetState *source)
{
    StarsFuelFleet fleet;
    StarsU16 ship;
    memset(&fleet, 0, sizeof fleet);
    fleet.player = owner;
    fleet.status_flags = owner == 1 ? get_u16(trace->raw_fleets[id], 4U) : source->fleet_flags;
    for (ship = 0U; ship < 16U; ++ship)
        fleet.ship_counts[ship] = owner == 1 ?
            get_u16(trace->raw_fleets[id], (StarsU16)(12U + ship * 2U)) : source->ship_counts[ship];
    return turn_fuel_capacity(trace, &fleet);
}

static StarsI16 get_order(void *opaque, const StarsMoveFleet *fleet,
    StarsMoveFleetOrder *out)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    const StarsU8 *raw;
    StarsU16 flags, order_index;
    if (!fleet || !out || fleet->id < 0 || fleet->id >= 4 ||
            !trace->plord[fleet->id][3]) { ++trace->failures; return 0; }
    flags = get_u16(trace->plord[fleet->id], 10U);
    if ((flags & 15U) == 1U || (flags & 15U) == 6U ||
            trace->plord[fleet->id][3] < 2U) return 0;
    /* These planet-bound fleets finish in the first pass; later passes are
     * for fleet pursuit in the original, not repeated planet arrivals. */
    if (fleet->status & 0x2000U) return 0;
    order_index = 1U;
    raw = trace->plord[fleet->id] + STARS_AI_PLORD_HEADER_SIZE +
        order_index * STARS_AI_PLORD_ORDER_SIZE;
    flags = get_u16(raw, 6U);
    out->kind = (StarsI16)(flags & 15U);
    out->target_kind = (StarsI16)((flags >> 8) & 15U);
    out->target_id = (StarsI16)get_u16(raw, 4U);
    out->target_x = (StarsI16)get_u16(raw, 0U);
    out->target_y = (StarsI16)get_u16(raw, 2U);
    out->warp_level = (StarsI16)((flags >> 4) & 15U);
    if (!out->warp_level) return 0;
    out->colonist_cargo = (StarsI32)get_u32(trace->raw_fleets[fleet->id], 88U);
    /* Source skips colonist deaths with no colonists, and cheap-engine
     * failure below warp 7. Fail if this pair's omitted effects become live. */
    if ((get_u16(trace->complete_input.views[0].players[fleet->owner], 84U) & 4U) ||
            out->colonist_cargo != 0L ||
            (out->warp_level > 6 &&
             (get_u32(trace->complete_input.views[0].players[fleet->owner], 78U) & 0x100UL))) {
        ++trace->failures; return 0;
    }
    return 1;
}

static StarsI32 distance(void *opaque, StarsI16 x1, StarsI16 y1,
    StarsI16 x2, StarsI16 y2)
{
    (void)opaque;
    /* Original MoveFleets 10b0:43c0..4406 rounds travel up. */
    return (StarsI32)(stars_get_distance(x1, y1, x2, y2) + 0.9999);
}

static StarsI32 movement_limit(void *opaque, const StarsMoveFleet *fleet,
    const StarsMoveFleetOrder *order, StarsI32 distance_to_target, StarsI16 pass)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    (void)fleet;
    (void)distance_to_target;
    /* Original 10b0:4240..426c uses the next waypoint's warp. */
    if (pass != 0 || order->target_kind != 1 || order->warp_level < 1 ||
            order->warp_level > 9) {
        ++trace->failures;
        return 0L;
    }
    return (StarsI32)order->warp_level * order->warp_level;
}

static StarsI32 current_fuel_estimate(void *opaque, const StarsMoveFleet *fleet,
    const StarsMoveFleetOrder *order, StarsI32 travel, StarsI32 *gain)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsEstFuelInput input;
    StarsEstFuelFleet current;
    StarsRamScoopDesign scoops[16];
    StarsI32 cost;
    StarsU16 ship, slot, warp;
    const Soe3InputFleet *source = 0;
    const StarsU8 *bank;
    if (fleet->owner < 0 || fleet->owner > 1 || fleet->id < 0 || fleet->id >= 6) {
        ++trace->failures; return 0L;
    }
    for (slot = 0U; slot < trace->complete_input.views[0].fleet_count; ++slot) {
        const Soe3InputFleet *candidate = &trace->complete_input.views[0].fleets[slot];
        if (candidate->owner == fleet->owner && (candidate->id_word & 511U) == (StarsU16)fleet->id)
            source = candidate;
    }
    if (!source) { ++trace->failures; return 0L; }
    memset(&input, 0, sizeof input); memset(&current, 0, sizeof current);
    memset(scoops, 0, sizeof scoops);
    current.player = fleet->owner; current.destination = order->target_id;
    current.cargo_weight = fleet->fuel;
    for (slot = 0U; slot < 4U; ++slot)
        current.mineral_weight[slot] = fleet->owner == 1 ?
            (StarsI32)get_u32(trace->raw_fleets[fleet->id], (StarsU16)(76U + slot * 4U)) :
            (StarsI32)source->state.minerals[slot];
    bank = fleet->owner == 1 ? trace->designs : trace->complete_input.views[0].ship_designs[0];
    for (ship = 0U; ship < 16U; ++ship) {
        const StarsU8 *design = bank + ship * 147U;
        StarsU8 hull[143];
        StarsShipComponentStat components[16];
        StarsShipDesignStat shape;
        StarsHullStat capacity;
        StarsEstFuelSlot *entry = &current.slots[ship];
        StarsU16 engine_slot = 16U;
        entry->cargo_count = fleet->owner == 1 ?
            get_u16(trace->raw_fleets[fleet->id], (StarsU16)(12U + ship * 2U)) : source->state.ship_counts[ship];
        if (!entry->cargo_count) continue;
        if (design[122] > 16U || !scrap_read_hull(trace, get_u16(design, 0U), hull, sizeof hull)) {
            ++trace->failures; return 0L;
        }
        for (slot = 0U; slot < design[122]; ++slot) {
            components[slot].group = get_u16(design, (StarsU16)(58U + slot * 4U));
            components[slot].packed_count_type = get_u16(design, (StarsU16)(60U + slot * 4U));
            if (engine_slot == 16U && components[slot].group == 1U) engine_slot = slot;
        }
        shape.components = components; shape.component_count = design[122];
        capacity.cargo_max = get_u16(hull, 52U); capacity.fuel_max = get_u16(hull, 54U);
        entry->ship_stat = stars_wt_max_ship_design_stat(&shape, &capacity, 2);
        entry->hull_fuel_weight = get_u16(design, 40U);
        if (engine_slot < design[122] &&
                (components[engine_slot].packed_count_type >> 8) >=
                (get_u16(hull, (StarsU16)(60U + engine_slot * 4U)) >> 8)) {
            const StarsU8 *engine;
            entry->engine_id = (StarsI16)(components[engine_slot].packed_count_type & 255U);
            engine = scrap_part_record(1U, (StarsU16)entry->engine_id);
            if (!engine) { ++trace->failures; return 0L; }
            entry->engine_available = 1;
            for (warp = 0U; warp < 12U; ++warp)
                entry->fuel_used[warp] = (StarsI16)get_u16(engine, (StarsU16)(54U + warp * 2U));
            scoops[ship].ship_count = (StarsU16)entry->cargo_count;
            scoops[ship].scoop_count = design[61U];
            for (warp = 0U; warp < 11U; ++warp)
                scoops[ship].fuel_used[warp] = entry->fuel_used[warp] != 0;
        }
    }
    input.fleet = &current; input.warp = order->warp_level; input.travel = travel;
    input.race_grbit = (StarsI16)(get_u32(trace->complete_input.views[0].players[fleet->owner], 78U) & 1UL);
    input.game_flags = &trace->game_flags;
    cost = stars_est_fuel_use(&input);
    *gain = stars_fuel_gain_from_ram_scoops(scoops, 16U, order->warp_level, travel);
    return cost;
}

static StarsI32 fuel_use(void *opaque, const StarsMoveFleet *fleet,
    const StarsMoveFleetOrder *order, StarsI32 travel)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsI32 gain = 0L;
    StarsI32 cost = current_fuel_estimate(opaque, fleet, order, travel, &gain);
    ++trace->move_fuel_calls; trace->fuel_distance = travel;
    if (gain != 0L) ++trace->failures;
    /* The pinned moving fleet has enough fuel and positive fuel use. These
     * guards prevent the semantic mover's fuel-shortage/zero-cost fallbacks
     * from silently standing in for the original branches. */
    if (cost <= 0L || cost > fleet->fuel) ++trace->failures;
    return cost;
}

/* AI speed planning reads the current AI view, before host turn loading. */
typedef struct StartupSpeed {
    TurinTrace *trace;
    StarsU16 id;
    StarsFuelWaypointOrder orders[2];
} StartupSpeed;

static const StarsReadPlanetState *speed_planet(StartupSpeed *speed, StarsI16 id)
{
    const Soe3InputView *view = &speed->trace->complete_input.views[2];
    StarsU16 i;
    for (i = 0; i < view->planet_count; ++i)
        if (view->planets[i].id == id) return &view->planets[i];
    return 0;
}
static StarsI16 speed_planet_at(StartupSpeed *speed, StarsPoint point)
{
    StarsI16 id = -1;
    StarsU16 i;
    /* FFindNearestObject mask 0x81: exact coordinates, last matching planet. */
    for (i = 0; i < TURIN_RETAINED_POINT_COUNT; ++i)
        if (speed->trace->points[i].x == point.x &&
                speed->trace->points[i].y == point.y) id = (StarsI16)i;
    return id;
}
static StarsI16 speed_nearest(void *opaque, StarsPoint point,
    StarsI16 *owner, StarsI16 *id)
{
    StartupSpeed *speed = (StartupSpeed *)opaque;
    const StarsReadPlanetState *planet;
    *id = speed_planet_at(speed, point);
    planet = speed_planet(speed, *id);
    if (!planet) return 0;
    *owner = planet->player; return 1;
}
static StarsI16 speed_ideal(void *opaque, StarsI16 ignore)
{
    StartupSpeed *speed = (StartupSpeed *)opaque;
    StarsI16 counts[16]; StarsU16 i;
    for (i = 0; i < 16U; ++i) counts[i] = (StarsI16)get_u16(
        speed->trace->raw_fleets[speed->id], (StarsU16)(12U + 2U * i));
    return current_ideal_warp(speed->trace, counts, ignore);
}
static double speed_distance(void *opaque, StarsI16 x1, StarsI16 y1,
    StarsI16 x2, StarsI16 y2)
{
    (void)opaque; return stars_get_distance(x1, y1, x2, y2);
}
static StarsI16 speed_floor_distance(void *opaque, StarsPoint from, StarsPoint to)
{
    return (StarsI16)speed_distance(opaque, from.x, from.y, to.x, to.y);
}
static StarsI32 speed_estimate(StartupSpeed *speed, StarsI16 warp,
    StarsI32 travel, StarsI32 *gain)
{
    StarsMoveFleet fleet;
    StarsMoveFleetOrder order;
    memset(&fleet, 0, sizeof fleet); memset(&order, 0, sizeof order);
    fleet.id = (StarsI16)speed->id; fleet.owner = 1;
    fleet.fuel = (StarsI32)get_u32(speed->trace->raw_fleets[speed->id], 92U);
    order.target_id = speed->orders[1].target_id; order.warp_level = warp;
    /* EstFuelUse's -1 path truncates DGetDistance (SHIP before A76E). */
    if (travel == -1L) travel = (StarsI16)speed_distance(speed,
        speed->orders[0].x, speed->orders[0].y,
        speed->orders[1].x, speed->orders[1].y);
    return current_fuel_estimate(speed->trace, &fleet, &order, travel, gain);
}
static StarsI32 speed_fuel_estimate(void *opaque, const StarsFuelWaypointFleet *fleet,
    StarsI16 segment, StarsI16 warp, StarsI32 travel, StarsI16 flag)
{
    StartupSpeed *speed = (StartupSpeed *)opaque;
    StarsI32 gain = 0;
    (void)fleet;
    if (segment != 0 || flag != 0) ++speed->trace->failures;
    return speed_estimate(speed, warp, travel, &gain);
}
static StarsI32 speed_scoop(void *opaque, const StarsFuelWaypointFleet *fleet,
    StarsI16 warp, StarsI32 travel)
{
    StarsI32 gain = 0;
    (void)fleet;
    (void)speed_estimate((StartupSpeed *)opaque, warp, travel, &gain);
    return gain;
}
static StarsI16 speed_hull(void *opaque, const StarsFuelWaypointFleet *fleet,
    StarsI16 slot)
{
    StartupSpeed *speed = (StartupSpeed *)opaque;
    (void)fleet;
    return (StarsI16)get_u16(speed->trace->designs, (StarsU16)(slot * 147U));
}
static StarsI16 speed_planet_info(void *opaque, StarsI16 id, StarsI16 *owner,
    StarsU16 *flags, StarsI16 *cargo)
{
    StartupSpeed *speed = (StartupSpeed *)opaque;
    const StarsReadPlanetState *planet = speed_planet(speed, id);
    StarsU16 slot; StarsU8 hull[143];
    if (!planet) return 0;
    *owner = planet->player; *flags = planet->flags; *cargo = 0;
    if (*owner == 1 && (*flags & 0x0200U)) {
        slot = (StarsU16)(planet->opaque_44 & 15UL);
        if (slot >= 10U || !scrap_read_hull(speed->trace,
                get_u16(speed->trace->starbase_designs, (StarsU16)(slot * 147U)),
                hull, sizeof hull)) { ++speed->trace->failures; return 0; }
        *cargo = (StarsI16)get_u16(hull, 52U);
    }
    return 1;
}
static StarsI16 speed_refuel(void *opaque, StarsI16 id)
{
    StarsI16 owner, cargo; StarsU16 flags;
    return speed_planet_info(opaque, id, &owner, &flags, &cargo) &&
        owner == 1 && (flags & 0x0200U) && cargo != 0;
}
static StarsI32 speed_fuel(void *opaque, StarsI16 warp)
{
    StartupSpeed *speed = (StartupSpeed *)opaque;
    StarsFuelWaypointFleet fleet; StarsFuelWaypointOps ops;
    StarsFuelUseToWaypointInput input;
    StarsFuelWaypointOrder orders[2]; StarsU16 i;
    memset(&fleet, 0, sizeof fleet); memset(&ops, 0, sizeof ops);
    memset(&input, 0, sizeof input); memcpy(orders, speed->orders, sizeof orders);
    orders[1].flags = (StarsU16)((orders[1].flags & 0xff0fU) | ((warp & 15U) << 4));
    fleet.owner = 1; fleet.orders = orders;
    for (i = 0; i < 16U; ++i) fleet.cargo[i] = get_u16(
        speed->trace->raw_fleets[speed->id], (StarsU16)(12U + i * 2U));
    ops.estimate_fuel = speed_fuel_estimate; ops.ram_scoop_gain = speed_scoop;
    ops.cargo_hull_type = speed_hull; ops.planet_info = speed_planet_info;
    ops.precise_distance = speed_distance; ops.context = speed;
    input.fleet = &fleet; input.waypoint_count = 1; input.max_cargo = 1;
    input.ops = &ops; ++speed->trace->basic_speed_fuel_calls;
    return stars_fuel_use_to_waypoint(&input);
}
static StarsI32 speed_capacity(void *opaque, StarsI16 which)
{
    StartupSpeed *speed = (StartupSpeed *)opaque;
    if (which != 1) ++speed->trace->failures;
    return move_current_capacity(speed->trace, 1, speed->id, 0);
}
static StarsI16 speed_has_gate(StartupSpeed *speed, const StarsReadPlanetState *planet)
{
    const StarsU8 *design; StarsU16 slot, i;
    if (!planet || !(planet->flags & 0x0200U)) return 0;
    if (planet->player != 1) { ++speed->trace->failures; return 0; }
    slot = (StarsU16)(planet->opaque_44 & 15UL);
    if (slot >= 10U) { ++speed->trace->failures; return 0; }
    design = speed->trace->starbase_designs + slot * 147U;
    if (design[122] > 16U) { ++speed->trace->failures; return 0; }
    for (i = 0; i < design[122]; ++i)
        if (get_u16(design, (StarsU16)(58U + i * 4U)) == 0x0200U &&
                design[61U + i * 4U] && design[60U + i * 4U] < 7U) return 1;
    return 0;
}
static StarsI16 speed_gate(void *opaque, StarsPoint from, StarsPoint to)
{
    StartupSpeed *speed = (StartupSpeed *)opaque;
    const StarsReadPlanetState *src, *dst;
    StarsI16 id = speed_planet_at(speed, to);
    StarsU16 i, j;
    if (id < 0) return 0;
    dst = speed_planet(speed, id);
    if (dst && dst->player == 1 && !speed_has_gate(speed, dst)) return 0;
    if (dst && dst->player == -1 && dst->turn ==
            get_u16(speed->trace->complete_input.game, 18U)) return 0;
    src = speed_planet(speed, speed_planet_at(speed, from));
    /* Pinned source planet has no gate and the fleet lacks a jumpgate.
     * Reject newly eligible jumps rather than silently replacing their checks. */
    if (!src || src->player != 1 || speed_has_gate(speed, src)) {
        ++speed->trace->failures; return 0;
    }
    for (i = 0; i < 16U; ++i) {
        const StarsU8 *design = speed->trace->designs + i * 147U;
        if (!get_u16(speed->trace->raw_fleets[speed->id], (StarsU16)(12U + 2U*i))) continue;
        if (design[122] > 16U) { ++speed->trace->failures; return 0; }
        for (j = 0; j < design[122]; ++j)
            if (get_u16(design, (StarsU16)(58U+4U*j)) == 0x1000U &&
                    design[61U+4U*j] && design[60U+4U*j] == 9U) break;
        if (j == design[122]) return 0;
    }
    ++speed->trace->failures; return 0;
}
static StarsU16 speed_best(void *opaque, const StarsU8 *fleet, const StarsMoveOrder *unused)
{
    StartupSpeed *speed = (StartupSpeed *)opaque;
    StarsWarpBestState state; StarsWarpBestOps ops;
    const StarsReadPlanetState *planet; StarsU16 i, j;
    (void)unused;
    memset(&state, 0, sizeof state); memset(&ops, 0, sizeof ops);
    state.order_index = 1; state.order_count = speed->trace->plord[speed->id][3];
    state.order_warp = (StarsI16)((speed->orders[1].flags >> 4) & 15U);
    state.previous_warp = (StarsI16)((speed->orders[0].flags >> 4) & 15U);
    state.order_kind = (StarsI16)(speed->orders[1].flags & 15U);
    state.target_kind = (StarsI16)((speed->orders[1].flags >> 8) & 15U);
    state.previous_point.x = speed->orders[0].x; state.previous_point.y = speed->orders[0].y;
    state.order_point.x = speed->orders[1].x; state.order_point.y = speed->orders[1].y;
    state.fleet_player = 1; state.current_player = 1;
    state.ai_mode = speed->trace->ai_input.ai_active;
    state.fuel_on_board = (StarsI32)get_u32(fleet, 92U);
    for (i = 0; i < 16U; ++i) {
        const StarsU8 *design = speed->trace->designs + i * 147U;
        if (!get_u16(fleet, (StarsU16)(12U+2U*i))) continue;
        if (design[122] > 16U) { ++speed->trace->failures; return 0; }
        for (j = 0; j < design[122]; ++j)
            if (get_u16(design, (StarsU16)(58U+4U*j)) == 0x1000U &&
                    design[60U+4U*j] <= 1U) state.colony_hull = 1;
    }
    planet = speed_planet(speed, speed_planet_at(speed, state.order_point));
    if (planet && planet->player == 1) {
        i = (StarsU16)(planet->opaque_44 & 15UL);
        if (i >= 10U) { ++speed->trace->failures; return 0; }
        state.destination_disables_ai_limit = get_u16(speed->trace->starbase_designs,
            (StarsU16)(i * 147U)) != 32U;
    }
    ops.ideal_warp = speed_ideal; ops.nearest_planet = speed_nearest;
    ops.fuel_to_waypoint = speed_fuel; ops.fleet_stat = speed_capacity;
    ops.has_cargo_hull = speed_refuel; ops.distance_term = speed_floor_distance;
    ops.can_use_gate = speed_gate; state.ops = &ops; state.context = speed;
    return (StarsU16)stars_iwarp_best_for_waypoint(&state);
}
static StarsI16 speed_current(void *opaque, const StarsU8 *fleet)
{
    StartupSpeed *speed = (StartupSpeed *)opaque;
    (void)fleet; return (StarsI16)((get_u16(speed->trace->plord[speed->id], 28U) >> 4) & 15U);
}
static void speed_publish(void *opaque, const StarsU8 *fleet, StarsU16 warp)
{
    StartupSpeed *speed = (StartupSpeed *)opaque;
    StarsU8 *order = speed->trace->plord[speed->id] + 22U;
    (void)fleet;
    put_u16(order, 6U, (StarsU16)((get_u16(order, 6U) & 0xff0fU) | ((warp & 15U) << 4)));
    /* Preserve a visible failure until the changed-speed fleet publication
     * branch is bound; the pinned pair is expected to retain its speed. */
    ++speed->trace->failures;
}
static void startup_keep_speed(void *opaque, const StarsU8 *fleet,
    StarsI16 parameter, StarsI16 count, const StarsU8 *const *things)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StartupSpeed speed; StarsSetAiFleetIdealSpeedOps ops; StarsU16 i;
    memset(&speed, 0, sizeof speed); memset(&ops, 0, sizeof ops);
    speed.trace = trace; speed.id = get_u16(fleet, 0U);
    if (count != 0 || speed.id >= TURIN_RETAINED_FLEET_COUNT ||
            get_u16(fleet, 2U) != 1U || trace->plord[speed.id][3] < 2U) {
        ++trace->failures; return;
    }
    for (i = 0; i < 2U; ++i) {
        const StarsU8 *order = trace->plord[speed.id] + 4U + 18U*i;
        speed.orders[i].x = (StarsI16)get_u16(order, 0U);
        speed.orders[i].y = (StarsI16)get_u16(order, 2U);
        speed.orders[i].target_id = (StarsI16)get_u16(order, 4U);
        speed.orders[i].flags = get_u16(order, 6U);
    }
    startup_select_current_object(trace, 2, (StarsI16)speed.id);
    ops.best_warp = speed_best; ops.current_speed = speed_current; ops.set_speed = speed_publish;
    stars_set_ai_fleet_ideal_speed(fleet, parameter, count, things, 0U, 1, &ops, &speed);
    ++trace->basic_speed_calls;
}

static StarsI16 get_owner0_order(void *opaque, const StarsMoveFleet *fleet,
    StarsMoveFleetOrder *out)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    const Soe3InputView *host = &trace->complete_input.views[0];
    StarsU16 index;
    if (!fleet || !out || fleet->owner != 0 || fleet->id < 0 ||
            fleet->id >= (StarsI16)TURIN_HST_OWNER0_INPUT_FLEET_COUNT) {
        ++trace->failures;
        return 0;
    }
    for (index = 0U; index < host->fleet_count; ++index) {
        const Soe3InputFleet *source = &host->fleets[index];
        if (source->owner == 0 &&
                (source->id_word & 511U) == (StarsU16)fleet->id) {
            /* Original 10b0:33ec..3413 skips fleets without a next order.
             * All loaded owner-0 survivors take this branch in this pair. */
            if (source->state.order_count < 2U) return 0;
            ++trace->failures;
            return 0;
        }
    }
    ++trace->failures;
    return 0;
}

static StarsI16 turn_read_hst_owner0_fleet(TurinTrace *trace,
    StarsU16 index, StarsMoveFleet *fleet)
{
    const Soe3InputView *host;
    const Soe3InputFleet *loaded = 0;
    StarsReadFleetState state;
    StarsU16 slot;
    StarsU16 id_word;
    if (!trace || !fleet || !trace->complete_input_loaded ||
            index >= TURIN_HST_OWNER0_INPUT_FLEET_COUNT) return 0;
    host = &trace->complete_input.views[0];
    for (slot = 0U; slot < host->fleet_count; ++slot)
        if (host->fleets[slot].owner == 0 &&
                (host->fleets[slot].id_word & 0x01ffU) == index)
            loaded = &host->fleets[slot];
    if (!loaded || loaded->state.order_count != 1U) return 0;
    id_word = loaded->id_word;
    state = loaded->state;

    memcpy(trace->turn_owner0_input_orders[index], state.orders,
        STARS_AI_PLORD_ORDER_SIZE);
    memset(fleet, 0, sizeof *fleet);
    fleet->id = (StarsI16)(id_word & 0x01ffU);
    fleet->owner = 0;
    fleet->x = state.x;
    fleet->y = state.y;
    fleet->target_id = state.planet;
    fleet->fuel = (StarsI32)state.minerals[4];
    fleet->fuel_capacity = move_current_capacity(trace, 0, index, &state);
    fleet->active = (state.fleet_flags & 0x0400U) == 0U;
    ++trace->turn_m1_fleet_read_calls;
    return 1;
}

static void run_retained_owner0_move_fleets(TurinTrace *trace)
{
    StarsMoveFleetsOps ops;
    StarsMoveFleetsInput input;
    StarsMoveFleet *fleets = trace->turn_owner0_moving_fleets;
    StarsU16 index;
    memset(&ops, 0, sizeof ops);
    memset(&input, 0, sizeof input);
    memset(fleets, 0, sizeof trace->turn_owner0_moving_fleets);
    ops.get_order = get_owner0_order;
    ops.distance = distance;
    ops.movement_limit = movement_limit;
    ops.fuel_use = fuel_use;
    ops.context = trace;
    for (index = 0U; index < TURIN_HST_OWNER0_INPUT_FLEET_COUNT; ++index)
        if (!turn_read_hst_owner0_fleet(trace, index, &fleets[index])) {
            ++trace->failures;
            return;
        }
    input.fleets = fleets;
    input.fleet_count = TURIN_HST_OWNER0_INPUT_FLEET_COUNT;
    input.post_movement = 0;
    input.ops = &ops;
    stars_move_fleets(&input);
    for (index = 0U; index < TURIN_HST_OWNER0_INPUT_FLEET_COUNT; ++index) {
        if (index == 0U && trace->seeded_action) {
            if (fleets[index].active || fleets[index].status != 0)
                ++trace->failures;
            continue;
        }
        if (!fleets[index].active || fleets[index].status != 0 ||
                fleets[index].target_id !=
                    retained_hst_owner0_input_planets[index] ||
                fleets[index].x != retained_hst_owner0_input_points[index][0] ||
                fleets[index].y != retained_hst_owner0_input_points[index][1] ||
                fleets[index].fuel != (StarsI32)
                    retained_hst_owner0_input_cargo[index][4])
            ++trace->failures;
    }
}

static void check_at(TurinTrace *trace, int condition, int line)
{
    if (!condition) {
        fprintf(stderr, "check failed at line %d\n", line);
        ++trace->failures;
    }
}
#define check(trace, condition) check_at((trace), (condition), __LINE__)

static void check_scrap_loaded_fixture(TurinTrace *trace)
{
    check(trace, get_u16(trace->designs + 2U * 147U, 0x2aU) == 123U &&
        get_u16(trace->designs + 2U * 147U, 0x2cU) == 41U &&
        get_u16(trace->designs + 2U * 147U, 0x2eU) == 0U &&
        get_u16(trace->designs + 2U * 147U, 0x30U) == 12U);
    check(trace, (get_u16(trace->plord[2], 10U) & 0x100fU) == 0x1005U);
    check(trace, !trace->raw_fleet_present[3] &&
        get_u16(trace->raw_fleets[2], 16U) == 2U &&
        get_u32(trace->raw_fleets[2], 92U) == 420UL);
    ++trace->pre_scrap_checks_done;
}

static StarsI16 run_retained_population_growth(TurinTrace *trace)
{
    TurinPopulationSummary *summary = &trace->population_summary;
    StarsCalcPlanetMaxPopInput input;
    StarsU8 *race_data = trace->guess_race_data_storage;
    StarsU16 axis;
    StarsI16 player_index;
    const Soe3InputView *host = &trace->complete_input.views[0];
    const StarsReadPlanetState *planet0 = trace->seeded_mode ?
        &trace->turn_planet_states[10] : &host->planets[10];
    const StarsReadPlanetState *planet1 = trace->seeded_mode ?
        &trace->turn_planet_states[24] : &host->planets[24];
    StarsI16 race0 = (StarsI16)(signed char)host->players[0][76];
    StarsI16 race1 = (StarsI16)(signed char)host->players[1][76];
    if (planet0->id != 10 || planet0->player != 0 || planet1->id != 24 ||
            planet1->player != 1 || race0 == 8 || race1 == 8) ++trace->failures;
    memset(race_data, 0, sizeof trace->guess_race_data_storage);
    for (player_index = 0; player_index < 2; ++player_index)
        memcpy(race_data + 0x59a2U + (StarsU16)player_index * 192U,
            host->players[player_index], 192U);
    trace->guess_race_data = race_data;
    for (axis = 0; axis < 3U; ++axis)
        race_data[12U + axis] = planet0->environment[axis];
    summary->planet10_desirability = stars_planet_desirability(
        race_data, 0U, 0);
    input.planet_owner = 0;
    input.player = 0;
    input.race_stat = race0;
    input.race_grbit = (StarsI16)((get_u32(host->players[0], 78U) >> 9) & 1UL);
    input.desirability = summary->planet10_desirability;
    input.starbase_population = 0L;
    input.has_starbase = 0U;
    summary->planet10_maximum_population =
        stars_calc_planet_max_pop(&input);
    summary->planet10.owner = 0;
    summary->planet10.desirability = summary->planet10_desirability;
    summary->planet10.population = (StarsI32)planet0->minerals[3];
    summary->planet10.fraction = (StarsI16)(planet0->opaque_20[0] & 0xffUL);
    summary->planet10_growth_rate = (StarsI16)((signed char)host->players[0][25] *
        (race0 == 0 ? 2 : 1));
    summary->planet10_population_growth = stars_change_population(
        &summary->planet10, summary->planet10_maximum_population,
        summary->planet10_growth_rate, 0, 1);
    for (axis = 0; axis < 3U; ++axis)
        race_data[12U + axis] = planet1->environment[axis];
    summary->desirability = stars_planet_desirability(race_data, 0U, 1);
    input.planet_owner = 1;
    input.player = 1;
    input.race_stat = race1;
    input.race_grbit = (StarsI16)((get_u32(host->players[1], 78U) >> 9) & 1UL);
    input.desirability = summary->desirability;
    input.starbase_population = 0L;
    input.has_starbase = 0U;
    summary->maximum_population = stars_calc_planet_max_pop(&input);
    summary->supply_population_after_transfer =
        trace->supply_planet.minerals[3];
    summary->owner1.owner = 1;
    summary->owner1.desirability = summary->desirability;
    summary->owner1.population =
        (StarsI32)trace->supply_planet.minerals[3];
    summary->owner1.fraction = (StarsI16)(planet1->opaque_20[0] & 0xffUL);
    summary->growth_rate = (StarsI16)((signed char)host->players[1][25] *
        (race1 == 0 ? 2 : 1));
    summary->population_growth = stars_change_population(&summary->owner1,
        summary->maximum_population, summary->growth_rate, 0, 1);
    trace->supply_planet.minerals[3] =
        (StarsU32)summary->owner1.population;
    trace->guess_runtime_populations[0] =
        (StarsU32)summary->planet10.population;
    trace->guess_runtime_populations[1] =
        (StarsU32)summary->owner1.population;
    check(trace, summary->desirability == 100);
    check(trace, summary->maximum_population == 10000L);
    check(trace, summary->population_growth == 31L);
    check(trace, trace->supply_planet.minerals[3] ==
        TURIN_ORIGINAL_PLANET_COLONISTS);
    check(trace, summary->owner1.fraction ==
        TURIN_ORIGINAL_POPULATION_FRACTION);
    check(trace, summary->planet10_desirability == 100 &&
        summary->planet10_maximum_population == 12000L);
    check(trace, summary->planet10_population_growth == 37L);
    check(trace, summary->planet10.population ==
        TURIN_PLANET10_POPULATION_EXPECTED);
    check(trace, summary->planet10.fraction == TURIN_PLANET10_FRACTION_EXPECTED);
    check(trace, summary->planet10_growth_rate == 15 &&
        TURIN_PLANET10_PLAYER_IDEAL_GROWTH == 15);
    return 7;
}

static void run_retained_move_fleets(TurinTrace *trace)
{
    StarsMoveFleetsOps ops;
    StarsMoveFleetsInput input;
    StarsMoveFleet *fleets = trace->turn_moving_fleets;
    StarsU16 index;
    memset(&ops, 0, sizeof ops);
    memset(&input, 0, sizeof input);
    memset(fleets, 0, sizeof trace->turn_moving_fleets);
    ops.get_order = get_order;
    ops.distance = distance;
    ops.movement_limit = movement_limit;
    ops.fuel_use = fuel_use;
    ops.context = trace;
    for (index = 0U; index < TURIN_RETAINED_FLEET_COUNT; ++index) {
        const StarsU8 *raw = trace->raw_fleets[index];
        if (!trace->raw_fleet_present[index]) continue;
        fleets[index].id = (StarsI16)get_u16(raw, 0U);
        fleets[index].owner = (StarsI16)get_u16(raw, 2U);
        fleets[index].x = (StarsI16)get_u16(raw, 8U);
        fleets[index].y = (StarsI16)get_u16(raw, 10U);
        fleets[index].target_id = (StarsI16)get_u16(raw, 6U);
        fleets[index].fuel = (StarsI32)get_u32(raw, 92U);
        fleets[index].fuel_capacity = move_current_capacity(trace, 1, index, 0);
        fleets[index].active = (StarsI16)(trace->raw_fleet_present[index] &&
            (get_u16(raw, 4U) & 0x0400U) == 0U);
        fleets[index].warp_level = (StarsI16)(
            (get_u16(trace->plord[index], 10U) >> 4) & 15U);
    }
    input.fleets = fleets;
    input.fleet_count = TURIN_RETAINED_FLEET_COUNT;
    input.post_movement = 0;
    input.ops = &ops;
    stars_move_fleets(&input);
    run_retained_owner0_move_fleets(trace);
}

static StarsI16 turn_read_m2_fleet(TurinTrace *trace, StarsU16 index)
{
    const Soe3InputView *view;
    const Soe3InputFleet *loaded = 0;
    StarsReadFleetState state;
    StarsU16 slot;
    StarsU16 ship;
    StarsU16 lane;
    StarsU16 raw_offset;
    StarsU16 id_word;
    StarsI16 owner;
    const StarsU8 *order_body;
    if (!trace || !trace->complete_input_loaded ||
            index >= TURIN_M2_INPUT_FLEET_COUNT) return 0;
    view = &trace->complete_input.views[2];
    for (slot = 0U; slot < view->fleet_count; ++slot)
        if (view->fleets[slot].owner == 1 &&
                (view->fleets[slot].id_word & 0x01ffU) == index)
            loaded = &view->fleets[slot];
    if (!loaded || loaded->state.order_count != 1U) return 0;
    id_word = loaded->id_word;
    owner = loaded->owner;
    state = loaded->state;
    order_body = state.orders;

    pack_input_fleet(trace->raw_fleets[index], loaded);
    trace->raw_fleet_present[index] = 1;
    memset(&trace->fleet_storage[index], 0,
        sizeof trace->fleet_storage[index]);
    memcpy(trace->fleet_storage[index].bytes,
        trace->raw_fleets[index], sizeof trace->raw_fleets[index]);
    trace->fleet_items[index] = &trace->fleet_storage[index];

    memset(&trace->fleets[index], 0, sizeof trace->fleets[index]);
    trace->fleets[index].id = (StarsI16)(id_word & 0x01ffU);
    trace->fleets[index].owner = owner;
    trace->fleets[index].planet_id = state.planet;
    trace->fleets[index].cord = (StarsI16)state.order_count;
    for (ship = 0U; ship < 16U; ++ship)
        trace->fleets[index].ship_counts[ship] =
            (StarsI16)state.ship_counts[ship];
    for (lane = 0U; lane < 5U; ++lane)
        trace->fleets[index].current_cargo[lane] =
            (StarsI32)state.minerals[lane];
    memset(trace->plord[index], 0, sizeof trace->plord[index]);
    trace->plord[index][3] = (StarsU8)state.order_count;
    memcpy(trace->plord[index] + STARS_AI_PLORD_HEADER_SIZE,
        state.orders, state.order_count * STARS_AI_PLORD_ORDER_SIZE);
    trace->fleets[index].raw_plord = trace->plord[index];

    for (ship = 0U; ship < 16U; ++ship) {
        raw_offset = (StarsU16)(12U + ship * 2U);
        if (get_u16(trace->raw_fleets[index], raw_offset) !=
                (StarsU16)retained_ship_counts[index][ship])
            return 0;
    }
    for (lane = 0U; lane < 5U; ++lane)
        if (get_u32(trace->raw_fleets[index],
                (StarsU16)(76U + lane * 4U)) !=
                (StarsU32)retained_fleet_cargo[index][lane])
            return 0;
    if (memcmp(trace->raw_fleets[index], retained_raw_fleets[index],
            sizeof trace->raw_fleets[index]) != 0)
        return 0;
    for (raw_offset = 0U; raw_offset < 8U; ++raw_offset) {
        StarsU8 expected_order = order_body[raw_offset];
        if (raw_offset == 6U) expected_order = (StarsU8)(expected_order & 0xdfU);
        if (state.orders[raw_offset] != expected_order)
            return 0;
    }
    for (raw_offset = 8U; raw_offset < STARS_AI_PLORD_ORDER_SIZE;
            ++raw_offset)
        if (state.orders[raw_offset] != 0U) return 0;
    ++trace->turn_m2_fleet_read_calls;
    return 1;
}

static StarsI16 turn_read_player_planet_inputs(TurinTrace *trace,
    StarsI16 player)
{
    const Soe3InputView *source;
    StarsReadPlanetState *states;
    StarsU16 count;
    StarsU16 index;
    StarsU16 planet_id;
    StarsI16 failures_before;
    if (!trace || !trace->complete_input_loaded) return 0;
    failures_before = trace->failures;
    if (player == 0) {
        states = trace->turn_m1_planet_states;
        count = TURIN_M1_PLANET_RECORD_COUNT;
    } else if (player == 1) {
        states = trace->turn_m2_planet_states;
        count = TURIN_M2_PLANET_INPUT_COUNT;
    } else {
        ++trace->failures;
        return 0;
    }
    source = &trace->complete_input.views[player + 1];
    if (source->planet_count != count) return 0;
    for (index = 0U; index < count; ++index) {
        states[index] = source->planets[index];
        planet_id = (StarsU16)states[index].id;
        if ((player == 0 && planet_id == 10U) ||
                (player == 1 && planet_id == 24U)) {
            const StarsReadPlanetState *hst_state =
                &trace->turn_planet_states[planet_id];
            StarsU16 mineral;
            if (states[index].player != hst_state->player ||
                    (states[index].flags & 0xf7ffU) !=
                        (hst_state->flags & 0xf7ffU) ||
                    states[index].routing_word != hst_state->routing_word ||
                    states[index].opaque_20[0] != hst_state->opaque_20[0])
                ++trace->failures;
            for (mineral = 0U; mineral < 4U; ++mineral)
                if (states[index].minerals[mineral] !=
                        hst_state->minerals[mineral])
                    ++trace->failures;
        }
        if (player == 0)
            ++trace->turn_m1_planet_read_calls;
        else
            ++trace->turn_m2_planet_read_calls;
    }
    return (StarsI16)(trace->failures == failures_before);
}

static StarsI16 turn_load_player_log(void *opaque,
    StarsGenerateTurnState *state, StarsI16 player)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    if (!state || player < 0 || player > 1 ||
            !turn_read_player_planet_inputs(trace, player) ||
            !read_input_player(trace, (StarsU16)(player + 2))) {
        ++trace->failures;
        return 0;
    }
    if (player == 1 && !turn_read_m2_shdef_input(trace)) {
        ++trace->failures;
        return 0;
    }
    /* FGenerateTurn sets the absent-log sentinel immediately before each
     * player's load. FLoadLogFile replaces it only for a matching log. */
    put_u32(trace->turn_serials[player], 0U, 0xffffffffUL);
    if (player == 1 && !startup_replay_memory_logs(trace)) return 0;
    ++trace->turn_load_log_calls;
    return 1;
}

static void turn_validate_player_logs(void *opaque, StarsGenerateTurnState *state)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsU16 i, j;
    StarsU16 merged_seen = 0U;
    if (!state || !state->logs_loaded || state->player_count != 2 ||
            trace->turn_load_log_calls != 2 || trace->turn_waypoint_source_calls != 0) {
        ++trace->failures; return;
    }
    for (i = 0U; i < trace->complete_input.views[0].fleet_count; ++i) {
        const Soe3InputFleet *fleet = &trace->complete_input.views[0].fleets[i];
        StarsU16 id = (StarsU16)(fleet->id_word & 511U);
        if (fleet->owner == 1 && (id == 2U || id == 3U)) {
            const StarsU8 *raw = trace->raw_fleets[id];
            merged_seen = (StarsU16)(merged_seen | (1U << (id - 2U)));
            if (fleet->state.fleet_flags != (StarsU16)(get_u16(raw, 4U) |
                    (trace->raw_fleet_present[id] ? 0U : 0x0400U)))
                ++trace->failures;
            for (j = 0U; j < 32U; ++j)
                if (fleet->state.ship_counts[j] != get_u16(raw,
                        (StarsU16)(12U + j * 2U))) ++trace->failures;
            for (j = 0U; j < 5U; ++j)
                if (fleet->state.minerals[j] != get_u32(raw,
                        (StarsU16)(76U + j * 4U))) ++trace->failures;
        }
    }
    if (merged_seen != 3U) ++trace->failures;
    /* Original FGenerateTurn's post-log serial/duplicate pass. wMdPlr
     * (+6) and wFlags (+84) are distinct words in the loaded PLAYER. */
    for (i = 0U; i < 2U; ++i) {
        StarsU8 *player = trace->complete_input.views[0].players[i];
        StarsU16 flags = get_u16(player, 84U);
        StarsU32 serial = get_u32(trace->turn_serials[i], 0U);
        if ((flags & 2U) || (get_u16(player, 6U) & 0x0200U) ||
                ((trace->game_flags & 0x0800U) && i == 0U)) {
            put_u16(player, 84U, (StarsU16)(flags & 0xfffbU));
            continue;
        }
        if (serial == 0xffffffffUL) continue;
        if (!stars_file_valid_serial_long(original_dgroup, serial)) {
            put_u16(player, 84U, (StarsU16)(flags | 4U));
            continue;
        }
        put_u16(player, 84U, (StarsU16)(flags & 0xfffbU));
        for (j = 0U; j < i; ++j) {
            StarsU8 *other = trace->complete_input.views[0].players[j];
            if (!(get_u16(other, 84U) & 2U) && !(get_u16(other, 6U) & 0x0200U) &&
                    serial == get_u32(trace->turn_serials[j], 0U) &&
                    memcmp(trace->turn_serials[i] + 4U, trace->turn_serials[j] + 4U, 11U)) {
                put_u16(player, 84U, (StarsU16)(get_u16(player, 84U) | 4U));
                put_u16(other, 84U, (StarsU16)(get_u16(other, 84U) | 4U));
            }
        }
    }
    /* No flagged player is eligible for the ensuing original messages in
     * either pinned input. Fail explicitly if this bounded adapter changes. */
    for (i = 0U; i < 2U; ++i)
        if (get_u16(trace->complete_input.views[0].players[i], 84U) & 4U)
            ++trace->failures;
    for (i = 0U; i < 2U; ++i) {
        for (j = 0U; j < 16U; ++j) {
            StarsU8 *design = i == 1U ? trace->designs + j * 147U :
                trace->complete_input.views[0].ship_designs[i] + j * 147U;
            stars_prepare_turn_design(design);
            if (i == 1U)
                memcpy(trace->complete_input.views[0].ship_designs[i] + j * 147U, design, 147U);
            ++trace->turn_prepared_designs;
        }
    }
    for (i = 0U; i < trace->complete_input.views[0].fleet_count; ++i) {
        Soe3InputFleet *fleet = &trace->complete_input.views[0].fleets[i];
        StarsU16 id = fleet->id_word & 511U;
        StarsU16 count = fleet->state.order_count;
        StarsU16 flags = fleet->state.fleet_flags;
        const StarsU8 *first = fleet->state.orders;
        if (fleet->owner == 1) {
            if (id >= 4U) { ++trace->failures; return; }
            count = trace->plord[id][3]; first = trace->plord[id] + 4U;
            flags = get_u16(trace->raw_fleets[id], 4U);
        }
        flags = stars_prepare_turn_fleet_flags(flags, count, count ? get_u16(first, 6U) : 0U);
        fleet->state.fleet_flags = flags;
        if (fleet->owner == 1) put_u16(trace->raw_fleets[id], 4U, flags);
        /* These current orders target planets. A follow loop must never be
         * silently skipped if another input makes it eligible. */
        if (flags & 0x8000U) ++trace->failures;
        ++trace->turn_prepared_fleets;
    }
    ++trace->turn_serial_validation_calls;
}

/* The pinned orders target planets. Fleet/thing replacement and its
 * message/tie-breaking services must not be silently approximated if new
 * inputs make those branches eligible. */
static void turn_validate_current_waypoints(TurinTrace *trace)
{
    StarsWaypointFleet fleets[10];
    StarsWaypointThing things[4];
    StarsWaypointRecord orders[20];
    Soe3InputFleet *sources[10];
    StarsMoveFleet *moving[10];
    StarsU8 *raw_orders[20];
    StarsU8 *first_orders[10];
    StarsI16 original_x[10], original_y[10];
    StarsU16 owner_offsets[3];
    StarsU16 fleet_count = 0U, order_count = 0U;
    StarsU16 index, owner, j, id, count, flags;
    StarsI16 stage = trace->turn_waypoint_source_calls;
    StarsI16 limit = (StarsI16)((get_u16(trace->complete_input.game, 4U) + 1U) * 400U);
    memset(fleets, 0, sizeof fleets); memset(things, 0, sizeof things);
    memset(orders, 0, sizeof orders); memset(moving, 0, sizeof moving);
    if (stage >= 3) { ++trace->failures; return; }
    for (index = 0U; index < trace->complete_input.views[0].thing_count; ++index) {
        const StarsU8 *raw = trace->complete_input.views[0].things[index];
        things[index].id = (StarsI16)get_u16(raw, 0U);
        things[index].flags = get_u16(raw, 0U);
        things[index].x = (StarsI16)get_u16(raw, 2U);
        things[index].y = (StarsI16)get_u16(raw, 4U);
    }
    for (owner = 0U; owner < 2U; ++owner) {
        owner_offsets[owner] = order_count;
        for (index = 0U; index < trace->complete_input.views[0].fleet_count; ++index) {
            Soe3InputFleet *source = &trace->complete_input.views[0].fleets[index];
            StarsU8 *raw;
            if (source->owner != (StarsI16)owner) continue;
            id = (StarsU16)(source->id_word & 511U);
            if (owner == 1U) {
                if (id >= 4U) { ++trace->failures; return; }
                if (!trace->raw_fleet_present[id]) continue;
                flags = get_u16(trace->raw_fleets[id], 4U);
                raw = trace->plord[id] + STARS_AI_PLORD_HEADER_SIZE;
                count = trace->plord[id][3];
            } else {
                flags = source->state.fleet_flags;
                raw = source->state.orders; count = source->state.order_count;
            }
            if (flags & 0x0400U) continue;
            if (fleet_count >= 10U) { ++trace->failures; return; }
            if (trace->turn_move_fleets_calls) {
                moving[fleet_count] = turn_current_moving_fleet(trace, source);
                if (!moving[fleet_count]) continue;
            }
            if (fleet_count >= 10U || order_count + count > 20U || count == 0U) {
                ++trace->failures; return;
            }
            sources[fleet_count] = source;
            fleets[fleet_count].id = (StarsI16)source->id_word;
            fleets[fleet_count].player = source->owner;
            fleets[fleet_count].flags = flags;
            fleets[fleet_count].x = moving[fleet_count] ? moving[fleet_count]->x :
                (owner == 1U ? (StarsI16)get_u16(trace->raw_fleets[id], 8U) : source->state.x);
            fleets[fleet_count].y = moving[fleet_count] ? moving[fleet_count]->y :
                (owner == 1U ? (StarsI16)get_u16(trace->raw_fleets[id], 10U) : source->state.y);
            first_orders[fleet_count] = raw;
            original_x[fleet_count] = fleets[fleet_count].x;
            original_y[fleet_count] = fleets[fleet_count].y;
            ++fleet_count;
            for (j = 0U; j < count; ++j) {
                StarsWaypointRecord *order = &orders[order_count];
                StarsU8 *record = raw + j * STARS_AI_PLORD_ORDER_SIZE;
                order->x = (StarsI16)get_u16(record, 0U);
                order->y = (StarsI16)get_u16(record, 2U);
                order->target_id = (StarsI16)get_u16(record, 4U);
                order->type_flags = get_u16(record, 6U);
                if (((order->type_flags >> 8) & 15U) != 1U) {
                    fprintf(stderr, "unsupported waypoint owner=%u fleet=%u order=%u flags=%x\n",
                        owner, id, j, order->type_flags);
                    ++trace->failures; return;
                }
                raw_orders[order_count++] = record;
            }
        }
    }
    owner_offsets[2] = order_count;
    for (owner = 0U; owner < 2U; ++owner)
        if (stars_validate_waypoints(fleets, fleet_count, orders + owner_offsets[owner],
                (StarsU16)(owner_offsets[owner + 1U] - owner_offsets[owner]),
                things, trace->complete_input.views[0].thing_count, limit, (StarsI16)owner))
            ++trace->failures;
    for (index = 0U; index < order_count; ++index) {
        put_u16(raw_orders[index], 0U, (StarsU16)orders[index].x);
        put_u16(raw_orders[index], 2U, (StarsU16)orders[index].y);
        put_u16(raw_orders[index], 4U, (StarsU16)orders[index].target_id);
        put_u16(raw_orders[index], 6U, orders[index].type_flags);
    }
    for (index = 0U; index < fleet_count; ++index) {
        Soe3InputFleet *source = sources[index];
        id = (StarsU16)(source->id_word & 511U);
        if (original_x[index] != fleets[index].x)
            put_u16(first_orders[index], 0U, (StarsU16)fleets[index].x);
        if (original_y[index] != fleets[index].y)
            put_u16(first_orders[index], 2U, (StarsU16)fleets[index].y);
        source->state.x = fleets[index].x; source->state.y = fleets[index].y;
        if (moving[index]) { moving[index]->x = fleets[index].x; moving[index]->y = fleets[index].y; }
        if (source->owner == 1) {
            put_u16(trace->raw_fleets[id], 8U, (StarsU16)fleets[index].x);
            put_u16(trace->raw_fleets[id], 10U, (StarsU16)fleets[index].y);
        }
    }
    trace->turn_waypoint_fleets[stage] = (StarsI16)fleet_count;
    trace->turn_waypoint_orders[stage] = (StarsI16)order_count;
    ++trace->turn_waypoint_source_calls;
}

static void turn_validate_waypoints(void *opaque,
    StarsGenerateTurnState *state)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    (void)state;
    turn_validate_current_waypoints(trace);
    ++trace->turn_validate_calls;
}

static void turn_set_battle_id(void *opaque, StarsU16 battle_id)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    trace->turn_battle_id = battle_id;
    ++trace->turn_battle_calls;
}

static void turn_publish_order_planets(TurinTrace *trace)
{
    StarsU16 i;
    for (i = 0U; i < trace->complete_input.views[0].planet_count; ++i) {
        const StarsOrdersPlanet *order = &trace->turn_order_planets[i];
        if (order->id != i) { ++trace->failures; return; }
        trace->turn_planet_states[i].flags = order->flags;
        trace->turn_planet_states[i].turn = order->turn;
    }
}

static void turn_visit_remaining_orders(TurinTrace *trace, StarsI16 pass)
{
    Soe3InputView *world = &trace->complete_input.views[0];
    StarsSatisfyFleet fleet;
    StarsSatisfyOrder order;
    StarsSatisfyOrdersInput input;
    StarsSatisfyOrdersOps ops;
    StarsU16 i;
    memset(&input, 0, sizeof input); memset(&ops, 0, sizeof ops);
    input.fleets = &fleet; input.fleet_count = 1U; input.pass = pass;
    input.ops = &ops; ops.context = trace;
    for (i = 0U; i < world->fleet_count; ++i) {
        Soe3InputFleet *current = &world->fleets[i];
        StarsU16 id = (StarsU16)(current->id_word & 511U);
        StarsU8 *first = current->state.orders;
        StarsU16 count = current->state.order_count;
        memset(&fleet, 0, sizeof fleet); memset(&order, 0, sizeof order);
        fleet.id = (StarsI16)id; fleet.owner = current->owner;
        fleet.flags = current->state.fleet_flags;
        fleet.turn_flags = (StarsU16)(current->state.overlap_116[0] >> 16);
        if (current->owner == 1) {
            if (id >= TURIN_RETAINED_FLEET_COUNT) { ++trace->failures; return; }
            fleet.flags = get_u16(trace->raw_fleets[id], 4U);
            fleet.turn_flags = get_u16(trace->raw_fleets[id], 118U);
            if (!trace->raw_fleet_present[id]) fleet.flags |= 0x0400U;
            first = trace->plord[id] + 4U; count = trace->plord[id][3];
        }
        if (pass == 1 && !(fleet.flags & 0x0400U) && !(fleet.turn_flags & 0x0080U) &&
                count != 0U && (get_u16(first, 6U) & 15U) == 5U) {
            StarsU16 mineral;
            if (trace->turn_task5_visit_count >= 2U) { ++trace->failures; return; }
            trace->turn_task5_visit_ids[trace->turn_task5_visit_count++] = current->id_word;
            if (current->owner == 1 && id == 2U) {
                trace->scrap_loaded = scrap_load_inputs(trace);
                if (!trace->scrap_loaded) { ++trace->failures; return; }
                check_scrap_loaded_fixture(trace);
                trace->scrap_result = run_task5_scrap(trace);
                for (mineral = 0U; mineral < 4U; ++mineral)
                    trace->turn_planet_states[24].minerals[mineral] =
                        get_u32(trace->scrap_planet, (StarsU16)(28U + mineral * 4U));
                trace->turn_planet_states[24].flags = get_u16(trace->scrap_planet, 4U);
                fleet.flags = get_u16(trace->raw_fleets[id], 4U);
                fleet.turn_flags = get_u16(trace->raw_fleets[id], 118U);
            } else if (current->owner == 0 && id == 0U) {
                if (!run_owner0_task5_scrap(trace)) { ++trace->failures; return; }
                fleet.flags = current->state.fleet_flags;
                fleet.turn_flags = (StarsU16)(current->state.overlap_116[0] >> 16);
            } else { ++trace->failures; return; }
        }
        if (!(fleet.flags & 0x0400U) && !(fleet.turn_flags & 0x0080U)) {
            if (count == 0U || (get_u16(first, 6U) & 15U) != 0U) {
                /* Task 5 ran at its current table position. Any other
                 * active task needs its real services before proceeding. */
                ++trace->failures; return;
            }
            order.kind = 0; order.active = (get_u16(first, 6U) & 0x1000U) != 0U;
            fleet.orders = &order; fleet.order_count = 1U;
            ++trace->turn_passive_orders[pass - 1];
        }
        stars_satisfy_orders(&input);
        current->state.fleet_flags = fleet.flags;
        current->state.overlap_116[0] = (current->state.overlap_116[0] & 0xffffUL) |
            ((StarsU32)fleet.turn_flags << 16);
        if (current->owner == 1) {
            put_u16(trace->raw_fleets[id], 4U, fleet.flags);
            put_u16(trace->raw_fleets[id], 118U, fleet.turn_flags);
        }
        ++trace->turn_order_fleet_visits[pass - 1];
    }
}

static void turn_satisfy_orders(void *opaque, StarsI16 pass)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsI16 index = trace->turn_order_pass_calls;
    StarsU16 planet;
    if (index < 0 || index >= 4) {
        ++trace->failures;
        return;
    }
    /* DoOrders updates planet flags before any pass and resets the turn
     * field before the post-movement passes. Publish those source writes
     * before order handlers read the shared world. */
    turn_publish_order_planets(trace);
    trace->turn_order_passes[index] = pass;
    ++trace->turn_order_pass_calls;
    turn_visit_remaining_orders(trace, pass);
    /* Preserve any order-handler mutation before the next source pass. */
    for (planet = 0U; planet < trace->complete_input.views[0].planet_count; ++planet) {
        trace->turn_order_planets[planet].player = trace->turn_planet_states[planet].player;
        trace->turn_order_planets[planet].flags = trace->turn_planet_states[planet].flags;
        trace->turn_order_planets[planet].turn = trace->turn_planet_states[planet].turn;
    }
}

static void turn_order_thing_interactions(void *opaque, StarsI16 post_move)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    const Soe3InputView *world = &trace->complete_input.views[0];
    StarsThingInteractionThing things[4];
    StarsThingInteractionFleet fleets[10];
    StarsThingInteractionPlanet planets[32];
    StarsThingInteractionPlayer players[2];
    StarsU16 i, lane, count = 0U, phase = post_move ? 1U : 0U;
    if (world->thing_count > 4U || world->fleet_count > 10U || world->planet_count > 32U) {
        ++trace->failures; return;
    }
    memset(things, 0, sizeof things); memset(fleets, 0, sizeof fleets);
    memset(planets, 0, sizeof planets); memset(players, 0, sizeof players);
    for (i = 0U; i < world->thing_count; ++i) {
        const StarsU8 *raw = world->things[i];
        things[i].overlap = get_u16(raw, 0U);
        things[i].x = (StarsI16)get_u16(raw, 2U);
        things[i].y = (StarsI16)get_u16(raw, 4U);
        /* Original DoThingInteractions considers kind 3 only. The pinned
         * world contains wormholes (kind 2); trader effects are unbound and
         * must fail explicitly if a different input activates them. */
        if ((things[i].overlap >> 13) != 2U) { ++trace->failures; return; }
    }
    for (i = 0U; i < world->fleet_count; ++i) {
        const Soe3InputFleet *fleet = &world->fleets[i];
        if (fleet->state.fleet_flags & 0x0400U) continue;
        fleets[count].id = fleet->id_word;
        fleets[count].player = fleet->owner;
        fleets[count].planet = fleet->state.planet;
        fleets[count].x = fleet->state.x; fleets[count].y = fleet->state.y;
        fleets[count].overlap = fleet->state.fleet_flags;
        for (lane = 0U; lane < 3U; ++lane)
            fleets[count].minerals[lane] = fleet->state.minerals[lane];
        for (lane = 0U; lane < 16U; ++lane) {
            fleets[count].design_counts[lane] = fleet->state.ship_counts[lane];
            fleets[count].ship_count = (StarsU16)(fleets[count].ship_count + fleet->state.ship_counts[lane]);
        }
        ++count;
    }
    for (i = 0U; i < world->planet_count; ++i) {
        const StarsReadPlanetState *planet = &trace->turn_planet_states[i];
        planets[i].id = (StarsU16)planet->id; planets[i].owner = planet->player;
        planets[i].overlap = planet->flags;
        planets[i].x = trace->complete_input.coordinates[i][0];
        planets[i].y = trace->complete_input.coordinates[i][1];
        for (lane = 0U; lane < 3U; ++lane) planets[i].minerals[lane] = planet->minerals[lane];
    }
    for (i = 0U; i < 2U; ++i) {
        memcpy(players[i].technology, world->players[i] + 26U, 6U);
        players[i].detail_flags = get_u16(world->players[i], 6U);
    }
    stars_do_thing_interactions(post_move, things, world->thing_count,
        fleets, count, planets, world->planet_count, players, 0, trace,
        (StarsI16)get_u16(trace->complete_input.game, 18U));
    ++trace->turn_interaction_calls[phase];
    trace->turn_interaction_things[phase] = world->thing_count;
}

static void turn_link_current_battle_fleets(TurinTrace *trace)
{
    StarsLinkFleet nodes[10], *items[10];
    Soe3InputView *world = &trace->complete_input.views[0];
    StarsU16 i, j;
    if (world->fleet_count > 10U) { ++trace->failures; return; }
    memset(nodes, 0, sizeof nodes);
    for (i = 0U; i < world->fleet_count; ++i) {
        nodes[i].x = world->fleets[i].state.x;
        nodes[i].y = world->fleets[i].state.y;
        nodes[i].flags = world->fleets[i].state.fleet_flags;
        if (world->fleets[i].owner == 1) {
            StarsU16 id = (StarsU16)(world->fleets[i].id_word & 511U);
            if (id >= TURIN_RETAINED_FLEET_COUNT) { ++trace->failures; return; }
            nodes[i].flags = get_u16(trace->raw_fleets[id], 4U);
            if (!trace->raw_fleet_present[id]) nodes[i].flags |= 0x0400U;
        }
        items[i] = &nodes[i];
    }
    stars_link_fleet_records(items, world->fleet_count, &trace->startup_log_flags);
    trace->turn_battle_link_count = world->fleet_count;
    for (i = 0U; i < world->fleet_count; ++i) {
        trace->turn_battle_links[i] = 0xffffU;
        if (nodes[i].next) {
            for (j = 0U; j < world->fleet_count; ++j)
                if (nodes[i].next == &nodes[j]) trace->turn_battle_links[i] = j;
            if (trace->turn_battle_links[i] == 0xffffU) ++trace->failures;
        }
        /* Retain intermediate flags; DoBattles publishes the final flags
         * after eligibility has marked each position group. */
        trace->turn_battle_link_flags[i] = nodes[i].flags;
    }
    ++trace->turn_battle_link_calls;
}

typedef struct TurnBattleContext {
    TurinTrace *trace;
    StarsDoBattlesFleet fleets[10];
} TurnBattleContext;

static StarsI16 turn_battle_design_teeth(TurinTrace *trace, const StarsU8 *design)
{
    StarsHullComponent components[16];
    StarsU16 i;
    if (design[122] > 16U) { ++trace->failures; return 0; }
    for (i = 0U; i < design[122]; ++i) {
        components[i].attributes = get_u16(design, (StarsU16)(58U + i * 4U));
        components[i].packed_damage = get_u16(design, (StarsU16)(60U + i * 4U));
    }
    return stars_hull_has_teeth(components, design[122]);
}

static StarsI16 turn_battle_relation(void *opaque, StarsI16 owner, StarsI16 other)
{
    TurnBattleContext *context = (TurnBattleContext *)opaque;
    if (owner < 0 || owner > 1 || other < 0 || other > 1) {
        ++context->trace->failures; return 0;
    }
    return context->trace->complete_input.views[0].players[owner][112U + other];
}

static void turn_battle_link(void *opaque, StarsI16 post_move)
{
    TurnBattleContext *context = (TurnBattleContext *)opaque;
    TurinTrace *trace = context->trace;
    StarsU16 i;
    if (post_move != 1) { ++trace->failures; return; }
    turn_link_current_battle_fleets(trace);
    for (i = 0U; i < trace->turn_battle_link_count; ++i) {
        context->fleets[i].flags = trace->turn_battle_link_flags[i];
        context->fleets[i].has_next = trace->turn_battle_links[i] != 0xffffU;
    }
}

static StarsI16 turn_battle_eligibility(void *opaque, const StarsDoBattlesFleet *first,
    StarsU16 *attacks, StarsU16 *players, StarsU16 *spectators)
{
    TurnBattleContext *context = (TurnBattleContext *)opaque;
    TurinTrace *trace = context->trace;
    Soe3InputView *world = &trace->complete_input.views[0];
    StarsCplrBattleInput input;
    StarsCplrBattleFleet group[10];
    StarsU16 indices[10], count = 0U, current, head, slot, plan;
    StarsI16 result;
    const StarsReadPlanetState *planet;
    memset(&input, 0, sizeof input); memset(group, 0, sizeof group);
    head = (StarsU16)(first - context->fleets); current = head;
    do {
        const Soe3InputFleet *fleet;
        const StarsU8 *bank;
        if (current >= world->fleet_count || count >= 10U) {
            ++trace->failures; return 0;
        }
        fleet = &world->fleets[current];
        if (fleet->owner < 0 || fleet->owner > 1 || fleet->state.design_plan >= 16U ||
                !world->battle_plan_present[fleet->owner][fleet->state.design_plan]) {
            ++trace->failures; return 0;
        }
        plan = get_u16(world->battle_plans[fleet->owner][fleet->state.design_plan], 2U);
        group[count].player = fleet->owner; group[count].planet_id = fleet->state.planet;
        group[count].flags = context->fleets[current].flags;
        group[count].plan_attacker = (StarsU8)(plan & 15U);
        group[count].plan_target = (StarsU8)((plan >> 8) & 31U);
        bank = fleet->owner == 1 ? trace->designs : world->ship_designs[0];
        for (slot = 0U; slot < 16U; ++slot) {
            const StarsU8 *design = bank + slot * 147U;
            group[count].ships[slot] = fleet->state.ship_counts[slot];
            if (group[count].ships[slot] && design[123] == 7U &&
                    turn_battle_design_teeth(trace, design)) group[count].has_teeth = 1;
        }
        indices[count++] = current; current = trace->turn_battle_links[current];
    } while (current != head);
    input.planet_owner = -1;
    if (group[0].planet_id >= 0) {
        const StarsU8 *bank;
        if (group[0].planet_id >= 32) { ++trace->failures; return 0; }
        planet = &trace->turn_planet_states[group[0].planet_id];
        input.planet_owner = planet->player;
        input.planet_is_starbase = (planet->flags & 0x0200U) != 0U;
        if (input.planet_is_starbase) {
            if (planet->player < 0 || planet->player > 1 || !world->battle_plan_present[planet->player][0]) {
                ++trace->failures; return 0;
            }
            slot = (StarsU16)(planet->opaque_44 & 15U);
            if (slot >= 10U) { ++trace->failures; return 0; }
            bank = planet->player == 1 ? trace->starbase_designs : world->starbase_designs[0];
            input.planet_has_teeth = turn_battle_design_teeth(trace, bank + slot * 147U);
            input.planet_target = (StarsU8)((get_u16(world->battle_plans[planet->player][0], 2U) >> 8) & 31U);
        }
    }
    input.fleets = group; input.fleet_count = (StarsI16)count; input.player_count = 2;
    input.attack_masks = attacks; input.player_mask = players; input.spectator_mask = spectators;
    input.relation = turn_battle_relation; input.context = context;
    result = stars_cplr_battle(&input);
    for (current = 0U; current < count; ++current)
        context->fleets[indices[current]].flags = group[current].flags;
    ++trace->turn_battle_eligibility_calls;
    return result;
}

static void turn_battle_unexpected(void *opaque, const StarsDoBattlesFleet *fleet,
    StarsI16 result, const StarsU16 *attacks, StarsU16 players, StarsU16 spectators)
{
    TurnBattleContext *context = (TurnBattleContext *)opaque;
    (void)fleet; (void)result; (void)attacks; (void)players; (void)spectators;
    ++context->trace->turn_battle_output_blocks;
    ++context->trace->failures;
}

static StarsI16 turn_bomb_unexpected(void *opaque, const StarsDoBombingFleet *fleet,
    StarsDoBombingDamage *damage)
{
    TurnBattleContext *context = (TurnBattleContext *)opaque;
    (void)fleet; (void)damage; ++context->trace->failures; return 0;
}

static void turn_battle_bombing(void *opaque)
{
    TurnBattleContext *context = (TurnBattleContext *)opaque;
    TurinTrace *trace = context->trace;
    const Soe3InputView *world = &trace->complete_input.views[0];
    StarsDoBombingInput input;
    StarsDoBombingFleet fleets[10];
    StarsDoBombingPlanet planets[32];
    StarsU16 i;
    memset(&input, 0, sizeof input); memset(fleets, 0, sizeof fleets);
    memset(planets, 0, sizeof planets);
    for (i = 0U; i < world->fleet_count; ++i) {
        fleets[i].id = (StarsI16)world->fleets[i].id_word;
        fleets[i].player = world->fleets[i].owner;
        fleets[i].planet_id = world->fleets[i].state.planet;
        fleets[i].flags = context->fleets[i].flags;
    }
    for (i = 0U; i < world->planet_count; ++i) {
        planets[i].id = trace->turn_planet_states[i].id;
        planets[i].owner = trace->turn_planet_states[i].player;
        planets[i].flags = trace->turn_planet_states[i].flags;
    }
    input.fleets = fleets; input.fleet_count = world->fleet_count;
    input.planets = planets; input.planet_count = world->planet_count;
    input.calculate = turn_bomb_unexpected; input.context = context;
    stars_do_bombing(&input);
    ++trace->turn_bombing_calls;
}

static void *turn_battle_allocate(void *opaque, StarsU16 size, StarsU16 tag)
{
    TurnBattleContext *context = (TurnBattleContext *)opaque;
    void *memory;
    if (tag != 6U) ++context->trace->failures;
    memory = malloc(size);
    if (!memory) ++context->trace->failures;
    return memory;
}

static void turn_battle_free(void *opaque, void *memory, StarsU16 tag)
{
    (void)opaque; (void)tag; free(memory);
}

static void turn_do_current_battles(void *opaque, StarsI16 post_move)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    TurnBattleContext context;
    StarsDoBattlesInput input;
    StarsU8 current[2];
    StarsU16 i;
    memset(&context, 0, sizeof context); memset(&input, 0, sizeof input);
    if (trace->complete_input.views[0].fleet_count > 10U) { ++trace->failures; return; }
    context.trace = trace; input.context = &context;
    input.fleets = context.fleets; input.fleet_count = trace->complete_input.views[0].fleet_count;
    input.link_fleets = turn_battle_link; input.allocate = turn_battle_allocate;
    input.free_memory = turn_battle_free; input.cplr_battle = turn_battle_eligibility;
    input.cool_battle = turn_battle_unexpected; input.do_bombing = turn_battle_bombing;
    input.battle_current = current;
    stars_do_battles(post_move, &input);
    if (get_u16(current, 0U) != 0xffffU) ++trace->failures;
    for (i = 0U; i < input.fleet_count; ++i) {
        Soe3InputFleet *fleet = &trace->complete_input.views[0].fleets[i];
        StarsU16 id = (StarsU16)(fleet->id_word & 511U);
        fleet->state.fleet_flags = context.fleets[i].flags;
        if (fleet->owner == 1 && id < TURIN_RETAINED_FLEET_COUNT)
            put_u16(trace->raw_fleets[id], 4U, context.fleets[i].flags);
    }
}

static StarsI16 turn_unexpected_drop_planet(void *opaque, StarsI16 id,
    StarsColonistPlanet *planet)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    (void)id; (void)planet; ++trace->failures; return 0;
}

static void turn_consume_colonist_drops(void *opaque, StarsI16 argument)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsDropColonistsInput input;
    StarsDropColonistsOps ops;
    if (argument != 0 || trace->turn_drop_count != 0U ||
            (trace->turn_order_pass_calls != 1 && trace->turn_order_pass_calls != 3)) {
        ++trace->failures; return;
    }
    memset(&input, 0, sizeof input); memset(&ops, 0, sizeof ops);
    ops.lookup_planet = turn_unexpected_drop_planet; ops.context = trace;
    input.drops = trace->turn_drops; input.drop_count = trace->turn_drop_count;
    input.ops = &ops;
    stars_drop_colonists(&input);
    ++trace->turn_drop_calls;
}

static StarsI16 turn_unexpected_transfer_owner(void *opaque, StarsI16 group,
    StarsU16 id, StarsI16 *owner)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    (void)group; (void)id; (void)owner; ++trace->failures; return 0;
}

static StarsI32 turn_unexpected_transfer_cargo(void *opaque, StarsI16 group,
    StarsU16 id, StarsI16 supply, StarsI32 amount)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    (void)group; (void)id; (void)supply; (void)amount;
    ++trace->failures; return 0L;
}

static void turn_unexpected_transfer_message(void *opaque, const StarsTransferMessage *message)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    (void)message; ++trace->failures;
}

static void turn_consume_transfers(void *opaque, StarsI16 argument)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsTransferToOthersInput input;
    if (argument != 0 || trace->turn_transfer_count != 0U || trace->turn_order_pass_calls != 2) {
        ++trace->failures; return;
    }
    memset(&input, 0, sizeof input);
    input.records = trace->turn_transfers; input.record_count = trace->turn_transfer_count;
    input.lookup_owner = turn_unexpected_transfer_owner;
    input.apply_cargo = turn_unexpected_transfer_cargo;
    input.send_message = turn_unexpected_transfer_message; input.context = trace;
    stars_transfer_to_others(&input);
    ++trace->turn_transfer_calls;
}

static void turn_order_research(void *, StarsI16);

static void turn_do_orders(void *opaque, StarsGenerateTurnState *state,
    StarsI16 mode)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsDoOrdersInput input;
    StarsI16 index = mode != 0 ? 1 : 0;
    StarsU16 planet;
    memset(&input, 0, sizeof input);
    for (planet = 0U; planet < trace->complete_input.views[0].planet_count; ++planet) {
        const StarsReadPlanetState *current = &trace->turn_planet_states[planet];
        trace->turn_order_planets[planet].id = (StarsU16)current->id;
        trace->turn_order_planets[planet].player = current->player;
        trace->turn_order_planets[planet].flags = current->flags;
        trace->turn_order_planets[planet].turn = current->turn;
    }
    input.planets = trace->turn_order_planets;
    input.planet_count = (StarsI16)trace->complete_input.views[0].planet_count;
    input.game_turn = (StarsU16)state->turn;
    input.set_battle_id = turn_set_battle_id;
    input.do_battles = turn_do_current_battles;
    input.do_thing_interactions = turn_order_thing_interactions;
    input.update_research = turn_order_research;
    input.drop_colonists = turn_consume_colonist_drops;
    input.transfer_to_others = turn_consume_transfers;
    input.satisfy_orders = turn_satisfy_orders;
    input.context = trace;
    ++trace->turn_orders_calls[index];
    stars_do_orders(mode, &input);
    turn_publish_order_planets(trace);
}

static void turn_unmark_mine_fields(void *opaque,
    StarsGenerateTurnState *state)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsU16 index;
    (void)state;
    for (index = 0; index < TURIN_RETAINED_THING_COUNT; ++index)
        memcpy(trace->turn_thing_bytes + index * 18U,
            trace->complete_input.views[0].things[index], 18U);
    stars_unmark_mine_fields(trace->turn_thing_bytes,
        TURIN_RETAINED_THING_COUNT);
    ++trace->turn_unmark_mine_calls;
    for (index = 0; index < TURIN_RETAINED_THING_COUNT; ++index)
        if (memcmp(trace->turn_thing_bytes + index * 18U,
                retained_wormholes[index], 18U) != 0)
            ++trace->failures;
}

static void turn_thing_decay_free(void *opaque,
    StarsThingDecayThing *thing)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    (void)thing;
    ++trace->turn_thing_decay_free_calls;
}

static void turn_thing_decay(void *opaque, StarsGenerateTurnState *state)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsThingDecayInput input;
    StarsThingDecayFleet fleets[10];
    Soe3InputFleet *sources[10];
    StarsU16 fleet_count = 0U;
    StarsThingDecayThing things[TURIN_RETAINED_THING_COUNT];
    StarsU16 index;
    StarsU16 identity;
    (void)state;
    memset(&input, 0, sizeof input);
    memset(fleets, 0, sizeof fleets);
    memset(things, 0, sizeof things);
    for (index = 0U; index < trace->complete_input.views[0].fleet_count; ++index) {
        Soe3InputFleet *source = &trace->complete_input.views[0].fleets[index];
        StarsMoveFleet *moving = turn_current_moving_fleet(trace, source);
        StarsU16 id = (StarsU16)(source->id_word & 511U);
        if (!moving) continue;
        if (fleet_count >= 10U) { ++trace->failures; return; }
        sources[fleet_count] = source;
        fleets[fleet_count].x = moving->x;
        fleets[fleet_count].y = moving->y;
        fleets[fleet_count].status_flags = source->owner == 1 ?
            get_u16(trace->raw_fleets[id], 4U) : source->state.fleet_flags;
        ++fleet_count;
    }
    if (fleet_count != (trace->rototill_seeded_mode ? 10U :
            (trace->seeded_action ? 7U : 8U))) ++trace->failures;
    for (index = 0; index < TURIN_RETAINED_THING_COUNT; ++index) {
        const StarsU8 *raw = trace->turn_thing_bytes + index * 18U;
        identity = get_u16(raw, 0U);
        things[index].kind = (StarsU16)(identity >> 13);
        things[index].owner = (StarsI16)((identity >> 9) & 0x0fU);
        things[index].x = (StarsI16)get_u16(raw, 2U);
        things[index].y = (StarsI16)get_u16(raw, 4U);
        things[index].active = 1U;
        if (things[index].kind != 2U)
            ++trace->failures;
    }
    input.fleets = fleets;
    input.fleet_count = fleet_count;
    input.things = things;
    input.thing_count = TURIN_RETAINED_THING_COUNT;
    input.ops.free_thing = turn_thing_decay_free;
    input.context = trace;
    stars_thing_decay(&input);
    ++trace->turn_thing_decay_calls;
    if (trace->turn_thing_decay_free_calls != 0)
        ++trace->failures;
    for (index = 0U; index < fleet_count; ++index) {
        Soe3InputFleet *source = sources[index];
        StarsU16 id = (StarsU16)(source->id_word & 511U);
        if ((fleets[index].status_flags & 0x1000U) != 0U)
            ++trace->failures;
        source->state.fleet_flags = fleets[index].status_flags;
        if (source->owner == 1)
            put_u16(trace->raw_fleets[id], 4U, fleets[index].status_flags);
    }
}

static void turn_spank_cheaters_message(void *opaque, StarsI16 player,
    StarsI16 message, StarsI16 severity, StarsI32 object, StarsI32 value,
    StarsI16 extra)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    (void)player;
    (void)message;
    (void)severity;
    (void)object;
    (void)value;
    (void)extra;
    ++trace->failures;
}

static void turn_spank_cheaters(void *opaque, StarsGenerateTurnState *state)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsSpankCheatersInput input;
    StarsSeeds seeds_before = trace->seeds;
    StarsU8 dead_players[16];
    StarsI16 random_calls_before = trace->random_calls;
    StarsI16 player;
    memset(&input, 0, sizeof input);
    memset(dead_players, 0, sizeof dead_players);
    if (!state || state->player_count < 0 || state->player_count > 16 ||
            state->turn != 0)
        ++trace->failures;
    if (state) {
        for (player = 0; player < state->player_count && player < 16;
                ++player)
            dead_players[player] =
                trace->players[player].dead != 0 ? 1U : 0U;
        input.turn = state->turn;
        input.player_count = state->player_count;
    }
    input.dead_players = dead_players;
    input.random = random_value;
    input.send_message = turn_spank_cheaters_message;
    input.context = trace;
    ++trace->turn_spank_cheaters_calls;
    stars_spank_the_cheaters(&input);
    if (trace->random_calls != random_calls_before ||
            trace->seeds.first != seeds_before.first ||
            trace->seeds.second != seeds_before.second)
        ++trace->failures;
}

/* Locate mutable host state by identity, never by an expected output row. */
static Soe3InputFleet *turn_host_fleet(TurinTrace *trace, StarsU16 owner,
    StarsU16 id)
{
    StarsU16 index;
    Soe3InputView *host = &trace->complete_input.views[0];
    for (index = 0U; index < host->fleet_count; ++index)
        if (host->fleets[index].owner == (StarsI16)owner &&
                (host->fleets[index].id_word & 0x01ffU) == id)
            return &host->fleets[index];
    ++trace->failures;
    return 0;
}

static StarsI16 turn_loaded_race_stat(void *opaque, StarsI16 player,
    StarsI16 stat)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    if (player < 0 || player >= 2 || stat != 14) {
        ++trace->failures;
        return 0;
    }
    return (StarsI16)(signed char)trace->complete_input.views[0].players[player][76];
}

static StarsMoveFleet *turn_current_moving_fleet(TurinTrace *trace,
    const Soe3InputFleet *source)
{
    StarsU16 id = (StarsU16)(source->id_word & 0x01ffU);
    StarsMoveFleet *moving;
    if (source->owner == 0 && id < 6U)
        moving = &trace->turn_owner0_moving_fleets[id];
    else if (source->owner == 1 && id < TURIN_RETAINED_FLEET_COUNT)
        moving = &trace->turn_moving_fleets[id];
    else {
        ++trace->failures;
        return 0;
    }
    return moving->active ? moving : 0;
}

static StarsI32 current_fleet_capacity(void *opaque, const StarsFuelFleet *fleet,
    StarsI16 statistic)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsFleetStatEntry entries[16];
    StarsShipDesignStat designs[16];
    StarsHullStat hulls[16];
    StarsShipComponentStat components[16][16];
    StarsU16 ship, slot;
    const StarsU8 *bank = fleet->player == 1 ? trace->designs :
        trace->complete_input.views[0].ship_designs[0];
    memset(entries, 0, sizeof entries);
    for (ship = 0U; ship < 16U; ++ship) {
        const StarsU8 *design = bank + ship * 147U;
        StarsU8 hull[143];
        if (!fleet->ship_counts[ship]) continue;
        if (design[122] > 16U || !scrap_read_hull(trace,
                get_u16(design, 0U), hull, sizeof hull)) {
            ++trace->failures;
            return 0L;
        }
        for (slot = 0U; slot < design[122]; ++slot) {
            components[ship][slot].group = get_u16(design,
                (StarsU16)(58U + 4U * slot));
            components[ship][slot].packed_count_type = get_u16(design,
                (StarsU16)(60U + 4U * slot));
        }
        designs[ship].component_count = design[122];
        designs[ship].components = components[ship];
        hulls[ship].fuel_max = get_u16(hull, 54U);
        hulls[ship].cargo_max = get_u16(hull, 52U);
        entries[ship].ship_count = (StarsI16)fleet->ship_counts[ship];
        entries[ship].design = &designs[ship];
        entries[ship].hull = &hulls[ship];
    }
    return stars_get_fleet_stat(entries, 16U, fleet->status_flags, statistic);
}

static StarsI32 turn_fuel_capacity(void *opaque, const StarsFuelFleet *fleet)
{
    return current_fleet_capacity(opaque, fleet, 1);
}

static StarsI16 turn_fuel_can_refuel(void *opaque, const StarsFuelFleet *fleet)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsReadPlanetState *planet;
    const StarsU8 *bank;
    StarsU8 hull[143];
    StarsU16 slot;
    if (fleet->destination < 0) return 0;
    if (fleet->destination >= TURIN_RETAINED_POINT_COUNT) {
        ++trace->failures; return 0;
    }
    planet = &trace->turn_planet_states[fleet->destination];
    if (!(planet->flags & 0x0200U) || planet->player < 0) return 0;
    if (planet->player > 1 || fleet->player < 0 || fleet->player > 1) {
        ++trace->failures; return 0;
    }
    /* The base owner's relationship towards the fleet owner controls access. */
    if (planet->player != fleet->player &&
            trace->complete_input.views[0].players[planet->player]
                [112U + fleet->player] != 1U) return 0;
    slot = (StarsU16)(planet->opaque_44 & 15U);
    bank = planet->player == 1 ? trace->starbase_designs :
        trace->complete_input.views[0].starbase_designs[0];
    if (slot >= 10U || !scrap_read_hull(trace,
            get_u16(bank, (StarsU16)(slot * 147U)), hull, sizeof hull)) {
        ++trace->failures; return 0;
    }
    return get_u16(hull, 52U) != 0U;
}

static void turn_set_fuel(void *opaque, StarsFuelFleet *fleet, StarsI32 value)
{
    (void)opaque;
    fleet->fuel = value;
}

static void turn_fuel_fleets(void *opaque, StarsGenerateTurnState *state)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    Soe3InputView *host = &trace->complete_input.views[0];
    StarsU16 index, ship, slot, count = 0U;
    StarsSeeds seeds_before = trace->seeds;
    if (!state || state->turn != 0 || state->player_count != 2)
        ++trace->failures;
    for (index = 0U; index < host->fleet_count; ++index) {
        Soe3InputFleet *source = &host->fleets[index];
        StarsMoveFleet *moving = turn_current_moving_fleet(trace, source);
        StarsFuelFleet fleet;
        StarsFuelInput input;
        const StarsU8 *bank;
        StarsU16 id = (StarsU16)(source->id_word & 0x01ffU);
        if (!moving) continue;
        ++count;
        memset(&fleet, 0, sizeof fleet);
        memset(&input, 0, sizeof input);
        fleet.player = source->owner;
        fleet.destination = moving->target_id;
        fleet.status_flags = source->owner == 1 ?
            get_u16(trace->raw_fleets[id], 4U) : source->state.fleet_flags;
        fleet.fuel = moving->fuel;
        fleet.base_fuel = moving->fuel;
        bank = source->owner == 1 ? trace->designs : host->ship_designs[0];
        for (ship = 0U; ship < 16U; ++ship) {
            const StarsU8 *design = bank + ship * 147U;
            fleet.ship_counts[ship] = source->owner == 1 ?
                get_u16(trace->raw_fleets[id], (StarsU16)(12U + ship * 2U)) :
                source->state.ship_counts[ship];
            if (!fleet.ship_counts[ship]) continue;
            fleet.designs[ship].hull_kind = (StarsI16)get_u16(design, 0U);
            if (design[122] > 16U) { ++trace->failures; continue; }
            for (slot = 0U; slot < design[122]; ++slot)
                if (get_u16(design, (StarsU16)(58U + slot * 4U)) == 0x0800U &&
                        design[60U + slot * 4U] == 16U)
                    fleet.designs[ship].fuel_component += design[61U + slot * 4U];
        }
        input.fleets = &fleet;
        input.fleet_count = 1U;
        input.ops.current_fuel = turn_fuel_capacity;
        input.ops.can_refuel = turn_fuel_can_refuel;
        input.ops.set_fuel = turn_set_fuel;
        input.context = trace;
        stars_fuel_fleets(&input);
        moving->fuel = fleet.fuel;
        source->state.minerals[4] = (StarsU32)fleet.fuel;
        if (source->owner == 1)
            put_u32(trace->raw_fleets[id], 92U, (StarsU32)fleet.fuel);
    }
    ++trace->turn_fuel_calls;
    trace->turn_fuel_fleet_count = count;
    if (count != (trace->rototill_seeded_mode ? 10U :
            (trace->seeded_action ? 7U : 8U)) ||
            trace->seeds.first != seeds_before.first ||
            trace->seeds.second != seeds_before.second)
        ++trace->failures;
}

static void turn_heal_ships(void *opaque, StarsGenerateTurnState *state)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsHealFleet fleets[10];
    StarsHealPlanet planets[TURIN_RETAINED_POINT_COUNT];
    Soe3InputFleet *sources[10];
    StarsHealInput input;
    Soe3InputView *host = &trace->complete_input.views[0];
    StarsU16 index, ship, count, id, flags;
    StarsSeeds seeds_before = trace->seeds;
    memset(&input, 0, sizeof input);
    memset(fleets, 0, sizeof fleets);
    memset(planets, 0, sizeof planets);
    count = 0U;
    if (!state || state->turn != 0 || state->player_count != 2)
        ++trace->failures;
    for (index = 0U; index < host->fleet_count; ++index) {
        Soe3InputFleet *source = &host->fleets[index];
        StarsMoveFleet *moving;
        StarsHealFleet *fleet;
        const StarsU8 *designs;
        id = (StarsU16)(source->id_word & 0x01ffU);
        moving = turn_current_moving_fleet(trace, source);
        if (!moving) continue;
        fleet = &fleets[count];
        sources[count++] = source;
        fleet->player = source->owner;
        fleet->destination = moving->target_id;
        flags = source->owner == 1 ? get_u16(trace->raw_fleets[id], 4U) :
            source->state.fleet_flags;
        /* Movement's 0x2000 marks movement, opposite to FLEET.fHereAllTurn. */
        fleet->status_flags = (StarsU16)((flags & 0xdfffU) |
            ((moving->status & 0x2000) ? 0U : 0x2000U));
        designs = source->owner == 1 ? trace->designs : host->ship_designs[0];
        for (ship = 0U; ship < 16U; ++ship) {
            fleet->ship_presence[ship] = source->owner == 1 ?
                get_u16(trace->raw_fleets[id], (StarsU16)(12U + ship * 2U)) :
                source->state.ship_counts[ship];
            fleet->packed_ship_condition[ship] = source->owner == 1 ?
                get_u16(trace->raw_fleets[id], (StarsU16)(44U + ship * 2U)) :
                source->state.ship_counts[ship + 16U];
            fleet->hull_kind[ship] = (StarsI16)get_u16(designs,
                (StarsU16)(ship * 147U));
        }
    }
    for (index = 0U; index < TURIN_RETAINED_POINT_COUNT; ++index) {
        StarsReadPlanetState *source = &trace->turn_planet_states[index];
        StarsHealPlanet *planet = &planets[index];
        planet->id = source->id;
        planet->owner = source->player;
        planet->starbase_present = (StarsU16)((source->flags >> 9) & 1U);
        planet->starbase_damage = (StarsU16)((source->opaque_44 >> 4) & 0xfffU);
        planet->starbase_blocked = (StarsU16)((source->opaque_44 >> 30) & 1U);
        if (planet->starbase_present) {
            const StarsU8 *designs;
            StarsU8 hull[143];
            StarsU16 slot = (StarsU16)(source->opaque_44 & 15U);
            if (source->player < 0 || source->player > 1 || slot >= 10U) {
                ++trace->failures;
                continue;
            }
            designs = source->player == 1 ? trace->starbase_designs :
                host->starbase_designs[0];
            if (!scrap_read_hull(trace, get_u16(designs,
                    (StarsU16)(slot * 147U)), hull, sizeof hull))
                ++trace->failures;
            else
                planet->starbase_has_cargo = get_u16(hull, 52U) != 0U;
        }
    }
    input.fleets = fleets;
    input.fleet_count = count;
    input.planets = planets;
    input.planet_count = TURIN_RETAINED_POINT_COUNT;
    input.race_stat = turn_loaded_race_stat;
    input.context = trace;
    stars_heal_ships(&input);
    for (index = 0U; index < count; ++index) {
        Soe3InputFleet *source = sources[index];
        id = (StarsU16)(source->id_word & 0x01ffU);
        for (ship = 0U; ship < 16U; ++ship) {
            StarsU16 damage = fleets[index].packed_ship_condition[ship];
            source->state.ship_counts[ship + 16U] = damage;
            if (source->owner == 1)
                put_u16(trace->raw_fleets[id], (StarsU16)(44U + ship * 2U), damage);
        }
    }
    for (index = 0U; index < TURIN_RETAINED_POINT_COUNT; ++index)
        trace->turn_planet_states[index].opaque_44 =
            (trace->turn_planet_states[index].opaque_44 & 0xffff000fUL) |
            ((StarsU32)planets[index].starbase_damage << 4);
    ++trace->turn_heal_calls;
    trace->turn_heal_fleet_count = count;
    if (count != (trace->rototill_seeded_mode ? 10U :
            (trace->seeded_action ? 7U : 8U)) ||
            trace->seeds.first != seeds_before.first ||
            trace->seeds.second != seeds_before.second)
        ++trace->failures;
}

static void turn_breed_colonists(void *opaque, StarsGenerateTurnState *state)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsTransitBreedFleet fleets[10];
    StarsTransitBreedInput input;
    StarsU16 index, count = 0U;
    Soe3InputView *host = &trace->complete_input.views[0];
    StarsSeeds seeds_before = trace->seeds;
    StarsI16 random_calls_before = trace->random_calls;
    memset(fleets, 0, sizeof fleets);
    memset(&input, 0, sizeof input);
    if (!state || state->player_count != 2 || state->turn != 0)
        ++trace->failures;
    /* These pinned races take the original early return. Reject a newly
     * eligible input until its cargo/message paths are bound. */
    for (index = 0U; index < 2U; ++index)
        if (turn_loaded_race_stat(trace, (StarsI16)index, 14) == 4) {
            ++trace->failures; return;
        }
    for (index = 0U; index < host->fleet_count; ++index) {
        Soe3InputFleet *source = &host->fleets[index];
        StarsMoveFleet *moving = turn_current_moving_fleet(trace, source);
        StarsTransitBreedFleet *fleet;
        StarsU16 id = (StarsU16)(source->id_word & 0x01ffU);
        if (!moving) continue;
        fleet = &fleets[count++];
        fleet->player = source->owner;
        fleet->id = source->id_word;
        fleet->destination = moving->target_id;
        fleet->status_flags = source->state.fleet_flags;
        fleet->growth_rate = (StarsI16)(signed char)host->players[source->owner][25];
        fleet->colonists = source->owner == 1 ?
            get_u32(trace->raw_fleets[id], 88U) : source->state.minerals[3];
        if (source->owner == 1 && id == 1U)
            fleet->colonists = trace->supply_fleet.minerals[3];
    }
    input.fleets = fleets;
    input.fleet_count = count;
    input.player_count = state ? state->player_count : 0;
    input.ops.race_stat = turn_loaded_race_stat;
    input.ops.random = random_value;
    input.context = trace;
    ++trace->turn_breed_colonists_calls;
    stars_breed_colonists_in_transit(&input);
    if (trace->random_calls != random_calls_before ||
            trace->seeds.first != seeds_before.first ||
            trace->seeds.second != seeds_before.second)
        ++trace->failures;
}

static void turn_auto_terraform(void *opaque, StarsGenerateTurnState *state)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsAutoTerraformPlanet planets[TURIN_RETAINED_POINT_COUNT];
    StarsAutoTerraformInput input;
    StarsU16 index, axis;
    StarsSeeds seeds_before = trace->seeds;
    StarsI16 random_calls_before = trace->random_calls;
    memset(planets, 0, sizeof planets);
    memset(&input, 0, sizeof input);
    if (!state || state->player_count != 2 || state->turn != 0)
        ++trace->failures;
    for (index = 0U; index < 2U; ++index)
        if (turn_loaded_race_stat(trace, (StarsI16)index, 14) == 3) {
            ++trace->failures; return;
        }
    for (index = 0U; index < TURIN_RETAINED_POINT_COUNT; ++index) {
        const StarsReadPlanetState *source = &trace->turn_planet_states[index];
        planets[index].id = (StarsU16)source->id;
        planets[index].player = source->player;
        planets[index].population = (StarsI32)source->minerals[3];
        for (axis = 0U; axis < 3U; ++axis) {
            planets[index].environment[axis] = (signed char)source->environment[axis];
            planets[index].original_environment[axis] =
                (signed char)source->environment_original[axis];
        }
    }
    input.planets = planets;
    input.planet_count = TURIN_RETAINED_POINT_COUNT;
    input.player_count = state ? state->player_count : 0;
    input.ops.race_stat = turn_loaded_race_stat;
    input.ops.random = random_value;
    input.context = trace;
    ++trace->turn_auto_terraform_calls;
    stars_auto_terraform(&input);
    if (trace->random_calls != random_calls_before ||
            trace->seeds.first != seeds_before.first ||
            trace->seeds.second != seeds_before.second)
        ++trace->failures;
}

static StarsI32 turn_remote_amount(void *opaque, const StarsRemoteTurnFleet *fleet)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsTerraFleetInput input;
    StarsTerraComponent components[16][16];
    StarsU16 ship, slot;
    StarsU16 id = (StarsU16)(fleet->object_token & 0x01ffU);
    Soe3InputFleet *source = turn_host_fleet(trace, (StarsU16)fleet->player, id);
    const StarsU8 *bank = fleet->player == 1 ? trace->designs :
        trace->complete_input.views[0].ship_designs[0];
    memset(&input, 0, sizeof input);
    ++trace->turn_remote_amount_calls;
    if (!source) return 0L;
    for (ship = 0U; ship < 16U; ++ship) {
        const StarsU8 *design = bank + ship * 147U;
        input.component_counts[ship] = (StarsI16)(fleet->player == 1 ?
            get_u16(trace->raw_fleets[id], (StarsU16)(12U + ship * 2U)) :
            source->state.ship_counts[ship]);
        if (!input.component_counts[ship]) continue;
        if (design[122] > 16U) { ++trace->failures; return 0L; }
        input.designs[ship].component_count = design[122];
        input.designs[ship].components = components[ship];
        for (slot = 0U; slot < design[122]; ++slot) {
            components[ship][slot].type = get_u16(design,
                (StarsU16)(58U + slot * 4U));
            components[ship][slot].encoded_amount = get_u16(design,
                (StarsU16)(60U + slot * 4U));
        }
    }
    return stars_terra_from_fleet(&input);
}

/* Unreachable for the pinned inputs: a positive capability must fail this
 * scoped replay rather than silently skip unbound terraforming effects. */
static StarsI16 turn_remote_unexpected_desirability(void *opaque,
    const StarsRemoteTurnPlanet *planet, StarsI16 player)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    (void)planet; (void)player;
    ++trace->failures; return 0;
}

static StarsI16 turn_remote_unexpected_best(void *opaque,
    StarsRemoteTurnPlanet *planet, StarsI16 player, StarsI16 help)
{
    (void)help;
    return turn_remote_unexpected_desirability(opaque, planet, player);
}

static void turn_remote_terraform(void *opaque, StarsGenerateTurnState *state)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsRemoteTurnInput input;
    StarsRemoteTurnFleet fleets[10];
    StarsRemoteTurnPlanet planets[TURIN_RETAINED_POINT_COUNT];
    Soe3InputView *host = &trace->complete_input.views[0];
    StarsSeeds seeds_before = trace->seeds;
    StarsU16 index, axis, count = 0U;
    memset(&input, 0, sizeof input);
    memset(fleets, 0, sizeof fleets);
    memset(planets, 0, sizeof planets);
    if (!state || state->turn != 0 || state->player_count != 2)
        ++trace->failures;
    for (index = 0U; index < host->fleet_count; ++index) {
        Soe3InputFleet *source = &host->fleets[index];
        StarsMoveFleet *moving = turn_current_moving_fleet(trace, source);
        StarsRemoteTurnFleet *fleet;
        if (!moving) continue;
        fleet = &fleets[count++];
        fleet->player = source->owner;
        fleet->planet_id = moving->target_id;
        fleet->object_token = source->id_word;
        fleet->object_subtoken = source->id_word;
        fleet->status_flags = source->state.fleet_flags;
        /* Audit all survivors, including those the orbit/ownership gate skips. */
        if (turn_remote_amount(trace, fleet) != 0L)
            ++trace->failures;
    }
    for (index = 0U; index < TURIN_RETAINED_POINT_COUNT; ++index) {
        const StarsReadPlanetState *source = &trace->turn_planet_states[index];
        planets[index].id = (StarsU16)source->id;
        planets[index].player = source->player;
        planets[index].status_flags = source->flags;
        /* Original rgMinConc[axis+3] aliases environment, not mineral richness. */
        for (axis = 0U; axis < 3U; ++axis)
            planets[index].mineral_concentration[axis] =
                (signed char)source->environment[axis];
    }
    input.fleets = fleets; input.fleet_count = count;
    input.planets = planets; input.planet_count = TURIN_RETAINED_POINT_COUNT;
    input.ops.terraforming_amount = turn_remote_amount;
    input.ops.best_terraform = turn_remote_unexpected_best;
    input.ops.desirability = turn_remote_unexpected_desirability;
    input.context = trace;
    stars_remote_terraform(&input);
    for (index = 0U; index < TURIN_RETAINED_POINT_COUNT; ++index)
        for (axis = 0U; axis < 3U; ++axis)
            if ((StarsU8)planets[index].mineral_concentration[axis] !=
                    trace->turn_planet_states[index].environment[axis])
                ++trace->failures;
    ++trace->turn_remote_terraform_calls;
    trace->turn_remote_fleet_count = count;
    if (count != (trace->rototill_seeded_mode ? 10U :
            (trace->seeded_action ? 7U : 8U)) ||
            trace->seeds.first != seeds_before.first ||
            trace->seeds.second != seeds_before.second)
        ++trace->failures;
}

static StarsI32 turn_sweep_component(void *opaque, StarsU16 encoded,
    StarsU16 starbase, StarsU16 slot)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    const StarsU8 *part = scrap_part_record(0x10U, (StarsU16)(encoded & 255U));
    StarsU16 abilities;
    StarsI32 range, damage;
    (void)slot;
    if (!part) { ++trace->failures; return 0L; }
    abilities = get_u16(part, 58U);
    /* Original CMineSweepFromLphul gives ability bit 1 precedence. */
    if (abilities & 2U) range = 4L;
    else {
        if (abilities & 1U) return 0L;
        range = (StarsI16)get_u16(part, 52U);
    }
    range += starbase;
    damage = (StarsI16)get_u16(part, 54U);
    return range * range * (StarsI32)(encoded >> 8) * damage;
}

static StarsI32 turn_sweep_design(TurinTrace *trace, const StarsU8 *design)
{
    StarsMineSweepHullInput input;
    StarsMineSweepComponent parts[16];
    StarsU16 slot;
    if (design[122] > 16U) { ++trace->failures; return 0L; }
    memset(&input, 0, sizeof input); memset(parts, 0, sizeof parts);
    for (slot = 0U; slot < design[122]; ++slot) {
        parts[slot].type = get_u16(design, (StarsU16)(58U + slot * 4U));
        parts[slot].encoded_value = get_u16(design, (StarsU16)(60U + slot * 4U));
    }
    input.hull_definition = get_u16(design, 0U);
    input.components = parts; input.component_count = design[122];
    input.component_power = turn_sweep_component; input.context = trace;
    return stars_mine_sweep_from_hull(&input);
}

static StarsI32 turn_sweep_ship(void *opaque, StarsI16 player, StarsU16 slot)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    const StarsU8 *bank;
    if (player < 0 || player > 1 || slot >= 16U) { ++trace->failures; return 0L; }
    bank = player == 1 ? trace->designs : trace->complete_input.views[0].ship_designs[player];
    return turn_sweep_design(trace, bank + slot * 147U);
}

static StarsI32 turn_sweep_fleet_power(void *opaque,
    const StarsSweepMinesFleet *fleet)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsMineSweepFleetInput input;
    Soe3InputFleet *source = turn_host_fleet(trace, (StarsU16)fleet->player, (StarsU16)fleet->id);
    StarsU16 slot;
    ++trace->turn_sweep_fleet_power_calls;
    if (!source) { ++trace->failures; return 0L; }
    memset(&input, 0, sizeof input);
    input.player = fleet->player; input.power = turn_sweep_ship; input.context = trace;
    for (slot = 0U; slot < 16U; ++slot)
        input.mine_sweep_counts[slot] = (StarsI16)(fleet->player == 1 ?
            get_u16(trace->raw_fleets[fleet->id], (StarsU16)(12U + 2U * slot)) :
            source->state.ship_counts[slot]);
    return stars_mine_sweep_from_fleet(&input);
}

static StarsI32 turn_sweep_planet_power(void *opaque,
    const StarsSweepMinesPlanet *planet)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    const StarsReadPlanetState *source;
    const StarsU8 *bank;
    StarsU16 slot;
    ++trace->turn_sweep_planet_power_calls;
    if (planet->id < 0 || planet->id >= 32 || planet->player < 0 || planet->player > 1) {
        ++trace->failures; return 0L;
    }
    source = &trace->turn_planet_states[planet->id];
    if ((source->flags & 0x0200U) == 0U) return 0L;
    slot = (StarsU16)(source->opaque_44 & 15UL);
    if (slot >= 10U) { ++trace->failures; return 0L; }
    bank = planet->player == 1 ? trace->starbase_designs :
        trace->complete_input.views[0].starbase_designs[planet->player];
    return turn_sweep_design(trace, bank + slot * 147U);
}

static StarsI16 turn_sweep_can_attack(void *opaque, StarsI16 attacker,
    StarsI16 owner)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    ++trace->turn_sweep_can_attack_calls;
    if (attacker < 0 || attacker > 1 || owner < 0 || owner > 1) {
        ++trace->failures; return 0;
    }
    return (StarsI16)(attacker != owner &&
        trace->complete_input.views[0].players[attacker][112U + owner] != 1U);
}

static void turn_sweep_message(void *opaque, StarsI16 player,
    StarsI16 message, StarsI16 source, StarsI32 amount, StarsI16 mine_type)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    (void)player;
    (void)message;
    (void)source;
    (void)amount;
    (void)mine_type;
    ++trace->turn_sweep_message_calls;
}

static void turn_sweep_free(void *opaque, StarsSweepMinesThing *thing)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    (void)thing;
    ++trace->turn_sweep_free_calls;
}

static void turn_sweep_for_mines(void *opaque,
    StarsGenerateTurnState *state)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsSweepMinesFleet fleets[TURIN_HST_OWNER0_INPUT_FLEET_COUNT +
        TURIN_RETAINED_FLEET_COUNT];
    StarsSweepMinesPlanet planets[TURIN_RETAINED_POINT_COUNT];
    StarsSweepMinesThing things[TURIN_RETAINED_THING_COUNT];
    StarsSweepMinesInput input;
    StarsU16 index;
    StarsU16 fleet_count = 0U;
    StarsU16 planet_count = 0U;
    StarsU16 thing_count = 0U;

    memset(fleets, 0, sizeof fleets);
    memset(planets, 0, sizeof planets);
    memset(things, 0, sizeof things);
    memset(&input, 0, sizeof input);
    if (!state || state->turn != 0 || state->player_count != 2 ||
            trace->turn_move_fleets_calls != 1 ||
            trace->wormhole_move_random_calls !=
                TURIN_RETAINED_WORMHOLE_DRAW_COUNT)
        ++trace->failures;

    for (index = 0U; index < TURIN_HST_OWNER0_INPUT_FLEET_COUNT; ++index) {
        const StarsMoveFleet *source = &trace->turn_owner0_moving_fleets[index];
        StarsSweepMinesFleet *fleet;
        if (!source->active)
            continue;
        fleet = &fleets[fleet_count++];
        fleet->player = 0;
        fleet->id = source->id;
        fleet->x = source->x;
        fleet->y = source->y;
        fleet->status_flags = turn_host_fleet(trace, 0U, (StarsU16)source->id)->state.fleet_flags;
    }
    for (index = 0U; index < (trace->rototill_seeded_mode ?
            TURIN_RETAINED_FLEET_COUNT : 2U); ++index) {
        const StarsMoveFleet *source = &trace->turn_moving_fleets[index];
        StarsSweepMinesFleet *fleet;
        if (!source->active)
            continue;
        fleet = &fleets[fleet_count++];
        fleet->player = 1;
        fleet->id = source->id;
        fleet->x = source->x;
        fleet->y = source->y;
        fleet->status_flags = get_u16(trace->raw_fleets[index], 4U);
    }
    for (index = 0U; index < TURIN_RETAINED_POINT_COUNT; ++index) {
        planets[index].player = trace->turn_planet_states[index].player;
        planets[index].id = (StarsI16)index;
        planets[index].x = trace->complete_input.coordinates[index][0];
        planets[index].y = trace->complete_input.coordinates[index][1];
        ++planet_count;
    }
    for (index = 0U; index < TURIN_RETAINED_THING_COUNT; ++index) {
        const StarsMoveThing *source = &trace->retained_move_things[index];
        StarsSweepMinesThing *thing = &things[thing_count++];
        thing->kind = (StarsU16)source->kind;
        thing->owner = source->owner;
        thing->x = source->x;
        thing->y = source->y;
        thing->active = (StarsU8)(source->active != 0);
        if (thing->kind != 2U || !thing->active)
            ++trace->failures;
    }

    input.fleets = fleets;
    input.fleet_count = fleet_count;
    input.planets = planets;
    input.planet_count = planet_count;
    input.things = things;
    input.thing_count = thing_count;
    input.ops.fleet_power = turn_sweep_fleet_power;
    input.ops.planet_power = turn_sweep_planet_power;
    input.ops.can_attack = turn_sweep_can_attack;
    input.ops.send_message = turn_sweep_message;
    input.ops.free_thing = turn_sweep_free;
    input.context = trace;
    trace->turn_sweep_fleet_count = (StarsI16)fleet_count;
    trace->turn_sweep_planet_count = (StarsI16)planet_count;
    trace->turn_sweep_thing_count = (StarsI16)thing_count;
    ++trace->turn_sweep_for_mines_calls;
    stars_sweep_for_mines(&input);
    for (index = 0U; index < thing_count; ++index)
        if (things[index].kind != 2U || !things[index].active ||
                things[index].owner_mask != 0UL)
            ++trace->failures;
}

static void turn_move_things(void *opaque, StarsGenerateTurnState *state,
    StarsI16 post_production)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    (void)state;
    if (post_production)
        run_retained_wormhole_movement(trace);
    else {
        ++trace->turn_pre_move_things_calls;
        run_retained_preproduction_thing_move(trace);
    }
}

static void turn_store_waypoint(StarsU8 *raw, const StarsKillUsedWaypointOrder *order)
{
    put_u16(raw, 0U, (StarsU16)order->x);
    put_u16(raw, 2U, (StarsU16)order->y);
    put_u16(raw, 4U, (StarsU16)order->target_id);
    put_u16(raw, 6U, order->flags);
    memcpy(raw + 8U, order->payload, 10U);
}

static void turn_load_waypoint(StarsKillUsedWaypointOrder *order, const StarsU8 *raw)
{
    order->x = (StarsI16)get_u16(raw, 0U);
    order->y = (StarsI16)get_u16(raw, 2U);
    order->target_id = (StarsI16)get_u16(raw, 4U);
    order->flags = get_u16(raw, 6U);
    memcpy(order->payload, raw + 8U, 10U);
}

static void turn_delete_used_waypoint(void *opaque,
    StarsKillUsedWaypointFleet *fleet, StarsI16 index, StarsI16 recycle)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsDeleteWpFarInput input;
    StarsU8 records[36], count;
    StarsU16 i;
    if (fleet->cord != 2 || index != 1) { ++trace->failures; return; }
    count = (StarsU8)fleet->cord;
    for (i = 0U; i < count; ++i) turn_store_waypoint(records + i * 18U, &fleet->orders[i]);
    input.records = records; input.cord = count; input.iord_mac = &count;
    stars_delete_wp_far(&input, (StarsU16)index, (StarsU16)recycle);
    fleet->cord = (StarsI16)input.cord;
    if (input.cord != count) ++trace->failures;
    for (i = 0U; i < count; ++i) turn_load_waypoint(&fleet->orders[i], records + i * 18U);
}

static void turn_used_waypoint_message(void *opaque, StarsI16 owner,
    StarsI16 code, StarsI16 fleet)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsI16 parameters[7];
    StarsI16 index = trace->turn_used_waypoint_messages++;
    memset(parameters, 0, sizeof parameters); parameters[0] = fleet;
    turn_send_message(trace, owner, code, (StarsI16)(fleet | 0x8000U), parameters);
    if (index >= 10) { ++trace->failures; return; }
    trace->turn_used_waypoint_message[index][0] = owner;
    trace->turn_used_waypoint_message[index][1] = code;
    trace->turn_used_waypoint_message[index][2] = fleet;
}

static void turn_finish_movement_orders(TurinTrace *trace)
{
    StarsKillUsedWaypointsInput input;
    StarsKillUsedWaypointOps ops;
    StarsKillUsedWaypointFleet fleets[10];
    StarsKillUsedWaypointOrder orders[10][2];
    Soe3InputFleet *sources[10];
    Soe3InputView *host = &trace->complete_input.views[0];
    StarsU16 index, j, count = 0U;
    memset(&ops, 0, sizeof ops); memset(fleets, 0, sizeof fleets);
    memset(orders, 0, sizeof orders);
    for (index = 0U; index < host->fleet_count; ++index) {
        Soe3InputFleet *source = &host->fleets[index];
        StarsMoveFleet *moving = turn_current_moving_fleet(trace, source);
        StarsU16 id = (StarsU16)(source->id_word & 511U);
        const StarsU8 *raw;
        StarsKillUsedWaypointFleet *fleet;
        if (!moving) continue;
        fleet = &fleets[count]; sources[count] = source;
        fleet->id = (StarsI16)source->id_word; fleet->owner = source->owner;
        fleet->x = moving->x; fleet->y = moving->y; fleet->id_planet = moving->target_id;
        fleet->status_flags = (StarsI16)source->state.fleet_flags;
        fleet->cord = source->owner == 1 ? trace->plord[id][3] : source->state.order_count;
        if (fleet->cord < 1 || fleet->cord > 2) { ++trace->failures; return; }
        raw = source->owner == 1 ? trace->plord[id] + 4U : source->state.orders;
        fleet->orders = orders[count];
        for (j = 0U; j < (StarsU16)fleet->cord; ++j) {
            turn_load_waypoint(&fleet->orders[j], raw + j * 18U);
            if (((fleet->orders[j].flags >> 8) & 15U) != 1U) {
                ++trace->failures; return;
            }
        }
        if (moving->status & 0x2000U) {
            /* MoveFleets 10b0:4e12..4e6d updates waypoint zero before cleanup. */
            fleet->orders[0].x = moving->x; fleet->orders[0].y = moving->y;
            fleet->orders[0].target_id = moving->target_id;
            fleet->orders[0].flags = (StarsU16)((fleet->orders[0].flags & 0xf0ffU) |
                (moving->target_id < 0 ? 0x0400U : 0x0100U));
        }
        ++count;
    }
    ops.context = trace; ops.delete_waypoint = turn_delete_used_waypoint;
    ops.send_message = turn_used_waypoint_message;
    input.fleets = fleets; input.fleet_count = count; input.ops = &ops;
    stars_kill_used_waypoints(&input);
    ++trace->turn_used_waypoint_calls;
    for (index = 0U; index < count; ++index) {
        Soe3InputFleet *source = sources[index];
        StarsU16 id = (StarsU16)(source->id_word & 511U);
        source->state.order_count = (StarsU16)fleets[index].cord;
        for (j = 0U; j < source->state.order_count; ++j)
            turn_store_waypoint(source->state.orders + j * 18U, &orders[index][j]);
        if (source->owner == 1) {
            trace->plord[id][3] = (StarsU8)source->state.order_count;
            put_u16(trace->raw_fleets[id], 98U, source->state.order_count);
            memcpy(trace->plord[id] + 4U, source->state.orders, source->state.order_count * 18U);
        }
    }
}

static void turn_move_fleets(void *opaque, StarsGenerateTurnState *state)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    Soe3InputView *host = &trace->complete_input.views[0];
    StarsU16 index;
    (void)state;
    ++trace->turn_move_fleets_calls;
    run_retained_move_fleets(trace);
    for (index = 0U; index < host->fleet_count; ++index) {
        Soe3InputFleet *source = &host->fleets[index];
        StarsMoveFleet *moving = turn_current_moving_fleet(trace, source);
        StarsU16 id = (StarsU16)(source->id_word & 511U);
        StarsU16 flags = source->owner == 1 ?
            get_u16(trace->raw_fleets[id], 4U) : source->state.fleet_flags;
        StarsU32 direction = 0UL;
        if (!moving) continue;
        if (moving->status & 0x2000U) {
            StarsI16 dx = (StarsI16)(moving->x - source->state.x);
            StarsI16 dy = (StarsI16)(moving->y - source->state.y);
            const StarsU8 *next = source->owner == 1 ?
                trace->plord[id] + 4U + 18U : source->state.orders + 18U;
            if (dx || dy) {
                while (dx > 127 || dx < -127 || dy > 127 || dy < -127) {
                    dx /= 2; dy /= 2;
                }
                direction = (StarsU32)(dx + 127) |
                    ((StarsU32)(dy + 127) << 8) |
                    ((StarsU32)((get_u16(next, 6U) >> 4) & 15U) << 16) |
                    0x00100000UL;
            }
        }
        source->state.overlap_116[0] = direction;
        /* Original MoveFleets initializes fHereAllTurn, clearing it on travel. */
        flags = (StarsU16)(flags | 0x2800U);
        if (moving->status & 0x2000U) flags = (StarsU16)(flags & ~0x2000U);
        source->state.fleet_flags = flags;
        source->state.x = moving->x;
        source->state.y = moving->y;
        source->state.planet = moving->target_id;
        source->state.minerals[4] = (StarsU32)moving->fuel;
        if (source->owner == 1) {
            StarsU8 *raw = trace->raw_fleets[id];
            put_u16(raw, 4U, flags);
            put_u16(raw, 6U, (StarsU16)moving->target_id);
            put_u16(raw, 8U, (StarsU16)moving->x);
            put_u16(raw, 10U, (StarsU16)moving->y);
            put_u32(raw, 92U, (StarsU32)moving->fuel);
            put_u32(raw, 116U, direction);
        }
    }
    turn_finish_movement_orders(trace);
}

static StarsI16 turn_produce_extra(void *opaque, StarsI16 planet)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    if (planet < 0 || planet >= 32) { ++trace->failures; return 0L; }
    return (StarsI16)trace->planet_resource_extra[planet];
}

/* Commit Produce's working banks before population/research/random events.
 * They all observe the same world; never copy an older bank over later effects. */
static void turn_produce_commit(TurinTrace *trace)
{
    StarsU16 index, mineral;
    for (index = 0U; index < 32U; ++index) {
        StarsProducePlanet *planet = &trace->turn_produce_planets[index];
        StarsReadPlanetState *target = &trace->turn_planet_states[index];
        if (planet->id != target->id || planet->owner != target->player) {
            ++trace->failures; return;
        }
        for (mineral = 0U; mineral < 3U; ++mineral)
            target->minerals[mineral] = (StarsU32)planet->mineral[mineral];
        if (planet->items) {
            if (planet->id != trace->turn_committed_planet ||
                    planet->items != trace->turn_committed_queue) ++trace->failures;
            trace->turn_committed_queue_count = planet->item_count;
        }
    }
    ++trace->turn_production_commits;
}

static void turn_produce_mine_minerals(void *opaque)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsI16 mineral;
    StarsU16 player;
    run_retained_mining(trace, trace->seeded_action);
    for (player = 0U; player < 2U; ++player)
        put_u32(trace->complete_input.views[0].players[player], 58U, 0UL);
    if (!trace->turn_produce_planets) {
        ++trace->failures;
        return;
    }
    for (mineral = 0; mineral < 3; ++mineral) {
        trace->turn_produce_planets[10].mineral[mineral] =
            trace->mining_stock[0][mineral];
        trace->turn_produce_planets[24].mineral[mineral] =
            trace->mining_stock[1][mineral];
        trace->turn_planet_states[10].minerals[mineral] =
            (StarsU32)trace->mining_stock[0][mineral];
        trace->turn_planet_states[24].minerals[mineral] =
            (StarsU32)trace->mining_stock[1][mineral];
        trace->turn_planet_states[10].mineral_levels[mineral] =
            trace->mining_inputs[0].percent_level[mineral];
        trace->turn_planet_states[24].mineral_levels[mineral] =
            trace->mining_inputs[1].percent_level[mineral];
        trace->turn_planet_states[10].mineral_concentrations[mineral] =
            trace->mining_inputs[0].concentration[mineral];
        trace->turn_planet_states[24].mineral_concentrations[mineral] =
            trace->mining_inputs[1].concentration[mineral];
    }
}

static void turn_produce_message(void *opaque, StarsI16 player,
    StarsI16 message, StarsI16 planet, StarsI16 object, StarsI16 value)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsI16 index = trace->turn_produce_message_calls;
    StarsI16 parameters[7];
    memset(parameters, 0, sizeof parameters);
    parameters[0] = object; parameters[1] = value;
    turn_send_message(trace, player, message, planet, parameters);
    if (index < 0 || index >= 4) {
        ++trace->failures;
        return;
    }
    trace->turn_produce_message_player[index] = player;
    trace->turn_produce_message_code[index] = message;
    trace->turn_produce_message_planet[index] = planet;
    trace->turn_produce_message_object[index] = object;
    trace->turn_produce_message_value[index] = value;
    ++trace->turn_produce_message_calls;
}

static StarsI16 turn_produce_costs(void *opaque,
    const StarsProducePlanet *planet, const StarsProduceItem *item,
    StarsI32 costs[4])
{
    TurinTrace *trace = (TurinTrace *)opaque;
    const StarsU8 *bank;
    StarsU16 values[4];
    StarsI16 index;
    if (!planet || planet->owner < 0 || planet->owner > 1 || !item ||
            item->type < 0 || item->type >= 16 || item->object_kind != 2) {
        ++trace->failures;
        return 0;
    }
    bank = planet->owner == 1 ? trace->designs :
        trace->complete_input.views[0].ship_designs[0];
    stars_get_true_hull_cost(bank + item->type * 147U, values);
    for (index = 0; index < 4; ++index) costs[index] = values[index];
    return 1;
}

static StarsI16 turn_score_resource_stat(void *opaque, StarsI16 player, StarsI16 stat)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    if (player < 0 || player > 1 || stat < 0 || stat > 15) {
        ++trace->failures; return 0;
    }
    return (StarsI16)(signed char)trace->complete_input.views[0].players[player][62 + stat];
}

static StarsI16 turn_score_desirability(void *opaque, const StarsU8 *planet,
    StarsI16 player)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsU8 *near_data = trace->guess_race_data_storage;
    if (!planet || player < 0 || player > 1) { ++trace->failures; return 0; }
    memcpy(near_data + 12U, planet + 12U, 3U);
    memcpy(near_data + 0x59a2U + (StarsU16)player * 192U,
        trace->complete_input.views[0].players[player], 192U);
    return stars_planet_desirability(near_data, 0U, player);
}

static StarsI32 turn_score_max_population(void *opaque, StarsU16 id, StarsI16 player)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsCalcPlanetMaxPopInput input;
    StarsU8 raw[56];
    if (id >= 32U || player < 0 || player > 1) { ++trace->failures; return 0; }
    memset(&input, 0, sizeof input);
    pack_loaded_planet(raw, &trace->turn_planet_states[id]);
    input.planet_owner = trace->turn_planet_states[id].player;
    input.player = player;
    input.race_stat = turn_score_resource_stat(trace, player, 14);
    input.race_grbit = (StarsI16)((get_u32(
        trace->complete_input.views[0].players[player], 78U) >> 9) & 1UL);
    input.desirability = turn_score_desirability(trace, raw, player);
    if (input.race_stat == 8) { ++trace->failures; return 0L; }
    return stars_calc_planet_max_pop(&input);
}

static StarsI32 turn_produce_resources(void *opaque,
    const StarsProducePlanet *planet, StarsI16 owner)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsCResourcesAtPlanetOps resource_ops;
    StarsU8 runtime_planet[56];
    StarsI32 result;
    const StarsReadPlanetState *loaded;
    if (!planet || planet->id < 0 || planet->id >= 32 || owner < 0 || owner > 1) {
        ++trace->failures; return 0L;
    }
    loaded = &trace->turn_planet_states[planet->id];
    if (loaded->player != owner) { ++trace->failures; return 0L; }
    pack_loaded_planet(runtime_planet, loaded);
    if (planet->id == 24 && owner == 1)
        put_u32(runtime_planet, 40U, trace->supply_planet.minerals[3]);
    memset(&resource_ops, 0, sizeof resource_ops);
    resource_ops.calc_max_population = turn_score_max_population;
    resource_ops.planet_desirability = turn_score_desirability;
    resource_ops.get_player_stat = turn_score_resource_stat;
    resource_ops.factory_calc_max_population =
        turn_score_max_population;
    resource_ops.factory_get_player_stat = turn_score_resource_stat;
    result = stars_cresources_at_planet(runtime_planet, owner,
        &resource_ops, trace);
    if (owner == 1) trace->turn_produce_base_resources = result;
    ++trace->turn_produce_resource_calls;
    return result;
}

static StarsI16 turn_produce_build_item(void *opaque,
    StarsProducePlanet *planet, StarsProduceItem *item,
    StarsProduceItem *partial, StarsI32 resources[4], StarsI16 *status)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsBuildProductionOps build_ops;
    StarsBuildProductionInput build_input;
    StarsI16 index;
    StarsI16 built;

    memset(&build_ops, 0, sizeof build_ops);
    memset(&build_input, 0, sizeof build_input);
    build_ops.costs = turn_produce_costs;
    build_ops.context = trace;
    build_input.planet = planet;
    build_input.item = item;
    build_input.partial = partial;
    build_input.status = status;
    build_input.ops = &build_ops;
    for (index = 0; index < 4; ++index)
        build_input.resources[index] = resources[index];
    built = stars_build_production_item(&build_input);
    /* The pinned turn can only fund a partial hull. A completed build needs
     * creation/message services and must never silently pass this adapter. */
    if (built != 0) ++trace->failures;
    for (index = 0; index < 4; ++index)
        resources[index] = build_input.resources[index];
    ++trace->turn_produce_build_calls;
    return built;
}

static void turn_produce_update_population(void *opaque,
    StarsProducePlanet *planet)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsI16 index = trace->turn_produce_population_calls;
    if (trace->turn_production_planet_visits == 0)
        turn_produce_commit(trace);
    ++trace->turn_production_planet_visits;
    if (planet->owner < 0) {
        if (trace->turn_planet_states[planet->id].minerals[3] != 0UL)
            ++trace->failures;
        return;
    }
    if (index == 0) {
        if (planet->id != 10 || planet->owner != 0)
            ++trace->failures;
        (void)run_retained_population_growth(trace);
    } else if (index == 1) {
        if (planet->id != 24 || planet->owner != 1)
            ++trace->failures;
        trace->turn_planet_states[10].minerals[3] =
            trace->guess_runtime_populations[0];
        trace->turn_planet_states[24].minerals[3] =
            trace->guess_runtime_populations[1];
        trace->turn_planet_states[10].opaque_20[0] =
            (trace->turn_planet_states[10].opaque_20[0] & 0xffffff00UL) |
            (StarsU32)trace->population_summary.planet10.fraction;
        trace->turn_planet_states[24].opaque_20[0] =
            (trace->turn_planet_states[24].opaque_20[0] & 0xffffff00UL) |
            (StarsU32)trace->population_summary.owner1.fraction;
    } else
        ++trace->failures;
    ++trace->turn_produce_population_calls;
}

static void turn_produce_random_events(void *opaque)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    ++trace->turn_produce_random_event_calls;
    run_retained_random_events(trace, trace->seeded_action);
}

static StarsI16 turn_seed_planet_desirability(void *opaque,
    const StarsTurinTurnPlanet *planet, StarsI16 player)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsU8 data[65536];
    StarsI16 result;
    if (!planet || planet->id != 24 || planet->owner != 1 || player != 1) {
        ++trace->failures;
        return -1;
    }
    memset(data, 0, sizeof data);
    memcpy(data + 12U, trace->complete_input.views[2].planets[0].environment, 3U);
    memcpy(data + 0x59a2U + (StarsU16)player * 192U,
        trace->complete_input.views[0].players[player], 192U);
    result = stars_planet_desirability(data, 0U, player);
    if (result != 100)
        ++trace->failures;
    return result;
}

static StarsI16 turn_seed_queue_production(void *opaque,
    const StarsTurinTurnPlanet *planet, StarsI16 design, StarsI16 quantity)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsProduceItem *item;
    if (!planet || planet->id != 24 || planet->owner != 1 ||
            design != 0 || quantity != 1 ||
            trace->turn_seed_queue_calls >= 2)
        return 0;
    stars_add_item_to_queue((StarsAiProductionQueue *)0, &trace->turn_ai_queue,
        (StarsU16)design, (StarsU16)quantity, 2U, 1, 0, trace);
    if (trace->turn_ai_queue.count != (StarsU16)(trace->turn_seed_queue_calls + 1))
        return 0;
    item = &trace->turn_produce_queue[trace->turn_seed_queue_calls];
    memset(item, 0, sizeof *item);
    {
        StarsU32 packed = get_u32(trace->turn_ai_queue.records,
            (StarsU16)(trace->turn_seed_queue_calls * 4));
        item->type = (StarsI16)((packed >> 10) & 127UL);
        item->object_kind = (StarsI16)((packed >> 17) & 7UL);
        item->quantity = (StarsI16)(packed & 1023UL);
        item->progress = (StarsI16)((packed >> 20) & 127UL);
        item->active = 1;
    }
    ++trace->turn_seed_queue_calls;
    return 1;
}

static void turn_finish_free(void *opaque, StarsU8 *product)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    /* Storage belongs to the probe arena; release the old ownership only. */
    if (product && product != (StarsU8 *)trace->turn_committed_queue)
        ++trace->failures;
    trace->turn_committed_queue = 0;
    trace->turn_committed_queue_count = 0U;
}
static void turn_finish_lookup(void *opaque, StarsI16 id, StarsU8 *product)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    if (id == -1) {
        if (trace->turn_finish_lookup_calls != 0 ||
                product != (StarsU8 *)trace->turn_produce_queue)
            ++trace->failures;
        trace->turn_committed_queue = (StarsProduceItem *)product;
        trace->turn_committed_queue_count = (StarsU16)trace->turn_seed_queue_calls;
    } else if (id != trace->turn_committed_planet ||
            trace->turn_finish_lookup_calls != 1 ||
            product != (StarsU8 *)trace->turn_committed_queue)
        ++trace->failures;
    ++trace->turn_finish_lookup_calls;
}
static void turn_finish_unexpected_ui(void *opaque)
{ ++((TurinTrace *)opaque)->failures; }
static void turn_finish_unexpected_draw(void *opaque, StarsU16 window, StarsU16 flags)
{ (void)window; (void)flags; ++((TurinTrace *)opaque)->failures; }

static StarsI16 turn_seed_finish_production(void *opaque,
    const StarsTurinTurnPlanet *planet, StarsI16 changed)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsFinishProductionInput input;
    StarsFinishProductionOps ops;
    StarsLogPlanet old_planet, new_planet;
    StarsLogChangePlanetState log_state;
    StarsLogChangePlanetOps log_ops;
    if (!planet || planet->id != 24 || planet->owner != 1 || changed != 1) {
        ++trace->failures; return 0;
    }
    memset(&input, 0, sizeof input); memset(&ops, 0, sizeof ops);
    input.write = changed;
    input.game_flags = trace->ai_production_saved_flags;
    input.planet_bits = trace->complete_input.views[2].planets[0].opaque_20[1];
    input.player = planet->owner; input.planet_id = planet->id;
    input.ai_active = trace->ai_input.ai_active;
    input.selected_product = (StarsU8 *)trace->turn_committed_queue;
    input.global_product = (StarsU8 *)trace->turn_produce_queue;
    input.global_count = (StarsU8)trace->turn_seed_queue_calls;
    ops.free_product = turn_finish_free; ops.lookup_planet = turn_finish_lookup;
    ops.fill_list = turn_finish_unexpected_ui; ops.draw_ship = turn_finish_unexpected_draw;
    ops.advance_tutor = turn_finish_unexpected_ui;
    input.ops = &ops; input.context = trace;
    trace->turn_committed_planet = planet->id;
    stars_finish_production(&input);
    if (input.global_product != 0 || input.selected_product !=
            (StarsU8 *)trace->turn_committed_queue || trace->turn_finish_lookup_calls != 2 ||
            trace->turn_committed_queue_count != 2U)
        ++trace->failures;
    memset(&old_planet, 0, sizeof old_planet);
    old_planet.object_id = planet->id;
    memcpy(old_planet.minerals, trace->complete_input.views[2].planets[0].minerals,
        sizeof old_planet.minerals);
    old_planet.overlap_20 = trace->complete_input.views[2].planets[0].opaque_20[1];
    old_planet.overlap_44 = trace->complete_input.views[2].planets[0].opaque_44;
    old_planet.overlap_48 = trace->complete_input.views[2].planets[0].opaque_48;
    new_planet = old_planet;
    new_planet.has_production = trace->turn_ai_queue.count != 0U;
    new_planet.production_count = trace->turn_ai_queue.count;
    new_planet.production = trace->turn_ai_queue.records;
    memset(&log_state, 0, sizeof log_state); memset(&log_ops, 0, sizeof log_ops);
    log_ops.write_memory = startup_capture_memory_log;
    stars_log_change_planet(&old_planet, &new_planet, trace->game_flags,
        &log_state, &log_ops, trace);
    ++trace->turn_seed_finish_calls;
    return 1;
}

/* Turin receives a scout-adapter context; production adapters own the
 * shared trace. No second AI pass is permitted at the production stage. */
static void startup_build_catalog(TurinTrace *, StarsProductionList *,
    StarsProductionList *, StarsU16);
static StarsI16 startup_queue_prepare(void *opaque,
    const StarsTurinTurnPlanet *planet, StarsI16 turn)
{
    TurinTrace *trace = startup_trace(opaque);
    StarsProductionList initial_queue;
    StarsTurinDroneAiTurnInput *policy = &trace->turin_input;
    const StarsU8 *player = trace->complete_input.views[2].players[1];
    StarsI16 role;
    /* These switches assert populated policy inputs, not branch exclusion. */
    policy->production_policy_valid = 1;
    policy->design_1_policy_valid = 1;
    policy->minelayer_policy_valid = 1;
    policy->support_policy_valid = 1;
    policy->production_designs_valid = 1;
    policy->ai_planet_scale = (StarsI16)get_u16(trace->ai_data, 2U);
    policy->player_planet_count = (StarsI16)get_u16(player, 2U);
    policy->player_tech_2 = player[28U];
    policy->design_1_existing = get_u32(trace->designs + 147U, 131U);
    policy->design_12_eligible =
        (get_u16(trace->designs + 12U * 147U, 123U) & 0x200U) == 0U;
    policy->design_15_inactive =
        (get_u16(trace->designs + 15U * 147U, 123U) & 0x200U) == 0U;
    policy->base_design_inactive =
        (get_u16(trace->designs, 123U) & 0x200U) == 0U;
    policy->base_design_hull_id = (StarsI16)get_u16(trace->designs, 0U);
    policy->base_design_existing = get_u32(trace->designs, 131U);
    policy->base_design_built = get_u32(trace->designs, 127U);
    for (role = 0; role < 8; ++role) {
        StarsI16 selected = policy->design_selection[role];
        if (selected >= 0) {
            StarsU16 costs[4];
            StarsI16 lane;
            const StarsU8 *raw = trace->designs + selected * 147U;
            policy->design_existing[role] = get_u32(raw, 131U);
            stars_get_true_hull_cost(raw, costs);
            for (lane = 0; lane < 4; ++lane)
                policy->design_hull_cost[role][lane] = costs[lane];
            /* Resource/queue budgeting for these newly eligible roles must
             * be bound before accepting a different starting case. */
            if (role == 0 || role == 3 || role == 5 || role == 7)
                return 0;
        }
    }
    if (turn != 0 || planet->id != trace->planet.id ||
            trace->turn_committed_queue || trace->turn_queue_prepare_calls)
        return 0;
    startup_select_current_planet(trace, planet->id);
    /* Gate 1 proves the selected queue empty. InitProduction still builds
     * the current catalog and initializes its private queue before adding. */
    memset(trace->turn_ai_queue_records, 0, sizeof trace->turn_ai_queue_records);
    trace->ai_production_saved_flags = (StarsU16)((trace->game_flags & ~0x20U) |
        (((trace->complete_input.views[2].planets[0].opaque_20[1] >> 23) & 1UL) << 5));
    trace->game_flags = trace->ai_production_saved_flags;
    trace->turn_ai_queue.records = trace->turn_ai_queue_records;
    trace->turn_ai_queue.count = 0U; trace->turn_ai_queue.capacity = 64U;
    startup_build_catalog(trace, &trace->first_queue_catalog, &initial_queue,
        trace->ai_production_saved_flags);
    if (initial_queue.item_count || !trace->first_queue_catalog.item_count)
        ++trace->failures;
    ++trace->first_queue_init_calls;
    ++trace->turn_queue_prepare_calls;
    return 1;
}
static StarsI16 startup_queue_desirability(void *opaque,
    const StarsTurinTurnPlanet *planet, StarsI16 player)
{ return turn_seed_planet_desirability(startup_trace(opaque), planet, player); }
static StarsI16 startup_queue_add(void *opaque,
    const StarsTurinTurnPlanet *planet, StarsI16 design, StarsI16 quantity)
{ return turn_seed_queue_production(startup_trace(opaque), planet, design, quantity); }
static StarsI16 startup_queue_finish(void *opaque,
    const StarsTurinTurnPlanet *planet, StarsI16 changed)
{ return turn_seed_finish_production(startup_trace(opaque), planet, changed); }

/* Final AI queue budget, evaluated at Turin's original fill-queue boundary.
 * Selection/catalog initialization and finalization are audited separately. */
static StarsI32 startup_final_max_population(void *opaque, StarsU16 id,
    StarsI16 player)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsCalcPlanetMaxPopInput input;
    if (id != get_u16(trace->final_queue_planet, 0U) || player != 1) {
        ++trace->failures; return 0;
    }
    memset(&input, 0, sizeof input);
    input.planet_owner = (StarsI16)get_u16(trace->final_queue_planet, 2U);
    input.player = player;
    input.race_stat = turn_score_resource_stat(trace, player, 14);
    input.race_grbit = (StarsI16)((get_u32(
        trace->complete_input.views[0].players[player], 78U) >> 9) & 1UL);
    input.desirability = turn_score_desirability(trace,
        trace->final_queue_planet, player);
    return stars_calc_planet_max_pop(&input);
}

static void startup_final_estimate(void *opaque, const StarsU8 *planet,
    StarsI32 *output, StarsI16 mines, StarsI16 apply)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsMineralMinedInput input;
    StarsMineralMinedOps ops;
    StarsCMaxOperableMinesOps mine_ops;
    StarsI16 i;
    StarsU16 id = get_u16(planet, 0U);
    const StarsReadPlanetState *state;
    if (id >= 32U || mines != -1 || apply != 0) {
        ++trace->failures; return;
    }
    state = &trace->complete_input.views[2].planets[0];
    if (state->id != (StarsI16)id) { ++trace->failures; return; }
    memset(&input, 0, sizeof input); memset(&ops, 0, sizeof ops);
    memset(&mine_ops, 0, sizeof mine_ops);
    mine_ops.max_mines.calc_max_population = startup_final_max_population;
    mine_ops.max_mines.get_player_stat = turn_score_resource_stat;
    input.player = state->player;
    if (turn_score_resource_stat(trace, state->player, 14) == 8) {
        ++trace->failures; return;
    }
    input.operating_mines = stars_cmines_operating(planet, &mine_ops, trace);
    input.efficiency_tenths = turn_score_resource_stat(trace, state->player, 4);
    input.c_mines = mines;
    input.force_concentration_floor = (state->flags >> 10) & 1U;
    input.random_rounding = (trace->game_flags >> 1) & 1U;
    ops.random = startup_prepare_random; ops.context = trace;
    input.ops = &ops;
    for (i = 0; i < 3; ++i)
        input.concentration[i] = state->mineral_concentrations[i];
    stars_est_minerals_mined(&input);
    for (i = 0; i < 3; ++i) output[i] = input.output[i];
}

static StarsI16 startup_final_resources(void *opaque, const StarsU8 *planet,
    StarsI16 player)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsCResourcesAtPlanetOps ops;
    memset(&ops, 0, sizeof ops);
    ops.calc_max_population = startup_final_max_population;
    ops.planet_desirability = turn_score_desirability;
    ops.get_player_stat = turn_score_resource_stat;
    ops.factory_calc_max_population = startup_final_max_population;
    ops.factory_get_player_stat = turn_score_resource_stat;
    return (StarsI16)stars_cresources_at_planet(planet, player, &ops, trace);
}
static StarsI16 startup_final_percent(void *opaque, StarsI16 player)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    if (player != 1) { ++trace->failures; return 0; }
    return trace->complete_input.views[0].players[player][56];
}
static const StarsU8 *startup_final_queue(void *opaque, const StarsU8 *planet,
    StarsU16 *count)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    if (get_u16(planet, 0U) != (StarsU16)trace->turn_committed_planet ||
            trace->turn_ai_queue.count > 64U) {
        ++trace->failures; *count = 0; return 0;
    }
    *count = trace->turn_ai_queue.count;
    memset(trace->final_queue_packed, 0, sizeof trace->final_queue_packed);
    trace->final_queue_packed[3] = (StarsU8)*count;
    memcpy(trace->final_queue_packed + 4U, trace->turn_ai_queue.records, *count * 4U);
    return trace->final_queue_packed;
}
static StarsI16 startup_final_hull(void *opaque, StarsI16 player, StarsU16 slot,
    StarsI16 special, StarsProductionHull *hull)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    const StarsU8 *raw;
    if (player != 1 || special || slot >= 16U) {
        ++trace->failures; return 0;
    }
    raw = trace->designs + slot * 147U;
    memset(hull, 0, sizeof *hull);
    hull->hidden_flags = get_u16(raw, 123U);
    stars_get_true_hull_cost(raw, hull->base_costs);
    return 1;
}
static void startup_final_cost(void *opaque, const StarsU8 *planet,
    const StarsU8 *item, StarsI32 *costs, StarsI16 player, StarsI16 one)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsProductionCostInput input;
    StarsProductionCostOps ops;
    StarsU32 packed = get_u32(item, 0U), result[4];
    StarsI16 i;
    memset(&input, 0, sizeof input); memset(&ops, 0, sizeof ops);
    input.item_variant = (StarsU16)((packed >> 17) & 7UL);
    if (input.item_variant != 2U || (packed >> 20) != 0UL) {
        ++trace->failures;
        for (i = 0; i < 4; ++i) costs[i] = 0;
        return;
    }
    input.player = player; input.planet_player = (StarsI16)get_u16(planet, 2U);
    input.item_kind = (StarsU16)((packed >> 10) & 127UL);
    input.quantity = (StarsU16)(packed & 1023UL); input.only_one = one;
    ops.lookup_hull = startup_final_hull; ops.race_stat = turn_score_resource_stat;
    input.ops = &ops; input.context = trace;
    stars_get_production_costs(&input, result);
    for (i = 0; i < 4; ++i) costs[i] = (StarsI32)result[i];
}
typedef struct StartupEta {
    TurinTrace *trace;
    StarsU8 planet[56];
    StarsU8 fraction;
} StartupEta;
static void eta_sync(StartupEta *eta, const StarsEstimateProdPlanet *planet)
{
    StarsU16 i;
    StarsU32 improvements = get_u32(eta->planet, 20U);
    for (i = 0; i < 3U; ++i)
        put_u32(eta->planet, (StarsU16)(28U + i*4U), (StarsU32)planet->minerals[i]);
    put_u32(eta->planet, 40U, (StarsU32)planet->population);
    improvements = (improvements & 255UL) | ((StarsU32)planet->mines << 8) |
        ((StarsU32)planet->factories << 20);
    put_u32(eta->planet, 20U, improvements);
}
static void eta_mined(void *opaque, StarsEstimateProdPlanet *planet, StarsI32 output[3])
{
    StartupEta *eta = (StartupEta *)opaque;
    eta_sync(eta, planet);
    /* Same EstMineralsMined result as apply=1; the scheduler adds it to its
     * private stock, so the real selected planet remains unchanged. */
    startup_final_estimate(eta->trace, eta->planet, output, -1, 0);
}
static StarsI32 eta_resources(void *opaque, const StarsEstimateProdPlanet *planet)
{
    StartupEta *eta = (StartupEta *)opaque;
    eta_sync(eta, planet);
    return startup_final_resources(eta->trace, eta->planet, 1);
}
static StarsI16 eta_costs(void *opaque, const StarsProducePlanet *planet,
    const StarsProduceItem *item, StarsI32 costs[4])
{
    StartupEta *eta = (StartupEta *)opaque;
    StarsU8 raw[4];
    (void)planet;
    put_u32(raw, 0U, ((StarsU32)item->object_kind << 17) |
        ((StarsU32)item->type << 10) | (StarsU16)item->quantity);
    startup_final_cost(eta->trace, eta->planet, raw, costs, 1, 1);
    return 1;
}
static void eta_to_build(const StarsEstimateProdItem *from, StarsProduceItem *to)
{
    memset(to, 0, sizeof *to);
    to->type = (StarsI16)((from->packed >> 10) & 127UL);
    to->object_kind = (StarsI16)((from->packed >> 17) & 7UL);
    to->quantity = (StarsI16)from->quantity;
    to->progress = (StarsI16)((from->packed >> 20) & 127UL);
    to->active = 1;
}
static void eta_from_build(const StarsProduceItem *from, StarsEstimateProdItem *to)
{
    to->quantity = (StarsU16)from->quantity;
    to->packed = (to->packed & 0xf8000000UL) | (StarsU16)from->quantity |
        ((StarsU32)from->type << 10) | ((StarsU32)from->object_kind << 17) |
        ((StarsU32)from->progress << 20);
}
static StarsI16 eta_build(void *opaque, StarsEstimateProdPlanet *planet,
    StarsEstimateProdItem *item, StarsEstimateProdItem *partial,
    StarsI32 budget[4], StarsI16 alchemy, StarsI16 *status)
{
    StartupEta *eta = (StartupEta *)opaque;
    StarsBuildProductionInput input; StarsBuildProductionOps ops;
    StarsProducePlanet build_planet; StarsProduceItem build_item, build_partial;
    StarsI16 built; StarsU16 i;
    memset(&input, 0, sizeof input); memset(&ops, 0, sizeof ops);
    memset(&build_planet, 0, sizeof build_planet);
    memset(&build_partial, 0, sizeof build_partial);
    eta_sync(eta, planet); eta_to_build(item, &build_item);
    /* This pair simulates finite ship production. Newly eligible automatic
     * or special items require their complete cost/capacity adapters. */
    if (build_item.object_kind != 2 || build_item.type >= 16 || alchemy) {
        ++eta->trace->failures; *status = 2; return 0;
    }
    build_planet.id = (StarsI16)get_u16(eta->planet, 0U); build_planet.owner = 1;
    input.planet = &build_planet; input.item = &build_item;
    input.partial = partial ? &build_partial : 0;
    input.status = status; input.ops = &ops;
    ops.costs = eta_costs; ops.context = eta;
    for (i = 0; i < 4U; ++i) input.resources[i] = budget[i];
    built = stars_build_production_item(&input);
    for (i = 0; i < 4U; ++i) budget[i] = input.resources[i];
    eta_from_build(&build_item, item);
    if (partial) eta_from_build(&build_partial, partial);
    ++eta->trace->basic_eta_builds;
    return built;
}
static void eta_population(void *opaque, StarsEstimateProdPlanet *planet,
    StarsI16 years)
{
    StartupEta *eta = (StartupEta *)opaque;
    StarsPopulationState state; StarsI16 race, growth;
    eta_sync(eta, planet);
    memset(&state, 0, sizeof state);
    state.owner = 1; state.population = planet->population; state.fraction = eta->fraction;
    state.desirability = turn_score_desirability(eta->trace, eta->planet, 1);
    race = turn_score_resource_stat(eta->trace, 1, 14);
    if (race == 8 || years != 1) { ++eta->trace->failures; return; }
    growth = (StarsI16)((signed char)eta->trace->complete_input.views[0].players[1][25] *
        (race == 0 ? 2 : 1));
    stars_change_population(&state, startup_final_max_population(eta->trace,
        get_u16(eta->planet, 0U), 1), growth, 0, 1);
    planet->population = (StarsI16)state.population; eta->fraction = state.fraction;
    ++eta->trace->basic_eta_years;
}
static void startup_blocked_queue(TurinTrace *trace)
{
    StartupEta eta;
    StarsEstimateProdInput input; StarsEstimateProdPlanet planet;
    StarsEstimateProdItem queue[64];
    StarsI32 cost[4], resources; StarsU32 packed; StarsU16 i, kind, group;
    StarsU8 saved_planet[56]; StarsU32 saved_queue[64];
    if (trace->turn_ai_queue.count == 0U || trace->turn_ai_queue.count > 64U) {
        ++trace->failures; return;
    }
    packed = get_u32(trace->turn_ai_queue.records, 0U);
    kind = (StarsU16)((packed >> 10) & 127UL); group = (StarsU16)((packed >> 17) & 7UL);
    ++trace->basic_blocked_calls;
    if (group == 1U && (kind == 8U || kind == 3U || kind == 11U || kind == 12U)) return;
    startup_select_current_planet(trace, trace->turn_committed_planet);
    memset(&eta, 0, sizeof eta); memset(&input, 0, sizeof input);
    memset(&planet, 0, sizeof planet); memset(queue, 0, sizeof queue);
    eta.trace = trace; memcpy(eta.planet, trace->final_queue_planet, sizeof eta.planet);
    memcpy(saved_planet, trace->final_queue_planet, sizeof saved_planet);
    memcpy(saved_queue, trace->turn_ai_queue.records, trace->turn_ai_queue.count * 4U);
    eta.fraction = eta.planet[20];
    for (i = 0; i < 3U; ++i) planet.minerals[i] = (StarsI32)get_u32(eta.planet, (StarsU16)(28U+4U*i));
    planet.population = (StarsI16)get_u32(eta.planet, 40U);
    planet.flags = get_u32(eta.planet, 24U);
    planet.mines = (StarsU16)((get_u32(eta.planet, 20U) >> 8) & 4095UL);
    planet.factories = (StarsU16)((get_u32(eta.planet, 20U) >> 20) & 4095UL);
    planet.research_percent = startup_final_percent(trace, 1);
    for (i = 0; i < trace->turn_ai_queue.count; ++i) {
        queue[i].packed = get_u32(trace->turn_ai_queue.records, (StarsU16)(i*4U));
        queue[i].quantity = (StarsU16)(queue[i].packed & 1023UL);
    }
    input.planet = &planet; input.queue = queue; input.queue_count = trace->turn_ai_queue.count;
    input.first_turn = &trace->basic_eta_first; input.last_turn = &trace->basic_eta_last;
    input.estimate_mined = eta_mined; input.resources_at_planet = eta_resources;
    input.build_item_mutable = eta_build; input.change_population = eta_population;
    input.context = &eta;
    stars_estimate_item_prod_schedule(&input);
    if (memcmp(saved_planet, trace->final_queue_planet, sizeof saved_planet) ||
            memcmp(saved_queue, trace->turn_ai_queue.records, trace->turn_ai_queue.count*4U))
        ++trace->failures;
    if (trace->basic_eta_first == 1) return;
    startup_final_cost(trace, trace->final_queue_planet, trace->turn_ai_queue.records, cost, 1, 1);
    resources = startup_final_resources(trace, trace->final_queue_planet, 1);
    if (!(planet.flags & (1UL << 23))) resources -= resources * planet.research_percent / 100L;
    trace->basic_blocked_unit_cost = cost[3]; trace->basic_blocked_resources = resources;
    fprintf(stderr, "AI_BLOCKED_QUEUE first=%d last=%d builds=%u years=%u unit=%ld resources=%ld\n",
        trace->basic_eta_first, trace->basic_eta_last, trace->basic_eta_builds,
        trace->basic_eta_years, (long)cost[3], (long)resources);
    /* Source only tries inserted mines/alchemy when delay is mineral-limited.
     * Resource-limited builds return unchanged; reject newly eligible work. */
    if (cost[3] <= resources * ((trace->basic_eta_first == -1 ? 600 : trace->basic_eta_first) - 1L))
        ++trace->failures;
}

static StarsI16 startup_final_unexpected_add(void *opaque, StarsI16 kind,
    StarsU16 quantity, StarsI16 now, StarsI16 flags)
{
    (void)kind; (void)quantity; (void)now; (void)flags;
    ++((TurinTrace *)opaque)->failures;
    return 0;
}
static StarsU16 startup_final_warp_items(void *opaque, StarsI16 player,
    StarsU16 slot, StarsWarpMAItem *items, StarsU16 capacity)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    const StarsU8 *design;
    StarsU16 i, count;
    if (player != 1 || slot >= 10U) { ++trace->failures; return 0; }
    design = trace->starbase_designs + slot * 147U;
    count = design[122];
    if (count > 16U || count > capacity) { ++trace->failures; return 0; }
    for (i = 0; i < count; ++i) {
        items[i].identifier = get_u16(design, (StarsU16)(58U + i * 4U));
        items[i].flags = get_u16(design, (StarsU16)(60U + i * 4U));
    }
    return count;
}
static StarsI16 startup_final_terra_part(void *opaque, StarsU16 group,
    StarsU16 item, StarsI16 *dp, StarsI16 *cost)
{
    StarsAiPart part;
    if (!startup_lookup_part(opaque, group, item, &part)) return 0;
    *dp = (StarsI16)get_u16(part.component, 52U);
    *cost = (StarsI16)get_u16(part.component, 42U);
    return 1;
}
static StarsI16 startup_final_terra(void *opaque, const StarsU8 *raw,
    StarsI16 *minimum, StarsI16 *maximum, StarsI16 *cost, StarsI16 help)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsCanTerraformInput input;
    StarsCanTerraformPlanet planet;
    StarsI16 player = 1, i;
    memset(&input, 0, sizeof input); memset(&planet, 0, sizeof planet);
    planet.player = (StarsI16)get_u16(raw, 2U);
    for (i = 0; i < 3; ++i) {
        planet.environment[i] = (signed char)raw[12 + i];
        planet.original_environment[i] = (signed char)raw[15 + i];
        input.race_center[i] = (signed char)trace->complete_input.views[0].players[1][16 + i];
        input.race_environment_enabled[i] =
            (signed char)trace->complete_input.views[0].players[1][19 + i];
    }
    input.planet = &planet; input.current_player = &player;
    input.lookup_part = startup_final_terra_part; input.context = trace;
    return stars_can_terraform_lppl(&input, minimum, maximum, cost, help);
}
static StarsI16 startup_selection_planet(void *opaque, StarsI16 id,
    StarsChangeMainPlanet *planet)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    const Soe3InputView *view = &trace->complete_input.views[2];
    StarsU16 i;
    for (i = 0; i < view->planet_count; ++i) {
        if (view->planets[i].id != id) continue;
        planet->id = id; planet->point = trace->points[id];
        pack_loaded_planet(trace->final_queue_planet, &view->planets[i]);
        if (id == trace->planet.id)
            put_u32(trace->final_queue_planet, 40U, trace->supply_planet.minerals[3]);
        ++trace->ai_selection_loads;
        return 1;
    }
    ++trace->failures; return 0;
}
static void startup_selection_copy(void *opaque, const StarsChangeMainFleet *from,
    StarsChangeMainFleet *to)
{
    (void)opaque; *to = *from;
}
static void startup_selection_invalidate(void *opaque, StarsI16 report,
    StarsI16 reload)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsInvalidateReportInput input;
    StarsInvalidateReportRecord *current = &trace->fleet_report;
    memset(&input, 0, sizeof input);
    input.current = &current; input.report = report; input.reload = reload;
    input.ai_mode = trace->ai_input.ai_active;
    stars_invalidate_report(&input);
    if (current != &trace->fleet_report) ++trace->failures;
}
static StarsI16 startup_selection_fleet(void *opaque, StarsI16 id,
    StarsChangeMainFleet *fleet)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    const StarsU8 *raw;
    if (id < 0 || id >= TURIN_RETAINED_FLEET_COUNT ||
            !trace->raw_fleet_present[id]) { ++trace->failures; return 0; }
    raw = trace->raw_fleets[id];
    memset(fleet, 0, sizeof *fleet);
    fleet->id_planet = (StarsI16)get_u16(raw, 6U);
    fleet->player = (StarsI16)get_u16(raw, 2U);
    fleet->point.x = (StarsI16)get_u16(raw, 8U);
    fleet->point.y = (StarsI16)get_u16(raw, 10U);
    fleet->planet_slot = id;
    fleet->order_max = trace->plord[id][3]; fleet->present = 1;
    return 1;
}
static void startup_select_current_object(TurinTrace *trace, StarsI16 kind, StarsI16 id)
{
    StarsChangeMainObjInput input;
    StarsChangeMainFleet fleets[TURIN_RETAINED_FLEET_COUNT];
    StarsU16 i, count = 0;
    const StarsU8 *raw;
    memset(&input, 0, sizeof input); memset(fleets, 0, sizeof fleets);
    for (i = 0; i < trace->fleet_count; ++i) {
        StarsU16 fleet_id;
        raw = trace->fleet_items[i]->bytes;
        fleet_id = get_u16(raw, 0U);
        if (fleet_id >= TURIN_RETAINED_FLEET_COUNT || !trace->raw_fleet_present[fleet_id]) {
            ++trace->failures; return;
        }
        raw = trace->raw_fleets[fleet_id];
        fleets[count].id_planet = (StarsI16)get_u16(raw, 6U);
        fleets[count].player = (StarsI16)get_u16(raw, 2U);
        fleets[count].point.x = (StarsI16)get_u16(raw, 8U);
        fleets[count].point.y = (StarsI16)get_u16(raw, 10U);
        fleets[count].planet_slot = (StarsI16)fleet_id;
        fleets[count].order_max = trace->plord[fleet_id][3];
        fleets[count].present = 1; ++count;
    }
    input.f_ai = trace->ai_input.ai_active; input.current_player = 1;
    input.selection = &trace->ai_selection;
    input.fleets = fleets; input.fleet_count = (StarsI16)count;
    input.planet_points = trace->points;
    input.lookup_planet = startup_selection_planet;
    input.lookup_fleet = startup_selection_fleet;
    input.copy_fleet = startup_selection_copy;
    input.invalidate_report = startup_selection_invalidate;
    input.fill_planet_production = turn_finish_unexpected_ui;
    input.context = trace;
    stars_change_main_obj_sel(kind, id, &input);
    if (trace->ai_selection.grobj != kind || trace->ai_selection.id != id ||
            (kind == 1 && trace->ai_selection.planet_id != id)) ++trace->failures;
    trace->session.selected_kind = (StarsU16)trace->ai_selection.grobj;
    trace->session.selected_full_kind = (StarsU16)trace->ai_selection.grobj_full;
    trace->session.selected_id = trace->ai_selection.id;
    trace->session.selected_point = trace->ai_selection.point;
    trace->session.selected_planet = trace->ai_selection.planet_id;
    trace->session.selected_waypoint = trace->ai_selection.active_waypoint;
    ++trace->ai_selection_calls;
}
static void startup_select_current_planet(TurinTrace *trace, StarsI16 id)
{
    startup_select_current_object(trace, 1, id);
}
static void startup_final_select(void *opaque, StarsI16 kind, StarsI16 id)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    if (kind != 1 || id != trace->turn_committed_planet ||
            trace->final_queue_selection_calls != trace->final_queue_phase) ++trace->failures;
    startup_select_current_planet(trace, id);
    trace->final_queue_saved_flags = (StarsU16)((trace->game_flags & ~0x20U) |
        (((get_u32(trace->final_queue_planet, 24U) >> 23) & 1UL) << 5));
    trace->game_flags = trace->final_queue_saved_flags;
    ++trace->final_queue_selection_calls;
}
static void startup_build_catalog(TurinTrace *trace, StarsProductionList *available,
    StarsProductionList *queue, StarsU16 saved_flags)
{
    StarsInitProductionInput input;
    StarsCMaxMinesOps capacity;
    StarsCMaxDefensesOps defense;
    StarsU32 ships[16], bases[10];
    StarsU8 hull[143];
    const StarsU8 *raw = trace->final_queue_planet;
    const StarsU8 *design;
    StarsU16 i, flags, current, cargo = 0;
    StarsAiPart part;
    memset(&input, 0, sizeof input); memset(&capacity, 0, sizeof capacity);
    memset(&defense, 0, sizeof defense);
    memset(queue, 0, sizeof *queue);
    current = (StarsU16)(get_u32(raw, 44U) & 15U);
    flags = get_u16(raw, 4U);
    if ((flags & 0x200U) != 0) {
        if (current >= 10U || !scrap_read_hull(trace,
                get_u16(trace->starbase_designs + current * 147U, 0U), hull, sizeof hull)) {
            ++trace->failures; return;
        }
        cargo = get_u16(hull, 52U);
    }
    for (i = 0; i < 16U && cargo != 0U; ++i) {
        design = trace->designs + i * 147U;
        if ((get_u16(design, 123U) & 0x8200U) == 0U &&
                get_u16(design, 40U) <= cargo)
            ships[input.ship_item_count++] = 0x40000UL | ((StarsU32)i << 10) | 0x3ffUL;
    }
    for (i = 0; i < 10U; ++i) {
        design = trace->starbase_designs + i * 147U;
        if ((get_u16(design, 123U) & 0x8200U) == 0U &&
                (i != current || (flags & 0x200U) == 0U))
            bases[input.starbase_item_count++] = 0x40000UL |
                ((StarsU32)(i + 16U) << 10) | 1UL;
    }
    input.ship_items = ships; input.starbase_items = bases;
    input.available = available; input.queue = queue;
    input.planetary_part_14_lookup = startup_lookup_part(trace, 0x8000U, 14U, &part);
    input.warp_gate_count = (StarsU16)stars_iwarp_ma_from_planet(raw, 1, 0,
        startup_final_warp_items, 0, trace);
    capacity.calc_max_population = startup_final_max_population;
    capacity.get_player_stat = turn_score_resource_stat;
    input.factories_current = (StarsU16)((get_u32(raw, 20U) >> 20) & 4095UL);
    input.mines_current = (StarsU16)((get_u32(raw, 20U) >> 8) & 4095UL);
    input.factories_max = (StarsU16)stars_cmax_factories(raw, 1, &capacity, trace);
    input.mines_max = (StarsU16)stars_cmax_mines(raw, 1, &capacity, trace);
    defense.planet_desirability = turn_score_desirability;
    defense.get_player_stat = turn_score_resource_stat;
    input.defenses_current = (StarsU16)(get_u32(raw, 24U) & 4095UL);
    input.defenses_max = (StarsU16)stars_cmax_defenses(raw, 1, &defense, trace);
    input.scanner_type = (StarsU16)((get_u32(raw, 24U) >> 12) & 31UL);
    input.terraform_count = (StarsU16)stars_ipct_can_terraform(raw, startup_final_terra, trace);
    input.race_stat = (StarsU16)turn_score_resource_stat(trace, 1, 14);
    input.game_flags = saved_flags; input.tutorial_player = 1;
    input.advance_tutor = turn_finish_unexpected_ui; input.context = trace;
    queue->item_count = (StarsU8)trace->turn_ai_queue.count;
    for (i = 0; i < trace->turn_ai_queue.count; ++i)
        queue->items[i] = get_u32(trace->turn_ai_queue.records, (StarsU16)(i * 4U));
    stars_init_production(&input);
    /* Reconciliation preserves the current empty or two-item queue. */
    if (queue->item_count != trace->turn_ai_queue.count)
        ++trace->failures;
    for (i = 0; i < queue->item_count; ++i)
        if (queue->items[i] != get_u32(trace->turn_ai_queue.records,
                (StarsU16)(i * 4U))) ++trace->failures;
}
static void startup_final_initialize(void *opaque, StarsU8 *scratch, StarsU16 size)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    if (size != 256U || !scratch || trace->final_queue_live ||
            trace->final_queue_selection_calls != trace->final_queue_phase + 1U) {
        ++trace->failures; return;
    }
    memset(scratch, 0, size);
    trace->final_queue_live = 1;
    startup_build_catalog(trace, &trace->final_queue_catalog,
        &trace->final_queue_copy, trace->final_queue_saved_flags);
    ++trace->final_queue_init_calls;
}
static void startup_final_free(void *opaque, StarsU8 *product)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    if (!trace->final_queue_live || product != (StarsU8 *)&trace->final_queue_copy)
        ++trace->failures;
    trace->final_queue_live = 0;
    ++trace->final_queue_free_calls;
}
static void startup_final_finish(void *opaque, StarsI16 changed)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsFinishProductionInput input;
    StarsFinishProductionOps ops;
    if (changed != 0 || trace->final_queue_budget_calls != trace->final_queue_phase ||
            trace->final_queue_init_calls != trace->final_queue_phase + 1U) { ++trace->failures; return; }
    memset(&input, 0, sizeof input); memset(&ops, 0, sizeof ops);
    input.planet_bits = get_u32(trace->final_queue_planet, 24U);
    input.game_flags = trace->final_queue_saved_flags;
    input.player = 1; input.planet_id = trace->turn_committed_planet; input.ai_active = 1;
    input.selected_product = trace->turn_ai_queue.records;
    input.global_product = (StarsU8 *)&trace->final_queue_copy;
    input.global_count = trace->final_queue_copy.item_count;
    ops.free_product = startup_final_free; ops.fill_list = turn_finish_unexpected_ui;
    ops.draw_ship = turn_finish_unexpected_draw; ops.advance_tutor = turn_finish_unexpected_ui;
    input.ops = &ops; input.context = trace;
    stars_finish_production(&input);
    if (input.global_product != 0 || input.selected_product != trace->turn_ai_queue.records ||
            input.planet_bits != get_u32(trace->final_queue_planet, 24U)) ++trace->failures;
    put_u32(trace->final_queue_planet, 24U, input.planet_bits);
    ++trace->final_queue_finish_calls;
}
static StarsI16 startup_final_budget(void *opaque, const StarsU8 *planet)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsGetResourcesAvailableOps resources;
    StarsGetProdQCostOps queue;
    StarsFillProdMinesAndFactoriesInput fill;
    StarsU8 raw[56];
    if (trace->turn_committed_planet != 24 || trace->turn_seed_finish_calls != 1 ||
            trace->final_queue_budget_calls != 0) {
        ++trace->failures; return 0;
    }
    if (planet != trace->final_queue_planet || !trace->final_queue_live)
        ++trace->failures;
    memcpy(raw, trace->final_queue_planet, sizeof raw);
    memset(&resources, 0, sizeof resources); memset(&queue, 0, sizeof queue);
    resources.estimate_minerals = startup_final_estimate;
    resources.resources_at_planet = startup_final_resources;
    resources.research_percent = startup_final_percent;
    queue.get_queue = startup_final_queue; queue.get_costs = startup_final_cost;
    stars_get_resources_available(raw, trace->final_queue_available, 1,
        get_u32(raw, 24U), &resources, trace);
    stars_get_prod_q_cost(raw, trace->final_queue_cost, 1, &queue, trace);
    ++trace->final_queue_budget_calls;
    if (trace->final_queue_available[3] >= trace->final_queue_cost[3]) {
        ++trace->failures; return 0;
    }
    /* The derived resource deficit proves the original return occurs before
     * catalog access. Reject newly eligible filling until its full binding. */
    memset(&fill, 0, sizeof fill);
    memcpy(fill.available, trace->final_queue_available, sizeof fill.available);
    memcpy(fill.queued_cost, trace->final_queue_cost, sizeof fill.queued_cost);
    fill.add_item = startup_final_unexpected_add; fill.context = trace;
    if (stars_fill_prod_mines_and_factories(&fill) != 0) {
        ++trace->failures; return 0;
    }
    return 0;
}

static StarsI16 startup_basic_begin(TurinTrace *trace, StarsI16 id)
{
    StarsGetResourcesAvailableOps resources;
    StarsGetProdQCostOps queue;
    StarsU8 scratch[256];
    StarsU16 mode, i;
    StarsU32 population;
    if (trace->basic_queue_begin_calls != 0U || trace->final_queue_phase != 0U ||
            id != trace->complete_input.views[2].planets[0].id) {
        ++trace->failures; return 0;
    }
    pack_loaded_planet(trace->final_queue_planet,
        &trace->complete_input.views[2].planets[0]);
    population = trace->supply_planet.minerals[3];
    put_u32(trace->final_queue_planet, 40U, population);
    mode = (StarsU16)(get_u16(trace->complete_input.views[0].players[1], 6U) >> 13);
    if (population <= (mode == 0U ? 39UL : 59UL)) {
        /* The current pair enters the population gate. Marker-based
         * admission on other inputs needs a separate binding. */
        ++trace->failures; return 0;
    }
    memset(&resources, 0, sizeof resources); memset(&queue, 0, sizeof queue);
    resources.estimate_minerals = startup_final_estimate;
    resources.resources_at_planet = startup_final_resources;
    resources.research_percent = startup_final_percent;
    queue.get_queue = startup_final_queue; queue.get_costs = startup_final_cost;
    stars_get_resources_available(trace->final_queue_planet,
        trace->basic_queue_available, 1, get_u32(trace->final_queue_planet, 24U),
        &resources, trace);
    stars_get_prod_q_cost(trace->final_queue_planet,
        trace->basic_queue_cost, 1, &queue, trace);
    /* Original i<3 excludes mineral deficits, but deliberately admits a
     * resource-only deficit in lane 3. Do not require all four affordable. */
    if (mode != 5U)
        for (i = 0; i < 3U; ++i)
            if (trace->basic_queue_available[i] < trace->basic_queue_cost[i]) {
                ++trace->failures; return 0;
            }
    startup_final_select(trace, 1, id);
    startup_final_initialize(trace, scratch, sizeof scratch);
    ++trace->basic_queue_begin_calls;
    return trace->final_queue_live != 0U;
}
static void startup_basic_finish(TurinTrace *trace)
{
    if (trace->basic_queue_begin_calls != 1U || trace->basic_queue_finish_calls != 0U) {
        ++trace->failures; return;
    }
    startup_final_finish(trace, 0);
    ++trace->basic_queue_finish_calls;
}

static void startup_basic_candidates(TurinTrace *trace,
    const StarsProductionList *list, StarsAiProductionCandidate *items)
{
    StarsU16 i;
    memset(items, 0, sizeof(*items) * 128U);
    for (i = 0; i < list->item_count; ++i) {
        StarsU32 word = list->items[i];
        items[i].category = (StarsU8)((word >> 17) & 7UL);
        items[i].subtype = (StarsU16)((word >> 10) & 127UL);
        items[i].quantity = (StarsU16)(word & 1023UL);
        items[i].queue_type = (StarsI16)items[i].subtype;
        /* These pinned catalogs contain no scanner or terraforming choice.
         * Such an item would need actual per-item costing before admission. */
        if (items[i].category == 1U && (items[i].subtype == 12U ||
                (items[i].subtype >= 18U && items[i].subtype <= 26U)))
            ++trace->failures;
    }
}
static StarsI16 startup_basic_unexpected_packet(void *opaque, StarsI16 type,
    StarsI16 quantity)
{
    startup_unexpected_defense_add(opaque, type, quantity);
    return 0;
}
static void startup_basic_packet_scanner(TurinTrace *trace)
{
    StarsAiProductionCandidate pending[128], catalog[128];
    StarsAiFlingPlanet planet;
    StarsAiFlingOps ops;
    StarsI32 total;
    StarsU16 i;
    const StarsReadPlanetState *loaded = &trace->complete_input.views[2].planets[0];
    if (!trace->final_queue_live || trace->startup_upgrade_calls != 1) {
        ++trace->failures; return;
    }
    total = trace->basic_queue_available[0] + trace->basic_queue_available[1] +
        trace->basic_queue_available[2];
    if (total > 3000L) { ++trace->failures; return; }
    memset(&planet, 0, sizeof planet); memset(&ops, 0, sizeof ops);
    planet.id = (StarsU16)loaded->id; planet.point = trace->points[loaded->id];
    planet.owner = loaded->player; planet.turn_seen = loaded->turn;
    planet.flags = loaded->flags;
    for (i = 0; i < 3U; ++i)
        planet.mineral_values[i] = (StarsU16)loaded->minerals[i];
    ops.random_value = startup_prepare_random;
    ops.queue_item = startup_basic_unexpected_packet;
    if (stars_f_ai_fling(&planet, 1U, planet.point,
            get_u16(trace->complete_input.game, 18U), 1, trace->basic_queue_available,
            (StarsU16)stars_iwarp_ma_from_planet(trace->final_queue_planet, 1, 0,
                startup_final_warp_items, 0, trace), &ops, trace) != 0)
        ++trace->failures;
    ++trace->basic_packet_calls;
    startup_basic_candidates(trace, &trace->final_queue_copy, pending);
    startup_basic_candidates(trace, &trace->final_queue_catalog, catalog);
    if (stars_f_queue_ai_scanner(pending, trace->final_queue_copy.item_count,
            catalog, trace->final_queue_catalog.item_count, trace->basic_queue_available,
            trace->basic_queue_cost, startup_unexpected_defense_add, trace) != 0)
        ++trace->failures;
    ++trace->basic_scanner_calls;
}
static void startup_basic_terraform(TurinTrace *trace)
{
    StarsAiProductionCandidate pending[128], catalog[128];
    StarsU16 mode, i;
    StarsU32 population = get_u32(trace->final_queue_planet, 40U);
    mode = (StarsU16)(get_u16(trace->complete_input.views[0].players[1], 6U) >> 13);
    if (mode == 0U || !trace->final_queue_live) { ++trace->failures; return; }
    for (i = 0; i < 3U; ++i)
        if (trace->final_queue_planet[12U + i] !=
                trace->complete_input.views[0].players[1][16U + i])
            ++trace->failures;
    startup_basic_candidates(trace, &trace->final_queue_copy, pending);
    startup_basic_candidates(trace, &trace->final_queue_catalog, catalog);
    if (stars_f_queue_ai_terraforming((StarsI16)mode,
            (StarsI32)(StarsI16)(population >> 16), (StarsU16)population,
            pending, trace->final_queue_copy.item_count, catalog,
            trace->final_queue_catalog.item_count, trace->basic_queue_cost,
            startup_unexpected_defense_add, trace) != 0) ++trace->failures;
    ++trace->basic_terraform_calls;
}

static StarsI16 startup_final_queue_pass(void *opaque)
{
    TurinTrace *trace = startup_trace(opaque);
    StarsProductionQueueOps ops;
    const StarsU8 *planets[1];
    StarsI16 failures = trace->failures;
    if (trace->startup_owned_planet_count != 1U ||
            trace->startup_owned_planets[0] != (StarsU16)trace->turn_committed_planet ||
            trace->complete_input.views[2].planet_count != 1U ||
            trace->complete_input.views[2].planets[0].player != 1) {
        ++trace->failures; return 0;
    }
    if (trace->ai_selection_calls != 4U || trace->ai_selection_loads != 2U) {
        ++trace->failures; return 0;
    }
    if (trace->first_queue_init_calls != 1U ||
            trace->basic_queue_finish_calls != 1U || trace->final_queue_live != 0U ||
            trace->basic_packet_calls != 1U || trace->basic_scanner_calls != 1U ||
            trace->basic_terraform_calls != 1U || trace->basic_keep_calls != 1U ||
            trace->basic_speed_calls != 1U || trace->basic_speed_fuel_calls == 0U ||
            trace->basic_keep_fleets != trace->fleet_count ||
            trace->basic_starbase_calls != 1U ||
            trace->basic_starbase_visits != trace->complete_input.views[2].planet_count ||
            trace->basic_attack_gate_calls != 1U || trace->basic_blocked_calls != 1U ||
            trace->basic_eta_builds == 0U) {
        ++trace->failures; return 0;
    }
    trace->final_queue_phase = 1U;
    planets[0] = trace->final_queue_planet;
    memset(&ops, 0, sizeof ops);
    ops.change_selection = startup_final_select;
    ops.initialize = startup_final_initialize;
    ops.fill = startup_final_budget;
    ops.finish = startup_final_finish;
    stars_fill_production_queue(planets, 1U, &ops, trace);
    if (trace->final_queue_selection_calls != 2U || trace->final_queue_init_calls != 2U ||
            trace->final_queue_finish_calls != 2U || trace->final_queue_free_calls != 2U ||
            trace->final_queue_live != 0U) ++trace->failures;
    if (trace->ai_selection_calls != 5U || trace->ai_selection_loads != 2U)
        ++trace->failures;
    return trace->failures == failures;
}

/* Bind production's research boundary before random events.  All inputs
 * come from mutable player state and current planet resource allocations. */
static StarsI16 turn_research_grbit(void *opaque, StarsI16 player, StarsI16 bit)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    if (player < 0 || player >= 2 || bit < 0 || bit >= 32) {
        ++trace->failures; return 0;
    }
    return (StarsI16)((get_u32(trace->complete_input.views[0].players[player],
        78U) >> bit) & 1UL);
}

static StarsU32 turn_research_cost(void *opaque, StarsI16 tech,
    StarsI16 level, StarsI16 player)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsTechLevelCostInput input;
    StarsTechLevelCostOps ops;
    memset(&input, 0, sizeof input); memset(&ops, 0, sizeof ops);
    if (player < 0 || player >= 2) { ++trace->failures; return 0UL; }
    /* The legacy API calls these colonist_counts; the original sums rgTech. */
    input.colonist_counts = (const char *)trace->complete_input.views[0].players[player] + 26;
    input.level_costs = original_research_costs; input.level_count = 27U;
    input.game_flags = get_u16(trace->complete_input.game, 16U);
    input.tech = tech; input.level = level; input.player = player;
    ops.get_race_stat = turn_score_resource_stat;
    return (StarsU32)stars_get_tech_level_cost(&input, &ops, trace);
}

static void turn_research_level_message(void *opaque, StarsI16 player,
    StarsI16 message, StarsI16 a, StarsI16 b, StarsI16 c, StarsI16 d)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    /* Neither pinned input reaches a breakthrough.  Do not silently accept
     * a newly eligible level/unlock path before its integration is bound. */
    (void)player; (void)message; (void)a; (void)b; (void)c; (void)d;
    ++trace->failures;
}

static void turn_research_pool_message(void *opaque, StarsI16 player,
    StarsI16 message, StarsI16 tech, StarsI16 share)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsI16 *row;
    StarsI16 parameters[7];
    memset(parameters, 0, sizeof parameters);
    parameters[0] = tech; parameters[1] = share;
    turn_send_message(trace, player, message, -2, parameters);
    if (trace->turn_research_pool_count >= 12) { ++trace->failures; return; }
    row = trace->turn_research_pool_messages[trace->turn_research_pool_count++];
    row[0] = player; row[1] = message; row[2] = -2; row[3] = tech; row[4] = share;
}

static void turn_update_current_research(void *opaque, StarsI16 use_pool)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsResearchPlayer players[2];
    StarsResearchInput input;
    StarsResearchOps ops;
    StarsU16 index, tech;
    StarsU8 *raw;
    memset(players, 0, sizeof players); memset(&input, 0, sizeof input);
    memset(&ops, 0, sizeof ops);
    for (index = 0U; index < 2U; ++index) {
        raw = trace->complete_input.views[0].players[index];
        players[index].id = (StarsI16)index;
        players[index].dead_flags = get_u16(raw, 84U);
        players[index].current_technology = raw[57];
        players[index].current_mode = (StarsU8)(raw[57] >> 4);
        players[index].research_last_year = get_u32(raw, 58U);
        for (tech = 0U; tech < 6U; ++tech) {
            players[index].technology_level[tech] = raw[26U + tech];
            players[index].research[tech] = get_u32(raw, (StarsU16)(32U + tech * 4U));
        }
    }
    ops.race_stat = turn_score_resource_stat; ops.race_grbit = turn_research_grbit;
    ops.level_cost = turn_research_cost; ops.send_message = turn_research_level_message;
    ops.send_pool_message = turn_research_pool_message;
    input.players = players; input.player_count = 2U; input.use_pool = use_pool;
    input.game_flags = get_u16(trace->complete_input.game, 16U);
    input.ops = &ops; input.context = trace;
    stars_update_research_status(&input);
    for (index = 0U; index < 2U; ++index) {
        raw = trace->complete_input.views[0].players[index];
        raw[57] = (StarsU8)((players[index].current_technology & 15U) |
            (players[index].current_mode << 4));
        for (tech = 0U; tech < 6U; ++tech) {
            raw[26U + tech] = players[index].technology_level[tech];
            put_u32(raw, (StarsU16)(32U + tech * 4U), players[index].research[tech]);
        }
    }
}

static void turn_order_research(void *opaque, StarsI16 use_pool)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    if (use_pool != 0 || trace->turn_order_research_calls >= 2U ||
            trace->turn_order_pass_calls != (trace->turn_order_research_calls ? 3 : 1) ||
            trace->turn_produce_calls != (StarsI16)trace->turn_order_research_calls) {
        ++trace->failures; return;
    }
    ++trace->turn_order_research_calls;
    turn_update_current_research(trace, use_pool);
}

static void turn_produce_research(void *opaque, StarsI16 use_pool)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsU16 index;
    StarsU8 *raw;
    if (trace->turn_production_commits != 1 ||
            trace->turn_production_planet_visits != 32) ++trace->failures;
    ++trace->turn_research_calls;
    for (index = 0U; index < trace->complete_input.views[0].planet_count; ++index) {
        StarsProducePlanet *planet = &trace->turn_produce_planets[index];
        if (planet->owner < 0) continue;
        raw = trace->complete_input.views[0].players[planet->owner];
        put_u32(raw, 58U, get_u32(raw, 58U) + (StarsU32)planet->last_year_resources);
    }
    turn_update_current_research(trace, use_pool);
    for (index = 0U; index < 2U; ++index) {
        raw = trace->complete_input.views[0].players[index];
        if (memcmp(raw + 26U, expected_research_players[trace->seeded_action][index], 36U) != 0) {
            fprintf(stderr, "research player %u differs from original\n", index);
            ++trace->failures;
        }
    }
    if (trace->turn_research_pool_count != 1 ||
            trace->turn_research_pool_messages[0][0] != 1 ||
            trace->turn_research_pool_messages[0][1] != 0x159 ||
            trace->turn_research_pool_messages[0][3] != 0 ||
            trace->turn_research_pool_messages[0][4] != 8)
        ++trace->failures;
}

static void turn_produce(void *opaque, StarsGenerateTurnState *state)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsProducePlanet planets[32];
    StarsProduceOps ops;
    StarsProduceInput input;
    StarsU16 index;
    StarsI16 mineral;
    (void)state;
    ++trace->turn_produce_calls;
    memset(planets, 0, sizeof planets);
    if (trace->turn_queue_prepare_calls != 1 ||
            trace->turin_input.production_hulls_queued != 2 ||
            trace->turn_seed_finish_calls != 1)
        ++trace->failures;
    memset(&ops, 0, sizeof ops);
    memset(&input, 0, sizeof input);
    if (trace->complete_input.views[0].planet_count != 32U) {
        ++trace->failures; return;
    }
    for (index = 0U; index < 32U; ++index) {
        const StarsReadPlanetState *loaded = &trace->turn_planet_states[index];
        StarsI16 owner = loaded->player;
        planets[index].id = loaded->id;
        planets[index].owner = owner;
        for (mineral = 0; mineral < 3; ++mineral)
            planets[index].mineral[mineral] = (StarsI32)loaded->minerals[mineral];
        if (owner < 0) continue;
        if (owner >= 2) { ++trace->failures; return; }
        planets[index].cheater = (StarsI16)((get_u16(
            trace->complete_input.views[0].players[owner], 84U) >> 2) & 1U);
        planets[index].research_percent = trace->complete_input.views[0].players[owner][56];
        planets[index].research_exempt = (StarsI16)((loaded->opaque_20[1] >> 23) & 1UL);
        planets[index].factory_count = (StarsI16)((loaded->opaque_20[0] >> 20) & 4095UL);
        planets[index].mine_count = (StarsI16)((loaded->opaque_20[0] >> 8) & 4095UL);
        planets[index].defense_count = (StarsI16)(loaded->opaque_20[1] & 4095UL);
    }
    planets[24].items = trace->turn_committed_queue;
    planets[24].item_count = trace->turn_committed_queue_count;
    planets[24].item_capacity = trace->turn_committed_queue_count;
    trace->turn_produce_planets = planets;
    ops.mine_minerals = turn_produce_mine_minerals;
    ops.resources_at_planet = turn_produce_resources;
    ops.planet_extra = turn_produce_extra;
    ops.build_item_with_resources = turn_produce_build_item;
    ops.send_message = turn_produce_message;
    ops.update_populations = turn_produce_update_population;
    ops.update_research = turn_produce_research;
    ops.random_events = turn_produce_random_events;
    ops.context = trace;
    input.planets = planets;
    input.planet_count = trace->complete_input.views[0].planet_count;
    input.game_flags = get_u16(trace->complete_input.game, 16U);
    input.ops = &ops;
    stars_produce(&input);
    if (trace->turn_produce_message_calls != 1 ||
            trace->turn_produce_message_player[0] != 0 ||
            trace->turn_produce_message_code[0] != 0x3f ||
            trace->turn_produce_message_planet[0] != 10 ||
            trace->turn_produce_message_object[0] != 10 ||
            trace->turn_produce_message_value[0] != 0 ||
            planets[10].mineral[0] != trace->mining_stock[0][0] ||
            planets[10].mineral[1] != trace->mining_stock[0][1] ||
            planets[10].mineral[2] != trace->mining_stock[0][2] ||
            trace->turn_produce_build_calls != 1 ||
            trace->turn_produce_resource_calls != 2 ||
            trace->turn_produce_base_resources != 32L ||
            planets[24].cheater != 0 ||
            planets[24].research_percent != 15 ||
            planets[24].research_exempt != 0 ||
            planets[10].last_year_resources != 35L ||
            planets[24].last_year_resources != 4L ||
            planets[24].item_count != 2U ||
            trace->turn_produce_queue[0].progress != 81 ||
            trace->turn_produce_queue[1].progress != 0 ||
            trace->turn_produce_queue[0].quantity != 1 ||
            trace->turn_produce_queue[1].quantity != 1 ||
            trace->turn_production_commits != 1 ||
            trace->turn_production_planet_visits != 32 ||
            trace->turn_research_calls != 1 ||
            trace->turn_produce_population_calls != 2 ||
            trace->turn_produce_random_event_calls != 1) {
        fprintf(stderr, "produce summary base=%ld calls=%d supply=%lu cheat=%d pct=%d exempt=%d last=%ld prog=%d builds=%d\n",
            (long)trace->turn_produce_base_resources,
            trace->turn_produce_resource_calls,
            (unsigned long)trace->supply_planet.minerals[3],
            planets[24].cheater, planets[24].research_percent,
            planets[24].research_exempt,
            (long)planets[24].last_year_resources,
            trace->turn_produce_queue[0].progress,
            trace->turn_produce_build_calls);
        ++trace->failures;
    }
    trace->turn_produce_planets = (StarsProducePlanet *)0;
}

static void turn_update_guesses(void *opaque,
    StarsGenerateTurnState *state)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsU16 index;
    (void)state;
    ++trace->turn_update_guesses_calls;
    run_retained_update_guesses(trace);
    for (index = 0U; index < TURIN_RETAINED_POINT_COUNT; ++index)
        trace->turn_planet_states[index].routing_word =
            trace->runtime_guess_planets[index].guess_word;
}

static StarsI16 turn_host_mark_open(void *opaque, StarsU16 type, StarsI16 player, StarsU16 mode)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    return type == 2U && player == -1 && mode == 0x12U && trace->active_file_sizes[0] >= 18U;
}

static void turn_host_mark_write(void *opaque, StarsU16 type, const StarsU8 *body, StarsU16 size)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    if (type != 8U || size != 16U) { ++trace->failures; return; }
    put_u16(trace->active_files[0], 16U, get_u16(body, 0U));
}

static void turn_mark_backup(void *opaque, StarsGenerateTurnState *state)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsU16 player;
    StarsMarkFileInput input;
    StarsMarkFileState marked;
    StarsMarkFileOps ops;
    (void)state;
    for (player = 0U; player < 3U; ++player) {
        trace->active_file_sizes[player] = input_file_sizes[trace->seeded_action][player];
        memcpy(trace->active_files[player], input_file_bytes[trace->seeded_action][player],
            trace->active_file_sizes[player]);
    }
    memset(&input, 0, sizeof input); memset(&ops, 0, sizeof ops);
    input.type = 2U; input.player = -1; input.mark_mode = 1U; input.value = 0;
    input.game_lid = get_u32(trace->complete_input.game, 0U);
    input.header_type = (StarsU16)(get_u16(trace->active_files[0], 0U) >> 10);
    input.header_version = get_u16(trace->active_files[0], 10U);
    input.header_game_lid = get_u32(trace->active_files[0], 6U);
    input.header_flags = get_u16(trace->active_files[0], 16U);
    ops.open_file = turn_host_mark_open; ops.write_record = turn_host_mark_write;
    if (!stars_mark_file(&input, &marked, &ops, trace) ||
            (get_u16(trace->active_files[0], 16U) & 0x0200U)) ++trace->failures;
    memcpy(trace->append_input_header, input_m1_headers[trace->seeded_action], 18U);
    ++trace->turn_mark_backup_calls;
}

static void turn_refresh_buildability(TurinTrace *trace);
static void turn_refresh_starbase_cloak(TurinTrace *trace);

static void turn_prepare_output_files(void *opaque, StarsGenerateTurnState *state)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    if (!state || state->turn != 1 || trace->turn_score_updates == 0U ||
            trace->turn_mark_backup_calls != 1 || trace->turn_write_data_calls != 0) {
        ++trace->failures; return;
    }
    ++trace->turn_prepare_output_calls;
    turn_refresh_starbase_cloak(trace);
    turn_refresh_buildability(trace);
    {
        StarsU16 player, index, size;
        for (player = 0U; player < 2U; ++player) {
            state->player_file_append[player] = (StarsU8)!trace->runtime_log_present[player];
            if (trace->runtime_log_present[player]) {
                if (player != 1U || !trace->generated_log_size) { ++trace->failures; return; }
                trace->backup_log_size = trace->generated_log_size;
                memcpy(trace->backup_log, trace->generated_log_file, trace->backup_log_size);
                trace->generated_log_size = 0U;
                memset(trace->generated_log_file, 0, sizeof trace->generated_log_file);
                trace->runtime_log_present[player] = 0U;
                ++trace->backup_moves;
            }
            index = (StarsU16)(player + 1U); size = trace->active_file_sizes[index];
            memcpy(trace->backup_files[index], trace->active_files[index], size);
            trace->backup_file_sizes[index] = size;
            if (state->player_file_append[player]) ++trace->backup_copies;
            else {
                trace->active_file_sizes[index] = 0U;
                memset(trace->active_files[index], 0, sizeof trace->active_files[index]);
                ++trace->backup_moves;
            }
        }
        size = trace->active_file_sizes[0];
        memcpy(trace->backup_files[0], trace->active_files[0], size);
        trace->backup_file_sizes[0] = size; trace->active_file_sizes[0] = 0U;
        memset(trace->active_files[0], 0, sizeof trace->active_files[0]);
        ++trace->backup_moves;
        for (index = 0U; index < 3U; ++index) {
            size = input_file_sizes[trace->seeded_action][index];
            if (trace->backup_file_sizes[index] != size ||
                    memcmp(trace->backup_files[index], input_file_bytes[trace->seeded_action][index], 16U) ||
                    memcmp(trace->backup_files[index] + 18U,
                        input_file_bytes[trace->seeded_action][index] + 18U, size - 18U))
                ++trace->failures;
            if (get_u16(trace->backup_files[index], 16U) !=
                    (StarsU16)(get_u16(input_file_bytes[trace->seeded_action][index], 16U) &
                    (index == 0U ? 0xfdffU : 0xffffU))) ++trace->failures;
        }
    }
    trace->saved_flag = turn_main_random(trace, 8);
    if (trace->saved_flag != (trace->seeded_action ? 4 : 1)) {
        fprintf(stderr, "saved flag=%d\n", trace->saved_flag);
        ++trace->failures;
    }
    put_u16(trace->rng_file_data, 0x0080U,
        (StarsU16)((StarsU16)trace->saved_flag << 9));
    put_u16(trace->complete_input.game, 16U,
        (StarsU16)((get_u16(trace->complete_input.game, 16U) & 0xf1ffU) |
        ((StarsU16)trace->saved_flag << 9)));
}

static void turn_mark_message_senders(void *opaque, StarsI16 player)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsU16 details[2], i;
    /* No outgoing player-message records occur in the pinned input/logs. */
    if (trace->turn_outgoing_count != 0U) { ++trace->failures; return; }
    for (i = 0U; i < 2U; ++i)
        details[i] = get_u16(trace->complete_input.views[0].players[i], 6U);
    stars_mark_players_that_sent_messages(player, trace->turn_outgoing_messages,
        trace->turn_outgoing_count, details, 2U);
    for (i = 0U; i < 2U; ++i)
        put_u16(trace->complete_input.views[0].players[i], 6U, details[i]);
    ++trace->turn_mark_message_calls;
}

static void *turn_unexpected_lost_planet(void *opaque, StarsU16 id)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    (void)id; ++trace->failures; return 0;
}

static void turn_unexpected_loss_mark(void *opaque, void *planet,
    StarsI16 player, StarsI16 detail)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    (void)planet; (void)player; (void)detail; ++trace->failures;
}

static void turn_mark_message_planet_losses(void *opaque, StarsI16 player)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsPlanetLossOps ops;
    /* Current emitted messages contain no loss IDs. A newly eligible loss
     * must fail until the per-view planet marking adapter is implemented. */
    ops.lookup = turn_unexpected_lost_planet; ops.mark = turn_unexpected_loss_mark;
    stars_mark_planets_player_lost(trace->turn_message_buffer,
        trace->turn_message_bytes, player, &ops, trace);
    ++trace->turn_mark_loss_calls;
}

static void turn_stream_record(TurinTrace *, StarsU16, StarsU16,
    const StarsU8 *);

static void turn_capture_messages(void *opaque, StarsU16 type,
    StarsU16 length, const void *record)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsI16 player = trace->turn_message_write_player;
    if (player < 0 || player > 1 || type != 12U ||
            (StarsU32)trace->turn_written_message_bytes[player] + length > 1024UL) {
        ++trace->failures; return;
    }
    memcpy(trace->turn_written_messages[player] + trace->turn_written_message_bytes[player], record, length);
    trace->turn_written_message_bytes[player] = (StarsU16)(trace->turn_written_message_bytes[player] + length);
    turn_stream_record(trace, type, length, (const StarsU8 *)record);
}

static void turn_data_file_messages(void *opaque, StarsI16 player)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    /* No X1 is supplied or generated: original FGenerateTurn appends M1,
     * preserving its existing records. AI generates X2, so M2 is replaced. */
    if (player == 0 && trace->turn_data_file_append_calls == 1U) {
        trace->turn_written_message_bytes[0] = input_m1_message_sizes[trace->seeded_action];
        memcpy(trace->turn_written_messages[0], input_m1_messages[trace->seeded_action],
            trace->turn_written_message_bytes[0]);
    }
    trace->turn_message_write_player = player;
    stars_write_player_messages(trace->turn_message_buffer, trace->turn_message_bytes,
        player, 0, 0U, turn_capture_messages, trace);
    if (player >= 0 && player < 2 &&
            (trace->turn_written_message_bytes[player] != expected_turn_message_sizes[trace->seeded_action][player] ||
            memcmp(trace->turn_written_messages[player], expected_turn_messages[trace->seeded_action][player],
                expected_turn_message_sizes[trace->seeded_action][player]) != 0)) {
        fprintf(stderr, "message mismatch player=%d bytes=%u expected=%u\n", player,
            trace->turn_written_message_bytes[player], expected_turn_message_sizes[trace->seeded_action][player]);
        ++trace->failures;
    }
    ++trace->turn_write_message_calls;
}

static void turn_capture_planet_record(void *opaque, StarsU16 type,
    StarsU16 length, const StarsU8 *record)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsI16 index = trace->turn_planet_write_index;
    if (index < 0 || index >= (StarsI16)TURIN_RETAINED_ALL_PLANET_COUNT ||
            length > 128U) {
        ++trace->failures;
        return;
    }
    trace->turn_planet_record_types[index] = type;
    trace->turn_planet_record_lengths[index] = length;
    memcpy(trace->turn_planet_records[index], record, length);
    turn_stream_record(trace, type, length, record);
    ++trace->turn_planet_write_calls;
}

static void turn_capture_production_queue(void *opaque, StarsU16 type,
    StarsU16 length, const StarsU8 *record)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsI16 file_index = trace->output_bof_index;
    if ((file_index != 1 && file_index != 2) || type != 0x1cU ||
            length != 8U || memcmp(record,
                retained_planet24_production_queue, length) != 0) {
        fprintf(stderr, "production queue file=%d type=%u len=%u\n",
            file_index, (unsigned)type, (unsigned)length);
        ++trace->failures;
    }
    if (file_index >= 0 && file_index < 3)
        ++trace->turn_production_queue_write_counts[file_index];
    turn_stream_record(trace, type, length, record);
}

static void turn_write_planet24_production_queue(TurinTrace *trace)
{
    StarsU8 record[128];
    StarsU16 length;
    if (!stars_write_production_queue_record(trace->turn_committed_queue,
            trace->turn_committed_queue_count, record, (StarsU16)sizeof record, &length,
            turn_capture_production_queue, trace) || length != 8U)
        ++trace->failures;
}

static void turn_write_hst_planets(TurinTrace *trace)
{
    const StarsU8 (*expected)[37] = trace->seeded_action ?
        retained_all_planet_output_bodies_action :
        retained_all_planet_output_bodies_control;
    const StarsU16 *expected_lengths = trace->seeded_action ?
        retained_all_planet_output_lengths_action :
        retained_all_planet_output_lengths_control;
    const StarsU32 (*expected_minerals)[4] = trace->seeded_action ?
        retained_turn_output_planet_minerals_action :
        retained_turn_output_planet_minerals_control;
    StarsU8 record[128];
    StarsU16 length;
    StarsU16 index;
    StarsU16 mineral;
    StarsU16 planet_id;
    StarsI16 state_index;
    StarsI16 levels_differ;
    StarsReadPlanetState *planet_state;
    trace->turn_planet_write_calls = 0;
    trace->turn_planet_level_divergences = 0;
    trace->turn_planet_stock_divergences = 0;
    for (index = 0U; index < TURIN_RETAINED_ALL_PLANET_COUNT; ++index) {
        trace->turn_planet_write_index = (StarsI16)index;
        planet_state = &trace->turn_planet_states[index];
        planet_id = (StarsU16)planet_state->id;
        if (planet_id >= TURIN_RETAINED_ALL_PLANET_COUNT ||
                planet_id != retained_all_planet_ids[index]) {
            ++trace->failures;
            continue;
        }
        state_index = planet_id == 10U ? 0 : planet_id == 24U ? 1 : -1;
        planet_state->flags = (StarsU16)(planet_state->flags & 0xf7ffU);
        if (!stars_write_planet(planet_state, 0x0dU,
                0U, record, (StarsU16)sizeof record, &length,
                turn_capture_planet_record, trace)) {
            ++trace->failures;
            continue;
        }
        if (trace->turn_planet_record_types[index] != 0x0dU ||
                trace->turn_planet_record_lengths[index] != expected_lengths[index] ||
                length != expected_lengths[index] ||
                memcmp(trace->turn_planet_records[index], expected[index],
                    expected_lengths[index]) != 0) {
            fprintf(stderr, "planet writer id=%u index=%u got=%u expected=%u\n",
                (unsigned)planet_id, (unsigned)index,
                (unsigned)length, (unsigned)expected_lengths[index]);
            ++trace->failures;
        }
        if (planet_id == 24U)
            turn_write_planet24_production_queue(trace);
        if (state_index < 0) continue;
        levels_differ = 0;
        for (mineral = 0U; mineral < 3U; ++mineral)
            if (planet_state->mineral_levels[mineral] !=
                    retained_turn_output_planet_levels[(StarsU16)state_index][mineral])
                levels_differ = 1;
        if (levels_differ) ++trace->turn_planet_level_divergences;
        for (mineral = 0U; mineral < 4U; ++mineral) {
            if (planet_state->minerals[mineral] !=
                    expected_minerals[(StarsU16)state_index][mineral])
                ++trace->turn_planet_stock_divergences;
            if (get_u16(trace->turn_planet_records[index],
                    (StarsU16)(17U + mineral * 2U)) !=
                    (StarsU16)planet_state->minerals[mineral])
                ++trace->failures;
        }
    }
}

static void turn_capture_player_planet(void *opaque, StarsU16 type,
    StarsU16 length, const StarsU8 *record)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsI16 file_index = trace->output_bof_index;
    StarsI16 index = trace->turn_player_planet_write_index;
    const StarsU8 (*expected_bodies)[37];
    const StarsU16 *expected_types;
    const StarsU16 *expected_lengths;
    StarsU16 expected_count;
    if (file_index == 0) {
        expected_bodies = trace->seeded_action ?
            retained_m1_planet_output_bodies_action :
            retained_m1_planet_output_bodies_control;
        expected_types = trace->seeded_action ?
            retained_m1_planet_output_types_action :
            retained_m1_planet_output_types_control;
        expected_lengths = trace->seeded_action ?
            retained_m1_planet_output_lengths_action :
            retained_m1_planet_output_lengths_control;
        expected_count = TURIN_M1_OUTPUT_PLANET_COUNT;
    } else if (file_index == 1) {
        expected_bodies = trace->seeded_action ?
            retained_m2_planet_output_bodies_action :
            retained_m2_planet_output_bodies_control;
        expected_types = trace->seeded_action ?
            retained_m2_planet_output_types_action :
            retained_m2_planet_output_types_control;
        expected_lengths = trace->seeded_action ?
            retained_m2_planet_output_lengths_action :
            retained_m2_planet_output_lengths_control;
        expected_count = TURIN_M2_OUTPUT_PLANET_COUNT;
    } else {
        ++trace->failures;
        return;
    }
    if (index < 0 || index >= (StarsI16)expected_count || length > 37U ||
            type != expected_types[index] ||
            length != expected_lengths[index] ||
            memcmp(record, expected_bodies[index], length) != 0) {
        fprintf(stderr, "player planet output file=%d index=%d type=%u len=%u\n",
            file_index, index, (unsigned)type, (unsigned)length);
        ++trace->failures;
    }
    turn_stream_record(trace, type, length, record);
    ++trace->turn_player_planet_write_counts[file_index];
}

static void turn_write_client_planets(TurinTrace *trace, StarsI16 viewer)
{
    StarsReadPlanetState state;
    StarsU8 record[128];
    StarsU16 length, index, type, count = 0U;
    for (index = 0U; index < trace->complete_input.views[0].planet_count; ++index) {
        state = trace->turn_client_visibility_planets[viewer][index];
        if (!(state.flags & 0x0100U)) continue;
        trace->turn_player_planet_write_index = (StarsI16)count++;
        type = (state.flags & 255U) == 7U ? 0x0dU : 0x0eU;
        /* FWriteDataFile uses a temporary detail-3 view for detail 2. */
        if ((state.flags & 255U) == 2U)
            state.flags = (StarsU16)((state.flags & 0xfd00U) | 3U);
        if (!stars_write_planet(&state, type, 0U, record, (StarsU16)sizeof record,
                &length, turn_capture_player_planet, trace)) ++trace->failures;
        if (type == 0x0dU && state.id == 24) turn_write_planet24_production_queue(trace);
    }
    if (count != (viewer == 0 ? TURIN_M1_OUTPUT_PLANET_COUNT : TURIN_M2_OUTPUT_PLANET_COUNT))
        ++trace->failures;
}

static void turn_write_m1_planets(TurinTrace *trace) { turn_write_client_planets(trace, 0); }
static void turn_write_m2_planets(TurinTrace *trace) { turn_write_client_planets(trace, 1); }

typedef struct TurnVisibilityContext {
    TurinTrace *trace;
    StarsReadPlanetState *planet_states;
    StarsVisPFPlayer *players;
    StarsVisPFPlanet *planets;
} TurnVisibilityContext;

static void turn_mark_visible_starbase(void *opaque, StarsI16 owner,
    StarsU16 slot, StarsU16 fleet_word)
{
    TurnVisibilityContext *context = (TurnVisibilityContext *)opaque;
    StarsU16 *flags;
    (void)fleet_word;
    if (owner < 0 || owner >= 2 || slot >= 10U) { ++context->trace->failures; return; }
    flags = &context->players[owner].starbase_flags[slot];
    if (!(*flags & 0x0100U)) {
        *flags = (StarsU16)((*flags & 0xfe00U) | 0x0100U);
        context->players[owner].fleet_count = (StarsU16)((context->players[owner].fleet_count & 0x0fffU) |
            ((context->players[owner].fleet_count + 0x1000U) & 0xf000U));
    }
    if ((*flags & 255U) < 3U) *flags = (StarsU16)((*flags & 0xff00U) | 3U);
}

static void turn_mark_visible_planet(void *opaque, StarsU16 planet_id,
    StarsI16 viewer, StarsI16 detail)
{
    TurnVisibilityContext *context = (TurnVisibilityContext *)opaque;
    StarsReadPlanetState *planet;
    StarsU8 raw[56], players[384];
    StarsU16 owner;
    if (!context || !context->trace || !context->planet_states || !context->players || !context->planets ||
            planet_id >= TURIN_RETAINED_POINT_COUNT || viewer < 0 || viewer > 1 ||
            detail < 0 || detail > 7) {
        if (context && context->trace) ++context->trace->failures;
        return;
    }
    planet = &context->planet_states[planet_id];
    if (planet->player >= 0 && planet->player != viewer) {
        /* Enemy-starbase attenuation/visibility publication is not bound. */
        ++context->trace->failures; return;
    }
    pack_loaded_planet(raw, planet);
    put_u16(raw, 4U, context->planets[planet_id].flags);
    memset(players, 0, sizeof players);
    for (owner = 0U; owner < 2U; ++owner) {
        put_u16(players, (StarsU16)(owner * 192U + 2U), context->players[owner].planet_count);
        put_u16(players, (StarsU16)(owner * 192U + 4U), context->players[owner].fleet_count);
        put_u16(players, (StarsU16)(owner * 192U + 6U), context->players[owner].detail_flags);
    }
    stars_mark_planet(raw, players, 192U, viewer, (StarsU16)detail, turn_mark_visible_starbase, context);
    planet->flags = get_u16(raw, 4U);
    context->planets[planet_id].flags = planet->flags;
    for (owner = 0U; owner < 2U; ++owner) {
        context->players[owner].planet_count = get_u16(players, (StarsU16)(owner * 192U + 2U));
        context->players[owner].detail_flags = get_u16(players, (StarsU16)(owner * 192U + 6U));
    }
    if (viewer == 1 && planet_id == 26U) {
        context->trace->turn_visible_planet26 = *planet;
        ++context->trace->turn_visible_planet_mark_calls;
    }
}

static StarsI16 turn_current_design_scanner(TurinTrace *trace, StarsI16 owner,
    const StarsU8 *design, StarsI16 *planet_range, StarsI16 *detection, StarsI16 *steal)
{
    StarsScannerComponent parts[16];
    StarsU16 part, count, group, packed, hull;
    StarsI16 race = turn_score_resource_stat(trace, owner, 14);
    StarsI32 base_scan, base_plan, tech;
    const StarsU8 *record;
    count = design[122];
    if (count > 16U) { ++trace->failures; return -1; }
    memset(parts, 0, sizeof parts);
    for (part = 0U; part < count; ++part) {
        group = get_u16(design, (StarsU16)(58U + part * 4U));
        packed = get_u16(design, (StarsU16)(60U + part * 4U));
        parts[part].kind = group; parts[part].packed_count_type = packed;
        if (group == 0x0800U && (packed & 255U) == 15U && (packed >> 8) != 0U) {
            /* Bind the original tachyon table before using detector effects. */
            ++trace->failures; return -1;
        }
        if (group == 2U && (packed >> 8) != 0U) {
            record = scrap_part_record(group, (StarsU16)(packed & 255U));
            if (!record) { ++trace->failures; return -1; }
            parts[part].range = (StarsI16)get_u16(record, 52U);
            parts[part].abilities = (StarsI16)get_u16(record, 54U);
        }
    }
    base_scan = 0L; base_plan = 0L; hull = get_u16(design, 0U);
    if (race == 9 && (hull == 4U || hull == 5U || hull == 6U)) {
        if (get_u16(trace->complete_input.game, 16U) & 8U) {
            base_scan = 2560000L; base_plan = 160000L;
        } else {
            tech = trace->complete_input.views[0].players[owner][30U];
            /* Current helper's base powers are signed 32-bit. */
            if (tech <= 0L || tech > 10L) { ++trace->failures; return -1; }
            base_plan = tech * 10L; base_plan *= base_plan; base_plan *= base_plan;
            base_scan = base_plan * 16L;
        }
    }
    return stars_get_design_scanner_range(parts, count,
        base_scan, base_plan, (StarsI16)((get_u32(trace->complete_input.views[0].players[owner],
            78U) >> 10) & 1UL), 0, planet_range, detection,
        steal);
}

static StarsI16 turn_visibility_scanner_values(void *opaque, const StarsVisPFFleet *fleet,
    StarsU16 *steal, StarsI16 *planet_range)
{
    TurnVisibilityContext *context = (TurnVisibilityContext *)opaque;
    TurinTrace *trace = context->trace;
    StarsScannerDesign designs[16];
    StarsU16 slot;
    StarsI16 steal_flags = 0, range;
    const StarsU8 *bank;
    if (!fleet || fleet->player < 0 || fleet->player > 1 || !steal) {
        ++trace->failures; return -1;
    }
    bank = fleet->player == 1 ? trace->designs :
        trace->complete_input.views[0].ship_designs[fleet->player];
    memset(designs, 0, sizeof designs);
    for (slot = 0U; slot < 16U; ++slot) {
        designs[slot].ship_count = fleet->ship_counts[slot];
        if (!designs[slot].ship_count) continue;
        designs[slot].scanner_range = turn_current_design_scanner(trace, fleet->player,
            bank + slot * 147U, &designs[slot].planet_range, 0, &designs[slot].steal_flags);
    }
    range = stars_get_fleet_scanner_range(designs, 16U, planet_range, 0, &steal_flags);
    *steal = (StarsU16)steal_flags;
    return range;
}

static StarsI16 turn_visibility_scanner(void *opaque, const StarsVisPFFleet *fleet, StarsU16 *steal)
{ return turn_visibility_scanner_values(opaque, fleet, steal, 0); }

static void turn_visibility_unexpected_fleet(void *opaque, StarsU16 id, StarsI16 detail)
{
    TurnVisibilityContext *context = (TurnVisibilityContext *)opaque;
    (void)id; (void)detail; ++context->trace->failures;
}

static StarsI16 turn_visibility_unexpected_cloak(void *opaque, const StarsVisFleetScanFleet *fleet)
{
    TurnVisibilityContext *context = (TurnVisibilityContext *)opaque;
    (void)fleet; ++context->trace->failures; return 0;
}

typedef struct TurnScannerLookup {
    TurinTrace *trace;
    StarsI16 owner;
} TurnScannerLookup;

static StarsI16 turn_scanner_race(void *opaque, StarsI16 owner, StarsI16 stat)
{
    TurnScannerLookup *lookup = (TurnScannerLookup *)opaque;
    return turn_score_resource_stat(lookup->trace, owner, stat);
}

static StarsI16 turn_scanner_trait(void *opaque, StarsI16 owner, StarsI16 bit)
{
    TurnScannerLookup *lookup = (TurnScannerLookup *)opaque;
    if (owner < 0 || owner > 1 || bit < 0 || bit > 31) {
        ++lookup->trace->failures; return 0;
    }
    return (StarsI16)((get_u32(lookup->trace->complete_input.views[0].players[owner],
        78U) >> bit) & 1UL);
}

static StarsI16 turn_scanner_dp(void *opaque, StarsU32 token)
{
    TurnScannerLookup *lookup = (TurnScannerLookup *)opaque;
    const StarsU8 *part = scrap_part_record((StarsU16)(token >> 16), (StarsU16)token);
    if (!part) { ++lookup->trace->failures; return 0; }
    return (StarsI16)get_u16(part, 52U);
}

static StarsI16 turn_scanner_tech(void *opaque, StarsU32 token)
{
    TurnScannerLookup *lookup = (TurnScannerLookup *)opaque;
    const StarsU8 *part = scrap_part_record((StarsU16)(token >> 16), (StarsU16)token);
    const StarsU8 *player = lookup->trace->complete_input.views[0].players[lookup->owner];
    StarsI16 current[6], required[6];
    StarsU16 i;
    if (!part) { ++lookup->trace->failures; return 0; }
    for (i = 0; i < 6U; ++i) { current[i] = player[26U+i]; required[i] = part[2U+i]; }
    return stars_tech_status(current, required, player[57]);
}

static StarsI16 turn_scanner_lookup(void *opaque, const StarsU8 *part)
{
    TurnScannerLookup *lookup = (TurnScannerLookup *)opaque;
    StarsLookupPartInput input;
    memset(&input, 0, sizeof input);
    input.group = get_u16(part, 0U); input.item = part[2]; input.player = lookup->owner;
    input.context = lookup; input.race_stat = turn_scanner_race;
    input.race_grbit = turn_scanner_trait; input.resolve = startup_part_token;
    input.tech_status = turn_scanner_tech; input.armor_dp = turn_scanner_dp;
    return stars_lookup_part(&input);
}

static StarsI16 turn_build_part(void *opaque, StarsU16 group, StarsU16 packed)
{
    StarsU8 part[4];
    put_u16(part, 0U, group); put_u16(part, 2U, packed);
    return turn_scanner_lookup(opaque, part);
}

static void turn_refresh_buildability(TurinTrace *trace)
{
    TurnScannerLookup lookup;
    StarsShieldComponent components[16];
    StarsU16 owner, slot, i, flags, count;
    StarsI16 scan, planet_scan = 0, detection = 100, steal = 0;
    lookup.trace = trace;
    for (owner = 0U; owner < 2U; ++owner) {
        lookup.owner = (StarsI16)owner;
        for (slot = 0U; slot < 16U; ++slot) {
            StarsU8 *design = owner == 1U ? trace->designs + slot * 147U :
                trace->complete_input.views[0].ship_designs[owner] + slot * 147U;
            flags = get_u16(design, 123U);
            if (flags & 0x0200U) continue;
            scan = turn_current_design_scanner(trace, (StarsI16)owner, design,
                &planet_scan, &detection, &steal);
            put_u16(design, 141U, (StarsU16)scan);
            put_u16(design, 143U, (StarsU16)planet_scan);
            design[145] = (StarsU8)detection; design[146] = (StarsU8)steal;
            ++trace->turn_scanner_refreshes;
            count = design[122];
            if (count > 16U) { ++trace->failures; return; }
            memset(components, 0, sizeof components);
            for (i = 0U; i < count; ++i) {
                components[i].kind = get_u16(design, (StarsU16)(58U + i * 4U));
                components[i].packed_count_type = get_u16(design, (StarsU16)(60U + i * 4U));
            }
            if (!stars_can_build_design((StarsI16)get_u16(design, 0U), components,
                    count, turn_build_part, &lookup))
                put_u16(design, 123U, (StarsU16)(flags | 0x8000U));
            if (owner == 1U)
                memcpy(trace->complete_input.views[0].ship_designs[owner] + slot * 147U, design, 147U);
        }
    }
}

static StarsI16 turn_cloak_points(void *opaque, StarsU16 group, StarsU16 item)
{
    return turn_scanner_dp(opaque, ((StarsU32)group << 16) | item);
}

static void turn_refresh_starbase_cloak(TurinTrace *trace)
{
    TurnScannerLookup lookup;
    StarsPctCloakOps ops;
    StarsCloakSection sections[16];
    StarsU16 owner, slot, i, count;
    StarsI32 visible;
    memset(&ops, 0, sizeof ops);
    ops.get_race_grbit = turn_scanner_trait;
    ops.get_player_stat = turn_scanner_race;
    ops.get_part_points = turn_cloak_points;
    lookup.trace = trace;
    for (owner = 0U; owner < 2U; ++owner) {
        lookup.owner = (StarsI16)owner;
        for (slot = 0U; slot < 10U; ++slot) {
            StarsU8 *design = owner == 1U ? trace->starbase_designs + slot * 147U :
                trace->complete_input.views[0].starbase_designs[owner] + slot * 147U;
            if (get_u16(design, 123U) & 0x0200U) continue;
            count = design[122];
            if (count > 16U) { ++trace->failures; return; }
            memset(sections, 0, sizeof sections);
            for (i = 0U; i < count; ++i) {
                sections[i].group = get_u16(design, (StarsU16)(58U + i * 4U));
                sections[i].flags = get_u16(design, (StarsU16)(60U + i * 4U));
            }
            visible = 100L - stars_pct_cloak_from_huldef(design, (StarsI16)owner,
                0, sections, count, &ops, &lookup);
            put_u32(design, 135U, (StarsU32)(visible * visible));
            if (owner == 1U)
                memcpy(trace->complete_input.views[0].starbase_designs[owner] + slot * 147U, design, 147U);
            ++trace->turn_cloak_refreshes;
        }
    }
}

static StarsI16 turn_planet_visibility_scanner(void *opaque,
    const StarsVisPlanetScanPlanet *planet, StarsI16 *deep)
{
    TurnVisibilityContext *context = (TurnVisibilityContext *)opaque;
    const StarsReadPlanetState *state = &context->planet_states[planet->id];
    TurnScannerLookup lookup;
    StarsU8 part[4];
    StarsI16 race, trait, dp = 0, blocked, deep_enabled = 0;
    lookup.trace = context->trace; lookup.owner = planet->owner;
    if (lookup.owner < 0 || lookup.owner > 1) { ++lookup.trace->failures; *deep = 0; return 0; }
    race = turn_scanner_race(&lookup, lookup.owner, 14);
    trait = turn_scanner_trait(&lookup, lookup.owner, 10);
    blocked = (StarsI16)(((state->opaque_20[1] >> 12) & 31UL) == 31UL);
    if (race == 8 && (planet->flags & 0x0200U)) {
        const StarsU8 *bank = lookup.owner == 1 ? lookup.trace->starbase_designs :
            lookup.trace->complete_input.views[0].starbase_designs[lookup.owner];
        if (planet->starbase_slot >= 10U) { ++lookup.trace->failures; *deep = 0; return 0; }
        deep_enabled = (StarsI16)(get_u16(bank + planet->starbase_slot * 147U, 0U) > 34U);
    }
    if (race != 8 && !blocked) {
        memset(part, 0, sizeof part);
        if (stars_lookup_best_planetary_scanner(part, turn_scanner_lookup, &lookup) != 1) {
            ++lookup.trace->failures; *deep = 0; return 0;
        }
        dp = turn_scanner_dp(&lookup, ((StarsU32)get_u16(part, 0U) << 16) | part[2]);
    }
    return stars_get_planet_scanner_range(race, trait, (StarsI32)state->minerals[3],
        dp, blocked, deep_enabled, deep);
}

static void turn_scan_visibility_fleets(TurnVisibilityContext *context,
    StarsVisPFFleet *current, StarsI16 viewer)
{
    TurinTrace *trace = context->trace;
    const Soe3InputView *world = &trace->complete_input.views[0];
    StarsVisFleetScanFleet fleets[10];
    StarsVisThingPlanet planets[32];
    StarsVisThingTarget things[4];
    StarsVisPlanetScanPlanet scan_planets[32];
    StarsVisThingFleet thing_fleets[10];
    StarsSetVisPFPlanetsInput planet_input;
    StarsSetVisPFPlanetsOps planet_ops;
    StarsSetVisPFThingsInput thing_input;
    StarsSetVisPFFleetsInput input;
    StarsSetVisPFFleetsOps ops;
    StarsU16 details[2], i;
    memset(fleets, 0, sizeof fleets); memset(planets, 0, sizeof planets);
    memset(things, 0, sizeof things); memset(&input, 0, sizeof input); memset(&ops, 0, sizeof ops);
    if (world->fleet_count > 10U || world->planet_count > 32U || world->thing_count > 4U) {
        ++trace->failures; return;
    }
    for (i = 0U; i < world->fleet_count; ++i) {
        const Soe3InputFleet *source = &world->fleets[i];
        fleets[i].id = source->id_word; fleets[i].player = source->owner;
        fleets[i].x = source->state.x; fleets[i].y = source->state.y;
        fleets[i].planet_id = current[i].planet_id; fleets[i].flags = current[i].flags;
        fleets[i].detection_percent = 100;
        if (source->owner == viewer && !(current[i].flags & 0x0400U))
            fleets[i].scanner_range = (StarsU16)turn_visibility_scanner_values(context, &current[i],
                &fleets[i].scanner_bits, &fleets[i].planet_range);
    }
    for (i = 0U; i < world->planet_count; ++i) {
        planets[i].id = i; planets[i].x = trace->complete_input.coordinates[i][0];
        planets[i].y = trace->complete_input.coordinates[i][1];
        planets[i].owner = context->planets[i].player; planets[i].flags = context->planets[i].flags;
    }
    for (i = 0U; i < world->thing_count; ++i) {
        const StarsU8 *raw = world->things[i];
        things[i].kind = (StarsU16)(get_u16(raw, 0U) >> 13);
        if (things[i].kind != 2U) { ++trace->failures; return; }
        things[i].owner = (StarsI16)((get_u16(raw, 0U) >> 9) & 15U);
        things[i].x = (StarsI16)get_u16(raw, 2U); things[i].y = (StarsI16)get_u16(raw, 4U);
        things[i].dest_x = trace->turn_fleet_visibility_things[viewer][i].dest_x;
        things[i].bits_2 = get_u16(raw, 8U);
    }
    for (i = 0U; i < 2U; ++i) details[i] = context->players[i].detail_flags;
    input.viewer = viewer; input.fleets = fleets; input.fleet_count = world->fleet_count;
    input.planets = planets; input.planet_count = world->planet_count;
    input.things = things; input.thing_count = world->thing_count;
    input.player_detail_flags = details; input.player_count = 2U;
    ops.mark_planet = turn_mark_visible_planet;
    ops.mark_fleet = turn_visibility_unexpected_fleet; ops.cloak_percent = turn_visibility_unexpected_cloak;
    stars_set_vis_pf_fleets(&input, &ops, context);
    /* SetVisiblePlanFleet: fleet scan, planet scan, thing scan, finish. */
    memset(scan_planets, 0, sizeof scan_planets);
    memset(&planet_input, 0, sizeof planet_input); memset(&planet_ops, 0, sizeof planet_ops);
    for (i = 0U; i < world->planet_count; ++i) {
        scan_planets[i].id = i; scan_planets[i].x = planets[i].x; scan_planets[i].y = planets[i].y;
        scan_planets[i].owner = context->planets[i].player;
        scan_planets[i].flags = context->planets[i].flags;
        scan_planets[i].starbase_slot = (StarsU16)(context->planet_states[i].opaque_44 & 15UL);
    }
    planet_input.viewer = viewer; planet_input.race_stat = turn_score_resource_stat(trace, viewer, 14);
    /* Stargate scanning is an additional race-7 service, absent here. */
    if (planet_input.race_stat == 7) { ++trace->failures; return; }
    planet_input.planets = scan_planets; planet_input.planet_count = world->planet_count;
    planet_input.fleets = fleets; planet_input.fleet_count = world->fleet_count;
    planet_input.things = things; planet_input.thing_count = world->thing_count;
    planet_input.player_detail_flags = details; planet_input.player_count = 2U;
    planet_ops.scanner = turn_planet_visibility_scanner;
    planet_ops.mark_planet = turn_mark_visible_planet;
    planet_ops.mark_fleet = turn_visibility_unexpected_fleet;
    planet_ops.cloak_percent = turn_visibility_unexpected_cloak;
    stars_set_vis_pf_planets(&planet_input, &planet_ops, context);
    ++trace->turn_planet_visibility_calls[viewer];
    memset(&thing_input, 0, sizeof thing_input); memset(thing_fleets, 0, sizeof thing_fleets);
    for (i = 0U; i < world->fleet_count; ++i) {
        thing_fleets[i].id = fleets[i].id; thing_fleets[i].x = fleets[i].x;
        thing_fleets[i].y = fleets[i].y; thing_fleets[i].flags = fleets[i].flags;
        thing_fleets[i].planet_id = fleets[i].planet_id;
    }
    for (i = 0U; i < world->planet_count; ++i) planets[i].flags = context->planets[i].flags;
    thing_input.viewer = viewer; thing_input.race_stat = planet_input.race_stat;
    /* Original races 5/6 alone enter thing-centered scanning. Fail if the
     * pinned input changes to require cloak/random/mark services here. */
    if (thing_input.race_stat == 5 || thing_input.race_stat == 6) { ++trace->failures; return; }
    thing_input.fleets = thing_fleets; thing_input.fleet_count = world->fleet_count;
    thing_input.planets = planets; thing_input.planet_count = world->planet_count;
    thing_input.things = things; thing_input.thing_count = world->thing_count;
    thing_input.player_detail_flags = details; thing_input.player_count = 2U;
    stars_set_vis_pf_things(&thing_input, 0, context);
    ++trace->turn_thing_visibility_calls[viewer];
    for (i = 0U; i < world->fleet_count; ++i)
        trace->turn_visibility_fleet_flags[viewer][i] = fleets[i].flags;
    memcpy(trace->turn_fleet_visibility_things[viewer], things, sizeof things);
    for (i = 0U; i < world->thing_count; ++i) {
        StarsU8 *raw = trace->complete_input.views[0].things[i];
        put_u16(raw, 6U, things[i].dest_x);
        put_u16(raw, 8U, (StarsU16)things[i].bits_2);
        memcpy(trace->turn_thing_bytes + i * 18U, raw, 18U);
    }
    ++trace->turn_fleet_visibility_calls[viewer];
    /* No pinned enemy enters scanner range. Other eligible visibility paths
     * must bind actual cloak/marking services before they can be accepted. */
    for (i = 0U; i < 2U; ++i)
        if (details[i] != context->players[i].detail_flags) ++trace->failures;
}

static StarsI32 turn_visibility_unexpected_mining(void *opaque, const StarsVisPFFleet *fleet)
{
    TurnVisibilityContext *context = (TurnVisibilityContext *)opaque;
    (void)fleet; ++context->trace->failures; return 0L;
}

static void turn_finish_visibility(TurinTrace *trace, StarsVisPFPlayer players[2], StarsI16 viewer)
{
    StarsVisibleFinishPlayer finish[2];
    StarsVisibleFinishInput input;
    StarsU16 owner, slot;
    memset(finish, 0, sizeof finish); memset(&input, 0, sizeof input);
    for (owner = 0U; owner < 2U; ++owner) {
        const StarsU8 *ships = owner == 1U ? trace->designs : trace->complete_input.views[0].ship_designs[owner];
        const StarsU8 *bases = owner == 1U ? trace->starbase_designs : trace->complete_input.views[0].starbase_designs[owner];
        finish[owner].shdef_count = players[owner].design_count;
        finish[owner].detail_flags = players[owner].detail_flags;
        finish[owner].fleet_count = players[owner].fleet_count;
        for (slot = 0U; slot < 16U; ++slot) {
            finish[owner].fleet_flags[slot] = players[owner].design_flags[slot];
            finish[owner].fleet_known[slot] = get_u16(ships + slot * 147U, 139U);
        }
        for (slot = 0U; slot < 10U; ++slot) {
            finish[owner].starbase_flags[slot] = players[owner].starbase_flags[slot];
            finish[owner].starbase_known[slot] = get_u16(bases + slot * 147U, 139U);
        }
    }
    input.viewer = viewer; input.race_stat = turn_score_resource_stat(trace, viewer, 14);
    input.players = finish; input.player_count = 2U;
    stars_set_visible_pf_finish(&input);
    for (owner = 0U; owner < 2U; ++owner) {
        players[owner].design_count = finish[owner].shdef_count;
        players[owner].detail_flags = finish[owner].detail_flags;
        players[owner].fleet_count = finish[owner].fleet_count;
        memcpy(players[owner].design_flags, finish[owner].fleet_flags, sizeof players[owner].design_flags);
        memcpy(players[owner].starbase_flags, finish[owner].starbase_flags, sizeof players[owner].starbase_flags);
    }
}

static void turn_publish_visibility_players(TurinTrace *trace, const StarsVisPFPlayer players[2])
{
    StarsU16 owner, slot;
    for (owner = 0U; owner < 2U; ++owner) {
        StarsU8 *raw = trace->complete_input.views[0].players[owner];
        StarsU8 *ships = trace->complete_input.views[0].ship_designs[owner];
        StarsU8 *bases = trace->complete_input.views[0].starbase_designs[owner];
        if (owner == 1U) {
            memcpy(ships, trace->designs, 16U * 147U);
            memcpy(bases, trace->starbase_designs, 10U * 147U);
        }
        raw[1] = (StarsU8)players[owner].design_count;
        put_u16(raw, 2U, players[owner].planet_count);
        put_u16(raw, 4U, players[owner].fleet_count);
        put_u16(raw, 6U, players[owner].detail_flags);
        for (slot = 0U; slot < 16U; ++slot) {
            put_u16(ships + slot * 147U, 123U, players[owner].design_flags[slot]);
            put_u32(ships + slot * 147U, 131U, players[owner].design_built[slot]);
            if (owner == 1U) {
                put_u16(trace->designs + slot * 147U, 123U, players[owner].design_flags[slot]);
                put_u32(trace->designs + slot * 147U, 131U, players[owner].design_built[slot]);
            }
        }
        for (slot = 0U; slot < 10U; ++slot) {
            put_u16(bases + slot * 147U, 123U, players[owner].starbase_flags[slot]);
            put_u32(bases + slot * 147U, 131U, players[owner].starbase_built[slot]);
            if (owner == 1U) {
                put_u16(trace->starbase_designs + slot * 147U, 123U, players[owner].starbase_flags[slot]);
                put_u32(trace->starbase_designs + slot * 147U, 131U, players[owner].starbase_built[slot]);
            }
        }
    }
}

static void turn_prepare_visibility_init(TurinTrace *trace, StarsI16 viewer)
{
    StarsReadPlanetState planet_states[TURIN_RETAINED_POINT_COUNT];
    StarsVisPFPlayer players[2];
    StarsVisPFPlanet planets[TURIN_RETAINED_POINT_COUNT];
    StarsVisPFFleet fleets[10];
    StarsVisPFThing things[4];
    const Soe3InputView *world = &trace->complete_input.views[0];
    StarsVisPFInitInput input;
    StarsVisPFInitOps ops;
    TurnVisibilityContext context;
    StarsU16 index, owner, slot;
    if (viewer == 1 && trace->turn_visible_planet26_valid) {
        ++trace->failures;
        return;
    }
    if (viewer == 1 && (trace->turn_moving_fleets[0].id != 0 ||
            trace->turn_moving_fleets[0].owner != 1 ||
            trace->turn_moving_fleets[0].moving != 0 ||
            trace->turn_moving_fleets[0].target_id != 26 ||
            trace->turn_moving_fleets[0].x != retained_points[26].x ||
            trace->turn_moving_fleets[0].y != retained_points[26].y)) {
        ++trace->failures;
        return;
    }
    memcpy(planet_states, trace->turn_planet_states, sizeof planet_states);
    memset(planets, 0, sizeof planets);
    memset(players, 0, sizeof players);
    memset(fleets, 0, sizeof fleets);
    memset(&input, 0, sizeof input);
    memset(&ops, 0, sizeof ops);
    for (index = 0U; index < TURIN_RETAINED_POINT_COUNT; ++index) {
        if (planet_states[index].id != (StarsI16)index) {
            ++trace->failures;
            return;
        }
        planets[index].id = index;
        planets[index].player = planet_states[index].player;
        planets[index].flags = planet_states[index].flags;
        planets[index].starbase_slot =
            (StarsU16)(planet_states[index].opaque_44 & 0x0fUL);
    }
    for (owner = 0U; owner < 2U; ++owner) {
        const StarsU8 *ships = owner == 1U ? trace->designs : world->ship_designs[owner];
        const StarsU8 *bases = owner == 1U ? trace->starbase_designs : world->starbase_designs[owner];
        players[owner].dead = (StarsU16)(get_u16(world->players[owner], 84U) & 1U);
        players[owner].detail_flags = get_u16(world->players[owner], 6U);
        players[owner].fleet_count = get_u16(world->players[owner], 4U);
        for (slot = 0U; slot < 16U; ++slot) {
            players[owner].design_flags[slot] = get_u16(ships + slot * 147U, 123U);
            players[owner].design_built[slot] = get_u32(ships + slot * 147U, 131U);
        }
        for (slot = 0U; slot < 10U; ++slot) {
            players[owner].starbase_flags[slot] = get_u16(bases + slot * 147U, 123U);
            players[owner].starbase_built[slot] = get_u32(bases + slot * 147U, 131U);
        }
    }
    if (world->fleet_count > 10U) { ++trace->failures; return; }
    for (index = 0U; index < world->fleet_count; ++index) {
        const Soe3InputFleet *source = &world->fleets[index];
        StarsU16 id = (StarsU16)(source->id_word & 511U);
        fleets[index].player = source->owner;
        fleets[index].planet_id = source->state.planet;
        fleets[index].flags = source->state.fleet_flags;
        fleets[index].mine_special = (StarsU16)((source->state.fleet_flags & 0x2000U) != 0U &&
            source->state.order_count != 0U && (get_u16(source->state.orders, 6U) & 15U) == 3U);
        for (slot = 0U; slot < 16U; ++slot)
            fleets[index].ship_counts[slot] = source->state.ship_counts[slot];
        if (source->owner == 1) {
            if (id >= TURIN_RETAINED_FLEET_COUNT) { ++trace->failures; return; }
            fleets[index].flags = get_u16(trace->raw_fleets[id], 4U);
            fleets[index].mine_special = (StarsU16)((fleets[index].flags & 0x2000U) != 0U &&
                trace->plord[id][3] != 0U && (get_u16(trace->plord[id], 10U) & 15U) == 3U);
            if (!trace->raw_fleet_present[id]) fleets[index].flags |= 0x0400U;
            for (slot = 0U; slot < 16U; ++slot)
                fleets[index].ship_counts[slot] = get_u16(trace->raw_fleets[id],
                    (StarsU16)(12U + slot * 2U));
        }
    }
    memset(things, 0, sizeof things);
    if (world->thing_count > 4U) { ++trace->failures; return; }
    for (index = 0U; index < world->thing_count; ++index) {
        const StarsU8 *raw = world->things[index];
        things[index].kind = (StarsU16)(get_u16(raw, 0U) >> 13);
        if (things[index].kind != 2U) { ++trace->failures; return; }
        things[index].owner = (StarsI16)((get_u16(raw, 0U) >> 9) & 15U);
        things[index].dest_x = get_u16(raw, 6U);
    }
    input.things = things; input.thing_count = world->thing_count;
    input.viewer = viewer;
    input.race_stat = viewer == -1 ? 0 : turn_score_resource_stat(trace, viewer, 14);
    input.players = players;
    input.player_count = 2U;
    input.planets = planets;
    input.planet_count = TURIN_RETAINED_POINT_COUNT;
    input.fleets = fleets;
    input.fleet_count = world->fleet_count;
    ops.mark_planet = turn_mark_visible_planet;
    ops.scanner = turn_visibility_scanner;
    ops.mine_from_fleet = turn_visibility_unexpected_mining;
    context.trace = trace;
    context.planet_states = planet_states;
    context.players = players; context.planets = planets;
    stars_set_vis_pf_init(&input, &ops, &context);
    for (index = 0U; index < world->fleet_count; ++index)
        trace->turn_visibility_fleet_flags[viewer == -1 ? 2 : viewer][index] = fleets[index].flags;
    for (index = 0U; index < world->thing_count; ++index) {
        StarsU8 *raw = trace->complete_input.views[0].things[index];
        trace->turn_fleet_visibility_things[viewer == -1 ? 2 : viewer][index].dest_x = things[index].dest_x;
        put_u16(raw, 6U, things[index].dest_x);
        memcpy(trace->turn_thing_bytes + index * 18U, raw, 18U);
    }
    if (viewer == -1) {
        /* SetVisiblePlanFleet assigns the complete map count to player 0. */
        players[0].planet_count = input.planet_count;
        turn_publish_visibility_players(trace, players);
        memcpy(trace->turn_host_visibility_players, players, sizeof players);
        ++trace->turn_host_visibility_calls;
        return;
    }
    turn_scan_visibility_fleets(&context, fleets, viewer);
    for (index = 0U; index < input.planet_count; ++index)
        planet_states[index].flags = planets[index].flags;
    memcpy(trace->turn_client_visibility_planets[viewer], planet_states, sizeof planet_states);
    if (viewer == 0) {
        turn_finish_visibility(trace, players, viewer);
        turn_publish_visibility_players(trace, players);
        memcpy(trace->turn_player0_visibility_players, players, sizeof players);
        ++trace->turn_player0_visibility_calls;
        return;
    }
    if (trace->turn_visible_planet_mark_calls != 1 ||
            trace->turn_visible_planet26.id != 26 ||
            trace->turn_visible_planet26.flags != 0x0103U) {
        ++trace->failures;
        return;
    }
    turn_finish_visibility(trace, players, viewer);
    turn_publish_visibility_players(trace, players);
    memcpy(trace->turn_player1_visibility_players, players, sizeof players);
    trace->turn_player1_visibility_fleets = input.fleet_count;
    trace->turn_visible_planet26_valid = 1;
    ++trace->turn_player1_visibility_calls;
}

static void turn_data_file_set_visibility(void *opaque, StarsI16 player)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    ++trace->turn_data_file_visibility_calls;
    if (player >= -1 && player < 2) turn_prepare_visibility_init(trace, player);
}

static void turn_stream_transfer(void *opaque, const StarsU8 *bytes,
    StarsU16 count)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsU16 index = trace->output_file_index;
    StarsU16 size;
    if (!trace->output_file_open || index >= 3U ||
            (StarsU32)trace->active_file_sizes[index] + count >
                sizeof trace->active_files[index]) {
        ++trace->failures; return;
    }
    size = trace->active_file_sizes[index];
    memcpy(trace->active_files[index] + size, bytes, count);
    trace->active_file_sizes[index] = (StarsU16)(size + count);
}

static void turn_stream_record(TurinTrace *trace, StarsU16 type,
    StarsU16 size, const StarsU8 *body)
{
    if (type > 63U || size > 1023U || (!body && size)) {
        ++trace->failures; return;
    }
    stars_write_record(trace->rng_file_data, (StarsI16)type, size, body,
        turn_stream_transfer, trace);
}

static StarsI16 turn_data_file_bof(void *opaque, StarsU16 file_type,
    StarsI16 player, StarsI16 multi)
{
    static const StarsI16 players[3] = {0, 1, -1};
    static const StarsI16 file_types[3] = {3, 3, 2};
    const StarsU8 (*expected_records)[18];
    const StarsU32 *expected_ticks;
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsI16 index = trace->output_bof_index;
    StarsU16 expected_flags;
    StarsU16 stored_salt;
    StarsSeeds before = trace->seeds;
    if (index < 0 || index >= 3 || player != players[index]) {
        ++trace->failures;
        return 0;
    }
    if (file_type != (StarsU16)file_types[index]) {
        ++trace->failures;
        return 0;
    }
    expected_records = trace->seeded_action ?
        retained_output_bof_records_action : retained_output_bof_records_control;
    expected_ticks = trace->seeded_action ? retained_output_bof_ticks_action :
        retained_output_bof_ticks_control;
    expected_flags = get_u16(expected_records[index], 16U);
    memcpy(trace->rng_file_data + 0x0a1cU, retained_bof_marker, 4U);
    if (memcmp(trace->complete_input.game, retained_game_id, 4U) != 0)
        ++trace->failures;
    memcpy(trace->rng_file_data + 0x0070U, trace->complete_input.game, 4U);
    put_u16(trace->rng_file_data, 0x0082U, 1U);
    trace->output_bof_tick_counts[index] = expected_ticks[index];
    put_u32(trace->rng_file_data, 0x1370U, trace->seeds.first);
    put_u32(trace->rng_file_data, 0x1374U, trace->seeds.second);
    put_u16(trace->rng_file_data, 0x07caU, trace->game_flags);
    put_u16(trace->rng_file_data, 0x07ceU, trace->startup_log_flags);
    stars_write_bof(trace->rng_file_data, players[index], file_types[index],
        multi, retained_output_tick_count, capture_bof_write, trace);
    trace->seeds.first = get_u32(trace->rng_file_data, 0x1370U);
    trace->seeds.second = get_u32(trace->rng_file_data, 0x1374U);
    stored_salt = (StarsU16)(get_u16(
        trace->bof_write_records[2 + index], 14U) >> 5);
    trace->output_bof_random_values[index] = (StarsI16)(
        (stored_salt + 2048U -
            (StarsU16)(trace->output_bof_tick_counts[index] & 0x07ffUL)) &
        0x07ffU);
    turn_audit_main_draw(trace, before, 2000, trace->output_bof_random_values[index]);
    if (trace->bof_write_sizes[2 + index] != 18 ||
            (get_u16(trace->bof_write_records[2 + index], 16U) & 0x00ffU) !=
                (StarsU16)file_types[index] ||
            (get_u16(trace->bof_write_records[2 + index], 14U) & 31U) !=
                (StarsU16)(players[index] & 31) ||
            (expected_flags & 0x0b00U) != 0U ||
            ((expected_flags >> 13) & 7U) != (StarsU16)trace->saved_flag ||
            ((get_u16(trace->bof_write_records[2 + index], 16U) >> 13) & 7U)
                != (StarsU16)trace->saved_flag ||
            memcmp(trace->bof_write_records[2 + index],
                expected_records[index], 18U) != 0) {
        fprintf(stderr, "output BOF[%d] size=%d type=%u player=%u salt=%u\n",
            index, trace->bof_write_sizes[2 + index],
            (unsigned)get_u16(trace->bof_write_records[2 + index], 16U),
            (unsigned)(get_u16(trace->bof_write_records[2 + index], 14U) & 31U),
            (unsigned)(get_u16(trace->bof_write_records[2 + index], 14U) >> 5));
        ++trace->failures;
    }
    ++trace->bof_writer_calls;
    return 1;
}

static StarsI16 turn_data_file_create(void *opaque, StarsU16 type, StarsI16 player)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsU16 index = (StarsU16)(player + 1);
    if (player < -1 || player > 1 || trace->active_file_sizes[index] ||
            trace->output_file_open) {
        ++trace->failures; return 0;
    }
    ++trace->turn_data_file_create_calls;
    if (!turn_data_file_bof(opaque, type, player, 0)) return 0;
    memcpy(trace->active_files[index],
        trace->bof_write_records[2 + trace->output_bof_index], 18U);
    trace->active_file_sizes[index] = 18U;
    trace->output_file_prefix_sizes[index] = 0U;
    trace->output_file_index = index;
    trace->output_file_open = 1U;
    return 1;
}

static StarsI16 turn_append_open(void *opaque, StarsU16 type, StarsI16 player, StarsU16 mode)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    if (type != 3U || player != 0 || mode != 0x12U ||
            !trace->active_file_sizes[1] || trace->output_file_open) {
        ++trace->failures; return 0;
    }
    return 1;
}

static void turn_append_mark_record(void *opaque, StarsU16 type, const StarsU8 *body, StarsU16 size)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    if (type != 8U || size != 16U) { ++trace->failures; return; }
    put_u16(trace->append_input_header, 16U, get_u16(body, 0U));
    put_u16(trace->active_files[1], 16U, get_u16(body, 0U));
}

static StarsI16 turn_append_mark(void *opaque, StarsU16 type, StarsI16 player,
    StarsU16 mode, StarsI16 value)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsMarkFileInput input;
    StarsMarkFileState state;
    StarsMarkFileOps ops;
    memset(&input, 0, sizeof input); memset(&ops, 0, sizeof ops);
    input.type = type; input.player = player; input.mark_mode = mode; input.value = value;
    input.game_lid = get_u32(trace->complete_input.game, 0U);
    input.header_type = (StarsU16)(get_u16(trace->append_input_header, 0U) >> 10);
    input.header_version = get_u16(trace->append_input_header, 10U);
    input.header_game_lid = get_u32(trace->append_input_header, 6U);
    input.header_flags = get_u16(trace->append_input_header, 16U);
    ops.open_file = turn_append_open; ops.write_record = turn_append_mark_record;
    ++trace->turn_append_mark_calls;
    if (!stars_mark_file(&input, &state, &ops, trace) || !state.changed || !state.file_open) {
        ++trace->failures; return 0;
    }
    return 1;
}

static void turn_append_bof(void *opaque, StarsI16 player, StarsI16 type, StarsI16 multi)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsU16 size = trace->active_file_sizes[1];
    if (player != 0 || type != 3 || multi != 1 || trace->output_file_open ||
            size < 18U || (StarsU32)size + 18U > sizeof trace->active_files[1]) {
        ++trace->failures; return;
    }
    /* Same BOF service/RNG; append retains and marks the starting BOF. */
    if (!turn_data_file_bof(trace, (StarsU16)type, player, multi)) {
        ++trace->failures; return;
    }
    memcpy(trace->active_files[1] + size,
        trace->bof_write_records[2 + trace->output_bof_index], 18U);
    trace->output_file_prefix_sizes[1] = size;
    trace->active_file_sizes[1] = (StarsU16)(size + 18U);
    trace->output_file_index = 1U;
    trace->output_file_open = 1U;
}

static StarsI16 turn_data_file_append(void *opaque, StarsI16 player)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    ++trace->turn_data_file_append_calls;
    return stars_append_file(player, turn_append_mark, turn_append_bof, trace);
}

static void turn_data_file_planets(void *opaque, StarsI16 player)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    ++trace->turn_data_file_planet_calls;
    if (player == 0)
        turn_write_m1_planets(trace);
    else if (player == 1)
        turn_write_m2_planets(trace);
    else if (player == -1)
        turn_write_hst_planets(trace);
    else
        ++trace->failures;
}

static StarsI16 turn_score_engine(void *opaque, StarsU16 id,
    StarsSpeedEngine *engine)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    const StarsU8 *record = scrap_part_record(1U, id);
    StarsU16 warp;
    if (!record) { ++trace->failures; return 0; }
    engine->engine_id = id;
    for (warp = 0U; warp < 10U; ++warp)
        engine->fuel_used[warp] = get_u16(record, (StarsU16)(54U + 2U * warp));
    return 1;
}

static StarsI32 turn_score_design_power(TurinTrace *trace, const StarsU8 *raw)
{
    StarsPowerComponent components[16];
    StarsSpeedComponent speed_components[16];
    StarsSpeedDesign design;
    StarsSpeedInput input;
    StarsU16 index, count = raw[122];
    StarsI16 speed;
    if (count > 16U) { ++trace->failures; return 0L; }
    memset(components, 0, sizeof components);
    memset(speed_components, 0, sizeof speed_components);
    memset(&input, 0, sizeof input);
    for (index = 0U; index < count; ++index) {
        StarsU16 group = get_u16(raw, (StarsU16)(58U + 4U * index));
        StarsU16 item = raw[60U + 4U * index];
        StarsU16 amount = raw[61U + 4U * index];
        const StarsU8 *part;
        components[index].kind = group;
        components[index].packed_count_type = (StarsU16)(item | (amount << 8));
        speed_components[index].group = group;
        speed_components[index].type = item;
        speed_components[index].count = amount;
        if (!amount || (group != 0x10U && group != 0x20U &&
                group != 0x40U && !(group == 0x800U &&
                (item == 12U || item == 13U)))) continue;
        part = scrap_part_record(group, item);
        if (!part) { ++trace->failures; continue; }
        /* Original FLookupPart overlays the common HUL offsets on each
         * component type. These are weapon statistics, not hull capacities. */
        if (group == 0x10U || group == 0x20U) {
            components[index].fuel_max = get_u16(part, 54U);
            components[index].cargo_max = get_u16(part, 52U);
        }
        if (group == 0x10U) components[index].flags = get_u16(part, 58U);
        if (group == 0x40U) components[index].fuel_max = get_u16(part, 54U);
        if (group == 0x40U || group == 0x800U)
            components[index].damage_points = get_u16(part, 56U);
    }
    design.empty_weight = get_u16(raw, 40U);
    design.components = speed_components;
    design.component_count = count;
    input.design = &design;
    input.engine_lookup = turn_score_engine;
    input.context = trace;
    speed = stars_speed_of_ship(&input);
    return stars_compute_design_power(components, count, speed);
}

/* Derive current design power before current-world score calculation. */
static void turn_prepare_score_designs(void *opaque, StarsGenerateTurnState *state)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsU16 owner, slot;
    StarsSeeds before = trace->seeds;
    if (!state || state->player_count != 2) ++trace->failures;
    trace->turn_score_design_count = 0U;
    for (owner = 0U; owner < 2U; ++owner) {
        const StarsU8 *bank = owner == 1U ? trace->designs :
            trace->complete_input.views[0].ship_designs[0];
        for (slot = 0U; slot < 16U; ++slot) {
            const StarsU8 *raw = bank + slot * 147U;
            StarsScoreDesign *design = &trace->turn_score_designs[owner][slot];
            design->disabled = (get_u16(raw, 123U) & 0x0200U) != 0U;
            design->power = design->disabled ? 0L : turn_score_design_power(trace, raw);
            ++trace->turn_score_design_count;
        }
    }
    memset(trace->turn_score_summaries, 0, sizeof trace->turn_score_summaries);
    for (slot = 0U; slot < trace->complete_input.views[0].fleet_count; ++slot) {
        Soe3InputFleet *source = &trace->complete_input.views[0].fleets[slot];
        StarsU16 id = (StarsU16)(source->id_word & 0x01ffU);
        StarsU16 ship;
        if (!turn_current_moving_fleet(trace, source)) continue;
        owner = (StarsU16)source->owner;
        for (ship = 0U; ship < 16U; ++ship) {
            const StarsScoreDesign *design = &trace->turn_score_designs[owner][ship];
            StarsU16 count = owner == 1U ? get_u16(trace->raw_fleets[id],
                (StarsU16)(12U + ship * 2U)) : source->state.ship_counts[ship];
            StarsU16 category = design->power < 1L ? 0U :
                (design->power < 2000L ? 1U : 2U);
            if (!design->disabled)
                trace->turn_score_summaries[owner].fleet_counts[category] += count;
        }
    }
    /* The Turin fixture's score history is not the Rototill fixture's oracle. */
    if (!trace->rototill_seeded_mode)
        for (owner = 0U; owner < 2U; ++owner) {
            const StarsU8 *expected = trace->seeded_action ?
                retained_score_history_action[owner] : retained_score_history_control[owner];
            for (slot = 0U; slot < 3U; ++slot)
                if (stars_pack_long((StarsU32)trace->turn_score_summaries[owner].fleet_counts[slot]) !=
                        get_u16(expected, (StarsU16)(16U + slot * 2U)))
                    ++trace->failures;
        }
    if (before.first != trace->seeds.first || before.second != trace->seeds.second)
        ++trace->failures;
}

static StarsI16 turn_score_calculate(void *opaque, StarsI16 player,
    StarsUpdatePlayerScore *score)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsScorePlanet planets[32];
    StarsScoreFleet fleets[10];
    StarsCResourcesAtPlanetOps ops;
    Soe3InputView *host = &trace->complete_input.views[0];
    StarsU16 index, ship, count = 0U;
    StarsI16 tech[6], dead, extra;
    StarsScoreSummary *summary = &trace->turn_score_summaries[player];
    memset(planets, 0, sizeof planets); memset(fleets, 0, sizeof fleets);
    memset(&ops, 0, sizeof ops);
    ops.calc_max_population = turn_score_max_population;
    ops.factory_calc_max_population = turn_score_max_population;
    ops.planet_desirability = turn_score_desirability;
    ops.get_player_stat = turn_score_resource_stat;
    ops.factory_get_player_stat = turn_score_resource_stat;
    for (index = 0U; index < 32U; ++index) {
        const StarsReadPlanetState *source = &trace->turn_planet_states[index];
        StarsU8 raw[56];
        planets[index].owner = source->player;
        if (source->player != player) continue;
        planets[index].mineral_value = (StarsI32)source->minerals[3];
        pack_loaded_planet(raw, source);
        planets[index].resources = stars_cresources_at_planet(raw, player, &ops, trace);
        if (source->flags & 0x0200U) {
            StarsU8 hull[143];
            StarsU16 slot = (StarsU16)(source->opaque_44 & 15U);
            const StarsU8 *bank = player == 1 ? trace->starbase_designs :
                host->starbase_designs[0];
            if (slot >= 10U || !scrap_read_hull(trace,
                    get_u16(bank, (StarsU16)(147U * slot)), hull, sizeof hull))
                ++trace->failures;
            else planets[index].score_starbase = get_u16(hull, 52U) != 0U;
        }
    }
    for (index = 0U; index < host->fleet_count; ++index) {
        Soe3InputFleet *source = &host->fleets[index];
        StarsU16 id = (StarsU16)(source->id_word & 0x01ffU);
        StarsScoreFleet *fleet;
        if (!turn_current_moving_fleet(trace, source)) continue;
        fleet = &fleets[count++]; fleet->owner = source->owner;
        fleet->flags = source->state.fleet_flags;
        for (ship = 0U; ship < 16U; ++ship)
            fleet->component_counts[ship] = source->owner == 1 ?
                get_u16(trace->raw_fleets[id], (StarsU16)(12U + ship * 2U)) :
                source->state.ship_counts[ship];
    }
    for (index = 0U; index < 6U; ++index)
        tech[index] = (StarsI16)(signed char)host->players[player][26U + index];
    dead = (StarsI16)(get_u16(host->players[player], 84U) & 1U);
    stars_calc_player_score(player, planets, 32U, tech, dead,
        trace->turn_score_designs[player], 16U, fleets, count, 0, summary);
    extra = 0;
    if (summary->fleet_counts[2] > 0L)
        extra = (StarsI16)((summary->fleet_counts[2] * 8L * summary->planet_count) /
            (summary->fleet_counts[2] + summary->planet_count));
    if (extra) stars_calc_player_score(player, planets, 32U, tech, dead,
        trace->turn_score_designs[player], 16U, fleets, count, extra, summary);
    score->total = summary->score;
    score->planets = (StarsI16)summary->planet_count;
    score->resources = summary->resources;
    score->ship_value = summary->fleet_counts[2];
    score->tech_count = (StarsI16)summary->technology_levels;
    score->dead = dead;
    for (index = 0U; index < 6U; ++index) score->technology_levels[index] = tech[index];
    return 1;
}

static StarsI16 turn_score_victory_value(void *opaque, StarsI16 index)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsU8 data[0xb0];
    memset(data, 0, sizeof data);
    memcpy(data + 0x70U, trace->complete_input.game, 64U);
    return stars_get_victory_value(data, 0x70U, index, 0);
}
static StarsI16 turn_score_victory_check(void *opaque, StarsI16 index)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    return stars_get_victory_check(trace->complete_input.game, 0U, index);
}
static void turn_score_record(void *opaque, StarsI16 player,
    const StarsUpdatePlayerScore *score)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsU16 flags = (StarsU16)(player | 0x20U | score->achievement_flags);
    put_u16(trace->complete_input.views[0].players[player], 10U,
        (StarsU16)score->rank);
    stars_pack_score_history_record(&trace->turn_score_summaries[player], flags,
        (StarsU16)score->rank, trace->turn_score_records[player]);
    ++trace->turn_score_updates;
}
static void turn_score_unexpected_dead(void *opaque, StarsI16 player)
{ (void)player; ++((TurinTrace *)opaque)->failures; }
static void turn_score_unexpected_message(void *opaque, StarsI16 player,
    StarsI16 message, StarsU16 mask)
{ (void)player; (void)message; (void)mask; ++((TurinTrace *)opaque)->failures; }
static void turn_score_unexpected_dirty(void *opaque)
{ ++((TurinTrace *)opaque)->failures; }

static void turn_update_scores(void *opaque, StarsGenerateTurnState *state)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsUpdatePlayerScoreInput input;
    StarsUpdatePlayerScoreOps ops;
    StarsSeeds before = trace->seeds;
    turn_prepare_score_designs(trace, state);
    memset(&input, 0, sizeof input); memset(&ops, 0, sizeof ops);
    input.player_count = state->player_count; input.planet_count = 32;
    input.turn = state->turn; input.players = trace->turn_player_scores;
    ops.calculate = turn_score_calculate;
    ops.victory_value = turn_score_victory_value; ops.victory_check = turn_score_victory_check;
    ops.record = turn_score_record; ops.mark_dead = turn_score_unexpected_dead;
    ops.send_message = turn_score_unexpected_message; ops.mark_dirty = turn_score_unexpected_dirty;
    ops.context = trace; input.ops = &ops;
    stars_update_player_scores(&input);
    if (before.first != trace->seeds.first || before.second != trace->seeds.second)
        ++trace->failures;
}

static void turn_capture_score_record(void *opaque, StarsU16 type,
    StarsU16 length, const StarsU8 *record)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    const StarsU8 (*expected)[24] = trace->seeded_action ?
        retained_score_history_action : retained_score_history_control;
    StarsI16 file_index = trace->output_bof_index;
    if (file_index < 0 || file_index > 1 || type != 0x2dU ||
            length != 24U ||
            memcmp(record, expected[file_index], 24U) != 0) {
        ++trace->failures;
        return;
    }
    turn_stream_record(trace, type, length, record);
    ++trace->turn_score_record_counts[file_index];
}

static void turn_data_file_scores(void *opaque, StarsI16 player)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsWriteScoreRecordsInput input;
    memset(&input, 0, sizeof input);
    if (trace->turn_score_design_count != 32U) ++trace->failures;
    if (trace->turn_score_updates != 2U) ++trace->failures;
    if (player >= 0 && player < 2 &&
            get_u16(trace->complete_input.views[0].players[player], 10U) !=
            get_u16(trace->turn_score_records[player], 2U)) ++trace->failures;
    input.records = &trace->turn_score_records[0][0];
    input.player_count = 2U;
    input.file_player = player;
    input.game_flags = trace->game_flags;
    input.turn = 1U;
    if (!stars_write_score_records(&input, turn_capture_score_record, trace))
        ++trace->failures;
}

static void turn_capture_thing_record(void *opaque, StarsU16 type,
    StarsU16 length, const StarsU8 *record)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    const StarsU8 (*expected)[18] = trace->seeded_action ?
        retained_wormhole_output_action : retained_wormhole_output_control;
    StarsI16 file_index = trace->output_bof_index;
    StarsI16 record_index;
    if (file_index != 2 || type != 0x2bU) {
        ++trace->failures;
        return;
    }
    record_index = trace->turn_thing_record_counts[file_index];
    if (record_index == 0) {
        if (length != 2U || get_u16(record, 0U) !=
                TURIN_RETAINED_THING_COUNT)
            ++trace->failures;
    } else if (record_index <= (StarsI16)TURIN_RETAINED_THING_COUNT) {
        if (length != 18U || memcmp(record,
                expected[record_index - 1], 18U) != 0)
            ++trace->failures;
    } else {
        ++trace->failures;
    }
    turn_stream_record(trace, type, length, record);
    ++trace->turn_thing_record_counts[file_index];
}

static void turn_data_file_things(void *opaque, StarsI16 player)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    const Soe3InputView *world = &trace->complete_input.views[0];
    StarsThing things[TURIN_RETAINED_THING_COUNT];
    StarsU16 index, count = 0U;
    StarsU16 view = player == -1 ? 2U : (StarsU16)player;
    if (view > 2U || world->thing_count > TURIN_RETAINED_THING_COUNT) {
        ++trace->failures; return;
    }
    for (index = 0U; index < world->thing_count; ++index) {
        const StarsVisThingTarget *visible = &trace->turn_fleet_visibility_things[view][index];
        const StarsU8 *raw = world->things[index];
        /* All current things are wormholes. Original FWriteDataFile tests
         * their current visibility bit, even when owned by the viewer. */
        if ((get_u16(raw, 0U) >> 13) != 2U) { ++trace->failures; return; }
        if (player != -1 && !(visible->dest_x & 0x2000U)) continue;
        memcpy(things[count].raw, raw, 18U);
        put_u16(things[count].raw, 6U, visible->dest_x);
        if (player != -1) put_u16(things[count].raw, 8U, (StarsU16)visible->bits_2);
        ++count;
    }
    if (!stars_write_thing_records(things, count, turn_capture_thing_record, trace))
        ++trace->failures;
}

static void turn_capture_battle_plan(void *opaque, StarsU16 type, StarsU16 size,
    const StarsU8 *body)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsU16 file = (StarsU16)trace->output_bof_index;
    StarsU16 index = trace->turn_plan_write_counts[file]++;
    if (type != 30U || index >= expected_plan_counts[trace->seeded_action][file] ||
            size != expected_plan_sizes[trace->seeded_action][file][index] ||
            memcmp(body, expected_plan_bodies[trace->seeded_action][file][index], size) != 0) {
        fprintf(stderr, "battle plan file=%u index=%u size=%u differs\n", file, index, size);
        ++trace->failures;
    }
    turn_stream_record(trace, type, size, body);
}

static void turn_data_file_battle_plans(void *opaque, StarsI16 owner)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    const Soe3InputView *world = &trace->complete_input.views[0];
    StarsU16 slot;
    if (owner < 0 || owner > 1) { ++trace->failures; return; }
    for (slot = 0U; slot < 16U; ++slot)
        if (world->battle_plan_present[owner][slot])
            stars_write_battle_plan(world->battle_plans[owner][slot], original_dgroup, 0,
                turn_capture_battle_plan, 0, trace);
}

static void turn_data_file_turn(void *opaque, StarsU16 turn)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsI16 index = trace->output_bof_index;
    if (index < 0 || index >= 3 || turn != 1U) {
        ++trace->failures;
        return;
    }
    if (trace->turn_plan_write_counts[index] != expected_plan_counts[trace->seeded_action][index])
        ++trace->failures;
    if (trace->turn_design_write_counts[index] != expected_design_counts[trace->seeded_action][index])
        ++trace->failures;
    trace->turn_data_turn_values[index] = turn;
    put_u16(trace->turn_data_turn_marker_bodies[index], 0U, turn);
    turn_stream_record(trace, 0U, 2U,
        trace->turn_data_turn_marker_bodies[index]);
    ++trace->turn_data_turn_marker_calls;
    ++trace->output_bof_index;
}

static void turn_data_file_close(void *opaque)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsU16 index = trace->output_file_index;
    if (!trace->output_file_open || index >= 3U ||
            trace->active_file_sizes[index] <
                (StarsU16)(trace->output_file_prefix_sizes[index] + 18U) ||
            memcmp(trace->active_files[index] +
                trace->output_file_prefix_sizes[index],
                trace->bof_write_records[2 + trace->turn_data_file_close_calls],
                18U) != 0) {
        ++trace->failures; return;
    }
    trace->output_file_open = 0U;
    ++trace->output_file_closes[index];
    ++trace->turn_data_file_close_calls;
}

typedef struct TurnPlayerFleetWriteContext {
    TurinTrace *trace;
    StarsU16 file_index;
    StarsU16 index;
    StarsU16 fleet_id;
    StarsU16 id_word;
    StarsU16 owner;
    StarsI16 planet;
    StarsI16 x;
    StarsI16 y;
    StarsU16 expected_fleet_length;
    StarsU16 expected_order_length;
    const StarsU8 *expected_fleet_body;
    const StarsU8 *expected_order_body;
} TurnPlayerFleetWriteContext;

static void turn_capture_player_fleet_record(void *opaque, StarsU16 type,
    StarsU16 length, const StarsU8 *record)
{
    TurnPlayerFleetWriteContext *context =
        (TurnPlayerFleetWriteContext *)opaque;
    TurinTrace *trace = context->trace;
    StarsU8 body[128];
    StarsU16 body_length;
    if (type == 0x10U) {
        if (length < 4U || length > 118U) {
            ++trace->failures;
            return;
        }
        memset(body, 0, sizeof body);
        put_u16(body, 0U, context->id_word);
        put_u16(body, 2U, context->owner);
        put_u16(body, 4U, get_u16(record, 0U));
        put_u16(body, 6U, (StarsU16)context->planet);
        put_u16(body, 8U, (StarsU16)context->x);
        put_u16(body, 10U, (StarsU16)context->y);
        put_u16(body, 12U, get_u16(record, 2U));
        memcpy(body + 14U, record + 4U, (size_t)(length - 4U));
        body_length = (StarsU16)(length + 10U);
        if (body_length != context->expected_fleet_length ||
                memcmp(body, context->expected_fleet_body, body_length) != 0) {
            fprintf(stderr, "file %u fleet %u differs: len=%u expected=%u\n",
                (unsigned)context->file_index, (unsigned)context->fleet_id,
                (unsigned)body_length,
                (unsigned)context->expected_fleet_length);
            ++trace->failures;
        }
        turn_stream_record(trace, type, body_length, body);
        ++trace->turn_player_fleet_write_counts[context->file_index];
    } else if (type == 0x14U) {
        if (length != context->expected_order_length ||
                memcmp(record, context->expected_order_body, length) != 0) {
            fprintf(stderr, "file %u fleet %u order differs: len=%u\n",
                (unsigned)context->file_index, (unsigned)context->fleet_id,
                (unsigned)length);
            ++trace->failures;
        }
        turn_stream_record(trace, type, length, record);
        ++trace->turn_player_order_write_counts[context->file_index];
    } else {
        fprintf(stderr, "unexpected file %u fleet %u subrecord type=%u len=%u\n",
            (unsigned)context->file_index, (unsigned)context->fleet_id,
            (unsigned)type, (unsigned)length);
        ++trace->failures;
    }
}

static StarsU16 turn_hst_owner0_output_count(const TurinTrace *trace)
{
    return trace->seeded_action ? TURIN_HST_OWNER0_ACTION_FLEET_COUNT :
        TURIN_HST_OWNER0_CONTROL_FLEET_COUNT;
}

static StarsI16 turn_data_file_fleet_predicate(void *opaque, StarsU16 index)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsI16 file = trace->output_bof_index;
    if (file < 0 || file > 2 || index >= trace->complete_input.views[0].fleet_count) {
        ++trace->failures; return 0;
    }
    return (StarsI16)((trace->turn_visibility_fleet_flags[file][index] & 0x0100U) != 0U);
}

static void turn_data_file_write_fleet(void *opaque, StarsU16 index)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsWriteFleetInput input;
    StarsWriteFleetOps ops;
    TurnPlayerFleetWriteContext context;
    StarsU8 record[128];
    StarsU16 length;
    StarsU16 ship;
    StarsU16 cargo;
    StarsU16 status;
    StarsU16 flags;
    StarsU16 order_index;
    StarsU16 owner;
    StarsU16 source_index;
    StarsU16 expected_index;
    const StarsMoveFleet *moving;
    const StarsU8 *expected_fleet_body;
    const StarsU8 *expected_order_body;
    const StarsU16 *expected_fleet_lengths;
    const StarsU16 *expected_order_lengths;
    const StarsU16 *expected_owner0_ids;
    const StarsU8 (*expected_owner0_fleets)[64];
    const StarsU8 (*expected_owner0_orders)[18];
    StarsU16 fleet_id;
    StarsU16 expected_count;
    const Soe3InputFleet *current_fleet;
    current_fleet = &trace->complete_input.views[0].fleets[index];
    owner = current_fleet->owner;
    fleet_id = (StarsU16)(current_fleet->id_word & 511U);
    source_index = fleet_id;
    if (owner > 1U || source_index >= (owner == 0U ? 6U : 4U)) {
        ++trace->failures; return;
    }
    moving = owner == 1U ? &trace->turn_moving_fleets[source_index] :
        &trace->turn_owner0_moving_fleets[source_index];
    /* Expected IDs select assertions only. Runtime fleet identity comes from
     * the current table selected by the computed visibility predicate. */
    if (owner == 0U) {
        expected_count = turn_hst_owner0_output_count(trace);
        if (trace->seeded_action) {
            expected_owner0_ids = retained_hst_owner0_output_ids_action;
            expected_fleet_lengths = retained_hst_owner0_output_lengths_action;
            expected_owner0_fleets = retained_hst_owner0_output_bodies_action;
            expected_order_lengths = retained_hst_owner0_order_lengths_action;
            expected_owner0_orders = retained_hst_owner0_order_bodies_action;
        } else {
            expected_owner0_ids = retained_hst_owner0_output_ids_control;
            expected_fleet_lengths = retained_hst_owner0_output_lengths_control;
            expected_owner0_fleets = retained_hst_owner0_output_bodies_control;
            expected_order_lengths = retained_hst_owner0_order_lengths_control;
            expected_owner0_orders = retained_hst_owner0_order_bodies_control;
        }
    } else {
        expected_count = TURIN_M2_OUTPUT_FLEET_COUNT;
        expected_owner0_ids = retained_m2_fleet_output_ids;
        expected_fleet_lengths = retained_m2_fleet_output_lengths;
        expected_order_lengths = retained_m2_order_output_lengths;
        expected_owner0_fleets = 0; expected_owner0_orders = 0;
    }
    for (expected_index = 0U; expected_index < expected_count; ++expected_index)
        if (expected_owner0_ids[expected_index] == fleet_id) break;
    if (expected_index == expected_count) { ++trace->failures; return; }
    if (owner == 0U) {
        expected_fleet_body = expected_owner0_fleets[expected_index];
        expected_order_body = expected_owner0_orders[expected_index];
    } else {
        expected_fleet_body = retained_m2_fleet_output_bodies[expected_index];
        expected_order_body = retained_m2_order_output_bodies[expected_index];
    }
    memset(&input, 0, sizeof input);
    memset(&ops, 0, sizeof ops);
    memset(&context, 0, sizeof context);
    flags = trace->turn_visibility_fleet_flags[trace->output_bof_index][index];
    /* For these retained fleets, output fHereAllTurn follows whether current
     * production movement set its moved status bit. */
    status = (StarsU16)moving->status;
    if ((status & 0x2000U) != 0U)
        flags = (StarsU16)(flags & 0xdfffU);
    else
        flags = (StarsU16)(flags | 0x2000U);
    if (trace->turn_fuel_calls != 1 || trace->turn_fuel_fleet_count !=
            (trace->seeded_action ? 7U : 8U))
        ++trace->failures;
    if (trace->turn_heal_calls != 1 || trace->turn_heal_fleet_count !=
            (trace->seeded_action ? 7U : 8U))
        ++trace->failures;
    input.fleet_flags = flags;
    {
        Soe3InputFleet *current = turn_host_fleet(trace, owner, fleet_id);
        if (!current) return;
        for (cargo = 0U; cargo < 5U; ++cargo)
            input.minerals[cargo] = owner == 1U ?
                get_u32(trace->raw_fleets[source_index], (StarsU16)(76U + cargo * 4U)) :
                current->state.minerals[cargo];
        input.design_plan = owner == 1U ? trace->raw_fleets[source_index][96U] :
            (StarsU8)current->state.design_plan;
        input.cord = (StarsU8)current->state.order_count;
        input.order_count = current->state.order_count;
        for (ship = 0U; ship < 32U; ++ship)
            input.ship_counts[ship] = owner == 1U ?
                get_u16(trace->raw_fleets[source_index],
                    (StarsU16)(12U + ship * 2U)) :
                current->state.ship_counts[ship];
    }
    if (owner == 1U) {
        order_index = 0U;
        if (trace->plord[source_index][3] <= order_index) {
            ++trace->failures;
            return;
        }
        input.orders = trace->plord[source_index] + STARS_AI_PLORD_HEADER_SIZE +
            order_index * STARS_AI_PLORD_ORDER_SIZE;
    } else {
        input.orders = turn_host_fleet(trace, owner, fleet_id)->state.orders;
    }
    ops.write_record = turn_capture_player_fleet_record;
    context.trace = trace;
    context.file_index = (StarsU16)trace->output_bof_index;
    context.index = expected_index;
    context.fleet_id = fleet_id;
    context.id_word = current_fleet->id_word;
    context.owner = owner;
    context.planet = moving->target_id;
    context.x = moving->x;
    context.y = moving->y;
    context.expected_fleet_length = expected_fleet_lengths[expected_index];
    context.expected_order_length = expected_order_lengths[expected_index];
    context.expected_fleet_body = expected_fleet_body;
    context.expected_order_body = expected_order_body;
    if (!stars_write_fleet(&input, record, (StarsU16)sizeof record,
            &length, &ops, &context))
        ++trace->failures;
}

static StarsI16 turn_data_design_predicate(void *opaque, StarsU16 owner,
    StarsU16 bank, StarsU16 slot)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    const StarsU8 *raw;
    if (owner > 1U || bank > 1U || slot >= (bank ? 10U : 16U)) {
        ++trace->failures; return 0;
    }
    if (!(get_u16(trace->complete_input.views[0].players[owner], 6U) & 0x0100U)) return 0;
    raw = bank ? trace->complete_input.views[0].starbase_designs[owner] :
        trace->complete_input.views[0].ship_designs[owner];
    return (StarsI16)((get_u16(raw + slot * 147U, 123U) & 0x0300U) == 0x0100U);
}

static void turn_data_write_design(void *opaque, StarsU16 owner, StarsU16 bank, StarsU16 slot)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    const StarsU8 *raw = (bank ? trace->complete_input.views[0].starbase_designs[owner] :
        trace->complete_input.views[0].ship_designs[owner]) + slot * 147U;
    StarsWriteShdefInput input;
    StarsWriteShdefOps ops;
    StarsU8 out[160];
    StarsU16 size = 0U, file = (StarsU16)trace->output_bof_index;
    StarsU16 index = trace->turn_design_write_counts[file]++;
    memset(&input, 0, sizeof input); memset(&ops, 0, sizeof ops);
    input.flags = get_u16(raw, 123U); input.hull_id = get_u16(raw, 0U);
    input.bitmap = (StarsU8)get_u16(raw, 50U); input.component_count = raw[122];
    input.design_points = get_u16(raw, 56U); input.turn = get_u16(raw, 125U);
    input.built = get_u32(raw, 127U); input.existing = get_u32(raw, 131U);
    input.empty_weight = get_u16(raw, 40U); input.component_data = raw + 58U;
    input.component_data_size = (StarsU16)(raw[122] * 4U); input.name = raw + 8U;
    ops.compress_name = startup_compress_name;
    if (!stars_write_rt_shdef(&input, out, sizeof out, &size, &ops, trace) ||
            index >= expected_design_counts[trace->seeded_action][file] ||
            size != expected_design_sizes[trace->seeded_action][file][index] ||
            memcmp(out, expected_design_bodies[trace->seeded_action][file][index], size) != 0) {
        StarsU16 byte;
        for (byte = 0U; byte < size; ++byte)
            if (out[byte] != expected_design_bodies[trace->seeded_action][file][index][byte])
                fprintf(stderr, "design byte %u got=%u expected=%u\n", byte, out[byte], expected_design_bodies[trace->seeded_action][file][index][byte]);
        fprintf(stderr, "design file=%u owner=%u bank=%u slot=%u index=%u size=%u differs\n",
            file, owner, bank, slot, index, size);
        ++trace->failures;
    }
    turn_stream_record(trace, 0x1aU, size, out);
}

typedef struct TurnPlayerWriteContext {
    TurinTrace *trace;
    StarsU8 *data;
    StarsU16 player, calls, length;
    StarsU8 *record;
    StarsU16 *record_size;
} TurnPlayerWriteContext;

static void turn_capture_current_player(void *opaque, const StarsU8 *bytes, StarsU16 count)
{
    TurnPlayerWriteContext *context = (TurnPlayerWriteContext *)opaque;
    TurinTrace *trace = context->trace;
    if (context->calls++ == 0U) {
        if (count != 2U || (get_u16(bytes, 0U) >> 10) != 6U) { ++trace->failures; return; }
        context->length = (StarsU16)(get_u16(bytes, 0U) & 1023U);
    } else {
        if (count != context->length || count > 266U) { ++trace->failures; return; }
        /* The source writer's storage buffer retains the clear record body.
         * Complete encrypted stream assembly belongs to Gate 3. */
        memcpy(context->record, context->data + 0x4000U, count);
        *context->record_size = count;
    }
}

static StarsI16 turn_data_current_player_predicate(void *opaque, StarsU16 player)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    const StarsVisPFPlayer *view;
    if (player >= 2U) return 0;
    view = trace->output_bof_index == 2 ? &trace->turn_host_visibility_players[player] :
        (trace->output_bof_index == 1 ? &trace->turn_player1_visibility_players[player] :
        &trace->turn_player0_visibility_players[player]);
    return (StarsI16)((view->detail_flags & 0x0100U) != 0U ||
        (get_u16(trace->complete_input.views[0].players[player], 84U) & 1U) != 0U);
}

static void turn_data_current_player(void *opaque, StarsU16 player)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    const StarsVisPFPlayer *view;
    StarsU8 *data;
    TurnPlayerWriteContext context;
    StarsU16 i, expected;
    const StarsU8 *expected_record;
    /* FWriteDataFile forces the low player-mode bits to 7 for raTerra
     * immediately before WriteRtPlr. The loaded majors are 9 and 1. */
    if (turn_score_resource_stat(trace, (StarsI16)player, 14) == 3) {
        ++trace->failures; return;
    }
    if (player >= 2U || (trace->output_bof_index == 2 && !trace->turn_host_visibility_calls) ||
            (trace->output_bof_index == 1 && !trace->turn_player1_visibility_calls) ||
            (trace->output_bof_index == 0 && !trace->turn_player0_visibility_calls)) {
        ++trace->failures; return;
    }
    data = (StarsU8 *)calloc(65536U, 1U);
    if (!data) { ++trace->failures; return; }
    memcpy(data, retained_input_decode_prefix, sizeof retained_input_decode_prefix);
    memcpy(data + 0x3000U, trace->complete_input.views[0].players[player], 192U);
    view = trace->output_bof_index == 2 ? &trace->turn_host_visibility_players[player] :
        (trace->output_bof_index == 1 ? &trace->turn_player1_visibility_players[player] :
        &trace->turn_player0_visibility_players[player]);
    data[0x3001U] = view->design_count;
    put_u16(data, 0x3002U, view->planet_count);
    put_u16(data, 0x3004U, view->fleet_count);
    put_u16(data, 0x3006U, view->detail_flags);
    memset(&context, 0, sizeof context);
    context.trace = trace; context.data = data; context.player = player;
    if (trace->output_bof_index == 2) {
        context.record = trace->turn_host_player_records[player];
        context.record_size = &trace->turn_host_player_sizes[player];
        expected = expected_host_player_sizes[trace->seeded_action][player];
        expected_record = expected_host_players[trace->seeded_action][player];
    } else if (trace->output_bof_index == 1) {
        if (player != 1U) { free(data); ++trace->failures; return; }
        context.record = trace->turn_m2_player_record;
        context.record_size = &trace->turn_m2_player_size;
        expected = expected_m2_player_sizes[trace->seeded_action];
        expected_record = expected_m2_players[trace->seeded_action];
    } else {
        if (player != 0U) { free(data); ++trace->failures; return; }
        context.record = trace->turn_m1_player_record;
        context.record_size = &trace->turn_m1_player_size;
        expected = expected_m1_player_sizes[trace->seeded_action];
        expected_record = expected_m1_players[trace->seeded_action];
    }
    stars_write_player_record(data, 0x3000U, 0x4000U, turn_capture_current_player, &context);
    if (context.calls != 2U || context.length != expected ||
            memcmp(context.record, expected_record, expected) != 0) {
        fprintf(stderr, "file %d player %u length=%u expected=%u\n", trace->output_bof_index, player, context.length, expected);
        for (i = 0U; i < expected; ++i)
            if (context.record[i] != expected_record[i])
                fprintf(stderr, " player byte %u actual=%u expected=%u\n", i,
                    context.record[i], expected_record[i]);
        ++trace->failures;
    }
    if (context.calls == 2U)
        turn_stream_record(trace, 6U, context.length, context.record);
    memcpy(trace->complete_input.views[0].players[player], data + 0x3000U, 192U);
    free(data);
    if (trace->output_bof_index == 2) ++trace->turn_host_player_writes;
    else if (trace->output_bof_index == 1) ++trace->turn_m2_player_writes;
    else ++trace->turn_m1_player_writes;
}

static void turn_prepare_output_view(void *opaque, StarsI16 viewer)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    const Soe3InputView *world = &trace->complete_input.views[0];
    StarsU16 index, slot;
    if (viewer < 0 || viewer > 1 || !(trace->game_flags & 0x0002U)) {
        ++trace->failures; return;
    }
    ++trace->turn_prepare_view_calls[viewer];
    /* Original FWriteDataFile's gd.grBits bit-1 block prepares orders.
     * Prove every inner mutation inapplicable from current fleets/orders,
     * independently of that external session flag. */
    for (index = 0U; index < world->fleet_count; ++index) {
        const Soe3InputFleet *fleet = &world->fleets[index];
        const StarsU8 *orders = fleet->state.orders;
        StarsU16 id = (StarsU16)(fleet->id_word & 511U);
        StarsU16 count = fleet->state.order_count, flags, ships = 0U;
        if ((StarsI16)((fleet->id_word >> 9) & 15U) != viewer ||
                (trace->turn_visibility_fleet_flags[viewer][index] & 0x0400U)) continue;
        if (fleet->owner == 1) {
            if (id >= 4U) { ++trace->failures; return; }
            orders = trace->plord[id] + STARS_AI_PLORD_HEADER_SIZE;
            count = trace->plord[id][3];
        }
        for (slot = 0U; slot < 16U; ++slot)
            if (fleet->owner == 1 ? get_u16(trace->raw_fleets[id], (StarsU16)(12U + slot * 2U)) :
                    fleet->state.ship_counts[slot]) ships = 1U;
        /* Empty-fleet retirement, target conversion, patrol and additional
         * waypoint tracking need actual services if they become eligible. */
        flags = count ? get_u16(orders, 6U) : 0U;
        if (!ships || count != 1U || ((flags >> 8) & 15U) == 2U || (flags & 15U) == 7U) {
            fprintf(stderr, "prepare-view requires service: viewer=%d fleet=%u count=%u flags=%u ships=%u\n",
                viewer, id, count, flags, ships);
            ++trace->failures; return;
        }
        ++trace->turn_prepare_view_fleets[viewer];
    }
}

static void turn_write_battle_output(void *opaque, StarsI16 player)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsWriteBattlesInput input;
    memset(&input, 0, sizeof input);
    input.player = player;
    /* Input census rejects battle records; the source battle stage rejects
     * newly eligible combat. Thus the current output list is empty. */
    input.block_count = trace->turn_battle_output_blocks;
    if (input.block_count || !stars_write_battles(&input, 0, trace))
        ++trace->failures;
    ++trace->turn_battle_output_calls;
}

static void turn_write_salt_output(void *opaque, StarsU32 salt)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    (void)salt;
    /* A nonzero loaded salt needs a type-36 writer in this bounded probe. */
    ++trace->failures;
}

static void turn_write_data_file(void *opaque,
    StarsGenerateTurnState *state, StarsI16 player, StarsI16 flags)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    StarsWriteDataFileInput input;
    StarsWriteDataFileState write_state;
    StarsWriteDataFileOps ops;
    StarsU16 fleet_count;
    memset(&input, 0, sizeof input);
    memset(&write_state, 0, sizeof write_state);
    memset(&ops, 0, sizeof ops);
    input.player = player;
    input.append = flags;
    input.player_count = (StarsU16)state->player_count;
    input.turn = (StarsU16)state->turn;
    input.salt = input_host_salts[trace->seeded_action];
    input.session_flags = trace->game_flags;
    fleet_count = trace->complete_input.views[0].fleet_count;
    input.fleet_count = fleet_count;
    ops.set_visible_plan_fleet = turn_data_file_set_visibility;
    ops.prepare_view = turn_prepare_output_view;
    ops.mark_messages = turn_mark_message_senders;
    ops.mark_planets_lost = turn_mark_message_planet_losses;
    ops.create_file = turn_data_file_create;
    ops.append_file = turn_data_file_append;
    ops.write_player_predicate = turn_data_current_player_predicate;
    ops.write_player = turn_data_current_player;
    ops.write_messages = turn_data_file_messages;
    ops.write_planets = turn_data_file_planets;
    ops.write_design_predicate = turn_data_design_predicate;
    ops.write_design = turn_data_write_design;
    ops.write_scores = turn_data_file_scores;
    ops.write_things = turn_data_file_things;
    ops.write_battle_plans = turn_data_file_battle_plans;
    ops.write_turn = turn_data_file_turn;
    ops.write_fleet_predicate = turn_data_file_fleet_predicate;
    ops.write_fleet = turn_data_file_write_fleet;
    ops.close_file = turn_data_file_close;
    ops.write_battles = turn_write_battle_output;
    ops.write_salt = turn_write_salt_output;
    ++trace->turn_write_data_calls;
    if (!stars_write_data_file(&input, &write_state, &ops, trace))
        ++trace->failures;
}

static void turn_begin_generation(void *opaque, StarsGenerateTurnState *state)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    if (!state || trace->turn_generation_flag_entries ||
            trace->turn_generation_flag_exits) {
        ++trace->failures; return;
    }
    trace->turn_generation_prior_flags = trace->game_flags;
    trace->game_flags = (StarsU16)((trace->game_flags & 0xfdfdU) | 0x0202U);
    ++trace->turn_generation_flag_entries;
}

static void turn_end_generation(void *opaque, StarsGenerateTurnState *state)
{
    TurinTrace *trace = (TurinTrace *)opaque;
    if (!state || trace->turn_generation_flag_entries != 1U ||
            trace->turn_generation_flag_exits ||
            (trace->game_flags & 0x0202U) != 0x0202U ||
            !state->output_written || trace->turn_data_file_close_calls != 3) {
        ++trace->failures; return;
    }
    trace->game_flags = (StarsU16)(trace->game_flags & 0xfdfdU);
    trace->session.memory_log_current = 0;
    ++trace->turn_generation_flag_exits;
}

static void run_generate_turn(TurinTrace *trace)
{
    StarsGenerateTurnState state;
    StarsGenerateTurnOps ops;
    StarsI16 index;
    const StarsI16 *expected_random;
    memset(&state, 0, sizeof state);
    memset(&ops, 0, sizeof ops);
    ops.destroy_game = startup_host_destroy;
    ops.load_game = startup_turn_load;
    ops.initialize_turn = turn_begin_generation;
    ops.cleanup = turn_end_generation;
    ops.load_player_log = turn_load_player_log;
    ops.validate_player_logs = turn_validate_player_logs;
    ops.validate_waypoints = turn_validate_waypoints;
    ops.do_orders = turn_do_orders;
    ops.move_things = turn_move_things;
    ops.move_fleets = turn_move_fleets;
    ops.unmark_mine_fields = turn_unmark_mine_fields;
    ops.thing_decay = turn_thing_decay;
    ops.breed_colonists = turn_breed_colonists;
    ops.produce = turn_produce;
    ops.fuel_fleets = turn_fuel_fleets;
    ops.sweep_for_mines = turn_sweep_for_mines;
    ops.heal_ships = turn_heal_ships;
    ops.auto_terraform = turn_auto_terraform;
    ops.remote_terraforming = turn_remote_terraform;
    ops.spank_cheaters = turn_spank_cheaters;
    ops.update_guesses = turn_update_guesses;
    ops.update_scores = turn_update_scores;
    ops.mark_backup = turn_mark_backup;
    ops.prepare_output_files = turn_prepare_output_files;
    ops.write_data_file = turn_write_data_file;
    ops.random_value = random_value;
    ops.context = trace;
    ++trace->startup_turn_shuffle_calls;
    if (!stars_generate_turn(&state, &ops)) {
        ++trace->failures;
        return;
    }
    if (trace->output_file_open || trace->output_file_closes[0] != 1U ||
            trace->output_file_closes[1] != 1U ||
            trace->output_file_closes[2] != 1U ||
            trace->output_file_prefix_sizes[0] != 0U ||
            trace->output_file_prefix_sizes[1] != trace->backup_file_sizes[1] ||
            trace->output_file_prefix_sizes[2] != 0U ||
            trace->active_file_sizes[0] <= 18U ||
            trace->active_file_sizes[1] <=
                (StarsU16)(trace->backup_file_sizes[1] + 18U) ||
            trace->active_file_sizes[2] <= 18U ||
            memcmp(trace->active_files[1], trace->backup_files[1], 16U) != 0 ||
            memcmp(trace->active_files[1] + 18U,
                trace->backup_files[1] + 18U,
                trace->backup_file_sizes[1] - 18U) != 0 ||
            get_u16(trace->active_files[1], 16U) !=
                (StarsU16)(get_u16(trace->backup_files[1], 16U) | 0x0400U))
        ++trace->failures;
    trace->startup_turn_order[0] = state.player_order[0];
    trace->startup_turn_order[1] = state.player_order[1];
    trace->turn_state_turn = state.turn;
    trace->turn_state_generated = state.generated;
    trace->turn_state_output_written = state.output_written;
    if (!state.generated || !state.output_written ||
            state.player_order_count != 2 || state.turn != 1 ||
            trace->turn_load_log_calls != 2 ||
            trace->input_player_reads[0] != 1U ||
            trace->input_player_reads[1] != 1U ||
            trace->input_player_reads[2] != 1U ||
            trace->input_player_reads[3] != 2U ||
            trace->turn_m1_planet_read_calls !=
                (StarsI16)TURIN_M1_PLANET_RECORD_COUNT ||
            trace->turn_m2_planet_read_calls !=
                (StarsI16)TURIN_M2_PLANET_INPUT_COUNT ||
            trace->turn_m2_shdef_read_calls !=
                (StarsI16)TURIN_M2_SHDEF_DECODABLE_COUNT ||
            trace->turn_m2_shdef_unresolved_calls != 0 ||
            trace->turn_m2_fleet_read_calls !=
                (StarsI16)TURIN_M2_INPUT_FLEET_COUNT ||
            trace->turn_validate_calls != 2 ||
            trace->turn_waypoint_source_calls != 3 ||
            trace->turn_used_waypoint_calls != 1 ||
            trace->turn_used_waypoint_messages != 1 ||
            trace->turn_used_waypoint_message[0][0] != 1 ||
            trace->turn_used_waypoint_message[0][1] != 0x4e ||
            (trace->turn_used_waypoint_message[0][2] & 511) != 0 ||
            trace->turn_waypoint_fleets[0] != 9 ||
            trace->turn_waypoint_orders[0] != 10 ||
            trace->turn_waypoint_fleets[1] != (trace->seeded_action ? 7 : 8) ||
            trace->turn_waypoint_fleets[2] != (trace->seeded_action ? 7 : 8) ||
            trace->turn_waypoint_orders[1] != (trace->seeded_action ? 7 : 8) ||
            trace->turn_waypoint_orders[2] != (trace->seeded_action ? 7 : 8) ||
            trace->turn_unmark_mine_calls != 1 ||
            trace->turn_pre_move_things_calls != 1 ||
            trace->turn_thing_decay_calls != 1 ||
            trace->turn_thing_decay_free_calls != 0 ||
            trace->turn_orders_calls[0] != 1 ||
            trace->turn_orders_calls[1] != 1 ||
            trace->turn_task5_visit_count != (trace->seeded_action ? 2U : 1U) ||
            (trace->seeded_action && trace->turn_task5_visit_ids[0] != 0U) ||
            trace->turn_task5_visit_ids[trace->seeded_action ? 1 : 0] != 514U ||
            trace->turn_order_pass_calls != 4 ||
            trace->turn_spank_cheaters_calls != 1 ||
            trace->turn_breed_colonists_calls != 1 ||
            trace->turn_auto_terraform_calls != 1 ||
            trace->turn_remote_terraform_calls != 1 ||
            trace->turn_remote_fleet_count != (trace->seeded_action ? 7U : 8U) ||
            trace->turn_sweep_for_mines_calls != 1 ||
            trace->turn_sweep_fleet_count !=
                (trace->seeded_action ? 7 : 8) ||
            trace->turn_sweep_fleet_power_calls !=
                trace->turn_sweep_fleet_count ||
            trace->turn_sweep_planet_count != TURIN_RETAINED_POINT_COUNT ||
            trace->turn_sweep_planet_power_calls !=
                TURIN_RETAINED_OWNED_PLANET_COUNT ||
            trace->turn_sweep_thing_count != TURIN_RETAINED_THING_COUNT ||
            trace->turn_sweep_can_attack_calls != 0 ||
            trace->turn_sweep_message_calls != 0 ||
            trace->turn_sweep_free_calls != 0 ||
            trace->turn_order_passes[0] != 1 ||
            trace->turn_order_passes[1] != 2 ||
            trace->turn_order_passes[2] != 3 ||
            trace->turn_order_passes[3] != 4 ||
            trace->turn_battle_eligibility_calls == 0U || trace->turn_bombing_calls != 1U ||
            trace->turn_battle_link_calls != 1U ||
            trace->turn_battle_link_count != trace->complete_input.views[0].fleet_count ||
            trace->turn_drop_calls != 2U || trace->turn_transfer_calls != 1U ||
            trace->turn_drop_count != 0U || trace->turn_transfer_count != 0U ||
            trace->turn_order_research_calls != 2U ||
            trace->turn_interaction_calls[0] != 1U || trace->turn_interaction_calls[1] != 1U ||
            trace->turn_interaction_things[0] != 4U || trace->turn_interaction_things[1] != 4U ||
            trace->turn_battle_calls != 1 || trace->turn_battle_id != 1U ||
            trace->turn_move_fleets_calls != 1 || trace->turn_produce_calls != 1 ||
            trace->turn_m1_fleet_read_calls !=
                (StarsI16)TURIN_HST_OWNER0_INPUT_FLEET_COUNT ||
            trace->turn_hst_planet_read_calls !=
                (StarsI16)TURIN_RETAINED_POINT_COUNT ||
            trace->turn_queue_prepare_calls != 1 ||
            trace->turn_seed_queue_calls != 2 ||
            trace->turn_seed_finish_calls != 1 ||
            trace->final_queue_budget_calls != 1 ||
            trace->startup_defense_gate_calls != 1U ||
            trace->turn_owner0_task5_result !=
                (trace->seeded_action ? 1 : 0) ||
            trace->turn_update_guesses_calls != 1 ||
            trace->turn_mark_backup_calls != 1 ||
            trace->turn_generation_flag_entries != 1U ||
            trace->turn_generation_flag_exits != 1U ||
            (trace->game_flags & 0x0202U) != 0U ||
            trace->turn_prepare_output_calls != 1U ||
            trace->turn_prepare_view_calls[0] != 1U ||
            trace->turn_prepare_view_calls[1] != 1U ||
            trace->turn_prepare_view_fleets[0] != (trace->seeded_action ? 5U : 6U) ||
            trace->turn_prepare_view_fleets[1] != 2U ||
            trace->turn_planet_visibility_calls[0] != 1U ||
            trace->turn_planet_visibility_calls[1] != 1U ||
            trace->turn_thing_visibility_calls[0] != 1U ||
            trace->turn_thing_visibility_calls[1] != 1U ||
            trace->turn_fleet_visibility_calls[0] != 1U ||
            trace->turn_fleet_visibility_calls[1] != 1U ||
            trace->turn_m1_player_writes != 1U ||
            trace->turn_m2_player_writes != 1U ||
            trace->turn_host_player_writes != 2U ||
            trace->turn_mark_message_calls != 3U ||
            trace->turn_mark_loss_calls != 3U ||
            trace->turn_write_message_calls != 3 ||
            trace->turn_write_data_calls != 3 ||
            trace->turn_data_file_create_calls != 2 ||
            trace->turn_data_file_append_calls != 1U ||
            trace->turn_append_mark_calls != 1U ||
            trace->turn_data_file_planet_calls != 3 ||
            trace->turn_score_record_counts[0] != 1 ||
            trace->turn_score_record_counts[1] != 1 ||
            trace->turn_score_record_counts[2] != 0 ||
            trace->turn_data_file_close_calls != 3 ||
            trace->turn_data_file_visibility_calls != 6 ||
            trace->turn_player1_visibility_calls != 1 ||
            trace->turn_visible_planet_mark_calls != 1 ||
            !trace->turn_visible_planet26_valid ||
            trace->turn_visible_planet26.flags != 0x0103U ||
            trace->turn_data_turn_marker_calls != 3 ||
            trace->turn_player_planet_write_counts[0] !=
                (StarsI16)TURIN_M1_OUTPUT_PLANET_COUNT ||
            trace->turn_player_planet_write_counts[1] != 2 ||
            trace->turn_player_fleet_write_counts[0] !=
                (StarsI16)turn_hst_owner0_output_count(trace) ||
            trace->turn_player_fleet_write_counts[1] !=
                (StarsI16)TURIN_M2_OUTPUT_FLEET_COUNT ||
            trace->turn_player_fleet_write_counts[2] !=
                (StarsI16)(turn_hst_owner0_output_count(trace) +
                    TURIN_M2_OUTPUT_FLEET_COUNT) ||
            trace->turn_player_order_write_counts[0] !=
                (StarsI16)turn_hst_owner0_output_count(trace) ||
            trace->turn_player_order_write_counts[1] !=
                (StarsI16)TURIN_M2_OUTPUT_FLEET_COUNT ||
            trace->turn_player_order_write_counts[2] !=
                (StarsI16)(turn_hst_owner0_output_count(trace) +
                    TURIN_M2_OUTPUT_FLEET_COUNT) ||
            trace->turn_production_queue_write_counts[0] != 0 ||
            trace->turn_production_queue_write_counts[1] != 1 ||
            trace->turn_production_queue_write_counts[2] != 1 ||
            trace->turn_thing_record_counts[0] != 0 ||
            trace->turn_thing_record_counts[1] != 0 ||
            trace->turn_thing_record_counts[2] !=
                (StarsI16)(TURIN_RETAINED_THING_COUNT + 1U) ||
            trace->output_bof_index != 3 ||
            trace->turn_data_turn_values[0] != 1U ||
            trace->turn_data_turn_values[1] != 1U ||
            trace->turn_data_turn_values[2] != 1U ||
            get_u16(trace->turn_data_turn_marker_bodies[0], 0U) != 1U ||
            get_u16(trace->turn_data_turn_marker_bodies[1], 0U) != 1U ||
            get_u16(trace->turn_data_turn_marker_bodies[2], 0U) != 1U ||
            trace->turn_planet_write_calls !=
                (StarsI16)TURIN_RETAINED_ALL_PLANET_COUNT ||
            trace->turn_planet_level_divergences != 0 ||
            trace->turn_planet_stock_divergences != 0 ||
            (trace->turn_order_planets[10].flags & 0x2000U) == 0U ||
            (trace->turn_order_planets[24].flags & 0x2000U) == 0U ||
            trace->turn_order_planets[10].turn != 0U ||
            trace->turn_order_planets[24].turn != 0U)
        ++trace->failures;
    for (index = 0; index < 4; ++index)
        if (trace->turn_order_fleet_visits[index] != trace->complete_input.views[0].fleet_count ||
                trace->turn_passive_orders[index] != (trace->seeded_action ? 7U : 8U))
            ++trace->failures;
    expected_random = trace->seeded_action ? retained_output_bof_values_action :
        retained_output_bof_values_control;
    for (index = 0; index < 3; ++index)
        if (trace->output_bof_random_values[index] != expected_random[index])
            ++trace->failures;
}

/* Separate perturbation test: never alter either oracle replay. This makes
 * damage write-back observable even when retained fleets are undamaged. */
static void check_healing_adapter_writeback(TurinTrace *original)
{
    TurinTrace *copy = (TurinTrace *)malloc(sizeof *copy);
    StarsGenerateTurnState state;
    Soe3InputFleet *owner0;
    Soe3InputFleet *owner1;
    StarsU16 ship;
    if (!copy) { ++original->failures; return; }
    *copy = *original;
    memset(&state, 0, sizeof state);
    state.player_count = 2;
    owner0 = turn_host_fleet(copy, 0U, 1U);
    owner1 = turn_host_fleet(copy, 1U, 0U);
    if (!owner0 || !owner1) {
        ++original->failures; free(copy); return;
    }
    copy->complete_input.views[0].players[0][76] = 0;
    copy->complete_input.views[0].players[1][76] = 0;
    owner0->state.fleet_flags = 0;
    put_u16(copy->raw_fleets[0], 4U, 0U);
    for (ship = 0U; ship < 16U; ++ship) {
        owner0->state.ship_counts[ship] = ship == 0U;
        put_u16(copy->raw_fleets[0], (StarsU16)(12U + ship * 2U), ship == 0U);
    }
    put_u16(copy->complete_input.views[0].ship_designs[0], 0U, 0U);
    put_u16(copy->designs, 0U, 0U);
    owner0->state.ship_counts[16] = (80U << 7) | 3U;
    put_u16(copy->raw_fleets[0], 44U, (80U << 7) | 3U);
    copy->turn_owner0_moving_fleets[1].status = 0x2000;
    copy->turn_moving_fleets[0].status = 0x2000;
    copy->turn_planet_states[10].player = 0;
    copy->turn_planet_states[10].flags |= 0x0200U;
    copy->turn_planet_states[10].opaque_44 = 80UL << 4;
    put_u16(copy->complete_input.views[0].starbase_designs[0], 0U, 32U);
    turn_heal_ships(copy, &state);
    check(original, copy->failures == original->failures &&
        owner0->state.ship_counts[16] == ((75U << 7) | 3U) &&
        owner1->state.ship_counts[16] == ((75U << 7) | 3U) &&
        get_u16(copy->raw_fleets[0], 44U) == ((75U << 7) | 3U) &&
        copy->turn_planet_states[10].opaque_44 == (30UL << 4));
    free(copy);
}

static void check_fuel_adapter_writeback(TurinTrace *original)
{
    TurinTrace *copy = (TurinTrace *)malloc(sizeof *copy);
    StarsGenerateTurnState state;
    Soe3InputFleet *owner0;
    Soe3InputFleet *owner1;
    StarsFuelFleet access;
    StarsU8 *design;
    StarsU8 hull[143];
    StarsI32 expected_capacity;
    StarsU16 ship;
    if (!copy) { ++original->failures; return; }
    *copy = *original;
    memset(&state, 0, sizeof state); state.player_count = 2;
    memset(&access, 0, sizeof access);
    owner0 = turn_host_fleet(copy, 0U, 1U);
    owner1 = turn_host_fleet(copy, 1U, 0U);
    if (!owner0 || !owner1) { ++original->failures; free(copy); return; }
    owner0->state.fleet_flags = 7U;
    put_u16(copy->raw_fleets[0], 4U, 7U);
    for (ship = 0U; ship < 16U; ++ship) {
        owner0->state.ship_counts[ship] = ship == 0U;
        put_u16(copy->raw_fleets[0], (StarsU16)(12U + ship * 2U), ship == 0U);
    }
    design = copy->complete_input.views[0].ship_designs[0];
    put_u16(design, 0U, 25U); /* One transport and two generators. */
    design[122] = 1U;
    put_u16(design, 58U, 0x0800U); design[60] = 16U; design[61] = 2U;
    copy->turn_owner0_moving_fleets[1].target_id = -1;
    copy->turn_owner0_moving_fleets[1].fuel = 10L;
    design = copy->designs;
    put_u16(design, 0U, 4U); design[122] = 0U;
    if (!scrap_read_hull(copy, 4U, hull, sizeof hull)) {
        ++original->failures; free(copy); return;
    }
    expected_capacity = get_u16(hull, 54U);
    copy->turn_moving_fleets[0].target_id = 10;
    copy->turn_moving_fleets[0].fuel = 10L;
    copy->turn_planet_states[10].player = 0;
    copy->turn_planet_states[10].flags |= 0x0200U;
    copy->turn_planet_states[10].opaque_44 = 0UL;
    put_u16(copy->complete_input.views[0].starbase_designs[0], 0U, 33U);
    access.player = 1; access.destination = 10;
    copy->complete_input.views[0].players[0][113] = 0U;
    copy->complete_input.views[0].players[1][112] = 1U;
    check(original, !turn_fuel_can_refuel(copy, &access));
    copy->complete_input.views[0].players[0][113] = 1U;
    check(original, turn_fuel_can_refuel(copy, &access));
    put_u16(copy->complete_input.views[0].starbase_designs[0], 0U, 32U);
    check(original, !turn_fuel_can_refuel(copy, &access));
    put_u16(copy->complete_input.views[0].starbase_designs[0], 0U, 33U);
    turn_fuel_fleets(copy, &state);
    check(original, copy->failures == 0 &&
        copy->turn_owner0_moving_fleets[1].fuel == 310L &&
        owner0->state.minerals[4] == 310UL &&
        copy->turn_moving_fleets[0].fuel == expected_capacity &&
        owner1->state.minerals[4] == (StarsU32)expected_capacity &&
        get_u32(copy->raw_fleets[0], 92U) == (StarsU32)expected_capacity);
    /* Production above capacity saturates at hull + component capacity. */
    if (!scrap_read_hull(copy, 25U, hull, sizeof hull)) {
        ++original->failures; free(copy); return;
    }
    expected_capacity = (StarsI32)get_u16(hull, 54U) + 400L;
    copy->turn_owner0_moving_fleets[1].fuel = expected_capacity - 1L;
    turn_fuel_fleets(copy, &state);
    check(original, copy->turn_owner0_moving_fleets[1].fuel == expected_capacity &&
        owner0->state.minerals[4] == (StarsU32)expected_capacity);
    free(copy);
}

static void check_terraform_gate_adapters(TurinTrace *original)
{
    TurinTrace *copy = (TurinTrace *)malloc(sizeof *copy);
    StarsRemoteTurnFleet fleet;
    StarsGenerateTurnState state;
    StarsU16 owner, ship;
    StarsI16 previous_failures;
    if (!copy) { ++original->failures; return; }
    *copy = *original;
    memset(&fleet, 0, sizeof fleet);
    memset(&state, 0, sizeof state); state.player_count = 2;
    for (owner = 0U; owner < 2U; ++owner) {
        StarsU16 id = owner == 0U ? 1U : 0U;
        Soe3InputFleet *source = turn_host_fleet(copy, owner, id);
        StarsU8 *bank = owner == 0U ?
            copy->complete_input.views[0].ship_designs[0] : copy->designs;
        if (!source) { ++original->failures; free(copy); return; }
        for (ship = 0U; ship < 16U; ++ship) {
            source->state.ship_counts[ship] = ship == 0U ? 3U : 0U;
            if (owner == 1U)
                put_u16(copy->raw_fleets[id], (StarsU16)(12U + ship * 2U),
                    source->state.ship_counts[ship]);
        }
        bank[122] = 1U;
        put_u16(bank, 58U, 0x0080U);
        bank[60] = 7U; bank[61] = 2U;
        fleet.player = (StarsI16)owner;
        fleet.object_token = source->id_word;
        check(original, turn_remote_amount(copy, &fleet) == 6L);
        bank[60] = 6U;
        check(original, turn_remote_amount(copy, &fleet) == 0L);
    }
    previous_failures = copy->failures;
    copy->complete_input.views[0].players[0][76] = 3U;
    turn_auto_terraform(copy, &state);
    check(original, copy->failures == previous_failures + 1);
    previous_failures = copy->failures;
    copy->complete_input.views[0].players[0][76] = 4U;
    turn_breed_colonists(copy, &state);
    check(original, copy->failures == previous_failures + 1);
    free(copy);
}

static void check_current_score_inputs(TurinTrace *original)
{
    TurinTrace *copy = (TurinTrace *)malloc(sizeof *copy);
    StarsUpdatePlayerScore score;
    StarsU8 packed[24];
    StarsI16 old_tech;
    if (!copy) { ++original->failures; return; }
    *copy = *original;
    memset(&score, 0, sizeof score);
    copy->turn_planet_states[10].minerals[3] += 1000UL;
    turn_score_calculate(copy, 0, &score);
    stars_pack_score_history_record(&copy->turn_score_summaries[0],
        get_u16(original->turn_score_records[0], 0U),
        get_u16(original->turn_score_records[0], 2U), packed);
    check(original, copy->failures == 0 &&
        score.resources > original->turn_player_scores[0].resources &&
        score.total > original->turn_player_scores[0].total &&
        memcmp(packed, original->turn_score_records[0], 24U) != 0);
    old_tech = original->turn_player_scores[1].tech_count;
    ++copy->complete_input.views[0].players[1][26];
    turn_score_calculate(copy, 1, &score);
    check(original, score.tech_count == old_tech + 1 &&
        score.total > original->turn_player_scores[1].total &&
        score.technology_levels[0] == original->turn_player_scores[1].technology_levels[0] + 1);
    free(copy);
}

static void check_current_production_costs(TurinTrace *original)
{
    TurinTrace *copy = (TurinTrace *)malloc(sizeof *copy);
    StarsProducePlanet planet;
    StarsProduceItem item;
    StarsI32 costs[4];
    StarsU16 owner;
    if (!copy) { ++original->failures; return; }
    *copy = *original;
    memset(&planet, 0, sizeof planet); memset(&item, 0, sizeof item);
    item.object_kind = 2;
    for (owner = 0U; owner < 2U; ++owner) {
        StarsU8 *design = owner == 1U ? copy->designs :
            copy->complete_input.views[0].ship_designs[0];
        planet.owner = (StarsI16)owner;
        put_u16(design, 42U, 71U); put_u16(design, 44U, 13U);
        put_u16(design, 46U, 17U); put_u16(design, 48U, 19U);
        check(original, turn_produce_costs(copy, &planet, &item, costs) &&
            costs[0] == 13L && costs[1] == 17L && costs[2] == 19L && costs[3] == 71L);
    }
    free(copy);
}

static void check_current_sweep_power(TurinTrace *trace)
{
    TurinTrace *copy = (TurinTrace *)malloc(sizeof *copy);
    StarsU8 design[147];
    StarsSweepMinesFleet fleet;
    StarsSweepMinesPlanet planet;
    Soe3InputFleet *source;
    const StarsU8 *laser = scrap_part_record(0x10U, 0U);
    if (!copy) { ++trace->failures; return; }
    *copy = *trace;
    memset(design, 0, sizeof design); memset(&fleet, 0, sizeof fleet);
    memset(&planet, 0, sizeof planet);
    /* Pinned Laser: range 1, power 10, no special abilities. */
    check(trace, laser && get_u16(laser, 52U) == 1U &&
        get_u16(laser, 54U) == 10U && get_u16(laser, 58U) == 0U);
    design[122] = 1U; put_u16(design, 58U, 0x10U);
    put_u16(design, 60U, 0x0200U);
    check(trace, turn_sweep_design(copy, design) == 20L);
    memcpy(copy->complete_input.views[0].ship_designs[0], design, sizeof design);
    source = turn_host_fleet(copy, 0U, 1U);
    if (!source) { ++trace->failures; free(copy); return; }
    memset(source->state.ship_counts, 0, sizeof source->state.ship_counts);
    source->state.ship_counts[0] = 3U;
    fleet.player = 0; fleet.id = 1;
    check(trace, turn_sweep_fleet_power(copy, &fleet) == 60L);
    put_u16(design, 0U, 32U);
    memcpy(copy->complete_input.views[0].starbase_designs[0], design, sizeof design);
    copy->turn_planet_states[10].flags |= 0x0200U;
    copy->turn_planet_states[10].opaque_44 &= ~15UL;
    planet.id = 10; planet.player = 0;
    check(trace, turn_sweep_planet_power(copy, &planet) == 80L);
    copy->turn_planet_states[10].flags &= (StarsU16)~0x0200U;
    check(trace, turn_sweep_planet_power(copy, &planet) == 0L);
    copy->complete_input.views[0].players[0][113] = 1U;
    check(trace, turn_sweep_can_attack(copy, 0, 1) == 0);
    copy->complete_input.views[0].players[0][113] = 0U;
    check(trace, turn_sweep_can_attack(copy, 0, 1) == 1);
    check(trace, copy->failures == trace->failures);
    free(copy);
}

static void capture_starbase_request(void *opaque, StarsI16 design, StarsI16 kind)
{
    StarsU16 *mask = (StarsU16 *)opaque;
    (void)kind;
    *mask = (StarsU16)(*mask | (1U << design));
}

static void check_current_ai_design_inputs(TurinTrace *trace)
{
    TurinTrace *copy = (TurinTrace *)malloc(sizeof *copy);
    TurnBattleContext battle;
    StarsPoint point;
    StarsU16 mask = 0U;
    StarsU16 slot;
    if (!copy) { ++trace->failures; return; }
    *copy = *trace;
    check(trace, turn_log_accept_header(copy, 0x2a60U, 0x1001U));
    check(trace, (get_u16(copy->complete_input.views[0].players[1], 84U) & 2U) != 0U &&
        (copy->log_view_flags_high & 4U) != 0U &&
        get_u16(copy->complete_input.views[0].players[1], 6U) ==
        get_u16(trace->complete_input.views[0].players[1], 6U));
    check(trace, turn_log_accept_header(copy, 0x2a60U, 1U));
    check(trace, (get_u16(copy->complete_input.views[0].players[1], 84U) & 2U) == 0U &&
        (copy->log_view_flags_high & 4U) == 0U);
    *copy = *trace;
    copy->turn_order_pass_calls = 1;
    copy->turn_planet_states[24].minerals[0] = 701UL;
    copy->turn_planet_states[24].minerals[3] = 219UL;
    copy->turn_planet_states[24].flags = 0x2f07U;
    check(trace, scrap_load_inputs(copy) == 1 &&
        get_u32(copy->scrap_planet, 28U) == 701UL &&
        get_u32(copy->scrap_planet, 40U) == 219UL &&
        get_u16(copy->scrap_planet, 4U) == 0x2f07U);
    memcpy(copy, trace, sizeof *copy);
    {
        TurnVisibilityContext context;
        StarsVisPFFleet fleet;
        StarsU16 steal = 0U, slot;
        memset(&fleet, 0, sizeof fleet);
        context.trace = copy; context.planet_states = copy->turn_planet_states;
        fleet.player = 1; fleet.ship_counts[0] = 1U;
        check(trace, turn_visibility_scanner(&context, &fleet, &steal) >= 0);
        for (slot = 0U; slot < copy->designs[122]; ++slot)
            if (get_u16(copy->designs, (StarsU16)(58U + slot * 4U)) == 2U)
                put_u16(copy->designs, (StarsU16)(60U + slot * 4U), 0U);
        check(trace, turn_visibility_scanner(&context, &fleet, &steal) == -1);
    }
    memcpy(copy, trace, sizeof *copy);
    copy->turn_visible_planet26_valid = 0;
    copy->turn_visible_planet_mark_calls = 0;
    put_u16(copy->raw_fleets[0], 12U, 3U);
    turn_prepare_visibility_init(copy, 1);
    check(trace, copy->failures == trace->failures &&
        copy->turn_player1_visibility_fleets == copy->complete_input.views[0].fleet_count &&
        copy->turn_player1_visibility_players[1].design_built[0] == 3UL &&
        (copy->turn_player1_visibility_players[1].fleet_count & 0x0fffU) == 2U);
    memcpy(copy, trace, sizeof *copy);
    copy->turn_outgoing_count = 1U;
    turn_mark_message_senders(copy, 0);
    check(trace, copy->failures == trace->failures + 1);
    memcpy(copy, trace, sizeof *copy);
    copy->turn_message_buffer[0] = 0U;
    copy->turn_message_buffer[1] = 7U;
    copy->turn_message_buffer[2] = 0U;
    put_u16(copy->turn_message_buffer, 3U, 10U);
    copy->turn_message_bytes = 5U;
    turn_mark_message_planet_losses(copy, 0);
    check(trace, copy->failures == trace->failures + 1);
    memcpy(copy, trace, sizeof *copy);
    copy->turn_drop_count = 1U; copy->turn_order_pass_calls = 1;
    turn_consume_colonist_drops(copy, 0);
    check(trace, copy->failures == trace->failures + 1);
    *copy = *trace;
    copy->turn_transfer_count = 1U; copy->turn_order_pass_calls = 2;
    turn_consume_transfers(copy, 0);
    check(trace, copy->failures == trace->failures + 1);
    *copy = *trace;
    put_u16(copy->plord[1], 10U, (StarsU16)((get_u16(copy->plord[1], 10U) & 0xfff0U) | 1U));
    turn_visit_remaining_orders(copy, 2);
    check(trace, copy->failures == trace->failures + 1);
    *copy = *trace;
    memset(&battle, 0, sizeof battle); battle.trace = copy;
    for (slot = 0U; slot < copy->complete_input.views[0].fleet_count; ++slot)
        battle.fleets[slot].flags = copy->complete_input.views[0].fleets[slot].state.fleet_flags;
    copy->complete_input.views[0].fleets[0].owner = 0;
    copy->complete_input.views[0].fleets[0].state.planet = 24;
    battle.fleets[0].flags = 7U;
    copy->turn_planet_states[24].flags &= 0xfdffU;
    turn_battle_bombing(&battle);
    check(trace, copy->failures == trace->failures + 1);
    *copy = *trace;
    copy->scout.context = copy;
    put_u16(copy->complete_input.views[0].things[0], 0U,
        (StarsU16)((get_u16(copy->complete_input.views[0].things[0], 0U) & 0x1fffU) | 0x6000U));
    turn_order_thing_interactions(copy, 1);
    check(trace, copy->failures == trace->failures + 1);
    copy->failures = trace->failures;
    memcpy(copy->complete_input.views[0].things[0], trace->complete_input.views[0].things[0], 18U);
    put_u16(copy->raw_fleets[1], 14U, 2U);
    check(trace, supply_fleet_stat(copy, copy->raw_fleets[1], 2U) == 50UL);
    put_u16(copy->raw_fleets[1], 14U, 0U);
    check(trace, supply_fleet_stat(copy, copy->raw_fleets[1], 2U) == 0UL);
    put_u16(copy->raw_fleets[1], 14U, get_u16(trace->raw_fleets[1], 14U));
    put_u16(copy->designs, 147U + 64U, 0x0102U);
    check(trace, supply_fleet_stat(copy, copy->raw_fleets[1], 2U) == 75UL);
    put_u16(copy->designs, 147U + 64U, get_u16(trace->designs, 147U + 64U));
    copy->complete_input.views[0].players[1][56] = 99U;
    copy->complete_input.views[0].players[1][57] = 99U;
    put_u16(copy->raw_fleets[2], 16U, 99U);
    put_u16(copy->raw_fleets[3], 16U, 99U);
    copy->turn_produce_queue[0].quantity = 99;
    copy->turn_produce_queue[1].quantity = 99;
    copy->supply_fleet.minerals[3] = 99UL;
    copy->supply_planet.minerals[3] = 99UL;
    put_u32(copy->raw_fleets[1], 88U, 99UL);
    copy->turn_planet_states[24].minerals[3] = 99UL;
    memset(copy->plord[0] + 4U, 0xa5, 36U);
    memset(copy->plord[2] + 4U, 0xa5, 36U);
    copy->generated_log_size = copy->backup_log_size;
    memcpy(copy->generated_log_file, copy->backup_log, copy->backup_log_size);
    copy->runtime_log_present[1] = 1U;
    copy->startup_log_off = 2U;
    copy->startup_log_flags = 0x0540U;
    /* The closed encrypted file is authoritative after both the live AI log
     * and the previous decoded buffer have been released or changed. */
    memset(copy->startup_merge_inputs, 0xa5, sizeof copy->startup_merge_inputs);
    memset(copy->startup_memory_records, 0xa5, sizeof copy->startup_memory_records);
    copy->startup_memory_count = 0U;
    memset(copy->saved_mutation_stream, 0xa5, sizeof copy->saved_mutation_stream);
    /* Wrong-game and truncated files cannot silently reuse the old log. */
    copy->generated_log_file[6] ^= 1U;
    check(trace, !turn_load_generated_log(copy));
    copy->generated_log_file[6] ^= 1U;
    --copy->generated_log_size;
    check(trace, !turn_load_generated_log(copy));
    ++copy->generated_log_size;
    put_u16(copy->complete_input.game, 14U, 1U);
    check(trace, startup_replay_memory_logs(copy));
    check(trace, get_u16(copy->complete_input.game, 14U) == 0U &&
        copy->log_dirty_calls == trace->log_dirty_calls + 1U &&
        copy->log_title_updates == trace->log_title_updates + 1U);
    check(trace, copy->startup_log_off == 2U && copy->startup_log_flags == 0x0140U);
    check(trace, copy->supply_fleet.minerals[3] == 25UL &&
        get_u32(copy->raw_fleets[1], 88U) == 25UL &&
        copy->supply_planet.minerals[3] == 225UL &&
        copy->turn_planet_states[24].minerals[3] == 225UL);
    for (slot = 0U; slot < copy->complete_input.views[0].fleet_count; ++slot) {
        const Soe3InputFleet *fleet = &copy->complete_input.views[0].fleets[slot];
        if (fleet->owner == 1 && (fleet->id_word & 511U) == 1U)
            check(trace, fleet->state.minerals[3] == 25UL);
    }
    check(trace, copy->plord[0][3] == 2U && copy->raw_fleets[0][98] == 2U &&
        memcmp(copy->plord[0] + 22U, trace->startup_memory_records[7] + 4U, 8U) == 0);
    check(trace, copy->plord[2][3] == 1U && copy->raw_fleets[2][98] == 1U &&
        memcmp(copy->plord[2] + 4U, trace->startup_memory_records[9] + 4U, 8U) == 0);
    for (slot = 8U; slot < 18U; ++slot) {
        check(trace, copy->plord[0][22U + slot] == 0U);
        check(trace, copy->plord[2][4U + slot] == 0U);
    }
    check(trace, get_u16(copy->raw_fleets[2], 16U) ==
        get_u16(trace->startup_merge_inputs[0], 16U) + get_u16(trace->startup_merge_inputs[1], 16U) &&
        get_u16(copy->raw_fleets[3], 16U) == 0U && copy->raw_fleet_present[3] == 0);
    check(trace, copy->turn_committed_queue == copy->turn_produce_queue &&
        copy->turn_committed_queue_count == 2U && copy->turn_produce_queue[0].quantity == 1 &&
        copy->turn_produce_queue[1].quantity == 1 && copy->turn_produce_queue[0].progress == 0);
    check(trace, copy->complete_input.views[0].players[1][56] == trace->startup_memory_records[4][0] &&
        copy->complete_input.views[0].players[1][57] == trace->startup_memory_records[4][1]);
    check(trace, copy->ai_data[0] == 2U && get_u16(copy->ai_data, 2U) == 0U);
    put_u16(copy->ai_data, 2U, 7U);
    startup_validate_history(&copy->scout);
    check(trace, get_u16(copy->ai_data, 2U) == 0U);
    copy->ai_data[0] = 3U;
    put_u16(copy->ai_data, 2U, 7U);
    startup_validate_history(&copy->scout);
    check(trace, get_u16(copy->ai_data, 2U) == 7U);
    copy->ai_data[0] = 2U;
    put_u16(copy->complete_input.views[0].players[1], 6U,
        (StarsU16)((get_u16(copy->complete_input.views[0].players[1], 6U) & 0x1fffU) | 0x8000U));
    startup_validate_history(&copy->scout);
    check(trace, get_u16(copy->ai_data, 2U) == 7U);
    put_u16(copy->complete_input.views[0].players[1], 6U,
        get_u16(trace->complete_input.views[0].players[1], 6U));
    put_u16(copy->ai_data, 2U, 0U);
    startup_read_starbase_state(copy);
    stars_ensure_ai_starbase_designs(0, copy->startup_capabilities,
        copy->startup_starbase_designs, capture_starbase_request, &mask);
    check(trace, mask == 0U);
    for (slot = 1U; slot <= 3U; ++slot)
        put_u16(copy->starbase_designs, (StarsU16)(slot * 147U + 123U),
            (StarsU16)(get_u16(copy->starbase_designs,
                (StarsU16)(slot * 147U + 123U)) | 0x0200U));
    startup_read_starbase_state(copy);
    mask = 0U;
    stars_ensure_ai_starbase_designs(0, copy->startup_capabilities,
        copy->startup_starbase_designs, capture_starbase_request, &mask);
    check(trace, mask == 0x001eU);
    for (slot = 1U; slot <= 3U; ++slot)
        put_u16(copy->starbase_designs, (StarsU16)(slot * 147U + 123U),
            (StarsU16)(get_u16(copy->starbase_designs,
                (StarsU16)(slot * 147U + 123U)) & ~0x0200U));
    startup_read_starbase_state(copy);
    mask = 0U;
    stars_ensure_ai_starbase_designs(0, copy->startup_capabilities,
        copy->startup_starbase_designs, capture_starbase_request, &mask);
    check(trace, mask == 0U);
    point.x = trace->turn_moving_fleets[0].x;
    point.y = trace->turn_moving_fleets[0].y;
    check(trace, ideal_warp(copy, &copy->fleets[0],
        trace->turn_moving_fleets[0].target_id, point) == 5);
    copy->designs[122] = 0U;
    check(trace, ideal_warp(copy, &copy->fleets[0],
        trace->turn_moving_fleets[0].target_id, point) == 0);
    put_u16(copy->designs, 0U, get_u16(copy->designs + 147U, 0U));
    check(trace, fleet_has_teeth(copy, &copy->fleets[0]) == 0);
    check(trace, startup_latest_starbase(&copy->scout) == 0);
    put_u16(copy->starbase_designs, 0x35aU,
        (StarsU16)(get_u16(copy->starbase_designs, 0x35aU) & ~0x0200U));
    put_u16(copy->starbase_designs, 0x35cU,
        (StarsU16)(get_u16(copy->starbase_designs, 0x7dU) + 1U));
    check(trace, startup_latest_starbase(&copy->scout) == 5);
    check(trace, read_retained_m2_shdef_bank(copy, 0));
    check(trace, copy->designs[122] == 0U &&
        startup_latest_starbase(&copy->scout) == 5);
    check(trace, copy->failures == trace->failures);
    free(copy);
}

static void check_current_movement_survival(TurinTrace *trace)
{
    TurinTrace *copy = (TurinTrace *)malloc(sizeof *copy);
    Soe3InputFleet *source;
    StarsMoveFleet fleet;
    StarsI16 expected;
    if (!copy) { ++trace->failures; return; }
    *copy = *trace;
    source = turn_host_fleet(copy, 0, 0);
    if (!source) { ++trace->failures; free(copy); return; }
    expected = (source->state.fleet_flags & 0x0400U) == 0U;
    copy->seeded_action = !copy->seeded_action;
    check(trace, turn_read_hst_owner0_fleet(copy, 0U, &fleet));
    check(trace, fleet.active == expected);
    source->state.fleet_flags ^= 0x0400U;
    check(trace, turn_read_hst_owner0_fleet(copy, 0U, &fleet));
    check(trace, fleet.active == !expected);
    check(trace, copy->failures == trace->failures);
    free(copy);
}

static void check_current_movement_fuel(TurinTrace *trace)
{
    TurinTrace *copy = (TurinTrace *)malloc(sizeof *copy);
    StarsMoveFleetOrder order;
    StarsI32 baseline, heavier;
    StarsU16 mass;
    if (!copy) { ++trace->failures; return; }
    *copy = *trace; memset(&order, 0, sizeof order);
    copy->turn_moving_fleets[0].status = 0;
    order.warp_level = (StarsI16)((get_u16(copy->plord[0], 10U) >> 4) & 15U);
    order.target_kind = 1;
    order.target_id = (StarsI16)get_u16(copy->plord[0], 8U);
    baseline = fuel_use(copy, &copy->turn_moving_fleets[0], &order, trace->fuel_distance);
    check(trace, baseline == 2L);
    check(trace, movement_limit(copy, &copy->turn_moving_fleets[0],
        &order, 100L, 0) == 25L);
    order.warp_level = 3;
    check(trace, movement_limit(copy, &copy->turn_moving_fleets[0],
        &order, 100L, 0) == 9L);
    order.warp_level = 5;
    mass = get_u16(copy->designs, 40U);
    put_u16(copy->designs, 40U, (StarsU16)(mass * 10U));
    heavier = fuel_use(copy, &copy->turn_moving_fleets[0], &order, trace->fuel_distance);
    check(trace, heavier > baseline);
    check(trace, copy->failures == trace->failures);
    free(copy);
}

static void startup_load_runtime(TurinTrace *trace)
{
    StarsI16 point_index, fleet_index;
    StarsU16 axis;
    StarsU8 planet_raw[56];
    StarsTurinFleetPlan local_plan;
    StarsTurinTurnFleet local_fleet;
    StarsTurinTurnPlanet local_planet;
    for (point_index = 0;
            point_index < TURIN_RETAINED_POINT_COUNT; ++point_index) {
        trace->points[point_index].x = trace->complete_input.coordinates[point_index][0];
        trace->points[point_index].y = trace->complete_input.coordinates[point_index][1];
    }
    pack_loaded_planet(planet_raw, &trace->complete_input.views[2].planets[0]);
    if (!stars_turin_planet_from_packed(planet_raw, sizeof planet_raw, &trace->planet)) {
        ++trace->failures; return;
    }
    trace->planet.production_slot_open = (StarsI16)(
        (trace->complete_input.views[2].planets[0].flags & 0x0200U) != 0U &&
        trace->complete_input.views[2].planets[0].minerals[3] >= 200UL);
    for (axis = 0U; axis < 4U; ++axis) {
        trace->supply_planet.minerals[axis] =
            trace->complete_input.views[2].planets[0].minerals[axis];
    }
    for (axis = 0U; axis < 5U; ++axis)
        trace->supply_fleet.minerals[axis] =
            trace->complete_input.views[2].fleets[1].state.minerals[axis];
    trace->supply_fleet.fleet_flags =
        trace->complete_input.views[2].fleets[1].state.fleet_flags;
    for (fleet_index = 0;
            fleet_index < (StarsI16)TURIN_M2_INPUT_FLEET_COUNT;
            ++fleet_index)
        if (!turn_read_m2_fleet(trace, (StarsU16)fleet_index))
            ++trace->failures;
    trace->fleet_count = TURIN_RETAINED_FLEET_COUNT;
    trace->fleet_report.report = 1;
    trace->fleet_report.cached = 1;
    local_fleet = trace->fleets[1];
    local_planet = trace->planet;
    check(trace, stars_turin_plan_local_transport(&local_fleet,
        &local_planet, 1, &local_plan) &&
        local_plan.action == STARS_TURIN_FLEET_ACTION_TRANSPORT &&
        local_plan.reason == 2);
    local_planet.owner = 2;
    check(trace, stars_turin_plan_local_transport(&local_fleet,
        &local_planet, 1, &local_plan) &&
        local_plan.action == STARS_TURIN_FLEET_ACTION_NONE);
    local_planet.owner = 1;
    local_planet.current_minerals[3] = 0L;
    local_fleet.current_cargo[3] = 0L;
    local_fleet.current_cargo[4] = 0L;
    check(trace, stars_turin_plan_local_transport(&local_fleet,
        &local_planet, 1, &local_plan) &&
        local_plan.action == STARS_TURIN_FLEET_ACTION_NONE);
    local_planet.current_minerals[3] = TURIN_LOCAL_TRANSPORT_PLANET_POPULATION;
    local_fleet.owner = 2;
    check(trace, stars_turin_plan_local_transport(&local_fleet,
        &local_planet, 1, &local_plan) &&
        local_plan.action == STARS_TURIN_FLEET_ACTION_NONE);
}

int main(int argc, char **argv)
{
    TurinTrace trace;
    StarsI16 ship_index;
    StarsI16 checks = 0;
    const StarsU8 *second_order;
    StarsU16 flags;
    StarsU16 axis;
    StarsU32 startup_tick;
    StarsU16 submit_saved;
    int seeded_action;
    StarsU32 cargo_before;
    StarsU32 cargo_after;
    if (argc != 2 && argc != 3)
        return 2;
    memset(&trace, 0, sizeof trace);
    seeded_action = 0;
    if (strcmp(argv[1], "control") == 0 ||
            strcmp(argv[1], "action") == 0) {
        trace.seeded_mode = 1;
        seeded_action = strcmp(argv[1], "action") == 0;
        trace.seeded_action = (StarsI16)seeded_action;
        startup_tick = seeded_action ? TURIN_RETAINED_TICK_ACTION :
            TURIN_RETAINED_TICK_CONTROL;
        stars_randomize2(&trace.seeds, startup_tick);
        trace.main_rng_last = trace.seeds;
    } else {
        trace.random_result = (StarsI16)atoi(argv[1]);
        if (trace.random_result < 0 || trace.random_result >= 100)
            return 2;
    }
    trace.complete_input = soe3_inputs[trace.seeded_action ? 1 : 0];
    trace.complete_input_loaded = 1;
    if (trace.complete_input.coordinate_count != TURIN_RETAINED_POINT_COUNT)
        return 3;
    checks += 4;
    memset(&trace.scout, 0, sizeof trace.scout);
    trace.scout.planet_points = trace.points;
    trace.scout.ai_planet_records = trace.ai_planet_records;
    trace.scout.ai_planet_records_size = sizeof trace.ai_planet_records;
    trace.scout.planet_count = TURIN_RETAINED_POINT_COUNT;
    trace.scout.ai_planet_record_stride = 16;
    trace.scout.unknown_status_offset = 15;
    trace.scout.order_capacity = 2;
    trace.scout.status_ready = 1;
    trace.scout.fleet_point = fleet_point;
    trace.scout.fleet_has_teeth = fleet_has_teeth;
    trace.scout.ideal_warp = ideal_warp;
    trace.scout.find_wormhole = find_wormhole;
    trace.scout.random_value = random_value;
    trace.scout.context = &trace;
    memset(&trace.turin_ops, 0, sizeof trace.turin_ops);
    trace.turin_ops.merge_shdefs = merge_shdefs;
    trace.turin_ops.ensure_ai = trace.seeded_mode ? startup_ensure_ai : 0;
    trace.turin_ops.check_design_status = trace.seeded_mode ? startup_check_design_status : 0;
    trace.turin_ops.ensure_shdefs = trace.seeded_mode ? startup_ensure_ship_designs : 0;
    trace.turin_ops.fill_queue = trace.seeded_mode ? startup_final_queue_pass : 0;
    trace.turin_ops.handle_tasks = trace.seeded_mode ?
        startup_handle_tasks : 0;
    trace.turin_ops.planet_desirability = startup_queue_desirability;
    trace.turin_ops.prepare_production = startup_queue_prepare;
    trace.turin_ops.queue_production = startup_queue_add;
    trace.turin_ops.finish_production = startup_queue_finish;
    trace.turin_ops.prepare_fleets = trace.seeded_mode ? startup_first_fleet_pass : 0;
    trace.turin_ops.plan_fleet_action = plan_turin_fleet;
    trace.turin_ops.move = stars_turin_apply_scout_planet;
    trace.turin_ops.transport = transfer_turin_supply;
    trace.turin_ops.visit_fleet_typed = visit_fleet;
    trace.turin_ops.context = &trace.scout;
    memset(&trace.turin_input, 0, sizeof trace.turin_input);
    trace.turin_input.turn = (StarsI16)get_u16(trace.complete_input.game, 18U);
    trace.turin_input.plan_max = get_u16(trace.complete_input.game, 10U);
    trace.turin_input.player_id = 1;
    trace.turin_input.player_tech_level_valid = 1;
    trace.turin_input.player_tech_level = trace.complete_input.views[0].players[1][29U];
    trace.turin_input.planet_count = 1;
    trace.turin_input.planets = &trace.planet;
    trace.turin_input.fleet_count = TURIN_RETAINED_FLEET_COUNT;
    trace.turin_input.fleets = trace.fleets;
    trace.turin_input.ops = &trace.turin_ops;

    memset(&trace.ai_input, 0, sizeof trace.ai_input);
    trace.ai_input.players = trace.players;
    trace.ai_input.player_count = 2;
    trace.ai_input.planet_capacity = TURIN_RETAINED_POINT_COUNT;
    trace.ai_input.load_game = load_game;
    trace.ai_input.destroy_game = startup_destroy_game;
    trace.session.player = -1;
    trace.ai_input.current_player = &trace.session.player;
    trace.ai_input.allocate = allocate;
    trace.ai_input.free_memory = free_memory;
    trace.ai_input.compute_design_powers = startup_compute_powers;
    trace.ai_input.mark_planets_under_attack = startup_mark_attacks;
    trace.ai_input.increase_minefield_sizes = startup_increase_minefields;
    trace.ai_input.init_random_planet_list = startup_init_planet_list;
    trace.ai_input.personalities[1] = run_turin_personality;
    trace.ai_input.write_log = write_log;
    trace.ai_input.write_history = write_history;
    trace.ai_input.context = &trace;
    /* EnsureAis saves submit state, sets gd.grBits bit 4 around DoAiTurn,
     * then restores it before host generation (MDI 56BC). */
    submit_saved = (StarsU16)(trace.game_flags & 0x0010U);
    trace.game_flags |= 0x0010U;
    stars_do_ai_turn(1, 0xffffU, &trace.ai_input);
    check(&trace, trace.session.player == -1 && trace.ai_destroy_calls == 1U &&
        trace.ai_destroy_heaps == 9U && trace.ai_reset_messages == 1U);
    trace.game_flags = (StarsU16)((trace.game_flags & 0xffefU) | submit_saved);
    put_u16(trace.rng_file_data, 0x07caU, trace.game_flags);
    if (trace.seeded_mode) {
        run_generate_turn(&trace);
        check(&trace, trace.turin_input.production_policy_valid == 1 &&
            trace.turin_input.design_1_policy_valid == 1 &&
            trace.turin_input.minelayer_policy_valid == 1 &&
            trace.turin_input.support_policy_valid == 1 &&
            trace.turin_input.production_designs_valid == 1 &&
            trace.turin_input.player_tech_2 <= 4 &&
            trace.turin_input.unowned_viable_count == 0 &&
            trace.turin_input.enemy_viable_count == 0 &&
            trace.turin_input.design_12_eligible == 0 &&
            trace.turin_input.design_selection[2] < 0 &&
            trace.turin_input.design_selection[0] < 0 &&
            trace.turin_input.design_selection[3] < 0 &&
            trace.turin_input.design_selection[5] < 0 &&
            trace.turin_input.design_selection[7] < 0);
        check(&trace, trace.host_destroy_calls == 1U &&
            trace.host_saved_selection_calls == 1U &&
            trace.host_saved_selection_kind != 0U &&
            trace.host_saved_selection_id >= 0 &&
            ((trace.host_saved_ini_flags >> 5) & 15U) ==
                trace.host_saved_selection_kind &&
            trace.host_saved_selection_player == -1 &&
            trace.host_saved_selection_lid == get_u32(trace.complete_input.game, 0U) &&
            trace.session.selected_kind == 0U &&
            trace.session.selected_id == -1 &&
            trace.session.game_lid == get_u32(trace.complete_input.game, 0U) &&
            trace.session.player_count == get_u16(trace.complete_input.game, 8U) &&
            trace.session.planet_count == TURIN_RETAINED_POINT_COUNT);
        check(&trace, trace.main_rng_count == expected_main_rng_count[trace.seeded_action] &&
            trace.seeds.first == trace.main_rng_last.first && trace.seeds.second == trace.main_rng_last.second);
        trace.main_rng_complete = 1U;
    } else {
        trace.scrap_loaded = scrap_load_inputs(&trace);
        check(&trace, trace.scrap_loaded == 1);
        checks += 1;
        if (trace.scrap_loaded)
            check_scrap_loaded_fixture(&trace);
    }
    checks += 3;
    if (!trace.seeded_mode && trace.scrap_loaded)
        trace.scrap_result = run_task5_scrap(&trace);
    check(&trace, trace.scrap_result == 1);
    check(&trace, (get_u16(trace.raw_fleets[2], 4U) & 0x0400U) != 0U);
    check(&trace, (get_u16(trace.plord[2], 10U) & 0x1000U) == 0U);
    check(&trace, trace.scrap_order_done_calls == 1);
    check(&trace, get_u32(trace.scrap_planet, 0x1cU) == 597UL);
    check(&trace, get_u32(trace.scrap_planet, 0x20U) == 956UL);
    check(&trace, get_u32(trace.scrap_planet, 0x24U) == 519UL);
    check(&trace, get_u32(trace.scrap_planet, 0x28U) ==
        (trace.seeded_mode ? 225UL : 250UL));
    check(&trace, trace.scrap_messages == 2 &&
        trace.scrap_message_recipient[0] == 1 &&
        trace.scrap_message_code[0] == 0x5a &&
        trace.scrap_message_recipient[1] == 1 &&
        trace.scrap_message_code[1] == 0x141);
    check(&trace, trace.scrap_message_payload[0][0] == 24 &&
        trace.scrap_message_payload[0][1] == 0x0402 &&
        trace.scrap_message_payload[0][2] == 84);
    check(&trace, trace.scrap_message_payload[1][0] == 24 &&
        trace.scrap_message_payload[1][1] == 2);
    check(&trace, trace.scrap_tech_mark_calls == 1 &&
        trace.scrap_tech_lookup_calls > 0);
    check(&trace, trace.scrap_tech_learn_calls == 1 &&
        trace.scrap_tech_random_calls ==
            (trace.seeded_mode ?
                (seeded_action ? 2 : TURIN_RETAINED_TECH_DRAWS_CONTROL) :
                1) && trace.tech_goto == 0U);
    check(&trace, trace.scrap_remove_calls == 1 && trace.game_flags == 0U);
    checks += 14;
    if (!trace.seeded_mode)
        checks += (StarsI16)(run_retained_population_growth(&trace) + 5);
    check(&trace, trace.load_calls == 1);
    check(&trace, trace.personality_calls == 1);
    check(&trace, trace.log_calls == 1 && trace.history_calls == 1);
    check(&trace, trace.ai_input.ai_active == 0);
    check(&trace, trace.ai_allocation_calls == 3U && trace.ai_release_calls == 3U &&
        trace.ai_live_allocations == 0U);
    check(&trace, trace.random_calls == (trace.seeded_mode ? 4 : 1));
    checks += 3;
    check(&trace, trace.fleet_teeth_calls > 0 && trace.hull_lookup_calls > 0);
    {
        StarsU16 count;
        const StarsHullComponent *components;
        StarsI16 unarmed;
        components = lookup_hull_components(&trace, 1U, 0U, &count);
        check(&trace, count == 3U && stars_hull_has_teeth(components, count) == 1);
        components = lookup_hull_components(&trace, 1U, 1U, &count);
        unarmed = stars_hull_has_teeth(components, count) == 0;
        components = lookup_hull_components(&trace, 1U, 2U, &count);
        check(&trace, unarmed && stars_hull_has_teeth(components, count) == 0);
    }
    checks += 1;
    check(&trace, trace.ideal_engine_lookups == 1U + 2U * trace.basic_speed_calls &&
        trace.ideal_engine_id == 1);
    checks += 1;
    check(&trace, trace.wormhole_calls ==
        (trace.random_values[0] < 5 ? 1 : 0));
    check(&trace, trace.order_warp_calls == 1);
    check(&trace, trace.fleets[0].raw_plord == trace.plord[0]);
    checks += 4;
    check(&trace, trace.merge_calls == 4);
    check(&trace, trace.merge_masks[0] == (StarsI16)-0x1f10 &&
        trace.merge_masks[1] == 0x1000 && trace.merge_masks[2] == 0x0c00 &&
        trace.merge_masks[3] == 0x000c);
    check(&trace, trace.fleets[2].ship_counts[2] == 2 &&
        trace.fleets[3].owner == -1 && trace.production_merge_calls == 1 &&
        trace.production_merge_cargo_calls == 1 &&
        trace.production_merge_balance_lookup_calls == 2 &&
        trace.production_merge_lookup_calls == 2 &&
        trace.production_merge_delete_calls == 1 &&
        trace.production_merge_invalidate_calls == 1 &&
        trace.production_merge_deleted_id == 3 &&
        trace.production_merge_destination_id == 2 &&
        !trace.raw_fleet_present[3] &&
        get_u16(trace.raw_fleets[2], 16U) == 2U &&
        get_u16(trace.raw_fleets[3], 16U) == 0U &&
        get_u32(trace.raw_fleets[2], 92U) == 420UL &&
        get_u32(trace.raw_fleets[3], 92U) == 0UL);
    check(&trace, trace.fleet_count == 3U &&
        get_u16(trace.fleet_items[2]->bytes, 0U) == 2U &&
        (get_u16(trace.fleet_storage[3].bytes, 4U) & 0x0400U) != 0U);
    check(&trace, trace.fleet_report.cached == 0 &&
        (trace.fleet_report_flags & 0x0200U) != 0U);
    for (axis = 0; axis < 5U; ++axis) {
        cargo_before = retained_fleet_cargo[2][axis] +
            retained_fleet_cargo[3][axis];
        cargo_after = get_u32(trace.raw_fleets[2],
            (StarsU16)(76U + axis * 4U)) + get_u32(
            trace.raw_fleets[3], (StarsU16)(76U + axis * 4U));
        check(&trace, cargo_after == cargo_before &&
            get_u32(trace.raw_fleets[3],
                (StarsU16)(76U + axis * 4U)) == 0UL);
    }
    check(&trace, (get_u16(trace.plord[2], 10U) & 0x000fU) == 5U);
    check(&trace, (get_u16(trace.plord[3], 10U) & 0x000fU) == 0U);
    checks += 15;
    check(&trace, trace.transport_plan_calls == 1);
    checks += 1;
    check(&trace, trace.transport_calls == 1);
    check(&trace, trace.supply_calls == 3);
    check(&trace,
        trace.population_summary.supply_population_after_transfer == 225UL);
    check(&trace, trace.supply_fleet.minerals[3] == 25UL);
    check(&trace, trace.supply_cargo_free_calls == 1U &&
        trace.supply_fleet_stat_calls == 1U &&
        trace.supply_fleet_stat_value == 25L &&
        trace.supply_cargo_free_value == 25UL);
    check(&trace, trace.supply_group[0] == 1 && trace.supply_id[0] == 24 &&
        trace.supply_index[0] == 3 && trace.supply_delta[0] == 0L);
    check(&trace, trace.supply_group[1] == 2 && trace.supply_id[1] == 1 &&
        trace.supply_index[1] == 3 && trace.supply_delta[1] == 25L);
    check(&trace, trace.supply_group[2] == 1 && trace.supply_id[2] == 24 &&
        trace.supply_index[2] == 3 && trace.supply_delta[2] == -25L);
    check(&trace, trace.failures == 0);
    checks += 6;
    second_order = trace.plord[0] + STARS_AI_PLORD_HEADER_SIZE +
        (trace.seeded_mode ? 0U : STARS_AI_PLORD_ORDER_SIZE);
    flags = get_u16(second_order, 6U);
    checks += 7;
    check(&trace, trace.plord[0][3] == (trace.seeded_mode ? 1U : 2U));
    check(&trace, get_u16(second_order, 0U) == TURIN_RETAINED_EXPECTED_X);
    check(&trace, get_u16(second_order, 2U) == TURIN_RETAINED_EXPECTED_Y);
    check(&trace, get_u16(second_order, 4U) == TURIN_RETAINED_EXPECTED_TARGET);
    check(&trace, flags == TURIN_RETAINED_EXPECTED_FLAGS);
    check(&trace, trace.seeded_mode || (trace.plord[0][4] == 0x0aU &&
        trace.plord[0][5] == 0x05U));
    check(&trace, trace.seeded_mode || (trace.plord[0][10] == 0x00U &&
        trace.plord[0][11] == 0x11U));

    if (!trace.seeded_mode)
        run_retained_move_fleets(&trace);
    checks += 9;
    check(&trace, trace.turn_moving_fleets[0].x == TURIN_RETAINED_EXPECTED_X &&
        trace.turn_moving_fleets[0].y == TURIN_RETAINED_EXPECTED_Y);
    check(&trace, trace.turn_moving_fleets[0].fuel == TURIN_RETAINED_EXPECTED_FUEL);
    check(&trace, trace.turn_moving_fleets[0].target_id == retained_expected_target);
    check(&trace, trace.move_fuel_calls == 1);
    check(&trace, trace.fuel_distance == TURIN_RETAINED_MOVE_DISTANCE);
    check(&trace, trace.turn_moving_fleets[1].x == retained_points[24].x &&
        trace.turn_moving_fleets[1].y == retained_points[24].y);
    check(&trace, trace.turn_moving_fleets[1].fuel == retained_fleet_cargo[1][4]);
    check(&trace, trace.turn_moving_fleets[1].target_id == 24 &&
        trace.turn_moving_fleets[1].moving == 0);
    check(&trace, trace.failures == 0);
    if (trace.failures == 0)
        if (trace.seeded_mode) {
            const StarsU16 *expected_names = seeded_action ?
                retained_startup_name_ids_action :
                retained_startup_name_ids_control;
            const StarsI16 expected_designs[4] = {18, 20, 17, 19};
            const StarsI16 expected_bitmaps[4] = {137, 138, 128, 129};
            check(&trace, trace.seeded_mode && trace.startup_ensure_calls == 1);
            check(&trace, trace.startup_history_record_count == 4U &&
                trace.startup_history_closed == 1U && trace.startup_design_log_count == 4U &&
                trace.generated_log_closed == 1U && trace.generated_log_loads == 1U &&
                trace.log_dirty_calls == 2U && trace.log_file_version == 0x2a60U &&
                trace.turn_serial_validation_calls == 1U &&
                trace.turn_prepared_designs == 32U && trace.turn_prepared_fleets == 10U &&
                trace.turn_battle_output_calls == 3U && trace.turn_battle_output_blocks == 0U &&
                trace.turn_scanner_refreshes > 0U && trace.turn_cloak_refreshes > 0U &&
                trace.first_pass_visits == 3U &&
                trace.startup_design_status_calls == 7U &&
                trace.startup_ship_maintenance_calls == 1U &&
                trace.backup_moves == 3U && trace.backup_copies == 1U &&
                trace.backup_log_size > 0U && trace.generated_log_size == 0U &&
                trace.runtime_log_present[0] == 0U && trace.runtime_log_present[1] == 0U &&
                get_u32(trace.turn_serials[0], 0U) == 0xffffffffUL &&
                get_u32(trace.turn_serials[1], 0U) == trace.replay_serial &&
                memcmp(trace.turn_serials[1] + 4U, trace.replay_environment, 11U) == 0 &&
                trace.turn_serials[1][15] == 0U &&
                trace.startup_memory_count == 10U && trace.saved_mutation_records == 10U &&
                trace.saved_mutation_size > 0U && trace.startup_memory_applied == 10U &&
                trace.startup_memory_types[8] == 1U && trace.startup_memory_sizes[8] == 7U &&
                memcmp(trace.startup_memory_records[8], expected_cargo_log[seeded_action ? 1 : 0], 7U) == 0 &&
                trace.startup_memory_types[7] == 4U && trace.startup_memory_sizes[7] == 12U &&
                trace.startup_memory_types[9] == 5U && trace.startup_memory_sizes[9] == 12U &&
                memcmp(trace.startup_memory_records[7], expected_order_logs[seeded_action ? 1 : 0][0], 12U) == 0 &&
                memcmp(trace.startup_memory_records[9], expected_order_logs[seeded_action ? 1 : 0][1], 12U) == 0 &&
                trace.startup_memory_types[5] == 23U && trace.startup_memory_sizes[5] == 9U &&
                memcmp(trace.startup_memory_records[5], expected_balance_log[seeded_action ? 1 : 0], 9U) == 0 &&
                trace.startup_memory_types[6] == 29U && trace.startup_memory_sizes[6] == 10U &&
                memcmp(trace.startup_memory_records[6], expected_production_log[seeded_action ? 1 : 0], 10U) == 0 &&
                trace.startup_memory_types[4] == 34U && trace.startup_memory_sizes[4] == 2U &&
                memcmp(trace.startup_memory_records[4], expected_research_log[seeded_action ? 1 : 0], 2U) == 0);
            ++checks;
            for (axis = 0U; axis < 4U; ++axis) {
                StarsU16 run = seeded_action ? 1U : 0U;
                check(&trace, trace.startup_history_types[axis] == expected_history_types[run][axis] &&
                    trace.startup_history_sizes[axis] == expected_history_sizes[run][axis] &&
                    memcmp(trace.startup_history_records[axis], expected_history_bodies[run][axis],
                        expected_history_sizes[run][axis]) == 0);
                check(&trace, expected_design_logs_types[run][axis] == 27U &&
                    trace.startup_design_log_sizes[axis] == expected_design_logs_sizes[run][axis] &&
                    memcmp(trace.startup_design_logs[axis], expected_design_logs_bodies[run][axis],
                        expected_design_logs_sizes[run][axis]) == 0);
                checks += 2;
            }
            check(&trace, trace.startup_owned_planet_count == 1U &&
                trace.startup_owned_planets[0] == 24U &&
                trace.startup_prepare_random_calls == 0U &&
                trace.startup_mine_prepare_calls == 1U &&
                trace.startup_attack_fleets == 4U && trace.startup_power_designs == 3U);
            ++checks;
            check(&trace, trace.startup_history_calls == 1 &&
                trace.startup_research_order == TURIN_RETAINED_RESEARCH_ORDER &&
                trace.startup_latest_design ==
                    TURIN_RETAINED_LATEST_STARBASE_DESIGN);
            check(&trace, trace.startup_research_writes == 1 &&
                trace.startup_written_current_tech ==
                    TURIN_RETAINED_OUTPUT_CURRENT_TECH &&
                trace.startup_written_percent ==
                    TURIN_RETAINED_OUTPUT_PERCENT_RESEARCH &&
                trace.startup_research.current_tech ==
                    TURIN_RETAINED_OUTPUT_CURRENT_TECH &&
                trace.startup_research.percent_research ==
                    TURIN_RETAINED_OUTPUT_PERCENT_RESEARCH);
            check(&trace, trace.startup_name_count == 4 &&
                trace.startup_name_random_calls == 4);
            check(&trace, trace.startup_design_change_count == 4U &&
                (get_u16(trace.complete_input.views[0].players[1], 4U) >> 12) == 5U);
            ++checks;
            for (axis = 0; axis < 4U; ++axis) {
                const StarsU8 *published = trace.starbase_designs +
                    (expected_designs[axis] - 16) * 147U;
                check(&trace, memcmp(published + 8U, trace.startup_design_changes[axis] + 8U, 32U) == 0 &&
                    memcmp(published + 58U, trace.startup_design_changes[axis] + 58U, 64U) == 0 &&
                    get_u16(published, 56U) == expected_startup_damage[seeded_action ? 1 : 0][axis] &&
                    (get_u16(published, 123U) & 0x0200U) == 0U &&
                    get_u16(published, 125U) == 0U && get_u32(published, 127U) == 0UL &&
                    get_u32(published, 131U) == 0UL);
                ++checks;
                check(&trace, memcmp(trace.startup_name_text[axis],
                    expected_startup_names[seeded_action ? 1 : 0][axis],
                    32U) == 0);
                ++checks;
                check(&trace, memcmp(trace.startup_created_components[axis],
                    expected_startup_components[seeded_action ? 1 : 0][axis],
                    64U) == 0);
                ++checks;
                check(&trace, trace.startup_name_ids[axis] ==
                        expected_names[axis] &&
                    trace.startup_name_values[axis] ==
                        (StarsI16)(expected_names[axis] - 0x432U) &&
                    trace.startup_name_limits[axis] == 13 &&
                    trace.startup_name_designs[axis] ==
                        expected_designs[axis] &&
                    trace.startup_name_bitmaps[axis] ==
                        expected_bitmaps[axis]);
            }
            for (ship_index = 0; ship_index < 6; ++ship_index)
                check(&trace, trace.startup_research.tech[ship_index] ==
                    retained_startup_player_technology[ship_index]);
            check(&trace, trace.startup_upgrade_calls == 1 &&
                trace.startup_turn_shuffle_calls == 1 &&
                trace.random_calls == 4 &&
                trace.random_limits[0] == 100 &&
                trace.random_values[0] == (seeded_action ?
                    TURIN_RETAINED_SCOUT_DRAW_ACTION : TURIN_RETAINED_SCOUT_DRAW_CONTROL) &&
                trace.random_limits[1] == 100 &&
                trace.random_values[1] ==
                    (StarsI16)(seeded_action ?
                        TURIN_RETAINED_UPGRADE_DRAW_ACTION :
                        TURIN_RETAINED_UPGRADE_DRAW_CONTROL) &&
                trace.random_limits[2] == 2 &&
                trace.random_limits[3] == 1 &&
                trace.random_values[2] ==
                    (StarsI16)(seeded_action ?
                        TURIN_RETAINED_SHUFFLE_DRAW_ACTION_0 :
                        TURIN_RETAINED_SHUFFLE_DRAW_CONTROL_0) &&
                trace.random_values[3] ==
                    (StarsI16)(seeded_action ?
                        TURIN_RETAINED_SHUFFLE_DRAW_ACTION_1 :
                        TURIN_RETAINED_SHUFFLE_DRAW_CONTROL_1));
            check(&trace, trace.bof_writer_calls == 5 &&
                trace.bof_random_values[0] ==
                    (StarsI16)(seeded_action ?
                        TURIN_RETAINED_BOF_DRAW_ACTION_0 :
                        TURIN_RETAINED_BOF_DRAW_CONTROL_0) &&
                trace.bof_random_values[1] ==
                    (StarsI16)(seeded_action ?
                        TURIN_RETAINED_BOF_DRAW_ACTION_1 :
                        TURIN_RETAINED_BOF_DRAW_CONTROL_1));
            check(&trace, trace.startup_turn_order[0] ==
                    (seeded_action ? 1 : 0) &&
                trace.startup_turn_order[1] ==
                    (seeded_action ? 0 : 1));
            check(&trace, trace.startup_player_tech_calls ==
                (seeded_action ? 1 : 0));
            check(&trace, trace.scrap_tech_random_calls ==
                (seeded_action ? TURIN_RETAINED_TECH_DRAWS_ACTION :
                    TURIN_RETAINED_TECH_DRAWS_CONTROL));
            for (ship_index = 0;
                    ship_index < TURIN_RETAINED_TECH_DRAWS_CONTROL;
                    ++ship_index) {
                if (seeded_action &&
                        ship_index >= TURIN_RETAINED_TECH_DRAWS_ACTION)
                    break;
                check(&trace, trace.tech_random_limits[ship_index] ==
                        (seeded_action ? retained_tech_limits_action[ship_index] :
                            retained_tech_limits_control[ship_index]) &&
                        trace.tech_random_values[ship_index] ==
                            (seeded_action ? retained_tech_values_action[ship_index] :
                                retained_tech_values_control[ship_index]));
            }
            check(&trace, trace.bof_writer_calls == 5);
            check_healing_adapter_writeback(&trace);
            check_fuel_adapter_writeback(&trace);
            check_terraform_gate_adapters(&trace);
            check_current_score_inputs(&trace);
            check_current_production_costs(&trace);
            check_current_sweep_power(&trace);
            check_current_movement_fuel(&trace);
            check_current_movement_survival(&trace);
            check_current_ai_design_inputs(&trace);
            if (trace.failures == 0) {
                printf("SATISFY_ORDERS_TURIN_BINDING_SEEDED=PASS STREAM_DRAWS=%d AI_NAMES=4 BOFS=5 SHUFFLE=PASS TECH=PASS MINING=PASS EVENTS=PASS MINE_SWEEP=PASS WORMHOLES=PASS FUELING=PASS HEALING=PASS TERRAFORM_GATES=PASS SCORES=PASS GUESSES=PASS GUESS_INPUTS=32/32 SAVED_FLAG=PASS OUTPUT_BOFS=3/3 TURN_MARKERS=3/3 PLAYER_PLANETS=5/5 TYPE28_QUEUE=2/2 M1_INPUT_PLANETS=3/3 M2_INPUT_PLANETS=1/1 M2_SHDEF_READS=4/4 LOOKUP_FAILURES=0 INPUT_PLAYER_READS=5/5 M2_INPUT_FLEETS=4/4 M1_INPUT_FLEETS=6/6 HST_INPUT_PLANETS=32/32 ",
                    seeded_action ? 39 : 57);
                printf("M1_FLEETS=%d/%d M1_ORDERS=%d/%d M2_FLEETS=2/2 M2_ORDERS=2/2 HST_FLEETS=%d/%d HST_ORDERS=%d/%d PLANET_RECORDS=32/32 SCOUT=PASS\n",
                    (int)turn_hst_owner0_output_count(&trace),
                    (int)turn_hst_owner0_output_count(&trace),
                    (int)turn_hst_owner0_output_count(&trace),
                    (int)turn_hst_owner0_output_count(&trace),
                    (int)(turn_hst_owner0_output_count(&trace) +
                        TURIN_M2_OUTPUT_FLEET_COUNT),
                    (int)(turn_hst_owner0_output_count(&trace) +
                        TURIN_M2_OUTPUT_FLEET_COUNT),
                    (int)(turn_hst_owner0_output_count(&trace) +
                        TURIN_M2_OUTPUT_FLEET_COUNT),
                    (int)(turn_hst_owner0_output_count(&trace) +
                        TURIN_M2_OUTPUT_FLEET_COUNT));
            }
            else
                printf("SATISFY_ORDERS_TURIN_BINDING_SEEDED=FAIL FAILURES=%d\n",
                    (int)trace.failures);
        } else {
            puts("SATISFY_ORDERS_TURIN_BINDING_CHECKS=81 FAILURES=0 RESULT=PASS");
        }
    else {
        printf("SATISFY_ORDERS_TURIN_BINDING_CHECKS=%d FAILURES=%d RESULT=FAIL\n",
            (int)checks, (int)trace.failures);
    }
    if (trace.failures == 0 && trace.seeded_mode && argc == 3) {
        static const char *suffixes[3] = {"HST", "M1", "M2"};
        StarsU16 file_index;
        for (file_index = 0U; file_index < 3U; ++file_index) {
            char path[1024];
            FILE *stream;
            if (strlen(argv[2]) + 11U >= sizeof path) return 4;
            sprintf(path, "%s/CLEAN.%s", argv[2], suffixes[file_index]);
            stream = fopen(path, "wb");
            if (!stream) return 4;
            if (fwrite(trace.active_files[file_index], 1U,
                    trace.active_file_sizes[file_index], stream) !=
                    trace.active_file_sizes[file_index] || fclose(stream) != 0)
                return 4;
        }
    }
    return trace.failures == 0 ? 0 : 1;
}
