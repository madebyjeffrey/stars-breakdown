# DoCyberPackets reconstruction

`DoCyberPackets` (`0016:1a78-2829`, 3,505 bytes) selects packet production
for eligible Cyber AI planets. The typed C adapter filters owned packet-ready
planets, chooses a target, selects the mineral with the best post-cost
remainder, limits the packet count, queues the packet item, and debits the
source strength before reporting the selected source and destination.

The source contains named packet constants and ordinary C policy code, with no
assembly or emitted-byte replay. The source probe covers ownership/readiness
filtering, target selection, mineral choice, count limiting, queue insertion,
and strength debit. It passes strict host C89 checks and the native VC++
1.52c Win98 build/link/runtime gate. The executable report is `CHECKS=1`,
`FAILURES=0`, `RESULT=PASS` in
`runs/vmctl-vc152al/do-cyber-packets/DPRES.TXT`.
