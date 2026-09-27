# UpdatePlayerScores packed/integration checkpoint

The recovered packed layouts and the reconstructed score procedure were
checked together in a fresh VC1.52c/Win98 harness. The adapter uses the
recovered `_scorex` offsets (`score` at 4; `lScore` 4, `cResources` 8,
`cPlanet` 12, `rgcsh[2]` 20, and `cTechLevels` 22), the `_scorex.iRank` word
at 2, the valid/achievement bits in the word at 0, and `_player.fDead` at
offset 84. The callback writes the updated rank, validity, achievement mask,
score fields, and dead bit back to those raw records.

The same run routes the procedure's prepended notifications through the
reconstructed message packer and checks the resulting record lengths and
message headers. The Win98 report is
`UPDATE_PLAYER_SCORES_INTEGRATION_CHECKS=6 FAILURES=0 RESULT=PASS`; compile and
link also pass, and the post-run window list contains no Program Error dialog.

The original executable's `Player Scores` dialog was also opened from the
Report menu. Its player headings and nine score categories were visible
without an error dialog; the loaded save had blank score cells, so this
screenshot is treated as a UI/consumer-boundary observation rather than a
populated-value differential. The raw original `UpdatePlayerScores`,
`DrawScoreReport`, and `ScoreXDlg` procedures provide the instruction-level
producer/consumer agreement for the packed fields.

Evidence:

- `runs/vmctl-vc152upsint/UPR.TXT`
- `runs/vmctl-vc152upsint/INSTA.TXT`
- `runs/vmctl-vc152upsint/INCL.LOG`
- `runs/vmctl-vc152upsint/INLINK.LOG`
- `runs/vmctl-vc152upsint/INAT.MAP`
- `runs/vmctl-vc152current/score-report.bmp`
- `tests/update_player_scores_integration_probe.c`
- `analysis/decompiler/export/raw/10b8_6258_UpdatePlayerScores.c`
- `analysis/decompiler/export/raw/1108_1e0c_DrawScoreReport.c`
- `analysis/decompiler/export/raw/1108_0f66_ScoreXDlg.c`

The initial packed checkpoint advanced `UpdatePlayerScores` from Phase 2 step
4/6 to 5/6. The closure below completes the remaining Step-5 work.

## E2 closure and promotion

The remaining Step-5 boundary was closed with a separate populated-score
replay. `tests/update_player_scores_e2_probe.c` includes the existing packed
adapter but computes the expected result independently before calling the
reconstruction. Six cases cover empty-player death, unique winner/runner-up,
tied winners, all-but-one-dead elimination, a single-player turn, and multiple
achievement-mask recipients. The checks compare the typed result, packed
`_scorex` and `_player` fields, message count, dirty/report invalidation, and a
report-consumer checksum over the packed records.

The host run reports
`UPDATE_PLAYER_SCORES_E2_CHECKS=36 FAILURES=0 RESULT=PASS`. The same source was
compiled and linked with VC++ 1.52c in fresh guest directory `C:\UPSINT2`; the
native report also passes and the final window list contains no Program Error
dialog. This closes E1 (populated score matrix) and E2 (complete turn/report
replay) at the documented packed acceptance boundary. The original executable
still provides the instruction-level producer/consumer evidence and the live
dialog screenshot remains the documented UI observation; no populated values
were invented from the blank save.

Evidence:

- `tests/update_player_scores_e2_probe.c`
- `tests/update_player_scores_integration_probe.c`
- `compat/vc152/update-player-scores-integration/UPSE2.C`
- `compat/vc152/update-player-scores-integration/BUILDE2.BAT`
- `runs/vmctl-vc152upsint2/UPSE2STA-final2.TXT`
- `runs/vmctl-vc152upsint2/UPSE2CL-final2.LOG`
- `runs/vmctl-vc152upsint2/UPSE2LINK-final2.LOG`
- `runs/vmctl-vc152upsint2/UPSE2-final2.TXT`
- `runs/vmctl-vc152upsint2/UPSE2-final.MAP`
- `runs/vmctl-vc152upsint2/native-screen-final2.bmp`

`UpdatePlayerScores` is now promoted to Phase 2 step 6/6.
