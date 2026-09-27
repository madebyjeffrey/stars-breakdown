"""Source message-table initialization for the pinned SO-E3 input records."""
import ctypes as C
from dataclasses import dataclass
from pathlib import Path

from tools.ne_layout import NE

U8, U16, I16 = C.c_uint8, C.c_uint16, C.c_int16
P8 = C.POINTER(U8)
COUNT = C.CFUNCTYPE(U16, C.c_void_p, I16)
SELECT = C.CFUNCTYPE(I16, C.c_void_p, I16)
APPEND = C.CFUNCTYPE(None, C.c_void_p, P8, U16)


class Record(C.Structure):
    _fields_ = [('kind', U16), ('bytes', U16), ('data', P8)]


class ReadInput(C.Structure):
    _fields_ = [('message_records', C.POINTER(Record)), ('message_record_count', U16),
        ('incoming_records', C.POINTER(Record)), ('incoming_record_count', U16),
        ('message_buffer', P8), ('message_capacity', U16), ('message_bytes', U16),
        ('message_count', U16), ('sent_bits', P8), ('sent_bit_bytes', U16),
        ('incoming', C.c_void_p), ('incoming_capacity', U16), ('incoming_count', U16),
        ('current_message', I16), ('parameter_count', COUNT),
        ('select_first', SELECT), ('context', C.c_void_p)]


class MessageState(C.Structure):
    _fields_ = [('message_memory', U16), ('message_count', U16),
        ('player_message_count', U16), ('current_message', I16),
        ('view_filtered', U16), ('filtered', U8 * 256), ('sent', U8 * 256)]


class MessageOps(C.Structure):
    _fields_ = [('message_id', SELECT), ('parameter_count', COUNT),
        ('player_flags', C.c_void_p), ('append_message', APPEND),
        ('prepend_message', C.c_void_p)]


class BigMessage(C.Structure):
    _fields_ = [('message', I16), ('goto_id', I16), ('parameters', I16 * 7)]


@dataclass
class LoadedMessages:
    buffer: bytes
    sent_bits: bytes
    current_message: int
    decoded: tuple
    record_offsets: tuple


class MessageReader:
    def __init__(self, inspector):
        self.lib = inspector.lib
        # Inspector already pins this executable's SHA-256. NE.raw is zero
        # indexed: rgcMsgArgs is logical segment 7, offset 0x5b0e, 387 bytes.
        original = Path(__file__).resolve().parents[1] / 'target/stars.exe'
        self.counts = bytes(NE(original).raw[6])[0x5b0e:0x5b0e + 387]
        if len(self.counts) != 387 or max(self.counts) > 7:
            raise ValueError('Invalid original message parameter table')
        self.lib.stars_read_player_messages.argtypes = [C.POINTER(ReadInput)]
        self.lib.stars_read_player_messages.restype = I16
        self.lib.stars_message_next.argtypes = [C.POINTER(MessageState), I16,
            C.POINTER(MessageOps), C.c_void_p]
        self.lib.stars_message_next.restype = I16
        self.lib.stars_get_message_big.argtypes = [P8, U16, U16, I16,
            C.POINTER(MessageOps), C.c_void_p, C.POINTER(BigMessage)]
        self.lib.stars_get_message_big.restype = I16
        self.lib.stars_send_player_message2_xgen.argtypes = [C.POINTER(MessageState)] + [I16] * 5 + [C.POINTER(MessageOps), C.c_void_p]
        self.lib.stars_send_player_message2_xgen.restype = I16

    def read_file(self, loaded):
        records = [r for r in loaded.records if r.kind == 12]
        if any(r.kind == 40 for r in loaded.records):
            raise ValueError('Incoming player-message adapter not bound for this corpus')
        wire = b''.join(r.body for r in records)
        cursor, ids = 0, []
        while cursor < len(wire):
            if len(wire) - cursor < 4:
                raise ValueError('Truncated MSGHDR')
            word = int.from_bytes(wire[cursor:cursor + 2], 'little')
            identity, mask = word & 511, word >> 9
            if identity >= len(self.counts):
                raise ValueError('Unknown message identity')
            cursor += 4 + sum(1 + ((mask >> i) & 1) for i in range(self.counts[identity]))
            if cursor > len(wire):
                raise ValueError('Truncated message arguments')
            ids.append(identity)
        if len(wire) > 65535:
            raise ValueError('Message table exceeds source buffer capacity')
        spans = [(U8 * len(r.body)).from_buffer_copy(r.body) for r in records]
        source_records = (Record * len(records))(*[
            Record(r.kind, len(r.body), span) for r, span in zip(records, spans)])
        buffer, sent = (U8 * max(1, len(wire)))(), (U8 * 64)()
        count = COUNT(lambda _, identity: self.counts[identity])
        appended = []

        def append(_, packet, size):
            body = bytes(packet[:size])
            appended.append(body)
            ids.append(int.from_bytes(body[:2], 'little') & 511)

        ops = MessageOps(message_id=SELECT(lambda _, index: ids[index]),
            parameter_count=count, append_message=APPEND(append))
        selection = MessageState(current_message=-1)

        def select(_, filtered):
            selection.message_count = source.message_count
            return self.lib.stars_message_next(C.byref(selection), filtered, C.byref(ops), None)

        source = ReadInput(message_records=source_records, message_record_count=len(records),
            message_buffer=buffer, message_capacity=len(buffer), sent_bits=sent,
            sent_bit_bytes=64, parameter_count=count, select_first=SELECT(select))
        if not self.lib.stars_read_player_messages(C.byref(source)) or (
                source.message_bytes != len(wire) or source.message_count != len(ids)):
            raise ValueError('Source message initialization coverage mismatch')
        selection.message_memory = source.message_bytes
        selection.message_count = source.message_count
        selection.current_message = source.current_message
        selection.sent[:64] = sent
        for request in loaded.planet_messages:
            if not self.lib.stars_send_player_message2_xgen(C.byref(selection),
                    *request, C.byref(ops), None):
                raise ValueError('Planet discovery message initialization failed')
        wire += b''.join(appended)
        buffer = (U8 * max(1, len(wire)))()
        buffer[:len(wire)] = wire
        decoded = []
        for index in range(selection.message_count):
            message = BigMessage()
            if not self.lib.stars_get_message_big(buffer, selection.message_memory,
                    selection.message_count, index, C.byref(ops), None, C.byref(message)):
                raise ValueError('Source message decode failed')
            decoded.append((message.message, message.goto_id,
                tuple(message.parameters[:self.counts[message.message]])))
        return LoadedMessages(wire, bytes(selection.sent[:64]),
            selection.current_message, tuple(decoded), tuple(r.offset for r in records))
