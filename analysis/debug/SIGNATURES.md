# Recovered effective signatures

Names are recovered debug names. Types are human-readable descriptions; compilable declarations are in `reconstructed/include/sfuncs.h`. The two signature reconciliations are explained in README.md.

| Module | Function | Address | Return | Convention | Parameters |
|---|---|---|---|---|---|
| turn3.obj | `SatisfyOrders` | `0017:6798` | void | far cdecl | int iPass |
| tb.obj | `TbWndProc` | `000e:001e` | long | far pascal | unsigned int hwnd, unsigned int msg, unsigned int wParam, long lParam |
| tb.obj | `DrawToolbar` | `000e:06f0` | void | far cdecl | unsigned int hdc, tagRECT near * prc |
| tb.obj | `DrawBitmapButton` | `000e:078c` | void | far cdecl | unsigned int hdc, tagPOINT pt, int ibtn, int fDown |
| tb.obj | `ItbFromPpt` | `000e:0b12` | int | far cdecl | tagPOINT near * ppt |
| tb.obj | `DxOfBtn` | `000e:0bb2` | int | far cdecl | int itb |
| tb.obj | `FIsButtonDown` | `000e:0c3a` | int | far cdecl | int itb |
| tb.obj | `ExecuteButton` | `000e:0db6` | void | far cdecl | int itb, int fDown |
| tb.obj | `TerminateToolbarFocus` | `000e:1680` | void | far cdecl | int fCancel |
| tb.obj | `ShowTooltip` | `000e:17ea` | void | far cdecl | int ids, tagRECT near * prc |
| tb.obj | `TooltipWndProc` | `000e:19e4` | long | far pascal | unsigned int hwnd, unsigned int msg, unsigned int wParam, long lParam |
| tb.obj | `FakeComboProc` | `000e:1d72` | long | far pascal | unsigned int hwnd, unsigned short msg, unsigned short wParam, long lParam |
| tb.obj | `FakeCEProc` | `000e:1df0` | long | far pascal | unsigned int hwnd, unsigned short msg, unsigned short wParam, long lParam |
| ai4.obj | `DoCyberAiTurn` | `0016:002a` | void | far cdecl | _prod near * rgprod |
| ai4.obj | `DoCyberPackets` | `0016:1a78` | void | far cdecl | (none recorded) |
| ai4.obj | `IdGetBestScannerDest` | `0016:282a` | int | far cdecl | _planet far * lppl, int iDir |
| ai4.obj | `FAddPacketToQueue` | `0016:2bc0` | int | far cdecl | _planet far * lppl |
| ai4.obj | `FillProductionQueue` | `0016:2ce2` | void | far cdecl | (none recorded) |
| ai4.obj | `FFillProdMinesAndFactories` | `0016:2d72` | int | far cdecl | _planet far * lppl |
| ai4.obj | `DoCyberFreighter` | `0016:37b0` | void | far cdecl | _fleet far * lpfl, _cyberinfotemp far * lpciPlanTemp |
| ai4.obj | `FEnumDropOffStage1` | `0016:3d00` | int | far cdecl | _planet far * lpplSrc, _planet far * lpplTest |
| ai4.obj | `FEnumDropOffStage2` | `0016:3dfe` | int | far cdecl | _planet far * lpplSrc, _planet far * lpplTest |
| ai4.obj | `FEnumPickUp` | `0016:3f00` | int | far cdecl | _planet far * lpplSrc, _planet far * lpplTest |
| ai4.obj | `FEnumNeedMinerals` | `0016:3f5e` | int | far cdecl | _planet far * lpplSrc, _planet far * lpplTest |
| ai4.obj | `FEnumPktAttack` | `0016:4204` | int | far cdecl | _planet far * lpplSrc, _planet far * lpplTest |
| ai4.obj | `FEnumCalcEnemyPlanets` | `0016:45a6` | int | far cdecl | _planet far * lpplSrc, _planet far * lpplTest |
| ai4.obj | `iBuildCyberStarbase` | `0016:45de` | int | far cdecl | _planet far * lppl |
| ai4.obj | `EnsureCyberAiShdefs` | `0016:4826` | void | far cdecl | int iroCur |
| ai4.obj | `iAddAttackFleet` | `0016:4eba` | int | far cdecl | _planet far * lppl, int iAttackStr, int iBestDestroyer, int iBestBattle, int iBestSBDefender |
| ai4.obj | `TargetCyberArmada` | `0016:51a4` | void | far cdecl | _fleet far * lpfl |
| ai3.obj | `DoMacintiAiTurn` | `0015:0008` | void | far cdecl | _prod near * rgprod |
| ai3.obj | `EnsureMacintiShdefs` | `0015:2b3c` | void | far cdecl | (none recorded) |
| ai3.obj | `FRetargetMiner` | `0015:336c` | int | far cdecl | _fleet far * lpfl |
| ai3.obj | `IdTargetMacFreighter` | `0015:3524` | int | far cdecl | _fleet far * lpfl |
| ai3.obj | `TargetMacArmada` | `0015:3e3a` | void | far cdecl | _fleet far * lpfl |
| ai3.obj | `FPotentMacWarFleet` | `0015:42ec` | int | far cdecl | _fleet far * lpfl, int near * pcEquiv |
| ai2.obj | `DoMaidAiTurn` | `0014:0000` | void | far cdecl | _prod near * rgprod |
| ai2.obj | `FPotentISWarFleet` | `0014:012e` | int | far cdecl | _fleet far * lpfl, int iPotency |
| ai2.obj | `DoAutomitronAiTurn` | `0014:01e0` | void | far cdecl | _prod near * rgprod |
| ai2.obj | `EnsureISShdefs` | `0014:1938` | void | far cdecl | int iroCur |
| ai2.obj | `DoRototillAiTurn` | `0014:1e22` | void | far cdecl | _prod near * rgprod |
| ai2.obj | `EnsureCAShdefs` | `0014:3020` | void | far cdecl | int iroCur |
| ship2.obj | `ZipOrderDlg` | `0011:0000` | int | far pascal | unsigned int hwnd, unsigned int message, unsigned short wParam, long lParam |
| ship2.obj | `EnableZipBtns` | `0011:0832` | void | far cdecl | unsigned int hwnd, int iSel |
| ship2.obj | `RenameZipDlg` | `0011:0880` | int | far pascal | unsigned int hwnd, unsigned int message, unsigned short wParam, long lParam |
| ship2.obj | `RenameDlg` | `0011:0a68` | int | far pascal | unsigned int hwnd, unsigned int message, unsigned short wParam, long lParam |
| ship2.obj | `FStargateJump` | `0011:0cfe` | int | far cdecl | _fleet far * lpfl, int isbsSrc, int isbsDst, int dDist |
| ship2.obj | `MdCalcStargateDamage` | `0011:152e` | int | far cdecl | int isbsSrc, int isbsDst, int dDist, int wt, int near * ppctDmg |
| ship2.obj | `KillUsedWaypoints` | `0011:189a` | void | far cdecl | (none recorded) |
| ship2.obj | `NoAutoTrackFleet` | `0011:1d32` | void | far cdecl | _fleet far * lpflTarget |
| ship2.obj | `AutoRouteFleet` | `0011:1e52` | void | far cdecl | _fleet far * lpfl, _planet far * lppl |
| ship2.obj | `FColonizer` | `0011:227c` | int | far cdecl | _fleet far * lpfl |
| ship2.obj | `FScout` | `0011:2322` | int | far cdecl | _fleet far * lpfl |
| ship2.obj | `AutoFleetOrder` | `0011:23c8` | void | far cdecl | _fleet far * lpfl, _planet far * lppl |
| ship2.obj | `CMineFromLpfl` | `0011:25d2` | long | far cdecl | _fleet far * lpfl |
| ship2.obj | `PctTerraFromLpfl` | `0011:275c` | long | far cdecl | _fleet far * lpfl |
| ship2.obj | `CLayMinesFromLpfl` | `0011:2886` | long | far cdecl | _fleet far * lpfl, int iType, int ishdef |
| ship2.obj | `CMineSweepFromLpfl` | `0011:2b26` | long | far cdecl | _fleet far * lpfl |
| ship2.obj | `CMineSweepFromLphul` | `0011:2bfa` | long | far cdecl | _hul far * lphul |
| ship2.obj | `PctCloakFromLpfl` | `0011:2d5e` | int | far cdecl | _fleet far * lpfl |
| ship2.obj | `CPtsCloakFromLphs` | `0011:3170` | int | far cdecl | _hs far * lphs |
| ship2.obj | `MergeFleetsDlg` | `0011:3376` | int | far pascal | unsigned int hwnd, unsigned int msg, unsigned short wParam, long lParam |
| ship2.obj | `MarkTechsSeen` | `0011:36b6` | void | far cdecl | _hul far * lphul, int iplr |
| thing.obj | `LpthNew` | `0023:0000` | _thing far * | far cdecl | int iplr, int ith |
| thing.obj | `FreeLpth` | `0023:0224` | void | far cdecl | _thing far * lpth |
| thing.obj | `CPlanetsInCircle` | `0023:02a0` | int | far cdecl | tagPOINT pt, long r2 |
| thing.obj | `DrawThingGauge` | `0023:044e` | void | far cdecl | unsigned int hdc, tagRECT near * prc, _thing far * lpth, int md |
| thing.obj | `IValidateWormholePos` | `0023:064c` | int | far cdecl | _thing far * lpthWorm |
| thing.obj | `PctWormholeMoves` | `0023:0adc` | int | far cdecl | _thing far * lpth |
| thing.obj | `DoThingInteractions` | `0023:0b3a` | void | far cdecl | int fPostMove |
| thing.obj | `IdmGiveTraderPart` | `0023:1a96` | int | far cdecl | unsigned short grbitTrader, int iplr, unsigned short near * piGoto |
| report.obj | `ReportDlg` | `0022:0018` | long | far pascal | unsigned int hwnd, unsigned int msg, unsigned short wParam, long lParam |
| report.obj | `SetHScrollBar` | `0022:09e8` | void | far cdecl | (none recorded) |
| report.obj | `DrawReport` | `0022:0bae` | void | far cdecl | unsigned int hwnd, unsigned int hdc, tagRECT near * prc |
| report.obj | `ScoreXDlg` | `0022:0f66` | int | far pascal | unsigned int hwnd, unsigned int message, unsigned short wParam, long lParam |
| report.obj | `InitScoreDlg` | `0022:13b6` | void | far cdecl | unsigned int hwnd, int fVictory |
| report.obj | `DrawVCReport` | `0022:168e` | void | far cdecl | unsigned int hdc |
| report.obj | `DrawScoreReport` | `0022:1e0c` | void | far cdecl | unsigned int hdc |
| report.obj | `DrawHistoryReport` | `0022:2494` | void | far cdecl | unsigned int hdc |
| report.obj | `LFetchScoreXVal` | `0022:2f94` | long | far cdecl | _scorex far * lpsx, int iVal |
| report.obj | `DxReportColHdr` | `0022:305e` | int | far cdecl | int irpt, int iCol, char near * psz, unsigned int hdc |
| report.obj | `DrawReportItem` | `0022:3398` | void | far cdecl | unsigned int hdc, tagRECT near * prc, int irpt, int irow, int icol |
| report.obj | `DrawMineralItem` | `0022:4ebe` | void | far cdecl | unsigned int hdc, int x, int y, int iMineral, long l |
| report.obj | `PszGetDestName` | `0022:4f60` | char near * | far cdecl | _fleet far * lpfl, unsigned int hdc |
| report.obj | `FDestIsWP0` | `0022:50b4` | int | far cdecl | _fleet far * lpfl |
| report.obj | `PszGetETA` | `0022:51a8` | char near * | far cdecl | unsigned int hdc, _fleet far * lpfl, int near * pcYears |
| report.obj | `PszGetTaskName` | `0022:53b8` | char near * | far cdecl | _fleet far * lpfl, int near * picr |
| report.obj | `SortReportCache` | `0022:589c` | void | far cdecl | int irpt, int icol |
| report.obj | `ICompReport` | `0022:5bb8` | int | far cdecl | void near * arg1, void near * arg2 |
| report.obj | `ReportColumnPopup` | `0022:74d4` | void | far cdecl | tagPOINT pt, int icol, int fRightBtn |
| report.obj | `InvalidateReport` | `0022:7af6` | void | far cdecl | int irpt, int fReload |
| report.obj | `ExecuteReportClick` | `0022:7cd6` | void | far cdecl | tagPOINT pt, int irpt, int icol, int irow |
| report.obj | `DumpUniverse` | `0022:851e` | void | far cdecl | (none recorded) |
| report.obj | `DumpPlanets` | `0022:86e4` | void | far cdecl | (none recorded) |
| report.obj | `DumpFleets` | `0022:9530` | void | far cdecl | (none recorded) |
| report.obj | `PrintMapDlg` | `0022:a1c2` | int | far pascal | unsigned int hwnd, unsigned int msg, unsigned short wParam, long lParam |
| tutor2.obj | `CchTutorString` | `0021:5a14` | int | far cdecl | char near * pchOut, int idt |
| tutor.obj | `TutorDlg` | `0020:0000` | int | far pascal | unsigned int hwnd, unsigned int message, unsigned short wParam, long lParam |
| tutor.obj | `PanicDlg` | `0020:026a` | int | far pascal | unsigned int hwnd, unsigned int message, unsigned short wParam, long lParam |
| tutor.obj | `ShowTutor` | `0020:0374` | void | far cdecl | int fShow |
| tutor.obj | `DrawTutorText` | `0020:03c0` | void | far cdecl | unsigned int hwnd |
| tutor.obj | `StartTutor` | `0020:06b4` | void | far cdecl | int fRestart |
| tutor.obj | `AdvanceTutor` | `0020:0a30` | void | far cdecl | (none recorded) |
| tutor.obj | `EndTutor` | `0020:0c02` | void | far cdecl | int fClose |
| tutor.obj | `SaveGameState` | `0020:0c90` | void | far cdecl | (none recorded) |
| tutor.obj | `RestoreGameState` | `0020:0e1c` | void | far cdecl | (none recorded) |
| tutor.obj | `FAskKillTutor` | `0020:0f36` | int | far cdecl | (none recorded) |
| tutor.obj | `FTutorTaskDone` | `0020:0fbc` | int | far cdecl | (none recorded) |
| tutor.obj | `FCheckZip` | `0020:6460` | int | far cdecl | int iZip, _itemaction far * lpiaGoal, int ids |
| tutor.obj | `FCheckTemplate` | `0020:666e` | int | far cdecl | int iTemplate |
| tutor.obj | `TutorError` | `0020:67ae` | void | far cdecl | int idsError |
| tutor.obj | `FCheckScanner` | `0020:685c` | int | far cdecl | int md, int iZoom |
| tutor.obj | `FCheckFleetName` | `0020:690a` | int | far cdecl | int id, int ids |
| tutor.obj | `FCheckSummary` | `0020:69e2` | int | far cdecl | unsigned int grobj, int id |
| tutor.obj | `FCheckSelection` | `0020:6af4` | int | far cdecl | unsigned int grobj, int id |
| tutor.obj | `FCheckMessages` | `0020:6c48` | int | far cdecl | int imsg, int idm, int fFilter |
| tutor.obj | `FCheckResearch` | `0020:6da4` | int | far cdecl | int iTech, int iTechNext, int pct |
| tutor.obj | `FCheckFleetWP` | `0020:6df4` | int | far cdecl | unsigned int ifl, int iord, unsigned int grobj, int id, unsigned int grTask, unsigned int iWarp |
| tutor.obj | `FCheckPlanetRoute` | `0020:6f86` | int | far cdecl | int idpl, int idplRoute |
| tutor.obj | `FCheckLayingWP` | `0020:6ff4` | int | far cdecl | unsigned int ifl, int iord, int id, int iYears |
| tutor.obj | `FCheckColonizeWP` | `0020:70c0` | int | far cdecl | unsigned int ifl, int id, unsigned int iWarp |
| tutor.obj | `FCheckPatrolWP` | `0020:71ac` | int | far cdecl | unsigned int ifl, int iord, int id, unsigned int iWarp, unsigned int iPlan, unsigned int iDist |
| tutor.obj | `FCheckXferWP` | `0020:7280` | int | far cdecl | unsigned int ifl, int iord, int id, unsigned int iWarp, _itemaction far * lpiaGoal |
| tutor.obj | `FCheckQueue` | `0020:7442` | int | far cdecl | int ipl, int iprod, unsigned int grobj, unsigned int iItem, unsigned int cItem, unsigned short fNoResearch |
| tutor.obj | `FCheckBtlPlan` | `0020:760a` | int | far cdecl | int ibp, unsigned short imdTarget, unsigned short fSpread, unsigned short fBomb, unsigned short fDump, unsigned short mdUnarmed, unsigned short mdScout, unsigned short mdWar, unsigned short mdBomber |
| tutor.obj | `FCheckCargo` | `0020:7664` | int | far cdecl | _fleet far * lpfl, int wtMin1, int wtMin2, int wtMin3, int wtColonists |
| tutor.obj | `FCheckBuilderPart` | `0020:77d8` | int | far cdecl | int iSlot, _hs near * phs, unsigned int cInit |
| tutor.obj | `FCheckShipBuilder` | `0020:7964` | int | far cdecl | int iCategory, int iShip |
| tutor.obj | `FTutorialEnabledShipBuilder` | `0020:79f6` | int | far cdecl | int itutsbAction |
| tutor.obj | `FOKMergeDialog` | `0020:81da` | int | far cdecl | (none recorded) |
| strings.obj | `PszGetCompressedString` | `0003:743a` | char near * | far cdecl | int ids |
| aiutil.obj | `FCreateAiShdef` | `0013:012c` | int | far cdecl | int ishdef, int ihul, unsigned char far * rgaip |
| aiutil.obj | `FGetAIPart` | `0013:043e` | int | far cdecl | int aip, _part near * ppart |
| aiutil.obj | `PickANameAndBmp` | `0013:055a` | void | far cdecl | _shdef near * pshdef, int ids, int cids, int ibmpStart |
| aiutil.obj | `FChangeAiShdef` | `0013:08a2` | int | far cdecl | _shdef near * pshdef, int ishdef |
| aiutil.obj | `XferAiSupply` | `0013:0ad0` | int | far cdecl | int grobjSrc, int idSrc, int grobjDst, int idDst, int iSupply, int cQuan |
| aiutil.obj | `XferAiTroopers` | `0013:0bc2` | int | far cdecl | int idSrc, int idDst, int cQuan |
| aiutil.obj | `FColonizeAiFleet` | `0013:0c78` | int | far cdecl | _fleet far * lpfl, int idPlanet |
| aiutil.obj | `FGotoWormholeAiFleet` | `0013:0d5c` | int | far cdecl | _fleet far * lpfl, _thing far * lpthWorm |
| aiutil.obj | `IdNearestColonizablePlanet` | `0013:0e3e` | int | far cdecl | _fleet far * lpflCol, _thing far * near * plpthWorm |
| aiutil.obj | `LpthWormFind` | `0013:12e6` | _thing far * | far cdecl | tagPOINT near * ppt, long d2 |
| aiutil.obj | `UlFleetPower` | `0013:14de` | unsigned long | far cdecl | _fleet far * lpfl |
| aiutil.obj | `IdNearestUnknownPlanet` | `0013:15a4` | int | far cdecl | _fleet far * lpfl, _thing far * near * plpthWorm |
| aiutil.obj | `AddMinesToBlockedQueues` | `0013:1792` | void | far cdecl | (none recorded) |
| aiutil.obj | `FFleetInField` | `0013:1cf6` | int | far cdecl | _fleet far * lpfl, _thing far * lpth |
| aiutil.obj | `SetAiFleetIdealSpeed` | `0013:1d9e` | void | far cdecl | _fleet far * lpfl, int wtFuelMax, int cMinefields, _thing far * near * rglpth |
| aiutil.obj | `IdTargetAttack` | `0013:1ffe` | int | far cdecl | _fleet far * lpfl, _fleet far * lpflAtk, _fleet far * lpflEnemy, int fOnlyHumans |
| aiutil.obj | `IdTargetFreighter` | `0013:286c` | int | far cdecl | _fleet far * lpflFr, _planet far * lpplHome |
| aiutil.obj | `FSalvageTargetFreighter2` | `0013:395a` | int | far cdecl | _fleet far * lpflFr, int fNeedy, int iWorst, int pctFull, long wtCargoMax, long scoreBest, _thing far * near * plpthBest, int near * pidBest |
| aiutil.obj | `FMoveAiFleet` | `0013:3d0a` | int | far cdecl | _fleet far * lpfl, _order near * pord, int fAppend |
| aiutil.obj | `AddItemToQueue` | `0013:3e50` | void | far cdecl | unsigned int iItem, unsigned int cItem, unsigned int grobj, int mdAddItem |
| aiutil.obj | `IroEnsureAi` | `0013:425a` | int | far cdecl | unsigned char far * lpbRes, int cRes, int near * pishdefSBLatest, int pct |
| aiutil.obj | `KeepFleetsMoving` | `0013:45ca` | void | far cdecl | (none recorded) |
| aiutil.obj | `FShouldWeBuildColonizers` | `0013:476a` | int | far cdecl | int near * pcCol |
| aiutil.obj | `FIsAiAttack` | `0013:4a72` | int | far cdecl | _fleet far * lpfl |
| aiutil.obj | `FIsTurinDroneAiAttack` | `0013:4b9a` | int | far cdecl | _fleet far * lpfl |
| aiutil.obj | `FIsAiTransport` | `0013:4c08` | int | far cdecl | _fleet far * lpfl |
| aiutil.obj | `ValidateStarbaseHistory` | `0013:4cf0` | void | far cdecl | (none recorded) |
| aiutil.obj | `GetResourcesAvailable` | `0013:56d0` | void | far cdecl | _planet far * lppl, long near * rgRes |
| aiutil.obj | `GetProdQCost` | `0013:57c0` | void | far cdecl | _planet far * lppl, long near * rgCost |
| aiutil.obj | `MergeAllShdefs` | `0013:58be` | void | far cdecl | int grbitish |
| aiutil.obj | `LpflFindClosestEnum` | `0013:5bae` | _fleet far * | far cdecl | _fleet far * lpfl, function returning int far * pfn |
| aiutil.obj | `LpplFindClosestEnum` | `0013:5cd4` | _planet far * | far cdecl | _planet far * lppl, function returning int far * pfn |
| aiutil.obj | `LpplFindBestEnum` | `0013:5e06` | _planet far * | far cdecl | _planet far * lppl, function returning int far * pfn |
| aiutil.obj | `IdRandomPlanetNearby` | `0013:5f54` | int | far cdecl | tagPOINT pt, int cDist, int fAvoidStarbases |
| aiutil.obj | `ClearAiCurrentTask` | `0013:60c0` | void | far cdecl | _fleet far * lpfl, int fChangeSel |
| aiutil.obj | `FEnumOurStarbase` | `0013:6110` | int | far cdecl | _planet far * lpplSrc, _planet far * lpplTest |
| aiutil.obj | `FFleetMightHaveTeeth` | `0013:6152` | int | far cdecl | _fleet far * lpfl |
| aiutil.obj | `IdTargetScout` | `0013:61de` | int | far cdecl | _fleet far * lpfl, _fleet far * lpflAtk, _fleet far * lpflEnemy, int fOnlyHumans, _thing far * near * plpthWorm |
| aiutil.obj | `MarkPlanetsUnderAttack` | `0013:6896` | void | far cdecl | (none recorded) |
| aiutil.obj | `FixPlanetsUnderAttack` | `0013:69e6` | void | far cdecl | _prod near * rgprod |
| aiutil.obj | `QuickBuildDefenses` | `0013:6a7e` | void | far cdecl | _planet far * lppl, _prod near * rgprod |
| aiutil.obj | `IdplFindClosestStarbase` | `0013:6e04` | int | far cdecl | _fleet far * lpfl, int fBigOnes |
| aiutil.obj | `FMoveToNearestStarbase` | `0013:6f7e` | int | far cdecl | _fleet far * lpfl, int fBigOnes |
| aiutil.obj | `MoveToNearestPlanetOrEnemy` | `0013:7014` | void | far cdecl | _fleet far * lpfl, int dEnemyRange |
| aiutil.obj | `EnsureAiStarbaseDesigns` | `0013:7222` | void | far cdecl | (none recorded) |
| aiutil.obj | `EnsureMacintiStarbaseDesigns` | `0013:76e4` | void | far cdecl | unsigned char near * rgSB |
| aiutil.obj | `FCreateAiStarbase` | `0013:7bca` | int | far cdecl | int ishdef, int iLevel, int aisb, int isb |
| aiutil.obj | `FAIFling` | `0013:7dd6` | int | far cdecl | _planet far * lppl, long near * rgResAvail |
| aiutil.obj | `IshdefAiSBLatestOF` | `0013:8458` | int | far cdecl | (none recorded) |
| aiutil.obj | `IshdefAiSBLatest` | `0013:84be` | int | far cdecl | (none recorded) |
| aiutil.obj | `QueueAiStarbases` | `0013:8524` | void | far cdecl | _prod near * rgprod, int ishdefSBLatest |
| aiutil.obj | `FUpgradeAiStarbase` | `0013:882a` | int | far cdecl | _planet far * lppl, int ishdefSBLatest |
| aiutil.obj | `FQueueAiTerraforming` | `0013:8d28` | int | far cdecl | _planet far * lppl, long near * rgResAvail, long near * rgResCost |
| aiutil.obj | `FQueueAiScanner` | `0013:90d6` | int | far cdecl | _planet far * lppl, long near * rgResAvail, long near * rgResCost |
| aiutil.obj | `FQueueAiDefenses` | `0013:939a` | int | far cdecl | _planet far * lppl, long near * rgResAvail, long near * rgResCost |
| aiutil.obj | `HandleBasicAiTasks` | `0013:95a4` | void | far cdecl | int iroCur, _prod near * rgprod, int ishdefSBLatest, long near * rgResAvail, long near * rgResCost |
| aiutil.obj | `SplitOutShdefs` | `0013:98d8` | void | far cdecl | unsigned char near * rgbIsh |
| aiutil.obj | `CheckAiShdefStatus` | `0013:9b10` | int | far cdecl | int ishBeg, int ishEnd, unsigned short cRecyclePeriod, int near * piLatest, unsigned char near * rgbOld |
| aiutil.obj | `IncreaseAIMinefieldSizes` | `0013:9c66` | void | far cdecl | (none recorded) |
| aiutil.obj | `FFindBuddyAndJoinUp` | `0013:9d18` | int | far cdecl | _fleet far * lpfl, int ishLo, int ishHi, long lMaxDist1, long lMaxDist2 |
| aiutil.obj | `FShouldPlanetBuildColonizer` | `0013:9f30` | int | far cdecl | _planet far * lpplSrc |
| aiutil.obj | `InitRandomPlanetList` | `0013:a082` | void | far cdecl | (none recorded) |
| ai.obj | `DoAiTurn` | `0012:0000` | void | far cdecl | int iPlayer, unsigned short wMdPlr |
| ai.obj | `DoRobotoidAiTurn` | `0012:0312` | void | far cdecl | _prod near * rgprod |
| ai.obj | `EnsureRobotoidShdefs` | `0012:20ae` | void | far cdecl | (none recorded) |
| ai.obj | `IdTargetArmada` | `0012:288e` | int | far cdecl | _fleet far * lpfl |
| ai.obj | `FPotentRobWarFleet` | `0012:31bc` | int | far cdecl | _fleet far * lpfl, int iPotency |
| ai.obj | `FEnumCalcEnemyFleets` | `0012:325c` | int | far cdecl | _fleet far * lpflSrc, _fleet far * lpflTest |
| ai.obj | `FEnumCalcArmadaDest` | `0012:3286` | int | far cdecl | _planet far * lpplSrc, _planet far * lpplTest |
| ai.obj | `FEnumCalcArmadaHumanDest` | `0012:3406` | int | far cdecl | _planet far * lpplSrc, _planet far * lpplTest |
| ai.obj | `DoTurinDroneAiTurn` | `0012:3670` | void | far cdecl | _prod near * rgprod |
| ai.obj | `EnsureTurinDroneShdefs` | `0012:58ba` | void | far cdecl | int iroCur |
| ai.obj | `FEnumCalcMinerDest` | `0012:5f32` | int | far cdecl | _planet far * lpplSrc, _planet far * lpplTest |
| ai.obj | `FEnumCalcColonistDrop` | `0012:5fc8` | int | far cdecl | _planet far * lpplSrc, _planet far * lpplTest |
| vcr.obj | `BattleVCR` | `001e:0000` | void | far cdecl | int iBattle |
| vcr.obj | `CBattles` | `001e:028c` | int | far cdecl | (none recorded) |
| vcr.obj | `BtlDataGet` | `001e:0362` | _btldata far * | far cdecl | int i |
| vcr.obj | `CBattleUnits` | `001e:0450` | long | far cdecl | _btldata far * lpbd, unsigned short grbitBU |
| vcr.obj | `CBattleKills` | `001e:062e` | long | far cdecl | _btldata far * lpbd, int fOurDead |
| vcr.obj | `LdpFromItokDv` | `001e:07a8` | long | far cdecl | int itok, _dv far * lpdv |
| vcr.obj | `SetVCRBoard` | `001e:08d8` | int | far cdecl | int iStep |
| vcr.obj | `VCRDlg` | `001e:0e90` | int | far pascal | unsigned int hwnd, unsigned int message, unsigned short wParam, long lParam |
| vcr.obj | `GetVCRStats` | `001e:193e` | void | far cdecl | int itok, long near * pdpArmor, _dv near * pdv, long near * pdpShields, int near * pcsh |
| vcr.obj | `DrawVCR` | `001e:1c62` | void | far cdecl | unsigned int hdc, int iStart, int iEnd |
| vcr.obj | `Delay` | `001e:3a3e` | void | far cdecl | int ctick |
| vcr.obj | `AnimateAttack` | `001e:3ac2` | void | far cdecl | unsigned int hdc |
| vcr.obj | `PopupVCRMenu` | `001e:4518` | int | far cdecl | unsigned int hwnd, int x, int y, unsigned char brc |
| vcr.obj | `EnableVCRButtons` | `001e:48f6` | void | far cdecl | (none recorded) |
| utilgen.obj | `PushRandom` | `0009:1440` | void | far cdecl | long lNew1, long lNew2 |
| utilgen.obj | `PopRandom` | `0009:14a2` | void | far cdecl | (none recorded) |
| utilgen.obj | `Randomize` | `0009:15ea` | void | far cdecl | unsigned long dw |
| utilgen.obj | `Randomize2` | `0009:165a` | void | far cdecl | unsigned long dw |
| utilgen.obj | `Random` | `0009:16d2` | int | far cdecl | int c |
| utilgen.obj | `RandomSeedDlg` | `0009:188a` | int | far pascal | unsigned int hwnd, unsigned int message, unsigned short wParam, long lParam |
| utilgen.obj | `GetFileSeeds` | `0009:1a4e` | void | far cdecl | long near * pl1, long near * pl2 |
| utilgen.obj | `SetFileSeeds` | `0009:1a7c` | void | far cdecl | long l1, long l2 |
| utilgen.obj | `SetFileXorStream` | `0009:1aa6` | void | far cdecl | long lid, short lSalt, int turn, int iPlayer, int fCrippled |
| utilgen.obj | `LGetNextFileXor` | `0009:1b54` | long | far cdecl | (none recorded) |
| utilgen.obj | `XorFileBuf` | `0009:1cc4` | void | far cdecl | char near * rgb, int cb |
| utilgen.obj | `ICompLong` | `0009:1d74` | int | far cdecl | void near * arg1, void near * arg2 |
| utilgen.obj | `PszGetCompressedPlanet` | `0009:1d96` | char near * | far cdecl | int id |
| utilgen.obj | `OutputFileString` | `0009:1f64` | void | far cdecl | char near * szFile, char near * sz |
| utilgen.obj | `CopyFile` | `0009:1ffa` | void | far cdecl | char near * szSrc, char near * szDst |
| utilgen.obj | `AlertSz` | `0009:2160` | int | far cdecl | char near * sz, int mbType |
| utilgen.obj | `CchGetString` | `0009:221e` | int | far cdecl | int ids, char near * psz |
| utilgen.obj | `PszFromInt` | `0009:2274` | char near * | far cdecl | int i, int near * pcch |
| utilgen.obj | `PszFromLong` | `0009:22b6` | char near * | far cdecl | long l, int near * pcch |
| utilgen.obj | `PszFromLongK` | `0009:22fe` | char near * | far cdecl | long l, int near * pcch |
| utilgen.obj | `CommaFormatLong` | `0009:2440` | int | far cdecl | char near * psz, long l |
| utilgen.obj | `CtrTextOut` | `0009:2534` | void | far cdecl | unsigned int hdc, int x, int y, char near * psz, int cLen |
| utilgen.obj | `DxStreamTextOut` | `0009:2590` | int | far cdecl | unsigned int hdc, int near * px, int y, char near * psz, int cLen, int fPrint |
| utilgen.obj | `WrapTextOut` | `0009:25fe` | void | far cdecl | unsigned int hdc, int near * px, int near * py, char near * psz, int cLen, int xLeft, int dxWidth, int near * pxMax, int fNewLine, int fPrint |
| utilgen.obj | `AddBackTrailingSpaces` | `0009:280c` | void | far cdecl | char near * near * ppch, char near * pchEnd |
| utilgen.obj | `ChopLastWord` | `0009:2842` | void | far cdecl | char near * pBeg, char near * near * ppEnd |
| utilgen.obj | `ChopTrailingSpaces` | `0009:28c6` | void | far cdecl | char near * pBeg, char near * near * ppEnd |
| utilgen.obj | `RcCtrTextOut` | `0009:28fc` | void | far cdecl | unsigned int hdc, tagRECT near * prc, char near * psz, int cLen |
| utilgen.obj | `RightTextOut` | `0009:29d6` | void | far cdecl | unsigned int hdc, int x, int y, char near * psz, int cLen, int dxErase |
| utilgen.obj | `DiaganolTextOut` | `0009:2afa` | void | far cdecl | unsigned int hdc, tagRECT near * prc, char near * psz, int cLen |
| utilgen.obj | `ExpandRc` | `0009:2f0c` | void | far cdecl | tagRECT near * prc, int dx, int dy |
| utilgen.obj | `OffsetRc` | `0009:2f3e` | void | far cdecl | tagRECT near * prc, int dx, int dy |
| utilgen.obj | `BoundPoints` | `0009:2f70` | void | far cdecl | tagRECT near * prc, tagPOINT near * rgpt, int cpt |
| utilgen.obj | `StickyDlgPos` | `0009:3094` | void | far cdecl | unsigned int hwnd, tagPOINT near * ppt, int fInit |
| utilgen.obj | `LDrawGauge` | `0009:31a2` | long | far cdecl | unsigned int hdc, tagRECT near * prc, int cSegs, long near * rgSize, unsigned int near * rghbr, long cTot |
| utilgen.obj | `_Draw3dFrame` | `0009:336a` | void | far cdecl | unsigned int hdc, tagRECT near * prc, int fErase |
| utilgen.obj | `InitBtnTrack` | `0009:354c` | void | far cdecl | _btnt near * pbtnt, unsigned int hwnd, unsigned int hdc, tagRECT near * prc, int btf, int dTimer, int fInitDown, int fNoEndRedraw, char near * szText |
| utilgen.obj | `FTrackBtn` | `0009:364a` | int | far cdecl | _btnt near * pbtnt |
| utilgen.obj | `DrawBtn` | `0009:38b8` | void | far cdecl | unsigned int hdc, tagRECT near * prc, int bt, int fDown, char near * szText |
| utilgen.obj | `FGetMouseMove` | `0009:4146` | int | far cdecl | tagPOINT near * ppt |
| utilgen.obj | `FGetRMouseMove` | `0009:41e0` | int | far cdecl | tagPOINT near * ppt |
| utilgen.obj | `DrawFuzzyBorder` | `0009:427a` | void | far cdecl | unsigned int hdc, tagRECT near * prc |
| utilgen.obj | `FStringFitsScreen` | `0009:43aa` | int | far cdecl | char far * lpsz, int dxMax |
| utilgen.obj | `HbrGet` | `0009:4448` | unsigned int | far cdecl | unsigned long cr |
| utilgen.obj | `FreeHbr` | `0009:4532` | void | far cdecl | unsigned int hbr |
| utilgen.obj | `FCompressUserString` | `0009:45a0` | int | far cdecl | char far * szIn, char far * szOut, int near * pcOut |
| utilgen.obj | `FDecompressUserString` | `0009:46e8` | int | far cdecl | char far * szIn, int cIn, char far * szOut, int near * pcOut |
| utilgen.obj | `NybbleFromCh` | `0009:4880` | int | far cdecl | unsigned char ch |
| utilgen.obj | `ChFromNybble` | `0009:49ea` | char | far cdecl | int nyb |
| utilgen.obj | `DibNumColors` | `0009:4a9a` | unsigned short | far cdecl | void far * pv |
| utilgen.obj | `HpalFromDib` | `0009:4b50` | unsigned int | far cdecl | unsigned int hdib |
| utilgen.obj | `HpalBlackReserved` | `0009:4c9c` | unsigned int | far cdecl | (none recorded) |
| utilgen.obj | `PaletteSize` | `0009:4d60` | unsigned short | far cdecl | void far * pv |
| utilgen.obj | `DibBlt` | `0009:4db8` | int | far cdecl | unsigned int hdc, int x0, int y0, int dx, int dy, unsigned int hdib, int x1, int y1, int dxSrc, int dySrc, long rop |
| utilgen.obj | `DibFromBitmap` | `0009:4e90` | unsigned int | far cdecl | unsigned int hbm, unsigned long biStyle, unsigned short biBits, unsigned int hpal |
| utilgen.obj | `HdibLoadBigResource` | `0009:5220` | unsigned int | far cdecl | int idb |
| utilgen.obj | `ReadBigBlock` | `0009:5310` | int | far cdecl | int hFile, char far * lpBuffer, unsigned long dwSize |
| utilgen.obj | `FIntersectCircleLine` | `0009:53b4` | int | far cdecl | tagPOINT ptL1, tagPOINT ptL2, tagPOINT ptC, long r2, int dMax, int near * pdStart, int near * pdEnd |
| utilgen.obj | `IntToRoman` | `0009:5830` | void | far cdecl | int i, char near * pszOut |
| utilgen.obj | `FCheckPassword` | `0009:58d8` | int | far cdecl | (none recorded) |
| utilgen.obj | `LSaltFromSz` | `0009:59ce` | long | far cdecl | char near * psz |
| utilgen.obj | `PasswordDlg` | `0009:5a72` | int | far pascal | unsigned int hwnd, unsigned int message, unsigned short wParam, long lParam |
| utilgen.obj | `NewPasswordDlg` | `0009:5cba` | int | far pascal | unsigned int hwnd, unsigned int message, unsigned short wParam, long lParam |
| utilgen.obj | `GetDiskSerialNumber` | `0009:5fc0` | unsigned long | far cdecl | (none recorded) |
| utilgen.obj | `ShowProgressGauge` | `0009:636c` | void | far cdecl | (none recorded) |
| utilgen.obj | `HideProgressGauge` | `0009:63b2` | void | far cdecl | (none recorded) |
| utilgen.obj | `UpdateProgressGauge` | `0009:63da` | void | far cdecl | int pctX10 |
| utilgen.obj | `ProgressGaugeDlg` | `0009:647e` | int | far pascal | unsigned int hwnd, unsigned int message, unsigned short wParam, long lParam |
| utilgen.obj | `DrawProgressGauge` | `0009:65f6` | void | far cdecl | unsigned int hdcOrig, int fFull, int iNumOnly |
| utilgen.obj | `LDistance2` | `0009:685c` | long | far cdecl | tagPOINT pt1, tagPOINT pt2 |
| utilgen.obj | `PszGetLine` | `0009:68ba` | char far * | far cdecl | char far * near * ppszBeg |
| utilgen.obj | `CParseNumbers` | `0009:6986` | int | far cdecl | char far * psz, long near * pl, int cMax |
| utilgen.obj | `HfontPrinterCreate` | `0009:6aa6` | unsigned int | far cdecl | unsigned int hdc, int iSize, int near * pdyFont |
| race.obj | `RaceCreationWizard` | `001d:0000` | int | far cdecl | unsigned int hwndParent, int fReadOnly, int fDontWrite |
| race.obj | `RaceWizardDlg1` | `001d:03ac` | int | far pascal | unsigned int hwnd, unsigned int message, unsigned short wParam, long lParam |
| race.obj | `RaceWizardDlg2` | `001d:1060` | int | far pascal | unsigned int hwnd, unsigned int message, unsigned short wParam, long lParam |
| race.obj | `DrawRace2` | `001d:179a` | void | far cdecl | unsigned int hwnd, unsigned int hdc, int iDraw |
| race.obj | `IrcRaceDlgHitTest` | `001d:2164` | int | far cdecl | tagPOINT pt |
| race.obj | `FTrackRaceDlg2` | `001d:2204` | int | far cdecl | unsigned int hwnd, tagPOINT pt, int kbd |
| race.obj | `RaceWizardDlg3` | `001d:2792` | int | far pascal | unsigned int hwnd, unsigned int message, unsigned short wParam, long lParam |
| race.obj | `DrawRace3` | `001d:2aa4` | void | far cdecl | unsigned int hwnd, unsigned int hdc, int iDraw |
| race.obj | `FTrackRaceDlg3` | `001d:2fb8` | int | far cdecl | unsigned int hwnd, tagPOINT pt, int kbd |
| race.obj | `GetRaceStat` | `001d:30d2` | int | far cdecl | _player near * pplr, int iStat |
| race.obj | `SetRaceStat` | `001d:3114` | int | far cdecl | _player near * pplr, int iStat, int iVal |
| race.obj | `GetRaceGrbit` | `001d:3176` | int | far cdecl | _player near * pplr, int ibit |
| race.obj | `SetRaceGrbit` | `001d:31b8` | void | far cdecl | _player near * pplr, int ibit, int fSet |
| race.obj | `RaceWizardDlg4` | `001d:320a` | int | far pascal | unsigned int hwnd, unsigned int message, unsigned short wParam, long lParam |
| race.obj | `RaceWizardDlg5` | `001d:378e` | int | far pascal | unsigned int hwnd, unsigned int message, unsigned short wParam, long lParam |
| race.obj | `RaceWizardDlg6` | `001d:3bae` | int | far pascal | unsigned int hwnd, unsigned int message, unsigned short wParam, long lParam |
| race.obj | `BoundsCheckPlayer` | `001d:40ae` | void | far cdecl | _player near * pplr |
| race.obj | `CAdvantagePoints` | `001d:444c` | int | far cdecl | _player near * pplr |
| race.obj | `LInnateRaceHabitability` | `001d:4c6e` | long | far cdecl | _player near * pplr |
| race.obj | `InvalidateAdvPtsRect` | `001d:54d4` | void | far cdecl | unsigned int hwnd |
| race.obj | `DrawRaceAdvantagePoints` | `001d:55c6` | void | far cdecl | unsigned int hdc, tagRECT near * prc, _player near * pplr |
| race.obj | `IRaceChecksum` | `001d:5888` | unsigned short | far cdecl | _player near * pplr |
| race.obj | `FSaveRace` | `001d:58d4` | int | far cdecl | char near * szFileSuggest, _player near * pplr |
| race.obj | `SetRCWTitle` | `001d:5ab8` | void | far cdecl | unsigned int hwnd, int iStep |
| race.obj | `CreateRandomRace` | `001d:5b08` | void | far cdecl | _player near * pplr |
| race.obj | `PctTrueMaxGrowth` | `001d:65d4` | int | far cdecl | int iplr |
| battle.obj | `RelationsDlg` | `001f:0088` | int | far pascal | unsigned int hwnd, unsigned int message, unsigned short wParam, long lParam |
| battle.obj | `NewPlanNameDlg` | `001f:04ce` | int | far pascal | unsigned int hwnd, unsigned int message, unsigned short wParam, long lParam |
| battle.obj | `BattlePlansDlg` | `001f:0652` | int | far pascal | unsigned int hwnd, unsigned int message, unsigned short wParam, long lParam |
| battle.obj | `FDeleteBattlePlan` | `001f:1706` | int | far cdecl | int iplan, int fWarn |
| battle.obj | `SpankTheCheaters` | `001f:192a` | void | far cdecl | (none recorded) |
| battle.obj | `FFleetHasBombs` | `001f:1e16` | int | far cdecl | _fleet far * lpfl |
| battle.obj | `FHullHasBombs` | `001f:1ec2` | int | far cdecl | _hul far * lphul |
| battle.obj | `FFleetHasTeeth` | `001f:1fbe` | int | far cdecl | _fleet far * lpfl |
| battle.obj | `FHullHasTeeth` | `001f:2072` | int | far cdecl | _hul far * lphul |
| battle.obj | `FFuelTanker` | `001f:20f6` | int | far cdecl | _shdef far * lpshdef |
| battle.obj | `CheckTarget` | `001f:212a` | void | far cdecl | _tok far * ptok, _fleet far * lpfl, int ishdef |
| battle.obj | `FDumpCargo` | `001f:234a` | int | far cdecl | _fleet far * lpfl |
| battle.obj | `DropSalvage` | `001f:24dc` | void | far cdecl | _thing far * near * plpth, long far * rgwtMinerals, int iplr, tagPOINT near * ppt |
| battle.obj | `CplrBattle` | `001f:2952` | int | far cdecl | _fleet far * lpfl, unsigned short near * rggrfAttack, unsigned short near * pgrfPlayer, unsigned short near * pgrfSpectator |
| battle.obj | `SpdOfShip` | `001f:339c` | int | far cdecl | _fleet far * lpfl, int ishdef, _tok far * ptok, int fDumpCargo, _shdef far * lpshdef |
| battle.obj | `LpshdefFromTok` | `001f:388e` | _shdef far * | far cdecl | _tok far * ptok |
| battle.obj | `FCanKillTok` | `001f:391e` | int | far cdecl | _tok far * ptok1, _tok far * ptok2 |
| battle.obj | `DoBattles` | `001f:3a26` | void | far cdecl | int fPostMovement |
| battle.obj | `RegenShield` | `001f:3c16` | void | far cdecl | _tok far * ptok |
| battle.obj | `InitFromHuldef` | `001f:3cba` | int | far cdecl | _hul far * lphul, int near * ppctBC |
| battle.obj | `CheckInitiative` | `001f:3e68` | void | far cdecl | _tok far * ptok |
| battle.obj | `CheckWeapons` | `001f:3ec2` | void | far cdecl | _tok far * ptok, int near * pfDampeningField, unsigned char near * pinit |
| battle.obj | `RandomizeTokOrder` | `001f:44d4` | void | far cdecl | (none recorded) |
| battle.obj | `InitializeBoard` | `001f:45b4` | void | far cdecl | _fleet far * lpfl, int ibrc, unsigned short grfPlayer, unsigned char near * pinit, int near * pinitMin, int near * pinitMac |
| battle.obj | `DzFromBrcBrc` | `001f:4ca8` | int | far cdecl | unsigned char brc1, unsigned char brc2 |
| battle.obj | `DpFromPtokBrcToBrc` | `001f:4d2e` | long | far cdecl | _tok far * ptok, unsigned char brcSrc, unsigned char brcTarget, _tok far * ptokTarget, int fProximity |
| battle.obj | `DzMoveRangeToConsider` | `001f:5312` | int | far cdecl | _tok far * ptok, unsigned short grfAttack, unsigned char near * pbrc |
| battle.obj | `FDoesPrimaryTargetTypeExist` | `001f:56d8` | int | far cdecl | _tok far * ptok, unsigned short grfAttack |
| battle.obj | `FIsTargetOfMdTarget` | `001f:587a` | int | far cdecl | _tok far * ptok, int mdTarget |
| battle.obj | `ScoreGuessBattleDamage` | `001f:598c` | long | far cdecl | _tok far * ptokSrc, unsigned char brc, int fPrimary, unsigned short grfAttack |
| battle.obj | `ScoreFromGiveAndTakeAndTactic` | `001f:5e34` | long | far cdecl | long dpGive, long dpTake, int mdTactic |
| battle.obj | `DxyMoveTokTo` | `001f:5f18` | int | far cdecl | _tok far * ptok, int spdMove, unsigned short grfAttack |
| battle.obj | `CTorpHit` | `001f:6790` | long | far cdecl | long cTorpBase, _tok far * ptok, int pctBase, int pctBC |
| battle.obj | `FAttack` | `001f:69ac` | int | far cdecl | int itokAttacker, int init, _btlrec far * lpbtlrec, unsigned short grfAttack |
| battle.obj | `KillShips` | `001f:7cde` | void | far cdecl | _tok far * ptok, int cshKill, int ishdef, _fleet far * lpfl, int fFallout |
| battle.obj | `CreateSalvage` | `001f:7ee8` | void | far cdecl | _fleet near * pfl, _thing far * near * plpth |
| battle.obj | `FDamageTok` | `001f:81d4` | int | far cdecl | _tok far * ptok, int itok, long near * pdpBeam, long dpTorp, unsigned short grfWeapon, int fShieldsOnly, long near * pcTorp |
| battle.obj | `DxyFromSpdRound` | `001f:8b28` | int | far cdecl | unsigned short spd, int iRound |
| battle.obj | `FDoCoolBattle` | `001f:8bcc` | int | far cdecl | _fleet far * lpfl, int cplr, unsigned short near * rggrfAttack, unsigned short grfPlayer, unsigned short grfSpectator |
| battle.obj | `ITechLearnATech` | `001f:9918` | int | far cdecl | int iplr, int x, int y, int idm, unsigned short near * piGoto |
| battle.obj | `SendBattleMessages` | `001f:9c0e` | void | far cdecl | _fleet far * lpflBtl, int cplr, int idBtl, unsigned short near * rgPlrLosses, int grfPlayer, int cShipsInvolved, int cShdefsInvolved, unsigned short grfSpectator |
| battle.obj | `FAttackPlayer` | `001f:ae06` | int | far cdecl | _fleet far * lpfl, int iplr |
| battle.obj | `DoBombing` | `001f:aefa` | void | far cdecl | (none recorded) |
| memory.obj | `LphbAlloc` | `000d:0000` | _hb far * | far cdecl | unsigned short cb, int ht |
| memory.obj | `LphbReAlloc` | `000d:0108` | _hb far * | far cdecl | _hb far * lphb |
| memory.obj | `FreeHb` | `000d:02d8` | void | far cdecl | _hb far * lphb |
| memory.obj | `ResetHb` | `000d:0348` | void | far cdecl | int ht |
| memory.obj | `LpAlloc` | `000d:03b2` | void far * | far cdecl | unsigned short cb, int ht |
| memory.obj | `LphbFromLpHt` | `000d:058c` | _hb far * | far cdecl | void far * lp, int ht |
| memory.obj | `LpReAlloc` | `000d:0660` | void far * | far cdecl | void far * lp, unsigned short cb, int ht |
| memory.obj | `FreeLp` | `000d:07a8` | void | far cdecl | void far * lp, int ht |
| memory.obj | `LpplReAlloc` | `000d:0836` | _pl far * | far cdecl | _pl far * lppl, unsigned short cAlloc |
| memory.obj | `LpplAlloc` | `000d:088c` | _pl far * | far cdecl | unsigned short cbItem, unsigned short cAlloc, int ht |
| memory.obj | `FreePl` | `000d:0918` | void | far cdecl | _pl far * lppl |
| research.obj | `ResearchDlg` | `001c:0000` | int | far pascal | unsigned int hwnd, unsigned int message, unsigned short wParam, long lParam |
| research.obj | `DrawResearchDlg` | `001c:090a` | void | far cdecl | unsigned int hwnd, unsigned int hdc, tagRECT near * prc, int grbitDraw |
| research.obj | `FTrackResearchDlg` | `001c:1a8c` | int | far cdecl | unsigned int hwnd, int x, int y, int fkb |
| research.obj | `GetTechLevelCost` | `001c:1dba` | long | far cdecl | int iTech, int iLevel, int iplr |
| research.obj | `BrowserDlg` | `001c:1ed8` | int | far pascal | unsigned int hwnd, unsigned int message, unsigned short wParam, long lParam |
| research.obj | `BrowserWndProc` | `001c:2876` | long | far pascal | unsigned int hwnd, unsigned int message, unsigned int wParam, long lParam |
| research.obj | `DisplayComponentInfo` | `001c:2ac6` | void | far cdecl | unsigned int hdc, int dx, int dy, _part near * ppart |
| research.obj | `ProjectedResearchSpending` | `001c:65ae` | long | far cdecl | long pct |
| research.obj | `CostOfDevelopingItem` | `001c:66e0` | long | far cdecl | char far * rgTech |
| research.obj | `FShouldPartBeHidden` | `001c:68c4` | int | far cdecl | _part near * ppart |
| produce.obj | `ChangeProduction` | `001b:0000` | int | far cdecl | int fClear |
| produce.obj | `InitProduction` | `001b:015e` | void | far cdecl | _prod near * rgprod |
| produce.obj | `FinishProduction` | `001b:1090` | void | far cdecl | int fWrite |
| produce.obj | `ProductionDlg` | `001b:1204` | int | far pascal | unsigned int hwnd, unsigned int message, unsigned short wParam, long lParam |
| produce.obj | `ProdCommandHandler` | `001b:1994` | void | far cdecl | unsigned int hwnd, unsigned short wParam, long lParam |
| produce.obj | `InitializeProductionDlg` | `001b:3430` | void | far cdecl | unsigned int hwnd |
| produce.obj | `DrawProductionDlg` | `001b:35dc` | void | far cdecl | unsigned int hwnd, unsigned int hdc, tagRECT near * prc, int iDraw |
| produce.obj | `FillProdSrcLB` | `001b:3b00` | void | far cdecl | unsigned int hwndLB, int mdFill |
| produce.obj | `PszNameProdItem` | `001b:3c92` | char near * | far cdecl | _prod far * lpprod |
| produce.obj | `GetProductionCosts` | `001b:3f20` | void | far cdecl | _planet far * lppl, _prod far * lpprod, unsigned long near * rgCost, int iplr, int fOnlyOne |
| produce.obj | `EstimateItemProdSched` | `001b:4f40` | void | far cdecl | _planet far * lppl, PLPROD far * lpplprod, int iItem, int near * piFirst, int near * piLast |
| produce.obj | `ZipProdDlg` | `001b:5490` | int | far pascal | unsigned int hwnd, unsigned int message, unsigned short wParam, long lParam |
| produce.obj | `EnableZipProdBtns` | `001b:5df0` | void | far cdecl | unsigned int hwnd, int iSel |
| produce.obj | `FillZipProdLB` | `001b:5e58` | void | far cdecl | unsigned int hwndDlg, _zipprodq near * pzpq |
| parts.obj | `LphuldefSBFromId` | `0002:510a` | _huldef far * | far cdecl | int id |
| parts.obj | `LphuldefFromId` | `0002:512c` | _huldef far * | far cdecl | int id |
| parts.obj | `LpengineFromId` | `0002:5168` | _engine far * | far cdecl | int id |
| parts.obj | `LpscannerFromId` | `0002:518a` | _scanner far * | far cdecl | int id |
| parts.obj | `LpshdefT` | `0002:51ac` | _shdef far * | far cdecl | (none recorded) |
| parts.obj | `LpshdefSBT` | `0002:51c4` | _shdef far * | far cdecl | (none recorded) |
| parts.obj | `LpplanetaryFromId` | `0002:51dc` | _planetary far * | far cdecl | int id |
| parts.obj | `FLookupPartX` | `0002:51fe` | int | far cdecl | _part near * ppart, unsigned short grhst, unsigned short iItem |
| parts.obj | `FLookupPart` | `0002:524e` | int | far cdecl | _part near * ppart |
| parts.obj | `LookupBestPlanetaryScanner` | `0002:60be` | void | far cdecl | _part near * ppart |
| parts.obj | `TechStatus` | `0002:6148` | int | far cdecl | char far * rgTech |
| build.obj | `ShipBuilder` | `001a:008c` | int | far cdecl | tagPOINT ptDlgSize |
| build.obj | `ShowMainControls` | `001a:0160` | void | far cdecl | unsigned int hwnd, int sw |
| build.obj | `FCheckQueuedShip` | `001a:027c` | int | far cdecl | unsigned int hwnd, _shdef far * lpshdef, int fEdit |
| build.obj | `SlotDlg` | `001a:0550` | int | far pascal | unsigned int hwnd, unsigned int message, unsigned short wParam, long lParam |
| build.obj | `DrawSlotDlg` | `001a:2650` | void | far cdecl | unsigned int hwnd, unsigned int hdc, tagRECT near * prc, int iDraw |
| build.obj | `FTrackSlot` | `001a:306a` | int | far cdecl | unsigned int hwnd, int x, int y, int fkb, int fListBox, int fRightBtn |
| build.obj | `DrawBuildSelComp` | `001a:3ab2` | void | far cdecl | unsigned int hwnd, unsigned int hdc, int iDraw |
| build.obj | `PctJammerFromHul` | `001a:42e6` | int | far cdecl | _hul far * lphul |
| build.obj | `DrawBuildSelHull` | `001a:451e` | void | far cdecl | unsigned int hwnd, unsigned int hdc, int iDraw, tagRECT near * prc |
| build.obj | `SetBuildSelection` | `001a:53da` | void | far cdecl | int iSrc |
| build.obj | `IDropPart` | `001a:5476` | int | far cdecl | tagPOINT pt, _hs hsSrc, int iSrc, int fNoModify |
| build.obj | `DrawDlgLBEntireItem` | `001a:59f0` | void | far cdecl | tagDRAWITEMSTRUCT far * lpdis, int inflate |
| build.obj | `NthValidShdef` | `001a:5c06` | _shdef far * | far cdecl | int n |
| build.obj | `NthValidEnemyShdef` | `001a:5cf4` | _shdef far * | far cdecl | int n |
| build.obj | `FillBuildDD` | `001a:5e80` | void | far cdecl | unsigned int hwndDD, int md |
| build.obj | `FillBuildPartsLB` | `001a:63e8` | void | far cdecl | unsigned int hwndLB, int grbit |
| build.obj | `UpdateSlotGlobals` | `001a:6528` | void | far cdecl | (none recorded) |
| build.obj | `IEmptyBmpFromGrhst` | `001a:6716` | int | far cdecl | int grhst |
| build.obj | `FakeListProc` | `001a:6758` | long | far pascal | unsigned int hwnd, unsigned short msg, unsigned short wParam, long lParam |
| build.obj | `MakeNewName` | `001a:694c` | void | far cdecl | char far * lpsz |
| build.obj | `KillQueuedMassPackets` | `001a:6a52` | void | far cdecl | _planet far * lppl |
| build.obj | `KillQueuedShips` | `001a:6c2a` | void | far cdecl | _planet far * lppl |
| popup.obj | `PopupWndProc` | `0019:0000` | long | far pascal | unsigned int hwnd, unsigned int message, unsigned int wParam, long lParam |
| popup.obj | `FIsPopupHullType` | `0019:0148` | int | far cdecl | int ishdef |
| popup.obj | `DrawPopup` | `0019:01c0` | void | far cdecl | unsigned int hwnd, unsigned int hdc |
| popup.obj | `Popup` | `0019:0c7c` | void | far cdecl | unsigned int hwnd, int x, int y |
| popup.obj | `PopupMenu` | `0019:136c` | int | far cdecl | unsigned int hwnd, int x, int y, int cString, long near * rgids, char near * near * rgsz, int iChecked, int fRightBtn |
| popup.obj | `PtDisplayPlanetStateInfo` | `0019:1938` | tagPOINT | far cdecl | unsigned int hdc, int fPrint |
| popup.obj | `PtDisplayPlanetPopInfo` | `0019:228e` | tagPOINT | far cdecl | unsigned int hdc, int fPrint |
| popup.obj | `PtDisplayZipOrdInfo` | `0019:2d66` | tagPOINT | far cdecl | unsigned int hdc, int xCtr, int fPrint |
| popup.obj | `PtDisplayFactoryMineInfo` | `0019:3110` | tagPOINT | far cdecl | unsigned int hdc, int dx, int fPrint |
| popup.obj | `PtDisplayResourceInfo` | `0019:3378` | tagPOINT | far cdecl | unsigned int hdc, int dx, int fPrint |
| popup.obj | `PtDisplayString` | `0019:355c` | tagPOINT | far cdecl | unsigned int hdc, int dx, int fPrint |
| msg.obj | `MessageWndProc` | `0007:5c92` | long | far pascal | unsigned int hwnd, unsigned int message, unsigned int wParam, long lParam |
| msg.obj | `SetMsgTitle` | `0007:7218` | void | far cdecl | unsigned int hwnd |
| msg.obj | `IMsgNext` | `0007:7808` | int | far cdecl | int fFilteredOnly |
| msg.obj | `IMsgPrev` | `0007:78d8` | int | far cdecl | int fFilteredOnly |
| msg.obj | `DecorateMsgTitleBar` | `0007:799c` | void | far cdecl | unsigned int hdc, tagRECT near * prc |
| msg.obj | `HtMsgBox` | `0007:7d8c` | int | far cdecl | tagPOINT pt |
| msg.obj | `FSendPlrMsg2` | `0007:7eaa` | int | far cdecl | int iPlr, int iMsg, int iObj, int p1, int p2 |
| msg.obj | `FSendPlrMsg` | `0007:7ee8` | int | far cdecl | int iPlr, int iMsg, int iObj, int p1, int p2, int p3, int p4, int p5, int p6, int p7 |
| msg.obj | `FSendPrependedPlrMsg` | `0007:7f80` | int | far cdecl | int iPlr, int iMsg, int iObj, int p1, int p2, int p3, int p4, int p5, int p6, int p7 |
| msg.obj | `PackageUpMsg` | `0007:802a` | int | far cdecl | unsigned char near * pb, int iPlr, int iMsg, int iObj, int p1, int p2, int p3, int p4, int p5, int p6, int p7 |
| msg.obj | `FSendPlrMsg2XGen` | `0007:823a` | int | far cdecl | int fPrepend, int iMsg, int iObj, int p1, int p2 |
| msg.obj | `IdmGetMessageN` | `0007:8412` | int | far cdecl | int iMsg |
| msg.obj | `FGetNMsgbig` | `0007:8444` | int | far cdecl | int iMsg, _msgbig near * pmb |
| msg.obj | `PszGetMessageN` | `0007:8580` | char near * | far cdecl | int iMsg |
| msg.obj | `PszFormatString` | `0007:85cc` | char near * | far cdecl | char near * pszFormat, int far * pParamsReal |
| msg.obj | `MsgDlg` | `0007:8f68` | int | far pascal | unsigned int hwnd, unsigned int message, unsigned short wParam, long lParam |
| msg.obj | `PszFormatMessage` | `0007:9220` | char near * | far cdecl | int idm, int far * pParams |
| msg.obj | `PszFormatIds` | `0007:924c` | char near * | far cdecl | int ids, int far * pParams |
| msg.obj | `FRemovePlayerMessage` | `0007:9278` | int | far cdecl | int iPlr, int iMsg, int iObj |
| msg.obj | `FFindPlayerMessage` | `0007:932a` | int | far cdecl | int iPlr, int iMsg, int iObj |
| msg.obj | `MarkPlanetsPlayerLost` | `0007:93c6` | void | far cdecl | int iPlayer |
| msg.obj | `MarkPlayersThatSentMsgs` | `0007:9604` | void | far cdecl | int iPlayer |
| msg.obj | `WritePlayerMessages` | `0007:9702` | void | far cdecl | int iPlayer |
| msg.obj | `ResetMessages` | `0007:98d6` | void | far cdecl | (none recorded) |
| msg.obj | `ReadPlayerMessages` | `0007:994a` | void | far cdecl | (none recorded) |
| msg.obj | `FFinishPlrMsgEntry` | `0007:9bd6` | int | far cdecl | int dInc |
| msg.obj | `PszGetCompressedMessage` | `0007:9eb8` | char near * | far cdecl | int idm |
| msg.obj | `SetFilteringGroups` | `0007:a018` | void | far cdecl | int idm, int fSet |
| create.obj | `InitBattlePlan` | `0010:00c0` | void | far cdecl | _btlplan far * lpbtlplan, int iplan, int iplr |
| create.obj | `GenerateWorld` | `0010:0136` | int | far cdecl | int fBatchMode |
| create.obj | `CreateStartupShip` | `0010:4690` | int | far cdecl | int iplr, int idPlanet, int ishdef, int fAddShdef |
| create.obj | `GenNewGameFromFile` | `0010:4812` | int | far cdecl | char near * pszFile |
| create.obj | `CreateTutorWorld` | `0010:5e5e` | void | far cdecl | (none recorded) |
| create.obj | `NewGameWizard` | `0010:6022` | void | far cdecl | unsigned int hwnd, int fReadOnly |
| create.obj | `InitNewGamePlr` | `0010:6e44` | void | far cdecl | int iStepMaxSoFar, int lvlAi |
| create.obj | `InitNewGame3` | `0010:74f8` | void | far cdecl | (none recorded) |
| create.obj | `FGetNewGameName` | `0010:7508` | int | far cdecl | char near * szFileSuggest |
| create.obj | `SimpleNewGameDlg` | `0010:76aa` | int | far pascal | unsigned int hwnd, unsigned int message, unsigned short wParam, long lParam |
| create.obj | `NewGameDlg` | `0010:7e92` | int | far pascal | unsigned int hwnd, unsigned int message, unsigned short wParam, long lParam |
| create.obj | `NewGameDlg2` | `0010:87d2` | int | far pascal | unsigned int hwnd, unsigned int message, unsigned short wParam, long lParam |
| create.obj | `DrawNewGame2` | `0010:9556` | void | far cdecl | unsigned int hwnd, unsigned int hdc, int iDraw |
| create.obj | `NewGameDlg3` | `0010:99f4` | int | far pascal | unsigned int hwnd, unsigned int message, unsigned short wParam, long lParam |
| create.obj | `DrawNewGame3` | `0010:9d5e` | void | far cdecl | unsigned int hwnd, unsigned int hdc, int iDraw |
| create.obj | `FTrackNewGameDlg3` | `0010:a210` | int | far cdecl | unsigned int hwnd, tagPOINT pt, int kbd |
| create.obj | `SetNGWTitle` | `0010:a320` | void | far cdecl | unsigned int hwnd, int iStep |
| create.obj | `LpplrComp` | `0010:b570` | _player far * | far cdecl | int idAi, int lvlAi |
| create.obj | `SetVCCheck` | `0010:b5bc` | void | far cdecl | _game near * pgame, int vc, int fChecked |
| create.obj | `GetVCCheck` | `0010:b60c` | int | far cdecl | _game near * pgame, int vc |
| create.obj | `SetVCVal` | `0010:b644` | int | far cdecl | _game near * pgame, int vc, int val |
| create.obj | `GetVCVal` | `0010:b710` | int | far cdecl | _game near * pgame, int vc, int fRaw |
| save.obj | `WriteOrders` | `000f:547e` | void | far cdecl | _fleet far * lpfl |
| save.obj | `WriteRtPlr` | `000f:551c` | void | far cdecl | _player near * pplr, unsigned char near * pbStore |
| save.obj | `WriteRtShDef` | `000f:574e` | void | far cdecl | _shdef far * lpshdef, unsigned char near * near * ppbStore |
| save.obj | `FWriteDataFile` | `000f:5964` | int | far cdecl | char near * pszFileBase, int iPlayer, int fAppend |
| save.obj | `FAppendFile` | `000f:704e` | int | far cdecl | int iPlayer |
| save.obj | `WriteBattles` | `000f:709c` | void | far cdecl | int iPlayer |
| save.obj | `WritePlanet` | `000f:7a6a` | void | far cdecl | _planet far * lppl, int rt, int fHistory |
| save.obj | `WriteFleet` | `000f:81c6` | void | far cdecl | _fleet far * lpfl |
| save.obj | `WriteRtString` | `000f:87b4` | void | far cdecl | char far * lpsz |
| save.obj | `MarkFleet` | `000f:885e` | void | far cdecl | _fleet far * lpfl, int det |
| save.obj | `WriteBattlePlan` | `000f:89b8` | void | far cdecl | _btlplan far * lpbtlplan, int fLog |
| save.obj | `MarkPlanet` | `000f:8adc` | void | far cdecl | _planet far * lppl, int iPlr, unsigned int det |
| save.obj | `SetSzWorkFromDt` | `000f:8cfe` | void | far cdecl | unsigned int dt, int iPlayer |
| save.obj | `FCreateFile` | `000f:8e16` | int | far cdecl | unsigned int dt, int iPlayer, char near * szForceName |
| save.obj | `WriteBOF` | `000f:8ea4` | void | far cdecl | int iPlayer, int dt, int fMulti |
| save.obj | `FMarkFile` | `000f:904a` | int | far cdecl | unsigned int dt, int iPlayer, int mdMark, int f |
| save.obj | `WriteRt` | `000f:947c` | void | far cdecl | int rt, int cb, void far * rg |
| save.obj | `RgToStream` | `000f:9554` | void | far cdecl | void far * rg, unsigned int cb |
| save.obj | `SetVisiblePlanFleet` | `000f:95bc` | void | far cdecl | int iPlr |
| save.obj | `SetVisPFInit` | `000f:9654` | void | far cdecl | int iPlr |
| save.obj | `SetVisPFFleets` | `000f:a100` | void | far cdecl | int iPlr |
| save.obj | `SetVisPFPlanets` | `000f:abde` | void | far cdecl | int iPlr |
| save.obj | `SetVisPFThings` | `000f:b9ee` | void | far cdecl | int iPlr |
| save.obj | `SetVisPFFinish` | `000f:c41c` | void | far cdecl | int iPlr |
| turn2.obj | `Produce` | `0018:0000` | void | far cdecl | (none recorded) |
| turn2.obj | `CBuildProdItem` | `0018:0c92` | int | far cdecl | _planet far * lppl, _prod far * lpprod, _prod near * pprodPartial, long near * rgRes, int fAlchemy, int near * pmdStatus, int fCalcOnly |
| turn2.obj | `FBuildObject` | `0018:19b2` | int | far cdecl | _planet far * lppl, int grobj, int iItem, int cBuilt, long near * rgMinerals |
| turn2.obj | `CreateShip` | `0018:2fd6` | void | far cdecl | int iPlr, _fleet far * lpfl, int ishdef, int cShip |
| turn2.obj | `RandomEvents` | `0018:3064` | void | far cdecl | (none recorded) |
| turn2.obj | `TransferToOthers` | `0018:3088` | void | far cdecl | (none recorded) |
| turn2.obj | `DropColonists` | `0018:34e2` | void | far cdecl | (none recorded) |
| turn2.obj | `HealShips` | `0018:444c` | void | far cdecl | (none recorded) |
| turn2.obj | `AutoTerraform` | `0018:48f6` | void | far cdecl | (none recorded) |
| turn2.obj | `RemoteTerraforming` | `0018:4c56` | void | far cdecl | (none recorded) |
| turn2.obj | `FQueueColonistDrop` | `0018:4faa` | int | far cdecl | _fleet far * lpfl, _planet far * lppl, long cColonists |
| turn2.obj | `UpdatePopulations` | `0018:50a0` | void | far cdecl | (none recorded) |
| turn2.obj | `UpdateGuesses` | `0018:532c` | void | far cdecl | (none recorded) |
| turn2.obj | `MineMinerals` | `0018:55a4` | void | far cdecl | (none recorded) |
| turn2.obj | `MeteorStrike` | `0018:560e` | void | far cdecl | (none recorded) |
| turn2.obj | `TossNonAutoBuildItems` | `0018:5aec` | void | far cdecl | _planet far * lppl |
| turn2.obj | `PlanetaryClimateChange` | `0018:5c54` | void | far cdecl | (none recorded) |
| turn2.obj | `DiscoverNewMinerals` | `0018:5e0c` | void | far cdecl | (none recorded) |
| turn2.obj | `MysteryTrader` | `0018:5efa` | void | far cdecl | (none recorded) |
| turn2.obj | `UpdatePlayerScores` | `0018:6258` | void | far cdecl | (none recorded) |
| turn2.obj | `CreateBackupDir` | `0018:6dca` | void | far cdecl | (none recorded) |
| turn2.obj | `FPacketDecay` | `0018:6e9c` | int | far cdecl | _thing far * lpth, int pctRate |
| turn2.obj | `ThingDecay` | `0018:70c6` | void | far cdecl | (none recorded) |
| turn2.obj | `UnmarkMineFields` | `0018:7638` | void | far cdecl | (none recorded) |
| turn2.obj | `SweepForMines` | `0018:76a4` | void | far cdecl | (none recorded) |
| turn2.obj | `BreedColonistsInTransit` | `0018:7e4e` | void | far cdecl | (none recorded) |
| turn2.obj | `UpdateResearchStatus` | `0018:80fe` | void | far cdecl | int fUsePool |
| turn2.obj | `IBestRemoteTerra` | `0018:8b70` | int | far cdecl | _planet far * lppl, int iplr, int fHelp |
| turn.obj | `FGenerateTurn` | `0017:0000` | int | far cdecl | (none recorded) |
| turn.obj | `DoOrders` | `0017:179a` | void | far cdecl | int fPostMovement |
| turn.obj | `MoveThings` | `0017:18f4` | void | far cdecl | int fPostProd |
| turn.obj | `FuelFleets` | `0017:2efa` | void | far cdecl | (none recorded) |
| turn.obj | `MoveFleets` | `0017:32ce` | void | far cdecl | (none recorded) |
| turn.obj | `FTravelThroughMineFields` | `0017:4f60` | int | far cdecl | _fleet far * lpfl, int near * pdTravel, _thing far * lpthHit |
| log.obj | `LogSplitFleet` | `000a:8b0e` | void | far cdecl | short id |
| log.obj | `LogMergeFleet` | `000a:8b6c` | void | far cdecl | short id |
| log.obj | `LogChangeShDef` | `000a:8c28` | void | far cdecl | _shdef far * lpshdefNew |
| log.obj | `LogChangeName` | `000a:8d5a` | void | far cdecl | int grobj, int id, char near * szName |
| log.obj | `LogChangeFleet` | `000a:8ebe` | void | far cdecl | _fleet far * pfl, _fleet near * pflNew |
| log.obj | `LogChangeRelations` | `000a:9340` | void | far cdecl | (none recorded) |
| log.obj | `LogChangeBtlplan` | `000a:93d0` | void | far cdecl | _btlplan near * pbtlplan |
| log.obj | `LogChangePlanet` | `000a:9420` | void | far cdecl | _planet far * ppl, _planet near * pplNew |
| log.obj | `LogChangeThing` | `000a:9908` | void | far cdecl | _thing far * lpth, _thing near * pthNew |
| log.obj | `LogMakeValidXfer` | `000a:99f6` | void | far cdecl | _logxfer near * plx1, _logxfer near * plx2 |
| log.obj | `LogMakeValidXferf` | `000a:9fa6` | void | far cdecl | _logxferf near * plxf1, _logxferf near * plxf2 |
| log.obj | `CancelMemRt` | `000a:a108` | void | far cdecl | int rt |
| log.obj | `WriteMemRt` | `000a:a130` | void | far cdecl | int rt, int cb, void near * rg |
| log.obj | `DirtyGame` | `000a:a228` | void | far cdecl | int fDirty |
| log.obj | `FGetPrevLogRt` | `000a:a25e` | int | far cdecl | _hdr near * phdr, unsigned char near * pb |
| log.obj | `FRunLogFile` | `000a:a2d6` | int | far cdecl | (none recorded) |
| log.obj | `FRunLogRecord` | `000a:a38c` | int | far cdecl | int rt, int cb, unsigned char far * lpb |
| log.obj | `FLoadLogFile` | `000a:c7a2` | int | far cdecl | char near * pszLog |
| log.obj | `FCheckLogFile` | `000a:cccc` | int | far cdecl | int iplr, int near * pfError |
| log.obj | `FWriteLogFile` | `000a:cdf6` | int | far cdecl | char near * pszFileBase, int iPlayer |
| log.obj | `FWriteTutorialMFile` | `000a:d058` | int | far cdecl | int iTurn |
| log.obj | `FWriteHistFile` | `000a:d29e` | int | far cdecl | int iPlayer |
| log.obj | `EnumLogRts` | `000a:d6e0` | void | far cdecl | function returning int far * pfn, void far * lpPass, int iPass |
| ship.obj | `DrawShipOrders` | `000b:0000` | void | far cdecl | unsigned int hdc, _tile near * ptile, _obj obj |
| ship.obj | `DrawShipWayPtOrders` | `000b:0912` | void | far cdecl | unsigned int hdc, _tile near * ptile, _obj obj |
| ship.obj | `DrawShipPlanet` | `000b:17b6` | void | far cdecl | unsigned int hdc, _tile near * ptile, _obj obj |
| ship.obj | `DrawShipCargo` | `000b:1a54` | void | far cdecl | unsigned int hdc, _tile near * ptile, _obj obj |
| ship.obj | `DrawFleetComp` | `000b:1e72` | void | far cdecl | unsigned int hdc, _tile near * ptile, _obj obj |
| ship.obj | `FCanSplit` | `000b:245c` | int | far cdecl | long cBoat |
| ship.obj | `FCanSplitAll` | `000b:24ae` | int | far cdecl | long cBoat |
| ship.obj | `FCanMerge` | `000b:2522` | int | far cdecl | _fleet near * pfl |
| ship.obj | `ShipCommandProc` | `000b:2640` | void | far cdecl | unsigned int hwnd, unsigned int wParam, long lParam |
| ship.obj | `SelectAdjFleet` | `000b:3d32` | void | far cdecl | int dInc, int idFleet |
| ship.obj | `SetFleetDropDownSel` | `000b:400e` | void | far cdecl | int id |
| ship.obj | `LGetFleetStat` | `000b:40f2` | long | far cdecl | _fleet far * lpfl, int grStat |
| ship.obj | `WtMaxShdefStat` | `000b:41b2` | int | far cdecl | _shdef far * lpshdef, int grStat |
| ship.obj | `DrawFleetGauge` | `000b:44b6` | void | far cdecl | unsigned int hdc, tagRECT near * prc, _fleet far * lpfl, int grbit |
| ship.obj | `DrawFleetBitmap` | `000b:490e` | void | far cdecl | _fleet far * lpfl, unsigned int hdc, int x, int y, int fFrame, int ibmp, int cDiff, int fShrink, int ibmpRace, int csh |
| ship.obj | `FEnumCalcJettison` | `000b:4df4` | int | far cdecl | void far * lprt, int rt, int cb, _planet far * lppl, int iFleet |
| ship.obj | `TransferStuff` | `000b:4faa` | int | far cdecl | int id1, int grobj1, int id2, int grobj2, int mdXfer |
| ship.obj | `TransferDlg` | `000b:5686` | int | far pascal | unsigned int hwnd, unsigned int message, unsigned short wParam, long lParam |
| ship.obj | `FTrackXfer` | `000b:5a16` | int | far cdecl | unsigned int hwnd, int x, int y, int fkb |
| ship.obj | `GetCargoFree` | `000b:5f98` | long | far cdecl | _fleet far * lpfl |
| ship.obj | `GetFuelFree` | `000b:6004` | long | far cdecl | _fleet far * lpfl |
| ship.obj | `ChgCargo` | `000b:6034` | long | far cdecl | int grobj, int id, int iSupply, long dChg, void near * pobj |
| ship.obj | `XferSupply` | `000b:64cc` | long | far cdecl | int iSupply, long cQuan |
| ship.obj | `UpdateXferBtns` | `000b:66a8` | void | far cdecl | (none recorded) |
| ship.obj | `DrawXferDlg` | `000b:6908` | void | far cdecl | unsigned int hwnd, unsigned int hdc, tagRECT near * prc, int iSupply |
| ship.obj | `GetXferLeftRightRcs` | `000b:6b46` | void | far cdecl | tagRECT near * prcWhole, tagRECT near * prcLeft, tagRECT near * prcRight |
| ship.obj | `FSetupXferBtns` | `000b:6bea` | int | far cdecl | tagRECT near * prc |
| ship.obj | `DrawThingXferSide` | `000b:7088` | void | far cdecl | unsigned int hdc, tagRECT near * prc, _thing near * pth, int iSupply |
| ship.obj | `DrawFleetCargoXferSide` | `000b:72de` | void | far cdecl | unsigned int hdc, tagRECT near * prc, _fleet near * pfl, int iSupply |
| ship.obj | `DrawFleetShipsXferSide` | `000b:7726` | void | far cdecl | unsigned int hdc, tagRECT near * prc, _fleet near * pfl, int iSupply |
| ship.obj | `DrawPlanetXferSide` | `000b:79ca` | void | far cdecl | unsigned int hdc, tagRECT near * prc, _planet near * ppl, int iSupply |
| ship.obj | `ClickInShipOrders` | `000b:7cda` | unsigned int | far cdecl | tagPOINT pt, int sks, int fCursor, int fRightBtn |
| ship.obj | `FillFleetCompLB` | `000b:9166` | void | far cdecl | (none recorded) |
| ship.obj | `FillOrdersLB` | `000b:928c` | void | far cdecl | (none recorded) |
| ship.obj | `SetOrdersLbSel` | `000b:9354` | void | far cdecl | int iSel |
| ship.obj | `UpdateOrdersDDs` | `000b:93ee` | void | far cdecl | int iLevel |
| ship.obj | `FillBattleDD` | `000b:9a36` | void | far cdecl | int iSel |
| ship.obj | `DeleteCurWayPoint` | `000b:9b08` | void | far cdecl | int fBackup |
| ship.obj | `DeleteWpFar` | `000b:9e28` | void | far cdecl | _fleet far * lpfl, int iDel, int fRecycle |
| ship.obj | `EstFuelUse` | `000b:9fe4` | long | far cdecl | _fleet far * lpfl, int iOrd, int iWarp, long dTravel, int fRangeOnly |
| ship.obj | `FakeEditProc` | `000b:a700` | long | far pascal | unsigned int hwnd, unsigned short msg, unsigned short wParam, long lParam |
| ship.obj | `IFindIdealWarp` | `000b:a76e` | int | far cdecl | _fleet far * lpfl, int fIgnoreScoops |
| ship.obj | `LFuelUseToWaypoint` | `000b:a9f4` | long | far cdecl | _fleet far * lpfl, int iwp, int fMaxCargo |
| ship.obj | `FleetTransferCargoBalance` | `000b:ae74` | void | far cdecl | _fleet near * pflNew1, _fleet near * pflNew2 |
| ship.obj | `DestroyAllIshdefSB` | `000b:c280` | void | far cdecl | int ishdefSB, int iplr |
| ship.obj | `DestroyAllIshdef` | `000b:c336` | void | far cdecl | int ishdef, int iplr |
| ship.obj | `RemoveIshdefFromAllQueues` | `000b:c5e6` | void | far cdecl | int ishdef, int fSpaceDocks |
| ship.obj | `CshQueued` | `000b:c82c` | int | far cdecl | int ishdef, int near * pfProgress, int fSpaceDocks |
| ship.obj | `Merge2Fleets` | `000b:c9f6` | void | far cdecl | _fleet far * lpflDst, _fleet far * lpflDel, int fNoDelete |
| ship.obj | `FleetOrdersChangeTarget` | `000b:cafe` | void | far cdecl | _fleet far * lpflOld |
| ship.obj | `GetTruePartCost` | `000b:cd00` | void | far cdecl | int iPlayer, _part near * ppart, unsigned short near * rgCost |
| planet.obj | `PlanetWndProc` | `000a:0000` | long | far pascal | unsigned int hwnd, unsigned int message, unsigned int wParam, long lParam |
| planet.obj | `DrawPlanShip` | `000a:0d16` | void | far cdecl | unsigned int hdc, int grbit |
| planet.obj | `FDrawTileNC` | `000a:1086` | int | far cdecl | unsigned int hdc, _tile near * ptile, tagRECT near * prc, char near * pszTitle |
| planet.obj | `DrawPlanetMinSum` | `000a:12b2` | void | far cdecl | unsigned int hdc, _tile near * ptile, _obj obj |
| planet.obj | `DrawPlanetStats` | `000a:1716` | void | far cdecl | unsigned int hdc, _tile near * ptile, _obj obj |
| planet.obj | `FGetBestDefensePart` | `000a:21f6` | int | far cdecl | _part near * ppart |
| planet.obj | `DrawPlanetStarbase` | `000a:22cc` | void | far cdecl | unsigned int hdc, _tile near * ptile, _obj obj |
| planet.obj | `DrawMassWarpGauge` | `000a:2afa` | void | far cdecl | unsigned int hdc, tagRECT near * prc, int iBest, int iCur |
| planet.obj | `DrawPlanetProduction` | `000a:2c38` | void | far cdecl | unsigned int hdc, _tile near * ptile, _obj obj |
| planet.obj | `PszProductionETA` | `000a:310c` | char near * | far cdecl | _planet far * lppl, PLPROD far * lpplprod, int iItem, int near * etaFirst, int near * etaLast |
| planet.obj | `DrawPlanShipBitmap` | `000a:3336` | void | far cdecl | unsigned int hdc, _tile near * ptile, _obj obj |
| planet.obj | `DrawPlanetShipList` | `000a:377e` | void | far cdecl | unsigned int hdc, _tile near * ptile, _obj obj |
| planet.obj | `SetPlanetTitleBar` | `000a:3dec` | void | far cdecl | unsigned int hwnd |
| planet.obj | `ChangeMainObjSel` | `000a:3e7a` | void | far cdecl | int grobjNew, int iObjSel |
| planet.obj | `FillShipDD` | `000a:42a0` | void | far cdecl | int idSkip |
| planet.obj | `SelectAdjPlanet` | `000a:44da` | void | far cdecl | int dInc, int idPlanet |
| planet.obj | `IdFindAdjStarbase` | `000a:474e` | int | far cdecl | int idPlanet, int fNext |
| planet.obj | `PlanetClick` | `000a:489e` | void | far cdecl | int x, int y, int sks, int fRightBtn |
| planet.obj | `ClickInPlanetOrders` | `000a:515c` | unsigned int | far cdecl | tagPOINT pt, int sks, int fCursor, int fRightBtn |
| planet.obj | `EnsureTileSize` | `000a:587e` | void | far cdecl | int fSmallTiles |
| planet.obj | `ReflowColumn` | `000a:5b80` | void | far cdecl | int iCol, int iTile, int fRedraw |
| planet.obj | `IBestTerraform` | `000a:5dd2` | int | far cdecl | _planet far * lppl, int fHelp |
| planet.obj | `PszCalcEnvVar` | `000a:5fcc` | char near * | far cdecl | int iEnv, int iVar |
| planet.obj | `PszCalcGravity` | `000a:6058` | char near * | far cdecl | int iGravity |
| planet.obj | `HandleFocusState` | `000a:60e6` | void | far cdecl | tagDRAWITEMSTRUCT far * lpdis, int inflate |
| planet.obj | `DrawCBEntireItem` | `000a:6128` | void | far cdecl | tagDRAWITEMSTRUCT far * lpdis, int inflate |
| planet.obj | `DrawProductionItem` | `000a:6208` | void | far cdecl | unsigned int hdc, tagRECT near * prc, char near * psz, int inflate, int fSelected, int fListbox |
| planet.obj | `FillPlanetProdLB` | `000a:6692` | void | far cdecl | unsigned int hwnd, PLPROD far * lpplprod, _planet far * lppl |
| planet.obj | `PctPlanetCapacity` | `000a:6aca` | int | far cdecl | _planet far * lppl |
| planet.obj | `PctPlanetOptValue` | `000a:6b88` | int | far cdecl | _planet far * lppl, int iPlr |
| planet.obj | `PctPlanetDesirability` | `000a:6e1e` | int | far cdecl | _planet far * lppl, int iPlr |
| planet.obj | `CalcPlanetMaxPop` | `000a:7096` | long | far cdecl | int idpl, int iplr |
| planet.obj | `CMaxMines` | `000a:7248` | short | far cdecl | _planet far * lppl, int iplr |
| planet.obj | `CMaxOperableMines` | `000a:7304` | short | far cdecl | _planet far * lppl, int iplr, int fNextYear |
| planet.obj | `CMinesOperating` | `000a:73fc` | short | far cdecl | _planet far * lppl |
| planet.obj | `CFactoriesOperating` | `000a:74be` | short | far cdecl | _planet far * lppl |
| planet.obj | `CMaxFactories` | `000a:755c` | short | far cdecl | _planet far * lppl, int iplr |
| planet.obj | `CMaxOperableFactories` | `000a:7618` | short | far cdecl | _planet far * lppl, int iplr, int fNextYear |
| planet.obj | `CMaxDefenses` | `000a:7710` | short | far cdecl | _planet far * lppl, int iplr |
| planet.obj | `CMaxOperableDefenses` | `000a:77ae` | short | far cdecl | _planet far * lppl, int iplr, int fNextYear |
| planet.obj | `CResourcesAtPlanet` | `000a:788e` | short | far cdecl | _planet far * lppl, int iplr |
| planet.obj | `IWarpMAFromLppl` | `000a:7b10` | int | far cdecl | _planet far * lppl, int near * pfTwo |
| planet.obj | `StargateRangeFromLppl` | `000a:7cfa` | int | far cdecl | _planet far * lppl, int iplr, int ish |
| planet.obj | `FProdIsTerra` | `000a:7e9a` | int | far cdecl | _prod far * lpprod |
| planet.obj | `IpctCanTerraformLppl` | `000a:7f56` | int | far cdecl | _planet far * lppl |
| planet.obj | `FCanTerraformLppl` | `000a:8022` | int | far cdecl | _planet far * lppl, int near * rgEnvMin, int near * rgEnvMax, int near * rgEnvCost, int fHelp |
| planet.obj | `UninhabitPlanet` | `000a:8732` | void | far cdecl | _planet far * lppl |
| planet.obj | `PctCloakFromHuldef` | `000a:88c0` | int | far cdecl | _hul far * lphul, int iplr, int near * ppctSteal |
| file.obj | `FReadShDef` | `000f:0006` | int | far cdecl | _rtshdef far * lprt, _shdef far * lpshdef, int iplrLoad |
| file.obj | `ReadRtPlr` | `000f:05e2` | void | far cdecl | _player near * pplr, unsigned char near * pbIn |
| file.obj | `FLoadGame` | `000f:0810` | int | far cdecl | char near * pszFileName, char near * pszExt |
| file.obj | `FReadPlanet` | `000f:3206` | int | far cdecl | int iPlayer, _planet far * lppl, int fHistory, int fPreInited |
| file.obj | `FReadFleet` | `000f:3a4c` | int | far cdecl | _fleet far * lpfl |
| file.obj | `UnpackBattlePlan` | `000f:40ce` | void | far cdecl | unsigned char far * lpb, _btlplan far * lpbtlplan, int iplan |
| file.obj | `UpdateBattleRecords` | `000f:41ac` | void | far cdecl | (none recorded) |
| file.obj | `AskSaveDialog` | `000f:432a` | int | far pascal | unsigned int hwnd, unsigned int message, unsigned short wParam, long lParam |
| file.obj | `PromptSaveGame` | `000f:43ee` | void | far cdecl | (none recorded) |
| file.obj | `DestroyCurGame` | `000f:44b0` | void | far cdecl | (none recorded) |
| file.obj | `FBogusLong` | `000f:484c` | int | far cdecl | unsigned long lSerial |
| file.obj | `FValidSerialLong` | `000f:48c4` | int | far cdecl | unsigned long lSerial |
| file.obj | `FileError` | `000f:4a10` | void | far cdecl | int ids |
| file.obj | `GetFileStatus` | `000f:4a60` | void | far cdecl | int dt, int iPlayer |
| file.obj | `FOpenFile` | `000f:4ac2` | int | far cdecl | unsigned int dt, int iPlayer, int md |
| file.obj | `FNewTurnAvail` | `000f:4f22` | int | far cdecl | int idPlayer |
| file.obj | `FCheckFile` | `000f:4fb2` | int | far cdecl | unsigned int dt, int iPlayer, unsigned int md |
| file.obj | `ReadRt` | `000f:5168` | void | far cdecl | (none recorded) |
| file.obj | `FBadFileError` | `000f:524e` | int | far cdecl | int ids |
| file.obj | `StreamOpen` | `000f:52ae` | void | far cdecl | char near * szFile, int mdOpen |
| file.obj | `StreamClose` | `000f:53cc` | void | far cdecl | (none recorded) |
| file.obj | `RgFromStream` | `000f:53f4` | void | far cdecl | void far * rg, unsigned int cb |
| init.obj | `FCreateStuff` | `0001:0014` | int | far cdecl | (none recorded) |
| init.obj | `FCreateFonts` | `0001:0ab2` | int | far cdecl | unsigned int hdc |
| init.obj | `InitInstance` | `0001:0d70` | int | far cdecl | int nCmdShow |
| init.obj | `InitTiles` | `0001:0eaa` | void | far cdecl | (none recorded) |
| init.obj | `GetIniWinRc` | `0001:1020` | void | far cdecl | char near * szSection, char near * szIniFile, int ids, _wn near * pwn |
| init.obj | `ReadIniSettings` | `0001:124c` | void | far cdecl | (none recorded) |
| init.obj | `ReadIniTileSettings` | `0001:2b8a` | void | far cdecl | char near * pszFormat, _tile near * rgtile, int ctile |
| scan.obj | `ScannerWndProc` | `000c:0032` | long | far pascal | unsigned int hwnd, unsigned int msg, unsigned int wParam, long lParam |
| scan.obj | `PtToScan` | `000c:0efc` | int | far cdecl | int d |
| scan.obj | `ScanToPt` | `000c:0fc2` | int | far cdecl | int d |
| scan.obj | `DrawScanner` | `000c:108a` | int | far cdecl | unsigned int hdc, tagRECT near * prc |
| scan.obj | `DrawScanFleetCount` | `000c:47d2` | void | far cdecl | _fleet far * lpfl, int x, int y, unsigned int hdc, unsigned int hdcMem |
| scan.obj | `CShipsScanVis` | `000c:4bf4` | long | far cdecl | _fleet far * lpfl |
| scan.obj | `DrawRadarCircle` | `000c:4e7c` | void | far cdecl | _drawcir near * pdc, tagRECT near * prc |
| scan.obj | `DrawShipScanPath` | `000c:540c` | void | far cdecl | unsigned int hdc, int fShow |
| scan.obj | `DrawScannerSBar` | `000c:62d8` | void | far cdecl | unsigned int hdc, tagRECT near * prc, _sbar near * psbar, int fFullRedraw |
| scan.obj | `DrawLockLight` | `000c:6b00` | void | far cdecl | unsigned int hdc, tagRECT near * prc, int fFullRedraw |
| scan.obj | `SetScanScrollBars` | `000c:6c14` | void | far cdecl | unsigned int hwnd |
| scan.obj | `ScrollScanner` | `000c:6d30` | void | far cdecl | int dx, int dy |
| scan.obj | `RedrawScanSel` | `000c:6f30` | void | far cdecl | unsigned int hdc, int fVis |
| scan.obj | `FEnsurePointOnScreen` | `000c:715c` | int | far cdecl | tagPOINT pt, int fScroll |
| scan.obj | `CtrPointScan` | `000c:7278` | void | far cdecl | tagPOINT pt, int fScroll |
| scan.obj | `LogicalToScan` | `000c:744e` | void | far cdecl | tagPOINT near * ppt |
| scan.obj | `ScanToLogical` | `000c:7490` | void | far cdecl | tagPOINT near * ppt |
| scan.obj | `FAddWayPoint` | `000c:7504` | int | far cdecl | tagPOINT ptIn, _scan near * pscan |
| scan.obj | `IWarpBestForWaypoint` | `000c:7a18` | int | far cdecl | _fleet far * lpfl, _order far * lpord |
| scan.obj | `FNearAWayPoint` | `000c:8074` | int | far cdecl | tagPOINT pt, int fLogical |
| scan.obj | `FHandleWayPointDrag` | `000c:8176` | int | far cdecl | tagPOINT pt |
| scan.obj | `DrawScanXorLines` | `000c:8af6` | void | far cdecl | unsigned int hdc, tagPOINT near * rgpt, int cpt |
| scan.obj | `SetScanWp` | `000c:8c5a` | int | far cdecl | int iNew |
| scan.obj | `ChangeScanSel` | `000c:8cc4` | void | far cdecl | _scan near * pscan, int fValidScan |
| scan.obj | `FGetNextObjHere` | `000c:909c` | int | far cdecl | _scan near * pscan, int fOnlyOurs |
| scan.obj | `FindDlg` | `000c:9286` | int | far pascal | unsigned int hwnd, unsigned int msg, unsigned short wParam, long lParam |
| scan.obj | `FSelectSz` | `000c:945a` | int | far cdecl | char near * szName |
| scan.obj | `GetScanFleetOrientation` | `000c:978c` | void | far cdecl | _fleet far * lpfl, tagPOINT near * ppt, tagPOINT near * pptD |
| scan.obj | `GetDxDyOrientation` | `000c:987c` | void | far cdecl | int dx, int dy, tagPOINT near * ppt, tagPOINT near * pptD |
| scan.obj | `FHandleMeasuringTape` | `000c:9974` | int | far cdecl | _scan near * pscan, tagPOINT pt |
| util.obj | `FLookupSelPlanet` | `0008:0000` | int | far cdecl | _planet near * ppl |
| util.obj | `FDupPlanet` | `0008:0032` | int | far cdecl | _planet far * lppl, _planet near * ppl |
| util.obj | `LpthFromId` | `0008:01b2` | _thing far * | far cdecl | int idth |
| util.obj | `LpplFromId` | `0008:021e` | _planet far * | far cdecl | int idPlanet |
| util.obj | `CalcPctSurvive` | `0008:02f6` | void | far cdecl | _planet far * lppl, float near * ppct, float near * ppctSmart |
| util.obj | `FLookupPlanet` | `0008:04a6` | int | far cdecl | int iPlanet, _planet near * ppl |
| util.obj | `DpOfLpflIshdef` | `0008:0746` | long | far cdecl | _fleet far * lpfl, int ishdef |
| util.obj | `FLookupThing` | `0008:07fe` | int | far cdecl | int idth, _thing near * pth |
| util.obj | `SelectOursAtObject` | `0008:08f2` | void | far cdecl | tagPOINT near * ppt |
| util.obj | `LComputePower` | `0008:0b32` | long | far cdecl | _shdef far * lpshdef |
| util.obj | `ComputeShdefPowers` | `0008:0e4e` | void | far cdecl | (none recorded) |
| util.obj | `DpShieldOfShdef` | `0008:0f24` | long | far cdecl | _shdef far * lpshdef, int iplr |
| util.obj | `IStargateFromLppl` | `0008:10fa` | int | far cdecl | _planet far * lppl |
| util.obj | `PszPlayerName` | `0008:11f2` | char near * | far cdecl | int iPlayer, int fCapital, int fPlural, int fThe, int grWord, _player near * pplr |
| util.obj | `FCalcFleetBombDamage` | `0008:145c` | int | far cdecl | _fleet far * lpfl, long near * pdmgPeople, long near * pdmgPeopleMin, long near * pdmgPeopleSmart, long near * pdmgBldg, long near * ppctTerra, int near * pfMulti |
| util.obj | `LinkFleets` | `0008:1bb4` | void | far cdecl | int fUnused |
| util.obj | `ICompFleetPoint` | `0008:1f0c` | int | far cdecl | void near * arg1, void near * arg2 |
| util.obj | `ICompFleetPoint2` | `0008:1fa2` | int | far cdecl | void near * arg1, void near * arg2 |
| util.obj | `FLookupSelShip` | `0008:2032` | int | far cdecl | _fleet near * pfl |
| util.obj | `LpflFromId` | `0008:2078` | _fleet far * | far cdecl | int idFleet |
| util.obj | `FLookupFleet` | `0008:2160` | int | far cdecl | int idFleet, _fleet near * pfl |
| util.obj | `FDupFleet` | `0008:2332` | int | far cdecl | _fleet far * lpfl, _fleet near * pfl |
| util.obj | `FLookupObject` | `0008:24a0` | int | far cdecl | int grobj, int id, void near * pobj |
| util.obj | `FLookupOrbitingXfer` | `0008:24fa` | int | far cdecl | int idPlanet, int iNth, _xfer near * pxf, int idSkip |
| util.obj | `PszGetThingName` | `0008:26de` | char near * | far cdecl | int id |
| util.obj | `PszGetFleetName` | `0008:292c` | char near * | far cdecl | int id |
| util.obj | `WFromLpfl` | `0008:2b10` | unsigned short | far cdecl | _fleet far * lpfl |
| util.obj | `PszFleetNameFromWord` | `0008:2b5e` | char near * | far cdecl | unsigned short w |
| util.obj | `PszGetPlanetName` | `0008:2c6a` | char near * | far cdecl | int id |
| util.obj | `IflFromLpfl` | `0008:2ce8` | int | far cdecl | _fleet far * lpfl |
| util.obj | `FDeleteFleet` | `0008:2d44` | int | far cdecl | int idFleet, int grobjSel, int idSel |
| util.obj | `LpflNew` | `0008:300c` | _fleet far * | far cdecl | int iPlr, int idPl |
| util.obj | `LpflNewSplit` | `0008:3372` | _fleet far * | far cdecl | _fleet near * pfl |
| util.obj | `FFleetMergeAll` | `0008:34d8` | int | far cdecl | _fleet near * pfl |
| util.obj | `FFleetSplitAll` | `0008:3a00` | int | far cdecl | _fleet near * pfl |
| util.obj | `PszGetLocName` | `0008:3b08` | char near * | far cdecl | int grobj, int id, int x, int y |
| util.obj | `CchGetETA` | `0008:3bc8` | int | far cdecl | unsigned int hdc, _fleet far * lpfl, char near * sz, int iwp, int fSmall |
| util.obj | `IshdefPrimaryFromLpfl` | `0008:3e1c` | int | far cdecl | _fleet far * lpfl, int near * pcDiff |
| util.obj | `PszGetDistance` | `0008:3f00` | char near * | far cdecl | int x1, int y1, int x2, int y2 |
| util.obj | `DGetDistance` | `0008:3fe4` | double | far cdecl | int x1, int y1, int x2, int y2 |
| util.obj | `FFindNearestObject` | `0008:4070` | int | far cdecl | tagPOINT pt, int grobj, _scan near * pscan |
| util.obj | `UpdateShdefCost` | `0008:47b0` | void | far cdecl | _shdef far * lpshdef |
| util.obj | `WPackLong` | `0008:4ba2` | unsigned short | far cdecl | long l |
| util.obj | `GetPlanetScannerRange` | `0008:4c02` | int | far cdecl | _planet far * lppl, int near * pDeep |
| util.obj | `GetCachedFleetScannerRange` | `0008:4e02` | int | far cdecl | _fleet far * lpfl, int near * pdPlanRange, int near * ppctDetect, int near * piSteal |
| util.obj | `GetFleetScannerRange` | `0008:4fb8` | int | far cdecl | _fleet far * lpfl, int near * pdPlanRange, int near * ppctDetect, int near * piSteal |
| util.obj | `GetShdefScannerRange` | `0008:50d0` | int | far cdecl | _shdef far * lpshdef, int iplr, int near * pdPlanRange, int near * ppctDetect, int near * piSteal |
| util.obj | `LCalcFuelGainFromRamScoops` | `0008:56b8` | long | far cdecl | _fleet far * lpfl, int iWarp, long dTravel |
| util.obj | `CalcPlayerScore` | `0008:58a6` | long | far cdecl | int iPlr, _score near * pscore |
| util.obj | `GetTrueHullCost` | `0008:5dba` | void | far cdecl | int iPlayer, _hul far * lphul, unsigned short near * rgCost |
| util.obj | `DecorateHullName` | `0008:5e0e` | void | far cdecl | int iplr, int ish, char near * psz |
| util.obj | `DrawABunchOfStars` | `0008:5fe2` | void | far cdecl | unsigned int hdc, tagRECT near * prc |
| util.obj | `LongFromSerialCh` | `0008:6280` | long | far cdecl | char ch |
| util.obj | `FValidSerialNo` | `0008:62f8` | int | far cdecl | char near * psz, long near * plSerial |
| util.obj | `FMatchTarget` | `0008:6612` | int | far cdecl | _fleet far * lpflTarget, int mdTarget, int fExact |
| util.obj | `ValidateWaypoints` | `0008:68c6` | void | far cdecl | (none recorded) |
| util.obj | `ChgPopFromPlanet` | `0008:7082` | long | far cdecl | _planet far * lppl, int fUpdate |
| util.obj | `FCanFleetUseStargates` | `0008:75e2` | int | far cdecl | _fleet far * lpfl, tagPOINT ptSrc, tagPOINT ptDst |
| util.obj | `FFleetCanJumpgate` | `0008:7960` | int | far cdecl | _fleet far * lpfl |
| util.obj | `WtFromLpfl` | `0008:7a68` | long | far cdecl | _fleet far * lpfl |
| util.obj | `FCanBuildShdef` | `0008:7b40` | int | far cdecl | _shdef far * lpshdef, int iplr |
| util.obj | `IshFindSimilarDesign` | `0008:7c5e` | int | far cdecl | _hul far * lphul, int iPlrDst |
| util.obj | `DrawPlanetPrintDot` | `0008:7e44` | void | far cdecl | unsigned int hdc, int x, int y, int iSize |
| util.obj | `ClearFile` | `0008:7f6a` | void | far cdecl | int dt |
| util.obj | `OutputSz` | `0008:7fe6` | void | far cdecl | int dt, char far * sz |
| util.obj | `TurnLog` | `0008:80c2` | void | far cdecl | int ids |
| mine.obj | `MineWndProc` | `0006:0000` | long | far pascal | unsigned int hwnd, unsigned int message, unsigned int wParam, long lParam |
| mine.obj | `InvalidateMineralBars` | `0006:040a` | void | far cdecl | (none recorded) |
| mine.obj | `GetMineFieldCounts` | `0006:05ac` | void | far cdecl | unsigned int id, int near * pithm, int near * pcthm |
| mine.obj | `DrawMineSurvey` | `0006:065a` | void | far cdecl | unsigned int hdc, tagRECT near * prc |
| mine.obj | `HtMineWindow` | `0006:37ac` | int | far cdecl | unsigned int hwnd, int x, int y |
| mine.obj | `MineClick` | `0006:3b4e` | void | far cdecl | int x, int y, int msg, int sks |
| mine.obj | `SetMineralTitleBar` | `0006:47dc` | void | far cdecl | unsigned int hwnd |
| mine.obj | `DrawSelectionArrow` | `0006:4a68` | void | far cdecl | unsigned int hdc, tagRECT near * prc, int fEnabled |
| mine.obj | `DrawDiamond` | `0006:4b60` | void | far cdecl | unsigned int hdc, tagRECT near * prc, unsigned int hbr |
| mine.obj | `FOtherStuffAtScanSel` | `0006:4d6e` | int | far cdecl | (none recorded) |
| mine.obj | `PopupMineralScanChoices` | `0006:4ecc` | void | far cdecl | unsigned int hwnd, int x, int y |
| mine.obj | `EstMineralsMined` | `0006:5362` | void | far cdecl | _planet far * lppl, long near * plQuan, long cMines, int fApply |
| mdi.obj | `InitMDIApp` | `0005:0000` | int | far cdecl | (none recorded) |
| mdi.obj | `CreateChildWindows` | `0005:038c` | void | far cdecl | (none recorded) |
| mdi.obj | `FrameWndProc` | `0005:06d6` | long | far pascal | unsigned int hwnd, unsigned int msg, unsigned int wParam, long lParam |
| mdi.obj | `InvertPaneBorder` | `0005:1e3c` | tagPOINT | far cdecl | unsigned int hdc, int grSel, tagPOINT dpt, tagPOINT near * pdptPrev |
| mdi.obj | `HcrsFromFrameWindowPt` | `0005:24b0` | unsigned int | far cdecl | tagPOINT pt, int near * pgrSel |
| mdi.obj | `RestoreSelection` | `0005:2614` | void | far cdecl | (none recorded) |
| mdi.obj | `FormatSerialAndEnv` | `0005:2886` | void | far cdecl | long lSerial, unsigned char near * pbEnv, char near * pszOut |
| mdi.obj | `FSerialAndEnvFromSz` | `0005:2aec` | int | far cdecl | long near * plSerial, unsigned char near * pbEnv, char near * pszIn |
| mdi.obj | `FFindSomethingAndSelectIt` | `0005:2e1c` | int | far cdecl | (none recorded) |
| mdi.obj | `CommandHandler` | `0005:2f7a` | void | far cdecl | unsigned int hwnd, unsigned short wParam |
| mdi.obj | `InitializeMenu` | `0005:5376` | void | far cdecl | unsigned int hmenu |
| mdi.obj | `EnsureAis` | `0005:56bc` | void | far cdecl | (none recorded) |
| mdi.obj | `GetASubMenu` | `0005:589a` | unsigned int | far cdecl | unsigned int hwnd, int iMenu |
| mdi.obj | `FOpenGame` | `0005:58f4` | int | far cdecl | unsigned int hwnd, int fRaceOnly |
| mdi.obj | `FWasRaceFile` | `0005:5da8` | int | far cdecl | char near * szFile, int fChkPass |
| mdi.obj | `BringUpHostDlg` | `0005:5ffc` | void | far cdecl | (none recorded) |
| mdi.obj | `DrawHostDialog2` | `0005:6240` | void | far cdecl | unsigned int hwnd, unsigned int hdcIn |
| mdi.obj | `VerifyTurns` | `0005:6686` | void | far cdecl | (none recorded) |
| mdi.obj | `CTurnsOutSafe` | `0005:6996` | int | far cdecl | (none recorded) |
| mdi.obj | `CFindTurnsOutstanding` | `0005:6a26` | int | far cdecl | (none recorded) |
| mdi.obj | `HostModeDialog` | `0005:6c16` | int | far pascal | unsigned int hwnd, unsigned int message, unsigned short wParam, long lParam |
| mdi.obj | `HostOptionsDialog` | `0005:75ce` | int | far pascal | unsigned int hwnd, unsigned int message, unsigned short wParam, long lParam |
| mdi.obj | `DrawHostOptions` | `0005:7706` | void | far cdecl | unsigned int hwnd, unsigned int hdc, int iDraw |
| mdi.obj | `HostTimerProc` | `0005:7716` | void | far pascal | unsigned int hwnd, unsigned int msg, unsigned int idTimer, unsigned long dwTime |
| mdi.obj | `GetWindowRc` | `0005:79ac` | void | far cdecl | unsigned int hwnd, tagRECT near * prc |
| mdi.obj | `SetWindowIniString` | `0005:79f6` | void | far cdecl | char near * sz, unsigned int hwnd |
| mdi.obj | `WriteIniSettings` | `0005:7a76` | void | far cdecl | (none recorded) |
| mdi.obj | `RefitFrameChildren` | `0005:8c88` | void | far cdecl | (none recorded) |
| mdi.obj | `TitleWndProc` | `0005:9126` | long | far pascal | unsigned int hwnd, unsigned int msg, unsigned int wParam, long lParam |
| stars.obj | `WinMain` | `0004:0000` | int | far pascal | unsigned int hInstance, unsigned int hPrevInstance, char far * lpCmdLine, int nCmdShow |
| stars.obj | `FSetUpBatchProcessing` | `0004:06a4` | int | far cdecl | (none recorded) |
| stars.obj | `IPlrAlsoCheater` | `0004:07aa` | int | far cdecl | int iplr |
| stars.obj | `FGetSystemColors` | `0004:08d2` | int | far cdecl | (none recorded) |
| stars.obj | `FreeStuff` | `0004:0bae` | void | far cdecl | (none recorded) |
| stars.obj | `SzVersion` | `0004:1212` | char near * | far cdecl | (none recorded) |
| stars.obj | `About` | `0004:1252` | int | far pascal | unsigned int hwnd, unsigned int message, unsigned short wParam, long lParam |
| stars.obj | `OrderInfoDlg` | `0004:151e` | int | far pascal | unsigned int hwnd, unsigned int message, unsigned short wParam, long lParam |
| stars.obj | `FHandleChar` | `0004:15de` | int | far cdecl | unsigned int hwnd, unsigned short ch, long lParam |
| stars.obj | `FHandleKey` | `0004:165a` | int | far cdecl | unsigned int hwnd, int iMsg, int iKey, unsigned long dw |
