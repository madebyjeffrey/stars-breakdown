"""Read every byte of the pinned SO-E3 inputs through reconstructed record I/O.

This is the input boundary for the full-turn loader. Players, designs, battle
plans, things, fleets/orders, planets and messages are initialized and bound
to the coordinator. Gate 1 is closed for the pinned pair; SO-E3 remains open.
No turn-one output or expected-output fixture is read here.
"""
from dataclasses import dataclass, field
import ctypes as C
import hashlib
import json
from pathlib import Path
import struct

from tools.race_file_compare import Inspector
from tools.stars_design_input import DesignReader
from tools.stars_fleet_input import FleetReader
from tools.stars_planet_input import PlanetReader
from tools.stars_message_input import MessageReader

ROOT = Path(__file__).resolve().parents[1]
PTR = C.POINTER(C.c_ubyte)
READ = C.CFUNCTYPE(None, C.c_void_p, PTR, C.c_uint16)


@dataclass(frozen=True)
class InputRecord:
    offset: int
    kind: int
    body: bytes
    turn: int


@dataclass
class InputFile:
    path: Path
    sha256: str
    records: list
    players: dict
    coordinates: tuple = ()
    designs: dict = field(default_factory=dict)
    design_banks: dict = field(default_factory=dict)
    battle_plans: dict = field(default_factory=dict)
    things: tuple = ()
    fleets: dict = field(default_factory=dict)
    planets: dict = field(default_factory=dict)
    planet_messages: tuple = ()
    messages: object = None
    runtime_players: dict = field(default_factory=dict)


class InputReader:
    def __init__(self, inspector=None):
        self.inspector = inspector or Inspector()
        self.inspector.lib.stars_read_record.argtypes = [PTR, READ, C.c_void_p]
        self.inspector.lib.stars_read_record.restype = None
        self.design_reader = DesignReader(self.inspector)
        self.fleet_reader = FleetReader(self.inspector)
        self.planet_reader = PlanetReader(self.inspector)
        self.message_reader = MessageReader(self.inspector)
        self.inspector.lib.stars_unpack_battle_plan.argtypes = [PTR, PTR, PTR, C.c_int16]
        self.inspector.lib.stars_unpack_battle_plan.restype = None

    def read(self, path, expected_hash, xy=False):
        path = Path(path)
        raw = path.read_bytes()
        digest = hashlib.sha256(raw).hexdigest()
        if digest != expected_hash:
            raise ValueError(f"Input hash mismatch: {path}")
        data = bytearray(self.inspector.initial)
        memory = (C.c_ubyte * 65536).from_buffer(data)
        records, players, coordinates = [], {}, []
        cursor, turn, version = 0, None, None
        while cursor < len(raw):
            start = cursor
            if cursor + 2 > len(raw):
                raise ValueError(f"Truncated header at {cursor}")
            header, = struct.unpack_from('<H', raw, cursor)
            kind, length = header >> 10, header & 1023
            end = cursor + 2 + length
            if end > len(raw):
                raise ValueError(f"Truncated body at {cursor}")
            # Bound the transfer before entering the original void I/O API.
            position = [cursor]
            errors = []

            def transfer(_, output, size):
                at = position[0]
                if at + size > end:
                    errors.append('record reader overran its framed input')
                    C.memset(output, 0, size)
                    return
                C.memmove(output, raw[at:at + size], size)
                position[0] += size

            callback = READ(transfer)
            self.inspector.lib.stars_read_record(memory, callback, None)
            if errors or position[0] != end:
                raise ValueError(f"Record transfer mismatch at {cursor}: {errors}")
            body = bytes(data[0x4b98:0x4b98 + length])
            if kind == 8:
                if length != 16 or body[:4] != b'J3J3':
                    raise ValueError('Invalid BOF')
                version, turn = struct.unpack_from('<HH', body, 8)
                if version != 0x2a60:
                    raise ValueError('Unsupported input version')
                struct.pack_into('<H', data, 0x750, version)
            elif turn is None:
                raise ValueError('Record before BOF')
            records.append(InputRecord(start, kind, body, turn))
            cursor = end
            if kind == 6:
                self.inspector.bounded_player(body)
                player = body[0]
                if player in players:
                    raise ValueError(f'Duplicate player {player}')
                # Input, destination and the stream scratch buffer are disjoint.
                data[0x3500:0x3500 + length] = body
                self.inspector.lib.stars_read_player_record(
                    memory, self.inspector.strings, 0x3000, 0x3500)
                players[player] = bytes(data[0x3000:0x30c0])
            if xy and kind == 7:
                if len(body) != 64 or coordinates:
                    raise ValueError('Invalid XY game record')
                count, = struct.unpack_from('<H', body, 10)
                if cursor + count * 4 > len(raw):
                    raise ValueError('Truncated XY coordinates')
                x = 1000
                for i in range(count):
                    packed, = struct.unpack_from('<I', raw, cursor + i * 4)
                    x += packed & 1023
                    coordinates.append((x, (packed >> 10) & 4095,
                                        (packed >> 22) & 1023))
                cursor += count * 4
        if not records or records[0].kind != 8 or records[-1].kind != 0:
            raise ValueError('Incomplete input record stream')
        if xy and [r.kind for r in records] != [8, 7, 0]:
            raise ValueError('Unexpected XY sections')
        # CreateUniverse ends XY with cPlayer; save-game streams end with
        # game.turn. Both markers are plaintext, outside the XOR stream.
        end_value = int.from_bytes(records[1].body[8:10], 'little') if xy else turn
        if len(records[-1].body) != 2 or int.from_bytes(records[-1].body, 'little') != end_value:
            raise ValueError('EOF marker disagrees with game metadata')
        result = InputFile(path, digest, records, players, tuple(coordinates))
        result.designs, result.design_banks = self.design_reader.read_file(result)
        self.initialize_battle_plans_and_things(result)
        return result

    def initialize_battle_plans_and_things(self, result):
        data = (C.c_ubyte * 65536).from_buffer_copy(self.inspector.initial)
        thing_records = []
        for record in result.records:
            if record.kind == 43:
                thing_records.append(record.body)
            if record.kind != 30:
                continue
            body = record.body
            if len(body) < 6 or body[4] > 32:
                raise ValueError('Invalid battle-plan name span')
            if body[4]:
                if len(body) != 5 + body[4]:
                    raise ValueError('Truncated compressed battle-plan name')
            elif body[-1] != 0 or len(body) > 37:
                raise ValueError('Invalid literal battle-plan name')
            key = (body[0] & 15, body[0] >> 4)
            if key in result.battle_plans or key[0] not in result.players:
                raise ValueError('Duplicate or unowned battle plan')
            # Original routine copies a 32-byte compressed scratch span.
            wire = (C.c_ubyte * 64).from_buffer_copy(body.ljust(64, b'\0'))
            output = (C.c_ubyte * 37)()
            self.inspector.lib.stars_unpack_battle_plan(data, wire, output, key[1])
            result.battle_plans[key] = bytes(output[:36])
        if thing_records:
            if len(thing_records[0]) != 2:
                raise ValueError('Missing thing-count prefix')
            count = int.from_bytes(thing_records[0], 'little')
            things = thing_records[1:]
            if count != len(things) or any(len(body) != 18 for body in things):
                raise ValueError('Thing table size disagrees with count prefix')
            if len({body[:2] for body in things}) != count:
                raise ValueError('Duplicate thing identity')
            # FLoadGame copies THING, then stamps the current game turn.
            turn = result.records[0].turn.to_bytes(2, 'little')
            result.things = tuple(body[:16] + turn for body in things)

    def load_run(self, run_directory):
        directory = Path(run_directory)
        manifest = json.loads((directory / 'RUN.JSON').read_text())
        if set(manifest['pre_run_absent_files']) != {'CLEAN.X2', 'CLEAN.H2'}:
            raise ValueError('Clean starting log/history absence is not pinned')
        result = {}
        for suffix in ('XY', 'HST', 'M1', 'M2'):
            name = 'CLEAN.' + suffix
            path = directory / name if suffix == 'XY' else directory / 'BACKUP' / name
            expected = manifest['input_files'][name]
            if path.stat().st_size != expected['bytes']:
                raise ValueError(f'Input size mismatch: {path}')
            result[suffix] = self.read(path, expected['sha256'], xy=suffix == 'XY')
        identities = {r.body[4:12] for f in result.values()
                      for r in f.records if r.kind == 8}
        if len(identities) != 1:
            raise ValueError('Input game/version/turn identities differ')
        for suffix in ('HST', 'M1', 'M2'):
            result[suffix].fleets = self.fleet_reader.read_file(
                result[suffix], result['XY'].coordinates)
            loading_player = -1 if suffix == 'HST' else int(suffix[1:]) - 1
            result[suffix].planets, result[suffix].planet_messages = self.planet_reader.read_file(
                result[suffix], loading_player, len(result['XY'].coordinates))
            result[suffix].messages = self.message_reader.read_file(result[suffix])
            self.finalize_player_counts(result[suffix])
        return result

    @staticmethod
    def finalize_player_counts(loaded):
        # FLoadGame clears saved counts and rebuilds them from initialized
        # objects. HST's saved cPlanet=32/0 describes stream distribution,
        # not final ownership. Preserve ReadRtPlr output for wire auditing.
        for owner, original in loaded.players.items():
            player = bytearray(original)
            planets = sum(p.player == owner for p in loaded.planets.values())
            fleets = sum(f.owner == owner for f in loaded.fleets.values())
            ships = sum(key[0] == owner and key[1] < 16 for key in loaded.designs)
            starbases = sum(key[0] == owner and key[1] >= 16 for key in loaded.designs)
            if fleets > 4095 or ships > 16 or starbases > 10:
                raise ValueError('Runtime player object count exceeds packed capacity')
            struct.pack_into('<H', player, 2, planets)
            struct.pack_into('<H', player, 4, (starbases << 12) | fleets)
            player[1] = ships
            loaded.runtime_players[owner] = bytes(player)


def load_pair(inspector=None):
    reader = InputReader(inspector)
    return {label: reader.load_run(ROOT / f'runs/vmctl-sye3k{index}')
            for index, label in enumerate(('control', 'action'))}


def record_runtime_references(loaded):
    """Account for each consumed record by its concrete runtime destination.

    This is an audit of the loader result, independent of the report's kind
    labels. Missing objects, unclaimed orders, and extra records fail it.
    """
    references, fleets = {}, {}
    for key, fleet in loaded.fleets.items():
        for offset in fleet.record_offsets:
            if offset in fleets:
                raise ValueError('Input record assigned to multiple fleets')
            fleets[offset] = key
    designs = iter(loaded.designs)
    thing_index = -1
    for record in loaded.records:
        body, kind = record.body, record.kind
        if kind == 8:
            destination = 'record_decoder.bof'
        elif kind == 0:
            destination = 'record_decoder.end_marker'
        elif kind == 7:
            if len(body) != 64 or not loaded.coordinates:
                raise ValueError('Game record has no initialized coordinate section')
            destination = 'game_and_coordinates'
        elif kind == 6:
            if body[0] not in loaded.runtime_players:
                raise ValueError('Player record has no finalized runtime player')
            destination = f'runtime_players[{body[0]}]'
        elif kind in (13, 14):
            identity = int.from_bytes(body[:2], 'little') & 2047
            if identity not in loaded.planets:
                raise ValueError('Planet record has no initialized planet')
            destination = f'planets[{identity}]'
        elif kind in (16, 19, 20):
            if record.offset not in fleets:
                raise ValueError('Fleet/order record has no initialized fleet')
            destination = f'fleets{fleets[record.offset]}'
        elif kind == 26:
            key = next(designs, None)
            if key is None or key[1] != (int.from_bytes(body[:2], 'little') >> 10) & 31:
                raise ValueError('Design record has no matching initialized slot')
            destination = f'designs{key}'
        elif kind == 30:
            key = (body[0] & 15, body[0] >> 4)
            if key not in loaded.battle_plans:
                raise ValueError('Battle plan has no initialized slot')
            destination = f'battle_plans{key}'
        elif kind == 43:
            if thing_index == -1:
                if int.from_bytes(body, 'little') != len(loaded.things):
                    raise ValueError('Thing count does not match runtime table')
                destination = 'thing_count'
            else:
                if thing_index >= len(loaded.things) or body[:16] != loaded.things[thing_index][:16]:
                    raise ValueError('Thing record has no matching runtime object')
                destination = f'things[{thing_index}]'
            thing_index += 1
        elif kind == 12:
            if loaded.messages is None or record.offset not in loaded.messages.record_offsets:
                raise ValueError('Message record has no initialized message table')
            destination = 'messages'
        else:
            raise ValueError(f'Unaccounted input record kind {kind}')
        if record.offset in references:
            raise ValueError('Duplicate record offset')
        references[record.offset] = destination
    if next(designs, None) is not None:
        raise ValueError('Initialized design has no source record')
    return references


def corpus_report(runs):
    groups = {0: 'metadata', 6: 'player', 7: 'metadata', 8: 'metadata',
              26: 'design', 30: 'battle_plan', 43: 'thing'}
    digest = lambda data: hashlib.sha256(data).hexdigest()
    report = {'gate': 'SO-E3 Gate 1', 'complete': True,
              'coordinator_integration_complete': True,
              'closure_audit': 'analysis/compat/satisfy-orders-e3-gate1-audit.md',
              'original_x86_player_comparison': 'Not performed by inventory generation; see EVIDENCE.md for test results',
              'uninitialized_object_records': 0, 'runs': {}}
    for label, files in runs.items():
        report['runs'][label] = {}
        for suffix, loaded in files.items():
            records = []
            references = record_runtime_references(loaded)
            fleet_offsets = {offset for fleet in loaded.fleets.values()
                             for offset in fleet.record_offsets}
            for record in loaded.records:
                group = groups.get(record.kind, 'uninitialized')
                if record.offset in fleet_offsets:
                    group = 'fleet_order'
                if record.kind in (13, 14) and len(record.body) >= 2 and (
                        int.from_bytes(record.body[:2], 'little') & 2047) in loaded.planets:
                    group = 'planet'
                if loaded.messages is not None and record.offset in loaded.messages.record_offsets:
                    group = 'message'
                report['uninitialized_object_records'] += group == 'uninitialized'
                records.append({'offset': record.offset, 'kind': record.kind,
                                'length': len(record.body), 'turn': record.turn,
                                'sha256': digest(record.body), 'runtime_group': group,
                                'runtime_reference': references[record.offset]})
            report['runs'][label][suffix] = {
                'path': str(loaded.path.relative_to(ROOT)), 'sha256': loaded.sha256,
                'coordinates': list(loaded.coordinates), 'records': records,
                'players': {str(k): {'bytes': len(v), 'sha256': digest(v)}
                            for k, v in loaded.players.items()},
                'runtime_players': {str(k): {'bytes': len(v), 'sha256': digest(v),
                    'owned_planets': int.from_bytes(v[2:4], 'little'),
                    'fleet_count': int.from_bytes(v[4:6], 'little') & 4095,
                    'ship_design_count': v[1]}
                    for k, v in loaded.runtime_players.items()},
                'designs': {f'{owner}:{slot}': {'bytes': len(v), 'sha256': digest(v),
                              'name': v[8:40].split(b'\0')[0].decode('cp1252')}
                            for (owner, slot), v in loaded.designs.items()},
                'design_banks': {f'{owner}:{"starbase" if starbase else "ship"}':
                                {'bytes': len(v), 'sha256': digest(v)}
                                for (owner, starbase), v in loaded.design_banks.items()},
                'battle_plans': {f'{owner}:{slot}': v.hex()
                                 for (owner, slot), v in loaded.battle_plans.items()},
                'things': [v.hex() for v in loaded.things],
                'planets': {str(identity): {
                    name: list(getattr(state, name)) if isinstance(getattr(state, name), C.Array)
                    else getattr(state, name)
                    for name, _ in state._fields_}
                    for identity, state in loaded.planets.items()},
                'planet_messages': list(loaded.planet_messages),
                'messages': None if loaded.messages is None else {
                    'buffer': loaded.messages.buffer.hex(),
                    'sent_bits': loaded.messages.sent_bits.hex(),
                    'current_message': loaded.messages.current_message,
                    'decoded': loaded.messages.decoded,
                    'record_offsets': loaded.messages.record_offsets},
                'fleets': {f'{owner}:{identity}': {
                    'id_word': fleet.id_word,
                    'record_offsets': list(fleet.record_offsets),
                    'flags': fleet.state.fleet_flags,
                    'planet': fleet.state.planet,
                    'point': [fleet.state.x, fleet.state.y],
                    'ship_counts_and_damage': list(fleet.state.ship_counts),
                    'cargo': list(fleet.state.minerals),
                    'overlap_116': list(fleet.state.overlap_116),
                    'overlap_44': list(fleet.state.overlap_44),
                    'design_plan': fleet.state.design_plan,
                    'order_count': fleet.state.order_count,
                    'orders': bytes(fleet.state.orders).hex(),
                    'name': fleet.state.name.decode('cp1252'),
                } for (owner, identity), fleet in loaded.fleets.items()},
            }
    return report


if __name__ == '__main__':
    import argparse
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('--output', type=Path, required=True)
    args = parser.parse_args()
    report = corpus_report(load_pair())
    args.output.write_text(json.dumps(report, indent=2) + '\n')
    print(f"Gate 1 input audit: {report['uninitialized_object_records']} uninitialized "
          f"object records; coordinator verification: {report['closure_audit']}; report: {args.output}")
