"""Initialize the retained planet records through source FReadPlanet.

Fresh HST and player-file views have separate planet tables. No history file
is supplied by the pinned input manifests, so preinitialized/history are zero.
"""
import ctypes as C
import struct

U8, U16, I16, U32 = C.c_uint8, C.c_uint16, C.c_int16, C.c_uint32
P8 = C.POINTER(U8)


class PlanetInput(C.Structure):
    _fields_ = [(name, U16) for name in (
        'id_player_word', 'flags_word', 'header_flags', 'game_turn',
        'previous_flags', 'previous_turn', 'history', 'preinitialized')]
    _fields_ += [('payload', P8), ('payload_size', U16)]


class PlanetState(C.Structure):
    _fields_ = [('id', I16), ('player', I16), ('flags', U16), ('turn', U16)]
    _fields_ += [(name, U8 * 3) for name in (
        'mineral_levels', 'mineral_concentrations', 'environment_original', 'environment')]
    _fields_ += [('routing_word', U16), ('minerals', U32 * 4),
                ('opaque_20', U32 * 2), ('opaque_44', U32), ('opaque_48', U16)]


RACE = C.CFUNCTYPE(I16, C.c_void_p, I16, I16)
VALUE = C.CFUNCTYPE(I16, C.c_void_p, C.POINTER(PlanetState), I16)
GROWTH = C.CFUNCTYPE(I16, C.c_void_p, I16)
SEND = C.CFUNCTYPE(None, C.c_void_p, I16, I16, I16, I16, I16)


class PlanetOps(C.Structure):
    _fields_ = [('race_stat', RACE), ('optimal_value', VALUE),
                ('desirability', VALUE), ('true_max_growth', GROWTH),
                ('send_message', SEND)]


def validate_span(record):
    """Bound the complete payload, including tails the void APIs can't report."""
    body = record.body
    if len(body) < 4:
        raise ValueError('Truncated planet header')
    identity, flags = struct.unpack_from('<HH', body)
    owner, detail, cursor = identity >> 11, flags & 127, 4

    def take(size):
        nonlocal cursor
        if cursor + size > len(body):
            raise ValueError('Truncated planet payload')
        span = body[cursor:cursor + size]
        cursor += size
        return span

    if detail >= 3:
        selector = take(1)[0]
        for axis in range(3):
            width = (selector >> (axis * 2)) & 3
            if width > 1:
                raise ValueError('Invalid planet mineral-level width')
            take(width)
        take(6)
        if flags & 0x400:
            take(3)
        if owner != 31:
            take(2)
        if detail > 3 and flags & 0x2000:
            selector = take(1)[0]
            take(sum((0, 1, 2, 4)[(selector >> (2 * i)) & 3] for i in range(4)))
    if detail > 3 and record.kind == 13:
        if flags & 0x800:
            take(8)
        if owner != 31:
            if flags & 0x200:
                take(4)
            if flags & 0x4000:
                take(2)
    elif flags & 0x200 and owner != 31:
        take(1)
    if cursor != len(body):
        raise ValueError('Unconsumed planet tail')


class PlanetReader:
    def __init__(self, inspector):
        self.lib = inspector.lib
        self.lib.stars_read_planet.argtypes = [C.POINTER(PlanetInput),
            C.POINTER(PlanetState), I16, C.POINTER(PlanetOps), C.c_void_p]
        self.lib.stars_read_planet.restype = I16
        self.lib.stars_planet_desirability.argtypes = [P8, U16, I16]
        self.lib.stars_planet_desirability.restype = I16
        self.lib.stars_get_race_stat.argtypes = [P8, U16, I16]
        self.lib.stars_get_race_stat.restype = I16
        self.lib.stars_true_max_growth.argtypes = [P8, I16]
        self.lib.stars_true_max_growth.restype = I16

    def read_file(self, loaded, loading_player, planet_count):
        memory = (U8 * 65536)()
        for owner, player in loaded.players.items():
            at = 0x59a2 + owner * 192
            memory[at:at + 192] = player
        planets, messages, errors = {}, [], []

        def race(_, player, which):
            return self.lib.stars_get_race_stat(memory, 0x59a2 + player * 192, which)

        def desirability(_, state, player):
            memory[12:15] = state.contents.environment
            return self.lib.stars_planet_desirability(memory, 0, player)

        def optimal(_, state, player):
            # None of the pinned views takes this branch. Fail explicitly if
            # another input needs the terraform adapter, rather than guessing.
            errors.append('Planet requires unresolved optimal-terraform callback')
            return 0

        ops = PlanetOps(RACE(race), VALUE(optimal), VALUE(desirability),
            GROWTH(lambda _, player: self.lib.stars_true_max_growth(memory, player)),
            SEND(lambda _, *args: messages.append(tuple(args))))
        for record in loaded.records:
            if record.kind not in (13, 14):
                continue
            validate_span(record)
            identity, flags = struct.unpack_from('<HH', record.body)
            planet_id = identity & 2047
            if planet_id >= planet_count or planet_id in planets:
                raise ValueError('Invalid or duplicate planet identity')
            payload = (U8 * max(1, len(record.body) - 4))()
            payload[:len(record.body) - 4] = record.body[4:]
            source = PlanetInput(id_player_word=identity, flags_word=flags,
                header_flags=(record.kind << 10) | len(record.body),
                game_turn=record.turn, payload=payload,
                payload_size=len(record.body) - 4)
            state = PlanetState()
            if not self.lib.stars_read_planet(C.byref(source), C.byref(state),
                    loading_player, C.byref(ops), None):
                raise ValueError('Source planet initialization rejected input')
            if errors:
                raise ValueError(errors[0])
            planets[planet_id] = state
        return planets, tuple(messages)
