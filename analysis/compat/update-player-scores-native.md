# UpdatePlayerScores native checkpoint

The fresh Win98 guest run is preserved in `runs/vmctl-vc152ups/`.

- `UPSSTA.TXT`: `COMPILE=PASS`, `LINK=PASS`
- `UPSCL.LOG`: VC++ 1.52c compiled the native wrapper containing the exact
  reviewed `UPS.C` and `PROBE.C` source
- `UPSLINK.LOG`: segmented link completed with the retained `WINSTUB.EXE`
- `UPSR.TXT`: `UPDATE_PLAYER_SCORES_CHECKS=34 FAILURES=0 RESULT=PASS`
- the post-run guest window list showed no Program Error dialog

The transferred `UPS.C` is byte-identical to
`reconstructed/src/update_player_scores.c`, and `PROBE.C` is byte-identical
to `tests/update_player_scores_source_probe.c`. The wrapper writes the
native report without requiring a console or modal message box.

The probe exercises score calculation, empty-player death notification,
victory thresholds, distinct technology threshold and category count,
achievement flags, ranking, unique-winner and runner-up paths,
multi-player achievement masks, dirty-state signaling, message selection,
all-but-one-dead winner/elimination handling, and the single-player path.

This advances `UpdatePlayerScores` from Phase 2 step 3/6 to step 4/6.
Packed `_scorex`/`_player` records, fixed-point helpers, global victory
fields, prepended-message serialization, and full turn/report integration
remain open.
