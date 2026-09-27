# Step 5: reviewed semantic core

This report records a **359-routine / 139,741-byte milestone**. For the current
source-review total and per-procedure evidence, use [TODO5](../../TODO5.md):
838 reviewed, 8 partial, and 0 open named procedures. The stricter Phase 2
integration/original-behavior gates are in [TODO7](../../TODO7.md). Step 5
remains in progress. A separate
[CRT audit](runtime-library.json) maps the linked runtime code; other unnamed
code and remaining CRT data still need review. These routines compile together
as a host C89 library and now support a partial native
[Win16 starting-screen and race-workshop application](start2.md). The complete application
and original far-call ABI remain unfinished. Innate race
scoring uses explicit bounded x87 rounding; see the U018 resolution.

Additional routines and their limits are described in [codec](codec.md),
[text utilities](text.md), [game creation](create.md), [race validation](race-validation.md),
[planet habitability](habitability.md), [innate habitability](innate-habitability.md),
[race scoring](race-scoring.md), [game strings](game-strings.md),
[random race creation](random-race.md), [player records](player-records.md),
[file headers and stream handling](file-streams.md), and
[file creation and alerts](file-creation.md), the integrated
[race component](race-component.md), [message formatting](message-formatting.md),
and [memory, startup resources and runtime mapping](foundations.md), plus the
[starting-screen milestone](starting-screen.md) and [START2 race workshop](start2.md).
The table below covers the initial 15 routines.

A complete host-side race-generation example is available with
`python3 tools/race_demo.py --seed 0`. It runs reconstructed C code with static
original data and needs no emulator. Player-record serialization is now available through
`--record-roundtrip --tick 0`, including the reconstructed clock-dependent BOF
header. Stream retry/transfer control flow is also reconstructed with explicit
OS, error-presentation and nonlocal-exit boundaries. Filename generation, file
creation recovery, error classification and dialog/log selection now have
reviewed source as well. Message expansion, logging, passwords, race save/load
and all six wizard pages now execute as an integrated host component. Its
[dependency audit](race-dependencies.json) and [Windows contract](race-window-api.md)
define the OS boundary now connected by START2; its native build and real-dialog
acceptance are recorded separately in the START2 report.

## Source and address mapping

Addresses are NE ordinals and half-open offsets, not runtime selectors.
Recovered declarations, parameter types, and source mappings are in
`analysis/debug/procedures.json`, `analysis/source_lines.json`, and
`reconstructed/include/sfuncs.h`. The `stars_` API names and explicit state
parameters are reconstruction choices. Generated storage headers remain separate.

| Original | NE range | Reviewed source / host entry |
|---|---|---|
| PushRandom | 9:1440–14A2 | near_state.c / stars_push_random |
| PopRandom | 9:14A2–14EA | near_state.c / stars_pop_random |
| Randomize | 9:15EA–165A | utilgen.c / stars_randomize |
| Randomize2 | 9:165A–16D2 | utilgen.c / stars_randomize2 |
| Random | 9:16D2–1889 | utilgen.c / stars_random |
| GetFileSeeds | 9:1A4E–1A7B | utilgen.c / stars_get_file_seeds |
| SetFileSeeds | 9:1A7C–1AA5 | utilgen.c / stars_set_file_seeds |
| SetFileXorStream | 9:1AA6–1B53 | utilgen.c / stars_set_file_xor_stream |
| LGetNextFileXor | 9:1B54–1CC3 | utilgen.c / stars_next_file_xor |
| FBogusLong | 15:484C–48C3 | file.c / stars_file_bogus_long |
| FValidSerialLong | 15:48C4–4A10 | file.c / stars_file_valid_serial_long |
| GetFileStatus | 15:4A60–4AC1 | file.c / stars_get_file_status |
| FNewTurnAvail | 15:4F22–4FB2 | file.c / stars_new_turn_available |
| FCheckFile | 15:4FB2–5167 | file.c / stars_check_file |
| WriteOrders | 15:547E–551B | file.c / stars_write_orders |
| WriteRtString | 15:87B4–885D | file.c / stars_write_runtime_string |
| FAppendFile | 15:704E–709B | file.c / stars_append_file |
| WriteBattlePlan | 15:89B8–8ADB | file.c / stars_write_battle_plan |
| UnpackBattlePlan | 15:40CE–41AB | file.c / stars_unpack_battle_plan |
| AskSaveDialog | 15:432A–43EE | file.c / stars_ask_save_dialog |
| PromptSaveGame | 15:43EE–44AF | file.c / stars_prompt_save_game |
| SetVisiblePlanFleet | 15:95BC–9653 | file.c / stars_set_visible_plan_fleet |
| InitInstance | 1:0D70–0EAA | startup.c / stars_init_instance |
| FFindSomethingAndSelectIt | 5:2E1C–2F79 | ui.c / stars_find_something_and_select_it |
| RestoreSelection | 5:2614–286F | ui.c / stars_restore_selection |
| HostOptionsDialog | 5:75CE–7701 | ui.c / stars_host_options_dialog |
| CFindTurnsOutstanding | 5:6A26–6C15 | ui.c / stars_find_turns_outstanding |
| LogChangeBtlplan | 10:93D0–9420 | file.c / stars_log_change_btlplan |
| LogSplitFleet | 10:8B0E–8B6C | file.c / stars_log_split_fleet |
| SetBuildSelection | 26:53DA–5475 | ui.c / stars_set_build_selection |
| FGetPrevLogRt | 10:A25E–A2D6 | file.c / stars_get_prev_log_rt |
| FRunLogFile | 10:A2D6–A38B | file.c / stars_run_log_file |
| LogMergeFleet | 10:8B6C–8C27 | file.c / stars_log_merge_fleet |
| LogChangeRelations | 10:9340–93D0 | file.c / stars_log_change_relations |
| WriteMemRt | 10:A130–A227 | common.c / stars_write_mem_rt |
| SetPlanetTitleBar | 10:3DEC–3E7A | ui.c / stars_set_planet_title_bar |
| EnumLogRts | 10:D6E0–D783 | file.c / stars_enum_log_rts |
| LogMakeValidXfer | 10:99F6–9FA6 | file.c / stars_log_make_valid_xfer |
| LogChangeThing | 10:9908–99F6 | file.c / stars_log_change_thing |
| LogChangeName | 10:8D5A–8EBE | file.c / stars_log_change_name |
| FGetBestDefensePart | 10:21F6–22CC | planet.c / stars_get_best_defense_part |
| ShowTutor | 20:0374–03BF | common.c / stars_show_tutor |
| EndTutor | 20:0C02–0C8F | common.c / stars_end_tutor |
| FAskKillTutor | 20:0F36–0F93 | common.c / stars_ask_kill_tutor |
| CheckInitiative | 1F:3E68–3EC1 | common.c / stars_check_initiative |
| CMaxMines | 0A:7248–7303 | planet.c / stars_cmax_mines |
| PctPlanetCapacity | 0A:6ACA–6B87 | planet.c / stars_pct_planet_capacity |
| CMaxOperableMines | 0A:7304–73FC | planet.c / stars_cmax_operable_mines |
| CMinesOperating | 0A:73FC–74BE | planet.c / stars_cmines_operating |
| CFactoriesOperating | 0A:74BE–755C | planet.c / stars_cfactories_operating |
| CMaxFactories | 0A:755C–7618 | planet.c / stars_cmax_factories |
| CMaxOperableFactories | 0A:7618–7710 | planet.c / stars_cmax_operable_factories |
| CMaxDefenses | 0A:7710–77AD | planet.c / stars_cmax_defenses |
| CMaxOperableDefenses | 0A:77AE–788E | planet.c / stars_cmax_operable_defenses |
| CResourcesAtPlanet | 0A:788E–7B0F | planet.c / stars_cresources_at_planet |
| IWarpMAFromLppl | 0A:7B10–7CF9 | planet.c / stars_iwarp_ma_from_planet |
| StargateRangeFromLppl | 0A:7CFA–7E99 | planet.c / stars_stargate_range_from_planet |
| FProdIsTerra | 0A:7E9A–7F55 | planet.c / stars_fprod_is_terra |
| IpctCanTerraformLppl | 0A:7F56–8021 | planet.c / stars_ipct_can_terraform |
| UninhabitPlanet | 0A:8732–88BF | planet.c / stars_uninhabit_planet |
| PctCloakFromHuldef | 0A:88C0–8B0D | planet.c / stars_pct_cloak_from_huldef |
| LogMakeValidXferf | 10:9FA6–A107 | file.c / stars_log_make_valid_xferf |
| EnsureCAShdefs | 20:3020–302F | common.c / stars_ensure_cas_hdefs |
| CancelMemRt | 10:A108–A130 | common.c / stars_cancel_mem_rt |
| DirtyGame | 10:A228–A25E | common.c / stars_dirty_game |
| GetFuelFree | 11:6004–6033 | common.c / stars_get_fuel_free |
| GetCargoFree | 11:5F98–6004 | common.c / stars_get_cargo_free |
| ChgCargo | 11:6034–64CC | common.c / stars_change_cargo |
| UpdateXferBtns | 11:66A8–6BEA | common.c / stars_update_xfer_buttons |
| DrawXferDlg | 11:6908–6B45 | common.c / stars_draw_xfer_dlg |
| FFuelTanker | 31:20F6–2129 | common.c / stars_fuel_tanker |
| FSetupXferBtns | 11:6BEA–7087 | common.c / stars_setup_xfer_buttons |
| DrawThingXferSide | 11:7088–72DD | common.c / stars_draw_thing_xfer_side |
| DrawFleetCargoXferSide | 11:72DE–7726 | common.c / stars_draw_fleet_cargo_xfer_side |
| DrawFleetShipsXferSide | 11:7726–79C9 | common.c / stars_draw_fleet_ships_xfer_side |
| DrawPlanetXferSide | 11:79CA–7CDA | common.c / stars_draw_planet_xfer_side |
| LogicalToScan | 12:744E–7490 | common.c / stars_logical_to_scan |
| ScanToLogical | 12:7490–7503 | common.c / stars_scan_to_logical |
| IdmGetMessageN | 7:8412–8444 | common.c / stars_idm_get_message_n |
| FGetNMsgbig | 7:8444–8580 | messages.c / stars_get_message_big |
| PszGetMessageN | 7:8580–85CC | common.c / stars_psz_get_message_n |
| FRemovePlayerMessage | 7:9278–932A | messages.c / stars_remove_player_message |
| FFindPlayerMessage | 7:932A–93C5 | messages.c / stars_find_player_message |
| ResetMessages | 7:98D6–9949 | messages.c / stars_reset_messages |
| MarkPlayersThatSentMsgs | 7:9604–9701 | messages.c / stars_mark_players_that_sent_messages |
| WritePlayerMessages | 7:9702–98D6 | messages.c / stars_write_player_messages |
| MarkPlanetsPlayerLost | 7:93C6–9603 | messages.c / stars_mark_planets_player_lost |
| FEnumCalcMinerDest | 12:5F32–5FC8 | ai.c / stars_enum_calc_miner_dest |
| FEnumCalcColonistDrop | 12:5FC8–60BC | ai.c / stars_enum_calc_colonist_drop |
| FEnumPickUp | 16:3F00–3F5E | ai.c / stars_enum_pick_up |
| DoMaidAiTurn | 14:0000–0063 | ai.c / stars_do_maid_ai_turn |
| FPotentISWarFleet | 14:012E–01CE | ai.c / stars_potent_is_war_fleet |
| IshdefAiSBLatestOF | 13:8458–84BE | ai.c / stars_ishdef_ai_sb_latest_of |
| IshdefAiSBLatest | 13:84BE–8523 | ai.c / stars_ishdef_ai_sb_latest |
| EnableZipProdBtns | 1B:5DF0–5E57 | produce.c / stars_enable_zip_prod_buttons |
| SetScanWp | 0C:8C5A–8CC4 | scan.c / stars_set_scan_waypoint |
| MineMinerals | 18:55A4–560E | turn2.c / stars_mine_minerals |
| UnmarkMineFields | 18:7638–76A3 | turn2.c / stars_unmark_mine_fields |
| DoThingInteractions | 23:0B3A–1A95 | thing.c / stars_do_thing_interactions |
| FakeEditProc | 0B:A700–A76E | ship.c / stars_fake_edit_proc |
| FIsTurinDroneAiAttack | 13:4B9A–4C08 | ai.c / stars_is_turin_drone_ai_attack |
| PtDisplayString | 19:355C–35DC | popup.c / stars_display_string |
| FHullHasTeeth | 1F:2072–20F6 | battle.c / stars_hull_has_teeth |
| DzFromBrcBrc | 1F:4CA8–4D2E | battle.c / stars_dz_from_brc_brc |
| LpshdefFromTok | 1F:388E–391D | battle.c / stars_lpshdef_from_tok |
| DxOfBtn | 0E:0BB2–0C39 | tb.c / stars_dx_of_btn |
| FFleetMightHaveTeeth | 13:6152–61DD | ai.c / stars_fleet_might_have_teeth |
| CreateShip | 18:2FD6–3064 | turn2.c / stars_create_ship |
| FillProductionQueue | 16:2CE2–2D71 | turn2.c / stars_fill_production_queue |
| FMoveToNearestStarbase | 13:6F7E–7014 | ai.c / stars_f_move_to_nearest_starbase |
| FixPlanetsUnderAttack | 13:69E6–6A7E | ai.c / stars_fix_planets_under_attack |
| SetOrdersLbSel | 0B:9354–93EE | ship.c / stars_set_orders_lb_sel |
| FEnumDropOffStage1 | 16:3D00–3DFE | ai.c / stars_f_enum_drop_off_stage1 |
| FEnumDropOffStage2 | 16:3DFE–3EFF | ai.c / stars_f_enum_drop_off_stage2 |
| DxyFromSpdRound | 1F:8B28–8BCB | battle.c / stars_dxy_from_spd_round |
| ItbFromPpt | 0E:0B12–0BB1 | tb.c / stars_itb_from_ppt |
| RegenShield | 1F:3C16–3CB9 | battle.c / stars_regen_shield |
| FCanKillTok | 1F:391E–3A26 | battle.c / stars_f_can_kill_tok |
| FColonizer | 11:227C–2322 | ship.c / stars_f_colonizer |
| FScout | 11:2322–23C8 | ship.c / stars_f_scout |
| FPotentRobWarFleet | 12:31BC–325C | ai.c / stars_potent_rob_war_fleet |
| FFleetInField | 13:1CF6–1D9D | ai.c / stars_f_fleet_in_field |
| FHullHasBombs | 1F:1EC2–1FBE | battle.c / stars_hull_has_bombs |
| FFleetHasBombs | 1F:1E16–1EC2 | battle.c / stars_fleet_has_bombs |
| FFleetHasTeeth | 1F:1FBE–2072 | battle.c / stars_fleet_has_teeth |
| FIsAiAttack | 13:4A72–4B9A | ai.c / stars_f_is_ai_attack |
| FIsAiTransport | 13:4C08–4CEF | ai.c / stars_f_is_ai_transport |
| PtToScan | 0C:0EFC–0FC1 | scan.c / stars_pt_to_scan |
| ScanToPt | 0C:0FC2–1089 | scan.c / stars_scan_to_pt |
| LFetchScoreXVal | 22:2F94–305E | common.c / stars_fetch_score_x_val |
| DrawToolbar | 0E:06F0–078C | tb.c / stars_draw_toolbar |
| EnableVCRButtons | 1E:48F6–49BE | battle.c / stars_enable_vcr_buttons |
| DrawMineralItem | 22:4EBE–4F5F | battle.c / stars_draw_mineral_item |
| IncreaseAIMinefieldSizes | 13:9C66–9D18 | ai.c / stars_increase_ai_minefield_sizes |
| DestroyAllIshdefSB | 0B:C280–C336 | ship.c / stars_destroy_all_ishdef_sb |
| XferAiTroopers | 13:0BC2–0C78 | ai.c / stars_xfer_ai_troopers |
| UlFleetPower | 13:14DE–15A4 | ai.c / stars_ul_fleet_power |
| RandomizeTokOrder | 1F:44D4–45B4 | battle.c / stars_randomize_tok_order |
| FColonizeAiFleet | 13:0C78–0D5B | ai.c / stars_f_colonize_ai_fleet |
| FGotoWormholeAiFleet | 13:0D5C–0E3E | ai.c / stars_f_goto_wormhole_ai_fleet |
| ScoreFromGiveAndTakeAndTactic | 1F:5E34–5F17 | battle.c / stars_score_from_give_take_tactic |
| FillOrdersLB | 0B:928C–9354 | ship.c / stars_fill_orders_lb |
| FillBattleDD | 0B:9A36–9B07 | ship.c / stars_fill_battle_dropdown |
| NthValidShdef | 1A:5C06–5CF4 | ship.c / stars_nth_valid_shdef |
| XferAiSupply | 13:0AD0–0BC1 | ai.c / stars_xfer_ai_supply |
| CreateBackupDir | 18:6DCA–6E9C | turn2.c / stars_create_backup_dir |
| iBuildCyberStarbase | 16:45DE–46AF | ai.c / stars_i_build_cyber_starbase |
| GetResourcesAvailable | 13:56D0–57BF | ai.c / stars_get_resources_available |
| GetProdQCost | 13:57C0–58BE | ai.c / stars_get_prod_q_cost |
| LpflFindClosestEnum | 13:5BAE–5CD4 | ai.c / stars_lpfl_find_closest_enum |
| LpplFindClosestEnum | 13:5CD4–5E06 | ai.c / stars_lppl_find_closest_enum |
| LpplFindBestEnum | 13:5E06–5F54 | ai.c / stars_lppl_find_best_enum |
| MarkPlanetsUnderAttack | 13:6896–69E5 | ai.c / stars_mark_planets_under_attack |
| IdplFindClosestStarbase | 13:6E04–6F7D | ai.c / stars_idpl_find_closest_starbase |
| LpthWormFind | 13:12E6–14DD | ai.c / stars_lpth_worm_find |
| IdNearestUnknownPlanet | 13:15A4–1792 | ai.c / stars_id_nearest_unknown_planet |
| FMoveAiFleet | 13:3D0A–3E50 | ai.c / stars_f_move_ai_fleet |
| AddItemToQueue | 13:3E50–425A | ai.c / stars_add_item_to_queue |
| IroEnsureAi | 13:425A–45C9 | ai.c / stars_iro_ensure_ai |
| KeepFleetsMoving | 13:45CA–476A | ai.c / stars_keep_fleets_moving |
| FShouldPlanetBuildColonizer | 13:9F30–A082 | ai.c / stars_f_should_planet_build_colonizer |
| InitRandomPlanetList | 13:A082–A1C3 | ai.c / stars_init_random_planet_list |
| FGetAIPart | 13:043E–0559 | ai.c / stars_f_get_ai_part |
| SetAiFleetIdealSpeed | 13:1D9E–1FFD | ai.c / stars_set_ai_fleet_ideal_speed |
| FFindBuddyAndJoinUp | 13:9D18–9F2F | ai.c / stars_f_find_buddy_and_join_up |
| FShouldWeBuildColonizers | 13:476A–4A72 | ai.c / stars_f_should_we_build_colonizers |
| MoveToNearestPlanetOrEnemy | 13:7014–7222 | ai.c / stars_move_to_nearest_planet_or_enemy |
| EnsureAiStarbaseDesigns | 13:7222–7688 | ai.c / stars_ensure_ai_starbase_designs |
| CheckAiShdefStatus | 13:9B10–9C65 | ai.c / stars_check_ai_shdef_status |
| FQueueAiDefenses | 13:939A–95A4 | ai.c / stars_f_queue_ai_defenses |
| FQueueAiScanner | 13:90D6–939A | ai.c / stars_f_queue_ai_scanner |
| FQueueAiTerraforming | 13:8D28–90D5 | ai.c / stars_f_queue_ai_terraforming |
| SplitOutShdefs | 13:98D8–9B10 | ai.c / stars_split_out_shdefs |
| IdNearestColonizablePlanet | 13:0E3E–12E5 | ai.c / stars_id_nearest_colonizable_planet |
| FSalvageTargetFreighter2 | 13:395A–3D0A | ai.c / stars_f_salvage_target_freighter |
| MergeAllShdefs | 13:58BE–5BAD | ai.c / stars_merge_all_shdefs |
| FCreateAiStarbase | 13:7BCA–7DCE | ai.c / stars_f_create_ai_starbase |
| FCreateAiShdef | 13:012C–043D | ai.c / stars_f_create_ai_shdef |
| PickANameAndBmp | 13:055A–08A2 | ai.c / stars_pick_ai_name_and_bitmap |
| FChangeAiShdef | 13:08A2–0AD0 | ai.c / stars_f_change_ai_shdef |
| AddMinesToBlockedQueues | 13:1792–1CF5 | ai.c / stars_add_mines_to_blocked_queues |
| IdTargetAttack | 13:1FFE–286C | ai.c / stars_id_target_attack |
| QuickBuildDefenses | 13:6A7E–6E04 | ai.c / stars_quick_build_defenses |
| EnsureMacintiStarbaseDesigns | 13:76E4–7BC9 | ai.c / stars_ensure_macinti_starbase_designs |
| FUpgradeAiStarbase | 13:882A–8D28 | ai.c / stars_f_upgrade_ai_starbase |
| QueueAiStarbases | 13:8524–882A | ai.c / stars_queue_ai_starbase |
| IdTargetScout | 13:61DE–6895 | ai.c / stars_id_target_scout |
| FAIFling | 13:7DD6–8458 | ai.c / stars_f_ai_fling |
| HandleBasicAiTasks | 13:95A4–98D7 | ai.c / stars_handle_basic_ai_tasks |
| IdTargetFreighter | 13:286C–395A | ai.c / stars_id_target_freighter |
| ValidateStarbaseHistory | 13:4CF0–56D0 | ai.c / stars_validate_starbase_history |
| FakeComboProc | 14:1D72–1DF0 | tb.c / stars_fake_combo_proc |
| FakeCEProc | 14:1DF0–1E6D | tb.c / stars_fake_ce_proc |
| FCanSplit | 11:245C–24AE | common.c / stars_can_split |
| FCanSplitAll | 11:24AE–2521 | common.c / stars_can_split_all |
| FCanMerge | 11:2522–263F | common.c / stars_can_merge |
| SetFleetDropDownSel | 11:400E–40F1 | common.c / stars_set_fleet_dropdown_sel |
| WtMaxShdefStat | 11:41B2–44B5 | common.c / stars_wt_max_ship_design_stat |
| LGetFleetStat | 11:40F2–41B1 | common.c / stars_get_fleet_stat |
| FEnumCalcJettison | 11:4DF4–4FAA | common.c / stars_enum_calc_jettison |
| XferSupply | 11:64CC–66A8 | common.c / stars_xfer_supply |
| GetXferLeftRightRcs | 11:6B46–6BEA | common.c / stars_get_xfer_left_right_rects |
| FIsPopupHullType | 25:0148–01BF | common.c / stars_is_popup_hull_type |
| HandleFocusState | 10:60E6–6127 | common.c / stars_handle_focus_state |
| EnableZipBtns | 17:0832–087F | common.c / stars_enable_zip_buttons |
| RandomEvents | 24:3064–3087 | common.c / stars_random_events |
| FEnumOurStarbase | 19:6110–6151 | common.c / stars_enum_our_starbase |
| FEnumCalcEnemyFleets | 18:325C–3286 | common.c / stars_enum_calc_enemy_fleet |
| FEnumCalcEnemyPlanets | 22:45A6–45DD | common.c / stars_enum_calc_enemy_planet |
| IEmptyBmpFromGrhst | 26:6716–6757 | common.c / stars_empty_bmp_from_grhst |
| FCheckResearch | 32:6DA4–6DF4 | common.c / stars_check_research |
| ClearAiCurrentTask | 19:60C0–6110 | common.c / stars_clear_ai_current_task |
| SetNGWTitle | 16:A320–A370 | common.c / stars_set_ngw_title |
| FCheckFleetWP | 32:6DF4–6F86 | common.c / stars_check_fleet_wp |
| FCheckPlanetRoute | 32:6F86–6FF3 | common.c / stars_check_planet_route |
| FCheckLayingWP | 32:6FF4–70C0 | common.c / stars_check_laying_wp |
| FCheckFleetName | 32:690A–69E1 | common.c / stars_check_fleet_name |
| FCheckScanner | 32:685C–690A | common.c / stars_check_scanner |
| TutorError | 32:67AE–685C | common.c / stars_tutor_error |
| FCheckTemplate | 32:666E–67AD | common.c / stars_check_template |
| FCheckSummary | 32:69E2–6AF4 | common.c / stars_check_summary |
| FCheckMessages | 32:6C48–6DA3 | common.c / stars_check_messages |
| FCheckSelection | 32:6AF4–6C47 | common.c / stars_check_selection |
| FCheckZip | 32:6460–6639 | common.c / stars_check_zip |
| FCheckCargo | 32:7664–77D8 | common.c / stars_check_cargo |
| FCheckShipBuilder | 32:7964–79F5 | common.c / stars_check_ship_builder |
| FCheckBuilderPart | 32:77D8–7964 | common.c / stars_check_builder_part |
| FCheckQueue | 32:7442–7609 | common.c / stars_check_queue |
| FCheckXferWP | 32:7280–7442 | common.c / stars_check_xfer_wp |
| FCheckColonizeWP | 32:70C0–71AC | common.c / stars_check_colonize_wp |
| FCheckPatrolWP | 32:71AC–7280 | common.c / stars_check_patrol_wp |
| FCheckBtlPlan | 32:760A–7664 | common.c / stars_check_battle_plan |
| FOKMergeDialog | 32:81DA–82A7 | common.c / stars_ok_merge_dialog |
| CchTutorString | 33:5A14–5B7E | common.c / stars_cch_tutor_string |
| FTutorialEnabledShipBuilder | 32:79F6–81DA | common.c / stars_tutorial_enabled_ship_builder |
| IPlrAlsoCheater | 4:07AA–08D1 | startup.c / stars_player_also_cheater |
| FSetUpBatchProcessing | 4:06A4–07AA | startup.c / stars_set_up_batch_processing |
| FormatSerialAndEnv | 5:2886–2AEB | format.c / stars_format_serial_and_env |
| FSerialAndEnvFromSz | 5:2AEC–2E1C | format.c / stars_serial_and_env_from_string |
| FHandleChar | 4:15DE–165A | startup.c / stars_handle_char |
| OrderInfoDlg | 4:151E–15DD | startup.c / stars_order_info_dialog |
| About | 4:1252–151E | startup.c / stars_about |
| FHandleKey | 4:165A–1C3C | startup.c / stars_handle_key |
| InitMDIApp | 5:0000–038B | mdi.c / stars_init_mdi_app |
| CommandHandler | 5:2F7A–5375 | mdi.c / stars_mdi_command_handler |
| CreateChildWindows | 5:038C–06D6 | child_windows.c / stars_create_child_windows |
| DrawHostDialog2 | 5:6240–6686 | draw_host.c / stars_draw_host_dialog |
| HostTimerProc | 5:7716–79AB | host_timer.c / stars_host_timer_proc |
| HostModeDialog | 5:6C16–75CE | host_mode.c / stars_host_mode_dialog |
| FOpenGame | 5:58F4–5DA7 | open_game.c / stars_open_game |
| FrameWndProc | 5:06D6–1E3C | frame.c / stars_frame_wnd_proc |
| WinMain | 4:0000–06A3 | winmain.c / stars_win_main |
| FCreateStuff | 1:0014–0AB2 | create_stuff.c / stars_create_stuff |
| ReadIniSettings | 1:124C–2B89 | read_ini.c / stars_read_ini_settings |
| WriteIniSettings | 5:7A76–8C87 | write_ini.c / stars_write_ini_settings |
| GetMineFieldCounts | 6:05AC–0659 | mine_simple.c / stars_get_mine_field_counts |
| DrawSelectionArrow | 6:4A68–4B5F | mine_simple.c / stars_draw_selection_arrow |
| DrawDiamond | 6:4B60–4D6E | mine_simple.c / stars_draw_diamond |
| FOtherStuffAtScanSel | 6:4D6E–4ECC | mine_selection.c / stars_other_stuff_at_scan_selection |
| InvalidateMineralBars | 6:040A–05AB | mine_bars.c / stars_invalidate_mineral_bars |
| SetMineralTitleBar | 6:47DC–4A67 | mine_title.c / stars_set_mineral_title_bar |
| IMsgNext / IMsgPrev | 7:7808–799C | messages.c / stars_message_next, stars_message_previous |
| PackageUpMsg | 7:802A–8239 | messages.c / stars_package_player_message |
| FSendPlrMsg | 7:7EE8–7F80 | messages.c / stars_send_player_message |
| FSendPlrMsg2 | 7:7EAA–7EE7 | messages.c / stars_send_player_message2 |
| FSendPrependedPlrMsg | 7:7F80–802A | messages.c / stars_send_player_message_prepend |
| FSendPlrMsg2XGen | 7:823A–8411 | messages.c / stars_send_player_message2_xgen |
| TitleWndProc | 5:9126–9840 | title_wnd.c / stars_title_wnd_proc; shared helpers remain in title.c |
| MarkFleet | 15:885E–89B7 | common.c / stars_mark_fleet |
| MarkPlanet | 15:8ADC–8CFD | common.c / stars_mark_planet |
| CTurnsOutSafe | 5:6996–6A25 | ui.c / stars_turns_out_safe |
| DrawHostOptions | 5:7706–7715 | ui.c / stars_draw_host_options |
| XorFileBuf | 9:1CC4–1D74 | near_state.c / stars_xor_file_buffer |
| ICompLong | 9:1D74–1D96 | utilgen.c / stars_compare_long |
| ExpandRc | 9:2F0C–2F3E | utilgen.c / stars_expand_rect |
| OffsetRc | 9:2F3E–2F70 | utilgen.c / stars_offset_rect |
| LDistance2 | 9:685C–68BA | utilgen.c / stars_distance_squared |
| GetRaceStat | 29:30D2–30F4 | race.c / stars_get_race_stat |

All listed routines use far C calls, with arguments starting at entry SP+4 and
caller cleanup. Returns use AX (16-bit) or DX:AX (32-bit); void routines have no
defined return. Original near pointers address DGROUP. The host adapters use
native pointers, explicit state, or a complete 65,536-byte DGROUP image.

## Behavioral findings

* Random uses two signed 32-bit seed recurrences with truncation toward zero.
  It updates both seeds before testing its signed 16-bit bound. A zero or
  negative bound returns zero but still changes state. Nonpositive seed
  difference gets 2,147,483,562 added before the unsigned remainder operation.
* File encoding uses the same recurrences but returns the uncorrected seed
  difference. It must not reuse Random's corrected output. The seed table is
  the 128 original words in NE 9:14EA–15EA, recovered into `seed_table.inc`.
* Randomize uses two six-bit indices; Randomize2 uses seven-bit indices with
  different XOR masks. Equal indices are separated. Ghidra's apparent global
  pointer reference was resolved against the actual CS-relative table load.
* File-stream setup selects table halves with salt bit 10. Its warmup count
  wraps to 16 bits and is tested as signed. GetFileSeeds preserves assignment
  order so aliased outputs retain the original behavior.
* PushRandom and PopRandom do not check their index. The source preserves
  index reloading between operations and writes through an explicit memory
  image, including aliasing with the seed/index globals. Their positions are
  DGROUP:1370/1374 (seeds), 1378 (index), and 5238 (saved pairs).
* XorFileBuf compares unsigned near pointers against a wrapping end pointer.
  Replacing this with a normal count loop changes behavior. Full words use
  one XOR value each; the tail consumes its low bytes. File seeds are at
  DGROUP:2272/2276. Seed updates precede buffer writes, including overlap.
* ICompLong returns the signed low word of a long subtraction. It is not a
  normalized ordering comparison: 0 and 65,536 compare equal.
* Rectangle operations wrap each coordinate to 16 bits. LDistance2 subtracts
  coordinates in 16 bits before sign extension and multiplication; widening
  the subtraction first would change large-distance results.
* GetRaceStat adds player offset, index, and 62 with near-pointer wrap, then
  sign-extends the selected byte. It does not range-check the index.

## Validation

`tests/test_core_machine.py` builds the C source with strict C89 warnings as
errors and optimization enabled, then compares it with original x86 execution.
`tools/core_machine.py` verifies the original executable's SHA-256, loads only
the selected original NE segments, and applies their real relocation records
to isolated segment addresses. Original compiler arithmetic helpers execute
unchanged. In the base harness, imports and unavailable segments are unmapped
and fail on use; there are no substitute Windows calls. Far return, stack balance, and SI/DI
preservation are checked on every invocation.

The ten differential test groups cover all 4,096/16,384 effective seed inputs;
281 seed pairs with eight bounds and file-XOR comparisons; all 2,048 effective
salts with varying game/player/turn values and signed warmup boundaries; all
8,192 effective RNG-stack positions, memory aliasing and empty-pop underflow;
625 distance combinations; 774 signed-byte/near-wrap race reads; file-seed
transfers and aliased outputs; comparison truncation; rectangle extremes;
and file-buffer tails, seed overlap, negative lengths, and end-pointer wrap.
Random test generation has a fixed seed. Full synthetic DGROUP images are
compared for routines whose writes can alias their own state.

This is real-mode CPU execution of selected integer code, not an emulation of
Windows protected-mode selector limits. Host pointers must denote valid objects;
the near-memory APIs require complete DGROUP storage. XorFileBuf word accesses
must stay inside the segment. Original faults from invalid selectors, segment
crossings, stack/code aliasing, or unbounded corrupt-memory execution are not
certified by these tests. A reconstructed Windows adapter and integration tests
remain necessary. Original runtime helpers used as the oracle are **not** counted
as reconstructed C source. The separate [library audit](runtime-library.json)
now supplies implementation mappings for all 101 code-bearing CRT records;
its data/provenance limits are recorded in [foundations](foundations.md).

The step 4 GetRaceStat trace independently anchors that routine in the live
Windows runtime. A new attempt to rewind a live Random call did not reach its
expected return breakpoint and left an application protection fault reported at
0009:175D. This debugger-induced failure is not accepted as behavioral evidence.
Its temporary breakpoints were removed and the failed process was closed.
The isolated comparisons above are the accepted new evidence; original files
were not patched. A fresh copy of the hash-verified clean2401 fixture is used
when reopening the game after this experiment.

## Reproduce

From the repository root, with the retained original executable present:

```sh
python3 -m pip install --target .tools/python-validation -r requirements-validation.txt
python3 tools/build_core.py
python3 -m unittest tests.test_core_machine -v
python3 tools/baseline.py --check
```

Unicorn is pinned to 2.1.4. On macOS its JIT needs execution permission outside
the restricted process sandbox. Output is `build/libstars_core.dylib` on macOS
and `build/libstars_core.so` on other supported hosts. The initial native
VC++ 1.52c [compatibility checks](../compat/vc152-return2.md) passed for their
selected source subset. The race component subsequently compiled and linked
with VC++ 1.52c and passed all 816 native checks in the
[RETURN4 replay](../compat/racevc-return4.md), with exact file/state matches for
the 88-case catalog. [RETURN5](../compat/racevc-return5.md) also passes all 816
checks in Win98 SE, with native progress/keyboard cancellation checked in
WfW 3.11. Its race-wizard Windows services remain modeled; a real wizard UI
adapter is still required.
