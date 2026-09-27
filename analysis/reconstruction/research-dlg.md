# ResearchDlg reconstruction

`ResearchDlg` is the Win16 dialog procedure for selecting the player's
research percentage, current technology field, and projected technology field.
The reconstructed source keeps the original message branches and packed-field
mapping while replacing DGROUP, USER, GDI, localization, and player-storage
access with typed callbacks.

## Recovered behavior

- Paint begins a dialog paint, obtains the client rectangle, delegates the
  research drawing pass, and ends the paint. Background erasure fills the
  client rectangle with the button-face brush.
- Control-color handling recognizes the six research radio controls and
  static controls with the original `WM_CTLCOLOR` high-word value, sets the
  button-face background, and returns the brush.
- Cursor handling maps the screen point into the dialog and identifies the
  future-technology and technology-note help regions.
- Initialization decodes the current and future technology nibbles, loads and
  measures the six localized radio labels, lays out the radio buttons, fills
  and selects the future-technology combo, measures the right-hand labels,
  sizes the dialog, restores the font/DC, sums resources on the player's
  planets, and computes projected research spending.
- Radio commands update the selected current technology and request a redraw;
  left-button messages retain the original `FTrackResearchDlg` boundary.
- OK maps the combo selection back to the packed future-technology nibble,
  commits changed research settings, writes the two-byte runtime record,
  advances the tutor hooks, saves the sticky position, and closes the dialog.

The source probe checks these branches with deterministic service callbacks.
A fresh VC++ 1.52c/Win98 checkpoint now compiles, links, and runs the exact
source with nine grouped native assertions passing. This advances the
procedure to Phase 2 step 4/6; packed player storage, localized resources,
and full Win16 control integration remain open.

The original Win98 executable has now been exercised through the dialog
boundary: `Commands > Research...` displayed technology status, projected
research, expected benefits, and resource allocation, and selecting Weapons
refreshed the current-field selection without a Program Error. Evidence is
preserved in `analysis/compat/research-integration.md` and
`runs/vmctl-vc152current/`. This advances `ResearchDlg` to Phase 2 step 5/6
for the original dialog/radio-selection boundary; packed player storage,
localized resources, exact Win16 control integration, and promotion remain
open.

The resource/control checkpoint was expanded in fresh `C:\RSD3`. The strict
host and VC++ 1.52c/Win98 matrix now passes 11 checks over the six technology
labels, the `Research` title, future-field IDs `0x53` through `0x5a`, and the
radio, combo, Done, and Help control lookups. Compile and link passed and no
Program Error dialog appeared. This closes the resource-ID/control-lookup
portion of step 5; full live USER message-loop behavior and promotion remain
open.

The packed-player probe was expanded to 10 strict C89/native cases. It now
checks that the research settings write uses player-record offset `0x22` and
length 2, preserves the ordinary packed value `0x12`, and handles the
zero-percent/technology-5/future-technology-6 boundary as `0x65`. The fresh
`C:\RSD2` VC++ 1.52c run passed without a Program Error dialog. This closes
the packed write-location and nibble-boundary concerns inside step 5/6;
localized resources, exact Win16 control integration, and promotion remain
open.

The isolated native-only `C:\RSDM` harness then created real Win16 parent and
child controls and ran the `GetMessage`/`TranslateMessage`/`DispatchMessage`
loop. It routed initialization, radio selection, and Done through the
reconstructed procedure and passed
`RESEARCH_DLG_USER_CHECKS=9 FAILURES=0 RESULT=PASS`; the final window list had
no Program Error dialog. This closes the last Step-5 USER/control-integration
unit. The harness intentionally reuses the existing resource/GDI probes for
those narrower boundaries; the final review-ledger promotion is now recorded,
so `ResearchDlg` is promoted to `6/6`.
