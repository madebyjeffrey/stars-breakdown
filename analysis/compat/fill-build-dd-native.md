# FillBuildDD native checkpoint

The fresh Win98 guest run is preserved in `runs/vmctl-vc152fbd/`.

- `FBDSTA.TXT`: `COMPILE=PASS`, `LINK=PASS`
- `FBDCL.LOG`: VC++ 1.52c compilation completed for the current
  `fill_build_dd.c` source and probe; it retains one non-fatal signed/unsigned
  comparison warning in the player-design loop
- `FBDLINK.LOG`: segmented link completed with `FBD.DEF`
- `FBDR.TXT`: `FILL_BUILD_DD_CHECKS=8 FAILURES=0 RESULT=PASS`
- `FBDR-CONSOLE.TXT`: empty; the process exited cleanly

The run used fresh `C:\FBD`, directory-first CRLF wrappers, a local
`WINSTUB.EXE`, and a native-only report path. The guest `windows` check after
execution showed no program-error dialog. This validates dropdown reset and
population, special-part and starbase label modes, delete/edit enablement,
queued-state handling, list-end notification, list-count clamping, and font
height/resize arithmetic. Exact packed design lookup, player-name formatting,
queue records, and full build-dialog integration remain outside this
checkpoint.

The packed design E1 follow-up is preserved in `runs/vmctl-vc152fbdp/`:

- `FBDPSTA.TXT`: `COMPILE=PASS`, `LINK=PASS`
- `FBDPCL.LOG`: VC++ 1.52c compilation of the packed-record source/probe
- `FBDPLINK.LOG`: segmented link completed with `FBDP.DEF`
- `FBDPE1.TXT`: `FILL_BUILD_DD_PACKED_E1_CHECKS=8 FAILURES=0 RESULT=PASS`
- `native-screen.bmp`: post-run Win98 screen capture

The process exited 0 and the post-run window inventory showed no Program Error
dialog. This closes E1: the 147-byte `_shdef` stride, packed `fFree` bit,
name offset, `cExist` edit-state boundary, typed-shadow conflict, and
neighbor preservation are checked. E2 localized labels/resources, E3 full
population/control integration, and promotion remain open.

The localized-label E2 follow-up is preserved in `runs/vmctl-vc152fbdl/`:

- `FBDLSTA.TXT`: `COMPILE=PASS`, `LINK=PASS`
- `FBDLCL.LOG`: VC++ 1.52c compilation of the exact resource-ID probe
- `FBDLLINK.LOG`: segmented link completed with `FBDL.DEF`
- `FBDLE2.TXT`: `FILL_BUILD_DD_LABELS_E2_CHECKS=58 FAILURES=0 RESULT=PASS`
- `native-screen.bmp`: post-run Win98 screen capture

The process exited 0 and the post-run window inventory showed no Program Error
dialog. This closes E2: the normal 13-entry and starbase 8-entry compressed
string tables, both label modes, reset/add/list-count/end ordering, and the
three nonzero-mode disable calls are checked. E3 full population/queue/resize/
caller integration and promotion remain open.

The complete-population E3 follow-up is preserved in
`runs/vmctl-vc152fbde2/`:

- `FBDESTA2.TXT`: `COMPILE=PASS`, `LINK=PASS`
- `FBDECL2.LOG`: VC++ 1.52c compilation of the full branch matrix
- `FBDELINK2.LOG`: segmented link completed with `FBDE.DEF`
- `FBDE3.TXT`: `FILL_BUILD_DD_E3_CHECKS=14 FAILURES=0 RESULT=PASS`
- `native-screen.bmp`: post-run Win98 screen capture

The process exited 0 and the post-run window inventory showed no Program Error
dialog. This closes E3: complete normal/empty/hidden/queued/built population,
special and enemy branches, starbase limits, selector and edit/delete state,
32-row resize clamping, and callback ordering are checked. Step-6 promotion
has now been recorded in the review ledger and derived ledgers. The initial
no-CRT `_printf` linker diagnostic is retained in `runs/vmctl-vc152fbde/`.
