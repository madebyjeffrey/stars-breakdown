# DoAutomitronAiTurn E1/E3 native packet — prepared, not run

The current packet is compat/vc152/automitron-e1-e3-branches-r12/ for a
fresh Win98 guest directory C:\AUE1N12. It snapshots current production
AUT.C, PLD.C and CORE.H, plus thirteen strict-C89 probes: colonizer seed
(19 checks), AI-planet record (14), live AI table (8), support queue (15),
planet census (5), early production queue (17), two hull batches (18),
packed PLPROD bypass (13), production lifecycle (16), live planet values
(8), fleet flags (8), packed route (10), and low-fuel scout (15). The Win16
driver writes AUE1RES.TXT with each probe's status and an aggregate result.
It compiles each probe separately with renamed main, retaining distinct
compile logs and BLDSTAT.TXT compile/link status. A pre-existing
BLDSTAT.TXT refuses reuse.

The package-layout test verifies all twenty-three files, DOS CRLF
wrappers, the guest directory, probe entrypoint flags, 127-byte
command/response lines, and overwrite refusal. Earlier r1–r11 packages
remain as history but are superseded. Additional object basenames were
shortened in r12 to keep the linker response line within the conservative
DOS limit.

No VC++ 1.52c compile, Win16 link, guest run, or original-turn comparison
has occurred. Treat the packet as transfer-ready input only. On agent
reconnection, create C:\AUE1N12, transfer every file, run RUNBLD.BAT via
command.com /c, retrieve BLDSTAT.TXT and all compile/link logs, then run
RUN.BAT and retrieve AUE1RES.TXT, executable, objects and map into a fresh
host runs/ directory. Process exit alone is not a pass.
