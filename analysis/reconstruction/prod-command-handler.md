# ProdCommandHandler reconstruction

`ProdCommandHandler` (`001b:1994-342f`, 6,811 bytes) dispatches production-dialog
commands through a typed C adapter. The reconstruction names the command IDs
and packed production-item fields, then handles dialog completion, help and
research toggles, adjacent-planet navigation, queue insertion, import/clear,
queue reordering, and quantity adjustment/removal.

The source is ordinary C and contains no assembly or emitted-byte replay. The
source probe exercises insertion, quantity adjustment, reordering, research
refresh, and dialog completion. It passes strict host C89 checks and the native
VC++ 1.52c Win98 build/link gate. The executable report is `CHECKS=1`,
`FAILURES=0`, `RESULT=PASS` in
`runs/vmctl-vc152aj/prod-command-handler/PCRES.TXT`.
