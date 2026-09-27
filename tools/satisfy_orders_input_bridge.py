"""Export source-initialized Gate 1 inputs to the strict-C89 coordinator.

Only InputReader results enter this bridge. It never reads expected turn-one
outputs. C aggregate initializers preserve host independence (no ctypes ABI
padding or native pointer bytes are exported).
"""
import ctypes as C


def initializer(value):
    if isinstance(value, C.Structure):
        return initializer([getattr(value, name) for name, _ in value._fields_])
    if isinstance(value, (bytes, bytearray)):
        return initializer(list(value) or [0])
    if isinstance(value, (list, tuple, C.Array)):
        return '{' + ','.join(initializer(v) for v in value) + '}'
    return str(value)


DECLARATIONS = '''
/* Complete mutable input views. File views remain separate: a client load
 * has discovery/first-year state that does not belong to the host view. */
typedef struct Soe3InputFleet {
    StarsU16 id_word;
    StarsI16 owner;
    StarsReadFleetState state;
} Soe3InputFleet;
typedef struct Soe3InputView {
    StarsU8 player_present[2];
    StarsU8 players[2][192];
    StarsU8 ship_designs[2][2352];
    StarsU8 starbase_designs[2][1470];
    StarsU8 design_present[2][32];
    StarsU8 battle_plan_present[2][16];
    StarsU8 battle_plans[2][16][36];
    StarsU16 planet_count;
    StarsReadPlanetState planets[32];
    StarsU16 fleet_count;
    Soe3InputFleet fleets[10];
    StarsU16 thing_count;
    StarsU8 things[4][18];
    StarsU16 message_bytes;
    StarsU16 message_count;
    StarsI16 current_message;
    StarsU8 message_sent[64];
    StarsU8 messages[512];
} Soe3InputView;
typedef struct Soe3CompleteInput {
    StarsU8 game[64];
    StarsU8 bof[16];
    StarsU16 coordinate_count;
    StarsI16 coordinates[32][3];
    Soe3InputView views[3]; /* HST, M1, M2 */
} Soe3CompleteInput;
'''


def view_values(loaded):
    if (set(loaded.players) - {0, 1} or len(loaded.planets) > 32 or
            len(loaded.fleets) > 10 or len(loaded.things) > 4 or
            loaded.messages is None or len(loaded.messages.buffer) > 512):
        raise ValueError('Input exceeds coordinator bridge capacity')
    if set(loaded.runtime_players) != set(loaded.players):
        raise ValueError('Player counts have not been finalized')
    players = [loaded.runtime_players.get(owner, bytes(192)) for owner in range(2)]
    banks = [[loaded.design_banks.get((owner, starbase), bytes(size))
              for owner in range(2)] for starbase, size in ((False, 2352), (True, 1470))]
    design_present = [[int((owner, slot) in loaded.designs) for slot in range(32)]
                      for owner in range(2)]
    plan_present = [[int((owner, slot) in loaded.battle_plans) for slot in range(16)]
                    for owner in range(2)]
    plans = [[loaded.battle_plans.get((owner, slot), bytes(36)) for slot in range(16)]
             for owner in range(2)]
    # Empty arrays use a typed zero structure, avoiding ambiguous nested
    # aggregate braces under -Werror=missing-braces.
    from tools.stars_planet_input import PlanetState
    from tools.stars_fleet_input import FleetState
    planets = list(loaded.planets.values()) or [PlanetState()]
    fleets = [[f.id_word, f.owner, f.state] for f in loaded.fleets.values()]
    return [[int(owner in loaded.players) for owner in range(2)], players,
            *banks, design_present, plan_present, plans, len(loaded.planets),
            planets, len(loaded.fleets), fleets or [[0, 0, FleetState()]],
            len(loaded.things), loaded.things or [bytes(18)],
            len(loaded.messages.buffer), len(loaded.messages.decoded),
            loaded.messages.current_message, loaded.messages.sent_bits,
            loaded.messages.buffer or b'\0']


def complete_input_header(runs):
    rows = []
    for label in ('control', 'action'):
        files = runs[label]
        game = next(r.body for r in files['XY'].records if r.kind == 7)
        bof = next(r.body for r in files['HST'].records if r.kind == 8)
        coordinates = files['XY'].coordinates
        if len(game) != 64 or len(bof) != 16 or len(coordinates) > 32:
            raise ValueError('Invalid game metadata for coordinator bridge')
        rows.append(initializer([game, bof, len(coordinates), coordinates,
            [view_values(files[suffix]) for suffix in ('HST', 'M1', 'M2')]]))
    return DECLARATIONS + '\nstatic const Soe3CompleteInput soe3_inputs[2] = {\n' + ',\n'.join(rows) + '\n};\n'
