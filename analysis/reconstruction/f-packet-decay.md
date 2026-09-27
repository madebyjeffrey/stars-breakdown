# FPacketDecay reconstruction

`reconstructed/src/f_packet_decay.c` is a typed semantic reconstruction of packet-object decay, preserving age thresholds, packet removal, and affected-fleet/state updates.

Validation: `tests/f_packet_decay_source_probe.c` passes under strict host C89, and the VC1.52c Win98 build and native probe pass in `runs/vmctl-vc152ew/PDSTA.TXT` and `runs/vmctl-vc152ew/PDRES.TXT`.
