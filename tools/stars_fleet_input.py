"""Bounded retained-file adapter to the production FReadFleet implementation.

Wire framing and span checks live here; ship/cargo/damage/order initialization
is performed by stars_read_fleet. This adapter deliberately rejects formats
outside the complete type-16 fleets in the pinned Gate 1 input corpus.
"""
import ctypes as C
from dataclasses import dataclass
import struct

U8, U16, I16, U32 = C.c_uint8, C.c_uint16, C.c_int16, C.c_uint32
P8 = C.POINTER(U8)


class FleetInput(C.Structure):
    _fields_ = [
        ('fleet_flags', U16), ('ship_bitmap', U16),
        ('ship_words', C.POINTER(U16)), ('ship_bytes', P8),
        ('mineral_bitmap', U16), ('mineral_payload', P8),
        ('mineral_payload_size', U16), ('header_type', U16),
        ('short_record_count', U16), ('tail_payload', P8),
        ('tail_payload_size', U16), ('planet', I16), ('x', I16),
        ('y', I16), ('game_turn', I16), ('planet_count', U16),
        ('planet_x', C.POINTER(I16)), ('planet_y', C.POINTER(I16)),
        ('name_payload', P8), ('name_payload_size', U16)]


class FleetState(C.Structure):
    _fields_ = [
        ('fleet_flags', U16), ('ship_counts', U16 * 32),
        ('minerals', U32 * 5), ('overlap_116', U32 * 2),
        ('overlap_44', U32 * 2), ('design_plan', U16),
        ('order_count', U16), ('planet', I16), ('x', I16), ('y', I16),
        ('orders', U8 * (32 * 18)), ('name', C.c_char * 33)]


@dataclass
class LoadedFleet:
    id_word: int
    owner: int
    state: FleetState
    record_offsets: tuple


class FleetReader:
    def __init__(self, inspector):
        self.lib = inspector.lib
        self.lib.stars_read_fleet.argtypes = [C.POINTER(FleetInput),
            C.POINTER(FleetState), C.c_void_p, C.c_void_p]
        self.lib.stars_read_fleet.restype = I16

    def read_file(self, loaded, coordinates):
        fleets, consumed = {}, set()
        xs = (I16 * len(coordinates))(*(p[0] for p in coordinates))
        ys = (I16 * len(coordinates))(*(p[1] for p in coordinates))
        for index, record in enumerate(loaded.records):
            if record.kind in (19, 20):
                if record.offset not in consumed:
                    raise ValueError('Orphan fleet order')
                continue
            if record.kind != 16:
                continue
            body = record.body
            if len(body) < 18:
                raise ValueError('Truncated fleet prefix')
            identity, owner, flags, planet, x, y, bitmap = struct.unpack_from('<HHHhhhH', body)
            if flags & 255 != 7 or owner not in loaded.players:
                raise ValueError('Unsupported fleet detail or absent owner')
            cursor = 14

            def take(size):
                nonlocal cursor
                if cursor + size > len(body):
                    raise ValueError('Truncated fleet payload')
                result = body[cursor:cursor + size]
                cursor += size
                return result

            ship_width = 1 if flags & 0x800 else 2
            ships = [int.from_bytes(take(ship_width), 'little')
                     for slot in range(16) if bitmap & (1 << slot)]
            ship_bytes, ship_words = (U8 * 16)(*ships), (U16 * 16)(*ships)
            mineral_bitmap = int.from_bytes(take(2), 'little')
            mineral_wire = take(sum((0, 1, 2, 4)[(mineral_bitmap >> (2 * lane)) & 3]
                                    for lane in range(5)))
            tail_start = cursor
            damage_bitmap = int.from_bytes(take(2), 'little')
            take(2 * damage_bitmap.bit_count())
            take(1)  # Battle-plan byte, decoded by the source reader.
            order_count = take(1)[0]
            if cursor != len(body) or order_count > 32:
                raise ValueError('Invalid fleet tail or order count')
            order_wire, offsets = bytearray(), [record.offset]
            for number in range(order_count):
                at = index + 1 + number
                if at >= len(loaded.records):
                    raise ValueError('Missing fleet order')
                order = loaded.records[at]
                if order.kind not in (19, 20) or len(order.body) != (18 if order.kind == 19 else 8):
                    raise ValueError('Invalid fleet order record')
                # FReadFleet clears its 18-byte scratch before reading either
                # format. Expanding here also supports mixed order sequences.
                order_wire.extend(order.body.ljust(18, b'\0'))
                offsets.append(order.offset)
            tail_wire = body[tail_start:] + order_wire
            minerals = (U8 * len(mineral_wire)).from_buffer_copy(mineral_wire)
            tail = (U8 * len(tail_wire)).from_buffer_copy(tail_wire)
            source = FleetInput(fleet_flags=flags, ship_bitmap=bitmap,
                ship_words=ship_words, ship_bytes=ship_bytes,
                mineral_bitmap=mineral_bitmap, mineral_payload=minerals,
                mineral_payload_size=len(mineral_wire), header_type=16,
                tail_payload=tail, tail_payload_size=len(tail_wire),
                planet=planet, x=x, y=y, game_turn=record.turn,
                planet_count=len(coordinates), planet_x=xs, planet_y=ys)
            state = FleetState()
            if not self.lib.stars_read_fleet(C.byref(source), C.byref(state), None, None):
                raise ValueError('Source fleet initialization rejected input')
            key = (owner, identity & 511)
            if key in fleets:
                raise ValueError('Duplicate fleet identity')
            fleets[key] = LoadedFleet(identity, owner, state, tuple(offsets))
            consumed.update(offsets)
        return fleets
