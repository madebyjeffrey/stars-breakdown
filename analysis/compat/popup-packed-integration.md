# Popup packed-record integration checkpoint

The CodeView `_popupdata` definition is a 22-byte packed record. The
integration probe decodes the recovered `grPopup` selector, planet id, and
slot/production fields, feeds those values into the typed `Popup` boundary,
and checks the resulting measurement, slot-update, window-creation, and
screen-clamping behavior.

The recovered selector fields are checked at offsets 0 (`grPopup`), 2
(`idPlanet`/`idPlan`), 4 (`iPlanetVar`/`cMax`), 6 (`iPlanVal`/`cCur`), 8
(`iPlanMin`/`cOperate`), 10 (`iPlanMax`/`fFactory`), 12 (`iPlrVal`), 14
(`iPlrMin`), 16 (`iPlrMax`), 18 (`idPlan`/`itok`), and 20 (`cMax`). The
adapter checks the 22-byte size, the raw little-endian field values, the
planet-id bridge for string popups, the slot extra-row bridge, and the
oversized component-popup clamp.

Host strict C89 and native VC++ 1.52c reports are:

`POPUP_PACKED_CHECKS=22 FAILURES=0 RESULT=PASS`

The native build in `runs/vmctl-vc152puint/` reports `COMPILE=PASS` and
`LINK=PASS`; the process exited 0 and the post-run window list contained no
Program Error dialog. Two earlier native reports are retained in the same run
directory as diagnostic history: one caught the expected screen-origin clamp
in the probe, and the second caught an incorrect self-count of 21 instead of
22. Neither was a source or compiler failure.

This was the initial Step-5 packed-record checkpoint. The E2 closure below
completes the remaining measurement, payload, layout, and promotion boundary.

Evidence:

- `tests/popup_packed_probe.c`
- `compat/vc152/popup-packed/`
- `analysis/debug/types.json`
- `analysis/decompiler/export/raw/10c0_0c7c_Popup.c`
- `analysis/reconstruction/popup.md`
- `runs/vmctl-vc152puint/PUINTSTA.TXT`
- `runs/vmctl-vc152puint/PUINTCL.LOG`
- `runs/vmctl-vc152puint/PUINTLINK.LOG`
- `runs/vmctl-vc152puint/PUINT.TXT`
- `runs/vmctl-vc152puint/PUINT2.TXT`
- `runs/vmctl-vc152puint/PUINTDEBUG.TXT`
- `runs/vmctl-vc152puint/PUINTDEBUG2.TXT`

## E2 closure and promotion

The remaining acceptance boundary was closed with
`tests/popup_e2_probe.c`. Its content-measurement adapter consumes localized
labels and packed 22-byte popup, 124-byte fleet, 56-byte planet, and 32-byte
hull payloads. It independently checks the returned dimensions and then
exercises every measured popup kind (1–8, 10, and 12) plus fixed kinds 9, 11,
13, and 14. The matrix also checks kind-specific values, slot updates, screen
clamping, font/message/capture setup, and the complete callback boundary.

Strict host C89 and a fresh VC++ 1.52c/Win98 run in `C:\PUINT2` both report
`POPUP_E2_CHECKS=148 FAILURES=0 RESULT=PASS`. Compile and link pass, and the
final Win98 window list and screenshot contain no Program Error dialog. The
raw original `Popup` source and the existing original popup screenshots remain
the original-behavior boundary; the probe does not invent unavailable live
localized strings or claim a broader application UI replay than it performs.

Evidence:

- `tests/popup_e2_probe.c`
- `compat/vc152/popup-packed/PUINTE2.C`
- `compat/vc152/popup-packed/BUILDE2.BAT`
- `runs/vmctl-vc152puint2/PUINTE2STA.TXT`
- `runs/vmctl-vc152puint2/PUINTE2CL.LOG`
- `runs/vmctl-vc152puint2/PUINTE2LINK.LOG`
- `runs/vmctl-vc152puint2/PUINTE2.TXT`
- `runs/vmctl-vc152puint2/PUINTE2.MAP`
- `runs/vmctl-vc152puint2/native-screen.bmp`

`Popup` is now promoted to Phase 2 step 6/6.

## Later original-geometry correction

The original live report popup showed that placement uses the clicked screen
point as the popup's **bottom-right** anchor. The earlier E2 oracle and
native result had a top-left rule hidden by clamping. Production `Popup` and
the oracle are corrected; fresh host/native `C:\PUIE4` passes 148/148 and
fresh `C:\ERUI2` passes an 86-check production report-click caller replay
against four original popup rectangles. The corrected evidence is in
`analysis/compat/execute-report-click-live-ui-geometry-e2.md` and
`runs/vmctl-gp-ui-r2/`; the old native result is historical, not the final
geometry authority.
