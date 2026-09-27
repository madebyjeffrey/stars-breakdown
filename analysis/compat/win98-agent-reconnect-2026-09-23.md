# StarDev/STARSAGT reconnection diagnostic — 2026-09-23

The StarDev 86Box manager reported the VM running as PID 97943, and a
read-only process check confirmed an 86Box `--vmpath .../StarDev --vmname
StarDev` process. The VM manager preview showed a Win98 desktop, but
computer control exposed only the manager window, not a separate guest
display. No hard reset or force shutdown was used.

`python3 ftp-share/VMCTL1/HOSTTEST.PY` passed the local protocol self-test.
The current `AGENT.C` network thread retries a failed host connection
after twenty 100 ms sleeps, about two seconds. A local listener bound
successfully to `0.0.0.0:2122` and received no guest connection during a
15-second observation window at about 06:11–06:12 MDT. The listener was
then stopped normally with Ctrl-C. Similar earlier checks also received
no connection. This does **not** prove whether STARSAGT has exited or
Win98 networking is failing, but it makes a host command/protocol fault
less likely and means native packets cannot be transferred yet.

Next guest-side check: in Win98, run `C:`, `CD \VMCTL1`, `STARTAGT` (or use
`C:\VMCTL1\STARTAGT.BAT`). Confirm the STARSAGT window remains open. Its
Try connection button initiates an immediate attempt; the window also
shows connection/error status. If it fails, inspect
`C:\STARSAGT\AGENT.LOG` and the guest network configuration. The host
listener can then be restarted and checked with `info`, `ping`, and
`windows`. Do not reset the VM to recover this connection. The current
Rototill native packet is r7 for fresh `C:\RTE1N7`; older r1–r6 packets
are stale for current source.
