"""Bind saved design records to source readers and original executable parts."""
import ctypes as C
import struct
from pathlib import Path
from tools.ne_layout import NE

U8 = C.c_ubyte
PTR = C.POINTER(U8)
RACE = C.CFUNCTYPE(C.c_int16, C.c_void_p, C.c_int16, C.c_int16)
HULL = C.CFUNCTYPE(C.c_uint16, C.c_void_p, C.c_uint16, PTR, C.c_uint16)
PART = C.CFUNCTYPE(C.c_int16, C.c_void_p, C.c_uint16, C.c_uint16, PTR, C.c_uint16)
COST = C.CFUNCTYPE(None, C.c_void_p, PTR)
NAME = C.CFUNCTYPE(C.c_int16, C.c_void_p, PTR, C.c_uint16, PTR, C.POINTER(C.c_uint16))


class Ops(C.Structure):
    _fields_ = [('hull', HULL), ('part', PART), ('cost', COST), ('name', NAME)]


class Reprice(C.Structure):
    _fields_ = [('segment', PTR), ('size', C.c_uint16),
                ('technology', C.POINTER(C.c_byte)), ('flags', C.POINTER(C.c_uint16)),
                ('race_stat', RACE), ('race_bit', RACE), ('context', C.c_void_p),
                ('player', C.c_int16)]


class Lookup(C.Structure):
    _fields_ = [('group', C.c_uint16), ('item', C.c_uint16), ('player', C.c_int16),
                ('component', C.c_uint32), ('segment', C.c_uint16),
                ('current_cs', C.c_uint16), ('base', C.c_uint16),
                ('offset', C.c_uint16), ('stride', C.c_uint16)] + [
                    (name, C.c_void_p) for name in
                    ('resolve', 'race_stat', 'race_bit', 'hidden', 'tech', 'armor', 'context')]


class DesignReader:
    def __init__(self, inspector):
        self.inspector = inspector
        self.lib = inspector.lib
        root = Path(__file__).resolve().parents[1]
        self.segment_bytes = bytes(NE(root / 'target/stars.exe').raw[1])
        self.segment = (U8 * len(self.segment_bytes)).from_buffer_copy(self.segment_bytes)
        self.data = (U8 * 65536).from_buffer_copy(inspector.initial)
        self.lib.stars_lookup_part.argtypes = [C.POINTER(Lookup)]
        self.lib.stars_lookup_part.restype = C.c_int16
        self.lib.stars_lookup_part_record.argtypes = [C.POINTER(Lookup), PTR, C.c_uint16]
        self.lib.stars_lookup_part_record.restype = PTR
        self.lib.stars_tech_status.argtypes = [C.POINTER(C.c_int16), C.POINTER(C.c_int16), C.c_uint16]
        self.lib.stars_tech_status.restype = C.c_int16
        self.lib.stars_read_shdef.argtypes = [PTR, C.c_uint16, PTR, C.c_uint16,
                                           C.c_int16, C.POINTER(Ops), C.c_void_p]
        self.lib.stars_read_shdef.restype = C.c_int16
        self.lib.stars_initialize_shdef_cost.argtypes = [C.POINTER(Reprice), PTR, C.c_uint16, PTR]
        self.lib.stars_initialize_shdef_cost.restype = C.c_int16
        self.lib.stars_decompress_user_string.argtypes = [PTR, PTR, C.c_int16, PTR, C.POINTER(C.c_int16)]
        self.lib.stars_decompress_user_string.restype = C.c_int

    def part(self, group, item):
        lookup = Lookup()
        lookup.group, lookup.item, lookup.player = group, item, -1
        self.lib.stars_lookup_part(C.byref(lookup))
        pointer = self.lib.stars_lookup_part_record(C.byref(lookup), self.segment, len(self.segment))
        if not pointer:
            raise ValueError(f'Unresolved part {group:#x}/{item}')
        return C.string_at(pointer, lookup.stride)

    def read(self, body, player, bank=None, effective_player=None):
        owner = player[0]
        effective = owner if effective_player is None else effective_player
        technology = (C.c_byte * 6).from_buffer_copy(player[26:32])
        current = (C.c_int16 * 6)(*technology)
        flags = C.c_uint16(0)
        race_stat = RACE(lambda _, p, field: C.c_int8(player[62 + field]).value)
        race_bit = RACE(lambda _, p, bit: (int.from_bytes(player[78:82], 'little') >> bit) & 1)
        price = Reprice(self.segment, len(self.segment), technology, C.pointer(flags),
                        race_stat, race_bit, None, effective)
        failures = []

        def hull(_, item, out, capacity):
            try:
                record = self.part(0x4000 if item < 32 else 0x400,
                                   item if item < 32 else item - 32)
                if len(record) != 143 or capacity < 143:
                    raise ValueError('Hull span mismatch')
                C.memmove(out, record, 143)
                return 1
            except ValueError as error:
                failures.append(str(error))
                return 0

        def part(_, group, item, out, capacity):
            try:
                record = self.part(group, item)
                if capacity < 8:
                    raise ValueError('Part adapter capacity')
                C.memset(out, 0, 8)
                out[4], out[5] = record[40:42]
                required = (C.c_int16 * 6)(*record[2:8])
                return self.lib.stars_tech_status(current, required, player[57])
            except ValueError as error:
                failures.append(str(error))
                return 0

        def cost(_, design):
            if not self.lib.stars_initialize_shdef_cost(C.byref(price), design, 147, design):
                failures.append('Source design initialization failed')

        def name(_, encoded, size, out, count):
            capacity = C.c_int16(count[0])
            ok = self.lib.stars_decompress_user_string(self.data, encoded, size, out, C.byref(capacity))
            if ok:
                count[0] = len(C.string_at(out))
            return ok

        ops = Ops(HULL(hull), PART(part), COST(cost), NAME(name))
        if bank is None:
            bank = bytearray(16 * 147)
            for slot in range(16):
                struct.pack_into('<H', bank, slot * 147 + 123, 0x200)
        storage = (U8 * len(bank)).from_buffer(bank)
        wire = (U8 * len(body)).from_buffer_copy(body)
        if not self.lib.stars_read_shdef(wire, len(body), storage, len(bank),
                                        effective, C.byref(ops), None) or failures:
            raise ValueError(f'Design read failed: {failures}')
        slot = (int.from_bytes(body[:2], 'little') >> 10) & 15
        return bytes(bank[slot * 147:(slot + 1) * 147])

    def read_file(self, input_file):
        """Assign both design sections in player/count order, as FLoadGame does."""
        ships = [(p, pstate[1]) for p, pstate in sorted(input_file.players.items())]
        bases = [(p, int.from_bytes(pstate[4:6], 'little') >> 12)
                 for p, pstate in sorted(input_file.players.items())]
        owners = [p for p, count in ships + bases for _ in range(count)]
        records = [r for r in input_file.records if r.kind == 26]
        if len(records) != len(owners):
            raise ValueError('Design section counts disagree with player records')
        result, banks = {}, {}
        for record, owner in zip(records, owners):
            slot = (int.from_bytes(record.body[:2], 'little') >> 10) & 31
            key = (owner, slot)
            if key in result:
                raise ValueError('Duplicate design slot')
            bank_key = owner, slot >= 16
            if bank_key not in banks:
                bank = bytearray((10 if slot >= 16 else 16) * 147)
                for i in range(len(bank) // 147):
                    struct.pack_into('<H', bank, i * 147 + 123, 0x200)
                banks[bank_key] = bank
            result[key] = self.read(record.body, input_file.players[owner], banks[bank_key])
        return result, {key: bytes(bank) for key, bank in banks.items()}
