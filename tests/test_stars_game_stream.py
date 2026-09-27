import struct
import unittest
from unittest.mock import patch

from tools.stars_game_stream import compare_records, decode_stream


class FakeLibrary:
    def stars_set_file_xor_stream(self, *arguments):
        pass


class FakeInspector:
    def __init__(self):
        self.lib = FakeLibrary()
        self.crypt_calls = 0

    def crypt(self, body, state):
        self.crypt_calls += 1
        return body


def record(kind, body):
    return struct.pack("<H", kind * 1024 + len(body)) + body


def bof(turn):
    return record(8, struct.pack("<4sI4H", b"J3J3", 123, 0x2a60,
                                 turn, 0, 3))


class StreamTests(unittest.TestCase):
    def test_appended_turn_and_plaintext_message(self):
        data = bof(0) + record(12, b"\x7f\x00") + record(0, b"\x00\x00") + \
            bof(1) + record(12, b"\x3f\x00\x0a\x00\x0a")
        inspector = FakeInspector()
        with patch("pathlib.Path.read_bytes", return_value=data):
            records = decode_stream("ignored.M1", inspector)
        self.assertEqual([r["turn"] for r in records], [0, 0, 0, 1, 1])
        self.assertEqual(records[-1]["body"], b"\x3f\x00\x0a\x00\x0a")
        self.assertEqual(inspector.crypt_calls, 2)
        self.assertTrue(compare_records(records, records)["plaintext_equal"])

    def test_truncated_body_rejected(self):
        with patch("pathlib.Path.read_bytes", return_value=bof(0) + b"\x05\x30x"):
            with self.assertRaisesRegex(ValueError, "truncated record body"):
                decode_stream("ignored.M1", FakeInspector())

    def test_record_before_bof_rejected(self):
        with patch("pathlib.Path.read_bytes", return_value=record(12, b"\x3f\x00")):
            with self.assertRaisesRegex(ValueError, "record before BOF"):
                decode_stream("ignored.M1", FakeInspector())


if __name__ == "__main__":
    unittest.main()
