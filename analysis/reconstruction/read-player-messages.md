# ReadPlayerMessages reconstruction

`reconstructed/src/read_player_messages.c` is a typed semantic reconstruction of player-message loading, preserving record boundaries, message filtering, text decoding, and destination updates.

Validation: `tests/read_player_messages_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ew/RMPSTA2.TXT` and `runs/vmctl-vc152ew/RMPRES2.TXT`.
