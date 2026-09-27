import ctypes as C
import math
import struct
import subprocess
import unittest
from pathlib import Path


ROOT = Path(__file__).resolve().parents[1]
PTR = C.POINTER(C.c_ubyte)


class WindowCall(C.Structure):
    _fields_ = [('operation', C.c_char_p), ('window', C.c_uint16),
                ('a', C.c_uint16), ('b', C.c_uint16), ('value', C.c_uint32),
                ('p', C.c_void_p), ('q', C.c_void_p)]


class Platform(C.Structure):
    _fields_ = [(name, C.c_void_p) for name in (
        'open', 'close', 'read', 'write', 'ticks', 'memory', 'access',
        'seek', 'date', 'time', 'timer_count', 'window', 'dialog',
        'save_choice', 'abort')]


class StreamApi(C.Structure):
    _fields_ = [(name, C.c_void_p) for name in (
        'open', 'close', 'read', 'write', 'ticks', 'memory', 'file_error',
        'write_error', 'abort')]


class AlertApi(C.Structure):
    _fields_ = [(name, C.c_void_p) for name in
                ('focus', 'message', 'output', 'format_id')]


class AboutOps(C.Structure):
    _fields_ = [(name, C.c_void_p) for name in (
        'get_dialog_item', 'version', 'set_window_text', 'set_timer',
        'kill_timer', 'show_order', 'end_dialog', 'get_client_rect',
        'get_dc', 'select_object', 'set_bk_mode', 'set_bk_color',
        'set_text_color', 'intersect_clip', 'tutorial_string',
                'draw_centered', 'offset_rect', 'fill_rect', 'select_clip',
                'release_dc')]


class HandleKeyOps(C.Structure):
    _fields_ = [(name, C.c_void_p) for name in (
        'get_active_window', 'destroy_window', 'send_message', 'get_focus',
        'get_parent', 'terminate_toolbar_focus', 'get_cursor_pos',
        'window_from_point', 'get_dialog_item', 'get_key_state',
        'invalidate_rect', 'is_button_down', 'execute_button',
        'delete_current_waypoint', 'lookup_fleet', 'draw_plan_ship',
        'execute_report_click')]


class BatchOps(C.Structure):
    _fields_ = [(name, C.c_void_p) for name in
                ('open', 'file_length', 'allocate', 'memory', 'read', 'close')]


class InitInstanceOps(C.Structure):
    _fields_ = [(name, C.c_void_p) for name in
                ('read_ini_settings', 'create_window', 'load_accelerator',
                 'show_window')]


class FindSomethingOps(C.Structure):
    _fields_ = [(name, C.c_void_p) for name in
                ('planet_from_id', 'memory', 'select_planet', 'select_fleet')]


class RestoreSelectionOps(C.Structure):
    _fields_ = [(name, C.c_void_p) for name in (
        'fleet_from_id', 'planet_from_id', 'memory', 'select_fleet',
        'select_planet', 'next_message', 'set_message_title', 'invalidate',
        'find_selection', 'advance_tutor')]


class HostOptionsOps(C.Structure):
    _fields_ = [(name, C.c_void_p) for name in (
        'begin_paint', 'end_paint', 'draw_options', 'get_client_rect',
        'fill_rect', 'set_bk_color', 'end_dialog', 'win_help')]


class FindTurnsOps(C.Structure):
    _fields_ = [('check_log_file', C.c_void_p), ('ticks', C.c_void_p)]


class LogChangeBtlplanOps(C.Structure):
    _fields_ = [('write_battle_plan', C.c_void_p), ('advance_tutor', C.c_void_p)]


class LogSplitFleetOps(C.Structure):
    _fields_ = [('write_memory', C.c_void_p), ('find_fleet', C.c_void_p)]


class LogChangeRelationsOps(C.Structure):
    _fields_ = [('write_memory', C.c_void_p), ('advance_tutor', C.c_void_p)]


class LogXferf(C.Structure):
    _fields_ = [('id', C.c_uint16), ('grobj', C.c_uint16),
                ('item', C.c_uint16 * 16)]


class LogXfer(C.Structure):
    _fields_ = [('id', C.c_uint16), ('grobj', C.c_uint16),
                ('item', C.c_uint32 * 5)]


class Thing(C.Structure):
    _fields_ = [('raw', C.c_ubyte * 18)]


class FleetNameRecord(C.Structure):
    _fields_ = [('name', C.c_void_p)]


class LogChangeNameOps(C.Structure):
    _fields_ = [('find_fleet', C.c_void_p), ('allocate_name', C.c_void_p),
                ('free_name', C.c_void_p), ('compress_name', C.c_void_p),
                ('write_memory', C.c_void_p), ('advance_tutor', C.c_void_p)]


class EndTutorOps(C.Structure):
    _fields_ = [('destroy_window', C.c_void_p), ('restore_game', C.c_void_p),
                ('get_tick', C.c_void_p), ('randomize2', C.c_void_p)]


class CheckInitiativeOps(C.Structure):
    _fields_ = [('lookup_hull', C.c_void_p), ('set_player', C.c_void_p),
                ('init_hull', C.c_void_p)]


class CMaxMinesOps(C.Structure):
    _fields_ = [('calc_max_population', C.c_void_p),
                ('get_player_stat', C.c_void_p)]


class CMaxOperableMinesOps(C.Structure):
    _fields_ = [('calc_max_population', C.c_void_p),
                ('get_player_stat', C.c_void_p),
                ('change_population', C.c_void_p)]


class CMaxOperableFactoriesOps(C.Structure):
    _fields_ = [('calc_max_population', C.c_void_p),
                ('get_player_stat', C.c_void_p),
                ('change_population', C.c_void_p)]


class CMaxDefensesOps(C.Structure):
    _fields_ = [('planet_desirability', C.c_void_p),
                ('get_player_stat', C.c_void_p)]


class CMaxOperableDefensesOps(C.Structure):
    _fields_ = [('planet_desirability', C.c_void_p),
                ('get_player_stat', C.c_void_p),
                ('change_population', C.c_void_p)]


class UninhabitPlanetOps(C.Structure):
    _fields_ = [('get_player_stat', C.c_void_p),
                ('free_production', C.c_void_p)]


class CloakSection(C.Structure):
    _fields_ = [('group', C.c_uint16), ('flags', C.c_uint16)]


class MergeFleet(C.Structure):
    _fields_ = [('player', C.c_int16), ('x', C.c_int16), ('y', C.c_int16),
                ('ship_counts', C.c_uint16 * 16), ('present', C.c_int16)]


class DropDownFleet(C.Structure):
    _fields_ = [('id', C.c_uint16), ('x', C.c_int16), ('y', C.c_int16)]


class SetFleetDropDownOps(C.Structure):
    _fields_ = [('send_message', C.c_void_p), ('draw_plan_ship', C.c_void_p)]


class ShipComponentStat(C.Structure):
    _fields_ = [('group', C.c_uint16), ('packed_count_type', C.c_uint16)]


class HullStat(C.Structure):
    _fields_ = [('fuel_max', C.c_uint16), ('cargo_max', C.c_uint16)]


class ShipDesignStat(C.Structure):
    _fields_ = [('component_count', C.c_uint16),
                ('components', C.POINTER(ShipComponentStat))]


class FleetStatEntry(C.Structure):
    _fields_ = [('ship_count', C.c_int16),
                ('design', C.POINTER(ShipDesignStat)),
                ('hull', C.POINTER(HullStat))]


class JettisonRecord(C.Structure):
    _fields_ = [('fleet_id', C.c_uint16), ('flags', C.c_ubyte),
                ('mineral_mask', C.c_ubyte),
                ('byte_amounts', C.c_int16 * 5),
                ('long_amounts', C.c_int32 * 5)]


class TransferEndpoint(C.Structure):
    _fields_ = [('group', C.c_int16), ('id', C.c_uint16),
                ('object', C.c_void_p)]


class CargoState(C.Structure):
    _fields_ = [('minerals', C.c_uint32 * 5),
                ('cargo', C.c_uint16 * 3),
                ('thing_capacity', C.c_uint16),
                ('fleet_flags', C.c_uint16), ('owner', C.c_uint16),
                ('ship_counts', C.c_uint16 * 16)]


class ChgCargoOps(C.Structure):
    _fields_ = [('lookup', C.c_void_p), ('store', C.c_void_p),
                ('cargo_free', C.c_void_p), ('fuel_free', C.c_void_p)]


class TransferButton(C.Structure):
    _fields_ = [('supply', C.c_int16), ('flags', C.c_uint16),
                ('button_state', C.c_uint16),
                ('rect', C.c_uint16 * 4)]


class TransferSide(C.Structure):
    _fields_ = [('group', C.c_int16),
                ('state', C.POINTER(CargoState))]


class DrawXferOps(C.Structure):
    _fields_ = [('acquire_dc', C.c_void_p), ('release_dc', C.c_void_p),
                ('paint_transfer', C.c_void_p), ('draw_button', C.c_void_p),
                ('draw_ships', C.c_void_p), ('draw_cargo', C.c_void_p),
                ('draw_planet', C.c_void_p), ('draw_thing', C.c_void_p)]


class DrawThingXferOps(C.Structure):
    _fields_ = [(name, C.c_void_p) for name in (
        'thing_name', 'label', 'set_text_color', 'set_background_color',
        'select_font', 'draw_frame', 'centered_text', 'right_text',
        'draw_gauge')]


class DrawFleetCargoXferOps(C.Structure):
    _fields_ = [(name, C.c_void_p) for name in (
        'fleet_name', 'label', 'set_text_color', 'set_background_color',
        'select_font', 'draw_frame', 'centered_text', 'right_text',
        'draw_gauge', 'draw_mineral')]


class DrawFleetShipsXferOps(C.Structure):
    _fields_ = [(name, C.c_void_p) for name in (
        'fleet_name', 'ship_label', 'set_text_color', 'set_background_color',
        'select_font', 'draw_frame', 'centered_text', 'right_text',
        'format_count')]


class DrawPlanetXferOps(C.Structure):
    _fields_ = [(name, C.c_void_p) for name in (
        'planet_name', 'unknown_name', 'label', 'format_value',
        'set_text_color', 'set_background_color', 'select_font', 'draw_frame',
        'centered_text', 'right_text', 'draw_value')]


class CResourcesAtPlanetOps(C.Structure):
    _fields_ = [('calc_max_population', C.c_void_p),
                ('planet_desirability', C.c_void_p),
                ('get_player_stat', C.c_void_p),
                ('get_player_byte', C.c_void_p),
                ('factory_calc_max_population', C.c_void_p),
                ('factory_get_player_stat', C.c_void_p),
                ('factory_change_population', C.c_void_p)]


class WarpMAItem(C.Structure):
    _fields_ = [('identifier', C.c_uint32), ('flags', C.c_uint16)]


class WriteMemRtOps(C.Structure):
    _fields_ = [('set_message_title', C.c_void_p), ('log_overflow', C.c_void_p)]


class BuildSelectionOps(C.Structure):
    _fields_ = [('get_client_rect', C.c_void_p), ('draw_slot', C.c_void_p),
                ('draw_component', C.c_void_p)]


class PlanetTitleOps(C.Structure):
    _fields_ = [('planet_name', C.c_void_p), ('fleet_name', C.c_void_p),
                ('string', C.c_void_p), ('set_window_text', C.c_void_p)]


class Rect(C.Structure):
    _fields_ = [('left', C.c_uint16), ('top', C.c_uint16),
                ('right', C.c_uint16), ('bottom', C.c_uint16)]


class PromptSaveOps(C.Structure):
    _fields_ = [(name, C.c_void_p) for name in
                ('make_proc', 'dialog_box', 'free_proc', 'write_log',
                 'write_history')]


class VisiblePlanFleetOps(C.Structure):
    _fields_ = [(name, C.c_void_p) for name in
                ('initialize', 'fleets', 'planets', 'things', 'finish',
                 'progress')]


class Runtime(C.Structure):
    _fields_ = [('data', PTR), ('strings', PTR), ('platform', C.c_void_p),
                ('context', C.c_void_p), ('stream', StreamApi),
                ('alerts', AlertApi), ('scratch_bottom', C.c_uint16),
                ('scratch_top', C.c_uint16), ('recovery', C.c_void_p),
                ('segments', PTR * 38)]


class ProgressState(C.Structure):
    _fields_ = [('window', C.c_uint16), ('percent', C.c_int16)]


class CommonSourceTests(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        subprocess.run(['python3', 'tools/build_core.py'], cwd=ROOT, check=True)
        cls.lib = C.CDLL(str(ROOT / 'build/libstars_core.dylib'))
        cls.lib.stars_get_true_hull_cost.argtypes = [PTR, C.POINTER(C.c_uint16)]
        cls.lib.stars_long_from_serial_char.restype = C.c_int32
        cls.lib.stars_long_from_serial_char.argtypes = [C.c_int16]
        cls.lib.stars_get_distance.restype = C.c_double
        cls.lib.stars_get_distance.argtypes = [C.c_int16, C.c_int16,
                                               C.c_int16, C.c_int16]
        for name in ('stars_compare_fleet_point', 'stars_compare_fleet_point2'):
            fn = getattr(cls.lib, name)
            fn.restype = C.c_int16
            fn.argtypes = [PTR, PTR]
        cls.lib.stars_thing_index_from_id.restype = C.c_int16
        cls.lib.stars_thing_index_from_id.argtypes = [PTR, C.c_uint16,
                                                       C.c_uint16, C.c_int16]
        cls.lib.stars_planet_index_from_id.restype = C.c_int16
        cls.lib.stars_planet_index_from_id.argtypes = [PTR, C.c_uint16,
                                                       C.c_uint16, C.c_uint16,
                                                       C.c_int16]
        cls.lib.stars_fleet_pointer_index.restype = C.c_int16
        cls.lib.stars_fleet_pointer_index.argtypes = [C.POINTER(C.c_uint32),
                                                      C.c_uint16, C.c_uint32]
        cls.lib.stars_clear_file.restype = C.c_int16
        cls.lib.stars_clear_file.argtypes = [PTR, PTR,
                                             C.POINTER(PTR), C.c_uint16,
                                             C.c_int16, PTR, C.c_uint16,
                                             C.c_void_p, C.c_void_p]
        cls.lib.stars_file_bogus_long.restype = C.c_int16
        cls.lib.stars_file_bogus_long.argtypes = [PTR, C.c_uint32]
        cls.lib.stars_file_valid_serial_long.restype = C.c_int16
        cls.lib.stars_file_valid_serial_long.argtypes = [PTR, C.c_uint32]
        cls.lib.stars_player_also_cheater.restype = C.c_int16
        cls.lib.stars_player_also_cheater.argtypes = [
            PTR, PTR, C.c_int16, C.c_int16, C.c_void_p, C.c_void_p]
        cls.lib.stars_handle_char.restype = C.c_int16
        cls.lib.stars_handle_char.argtypes = [
            C.c_uint16, C.c_uint16, C.c_uint16, C.c_uint16, C.c_uint16,
            C.c_uint32, C.c_void_p, C.c_void_p, C.c_void_p]
        cls.lib.stars_order_info_dialog.restype = C.c_int16
        cls.lib.stars_order_info_dialog.argtypes = [
            C.c_uint16, C.c_uint16, C.c_uint16, C.c_uint32, C.c_uint16,
            C.c_uint32, C.c_void_p, C.c_void_p, C.c_void_p, C.c_void_p,
            C.c_void_p]
        cls.lib.stars_about.restype = C.c_int16
        cls.lib.stars_about.argtypes = [
            PTR, C.c_uint16, C.c_uint16, C.c_uint16, C.c_uint32,
            C.POINTER(AboutOps), C.c_void_p]
        cls.lib.stars_handle_key.restype = C.c_int16
        cls.lib.stars_handle_key.argtypes = [
            PTR, C.c_uint16, C.c_uint16, C.c_uint16, C.c_uint32,
            C.POINTER(HandleKeyOps), C.c_void_p]
        cls.lib.stars_set_up_batch_processing.restype = C.c_int16
        cls.lib.stars_set_up_batch_processing.argtypes = [
            PTR, C.POINTER(BatchOps), C.c_void_p]
        cls.lib.stars_init_instance.restype = C.c_int16
        cls.lib.stars_init_instance.argtypes = [
            PTR, C.c_int16, C.POINTER(InitInstanceOps), C.c_void_p]
        cls.lib.stars_find_something_and_select_it.restype = C.c_int16
        cls.lib.stars_find_something_and_select_it.argtypes = [
            PTR, C.POINTER(FindSomethingOps), C.c_void_p]
        cls.lib.stars_restore_selection.argtypes = [
            PTR, C.POINTER(RestoreSelectionOps), C.c_void_p]
        cls.lib.stars_host_options_dialog.restype = C.c_int16
        cls.lib.stars_host_options_dialog.argtypes = [
            PTR, C.c_uint16, C.c_uint16, C.c_uint16, C.c_uint32,
            C.POINTER(HostOptionsOps), C.c_void_p]
        cls.lib.stars_find_turns_outstanding.restype = C.c_int16
        cls.lib.stars_find_turns_outstanding.argtypes = [
            PTR, C.POINTER(FindTurnsOps), C.c_void_p]
        cls.lib.stars_log_change_btlplan.argtypes = [
            PTR, PTR, C.POINTER(LogChangeBtlplanOps), C.c_void_p]
        cls.lib.stars_log_split_fleet.argtypes = [
            PTR, C.c_int16, C.POINTER(LogSplitFleetOps), C.c_void_p]
        cls.lib.stars_set_build_selection.argtypes = [
            PTR, C.c_int16, C.POINTER(BuildSelectionOps), C.c_void_p]
        cls.lib.stars_set_planet_title_bar.argtypes = [
            PTR, C.c_uint16, C.POINTER(PlanetTitleOps), C.c_void_p]
        cls.lib.stars_get_prev_log_rt.restype = C.c_int16
        cls.lib.stars_get_prev_log_rt.argtypes = [PTR, PTR, PTR, PTR]
        cls.lib.stars_run_log_file.restype = C.c_int16
        cls.lib.stars_run_log_file.argtypes = [PTR, PTR, C.c_void_p, C.c_void_p]
        cls.lib.stars_enum_log_rts.argtypes = [
            PTR, PTR, C.c_void_p, C.c_void_p, C.c_void_p]
        cls.lib.stars_log_merge_fleet.argtypes = [
            PTR, C.c_int16, C.c_void_p, C.c_void_p]
        cls.lib.stars_log_change_relations.argtypes = [
            PTR, PTR, PTR, C.POINTER(LogChangeRelationsOps), C.c_void_p]
        cls.lib.stars_log_make_valid_xferf.argtypes = [
            C.POINTER(LogXferf), C.POINTER(LogXferf), C.c_void_p, C.c_void_p]
        cls.lib.stars_log_make_valid_xfer.argtypes = [
            PTR, PTR, C.POINTER(LogXfer), C.POINTER(LogXfer),
            C.POINTER(WriteMemRtOps), C.c_void_p]
        cls.lib.stars_log_change_thing.argtypes = [
            PTR, PTR, C.POINTER(C.c_uint16), C.POINTER(LogXfer),
            C.POINTER(Thing), C.POINTER(Thing), C.POINTER(WriteMemRtOps),
            C.c_void_p]
        cls.lib.stars_log_change_name.argtypes = [
            C.c_int16, C.c_int16, PTR, C.c_uint16,
            C.POINTER(LogChangeNameOps), C.c_void_p]
        cls.lib.stars_get_best_defense_part.restype = C.c_int16
        cls.lib.stars_get_best_defense_part.argtypes = [PTR, C.c_void_p,
                                                        C.c_void_p]
        cls.lib.stars_end_tutor.argtypes = [PTR, PTR, C.c_int16,
                                            C.POINTER(EndTutorOps), C.c_void_p]
        cls.lib.stars_ask_kill_tutor.restype = C.c_int16
        cls.lib.stars_ask_kill_tutor.argtypes = [C.c_uint16, PTR, C.c_void_p,
                                                 C.c_void_p, C.c_void_p]
        cls.lib.stars_check_initiative.argtypes = [
            PTR, C.POINTER(CheckInitiativeOps), C.c_void_p]
        cls.lib.stars_cmax_mines.restype = C.c_int16
        cls.lib.stars_cmax_mines.argtypes = [PTR, C.c_int16,
                                             C.POINTER(CMaxMinesOps),
                                             C.c_void_p]
        cls.lib.stars_pct_planet_capacity.restype = C.c_int16
        cls.lib.stars_pct_planet_capacity.argtypes = [PTR, C.c_int16,
                                                      C.c_void_p, C.c_void_p]
        cls.lib.stars_cmax_operable_mines.restype = C.c_int16
        cls.lib.stars_cmax_operable_mines.argtypes = [PTR, C.c_int16,
                                                      C.c_int16,
                                                      C.POINTER(CMaxOperableMinesOps),
                                                      C.c_void_p]
        cls.lib.stars_cmines_operating.restype = C.c_int16
        cls.lib.stars_cmines_operating.argtypes = [
            PTR, C.POINTER(CMaxOperableMinesOps), C.c_void_p]
        cls.lib.stars_cmax_factories.restype = C.c_int16
        cls.lib.stars_cmax_factories.argtypes = [
            PTR, C.c_int16, C.POINTER(CMaxMinesOps), C.c_void_p]
        cls.lib.stars_cmax_operable_factories.restype = C.c_int16
        cls.lib.stars_cmax_operable_factories.argtypes = [
            PTR, C.c_int16, C.c_int16,
            C.POINTER(CMaxOperableFactoriesOps), C.c_void_p]
        cls.lib.stars_cfactories_operating.restype = C.c_int16
        cls.lib.stars_cfactories_operating.argtypes = [
            PTR, C.POINTER(CMaxOperableFactoriesOps), C.c_void_p]
        cls.lib.stars_cmax_defenses.restype = C.c_int16
        cls.lib.stars_cmax_defenses.argtypes = [
            PTR, C.c_int16, C.POINTER(CMaxDefensesOps), C.c_void_p]
        cls.lib.stars_cmax_operable_defenses.restype = C.c_int16
        cls.lib.stars_cmax_operable_defenses.argtypes = [
            PTR, C.c_int16, C.c_int16,
            C.POINTER(CMaxOperableDefensesOps), C.c_void_p]
        cls.lib.stars_cresources_at_planet.restype = C.c_int16
        cls.lib.stars_cresources_at_planet.argtypes = [
            PTR, C.c_int16, C.POINTER(CResourcesAtPlanetOps), C.c_void_p]
        cls.lib.stars_iwarp_ma_from_planet.restype = C.c_int16
        cls.lib.stars_iwarp_ma_from_planet.argtypes = [
            PTR, C.c_int16, C.POINTER(C.c_int16), C.c_void_p, C.c_void_p,
            C.c_void_p]
        cls.lib.stars_stargate_range_from_planet.restype = C.c_int16
        cls.lib.stars_stargate_range_from_planet.argtypes = [
            PTR, C.c_int16, C.c_int16, C.c_void_p, C.c_void_p, C.c_void_p]
        cls.lib.stars_fprod_is_terra.restype = C.c_int16
        cls.lib.stars_fprod_is_terra.argtypes = [PTR]
        cls.lib.stars_ipct_can_terraform.restype = C.c_int16
        cls.lib.stars_ipct_can_terraform.argtypes = [PTR, C.c_void_p,
                                                     C.c_void_p]
        cls.lib.stars_uninhabit_planet.argtypes = [
            PTR, C.POINTER(UninhabitPlanetOps), C.c_void_p]
        cls.lib.stars_pct_cloak_from_huldef.restype = C.c_int16
        cls.lib.stars_pct_cloak_from_huldef.argtypes = [
            PTR, C.c_int16, C.POINTER(C.c_int16), PTR, C.c_uint16,
            C.c_void_p, C.c_void_p]
        cls.lib.stars_can_merge.restype = C.c_int16
        cls.lib.stars_can_merge.argtypes = [C.POINTER(MergeFleet),
                                            C.POINTER(MergeFleet), C.c_uint16,
                                            C.c_int16]
        cls.lib.stars_set_fleet_dropdown_sel.argtypes = [
            C.c_uint16, C.c_uint16, C.c_int16, C.c_uint16, C.c_int16,
            C.c_int16, C.POINTER(DropDownFleet), C.c_uint16,
            C.POINTER(SetFleetDropDownOps), C.c_void_p]
        cls.lib.stars_wt_max_ship_design_stat.restype = C.c_int16
        cls.lib.stars_wt_max_ship_design_stat.argtypes = [
            C.POINTER(ShipDesignStat), C.POINTER(HullStat), C.c_int16]
        cls.lib.stars_get_fleet_stat.restype = C.c_int32
        cls.lib.stars_get_fleet_stat.argtypes = [
            C.POINTER(FleetStatEntry), C.c_uint16, C.c_uint16, C.c_int16]
        cls.lib.stars_enum_calc_jettison.restype = C.c_int16
        cls.lib.stars_enum_calc_jettison.argtypes = [
            C.POINTER(JettisonRecord), C.c_int16, C.POINTER(C.c_uint32),
            C.c_uint16, C.c_void_p, C.c_void_p]
        cls.lib.stars_xfer_supply.restype = C.c_int32
        cls.lib.stars_xfer_supply.argtypes = [
            C.c_int16, C.c_int32, C.POINTER(TransferEndpoint), C.c_void_p,
            C.c_void_p]
        cls.lib.stars_change_cargo.restype = C.c_int32
        cls.lib.stars_change_cargo.argtypes = [
            C.c_int16, C.c_uint16, C.c_int16, C.c_int32,
            C.POINTER(CargoState), C.POINTER(ChgCargoOps), C.c_void_p]
        cls.lib.stars_update_xfer_buttons.argtypes = [
            C.c_int16, C.c_int16, C.POINTER(TransferButton), C.c_uint16,
            C.c_void_p, C.c_void_p]
        cls.lib.stars_draw_xfer_dlg.argtypes = [
            C.c_uint16, C.c_uint16, C.POINTER(Rect), C.c_int16, C.c_int16,
            C.c_int16, C.POINTER(TransferButton), C.c_uint16,
            C.POINTER(TransferSide), C.POINTER(DrawXferOps), C.c_void_p]
        cls.lib.stars_setup_xfer_buttons.restype = C.c_int16
        cls.lib.stars_setup_xfer_buttons.argtypes = [
            C.POINTER(Rect), C.c_int16, C.c_int16, C.c_int16,
            C.POINTER(C.c_int16), C.POINTER(C.c_uint16),
            C.POINTER(TransferButton), C.c_uint16, C.c_void_p, C.c_void_p]
        cls.lib.stars_draw_thing_xfer_side.argtypes = [
            C.c_uint16, C.POINTER(Rect), C.POINTER(CargoState), C.c_uint16,
            C.c_int16, C.c_int16, C.c_uint16, C.c_uint16,
            C.POINTER(DrawThingXferOps), C.c_void_p]
        cls.lib.stars_draw_fleet_cargo_xfer_side.argtypes = [
            C.c_uint16, C.POINTER(Rect), C.POINTER(CargoState), C.c_uint16,
            C.c_uint16, C.c_int16, C.c_int16, C.c_uint16, C.c_uint16,
            C.POINTER(DrawFleetCargoXferOps), C.c_void_p]
        cls.lib.stars_draw_fleet_ships_xfer_side.argtypes = [
            C.c_uint16, C.POINTER(Rect), C.POINTER(CargoState), C.c_uint16,
            C.POINTER(C.c_uint16), C.c_uint16, C.c_int16, C.c_int16,
            C.c_uint16, C.c_uint16, C.POINTER(DrawFleetShipsXferOps),
            C.c_void_p]
        cls.lib.stars_draw_planet_xfer_side.argtypes = [
            C.c_uint16, C.POINTER(Rect), C.POINTER(CargoState), C.c_int16,
            C.c_int16, C.c_int16, C.c_uint16, C.c_uint16, C.c_uint16,
            C.POINTER(DrawPlanetXferOps), C.c_void_p]
        cls.lib.stars_get_xfer_left_right_rects.argtypes = [
            C.POINTER(Rect), C.POINTER(Rect), C.POINTER(Rect), C.c_int16]
        cls.lib.stars_show_tutor.argtypes = [C.POINTER(C.c_uint16),
                                             C.POINTER(C.c_uint16), C.c_int16,
                                             C.c_void_p, C.c_void_p]
        cls.lib.stars_write_mem_rt.argtypes = [
            PTR, PTR, C.c_int16, C.c_int16, PTR, C.POINTER(WriteMemRtOps),
            C.c_void_p]
        cls.lib.stars_get_file_status.restype = C.c_int16
        cls.lib.stars_get_file_status.argtypes = [PTR, C.c_uint16, C.c_int16,
                                                   C.c_void_p, C.c_void_p]
        cls.lib.stars_new_turn_available.restype = C.c_int16
        cls.lib.stars_new_turn_available.argtypes = [PTR, C.c_int16,
                                                     C.c_void_p, C.c_void_p,
                                                     C.c_void_p]
        cls.lib.stars_check_file.restype = C.c_int16
        cls.lib.stars_check_file.argtypes = [PTR, C.c_uint16, C.c_int16,
                                             C.c_uint16, C.c_void_p, C.c_void_p,
                                             C.c_void_p, C.c_void_p]
        cls.lib.stars_write_orders.argtypes = [PTR, C.c_uint16, C.c_void_p,
                                               C.c_void_p]
        cls.lib.stars_write_runtime_string.argtypes = [PTR, PTR, C.c_void_p,
                                                       C.c_void_p]
        cls.lib.stars_append_file.restype = C.c_int16
        cls.lib.stars_append_file.argtypes = [C.c_int16, C.c_void_p,
                                              C.c_void_p, C.c_void_p]
        cls.lib.stars_write_battle_plan.argtypes = [PTR, PTR, C.c_int16,
                                                    C.c_void_p, C.c_void_p,
                                                    C.c_void_p]
        cls.lib.stars_unpack_battle_plan.argtypes = [PTR, PTR, PTR, C.c_int16]
        cls.lib.stars_ask_save_dialog.restype = C.c_int16
        cls.lib.stars_ask_save_dialog.argtypes = [C.c_uint16, C.c_uint16,
                                                   C.c_uint16, C.c_int32,
                                                   PTR, C.c_void_p, C.c_void_p,
                                                   C.c_void_p]
        cls.lib.stars_prompt_save_game.argtypes = [PTR,
                                                    C.POINTER(PromptSaveOps),
                                                    C.c_void_p]
        cls.lib.stars_set_visible_plan_fleet.argtypes = [
            PTR, C.c_int16, C.POINTER(VisiblePlanFleetOps), C.c_void_p]
        cls.lib.stars_ensure_cas_hdefs.argtypes = [C.c_int16]
        cls.lib.stars_ensure_cas_hdefs.restype = None
        cls.lib.stars_cancel_mem_rt.argtypes = [PTR, C.c_int16]
        cls.lib.stars_dirty_game.argtypes = [PTR, C.c_int16, C.c_void_p,
                                              C.c_void_p]
        cls.lib.stars_get_fuel_free.restype = C.c_uint32
        cls.lib.stars_get_fuel_free.argtypes = [PTR, C.c_void_p, C.c_void_p]
        cls.lib.stars_get_cargo_free.restype = C.c_uint32
        cls.lib.stars_get_cargo_free.argtypes = [PTR, C.c_void_p, C.c_void_p]
        cls.lib.stars_fuel_tanker.restype = C.c_int16
        cls.lib.stars_fuel_tanker.argtypes = [PTR]
        cls.lib.stars_logical_to_scan.argtypes = [PTR, C.c_void_p, C.c_void_p]
        cls.lib.stars_scan_to_logical.argtypes = [PTR, C.c_void_p, C.c_void_p]
        cls.lib.stars_idm_get_message_n.restype = C.c_int16
        cls.lib.stars_idm_get_message_n.argtypes = [C.c_int16, C.c_void_p,
                                                     C.c_void_p]
        cls.lib.stars_psz_get_message_n.restype = PTR
        cls.lib.stars_psz_get_message_n.argtypes = [C.c_int16, C.c_void_p,
                                                     C.c_void_p, PTR,
                                                     C.c_void_p]
        for name in ('stars_can_split', 'stars_can_split_all'):
            fn = getattr(cls.lib, name)
            fn.restype = C.c_int16
            fn.argtypes = [PTR, C.c_int32]
        cls.lib.stars_is_popup_hull_type.restype = C.c_int16
        cls.lib.stars_is_popup_hull_type.argtypes = [C.c_uint16, PTR,
                                                      C.c_uint16,
                                                      C.POINTER(C.c_uint16)]
        cls.lib.stars_handle_focus_state.argtypes = [C.c_uint16, C.c_uint16,
                                                      C.POINTER(C.c_uint16),
                                                      C.c_uint16, C.c_int16,
                                                      C.c_void_p, C.c_void_p]
        cls.lib.stars_enable_zip_buttons.argtypes = [C.c_uint16, C.c_int16,
                                                      PTR, C.c_void_p,
                                                      C.c_void_p, C.c_void_p]
        cls.lib.stars_random_events.argtypes = [C.c_void_p, C.c_void_p,
                                                 C.c_void_p, C.c_void_p,
                                                 C.c_void_p]
        cls.lib.stars_format_serial_and_env.argtypes = [
            C.c_void_p, C.c_uint32, PTR, PTR, PTR]
        cls.lib.stars_format_serial_and_env_checked.restype = C.c_int16
        cls.lib.stars_format_serial_and_env_checked.argtypes = [
            C.c_void_p, C.c_uint32, PTR, PTR, PTR, C.c_uint16]
        cls.lib.stars_serial_and_env_from_string.restype = C.c_int16
        cls.lib.stars_serial_and_env_from_string.argtypes = [
            C.c_void_p, PTR, C.POINTER(C.c_uint32), PTR, PTR, C.c_void_p,
            C.c_void_p]
        cls.lib.stars_enum_our_starbase.restype = C.c_int16
        cls.lib.stars_enum_our_starbase.argtypes = [PTR, C.c_uint16]
        for name in ('stars_enum_calc_enemy_fleet',
                     'stars_enum_calc_enemy_planet'):
            fn = getattr(cls.lib, name)
            fn.restype = C.c_int16
            fn.argtypes = [PTR, C.c_uint16]
        cls.lib.stars_empty_bmp_from_grhst.restype = C.c_int16
        cls.lib.stars_empty_bmp_from_grhst.argtypes = [PTR, C.c_int16]
        cls.lib.stars_check_research.restype = C.c_int16
        cls.lib.stars_check_research.argtypes = [PTR, C.c_int16, C.c_int16,
                                                  C.c_int16]
        cls.lib.stars_clear_ai_current_task.argtypes = [
            PTR, PTR, C.c_int16, C.c_void_p, C.c_void_p, C.c_void_p]
        cls.lib.stars_set_ngw_title.argtypes = [
            PTR, C.c_uint16, C.c_int16, C.c_void_p, C.c_void_p, C.c_void_p,
            C.c_void_p]
        cls.lib.stars_check_fleet_wp.restype = C.c_int16
        cls.lib.stars_check_fleet_wp.argtypes = [
            PTR, PTR, PTR, C.c_uint16, C.c_int16, C.c_uint16, C.c_int16,
            C.c_uint16, C.c_uint16, C.c_void_p, C.c_void_p, C.c_void_p]
        cls.lib.stars_check_planet_route.restype = C.c_int16
        cls.lib.stars_check_planet_route.argtypes = [PTR, PTR, C.c_int16]
        cls.lib.stars_check_laying_wp.restype = C.c_int16
        cls.lib.stars_check_laying_wp.argtypes = [
            PTR, PTR, PTR, C.c_uint16, C.c_int16, C.c_int16, C.c_int16,
            C.c_void_p, C.c_void_p, C.c_void_p]
        cls.lib.stars_check_fleet_name.restype = C.c_int16
        cls.lib.stars_check_fleet_name.argtypes = [
            PTR, PTR, C.c_int16, C.c_void_p, C.c_void_p, C.c_void_p,
            C.c_void_p]
        cls.lib.stars_check_scanner.restype = C.c_int16
        cls.lib.stars_check_scanner.argtypes = [PTR, C.c_int16, C.c_int16]
        cls.lib.stars_tutor_error.argtypes = [
            PTR, C.c_int16, C.c_void_p, C.c_void_p, C.c_void_p]
        cls.lib.stars_check_template.restype = C.c_int16
        cls.lib.stars_check_template.argtypes = [PTR, PTR, C.c_int16]
        cls.lib.stars_check_summary.restype = C.c_int16
        cls.lib.stars_check_summary.argtypes = [PTR, C.c_uint16, C.c_int16,
                                                 PTR, PTR]
        cls.lib.stars_check_messages.restype = C.c_int16
        cls.lib.stars_check_messages.argtypes = [
            PTR, C.c_int16, C.c_int16, C.c_int16, PTR, C.c_void_p,
            C.c_void_p, C.c_void_p, C.c_void_p]
        cls.lib.stars_check_selection.restype = C.c_int16
        cls.lib.stars_check_selection.argtypes = [PTR, C.c_uint16, C.c_int16,
                                                   PTR]
        cls.lib.stars_check_zip.restype = C.c_int16
        cls.lib.stars_check_zip.argtypes = [
            PTR, C.c_int16, PTR, C.c_int16, C.c_void_p, C.c_void_p,
            C.c_void_p, C.c_void_p]
        cls.lib.stars_check_cargo.restype = C.c_int16
        cls.lib.stars_check_cargo.argtypes = [
            PTR, PTR, C.c_int16, C.c_int16, C.c_int16, C.c_int16,
            C.c_void_p, C.c_void_p, C.c_void_p]
        cls.lib.stars_check_ship_builder.restype = C.c_int16
        cls.lib.stars_check_ship_builder.argtypes = [
            PTR, C.c_uint16, C.c_int16, C.c_int16, C.c_int16, C.c_void_p,
            C.c_void_p, C.c_void_p]
        cls.lib.stars_check_builder_part.restype = C.c_int16
        cls.lib.stars_check_builder_part.argtypes = [
            PTR, C.c_uint16, C.c_int16, C.c_int16, PTR, PTR, C.c_uint16,
            C.c_void_p, C.c_void_p]
        cls.lib.stars_check_queue.restype = C.c_int16
        cls.lib.stars_check_queue.argtypes = [
            PTR, C.c_int16, C.c_int16, C.c_int16, C.c_uint16, C.c_uint16,
            C.c_uint16, C.c_uint16, PTR, PTR, C.c_uint16, C.c_void_p,
            C.c_void_p, C.c_void_p]
        cls.lib.stars_check_xfer_wp.restype = C.c_int16
        cls.lib.stars_check_xfer_wp.argtypes = [
            PTR, PTR, PTR, C.c_uint16, C.c_int16, C.c_int16, C.c_uint16,
            PTR, C.c_void_p, C.c_void_p, C.c_void_p]
        cls.lib.stars_check_colonize_wp.restype = C.c_int16
        cls.lib.stars_check_colonize_wp.argtypes = [
            PTR, PTR, PTR, C.c_uint16, C.c_int16, C.c_uint16, C.c_void_p,
            C.c_void_p, C.c_void_p]
        cls.lib.stars_check_patrol_wp.restype = C.c_int16
        cls.lib.stars_check_patrol_wp.argtypes = [
            PTR, PTR, PTR, C.c_uint16, C.c_int16, C.c_int16, C.c_uint16,
            C.c_uint16, C.c_uint16, C.c_void_p, C.c_void_p]
        cls.lib.stars_check_battle_plan.restype = C.c_int16
        cls.lib.stars_check_battle_plan.argtypes = [
            PTR, C.c_int16, C.c_uint16, C.c_uint16, C.c_uint16,
            C.c_uint16, C.c_uint16, C.c_uint16, C.c_uint16, C.c_uint16]
        cls.lib.stars_ok_merge_dialog.restype = C.c_int16
        cls.lib.stars_ok_merge_dialog.argtypes = [PTR, PTR, C.c_void_p,
                                                   C.c_void_p]
        cls.lib.stars_cch_tutor_string.restype = C.c_int16
        cls.lib.stars_cch_tutor_string.argtypes = [
            PTR, C.c_int16, C.c_int16, PTR, PTR, PTR, PTR]
        cls.lib.stars_tutorial_enabled_ship_builder.restype = C.c_int16
        cls.lib.stars_tutorial_enabled_ship_builder.argtypes = [
            PTR, C.c_int16, C.c_int16, C.c_uint16, C.c_uint16, C.c_uint16,
            PTR, PTR, C.c_void_p, C.c_void_p, C.c_void_p, C.c_void_p,
            C.c_void_p]
        cls.lib.stars_turns_out_safe.restype = C.c_int16
        cls.lib.stars_turns_out_safe.argtypes = [PTR, C.c_void_p, C.c_void_p]
        cls.lib.stars_draw_host_options.argtypes = [C.c_uint16, C.c_uint16,
                                                    C.c_int16]
        cls.lib.stars_mark_planet.argtypes = [PTR, PTR, C.c_uint16, C.c_int16,
                                              C.c_uint16, C.c_void_p, C.c_void_p]
        cls.lib.stars_mark_fleet.argtypes = [PTR, PTR, C.c_uint16, PTR,
                                             C.c_uint16, C.c_uint16]
        class ShieldComponent(C.Structure):
            _fields_ = [('kind', C.c_uint16), ('packed_count_type', C.c_uint16),
                        ('damage_points', C.c_uint16)]
        cls.ShieldComponent = ShieldComponent
        cls.lib.stars_shield_power.restype = C.c_uint32
        cls.lib.stars_shield_power.argtypes = [C.POINTER(ShieldComponent),
                                               C.c_uint16, C.c_int16]
        cls.lib.stars_primary_ship_design.restype = C.c_int16
        cls.lib.stars_primary_ship_design.argtypes = [C.POINTER(C.c_uint16),
                                                      C.POINTER(C.c_uint16),
                                                      C.c_uint16,
                                                      C.POINTER(C.c_int16)]
        cls.lib.stars_fleet_code.restype = C.c_uint16
        cls.lib.stars_fleet_code.argtypes = [C.c_uint16, C.c_int16, C.c_int16]
        cls.lib.stars_stargate_index.restype = C.c_int16
        cls.lib.stars_stargate_index.argtypes = [PTR, PTR, C.c_uint16,
                                                 C.c_uint16]
        cls.lib.stars_fleet_can_jumpgate.restype = C.c_int16
        cls.lib.stars_fleet_can_jumpgate.argtypes = [C.POINTER(C.c_uint16),
                                                     PTR, C.c_uint16,
                                                     C.c_uint16]
        cls.lib.stars_fleet_weight.restype = C.c_uint32
        cls.lib.stars_fleet_weight.argtypes = [C.POINTER(C.c_uint16),
                                               C.POINTER(C.c_uint32),
                                               C.POINTER(C.c_uint32)]
        cls.lib.stars_match_target.restype = C.c_int16
        cls.lib.stars_match_target.argtypes = [C.POINTER(C.c_uint16), PTR,
                                               C.c_uint16, C.c_int16,
                                               C.c_int16]
        cls.lib.stars_fleet_index_from_grouped_id.restype = C.c_int16
        cls.lib.stars_fleet_index_from_grouped_id.argtypes = [
            C.POINTER(C.c_uint16), C.c_uint16, C.POINTER(C.c_uint16),
            C.c_uint16, C.c_int16, C.c_int16]
        cls.lib.stars_lookup_object_index.restype = C.c_int16
        cls.lib.stars_lookup_object_index.argtypes = [
            C.c_int16, C.c_int16, C.POINTER(C.c_uint16), C.c_uint16,
            C.POINTER(C.c_uint16), C.c_uint16, C.POINTER(C.c_uint16),
            C.c_uint16]
        cls.lib.stars_selected_planet_index.restype = C.c_int16
        cls.lib.stars_selected_planet_index.argtypes = [C.c_int16, C.c_int16,
                                                        C.POINTER(C.c_uint16),
                                                        C.c_uint16]
        cls.lib.stars_selected_fleet_index.restype = C.c_int16
        cls.lib.stars_selected_fleet_index.argtypes = [
            C.c_int16, C.c_uint16, C.POINTER(C.c_uint16), C.c_uint16,
            C.POINTER(C.c_uint16), C.c_uint16]
        cls.lib.stars_lookup_thing_record.restype = C.c_int16
        cls.lib.stars_lookup_thing_record.argtypes = [
            C.c_int16, PTR, C.c_uint16, C.c_uint16, PTR, C.c_int16]
        cls.lib.stars_can_build_design.restype = C.c_int16
        cls.lib.stars_can_build_design.argtypes = [
            C.c_int16, C.POINTER(cls.ShieldComponent), C.c_uint16,
            C.c_void_p, C.c_void_p]
        cls.lib.stars_find_similar_design.restype = C.c_int16
        cls.lib.stars_find_similar_design.argtypes = [
            C.c_uint16, C.POINTER(C.c_uint16), C.POINTER(C.c_uint16),
            C.c_uint16, C.POINTER(C.c_uint16), C.POINTER(C.c_uint16),
            C.POINTER(C.c_uint16), C.POINTER(C.c_uint16),
            C.POINTER(C.c_uint16), C.c_uint16, C.c_uint16]
        cls.lib.stars_decorate_hull_name.argtypes = [
            PTR, C.c_uint16, C.POINTER(C.c_uint16), C.c_uint16,
            C.c_uint16, C.c_uint16, C.c_uint16, PTR, C.c_uint16]
        cls.lib.stars_turn_log.restype = C.c_int16
        cls.lib.stars_turn_log.argtypes = [C.c_int16, C.c_int16, C.c_int16,
                                           PTR, PTR, C.c_uint16, C.c_void_p,
                                           C.c_void_p]
        cls.lib.stars_update_progress.restype = C.c_int16
        cls.lib.stars_update_progress.argtypes = [
            C.POINTER(ProgressState), C.c_int16, C.c_uint16, C.c_void_p,
            C.c_void_p]
        cls.lib.stars_show_progress.restype = C.c_int16
        cls.lib.stars_show_progress.argtypes = [C.POINTER(ProgressState),
                                                C.c_void_p, C.c_void_p,
                                                C.c_void_p]
        cls.lib.stars_hide_progress.argtypes = [C.POINTER(ProgressState),
                                                C.c_void_p, C.c_void_p]
        cls.lib.stars_printer_font_create.restype = C.c_uint16
        cls.lib.stars_printer_font_create.argtypes = [C.POINTER(Runtime),
                                                      C.c_uint16, C.c_int16,
                                                      C.POINTER(C.c_int16)]
        cls.lib.stars_copy_file.restype = C.c_int16
        cls.lib.stars_copy_file.argtypes = [PTR, PTR, C.POINTER(StreamApi),
                                            C.c_void_p, C.c_void_p]
        cls.lib.stars_draw_planet_print_dot.argtypes = [C.POINTER(Runtime),
                                                        C.c_uint16, C.c_int16,
                                                        C.c_int16, C.c_int16]
        cls.lib.stars_intersect_circle_line.restype = C.c_int16
        cls.lib.stars_intersect_circle_line.argtypes = [
            C.c_int16, C.c_int16, C.c_int16, C.c_int16, C.c_int16,
            C.c_int16, C.c_uint32, C.c_int16, C.POINTER(C.c_int16),
            C.POINTER(C.c_int16)
        ]
        class Seeds(C.Structure):
            _fields_ = [('first', C.c_uint32), ('second', C.c_uint32)]
        cls.Seeds = Seeds
        cls.lib.stars_validate_serial_number.restype = C.c_int16
        cls.lib.stars_validate_serial_number.argtypes = [
            PTR, C.POINTER(Seeds), C.POINTER(C.c_uint32)
        ]
        class PowerComponent(C.Structure):
            _fields_ = [('kind', C.c_uint16),
                        ('packed_count_type', C.c_uint16),
                        ('fuel_max', C.c_uint16),
                        ('cargo_max', C.c_uint16),
                        ('damage_points', C.c_uint16),
                        ('flags', C.c_uint16)]
        class PowerDesign(C.Structure):
            _fields_ = [('hidden_flags', C.c_uint16),
                        ('component_count', C.c_uint16),
                        ('components', C.POINTER(PowerComponent)),
                        ('speed', C.c_int16), ('power', C.c_int32)]
        cls.PowerComponent = PowerComponent
        cls.PowerDesign = PowerDesign
        cls.lib.stars_compute_design_power.restype = C.c_int32
        cls.lib.stars_compute_design_power.argtypes = [
            C.POINTER(PowerComponent), C.c_uint16, C.c_int16]
        cls.lib.stars_compute_design_powers.argtypes = [
            C.POINTER(PowerDesign), C.c_uint16]
        class RamScoopDesign(C.Structure):
            _fields_ = [('ship_count', C.c_uint16),
                        ('scoop_count', C.c_uint16),
                        ('fuel_used', C.c_ubyte * 11)]
        cls.RamScoopDesign = RamScoopDesign
        cls.lib.stars_fuel_gain_from_ram_scoops.restype = C.c_int32
        cls.lib.stars_fuel_gain_from_ram_scoops.argtypes = [
            C.POINTER(RamScoopDesign), C.c_uint16, C.c_int16, C.c_int32]
        class ScannerDesign(C.Structure):
            _fields_ = [('ship_count', C.c_uint16),
                        ('scanner_range', C.c_int16),
                        ('planet_range', C.c_int16),
                        ('detection_percent', C.c_int16),
                        ('steal_flags', C.c_int16)]
        cls.ScannerDesign = ScannerDesign
        for name in ('stars_get_fleet_scanner_range',
                     'stars_get_cached_fleet_scanner_range'):
            fn = getattr(cls.lib, name)
            fn.restype = C.c_int16
            fn.argtypes = [C.POINTER(ScannerDesign), C.c_uint16,
                           C.POINTER(C.c_int16), C.POINTER(C.c_int16),
                           C.POINTER(C.c_int16)]
        cls.lib.stars_get_planet_scanner_range.restype = C.c_int16
        cls.lib.stars_get_planet_scanner_range.argtypes = [
            C.c_int16, C.c_int16, C.c_int32, C.c_int16, C.c_int16,
            C.c_int16, C.POINTER(C.c_int16)]
        class ScannerComponent(C.Structure):
            _fields_ = [('kind', C.c_uint16),
                        ('packed_count_type', C.c_uint16),
                        ('range', C.c_int16), ('abilities', C.c_int16)]
        cls.ScannerComponent = ScannerComponent
        cls.lib.stars_get_design_scanner_range.restype = C.c_int16
        cls.lib.stars_get_design_scanner_range.argtypes = [
            C.POINTER(ScannerComponent), C.c_uint16, C.c_int32, C.c_int32,
            C.c_int16, PTR, C.POINTER(C.c_int16), C.POINTER(C.c_int16),
            C.POINTER(C.c_int16)]
        cls.lib.stars_fleet_design_damage.restype = C.c_int32
        cls.lib.stars_fleet_design_damage.argtypes = [
            PTR, C.c_uint16, C.POINTER(C.c_int16), C.c_uint16]
        cls.lib.stars_lookup_orbiting_transfer.restype = C.c_int16
        cls.lib.stars_lookup_orbiting_transfer.argtypes = [
            C.c_int16, C.c_int16, PTR, C.c_uint16, C.c_uint16, PTR,
            C.c_uint16, C.c_uint16, C.c_int16, C.c_int16, C.c_int16, PTR]
        cls.lib.stars_can_fleet_use_stargates.restype = C.c_int16
        cls.lib.stars_can_fleet_use_stargates.argtypes = [
            C.c_int16, C.c_int16, C.c_int16, C.c_int16, C.c_int16,
            C.c_int16, C.c_int16, C.c_int16, C.c_int16, C.c_int16,
            C.c_int16, C.POINTER(C.c_int32), C.c_int16,
            C.POINTER(C.c_uint16), C.POINTER(C.c_int16), C.c_uint16,
            C.c_void_p, C.c_void_p]
        class CostComponent(C.Structure):
            _fields_ = [('kind', C.c_uint16),
                        ('packed_count_type', C.c_uint16),
                        ('ore_cost', C.c_uint16 * 3),
                        ('resource_cost', C.c_uint16),
                        ('weight_empty', C.c_uint16),
                        ('cargo_max', C.c_uint16)]
        class DesignCost(C.Structure):
            _fields_ = [('damage_points', C.c_int16),
                        ('ore_cost', C.c_uint16 * 3),
                        ('resource_cost', C.c_uint16),
                        ('weight_empty', C.c_uint16),
                        ('hull_weight_empty', C.c_uint16)]
        cls.CostComponent = CostComponent
        cls.DesignCost = DesignCost
        cls.lib.stars_update_design_cost.argtypes = [
            C.POINTER(DesignCost), C.POINTER(CostComponent), C.c_uint16,
            C.c_int16]
        cls.lib.stars_calc_pct_survive.argtypes = [
            C.c_int16, C.c_uint16, C.c_uint16, C.c_int16,
            C.POINTER(C.c_float), C.POINTER(C.c_float)]
        class PlanetProduction(C.Structure):
            _fields_ = [('count', C.c_uint16), ('capacity', C.c_uint16),
                        ('records', PTR)]
        class PlanetState(C.Structure):
            _fields_ = [('bytes', C.c_ubyte * 52),
                        ('production', C.POINTER(PlanetProduction))]
        cls.PlanetProduction = PlanetProduction
        cls.PlanetState = PlanetState
        cls.lib.stars_duplicate_planet.restype = C.c_int16
        cls.lib.stars_duplicate_planet.argtypes = [
            C.POINTER(PlanetState), C.POINTER(PlanetState)]
        cls.lib.stars_lookup_planet.restype = C.c_int16
        cls.lib.stars_lookup_planet.argtypes = [
            C.POINTER(C.POINTER(PlanetState)), C.c_uint16, C.c_int16,
            C.POINTER(PlanetState), C.c_void_p, C.c_void_p]
        class FleetOrders(C.Structure):
            _fields_ = [('count', C.c_uint16), ('capacity', C.c_uint16),
                        ('records', PTR)]
        class FleetState(C.Structure):
            _fields_ = [('bytes', C.c_ubyte * 124),
                        ('orders', C.POINTER(FleetOrders))]
        cls.FleetOrders = FleetOrders
        cls.FleetState = FleetState
        cls.lib.stars_duplicate_fleet.restype = C.c_int16
        cls.lib.stars_duplicate_fleet.argtypes = [
            C.POINTER(FleetState), C.POINTER(FleetState)]
        cls.lib.stars_lookup_fleet.restype = C.c_int16
        cls.lib.stars_lookup_fleet.argtypes = [
            C.POINTER(C.POINTER(FleetState)), C.c_uint16, C.c_int16,
            C.POINTER(FleetState), C.c_int16]
        class BombComponent(C.Structure):
            _fields_ = [('kind', C.c_uint16),
                        ('packed_damage_type', C.c_uint16),
                        ('defense_power', C.c_uint16),
                        ('fuel_max', C.c_uint16)]
        class BombDesignSlot(C.Structure):
            _fields_ = [('components', C.POINTER(BombComponent)),
                        ('component_count', C.c_uint16)]
        class BombFleet(C.Structure):
            _fields_ = [('player', C.c_int16), ('planet', C.c_int16),
                        ('next_index', C.c_int16), ('flags', C.c_uint16),
                        ('component_counts', C.c_uint16 * 16)]
        class BombDamage(C.Structure):
            _fields_ = [('people', C.c_int32), ('people_min', C.c_int32),
                        ('people_smart', C.c_int32), ('building', C.c_int32),
                        ('terraforming', C.c_int32), ('multiple', C.c_int16)]
        cls.BombComponent = BombComponent
        cls.BombDesignSlot = BombDesignSlot
        cls.BombFleet = BombFleet
        cls.BombDamage = BombDamage
        cls.lib.stars_calc_fleet_bomb_damage.restype = C.c_int16
        cls.lib.stars_calc_fleet_bomb_damage.argtypes = [
            C.POINTER(BombFleet), C.c_uint16, C.c_int16,
            C.POINTER(BombDesignSlot), C.c_int32, C.c_double, C.c_double,
            C.POINTER(BombDamage)]
        cls.lib.stars_delete_fleet.restype = C.c_int16
        cls.lib.stars_delete_fleet.argtypes = [
            C.POINTER(C.POINTER(FleetState)), C.POINTER(C.c_uint16),
            C.c_int16, C.c_int16, C.c_int16, C.c_void_p, C.c_void_p]
        class FleetTable(C.Structure):
            _fields_ = [('items', C.POINTER(C.POINTER(FleetState))),
                        ('count', C.c_uint16), ('capacity', C.c_uint16),
                        ('player_counts', C.c_uint16 * 16)]
        cls.FleetTable = FleetTable
        cls.lib.stars_split_all_fleet.restype = C.c_int16
        cls.lib.stars_split_all_fleet.argtypes = [
            C.POINTER(FleetTable), C.POINTER(FleetState), C.c_void_p,
            C.c_void_p, C.POINTER(C.c_uint16)]
        class EtaPoint(C.Structure):
            _fields_ = [('x', C.c_int16), ('y', C.c_int16),
                        ('warp_code', C.c_uint16)]
        cls.EtaPoint = EtaPoint
        cls.lib.stars_calculate_eta.restype = C.c_int16
        cls.lib.stars_calculate_eta.argtypes = [
            C.POINTER(EtaPoint), C.c_uint16, C.c_uint16, C.c_void_p,
            C.c_void_p, C.POINTER(C.c_int16), C.POINTER(C.c_int16)]
        cls.lib.stars_new_fleet.restype = C.c_int16
        cls.lib.stars_new_fleet.argtypes = [
            C.POINTER(FleetTable), C.c_int16, C.c_int16, C.c_int16,
            C.c_int16, C.POINTER(C.POINTER(FleetState))]
        cls.lib.stars_split_fleet.restype = C.c_int16
        cls.lib.stars_split_fleet.argtypes = [
            C.POINTER(FleetTable), C.POINTER(FleetState),
            C.POINTER(C.POINTER(FleetState)), C.c_void_p, C.c_void_p]
        class NearestPoint(C.Structure):
            _fields_ = [('x', C.c_int16), ('y', C.c_int16)]
        class NearestResult(C.Structure):
            _fields_ = [('x', C.c_int16), ('y', C.c_int16),
                        ('kind', C.c_int16), ('planet_index', C.c_int16),
                        ('fleet_index', C.c_int16),
                        ('waypoint_index', C.c_int16),
                        ('thing_index', C.c_int16)]
        cls.NearestPoint = NearestPoint
        cls.NearestResult = NearestResult
        class SelectFleet(C.Structure):
            _fields_ = [('id', C.c_int16), ('player', C.c_int16),
                        ('x', C.c_int16), ('y', C.c_int16)]
        class SelectThing(C.Structure):
            _fields_ = [('x', C.c_int16), ('y', C.c_int16),
                        ('selectable', C.c_int16)]
        cls.SelectFleet = SelectFleet
        cls.SelectThing = SelectThing
        cls.lib.stars_select_ours_at_object.restype = C.c_int16
        cls.lib.stars_select_ours_at_object.argtypes = [
            C.c_int16, C.c_int16, C.c_int16, C.POINTER(NearestPoint),
            C.c_uint16, C.POINTER(SelectFleet), C.c_uint16,
            C.POINTER(SelectThing), C.c_uint16, C.c_int16,
            C.c_void_p, C.c_void_p, C.c_void_p, C.c_void_p]
        class ScorePlanet(C.Structure):
            _fields_ = [('owner', C.c_int16), ('mineral_value', C.c_int32),
                        ('resources', C.c_int32),
                        ('score_starbase', C.c_int16)]
        class ScoreDesign(C.Structure):
            _fields_ = [('power', C.c_int32), ('disabled', C.c_int16)]
        class ScoreFleet(C.Structure):
            _fields_ = [('owner', C.c_int16), ('flags', C.c_uint16),
                        ('component_counts', C.c_uint16 * 16)]
        class ScoreSummary(C.Structure):
            _fields_ = [('score', C.c_int32), ('planet_count', C.c_int32),
                        ('resources', C.c_int32),
                        ('starbase_count', C.c_int32),
                        ('technology_levels', C.c_int32),
                        ('fleet_counts', C.c_int32 * 3)]
        cls.ScorePlanet, cls.ScoreDesign = ScorePlanet, ScoreDesign
        cls.ScoreFleet, cls.ScoreSummary = ScoreFleet, ScoreSummary
        cls.lib.stars_calc_player_score.restype = C.c_int32
        cls.lib.stars_calc_player_score.argtypes = [
            C.c_int16, C.POINTER(ScorePlanet), C.c_uint16,
            C.POINTER(C.c_int16), C.c_int16, C.POINTER(ScoreDesign),
            C.c_uint16, C.POINTER(ScoreFleet), C.c_uint16, C.c_int16,
            C.POINTER(ScoreSummary)]
        cls.lib.stars_pack_score_history_record.restype = None
        cls.lib.stars_pack_score_history_record.argtypes = [
            C.POINTER(ScoreSummary), C.c_uint16, C.c_uint16, PTR]
        cls.lib.stars_merge_all_fleet.restype = C.c_int16
        cls.lib.stars_merge_all_fleet.argtypes = [
            C.POINTER(FleetTable), C.POINTER(FleetState),
            C.POINTER(C.c_int16), C.c_uint16, C.POINTER(C.c_uint16)]
        class WaypointRecord(C.Structure):
            _fields_ = [('x', C.c_int16), ('y', C.c_int16),
                        ('type_flags', C.c_uint16), ('target_id', C.c_int16)]
        class WaypointFleet(C.Structure):
            _fields_ = [('id', C.c_int16), ('player', C.c_int16),
                        ('x', C.c_int16), ('y', C.c_int16),
                        ('weight', C.c_int32), ('flags', C.c_uint16)]
        class WaypointThing(C.Structure):
            _fields_ = [('id', C.c_int16), ('x', C.c_int16),
                        ('y', C.c_int16), ('flags', C.c_uint16)]
        cls.WaypointRecord = WaypointRecord
        cls.WaypointFleet = WaypointFleet
        cls.WaypointThing = WaypointThing
        cls.lib.stars_validate_waypoints.restype = C.c_int16
        cls.lib.stars_validate_waypoints.argtypes = [
            C.POINTER(WaypointFleet), C.c_uint16,
            C.POINTER(WaypointRecord), C.c_uint16,
            C.POINTER(WaypointThing), C.c_uint16, C.c_int16, C.c_int16]
        class PopulationState(C.Structure):
            _fields_ = [('owner', C.c_int16), ('desirability', C.c_int16),
                        ('population', C.c_int32), ('fraction', C.c_ubyte)]
        cls.PopulationState = PopulationState
        cls.lib.stars_change_population.restype = C.c_int32
        cls.lib.stars_change_population.argtypes = [
            C.POINTER(PopulationState), C.c_int32, C.c_int16, C.c_int16,
            C.c_int16]
        for name in ('stars_huldef_sb_from_id', 'stars_huldef_from_id',
                     'stars_engine_from_id', 'stars_scanner_from_id',
                     'stars_shdef_t', 'stars_shdef_sbt',
                     'stars_planetary_from_id'):
            getattr(cls.lib, name).restype = PTR
        cls.lib.stars_huldef_sb_from_id.argtypes = [PTR, C.c_int16]
        cls.lib.stars_huldef_from_id.argtypes = [PTR, PTR, C.c_int16]
        cls.lib.stars_engine_from_id.argtypes = [PTR, C.c_int16]
        cls.lib.stars_scanner_from_id.argtypes = [PTR, C.c_int16]
        cls.lib.stars_shdef_t.argtypes = [PTR]
        cls.lib.stars_shdef_sbt.argtypes = [PTR]
        cls.lib.stars_planetary_from_id.argtypes = [PTR, C.c_int16]
        cls.lib.stars_lookup_part_x.restype = C.c_int16
        cls.lib.stars_lookup_part_x.argtypes = [
            PTR, C.c_uint16, C.c_uint16, C.c_void_p, C.c_void_p]
        cls.lib.stars_lookup_best_planetary_scanner.restype = C.c_int16
        cls.lib.stars_lookup_best_planetary_scanner.argtypes = [
            PTR, C.c_void_p, C.c_void_p]
        cls.lib.stars_tech_status.restype = C.c_int16
        cls.lib.stars_tech_status.argtypes = [
            C.POINTER(C.c_int16), C.POINTER(C.c_int16), C.c_uint16]
        cls.lib.stars_planets_in_circle.restype = C.c_int16
        cls.lib.stars_planets_in_circle.argtypes = [
            C.c_int16, C.c_int16, C.c_uint32,
            C.POINTER(NearestPoint), C.c_uint16]
        cls.lib.stars_pct_wormhole_moves.restype = C.c_int16
        cls.lib.stars_pct_wormhole_moves.argtypes = [C.c_uint16]
        cls.lib.stars_free_thing.restype = C.c_int16
        cls.lib.stars_free_thing.argtypes = [PTR, C.POINTER(C.c_uint16),
                                              C.c_uint16]
        cls.lib.stars_new_thing.restype = C.c_int16
        cls.lib.stars_new_thing.argtypes = [
            PTR, C.POINTER(C.c_uint16), C.c_uint16, C.c_int16, C.c_int16,
            C.POINTER(C.c_uint16)]
        cls.lib.stars_give_trader_part.restype = C.c_int16
        cls.lib.stars_give_trader_part.argtypes = [
            C.POINTER(C.c_uint16), C.c_uint16, C.POINTER(C.c_uint16)]
        class WormholePoint(C.Structure):
            _fields_ = [('x', C.c_int16), ('y', C.c_int16),
                        ('identity', C.c_uint16), ('flags', C.c_uint16)]
        cls.WormholePoint = WormholePoint
        cls.lib.stars_validate_wormhole_position.restype = C.c_int16
        cls.lib.stars_validate_wormhole_position.argtypes = [
            C.c_int16, C.c_int16, C.c_uint16, C.c_int16,
            C.POINTER(WormholePoint), C.c_uint16,
            C.POINTER(NearestPoint), C.c_uint16,
            C.POINTER(NearestPoint), C.c_uint16]
        class ThingGaugeValues(C.Structure):
            _fields_ = [('total', C.c_int32), ('values', C.c_int32 * 3)]
        cls.ThingGaugeValues = ThingGaugeValues
        cls.lib.stars_draw_thing_gauge.restype = C.c_int16
        cls.lib.stars_draw_thing_gauge.argtypes = [
            C.POINTER(ThingGaugeValues), C.c_int16, C.c_void_p, C.c_void_p]
        class FrameResizeInfo(C.Structure):
            _fields_ = [('x_top', C.c_int16), ('y1', C.c_int16),
                        ('y2', C.c_int16)]
        cls.FrameResizeInfo = FrameResizeInfo
        cls.lib.stars_hcrs_from_frame_window_pt.restype = C.c_uint16
        cls.lib.stars_hcrs_from_frame_window_pt.argtypes = [
            C.c_int16, C.c_int16, C.POINTER(FrameResizeInfo), C.c_int16,
            C.c_uint16, C.c_uint16, C.c_uint16, C.POINTER(C.c_int16)]
        cls.lib.stars_get_a_sub_menu.restype = C.c_uint16
        cls.lib.stars_get_a_sub_menu.argtypes = [
            C.c_uint16, C.c_int16, C.c_uint16, C.c_int16,
            C.c_void_p, C.c_void_p, C.c_void_p]
        cls.lib.stars_find_nearest_object.restype = C.c_int16
        cls.lib.stars_find_nearest_object.argtypes = [
            C.c_int16, C.c_int16, C.c_uint16,
            C.POINTER(NearestPoint), C.c_uint16,
            C.POINTER(NearestPoint), C.c_uint16,
            C.POINTER(NearestPoint), C.c_uint16,
            C.POINTER(NearestPoint), C.c_uint16,
            C.POINTER(NearestResult)]
        class FleetLinkNode(C.Structure):
            _fields_ = [('x', C.c_int16), ('y', C.c_int16),
                        ('dead', C.c_int16), ('next_index', C.c_int16),
                        ('previous_index', C.c_int16)]
        cls.FleetLinkNode = FleetLinkNode
        cls.lib.stars_link_fleets.argtypes = [
            C.POINTER(FleetLinkNode), C.c_uint16]
        class GaugeRect(C.Structure):
            _fields_ = [('left', C.c_uint16), ('top', C.c_uint16),
                        ('right', C.c_uint16), ('bottom', C.c_uint16)]
        cls.GaugeRect = GaugeRect
        cls.lib.stars_draw_gauge.restype = C.c_int32
        cls.lib.stars_draw_gauge.argtypes = [
            C.c_void_p, C.c_uint16, C.POINTER(GaugeRect), C.c_uint16,
            C.POINTER(C.c_int32), C.POINTER(C.c_uint16), C.c_int32,
            C.c_void_p, C.c_void_p, C.c_uint16, C.c_uint16]
        class ProgressDialogOps(C.Structure):
            _fields_ = [('paint', C.c_void_p), ('erase', C.c_void_p),
                        ('color', C.c_void_p), ('init', C.c_void_p)]
        cls.ProgressDialogOps = ProgressDialogOps
        cls.lib.stars_progress_dialog.restype = C.c_int16
        cls.lib.stars_progress_dialog.argtypes = [
            C.POINTER(ProgressState), C.c_uint16, C.c_uint16, C.c_uint16,
            C.c_int16, C.POINTER(ProgressDialogOps), C.c_void_p]
        cls.lib.stars_draw_progress_gauge.restype = C.c_int16
        cls.lib.stars_draw_progress_gauge.argtypes = [
            C.POINTER(GaugeRect), C.c_int16, C.c_int16, C.c_void_p, C.c_void_p]
        cls.lib.stars_diagonal_text_out.restype = C.c_int16
        cls.lib.stars_diagonal_text_out.argtypes = [
            C.POINTER(GaugeRect), PTR, C.c_int16, C.c_void_p, C.c_void_p,
            C.c_void_p]
        class RandomSeedOps(C.Structure):
            _fields_ = [('text', C.c_void_p), ('randomize', C.c_void_p),
                        ('end', C.c_void_p)]
        cls.RandomSeedOps = RandomSeedOps
        cls.lib.stars_random_seed_dialog.restype = C.c_int16
        cls.lib.stars_random_seed_dialog.argtypes = [
            C.c_uint16, C.c_uint16, C.c_uint16, C.c_uint32,
            C.POINTER(RandomSeedOps), C.c_void_p]
        class DiskIdentity(C.Structure):
            _fields_ = [('present', C.c_int16),
                        ('serial_word_a', C.c_uint16),
                        ('serial_word_b', C.c_uint16),
                        ('label', PTR), ('free_nibble', C.c_ubyte)]
        cls.DiskIdentity = DiskIdentity
        cls.lib.stars_get_disk_serial_number.restype = C.c_uint16
        cls.lib.stars_get_disk_serial_number.argtypes = [
            C.POINTER(DiskIdentity), PTR]
        class BitmapSource(C.Structure):
            _fields_ = [('width', C.c_int32), ('height', C.c_int32),
                        ('planes', C.c_uint16), ('bits', C.c_uint16),
                        ('pixels', PTR), ('pixel_bytes', C.c_uint32)]
        cls.BitmapSource = BitmapSource
        cls.lib.stars_dib_from_bitmap.restype = C.c_uint32
        cls.lib.stars_dib_from_bitmap.argtypes = [
            C.POINTER(BitmapSource), C.c_uint32, C.c_uint16, PTR,
            C.c_uint16, PTR, C.c_uint32]
        class NewPasswordOps(C.Structure):
            _fields_ = [('text', C.c_void_p), ('mismatch', C.c_void_p),
                        ('write', C.c_void_p), ('end', C.c_void_p)]
        cls.NewPasswordOps = NewPasswordOps
        cls.lib.stars_new_password_dialog.restype = C.c_int16
        cls.lib.stars_new_password_dialog.argtypes = [
            C.c_uint16, C.c_uint16, C.c_uint16, C.c_uint32, C.c_int16,
            C.POINTER(C.c_uint32), C.POINTER(C.c_uint32),
            C.POINTER(NewPasswordOps), C.c_void_p]
        cls.lib.stars_dx_stream_text_out.restype = C.c_int16
        cls.lib.stars_dx_stream_text_out.argtypes = [
            C.POINTER(Runtime), C.c_uint16, C.POINTER(C.c_int16), C.c_int16,
            PTR, C.c_int16, C.c_int16
        ]
        cls.lib.stars_get_right_mouse_move.restype = C.c_int16
        cls.lib.stars_get_right_mouse_move.argtypes = [
            C.POINTER(Runtime), C.c_void_p
        ]
        cls.lib.stars_string_fits_screen.restype = C.c_int16
        cls.lib.stars_string_fits_screen.argtypes = [
            C.POINTER(Runtime), C.c_uint16, C.c_uint16, PTR, C.c_int16
        ]
        cls.lib.stars_wrap_text_out.argtypes = [
            C.POINTER(Runtime), C.c_uint16, C.POINTER(C.c_int16),
            C.POINTER(C.c_int16), PTR, C.c_int16, C.c_int16, C.c_int16,
            C.POINTER(C.c_int16), C.c_int16, C.c_int16
        ]
        cls.lib.stars_draw_fuzzy_border.argtypes = [
            C.POINTER(Runtime), C.c_uint16, C.c_void_p, C.c_uint16,
            C.c_uint32, C.c_uint32
        ]
        cls.lib.stars_black_reserved_palette.restype = C.c_uint16
        cls.lib.stars_black_reserved_palette.argtypes = [C.POINTER(Runtime)]

    def test_get_true_hull_cost_copies_original_fields(self):
        hull = (C.c_ubyte * 123)()
        values = {42: 0x1234, 44: 0x2345, 46: 0x3456, 48: 0x4567}
        for offset, value in values.items():
            hull[offset] = value & 0xff
            hull[offset + 1] = value >> 8
        cost = (C.c_uint16 * 4)()
        self.lib.stars_get_true_hull_cost(hull, cost)
        self.assertEqual(list(cost), [0x2345, 0x3456, 0x4567, 0x1234])

    def test_long_from_serial_char_exhaustive_signed_char_domain(self):
        for ch in range(-128, 128):
            value = ((ch - 0x16) & 0xffff
                     if not 65 <= ch <= 90 else ch - 65)
            if value & 0x8000 or (value < 1 and value < 0x20):
                value ^= 0x15
            expected = value if value < 0x8000 else value - 0x10000
            self.assertEqual(self.lib.stars_long_from_serial_char(ch), expected, ch)

    def test_get_distance_uses_wrapped_word_differences(self):
        cases = [(-10, 7, 20, -5), (32767, 0, -32768, 0),
                 (-32768, -32768, 32767, 32767)]
        for x1, y1, x2, y2 in cases:
            dx = C.c_int16(x2 - x1).value
            dy = C.c_int16(y2 - y1).value
            self.assertEqual(self.lib.stars_get_distance(x1, y1, x2, y2),
                             math.sqrt(dx * dx + dy * dy))

    def test_fleet_point_comparators_preserve_signed_high_word_order(self):
        values = [(-32768, 0), (-1, 0xffff), (-1, 0),
                  (0, 0), (0, 1), (32767, 0xffff)]
        for first in values:
            for second in values:
                record_a = (C.c_ubyte * 12)()
                record_b = (C.c_ubyte * 12)()
                point_a = (C.c_ubyte * 4)()
                for buffer, low, high, offset in (
                        (record_a, first[1], first[0], 8),
                        (record_b, second[1], second[0], 8),
                        (point_a, first[1], first[0], 0)):
                    buffer[offset] = low & 0xff
                    buffer[offset + 1] = low >> 8
                    raw_high = high & 0xffff
                    buffer[offset + 2] = raw_high & 0xff
                    buffer[offset + 3] = raw_high >> 8
                expected = (-1 if first < second else (1 if first > second else 0))
                self.assertEqual(self.lib.stars_compare_fleet_point(
                    record_a, record_b), expected)
                self.assertEqual(self.lib.stars_compare_fleet_point2(
                    point_a, record_b), expected)

    def test_thing_lookup_walks_contiguous_records_and_returns_first_match(self):
        records = (C.c_ubyte * (4 * 6))()
        for i, value in enumerate((7, 12, 12, -3, 91, 4)):
            raw = value & 0xffff
            records[i * 4] = raw & 0xff
            records[i * 4 + 1] = raw >> 8
        self.assertEqual(self.lib.stars_thing_index_from_id(
            records, 6, 4, 12), 1)
        self.assertEqual(self.lib.stars_thing_index_from_id(
            records, 6, 4, 99), -1)

    def test_planet_lookup_uses_direct_or_sorted_search(self):
        planets = (C.c_ubyte * (4 * 5))()
        for i, value in enumerate((2, 8, 19, 33, 47)):
            planets[i * 4] = value & 0xff
            planets[i * 4 + 1] = value >> 8
        self.assertEqual(self.lib.stars_planet_index_from_id(
            planets, 5, 5, 4, 3), 3)
        self.assertEqual(self.lib.stars_planet_index_from_id(
            planets, 5, 50, 4, 19), 2)
        self.assertEqual(self.lib.stars_planet_index_from_id(
            planets, 5, 50, 4, 20), -1)
        self.assertEqual(self.lib.stars_planet_index_from_id(
            planets, 5, 50, 4, 50), -1)

    def test_fleet_pointer_lookup_compares_full_far_pointer(self):
        pointers = (C.c_uint32 * 3)(0x11110022, 0x33330044, 0x11110022)
        self.assertEqual(self.lib.stars_fleet_pointer_index(
            pointers, 3, 0x11110022), 0)
        self.assertEqual(self.lib.stars_fleet_pointer_index(
            pointers, 3, 0x22220022), -1)

    def test_clear_file_preserves_extension_and_calls_unlink(self):
        calls = []
        callback_type = C.CFUNCTYPE(C.c_int16, C.c_void_p, PTR)

        @callback_type
        def unlink(_context, path):
            calls.append(C.string_at(path))
            return 37

        suffixes = (PTR * 2)(C.cast(C.c_char_p(b'.bak'), PTR),
                            C.cast(C.c_char_p(b'.tmp'), PTR))
        output = (C.c_ubyte * 64)()
        base = (C.c_ubyte * 32)(*b'game.sav\0')
        extension = (C.c_ubyte * 8)(*b'.sav\0')
        self.assertEqual(self.lib.stars_clear_file(
            base, extension, suffixes, 2, 1, output, 64,
            C.cast(unlink, C.c_void_p), None), 37)
        self.assertEqual(calls, [b'game..tmp'])

        base2 = (C.c_ubyte * 32)(*b'game\0')
        self.assertEqual(self.lib.stars_clear_file(
            base2, extension, suffixes, 2, 0, output, 64,
            C.cast(unlink, C.c_void_p), None), 37)
        self.assertEqual(calls[-1], b'game.sav.bak')

    def test_file_serial_long_uses_bogi_table_and_base36_remainder(self):
        data = (C.c_ubyte * 65536)()
        serial = 2 * (36 ** 4) + 1000
        target = serial ^ 0xa5a5a5a5
        struct.pack_into('<I', data, 0x766, target + 1)
        self.assertEqual(self.lib.stars_file_bogus_long(data, serial), 0)
        self.assertEqual(self.lib.stars_file_valid_serial_long(data, serial), 1)
        struct.pack_into('<I', data, 0x766, target)
        self.assertEqual(self.lib.stars_file_bogus_long(data, serial), 1)
        self.assertEqual(self.lib.stars_file_valid_serial_long(data, serial), 0)

    def test_player_also_cheater_matches_dead_player_serial_and_config(self):
        serials = (C.c_ubyte * 48)()
        players = (C.c_ubyte * (3 * 0xc0))()
        serial = 0x11223344
        struct.pack_into('<I', serials, 0, serial)
        struct.pack_into('<I', serials, 0x10, serial)
        serials[0x10 + 4] = 1
        struct.pack_into('<H', players, 0xc0 + 0x54, 4)
        valid_type = C.CFUNCTYPE(C.c_int16, C.c_void_p, C.c_uint32)

        @valid_type
        def valid(_context, value): return 1 if value == serial else 0

        callback = C.cast(valid, C.c_void_p)
        self.assertEqual(self.lib.stars_player_also_cheater(
            serials, players, 0, 3, callback, None), 1)
        struct.pack_into('<H', players, 0xc0 + 0x54, 0)
        self.assertEqual(self.lib.stars_player_also_cheater(
            serials, players, 0, 3, callback, None), -1)

        @valid_type
        def invalid(_context, _value): return 0

        self.assertEqual(self.lib.stars_player_also_cheater(
            serials, players, 0, 3, C.cast(invalid, C.c_void_p), None), -1)

    def test_handle_char_forwards_scanner_keys_except_message_edit_focus(self):
        focus_value = [0x2222]
        events = []
        focus_type = C.CFUNCTYPE(C.c_uint16, C.c_void_p)
        send_type = C.CFUNCTYPE(None, C.c_void_p, C.c_uint16, C.c_uint16,
                                C.c_uint16, C.c_uint32)

        @focus_type
        def get_focus(_context): return focus_value[0]

        @send_type
        def send(_context, hwnd, message, character, lparam):
            events.append((hwnd, message, character, lparam))

        callbacks = (C.cast(get_focus, C.c_void_p),
                     C.cast(send, C.c_void_p))
        self.assertEqual(self.lib.stars_handle_char(
            0x1111, 0, 0x3333, 0, ord('+'), 0x12345678,
            *callbacks, None), 1)
        self.assertEqual(events, [(0x1111, 0x102, ord('+'), 0x12345678)])
        self.assertEqual(self.lib.stars_handle_char(
            0x1111, 1, 0x3333, 0, ord('x'), 0, *callbacks, None), 0)
        focus_value[0] = 0x3333
        self.assertEqual(self.lib.stars_handle_char(
            0x1111, 1, 0x3333, 0, ord('v'), 0, *callbacks, None), 0)
        self.assertEqual(self.lib.stars_handle_char(
            0, 0, 0, 0, ord('V'), 0, *callbacks, None), 0)

    def test_order_info_dialog_dispatches_erase_color_and_commands(self):
        events = []
        rect_type = C.CFUNCTYPE(None, C.c_void_p, C.c_uint16,
                                C.POINTER(C.c_uint16))
        fill_type = C.CFUNCTYPE(None, C.c_void_p, C.c_uint16,
                                C.POINTER(C.c_uint16), C.c_uint16)
        color_type = C.CFUNCTYPE(None, C.c_void_p, C.c_uint16, C.c_uint32)
        end_type = C.CFUNCTYPE(None, C.c_void_p, C.c_uint16, C.c_int16)

        @rect_type
        def get_rect(_context, hwnd, rect):
            for i, value in enumerate((1, 2, 30, 40)):
                rect[i] = value
            events.append(('rect', hwnd))

        @fill_type
        def fill(_context, dc, rect, brush):
            events.append(('fill', dc, tuple(rect[:4]), brush))

        @color_type
        def color(_context, dc, value): events.append(('color', dc, value))

        @end_type
        def end(_context, hwnd, result): events.append(('end', hwnd, result))

        callbacks = tuple(C.cast(fn, C.c_void_p) for fn in
                          (get_rect, fill, color, end))
        self.assertEqual(self.lib.stars_order_info_dialog(
            0x100, 0x14, 0x200, 0, 7, 0x12345678, *callbacks, None), 1)
        self.assertEqual(self.lib.stars_order_info_dialog(
            0x100, 0x19, 0x200, 6 << 16, 7, 0x12345678,
            *callbacks, None), 7)
        self.assertEqual(self.lib.stars_order_info_dialog(
            0x100, 0x111, 2, 0, 7, 0, *callbacks, None), 1)
        self.assertEqual(events[0], ('rect', 0x100))
        self.assertEqual(events[1], ('fill', 0x200, (1, 2, 30, 40), 7))
        self.assertEqual(events[2], ('color', 0x200, 0x12345678))
        self.assertEqual(events[3], ('end', 0x100, 1))

    def test_about_initializes_version_timer_and_closes_dialog(self):
        data = (C.c_ubyte * 65536)()
        version = (C.c_ubyte * 8)(*b'v1.0\0\0\0\0')
        events = []
        get_item_type = C.CFUNCTYPE(C.c_uint16, C.c_void_p, C.c_uint16,
                                    C.c_uint16)
        version_type = C.CFUNCTYPE(C.c_void_p, C.c_void_p)
        set_text_type = C.CFUNCTYPE(None, C.c_void_p, C.c_uint16, PTR)
        timer_type = C.CFUNCTYPE(C.c_uint16, C.c_void_p, C.c_uint16,
                                 C.c_uint16, C.c_uint32)
        kill_type = C.CFUNCTYPE(None, C.c_void_p, C.c_uint16, C.c_uint16)
        show_type = C.CFUNCTYPE(None, C.c_void_p, C.c_uint16, C.c_uint16,
                                C.c_uint16)
        end_type = C.CFUNCTYPE(None, C.c_void_p, C.c_uint16, C.c_int16)

        @get_item_type
        def get_item(_context, hwnd, control):
            events.append(('item', hwnd, control))
            return 0x404

        @version_type
        def get_version(_context): return C.addressof(version)

        @set_text_type
        def set_text(_context, hwnd, text):
            events.append(('text', hwnd, C.string_at(text)))

        @timer_type
        def set_timer(_context, hwnd, timer, interval):
            events.append(('timer', hwnd, timer, interval))
            return 9

        @kill_type
        def kill_timer(_context, hwnd, timer):
            events.append(('kill', hwnd, timer))

        @show_type
        def show_order(_context, _hwnd, _template, _owner): pass

        @end_type
        def end_dialog(_context, hwnd, result): events.append(('end', hwnd, result))

        callbacks = tuple(C.cast(fn, C.c_void_p) for fn in (
            get_item, get_version, set_text, set_timer, kill_timer,
            show_order, end_dialog))
        ops = AboutOps(*(callbacks + (None,) * 13))
        data[0x10] = 7
        self.assertEqual(self.lib.stars_about(
            data, 0x100, 0x110, 0, 0, C.byref(ops), None), 1)
        self.assertEqual(struct.unpack_from('<h', data, 0x5994)[0], -11)
        self.assertEqual(struct.unpack_from('<H', data, 0x1a2)[0], 9)
        self.assertEqual(events, [('item', 0x100, 0x401),
                                  ('text', 0x404, b'v1.0'),
                                  ('timer', 0x100, 0x0e, 0x32)])
        self.assertEqual(self.lib.stars_about(
            data, 0x100, 0x111, 1, 0, C.byref(ops), None), 1)
        self.assertEqual(events[-2:], [('kill', 0x100, 9), ('end', 0x100, 1)])

    def test_handle_key_routes_windows_toolbar_scan_orders_and_reports(self):
        data = (C.c_ubyte * 65536)()
        events = []
        focus = {'value': 0x900}
        parent = {0x900: 0x19a, 0x901: 0x902, 0x902: 0x19a}
        point = {'value': (3, 4)}
        active = {'value': 0x111}
        key_state = {'value': -1}

        active_type = C.CFUNCTYPE(C.c_uint16, C.c_void_p)
        destroy_type = C.CFUNCTYPE(None, C.c_void_p, C.c_uint16)
        send_type = C.CFUNCTYPE(None, C.c_void_p, C.c_uint16, C.c_uint16,
                                C.c_uint16, C.c_uint32)
        focus_type = C.CFUNCTYPE(C.c_uint16, C.c_void_p)
        parent_type = C.CFUNCTYPE(C.c_uint16, C.c_void_p, C.c_uint16)
        terminate_type = C.CFUNCTYPE(None, C.c_void_p, C.c_uint16)
        cursor_type = C.CFUNCTYPE(None, C.c_void_p, C.POINTER(C.c_int16),
                                  C.POINTER(C.c_int16))
        point_type = C.CFUNCTYPE(C.c_uint16, C.c_void_p, C.c_int16,
                                 C.c_int16)
        item_type = C.CFUNCTYPE(C.c_uint16, C.c_void_p, C.c_uint16,
                                C.c_uint16)
        key_type = C.CFUNCTYPE(C.c_int16, C.c_void_p, C.c_uint16)
        invalidate_type = C.CFUNCTYPE(None, C.c_void_p, C.c_uint16)
        down_type = C.CFUNCTYPE(C.c_int16, C.c_void_p, C.c_uint16)
        execute_type = C.CFUNCTYPE(None, C.c_void_p, C.c_uint16,
                                   C.c_uint16)
        delete_type = C.CFUNCTYPE(None, C.c_void_p, C.c_uint16)
        lookup_type = C.CFUNCTYPE(None, C.c_void_p, C.c_int16, PTR)
        draw_type = C.CFUNCTYPE(None, C.c_void_p, C.c_uint16, C.c_uint16)
        report_type = C.CFUNCTYPE(None, C.c_void_p, C.c_int16, C.c_int16,
                                  C.c_uint16, C.c_uint16, C.c_int16)

        @active_type
        def get_active(_context): return active['value']

        @destroy_type
        def destroy(_context, window): events.append(('destroy', window))

        @send_type
        def send(_context, window, message, wparam, lparam):
            events.append(('send', window, message, wparam, lparam))

        @focus_type
        def get_focus(_context): return focus['value']

        @parent_type
        def get_parent(_context, window): return parent.get(window, 0)

        @terminate_type
        def terminate(_context, escape): events.append(('terminate', escape))

        @cursor_type
        def get_cursor(_context, x, y):
            x[0], y[0] = point['value']

        @point_type
        def window_from_point(_context, x, y):
            events.append(('point', x, y))
            return 0x190

        @item_type
        def get_item(_context, _window, _control): return 0

        @key_type
        def get_key_state(_context, _key): return key_state['value']

        @invalidate_type
        def invalidate(_context, window): events.append(('invalidate', window))

        @down_type
        def is_down(_context, _button): return 0

        @execute_type
        def execute(_context, button, down):
            events.append(('button', button, down))

        @delete_type
        def delete_waypoint(_context, backup): events.append(('delete', backup))

        @lookup_type
        def lookup(_context, fleet, fleet_data):
            events.append(('lookup', fleet, C.addressof(fleet_data.contents)))

        @draw_type
        def draw(_context, dc, flags): events.append(('draw', dc, flags))

        @report_type
        def report(_context, x, y, report_id, column, row):
            events.append(('report', x, y, report_id, column, row))

        functions = (get_active, destroy, send, get_focus, get_parent,
                     terminate, get_cursor, window_from_point, get_item,
                     get_key_state, invalidate, is_down, execute,
                     delete_waypoint, lookup, draw, report)
        ops = HandleKeyOps(*(C.cast(fn, C.c_void_p) for fn in functions))
        data[0x18e:0x190] = (0x111).to_bytes(2, 'little')
        self.assertEqual(self.lib.stars_handle_key(
            data, 0, 0x100, 0x1b, 0, C.byref(ops), None), 1)
        self.assertEqual(events[-1], ('destroy', 0x111))

        data[0x18e:0x190] = b'\0\0'
        data[0xb7e:0xb80] = (0x222).to_bytes(2, 'little')
        self.assertEqual(self.lib.stars_handle_key(
            data, 0, 0x100, 0x1b, 0, C.byref(ops), None), 1)
        self.assertEqual(events[-1], ('send', 0x222, 0x202, 0, 0))

        data[0xb7e:0xb80] = b'\0\0'
        data[0x19a:0x19c] = (0x19a).to_bytes(2, 'little')
        self.assertEqual(self.lib.stars_handle_key(
            data, 0, 0x101, 0x1b, 0, C.byref(ops), None), 0)
        self.assertEqual(events[-1], ('terminate', 1))

        data[0x190:0x192] = (0x190).to_bytes(2, 'little')
        self.assertEqual(self.lib.stars_handle_key(
            data, 0, 0x102, 0x10, 0, C.byref(ops), None), 0)
        self.assertEqual(events[-2:], [('point', 3, 4),
                                       ('send', 0x190, 0x20, 0x190, 0)])

        data[0x198:0x19a] = (0x198).to_bytes(2, 'little')
        focus['value'] = 0x999
        self.assertEqual(self.lib.stars_handle_key(
            data, 0, 0x100, 0x37, 0, C.byref(ops), None), 1)
        self.assertEqual(events[-2:], [('button', 7, 1), ('invalidate', 0x19a)])

        data[0x495a:0x495c] = (2).to_bytes(2, 'little')
        data[0x4960:0x4962] = (1).to_bytes(2, 'little')
        data[0x49d6:0x49d8] = (0x4a00).to_bytes(2, 'little')
        data[0x4a00 + 0x12 + 10:0x4a00 + 0x12 + 14] = (3 << 4).to_bytes(4, 'little')
        self.assertEqual(self.lib.stars_handle_key(
            data, 0, 0x100, 0xbc, 0, C.byref(ops), None), 1)
        self.assertEqual(int.from_bytes(data[0x4a00 + 0x12 + 10:0x4a00 + 0x12 + 14], 'little') & 0xf0, 0x20)
        self.assertEqual(events[-2][0], 'lookup')
        self.assertEqual(events[-1], ('draw', 0, 0x4220))

        self.assertEqual(self.lib.stars_handle_key(
            data, 0, 0x100, 0xdb, 0, C.byref(ops), None), 1)
        self.assertEqual(events[-1], ('report', 0, 0, 2, 0, -2))
        self.assertEqual(self.lib.stars_handle_key(
            data, 0, 0x100, 8, 0, C.byref(ops), None), 1)
        self.assertEqual(events[-1], ('delete', 8))

    def test_batch_setup_reads_to_newline_and_preserves_far_pointer_slots(self):
        data = (C.c_ubyte * 65536)()
        data[0x56a2:0x56a2 + 5] = b'input'
        batch = (C.c_ubyte * 8)(*b'abc\ndef')
        events = []
        open_type = C.CFUNCTYPE(C.c_int16, C.c_void_p, PTR, C.c_uint16)
        length_type = C.CFUNCTYPE(C.c_uint16, C.c_void_p, C.c_int16)
        alloc_type = C.CFUNCTYPE(C.c_uint32, C.c_void_p, C.c_uint16,
                                 C.c_int16)
        memory_type = C.CFUNCTYPE(C.c_void_p, C.c_void_p, C.c_uint32)
        read_type = C.CFUNCTYPE(C.c_uint16, C.c_void_p, C.c_int16, PTR,
                                C.c_uint16)
        close_type = C.CFUNCTYPE(None, C.c_void_p, C.c_int16)

        @open_type
        def open_file(_context, name, mode):
            events.append(('open', C.string_at(name), mode))
            return 7

        @length_type
        def file_length(_context, handle):
            events.append(('length', handle))
            return 4

        @alloc_type
        def allocate(_context, count, heap_type):
            events.append(('allocate', count, heap_type))
            return 0x12340020

        @memory_type
        def memory(_context, pointer):
            self.assertEqual(pointer, 0x12340020)
            return C.addressof(batch)

        @read_type
        def read_file(_context, handle, output, count):
            events.append(('read', handle, count))
            C.memmove(output, batch, count)
            return count

        @close_type
        def close_file(_context, handle): events.append(('close', handle))

        functions = (open_file, file_length, allocate, memory, read_file,
                     close_file)
        ops = BatchOps(*(C.cast(fn, C.c_void_p) for fn in functions))
        self.assertEqual(self.lib.stars_set_up_batch_processing(
            data, C.byref(ops), None), 1)
        self.assertEqual(bytes(data[0x56a2:0x56a6]), b'ab\0u')
        self.assertEqual(struct.unpack_from('<I', data, 0x19e)[0],
                         0x12340024)
        self.assertEqual(struct.unpack_from('<I', data, 0x51b0)[0],
                         0x12340024)
        self.assertEqual(events, [('open', b'input', 0x20), ('length', 7),
                                  ('allocate', 4, 9), ('read', 7, 4),
                                  ('close', 7)])

    def test_init_instance_clears_flags_creates_frame_loads_accelerators(self):
        data = (C.c_ubyte * 65536)()
        data[0x1d6:0x1d6 + 6] = b'frame\0'
        data[0x731:0x731 + 6] = b'class\0'
        struct.pack_into('<HHHHH', data, 0x2290, 10, 20, 640, 480, 1)
        struct.pack_into('<H', data, 0x5310, 0x55)
        events = []
        state = {'hwnd': 0x700}
        read_type = C.CFUNCTYPE(None, C.c_void_p)
        create_type = C.CFUNCTYPE(C.c_uint16, C.c_void_p, PTR, PTR,
                                  C.c_uint32, C.c_int16, C.c_int16, C.c_int16,
                                  C.c_int16, C.c_uint16)
        load_type = C.CFUNCTYPE(C.c_uint16, C.c_void_p, C.c_uint16,
                                C.c_uint16)
        show_type = C.CFUNCTYPE(None, C.c_void_p, C.c_uint16, C.c_uint16)

        @read_type
        def read_ini(_context): events.append(('read_ini',))

        @create_type
        def create(_context, first, second, style, left, top, right, bottom,
                   instance):
            events.append(('create', C.string_at(first), C.string_at(second),
                            style, left, top, right, bottom, instance))
            return state['hwnd']

        @load_type
        def load(_context, instance, resource):
            events.append(('load', instance, resource))
            return 8 if resource == 0x74 else 9

        @show_type
        def show(_context, hwnd, command): events.append(('show', hwnd, command))

        functions = (read_ini, create, load, show)
        ops = InitInstanceOps(*(C.cast(fn, C.c_void_p) for fn in functions))
        data[0x229a:0x229c] = (0xffff).to_bytes(2, 'little')
        self.assertEqual(self.lib.stars_init_instance(
            data, 1, C.byref(ops), None), 1)
        self.assertEqual(struct.unpack_from('<H', data, 0x229a)[0], 0xfe1a)
        self.assertEqual(struct.unpack_from('<h', data, 0x22a0)[0], -1)
        self.assertEqual(struct.unpack_from('<H', data, 0x258c)[0], 0x700)
        self.assertEqual(struct.unpack_from('<H', data, 0x23e6)[0], 8)
        self.assertEqual(struct.unpack_from('<H', data, 0x4a58)[0], 9)
        self.assertEqual(events, [
            ('read_ini',),
            ('create', b'frame', b'class', 0xcf0000, 10, 20, 640, 480, 0x55),
            ('load', 0x55, 0x74), ('load', 0x55, 0x438),
            ('show', 0x700, 3), ('show', 0x700, 0)])

    def test_find_something_prefers_home_planet_then_scans_planets_and_fleets(self):
        data = (C.c_ubyte * 65536)()
        player = 2
        home_slot = 0x59a2 + player * 0xc0 + 8
        data[0x18c:0x18e] = player.to_bytes(2, 'little')
        data[home_slot:home_slot + 2] = (55).to_bytes(2, 'little')
        events = []
        home = (C.c_ubyte * 8)()
        home[0:2] = (55).to_bytes(2, 'little')
        home[2:4] = player.to_bytes(2, 'little')
        planet0 = (C.c_ubyte * 8)()
        planet0[0:2] = (11).to_bytes(2, 'little')
        planet0[2:4] = (1).to_bytes(2, 'little')
        planet1 = (C.c_ubyte * 8)()
        planet1[0:2] = (12).to_bytes(2, 'little')
        planet1[2:4] = player.to_bytes(2, 'little')
        fleet_table = (C.c_ubyte * 4)()
        fleet = (C.c_ubyte * 8)()
        fleet[0:2] = (77).to_bytes(2, 'little')
        fleet[2:4] = player.to_bytes(2, 'little')
        memory_map = {0x50000020: home, 0x10000020: planet0,
                      0x10000058: planet1, 0x20000020: fleet_table,
                      0x30000020: fleet}
        C.memmove(fleet_table, (0x30000020).to_bytes(4, 'little'), 4)
        state = {'home': True}
        planet_from_id_type = C.CFUNCTYPE(C.c_uint32, C.c_void_p, C.c_int16)
        memory_type = C.CFUNCTYPE(C.c_void_p, C.c_void_p, C.c_uint32)
        select_type = C.CFUNCTYPE(None, C.c_void_p, C.c_uint16, C.c_uint16)

        @planet_from_id_type
        def planet_from_id(_context, _planet_id):
            return 0x50000020 if state['home'] else 0

        @memory_type
        def memory(_context, token):
            value = memory_map.get(token)
            return C.addressof(value) if value is not None else 0

        @select_type
        def select_planet(_context, mode, planet_id):
            events.append(('planet', mode, planet_id))

        @select_type
        def select_fleet(_context, mode, fleet_id):
            events.append(('fleet', mode, fleet_id))

        functions = (planet_from_id, memory, select_planet, select_fleet)
        ops = FindSomethingOps(*(C.cast(fn, C.c_void_p) for fn in functions))
        self.assertEqual(self.lib.stars_find_something_and_select_it(
            data, C.byref(ops), None), 1)
        self.assertEqual(events, [('planet', 0, 55)])

        state['home'] = False
        data[0xf6:0xfa] = (0x10000020).to_bytes(4, 'little')
        data[0x4b7c:0x4b7e] = (2).to_bytes(2, 'little')
        self.assertEqual(self.lib.stars_find_something_and_select_it(
            data, C.byref(ops), None), 1)
        self.assertEqual(events[-1], ('planet', 0, 12))

        data[0x4b7c:0x4b7e] = (0).to_bytes(2, 'little')
        data[0xfa:0xfe] = (0x20000020).to_bytes(4, 'little')
        data[0x5356:0x5358] = (1).to_bytes(2, 'little')
        self.assertEqual(self.lib.stars_find_something_and_select_it(
            data, C.byref(ops), None), 1)
        self.assertEqual(events[-1], ('fleet', 0, 77))

    def test_restore_selection_repairs_objects_and_updates_messages(self):
        data = (C.c_ubyte * 65536)()
        events = []
        player = 2
        home_slot = 0x59a2 + player * 0xc0 + 8
        planet = (C.c_ubyte * 8)()
        planet[0:2] = (10).to_bytes(2, 'little')
        planet[2:4] = player.to_bytes(2, 'little')
        data[0x18c:0x18e] = player.to_bytes(2, 'little')
        data[home_slot:home_slot + 2] = (10).to_bytes(2, 'little')
        data[0x22a0:0x22a2] = player.to_bytes(2, 'little')
        data[0x22a2:0x22a6] = (0x12345678).to_bytes(4, 'little')
        data[0x70:0x74] = (0x12345678).to_bytes(4, 'little')
        data[0x229a:0x229c] = (0x20).to_bytes(2, 'little')
        data[0x229e:0x22a0] = (10).to_bytes(2, 'little')
        data[0x229c:0x229e] = (7).to_bytes(2, 'little')
        data[0x82:0x84] = (7).to_bytes(2, 'little')
        data[0x22a8:0x22aa] = (2).to_bytes(2, 'little')
        data[0x0afe:0x0b00] = (5).to_bytes(2, 'little')
        data[0x0b0e:0x0b10] = (0).to_bytes(2, 'little')
        data[0x07ca:0x07cc] = (0x800).to_bytes(2, 'little')
        data[0x520c:0x520e] = (0).to_bytes(2, 'little')
        data[0x198:0x19a] = (0x333).to_bytes(2, 'little')
        memory_map = {0x50000020: planet}

        fleet_type = C.CFUNCTYPE(C.c_uint32, C.c_void_p, C.c_int16)
        planet_type = C.CFUNCTYPE(C.c_uint32, C.c_void_p, C.c_int16)
        memory_type = C.CFUNCTYPE(C.c_void_p, C.c_void_p, C.c_uint32)
        select_type = C.CFUNCTYPE(None, C.c_void_p, C.c_uint16, C.c_uint16)
        next_type = C.CFUNCTYPE(C.c_int16, C.c_void_p, C.c_int16)
        title_type = C.CFUNCTYPE(None, C.c_void_p, C.c_uint16)
        invalidate_type = C.CFUNCTYPE(None, C.c_void_p, C.c_uint16,
                                      C.c_uint16)
        find_type = C.CFUNCTYPE(None, C.c_void_p)
        advance_type = C.CFUNCTYPE(None, C.c_void_p)

        @fleet_type
        def fleet_from_id(_context, _object_id): return 0

        @planet_type
        def planet_from_id(_context, _object_id): return 0x50000020

        @memory_type
        def memory(_context, token):
            value = memory_map.get(token)
            return C.addressof(value) if value is not None else 0

        @select_type
        def select_fleet(_context, mode, object_id):
            events.append(('fleet', mode, object_id))

        @select_type
        def select_planet(_context, mode, object_id):
            events.append(('planet', mode, object_id))

        @next_type
        def next_message(_context, mode):
            events.append(('next', mode))
            return 4

        @title_type
        def set_title(_context, hwnd): events.append(('title', hwnd))

        @invalidate_type
        def invalidate(_context, hwnd, erase): events.append(('invalidate', hwnd, erase))

        @find_type
        def find_selection(_context): events.append(('find',))

        @advance_type
        def advance(_context): events.append(('advance',))

        functions = (fleet_from_id, planet_from_id, memory, select_fleet,
                     select_planet, next_message, set_title, invalidate,
                     find_selection, advance)
        ops = RestoreSelectionOps(*(C.cast(fn, C.c_void_p) for fn in functions))
        self.lib.stars_restore_selection(data, C.byref(ops), None)
        self.assertEqual(struct.unpack_from('<H', data, 0x229a)[0], 0)
        self.assertEqual(struct.unpack_from('<H', data, 0xb00)[0], 4)
        self.assertEqual(struct.unpack_from('<H', data, 0x520c)[0], 4)
        self.assertEqual(events, [('planet', 0, 10), ('next', 0),
                                  ('title', 0x333), ('invalidate', 0x333, 1),
                                  ('advance',)])

        data[0x229a:0x229c] = (0x20).to_bytes(2, 'little')
        data[0x229e:0x22a0] = (99).to_bytes(2, 'little')
        self.lib.stars_restore_selection(data, C.byref(ops), None)
        self.assertIn(('planet', 0, 99), events[-5:])

    def test_host_options_dialog_dispatches_paint_background_commands_and_help(self):
        data = (C.c_ubyte * 65536)()
        data[0x10:0x12] = (0x55aa).to_bytes(2, 'little')
        data[0x22ba:0x22be] = (0x12345678).to_bytes(4, 'little')
        data[0x14a:0x153] = b'help.hlp\0'
        events = []
        begin_type = C.CFUNCTYPE(C.c_uint16, C.c_void_p, C.c_uint16, PTR)
        end_paint_type = C.CFUNCTYPE(None, C.c_void_p, C.c_uint16, PTR)
        draw_type = C.CFUNCTYPE(None, C.c_void_p, C.c_uint16, C.c_uint16,
                                C.c_int16)
        rect_type = C.CFUNCTYPE(None, C.c_void_p, C.c_uint16,
                                C.POINTER(Rect))
        fill_type = C.CFUNCTYPE(None, C.c_void_p, C.c_uint16,
                                C.POINTER(Rect), C.c_uint16)
        color_type = C.CFUNCTYPE(None, C.c_void_p, C.c_uint16, C.c_uint32)
        end_dialog_type = C.CFUNCTYPE(None, C.c_void_p, C.c_uint16,
                                      C.c_int16)
        help_type = C.CFUNCTYPE(None, C.c_void_p, C.c_uint16, PTR,
                                C.c_uint16, C.c_uint16)

        @begin_type
        def begin(_context, hwnd, _paint):
            events.append(('begin', hwnd))
            return 0x44

        @end_paint_type
        def end_paint(_context, hwnd, _paint):
            events.append(('end', hwnd))

        @draw_type
        def draw(_context, hwnd, dc, mode):
            events.append(('draw', hwnd, dc, mode))

        @rect_type
        def get_rect(_context, hwnd, rect):
            events.append(('rect', hwnd))
            rect.contents.left = 1
            rect.contents.top = 2
            rect.contents.right = 30
            rect.contents.bottom = 40

        @fill_type
        def fill(_context, dc, rect, brush):
            events.append(('fill', dc, (rect.contents.left, rect.contents.top,
                                        rect.contents.right, rect.contents.bottom),
                           brush))

        @color_type
        def set_color(_context, dc, color):
            events.append(('color', dc, color))

        @end_dialog_type
        def end_dialog(_context, hwnd, result):
            events.append(('dialog', hwnd, result))

        @help_type
        def win_help(_context, hwnd, filename, help_type_value, command):
            events.append(('help', hwnd, C.string_at(filename),
                           help_type_value, command))

        functions = (begin, end_paint, draw, get_rect, fill, set_color,
                     end_dialog, win_help)
        ops = HostOptionsOps(*(C.cast(fn, C.c_void_p) for fn in functions))
        self.assertEqual(self.lib.stars_host_options_dialog(
            data, 0x10, 0x000f, 0, 0, C.byref(ops), None), 1)
        self.assertEqual(events[:3], [('begin', 0x10),
                                      ('draw', 0x10, 0x44, -1),
                                      ('end', 0x10)])

        self.assertEqual(self.lib.stars_host_options_dialog(
            data, 0x10, 0x14, 0x66, 0, C.byref(ops), None), 1)
        self.assertEqual(events[-2:], [
            ('rect', 0x10), ('fill', 0x66, (1, 2, 30, 40), 0x55aa)])
        self.assertEqual(self.lib.stars_host_options_dialog(
            data, 0x10, 0x19, 0x88, 0, C.byref(ops), None), 0x55aa)
        self.assertEqual(events[-1], ('color', 0x88, 0x12345678))
        self.assertEqual(self.lib.stars_host_options_dialog(
            data, 0x10, 0x110, 0x66, 0, C.byref(ops), None), 1)
        self.assertEqual(self.lib.stars_host_options_dialog(
            data, 0x10, 0x111, 2, 0, C.byref(ops), None), 1)
        self.assertEqual(self.lib.stars_host_options_dialog(
            data, 0x10, 0x111, 1, 0, C.byref(ops), None), 1)
        self.assertEqual(self.lib.stars_host_options_dialog(
            data, 0x10, 0x111, 0x76, 0, C.byref(ops), None), 1)
        self.assertEqual(self.lib.stars_host_options_dialog(
            data, 0x10, 2, 0, 0, C.byref(ops), None), 0)
        self.assertEqual(self.lib.stars_host_options_dialog(
            data, 0x10, 0x111, 3, 0, C.byref(ops), None), 0)
        self.assertEqual(events[-3:], [('dialog', 0x10, 0),
                                       ('dialog', 0x10, 1),
                                       ('help', 0x10, b'help.hlp', 1, 0x440)])

    def test_find_turns_outstanding_maps_log_statuses_and_restores_flags(self):
        data = (C.c_ubyte * 65536)()
        data[0x78:0x7a] = (3).to_bytes(2, 'little')
        data[0x7ca:0x7cc] = (0x0002).to_bytes(2, 'little')
        data[0x7cc:0x7ce] = (0).to_bytes(2, 'little')
        data[0x1d2:0x1d6] = (10).to_bytes(4, 'little')
        # Player 1 is a log-error entry; player 2 is dead.
        data[0x59a2 + 0xc0 + 6:0x59a2 + 0xc0 + 8] = (0x0200).to_bytes(2, 'little')
        data[0x59a2 + 2 * 0xc0 + 0x54] = 1
        events = []
        check_type = C.CFUNCTYPE(C.c_int16, C.c_void_p, C.c_int16,
                                 C.POINTER(C.c_int16))
        tick_type = C.CFUNCTYPE(C.c_uint32, C.c_void_p)

        @check_type
        def check(_context, player, error):
            events.append(('check', player))
            error[0] = 0
            return 0

        @tick_type
        def ticks(_context):
            events.append(('tick',))
            return 0x12345678

        ops = FindTurnsOps(C.cast(check, C.c_void_p), C.cast(ticks, C.c_void_p))
        self.assertEqual(self.lib.stars_find_turns_outstanding(
            data, C.byref(ops), None), 1)
        self.assertEqual([int.from_bytes(data[0x65a4 + i * 2:0x65a6 + i * 2],
                                         'little', signed=True) for i in range(3)],
                         [1, 0, -1])
        self.assertEqual(events, [('check', 0), ('tick',), ('check', 2),
                                  ('tick',)])
        self.assertEqual(int.from_bytes(data[0x1d2:0x1d6], 'little'),
                         0x12345678)
        self.assertEqual(int.from_bytes(data[0x74c:0x74e], 'little'), 0)
        self.assertEqual(int.from_bytes(data[0x7ca:0x7cc], 'little') & 2, 0)
        self.assertEqual(int.from_bytes(data[0x7cc:0x7ce], 'little') & 0x10, 0)

        # Error codes 1c, 1d and other values map to 4, 5 and 3.
        data[0x59a2 + 0xc0 + 6:0x59a2 + 0xc0 + 8] = (0).to_bytes(2, 'little')
        data[0x59a2 + 2 * 0xc0 + 0x54] = 0
        data[0x65a4:0x65aa] = b'\0' * 6

        @check_type
        def coded_check(_context, player, error):
            error[0] = {0: 0x1c, 1: 0x1d, 2: 7}[player]
            return 0

        ops = FindTurnsOps(C.cast(coded_check, C.c_void_p), C.cast(ticks, C.c_void_p))
        self.assertEqual(self.lib.stars_find_turns_outstanding(
            data, C.byref(ops), None), 3)
        self.assertEqual([int.from_bytes(data[0x65a4 + i * 2:0x65a6 + i * 2],
                                         'little', signed=True) for i in range(3)],
                         [4, 5, 3])

    def test_log_change_battle_plan_writes_record_and_advances_tutor(self):
        data = (C.c_ubyte * 65536)()
        plan = (C.c_ubyte * 36)()
        events = []
        data[0x7ca:0x7cc] = (0x0800).to_bytes(2, 'little')
        data[0x18c:0x18e] = (0).to_bytes(2, 'little')
        write_type = C.CFUNCTYPE(None, C.c_void_p, PTR, C.c_int16)
        advance_type = C.CFUNCTYPE(None, C.c_void_p)

        @write_type
        def write(_context, received_plan, log):
            events.append(('write', C.addressof(received_plan.contents), log))

        @advance_type
        def advance(_context): events.append(('advance',))

        ops = LogChangeBtlplanOps(C.cast(write, C.c_void_p),
                                  C.cast(advance, C.c_void_p))
        self.lib.stars_log_change_btlplan(data, plan, C.byref(ops), None)
        self.assertEqual(events, [('write', C.addressof(plan), 1),
                                  ('advance',)])
        self.assertEqual(int.from_bytes(data[0x520c:0x520e], 'little') & 4, 4)

        data[0x18c:0x18e] = (1).to_bytes(2, 'little')
        events[:] = []
        self.lib.stars_log_change_btlplan(data, plan, C.byref(ops), None)
        self.assertEqual(events, [('write', C.addressof(plan), 1)])

    def test_log_split_fleet_writes_memory_or_sets_live_fleet_flag(self):
        data = (C.c_ubyte * 65536)()
        events = []
        write_type = C.CFUNCTYPE(None, C.c_void_p, C.c_uint16, C.c_uint16,
                                 PTR)
        find_type = C.CFUNCTYPE(C.c_void_p, C.c_void_p, C.c_int16)
        fleet = (C.c_ubyte * 0x11a)()
        fleet[0x118:0x11a] = (0xffff).to_bytes(2, 'little')

        @write_type
        def write(_context, record_type, size, payload):
            events.append(('write', record_type, size,
                           bytes(payload[:size])))

        @find_type
        def find(_context, object_id):
            events.append(('find', object_id))
            return C.addressof(fleet)

        ops = LogSplitFleetOps(C.cast(write, C.c_void_p),
                               C.cast(find, C.c_void_p))
        self.lib.stars_log_split_fleet(data, -7, C.byref(ops), None)
        self.assertEqual(events, [('write', 0x18, 2, b'\xf9\xff')])

        data[0x7ca:0x7cc] = (2).to_bytes(2, 'little')
        events[:] = []
        self.lib.stars_log_split_fleet(data, 12, C.byref(ops), None)
        self.assertEqual(events, [('find', 12)])
        self.assertEqual(int.from_bytes(fleet[0x118:0x11a], 'little') & 0x20,
                         0x20)

        data[0x7ca:0x7cc] = (0).to_bytes(2, 'little')
        events[:] = []
        self.lib.stars_log_split_fleet(data, 1, C.byref(ops), None)
        self.assertEqual(events, [('write', 0x18, 2, b'\x01\0')])

    def test_set_build_selection_updates_slot_and_redraws_changed_items(self):
        data = (C.c_ubyte * 65536)()
        data[0x0c7c:0x0c7e] = (2).to_bytes(2, 'little')
        data[0x0c90:0x0c92] = (0x321).to_bytes(2, 'little')
        events = []
        rect_type = C.CFUNCTYPE(None, C.c_void_p, C.c_uint16,
                                C.POINTER(Rect))
        draw_slot_type = C.CFUNCTYPE(None, C.c_void_p, C.c_uint16,
                                     C.c_uint16, C.POINTER(Rect), C.c_int16)
        draw_component_type = C.CFUNCTYPE(None, C.c_void_p, C.c_uint16,
                                          C.c_uint16, C.c_int16)

        @rect_type
        def get_rect(_context, hwnd, rect):
            events.append(('rect', hwnd))
            rect.contents.left, rect.contents.top = 1, 2
            rect.contents.right, rect.contents.bottom = 30, 40

        @draw_slot_type
        def draw_slot(_context, hwnd, dc, rect, item):
            events.append(('slot', hwnd, dc, item,
                           (rect.contents.left, rect.contents.top,
                            rect.contents.right, rect.contents.bottom)))

        @draw_component_type
        def draw_component(_context, hwnd, dc, item):
            events.append(('component', hwnd, dc, item))

        ops = BuildSelectionOps(C.cast(get_rect, C.c_void_p),
                                C.cast(draw_slot, C.c_void_p),
                                C.cast(draw_component, C.c_void_p))
        self.lib.stars_set_build_selection(data, 5, C.byref(ops), None)
        self.assertEqual(int.from_bytes(data[0x0c7c:0x0c7e], 'little'), 5)
        self.assertEqual(events, [
            ('rect', 0x321), ('slot', 0x321, 0, 2, (1, 2, 30, 40)),
            ('slot', 0x321, 0, 5, (1, 2, 30, 40)),
            ('component', 0x321, 0, -1)])

        events[:] = []
        self.lib.stars_set_build_selection(data, 5, C.byref(ops), None)
        self.assertEqual(events, [])

        self.lib.stars_set_build_selection(data, -1, C.byref(ops), None)
        self.assertEqual(events[-2:], [('slot', 0x321, 0, 5, (1, 2, 30, 40)),
                                       ('component', 0x321, 0, -1)])

    def test_set_planet_title_bar_selects_planet_fleet_or_default_title(self):
        data = (C.c_ubyte * 65536)()
        events = []
        name_type = C.CFUNCTYPE(C.c_void_p, C.c_void_p, C.c_int16)
        string_type = C.CFUNCTYPE(C.c_void_p, C.c_void_p, C.c_uint16)
        set_text_type = C.CFUNCTYPE(None, C.c_void_p, C.c_uint16, PTR)
        strings = {0x33a: C.create_string_buffer(b'No selection'),
                   0x33b: C.create_string_buffer(b'Planet: ')}
        planets = {7: C.create_string_buffer(b'Earth')}
        fleets = {9: C.create_string_buffer(b'Wolfpack')}

        @name_type
        def planet_name(_context, object_id):
            return C.addressof(planets[object_id])

        @name_type
        def fleet_name(_context, object_id):
            return C.addressof(fleets[object_id])

        @string_type
        def get_string(_context, string_id):
            return C.addressof(strings[string_id])

        @set_text_type
        def set_text(_context, hwnd, text):
            events.append((hwnd, C.string_at(text)))

        ops = PlanetTitleOps(C.cast(planet_name, C.c_void_p),
                             C.cast(fleet_name, C.c_void_p),
                             C.cast(get_string, C.c_void_p),
                             C.cast(set_text, C.c_void_p))
        data[0x495a:0x495c] = (1).to_bytes(2, 'little')
        data[0x49ee:0x49f0] = (7).to_bytes(2, 'little')
        self.lib.stars_set_planet_title_bar(
            data, 0x111, C.byref(ops), None)
        self.assertEqual(events[-1], (0x111, b'Planet: Earth'))

        data[0x495a:0x495c] = (2).to_bytes(2, 'little')
        data[0x4972:0x4974] = (9).to_bytes(2, 'little')
        self.lib.stars_set_planet_title_bar(
            data, 0x111, C.byref(ops), None)
        self.assertEqual(events[-1], (0x111, b'Wolfpack'))

        data[0x495a:0x495c] = (3).to_bytes(2, 'little')
        self.lib.stars_set_planet_title_bar(
            data, 0x111, C.byref(ops), None)
        self.assertEqual(events[-1], (0x111, b'No selection'))

    def test_get_prev_log_rt_extracts_header_and_payload_or_no_record(self):
        data = (C.c_ubyte * 65536)()
        log = (C.c_ubyte * 32)()
        header = (C.c_ubyte * 2)()
        payload = (C.c_ubyte * 16)()
        data[0x9a6:0x9a8] = (4).to_bytes(2, 'little')
        log[4:9] = (0x8003).to_bytes(2, 'little') + b'abc'
        self.assertEqual(self.lib.stars_get_prev_log_rt(
            data, log, header, payload), 1)
        self.assertEqual(bytes(header), b'\x03\x80')
        self.assertEqual(bytes(payload[:3]), b'abc')

        data[0x9a6:0x9a8] = (0xffff).to_bytes(2, 'little')
        self.assertEqual(self.lib.stars_get_prev_log_rt(
            data, log, header, payload), 0)

    def test_run_log_file_dispatches_length_prefixed_records_and_restores_mode(self):
        data = (C.c_ubyte * 65536)()
        log = (C.c_ubyte * 32)()
        data[0x9a4:0x9a6] = (9).to_bytes(2, 'little')
        data[0x9a8:0x9aa] = (7).to_bytes(2, 'little')
        data[0x7ce:0x7d0] = (0xffff).to_bytes(2, 'little')
        # type 2, length 3, then type 1, length 2.
        log[0:5] = (0x0803).to_bytes(2, 'little') + b'abc'
        log[5:9] = (0x0402).to_bytes(2, 'little') + b'xy'
        events = []
        record_type = C.CFUNCTYPE(C.c_uint16, C.c_void_p, C.c_uint16,
                                  C.c_uint16, PTR)

        @record_type
        def run_record(_context, record_kind, size, payload):
            events.append((record_kind, size, bytes(payload[:size])))
            return 1 if record_kind == 2 else 0

        self.assertEqual(self.lib.stars_run_log_file(
            data, log, C.cast(run_record, C.c_void_p), None), 0)
        self.assertEqual(events, [(2, 3, b'abc'), (1, 2, b'xy')])
        self.assertEqual(int.from_bytes(data[0x9a8:0x9aa], 'little'), 7)
        self.assertEqual(int.from_bytes(data[0x7ce:0x7d0], 'little') & 0x400, 0)

        data[0x9a4:0x9a6] = b'\0\0'
        self.assertEqual(self.lib.stars_run_log_file(
            data, log, C.cast(run_record, C.c_void_p), None), 1)

    def test_enum_log_rts_visits_records_and_honors_visitor_stop(self):
        data = (C.c_ubyte * 65536)()
        log = (C.c_ubyte * 32)()
        data[0x9a4:0x9a6] = (9).to_bytes(2, 'little')
        log[0:5] = (0x0803).to_bytes(2, 'little') + b'abc'
        log[5:9] = (0x0402).to_bytes(2, 'little') + b'xy'
        events = []
        record_type = C.CFUNCTYPE(C.c_int16, C.c_void_p, C.c_uint16,
                                  C.c_uint16, PTR, C.c_void_p)

        @record_type
        def visit(_context, record_kind, size, payload, pass_value):
            events.append((record_kind, size, bytes(payload[:size]),
                           C.cast(pass_value, C.c_void_p).value))
            return 0 if record_kind == 1 else 1

        marker = C.c_int(7)
        self.lib.stars_enum_log_rts(
            data, log, C.cast(visit, C.c_void_p), None,
            C.cast(C.pointer(marker), C.c_void_p))
        self.assertEqual(events, [(2, 3, b'abc', C.addressof(marker)),
                                  (1, 2, b'xy', C.addressof(marker))])

    def test_log_merge_fleet_filters_sentinel_and_duplicate_ids(self):
        data = (C.c_ubyte * 65536)()
        data[0x2578:0x257a] = (4).to_bytes(2, 'little')
        values = [7, -1, 9, 7]
        for i, value in enumerate(values):
            data[0x51bc + i * 2:0x51be + i * 2] = (value & 0xffff).to_bytes(2, 'little')
        events = []
        write_type = C.CFUNCTYPE(None, C.c_void_p, C.c_uint16, C.c_uint16,
                                 PTR)

        @write_type
        def write(_context, record_type, size, payload):
            events.append((record_type, size,
                           [int.from_bytes(bytes(payload[i:i + 2]), 'little',
                                           signed=True)
                            for i in range(0, size, 2)]))

        self.lib.stars_log_merge_fleet(
            data, 7, C.cast(write, C.c_void_p), None)
        self.assertEqual(events, [(0x25, 4, [7, 9])])

        data[0x7ca:0x7cc] = (2).to_bytes(2, 'little')
        events[:] = []
        self.lib.stars_log_merge_fleet(
            data, 7, C.cast(write, C.c_void_p), None)
        self.assertEqual(events, [])

    def test_log_change_relations_reuses_previous_relation_log_and_advances_tutor(self):
        data = (C.c_ubyte * 65536)()
        log = (C.c_ubyte * 32)()
        relations = (C.c_ubyte * 8).from_buffer_copy(b'RELATION')
        data[0x78:0x7a] = (3).to_bytes(2, 'little')
        data[0x9a6:0x9a8] = (4).to_bytes(2, 'little')
        data[0x9a4:0x9a6] = (9).to_bytes(2, 'little')
        log[4:9] = (0x9803).to_bytes(2, 'little') + b'old'
        data[0x7ca:0x7cc] = (0x0800).to_bytes(2, 'little')
        events = []
        write_type = C.CFUNCTYPE(None, C.c_void_p, C.c_uint16, C.c_uint16,
                                 PTR)
        advance_type = C.CFUNCTYPE(None, C.c_void_p)

        @write_type
        def write(_context, record_type, size, payload):
            events.append(('write', record_type, size,
                           bytes(payload[:size]) if payload else b''))

        @advance_type
        def advance(_context): events.append(('advance',))

        ops = LogChangeRelationsOps(C.cast(write, C.c_void_p),
                                    C.cast(advance, C.c_void_p))
        self.lib.stars_log_change_relations(
            data, log, relations, C.byref(ops), None)
        self.assertEqual(int.from_bytes(data[0x9a4:0x9a6], 'little'), 4)
        self.assertEqual(events, [('write', 0x26, 3, b'REL'),
                                  ('advance',)])
        self.assertEqual(int.from_bytes(data[0x520c:0x520e], 'little') & 4, 4)

    def test_log_make_valid_xferf_builds_masked_nonempty_item_record(self):
        first = LogXferf(12, 3, (C.c_uint16 * 16)())
        first.item[0] = 5
        first.item[2] = 0xfffe
        second = LogXferf(99, 7, (C.c_uint16 * 16)())
        events = []
        write_type = C.CFUNCTYPE(None, C.c_void_p, C.c_uint16, C.c_uint16,
                                 PTR)

        @write_type
        def write(_context, record_type, size, payload):
            events.append((record_type, size, bytes(payload[:size])))

        self.lib.stars_log_make_valid_xferf(
            C.byref(first), C.byref(second), C.cast(write, C.c_void_p), None)
        self.assertEqual(events, [(0x17, 11,
                                   b'\x0c\x00\x63\x00\x73\x05\x00'
                                   b'\x05\x00\xfe\xff')])

        first.item[0] = 0
        first.item[2] = 0
        events[:] = []
        self.lib.stars_log_make_valid_xferf(
            C.byref(first), C.byref(second), C.cast(write, C.c_void_p), None)
        self.assertEqual(events, [])

    def test_log_make_valid_xfer_selects_narrowest_signed_representation(self):
        data = (C.c_ubyte * 65536)()
        log = (C.c_ubyte * 64)()
        data[0x09a4:0x09a6] = (0).to_bytes(2, 'little')
        first = LogXfer(12, 3, (100, 0, 0, 0, 0))
        second = LogXfer(99, 7, (0, 0, 0, 0, 0))
        events = []
        write_type = C.CFUNCTYPE(None, C.c_void_p, C.c_uint16, C.c_uint16,
                                 PTR)

        @write_type
        def write(_context, record_type, size, payload):
            events.append((record_type, size, bytes(payload[:size])))

        ops = WriteMemRtOps(None, None)
        self.lib.stars_log_make_valid_xfer(
            data, log, C.byref(first), C.byref(second), C.byref(ops), None)
        self.assertEqual(events, [])
        self.assertEqual(bytes(log[:2]), b'\x07\x04')
        self.assertEqual(bytes(log[2:9]), b'\x0c\x00\x63\x00\x73\x01\x64')

        data[0x5260:0x5262] = (0x0407).to_bytes(2, 'little')
        first.item[0] = 0x7f00
        self.lib.stars_log_make_valid_xfer(
            data, log, C.byref(first), C.byref(second), C.byref(ops), None)
        self.assertEqual(int.from_bytes(log[0:2], 'little'), 0x0808)

    def test_log_change_thing_accumulates_three_word_deltas(self):
        data = (C.c_ubyte * 65536)()
        log = (C.c_ubyte * 64)()
        old = Thing()
        new = Thing()
        old.raw[0:2] = (12).to_bytes(2, 'little')
        new.raw[0:2] = (12).to_bytes(2, 'little')
        for offset, value in ((8, 10), (10, 20), (12, 30)):
            old.raw[offset:offset + 2] = value.to_bytes(2, 'little')
        for offset, value in ((8, 12), (10, 20), (12, 29)):
            new.raw[offset:offset + 2] = value.to_bytes(2, 'little')
        pending = LogXfer()
        valid = C.c_uint16(0)
        ops = WriteMemRtOps(None, None)
        self.lib.stars_log_change_thing(
            data, log, C.byref(valid), C.byref(pending), C.byref(old),
            C.byref(new), C.byref(ops), None)
        self.assertEqual(valid.value, 1)
        self.assertEqual((pending.id, pending.grobj,
                          list(pending.item[:3])), (12, 8, [2, 0, 0xffffffff]))

        old2 = Thing()
        new2 = Thing()
        old2.raw[0:2] = new2.raw[0:2] = (12).to_bytes(2, 'little')
        for offset, value in ((8, 12), (10, 20), (12, 29)):
            old2.raw[offset:offset + 2] = value.to_bytes(2, 'little')
        for offset, value in ((8, 13), (10, 21), (12, 30)):
            new2.raw[offset:offset + 2] = value.to_bytes(2, 'little')
        self.lib.stars_log_change_thing(
            data, log, C.byref(valid), C.byref(pending), C.byref(old2),
            C.byref(new2), C.byref(ops), None)
        self.assertEqual(valid.value, 0)
        self.assertEqual(bytes(log[2:10]),
                         b'\x0c\x00\x0c\x00\x88\x05\x02\xff')

        data[0x7ca:0x7cc] = (2).to_bytes(2, 'little')
        self.lib.stars_log_change_thing(
            data, log, C.byref(valid), C.byref(pending), C.byref(old2),
            C.byref(new2), C.byref(ops), None)
        self.assertEqual(valid.value, 0)

    def test_log_change_name_replaces_name_and_writes_compressed_record(self):
        fleet = FleetNameRecord()
        old_name = C.create_string_buffer(b'old')
        old_pointer = C.cast(old_name, C.c_void_p).value
        fleet.name = old_pointer
        events = []
        allocations = []
        write_type = C.CFUNCTYPE(None, C.c_void_p, C.c_uint16, C.c_uint16,
                                 PTR)
        find_type = C.CFUNCTYPE(C.c_void_p, C.c_void_p, C.c_int16)
        alloc_type = C.CFUNCTYPE(C.c_void_p, C.c_void_p, C.c_uint16)
        free_type = C.CFUNCTYPE(None, C.c_void_p, C.c_void_p)
        compress_type = C.CFUNCTYPE(C.c_int16, C.c_void_p, PTR, PTR,
                                    C.POINTER(C.c_int16))
        advance_type = C.CFUNCTYPE(None, C.c_void_p)

        @find_type
        def find(_context, identifier):
            self.assertEqual(identifier, 44)
            return C.addressof(fleet)

        @alloc_type
        def allocate(_context, size):
            value = C.create_string_buffer(size)
            allocations.append(value)
            return C.cast(value, C.c_void_p).value

        @free_type
        def free(_context, pointer):
            self.assertEqual(pointer, old_pointer)

        @compress_type
        def compress(_context, source, output, length):
            self.assertEqual(bytes(source[:5]), b'Nova\0')
            output[0] = 0x81
            output[1] = 0x82
            length[0] = 2
            return 1

        @write_type
        def write(_context, record_type, size, payload):
            events.append((record_type, size, bytes(payload[:size])))

        advances = []
        @advance_type
        def advance(_context): advances.append(True)

        ops = LogChangeNameOps(C.cast(find, C.c_void_p),
                               C.cast(allocate, C.c_void_p),
                               C.cast(free, C.c_void_p),
                               C.cast(compress, C.c_void_p),
                               C.cast(write, C.c_void_p),
                               C.cast(advance, C.c_void_p))
        self.lib.stars_log_change_name(
            3, 44, (C.c_ubyte * 5).from_buffer_copy(b'Nova\0'), 0x0800,
            C.byref(ops), None)
        self.assertEqual(events, [(0x2c, 7,
                                   b',\x00\x03\x00\x02\x81\x82')])
        self.assertEqual(advances, [True])

    def test_get_best_defense_part_keeps_last_successful_item(self):
        part = (C.c_ubyte * 8)()
        calls = []
        highest = [11]
        lookup_type = C.CFUNCTYPE(C.c_int16, C.c_void_p, PTR)

        @lookup_type
        def lookup(_context, candidate):
            item = candidate[2]
            calls.append(item)
            if item <= highest[0]:
                candidate[4] = item
                candidate[5] = 0xa5
                return 1
            return 0

        self.assertEqual(self.lib.stars_get_best_defense_part(
            part, C.cast(lookup, C.c_void_p), None), 1)
        self.assertEqual(calls, [9, 10, 11, 12, 11])
        self.assertEqual(bytes(part), b'\x00\x80\x0b\x00\x0b\xa5\x00\x00')

        calls[:] = []
        highest[0] = 8
        self.assertEqual(self.lib.stars_get_best_defense_part(
            part, C.cast(lookup, C.c_void_p), None), 0)
        self.assertEqual(calls, [9, 9])

    def test_end_tutor_tears_down_active_session_and_randomizes(self):
        data = (C.c_ubyte * 65536)()
        tutor = (C.c_ubyte * 44)()
        data[0x7ca:0x7cc] = (0x0800).to_bytes(2, 'little')
        data[0x70 + 16:0x70 + 18] = (0xffff).to_bytes(2, 'little')
        tutor[0:2] = (0x1234).to_bytes(2, 'little')
        tutor[16:18] = (0x4321).to_bytes(2, 'little')
        events = []
        destroy_type = C.CFUNCTYPE(None, C.c_void_p, C.c_uint16)
        restore_type = C.CFUNCTYPE(None, C.c_void_p)
        tick_type = C.CFUNCTYPE(C.c_uint32, C.c_void_p)
        random_type = C.CFUNCTYPE(None, C.c_void_p, C.c_uint32)

        @destroy_type
        def destroy(_context, window): events.append(('destroy', window))

        @restore_type
        def restore(_context): events.append(('restore',))

        @tick_type
        def tick(_context): return 0x12345678

        @random_type
        def randomize(_context, value): events.append(('random', value))

        ops = EndTutorOps(C.cast(destroy, C.c_void_p), C.cast(restore, C.c_void_p),
                          C.cast(tick, C.c_void_p), C.cast(randomize, C.c_void_p))
        self.lib.stars_end_tutor(data, tutor, 1, C.byref(ops), None)
        self.assertEqual(events, [('destroy', 0x4321), ('restore',),
                                  ('random', 0x12345678)])
        self.assertEqual(int.from_bytes(data[0x7ca:0x7cc], 'little'), 0)
        self.assertEqual(int.from_bytes(data[0x70 + 16:0x70 + 18], 'little'),
                         0xfff7)
        self.assertEqual(bytes(tutor), b'\0' * 44)

    def test_ask_kill_tutor_requires_early_turn_confirmation(self):
        warning = (C.c_ubyte * 6).from_buffer_copy(b'close\0')
        events = []
        confirm_type = C.CFUNCTYPE(C.c_int16, C.c_void_p, PTR)
        end_type = C.CFUNCTYPE(None, C.c_void_p, C.c_int16)

        @confirm_type
        def confirm(_context, text):
            events.append(('confirm', bytes(text[:6])))
            return 6

        @end_type
        def end(_context, value): events.append(('end', value))

        self.assertEqual(self.lib.stars_ask_kill_tutor(
            10, warning, C.cast(confirm, C.c_void_p),
            C.cast(end, C.c_void_p), None), 1)
        self.assertEqual(events, [('confirm', b'close\0'), ('end', 1)])
        events[:] = []
        self.assertEqual(self.lib.stars_ask_kill_tutor(
            30, warning, C.cast(confirm, C.c_void_p),
            C.cast(end, C.c_void_p), None), 1)
        self.assertEqual(events, [('end', 1)])

        @confirm_type
        def reject(_context, _text): return 0

        events[:] = []
        self.assertEqual(self.lib.stars_ask_kill_tutor(
            10, warning, C.cast(reject, C.c_void_p),
            C.cast(end, C.c_void_p), None), 0)
        self.assertEqual(events, [])

    def test_check_initiative_restores_player_context_and_writes_token(self):
        token = (C.c_ubyte * 29)()
        token[2] = 7
        hull = (C.c_ubyte * 8).from_buffer_copy(b'HULLDATA')
        events = []
        lookup_type = C.CFUNCTYPE(C.c_void_p, C.c_void_p, PTR)
        player_type = C.CFUNCTYPE(None, C.c_void_p, C.c_int16)
        init_type = C.CFUNCTYPE(C.c_int16, C.c_void_p, PTR, PTR)

        @lookup_type
        def lookup(_context, passed_token):
            self.assertEqual(passed_token[2], 7)
            return C.addressof(hull)

        @player_type
        def set_player(_context, player): events.append(('player', player))

        @init_type
        def init(_context, passed_hull, pct):
            self.assertEqual(bytes(passed_hull[:8]), b'HULLDATA')
            pct[0] = 63
            return 12

        ops = CheckInitiativeOps(C.cast(lookup, C.c_void_p),
                                 C.cast(set_player, C.c_void_p),
                                 C.cast(init, C.c_void_p))
        self.lib.stars_check_initiative(token, C.byref(ops), None)
        self.assertEqual(events, [('player', 7), ('player', -1)])
        self.assertEqual((token[6], token[12]), (12, 63))

    def test_cmax_mines_scales_capacity_clamps_and_disables(self):
        planet = (C.c_ubyte * 2).from_buffer_copy(b'\x34\x12')
        calls = []
        calc_type = C.CFUNCTYPE(C.c_int32, C.c_void_p, C.c_uint16, C.c_int16)
        stat_type = C.CFUNCTYPE(C.c_int16, C.c_void_p, C.c_int16, C.c_int16)
        disabled = [False]

        @calc_type
        def calc(_context, identifier, player):
            calls.append(('calc', identifier, player))
            return 1200

        @stat_type
        def stat(_context, player, index):
            calls.append(('stat', player, index))
            return 50 if index == 6 else (8 if disabled[0] else 7)

        ops = CMaxMinesOps(C.cast(calc, C.c_void_p), C.cast(stat, C.c_void_p))
        self.assertEqual(self.lib.stars_cmax_mines(
            planet, 3, C.byref(ops), None), 600)
        disabled[0] = True
        self.assertEqual(self.lib.stars_cmax_mines(
            planet, 3, C.byref(ops), None), 0)
        self.assertEqual(calls[:2], [('calc', 0x1234, 3), ('stat', 3, 6)])

    def test_pct_planet_capacity_rounds_and_caps_ratio(self):
        planet = (C.c_ubyte * 56)()
        planet[0:2] = b'\x34\x12'
        calls = []
        populations = [1000]
        calc_type = C.CFUNCTYPE(C.c_int32, C.c_void_p, C.c_uint16, C.c_int16)

        @calc_type
        def calc(_context, identifier, player):
            calls.append((identifier, player))
            return populations[0]

        ctypes_calc = C.cast(calc, C.c_void_p)
        planet[40:44] = (500).to_bytes(4, 'little')
        self.assertEqual(self.lib.stars_pct_planet_capacity(
            planet, 2, ctypes_calc, None), 50)
        planet[40:44] = (20000).to_bytes(4, 'little')
        populations[0] = 100
        self.assertEqual(self.lib.stars_pct_planet_capacity(
            planet, 2, ctypes_calc, None), 999)
        populations[0] = 0
        self.assertEqual(self.lib.stars_pct_planet_capacity(
            planet, 2, ctypes_calc, None), 0)
        self.assertEqual(calls, [(0x1234, 2), (0x1234, 2), (0x1234, 2)])

    def test_cmax_operable_mines_applies_next_year_change_and_limits(self):
        planet = (C.c_ubyte * 56)()
        planet[0:2] = b'\x34\x12'
        planet[40:44] = (500).to_bytes(4, 'little')
        disabled = [False]
        calc_type = C.CFUNCTYPE(C.c_int32, C.c_void_p, C.c_uint16, C.c_int16)
        stat_type = C.CFUNCTYPE(C.c_int16, C.c_void_p, C.c_int16, C.c_int16)
        change_type = C.CFUNCTYPE(C.c_int32, C.c_void_p, PTR, C.c_int16)

        @calc_type
        def calc(_context, _identifier, _player): return 600

        @stat_type
        def stat(_context, _player, index):
            return 50 if index == 6 else (8 if disabled[0] else 7)

        @change_type
        def change(_context, _planet, mode):
            self.assertEqual(mode, 0)
            return 100

        ops = CMaxOperableMinesOps(C.cast(calc, C.c_void_p),
                                   C.cast(stat, C.c_void_p),
                                   C.cast(change, C.c_void_p))
        self.assertEqual(self.lib.stars_cmax_operable_mines(
            planet, 2, 0, C.byref(ops), None), 250)
        self.assertEqual(self.lib.stars_cmax_operable_mines(
            planet, 2, 1, C.byref(ops), None), 300)
        disabled[0] = True
        self.assertEqual(self.lib.stars_cmax_operable_mines(
            planet, 2, 0, C.byref(ops), None), 0)

    def test_cmines_operating_handles_owner_modes_and_mine_count_limit(self):
        planet = (C.c_ubyte * 56)()
        planet[2:4] = (2).to_bytes(2, 'little', signed=False)
        planet[20:24] = (123 << 8).to_bytes(4, 'little', signed=False)
        planet[34:38] = (1444).to_bytes(4, 'little', signed=False)
        planet[40:44] = (500).to_bytes(4, 'little', signed=False)
        special = [False]
        calc_type = C.CFUNCTYPE(C.c_int32, C.c_void_p, C.c_uint16, C.c_int16)
        stat_type = C.CFUNCTYPE(C.c_int16, C.c_void_p, C.c_int16, C.c_int16)

        @calc_type
        def calc(_context, _identifier, _player): return 600

        @stat_type
        def stat(_context, _player, index):
            return 8 if special[0] and index == 0x0e else (
                7 if index == 0x0e else 50)

        ops = CMaxOperableMinesOps(C.cast(calc, C.c_void_p),
                                   C.cast(stat, C.c_void_p), None)
        self.assertEqual(self.lib.stars_cmines_operating(
            planet, C.byref(ops), None), 123)
        special[0] = True
        self.assertEqual(self.lib.stars_cmines_operating(
            planet, C.byref(ops), None), 38)
        planet[2:4] = (0xffff).to_bytes(2, 'little', signed=False)
        self.assertEqual(self.lib.stars_cmines_operating(
            planet, C.byref(ops), None), 0)

    def test_factory_capacity_and_operating_paths(self):
        planet = (C.c_ubyte * 56)()
        planet[0:2] = (0x1234).to_bytes(2, 'little', signed=False)
        planet[2:4] = (2).to_bytes(2, 'little', signed=False)
        planet[20:24] = (37 << 20).to_bytes(4, 'little', signed=False)
        planet[40:44] = (500).to_bytes(4, 'little', signed=False)
        disabled = [False]
        calc_type = C.CFUNCTYPE(C.c_int32, C.c_void_p, C.c_uint16, C.c_int16)
        stat_type = C.CFUNCTYPE(C.c_int16, C.c_void_p, C.c_int16, C.c_int16)

        @calc_type
        def calc(_context, identifier, _player):
            self.assertEqual(identifier, 0x1234)
            return 600

        @stat_type
        def stat(_context, _player, index):
            return 8 if disabled[0] and index == 0x0e else (
                7 if index == 0x0e else 50)

        max_ops = CMaxMinesOps(C.cast(calc, C.c_void_p),
                               C.cast(stat, C.c_void_p))
        ops = CMaxOperableFactoriesOps(C.cast(calc, C.c_void_p),
                                       C.cast(stat, C.c_void_p), None)
        self.assertEqual(self.lib.stars_cmax_factories(
            planet, 2, C.byref(max_ops), None), 300)
        self.assertEqual(self.lib.stars_cmax_operable_factories(
            planet, 2, 0, C.byref(ops), None), 250)
        self.assertEqual(self.lib.stars_cfactories_operating(
            planet, C.byref(ops), None), 37)
        disabled[0] = True
        self.assertEqual(self.lib.stars_cmax_factories(
            planet, 2, C.byref(max_ops), None), 0)
        self.assertEqual(self.lib.stars_cmax_operable_factories(
            planet, 2, 0, C.byref(ops), None), 0)
        self.assertEqual(self.lib.stars_cfactories_operating(
            planet, C.byref(ops), None), 0)

    def test_cmax_defenses_clamps_desirability_and_applies_disable_stat(self):
        planet = (C.c_ubyte * 56)()
        desirability = [10]
        disabled = [False]
        desirability_type = C.CFUNCTYPE(C.c_int16, C.c_void_p, PTR, C.c_int16)
        stat_type = C.CFUNCTYPE(C.c_int16, C.c_void_p, C.c_int16, C.c_int16)

        @desirability_type
        def get_desirability(_context, _planet, _player):
            return desirability[0]

        @stat_type
        def stat(_context, _player, index):
            self.assertEqual(index, 0x0e)
            return 8 if disabled[0] else 7

        ops = CMaxDefensesOps(C.cast(get_desirability, C.c_void_p),
                              C.cast(stat, C.c_void_p))
        self.assertEqual(self.lib.stars_cmax_defenses(
            planet, 2, C.byref(ops), None), 40)
        desirability[0] = 0
        self.assertEqual(self.lib.stars_cmax_defenses(
            planet, 2, C.byref(ops), None), 10)
        desirability[0] = 30
        self.assertEqual(self.lib.stars_cmax_defenses(
            planet, 2, C.byref(ops), None), 100)
        disabled[0] = True
        self.assertEqual(self.lib.stars_cmax_defenses(
            planet, 2, C.byref(ops), None), 0)

    def test_cmax_operable_defenses_rounds_limits_and_applies_next_year(self):
        planet = (C.c_ubyte * 56)()
        planet[40:44] = (500).to_bytes(4, 'little', signed=False)
        disabled = [False]
        calc_type = C.CFUNCTYPE(C.c_int16, C.c_void_p, PTR, C.c_int16)
        stat_type = C.CFUNCTYPE(C.c_int16, C.c_void_p, C.c_int16, C.c_int16)
        change_type = C.CFUNCTYPE(C.c_int32, C.c_void_p, PTR, C.c_int16)

        @calc_type
        def desirability(_context, _planet, _player): return 10

        @stat_type
        def stat(_context, _player, index):
            return 8 if disabled[0] and index == 0x0e else 7

        @change_type
        def change(_context, _planet, mode):
            self.assertEqual(mode, 0)
            return 100

        ops = CMaxOperableDefensesOps(C.cast(desirability, C.c_void_p),
                                      C.cast(stat, C.c_void_p),
                                      C.cast(change, C.c_void_p))
        self.assertEqual(self.lib.stars_cmax_operable_defenses(
            planet, 2, 0, C.byref(ops), None), 20)
        self.assertEqual(self.lib.stars_cmax_operable_defenses(
            planet, 2, 1, C.byref(ops), None), 24)
        planet[40:44] = (25000).to_bytes(4, 'little', signed=False)
        self.assertEqual(self.lib.stars_cmax_operable_defenses(
            planet, 2, 0, C.byref(ops), None), 40)
        disabled[0] = True
        self.assertEqual(self.lib.stars_cmax_operable_defenses(
            planet, 2, 0, C.byref(ops), None), 0)

    def test_cresources_at_planet_combines_normal_and_special_paths(self):
        planet = (C.c_ubyte * 56)()
        planet[0:2] = (0x1234).to_bytes(2, 'little', signed=False)
        planet[20:24] = (30 << 20).to_bytes(4, 'little', signed=False)
        planet[40:44] = (500).to_bytes(4, 'little', signed=False)
        special = [False]
        calc_type = C.CFUNCTYPE(C.c_int32, C.c_void_p, C.c_uint16, C.c_int16)
        stat_type = C.CFUNCTYPE(C.c_int16, C.c_void_p, C.c_int16, C.c_int16)
        desirability_type = C.CFUNCTYPE(C.c_int16, C.c_void_p, PTR, C.c_int16)
        byte_type = C.CFUNCTYPE(C.c_int16, C.c_void_p, C.c_int16, C.c_uint16)

        @calc_type
        def calc(_context, identifier, _player):
            self.assertEqual(identifier, 0x1234)
            return 400

        @stat_type
        def stat(_context, _player, index):
            if index == 0:
                return 100
            if index == 1:
                return 20
            if index == 3:
                return 50
            return 8 if special[0] else 7

        @desirability_type
        def desirability(_context, _planet, _player): return 50

        @byte_type
        def race_byte(_context, _player, offset):
            self.assertEqual(offset, 0x1a)
            return 4

        ops = CResourcesAtPlanetOps(C.cast(calc, C.c_void_p),
                                    C.cast(desirability, C.c_void_p),
                                    C.cast(stat, C.c_void_p),
                                    C.cast(race_byte, C.c_void_p),
                                    C.cast(calc, C.c_void_p),
                                    C.cast(stat, C.c_void_p), None)
        self.assertEqual(self.lib.stars_cresources_at_planet(
            planet, 2, C.byref(ops), None), 64)
        special[0] = True
        self.assertEqual(self.lib.stars_cresources_at_planet(
            planet, 2, C.byref(ops), None), 11)
        planet[40:44] = (0).to_bytes(4, 'little', signed=False)
        self.assertEqual(self.lib.stars_cresources_at_planet(
            planet, 2, C.byref(ops), None), 0)

    def test_iwarp_ma_selects_maximum_and_tie_with_owner_gates(self):
        planet = (C.c_ubyte * 56)()
        planet[2:4] = (2).to_bytes(2, 'little', signed=False)
        planet[4:6] = (0x0200).to_bytes(2, 'little', signed=False)
        planet[44:48] = (3).to_bytes(4, 'little', signed=False)
        item_type = C.CFUNCTYPE(C.c_uint16, C.c_void_p, C.c_int16,
                                C.c_uint16, C.POINTER(WarpMAItem), C.c_uint16)
        race_type = C.CFUNCTYPE(C.c_int16, C.c_void_p, C.c_int16,
                                C.c_uint16, C.c_uint16)

        @item_type
        def items(_context, owner, machine_index, output, capacity):
            self.assertEqual((owner, machine_index, capacity), (2, 3, 256))
            values = [(0x0200, 0x0108), (0x0200, 0x0108),
                      (0x0200, 0x0009), (0x0201, 0x010f)]
            for i, (identifier, flags) in enumerate(values):
                output[i].identifier = identifier
                output[i].flags = flags
            return len(values)

        @race_type
        def race(_context, _owner, _machine_index, _offset): return 7

        two = C.c_int16()
        self.assertEqual(self.lib.stars_iwarp_ma_from_planet(
            planet, 2, C.byref(two), C.cast(items, C.c_void_p),
            C.cast(race, C.c_void_p), None), 6)
        self.assertEqual(two.value, 1)
        two.value = 9
        self.assertEqual(self.lib.stars_iwarp_ma_from_planet(
            planet, 99, C.byref(two), C.cast(items, C.c_void_p),
            C.cast(race, C.c_void_p), None), 6)
        planet[2:4] = (0xffff).to_bytes(2, 'little', signed=False)
        self.assertEqual(self.lib.stars_iwarp_ma_from_planet(
            planet, 2, C.byref(two), C.cast(items, C.c_void_p),
            C.cast(race, C.c_void_p), None), 0)

    def test_stargate_range_scans_parts_and_handles_unknown_fuel_range(self):
        planet = (C.c_ubyte * 56)()
        item_type = C.CFUNCTYPE(C.c_uint16, C.c_void_p, C.c_int16,
                                C.c_uint16, C.POINTER(WarpMAItem), C.c_uint16)
        fuel_type = C.CFUNCTYPE(C.c_int16, C.c_void_p, C.c_uint32,
                                C.c_uint16)
        unknown = [False]

        @item_type
        def items(_context, owner, machine_index, output, capacity):
            self.assertEqual((owner, capacity), (4, 256))
            self.assertIn(machine_index, (2, 9))
            values = [(0x0201, 0x0102), (0x0200, 0x0007),
                      (0x0200, 0x0105)]
            for i, (identifier, flags) in enumerate(values):
                output[i].identifier = identifier
                output[i].flags = flags
            return len(values)

        @fuel_type
        def fuel(_context, identifier, flags):
            self.assertEqual((identifier, flags), (0x0200, 0x0105))
            return -1 if unknown[0] else 400

        self.assertEqual(self.lib.stars_stargate_range_from_planet(
            None, 4, 2, C.cast(items, C.c_void_p), C.cast(fuel, C.c_void_p),
            None), 400)
        unknown[0] = True
        self.assertEqual(self.lib.stars_stargate_range_from_planet(
            None, 4, 2, C.cast(items, C.c_void_p), C.cast(fuel, C.c_void_p),
            None), 10000)
        planet[2:4] = (4).to_bytes(2, 'little', signed=False)
        planet[4:6] = (0x0200).to_bytes(2, 'little', signed=False)
        planet[44:48] = (9).to_bytes(4, 'little', signed=False)
        self.assertEqual(self.lib.stars_stargate_range_from_planet(
            planet, 99, 2, C.cast(items, C.c_void_p), C.cast(fuel, C.c_void_p),
            None), 10000)
        planet[4:6] = (0).to_bytes(2, 'little', signed=False)
        self.assertEqual(self.lib.stars_stargate_range_from_planet(
            planet, 99, 2, C.cast(items, C.c_void_p), C.cast(fuel, C.c_void_p),
            None), 0)

    def test_fprod_is_terra_checks_kind_and_terraforming_codes(self):
        product = (C.c_ubyte * 4)()
        for code in (4, 5, 0x0c):
            value = (1 << 17) | (code << 10)
            product[:] = value.to_bytes(4, 'little', signed=False)
            self.assertEqual(self.lib.stars_fprod_is_terra(product), 1)
        for value in ((1 << 17) | (3 << 10),
                      (2 << 17) | (4 << 10), 0):
            product[:] = value.to_bytes(4, 'little', signed=False)
            self.assertEqual(self.lib.stars_fprod_is_terra(product), 0)
        self.assertEqual(self.lib.stars_fprod_is_terra(None), 0)

    def test_ipct_can_terraform_sums_open_environment_deltas(self):
        planet = (C.c_ubyte * 56)()
        planet[12:15] = bytes((10, 25, 30))
        can_type = C.CFUNCTYPE(C.c_int16, C.c_void_p, PTR,
                               C.POINTER(C.c_int16), C.POINTER(C.c_int16),
                               C.POINTER(C.c_int16), C.c_int16)
        available = [True]

        @can_type
        def can(_context, _planet, env_min, env_max, env_cost, help_mode):
            self.assertEqual(help_mode, 1)
            for i, value in enumerate((-1, 20, 30)):
                env_min[i] = value
            for i, value in enumerate((40, -1, 50)):
                env_max[i] = value
            for i, value in enumerate((1, 2, 3)):
                env_cost[i] = value
            return 1 if available[0] else 0

        self.assertEqual(self.lib.stars_ipct_can_terraform(
            planet, C.cast(can, C.c_void_p), None), 55)
        available[0] = False
        self.assertEqual(self.lib.stars_ipct_can_terraform(
            planet, C.cast(can, C.c_void_p), None), 0)

    def test_uninhabit_planet_resets_state_restores_environment_and_frees_production(self):
        planet = (C.c_ubyte * 56)()
        planet[0:2] = (7).to_bytes(2, 'little', signed=False)
        planet[2:4] = (2).to_bytes(2, 'little', signed=False)
        planet[4:6] = (0xffff).to_bytes(2, 'little', signed=False)
        planet[12:15] = bytes((10, 20, 30))
        planet[15:18] = bytes((40, 50, 60))
        planet[24:28] = (0x4567).to_bytes(4, 'little', signed=False)
        planet[40:44] = (1234).to_bytes(4, 'little', signed=False)
        planet[44:48] = (0x12345678).to_bytes(4, 'little', signed=False)
        planet[52:56] = (0x87654321).to_bytes(4, 'little', signed=False)
        stat_type = C.CFUNCTYPE(C.c_int16, C.c_void_p, C.c_int16, C.c_int16)
        free_type = C.CFUNCTYPE(None, C.c_void_p, C.c_int16, C.c_uint32)
        freed = []

        @stat_type
        def stat(_context, owner, index):
            self.assertEqual((owner, index), (2, 0x0e))
            return 3

        @free_type
        def free(_context, planet_id, handle):
            freed.append((planet_id, handle))

        ops = UninhabitPlanetOps(C.cast(stat, C.c_void_p),
                                 C.cast(free, C.c_void_p))
        self.lib.stars_uninhabit_planet(planet, C.byref(ops), None)
        self.assertEqual(bytes(planet[12:15]), bytes((40, 50, 60)))
        self.assertEqual(int.from_bytes(bytes(planet[2:4]), 'little'), 0xffff)
        self.assertEqual(int.from_bytes(bytes(planet[40:44]), 'little'), 0)
        self.assertEqual(int.from_bytes(bytes(planet[44:48]), 'little'), 0)
        self.assertEqual(int.from_bytes(bytes(planet[52:56]), 'little'), 0)
        self.assertEqual(freed, [(7, 0x87654321)])
        self.assertEqual(int.from_bytes(bytes(planet[24:26]), 'little'), 0xf567)
        self.assertEqual(int.from_bytes(bytes(planet[26:28]), 'little'), 1)

    def test_pct_cloak_from_huldef_accumulates_sections_and_steal_thresholds(self):
        hull = (C.c_ubyte * 123)()
        hull[0:2] = (0x20).to_bytes(2, 'little', signed=False)
        sections = (CloakSection * 3)(CloakSection(1, 0x0108),
                                      CloakSection(2, 0x0106),
                                      CloakSection(2, 0x0105))
        grbit_type = C.CFUNCTYPE(C.c_int16, C.c_void_p, C.c_int16, C.c_int16)
        stat_type = C.CFUNCTYPE(C.c_int16, C.c_void_p, C.c_int16, C.c_int16)
        part_type = C.CFUNCTYPE(C.c_int16, C.c_void_p, C.c_uint16, C.c_uint16)

        @grbit_type
        def grbit(_context, _player, bit):
            self.assertEqual(bit, 3)
            return 1

        @stat_type
        def stat(_context, _player, index):
            return 7 if index == 0x0e else 0

        @part_type
        def part(_context, _group, _subtype): return 30

        ops = C.cast((C.c_void_p * 3)(C.cast(grbit, C.c_void_p),
                                      C.cast(stat, C.c_void_p),
                                      C.cast(part, C.c_void_p)), C.c_void_p)
        steal = C.c_int16()
        self.assertEqual(self.lib.stars_pct_cloak_from_huldef(
            hull, 2, C.byref(steal), C.cast(sections, PTR), 3, ops, None), 50)
        self.assertEqual(steal.value, 70)
        self.assertEqual(self.lib.stars_pct_cloak_from_huldef(
            hull, -1, C.byref(steal), C.cast(sections, PTR), 3, ops, None), 30)

    def test_show_tutor_calls_window_service_and_updates_low_flag(self):
        hwnd = C.c_uint16(0x1234)
        flags = C.c_uint16(0xfffe)
        events = []
        show_type = C.CFUNCTYPE(None, C.c_void_p, C.c_uint16, C.c_uint16)

        @show_type
        def show(_context, window, command):
            events.append((window, command))

        self.lib.stars_show_tutor(
            C.byref(hwnd), C.byref(flags), 1, C.cast(show, C.c_void_p), None)
        self.assertEqual(events, [(0x1234, 5)])
        self.assertEqual(flags.value, 0xffff)
        self.lib.stars_show_tutor(
            C.byref(hwnd), C.byref(flags), 0, C.cast(show, C.c_void_p), None)
        self.assertEqual(events[-1], (0x1234, 0))
        self.assertEqual(flags.value, 0xfffe)

    def test_write_mem_rt_appends_header_payload_and_restores_playback_behavior(self):
        data = (C.c_ubyte * 65536)()
        # The overflow callback does not abort the original append path. Keep
        # the backing buffer large enough for the final cursor=31999 case.
        log = (C.c_ubyte * 32005)()
        payload = (C.c_ubyte * 4).from_buffer_copy(b'abcd')
        data[0x09a4:0x09a6] = (10).to_bytes(2, 'little')
        data[0x5260:0x5262] = (0x1234).to_bytes(2, 'little')
        data[0x198:0x19a] = (0x333).to_bytes(2, 'little')
        events = []
        title_type = C.CFUNCTYPE(None, C.c_void_p, C.c_uint16)
        overflow_type = C.CFUNCTYPE(None, C.c_void_p)

        @title_type
        def title(_context, hwnd): events.append(('title', hwnd))

        @overflow_type
        def overflow(_context): events.append(('overflow',))

        ops = WriteMemRtOps(C.cast(title, C.c_void_p),
                            C.cast(overflow, C.c_void_p))
        self.lib.stars_write_mem_rt(
            data, log, 5, 4, payload, C.byref(ops), None)
        self.assertEqual(int.from_bytes(log[10:12], 'little'), 0x1404)
        self.assertEqual(bytes(log[12:16]), b'abcd')
        self.assertEqual(int.from_bytes(data[0x09a6:0x09a8], 'little'), 10)
        self.assertEqual(int.from_bytes(data[0x09a4:0x09a6], 'little'), 16)
        self.assertEqual(int.from_bytes(data[0x5260:0x5262], 'little'), 0x1234)
        self.assertEqual(events, [('title', 0x333)])

        data[0x09a8:0x09aa] = (1).to_bytes(2, 'little')
        self.lib.stars_write_mem_rt(
            data, log, 1, 2, payload, C.byref(ops), None)
        self.assertEqual(int.from_bytes(data[0x09a4:0x09a6], 'little'), 16)
        self.assertEqual(int.from_bytes(data[0x5260:0x5262], 'little'), 0x1234)

        data[0x09a8:0x09aa] = b'\0\0'
        data[0x09a4:0x09a6] = (31999).to_bytes(2, 'little')
        self.lib.stars_write_mem_rt(
            data, log, 1, 4, payload, C.byref(ops), None)
        self.assertIn(('overflow',), events)
        self.assertEqual(bytes(log[31999:32001]), b'\x04\x04')
        self.assertEqual(bytes(log[32001:32005]), b'abcd')
        self.assertEqual(int.from_bytes(data[0x09a4:0x09a6], 'little'), 32005)

    def test_file_status_builds_name_calls_access_mode_two_and_sets_bit(self):
        data = (C.c_ubyte * 65536)()
        data[0xa0c:0xa0c + 2] = b'%s'
        data[0xa10:0xa10 + 5] = b'.sav\0'
        data[0x56a2:0x56a2 + 8] = b'game.sav'
        data[0x56a2 + 8] = 0
        data[0x7ce] = 0xff
        calls = []
        callback_type = C.CFUNCTYPE(C.c_int16, C.c_void_p, PTR, C.c_int16)

        @callback_type
        def access(_context, path, mode):
            calls.append((C.string_at(path), mode))
            return 0

        self.assertEqual(self.lib.stars_get_file_status(
            data, 0, 0, C.cast(access, C.c_void_p), None), 0)
        self.assertEqual(calls, [(b'game.sav', 2)])
        self.assertEqual(data[0x7ce] & 0x20, 0)

        data[0x56a2:0x56a2 + 8] = b'game.sav'
        data[0x56a2 + 8] = 0
        @callback_type
        def missing(_context, path, mode):
            calls.append((C.string_at(path), mode))
            return -1

        self.assertEqual(self.lib.stars_get_file_status(
            data, 0, 0, C.cast(missing, C.c_void_p), None), 1)
        self.assertEqual(calls[-1], (b'game.sav', 2))
        self.assertEqual(data[0x7ce] & 0x20, 0x20)

    def test_turns_out_safe_restores_player_and_flag_state(self):
        data = (C.c_ubyte * 65536)()
        data[0x18c] = 0x34
        data[0x18d] = 0x12
        data[0x7ca] = 0xaa
        data[0x7cb] = 0x55
        observed = []
        callback_type = C.CFUNCTYPE(C.c_int16, C.c_void_p)

        @callback_type
        def find_turns(_context):
            observed.append((data[0x18c] | (data[0x18d] << 8),
                             data[0x7ca] | (data[0x7cb] << 8)))
            return -7

        self.assertEqual(self.lib.stars_turns_out_safe(
            data, C.cast(find_turns, C.c_void_p), None), -7)
        self.assertEqual(observed, [(0xffff, (0x55aa & 0xfff5) | 8)])
        self.assertEqual(data[0x18c] | (data[0x18d] << 8), 0x1234)
        self.assertEqual(data[0x7ca] | (data[0x7cb] << 8), 0x55aa)
        self.lib.stars_draw_host_options(1, 2, 3)

    def test_mark_planet_updates_mark_detail_counts_owner_and_design_hook(self):
        planet = (C.c_ubyte * 56)()
        players = (C.c_ubyte * (192 * 2))()
        struct.pack_into('<H', planet, 2, 1)
        struct.pack_into('<H', planet, 4, 0x0200)
        struct.pack_into('<H', planet, 44, 0x0002)
        struct.pack_into('<H', players, 192 + 4, 7)
        events = []
        callback_type = C.CFUNCTYPE(None, C.c_void_p, C.c_int16,
                                    C.c_uint16, C.c_uint16)

        @callback_type
        def mark_design(_context, owner, design, fleet_count):
            events.append((owner, design, fleet_count))

        self.lib.stars_mark_planet(
            planet, players, 192, 1, 3,
            C.cast(mark_design, C.c_void_p), None)
        self.assertEqual(struct.unpack_from('<H', planet, 4)[0], 0x0303)
        self.assertEqual(struct.unpack_from('<H', players, 192 + 2)[0], 1)
        self.assertEqual(struct.unpack_from('<H', players, 192 + 6)[0], 0x0103)
        self.assertEqual(events, [(1, 2, 7)])

        events[:] = []
        self.lib.stars_mark_planet(
            planet, players, 192, 1, 2,
            C.cast(mark_design, C.c_void_p), None)
        self.assertEqual(events, [])

    def test_mark_fleet_marks_designs_resets_player_count_and_raises_detail(self):
        fleet = (C.c_ubyte * 124)()
        players = (C.c_ubyte * (192 * 2))()
        designs = (C.c_ubyte * (0x93 * 16))()
        struct.pack_into('<H', fleet, 2, 1)
        fleet[12] = 4
        fleet[14] = 1
        struct.pack_into('<H', players, 192 + 4, 0xa005)
        self.lib.stars_mark_fleet(fleet, players, 192, designs, 0x93, 5)
        self.assertEqual(struct.unpack_from('<H', fleet, 4)[0], 0x0105)
        self.assertEqual(struct.unpack_from('<H', fleet, 118)[0], 0x0010)
        self.assertEqual(struct.unpack_from('<H', players, 192 + 4)[0], 0xa001)
        self.assertEqual(struct.unpack_from('<H', designs, 0x7b)[0], 0x0100)
        self.assertEqual(struct.unpack_from('<H', designs, 2 * 0x93 + 0x7b)[0], 0x0100)

        self.lib.stars_mark_fleet(fleet, players, 192, designs, 0x93, 7)
        self.assertEqual(struct.unpack_from('<H', fleet, 4)[0], 0x0107)
        self.assertEqual(struct.unpack_from('<H', players, 192 + 4)[0], 0xa001)

    def test_new_turn_available_restores_turn_and_warp_bits(self):
        data = (C.c_ubyte * 65536)()
        struct.pack_into('<H', data, 0x82, 10)
        struct.pack_into('<H', data, 0x80, 0xa600)
        events = []
        open_type = C.CFUNCTYPE(C.c_int16, C.c_void_p, C.c_uint16,
                                C.c_int16, C.c_uint16)
        close_type = C.CFUNCTYPE(None, C.c_void_p)

        @open_type
        def open_file(_context, file_type, player, mode):
            events.append(('open', file_type, player, mode))
            struct.pack_into('<H', data, 0x82, 11)
            return 1

        @close_type
        def close_file(_context):
            events.append(('close',))

        self.assertEqual(self.lib.stars_new_turn_available(
            data, 3, C.cast(open_file, C.c_void_p),
            C.cast(close_file, C.c_void_p), None), 1)
        self.assertEqual(events, [('open', 0x2003, 3, 0x20), ('close',)])
        self.assertEqual(struct.unpack_from('<H', data, 0x82)[0], 10)
        self.assertEqual(struct.unpack_from('<H', data, 0x80)[0], 0xa600)

        @open_type
        def no_file(_context, file_type, player, mode):
            events.append(('fail', file_type, player, mode))
            return 0

        self.assertEqual(self.lib.stars_new_turn_available(
            data, 3, C.cast(no_file, C.c_void_p), None, None), 0)

    def test_check_file_modes_read_flags_and_restores_state(self):
        data = (C.c_ubyte * 65536)()
        struct.pack_into('<H', data, 0x74c, 7)
        struct.pack_into('<H', data, 0x80, 0x0600)
        struct.pack_into('<H', data, 0x7ca, 0x0008 | 0x0002)
        events = []
        open_type = C.CFUNCTYPE(C.c_int16, C.c_void_p, C.c_uint16,
                                C.c_int16, C.c_uint16)
        read_type = C.CFUNCTYPE(None, C.c_void_p)
        close_type = C.CFUNCTYPE(None, C.c_void_p)

        @open_type
        def open_file(_context, file_type, player, mode):
            events.append(('open', file_type, player, mode))
            return 1

        @read_type
        def read_record(_context):
            events.append(('read',))
            struct.pack_into('<H', data, 0x2696, 6 << 10)
            data[0x4b98] = 3
            struct.pack_into('<H', data, 0x4b9e, 1 << 9)

        @close_type
        def close_file(_context):
            events.append(('close',))

        struct.pack_into('<H', data, 0x4bac, 0x0000)
        self.assertEqual(self.lib.stars_check_file(
            data, 2, 3, 1, C.cast(open_file, C.c_void_p),
            C.cast(read_record, C.c_void_p), C.cast(close_file, C.c_void_p), None), 1)
        self.assertEqual(events, [('open', 2, 3, 0x20), ('close',)])
        self.assertEqual(struct.unpack_from('<H', data, 0x74c)[0], 7)
        self.assertEqual(struct.unpack_from('<H', data, 0x80)[0], 0x0600)
        self.assertEqual(struct.unpack_from('<H', data, 0x7ca)[0], 0x000a)

        events[:] = []
        self.assertEqual(self.lib.stars_check_file(
            data, 0, 3, 8, C.cast(open_file, C.c_void_p),
            C.cast(read_record, C.c_void_p), C.cast(close_file, C.c_void_p), None), 1)
        self.assertEqual(events, [('open', 0, 3, 0x20), ('read',), ('close',)])

    def test_write_orders_selects_compact_or_full_record_type(self):
        orders = (C.c_ubyte * (0x12 * 3))()
        orders[0x12 + 6] = 1
        orders[0x24 + 6] = 0x21
        for i in range(len(orders)):
            orders[i] = (i + 3) & 0xff
        orders[6] = 0
        orders[0x12 + 6] = 1
        orders[0x24 + 6] = 0x21
        calls = []
        callback_type = C.CFUNCTYPE(None, C.c_void_p, C.c_uint16,
                                    C.c_uint16, PTR)

        @callback_type
        def write_record(_context, record_type, size, record):
            calls.append((record_type, size, C.string_at(record, size)))

        self.lib.stars_write_orders(orders, 3,
                                    C.cast(write_record, C.c_void_p), None)
        self.assertEqual([(x[0], x[1]) for x in calls],
                         [(0x14, 8), (0x13, 0x12), (0x13, 0x12)])
        self.assertEqual(calls[0][2], bytes(orders[:8]))
        self.assertEqual(calls[1][2], bytes(orders[0x12:0x24]))

    def test_write_runtime_string_emits_compressed_length_prefixed_record(self):
        data = (C.c_ubyte * 65536)()
        input_text = (C.c_ubyte * 4)(*b'ABC\0')
        calls = []
        callback_type = C.CFUNCTYPE(None, C.c_void_p, C.c_uint16,
                                    C.c_uint16, PTR)

        @callback_type
        def write_record(_context, record_type, size, record):
            calls.append((record_type, size, C.string_at(record, size)))

        self.lib.stars_write_runtime_string(
            data, input_text, C.cast(write_record, C.c_void_p), None)
        self.assertEqual(len(calls), 1)
        self.assertEqual(calls[0][0], 0x15)
        self.assertEqual(calls[0][1], calls[0][2][0] + 1)
        self.assertGreater(calls[0][2][0], 0)

    def test_append_file_marks_then_writes_bof_only_on_success(self):
        events = []
        mark_type = C.CFUNCTYPE(C.c_int16, C.c_void_p, C.c_uint16,
                                C.c_int16, C.c_uint16, C.c_int16)
        bof_type = C.CFUNCTYPE(None, C.c_void_p, C.c_int16, C.c_int16,
                               C.c_int16)

        @mark_type
        def mark(_context, file_type, player, mode, enabled):
            events.append(('mark', file_type, player, mode, enabled))
            return 1

        @bof_type
        def bof(_context, player, record_type, multi):
            events.append(('bof', player, record_type, multi))

        self.assertEqual(self.lib.stars_append_file(
            4, C.cast(mark, C.c_void_p), C.cast(bof, C.c_void_p), None), 1)
        self.assertEqual(events, [('mark', 0x2003, 4, 4, 1), ('bof', 4, 3, 1)])

        events[:] = []
        @mark_type
        def fail(_context, file_type, player, mode, enabled):
            events.append(('mark', file_type, player, mode, enabled))
            return 0

        self.assertEqual(self.lib.stars_append_file(
            4, C.cast(fail, C.c_void_p), C.cast(bof, C.c_void_p), None), 0)
        self.assertEqual(events, [('mark', 0x2003, 4, 4, 1)])

    def test_write_battle_plan_selects_delete_and_log_record_paths(self):
        plan = (C.c_ubyte * 36)()
        plan[0:4] = b'H\x10AD'
        plan[4:9] = b'Alpha'
        plan[9] = 0
        dgroup = (C.c_ubyte * 65536)()
        calls = []
        callback_type = C.CFUNCTYPE(None, C.c_void_p, C.c_uint16,
                                    C.c_uint16, PTR)

        @callback_type
        def write_rt(_context, record_type, size, record):
            calls.append(('rt', record_type, size, C.string_at(record, size)))

        @callback_type
        def write_mem(_context, record_type, size, record):
            calls.append(('mem', record_type, size, C.string_at(record, size)))

        self.lib.stars_write_battle_plan(
            plan, dgroup, 0, C.cast(write_rt, C.c_void_p),
            C.cast(write_mem, C.c_void_p), None)
        self.assertEqual(calls[0][0], 'rt')
        self.assertEqual(calls[0][1], 0x1e)
        self.assertEqual(calls[0][3][:4], b'H\x10AD')
        self.assertGreater(calls[0][2], 4)

        calls[:] = []
        plan[1] = 0x40
        self.lib.stars_write_battle_plan(
            plan, dgroup, 1, C.cast(write_rt, C.c_void_p),
            C.cast(write_mem, C.c_void_p), None)
        self.assertEqual(calls, [('mem', 0x1e, 2, b'H@')])

    def test_unpack_battle_plan_copies_header_and_decodes_name(self):
        record = (C.c_ubyte * 37)()
        record[:4] = b'\x25\x10\x34\x12'
        record[4] = 0
        record[5:11] = b'Alpha\0'
        plan = (C.c_ubyte * 36)()
        dgroup = (C.c_ubyte * 65536)()
        self.lib.stars_unpack_battle_plan(dgroup, record, plan, 3)
        self.assertEqual(bytes(plan[:4]), b'\x35\x10\x34\x12')
        self.assertEqual(bytes(plan[4:10]), b'Alpha\0')

        compressed = (C.c_ubyte * 32)()
        source = (C.c_ubyte * 4)(*b'ABC\0')
        capacity = C.c_int16(31)
        self.assertEqual(self.lib.stars_compress_user_string(
            dgroup, source, compressed, C.byref(capacity)), 1)
        record[0:4] = b'\x08\x00\0\0'
        record[4] = capacity.value
        for i in range(capacity.value):
            record[5 + i] = compressed[i]
        plan[:] = b'\0' * 36
        self.lib.stars_unpack_battle_plan(dgroup, record, plan, 2)
        self.assertEqual(bytes(plan[4:8]), b'ABC\0')
        self.assertEqual(plan[0], 0x28)

    def test_ask_save_dialog_dispatches_commands_and_help(self):
        events = []
        end_type = C.CFUNCTYPE(None, C.c_void_p, C.c_uint16, C.c_int16)
        help_type = C.CFUNCTYPE(None, C.c_void_p, C.c_uint16, PTR,
                                C.c_uint16, C.c_uint16)

        @end_type
        def end_dialog(_context, hwnd, result):
            events.append(('end', hwnd, result))

        @help_type
        def win_help(_context, hwnd, filename, command, data):
            events.append(('help', hwnd, C.string_at(filename), command, data))

        help_file = (C.c_ubyte * 9)(*b'HELP.HLP\0')
        end_ptr = C.cast(end_dialog, C.c_void_p)
        help_ptr = C.cast(win_help, C.c_void_p)
        self.assertEqual(self.lib.stars_ask_save_dialog(
            0x1234, 0x111, 0x429, 0, help_file, end_ptr, help_ptr, None), 1)
        self.assertEqual(self.lib.stars_ask_save_dialog(
            0x1234, 0x111, 0x42a, 0, help_file, end_ptr, help_ptr, None), 1)
        self.assertEqual(self.lib.stars_ask_save_dialog(
            0x1234, 0x111, 0x42b, 0, help_file, end_ptr, help_ptr, None), 1)
        self.assertEqual(self.lib.stars_ask_save_dialog(
            0x1234, 0x111, 0x76, 0, help_file, end_ptr, help_ptr, None), 1)
        self.assertEqual(events, [
            ('end', 0x1234, 1), ('end', 0x1234, -1),
            ('end', 0x1234, 0), ('help', 0x1234, b'HELP.HLP', 1, 0x442)])
        self.assertEqual(self.lib.stars_ask_save_dialog(
            0x1234, 2, 0, 0, help_file, end_ptr, help_ptr, None), 0)
        self.assertEqual(self.lib.stars_ask_save_dialog(
            0x1234, 0x110, 0, 0, help_file, end_ptr, help_ptr, None), 1)

    def test_prompt_save_game_selects_dialog_and_writes_after_success(self):
        data = (C.c_ubyte * 65536)()
        struct.pack_into('<HHHHH', data, 0x80, 0, 0, 0, 0, 0)
        struct.pack_into('<H', data, 0x5310, 0x1111)
        struct.pack_into('<H', data, 0x258c, 0x2222)
        struct.pack_into('<H', data, 0x18c, 4)
        struct.pack_into('<H', data, 0x7ca, 0x0003)
        data[0x56a2:0x56a7] = b'base\0'
        events = []
        make_type = C.CFUNCTYPE(C.c_void_p, C.c_void_p, C.c_uint16)
        dialog_type = C.CFUNCTYPE(C.c_int16, C.c_void_p, C.c_uint16,
                                  C.c_uint16, C.c_uint16, C.c_void_p)
        free_type = C.CFUNCTYPE(None, C.c_void_p, C.c_void_p)
        log_type = C.CFUNCTYPE(None, C.c_void_p, PTR, C.c_int16)
        hist_type = C.CFUNCTYPE(None, C.c_void_p, C.c_int16)

        @make_type
        def make_proc(_context, instance):
            events.append(('make', instance))
            return 0x3333

        @dialog_type
        def dialog_box(_context, instance, resource, frame, procedure):
            events.append(('dialog', instance, resource, frame, procedure))
            return -1

        @free_type
        def free_proc(_context, procedure):
            events.append(('free', procedure))

        @log_type
        def write_log(_context, base, player):
            events.append(('log', C.string_at(base), player))

        @hist_type
        def write_history(_context, player):
            events.append(('history', player))

        ops = PromptSaveOps(C.cast(make_proc, C.c_void_p),
                            C.cast(dialog_box, C.c_void_p),
                            C.cast(free_proc, C.c_void_p),
                            C.cast(write_log, C.c_void_p),
                            C.cast(write_history, C.c_void_p))
        self.lib.stars_prompt_save_game(data, C.byref(ops), None)
        self.assertEqual(events, [
            ('make', 0x1111), ('dialog', 0x1111, 0x42c, 0x2222, 0x3333),
            ('free', 0x3333), ('log', b'base', 4), ('history', 4)])
        self.assertEqual(struct.unpack_from('<H', data, 0x7ca)[0], 0x0013)

        events[:] = []
        struct.pack_into('<H', data, 0x80, 4)
        self.lib.stars_prompt_save_game(data, C.byref(ops), None)
        self.assertEqual(events[1][2], 0x7e9)

    def test_set_visible_plan_fleet_preserves_pass_and_progress_order(self):
        data = (C.c_ubyte * 65536)()
        struct.pack_into('<H', data, 0x7a, 37)
        events = []
        step_type = C.CFUNCTYPE(None, C.c_void_p, C.c_int16)
        progress_type = C.CFUNCTYPE(None, C.c_void_p, C.c_int16)

        @step_type
        def initialize(_context, player): events.append(('init', player))
        @step_type
        def fleets(_context, player): events.append(('fleets', player))
        @step_type
        def planets(_context, player): events.append(('planets', player))
        @step_type
        def things(_context, player): events.append(('things', player))
        @step_type
        def finish(_context, player): events.append(('finish', player))
        @progress_type
        def progress(_context, value): events.append(('progress', value))

        ops = VisiblePlanFleetOps(
            C.cast(initialize, C.c_void_p), C.cast(fleets, C.c_void_p),
            C.cast(planets, C.c_void_p), C.cast(things, C.c_void_p),
            C.cast(finish, C.c_void_p), C.cast(progress, C.c_void_p))
        self.lib.stars_set_visible_plan_fleet(data, 3, C.byref(ops), None)
        self.assertEqual(events, [
            ('init', 3), ('progress', -927), ('fleets', 3),
            ('progress', -927), ('planets', 3), ('progress', -927),
            ('things', 3), ('finish', 3)])
        events[:] = []
        self.lib.stars_set_visible_plan_fleet(data, -1, C.byref(ops), None)
        self.assertEqual(events, [('init', -1)])
        self.assertEqual(struct.unpack_from('<H', data, 0x59a4)[0], 37)

    def test_ensure_cas_hdefs_is_the_recovered_empty_hook(self):
        self.assertIsNone(self.lib.stars_ensure_cas_hdefs(7))

    def test_cancel_mem_rt_rewinds_cursor_and_masks_previous_header(self):
        data = (C.c_ubyte * 65536)()
        struct.pack_into('<H', data, 0x5260, 0x0c05)
        struct.pack_into('<H', data, 0x09a4, 0x0010)
        self.lib.stars_cancel_mem_rt(data, 9)
        self.assertEqual(struct.unpack_from('<H', data, 0x5260)[0], 5)
        self.assertEqual(struct.unpack_from('<H', data, 0x09a4)[0], 9)
        struct.pack_into('<H', data, 0x09a4, 1)
        self.lib.stars_cancel_mem_rt(data, 0)
        self.assertEqual(struct.unpack_from('<H', data, 0x09a4)[0], 0xfffa)

    def test_dirty_game_updates_only_on_change_and_skips_ai_title_update(self):
        data = (C.c_ubyte * 65536)()
        struct.pack_into('<H', data, 0x7e, 0)
        struct.pack_into('<H', data, 0x1442, 0)
        struct.pack_into('<H', data, 0x198, 0x2468)
        events = []
        title_type = C.CFUNCTYPE(None, C.c_void_p, C.c_uint16)

        @title_type
        def set_title(_context, hwnd): events.append(hwnd)

        callback = C.cast(set_title, C.c_void_p)
        self.lib.stars_dirty_game(data, 1, callback, None)
        self.assertEqual(events, [0x2468])
        self.assertEqual(struct.unpack_from('<H', data, 0x7e)[0], 1)
        self.lib.stars_dirty_game(data, 1, callback, None)
        self.assertEqual(events, [0x2468])
        struct.pack_into('<H', data, 0x1442, 1)
        self.lib.stars_dirty_game(data, 0, callback, None)
        self.assertEqual(events, [0x2468])
        self.assertEqual(struct.unpack_from('<H', data, 0x7e)[0], 0)

    def test_get_fuel_free_calls_stat_one_and_subtracts_minimum_as_long(self):
        fleet = (C.c_ubyte * 124)()
        struct.pack_into('<I', fleet, 0x5c, 250)
        calls = []
        stat_type = C.CFUNCTYPE(C.c_uint32, C.c_void_p, PTR, C.c_uint16)

        @stat_type
        def get_stat(_context, record, stat):
            calls.append((C.addressof(record.contents), stat))
            return 1000

        self.assertEqual(self.lib.stars_get_fuel_free(
            fleet, C.cast(get_stat, C.c_void_p), None), 750)
        self.assertEqual(calls[0][1], 1)
        struct.pack_into('<I', fleet, 0x5c, 0x1001)
        self.assertEqual(self.lib.stars_get_fuel_free(
            fleet, C.cast(get_stat, C.c_void_p), None), 0xfffff3e7)

    def test_get_cargo_free_accumulates_four_dwords_with_carry(self):
        fleet = (C.c_ubyte * 124)()
        values = (0xffff, 1, 0x10000, 2)
        for i, value in enumerate(values):
            struct.pack_into('<I', fleet, 0x4c + i * 4, value)
        calls = []
        stat_type = C.CFUNCTYPE(C.c_uint32, C.c_void_p, PTR, C.c_uint16)

        @stat_type
        def get_stat(_context, _record, stat):
            calls.append(stat)
            return 0x30000

        self.assertEqual(self.lib.stars_get_cargo_free(
            fleet, C.cast(get_stat, C.c_void_p), None), 0x0fffe)
        self.assertEqual(calls, [2])

    def test_fuel_tanker_accepts_only_the_two_tanker_hull_ids(self):
        ship = (C.c_ubyte * 4)()
        for hull, expected in ((0x18, 0), (0x19, 1), (0x1a, 1), (0x1b, 0)):
            struct.pack_into('<H', ship, 0, hull)
            self.assertEqual(self.lib.stars_fuel_tanker(ship), expected)

    def test_logical_to_scan_inverts_galactic_y_and_preserves_transform_order(self):
        data = (C.c_ubyte * 65536)()
        struct.pack_into('<HHH', data, 0, 1000, 700, 0)
        struct.pack_into('<H', data, 0x2572, 100)
        struct.pack_into('<H', data, 0x2570, 200)
        struct.pack_into('<H', data, 0xf2, 5000)
        values = []
        scan_type = C.CFUNCTYPE(C.c_int16, C.c_void_p, C.c_int16)

        @scan_type
        def pt_to_scan(_context, value):
            values.append(value)
            return value

        self.lib.stars_logical_to_scan(
            data, C.cast(pt_to_scan, C.c_void_p), None)
        self.assertEqual(values, [900, 4100])
        self.assertEqual(struct.unpack_from('<HH', data, 0), (900, 4100))

    def test_scan_to_logical_restores_offsets_and_applies_signed_clamps(self):
        data = (C.c_ubyte * 65536)()
        struct.pack_into('<HHHHH', data, 0, 5000, 500, 6000, 200, 100)
        struct.pack_into('<H', data, 0xf0, 6000)
        struct.pack_into('<H', data, 0xf2, 6000)
        struct.pack_into('<H', data, 0x2572, 100)
        struct.pack_into('<H', data, 0x2570, 200)
        calls = []
        scan_type = C.CFUNCTYPE(C.c_int16, C.c_void_p, C.c_int16)

        @scan_type
        def scan_to_pt(_context, value):
            calls.append(value)
            return value

        self.lib.stars_scan_to_logical(
            data, C.cast(scan_to_pt, C.c_void_p), None)
        self.assertEqual(calls, [5000, 500])
        self.assertEqual(struct.unpack_from('<HH', data, 0), (5100, 5300))

        struct.pack_into('<HH', data, 0, 7000, 10000)
        self.lib.stars_scan_to_logical(
            data, C.cast(scan_to_pt, C.c_void_p), None)
        self.assertEqual(struct.unpack_from('<HH', data, 0), (7000, 1000))

    def test_idm_get_message_n_returns_id_and_sets_failure_sentinel(self):
        message_type = C.CFUNCTYPE(C.c_int16, C.c_void_p, C.c_int16, PTR)

        @message_type
        def success(_context, message, record):
            value = message + 10
            record[0] = value & 0xff
            record[1] = (value >> 8) & 0xff
            return 1

        self.assertEqual(self.lib.stars_idm_get_message_n(
            7, C.cast(success, C.c_void_p), None), 17)

        @message_type
        def failure(_context, _message, record):
            record[0] = 0x34
            record[1] = 0x12
            return 0

        self.assertEqual(self.lib.stars_idm_get_message_n(
            7, C.cast(failure, C.c_void_p), None), -1)

    def test_psz_get_message_n_returns_empty_fallback_or_formats_parameters(self):
        message_type = C.CFUNCTYPE(C.c_int16, C.c_void_p, C.c_int16, PTR)
        format_type = C.CFUNCTYPE(C.c_void_p, C.c_void_p, C.c_int16, PTR)
        output = (C.c_ubyte * 32)()
        seen = []
        formatted = (C.c_ubyte * 8)(*b'FORMATT\0')

        @message_type
        def success(_context, message, record):
            record[0] = 0x34
            record[1] = 0x12
            record[4] = 0xaa
            record[5] = 0xbb
            return 1

        @format_type
        def format_message(_context, message, params):
            seen.append((message, params[0], params[1]))
            return C.addressof(formatted)

        result = self.lib.stars_psz_get_message_n(
            4, C.cast(success, C.c_void_p), C.cast(format_message, C.c_void_p),
            output, None)
        self.assertEqual(C.string_at(result), b'FORMATT')
        self.assertEqual(seen, [(0x1234, 0xaa, 0xbb)])

        @message_type
        def failure(_context, _message, _record): return 0

        output[0] = 0x7f
        result = self.lib.stars_psz_get_message_n(
            4, C.cast(failure, C.c_void_p), C.cast(format_message, C.c_void_p),
            output, None)
        self.assertEqual(C.addressof(result.contents), C.addressof(output))
        self.assertEqual(output[0], 0)

    def test_can_split_and_can_split_all_preserve_fleet_and_boat_bounds(self):
        data = (C.c_ubyte * 65536)()
        struct.pack_into('<H', data, 0x18c, 2)
        struct.pack_into('<H', data, 0x59a2 + 2 * 0xc0 + 4, 7)
        self.assertEqual(self.lib.stars_can_split(data, -1), 0)
        self.assertEqual(self.lib.stars_can_split(data, 1), 0)
        self.assertEqual(self.lib.stars_can_split(data, 2), 1)
        self.assertEqual(self.lib.stars_can_split(data, 0x10000), 1)
        self.assertEqual(self.lib.stars_can_split_all(data, 1), 0)
        self.assertEqual(self.lib.stars_can_split_all(data, 2), 1)
        self.assertEqual(self.lib.stars_can_split_all(data, 0x1f9), 1)
        self.assertEqual(self.lib.stars_can_split_all(data, 0x1fb), 0)
        struct.pack_into('<H', data, 0x59a2 + 2 * 0xc0 + 4, 0x0200)
        self.assertEqual(self.lib.stars_can_split(data, 2), 0)

    def test_can_merge_counts_same_owner_position_and_capacity(self):
        first_counts = (C.c_uint16 * 16)(4)
        second_counts = (C.c_uint16 * 16)(5)
        target = MergeFleet(2, 10, 20, first_counts, 1)
        fleets = (MergeFleet * 3)(
            target,
            MergeFleet(2, 10, 20, second_counts, 1),
            MergeFleet(7, 10, 20, (C.c_uint16 * 16)(99), 1))
        self.assertEqual(self.lib.stars_can_merge(
            C.byref(target), fleets, 3, 100), 1)
        self.assertEqual(self.lib.stars_can_merge(
            C.byref(target), fleets, 3, 32760), 0)

        one = (MergeFleet * 2)(target, MergeFleet(0, 0, 0,
                                                   (C.c_uint16 * 16)(), 0))
        self.assertEqual(self.lib.stars_can_merge(
            C.byref(target), one, 2, 100), 0)

    def test_set_fleet_dropdown_counts_prior_fleets_and_refreshes_preview(self):
        fleets = (DropDownFleet * 4)(
            DropDownFleet(3, 10, 20), DropDownFleet(4, 10, 20),
            DropDownFleet(5, 11, 20), DropDownFleet(6, 10, 20))
        calls = []
        send_type = C.CFUNCTYPE(None, C.c_void_p, C.c_uint16, C.c_uint16,
                                C.c_uint16, C.c_uint32)
        draw_type = C.CFUNCTYPE(None, C.c_void_p, C.c_uint16, C.c_uint16)

        @send_type
        def send(_context, window, message, value, extra):
            calls.append(('send', window, message, value, extra))

        @draw_type
        def draw(_context, x, code):
            calls.append(('draw', x, code))

        ops = SetFleetDropDownOps(C.cast(send, C.c_void_p),
                                  C.cast(draw, C.c_void_p))
        self.lib.stars_set_fleet_dropdown_sel(
            0x1234, 6, 2, 4, 10, 20, fleets, 4, C.byref(ops), None)
        self.assertEqual(calls, [('send', 0x1234, 0x40e, 1, 0),
                                 ('draw', 0, 0x4004)])

        calls[:] = []
        self.lib.stars_set_fleet_dropdown_sel(
            0x1234, 5, 0, 0, 10, 20, fleets, 4, C.byref(ops), None)
        self.assertEqual(calls[0], ('send', 0x1234, 0x40e, 2, 0))

    def test_wt_max_ship_design_stat_applies_component_multiplicity(self):
        components = (ShipComponentStat * 6)(
            ShipComponentStat(0x1000, (2 << 8) | 5),
            ShipComponentStat(0x1000, (3 << 8) | 6),
            ShipComponentStat(0x1000, (2 << 8) | 2),
            ShipComponentStat(0x1000, (3 << 8) | 3),
            ShipComponentStat(0x1000, (1 << 8) | 4),
            ShipComponentStat(0x0800, (2 << 8) | 0x10))
        design = ShipDesignStat(6, components)
        hull = HullStat(100, 200)
        self.assertEqual(self.lib.stars_wt_max_ship_design_stat(
            C.byref(design), C.byref(hull), 1), 2500)
        self.assertEqual(self.lib.stars_wt_max_ship_design_stat(
            C.byref(design), C.byref(hull), 2), 850)
        self.assertEqual(self.lib.stars_wt_max_ship_design_stat(
            C.byref(design), C.byref(hull), 3), 0)

    def test_get_fleet_stat_preserves_state_sentinel_and_signed_design_totals(self):
        design = ShipDesignStat(0, None)
        hull = HullStat(100, 200)
        entries = (FleetStatEntry * 2)(
            FleetStatEntry(2, C.pointer(design), C.pointer(hull)),
            FleetStatEntry(-1, C.pointer(design), C.pointer(hull)))
        self.assertEqual(self.lib.stars_get_fleet_stat(
            entries, 2, 7, 1), 100)
        self.assertEqual(self.lib.stars_get_fleet_stat(
            entries, 2, 7, 2), 200)
        self.assertEqual(self.lib.stars_get_fleet_stat(
            entries, 2, 6, 1), 32000)

    def test_enum_calc_jettison_subtracts_selected_byte_and_long_amounts(self):
        record = JettisonRecord(9, 0x42, 0x05,
                                (3, 7, 0, 0, 0),
                                (100, 200, 300, 400, 500))
        minerals = (C.c_uint32 * 5)(100, 200, 300, 400, 500)
        lookup_type = C.CFUNCTYPE(C.c_int16, C.c_void_p, C.c_uint16,
                                  C.POINTER(C.c_int16), C.POINTER(C.c_int16))

        @lookup_type
        def lookup(_context, fleet_id, x, y):
            if fleet_id not in (7, 9):
                return 0
            x[0], y[0] = 10, 20
            return 1

        self.assertEqual(self.lib.stars_enum_calc_jettison(
            C.byref(record), 1, minerals, 7, C.cast(lookup, C.c_void_p),
            None), 1)
        self.assertEqual(list(minerals), [97, 200, 293, 400, 500])

        minerals[:] = (100, 200, 300, 400, 500)
        self.assertEqual(self.lib.stars_enum_calc_jettison(
            C.byref(record), 2, minerals, 7, C.cast(lookup, C.c_void_p),
            None), 1)
        self.assertEqual(list(minerals), [0, 200, 100, 400, 500])

        record.flags = 0x41
        self.assertEqual(self.lib.stars_enum_calc_jettison(
            C.byref(record), 1, minerals, 7, C.cast(lookup, C.c_void_p),
            None), 1)

    def test_xfer_supply_clamps_to_source_and_rolls_back_actual_destination(self):
        endpoints = (TransferEndpoint * 2)(
            TransferEndpoint(1, 10, None), TransferEndpoint(2, 20, None))
        calls = []
        change_type = C.CFUNCTYPE(C.c_int32, C.c_void_p, C.c_int16,
                                  C.c_uint16, C.c_int16, C.c_int32,
                                  C.c_void_p)

        @change_type
        def change(_context, group, identifier, supply, quantity, _object):
            calls.append((group, identifier, supply, quantity))
            if quantity == 0:
                return 50 if identifier == 20 else 70
            return quantity - 5

        self.assertEqual(self.lib.stars_xfer_supply(
            3, 80, endpoints, C.cast(change, C.c_void_p), None), 45)
        self.assertEqual(calls, [(2, 20, 3, 0), (1, 10, 3, 50),
                                 (2, 20, 3, -45)])

        calls[:] = []
        self.assertEqual(self.lib.stars_xfer_supply(
            3, -80, endpoints, C.cast(change, C.c_void_p), None), 65)
        self.assertEqual(calls, [(1, 10, 3, 0), (2, 20, 3, 70),
                                 (1, 10, 3, -65)])
        self.assertEqual(self.lib.stars_xfer_supply(
            3, 0, endpoints, C.cast(change, C.c_void_p), None), 0)

    def test_change_cargo_handles_planet_packet_and_fleet_capacity_rules(self):
        planet = CargoState()
        planet.minerals[:] = (100, 200, 300, 400, 500)
        self.assertEqual(self.lib.stars_change_cargo(
            1, 7, 1, 0, C.byref(planet), None, None), 200)
        self.assertEqual(self.lib.stars_change_cargo(
            1, 7, 1, -300, C.byref(planet), None, None), -200)
        self.assertEqual(planet.minerals[1], 0)
        self.assertEqual(self.lib.stars_change_cargo(
            4, 0, 4, 90, None, None, None), 0)

        packet = CargoState()
        packet.cargo[:] = (30, 40, 50)
        packet.thing_capacity = 20
        self.assertEqual(self.lib.stars_change_cargo(
            8, 2, 0, 100, C.byref(packet), None, None), 80)
        self.assertEqual(list(packet.cargo), [110, 40, 50])
        self.assertEqual(self.lib.stars_change_cargo(
            8, 2, 0, -200, C.byref(packet), None, None), -110)
        self.assertEqual(packet.cargo[0], 0)
        self.assertEqual(self.lib.stars_change_cargo(
            8, 2, 3, 10, C.byref(packet), None, None), 0)

        fleet = CargoState()
        fleet.minerals[:] = (10, 20, 30, 40, 50)
        fleet.fleet_flags = 7
        free_type = C.CFUNCTYPE(C.c_uint32, C.c_void_p,
                                C.POINTER(CargoState))

        @free_type
        def cargo_free(_context, _state):
            return 60

        @free_type
        def fuel_free(_context, _state):
            return 40

        ops = ChgCargoOps(None, None, C.cast(cargo_free, C.c_void_p),
                          C.cast(fuel_free, C.c_void_p))
        self.assertEqual(self.lib.stars_change_cargo(
            2, 3, 0, 80, C.byref(fleet), C.byref(ops), None), 60)
        self.assertEqual(fleet.minerals[0], 70)
        self.assertEqual(self.lib.stars_change_cargo(
            2, 3, 3, 10, C.byref(fleet), C.byref(ops), None), 10)
        fleet.fleet_flags = 6
        self.assertEqual(self.lib.stars_change_cargo(
            2, 3, 3, 10, C.byref(fleet), C.byref(ops), None), 0)
        self.assertEqual(self.lib.stars_change_cargo(
            2, 3, 4, 50, C.byref(fleet), C.byref(ops), None), 40)

    def test_change_cargo_resolves_and_stores_null_planet_objects(self):
        planet = CargoState()
        planet.minerals[0] = 25
        events = []
        lookup_type = C.CFUNCTYPE(C.c_void_p, C.c_void_p,
                                  C.c_int16, C.c_uint16)
        store_type = C.CFUNCTYPE(None, C.c_void_p, C.c_int16, C.c_uint16,
                                 C.POINTER(CargoState))

        @lookup_type
        def lookup(_context, group, identifier):
            events.append(('lookup', group, identifier))
            return C.addressof(planet)

        @store_type
        def store(_context, group, identifier, state):
            events.append(('store', group, identifier, state.contents.minerals[0]))

        ops = ChgCargoOps(C.cast(lookup, C.c_void_p), C.cast(store, C.c_void_p),
                          None, None)
        self.assertEqual(self.lib.stars_change_cargo(
            1, 9, 0, 10, None, C.byref(ops), None), 10)
        self.assertEqual(planet.minerals[0], 35)
        self.assertEqual(events, [('lookup', 1, 9), ('store', 1, 9, 35)])

    def test_update_xfer_buttons_filters_active_supply_ranges_and_toggles_disabled(self):
        buttons = (TransferButton * 4)(
            TransferButton(0, 0x01, 0),
            TransferButton(4, 0x0d, 4),
            TransferButton(5, 0x05, 0),
            TransferButton(7, 0x00, 4))
        calls = []
        availability_type = C.CFUNCTYPE(C.c_int32, C.c_void_p, C.c_uint16,
                                        C.c_int16, C.c_int16)

        @availability_type
        def availability(_context, side, supply, mode):
            calls.append((side, supply, mode))
            return 0 if supply == 0 else 20

        self.lib.stars_update_xfer_buttons(
            0, 0, buttons, 4, C.cast(availability, C.c_void_p), None)
        self.assertEqual(calls, [(0, 0, 0), (3, 4, 0)])
        self.assertEqual([b.button_state for b in buttons], [4, 0, 0, 4])

        calls[:] = []
        self.lib.stars_update_xfer_buttons(
            1, 3, buttons, 4, C.cast(availability, C.c_void_p), None)
        self.assertEqual(calls, [(0, 0, 1), (3, 4, 1), (1, 5, 1)])
        self.assertEqual([b.button_state for b in buttons], [4, 0, 0, 4])

    def test_draw_xfer_dlg_paints_buttons_and_dispatches_each_object_side(self):
        whole = Rect(0, 0, 100, 60)
        states = (CargoState * 2)()
        sides = (TransferSide * 2)(TransferSide(2, C.pointer(states[0])),
                                   TransferSide(8, C.pointer(states[1])))
        buttons = (TransferButton * 2)(
            TransferButton(0, 1, 2, (1, 2, 3, 4)),
            TransferButton(1, 0, 7, (5, 6, 7, 8)))
        events = []
        acquire_type = C.CFUNCTYPE(C.c_uint16, C.c_void_p, C.c_uint16)
        release_type = C.CFUNCTYPE(None, C.c_void_p, C.c_uint16, C.c_uint16)
        paint_type = C.CFUNCTYPE(None, C.c_void_p, C.c_uint16, C.c_uint16,
                                 C.c_uint16, C.c_uint16, C.c_uint16,
                                 C.c_uint16)
        button_type = C.CFUNCTYPE(None, C.c_void_p, C.c_uint16,
                                  C.POINTER(Rect),
                                  C.c_uint16)
        side_type = C.CFUNCTYPE(None, C.c_void_p, C.c_uint16,
                                C.POINTER(Rect), C.POINTER(CargoState),
                                C.c_int16)

        @acquire_type
        def acquire(_context, window):
            events.append(('acquire', window))
            return 55

        @release_type
        def release(_context, window, hdc):
            events.append(('release', window, hdc))

        @paint_type
        def paint(_context, hdc, x, y, width, height, rop):
            events.append(('paint', hdc, x, y, width, height, rop))

        @button_type
        def draw_button(_context, hdc, rect, state):
            events.append(('button', hdc, (rect.contents.left, rect.contents.top,
                                           rect.contents.right, rect.contents.bottom),
                           state))

        @side_type
        def draw_cargo(_context, hdc, rect, _state, supply):
            events.append(('cargo', hdc, rect.contents.right, supply))

        @side_type
        def draw_thing(_context, hdc, rect, _state, supply):
            events.append(('thing', hdc, rect.contents.left, supply))

        ops = DrawXferOps(C.cast(acquire, C.c_void_p), C.cast(release, C.c_void_p),
                          C.cast(paint, C.c_void_p), C.cast(draw_button, C.c_void_p),
                          None, C.cast(draw_cargo, C.c_void_p), None,
                          C.cast(draw_thing, C.c_void_p))
        self.lib.stars_draw_xfer_dlg(
            9, 0, C.byref(whole), -1, 0, 8, buttons, 2, sides,
            C.byref(ops), None)
        self.assertEqual(events[0:3], [('acquire', 9),
                                       ('paint', 55, 50, 0, 1, 60, 0x42),
                                       ('button', 55, (1, 2, 3, 4), 2)])
        self.assertEqual(events[3:], [('cargo', 55, 35, -1),
                                      ('thing', 55, 65, -1),
                                      ('release', 9, 55)])

        events[:] = []
        self.lib.stars_draw_xfer_dlg(
            9, 55, C.byref(whole), -2, 1, 8, buttons, 2, sides,
            C.byref(ops), None)
        self.assertEqual(events, [('paint', 55, 50, 0, 1, 60, 0x42),
                                  ('button', 55, (1, 2, 3, 4), 2)])

    def test_setup_xfer_buttons_builds_rows_maps_hulls_and_adds_side_controls(self):
        whole = Rect(0, 0, 200, 100)
        groups = (C.c_int16 * 2)(1, 2)
        hull_map = (C.c_uint16 * 2)(10, 12)
        buttons = (TransferButton * 32)()
        availability_type = C.CFUNCTYPE(C.c_int32, C.c_void_p, C.c_uint16,
                                        C.c_int16, C.c_int16)

        @availability_type
        def availability(_context, _side, supply, _mode):
            return 0 if supply == 10 else 1

        self.assertEqual(self.lib.stars_setup_xfer_buttons(
            C.byref(whole), 1, 2, 8, groups, hull_map, buttons, 32,
            C.cast(availability, C.c_void_p), None), 1)
        self.assertEqual([buttons[i].supply for i in range(4)], [10, 10, 12, 12])
        self.assertEqual([buttons[i].button_state for i in range(4)], [6, 7, 2, 3])
        self.assertEqual(tuple(buttons[0].rect),
                         (90, 18, 101, 29))

        groups = (C.c_int16 * 2)(1, 2)
        buttons = (TransferButton * 32)()
        self.assertEqual(self.lib.stars_setup_xfer_buttons(
            C.byref(whole), 0, 0, 8, groups, None, buttons, 32,
            None, None), 1)
        self.assertEqual(sum(1 for i in range(32) if buttons[i].supply >= 0x80), 5)
        self.assertEqual(buttons[10].supply, 0x80)
        self.assertEqual(buttons[10].flags, 4)

    def test_draw_thing_xfer_side_preserves_title_labels_and_selected_gauges(self):
        area = Rect(10, 20, 90, 100)
        thing = CargoState()
        events = []
        label_type = C.CFUNCTYPE(C.c_void_p, C.c_void_p, C.c_int16)
        color_type = C.CFUNCTYPE(None, C.c_void_p, C.c_uint16, C.c_uint16)
        frame_type = C.CFUNCTYPE(None, C.c_void_p, C.c_uint16,
                                 C.POINTER(Rect), C.c_uint16)
        centered_type = C.CFUNCTYPE(None, C.c_void_p, C.c_uint16,
                                    C.POINTER(Rect), C.c_char_p, C.c_int16)
        right_type = C.CFUNCTYPE(None, C.c_void_p, C.c_uint16, C.c_int16,
                                 C.c_int16, C.c_char_p, C.c_int16, C.c_int16)
        gauge_type = C.CFUNCTYPE(None, C.c_void_p, C.c_uint16,
                                 C.POINTER(Rect), C.POINTER(CargoState),
                                 C.c_int16)

        @label_type
        def thing_name(_context, identifier):
            return C.addressof(name_buffer if identifier == 42 else unknown_buffer)

        @label_type
        def label(_context, identifier):
            return C.addressof(label_buffers[identifier])

        name_buffer = C.create_string_buffer(b'thing')
        unknown_buffer = C.create_string_buffer(b'unknown')
        label_buffers = {i: C.create_string_buffer(('label%d' % i).encode('ascii'))
                         for i in range(0x1ae, 0x1b3)}

        @color_type
        def text_color(_context, hdc, value): events.append(('text_color', hdc, value))

        @color_type
        def background(_context, hdc, value): events.append(('background', hdc, value))

        @color_type
        def font(_context, hdc, value): events.append(('font', hdc, value))

        @frame_type
        def frame(_context, hdc, rect, state):
            events.append(('frame', hdc, tuple((rect.contents.left, rect.contents.top,
                                                rect.contents.right, rect.contents.bottom)), state))

        @centered_type
        def centered(_context, hdc, _rect, text, state):
            events.append(('centered', hdc, text, state))

        @right_type
        def right(_context, hdc, x, y, text, width, erase):
            events.append(('right', hdc, x, y, text, width, erase))

        @gauge_type
        def gauge(_context, hdc, _rect, _state, index):
            events.append(('gauge', hdc, index))

        ops = DrawThingXferOps(C.cast(thing_name, C.c_void_p),
                               C.cast(label, C.c_void_p),
                               C.cast(text_color, C.c_void_p),
                               C.cast(background, C.c_void_p),
                               C.cast(font, C.c_void_p), C.cast(frame, C.c_void_p),
                               C.cast(centered, C.c_void_p), C.cast(right, C.c_void_p),
                               C.cast(gauge, C.c_void_p))
        self.lib.stars_draw_thing_xfer_side(
            7, C.byref(area), C.byref(thing), 42, -1, 8, 0x11, 0x22,
            C.byref(ops), None)
        self.assertEqual(events[0:5], [('text_color', 7, 0x11),
                                       ('background', 7, 0x22), ('font', 7, 1),
                                       ('frame', 7, (10, 20, 90, 100), 0),
                                       ('frame', 7, (11, 21, 89, 31), 0)])
        self.assertEqual(events[5], ('centered', 7, b'thing', 0))
        self.assertEqual([event[0:2] for event in events[6:10]],
                         [('right', 7), ('right', 7), ('right', 7), ('right', 7)])
        self.assertEqual([event[0:2] for event in events[10:]],
                         [('gauge', 7), ('gauge', 7), ('gauge', 7), ('gauge', 7)])

        events[:] = []
        self.lib.stars_draw_thing_xfer_side(
            7, C.byref(area), C.byref(thing), 42, 2, 8, 0, 0,
            C.byref(ops), None)
        self.assertEqual([event for event in events if event[0] == 'gauge'],
                         [('gauge', 7, 5), ('gauge', 7, 2)])
        events[:] = []
        self.lib.stars_draw_thing_xfer_side(
            7, C.byref(area), C.byref(thing), 42, 3, 8, 0, 0,
            C.byref(ops), None)
        self.assertEqual([event for event in events if event[0] == 'gauge'], [])

    def test_draw_fleet_cargo_xfer_side_separates_owned_gauges_from_foreign_minerals(self):
        area = Rect(0, 0, 100, 80)
        fleet = CargoState()
        fleet.owner = 3
        events = []
        string_type = C.CFUNCTYPE(C.c_void_p, C.c_void_p, C.c_int16)
        color_type = C.CFUNCTYPE(None, C.c_void_p, C.c_uint16, C.c_uint16)
        frame_type = C.CFUNCTYPE(None, C.c_void_p, C.c_uint16,
                                 C.POINTER(Rect), C.c_uint16)
        centered_type = C.CFUNCTYPE(None, C.c_void_p, C.c_uint16,
                                    C.POINTER(Rect), C.c_char_p, C.c_int16)
        right_type = C.CFUNCTYPE(None, C.c_void_p, C.c_uint16, C.c_int16,
                                 C.c_int16, C.c_char_p, C.c_int16, C.c_int16)
        gauge_type = C.CFUNCTYPE(None, C.c_void_p, C.c_uint16,
                                 C.POINTER(Rect), C.POINTER(CargoState),
                                 C.c_int16)
        mineral_type = C.CFUNCTYPE(None, C.c_void_p, C.c_uint16,
                                   C.POINTER(Rect), C.POINTER(CargoState),
                                   C.c_int16, C.c_int16)

        @string_type
        def string(_context, identifier):
            return C.addressof(string_buffers[identifier])

        string_buffers = {i: C.create_string_buffer(('s%d' % i).encode('ascii'))
                          for i in range(0x0, 0x1b2)}

        @color_type
        def color(_context, _hdc, _value): pass

        @frame_type
        def frame(_context, _hdc, _rect, _state): pass

        @centered_type
        def centered(_context, _hdc, _rect, _text, _state): pass

        @right_type
        def right(_context, _hdc, _x, _y, _text, _width, _erase): pass

        @gauge_type
        def gauge(_context, _hdc, _rect, _state, index): events.append(('gauge', index))

        @mineral_type
        def mineral(_context, _hdc, _rect, _state, index, selected):
            events.append(('mineral', index, selected))

        ops = DrawFleetCargoXferOps(
            C.cast(string, C.c_void_p), C.cast(string, C.c_void_p),
            C.cast(color, C.c_void_p), C.cast(color, C.c_void_p),
            C.cast(color, C.c_void_p), C.cast(frame, C.c_void_p),
            C.cast(centered, C.c_void_p), C.cast(right, C.c_void_p),
            C.cast(gauge, C.c_void_p), C.cast(mineral, C.c_void_p))
        self.lib.stars_draw_fleet_cargo_xfer_side(
            4, C.byref(area), C.byref(fleet), 8, 3, -1, 8, 0, 0,
            C.byref(ops), None)
        self.assertEqual([event[1] for event in events if event[0] == 'gauge'],
                         [4, 5, 0, 1, 2, 3])
        events[:] = []
        self.lib.stars_draw_fleet_cargo_xfer_side(
            4, C.byref(area), C.byref(fleet), 8, 9, -1, 8, 0, 0,
            C.byref(ops), None)
        self.assertEqual([event for event in events if event[0] == 'mineral'],
                         [('mineral', 4, 0), ('mineral', 0, 0),
                          ('mineral', 1, 0), ('mineral', 2, 0),
                          ('mineral', 3, 0)])

    def test_draw_fleet_ships_xfer_side_maps_hulls_and_stops_after_selected_row(self):
        area = Rect(0, 0, 100, 80)
        fleet = CargoState()
        fleet.ship_counts[2] = 7
        fleet.ship_counts[3] = 9
        hull_map = (C.c_uint16 * 2)(2, 3)
        events = []
        string_type = C.CFUNCTYPE(C.c_void_p, C.c_void_p, C.c_int16)
        color_type = C.CFUNCTYPE(None, C.c_void_p, C.c_uint16, C.c_uint16)
        frame_type = C.CFUNCTYPE(None, C.c_void_p, C.c_uint16,
                                 C.POINTER(Rect), C.c_uint16)
        centered_type = C.CFUNCTYPE(None, C.c_void_p, C.c_uint16,
                                    C.POINTER(Rect), C.c_char_p, C.c_int16)
        right_type = C.CFUNCTYPE(None, C.c_void_p, C.c_uint16, C.c_int16,
                                 C.c_int16, C.c_char_p, C.c_int16, C.c_int16)

        @string_type
        def string(_context, identifier):
            return C.addressof(string_buffers[identifier])

        string_buffers = {i: C.create_string_buffer(('s%d' % i).encode('ascii'))
                          for i in range(0x0, 0x1b2)}

        @color_type
        def color(_context, _hdc, _value): pass

        @frame_type
        def frame(_context, _hdc, _rect, selected): events.append(('frame', selected))

        @centered_type
        def centered(_context, _hdc, _rect, _text, _state): pass

        @right_type
        def right(_context, _hdc, _x, _y, text, _width, _erase):
            events.append(('right', text))

        ops = DrawFleetShipsXferOps(
            C.cast(string, C.c_void_p), C.cast(string, C.c_void_p),
            C.cast(color, C.c_void_p), C.cast(color, C.c_void_p),
            C.cast(color, C.c_void_p), C.cast(frame, C.c_void_p),
            C.cast(centered, C.c_void_p), C.cast(right, C.c_void_p),
            C.cast(string, C.c_void_p))
        self.lib.stars_draw_fleet_ships_xfer_side(
            4, C.byref(area), C.byref(fleet), 8, hull_map, 2, -1, 8,
            0, 0, C.byref(ops), None)
        self.assertEqual([event[0] for event in events],
                         ['frame', 'frame', 'right', 'frame', 'right', 'right',
                          'frame', 'right'])
        events[:] = []
        self.lib.stars_draw_fleet_ships_xfer_side(
            4, C.byref(area), C.byref(fleet), 8, hull_map, 2, 1, 8,
            0, 0, C.byref(ops), None)
        self.assertEqual(events, [('frame', 1), ('right', b's9')])

    def test_draw_planet_xfer_side_handles_unknown_title_and_skips_fuel_row(self):
        area = Rect(0, 0, 100, 80)
        planet = CargoState()
        planet.minerals[:] = (11, 22, 33, 44, 55)
        events = []
        string_type = C.CFUNCTYPE(C.c_void_p, C.c_void_p, C.c_int16)
        color_type = C.CFUNCTYPE(None, C.c_void_p, C.c_uint16, C.c_uint16)
        frame_type = C.CFUNCTYPE(None, C.c_void_p, C.c_uint16,
                                 C.POINTER(Rect), C.c_uint16)
        centered_type = C.CFUNCTYPE(None, C.c_void_p, C.c_uint16,
                                    C.POINTER(Rect), C.c_char_p, C.c_int16)
        right_type = C.CFUNCTYPE(None, C.c_void_p, C.c_uint16, C.c_int16,
                                 C.c_int16, C.c_char_p, C.c_int16, C.c_int16)
        value_type = C.CFUNCTYPE(None, C.c_void_p, C.c_uint16,
                                 C.POINTER(Rect), C.c_int32, C.c_int16)
        strings = {i: C.create_string_buffer(('s%d' % i).encode('ascii'))
                   for i in range(0x360, 0x1b2)}
        strings.update({i: C.create_string_buffer(('s%d' % i).encode('ascii'))
                        for i in range(0x1ae, 0x1b2)})

        @string_type
        def string(_context, identifier):
            if identifier not in strings:
                strings[identifier] = C.create_string_buffer(
                    ('s%d' % identifier).encode('ascii'))
            return C.addressof(strings[identifier])

        @color_type
        def color(_context, _hdc, _value): pass

        @frame_type
        def frame(_context, _hdc, _rect, selected): events.append(('frame', selected))

        @centered_type
        def centered(_context, _hdc, _rect, text, _state): events.append(('title', text))

        @right_type
        def right(_context, _hdc, _x, _y, _text, _width, _erase): events.append(('label', 1))

        @value_type
        def value(_context, _hdc, _rect, amount, selected):
            events.append(('value', amount, selected))

        ops = DrawPlanetXferOps(C.cast(string, C.c_void_p),
                                C.cast(string, C.c_void_p),
                                C.cast(string, C.c_void_p),
                                C.cast(string, C.c_void_p),
                                C.cast(color, C.c_void_p), C.cast(color, C.c_void_p),
                                C.cast(color, C.c_void_p), C.cast(frame, C.c_void_p),
                                C.cast(centered, C.c_void_p), C.cast(right, C.c_void_p),
                                C.cast(value, C.c_void_p))
        self.lib.stars_draw_planet_xfer_side(
            4, C.byref(area), C.byref(planet), -1, -1, 8, 0, 0, 20,
            C.byref(ops), None)
        self.assertIn(('title', b's866'), events)
        self.assertEqual([event[1] for event in events if event[0] == 'value'],
                         [11, 22, 33, 44])
        events[:] = []
        self.lib.stars_draw_planet_xfer_side(
            4, C.byref(area), C.byref(planet), 12, 2, 8, 0, 0, 20,
            C.byref(ops), None)
        self.assertEqual([event for event in events if event[0] == 'value'],
                         [('value', 33, 1)])

    def test_get_xfer_left_right_rects_splits_and_insets_both_halves(self):
        whole = Rect(0, 0, 100, 60)
        left = Rect()
        right = Rect()
        self.lib.stars_get_xfer_left_right_rects(
            C.byref(whole), C.byref(left), C.byref(right), 8)
        self.assertEqual((left.left, left.top, left.right, left.bottom),
                         (6, 4, 35, 56))
        self.assertEqual((right.left, right.top, right.right, right.bottom),
                         (65, 4, 94, 56))

    def test_popup_hull_type_handles_special_limits_and_visibility_mask(self):
        hull = (C.c_ubyte * 128)()
        masks = (C.c_uint16 * 16)()
        masks[3] = 0x00a0
        struct.pack_into('<H', hull, 0x7b, 3 << 10)
        self.assertEqual(self.lib.stars_is_popup_hull_type(
            0, hull, 0, masks), 1)
        self.assertEqual(self.lib.stars_is_popup_hull_type(
            0xff, hull, 0, masks), 1)
        self.assertEqual(self.lib.stars_is_popup_hull_type(
            1, hull, 0x00e0, masks), 0x00a0)
        self.assertEqual(self.lib.stars_is_popup_hull_type(
            1, hull, 0x0001, masks), 0)

    def test_handle_focus_state_frames_only_the_selected_item(self):
        rect = (C.c_uint16 * 4)(1, 2, 30, 40)
        events = []
        frame_type = C.CFUNCTYPE(None, C.c_void_p, C.c_uint16,
                                 C.POINTER(C.c_uint16), C.c_uint16)

        @frame_type
        def frame(_context, hdc, item_rect, brush):
            events.append((hdc, list(item_rect[:4]), brush))

        callback = C.cast(frame, C.c_void_p)
        self.lib.stars_handle_focus_state(
            0x10, 7, rect, 50, 99, callback, None)
        self.lib.stars_handle_focus_state(
            0, 7, rect, 50, 99, callback, None)
        self.assertEqual(events, [(7, [1, 2, 30, 40], 50)])

    def test_enable_zip_buttons_uses_valid_byte_for_both_controls(self):
        table = (C.c_ubyte * (24 * 2))()
        table[24 + 23] = 1
        events = []
        get_type = C.CFUNCTYPE(C.c_uint16, C.c_void_p, C.c_uint16,
                               C.c_uint16)
        enable_type = C.CFUNCTYPE(None, C.c_void_p, C.c_uint16, C.c_uint16)

        @get_type
        def get_item(_context, hwnd, control_id):
            events.append(('get', hwnd, control_id))
            return control_id + 1

        @enable_type
        def enable(_context, control, valid):
            events.append(('enable', control, valid))

        self.lib.stars_enable_zip_buttons(
            0x1234, 1, table, C.cast(get_item, C.c_void_p),
            C.cast(enable, C.c_void_p), None)
        self.assertEqual(events, [
            ('get', 0x1234, 0x817), ('enable', 0x818, 1),
            ('get', 0x1234, 0x41b), ('enable', 0x41c, 1)])

    def test_random_events_preserves_four_handler_order(self):
        events = []
        event_type = C.CFUNCTYPE(None, C.c_void_p)
        callbacks = []
        for name in ('meteor', 'climate', 'minerals', 'trader'):
            @event_type
            def event(_context, name=name): events.append(name)
            callbacks.append(event)
        self.lib.stars_random_events(
            *(C.cast(callback, C.c_void_p) for callback in callbacks), None)
        self.assertEqual(events, ['meteor', 'climate', 'minerals', 'trader'])

    def test_format_serial_and_env_restores_rng_and_emits_28_character_alphabet(self):
        state = self.Seeds(0x12345678, 0x23456789)
        before = (state.first, state.second)
        environment = (C.c_ubyte * 11)(*range(11))
        plan_order = (C.c_ubyte * 21)(*range(21))
        output = (C.c_ubyte * 29)()
        self.lib.stars_format_serial_and_env(
            C.byref(state), 0x10203040, environment, plan_order, output)
        text = bytes(output).split(b'\0', 1)[0]
        self.assertEqual(len(text), 28)
        self.assertTrue(all(ch in b'ABCDEFGHIJKLMNOPQRSTUVWXYZ'
                            b'abcdefghijklmnopqrstuvwxyz0123456789-*'
                            for ch in text))
        self.assertEqual((state.first, state.second), before)

    def test_checked_serial_formatter_rejects_unsafe_ctypes_buffers(self):
        state = self.Seeds(0x12345678, 0x23456789)
        before = (state.first, state.second)
        environment = (C.c_ubyte * 11)(*range(11))
        plan_order = (C.c_ubyte * 21)(*range(21))
        output = (C.c_ubyte * 29)(*([0x5a] * 29))
        self.assertEqual(self.lib.stars_format_serial_and_env_checked(
            C.byref(state), 0x10203040, environment, plan_order, output, 29), 1)
        self.assertEqual(len(bytes(output).split(b'\0', 1)[0]), 28)
        state.first, state.second = before
        output[:] = b'Z' * 29
        self.assertEqual(self.lib.stars_format_serial_and_env_checked(
            C.byref(state), 0x10203040, environment, plan_order, output, 28), 0)
        self.assertEqual(bytes(output), b'Z' * 29)
        self.assertEqual((state.first, state.second), before)
        plan_order[0] = 21
        self.assertEqual(self.lib.stars_format_serial_and_env_checked(
            C.byref(state), 0x10203040, environment, plan_order, output, 29), 0)
        self.assertEqual(bytes(output), b'Z' * 29)

    def test_serial_and_env_parser_round_trips_formatter_and_rejects_tampering(self):
        state = self.Seeds(0x12345678, 0x23456789)
        environment = (C.c_ubyte * 11)(*range(11))
        plan_order = (C.c_ubyte * 21)(*range(21))
        formatted = (C.c_ubyte * 29)()
        self.lib.stars_format_serial_and_env(
            C.byref(state), 0x10203040, environment, plan_order, formatted)
        serial = C.c_uint32()
        decoded_env = (C.c_ubyte * 11)()
        valid_type = C.CFUNCTYPE(C.c_int16, C.c_void_p, C.c_uint32)

        @valid_type
        def valid(_context, value): return 1 if value == 0x10203040 else 0

        self.assertEqual(self.lib.stars_serial_and_env_from_string(
            C.byref(state), formatted, C.byref(serial), decoded_env,
            plan_order, C.cast(valid, C.c_void_p), None), 1)
        self.assertEqual(serial.value, 0x10203040)
        self.assertEqual(bytes(decoded_env), bytes(environment))
        formatted[0] = ord('*') if formatted[0] != ord('*') else ord('A')
        self.assertEqual(self.lib.stars_serial_and_env_from_string(
            C.byref(state), formatted, C.byref(serial), decoded_env,
            plan_order, C.cast(valid, C.c_void_p), None), 0)

    def test_enum_our_starbase_requires_owner_and_starbase_flag(self):
        planet = (C.c_ubyte * 8)()
        struct.pack_into('<HH', planet, 2, 4, 0x0200)
        self.assertEqual(self.lib.stars_enum_our_starbase(planet, 4), 1)
        self.assertEqual(self.lib.stars_enum_our_starbase(planet, 5), 0)
        struct.pack_into('<H', planet, 4, 0)
        self.assertEqual(self.lib.stars_enum_our_starbase(planet, 4), 0)

    def test_enemy_fleet_and_planet_predicates_filter_owner_and_unowned(self):
        record = (C.c_ubyte * 8)()
        struct.pack_into('<H', record, 2, 3)
        self.assertEqual(self.lib.stars_enum_calc_enemy_fleet(record, 2), 1)
        self.assertEqual(self.lib.stars_enum_calc_enemy_fleet(record, 3), 0)
        self.assertEqual(self.lib.stars_enum_calc_enemy_planet(record, 2), 1)
        self.assertEqual(self.lib.stars_enum_calc_enemy_planet(record, 3), 0)
        struct.pack_into('<H', record, 2, 0xffff)
        self.assertEqual(self.lib.stars_enum_calc_enemy_planet(record, 2), 0)

    def test_empty_bmp_from_grhst_returns_first_of_21_table_matches(self):
        data = (C.c_ubyte * 65536)()
        struct.pack_into('<H', data, 0x0c52 + 2 * 2, 0x1234)
        struct.pack_into('<H', data, 0x0c52 + 14 * 2, 0x1234)
        self.assertEqual(self.lib.stars_empty_bmp_from_grhst(
            data, 0x1234), 2)
        self.assertEqual(self.lib.stars_empty_bmp_from_grhst(
            data, 0x4321), 0)

    def test_check_research_matches_signed_technology_nibbles(self):
        data = (C.c_ubyte * 65536)()
        data[0x59db] = 0x23
        data[0x59da] = 7
        self.assertEqual(self.lib.stars_check_research(data, 3, 2, 7), 1)
        self.assertEqual(bytes(data[0x5212:0x5214]), b'\x00\x00')

        data[0x59db] = 0xf3
        self.assertEqual(self.lib.stars_check_research(data, 3, -1, 7), 1)

    def test_check_research_sets_tutor_error_on_mismatch(self):
        data = (C.c_ubyte * 65536)()
        data[0x59db] = 0x23
        data[0x59da] = 7
        self.assertEqual(self.lib.stars_check_research(data, 4, 2, 7), 0)
        self.assertEqual(bytes(data[0x5212:0x5214]), b'\x2e\x04')

    def test_clear_ai_current_task_clears_order_flags_and_refreshes_selection(self):
        selection = (C.c_ubyte * 32)()
        fleet = (C.c_ubyte * 8)()
        struct.pack_into('<H', selection, 0x0a, 0xabcd)
        struct.pack_into('<H', fleet, 0, 0x1234)
        events = []
        change_type = C.CFUNCTYPE(None, C.c_void_p, C.c_int16,
                                  C.c_uint16)
        lookup_type = C.CFUNCTYPE(None, C.c_void_p, C.c_int16, PTR)
        @change_type
        def change(_context, mode, overlap): events.append(('change', mode, overlap))
        @lookup_type
        def lookup(_context, fleet_id, record):
            events.append(('lookup', fleet_id, bytes(record[0x0a:0x0c])))
        self.lib.stars_clear_ai_current_task(
            selection, fleet, 1, C.cast(change, C.c_void_p),
            C.cast(lookup, C.c_void_p), None)
        self.assertEqual(bytes(selection[0x0a:0x0c]), b'\xc0\xab')
        self.assertEqual(events, [('change', 2, 0x1234),
                                  ('lookup', -1, b'\xc0\xab')])

    def test_clear_ai_current_task_skips_change_callback_when_not_requested(self):
        selection = (C.c_ubyte * 32)()
        fleet = (C.c_ubyte * 8)()
        struct.pack_into('<H', selection, 0x0a, 0x000f)
        calls = []
        lookup_type = C.CFUNCTYPE(None, C.c_void_p, C.c_int16, PTR)
        @lookup_type
        def lookup(_context, fleet_id, _record): calls.append(fleet_id)
        self.lib.stars_clear_ai_current_task(
            selection, fleet, 0, None, C.cast(lookup, C.c_void_p), None)
        self.assertEqual(bytes(selection[0x0a:0x0c]), b'\x00\x00')
        self.assertEqual(calls, [-1])

    def test_set_ngw_title_gets_formats_and_publishes_the_title(self):
        data = (C.c_ubyte * 65536)()
        struct.pack_into('<H', data, 0x5320, 0x0200)
        events = []
        get_type = C.CFUNCTYPE(C.c_int16, C.c_void_p, C.c_uint16, PTR)
        format_type = C.CFUNCTYPE(C.c_int16, C.c_void_p, PTR, PTR,
                                  C.c_int16)
        set_type = C.CFUNCTYPE(None, C.c_void_p, C.c_uint16, PTR)
        @get_type
        def get_string(_context, string_id, output):
            events.append(('get', string_id))
            C.memmove(output, b'Step %d', 8)
            return 7
        @format_type
        def format_title(_context, output, fmt, step):
            events.append(('format', C.string_at(fmt), step))
            text = ('Step %d' % step).encode('ascii') + b'\x00'
            C.memmove(output, text, len(text))
            return len(text) - 1
        @set_type
        def set_title(_context, hwnd, text):
            events.append(('set', hwnd, C.string_at(text)))
        self.lib.stars_set_ngw_title(
            data, 9, 4, C.cast(get_string, C.c_void_p),
            C.cast(format_title, C.c_void_p), C.cast(set_title, C.c_void_p),
            None)
        self.assertEqual(events, [('get', 0x0310), ('format', b'Step %d', 4),
                                  ('set', 9, b'Step 4')])
        self.assertEqual(bytes(data[0x57a4:0x57ab]), b'Step 4\x00')

    def test_check_fleet_wp_validates_order_task_object_and_warp(self):
        data = (C.c_ubyte * 65536)()
        struct.pack_into('<H', data, 0x5212, 0x1111)
        fleet = (C.c_ubyte * 124)()
        orders = (C.c_ubyte * (4 + 18))()
        struct.pack_into('<H', fleet, 0x62, 1)
        struct.pack_into('<H', orders, 4 + 4, 42)
        struct.pack_into('<H', orders, 4 + 6, (3 << 8) | (5 << 4) | 2)
        errors = []
        selections = []
        error_type = C.CFUNCTYPE(None, C.c_void_p, C.c_int16)
        selection_type = C.CFUNCTYPE(C.c_int16, C.c_void_p, C.c_int16,
                                     C.c_uint16)
        @error_type
        def error(_context, code): errors.append(code)
        @selection_type
        def selection(_context, mode, ifl):
            selections.append((mode, ifl))
            return 0
        self.assertEqual(self.lib.stars_check_fleet_wp(
            data, fleet, orders, 7, 0, 3, 42, 2, 5,
            C.cast(error, C.c_void_p), C.cast(selection, C.c_void_p), None), 1)
        self.assertEqual(errors, [])
        self.assertEqual(selections, [])
        self.assertEqual(struct.unpack_from('<H', data, 0x5212)[0], 0x1111)

    def test_check_fleet_wp_reports_object_mismatch_and_restores_tutor_id(self):
        data = (C.c_ubyte * 65536)()
        struct.pack_into('<H', data, 0x5212, 0x2222)
        fleet = (C.c_ubyte * 124)()
        orders = (C.c_ubyte * 40)()
        struct.pack_into('<H', fleet, 0x62, 1)
        struct.pack_into('<H', orders, 8, 99)
        struct.pack_into('<H', orders, 10, (3 << 8) | 2)
        errors = []
        error_type = C.CFUNCTYPE(None, C.c_void_p, C.c_int16)
        @error_type
        def error(_context, code): errors.append(code)
        self.assertEqual(self.lib.stars_check_fleet_wp(
            data, fleet, orders, 7, 0, 4, 99, 2, 3,
            C.cast(error, C.c_void_p), None, None), 0)
        self.assertEqual(errors, [0x1ec])
        self.assertEqual(struct.unpack_from('<H', data, 0x5212)[0], 0x2222)

    def test_check_planet_route_matches_low_ten_bits_and_restores_tutor_id(self):
        data = (C.c_ubyte * 65536)()
        planet = (C.c_ubyte * 64)()
        struct.pack_into('<H', data, 0x5212, 0x3333)
        struct.pack_into('<H', planet, 0x30, 0xfc02)
        self.assertEqual(self.lib.stars_check_planet_route(data, planet, 1), 1)
        self.assertEqual(struct.unpack_from('<H', data, 0x5212)[0], 0x3333)

        self.assertEqual(self.lib.stars_check_planet_route(data, planet, 2), 0)
        self.assertEqual(struct.unpack_from('<H', data, 0x5212)[0], 0x05fb)

    def test_check_laying_wp_composes_fleet_check_and_years_match(self):
        data = (C.c_ubyte * 65536)()
        fleet = (C.c_ubyte * 124)()
        orders = (C.c_ubyte * 40)()
        struct.pack_into('<H', data, 0x5212, 0x4444)
        struct.pack_into('<H', fleet, 0x62, 1)
        struct.pack_into('<H', orders, 8, 17)
        struct.pack_into('<H', orders, 10, (1 << 8) | (4 << 4) | 6)
        struct.pack_into('<H', orders, 12, 9)
        self.assertEqual(self.lib.stars_check_laying_wp(
            data, fleet, orders, 2, 0, 17, 9, None, None, None), 1)
        self.assertEqual(struct.unpack_from('<H', data, 0x5212)[0], 0x4444)
        self.assertEqual(self.lib.stars_check_laying_wp(
            data, fleet, orders, 2, 0, 17, 10, None, None, None), 0)

    def test_check_laying_wp_uses_second_object_group_for_high_bit_ids(self):
        data = (C.c_ubyte * 65536)()
        fleet = (C.c_ubyte * 124)()
        orders = (C.c_ubyte * 22)()
        struct.pack_into('<H', fleet, 0x62, 1)
        struct.pack_into('<H', orders, 8, 17)
        struct.pack_into('<H', orders, 10, (2 << 8) | 6)
        struct.pack_into('<H', orders, 12, 9)
        self.assertEqual(self.lib.stars_check_laying_wp(
            data, fleet, orders, 2, 0, (C.c_int16(0x8000 | 17).value),
            9, None, None, None), 1)

    def test_check_fleet_name_handles_wildcard_and_case_insensitive_match(self):
        data = (C.c_ubyte * 65536)()
        name = C.create_string_buffer(b'Alpha Fleet')
        struct.pack_into('<H', data, 0x5212, 0x5555)
        get_type = C.CFUNCTYPE(C.c_int16, C.c_void_p, C.c_uint16, PTR)
        compare_type = C.CFUNCTYPE(C.c_int16, C.c_void_p, PTR, PTR)
        @get_type
        def get_string(_context, string_id, output):
            self.assertEqual(string_id, 9)
            C.memmove(output, b'ALPHA FLEET\x00', 12)
            return 11
        @compare_type
        def compare(_context, left, right):
            return (C.string_at(left).lower() != C.string_at(right).lower())
        self.assertEqual(self.lib.stars_check_fleet_name(
            data, C.cast(name, PTR), 9, C.cast(get_string, C.c_void_p),
            C.cast(compare, C.c_void_p), None, None), 1)
        self.assertEqual(struct.unpack_from('<H', data, 0x5212)[0], 0x5555)
        self.assertEqual(self.lib.stars_check_fleet_name(
            data, C.cast(name, PTR), -1, None, None, None, None), 1)

    def test_check_fleet_name_reports_mismatch_and_unresolved_sentinels(self):
        data = (C.c_ubyte * 65536)()
        name = C.create_string_buffer(b'Alpha')
        errors = []
        error_type = C.CFUNCTYPE(None, C.c_void_p, C.c_int16)
        get_type = C.CFUNCTYPE(C.c_int16, C.c_void_p, C.c_uint16, PTR)
        compare_type = C.CFUNCTYPE(C.c_int16, C.c_void_p, PTR, PTR)
        @error_type
        def error(_context, code): errors.append(code)
        @get_type
        def get_string(_context, _string_id, output):
            C.memmove(output, b'Beta\x00', 5)
            return 4
        @compare_type
        def compare(_context, _left, _right): return 1
        self.assertEqual(self.lib.stars_check_fleet_name(
            data, C.cast(name, PTR), 9, C.cast(get_string, C.c_void_p),
            C.cast(compare, C.c_void_p),
            C.cast(error, C.c_void_p), None), 0)
        self.assertEqual(errors, [0x4cf])
        self.assertEqual(struct.unpack_from('<H', data, 0x5212)[0], 0x0bee)
        self.assertEqual(self.lib.stars_check_fleet_name(
            data, None, 9, None, None, None, None), 1)

    def test_check_scanner_applies_nibble_and_zoom_constraints(self):
        data = (C.c_ubyte * 65536)()
        struct.pack_into('<H', data, 0x0588, 0x0013)
        struct.pack_into('<H', data, 0x05a0, 4)
        self.assertEqual(self.lib.stars_check_scanner(data, 3, 4), 1)
        self.assertEqual(self.lib.stars_check_scanner(data, 2, 4), 0)
        self.assertEqual(struct.unpack_from('<H', data, 0x5212)[0], 0x36b8)
        self.assertEqual(self.lib.stars_check_scanner(data, 3, 5), 0)
        self.assertEqual(struct.unpack_from('<H', data, 0x5212)[0], 0x36c6)

    def test_check_scanner_uses_subset_matching_and_tutor_bypass(self):
        data = (C.c_ubyte * 65536)()
        struct.pack_into('<H', data, 0x0588, 0x0035)
        struct.pack_into('<H', data, 0x05a0, 7)
        self.assertEqual(self.lib.stars_check_scanner(data, 0x15, -1), 1)
        data[0x520c] = 0
        data[0x520d] = 2
        self.assertEqual(self.lib.stars_check_scanner(data, 0x7f, 99), 1)

    def test_tutor_error_formats_and_alerts_new_errors_and_resets_progress(self):
        data = (C.c_ubyte * 65536)()
        events = []
        format_type = C.CFUNCTYPE(C.c_void_p, C.c_void_p, C.c_int16)
        alert_type = C.CFUNCTYPE(None, C.c_void_p, PTR, C.c_int16)
        text = C.create_string_buffer(b'error text')
        @format_type
        def format_ids(_context, error_id):
            events.append(('format', error_id))
            return C.addressof(text)
        @alert_type
        def alert(_context, message, message_type):
            events.append(('alert', C.string_at(message), message_type))
        self.lib.stars_tutor_error(
            data, 0x4cf, C.cast(format_ids, C.c_void_p),
            C.cast(alert, C.c_void_p), None)
        self.assertEqual(events, [('format', 0x4cf),
                                  ('alert', b'error text', 0x10)])
        self.assertEqual(struct.unpack_from('<H', data, 0x5214)[0], 0x4cf)

    def test_tutor_error_suppresses_repeated_errors_until_progress_limit(self):
        data = (C.c_ubyte * 65536)()
        events = []
        format_type = C.CFUNCTYPE(C.c_void_p, C.c_void_p, C.c_int16)
        alert_type = C.CFUNCTYPE(None, C.c_void_p, PTR, C.c_int16)
        @format_type
        def format_ids(_context, _error_id): events.append('format'); return 0
        @alert_type
        def alert(_context, _message, _message_type): events.append('alert')
        for _ in range(3):
            self.lib.stars_tutor_error(
                data, 7, C.cast(format_ids, C.c_void_p),
                C.cast(alert, C.c_void_p), None)
        self.assertEqual(events, ['format', 'alert'])
        self.assertEqual((struct.unpack_from('<H', data, 0x520c)[0] & 0x01c0), 0x80)
        for _ in range(2):
            self.lib.stars_tutor_error(
                data, 7, C.cast(format_ids, C.c_void_p),
                C.cast(alert, C.c_void_p), None)
        self.assertEqual(events, ['format', 'alert', 'format', 'alert'])
        self.assertEqual((struct.unpack_from('<H', data, 0x520c)[0] & 0x01c0), 0)
        data[0x520c] |= 0x20
        self.lib.stars_tutor_error(data, 8, None, None, None)
        self.assertEqual(struct.unpack_from('<H', data, 0x5214)[0], 0xffff)

    def test_check_template_installs_template_in_tutor_mode(self):
        data = (C.c_ubyte * 65536)()
        templates = (C.c_ubyte * 52)()
        for i, value in enumerate(range(26)):
            templates[26 + i] = value
        data[0x520d] = 2
        self.assertEqual(self.lib.stars_check_template(data, templates, 1), 1)
        self.assertEqual(data[0x2303], 1)
        self.assertEqual(bytes(data[0x2304:0x231e]), bytes(range(26)))
        self.assertEqual(struct.unpack_from('<H', data, 0x07ce)[0] & 0x20, 0x20)

    def test_check_template_validates_existing_header_and_queue_words(self):
        data = (C.c_ubyte * 65536)()
        templates = (C.c_ubyte * 26)(
            0, 2, 0x34, 0x12, 0x78, 0x56, 0xbc, 0x9a,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0)
        data[0x2303] = 1
        data[0x2304] = 0
        data[0x2305] = 2
        struct.pack_into('<HH', data, 0x2306, 0x1234, 0x5678)
        self.assertEqual(self.lib.stars_check_template(data, templates, 0), 1)
        self.assertEqual(struct.unpack_from('<H', data, 0x07ce)[0] & 0x20, 0x20)
        data[0x2307] ^= 1
        self.assertEqual(self.lib.stars_check_template(data, templates, 0), 0)

    def test_check_summary_matches_selected_planet_fleet_and_thing_records(self):
        data = (C.c_ubyte * 65536)()
        fleet = (C.c_ubyte * 124)()
        thing = (C.c_ubyte * 18)()
        struct.pack_into('<H', data, 0x4966, 1)
        struct.pack_into('<H', data, 0x496a, 12)
        self.assertEqual(self.lib.stars_check_summary(data, 1, 12, None, None), 1)
        struct.pack_into('<H', data, 0x4966, 2)
        struct.pack_into('<h', data, 0x496c, 3)
        struct.pack_into('<H', fleet, 0, 44)
        self.assertEqual(self.lib.stars_check_summary(data, 2, 44, fleet, None), 1)
        struct.pack_into('<H', data, 0x4966, 8)
        struct.pack_into('<H', data, 0x4970, 2)
        struct.pack_into('<H', thing, 0, 55)
        self.assertEqual(self.lib.stars_check_summary(data, 8, 55, None, thing), 1)

    def test_check_summary_sets_tutor_error_on_selection_mismatch_and_bypasses_in_summary_mode(self):
        data = (C.c_ubyte * 65536)()
        struct.pack_into('<H', data, 0x4966, 1)
        struct.pack_into('<H', data, 0x496a, 12)
        self.assertEqual(self.lib.stars_check_summary(data, 1, 13, None, None), 0)
        self.assertEqual(struct.unpack_from('<H', data, 0x5212)[0], 0x36d2)
        struct.pack_into('<H', data, 0x07ca, 2)
        self.assertEqual(self.lib.stars_check_summary(data, 99, 99, None, None), 1)

    def test_check_messages_accepts_current_message_and_restores_tutor_state(self):
        data = (C.c_ubyte * 65536)()
        bits = (C.c_ubyte * 4)(0xff, 0, 0, 0)
        struct.pack_into('<H', data, 0x0b00, 5)
        struct.pack_into('<H', data, 0x5212, 0x6666)
        lookup_type = C.CFUNCTYPE(C.c_int16, C.c_void_p, C.c_int16)
        @lookup_type
        def lookup(_context, message):
            self.assertEqual(message, 5)
            return 12
        self.assertEqual(self.lib.stars_check_messages(
            data, 5, 12, 0, bits, None, C.cast(lookup, C.c_void_p),
            None, None), 1)
        self.assertEqual(struct.unpack_from('<H', data, 0x5212)[0], 0x6666)

    def test_check_messages_handles_filter_enable_and_reject_paths(self):
        data = (C.c_ubyte * 65536)()
        bits = (C.c_ubyte * 4)(0, 0, 0, 0)
        struct.pack_into('<H', data, 0x0b00, 5)
        data[0x520d] = 2
        events = []
        set_type = C.CFUNCTYPE(None, C.c_void_p, C.c_int16, C.c_int16)
        @set_type
        def set_groups(_context, message_id, enabled):
            events.append((message_id, enabled))
        self.assertEqual(self.lib.stars_check_messages(
            data, 5, 12, 1, bits, None, None,
            C.cast(set_groups, C.c_void_p), None), 1)
        self.assertEqual(events, [(12, 1)])
        data[0x520d] = 0
        self.assertEqual(self.lib.stars_check_messages(
            data, 5, 12, 1, bits, None, None, None, None), 0)

    def test_check_messages_honors_end_message_and_summary_bypass(self):
        data = (C.c_ubyte * 65536)()
        bits = (C.c_ubyte * 4)(0xff, 0, 0, 0)
        struct.pack_into('<H', data, 0x0b00, 5)
        next_type = C.CFUNCTYPE(C.c_int16, C.c_void_p, C.c_int16)
        @next_type
        def next_message(_context, _start): return 4
        self.assertEqual(self.lib.stars_check_messages(
            data, 9999, -1, 0, bits, C.cast(next_message, C.c_void_p),
            None, None, None), 0)
        struct.pack_into('<H', data, 0x07ca, 2)
        self.assertEqual(self.lib.stars_check_messages(
            data, 9999, 12, 1, bits, None, None, None, None), 1)

    def test_check_selection_matches_selected_planet_and_fleet(self):
        data = (C.c_ubyte * 65536)()
        fleet = (C.c_ubyte * 124)()
        struct.pack_into('<H', data, 0x495a, 1)
        struct.pack_into('<H', data, 0x49ee, 12)
        self.assertEqual(self.lib.stars_check_selection(data, 1, 12, None), 1)
        struct.pack_into('<H', data, 0x495a, 2)
        struct.pack_into('<H', data, 0x4972, 44)
        self.assertEqual(self.lib.stars_check_selection(data, 2, 44, None), 1)
        struct.pack_into('<H', data, 0x5212, 0x7777)
        self.assertEqual(struct.unpack_from('<H', data, 0x5212)[0], 0x7777)

    def test_check_selection_handles_cross_selection_and_bypass(self):
        data = (C.c_ubyte * 65536)()
        fleet = (C.c_ubyte * 124)()
        struct.pack_into('<H', data, 0x495a, 2)
        struct.pack_into('<H', data, 0x4978, 19)
        self.assertEqual(self.lib.stars_check_selection(data, 1, 19, None), 0)
        struct.pack_into('<H', data, 0x495a, 1)
        struct.pack_into('<H', data, 0x4972, 19)
        struct.pack_into('<H', fleet, 6, 22)
        struct.pack_into('<H', data, 0x49ee, 0)
        self.assertEqual(self.lib.stars_check_selection(data, 2, 19, fleet), 0)
        struct.pack_into('<H', data, 0x520d, 2)
        self.assertEqual(self.lib.stars_check_selection(data, 99, 99, None), 1)

    def test_check_zip_validates_actions_and_localized_name(self):
        data = (C.c_ubyte * 65536)()
        goal = (C.c_ubyte * 10)()
        record = 0x5264
        data[record + 23] = 1
        for j, value in enumerate(b'Alpha\x00'):
            data[record + 10 + j] = value
        for i in range(5):
            struct.pack_into('<H', goal, i * 2, 0x2000)
            struct.pack_into('<H', data, record + i * 2, 0x2001)
        get_type = C.CFUNCTYPE(C.c_int16, C.c_void_p, C.c_uint16, PTR)
        compare_type = C.CFUNCTYPE(C.c_int16, C.c_void_p, PTR, PTR)
        @get_type
        def get_string(_context, _string_id, output):
            C.memmove(output, b'alpha\x00', 6)
            return 5
        @compare_type
        def compare(_context, left, right):
            return int(C.string_at(left).lower() != C.string_at(right).lower())
        self.assertEqual(self.lib.stars_check_zip(
            data, 0, goal, 5, C.cast(get_string, C.c_void_p),
            C.cast(compare, C.c_void_p), None, None), 1)

    def test_check_zip_tutor_mode_installs_goal_actions_and_name(self):
        data = (C.c_ubyte * 65536)()
        goal = (C.c_ubyte * 10)()
        data[0x520d] = 2
        for i in range(5):
            struct.pack_into('<H', goal, i * 2, 0x3000)
            struct.pack_into('<H', data, 0x5264 + i * 2, 0x0123)
        get_type = C.CFUNCTYPE(C.c_int16, C.c_void_p, C.c_uint16, PTR)
        @get_type
        def get_string(_context, _string_id, output):
            C.memmove(output, b'Beta\x00', 5)
            return 4
        self.assertEqual(self.lib.stars_check_zip(
            data, 0, goal, 6, C.cast(get_string, C.c_void_p),
            None, None, None), 1)
        self.assertEqual(data[0x5264], 0x23)
        self.assertEqual(data[0x5265] & 0xf0, 0x30)
        self.assertEqual(bytes(data[0x526e:0x5273]), b'Beta\x00')

    def test_check_cargo_matches_four_signed_long_requirements(self):
        data = (C.c_ubyte * 65536)()
        fleet = (C.c_ubyte * 124)()
        for offset, value in ((76, -2), (80, 3), (84, 0), (88, 7)):
            struct.pack_into('<i', fleet, offset, value)
        self.assertEqual(self.lib.stars_check_cargo(
            data, fleet, -2, 3, 0, 7, None, None, None), 1)
        self.assertEqual(struct.unpack_from('<H', data, 0x5212)[0], 0)

    def test_check_cargo_reports_nonzero_zero_requirement_and_selection_fallback(self):
        data = (C.c_ubyte * 65536)()
        fleet = (C.c_ubyte * 124)()
        struct.pack_into('<H', fleet, 0, 17)
        struct.pack_into('<i', fleet, 76, 4)
        errors = []
        selections = []
        error_type = C.CFUNCTYPE(None, C.c_void_p, C.c_int16)
        selection_type = C.CFUNCTYPE(C.c_int16, C.c_void_p, C.c_int16,
                                     C.c_uint16)
        @error_type
        def error(_context, code): errors.append(code)
        @selection_type
        def selection(_context, group, fleet_id):
            selections.append((group, fleet_id))
            return 0
        self.assertEqual(self.lib.stars_check_cargo(
            data, fleet, 0, 0, 0, 0, C.cast(error, C.c_void_p),
            C.cast(selection, C.c_void_p), None), 0)
        self.assertEqual(errors, [0x1ef])
        self.assertEqual(selections, [(2, 17)])

    def test_check_ship_builder_queries_control_and_matches_selected_ship(self):
        data = (C.c_ubyte * 65536)()
        struct.pack_into('<H', data, 0x5212, 0x8888)
        events = []
        get_type = C.CFUNCTYPE(C.c_uint16, C.c_void_p, C.c_uint16,
                               C.c_uint16)
        send_type = C.CFUNCTYPE(C.c_int32, C.c_void_p, C.c_uint16,
                                C.c_uint16)
        @get_type
        def get_item(_context, hwnd, control_id):
            events.append(('get', hwnd, control_id))
            return 44
        @send_type
        def send(_context, control, message):
            events.append(('send', control, message))
            return 7
        self.assertEqual(self.lib.stars_check_ship_builder(
            data, 9, 4, -1, 7, C.cast(get_item, C.c_void_p),
            C.cast(send, C.c_void_p), None), 1)
        self.assertEqual(events, [('get', 9, 0x81a), ('send', 44, 0x407)])
        self.assertEqual(struct.unpack_from('<H', data, 0x5212)[0], 0x8888)

    def test_check_ship_builder_rejects_dialog_category_and_ship_mismatches(self):
        data = (C.c_ubyte * 65536)()
        get_type = C.CFUNCTYPE(C.c_uint16, C.c_void_p, C.c_uint16,
                               C.c_uint16)
        send_type = C.CFUNCTYPE(C.c_int32, C.c_void_p, C.c_uint16,
                                C.c_uint16)
        @get_type
        def get_item(_context, _hwnd, _control_id): return 44
        @send_type
        def send(_context, _control, _message): return 7
        self.assertEqual(self.lib.stars_check_ship_builder(
            data, 0, 4, -1, 7, C.cast(get_item, C.c_void_p),
            C.cast(send, C.c_void_p), None), 0)
        self.assertEqual(self.lib.stars_check_ship_builder(
            data, 9, 4, 3, 7, C.cast(get_item, C.c_void_p),
            C.cast(send, C.c_void_p), None), 0)
        self.assertEqual(self.lib.stars_check_ship_builder(
            data, 9, 4, 4, 8, C.cast(get_item, C.c_void_p),
            C.cast(send, C.c_void_p), None), 0)

    def test_check_builder_part_matches_slot_counts_and_fields(self):
        data = (C.c_ubyte * 65536)()
        built = (C.c_ubyte * 4)()
        goal = (C.c_ubyte * 4)()
        struct.pack_into('<HH', built, 0, 0x1234, 0x0207)
        struct.pack_into('<HH', goal, 0, 0x1234, 0x0207)
        struct.pack_into('<H', data, 0x5212, 0x9999)
        self.assertEqual(self.lib.stars_check_builder_part(
            data, 9, 4, 0, built, goal, 2, None, None), 1)
        self.assertEqual(struct.unpack_from('<H', data, 0x5212)[0], 0x9999)

    def test_check_builder_part_reports_count_and_field_errors(self):
        data = (C.c_ubyte * 65536)()
        built = (C.c_ubyte * 4)()
        goal = (C.c_ubyte * 4)()
        errors = []
        error_type = C.CFUNCTYPE(None, C.c_void_p, C.c_int16)
        @error_type
        def error(_context, code): errors.append(code)
        struct.pack_into('<HH', built, 0, 0x1234, 0x0007)
        struct.pack_into('<HH', goal, 0, 0x1234, 0x0307)
        self.assertEqual(self.lib.stars_check_builder_part(
            data, 9, 4, 0, built, goal, 1, C.cast(error, C.c_void_p), None), 0)
        self.assertEqual(errors, [0x1f5])
        struct.pack_into('<H', built, 2, 0x0208)
        struct.pack_into('<H', goal, 2, 0x0207)
        self.assertEqual(self.lib.stars_check_builder_part(
            data, 9, 4, 0, built, goal, 2, C.cast(error, C.c_void_p), None), 0)
        self.assertEqual(errors[-1], 0x1f6)

    def test_check_queue_decodes_packed_production_record_and_research_bit(self):
        data = (C.c_ubyte * 65536)()
        planet = (C.c_ubyte * 64)()
        queue = (C.c_ubyte * 8)()
        packed = (3 << 17) | (12 << 10) | 7
        struct.pack_into('<I', queue, 0, packed)
        struct.pack_into('<I', planet, 0x24, 1 << 23)
        self.assertEqual(self.lib.stars_check_queue(
            data, 3, 19, 0, 3, 12, 7, 1, planet, queue, 1,
            None, None, None), 1)

    def test_check_queue_reports_field_error_and_runs_selection_fallback(self):
        data = (C.c_ubyte * 65536)()
        planet = (C.c_ubyte * 64)()
        queue = (C.c_ubyte * 8)()
        struct.pack_into('<I', queue, 0, (3 << 17) | (12 << 10) | 7)
        errors = []
        selections = []
        error_type = C.CFUNCTYPE(None, C.c_void_p, C.c_int16)
        selection_type = C.CFUNCTYPE(C.c_int16, C.c_void_p, C.c_int16,
                                     C.c_uint16)
        @error_type
        def error(_context, code): errors.append(code)
        @selection_type
        def selection(_context, group, planet_id): selections.append((group, planet_id)); return 0
        self.assertEqual(self.lib.stars_check_queue(
            data, 1, 19, 0, 3, 12, 8, 0, planet, queue, 1,
            C.cast(error, C.c_void_p), C.cast(selection, C.c_void_p), None), 0)
        self.assertEqual(errors, [0x1f0])
        self.assertEqual(selections, [(1, 19)])

    def test_check_xfer_wp_validates_fleet_waypoint_and_five_transfer_actions(self):
        data = (C.c_ubyte * 65536)()
        fleet = (C.c_ubyte * 124)()
        orders = (C.c_ubyte * 22)()
        goal = (C.c_ubyte * 10)()
        struct.pack_into('<H', fleet, 0x62, 1)
        struct.pack_into('<H', orders, 8, 77)
        struct.pack_into('<H', orders, 10, (1 << 8) | (4 << 4) | 1)
        for i in range(5):
            struct.pack_into('<H', orders, 12 + i * 2, 0x4000 | i)
            struct.pack_into('<H', goal, i * 2, 0x4000 | i)
        self.assertEqual(self.lib.stars_check_xfer_wp(
            data, fleet, orders, 4, 0, 77, 4, goal, None, None, None), 1)

    def test_check_xfer_wp_reports_action_group_mismatch(self):
        data = (C.c_ubyte * 65536)()
        fleet = (C.c_ubyte * 124)()
        orders = (C.c_ubyte * 22)()
        goal = (C.c_ubyte * 10)()
        struct.pack_into('<H', fleet, 0x62, 1)
        struct.pack_into('<H', orders, 8, 77)
        struct.pack_into('<H', orders, 10, (1 << 8) | (4 << 4) | 1)
        struct.pack_into('<H', orders, 12, 0x2000)
        struct.pack_into('<H', goal, 0, 0x3000)
        errors = []
        error_type = C.CFUNCTYPE(None, C.c_void_p, C.c_int16)
        @error_type
        def error(_context, code): errors.append(code)
        self.assertEqual(self.lib.stars_check_xfer_wp(
            data, fleet, orders, 4, 0, 77, 4, goal,
            C.cast(error, C.c_void_p), None, None), 0)
        self.assertEqual(errors, [0x268])

    def test_check_colonize_wp_checks_cargo_for_colonizer_planets(self):
        data = (C.c_ubyte * 65536)()
        fleet = (C.c_ubyte * 124)()
        for i in range(16): fleet[12 + i] = 1
        struct.pack_into('<H', fleet, 6, 0x0d)
        events = []
        cargo_type = C.CFUNCTYPE(C.c_int16, C.c_void_p, PTR, C.c_int16)
        @cargo_type
        def cargo(_context, _fleet, weight): events.append(weight); return 1
        self.assertEqual(self.lib.stars_check_colonize_wp(
            data, fleet, None, 4, 22, 0xffff,
            C.cast(cargo, C.c_void_p), None, None), 1)
        self.assertEqual(events, [400])

    def test_check_colonize_wp_uses_task_group_two_for_other_fleets(self):
        data = (C.c_ubyte * 65536)()
        fleet = (C.c_ubyte * 124)()
        orders = (C.c_ubyte * 40)()
        struct.pack_into('<H', fleet, 6, 8)
        struct.pack_into('<H', fleet, 0x62, 2)
        struct.pack_into('<H', orders, 26, 22)
        struct.pack_into('<H', orders, 28, (1 << 8) | (3 << 4) | 2)
        self.assertEqual(self.lib.stars_check_colonize_wp(
            data, fleet, orders, 4, 22, 3, None, None, None), 1)

    def test_check_patrol_wp_validates_task_seven_and_distance(self):
        data = (C.c_ubyte * 65536)()
        fleet = (C.c_ubyte * 124)()
        orders = (C.c_ubyte * 22)()
        struct.pack_into('<H', fleet, 0x62, 1)
        struct.pack_into('<H', orders, 8, 31)
        struct.pack_into('<H', orders, 10, (1 << 8) | (3 << 4) | 7)
        struct.pack_into('<H', orders, 14, 12)
        self.assertEqual(self.lib.stars_check_patrol_wp(
            data, fleet, orders, 4, 0, 31, 3, 9, 12, None, None), 1)
        self.assertEqual(self.lib.stars_check_patrol_wp(
            data, fleet, orders, 4, 0, 31, 3, 9, 0xffff, None, None), 1)

    def test_check_patrol_wp_sets_distance_error_state_on_mismatch(self):
        data = (C.c_ubyte * 65536)()
        fleet = (C.c_ubyte * 124)()
        orders = (C.c_ubyte * 22)()
        struct.pack_into('<H', fleet, 0x62, 1)
        struct.pack_into('<H', orders, 8, 31)
        struct.pack_into('<H', orders, 10, (1 << 8) | (3 << 4) | 7)
        struct.pack_into('<H', orders, 14, 12)
        self.assertEqual(self.lib.stars_check_patrol_wp(
            data, fleet, orders, 4, 0, 31, 3, 9, 13, None, None), 0)
        self.assertEqual(struct.unpack_from('<H', data, 0x5212)[0], 0x05ef)

    def test_check_battle_plan_bounds_plan_index_and_restores_tutor_id(self):
        data = (C.c_ubyte * 65536)()
        struct.pack_into('<H', data, 0x5212, 0x2468)
        data[0x4830] = 3
        args = (0x12, 0x34, 0x56, 0x78, 0x9a, 0xbc, 0xde, 0xf0)
        self.assertEqual(self.lib.stars_check_battle_plan(data, 0, *args), 1)
        self.assertEqual(bytes(data[0x5212:0x5214]), b'\x68\x24')
        self.assertEqual(self.lib.stars_check_battle_plan(data, 3, *args), 1)
        self.assertEqual(self.lib.stars_check_battle_plan(data, -1, *args), 0)
        self.assertEqual(bytes(data[0x5212:0x5214]), b'\x21\x0c')
        self.assertEqual(self.lib.stars_check_battle_plan(data, 4, *args), 0)
        self.assertEqual(bytes(data[0x5212:0x5214]), b'\x21\x0c')

    def test_ok_merge_dialog_enforces_turn_specific_merge_selection(self):
        data = (C.c_ubyte * 65536)()
        merge = (C.c_ubyte * 8)()
        struct.pack_into('<H', data, 0x82, 0x17)
        struct.pack_into('<HHHH', merge, 0, 4, 9, 0xffff, 0xffff)
        errors = []
        error_type = C.CFUNCTYPE(None, C.c_void_p, C.c_int16)

        @error_type
        def tutor_error(_context, code): errors.append(code)

        callback = C.cast(tutor_error, C.c_void_p)
        self.assertEqual(self.lib.stars_ok_merge_dialog(
            data, merge, callback, None), 1)
        struct.pack_into('<H', merge, 4, 3)
        self.assertEqual(self.lib.stars_ok_merge_dialog(
            data, merge, callback, None), 0)
        self.assertEqual(errors, [0x202])
        self.assertEqual(struct.unpack_from('<H', data, 0x5212)[0], 0x453)

        struct.pack_into('<H', data, 0x82, 0x1b)
        struct.pack_into('<HHHH', merge, 0, 0xffff, 0xffff, 2, 6)
        self.assertEqual(self.lib.stars_ok_merge_dialog(
            data, merge, callback, None), 1)
        struct.pack_into('<H', data, 0x82, 7)
        self.assertEqual(self.lib.stars_ok_merge_dialog(
            data, merge, callback, None), 0)
        self.assertEqual(errors[-1], 0x204)

    def test_cch_tutor_string_decodes_nibbles_and_reuses_last_output(self):
        output = (C.c_ubyte * 32)()
        base = (C.c_ubyte * 64)()
        offsets = (C.c_ubyte * 2)()
        encoded = (C.c_ubyte * 4)(0x12, 0xf3, 0, 0)
        resources = (C.c_ubyte * 256)()
        resources[1] = ord('A')
        resources[2] = ord('B')
        resources[3] = ord('C')
        base[0] = 2
        base[1] = 1
        base[2] = 1
        self.assertEqual(self.lib.stars_cch_tutor_string(
            output, 0, -1, base, offsets, encoded, resources), 2)
        self.assertEqual(bytes(output[:3]), b'AB\0')
        self.assertEqual(self.lib.stars_cch_tutor_string(
            output, 2, -1, base, offsets, encoded, resources), 1)
        self.assertEqual(bytes(output[:2]), b'C\0')
        output[:3] = b'OK\0'
        self.assertEqual(self.lib.stars_cch_tutor_string(
            output, 7, 7, base, offsets, encoded, resources), 2)
        self.assertEqual(bytes(output[:3]), b'OK\0')

    def test_tutorial_ship_builder_dispatches_actions_and_part_requirements(self):
        data = (C.c_ubyte * 65536)()
        expected = (C.c_ubyte * 4)(*b'CLS\0')
        current = (C.c_ubyte * 4)(*b'CLS\0')
        errors = []
        ships = []
        parts = []
        error_type = C.CFUNCTYPE(None, C.c_void_p, C.c_int16)
        ship_type = C.CFUNCTYPE(C.c_int16, C.c_void_p, C.c_int16, C.c_int16)
        part_type = C.CFUNCTYPE(C.c_int16, C.c_void_p, C.c_int16,
                                C.c_uint16, C.c_uint16, C.c_uint16)
        compare_type = C.CFUNCTYPE(C.c_int16, C.c_void_p, PTR, PTR)

        @error_type
        def tutor_error(_context, code): errors.append(code)

        @ship_type
        def check_ship(_context, category, ship):
            ships.append((category, ship))
            return 1

        @part_type
        def check_part(_context, slot, grhst, part, count):
            parts.append((slot, grhst, part, count))
            return 1

        @compare_type
        def compare(_context, left, right):
            return 0 if bytes(left[:3]) == bytes(right[:3]) else 1

        callbacks = (C.cast(check_ship, C.c_void_p),
                     C.cast(check_part, C.c_void_p),
                     C.cast(compare, C.c_void_p),
                     C.cast(tutor_error, C.c_void_p))
        struct.pack_into('<H', data, 0x82, 0x0d)
        struct.pack_into('<H', data, 0x520e, 0x0148)
        self.assertEqual(self.lib.stars_tutorial_enabled_ship_builder(
            data, 1, 0, 0, 0, 0, expected, current, *callbacks, None), 1)
        self.assertEqual(ships[-1], (1, 7))
        struct.pack_into('<H', data, 0x82, 0x0d)
        struct.pack_into('<H', data, 0x520e, 0x0150)
        parts[:] = []
        self.assertEqual(self.lib.stars_tutorial_enabled_ship_builder(
            data, 3, 0, 0, 0, 0, expected, current, *callbacks, None), 1)
        self.assertEqual(parts, [(0, 1, 0x103, 1), (1, 2, 0x101, 1),
                                 (2, 0x80, 0x102, 1),
                                 (3, 0x80, 0x102, 1)])
        self.assertEqual(self.lib.stars_tutorial_enabled_ship_builder(
            data, 4, 0, 0, 0, 0, expected, current, *callbacks, None), 0)
        self.assertEqual(errors[-1], 0x1f7)
        struct.pack_into('<H', data, 0x82, 7)
        self.assertEqual(self.lib.stars_tutorial_enabled_ship_builder(
            data, 1, 0, 0, 0, 0, expected, current, *callbacks, None), 0)
        self.assertEqual(errors[-1], 0x1f2)

    def test_shield_power_accumulates_component_modes_and_bonus(self):
        components = (self.ShieldComponent * 4)(
            self.ShieldComponent(4, 0x0300, 120),
            self.ShieldComponent(8, 0x0206, 999),
            self.ShieldComponent(8, 0x0109, 999),
            self.ShieldComponent(8, 0x0107, 999))
        base = 3 * 120 + 2 * 50 + 100
        self.assertEqual(self.lib.stars_shield_power(components, 4, 0), base)
        self.assertEqual(self.lib.stars_shield_power(components, 4, 1),
                         base + (base * 2) // 5)
        huge = (self.ShieldComponent * 1)(
            self.ShieldComponent(4, 0xffff, 0xffff))
        self.assertEqual(self.lib.stars_shield_power(huge, 1, 0), 0xffff)

    def test_primary_design_applies_special_penalty_before_next_comparison(self):
        counts = (C.c_uint16 * 4)(2, 5, 4, 0)
        types = (C.c_uint16 * 4)(0, 0x19, 0, 0)
        different = C.c_int16()
        self.assertEqual(self.lib.stars_primary_ship_design(
            counts, types, 4, C.byref(different)), 1)
        self.assertEqual(different.value, 3)
        counts = (C.c_uint16 * 3)(1, 2, 3)
        types = (C.c_uint16 * 3)(0, 0x19, 0x1a)
        self.assertEqual(self.lib.stars_primary_ship_design(
            counts, types, 3, C.byref(different)), 2)
        self.assertEqual(different.value, 3)

    def test_fleet_code_packs_id_primary_and_multiple_design_bit(self):
        self.assertEqual(self.lib.stars_fleet_code(0x03ff, 4, 1),
                         (4 << 9) | 0x01ff)
        self.assertEqual(self.lib.stars_fleet_code(7, 2, 2),
                         (2 << 9) | 7 | 0x2000)

    def test_stargate_lookup_uses_selected_design_and_component_encoding(self):
        planet = (C.c_ubyte * 64)()
        planet[4] = 0x00
        planet[5] = 0x02
        planet[44] = 2
        designs = (C.c_ubyte * (0x100 * 3))()
        base = 2 * 0x100
        designs[base + 0x7a] = 2
        for offset, values in ((0x3a, (0x00, 0x02, 0x01, 0x01)),
                               (0x3e, (0x00, 0x02, 0x05, 0x01))):
            for delta, value in enumerate(values):
                designs[base + offset + delta] = value
        self.assertEqual(self.lib.stars_stargate_index(
            planet, designs, 0x100, 3), 1)
        designs[base + 0x3d] = 0
        self.assertEqual(self.lib.stars_stargate_index(
            planet, designs, 0x100, 3), 5)

    def test_jumpgate_requires_every_active_design_to_have_component(self):
        counts = (C.c_uint16 * 3)(1, 0, 2)
        designs = (C.c_ubyte * (0x100 * 3))()
        for index in (0, 2):
            base = index * 0x100
            designs[base + 0x7a] = 1
            designs[base + 0x3a] = 0
            designs[base + 0x3b] = 0x10
            designs[base + 0x3c] = 9
            designs[base + 0x3d] = 1
        self.assertEqual(self.lib.stars_fleet_can_jumpgate(
            counts, designs, 0x100, 3), 1)
        designs[2 * 0x100 + 0x3c] = 8
        self.assertEqual(self.lib.stars_fleet_can_jumpgate(
            counts, designs, 0x100, 3), 0)

    def test_fleet_weight_sums_design_and_mineral_words(self):
        counts = (C.c_uint16 * 16)(1, 2, *([0] * 14))
        weights = (C.c_uint32 * 16)(10, 20, *([0] * 14))
        minerals = (C.c_uint32 * 4)(3, 4, 5, 6)
        self.assertEqual(self.lib.stars_fleet_weight(
            counts, weights, minerals), 10 + 40 + 18)

    def test_match_target_modes_preserve_class_rules(self):
        counts = (C.c_uint16 * 4)(1, 1, 0, 1)
        classes = (C.c_ubyte * 4)(1, 7, 3, 5)
        self.assertEqual(self.lib.stars_match_target(
            counts, classes, 4, 4, 1), 1)
        self.assertEqual(self.lib.stars_match_target(
            counts, classes, 4, 6, 1), 1)
        self.assertEqual(self.lib.stars_match_target(
            counts, classes, 4, 7, 1), 1)
        self.assertEqual(self.lib.stars_match_target(
            counts, classes, 4, 3, 1), 0)
        self.assertEqual(self.lib.stars_match_target(
            counts, classes, 4, 5, 1), 1)
        self.assertEqual(self.lib.stars_match_target(
            counts, classes, 4, 8, 0), 1)

    def test_fleet_id_search_respects_player_group_and_low_id_bits(self):
        ids = (C.c_uint16 * 5)(2, 7, 0x0207, 0x041f, 0x042c)
        player_counts = (C.c_uint16 * 3)(2, 1, 2)
        self.assertEqual(self.lib.stars_fleet_index_from_grouped_id(
            ids, 5, player_counts, 3, 1, 0x0207), 2)
        self.assertEqual(self.lib.stars_fleet_index_from_grouped_id(
            ids, 5, player_counts, 3, 2, 0x041f), 3)
        self.assertEqual(self.lib.stars_fleet_index_from_grouped_id(
            ids, 5, player_counts, 3, 1, 0x0208), -1)

    def test_object_lookup_dispatches_each_object_kind(self):
        planets = (C.c_uint16 * 2)(4, 9)
        fleets = (C.c_uint16 * 2)(12, 20)
        things = (C.c_uint16 * 2)(31, 44)
        args = (planets, 2, fleets, 2, things, 2)
        self.assertEqual(self.lib.stars_lookup_object_index(1, 9, *args), 1)
        self.assertEqual(self.lib.stars_lookup_object_index(2, 20, *args), 1)
        self.assertEqual(self.lib.stars_lookup_object_index(8, 44, *args), 1)
        self.assertEqual(self.lib.stars_lookup_object_index(8, 99, *args), -1)

    def test_selected_lookup_gates_by_object_kind_and_resolves_fleet_scan(self):
        planets = (C.c_uint16 * 2)(4, 9)
        self.assertEqual(self.lib.stars_selected_planet_index(
            1, 9, planets, 2), 1)
        self.assertEqual(self.lib.stars_selected_planet_index(
            2, 9, planets, 2), -1)
        selected = (C.c_uint16 * 2)(12, 20)
        fleets = (C.c_uint16 * 3)(7, 12, 20)
        self.assertEqual(self.lib.stars_selected_fleet_index(
            2, 1, selected, 2, fleets, 3), 2)
        self.assertEqual(self.lib.stars_selected_fleet_index(
            1, 1, selected, 2, fleets, 3), -1)

    def test_thing_lookup_record_copies_in_both_original_directions(self):
        things = (C.c_ubyte * (24 * 2))()
        for offset, value in ((0, 7), (24, 12)):
            things[offset] = value
            things[offset + 2] = value + 1
        record = (C.c_ubyte * 24)()
        self.assertEqual(self.lib.stars_lookup_thing_record(
            12, things, 2, 24, record, 0), 1)
        self.assertEqual(bytes(record[:4]), bytes((12, 0, 13, 0)))
        record[0] = 7
        record[2] = 99
        self.assertEqual(self.lib.stars_lookup_thing_record(
            -1, things, 2, 24, record, 0), 1)
        self.assertEqual(things[2], 99)

    def test_can_build_design_checks_hull_then_nonempty_components(self):
        calls = []
        callback_type = C.CFUNCTYPE(C.c_int16, C.c_void_p, C.c_uint16,
                                    C.c_uint16)

        @callback_type
        def lookup(_context, kind, value):
            calls.append((kind, value))
            return int((kind, value) != (0x1000, 0x0202))

        components = (self.ShieldComponent * 2)(
            self.ShieldComponent(0x200, 0x0101, 0),
            self.ShieldComponent(0x1000, 0x0202, 0))
        self.assertEqual(self.lib.stars_can_build_design(
            0x1f, components, 2, C.cast(lookup, C.c_void_p), None), 0)
        self.assertEqual(calls, [(0x4000, 0x1f), (0x200, 0x0101),
                                 (0x1000, 0x0202)])
        calls[:] = []
        components[1].packed_count_type = 0
        self.assertEqual(self.lib.stars_can_build_design(
            0x20, components, 2, C.cast(lookup, C.c_void_p), None), 1)
        self.assertEqual(calls, [(0x400, 0), (0x200, 0x0101)])

        @callback_type
        def unavailable_hull(_context, kind, value):
            return 2 if kind == 0x400 else 1

        @callback_type
        def unavailable_component(_context, kind, value):
            return 2 if kind == 0x200 else 1

        for rejected in (unavailable_hull, unavailable_component):
            self.assertEqual(self.lib.stars_can_build_design(
                0x20, components, 2, C.cast(rejected, C.c_void_p), None), 0)

    def test_similar_design_matches_zero_quantity_without_kind_and_filters_flags(self):
        target_kinds = (C.c_uint16 * 2)(0x100, 0x200)
        target_packed = (C.c_uint16 * 2)(0x0103, 0x0007)
        hulls = (C.c_uint16 * 3)(9, 9, 9)
        flags = (C.c_uint16 * 3)(0x8000, 0x8200, 0x8000)
        counts = (C.c_uint16 * 3)(2, 2, 2)
        kinds = (C.c_uint16 * 6)(0x100, 0x999, 0x100, 0x200,
                                 0x100, 0x200)
        packed = (C.c_uint16 * 6)(0x0103, 0x0001, 0x0103, 0x0007,
                                  0x0104, 0x0007)
        self.assertEqual(self.lib.stars_find_similar_design(
            9, target_kinds, target_packed, 2, hulls, flags, counts,
            kinds, packed, 3, 2), 0)
        packed[0] = 0x0104
        self.assertEqual(self.lib.stars_find_similar_design(
            9, target_kinds, target_packed, 2, hulls, flags, counts,
            kinds, packed, 3, 2), -1)

    def test_hull_name_decoration_uses_duplicate_ordinal_and_hidden_flag(self):
        names = (C.c_ubyte * (32 * 4))()
        for index, text in enumerate((b'Falcon', b'Falcon', b'Falcon', b'Hidden')):
            for delta, value in enumerate(text + b'\0'):
                names[index * 32 + delta] = value
        flags = (C.c_uint16 * 4)(0, 0, 0, 0x0200)
        output = (C.c_ubyte * 64)()
        self.lib.stars_decorate_hull_name(
            names, 32, flags, 3, 0, 2, 4, output, 64)
        self.assertEqual(C.string_at(output), b'Falcon (III)')
        self.lib.stars_decorate_hull_name(
            names, 32, flags, 0, 0, 1, 4, output, 64)
        self.assertEqual(C.string_at(output), b'Falcon')
        self.lib.stars_decorate_hull_name(
            names, 32, flags, 3, 0, 3, 4, output, 64)
        self.assertEqual(C.string_at(output), b'')

    def test_turn_log_emits_channel_six_only_when_enabled(self):
        events = []
        callback_type = C.CFUNCTYPE(None, C.c_void_p, C.c_int16, PTR)

        @callback_type
        def emit(_context, channel, text):
            events.append((channel, C.string_at(text)))

        fmt = (C.c_ubyte * 8)(*b'Turn %d\0')
        output = (C.c_ubyte * 32)()
        self.assertEqual(self.lib.stars_turn_log(
            -1, 7, 12, fmt, output, 32, C.cast(emit, C.c_void_p), None), 1)
        self.assertEqual(events, [(6, b'Turn 2413')])
        self.assertEqual(self.lib.stars_turn_log(
            0, 7, 12, fmt, output, 32, C.cast(emit, C.c_void_p), None), 0)

    def test_progress_update_preserves_special_codes_and_overflow_display_value(self):
        events = []
        callback_type = C.CFUNCTYPE(None, C.c_void_p, C.c_int16,
                                    C.c_int16)

        @callback_type
        def draw(_context, percent, number):
            events.append((percent, number))

        state = ProgressState(1, 10)
        self.assertEqual(self.lib.stars_update_progress(
            C.byref(state), -0x39e, 0, C.cast(draw, C.c_void_p), None), 1)
        self.assertEqual((state.percent, events[-1]), (14, (14, 0)))
        self.assertEqual(self.lib.stars_update_progress(
            C.byref(state), -0x39f, 0, C.cast(draw, C.c_void_p), None), 1)
        self.assertEqual((state.percent, events[-1]), (15, (15, 0)))
        self.assertEqual(self.lib.stars_update_progress(
            C.byref(state), 1200, 0, C.cast(draw, C.c_void_p), None), 0)
        self.assertEqual(state.percent, 15)
        self.assertEqual(self.lib.stars_update_progress(
            C.byref(state), 1200, 0x400, C.cast(draw, C.c_void_p), None), 1)
        self.assertEqual((state.percent, events[-1]), (15, (15, 1200)))
        self.assertEqual(self.lib.stars_update_progress(
            C.byref(state), -3, 0, C.cast(draw, C.c_void_p), None), 1)
        self.assertEqual((state.percent, events[-1]), (0, (0, 0)))

    def test_progress_show_hide_create_once_update_existing_and_reset(self):
        events = []
        create_type = C.CFUNCTYPE(C.c_uint16, C.c_void_p)
        draw_type = C.CFUNCTYPE(None, C.c_void_p, C.c_int16, C.c_int16)
        destroy_type = C.CFUNCTYPE(None, C.c_void_p, C.c_uint16)

        @create_type
        def create(_context):
            events.append(('create',))
            return 44

        @draw_type
        def draw(_context, percent, number):
            events.append(('draw', percent, number))

        @destroy_type
        def destroy(_context, window):
            events.append(('destroy', window))

        state = ProgressState(0, 77)
        self.assertEqual(self.lib.stars_show_progress(
            C.byref(state), C.cast(create, C.c_void_p),
            C.cast(draw, C.c_void_p), None), 1)
        self.assertEqual(events, [('create',)])
        self.assertEqual(self.lib.stars_show_progress(
            C.byref(state), C.cast(create, C.c_void_p),
            C.cast(draw, C.c_void_p), None), 1)
        self.assertEqual(events[-1], ('draw', 0, 0))
        self.lib.stars_hide_progress(C.byref(state),
                                     C.cast(destroy, C.c_void_p), None)
        self.assertEqual((state.window, events[-1]), (0, ('destroy', 44)))

    def test_printer_font_uses_dpi_height_and_restores_selected_object(self):
        events = []
        callback_type = C.CFUNCTYPE(C.c_uint32, C.c_void_p,
                                    C.POINTER(WindowCall))

        @callback_type
        def window(_context, call):
            item = call.contents
            if item.operation == b'GetDeviceCaps':
                return 144
            if item.operation == b'CreateFontIndirect':
                logfont = C.string_at(item.p, 50)
                events.append(('font', int.from_bytes(logfont[:2], 'little'),
                               logfont[18:].split(b'\0', 1)[0]))
                return 0x2222
            if item.operation == b'SelectObject':
                events.append(('select', item.window, item.a))
                return 0x3333
            if item.operation == b'GetTextMetrics':
                C.memset(item.p, 0, 32)
                C.cast(item.p, C.POINTER(C.c_uint16))[0] = 12
                C.cast(item.p, C.POINTER(C.c_uint16))[4] = 3
            return 0

        data = (C.c_ubyte * 65536)()
        for offset, value in enumerate(b'Arial\0'):
            data[0x6c6 + offset] = value
        platform = Platform()
        platform.window = C.cast(window, C.c_void_p).value
        runtime = Runtime()
        runtime.data = data
        runtime.platform = C.cast(C.pointer(platform), C.c_void_p).value
        dy = C.c_int16()
        self.assertEqual(self.lib.stars_printer_font_create(
            C.byref(runtime), 7, 12, C.byref(dy)), 0x2222)
        self.assertEqual(dy.value, 15)
        self.assertEqual(events, [('font', 0xffe8, b'Arial'),
                                  ('select', 7, 0x2222),
                                  ('select', 7, 0x3333)])

    def test_copy_file_transfers_0x800_chunks_and_final_remainder(self):
        source = bytes((i & 0xff for i in range(0x900)))
        destination = bytearray()
        events = []
        handles = {3: bytearray(source), 4: destination}
        callback_type = C.CFUNCTYPE(C.c_int16, C.c_void_p, PTR,
                                    C.c_uint16, C.POINTER(C.c_uint16))
        close_type = C.CFUNCTYPE(C.c_int16, C.c_void_p, C.c_int16)
        read_type = C.CFUNCTYPE(C.c_uint16, C.c_void_p, C.c_int16, PTR,
                                C.c_uint16)
        write_type = C.CFUNCTYPE(C.c_uint16, C.c_void_p, C.c_int16, PTR,
                                 C.c_uint16)
        length_type = C.CFUNCTYPE(C.c_uint32, C.c_void_p, C.c_int16)

        @callback_type
        def open_file(_ctx, name, mode, _error):
            events.append(('open', C.string_at(name), mode))
            return 3 if mode == 0x20 else 4

        @close_type
        def close_file(_ctx, handle):
            events.append(('close', handle))
            return 0

        @read_type
        def read_file(_ctx, handle, output, count):
            chunk = handles[handle][:count]
            del handles[handle][:len(chunk)]
            C.memmove(output, bytes(chunk), len(chunk))
            return len(chunk)

        @write_type
        def write_file(_ctx, handle, input_data, count):
            handles[handle].extend(C.string_at(input_data, count))
            return count

        @length_type
        def length_file(_ctx, _handle):
            return 0x900

        api = StreamApi()
        api.open = C.cast(open_file, C.c_void_p).value
        api.close = C.cast(close_file, C.c_void_p).value
        api.read = C.cast(read_file, C.c_void_p).value
        api.write = C.cast(write_file, C.c_void_p).value
        src = (C.c_ubyte * 5)(*b'src\0')
        dst = (C.c_ubyte * 5)(*b'dst\0')
        self.assertEqual(self.lib.stars_copy_file(
            src, dst, C.byref(api), None, C.cast(length_file, C.c_void_p)), 1)
        self.assertEqual(bytes(destination), source)
        self.assertEqual(events, [('open', b'src', 0x20),
                                  ('open', b'dst', 0x1012),
                                  ('close', 3), ('close', 4)])

    def test_planet_print_dot_draws_original_small_and_large_geometry(self):
        events = []
        callback_type = C.CFUNCTYPE(C.c_uint32, C.c_void_p,
                                    C.POINTER(WindowCall))

        @callback_type
        def window(_context, call):
            item = call.contents
            if item.operation == b'PatBlt':
                size = C.cast(item.p, C.POINTER(C.c_uint16))
                events.append((item.window, item.a, item.b,
                               size[0], size[1], item.value))
            return 0

        platform = Platform()
        platform.window = C.cast(window, C.c_void_p).value
        runtime = Runtime()
        runtime.platform = C.cast(C.pointer(platform), C.c_void_p).value
        self.lib.stars_draw_planet_print_dot(
            C.byref(runtime), 5, 20, 30, 0)
        self.assertEqual(events, [(5, 17, 29, 7, 3, 0x42),
                                  (5, 19, 27, 3, 7, 0x42),
                                  (5, 18, 28, 5, 5, 0x42)])
        events[:] = []
        self.lib.stars_draw_planet_print_dot(
            C.byref(runtime), 5, 20, 30, 1)
        self.assertEqual(events, [(5, 15, 28, 11, 5, 0x42),
                                  (5, 18, 25, 5, 11, 0x42),
                                  (5, 16, 27, 9, 7, 0x42),
                                  (5, 17, 26, 7, 9, 0x42)])

    def test_dx_stream_text_out_advances_and_optionally_emits(self):
        events = []
        callback_type = C.CFUNCTYPE(C.c_uint32, C.c_void_p,
                                    C.POINTER(WindowCall))

        @callback_type
        def window(_context, call):
            item = call.contents
            if item.operation == b'GetTextExtent':
                return 6 | (9 << 16)
            if item.operation == b'TextOut':
                events.append((item.window, item.a, item.b, item.value))
            return 0

        platform = Platform()
        platform.window = C.cast(window, C.c_void_p).value
        runtime = Runtime()
        runtime.platform = C.cast(C.pointer(platform), C.c_void_p).value
        text = (C.c_ubyte * 4)(ord('a'), ord('b'), ord('c'), 0)
        x = C.c_int16(32765)
        self.assertEqual(self.lib.stars_dx_stream_text_out(
            C.byref(runtime), 4, C.byref(x), 12, text, 0, 1), 6)
        self.assertEqual(x.value, -32765)
        self.assertEqual(events, [(4, 32765, 12, 3)])

    def test_right_mouse_move_accepts_right_button_release(self):
        callback_type = C.CFUNCTYPE(C.c_uint32, C.c_void_p,
                                    C.POINTER(WindowCall))

        @callback_type
        def window(_context, call):
            item = call.contents
            if item.operation == b'PeekMessage':
                message = bytearray(18)
                message[2:4] = (0x205).to_bytes(2, 'little')
                message[6:8] = (0xfff0).to_bytes(2, 'little')
                message[8:10] = (0x0024).to_bytes(2, 'little')
                C.memmove(item.p, bytes(message), len(message))
                return 1
            return 0

        platform = Platform()
        platform.window = C.cast(window, C.c_void_p).value
        runtime = Runtime()
        runtime.platform = C.cast(C.pointer(platform), C.c_void_p).value
        point = (C.c_uint16 * 2)()
        self.assertEqual(self.lib.stars_get_right_mouse_move(
            C.byref(runtime), point), 0)
        self.assertEqual(list(point), [0xfff0, 0x0024])

    def test_string_fits_screen_truncates_at_extent_boundary(self):
        events = []
        callback_type = C.CFUNCTYPE(C.c_uint32, C.c_void_p,
                                    C.POINTER(WindowCall))

        @callback_type
        def window(_context, call):
            item = call.contents
            if item.operation == b'GetDC':
                events.append('getdc')
                return 7
            if item.operation == b'SelectObject':
                events.append(('select', item.a))
                return 8
            if item.operation == b'GetTextExtent':
                return item.a * 4
            if item.operation == b'ReleaseDC':
                events.append(('release', item.a))
            return 0

        platform = Platform()
        platform.window = C.cast(window, C.c_void_p).value
        runtime = Runtime()
        runtime.platform = C.cast(C.pointer(platform), C.c_void_p).value
        text = (C.c_ubyte * 6)(ord('a'), ord('b'), ord('c'), ord('d'), ord('e'), 0)
        self.assertEqual(self.lib.stars_string_fits_screen(
            C.byref(runtime), 3, 9, text, 10), 0)
        self.assertEqual(bytes(text), b'ab\0\0\0\0')
        self.assertEqual(events, ['getdc', ('select', 9), ('select', 8),
                                  ('release', 7)])

    def test_wrap_text_out_emits_word_chunks_and_advances_lines(self):
        events = []
        callback_type = C.CFUNCTYPE(C.c_uint32, C.c_void_p,
                                    C.POINTER(WindowCall))

        @callback_type
        def window(_context, call):
            item = call.contents
            if item.operation == b'GetTextExtent':
                return item.a * 4
            if item.operation == b'TextOut':
                events.append((item.a, item.b, item.value,
                               C.string_at(item.p, item.value)))
            return 0

        platform = Platform()
        platform.window = C.cast(window, C.c_void_p).value
        runtime = Runtime()
        runtime.platform = C.cast(C.pointer(platform), C.c_void_p).value
        runtime.data = (C.c_ubyte * 65536)()
        runtime.data[0x23fa] = 12
        text = (C.c_ubyte * 12)(*b'hello world\0')
        x, y, maximum = C.c_int16(0), C.c_int16(10), C.c_int16(-32768)
        self.lib.stars_wrap_text_out(C.byref(runtime), 4, C.byref(x), C.byref(y),
                                     text, 0, 0, 20, C.byref(maximum), 0, 1)
        self.assertEqual(events, [(0, 10, 5, b'hello'),
                                  (0, 22, 5, b'world')])
        self.assertEqual((x.value, y.value, maximum.value), (20, 22, 20))

    def test_intersect_circle_line_preserves_projection_and_clamps(self):
        start, end = C.c_int16(), C.c_int16()
        hit = self.lib.stars_intersect_circle_line(
            0, 0, 100, 0, 50, 10, 225, 100,
            C.byref(start), C.byref(end))
        self.assertEqual((hit, start.value, end.value), (1, 39, 61))
        self.assertEqual(self.lib.stars_intersect_circle_line(
            0, 0, 100, 0, 50, 20, 225, 100,
            C.byref(start), C.byref(end)), 0)
        hit = self.lib.stars_intersect_circle_line(
            100, 0, 0, 0, 50, 10, 225, 40,
            C.byref(start), C.byref(end))
        self.assertEqual((hit, start.value, end.value), (1, 39, 40))

    def test_draw_fuzzy_border_preserves_edge_geometry_and_attributes(self):
        events = []
        callback_type = C.CFUNCTYPE(C.c_uint32, C.c_void_p,
                                    C.POINTER(WindowCall))

        @callback_type
        def window(_context, call):
            item = call.contents
            if item.operation == b'SetBkColor':
                events.append(('bk', item.value))
                return 0x11111111
            if item.operation == b'SetTextColor':
                events.append(('fg', item.value))
                return 0x22222222
            if item.operation == b'SelectObject':
                events.append(('select', item.a))
                return 8
            if item.operation == b'PatBlt':
                size = C.cast(item.p, C.POINTER(C.c_uint16 * 2)).contents
                events.append(('pat', item.a, item.b, size[0], size[1]))
            return 0

        platform = Platform()
        platform.window = C.cast(window, C.c_void_p).value
        runtime = Runtime()
        runtime.platform = C.cast(C.pointer(platform), C.c_void_p).value
        rect = (C.c_uint16 * 4)(10, 20, 50, 60)
        self.lib.stars_draw_fuzzy_border(
            C.byref(runtime), 4, rect, 7, 0xabcdef01, 0x10203040)
        self.assertEqual(events[0:3], [('bk', 0xabcdef01),
                                       ('fg', 0x10203040), ('select', 7)])
        self.assertEqual([x for x in events if x[0] == 'pat'], [
            ('pat', 10, 20, 40, 2), ('pat', 10, 58, 40, 2),
            ('pat', 10, 22, 2, 36), ('pat', 48, 22, 2, 36)])
        self.assertEqual(events[-3:], [('select', 8), ('fg', 0x22222222),
                                       ('bk', 0x11111111)])

    def test_black_reserved_palette_builds_all_entries(self):
        palettes = []
        callback_type = C.CFUNCTYPE(C.c_uint32, C.c_void_p,
                                    C.POINTER(WindowCall))

        @callback_type
        def window(_context, call):
            item = call.contents
            if item.operation == b'CreatePalette':
                palettes.append(C.string_at(item.p, 1028))
                return 0x1234
            return 0

        platform = Platform()
        platform.window = C.cast(window, C.c_void_p).value
        runtime = Runtime()
        runtime.platform = C.cast(C.pointer(platform), C.c_void_p).value
        self.assertEqual(self.lib.stars_black_reserved_palette(
            C.byref(runtime)), 0x1234)
        self.assertEqual(len(palettes), 1)
        palette = palettes[0]
        self.assertEqual(palette[:4], b'\0\3\0\1')
        self.assertEqual(palette[4:], b'\0\0\0\1' * 256)

    def test_serial_validation_restores_rng_and_reports_packed_value(self):
        state = self.Seeds(0x12345678, 0x23456789)
        original = (state.first, state.second)
        serial = (C.c_ubyte * 9)(*b'ABCDEFGH\0')
        packed = C.c_uint32()
        self.lib.stars_validate_serial_number(
            serial, C.byref(state), C.byref(packed))
        expected = (((((0x00 * 36 + 0x01) * 36 + 0x04) * 36 +
                      0x07) * 36 + 0x03))
        self.assertEqual(packed.value, expected)
        self.assertEqual((state.first, state.second), original)

    def test_design_power_reconstructs_component_classes_and_modifier(self):
        components = (self.PowerComponent * 4)(
            self.PowerComponent(0x10, 0x0200, 40, 7, 0, 0),
            self.PowerComponent(0x20, 0x0100, 30, 8, 0, 0),
            self.PowerComponent(0x40, 0x0300, 12, 0, 5, 0),
            self.PowerComponent(0x800, 0x010c, 0, 0, 20, 0))
        # cargo: 40*2*(7+3)/4 = 200, then *1.2 => 240;
        # fuel: 30*1*(8-2)/2 = 90; engine: (12+5)*3*2 = 102;
        # modifier: 1000*120/100; speed term: 240*(8-4)/10 = 96.
        self.assertEqual(self.lib.stars_compute_design_power(
            components, 4, 8), 528)

    def test_design_power_batch_skips_hidden_definitions(self):
        components = (self.PowerComponent * 1)(
            self.PowerComponent(0x40, 0x0100, 10, 0, 2, 0))
        designs = (self.PowerDesign * 2)(
            self.PowerDesign(0, 1, components, 4, 0),
            self.PowerDesign(0x0200, 1, components, 4, 77))
        self.lib.stars_compute_design_powers(designs, 2)
        self.assertEqual(designs[0].power, 24)
        self.assertEqual(designs[1].power, 77)

    def test_ram_scoop_gain_uses_consecutive_zero_use_warps(self):
        first = self.RamScoopDesign(2, 5, (C.c_ubyte * 11)(
            0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0))
        second = self.RamScoopDesign(1, 9, (C.c_ubyte * 11)(
            1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0))
        designs = (self.RamScoopDesign * 2)(first, second)
        # First design: 5*(1+2+3) per ship, times two ships; second is
        # blocked at warp zero. Travel distance scales the total by four.
        self.assertEqual(self.lib.stars_fuel_gain_from_ram_scoops(
            designs, 2, 0, 4), 240)
        self.assertEqual(self.lib.stars_fuel_gain_from_ram_scoops(
            designs, 2, 11, 4), 0)

    def test_scanner_ranges_aggregate_max_min_and_steal_flags(self):
        designs = (self.ScannerDesign * 3)(
            self.ScannerDesign(2, 30, 12, 80, 1),
            self.ScannerDesign(1, 45, 20, 60, 2),
            self.ScannerDesign(0, 90, 99, 1, 4))
        plan = C.c_int16()
        detect = C.c_int16()
        steal = C.c_int16(0)
        self.assertEqual(self.lib.stars_get_fleet_scanner_range(
            designs, 3, C.byref(plan), C.byref(detect), C.byref(steal)), 45)
        self.assertEqual((plan.value, detect.value, steal.value), (20, 60, 3))
        designs[1].scanner_range = 0x07ff
        designs[1].planet_range = 4
        self.assertEqual(self.lib.stars_get_cached_fleet_scanner_range(
            designs, 3, C.byref(plan), C.byref(detect), C.byref(steal)), 30)
        self.assertEqual((plan.value, detect.value), (12, 60))

    def test_planet_scanner_range_preserves_modes_deep_and_blocked_case(self):
        deep = C.c_int16(-1)
        self.assertEqual(self.lib.stars_get_planet_scanner_range(
            8, 0, 1600, 0, 0, 1, C.byref(deep)), 126)
        self.assertEqual(deep.value, 63)
        self.assertEqual(self.lib.stars_get_planet_scanner_range(
            8, 1, 1600, 0, 0, 1, C.byref(deep)), 177)
        self.assertEqual(deep.value, 0)
        self.assertEqual(self.lib.stars_get_planet_scanner_range(
            7, 0, 0, -40, 0, 0, C.byref(deep)), 40)
        self.assertEqual(deep.value, 20)
        self.assertEqual(self.lib.stars_get_planet_scanner_range(
            7, 0, 0, 40, 1, 0, C.byref(deep)), 0)

    def test_design_scanner_range_uses_fourth_power_components(self):
        parts = (self.ScannerComponent * 3)(
            self.ScannerComponent(2, 0x0206, 10, 0),
            self.ScannerComponent(8, 0x0109, 0, 0),
            self.ScannerComponent(0x800, 0x020f, 0, 0))
        table = (C.c_ubyte * 18)(*[100 - i for i in range(18)])
        plan = C.c_int16()
        detect = C.c_int16()
        steal = C.c_int16()
        result = self.lib.stars_get_design_scanner_range(
            parts, 3, 1, 1, 0, table, C.byref(plan),
            C.byref(detect), C.byref(steal))
        self.assertEqual(result, 80)
        self.assertEqual(plan.value, 57)
        self.assertEqual((detect.value, steal.value), (98, 0))

    def test_design_scanner_recognizes_builtin_power_without_components(self):
        plan = C.c_int16()
        result = self.lib.stars_get_design_scanner_range(
            None, 0, 60 ** 4, 30 ** 4, 0, None, C.byref(plan), None, None)
        self.assertEqual((result, plan.value), (60, 30))
        self.assertEqual(self.lib.stars_get_design_scanner_range(
            None, 0, 0, 0, 0, None, C.byref(plan), None, None), -1)

    def test_fleet_design_damage_uses_packed_count_and_hull_damage(self):
        fleet = (C.c_ubyte * 64)()
        # rgcsh[2] = 12; rgcsh[18] has low 7 bits 5 and high field 3.
        fleet[0x0c + 4] = 12
        fleet[0x0c + 36] = 5 | (3 << 7)
        fleet[0x0c + 37] = (5 | (3 << 7)) >> 8
        damage = (C.c_int16 * 16)(*[0] * 16)
        damage[2] = 200
        # 12 * (((5*200)/10)*3) / 5000 truncates to 0 with the original
        # integer steps; larger counts demonstrate the same ordering.
        fleet[0x0c + 4] = 120
        self.assertEqual(self.lib.stars_fleet_design_damage(
            fleet, 2, damage, 16), 7)
        fleet[0x0c + 4] = 1250
        fleet[0x0c + 5] = 1250 >> 8
        self.assertEqual(self.lib.stars_fleet_design_damage(
            fleet, 2, damage, 16), 75)

    def test_orbiting_transfer_searches_fleets_then_things(self):
        fleets = (C.c_ubyte * 248)()
        # Fleet 0: id 11, planet 4, point (10, 20); fleet 1 is the nth match.
        fleets[0:2] = (11).to_bytes(2, 'little', signed=True)
        fleets[6:8] = (4).to_bytes(2, 'little', signed=True)
        fleets[8:10] = (10).to_bytes(2, 'little', signed=True)
        fleets[10:12] = (20).to_bytes(2, 'little', signed=True)
        fleets[124:126] = (12).to_bytes(2, 'little', signed=True)
        fleets[130:132] = (4).to_bytes(2, 'little', signed=True)
        fleets[132:134] = (10).to_bytes(2, 'little', signed=True)
        fleets[134:136] = (20).to_bytes(2, 'little', signed=True)
        transfer = (C.c_ubyte * 128)()
        self.assertEqual(self.lib.stars_lookup_orbiting_transfer(
            4, 1, fleets, 2, 124, None, 0, 18, -1, 10, 20,
            transfer), 1)
        self.assertEqual(int.from_bytes(bytes(transfer[:2]), 'little'), 12)
        self.assertEqual(int.from_bytes(bytes(transfer[4:6]), 'little'), 12)
        things = (C.c_ubyte * 18)()
        full_id = 1 | (1 << 13)
        things[0:2] = full_id.to_bytes(2, 'little')
        things[2:4] = (30).to_bytes(2, 'little', signed=True)
        things[4:6] = (40).to_bytes(2, 'little', signed=True)
        self.assertEqual(self.lib.stars_lookup_orbiting_transfer(
            4, 0, fleets, 0, 124, things, 1, 18, -1, 30, 40,
            transfer), 1)
        self.assertEqual(int.from_bytes(bytes(transfer[:2]), 'little'), full_id)
        self.assertEqual(int.from_bytes(bytes(transfer[2:4]), 'little'), 8)

    def test_stargate_eligibility_preserves_owner_damage_and_flag_codes(self):
        callback_type = C.CFUNCTYPE(C.c_int16, C.c_void_p, C.c_int16,
                                    C.c_int16, C.c_int16, C.c_int16,
                                    C.POINTER(C.c_int16))
        @callback_type
        def damage(_context, source, destination, distance, hull, output):
            self.assertEqual((source, destination, distance), (3, 7, 80))
            output[0] = hull // 10
            return 1
        minerals = (C.c_int32 * 4)(0, 20, 0, 0)
        counts = (C.c_uint16 * 2)(1, 2)
        hulls = (C.c_int16 * 2)(30, 50)
        self.assertEqual(self.lib.stars_can_fleet_use_stargates(
            2, 1, 2, 0, 7, 1, 2, 0, 3, 1, 6, minerals, 80,
            counts, hulls, 2, damage, None), 7)
        self.assertEqual(self.lib.stars_can_fleet_use_stargates(
            2, 1, -1, 1, 7, 1, 2, 0, 3, 1, 6, minerals, 80,
            counts, hulls, 2, damage, None), 0)

    def test_update_design_cost_accumulates_resources_weight_and_damage(self):
        design = self.DesignCost(100, (10, 20, 30), 40, 50, 7)
        components = (self.CostComponent * 3)(
            self.CostComponent(4, 0x0203, (1, 2, 3), 4, 0, 0),
            self.CostComponent(8, 0x0109, (5, 6, 7), 8, 0, 12),
            self.CostComponent(0x1000, 0x0104, (9, 10, 11), 12, 0, 0))
        self.lib.stars_update_design_cost(C.byref(design), components, 3, 1)
        self.assertEqual(list(design.ore_cost), [26, 40, 54])
        self.assertEqual((design.resource_cost, design.weight_empty), (68, 78))
        # 100 + (2*65) + (12/2) + 50 = 286.
        self.assertEqual(design.damage_points, 286)

    def test_calc_pct_survive_uses_operable_cap_and_smart_damage_scale(self):
        ordinary = C.c_float()
        smart = C.c_float()
        self.lib.stars_calc_pct_survive(-1, 12, 4, 400,
                                        C.byref(ordinary), C.byref(smart))
        self.assertEqual((ordinary.value, smart.value), (1.0, 1.0))
        self.lib.stars_calc_pct_survive(2, 0x1005, 3, 400,
                                        C.byref(ordinary), C.byref(smart))
        self.assertAlmostEqual(ordinary.value, 0.6 ** 3, places=6)
        self.assertAlmostEqual(smart.value, 0.8 ** 3, places=6)

    def test_duplicate_planet_copies_scalars_and_production_tail(self):
        source_records = (C.c_ubyte * 8)(*range(8))
        source_prod = self.PlanetProduction(2, 2, source_records)
        source = self.PlanetState((C.c_ubyte * 52)(*range(52)),
                                  C.pointer(source_prod))
        destination = self.PlanetState((C.c_ubyte * 52)(*([99] * 52)), None)
        self.assertEqual(self.lib.stars_duplicate_planet(
            C.byref(destination), C.byref(source)), 1)
        self.assertEqual(bytes(destination.bytes), bytes(source.bytes))
        self.assertEqual(destination.production.contents.count, 2)
        self.assertEqual(bytes(destination.production.contents.records[:8]),
                         bytes(source_records))

    def test_lookup_planet_refreshes_and_commits_working_record(self):
        stored = self.PlanetState((C.c_ubyte * 52)(*([7] * 52)), None)
        stored.bytes[0], stored.bytes[1] = 4, 0
        planets = (C.POINTER(self.PlanetState) * 1)(C.pointer(stored))
        working = self.PlanetState((C.c_ubyte * 52)(*([2] * 52)), None)
        working.bytes[0], working.bytes[1] = 4, 0
        self.assertEqual(self.lib.stars_lookup_planet(
            planets, 1, 4, C.byref(working), None, None), 1)
        self.assertEqual(bytes(working.bytes), bytes(stored.bytes))
        working.bytes[10] = 99
        self.assertEqual(self.lib.stars_lookup_planet(
            planets, 1, -1, C.byref(working), None, None), 1)
        self.assertEqual(stored.bytes[10], 99)

    def test_duplicate_fleet_copies_scalars_and_order_tail(self):
        source_records = (C.c_ubyte * 36)(*range(36))
        source_orders = self.FleetOrders(2, 2, source_records)
        source = self.FleetState((C.c_ubyte * 124)(*range(124)),
                                 C.pointer(source_orders))
        destination = self.FleetState((C.c_ubyte * 124)(*([99] * 124)), None)
        self.assertEqual(self.lib.stars_duplicate_fleet(
            C.byref(source), C.byref(destination)), 1)
        self.assertEqual(bytes(destination.bytes), bytes(source.bytes))
        self.assertEqual(destination.orders.contents.count, 2)
        self.assertEqual(bytes(destination.orders.contents.records[:36]),
                         bytes(source_records))

    def test_delete_fleet_marks_dead_and_compacts_sorted_table(self):
        first = self.FleetState(
            (C.c_ubyte * 124)(1, 0, 0, 0, *([0] * 120)), None)
        second = self.FleetState(
            (C.c_ubyte * 124)(3, 0, 0, 0, *([0] * 120)), None)
        third = self.FleetState(
            (C.c_ubyte * 124)(7, 0, 0, 0, *([0] * 120)), None)
        table = (C.POINTER(self.FleetState) * 3)(
            C.pointer(first), C.pointer(second), C.pointer(third))
        count = C.c_uint16(3)
        self.assertEqual(self.lib.stars_delete_fleet(
            table, C.byref(count), 3, 0, -1, None, None), 1)
        self.assertEqual(count.value, 2)
        self.assertEqual([table[i].contents.bytes[0] for i in range(2)], [1, 7])
        self.assertEqual(second.bytes[4] | (second.bytes[5] << 8), 0x400)

    def test_new_fleet_initializes_packed_identity_point_and_first_order(self):
        table = self.FleetTable()
        result = C.POINTER(self.FleetState)()
        self.assertEqual(self.lib.stars_new_fleet(
            C.byref(table), 2, 17, 100, -40, C.byref(result)), 1)
        self.assertEqual((table.count, table.player_counts[2]), (1, 1))
        fleet = result.contents
        self.assertEqual(fleet.bytes[2], 2)
        self.assertEqual(fleet.bytes[6] | (fleet.bytes[7] << 8), 17)
        self.assertEqual(fleet.bytes[8] | (fleet.bytes[9] << 8), 100)
        self.assertEqual(fleet.bytes[10] | (fleet.bytes[11] << 8), 0xffd8)
        self.assertEqual(fleet.orders.contents.count, 1)
        self.assertEqual(fleet.orders.contents.records[7], 0x11)

    def test_split_fleet_carries_route_flags_orders_and_cord(self):
        source_records = (C.c_ubyte * 36)(*range(36))
        source_orders = self.FleetOrders(2, 2, source_records)
        raw = (C.c_ubyte * 124)(*([0] * 124))
        raw[2], raw[6], raw[8], raw[9] = 3, 4, 20, 0
        raw[10], raw[11], raw[98] = 0xf0, 0xff, 9
        raw[4], raw[5], raw[96] = 0x34, 0x02, 7
        source = self.FleetState(raw, C.pointer(source_orders))
        table = self.FleetTable()
        result = C.POINTER(self.FleetState)()
        self.assertEqual(self.lib.stars_split_fleet(
            C.byref(table), C.byref(source), C.byref(result), None, None), 1)
        fleet = result.contents
        self.assertEqual(fleet.bytes[2], 3)
        self.assertEqual(fleet.bytes[4] | (fleet.bytes[5] << 8), 0x234)
        self.assertEqual(fleet.bytes[98], 9)
        self.assertEqual(fleet.orders.contents.count, 2)
        self.assertEqual(bytes(fleet.orders.contents.records[:36]),
                         bytes(source_records))

    def test_find_nearest_object_returns_kind_coordinates_and_index(self):
        planets = (self.NearestPoint * 2)(
            self.NearestPoint(0, 0), self.NearestPoint(100, 100))
        fleets = (self.NearestPoint * 1)(self.NearestPoint(12, 9))
        result = self.NearestResult()
        self.assertEqual(self.lib.stars_find_nearest_object(
            10, 10, 3, planets, 2, fleets, 1, None, 0, None, 0,
            C.byref(result)), 1)
        self.assertEqual((result.kind, result.fleet_index,
                          result.x, result.y), (2, 0, 12, 9))
        self.assertEqual(result.planet_index, -1)

    def test_lookup_fleet_copies_orders_and_merges_negative_identity(self):
        target_records = (C.c_ubyte * 36)(*range(36))
        target_orders = self.FleetOrders(1, 2, target_records)
        target_raw = (C.c_ubyte * 124)(*([4] * 124))
        target = self.FleetState(target_raw, C.pointer(target_orders))
        items = (C.POINTER(self.FleetState) * 1)(C.pointer(target))
        working_records = (C.c_ubyte * 36)(*range(30, 66))
        working_orders = self.FleetOrders(2, 2, working_records)
        working_raw = (C.c_ubyte * 124)(*([9] * 124))
        working_raw[0], working_raw[1] = 0, 0
        working = self.FleetState(working_raw, C.pointer(working_orders))
        self.assertEqual(self.lib.stars_lookup_fleet(
            items, 1, -1, C.byref(working), 0), 1)
        self.assertEqual(target.bytes[20], 9)
        self.assertEqual(target.orders.contents.count, 2)
        self.assertEqual(bytes(target.orders.contents.records[:36]),
                         bytes(working_records))
        self.assertEqual(self.lib.stars_lookup_fleet(
            items, 1, 0, C.byref(working), 0), 1)
        self.assertEqual(bytes(working.bytes), bytes(target.bytes))
        self.assertEqual(working.orders.contents.count, 2)

    def test_fleet_bomb_damage_accumulates_component_classes_across_owner_chain(self):
        components = (self.BombComponent * 3)(
            self.BombComponent(0x40, 0x0204, 10, 3),
            self.BombComponent(0x40, 0x0309, 0, 0),
            self.BombComponent(0x40, 0x0101, 0, 10))
        slots = (self.BombDesignSlot * 16)()
        slots[0] = self.BombDesignSlot(C.pointer(components[0]), 1)
        slots[1] = self.BombDesignSlot(C.pointer(components[1]), 1)
        slots[2] = self.BombDesignSlot(C.pointer(components[2]), 1)
        fleets = (self.BombFleet * 2)()
        fleets[0].player, fleets[0].planet, fleets[0].next_index = 1, 7, 1
        fleets[0].component_counts[0] = 1
        fleets[0].component_counts[1] = 1
        fleets[1].player, fleets[1].planet, fleets[1].next_index = 1, 7, -1
        fleets[1].component_counts[2] = 1
        damage = self.BombDamage()
        self.assertEqual(self.lib.stars_calc_fleet_bomb_damage(
            fleets, 2, 0, slots, 1000, 1.0, 1000.0,
            C.byref(damage)), 1)
        self.assertEqual((damage.people, damage.people_min, damage.building,
                          damage.terraforming, damage.people_smart,
                          damage.multiple), (6, 6, 20, 3, 990, 1))

    def test_split_all_fleet_creates_one_fleet_per_extra_ship(self):
        raw = (C.c_ubyte * 124)(*([0] * 124))
        raw[2], raw[6], raw[8], raw[10] = 2, 7, 100, 200
        raw[12], raw[13] = 3, 0
        source = self.FleetState(raw, None)
        table = self.FleetTable()
        created = C.c_uint16()
        self.assertEqual(self.lib.stars_split_all_fleet(
            C.byref(table), C.byref(source), None, None,
            C.byref(created)), 1)
        self.assertEqual((created.value, table.count), (2, 2))
        self.assertEqual(source.bytes[12] | (source.bytes[13] << 8), 1)
        for i in range(2):
            self.assertEqual(table.items[i].contents.bytes[12], 1)

    def test_calculate_eta_uses_warp_speed_and_stargate_status(self):
        points = (self.EtaPoint * 2)(
            self.EtaPoint(0, 0, 0x20), self.EtaPoint(3, 4, 0))
        eta, special = C.c_int16(), C.c_int16()
        self.assertEqual(self.lib.stars_calculate_eta(
            points, 2, 1, None, None, C.byref(eta), C.byref(special)), 1)
        self.assertEqual((eta.value, special.value), (2, 0))
        points[0].warp_code = 0xb0
        self.assertEqual(self.lib.stars_calculate_eta(
            points, 2, 1, None, None, C.byref(eta), C.byref(special)), 1)
        self.assertEqual((eta.value, special.value), (0, 0))

    def test_select_ours_at_object_prioritizes_own_fleet_planet_and_thing(self):
        planets = (self.NearestPoint * 1)(self.NearestPoint(0, 0))
        fleets = (self.SelectFleet * 2)(
            self.SelectFleet(5, 2, 10, 10),
            self.SelectFleet(7, 1, 20, 20))
        things = (self.SelectThing * 1)(self.SelectThing(10, 10, 1))
        selected = []
        callback_type = C.CFUNCTYPE(None, C.c_void_p, C.c_int16)
        @callback_type
        def select(_ctx, value): selected.append(value)
        callbacks = [C.cast(select, C.c_void_p)] * 3
        self.assertEqual(self.lib.stars_select_ours_at_object(
            20, 20, 1, planets, 1, fleets, 2, things, 1, 1,
            callbacks[0], callbacks[1], callbacks[2], None), 1)
        self.assertEqual(selected, [7])
        selected[:] = []
        self.assertEqual(self.lib.stars_select_ours_at_object(
            10, 10, 1, planets, 1, fleets, 2, things, 1, 1,
            callbacks[0], callbacks[1], callbacks[2], None), 1)
        self.assertEqual(selected, [0])
        selected[:] = []
        self.assertEqual(self.lib.stars_select_ours_at_object(
            -1, 0, 1, planets, 1, fleets, 2, things, 1, 1,
            callbacks[0], callbacks[1], callbacks[2], None), 1)
        self.assertEqual(selected, [0])

    def test_calc_player_score_combines_planets_traits_designs_and_fleets(self):
        planets = (self.ScorePlanet * 2)(
            self.ScorePlanet(1, 1001, 60, 1),
            self.ScorePlanet(1, 0, 30, 0))
        traits = (C.c_int16 * 6)(1, 4, 7, 10, 0, 3)
        designs = (self.ScoreDesign * 2)(
            self.ScoreDesign(0, 0), self.ScoreDesign(2000, 0))
        fleets = (self.ScoreFleet * 1)()
        fleets[0].owner, fleets[0].component_counts[0] = 1, 1
        fleets[0].component_counts[1] = 2
        summary = self.ScoreSummary()
        self.assertEqual(self.lib.stars_calc_player_score(
            1, planets, 2, traits, 0, designs, 2, fleets, 1, 5,
            C.byref(summary)), 56)
        self.assertEqual((summary.planet_count, summary.resources,
                          summary.starbase_count, summary.technology_levels,
                          tuple(summary.fleet_counts)),
                         (2, 90, 1, 25, (1, 0, 2)))

    def test_calc_player_score_preserves_uncapped_history_fleet_counts(self):
        planets = (self.ScorePlanet * 1)(
            self.ScorePlanet(0, 250, 38, 1))
        traits = (C.c_int16 * 6)(3, 3, 3, 3, 3, 3)
        designs = (self.ScoreDesign * 2)(
            self.ScoreDesign(0, 0), self.ScoreDesign(1000, 0))
        fleets = (self.ScoreFleet * 1)()
        fleets[0].owner = 0
        fleets[0].component_counts[0] = 4
        fleets[0].component_counts[1] = 2
        summary = self.ScoreSummary()
        self.assertEqual(self.lib.stars_calc_player_score(
            0, planets, 1, traits, 0, designs, 2, fleets, 1, 0,
            C.byref(summary)), 25)
        self.assertEqual((summary.planet_count, summary.resources,
                          summary.starbase_count, summary.technology_levels,
                          tuple(summary.fleet_counts)),
                         (1, 38, 1, 18, (4, 2, 0)))

        fleets[0].component_counts[1] = 1
        self.assertEqual(self.lib.stars_calc_player_score(
            0, planets, 1, traits, 0, designs, 2, fleets, 1, 0,
            C.byref(summary)), 25)
        self.assertEqual(tuple(summary.fleet_counts), (4, 1, 0))

    def test_pack_score_history_matches_retained_scorex_layout(self):
        planets = (self.ScorePlanet * 1)(
            self.ScorePlanet(0, 250, 38, 1))
        traits = (C.c_int16 * 6)(3, 3, 3, 3, 3, 3)
        designs = (self.ScoreDesign * 2)(
            self.ScoreDesign(0, 0), self.ScoreDesign(1000, 0))
        fleets = (self.ScoreFleet * 1)()
        fleets[0].owner = 0
        fleets[0].component_counts[0] = 4
        fleets[0].component_counts[1] = 2
        summary = self.ScoreSummary()
        record = (C.c_ubyte * 24)()
        self.assertEqual(self.lib.stars_calc_player_score(
            0, planets, 1, traits, 0, designs, 2, fleets, 1, 0,
            C.byref(summary)), 25)
        self.lib.stars_pack_score_history_record(
            C.byref(summary), 0x20, 1, record)
        self.assertEqual(bytes(record), bytes.fromhex(
            "200001001900000026000000010001000400020000001200"))

    def test_merge_all_fleet_accumulates_counts_weights_and_compacts_foreign_source(self):
        destination = self.FleetState((C.c_ubyte * 124)(*([0] * 124)), None)
        destination.bytes[0] = 1
        destination.bytes[12], destination.bytes[100] = 2, 4
        source = self.FleetState((C.c_ubyte * 124)(*([0] * 124)), None)
        source.bytes[0] = 2
        source.bytes[12], source.bytes[100] = 3, 6
        table = (C.POINTER(self.FleetState) * 2)(
            C.pointer(destination), C.pointer(source))
        fleet_table = self.FleetTable(table, 2, 2, (C.c_uint16 * 16)())
        ids = (C.c_int16 * 1)(1)
        deleted = C.c_uint16()
        self.assertEqual(self.lib.stars_merge_all_fleet(
            C.byref(fleet_table), C.byref(destination), ids, 1,
            C.byref(deleted)), 1)
        self.assertEqual((deleted.value, fleet_table.count,
                          destination.bytes[12], destination.bytes[100]),
                         (1, 1, 5, 10))

    def test_validate_waypoints_clamps_fleets_and_refreshes_or_invalidates_targets(self):
        fleets = (self.WaypointFleet * 1)(
            self.WaypointFleet(2, 1, 1100, 1100, 5, 0))
        things = (self.WaypointThing * 1)(
            self.WaypointThing(3, 1200, 1200, 0))
        waypoints = (self.WaypointRecord * 3)(
            self.WaypointRecord(1100, 1100, 0x0200, 9),
            self.WaypointRecord(1200, 1200, 0x0800, 3),
            self.WaypointRecord(0, 0, 0x0800, 8))
        self.assertEqual(self.lib.stars_validate_waypoints(
            fleets, 1, waypoints, 3, things, 1, 1000, 1), 1)
        self.assertEqual(waypoints[0].target_id, 2)
        self.assertEqual((waypoints[1].x, waypoints[1].y), (1200, 1200))
        self.assertEqual(waypoints[2].type_flags & 0x0f00, 0x0400)

    def test_change_population_handles_growth_decline_and_update_gate(self):
        same_input = self.PopulationState(0, 100, 250, 0)
        self.assertEqual(self.lib.stars_change_population(
            C.byref(same_input), 10000, 15, 0, 1), 37)
        self.assertEqual((same_input.population, same_input.fraction),
                         (287, 50))
        state = self.PopulationState(1, 5, 10000, 0)
        growth = self.lib.stars_change_population(
            C.byref(state), 20000, 10, 0, 1)
        self.assertGreater(growth, 0)
        self.assertGreater(state.population, 10000)
        state = self.PopulationState(1, -1, 10000, 0)
        decline = self.lib.stars_change_population(
            C.byref(state), 20000, 10, 0, 1)
        self.assertLess(decline, 0)
        self.assertEqual(state.population, 9990)
        state = self.PopulationState(-1, 5, 10000, 0)
        self.assertEqual(self.lib.stars_change_population(
            C.byref(state), 20000, 10, 0, 1), 0)

    def test_component_accessors_preserve_original_table_strides_and_dispatch_setup(self):
        shared = (C.c_ubyte * (0x26 + 0x8f * 3 + 1))()
        planet = (C.c_ubyte * (0x8f * 32 + 1))()
        table = (C.c_ubyte * (0x768 + 0x38 * 2 + 1))()
        self.assertEqual(C.addressof(
            self.lib.stars_huldef_sb_from_id(shared, 2).contents) -
            C.addressof(shared), 0x26 + 0x8f * 2)
        self.assertEqual(C.addressof(
            self.lib.stars_huldef_from_id(planet, shared, 31).contents) -
            C.addressof(planet), 0x8f * 31)
        self.assertEqual(C.addressof(
            self.lib.stars_huldef_from_id(planet, shared, 33).contents) -
            C.addressof(shared), 0x26 + 0x8f)
        self.assertEqual(C.addressof(
            self.lib.stars_engine_from_id(table, 2).contents) -
            C.addressof(table), 0x4e * 2)
        self.assertEqual(C.addressof(
            self.lib.stars_scanner_from_id(table, 1).contents) -
            C.addressof(table), 0x768 + 0x38)
        self.assertEqual(C.addressof(
            self.lib.stars_planetary_from_id(table, 1).contents) -
            C.addressof(table), 0x1b)
        self.assertTrue(self.lib.stars_shdef_t(table))
        self.assertTrue(self.lib.stars_shdef_sbt(table))
        part = (C.c_ubyte * 4)()
        seen = []
        callback_type = C.CFUNCTYPE(C.c_int16, C.c_void_p, PTR)
        @callback_type
        def lookup(_ctx, p):
            seen.append(bytes(p[:4]))
            return 7
        self.assertEqual(self.lib.stars_lookup_part_x(
            part, 0x1234, 0x5678, C.cast(lookup, C.c_void_p), None), 7)
        self.assertEqual(seen, [bytes([0x34, 0x12, 0x78, 0])])

    def test_best_planetary_scanner_retries_lower_items_and_tech_status_classifies(self):
        part = (C.c_ubyte * 4)()
        seen = []
        callback_type = C.CFUNCTYPE(C.c_int16, C.c_void_p, PTR)
        @callback_type
        def lookup(_ctx, p):
            seen.append(p[2])
            return 1 if p[2] == 5 else 0
        self.assertEqual(self.lib.stars_lookup_best_planetary_scanner(
            part, C.cast(lookup, C.c_void_p), None), 1)
        self.assertEqual((seen, part[0], part[1], part[2]),
                         ([8, 7, 6, 5], 0, 0x80, 5))
        current = (C.c_int16 * 6)(1, 1, 1, 1, 1, 1)
        required = (C.c_int16 * 6)(1, 2, 1, 1, 1, 1)
        self.assertEqual(self.lib.stars_tech_status(current, required, 1), 2)
        required[2] = 3
        self.assertEqual(self.lib.stars_tech_status(current, required, 1), 99)

    def test_thing_helpers_count_circle_moves_and_compact_fixed_records(self):
        planets = (self.NearestPoint * 3)(
            self.NearestPoint(0, 0), self.NearestPoint(3, 4),
            self.NearestPoint(10, 0))
        self.assertEqual(self.lib.stars_planets_in_circle(
            0, 0, 25, planets, 3), 2)
        self.assertEqual(self.lib.stars_pct_wormhole_moves(0), 0)
        self.assertEqual(self.lib.stars_pct_wormhole_moves(0x3fc), 6)
        records = (C.c_ubyte * 54)(*range(54))
        count = C.c_uint16(3)
        self.assertEqual(self.lib.stars_free_thing(records, C.byref(count), 1), 1)
        self.assertEqual(count.value, 2)
        self.assertEqual(bytes(records[18:36]), bytes(range(36, 54)))

    def test_new_thing_inserts_sorted_packed_identity_and_trader_mapping(self):
        records = (C.c_ubyte * 72)()
        count = C.c_uint16(1)
        records[0], records[1] = 0x00, 0x01
        index = C.c_uint16()
        self.assertEqual(self.lib.stars_new_thing(
            records, C.byref(count), 4, 0, 0, C.byref(index)), 1)
        self.assertEqual((count.value, index.value), (2, 0))
        identity = records[0] | (records[1] << 8)
        self.assertEqual(identity, 0)
        bits, goto_id = C.c_uint16(), C.c_uint16()
        self.assertEqual(self.lib.stars_give_trader_part(
            C.byref(bits), 0x100, C.byref(goto_id)), 0x10c)
        self.assertEqual((bits.value, goto_id.value), (0x100, 0xce1e))

    def test_validate_wormhole_position_preserves_collision_and_distance_masks(self):
        things = (self.WormholePoint * 2)(
            self.WormholePoint(2005, 2000, 7, 0x4000),
            self.WormholePoint(2020, 2000, 8, 0x4000))
        planets = (self.NearestPoint * 1)(self.NearestPoint(2002, 2000))
        self.assertEqual(self.lib.stars_validate_wormhole_position(
            2000, 2000, 7, 10, things, 2, planets, 1, None, 0), 13)
        self.assertEqual(self.lib.stars_validate_wormhole_position(
            100, 2000, 7, 10, things, 2, planets, 1, None, 0), 15)

    def test_draw_thing_gauge_selects_single_and_mineral_segments(self):
        source = self.ThingGaugeValues(100, (C.c_int32 * 3)(10, 20, 30))
        events = []
        callback_type = C.CFUNCTYPE(None, C.c_void_p, C.c_int16,
                                    C.POINTER(C.c_int32), C.c_int32, C.c_int16)
        @callback_type
        def draw(_ctx, segments, values, total, mode):
            events.append((segments, tuple(values[:3]), total, mode))
        self.assertEqual(self.lib.stars_draw_thing_gauge(
            C.byref(source), 1, C.cast(draw, C.c_void_p), None), 1)
        self.assertEqual(events[-1], (1, (20, 0, 0), 100, 1))
        self.assertEqual(self.lib.stars_draw_thing_gauge(
            C.byref(source), 5, C.cast(draw, C.c_void_p), None), 1)
        self.assertEqual(events[-1], (3, (10, 20, 30), 100, 5))

    def test_frame_resize_cursor_selects_exact_layout_zones(self):
        info = self.FrameResizeInfo(100, 50, 200)
        selection = C.c_int16()
        self.assertEqual(self.lib.stars_hcrs_from_frame_window_pt(
            101, 55, C.byref(info), 0, 11, 22, 33,
            C.byref(selection)), 33)
        self.assertEqual(selection.value, 3)
        self.assertEqual(self.lib.stars_hcrs_from_frame_window_pt(
            80, 55, C.byref(info), 0, 11, 22, 33,
            C.byref(selection)), 11)
        self.assertEqual(selection.value, 2)
        self.assertEqual(self.lib.stars_hcrs_from_frame_window_pt(
            101, 205, C.byref(info), 1, 11, 22, 33,
            C.byref(selection)), 33)
        self.assertEqual(selection.value, 5)

    def test_get_a_sub_menu_adds_zoom_offset_before_submenu_lookup(self):
        events = []
        menu_type = C.CFUNCTYPE(C.c_uint16, C.c_void_p, C.c_uint16)
        sub_type = C.CFUNCTYPE(C.c_uint16, C.c_void_p, C.c_uint16,
                               C.c_int16)
        @menu_type
        def get_menu(_ctx, hwnd):
            events.append(('menu', hwnd))
            return 40
        @sub_type
        def get_sub(_ctx, menu, index):
            events.append(('sub', menu, index))
            return 90
        self.assertEqual(self.lib.stars_get_a_sub_menu(
            7, 3, 5, 1, C.cast(get_menu, C.c_void_p),
            C.cast(get_sub, C.c_void_p), None), 90)
        self.assertEqual(events, [('menu', 7), ('sub', 40, 4)])

    def test_link_fleets_forms_circular_chain_for_equal_coordinates(self):
        fleets = (self.FleetLinkNode * 4)(
            self.FleetLinkNode(20, 10, 0, -1, -1),
            self.FleetLinkNode(5, 2, 0, -1, -1),
            self.FleetLinkNode(20, 10, 0, -1, -1),
            self.FleetLinkNode(20, 10, 1, -1, -1))
        self.lib.stars_link_fleets(fleets, 4)
        self.assertEqual((fleets[0].next_index, fleets[0].previous_index),
                         (2, 2))
        self.assertEqual((fleets[2].next_index, fleets[2].previous_index),
                         (0, 0))
        self.assertEqual((fleets[3].next_index, fleets[3].previous_index),
                         (3, 3))

    def test_draw_gauge_returns_total_and_emits_segment_geometry(self):
        rect = self.GaugeRect(10, 20, 110, 40)
        sizes = (C.c_int32 * 2)(3, 2)
        brushes = (C.c_uint16 * 2)(7, 8)
        fills = []
        callback = C.CFUNCTYPE(None, C.c_void_p, C.c_uint16,
                               C.POINTER(self.GaugeRect), C.c_uint16)
        @callback
        def fill(_ctx, _dc, p, brush):
            fills.append((p.contents.left, p.contents.right, brush))
        result = self.lib.stars_draw_gauge(
            None, 4, C.byref(rect), 2, sizes, brushes, 5,
            None, fill, 1, 9)
        self.assertEqual(result, 5)
        self.assertEqual(fills, [(11, 69, 7), (69, 109, 8)])

    def test_diagonal_text_out_shrinks_and_draws_centered_layout(self):
        rect = self.GaugeRect(10, 20, 210, 120)
        text = C.create_string_buffer(b'Fleet')
        events = []
        measure_type = C.CFUNCTYPE(None, C.c_void_p, C.c_int16, PTR,
                                   C.c_int16, C.POINTER(C.c_int16),
                                   C.POINTER(C.c_int16))
        draw_type = C.CFUNCTYPE(None, C.c_void_p, C.c_int16, C.c_int16,
                                C.c_int16, C.c_int16, C.c_int16, C.c_int16,
                                PTR, C.c_int16)
        @measure_type
        def measure(_ctx, font, _text, _length, out_width, out_height):
            out_width[0], out_height[0] = (-font) * 2, -font
        @draw_type
        def draw(_ctx, x, y, width, height, font, angle, _text, length):
            events.append((x, y, width, height, font, angle, length))
        self.assertEqual(self.lib.stars_diagonal_text_out(
            C.byref(rect), C.cast(text, PTR), 0, C.cast(measure, C.c_void_p),
            C.cast(draw, C.c_void_p), None), 1)
        self.assertEqual(events[0][-1], 5)
        self.assertGreaterEqual(events[0][0], 10)
        self.assertGreaterEqual(events[0][1], 20)

    def test_disk_serial_number_packs_present_and_missing_drive_records(self):
        label = C.create_string_buffer(b'AB')
        drives = (self.DiskIdentity * 2)(
            self.DiskIdentity(1, 0, 0, C.cast(label, PTR), 3),
            self.DiskIdentity(0, 0, 0, None, 5))
        output = (C.c_ubyte * 11)()
        self.assertEqual(self.lib.stars_get_disk_serial_number(
            drives, output), 11)
        self.assertEqual(bytes(output),
                         bytes([0x12, 0, 0, 0, 0, 0,
                                0xa5, 0xdc, 0xa6, 0x7a, 0x35]))

    def test_dib_from_bitmap_writes_header_palette_and_pixels(self):
        pixels = (C.c_ubyte * 8)(*range(8))
        palette = (C.c_ubyte * 4)(1, 2, 3, 0)
        bitmap = self.BitmapSource(3, 2, 1, 8, pixels, 8)
        output = (C.c_ubyte * 52)()
        self.assertEqual(self.lib.stars_dib_from_bitmap(
            C.byref(bitmap), 3, 0, palette, 4, output, 52), 52)
        self.assertEqual(bytes(output[0:4]), bytes([40, 0, 0, 0]))
        self.assertEqual(bytes(output[4:12]),
                         bytes([3, 0, 0, 0, 2, 0, 0, 0]))
        self.assertEqual(bytes(output[40:44]), bytes([1, 2, 3, 0]))
        self.assertEqual(bytes(output[44:52]), bytes(range(8)))

    def test_progress_dialog_dispatches_messages_and_initializes_state(self):
        state = ProgressState(0, 77)
        events = []
        paint_type = C.CFUNCTYPE(None, C.c_void_p, C.c_uint16, C.c_int16)
        erase_type = C.CFUNCTYPE(None, C.c_void_p, C.c_uint16, C.c_uint16)
        color_type = C.CFUNCTYPE(C.c_uint16, C.c_void_p, C.c_uint16,
                                 C.c_uint16)
        init_type = C.CFUNCTYPE(None, C.c_void_p, C.c_uint16, C.c_int16)
        @paint_type
        def paint(_ctx, hwnd, full): events.append(('paint', hwnd, full))
        @erase_type
        def erase(_ctx, dc, _rect): events.append(('erase', dc))
        @color_type
        def color(_ctx, dc, _face): events.append(('color', dc)); return 23
        @init_type
        def init(_ctx, hwnd, turn): events.append(('init', hwnd, turn))
        ops = self.ProgressDialogOps(C.cast(paint, C.c_void_p),
                                     C.cast(erase, C.c_void_p),
                                     C.cast(color, C.c_void_p),
                                     C.cast(init, C.c_void_p))
        self.assertEqual(self.lib.stars_progress_dialog(
            C.byref(state), 9, 0x110, 0, 4, C.byref(ops), None), 1)
        self.assertEqual((state.window, state.percent), (9, 0))
        self.assertEqual(self.lib.stars_progress_dialog(
            C.byref(state), 9, 0x0f, 0, 0, C.byref(ops), None), 1)
        self.assertEqual(self.lib.stars_progress_dialog(
            C.byref(state), 9, 0x14, 6, 0, C.byref(ops), None), 1)
        self.assertEqual(self.lib.stars_progress_dialog(
            C.byref(state), 9, 0x19, 7, 0, C.byref(ops), None), 23)
        self.assertEqual(events, [('init', 9, 0x965), ('paint', 9, 1),
                                  ('erase', 6), ('color', 7)])

    def test_draw_progress_gauge_emits_filled_and_face_regions(self):
        client = self.GaugeRect(0, 0, 100, 40)
        regions = []
        callback = C.CFUNCTYPE(None, C.c_void_p,
                               C.POINTER(self.GaugeRect), C.c_uint16)
        @callback
        def paint(_ctx, p, brush):
            regions.append((p.contents.left, p.contents.top,
                            p.contents.right, p.contents.bottom, brush))
        self.assertEqual(self.lib.stars_draw_progress_gauge(
            C.byref(client), 500, 1, paint, None), 1)
        self.assertEqual(regions, [(7, 27, 93, 28, 0x21),
                                   (7, 27, 8, 33, 0x21),
                                   (10, 30, 50, 30, 0x21),
                                   (50, 30, 90, 30, 0)])

    def test_random_seed_dialog_parses_seed_and_rejects_zero(self):
        text_value = C.create_string_buffer(b'12034')
        events = []
        text_type = C.CFUNCTYPE(C.c_void_p, C.c_void_p,
                                C.c_uint16, C.c_uint16)
        random_type = C.CFUNCTYPE(None, C.c_void_p, C.c_uint32)
        end_type = C.CFUNCTYPE(None, C.c_void_p, C.c_uint16, C.c_int16)
        @text_type
        def text(_ctx, _hwnd, _control): return C.addressof(text_value)
        @random_type
        def randomize(_ctx, seed): events.append(('seed', seed))
        @end_type
        def end(_ctx, hwnd, result): events.append(('end', hwnd, result))
        ops = self.RandomSeedOps(C.cast(text, C.c_void_p),
                                 C.cast(randomize, C.c_void_p),
                                 C.cast(end, C.c_void_p))
        self.assertEqual(self.lib.stars_random_seed_dialog(
            4, 0x111, 1, 0, C.byref(ops), None), 1)
        self.assertEqual(events, [('seed', 12034), ('end', 4, 1)])
        text_value.value = b'0'
        self.assertEqual(self.lib.stars_random_seed_dialog(
            4, 0x111, 1, 0, C.byref(ops), None), 1)
        self.assertEqual(events, [('seed', 12034), ('end', 4, 1)])

    def test_new_password_dialog_requires_matching_salts_and_updates_state(self):
        first = C.create_string_buffer(b'abcd')
        second = C.create_string_buffer(b'abcd')
        events = []
        text_type = C.CFUNCTYPE(C.c_void_p, C.c_void_p,
                                C.c_uint16, C.c_uint16)
        mismatch_type = C.CFUNCTYPE(None, C.c_void_p, C.c_uint16)
        write_type = C.CFUNCTYPE(C.c_int16, C.c_void_p, C.c_uint32)
        end_type = C.CFUNCTYPE(None, C.c_void_p, C.c_uint16, C.c_int16)
        @text_type
        def text(_ctx, _hwnd, control):
            return C.addressof(first if control == 0x10c else second)
        @mismatch_type
        def mismatch(_ctx, hwnd): events.append(('mismatch', hwnd))
        @write_type
        def write(_ctx, salt): events.append(('write', salt)); return 1
        @end_type
        def end(_ctx, hwnd, result): events.append(('end', hwnd, result))
        ops = self.NewPasswordOps(C.cast(text, C.c_void_p),
                                  C.cast(mismatch, C.c_void_p),
                                  C.cast(write, C.c_void_p),
                                  C.cast(end, C.c_void_p))
        current, last = C.c_uint32(1), C.c_uint32(2)
        self.assertEqual(self.lib.stars_new_password_dialog(
            5, 0x111, 1, 0, -1, C.byref(current), C.byref(last),
            C.byref(ops), None), 1)
        self.assertNotEqual(current.value, 1)
        self.assertEqual(events[-2:], [('write', current.value), ('end', 5, 1)])
        second.value = b'wxyz'
        self.assertEqual(self.lib.stars_new_password_dialog(
            5, 0x111, 1, 0, 2, C.byref(current), C.byref(last),
            C.byref(ops), None), 0)
        self.assertEqual(events[-1], ('mismatch', 5))


if __name__ == '__main__':
    unittest.main()
