# Recovered data layouts

These are debug metadata, not reconstructed source declarations. Sizes are bytes in the original 16-bit build. Overlapping members and bit fields are preserved; do not paste these layouts into a modern compiler and assume the same packing.

## _planet — 56 bytes (type 0x1002)

| Offset | Member | Type |
|---:|---|---|
| 0 | `id` | short |
| 2 | `iPlayer` | short |
| 4 | `det` | unsigned short; bits 0..7 |
| 4 | `fInclude` | unsigned short; bits 8..8 |
| 4 | `fStarbase` | unsigned short; bits 9..9 |
| 4 | `fHomeworld` | unsigned short; bits 10..10 |
| 4 | `fFirstYear` | unsigned short; bits 11..11 |
| 4 | `unusedC` | unsigned short; bits 12..12 |
| 4 | `fWasInhabited` | unsigned short; bits 13..13 |
| 4 | `unusedD` | unsigned short; bits 14..15 |
| 6 | `rgpctMinLevel` | array of unsigned char (3 bytes total) |
| 9 | `rgMinConc` | array of unsigned char (3 bytes total) |
| 12 | `rgEnvVar` | array of char (3 bytes total) |
| 15 | `rgEnvVarOrig` | array of char (3 bytes total) |
| 18 | `uPopGuess` | unsigned short; bits 0..11 |
| 18 | `uDefGuess` | unsigned short; bits 12..15 |
| 18 | `uGuesses` | unsigned short |
| 20 | `iDeltaPop` | unsigned long; bits 0..7 |
| 20 | `cMines` | unsigned long; bits 8..19 |
| 20 | `cFactories` | unsigned long; bits 20..31 |
| 24 | `cDefenses` | unsigned long; bits 0..11 |
| 24 | `iScanner` | unsigned long; bits 12..16 |
| 24 | `unused5` | unsigned long; bits 17..21 |
| 24 | `fArtifact` | unsigned long; bits 22..22 |
| 24 | `fNoResearch` | unsigned long; bits 23..23 |
| 24 | `unused2` | unsigned long; bits 24..31 |
| 20 | `rgbImp` | array of unsigned char (8 bytes total) |
| 28 | `rgwtMin` | array of long (16 bytes total) |
| 44 | `isb` | unsigned short; bits 0..3 |
| 44 | `pctDp` | unsigned short; bits 4..15 |
| 46 | `idFling` | unsigned short; bits 0..9 |
| 46 | `iWarpFling` | unsigned short; bits 10..13 |
| 46 | `fNoHeal` | unsigned short; bits 14..14 |
| 46 | `unused3` | unsigned short; bits 15..15 |
| 44 | `lStarbase` | long |
| 48 | `idRoute` | unsigned short; bits 0..9 |
| 48 | `unused4` | unsigned short; bits 10..15 |
| 48 | `wRouting` | unsigned short |
| 50 | `turn` | short |
| 52 | `lpplprod` | PLPROD far * |

## _fleet — 124 bytes (type 0x1003)

| Offset | Member | Type |
|---:|---|---|
| 0 | `id` | short |
| 0 | `ifl` | unsigned short; bits 0..8 |
| 0 | `iplr` | unsigned short; bits 9..12 |
| 0 | `junk` | unsigned short; bits 13..15 |
| 2 | `iPlayer` | short |
| 4 | `det` | unsigned short; bits 0..7 |
| 4 | `fInclude` | unsigned short; bits 8..8 |
| 4 | `fRepOrders` | unsigned short; bits 9..9 |
| 4 | `fDead` | unsigned short; bits 10..10 |
| 4 | `fDone` | unsigned short; bits 11..11 |
| 4 | `fBombed` | unsigned short; bits 12..12 |
| 4 | `fHereAllTurn` | unsigned short; bits 13..13 |
| 4 | `fNoHeal` | unsigned short; bits 14..14 |
| 4 | `fMark` | unsigned short; bits 15..15 |
| 6 | `idPlanet` | short |
| 8 | `pt` | tagPOINT |
| 12 | `rgcsh` | array of short (32 bytes total) |
| 44 | `rgdv` | array of _dv (32 bytes total) |
| 44 | `wtFleet` | long |
| 76 | `rgwtMin` | array of long (20 bytes total) |
| 96 | `iplan` | unsigned char |
| 97 | `bUnused` | unsigned char |
| 98 | `cord` | short |
| 100 | `lpplord` | PLORD far * |
| 104 | `lpflNext` | _fleet far * |
| 108 | `lPower` | long |
| 108 | `dMoveLeft` | short |
| 110 | `dMoveUsed` | short |
| 112 | `lFuelUsed` | long |
| 116 | `dirLong` | long |
| 116 | `dirFltX` | unsigned short; bits 0..7 |
| 116 | `dirFltY` | unsigned short; bits 8..15 |
| 118 | `iwarpFlt` | unsigned short; bits 0..3 |
| 118 | `fdirValid` | unsigned short; bits 4..4 |
| 118 | `fCompChg` | unsigned short; bits 5..5 |
| 118 | `fTargeted` | unsigned short; bits 6..6 |
| 118 | `fSkipped` | unsigned short; bits 7..7 |
| 118 | `fUnused` | unsigned short; bits 8..15 |
| 120 | `lpszName` | char far * |

## _order — 18 bytes (type 0x1005)

| Offset | Member | Type |
|---:|---|---|
| 0 | `pt` | tagPOINT |
| 4 | `id` | short |
| 6 | `grTask` | unsigned short; bits 0..3 |
| 6 | `iWarp` | unsigned short; bits 4..7 |
| 6 | `grobj` | unsigned short; bits 8..11 |
| 6 | `fValidTask` | unsigned short; bits 12..12 |
| 6 | `fNoAutoTrack` | unsigned short; bits 13..13 |
| 6 | `fUnused` | unsigned short; bits 14..15 |
| 8 | `txp` | _taskxport |
| 8 | `tlm` | _tasklaymines |
| 8 | `tptl` | _taskpatrol |
| 8 | `tsell` | _tasksell |

## _thing — 18 bytes (type 0x1006)

| Offset | Member | Type |
|---:|---|---|
| 0 | `idFull` | unsigned short |
| 0 | `id` | unsigned short; bits 0..8 |
| 0 | `iplr` | unsigned short; bits 9..12 |
| 0 | `ith` | unsigned short; bits 13..15 |
| 2 | `pt` | tagPOINT |
| 6 | `rgb` | array of unsigned char (10 bytes total) |
| 6 | `thm` | _thmine |
| 6 | `thp` | _thpack |
| 6 | `thw` | _thworm |
| 6 | `tht` | _thtrader |
| 16 | `turn` | unsigned short |

## _shdef — 147 bytes (type 0x1008)

| Offset | Member | Type |
|---:|---|---|
| 0 | `hul` | _hul |
| 123 | `det` | unsigned short; bits 0..7 |
| 123 | `fInclude` | unsigned short; bits 8..8 |
| 123 | `fFree` | unsigned short; bits 9..9 |
| 123 | `ishdef` | unsigned short; bits 10..14 |
| 123 | `fGift` | unsigned short; bits 15..15 |
| 123 | `wFlags` | unsigned short |
| 125 | `turn` | unsigned short |
| 127 | `cBuilt` | unsigned long |
| 131 | `cExist` | unsigned long |
| 135 | `lPower` | long |
| 135 | `lVisible` | long |
| 139 | `grbitPlr` | unsigned short |
| 141 | `dScanRange` | unsigned short |
| 143 | `dScanRange2` | unsigned short |
| 145 | `pctDetect` | unsigned char |
| 146 | `iSteal` | unsigned char |

## _game — 64 bytes (type 0x1012)

| Offset | Member | Type |
|---:|---|---|
| 0 | `lid` | long |
| 4 | `mdSize` | short |
| 6 | `mdDensity` | short |
| 8 | `cPlayer` | short |
| 10 | `cPlanMax` | short |
| 12 | `mdStartDist` | short |
| 14 | `fDirty` | int |
| 16 | `fExtraFuel` | unsigned short; bits 0..0 |
| 16 | `fSlowTech` | unsigned short; bits 1..1 |
| 16 | `fSinglePlr` | unsigned short; bits 2..2 |
| 16 | `fTutorial` | unsigned short; bits 3..3 |
| 16 | `fAisBand` | unsigned short; bits 4..4 |
| 16 | `fBBSPlay` | unsigned short; bits 5..5 |
| 16 | `fVisScores` | unsigned short; bits 6..6 |
| 16 | `fNoRandom` | unsigned short; bits 7..7 |
| 16 | `fClumping` | unsigned short; bits 8..8 |
| 16 | `wGen` | unsigned short; bits 9..11 |
| 16 | `unused` | unsigned short; bits 12..15 |
| 16 | `wCrap` | unsigned short |
| 18 | `turn` | unsigned short |
| 20 | `rgvc` | array of unsigned char (12 bytes total) |
| 32 | `szName` | array of char (32 bytes total) |

## _gdata — 10 bytes (type 0x1013)

| Offset | Member | Type |
|---:|---|---|
| 0 | `grBits` | long |
| 0 | `fUnknownShip` | unsigned int; bits 0..0 |
| 0 | `fGeneratingTurn` | unsigned int; bits 1..1 |
| 0 | `fForceTurn` | unsigned int; bits 2..2 |
| 0 | `fHostMode` | unsigned int; bits 3..3 |
| 0 | `fSubmit` | unsigned int; bits 4..4 |
| 0 | `fNoResearchSav` | unsigned int; bits 5..5 |
| 0 | `fRadiatingEngine` | unsigned int; bits 6..6 |
| 0 | `fNoIdleChecks` | unsigned int; bits 7..7 |
| 0 | `fSendMsgMode` | unsigned int; bits 8..8 |
| 0 | `fRetryOpens` | unsigned int; bits 9..9 |
| 0 | `fAisDone` | unsigned int; bits 10..10 |
| 0 | `fTutorial` | unsigned int; bits 11..11 |
| 0 | `fGotoVCR` | unsigned int; bits 12..12 |
| 0 | `fVCRTimer` | unsigned int; bits 13..13 |
| 0 | `mdScreenSize` | unsigned int; bits 14..15 |
| 2 | `fGameOverMan` | unsigned int; bits 0..0 |
| 2 | `fDontDoLogFiles` | unsigned int; bits 1..1 |
| 2 | `fFileCrippled` | unsigned int; bits 2..2 |
| 2 | `fSmallTileMode` | unsigned int; bits 3..3 |
| 2 | `fAllAis` | unsigned int; bits 4..4 |
| 2 | `fReadOnly` | unsigned int; bits 5..5 |
| 2 | `fExitWindows` | unsigned int; bits 6..6 |
| 2 | `fPartialTurn` | unsigned int; bits 7..7 |
| 2 | `fSetMassMode` | unsigned int; bits 8..8 |
| 2 | `fRptSafeDraw` | unsigned int; bits 9..9 |
| 2 | `fProgressTxt` | unsigned int; bits 10..10 |
| 2 | `fSoundFX` | unsigned int; bits 11..11 |
| 2 | `fNoSound` | unsigned int; bits 12..12 |
| 2 | `fSetRouteMode` | unsigned int; bits 13..13 |
| 2 | `fBleedingEdge` | unsigned int; bits 14..14 |
| 2 | `fToolbar` | unsigned int; bits 15..15 |
| 4 | `grBits2` | long |
| 4 | `fNoScannerDraw` | unsigned int; bits 0..0 |
| 4 | `fTrialPeriodOver` | unsigned int; bits 1..1 |
| 4 | `fClose` | unsigned int; bits 2..2 |
| 4 | `fDontCalcBleed` | unsigned int; bits 3..3 |
| 4 | `fChgZipOrd` | unsigned int; bits 4..4 |
| 4 | `fChgZipProd` | unsigned int; bits 5..5 |
| 4 | `fChgScanner` | unsigned int; bits 6..6 |
| 4 | `fChgReports` | unsigned int; bits 7..7 |
| 4 | `fWriteTurnNum` | unsigned int; bits 8..8 |
| 4 | `fHotSeat` | unsigned int; bits 9..9 |
| 4 | `fFleetLinkValid` | unsigned int; bits 10..10 |
| 4 | `fScoreVictory` | unsigned int; bits 11..12 |
| 6 | `iCurGraph` | unsigned int; bits 0..3 |
| 6 | `fMusic` | unsigned int; bits 4..4 |
| 6 | `fPerPlayerDumps` | unsigned int; bits 5..5 |
| 6 | `fNoHostNames` | unsigned int; bits 6..6 |
| 8 | `fUnused2` | unsigned int; bits 0..13 |

## _player — 192 bytes (type 0x1014)

| Offset | Member | Type |
|---:|---|---|
| 0 | `iPlayer` | char |
| 1 | `cShDef` | char |
| 2 | `cPlanet` | short |
| 4 | `cFleet` | unsigned short; bits 0..11 |
| 4 | `cshdefSB` | unsigned short; bits 12..15 |
| 6 | `det` | unsigned short; bits 0..2 |
| 6 | `iPlrBmp` | unsigned short; bits 3..7 |
| 6 | `fInclude` | unsigned short; bits 8..8 |
| 6 | `mdPlayer` | unsigned short; bits 9..15 |
| 6 | `reserved` | unsigned short; bits 0..8 |
| 6 | `fAi` | unsigned short; bits 9..9 |
| 6 | `lvlAi` | unsigned short; bits 10..12 |
| 6 | `idAi` | unsigned short; bits 13..15 |
| 6 | `wMdPlr` | unsigned short |
| 8 | `idPlanetHome` | short |
| 10 | `wScore` | unsigned short |
| 12 | `lSalt` | long |
| 16 | `rgEnvVar` | array of char (3 bytes total) |
| 19 | `rgEnvVarMin` | array of char (3 bytes total) |
| 22 | `rgEnvVarMax` | array of char (3 bytes total) |
| 25 | `pctIdealGrowth` | char |
| 26 | `rgTech` | array of char (6 bytes total) |
| 32 | `rgResSpent` | array of unsigned long (24 bytes total) |
| 56 | `pctResearch` | char |
| 57 | `iTechCur` | char |
| 58 | `lResLastYear` | long |
| 62 | `rgAttr` | array of char (16 bytes total) |
| 78 | `grbitAttr` | unsigned long |
| 82 | `grbitTrader` | unsigned short |
| 84 | `fDead` | unsigned short; bits 0..0 |
| 84 | `fCrippled` | unsigned short; bits 1..1 |
| 84 | `fCheater` | unsigned short; bits 2..2 |
| 84 | `fLearned` | unsigned short; bits 3..3 |
| 84 | `fHacker` | unsigned short; bits 4..4 |
| 84 | `unused` | unsigned short; bits 5..15 |
| 84 | `wFlags` | unsigned short |
| 86 | `zpq1` | _zipprodq1 |
| 112 | `rgmdRelation` | array of char (16 bytes total) |
| 128 | `szName` | array of char (32 bytes total) |
| 160 | `szNames` | array of char (32 bytes total) |

## tagPOINT — 4 bytes (type 0x1018)

| Offset | Member | Type |
|---:|---|---|
| 0 | `x` | int |
| 2 | `y` | int |

## tagPAINTSTRUCT — 32 bytes (type 0x1019)

| Offset | Member | Type |
|---:|---|---|
| 0 | `hdc` | unsigned int |
| 2 | `fErase` | int |
| 4 | `rcPaint` | tagRECT |
| 12 | `fRestore` | int |
| 14 | `fIncUpdate` | int |
| 16 | `rgbReserved` | array of unsigned char (16 bytes total) |

## tagRECT — 8 bytes (type 0x101a)

| Offset | Member | Type |
|---:|---|---|
| 0 | `left` | int |
| 2 | `top` | int |
| 4 | `right` | int |
| 6 | `bottom` | int |

## _framestuff — 22 bytes (type 0x1038)

| Offset | Member | Type |
|---:|---|---|
| 0 | `dx` | int |
| 2 | `dy` | int |
| 4 | `xTop` | int |
| 6 | `y1` | int |
| 8 | `y2` | int |
| 10 | `dxPlanWant` | int |
| 12 | `dyMsgWant` | int |
| 14 | `dyMinWant` | int |
| 16 | `dx2PlanWant` | int |
| 18 | `dy2MsgWant` | int |
| 20 | `dy2MinWant` | int |

## _prod — 4 bytes (type 0x1039)

| Offset | Member | Type |
|---:|---|---|
| 0 | `cItem` | unsigned long; bits 0..9 |
| 0 | `iItem` | unsigned long; bits 10..16 |
| 0 | `grobj` | unsigned long; bits 17..19 |
| 0 | `pct` | unsigned long; bits 20..26 |
| 0 | `unused` | unsigned long; bits 27..31 |

## _cyberinfo — 2 bytes (type 0x103e)

| Offset | Member | Type |
|---:|---|---|
| 0 | `iLstPktDir` | unsigned short; bits 0..2 |
| 0 | `fBltColony` | unsigned short; bits 3..3 |
| 0 | `fLaunchedPkt` | unsigned short; bits 4..4 |
| 0 | `iPktTarget` | unsigned short; bits 5..6 |
| 0 | `fNeedScanPkt` | unsigned short; bits 7..7 |
| 0 | `unused` | unsigned short; bits 8..15 |
| 0 | `wInfo` | unsigned short |

## _cyberinfotemp — 2 bytes (type 0x1040)

| Offset | Member | Type |
|---:|---|---|
| 0 | `fIdleColonizers` | unsigned short; bits 0..0 |
| 0 | `cIdleFreighters` | unsigned short; bits 1..2 |
| 0 | `cFreightersDst` | unsigned short; bits 3..4 |
| 0 | `fNeedDefenders` | unsigned short; bits 5..5 |
| 0 | `fDefended` | unsigned short; bits 6..6 |
| 0 | `fUnderAttack` | unsigned short; bits 7..7 |
| 0 | `fNeedsMin1` | unsigned short; bits 8..8 |
| 0 | `fNeedsMin2` | unsigned short; bits 9..9 |
| 0 | `fNeedsMin3` | unsigned short; bits 10..10 |
| 0 | `unused` | unsigned short; bits 11..15 |
| 0 | `wInfo1` | unsigned short |

## _scan — 16 bytes (type 0x1047)

| Offset | Member | Type |
|---:|---|---|
| 0 | `pt` | tagPOINT |
| 4 | `grobj` | int |
| 6 | `grobjFull` | int |
| 8 | `idpl` | int |
| 10 | `ifl` | int |
| 12 | `iwp` | int |
| 14 | `ith` | int |

## PLPROD — 4 bytes (type 0x1054)

| Offset | Member | Type |
|---:|---|---|
| 0 | `cbItem` | unsigned short; bits 0..7 |
| 0 | `fMark` | unsigned short; bits 8..8 |
| 0 | `ht` | unsigned short; bits 9..11 |
| 0 | `cAlloc` | unsigned short; bits 12..15 |
| 2 | `iprodMax` | unsigned char |
| 3 | `iprodMac` | unsigned char |
| 4 | `rgprod` | array of _prod (0 bytes total) |

## _sel — 226 bytes (type 0x105b)

| Offset | Member | Type |
|---:|---|---|
| 0 | `pt` | tagPOINT |
| 4 | `grobj` | short |
| 6 | `grobjFull` | short |
| 8 | `id` | short |
| 10 | `iwpAct` | short |
| 12 | `scan` | _scan |
| 28 | `fl` | _fleet |
| 152 | `pl` | _planet |
| 208 | `th` | _thing |

## _part — 8 bytes (type 0x105e)

| Offset | Member | Type |
|---:|---|---|
| 0 | `hs` | _hs |
| 4 | `pcom` | _compart far * |
| 4 | `parmor` | _armor far * |
| 4 | `phul` | _hul far * |
| 4 | `pengine` | _engine far * |
| 4 | `pscanner` | _scanner far * |
| 4 | `pbeam` | _beam far * |
| 4 | `ptorp` | _torp far * |
| 4 | `pbomb` | _bomb far * |
| 4 | `pshield` | _shield far * |
| 4 | `pspecial` | _special far * |
| 4 | `pspecialsb` | _specialsb far * |
| 4 | `pmines` | _mines far * |
| 4 | `pmining` | _mining far * |
| 4 | `pplanetary` | _planetary far * |
| 4 | `pterra` | _terra far * |

## _hul — 123 bytes (type 0x1076)

| Offset | Member | Type |
|---:|---|---|
| 0 | `ihuldef` | short |
| 2 | `rgTech` | array of char (6 bytes total) |
| 8 | `szClass` | array of char (32 bytes total) |
| 40 | `wtEmpty` | unsigned short |
| 42 | `resCost` | unsigned short |
| 44 | `rgwtOreCost` | array of unsigned short (6 bytes total) |
| 50 | `ibmp` | short |
| 52 | `wtCargoMax` | unsigned short |
| 54 | `wtFuelMax` | unsigned short |
| 56 | `dp` | unsigned short |
| 58 | `rghs` | array of _hs (64 bytes total) |
| 122 | `chs` | unsigned char |

## _hs — 4 bytes (type 0x1078)

| Offset | Member | Type |
|---:|---|---|
| 0 | `grhst` | unsigned short |
| 2 | `iItem` | unsigned short; bits 0..7 |
| 2 | `cItem` | unsigned short; bits 8..15 |

## _ziporder — 24 bytes (type 0x1085)

| Offset | Member | Type |
|---:|---|---|
| 0 | `txp` | _taskxport |
| 10 | `szName` | array of char (13 bytes total) |
| 23 | `fValid` | unsigned char |

## _scorex — 24 bytes (type 0x109a)

| Offset | Member | Type |
|---:|---|---|
| 0 | `wWord` | unsigned short |
| 0 | `iPlayer` | unsigned short; bits 0..4 |
| 0 | `fValid` | unsigned short; bits 5..5 |
| 0 | `grbitVC` | unsigned short; bits 6..13 |
| 0 | `fWinner` | unsigned short; bits 14..14 |
| 0 | `fHistory` | unsigned short; bits 15..15 |
| 2 | `iRank` | int |
| 2 | `turn` | unsigned short |
| 4 | `score` | _score |

## _btldata — 14 bytes (type 0x10a2)

| Offset | Member | Type |
|---:|---|---|
| 0 | `id` | unsigned short |
| 2 | `cplr` | unsigned char |
| 3 | `ctok` | unsigned char |
| 4 | `grfPlr` | unsigned short |
| 6 | `cbData` | unsigned short |
| 8 | `idPlanet` | unsigned short |
| 10 | `pt` | tagPOINT |
| 14 | `rgtok` | array of _tok (0 bytes total) |

## _rpt — 54 bytes (type 0x10b4)

| Offset | Member | Type |
|---:|---|---|
| 0 | `grbitVisible` | long |
| 4 | `irpt` | short |
| 6 | `cFields` | short |
| 8 | `cFieldFirst` | short |
| 10 | `icolSort` | short |
| 12 | `fAscending` | short |
| 14 | `irowFirst` | short |
| 16 | `ptDlg` | tagPOINT |
| 20 | `ptSize` | tagPOINT |
| 24 | `fCached` | short |
| 26 | `rgbdx` | array of unsigned char (16 bytes total) |
| 42 | `cRows` | short |
| 44 | `cRowsVis` | short |
| 46 | `iSubsort` | short |
| 48 | `hwndVScroll` | unsigned int |
| 50 | `hwndHScroll` | unsigned int |
| 52 | `cColScroll` | short |

## _btlplan — 36 bytes (type 0x10be)

| Offset | Member | Type |
|---:|---|---|
| 0 | `iplr` | unsigned short; bits 0..3 |
| 0 | `iplan` | unsigned short; bits 4..7 |
| 0 | `mdTactic` | unsigned short; bits 8..11 |
| 0 | `unused1` | unsigned short; bits 12..13 |
| 0 | `fDelete` | unsigned short; bits 14..14 |
| 0 | `fDumpCargo` | unsigned short; bits 15..15 |
| 2 | `mdTarget1` | unsigned short; bits 0..3 |
| 2 | `mdTarget2` | unsigned short; bits 4..7 |
| 2 | `iplrAttack` | unsigned short; bits 8..12 |
| 2 | `unused2` | unsigned short; bits 13..15 |
| 4 | `szName` | array of unsigned char (32 bytes total) |

## _popupdata — 22 bytes (type 0x10c3)

| Offset | Member | Type |
|---:|---|---|
| 0 | `grPopup` | int |
| 2 | `rgi` | array of long (20 bytes total) |
| 2 | `idPlanet` | int |
| 4 | `iPlanetVar` | int |
| 6 | `iPlanVal` | int |
| 8 | `iPlanMin` | int |
| 10 | `iPlanMax` | int |
| 12 | `iPlrVal` | int |
| 14 | `iPlrMin` | int |
| 16 | `iPlrMax` | int |
| 2 | `idPlan` | int |
| 4 | `cMax` | int |
| 6 | `cCur` | int |
| 8 | `cOperate` | int |
| 10 | `fFactory` | int |
| 2 | `dxOut` | int |
| 4 | `psz` | char near * |
| 2 | `iPlayer` | int |
| 2 | `lpfl` | _fleet far * |
| 6 | `fRedDamage` | int |
| 8 | `dxDamage` | int |
| 10 | `grbit` | unsigned int |
| 2 | `part` | _part |
| 2 | `lpshdef` | _shdef far * |
| 6 | `fShowDamage` | int |
| 8 | `fHideCounts` | int |
| 10 | `fToken` | int |
| 12 | `fSummary` | int |
| 14 | `itok` | int |

## _itemaction — 2 bytes (type 0x10cb)

| Offset | Member | Type |
|---:|---|---|
| 0 | `cQuan` | unsigned short; bits 0..11 |
| 0 | `iAction` | unsigned short; bits 12..15 |

## _tok — 29 bytes (type 0x10e9)

| Offset | Member | Type |
|---:|---|---|
| 0 | `id` | unsigned short |
| 2 | `iplr` | unsigned char |
| 3 | `grobj` | unsigned char |
| 4 | `ishdef` | unsigned char |
| 5 | `brc` | unsigned char |
| 6 | `initBase` | unsigned char |
| 7 | `initMin` | unsigned char |
| 8 | `initMac` | unsigned char |
| 9 | `itokTarget` | unsigned char |
| 10 | `pctCloak` | unsigned char |
| 11 | `pctJam` | unsigned char |
| 12 | `pctBC` | unsigned char |
| 13 | `pctCap` | unsigned char |
| 14 | `pctBeamDef` | unsigned char |
| 15 | `wt` | unsigned short |
| 17 | `dpShield` | unsigned short |
| 19 | `csh` | unsigned short |
| 21 | `dv` | _dv |
| 23 | `mdTarget1` | unsigned short; bits 0..3 |
| 23 | `mdTarget2` | unsigned short; bits 4..7 |
| 23 | `mdTactic` | unsigned short; bits 8..11 |
| 23 | `mdTarget0` | unsigned short; bits 12..15 |
| 25 | `dxyLim` | unsigned short; bits 0..3 |
| 25 | `dxyMax` | unsigned short; bits 4..7 |
| 25 | `spd` | unsigned short; bits 8..11 |
| 25 | `cTarget` | unsigned short; bits 12..15 |
| 27 | `fActive` | unsigned short; bits 0..0 |
| 27 | `fDetector` | unsigned short; bits 1..1 |
| 27 | `fTorp` | unsigned short; bits 2..2 |
| 27 | `fRegen` | unsigned short; bits 3..3 |
| 27 | `fMoved` | unsigned short; bits 4..4 |
| 27 | `dzDis` | unsigned short; bits 5..9 |
| 27 | `dwt` | unsigned short; bits 10..13 |
| 27 | `dMovesLeft` | unsigned short; bits 14..15 |
| 27 | `wFlags` | unsigned short |

## _ini — 26 bytes (type 0x10ec)

| Offset | Member | Type |
|---:|---|---|
| 0 | `wnFrame` | _wn |
| 10 | `fStartupFile` | unsigned short; bits 0..0 |
| 10 | `fCmdLine` | unsigned short; bits 1..1 |
| 10 | `fWait` | unsigned short; bits 2..2 |
| 10 | `fGen` | unsigned short; bits 3..3 |
| 10 | `fTry` | unsigned short; bits 4..4 |
| 10 | `grobjSel` | unsigned short; bits 5..8 |
| 10 | `fBatch` | unsigned short; bits 9..9 |
| 10 | `fNewGame` | unsigned short; bits 10..10 |
| 10 | `fDumpFleets` | unsigned short; bits 11..11 |
| 10 | `fDumpPlanets` | unsigned short; bits 12..12 |
| 10 | `fDumpMap` | unsigned short; bits 13..13 |
| 10 | `fValidate` | unsigned short; bits 14..14 |
| 10 | `fLogging` | unsigned short; bits 15..15 |
| 10 | `wFlags` | unsigned short |
| 12 | `turn` | unsigned int |
| 14 | `iObjSel` | int |
| 16 | `idPlayer` | int |
| 18 | `lid` | long |
| 22 | `cTurnGen` | int |
| 24 | `iMsg` | int |

## _tutor — 44 bytes (type 0x10ed)

| Offset | Member | Type |
|---:|---|---|
| 0 | `wFlags` | int |
| 0 | `fVisible` | unsigned int; bits 0..0 |
| 0 | `fGameSaved` | unsigned int; bits 1..1 |
| 0 | `fChange` | unsigned int; bits 2..2 |
| 0 | `fTurnDone` | unsigned int; bits 3..3 |
| 0 | `fTutorDone` | unsigned int; bits 4..4 |
| 0 | `fNoErrors` | unsigned int; bits 5..5 |
| 0 | `cError` | unsigned int; bits 6..8 |
| 0 | `fAutoComplete` | unsigned int; bits 9..9 |
| 0 | `fProgress` | unsigned int; bits 10..10 |
| 0 | `fTBVis` | unsigned int; bits 11..11 |
| 0 | `fValidQ` | unsigned int; bits 12..12 |
| 0 | `fFreeing` | unsigned int; bits 13..13 |
| 0 | `fShowHidMsg` | unsigned int; bits 14..14 |
| 0 | `unused` | unsigned int; bits 15..15 |
| 2 | `idt` | int |
| 4 | `idtBold` | int |
| 6 | `idh` | int |
| 8 | `idsError` | int |
| 10 | `iScanZoom` | int |
| 12 | `icolFSort` | int |
| 14 | `grbitScan` | unsigned short |
| 16 | `hwnd` | unsigned int |
| 18 | `zpq` | _zipprodq1 |

## _zipprodq1 — 26 bytes (type 0x10ef)

| Offset | Member | Type |
|---:|---|---|
| 0 | `fNoResearch` | unsigned char |
| 1 | `cpq` | unsigned char |
| 2 | `rgpq` | array of _prodq1 (24 bytes total) |

## _zipprodq — 40 bytes (type 0x10f1)

| Offset | Member | Type |
|---:|---|---|
| 0 | `szName` | array of char (13 bytes total) |
| 13 | `fValid` | unsigned char |
| 14 | `zpq1` | _zipprodq1 |
| 14 | `fNoResearch` | unsigned char |
| 15 | `cpq` | unsigned char |
| 16 | `rgpq` | array of _prodq1 (24 bytes total) |

## _aipart — 2 bytes (type 0x1140)

| Offset | Member | Type |
|---:|---|---|
| 0 | `ibit` | unsigned short; bits 0..3 |
| 0 | `iItem` | unsigned short; bits 4..8 |
| 0 | `cItem` | unsigned short; bits 9..12 |
| 0 | `fRandom` | unsigned short; bits 13..15 |

## tagTIMERINFO — 12 bytes (type 0x114b)

| Offset | Member | Type |
|---:|---|---|
| 0 | `dwSize` | unsigned long |
| 4 | `dwmsSinceStart` | unsigned long |
| 8 | `dwmsThisVM` | unsigned long |

## _hb — 16 bytes (type 0x114f)

| Offset | Member | Type |
|---:|---|---|
| 0 | `cbFree` | unsigned short |
| 2 | `cbBlock` | unsigned short |
| 4 | `cbSlop` | unsigned short |
| 6 | `ibTop` | unsigned short |
| 8 | `lphbNext` | _hb far * |
| 12 | `hmem` | unsigned int |
| 14 | `ht` | unsigned char |
| 15 | `unused1` | unsigned char |

## _btlrec — 6 bytes (type 0x1156)

| Offset | Member | Type |
|---:|---|---|
| 0 | `itok` | unsigned char |
| 1 | `brcDest` | unsigned char |
| 2 | `ctok` | int |
| 4 | `iRound` | unsigned short; bits 0..3 |
| 4 | `dzDis` | unsigned short; bits 4..7 |
| 4 | `itokAttack` | unsigned short; bits 8..15 |
| 6 | `rgkill` | array of _kill (0 bytes total) |

## _dv — 2 bytes (type 0x1158)

| Offset | Member | Type |
|---:|---|---|
| 0 | `dp` | unsigned short |
| 0 | `pctSh` | unsigned short; bits 0..6 |
| 0 | `pctDp` | unsigned short; bits 7..15 |

## _btnt — 24 bytes (type 0x115c)

| Offset | Member | Type |
|---:|---|---|
| 0 | `hwnd` | unsigned int |
| 2 | `hdc` | unsigned int |
| 4 | `rc` | tagRECT |
| 12 | `dTimer` | int |
| 14 | `btf` | int |
| 16 | `szText` | char near * |
| 18 | `lTicks` | long |
| 22 | `fFirst` | unsigned short; bits 0..0 |
| 22 | `fDown` | unsigned short; bits 1..1 |
| 22 | `fInitDown` | unsigned short; bits 2..2 |
| 22 | `fCreatedDC` | unsigned short; bits 3..3 |
| 22 | `fNoEndRedraw` | unsigned short; bits 4..4 |
| 22 | `fUnused` | unsigned short; bits 5..15 |

## _find_t — 44 bytes (type 0x116e)

| Offset | Member | Type |
|---:|---|---|
| 0 | `reserved` | array of char (21 bytes total) |
| 21 | `attrib` | char |
| 22 | `wr_time` | unsigned int |
| 24 | `wr_date` | unsigned int |
| 26 | `size` | long |
| 30 | `name` | array of char (13 bytes total) |

## _diskfree_t — 8 bytes (type 0x1170)

| Offset | Member | Type |
|---:|---|---|
| 0 | `total_clusters` | unsigned int |
| 2 | `avail_clusters` | unsigned int |
| 4 | `sectors_per_cluster` | unsigned int |
| 6 | `bytes_per_sector` | unsigned int |

## tagOFSTRUCT — 136 bytes (type 0x117d)

| Offset | Member | Type |
|---:|---|---|
| 0 | `cBytes` | unsigned char |
| 1 | `fFixedDisk` | unsigned char |
| 2 | `nErrCode` | unsigned int |
| 4 | `reserved` | array of unsigned char (4 bytes total) |
| 8 | `szPathName` | array of char (128 bytes total) |

## tagLOGFONT — 50 bytes (type 0x1197)

| Offset | Member | Type |
|---:|---|---|
| 0 | `lfHeight` | int |
| 2 | `lfWidth` | int |
| 4 | `lfEscapement` | int |
| 6 | `lfOrientation` | int |
| 8 | `lfWeight` | int |
| 10 | `lfItalic` | unsigned char |
| 11 | `lfUnderline` | unsigned char |
| 12 | `lfStrikeOut` | unsigned char |
| 13 | `lfCharSet` | unsigned char |
| 14 | `lfOutPrecision` | unsigned char |
| 15 | `lfClipPrecision` | unsigned char |
| 16 | `lfQuality` | unsigned char |
| 17 | `lfPitchAndFamily` | unsigned char |
| 18 | `lfFaceName` | array of char (32 bytes total) |

## tagMSG — 18 bytes (type 0x11ab)

| Offset | Member | Type |
|---:|---|---|
| 0 | `hwnd` | unsigned int |
| 2 | `message` | unsigned int |
| 4 | `wParam` | unsigned int |
| 6 | `lParam` | long |
| 10 | `time` | unsigned long |
| 14 | `pt` | tagPOINT |

## tagBITMAPCOREHEADER — 12 bytes (type 0x11b9)

| Offset | Member | Type |
|---:|---|---|
| 0 | `bcSize` | unsigned long |
| 4 | `bcWidth` | short |
| 6 | `bcHeight` | short |
| 8 | `bcPlanes` | unsigned short |
| 10 | `bcBitCount` | unsigned short |

## tagBITMAPINFOHEADER — 40 bytes (type 0x11bb)

| Offset | Member | Type |
|---:|---|---|
| 0 | `biSize` | unsigned long |
| 4 | `biWidth` | long |
| 8 | `biHeight` | long |
| 12 | `biPlanes` | unsigned short |
| 14 | `biBitCount` | unsigned short |
| 16 | `biCompression` | unsigned long |
| 20 | `biSizeImage` | unsigned long |
| 24 | `biXPelsPerMeter` | long |
| 28 | `biYPelsPerMeter` | long |
| 32 | `biClrUsed` | unsigned long |
| 36 | `biClrImportant` | unsigned long |

## tagLOGPALETTE — 8 bytes (type 0x11be)

| Offset | Member | Type |
|---:|---|---|
| 0 | `palVersion` | unsigned short |
| 2 | `palNumEntries` | unsigned short |
| 4 | `palPalEntry` | array of tagPALETTEENTRY (4 bytes total) |

## tagBITMAP — 14 bytes (type 0x11c5)

| Offset | Member | Type |
|---:|---|---|
| 0 | `bmType` | int |
| 2 | `bmWidth` | int |
| 4 | `bmHeight` | int |
| 6 | `bmWidthBytes` | int |
| 8 | `bmPlanes` | unsigned char |
| 9 | `bmBitsPixel` | unsigned char |
| 10 | `bmBits` | void far * |

## tagTEXTMETRIC — 31 bytes (type 0x11db)

| Offset | Member | Type |
|---:|---|---|
| 0 | `tmHeight` | int |
| 2 | `tmAscent` | int |
| 4 | `tmDescent` | int |
| 6 | `tmInternalLeading` | int |
| 8 | `tmExternalLeading` | int |
| 10 | `tmAveCharWidth` | int |
| 12 | `tmMaxCharWidth` | int |
| 14 | `tmWeight` | int |
| 16 | `tmItalic` | unsigned char |
| 17 | `tmUnderlined` | unsigned char |
| 18 | `tmStruckOut` | unsigned char |
| 19 | `tmFirstChar` | unsigned char |
| 20 | `tmLastChar` | unsigned char |
| 21 | `tmDefaultChar` | unsigned char |
| 22 | `tmBreakChar` | unsigned char |
| 23 | `tmPitchAndFamily` | unsigned char |
| 24 | `tmCharSet` | unsigned char |
| 25 | `tmOverhang` | int |
| 27 | `tmDigitizedAspectX` | int |
| 29 | `tmDigitizedAspectY` | int |

## tagOFN — 72 bytes (type 0x1206)

| Offset | Member | Type |
|---:|---|---|
| 0 | `lStructSize` | unsigned long |
| 4 | `hwndOwner` | unsigned int |
| 6 | `hInstance` | unsigned int |
| 8 | `lpstrFilter` | char far * |
| 12 | `lpstrCustomFilter` | char far * |
| 16 | `nMaxCustFilter` | unsigned long |
| 20 | `nFilterIndex` | unsigned long |
| 24 | `lpstrFile` | char far * |
| 28 | `nMaxFile` | unsigned long |
| 32 | `lpstrFileTitle` | char far * |
| 36 | `nMaxFileTitle` | unsigned long |
| 40 | `lpstrInitialDir` | char far * |
| 44 | `lpstrTitle` | char far * |
| 48 | `Flags` | unsigned long |
| 52 | `nFileOffset` | unsigned int |
| 54 | `nFileExtension` | unsigned int |
| 56 | `lpstrDefExt` | char far * |
| 60 | `lCustData` | long |
| 64 | `lpfnHook` | function returning unsigned int far * |
| 68 | `lpTemplateName` | char far * |

## _engine — 78 bytes (type 0x121c)

| Offset | Member | Type |
|---:|---|---|
| 0 | `id` | int |
| 2 | `rgTech` | array of char (6 bytes total) |
| 8 | `szName` | array of char (32 bytes total) |
| 40 | `cMass` | int |
| 42 | `resCost` | unsigned short |
| 44 | `rgwtOreCost` | array of int (6 bytes total) |
| 50 | `ibmp` | int |
| 52 | `grfAbilities` | int |
| 54 | `rgcFuelUsed` | array of int (24 bytes total) |

## _pl — 4 bytes (type 0x125c)

| Offset | Member | Type |
|---:|---|---|
| 0 | `cbItem` | unsigned short; bits 0..7 |
| 0 | `fMark` | unsigned short; bits 8..8 |
| 0 | `ht` | unsigned short; bits 9..11 |
| 0 | `cAlloc` | unsigned short; bits 12..15 |
| 2 | `iMax` | unsigned char |
| 3 | `iMac` | unsigned char |
| 4 | `rgb` | array of unsigned char (0 bytes total) |

## tagDRAWITEMSTRUCT — 26 bytes (type 0x1277)

| Offset | Member | Type |
|---:|---|---|
| 0 | `CtlType` | unsigned int |
| 2 | `CtlID` | unsigned int |
| 4 | `itemID` | unsigned int |
| 6 | `itemAction` | unsigned int |
| 8 | `itemState` | unsigned int |
| 10 | `hwndItem` | unsigned int |
| 12 | `hDC` | unsigned int |
| 14 | `rcItem` | tagRECT |
| 22 | `itemData` | unsigned long |

## tagMEASUREITEMSTRUCT — 14 bytes (type 0x1279)

| Offset | Member | Type |
|---:|---|---|
| 0 | `CtlType` | unsigned int |
| 2 | `CtlID` | unsigned int |
| 4 | `itemID` | unsigned int |
| 6 | `itemWidth` | unsigned int |
| 8 | `itemHeight` | unsigned int |
| 10 | `itemData` | unsigned long |

## _huldef — 143 bytes (type 0x1289)

| Offset | Member | Type |
|---:|---|---|
| 0 | `hul` | _hul |
| 123 | `init` | unsigned short; bits 0..5 |
| 123 | `imdAttack` | unsigned short; bits 6..9 |
| 123 | `imdCategory` | unsigned short; bits 10..13 |
| 123 | `unused` | unsigned short; bits 14..15 |
| 125 | `wrcCargo` | unsigned short |
| 127 | `rgbrc` | array of unsigned char (16 bytes total) |

## _scanner — 56 bytes (type 0x128d)

| Offset | Member | Type |
|---:|---|---|
| 0 | `id` | int |
| 2 | `rgTech` | array of char (6 bytes total) |
| 8 | `szName` | array of char (32 bytes total) |
| 40 | `cMass` | int |
| 42 | `resCost` | unsigned short |
| 44 | `rgwtOreCost` | array of int (6 bytes total) |
| 50 | `ibmp` | int |
| 52 | `dRange` | int |
| 54 | `grfAbilities` | int |

## _planetary — 54 bytes (type 0x1291)

| Offset | Member | Type |
|---:|---|---|
| 0 | `id` | int |
| 2 | `rgTech` | array of char (6 bytes total) |
| 8 | `szName` | array of char (32 bytes total) |
| 40 | `cMass` | int |
| 42 | `resCost` | unsigned short |
| 44 | `rgwtOreCost` | array of int (6 bytes total) |
| 50 | `ibmp` | int |
| 52 | `grAbility` | int |

## _armor — 54 bytes (type 0x1299)

| Offset | Member | Type |
|---:|---|---|
| 0 | `id` | int |
| 2 | `rgTech` | array of char (6 bytes total) |
| 8 | `szName` | array of char (32 bytes total) |
| 40 | `cMass` | int |
| 42 | `resCost` | unsigned short |
| 44 | `rgwtOreCost` | array of int (6 bytes total) |
| 50 | `ibmp` | int |
| 52 | `dp` | int |

## _shield — 54 bytes (type 0x129c)

| Offset | Member | Type |
|---:|---|---|
| 0 | `id` | int |
| 2 | `rgTech` | array of char (6 bytes total) |
| 8 | `szName` | array of char (32 bytes total) |
| 40 | `cMass` | int |
| 42 | `resCost` | unsigned short |
| 44 | `rgwtOreCost` | array of int (6 bytes total) |
| 50 | `ibmp` | int |
| 52 | `dp` | int |

## _special — 54 bytes (type 0x129e)

| Offset | Member | Type |
|---:|---|---|
| 0 | `id` | int |
| 2 | `rgTech` | array of char (6 bytes total) |
| 8 | `szName` | array of char (32 bytes total) |
| 40 | `cMass` | int |
| 42 | `resCost` | unsigned short |
| 44 | `rgwtOreCost` | array of int (6 bytes total) |
| 50 | `ibmp` | int |
| 52 | `grAbility` | int |

## _mines — 54 bytes (type 0x12a1)

| Offset | Member | Type |
|---:|---|---|
| 0 | `id` | int |
| 2 | `rgTech` | array of char (6 bytes total) |
| 8 | `szName` | array of char (32 bytes total) |
| 40 | `cMass` | int |
| 42 | `resCost` | unsigned short |
| 44 | `rgwtOreCost` | array of int (6 bytes total) |
| 50 | `ibmp` | int |
| 52 | `grAbility` | int |

## _mining — 54 bytes (type 0x12a3)

| Offset | Member | Type |
|---:|---|---|
| 0 | `id` | int |
| 2 | `rgTech` | array of char (6 bytes total) |
| 8 | `szName` | array of char (32 bytes total) |
| 40 | `cMass` | int |
| 42 | `resCost` | unsigned short |
| 44 | `rgwtOreCost` | array of int (6 bytes total) |
| 50 | `ibmp` | int |
| 52 | `grAbility` | int |

## _terra — 54 bytes (type 0x12a6)

| Offset | Member | Type |
|---:|---|---|
| 0 | `id` | int |
| 2 | `rgTech` | array of char (6 bytes total) |
| 8 | `szName` | array of char (32 bytes total) |
| 40 | `cMass` | int |
| 42 | `resCost` | unsigned short |
| 44 | `rgwtOreCost` | array of int (6 bytes total) |
| 50 | `ibmp` | int |
| 52 | `grAbility` | int |

## _bomb — 58 bytes (type 0x12a8)

| Offset | Member | Type |
|---:|---|---|
| 0 | `id` | int |
| 2 | `rgTech` | array of char (6 bytes total) |
| 8 | `szName` | array of char (32 bytes total) |
| 40 | `cMass` | int |
| 42 | `resCost` | unsigned short |
| 44 | `rgwtOreCost` | array of int (6 bytes total) |
| 50 | `ibmp` | int |
| 52 | `cRounds` | int |
| 54 | `dDmgCol` | int |
| 56 | `dDmgBldg` | int |

## _torp — 60 bytes (type 0x12aa)

| Offset | Member | Type |
|---:|---|---|
| 0 | `id` | int |
| 2 | `rgTech` | array of char (6 bytes total) |
| 8 | `szName` | array of char (32 bytes total) |
| 40 | `cMass` | int |
| 42 | `resCost` | unsigned short |
| 44 | `rgwtOreCost` | array of int (6 bytes total) |
| 50 | `ibmp` | int |
| 52 | `dRangeMax` | int |
| 54 | `dp` | int |
| 56 | `init` | int |
| 58 | `dHitChance` | int |

## _beam — 60 bytes (type 0x12ac)

| Offset | Member | Type |
|---:|---|---|
| 0 | `id` | int |
| 2 | `rgTech` | array of char (6 bytes total) |
| 8 | `szName` | array of char (32 bytes total) |
| 40 | `cMass` | int |
| 42 | `resCost` | unsigned short |
| 44 | `rgwtOreCost` | array of int (6 bytes total) |
| 50 | `ibmp` | int |
| 52 | `dRangeMax` | int |
| 54 | `dp` | int |
| 56 | `init` | int |
| 58 | `grfAbilities` | int |

## _specialsb — 56 bytes (type 0x12b2)

| Offset | Member | Type |
|---:|---|---|
| 0 | `id` | int |
| 2 | `rgTech` | array of char (6 bytes total) |
| 8 | `szName` | array of char (32 bytes total) |
| 40 | `cMass` | int |
| 42 | `resCost` | unsigned short |
| 44 | `rgwtOreCost` | array of int (6 bytes total) |
| 50 | `ibmp` | int |
| 52 | `grAbility` | int |
| 54 | `grAbility2` | int |

## _msgplr — 12 bytes (type 0x12d4)

| Offset | Member | Type |
|---:|---|---|
| 0 | `lpmsgplrNext` | _msgplr far * |
| 4 | `iPlrFrom` | int |
| 6 | `iPlrTo` | int |
| 8 | `iInRe` | int |
| 10 | `cLen` | int |
| 12 | `rgbMsg` | array of unsigned char (0 bytes total) |

## _msgbig — 18 bytes (type 0x12d6)

| Offset | Member | Type |
|---:|---|---|
| 0 | `iMsg` | int |
| 2 | `wGoto` | int |
| 4 | `rgParam` | array of int (14 bytes total) |

## _msgturn — 5 bytes (type 0x12de)

| Offset | Member | Type |
|---:|---|---|
| 0 | `iPlr` | unsigned char; bits 0..3 |
| 0 | `cbParams` | unsigned char; bits 4..7 |
| 1 | `msghdr` | _msghdr |

## _msghdr — 4 bytes (type 0x12e0)

| Offset | Member | Type |
|---:|---|---|
| 0 | `iMsg` | unsigned short; bits 0..8 |
| 0 | `grWord` | unsigned short; bits 9..15 |
| 2 | `wGoto` | short |

## _hdr — 2 bytes (type 0x12ec)

| Offset | Member | Type |
|---:|---|---|
| 0 | `cb` | unsigned short; bits 0..9 |
| 0 | `rt` | unsigned short; bits 10..15 |

## _starpack — 4 bytes (type 0x12f6)

| Offset | Member | Type |
|---:|---|---|
| 0 | `dx` | unsigned long; bits 0..9 |
| 0 | `y` | unsigned long; bits 10..21 |
| 0 | `id` | unsigned long; bits 22..31 |

## _rtbof — 16 bytes (type 0x1323)

| Offset | Member | Type |
|---:|---|---|
| 0 | `rgid` | array of char (4 bytes total) |
| 4 | `lidGame` | long |
| 8 | `verInc` | unsigned short; bits 0..4 |
| 8 | `verMinor` | unsigned short; bits 5..11 |
| 8 | `verMajor` | unsigned short; bits 12..15 |
| 8 | `wVersion` | unsigned short |
| 10 | `turn` | unsigned short |
| 12 | `iPlayer` | short; bits 0..4 |
| 12 | `lSaltTime` | short; bits 5..15 |
| 14 | `dt` | unsigned short; bits 0..7 |
| 14 | `fDone` | unsigned short; bits 8..8 |
| 14 | `fInUse` | unsigned short; bits 9..9 |
| 14 | `fMulti` | unsigned short; bits 10..10 |
| 14 | `fGameOverMan` | unsigned short; bits 11..11 |
| 14 | `fCrippled` | unsigned short; bits 12..12 |
| 14 | `wGen` | unsigned short; bits 13..15 |

## _xfer — 128 bytes (type 0x132e)

| Offset | Member | Type |
|---:|---|---|
| 0 | `id` | int |
| 2 | `grobj` | int |
| 4 | `fl` | _fleet |
| 4 | `pl` | _planet |
| 4 | `th` | _thing |

## _xferfull — 25 bytes (type 0x1330)

| Offset | Member | Type |
|---:|---|---|
| 0 | `id1` | unsigned short |
| 2 | `id2` | unsigned short |
| 4 | `grobj1` | unsigned char; bits 0..3 |
| 4 | `grobj2` | unsigned char; bits 4..7 |
| 5 | `rgcQuan` | array of long (20 bytes total) |

## _coldrop — 12 bytes (type 0x1332)

| Offset | Member | Type |
|---:|---|---|
| 0 | `idFleetSrc` | int |
| 2 | `idPlr` | int |
| 4 | `idPlanetDst` | int |
| 6 | `fCanColonize` | unsigned short; bits 0..0 |
| 6 | `unused` | unsigned short; bits 1..15 |
| 8 | `cColonist` | long |

## _score — 20 bytes (type 0x1336)

| Offset | Member | Type |
|---:|---|---|
| 0 | `lScore` | long |
| 4 | `cResources` | long |
| 8 | `cPlanet` | int |
| 10 | `cStarbase` | int |
| 12 | `rgcsh` | array of unsigned short (6 bytes total) |
| 18 | `cTechLevels` | int |

## _turnserial — 16 bytes (type 0x133f)

| Offset | Member | Type |
|---:|---|---|
| 0 | `lSerialNumber` | long |
| 4 | `rgbConfig` | array of unsigned char (11 bytes total) |
| 15 | `bPad` | unsigned char |

## _rthisthdr — 4 bytes (type 0x1342)

| Offset | Member | Type |
|---:|---|---|
| 0 | `cPlanet` | short |
| 2 | `cPlanetExtra` | short |

## _rtChgProdQ — 2 bytes (type 0x1345)

| Offset | Member | Type |
|---:|---|---|
| 0 | `id` | short |
| 2 | `rgprod` | array of _prod (0 bytes total) |

## _prodq1 — 2 bytes (type 0x1349)

| Offset | Member | Type |
|---:|---|---|
| 0 | `w` | unsigned short |
| 0 | `mdIdle` | unsigned short; bits 0..5 |
| 0 | `cQuan` | unsigned short; bits 6..15 |

## tagEVENTMSG — 10 bytes (type 0x134b)

| Offset | Member | Type |
|---:|---|---|
| 0 | `message` | unsigned int |
| 2 | `paramL` | unsigned int |
| 4 | `paramH` | unsigned int |
| 6 | `time` | unsigned long |

## _rtlogthing — 4 bytes (type 0x134e)

| Offset | Member | Type |
|---:|---|---|
| 0 | `idFull` | unsigned short |
| 2 | `fDetonate` | int |

## tagFINDREPLACE — 36 bytes (type 0x1354)

| Offset | Member | Type |
|---:|---|---|
| 0 | `lStructSize` | unsigned long |
| 4 | `hwndOwner` | unsigned int |
| 6 | `hInstance` | unsigned int |
| 8 | `Flags` | unsigned long |
| 12 | `lpstrFindWhat` | char far * |
| 16 | `lpstrReplaceWith` | char far * |
| 20 | `wFindWhatLen` | unsigned int |
| 22 | `wReplaceWithLen` | unsigned int |
| 24 | `lCustData` | long |
| 28 | `lpfnHook` | function returning unsigned int far * |
| 32 | `lpTemplateName` | char far * |

## _rtChgPlanetLong — 6 bytes (type 0x135b)

| Offset | Member | Type |
|---:|---|---|
| 0 | `id` | int |
| 2 | `ul` | unsigned long |
| 2 | `fNoResearch` | unsigned long; bits 0..0 |
| 2 | `idFling` | unsigned long; bits 1..10 |
| 2 | `iWarpFling` | unsigned long; bits 11..14 |
| 2 | `idRoute` | unsigned long; bits 15..24 |
| 2 | `unused` | unsigned long; bits 25..31 |

## _planetsome — 23 bytes (type 0x1366)

| Offset | Member | Type |
|---:|---|---|
| 0 | `id` | short |
| 2 | `iPlayer` | short |
| 4 | `det` | unsigned short; bits 0..7 |
| 4 | `fInclude` | unsigned short; bits 8..8 |
| 4 | `fStarbase` | unsigned short; bits 9..9 |
| 4 | `unusedA` | unsigned short; bits 10..10 |
| 4 | `fFirstYear` | unsigned short; bits 11..11 |
| 4 | `unusedB` | unsigned short; bits 12..15 |
| 6 | `rgpctMinLevel` | array of unsigned short (6 bytes total) |
| 12 | `rgMinConc` | array of char (3 bytes total) |
| 15 | `rgEnvVar` | array of char (3 bytes total) |
| 18 | `rgEnvVarOrig` | array of char (3 bytes total) |
| 21 | `uPopGuess` | unsigned short; bits 0..11 |
| 21 | `uDefGuess` | unsigned short; bits 12..15 |
| 21 | `uGuesses` | unsigned short |

## tagRGBTRIPLE — 3 bytes (type 0x1368)

| Offset | Member | Type |
|---:|---|---|
| 0 | `rgbtBlue` | unsigned char |
| 1 | `rgbtGreen` | unsigned char |
| 2 | `rgbtRed` | unsigned char |

## tagBITMAPCOREINFO — 15 bytes (type 0x136b)

| Offset | Member | Type |
|---:|---|---|
| 0 | `bmciHeader` | tagBITMAPCOREHEADER |
| 12 | `bmciColors` | array of tagRGBTRIPLE (3 bytes total) |

## _exceptionl — 34 bytes (type 0x136f)

| Offset | Member | Type |
|---:|---|---|
| 0 | `type` | int |
| 2 | `name` | char near * |
| 4 | `arg1` | type_0042 |
| 14 | `arg2` | type_0042 |
| 24 | `retval` | type_0042 |

## tagGLYPHMETRICS — 12 bytes (type 0x1377)

| Offset | Member | Type |
|---:|---|---|
| 0 | `gmBlackBoxX` | unsigned int |
| 2 | `gmBlackBoxY` | unsigned int |
| 4 | `gmptGlyphOrigin` | tagPOINT |
| 8 | `gmCellIncX` | int |
| 10 | `gmCellIncY` | int |

## _taskxport — 10 bytes (type 0x137f)

| Offset | Member | Type |
|---:|---|---|
| 0 | `rgia` | array of _itemaction (10 bytes total) |

## _tasklaymines — 4 bytes (type 0x1381)

| Offset | Member | Type |
|---:|---|---|
| 0 | `cTime` | unsigned short |
| 2 | `cTimeOld` | unsigned short |

## _taskpatrol — 4 bytes (type 0x1383)

| Offset | Member | Type |
|---:|---|---|
| 0 | `iWarp` | unsigned short |
| 2 | `iDist` | unsigned short |

## _tasksell — 2 bytes (type 0x1385)

| Offset | Member | Type |
|---:|---|---|
| 0 | `iPlrX` | unsigned short |

## _complexl — 20 bytes (type 0x139a)

| Offset | Member | Type |
|---:|---|---|
| 0 | `x` | type_0042 |
| 10 | `y` | type_0042 |

## tagBITMAPFILEHEADER — 14 bytes (type 0x139d)

| Offset | Member | Type |
|---:|---|---|
| 0 | `bfType` | unsigned int |
| 2 | `bfSize` | unsigned long |
| 6 | `bfReserved1` | unsigned int |
| 8 | `bfReserved2` | unsigned int |
| 10 | `bfOffBits` | unsigned long |

## tagCLIENTCREATESTRUCT — 4 bytes (type 0x13a4)

| Offset | Member | Type |
|---:|---|---|
| 0 | `hWindowMenu` | unsigned int |
| 2 | `idFirstChild` | unsigned int |

## tagWNDCLASS — 26 bytes (type 0x13a7)

| Offset | Member | Type |
|---:|---|---|
| 0 | `style` | unsigned int |
| 2 | `lpfnWndProc` | function returning long far * |
| 6 | `cbClsExtra` | int |
| 8 | `cbWndExtra` | int |
| 10 | `hInstance` | unsigned int |
| 12 | `hIcon` | unsigned int |
| 14 | `hCursor` | unsigned int |
| 16 | `hbrBackground` | unsigned int |
| 18 | `lpszMenuName` | char far * |
| 22 | `lpszClassName` | char far * |

## tagMETAFILEPICT — 8 bytes (type 0x13ac)

| Offset | Member | Type |
|---:|---|---|
| 0 | `mm` | int |
| 2 | `xExt` | int |
| 4 | `yExt` | int |
| 6 | `hMF` | unsigned int |

## tagDEBUGHOOKINFO — 14 bytes (type 0x13ae)

| Offset | Member | Type |
|---:|---|---|
| 0 | `hModuleHook` | unsigned int |
| 2 | `reserved` | long |
| 6 | `lParam` | long |
| 10 | `wParam` | unsigned int |
| 12 | `code` | int |

## _obj — 2 bytes (type 0x13b2)

| Offset | Member | Type |
|---:|---|---|
| 0 | `ppl` | _planet near * |
| 0 | `pfl` | _fleet near * |
| 0 | `pth` | _thing near * |

## _tile — 16 bytes (type 0x13b6)

| Offset | Member | Type |
|---:|---|---|
| 0 | `yTop` | int |
| 2 | `dyFull` | int |
| 4 | `grbit` | int |
| 6 | `pfn` | function returning void far * |
| 10 | `iCol` | unsigned short; bits 0..2 |
| 10 | `id` | unsigned short; bits 3..6 |
| 10 | `fPopped` | unsigned short; bits 7..7 |
| 10 | `fNullPtr` | unsigned short; bits 8..8 |
| 10 | `fMinTitle` | unsigned short; bits 9..9 |
| 10 | `fErase` | unsigned short; bits 10..10 |
| 10 | `fFixCtls` | unsigned short; bits 11..11 |
| 10 | `fMinDraw` | unsigned short; bits 12..12 |
| 12 | `fUnused` | unsigned short; bits 0..3 |
| 14 | `idh` | unsigned short |

## HELPWININFO — 14 bytes (type 0x13df)

| Offset | Member | Type |
|---:|---|---|
| 0 | `wStructSize` | int |
| 2 | `x` | int |
| 4 | `y` | int |
| 6 | `dx` | int |
| 8 | `dy` | int |
| 10 | `wMax` | int |
| 12 | `rgchMember` | array of char (2 bytes total) |

## MENUITEMTEMPLATE — 5 bytes (type 0x13e2)

| Offset | Member | Type |
|---:|---|---|
| 0 | `mtOption` | unsigned int |
| 2 | `mtID` | unsigned int |
| 4 | `mtString` | array of char (1 bytes total) |

## MENUITEMTEMPLATEHEADER — 4 bytes (type 0x13e4)

| Offset | Member | Type |
|---:|---|---|
| 0 | `versionNumber` | unsigned int |
| 2 | `offset` | unsigned int |

## DOCINFO — 10 bytes (type 0x13e6)

| Offset | Member | Type |
|---:|---|---|
| 0 | `cbSize` | int |
| 2 | `lpszDocName` | char far * |
| 6 | `lpszOutput` | char far * |

## tagDEVNAMES — 8 bytes (type 0x13e8)

| Offset | Member | Type |
|---:|---|---|
| 0 | `wDriverOffset` | unsigned int |
| 2 | `wDeviceOffset` | unsigned int |
| 4 | `wOutputOffset` | unsigned int |
| 6 | `wDefault` | unsigned int |

## tagDRVCONFIGINFO — 12 bytes (type 0x13ea)

| Offset | Member | Type |
|---:|---|---|
| 0 | `dwDCISize` | unsigned long |
| 4 | `lpszDCISectionName` | char far * |
| 8 | `lpszDCIAliasName` | char far * |

## _logxfer — 24 bytes (type 0x13ec)

| Offset | Member | Type |
|---:|---|---|
| 0 | `id` | short |
| 2 | `grobj` | short |
| 4 | `rgdItem` | array of long (20 bytes total) |

## _btn — 14 bytes (type 0x13ef)

| Offset | Member | Type |
|---:|---|---|
| 0 | `rc` | tagRECT |
| 8 | `bt` | int |
| 10 | `iVal` | int |
| 12 | `fVisible` | unsigned short; bits 0..0 |
| 12 | `fDisabled` | unsigned short; bits 1..1 |
| 12 | `iSide` | unsigned short; bits 2..3 |
| 12 | `fUnused` | unsigned short; bits 4..15 |

## _kill — 8 bytes (type 0x13f3)

| Offset | Member | Type |
|---:|---|---|
| 0 | `itok` | unsigned char |
| 1 | `grfWeapon` | unsigned char |
| 2 | `cshKill` | unsigned short |
| 4 | `dpShield` | unsigned short |
| 6 | `dv` | _dv |

## _logxferf — 36 bytes (type 0x13fb)

| Offset | Member | Type |
|---:|---|---|
| 0 | `id` | short |
| 2 | `grobj` | short |
| 4 | `rgdItem` | array of short (32 bytes total) |

## _fleetsome — 12 bytes (type 0x13fd)

| Offset | Member | Type |
|---:|---|---|
| 0 | `id` | short |
| 2 | `iPlayer` | short |
| 4 | `det` | unsigned short; bits 0..7 |
| 4 | `fInclude` | unsigned short; bits 8..8 |
| 4 | `fRepOrders` | unsigned short; bits 9..9 |
| 4 | `fDead` | unsigned short; bits 10..10 |
| 4 | `fByteCsh` | unsigned short; bits 11..11 |
| 4 | `unused` | unsigned short; bits 12..15 |
| 6 | `idPlanet` | short |
| 8 | `pt` | tagPOINT |

## tagMETAHEADER — 18 bytes (type 0x1400)

| Offset | Member | Type |
|---:|---|---|
| 0 | `mtType` | unsigned int |
| 2 | `mtHeaderSize` | unsigned int |
| 4 | `mtVersion` | unsigned int |
| 6 | `mtSize` | unsigned long |
| 10 | `mtNoObjects` | unsigned int |
| 12 | `mtMaxRecord` | unsigned long |
| 16 | `mtNoParameters` | unsigned int |

## tagWINDEBUGINFO — 26 bytes (type 0x1402)

| Offset | Member | Type |
|---:|---|---|
| 0 | `flags` | unsigned int |
| 2 | `dwOptions` | unsigned long |
| 6 | `dwFilter` | unsigned long |
| 10 | `achAllocModule` | array of char (8 bytes total) |
| 18 | `dwAllocBreak` | unsigned long |
| 22 | `dwAllocCount` | unsigned long |

## tagLOGPEN — 10 bytes (type 0x1407)

| Offset | Member | Type |
|---:|---|---|
| 0 | `lopnStyle` | unsigned int |
| 2 | `lopnWidth` | tagPOINT |
| 6 | `lopnColor` | unsigned long |

## _rtxfer — 7 bytes (type 0x140c)

| Offset | Member | Type |
|---:|---|---|
| 0 | `id1` | unsigned short |
| 2 | `id2` | unsigned short |
| 4 | `grobj1` | unsigned char; bits 0..3 |
| 4 | `grobj2` | unsigned char; bits 4..7 |
| 5 | `grbitItems` | unsigned char |
| 6 | `rgcQuan` | array of char (1 bytes total) |

## tagFIXED — 4 bytes (type 0x140e)

| Offset | Member | Type |
|---:|---|---|
| 0 | `fract` | unsigned int |
| 2 | `value` | int |

## _compart — 52 bytes (type 0x1415)

| Offset | Member | Type |
|---:|---|---|
| 0 | `id` | int |
| 2 | `rgTech` | array of char (6 bytes total) |
| 8 | `szName` | array of char (32 bytes total) |
| 40 | `cMass` | int |
| 42 | `resCost` | unsigned short |
| 44 | `rgwtOreCost` | array of int (6 bytes total) |
| 50 | `ibmp` | int |

## tagDCB — 25 bytes (type 0x141d)

| Offset | Member | Type |
|---:|---|---|
| 0 | `Id` | unsigned char |
| 1 | `BaudRate` | unsigned int |
| 3 | `ByteSize` | unsigned char |
| 4 | `Parity` | unsigned char |
| 5 | `StopBits` | unsigned char |
| 6 | `RlsTimeout` | unsigned int |
| 8 | `CtsTimeout` | unsigned int |
| 10 | `DsrTimeout` | unsigned int |
| 12 | `fBinary` | unsigned int; bits 0..0 |
| 12 | `fRtsDisable` | unsigned int; bits 1..1 |
| 12 | `fParity` | unsigned int; bits 2..2 |
| 12 | `fOutxCtsFlow` | unsigned int; bits 3..3 |
| 12 | `fOutxDsrFlow` | unsigned int; bits 4..4 |
| 12 | `fDummy` | unsigned int; bits 5..6 |
| 12 | `fDtrDisable` | unsigned int; bits 7..7 |
| 12 | `fOutX` | unsigned int; bits 8..8 |
| 12 | `fInX` | unsigned int; bits 9..9 |
| 12 | `fPeChar` | unsigned int; bits 10..10 |
| 12 | `fNull` | unsigned int; bits 11..11 |
| 12 | `fChEvt` | unsigned int; bits 12..12 |
| 12 | `fDtrflow` | unsigned int; bits 13..13 |
| 12 | `fRtsflow` | unsigned int; bits 14..14 |
| 12 | `fDummy2` | unsigned int; bits 15..15 |
| 14 | `XonChar` | char |
| 15 | `XoffChar` | char |
| 16 | `XonLim` | unsigned int |
| 18 | `XoffLim` | unsigned int |
| 20 | `PeChar` | char |
| 21 | `EofChar` | char |
| 22 | `EvtChar` | char |
| 23 | `TxDelay` | unsigned int |

## _rtxferx — 8 bytes (type 0x1421)

| Offset | Member | Type |
|---:|---|---|
| 0 | `id1` | unsigned short |
| 2 | `id2` | unsigned short |
| 4 | `grobj1` | unsigned char; bits 0..3 |
| 4 | `grobj2` | unsigned char; bits 4..7 |
| 5 | `grbitItems` | unsigned char |
| 6 | `rgcQuan` | array of short (2 bytes total) |

## _thmine — 10 bytes (type 0x1429)

| Offset | Member | Type |
|---:|---|---|
| 0 | `cMines` | long |
| 4 | `grbitPlr` | unsigned short |
| 6 | `iType` | unsigned char |
| 7 | `fDetonate` | unsigned char |
| 8 | `grbitPlrNow` | unsigned short |

## tagCOMSTAT — 5 bytes (type 0x142b)

| Offset | Member | Type |
|---:|---|---|
| 0 | `status` | unsigned char |
| 1 | `cbInQue` | unsigned int |
| 3 | `cbOutQue` | unsigned int |

## tagMAT2 — 16 bytes (type 0x142d)

| Offset | Member | Type |
|---:|---|---|
| 0 | `eM11` | tagFIXED |
| 4 | `eM12` | tagFIXED |
| 8 | `eM21` | tagFIXED |
| 12 | `eM22` | tagFIXED |

## _rtxferl — 10 bytes (type 0x1431)

| Offset | Member | Type |
|---:|---|---|
| 0 | `id1` | unsigned short |
| 2 | `id2` | unsigned short |
| 4 | `grobj1` | unsigned char; bits 0..3 |
| 4 | `grobj2` | unsigned char; bits 4..7 |
| 5 | `grbitItems` | unsigned char |
| 6 | `rgcQuan` | array of long (4 bytes total) |

## _rtxferf — 9 bytes (type 0x1436)

| Offset | Member | Type |
|---:|---|---|
| 0 | `id1` | unsigned short |
| 2 | `id2` | unsigned short |
| 4 | `grobj1` | unsigned char; bits 0..3 |
| 4 | `grobj2` | unsigned char; bits 4..7 |
| 5 | `grbitItems` | unsigned short |
| 7 | `rgcQuan` | array of short (2 bytes total) |

## _btlrec26 — 6 bytes (type 0x1438)

| Offset | Member | Type |
|---:|---|---|
| 0 | `itok` | unsigned char |
| 1 | `brcDest` | unsigned char |
| 2 | `itokAttack` | unsigned char |
| 3 | `ctok` | unsigned char |
| 4 | `iRound` | unsigned short; bits 0..3 |
| 4 | `dzDis` | unsigned short; bits 4..7 |
| 4 | `unused` | unsigned short; bits 8..15 |
| 6 | `rgkill` | array of _kill (0 bytes total) |

## tagRGBQUAD — 4 bytes (type 0x143c)

| Offset | Member | Type |
|---:|---|---|
| 0 | `rgbBlue` | unsigned char |
| 1 | `rgbGreen` | unsigned char |
| 2 | `rgbRed` | unsigned char |
| 3 | `rgbReserved` | unsigned char |

## _thpack — 10 bytes (type 0x1441)

| Offset | Member | Type |
|---:|---|---|
| 0 | `idPlanet` | unsigned short; bits 0..9 |
| 0 | `iWarp` | unsigned short; bits 10..13 |
| 0 | `fMoved` | unsigned short; bits 14..14 |
| 0 | `fInclude` | unsigned short; bits 15..15 |
| 2 | `rgwtMin` | array of int (6 bytes total) |
| 8 | `wtMax` | unsigned short; bits 0..13 |
| 8 | `iDecayRate` | unsigned short; bits 14..15 |

## _rtshipint — 4 bytes (type 0x1443)

| Offset | Member | Type |
|---:|---|---|
| 0 | `id` | short |
| 2 | `i` | short |

## _aistarbase — 20 bytes (type 0x1446)

| Offset | Member | Type |
|---:|---|---|
| 0 | `idPlanet` | int |
| 2 | `cFreighter` | int |
| 4 | `rgflid` | array of int (16 bytes total) |

## _exception — 28 bytes (type 0x1448)

| Offset | Member | Type |
|---:|---|---|
| 0 | `type` | int |
| 2 | `name` | char near * |
| 4 | `arg1` | double |
| 12 | `arg2` | double |
| 20 | `retval` | double |

## tagHANDLETABLE — 2 bytes (type 0x144b)

| Offset | Member | Type |
|---:|---|---|
| 0 | `objectHandle` | array of unsigned int (2 bytes total) |

## _sbar — 12 bytes (type 0x144e)

| Offset | Member | Type |
|---:|---|---|
| 0 | `grbit` | int |
| 2 | `id` | int |
| 4 | `pt` | tagPOINT |
| 8 | `psz` | char near * |
| 10 | `pscan` | _scan near * |

## _rtshipint2 — 6 bytes (type 0x1451)

| Offset | Member | Type |
|---:|---|---|
| 0 | `id` | short |
| 2 | `i` | short |
| 4 | `i2` | short |

## tagWINDOWPOS — 14 bytes (type 0x1454)

| Offset | Member | Type |
|---:|---|---|
| 0 | `hwnd` | unsigned int |
| 2 | `hwndInsertAfter` | unsigned int |
| 4 | `x` | int |
| 6 | `y` | int |
| 8 | `cx` | int |
| 10 | `cy` | int |
| 12 | `flags` | unsigned int |

## tagCREATESTRUCT — 34 bytes (type 0x1457)

| Offset | Member | Type |
|---:|---|---|
| 0 | `lpCreateParams` | void far * |
| 4 | `hInstance` | unsigned int |
| 6 | `hMenu` | unsigned int |
| 8 | `hwndParent` | unsigned int |
| 10 | `cy` | int |
| 12 | `cx` | int |
| 14 | `y` | int |
| 16 | `x` | int |
| 18 | `style` | long |
| 22 | `lpszName` | char far * |
| 26 | `lpszClass` | char far * |
| 30 | `dwExStyle` | unsigned long |

## tagPD — 52 bytes (type 0x1459)

| Offset | Member | Type |
|---:|---|---|
| 0 | `lStructSize` | unsigned long |
| 4 | `hwndOwner` | unsigned int |
| 6 | `hDevMode` | unsigned int |
| 8 | `hDevNames` | unsigned int |
| 10 | `hDC` | unsigned int |
| 12 | `Flags` | unsigned long |
| 16 | `nFromPage` | unsigned int |
| 18 | `nToPage` | unsigned int |
| 20 | `nMinPage` | unsigned int |
| 22 | `nMaxPage` | unsigned int |
| 24 | `nCopies` | unsigned int |
| 26 | `hInstance` | unsigned int |
| 28 | `lCustData` | long |
| 32 | `lpfnPrintHook` | function returning unsigned int far * |
| 36 | `lpfnSetupHook` | function returning unsigned int far * |
| 40 | `lpPrintTemplateName` | char far * |
| 44 | `lpSetupTemplateName` | char far * |
| 48 | `hPrintTemplate` | unsigned int |
| 50 | `hSetupTemplate` | unsigned int |

## _thworm — 8 bytes (type 0x145d)

| Offset | Member | Type |
|---:|---|---|
| 0 | `iStable` | unsigned short; bits 0..1 |
| 0 | `cLastMove` | unsigned short; bits 2..11 |
| 0 | `fDestKnown` | unsigned short; bits 12..12 |
| 0 | `fInclude` | unsigned short; bits 13..13 |
| 2 | `grbitPlr` | unsigned short |
| 4 | `grbitPlrTrav` | unsigned short |
| 6 | `idPartner` | unsigned short |

## _aihist — 1284 bytes (type 0x1462)

| Offset | Member | Type |
|---:|---|---|
| 0 | `cbAiHist` | unsigned short |
| 2 | `cStarbase` | int |
| 4 | `rgasb` | array of _aistarbase (1280 bytes total) |

## tagCHOOSEFONT — 46 bytes (type 0x1468)

| Offset | Member | Type |
|---:|---|---|
| 0 | `lStructSize` | unsigned long |
| 4 | `hwndOwner` | unsigned int |
| 6 | `hDC` | unsigned int |
| 8 | `lpLogFont` | tagLOGFONT far * |
| 12 | `iPointSize` | int |
| 14 | `Flags` | unsigned long |
| 18 | `rgbColors` | unsigned long |
| 22 | `lCustData` | long |
| 26 | `lpfnHook` | function returning unsigned int far * |
| 30 | `lpTemplateName` | char far * |
| 34 | `hInstance` | unsigned int |
| 36 | `lpszStyle` | char far * |
| 40 | `nFontType` | unsigned int |
| 42 | `nSizeMin` | int |
| 44 | `nSizeMax` | int |

## _rtwaypt — 22 bytes (type 0x146a)

| Offset | Member | Type |
|---:|---|---|
| 0 | `id` | short |
| 2 | `iWaypt` | short |
| 4 | `order` | _order |

## _drawcir — 24 bytes (type 0x1477)

| Offset | Member | Type |
|---:|---|---|
| 0 | `rgx` | int near * |
| 2 | `rgy` | int near * |
| 4 | `rgrad` | int near * |
| 6 | `cCur` | int |
| 8 | `cMax` | int |
| 10 | `hdc` | unsigned int |
| 12 | `rcClip` | tagRECT |
| 20 | `fCovered` | int |
| 22 | `fHollowOut` | int |

## tagMETARECORD — 8 bytes (type 0x1479)

| Offset | Member | Type |
|---:|---|---|
| 0 | `rdSize` | unsigned long |
| 4 | `rdFunction` | unsigned int |
| 6 | `rdParm` | array of unsigned int (2 bytes total) |

## tagMDICREATESTRUCT — 26 bytes (type 0x147b)

| Offset | Member | Type |
|---:|---|---|
| 0 | `szClass` | char far * |
| 4 | `szTitle` | char far * |
| 8 | `hOwner` | unsigned int |
| 10 | `x` | int |
| 12 | `y` | int |
| 14 | `cx` | int |
| 16 | `cy` | int |
| 18 | `style` | unsigned long |
| 22 | `lParam` | long |

## _timer — 10 bytes (type 0x1482)

| Offset | Member | Type |
|---:|---|---|
| 0 | `mdForce` | int |
| 2 | `fAutoGenWhenIn` | int |
| 4 | `hrsForce` | int |
| 4 | `minForce` | unsigned int; bits 0..11 |
| 4 | `cPlr` | unsigned int; bits 12..15 |
| 6 | `tickcount` | long |

## _complex — 16 bytes (type 0x1484)

| Offset | Member | Type |
|---:|---|---|
| 0 | `x` | double |
| 8 | `y` | double |

## _rtchgname — 37 bytes (type 0x1486)

| Offset | Member | Type |
|---:|---|---|
| 0 | `id` | short |
| 2 | `grobj` | short |
| 4 | `rgb` | array of unsigned char (33 bytes total) |

## complex — 16 bytes (type 0x1489)

| Offset | Member | Type |
|---:|---|---|
| 0 | `x` | double |
| 8 | `y` | double |

## _lsb — 4 bytes (type 0x148b)

| Offset | Member | Type |
|---:|---|---|
| 0 | `isb` | unsigned short; bits 0..3 |
| 0 | `pctDp` | unsigned short; bits 4..15 |
| 2 | `idFling` | unsigned short; bits 0..9 |
| 2 | `iWarpFling` | unsigned short; bits 10..13 |
| 2 | `unused3` | unsigned short; bits 14..15 |

## tagPOINTFX — 8 bytes (type 0x148d)

| Offset | Member | Type |
|---:|---|---|
| 0 | `x` | tagFIXED |
| 4 | `y` | tagFIXED |

## tagPANOSE — 10 bytes (type 0x148f)

| Offset | Member | Type |
|---:|---|---|
| 0 | `bFamilyType` | unsigned char |
| 1 | `bSerifStyle` | unsigned char |
| 2 | `bWeight` | unsigned char |
| 3 | `bProportion` | unsigned char |
| 4 | `bContrast` | unsigned char |
| 5 | `bStrokeVariation` | unsigned char |
| 6 | `bArmStyle` | unsigned char |
| 7 | `bLetterform` | unsigned char |
| 8 | `bMidline` | unsigned char |
| 9 | `bXHeight` | unsigned char |

## tagOUTLINETEXTMETRIC — 114 bytes (type 0x1491)

| Offset | Member | Type |
|---:|---|---|
| 0 | `otmSize` | unsigned int |
| 2 | `otmTextMetrics` | tagTEXTMETRIC |
| 33 | `otmFiller` | unsigned char |
| 34 | `otmPanoseNumber` | tagPANOSE |
| 44 | `otmfsSelection` | unsigned int |
| 46 | `otmfsType` | unsigned int |
| 48 | `otmsCharSlopeRise` | int |
| 50 | `otmsCharSlopeRun` | int |
| 52 | `otmItalicAngle` | int |
| 54 | `otmEMSquare` | unsigned int |
| 56 | `otmAscent` | int |
| 58 | `otmDescent` | int |
| 60 | `otmLineGap` | unsigned int |
| 62 | `otmsCapEmHeight` | unsigned int |
| 64 | `otmsXHeight` | unsigned int |
| 66 | `otmrcFontBox` | tagRECT |
| 74 | `otmMacAscent` | int |
| 76 | `otmMacDescent` | int |
| 78 | `otmMacLineGap` | unsigned int |
| 80 | `otmusMinimumPPEM` | unsigned int |
| 82 | `otmptSubscriptSize` | tagPOINT |
| 86 | `otmptSubscriptOffset` | tagPOINT |
| 90 | `otmptSuperscriptSize` | tagPOINT |
| 94 | `otmptSuperscriptOffset` | tagPOINT |
| 98 | `otmsStrikeoutSize` | unsigned int |
| 100 | `otmsStrikeoutPosition` | int |
| 102 | `otmsUnderscorePosition` | int |
| 104 | `otmsUnderscoreSize` | int |
| 106 | `otmpFamilyName` | char near * |
| 108 | `otmpFaceName` | char near * |
| 110 | `otmpStyleName` | char near * |
| 112 | `otmpFullName` | char near * |

## _thtrader — 10 bytes (type 0x1496)

| Offset | Member | Type |
|---:|---|---|
| 0 | `ptDest` | tagPOINT |
| 4 | `iWarp` | unsigned short; bits 0..3 |
| 4 | `fInclude` | unsigned short; bits 4..4 |
| 4 | `unused` | unsigned short; bits 5..15 |
| 6 | `grbitPlr` | unsigned short |
| 8 | `grbitTrader` | unsigned short |

## tagCBT_CREATEWND — 6 bytes (type 0x149a)

| Offset | Member | Type |
|---:|---|---|
| 0 | `lpcs` | tagCREATESTRUCT far * |
| 4 | `hwndInsertAfter` | unsigned int |

## tagMOUSEHOOKSTRUCT — 12 bytes (type 0x149f)

| Offset | Member | Type |
|---:|---|---|
| 0 | `pt` | tagPOINT |
| 4 | `hwnd` | unsigned int |
| 6 | `wHitTestCode` | unsigned int |
| 8 | `dwExtraInfo` | unsigned long |

## tagTTPOLYCURVE — 12 bytes (type 0x14a6)

| Offset | Member | Type |
|---:|---|---|
| 0 | `wType` | unsigned int |
| 2 | `cpfx` | unsigned int |
| 4 | `apfx` | array of tagPOINTFX (8 bytes total) |

## tagNEWTEXTMETRIC — 41 bytes (type 0x14b2)

| Offset | Member | Type |
|---:|---|---|
| 0 | `tmHeight` | int |
| 2 | `tmAscent` | int |
| 4 | `tmDescent` | int |
| 6 | `tmInternalLeading` | int |
| 8 | `tmExternalLeading` | int |
| 10 | `tmAveCharWidth` | int |
| 12 | `tmMaxCharWidth` | int |
| 14 | `tmWeight` | int |
| 16 | `tmItalic` | unsigned char |
| 17 | `tmUnderlined` | unsigned char |
| 18 | `tmStruckOut` | unsigned char |
| 19 | `tmFirstChar` | unsigned char |
| 20 | `tmLastChar` | unsigned char |
| 21 | `tmDefaultChar` | unsigned char |
| 22 | `tmBreakChar` | unsigned char |
| 23 | `tmPitchAndFamily` | unsigned char |
| 24 | `tmCharSet` | unsigned char |
| 25 | `tmOverhang` | int |
| 27 | `tmDigitizedAspectX` | int |
| 29 | `tmDigitizedAspectY` | int |
| 31 | `ntmFlags` | unsigned long |
| 35 | `ntmSizeEM` | unsigned int |
| 37 | `ntmCellHeight` | unsigned int |
| 39 | `ntmAvgWidth` | unsigned int |

## tagWINDOWPLACEMENT — 22 bytes (type 0x14b5)

| Offset | Member | Type |
|---:|---|---|
| 0 | `length` | unsigned int |
| 2 | `flags` | unsigned int |
| 4 | `showCmd` | unsigned int |
| 6 | `ptMinPosition` | tagPOINT |
| 10 | `ptMaxPosition` | tagPOINT |
| 14 | `rcNormalPosition` | tagRECT |

## tagCBTACTIVATESTRUCT — 4 bytes (type 0x14b7)

| Offset | Member | Type |
|---:|---|---|
| 0 | `fMouse` | int |
| 2 | `hWndActive` | unsigned int |

## tagTTPOLYGONHEADER — 16 bytes (type 0x14c8)

| Offset | Member | Type |
|---:|---|---|
| 0 | `cb` | unsigned long |
| 4 | `dwType` | unsigned long |
| 8 | `pfxStart` | tagPOINTFX |

## tagPALETTEENTRY — 4 bytes (type 0x14ca)

| Offset | Member | Type |
|---:|---|---|
| 0 | `peRed` | unsigned char |
| 1 | `peGreen` | unsigned char |
| 2 | `peBlue` | unsigned char |
| 3 | `peFlags` | unsigned char |

## _rtshdef — 17 bytes (type 0x14cf)

| Offset | Member | Type |
|---:|---|---|
| 0 | `det` | unsigned short; bits 0..7 |
| 0 | `fInclude` | unsigned short; bits 8..8 |
| 0 | `fFree` | unsigned short; bits 9..9 |
| 0 | `ishdef` | unsigned short; bits 10..14 |
| 0 | `fGift` | unsigned short; bits 15..15 |
| 0 | `wFlags` | unsigned short |
| 2 | `ihuldef` | unsigned char |
| 3 | `ibmp` | unsigned char |
| 4 | `wtEmpty` | unsigned short |
| 4 | `dp` | unsigned short |
| 6 | `chs` | unsigned char |
| 7 | `turn` | unsigned short |
| 9 | `cBuilt` | unsigned long |
| 13 | `cExist` | unsigned long |
| 17 | `rghs` | array of _hs (0 bytes total) |

## _rtchgshdef — 19 bytes (type 0x14d9)

| Offset | Member | Type |
|---:|---|---|
| 0 | `mdChg` | unsigned short; bits 0..3 |
| 0 | `iPlr` | unsigned short; bits 4..7 |
| 0 | `ishdef` | unsigned short; bits 8..12 |
| 0 | `junk` | unsigned short; bits 13..15 |
| 2 | `rtshdef` | _rtshdef |

## tagDELETEITEMSTRUCT — 12 bytes (type 0x14db)

| Offset | Member | Type |
|---:|---|---|
| 0 | `CtlType` | unsigned int |
| 2 | `CtlID` | unsigned int |
| 4 | `itemID` | unsigned int |
| 6 | `hwndItem` | unsigned int |
| 8 | `itemData` | unsigned long |

## tagHARDWAREHOOKSTRUCT — 10 bytes (type 0x14dd)

| Offset | Member | Type |
|---:|---|---|
| 0 | `hWnd` | unsigned int |
| 2 | `wMessage` | unsigned int |
| 4 | `wParam` | unsigned int |
| 6 | `lParam` | long |

## _rtplanet — 4 bytes (type 0x14e2)

| Offset | Member | Type |
|---:|---|---|
| 0 | `id` | short; bits 0..10 |
| 0 | `iPlayer` | short; bits 11..15 |
| 2 | `det` | unsigned short; bits 0..6 |
| 2 | `fHomeworld` | unsigned short; bits 7..7 |
| 2 | `fInclude` | unsigned short; bits 8..8 |
| 2 | `fStarbase` | unsigned short; bits 9..9 |
| 2 | `fIncEVO` | unsigned short; bits 10..10 |
| 2 | `fIncImp` | unsigned short; bits 11..11 |
| 2 | `fIsArtifact` | unsigned short; bits 12..12 |
| 2 | `fIncSurfMin` | unsigned short; bits 13..13 |
| 2 | `fRouting` | unsigned short; bits 14..14 |
| 2 | `fFirstYear` | unsigned short; bits 15..15 |

## tagDRIVERINFOSTRUCT — 134 bytes (type 0x14e5)

| Offset | Member | Type |
|---:|---|---|
| 0 | `length` | unsigned int |
| 2 | `hDriver` | unsigned int |
| 4 | `hModule` | unsigned int |
| 6 | `szAliasName` | array of char (128 bytes total) |

## tagCHOOSECOLOR — 32 bytes (type 0x14ea)

| Offset | Member | Type |
|---:|---|---|
| 0 | `lStructSize` | unsigned long |
| 4 | `hwndOwner` | unsigned int |
| 6 | `hInstance` | unsigned int |
| 8 | `rgbResult` | unsigned long |
| 12 | `lpCustColors` | unsigned long far * |
| 16 | `Flags` | unsigned long |
| 20 | `lCustData` | long |
| 24 | `lpfnHook` | function returning unsigned int far * |
| 28 | `lpTemplateName` | char far * |

## tagABC — 6 bytes (type 0x14f0)

| Offset | Member | Type |
|---:|---|---|
| 0 | `abcA` | int |
| 2 | `abcB` | unsigned int |
| 4 | `abcC` | int |

## tagLOGBRUSH — 8 bytes (type 0x14f5)

| Offset | Member | Type |
|---:|---|---|
| 0 | `lbStyle` | unsigned int |
| 2 | `lbColor` | unsigned long |
| 6 | `lbHatch` | int |

## tagSIZE — 4 bytes (type 0x14f8)

| Offset | Member | Type |
|---:|---|---|
| 0 | `cx` | int |
| 2 | `cy` | int |

## _fleetid — 2 bytes (type 0x14fc)

| Offset | Member | Type |
|---:|---|---|
| 0 | `ifl` | unsigned short; bits 0..8 |
| 0 | `iplr` | unsigned short; bits 9..12 |
| 0 | `junk` | unsigned short; bits 13..15 |

## tagMULTIKEYHELP — 4 bytes (type 0x14ff)

| Offset | Member | Type |
|---:|---|---|
| 0 | `mkSize` | unsigned int |
| 2 | `mkKeylist` | unsigned char |
| 3 | `szKeyphrase` | array of unsigned char (1 bytes total) |

## _vers — 2 bytes (type 0x1502)

| Offset | Member | Type |
|---:|---|---|
| 0 | `verInc` | unsigned short; bits 0..4 |
| 0 | `verMinor` | unsigned short; bits 5..11 |
| 0 | `verMajor` | unsigned short; bits 12..15 |

## tagNCCALCSIZE_PARAMS — 28 bytes (type 0x150a)

| Offset | Member | Type |
|---:|---|---|
| 0 | `rgrc` | array of tagRECT (24 bytes total) |
| 24 | `lppos` | tagWINDOWPOS far * |

## tagCOMPAREITEMSTRUCT — 18 bytes (type 0x150f)

| Offset | Member | Type |
|---:|---|---|
| 0 | `CtlType` | unsigned int |
| 2 | `CtlID` | unsigned int |
| 4 | `hwndItem` | unsigned int |
| 6 | `itemID1` | unsigned int |
| 8 | `itemData1` | unsigned long |
| 12 | `itemID2` | unsigned int |
| 14 | `itemData2` | unsigned long |

## tagMINMAXINFO — 20 bytes (type 0x1511)

| Offset | Member | Type |
|---:|---|---|
| 0 | `ptReserved` | tagPOINT |
| 4 | `ptMaxSize` | tagPOINT |
| 8 | `ptMaxPosition` | tagPOINT |
| 12 | `ptMinTrackSize` | tagPOINT |
| 16 | `ptMaxTrackSize` | tagPOINT |

## tagSEGINFO — 16 bytes (type 0x1513)

| Offset | Member | Type |
|---:|---|---|
| 0 | `offSegment` | unsigned int |
| 2 | `cbSegment` | unsigned int |
| 4 | `flags` | unsigned int |
| 6 | `cbAlloc` | unsigned int |
| 8 | `h` | unsigned int |
| 10 | `alignShift` | unsigned int |
| 12 | `reserved` | array of unsigned int (4 bytes total) |

## tagKERNINGPAIR — 6 bytes (type 0x1518)

| Offset | Member | Type |
|---:|---|---|
| 0 | `wFirst` | unsigned short |
| 2 | `wSecond` | unsigned short |
| 4 | `iKernAmount` | int |

## tagENUMLOGFONT — 146 bytes (type 0x151f)

| Offset | Member | Type |
|---:|---|---|
| 0 | `elfLogFont` | tagLOGFONT |
| 50 | `elfFullName` | array of char (64 bytes total) |
| 114 | `elfStyle` | array of char (32 bytes total) |

## PLORD — 4 bytes (type 0x1528)

| Offset | Member | Type |
|---:|---|---|
| 0 | `cbItem` | unsigned short; bits 0..7 |
| 0 | `fMark` | unsigned short; bits 8..8 |
| 0 | `ht` | unsigned short; bits 9..11 |
| 0 | `cAlloc` | unsigned short; bits 12..15 |
| 2 | `iordMax` | unsigned char |
| 3 | `iordMac` | unsigned char |
| 4 | `rgord` | array of _order (0 bytes total) |

## _selSome — 28 bytes (type 0x152e)

| Offset | Member | Type |
|---:|---|---|
| 0 | `pt` | tagPOINT |
| 4 | `grobj` | short |
| 6 | `grobjFull` | short |
| 8 | `id` | short |
| 10 | `iwpAct` | short |
| 12 | `scan` | _scan |

## _planetminimal — 6 bytes (type 0x1532)

| Offset | Member | Type |
|---:|---|---|
| 0 | `id` | short |
| 2 | `iPlayer` | short |
| 4 | `det` | unsigned short; bits 0..7 |
| 4 | `fInclude` | unsigned short; bits 8..8 |
| 4 | `fStarbase` | unsigned short; bits 9..9 |
| 4 | `unusedA` | unsigned short; bits 10..10 |
| 4 | `fFirstYear` | unsigned short; bits 11..11 |
| 4 | `unusedB` | unsigned short; bits 12..15 |

## _wn — 10 bytes (type 0x1535)

| Offset | Member | Type |
|---:|---|---|
| 0 | `rc` | tagRECT |
| 8 | `fMaximized` | unsigned short; bits 0..0 |
| 8 | `fMinimized` | unsigned short; bits 1..1 |
| 8 | `fInitalized` | unsigned short; bits 2..2 |
| 8 | `fUnused` | unsigned short; bits 3..15 |

## tagBITMAPINFO — 44 bytes (type 0x1546)

| Offset | Member | Type |
|---:|---|---|
| 0 | `bmiHeader` | tagBITMAPINFOHEADER |
| 40 | `bmiColors` | array of tagRGBQUAD (4 bytes total) |

## tagRASTERIZER_STATUS — 6 bytes (type 0x154d)

| Offset | Member | Type |
|---:|---|---|
| 0 | `nSize` | int |
| 2 | `wFlags` | int |
| 4 | `nLanguageID` | int |

## _rtloghdr — 17 bytes (type 0x1550)

| Offset | Member | Type |
|---:|---|---|
| 0 | `cbLog` | short |
| 2 | `lSerialNumber` | long |
| 6 | `rgbConfig` | array of unsigned char (11 bytes total) |

## _mdplr — 2 bytes (type 0x1554)

| Offset | Member | Type |
|---:|---|---|
| 0 | `reserved` | unsigned short; bits 0..8 |
| 0 | `fAi` | unsigned short; bits 9..9 |
| 0 | `lvlAi` | unsigned short; bits 10..12 |
| 0 | `idAi` | unsigned short; bits 13..15 |

## _div_t — 4 bytes (type 0x156a)

| Offset | Member | Type |
|---:|---|---|
| 0 | `quot` | int |
| 2 | `rem` | int |

## _ldiv_t — 8 bytes (type 0x156d)

| Offset | Member | Type |
|---:|---|---|
| 0 | `quot` | long |
| 4 | `rem` | long |
