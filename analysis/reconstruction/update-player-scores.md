# UpdatePlayerScores

`UpdatePlayerScores` is represented as typed score/victory logic in
`reconstructed/src/update_player_scores.c`. It preserves per-player score
calculation, empty-player death detection based on planets and ship-value
channels and notification, victory metric thresholds (including the distinct
per-category technology threshold and technology-category count), achievement
flags/counts, rank ordering, unique-winner and
runner-up checks, multi-player achievement masks, dirty-state signaling, and
victory-message selection, including the separate all-but-one-dead
winner/eliminated-player notification path.

The packed `_scorex`/`_player` storage and prepended-message serialization are
now exercised through a raw-record integration adapter: the recovered score,
rank, validity/achievement, and dead-player offsets round-trip through the
procedure, and prepended message headers/lengths are checked. Exact fixed-point
arithmetic helpers and global victory settings remain separate service
boundaries.
The host probe covers ranking, dead-player, threshold, tie/mask, message,
all-but-one-dead, and single-player paths. A fresh VC++ 1.52c/Win98
checkpoint in `C:\UPS` compiled and linked the exact source and returned
`UPDATE_PLAYER_SCORES_CHECKS=34 FAILURES=0 RESULT=PASS` without a runtime
error dialog. A fresh packed/integration VC1.52c/Win98 run also reports
`UPDATE_PLAYER_SCORES_INTEGRATION_CHECKS=6 FAILURES=0 RESULT=PASS` without a
Program Error dialog. The original `Player Scores` dialog was observed with
its headings and category grid; the loaded save displayed blank values, so
that capture is a consumer-boundary observation rather than a populated-value
differential. See `analysis/compat/update-player-scores-integration.md`.
The separate E2 replay now closes the populated-score and turn/report
acceptance boundary. Its independent oracle covers six populated/dead/tied/
single-player cases and compares typed scores, packed report fields, message
counts, dirty invalidation, and the report-consumer checksum. Host and fresh
Win98 runs both report 36 checks with zero failures and no Program Error
dialog. The original blank-save dialog remains only UI observation; the raw
original producer/consumer instructions provide the original-behavior boundary.
`UpdatePlayerScores` is promoted to Phase 2 step 6/6; see
`analysis/compat/update-player-scores-integration.md` and
`runs/vmctl-vc152upsint2/`.

## Technology victory correction (2026-09-25)

The Gate 2 audit of original `UpdatePlayerScores` (NE 18:6258) found that
the source implementation and its earlier test oracle both compared the
score-history technology total with victory setting 1. The original instead
counts the six player technology levels meeting setting 1, then compares that
count with setting 2. This defect was not covered by the earlier cases.

`StarsUpdatePlayerScore` now carries `technology_levels[6]` separately from
`tech_count`, which remains the score-history total. The packed integration
adapter reads the six signed technology bytes from PLAYER offsets 26..31.
The source procedure and E2 oracle now use the original category-count rule.
Three new source cases distinguish one category at level 30 from three at
level 10 (same total), and verify that increasing the required category count
from three to four removes eligibility.

Current strict-C89 host reports: source **37/37**, packed integration **6/6**,
E2 **36/36**, all zero failures. The historical native reports above predate
this correction and do not establish native validation of these changes.
The earlier E2 oracle also contained the incorrect technology gate; its old
pass did not validate this rule. SO-E3 G2-11 remains open: score calculation,
metadata, current player updates, and output binding still need integration.
