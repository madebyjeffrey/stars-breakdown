"""Decode the scoped clean-turn fleet/order oracle from plaintext records.

Only the retained owner-fleet type-16 and compact type-20 formats are decoded.
The byte layout follows the original _fleet/_order CodeView fields and the
reconstructed WriteFleet/WriteOrders serializer; this is not a game-file parser.
The final type-16 byte is the original fleet `cord` field: FReadFleet uses it
as the number of following type-19/type-20 order records. The decoder names
that field `order_count` in its returned mapping for existing fixtures.
"""

import struct


def decode_owner_fleet(body):
    if len(body) < 19:
        raise ValueError("owner fleet record is truncated")
    id_word, owner, flags, planet, x, y, ship_bitmap = struct.unpack_from(
        "<HHHhhhH", body)
    if (flags & 0xff) != 7:
        raise ValueError("oracle expects owner fleet det=7")
    cursor = 14

    def take(size):
        nonlocal cursor
        if cursor + size > len(body):
            raise ValueError("fleet payload is truncated")
        value = int.from_bytes(body[cursor:cursor + size], "little")
        cursor += size
        return value

    ship_counts = [0] * 16
    for design in range(16):
        if ship_bitmap & (1 << design):
            ship_counts[design] = take(1 if flags & 0x0800 else 2)
    cargo_selector = take(2)
    cargo = []
    for index in range(5):
        width_code = (cargo_selector >> (index * 2)) & 3
        cargo.append(take((0, 1, 2, 4)[width_code]) if width_code else 0)
    extra_bitmap = take(2)
    extra_counts = [0] * 16
    for design in range(16):
        if extra_bitmap & (1 << design):
            extra_counts[design] = take(2)
    design_plan = take(1)
    order_count = take(1)
    if cursor != len(body):
        raise ValueError("owner fleet record has trailing bytes")
    return dict(id=id_word & 0x1ff, id_player_bits=(id_word >> 9) & 15,
                owner=owner, saved_flags=flags, planet=planet,
                point=[x, y], ship_bitmap=ship_bitmap,
                ship_counts=ship_counts, cargo=cargo,
                extra_bitmap=extra_bitmap, extra_counts=extra_counts,
                design_plan=design_plan, order_count=order_count)


def decode_compact_order(body):
    if len(body) != 8:
        raise ValueError("compact order must be eight bytes")
    x, y, target_id, flags = struct.unpack("<hhhH", body)
    if flags & 0x000f:
        raise ValueError("noncompact order in type-20 record")
    return dict(point=[x, y], target_id=target_id, flags=flags,
                task=flags & 15, warp=(flags >> 4) & 15,
                object_kind=(flags >> 8) & 15,
                valid_task=(flags >> 12) & 1)


def decode_planet_prefix(body):
    """Decode only the proven type-13 planet prefix, not its variable tail."""
    if len(body) < 11:
        raise ValueError("planet prefix is truncated")
    id_word, flags = struct.unpack_from("<HH", body)
    if (flags & 0x7f) <= 2:
        raise ValueError("oracle expects a full planet record")
    selector = body[4]
    cursor = 5
    levels = []
    for index in range(3):
        code = (selector >> (index * 2)) & 3
        if code == 0:
            levels.append(0)
        elif code == 1:
            if cursor >= len(body):
                raise ValueError("mineral level is truncated")
            levels.append(body[cursor])
            cursor += 1
        else:
            raise ValueError("invalid mineral level selector")
    if cursor + 6 > len(body):
        raise ValueError("planet environment is truncated")
    concentrations = list(body[cursor:cursor + 3])
    environment = list(body[cursor + 3:cursor + 6])
    if any(value > 100 for value in environment):
        raise ValueError("planet environment is outside range")
    cursor += 6
    return dict(id=id_word & 0x7ff,
                owner=(id_word >> 11) & 31,
                flags=flags, mineral_levels=levels,
                mineral_concentrations=concentrations,
                environment=environment,
                undecoded_tail_hex=body[cursor:].hex())
