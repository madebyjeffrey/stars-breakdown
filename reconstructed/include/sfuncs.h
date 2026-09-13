/* Effective signatures; prefixed names avoid SDK collisions. Not implementations. */
#ifndef STARS_RECOVERED_FUNCTIONS_H
#define STARS_RECOVERED_FUNCTIONS_H
#include "stypes.h"
/* SatisfyOrders at 0017:6798; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_turn3_SatisfyOrders(STARS_CV_0072 iPass);
/* TbWndProc at 000e:001e; global */
STARS_CV_0012 STARS_FAR STARS_PASCAL stars_tb_TbWndProc(STARS_CV_0073 hwnd, STARS_CV_0073 msg, STARS_CV_0073 wParam, STARS_CV_0012 lParam);
/* DrawToolbar at 000e:06f0; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_tb_DrawToolbar(STARS_CV_0073 hdc, STARS_CV_101b prc);
/* DrawBitmapButton at 000e:078c; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_tb_DrawBitmapButton(STARS_CV_0073 hdc, STARS_CV_1018 pt, STARS_CV_0072 ibtn, STARS_CV_0072 fDown);
/* ItbFromPpt at 000e:0b12; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_tb_ItbFromPpt(STARS_CV_1020 ppt);
/* DxOfBtn at 000e:0bb2; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_tb_DxOfBtn(STARS_CV_0072 itb);
/* FIsButtonDown at 000e:0c3a; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_tb_FIsButtonDown(STARS_CV_0072 itb);
/* ExecuteButton at 000e:0db6; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_tb_ExecuteButton(STARS_CV_0072 itb, STARS_CV_0072 fDown);
/* TerminateToolbarFocus at 000e:1680; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_tb_TerminateToolbarFocus(STARS_CV_0072 fCancel);
/* ShowTooltip at 000e:17ea; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_tb_ShowTooltip(STARS_CV_0072 ids, STARS_CV_101b prc);
/* TooltipWndProc at 000e:19e4; global */
STARS_CV_0012 STARS_FAR STARS_PASCAL stars_tb_TooltipWndProc(STARS_CV_0073 hwnd, STARS_CV_0073 msg, STARS_CV_0073 wParam, STARS_CV_0012 lParam);
/* FakeComboProc at 000e:1d72; global */
STARS_CV_0012 STARS_FAR STARS_PASCAL stars_tb_FakeComboProc(STARS_CV_0073 hwnd, STARS_CV_0021 msg, STARS_CV_0021 wParam, STARS_CV_0012 lParam);
/* FakeCEProc at 000e:1df0; global */
STARS_CV_0012 STARS_FAR STARS_PASCAL stars_tb_FakeCEProc(STARS_CV_0073 hwnd, STARS_CV_0021 msg, STARS_CV_0021 wParam, STARS_CV_0012 lParam);
/* DoCyberAiTurn at 0016:002a; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_ai4_DoCyberAiTurn(STARS_CV_103a rgprod);
/* DoCyberPackets at 0016:1a78; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_ai4_DoCyberPackets(void);
/* IdGetBestScannerDest at 0016:282a; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_ai4_IdGetBestScannerDest(STARS_CV_100b lppl, STARS_CV_0072 iDir);
/* FAddPacketToQueue at 0016:2bc0; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_ai4_FAddPacketToQueue(STARS_CV_100b lppl);
/* FillProductionQueue at 0016:2ce2; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_ai4_FillProductionQueue(void);
/* FFillProdMinesAndFactories at 0016:2d72; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_ai4_FFillProdMinesAndFactories(STARS_CV_100b lppl);
/* DoCyberFreighter at 0016:37b0; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_ai4_DoCyberFreighter(STARS_CV_1004 lpfl, STARS_CV_1041 lpciPlanTemp);
/* FEnumDropOffStage1 at 0016:3d00; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_ai4_FEnumDropOffStage1(STARS_CV_100b lpplSrc, STARS_CV_100b lpplTest);
/* FEnumDropOffStage2 at 0016:3dfe; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_ai4_FEnumDropOffStage2(STARS_CV_100b lpplSrc, STARS_CV_100b lpplTest);
/* FEnumPickUp at 0016:3f00; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_ai4_FEnumPickUp(STARS_CV_100b lpplSrc, STARS_CV_100b lpplTest);
/* FEnumNeedMinerals at 0016:3f5e; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_ai4_FEnumNeedMinerals(STARS_CV_100b lpplSrc, STARS_CV_100b lpplTest);
/* FEnumPktAttack at 0016:4204; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_ai4_FEnumPktAttack(STARS_CV_100b lpplSrc, STARS_CV_100b lpplTest);
/* FEnumCalcEnemyPlanets at 0016:45a6; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_ai4_FEnumCalcEnemyPlanets(STARS_CV_100b lpplSrc, STARS_CV_100b lpplTest);
/* iBuildCyberStarbase at 0016:45de; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_ai4_iBuildCyberStarbase(STARS_CV_100b lppl);
/* EnsureCyberAiShdefs at 0016:4826; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_ai4_EnsureCyberAiShdefs(STARS_CV_0072 iroCur);
/* iAddAttackFleet at 0016:4eba; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_ai4_iAddAttackFleet(STARS_CV_100b lppl, STARS_CV_0072 iAttackStr, STARS_CV_0072 iBestDestroyer, STARS_CV_0072 iBestBattle, STARS_CV_0072 iBestSBDefender);
/* TargetCyberArmada at 0016:51a4; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_ai4_TargetCyberArmada(STARS_CV_1004 lpfl);
/* DoMacintiAiTurn at 0015:0008; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_ai3_DoMacintiAiTurn(STARS_CV_103a rgprod);
/* EnsureMacintiShdefs at 0015:2b3c; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_ai3_EnsureMacintiShdefs(void);
/* FRetargetMiner at 0015:336c; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_ai3_FRetargetMiner(STARS_CV_1004 lpfl);
/* IdTargetMacFreighter at 0015:3524; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_ai3_IdTargetMacFreighter(STARS_CV_1004 lpfl);
/* TargetMacArmada at 0015:3e3a; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_ai3_TargetMacArmada(STARS_CV_1004 lpfl);
/* FPotentMacWarFleet at 0015:42ec; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_ai3_FPotentMacWarFleet(STARS_CV_1004 lpfl, STARS_CV_0172 pcEquiv);
/* DoMaidAiTurn at 0014:0000; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_ai2_DoMaidAiTurn(STARS_CV_103a rgprod);
/* FPotentISWarFleet at 0014:012e; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_ai2_FPotentISWarFleet(STARS_CV_1004 lpfl, STARS_CV_0072 iPotency);
/* DoAutomitronAiTurn at 0014:01e0; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_ai2_DoAutomitronAiTurn(STARS_CV_103a rgprod);
/* EnsureISShdefs at 0014:1938; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_ai2_EnsureISShdefs(STARS_CV_0072 iroCur);
/* DoRototillAiTurn at 0014:1e22; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_ai2_DoRototillAiTurn(STARS_CV_103a rgprod);
/* EnsureCAShdefs at 0014:3020; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_ai2_EnsureCAShdefs(STARS_CV_0072 iroCur);
/* ZipOrderDlg at 0011:0000; global */
STARS_CV_0072 STARS_FAR STARS_PASCAL stars_ship2_ZipOrderDlg(STARS_CV_0073 hwnd, STARS_CV_0073 message, STARS_CV_0021 wParam, STARS_CV_0012 lParam);
/* EnableZipBtns at 0011:0832; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_ship2_EnableZipBtns(STARS_CV_0073 hwnd, STARS_CV_0072 iSel);
/* RenameZipDlg at 0011:0880; global */
STARS_CV_0072 STARS_FAR STARS_PASCAL stars_ship2_RenameZipDlg(STARS_CV_0073 hwnd, STARS_CV_0073 message, STARS_CV_0021 wParam, STARS_CV_0012 lParam);
/* RenameDlg at 0011:0a68; global */
STARS_CV_0072 STARS_FAR STARS_PASCAL stars_ship2_RenameDlg(STARS_CV_0073 hwnd, STARS_CV_0073 message, STARS_CV_0021 wParam, STARS_CV_0012 lParam);
/* FStargateJump at 0011:0cfe; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_ship2_FStargateJump(STARS_CV_1004 lpfl, STARS_CV_0072 isbsSrc, STARS_CV_0072 isbsDst, STARS_CV_0072 dDist);
/* MdCalcStargateDamage at 0011:152e; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_ship2_MdCalcStargateDamage(STARS_CV_0072 isbsSrc, STARS_CV_0072 isbsDst, STARS_CV_0072 dDist, STARS_CV_0072 wt, STARS_CV_0172 ppctDmg);
/* KillUsedWaypoints at 0011:189a; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_ship2_KillUsedWaypoints(void);
/* NoAutoTrackFleet at 0011:1d32; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_ship2_NoAutoTrackFleet(STARS_CV_1004 lpflTarget);
/* AutoRouteFleet at 0011:1e52; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_ship2_AutoRouteFleet(STARS_CV_1004 lpfl, STARS_CV_100b lppl);
/* FColonizer at 0011:227c; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_ship2_FColonizer(STARS_CV_1004 lpfl);
/* FScout at 0011:2322; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_ship2_FScout(STARS_CV_1004 lpfl);
/* AutoFleetOrder at 0011:23c8; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_ship2_AutoFleetOrder(STARS_CV_1004 lpfl, STARS_CV_100b lppl);
/* CMineFromLpfl at 0011:25d2; global */
STARS_CV_0012 STARS_FAR STARS_CDECL stars_ship2_CMineFromLpfl(STARS_CV_1004 lpfl);
/* PctTerraFromLpfl at 0011:275c; global */
STARS_CV_0012 STARS_FAR STARS_CDECL stars_ship2_PctTerraFromLpfl(STARS_CV_1004 lpfl);
/* CLayMinesFromLpfl at 0011:2886; global */
STARS_CV_0012 STARS_FAR STARS_CDECL stars_ship2_CLayMinesFromLpfl(STARS_CV_1004 lpfl, STARS_CV_0072 iType, STARS_CV_0072 ishdef);
/* CMineSweepFromLpfl at 0011:2b26; global */
STARS_CV_0012 STARS_FAR STARS_CDECL stars_ship2_CMineSweepFromLpfl(STARS_CV_1004 lpfl);
/* CMineSweepFromLphul at 0011:2bfa; global */
STARS_CV_0012 STARS_FAR STARS_CDECL stars_ship2_CMineSweepFromLphul(STARS_CV_1077 lphul);
/* PctCloakFromLpfl at 0011:2d5e; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_ship2_PctCloakFromLpfl(STARS_CV_1004 lpfl);
/* CPtsCloakFromLphs at 0011:3170; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_ship2_CPtsCloakFromLphs(STARS_CV_1079 lphs);
/* MergeFleetsDlg at 0011:3376; global */
STARS_CV_0072 STARS_FAR STARS_PASCAL stars_ship2_MergeFleetsDlg(STARS_CV_0073 hwnd, STARS_CV_0073 msg, STARS_CV_0021 wParam, STARS_CV_0012 lParam);
/* MarkTechsSeen at 0011:36b6; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_ship2_MarkTechsSeen(STARS_CV_1077 lphul, STARS_CV_0072 iplr);
/* LpthNew at 0023:0000; global */
STARS_CV_1007 STARS_FAR STARS_CDECL stars_thing_LpthNew(STARS_CV_0072 iplr, STARS_CV_0072 ith);
/* FreeLpth at 0023:0224; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_thing_FreeLpth(STARS_CV_1007 lpth);
/* CPlanetsInCircle at 0023:02a0; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_thing_CPlanetsInCircle(STARS_CV_1018 pt, STARS_CV_0012 r2);
/* DrawThingGauge at 0023:044e; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_thing_DrawThingGauge(STARS_CV_0073 hdc, STARS_CV_101b prc, STARS_CV_1007 lpth, STARS_CV_0072 md);
/* IValidateWormholePos at 0023:064c; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_thing_IValidateWormholePos(STARS_CV_1007 lpthWorm);
/* PctWormholeMoves at 0023:0adc; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_thing_PctWormholeMoves(STARS_CV_1007 lpth);
/* DoThingInteractions at 0023:0b3a; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_thing_DoThingInteractions(STARS_CV_0072 fPostMove);
/* IdmGiveTraderPart at 0023:1a96; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_thing_IdmGiveTraderPart(STARS_CV_0021 grbitTrader, STARS_CV_0072 iplr, STARS_CV_0121 piGoto);
/* ReportDlg at 0022:0018; global */
STARS_CV_0012 STARS_FAR STARS_PASCAL stars_report_ReportDlg(STARS_CV_0073 hwnd, STARS_CV_0073 msg, STARS_CV_0021 wParam, STARS_CV_0012 lParam);
/* SetHScrollBar at 0022:09e8; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_report_SetHScrollBar(void);
/* DrawReport at 0022:0bae; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_report_DrawReport(STARS_CV_0073 hwnd, STARS_CV_0073 hdc, STARS_CV_101b prc);
/* ScoreXDlg at 0022:0f66; global */
STARS_CV_0072 STARS_FAR STARS_PASCAL stars_report_ScoreXDlg(STARS_CV_0073 hwnd, STARS_CV_0073 message, STARS_CV_0021 wParam, STARS_CV_0012 lParam);
/* InitScoreDlg at 0022:13b6; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_report_InitScoreDlg(STARS_CV_0073 hwnd, STARS_CV_0072 fVictory);
/* DrawVCReport at 0022:168e; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_report_DrawVCReport(STARS_CV_0073 hdc);
/* DrawScoreReport at 0022:1e0c; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_report_DrawScoreReport(STARS_CV_0073 hdc);
/* DrawHistoryReport at 0022:2494; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_report_DrawHistoryReport(STARS_CV_0073 hdc);
/* LFetchScoreXVal at 0022:2f94; global */
STARS_CV_0012 STARS_FAR STARS_CDECL stars_report_LFetchScoreXVal(STARS_CV_109b lpsx, STARS_CV_0072 iVal);
/* DxReportColHdr at 0022:305e; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_report_DxReportColHdr(STARS_CV_0072 irpt, STARS_CV_0072 iCol, STARS_CV_0170 psz, STARS_CV_0073 hdc);
/* DrawReportItem at 0022:3398; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_report_DrawReportItem(STARS_CV_0073 hdc, STARS_CV_101b prc, STARS_CV_0072 irpt, STARS_CV_0072 irow, STARS_CV_0072 icol);
/* DrawMineralItem at 0022:4ebe; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_report_DrawMineralItem(STARS_CV_0073 hdc, STARS_CV_0072 x, STARS_CV_0072 y, STARS_CV_0072 iMineral, STARS_CV_0012 l);
/* PszGetDestName at 0022:4f60; global */
STARS_CV_0170 STARS_FAR STARS_CDECL stars_report_PszGetDestName(STARS_CV_1004 lpfl, STARS_CV_0073 hdc);
/* FDestIsWP0 at 0022:50b4; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_report_FDestIsWP0(STARS_CV_1004 lpfl);
/* PszGetETA at 0022:51a8; global */
STARS_CV_0170 STARS_FAR STARS_CDECL stars_report_PszGetETA(STARS_CV_0073 hdc, STARS_CV_1004 lpfl, STARS_CV_0172 pcYears);
/* PszGetTaskName at 0022:53b8; global */
STARS_CV_0170 STARS_FAR STARS_CDECL stars_report_PszGetTaskName(STARS_CV_1004 lpfl, STARS_CV_0172 picr);
/* SortReportCache at 0022:589c; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_report_SortReportCache(STARS_CV_0072 irpt, STARS_CV_0072 icol);
/* ICompReport at 0022:5bb8; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_report_ICompReport(STARS_CV_0103 arg1, STARS_CV_0103 arg2);
/* ReportColumnPopup at 0022:74d4; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_report_ReportColumnPopup(STARS_CV_1018 pt, STARS_CV_0072 icol, STARS_CV_0072 fRightBtn);
/* InvalidateReport at 0022:7af6; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_report_InvalidateReport(STARS_CV_0072 irpt, STARS_CV_0072 fReload);
/* ExecuteReportClick at 0022:7cd6; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_report_ExecuteReportClick(STARS_CV_1018 pt, STARS_CV_0072 irpt, STARS_CV_0072 icol, STARS_CV_0072 irow);
/* DumpUniverse at 0022:851e; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_report_DumpUniverse(void);
/* DumpPlanets at 0022:86e4; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_report_DumpPlanets(void);
/* DumpFleets at 0022:9530; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_report_DumpFleets(void);
/* PrintMapDlg at 0022:a1c2; global */
STARS_CV_0072 STARS_FAR STARS_PASCAL stars_report_PrintMapDlg(STARS_CV_0073 hwnd, STARS_CV_0073 msg, STARS_CV_0021 wParam, STARS_CV_0012 lParam);
/* CchTutorString at 0021:5a14; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_tutor2_CchTutorString(STARS_CV_0170 pchOut, STARS_CV_0072 idt);
/* TutorDlg at 0020:0000; global */
STARS_CV_0072 STARS_FAR STARS_PASCAL stars_tutor_TutorDlg(STARS_CV_0073 hwnd, STARS_CV_0073 message, STARS_CV_0021 wParam, STARS_CV_0012 lParam);
/* PanicDlg at 0020:026a; global */
STARS_CV_0072 STARS_FAR STARS_PASCAL stars_tutor_PanicDlg(STARS_CV_0073 hwnd, STARS_CV_0073 message, STARS_CV_0021 wParam, STARS_CV_0012 lParam);
/* ShowTutor at 0020:0374; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_tutor_ShowTutor(STARS_CV_0072 fShow);
/* DrawTutorText at 0020:03c0; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_tutor_DrawTutorText(STARS_CV_0073 hwnd);
/* StartTutor at 0020:06b4; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_tutor_StartTutor(STARS_CV_0072 fRestart);
/* AdvanceTutor at 0020:0a30; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_tutor_AdvanceTutor(void);
/* EndTutor at 0020:0c02; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_tutor_EndTutor(STARS_CV_0072 fClose);
/* SaveGameState at 0020:0c90; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_tutor_SaveGameState(void);
/* RestoreGameState at 0020:0e1c; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_tutor_RestoreGameState(void);
/* FAskKillTutor at 0020:0f36; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_tutor_FAskKillTutor(void);
/* FTutorTaskDone at 0020:0fbc; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_tutor_FTutorTaskDone(void);
/* FCheckZip at 0020:6460; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_tutor_FCheckZip(STARS_CV_0072 iZip, STARS_CV_10cc lpiaGoal, STARS_CV_0072 ids);
/* FCheckTemplate at 0020:666e; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_tutor_FCheckTemplate(STARS_CV_0072 iTemplate);
/* TutorError at 0020:67ae; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_tutor_TutorError(STARS_CV_0072 idsError);
/* FCheckScanner at 0020:685c; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_tutor_FCheckScanner(STARS_CV_0072 md, STARS_CV_0072 iZoom);
/* FCheckFleetName at 0020:690a; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_tutor_FCheckFleetName(STARS_CV_0072 id, STARS_CV_0072 ids);
/* FCheckSummary at 0020:69e2; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_tutor_FCheckSummary(STARS_CV_0073 grobj, STARS_CV_0072 id);
/* FCheckSelection at 0020:6af4; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_tutor_FCheckSelection(STARS_CV_0073 grobj, STARS_CV_0072 id);
/* FCheckMessages at 0020:6c48; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_tutor_FCheckMessages(STARS_CV_0072 imsg, STARS_CV_0072 idm, STARS_CV_0072 fFilter);
/* FCheckResearch at 0020:6da4; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_tutor_FCheckResearch(STARS_CV_0072 iTech, STARS_CV_0072 iTechNext, STARS_CV_0072 pct);
/* FCheckFleetWP at 0020:6df4; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_tutor_FCheckFleetWP(STARS_CV_0073 ifl, STARS_CV_0072 iord, STARS_CV_0073 grobj, STARS_CV_0072 id, STARS_CV_0073 grTask, STARS_CV_0073 iWarp);
/* FCheckPlanetRoute at 0020:6f86; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_tutor_FCheckPlanetRoute(STARS_CV_0072 idpl, STARS_CV_0072 idplRoute);
/* FCheckLayingWP at 0020:6ff4; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_tutor_FCheckLayingWP(STARS_CV_0073 ifl, STARS_CV_0072 iord, STARS_CV_0072 id, STARS_CV_0072 iYears);
/* FCheckColonizeWP at 0020:70c0; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_tutor_FCheckColonizeWP(STARS_CV_0073 ifl, STARS_CV_0072 id, STARS_CV_0073 iWarp);
/* FCheckPatrolWP at 0020:71ac; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_tutor_FCheckPatrolWP(STARS_CV_0073 ifl, STARS_CV_0072 iord, STARS_CV_0072 id, STARS_CV_0073 iWarp, STARS_CV_0073 iPlan, STARS_CV_0073 iDist);
/* FCheckXferWP at 0020:7280; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_tutor_FCheckXferWP(STARS_CV_0073 ifl, STARS_CV_0072 iord, STARS_CV_0072 id, STARS_CV_0073 iWarp, STARS_CV_10cc lpiaGoal);
/* FCheckQueue at 0020:7442; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_tutor_FCheckQueue(STARS_CV_0072 ipl, STARS_CV_0072 iprod, STARS_CV_0073 grobj, STARS_CV_0073 iItem, STARS_CV_0073 cItem, STARS_CV_0021 fNoResearch);
/* FCheckBtlPlan at 0020:760a; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_tutor_FCheckBtlPlan(STARS_CV_0072 ibp, STARS_CV_0021 imdTarget, STARS_CV_0021 fSpread, STARS_CV_0021 fBomb, STARS_CV_0021 fDump, STARS_CV_0021 mdUnarmed, STARS_CV_0021 mdScout, STARS_CV_0021 mdWar, STARS_CV_0021 mdBomber);
/* FCheckCargo at 0020:7664; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_tutor_FCheckCargo(STARS_CV_1004 lpfl, STARS_CV_0072 wtMin1, STARS_CV_0072 wtMin2, STARS_CV_0072 wtMin3, STARS_CV_0072 wtColonists);
/* FCheckBuilderPart at 0020:77d8; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_tutor_FCheckBuilderPart(STARS_CV_0072 iSlot, STARS_CV_10e5 phs, STARS_CV_0073 cInit);
/* FCheckShipBuilder at 0020:7964; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_tutor_FCheckShipBuilder(STARS_CV_0072 iCategory, STARS_CV_0072 iShip);
/* FTutorialEnabledShipBuilder at 0020:79f6; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_tutor_FTutorialEnabledShipBuilder(STARS_CV_0072 itutsbAction);
/* FOKMergeDialog at 0020:81da; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_tutor_FOKMergeDialog(void);
/* PszGetCompressedString at 0003:743a; global */
STARS_CV_0170 STARS_FAR STARS_CDECL stars_strings_PszGetCompressedString(STARS_CV_0072 ids);
/* FCreateAiShdef at 0013:012c; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_aiutil_FCreateAiShdef(STARS_CV_0072 ishdef, STARS_CV_0072 ihul, STARS_CV_0220 rgaip);
/* FGetAIPart at 0013:043e; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_aiutil_FGetAIPart(STARS_CV_0072 aip, STARS_CV_10fa ppart);
/* PickANameAndBmp at 0013:055a; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_aiutil_PickANameAndBmp(STARS_CV_10fd pshdef, STARS_CV_0072 ids, STARS_CV_0072 cids, STARS_CV_0072 ibmpStart);
/* FChangeAiShdef at 0013:08a2; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_aiutil_FChangeAiShdef(STARS_CV_10fd pshdef, STARS_CV_0072 ishdef);
/* XferAiSupply at 0013:0ad0; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_aiutil_XferAiSupply(STARS_CV_0072 grobjSrc, STARS_CV_0072 idSrc, STARS_CV_0072 grobjDst, STARS_CV_0072 idDst, STARS_CV_0072 iSupply, STARS_CV_0072 cQuan);
/* XferAiTroopers at 0013:0bc2; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_aiutil_XferAiTroopers(STARS_CV_0072 idSrc, STARS_CV_0072 idDst, STARS_CV_0072 cQuan);
/* FColonizeAiFleet at 0013:0c78; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_aiutil_FColonizeAiFleet(STARS_CV_1004 lpfl, STARS_CV_0072 idPlanet);
/* FGotoWormholeAiFleet at 0013:0d5c; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_aiutil_FGotoWormholeAiFleet(STARS_CV_1004 lpfl, STARS_CV_1007 lpthWorm);
/* IdNearestColonizablePlanet at 0013:0e3e; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_aiutil_IdNearestColonizablePlanet(STARS_CV_1004 lpflCol, STARS_CV_1107 plpthWorm);
/* LpthWormFind at 0013:12e6; global */
STARS_CV_1007 STARS_FAR STARS_CDECL stars_aiutil_LpthWormFind(STARS_CV_1020 ppt, STARS_CV_0012 d2);
/* UlFleetPower at 0013:14de; global */
STARS_CV_0022 STARS_FAR STARS_CDECL stars_aiutil_UlFleetPower(STARS_CV_1004 lpfl);
/* IdNearestUnknownPlanet at 0013:15a4; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_aiutil_IdNearestUnknownPlanet(STARS_CV_1004 lpfl, STARS_CV_1107 plpthWorm);
/* AddMinesToBlockedQueues at 0013:1792; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_aiutil_AddMinesToBlockedQueues(void);
/* FFleetInField at 0013:1cf6; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_aiutil_FFleetInField(STARS_CV_1004 lpfl, STARS_CV_1007 lpth);
/* SetAiFleetIdealSpeed at 0013:1d9e; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_aiutil_SetAiFleetIdealSpeed(STARS_CV_1004 lpfl, STARS_CV_0072 wtFuelMax, STARS_CV_0072 cMinefields, STARS_CV_1107 rglpth);
/* IdTargetAttack at 0013:1ffe; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_aiutil_IdTargetAttack(STARS_CV_1004 lpfl, STARS_CV_1004 lpflAtk, STARS_CV_1004 lpflEnemy, STARS_CV_0072 fOnlyHumans);
/* IdTargetFreighter at 0013:286c; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_aiutil_IdTargetFreighter(STARS_CV_1004 lpflFr, STARS_CV_100b lpplHome);
/* FSalvageTargetFreighter2 at 0013:395a; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_aiutil_FSalvageTargetFreighter2(STARS_CV_1004 lpflFr, STARS_CV_0072 fNeedy, STARS_CV_0072 iWorst, STARS_CV_0072 pctFull, STARS_CV_0012 wtCargoMax, STARS_CV_0012 scoreBest, STARS_CV_1107 plpthBest, STARS_CV_0172 pidBest);
/* FMoveAiFleet at 0013:3d0a; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_aiutil_FMoveAiFleet(STARS_CV_1004 lpfl, STARS_CV_1114 pord, STARS_CV_0072 fAppend);
/* AddItemToQueue at 0013:3e50; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_aiutil_AddItemToQueue(STARS_CV_0073 iItem, STARS_CV_0073 cItem, STARS_CV_0073 grobj, STARS_CV_0072 mdAddItem);
/* IroEnsureAi at 0013:425a; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_aiutil_IroEnsureAi(STARS_CV_0220 lpbRes, STARS_CV_0072 cRes, STARS_CV_0172 pishdefSBLatest, STARS_CV_0072 pct);
/* KeepFleetsMoving at 0013:45ca; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_aiutil_KeepFleetsMoving(void);
/* FShouldWeBuildColonizers at 0013:476a; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_aiutil_FShouldWeBuildColonizers(STARS_CV_0172 pcCol);
/* FIsAiAttack at 0013:4a72; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_aiutil_FIsAiAttack(STARS_CV_1004 lpfl);
/* FIsTurinDroneAiAttack at 0013:4b9a; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_aiutil_FIsTurinDroneAiAttack(STARS_CV_1004 lpfl);
/* FIsAiTransport at 0013:4c08; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_aiutil_FIsAiTransport(STARS_CV_1004 lpfl);
/* ValidateStarbaseHistory at 0013:4cf0; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_aiutil_ValidateStarbaseHistory(void);
/* GetResourcesAvailable at 0013:56d0; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_aiutil_GetResourcesAvailable(STARS_CV_100b lppl, STARS_CV_0112 rgRes);
/* GetProdQCost at 0013:57c0; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_aiutil_GetProdQCost(STARS_CV_100b lppl, STARS_CV_0112 rgCost);
/* MergeAllShdefs at 0013:58be; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_aiutil_MergeAllShdefs(STARS_CV_0072 grbitish);
/* LpflFindClosestEnum at 0013:5bae; global */
STARS_CV_1004 STARS_FAR STARS_CDECL stars_aiutil_LpflFindClosestEnum(STARS_CV_1004 lpfl, STARS_CV_1123 pfn);
/* LpplFindClosestEnum at 0013:5cd4; global */
STARS_CV_100b STARS_FAR STARS_CDECL stars_aiutil_LpplFindClosestEnum(STARS_CV_100b lppl, STARS_CV_1126 pfn);
/* LpplFindBestEnum at 0013:5e06; global */
STARS_CV_100b STARS_FAR STARS_CDECL stars_aiutil_LpplFindBestEnum(STARS_CV_100b lppl, STARS_CV_1126 pfn);
/* IdRandomPlanetNearby at 0013:5f54; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_aiutil_IdRandomPlanetNearby(STARS_CV_1018 pt, STARS_CV_0072 cDist, STARS_CV_0072 fAvoidStarbases);
/* ClearAiCurrentTask at 0013:60c0; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_aiutil_ClearAiCurrentTask(STARS_CV_1004 lpfl, STARS_CV_0072 fChangeSel);
/* FEnumOurStarbase at 0013:6110; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_aiutil_FEnumOurStarbase(STARS_CV_100b lpplSrc, STARS_CV_100b lpplTest);
/* FFleetMightHaveTeeth at 0013:6152; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_aiutil_FFleetMightHaveTeeth(STARS_CV_1004 lpfl);
/* IdTargetScout at 0013:61de; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_aiutil_IdTargetScout(STARS_CV_1004 lpfl, STARS_CV_1004 lpflAtk, STARS_CV_1004 lpflEnemy, STARS_CV_0072 fOnlyHumans, STARS_CV_1107 plpthWorm);
/* MarkPlanetsUnderAttack at 0013:6896; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_aiutil_MarkPlanetsUnderAttack(void);
/* FixPlanetsUnderAttack at 0013:69e6; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_aiutil_FixPlanetsUnderAttack(STARS_CV_103a rgprod);
/* QuickBuildDefenses at 0013:6a7e; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_aiutil_QuickBuildDefenses(STARS_CV_100b lppl, STARS_CV_103a rgprod);
/* IdplFindClosestStarbase at 0013:6e04; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_aiutil_IdplFindClosestStarbase(STARS_CV_1004 lpfl, STARS_CV_0072 fBigOnes);
/* FMoveToNearestStarbase at 0013:6f7e; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_aiutil_FMoveToNearestStarbase(STARS_CV_1004 lpfl, STARS_CV_0072 fBigOnes);
/* MoveToNearestPlanetOrEnemy at 0013:7014; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_aiutil_MoveToNearestPlanetOrEnemy(STARS_CV_1004 lpfl, STARS_CV_0072 dEnemyRange);
/* EnsureAiStarbaseDesigns at 0013:7222; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_aiutil_EnsureAiStarbaseDesigns(void);
/* EnsureMacintiStarbaseDesigns at 0013:76e4; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_aiutil_EnsureMacintiStarbaseDesigns(STARS_CV_0120 rgSB);
/* FCreateAiStarbase at 0013:7bca; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_aiutil_FCreateAiStarbase(STARS_CV_0072 ishdef, STARS_CV_0072 iLevel, STARS_CV_0072 aisb, STARS_CV_0072 isb);
/* FAIFling at 0013:7dd6; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_aiutil_FAIFling(STARS_CV_100b lppl, STARS_CV_0112 rgResAvail);
/* IshdefAiSBLatestOF at 0013:8458; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_aiutil_IshdefAiSBLatestOF(void);
/* IshdefAiSBLatest at 0013:84be; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_aiutil_IshdefAiSBLatest(void);
/* QueueAiStarbases at 0013:8524; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_aiutil_QueueAiStarbases(STARS_CV_103a rgprod, STARS_CV_0072 ishdefSBLatest);
/* FUpgradeAiStarbase at 0013:882a; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_aiutil_FUpgradeAiStarbase(STARS_CV_100b lppl, STARS_CV_0072 ishdefSBLatest);
/* FQueueAiTerraforming at 0013:8d28; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_aiutil_FQueueAiTerraforming(STARS_CV_100b lppl, STARS_CV_0112 rgResAvail, STARS_CV_0112 rgResCost);
/* FQueueAiScanner at 0013:90d6; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_aiutil_FQueueAiScanner(STARS_CV_100b lppl, STARS_CV_0112 rgResAvail, STARS_CV_0112 rgResCost);
/* FQueueAiDefenses at 0013:939a; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_aiutil_FQueueAiDefenses(STARS_CV_100b lppl, STARS_CV_0112 rgResAvail, STARS_CV_0112 rgResCost);
/* HandleBasicAiTasks at 0013:95a4; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_aiutil_HandleBasicAiTasks(STARS_CV_0072 iroCur, STARS_CV_103a rgprod, STARS_CV_0072 ishdefSBLatest, STARS_CV_0112 rgResAvail, STARS_CV_0112 rgResCost);
/* SplitOutShdefs at 0013:98d8; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_aiutil_SplitOutShdefs(STARS_CV_0120 rgbIsh);
/* CheckAiShdefStatus at 0013:9b10; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_aiutil_CheckAiShdefStatus(STARS_CV_0072 ishBeg, STARS_CV_0072 ishEnd, STARS_CV_0021 cRecyclePeriod, STARS_CV_0172 piLatest, STARS_CV_0120 rgbOld);
/* IncreaseAIMinefieldSizes at 0013:9c66; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_aiutil_IncreaseAIMinefieldSizes(void);
/* FFindBuddyAndJoinUp at 0013:9d18; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_aiutil_FFindBuddyAndJoinUp(STARS_CV_1004 lpfl, STARS_CV_0072 ishLo, STARS_CV_0072 ishHi, STARS_CV_0012 lMaxDist1, STARS_CV_0012 lMaxDist2);
/* FShouldPlanetBuildColonizer at 0013:9f30; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_aiutil_FShouldPlanetBuildColonizer(STARS_CV_100b lpplSrc);
/* InitRandomPlanetList at 0013:a082; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_aiutil_InitRandomPlanetList(void);
/* DoAiTurn at 0012:0000; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_ai_DoAiTurn(STARS_CV_0072 iPlayer, STARS_CV_0021 wMdPlr);
/* DoRobotoidAiTurn at 0012:0312; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_ai_DoRobotoidAiTurn(STARS_CV_103a rgprod);
/* EnsureRobotoidShdefs at 0012:20ae; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_ai_EnsureRobotoidShdefs(void);
/* IdTargetArmada at 0012:288e; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_ai_IdTargetArmada(STARS_CV_1004 lpfl);
/* FPotentRobWarFleet at 0012:31bc; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_ai_FPotentRobWarFleet(STARS_CV_1004 lpfl, STARS_CV_0072 iPotency);
/* FEnumCalcEnemyFleets at 0012:325c; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_ai_FEnumCalcEnemyFleets(STARS_CV_1004 lpflSrc, STARS_CV_1004 lpflTest);
/* FEnumCalcArmadaDest at 0012:3286; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_ai_FEnumCalcArmadaDest(STARS_CV_100b lpplSrc, STARS_CV_100b lpplTest);
/* FEnumCalcArmadaHumanDest at 0012:3406; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_ai_FEnumCalcArmadaHumanDest(STARS_CV_100b lpplSrc, STARS_CV_100b lpplTest);
/* DoTurinDroneAiTurn at 0012:3670; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_ai_DoTurinDroneAiTurn(STARS_CV_103a rgprod);
/* EnsureTurinDroneShdefs at 0012:58ba; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_ai_EnsureTurinDroneShdefs(STARS_CV_0072 iroCur);
/* FEnumCalcMinerDest at 0012:5f32; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_ai_FEnumCalcMinerDest(STARS_CV_100b lpplSrc, STARS_CV_100b lpplTest);
/* FEnumCalcColonistDrop at 0012:5fc8; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_ai_FEnumCalcColonistDrop(STARS_CV_100b lpplSrc, STARS_CV_100b lpplTest);
/* BattleVCR at 001e:0000; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_vcr_BattleVCR(STARS_CV_0072 iBattle);
/* CBattles at 001e:028c; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_vcr_CBattles(void);
/* BtlDataGet at 001e:0362; global */
STARS_CV_10a3 STARS_FAR STARS_CDECL stars_vcr_BtlDataGet(STARS_CV_0072 i);
/* CBattleUnits at 001e:0450; global */
STARS_CV_0012 STARS_FAR STARS_CDECL stars_vcr_CBattleUnits(STARS_CV_10a3 lpbd, STARS_CV_0021 grbitBU);
/* CBattleKills at 001e:062e; global */
STARS_CV_0012 STARS_FAR STARS_CDECL stars_vcr_CBattleKills(STARS_CV_10a3 lpbd, STARS_CV_0072 fOurDead);
/* LdpFromItokDv at 001e:07a8; global */
STARS_CV_0012 STARS_FAR STARS_CDECL stars_vcr_LdpFromItokDv(STARS_CV_0072 itok, STARS_CV_1159 lpdv);
/* SetVCRBoard at 001e:08d8; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_vcr_SetVCRBoard(STARS_CV_0072 iStep);
/* VCRDlg at 001e:0e90; global */
STARS_CV_0072 STARS_FAR STARS_PASCAL stars_vcr_VCRDlg(STARS_CV_0073 hwnd, STARS_CV_0073 message, STARS_CV_0021 wParam, STARS_CV_0012 lParam);
/* GetVCRStats at 001e:193e; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_vcr_GetVCRStats(STARS_CV_0072 itok, STARS_CV_0112 pdpArmor, STARS_CV_115d pdv, STARS_CV_0112 pdpShields, STARS_CV_0172 pcsh);
/* DrawVCR at 001e:1c62; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_vcr_DrawVCR(STARS_CV_0073 hdc, STARS_CV_0072 iStart, STARS_CV_0072 iEnd);
/* Delay at 001e:3a3e; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_vcr_Delay(STARS_CV_0072 ctick);
/* AnimateAttack at 001e:3ac2; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_vcr_AnimateAttack(STARS_CV_0073 hdc);
/* PopupVCRMenu at 001e:4518; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_vcr_PopupVCRMenu(STARS_CV_0073 hwnd, STARS_CV_0072 x, STARS_CV_0072 y, STARS_CV_0020 brc);
/* EnableVCRButtons at 001e:48f6; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_vcr_EnableVCRButtons(void);
/* PushRandom at 0009:1440; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_utilgen_PushRandom(STARS_CV_0012 lNew1, STARS_CV_0012 lNew2);
/* PopRandom at 0009:14a2; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_utilgen_PopRandom(void);
/* Randomize at 0009:15ea; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_utilgen_Randomize(STARS_CV_0022 dw);
/* Randomize2 at 0009:165a; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_utilgen_Randomize2(STARS_CV_0022 dw);
/* Random at 0009:16d2; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_utilgen_Random(STARS_CV_0072 c);
/* RandomSeedDlg at 0009:188a; global */
STARS_CV_0072 STARS_FAR STARS_PASCAL stars_utilgen_RandomSeedDlg(STARS_CV_0073 hwnd, STARS_CV_0073 message, STARS_CV_0021 wParam, STARS_CV_0012 lParam);
/* GetFileSeeds at 0009:1a4e; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_utilgen_GetFileSeeds(STARS_CV_0112 pl1, STARS_CV_0112 pl2);
/* SetFileSeeds at 0009:1a7c; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_utilgen_SetFileSeeds(STARS_CV_0012 l1, STARS_CV_0012 l2);
/* SetFileXorStream at 0009:1aa6; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_utilgen_SetFileXorStream(STARS_CV_0012 lid, STARS_CV_0011 lSalt, STARS_CV_0072 turn, STARS_CV_0072 iPlayer, STARS_CV_0072 fCrippled);
/* LGetNextFileXor at 0009:1b54; global */
STARS_CV_0012 STARS_FAR STARS_CDECL stars_utilgen_LGetNextFileXor(void);
/* XorFileBuf at 0009:1cc4; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_utilgen_XorFileBuf(STARS_CV_0170 rgb, STARS_CV_0072 cb);
/* ICompLong at 0009:1d74; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_utilgen_ICompLong(STARS_CV_0103 arg1, STARS_CV_0103 arg2);
/* PszGetCompressedPlanet at 0009:1d96; global */
STARS_CV_0170 STARS_FAR STARS_CDECL stars_utilgen_PszGetCompressedPlanet(STARS_CV_0072 id);
/* OutputFileString at 0009:1f64; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_utilgen_OutputFileString(STARS_CV_0170 szFile, STARS_CV_0170 sz);
/* CopyFile at 0009:1ffa; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_utilgen_CopyFile(STARS_CV_0170 szSrc, STARS_CV_0170 szDst);
/* AlertSz at 0009:2160; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_utilgen_AlertSz(STARS_CV_0170 sz, STARS_CV_0072 mbType);
/* CchGetString at 0009:221e; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_utilgen_CchGetString(STARS_CV_0072 ids, STARS_CV_0170 psz);
/* PszFromInt at 0009:2274; global */
STARS_CV_0170 STARS_FAR STARS_CDECL stars_utilgen_PszFromInt(STARS_CV_0072 i, STARS_CV_0172 pcch);
/* PszFromLong at 0009:22b6; global */
STARS_CV_0170 STARS_FAR STARS_CDECL stars_utilgen_PszFromLong(STARS_CV_0012 l, STARS_CV_0172 pcch);
/* PszFromLongK at 0009:22fe; global */
STARS_CV_0170 STARS_FAR STARS_CDECL stars_utilgen_PszFromLongK(STARS_CV_0012 l, STARS_CV_0172 pcch);
/* CommaFormatLong at 0009:2440; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_utilgen_CommaFormatLong(STARS_CV_0170 psz, STARS_CV_0012 l);
/* CtrTextOut at 0009:2534; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_utilgen_CtrTextOut(STARS_CV_0073 hdc, STARS_CV_0072 x, STARS_CV_0072 y, STARS_CV_0170 psz, STARS_CV_0072 cLen);
/* DxStreamTextOut at 0009:2590; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_utilgen_DxStreamTextOut(STARS_CV_0073 hdc, STARS_CV_0172 px, STARS_CV_0072 y, STARS_CV_0170 psz, STARS_CV_0072 cLen, STARS_CV_0072 fPrint);
/* WrapTextOut at 0009:25fe; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_utilgen_WrapTextOut(STARS_CV_0073 hdc, STARS_CV_0172 px, STARS_CV_0172 py, STARS_CV_0170 psz, STARS_CV_0072 cLen, STARS_CV_0072 xLeft, STARS_CV_0072 dxWidth, STARS_CV_0172 pxMax, STARS_CV_0072 fNewLine, STARS_CV_0072 fPrint);
/* AddBackTrailingSpaces at 0009:280c; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_utilgen_AddBackTrailingSpaces(STARS_CV_118e ppch, STARS_CV_0170 pchEnd);
/* ChopLastWord at 0009:2842; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_utilgen_ChopLastWord(STARS_CV_0170 pBeg, STARS_CV_118e ppEnd);
/* ChopTrailingSpaces at 0009:28c6; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_utilgen_ChopTrailingSpaces(STARS_CV_0170 pBeg, STARS_CV_118e ppEnd);
/* RcCtrTextOut at 0009:28fc; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_utilgen_RcCtrTextOut(STARS_CV_0073 hdc, STARS_CV_101b prc, STARS_CV_0170 psz, STARS_CV_0072 cLen);
/* RightTextOut at 0009:29d6; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_utilgen_RightTextOut(STARS_CV_0073 hdc, STARS_CV_0072 x, STARS_CV_0072 y, STARS_CV_0170 psz, STARS_CV_0072 cLen, STARS_CV_0072 dxErase);
/* DiaganolTextOut at 0009:2afa; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_utilgen_DiaganolTextOut(STARS_CV_0073 hdc, STARS_CV_101b prc, STARS_CV_0170 psz, STARS_CV_0072 cLen);
/* ExpandRc at 0009:2f0c; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_utilgen_ExpandRc(STARS_CV_101b prc, STARS_CV_0072 dx, STARS_CV_0072 dy);
/* OffsetRc at 0009:2f3e; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_utilgen_OffsetRc(STARS_CV_101b prc, STARS_CV_0072 dx, STARS_CV_0072 dy);
/* BoundPoints at 0009:2f70; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_utilgen_BoundPoints(STARS_CV_101b prc, STARS_CV_1020 rgpt, STARS_CV_0072 cpt);
/* StickyDlgPos at 0009:3094; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_utilgen_StickyDlgPos(STARS_CV_0073 hwnd, STARS_CV_1020 ppt, STARS_CV_0072 fInit);
/* LDrawGauge at 0009:31a2; global */
STARS_CV_0012 STARS_FAR STARS_CDECL stars_utilgen_LDrawGauge(STARS_CV_0073 hdc, STARS_CV_101b prc, STARS_CV_0072 cSegs, STARS_CV_0112 rgSize, STARS_CV_0173 rghbr, STARS_CV_0012 cTot);
/* _Draw3dFrame at 0009:336a; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_utilgen__Draw3dFrame(STARS_CV_0073 hdc, STARS_CV_101b prc, STARS_CV_0072 fErase);
/* InitBtnTrack at 0009:354c; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_utilgen_InitBtnTrack(STARS_CV_11a3 pbtnt, STARS_CV_0073 hwnd, STARS_CV_0073 hdc, STARS_CV_101b prc, STARS_CV_0072 btf, STARS_CV_0072 dTimer, STARS_CV_0072 fInitDown, STARS_CV_0072 fNoEndRedraw, STARS_CV_0170 szText);
/* FTrackBtn at 0009:364a; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_utilgen_FTrackBtn(STARS_CV_11a3 pbtnt);
/* DrawBtn at 0009:38b8; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_utilgen_DrawBtn(STARS_CV_0073 hdc, STARS_CV_101b prc, STARS_CV_0072 bt, STARS_CV_0072 fDown, STARS_CV_0170 szText);
/* FGetMouseMove at 0009:4146; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_utilgen_FGetMouseMove(STARS_CV_1020 ppt);
/* FGetRMouseMove at 0009:41e0; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_utilgen_FGetRMouseMove(STARS_CV_1020 ppt);
/* DrawFuzzyBorder at 0009:427a; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_utilgen_DrawFuzzyBorder(STARS_CV_0073 hdc, STARS_CV_101b prc);
/* FStringFitsScreen at 0009:43aa; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_utilgen_FStringFitsScreen(STARS_CV_0270 lpsz, STARS_CV_0072 dxMax);
/* HbrGet at 0009:4448; global */
STARS_CV_0073 STARS_FAR STARS_CDECL stars_utilgen_HbrGet(STARS_CV_0022 cr);
/* FreeHbr at 0009:4532; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_utilgen_FreeHbr(STARS_CV_0073 hbr);
/* FCompressUserString at 0009:45a0; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_utilgen_FCompressUserString(STARS_CV_0270 szIn, STARS_CV_0270 szOut, STARS_CV_0172 pcOut);
/* FDecompressUserString at 0009:46e8; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_utilgen_FDecompressUserString(STARS_CV_0270 szIn, STARS_CV_0072 cIn, STARS_CV_0270 szOut, STARS_CV_0172 pcOut);
/* NybbleFromCh at 0009:4880; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_utilgen_NybbleFromCh(STARS_CV_0020 ch);
/* ChFromNybble at 0009:49ea; global */
STARS_CV_0070 STARS_FAR STARS_CDECL stars_utilgen_ChFromNybble(STARS_CV_0072 nyb);
/* DibNumColors at 0009:4a9a; global */
STARS_CV_0021 STARS_FAR STARS_CDECL stars_utilgen_DibNumColors(STARS_CV_0203 pv);
/* HpalFromDib at 0009:4b50; global */
STARS_CV_0073 STARS_FAR STARS_CDECL stars_utilgen_HpalFromDib(STARS_CV_0073 hdib);
/* HpalBlackReserved at 0009:4c9c; global */
STARS_CV_0073 STARS_FAR STARS_CDECL stars_utilgen_HpalBlackReserved(void);
/* PaletteSize at 0009:4d60; global */
STARS_CV_0021 STARS_FAR STARS_CDECL stars_utilgen_PaletteSize(STARS_CV_0203 pv);
/* DibBlt at 0009:4db8; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_utilgen_DibBlt(STARS_CV_0073 hdc, STARS_CV_0072 x0, STARS_CV_0072 y0, STARS_CV_0072 dx, STARS_CV_0072 dy, STARS_CV_0073 hdib, STARS_CV_0072 x1, STARS_CV_0072 y1, STARS_CV_0072 dxSrc, STARS_CV_0072 dySrc, STARS_CV_0012 rop);
/* DibFromBitmap at 0009:4e90; global */
STARS_CV_0073 STARS_FAR STARS_CDECL stars_utilgen_DibFromBitmap(STARS_CV_0073 hbm, STARS_CV_0022 biStyle, STARS_CV_0021 biBits, STARS_CV_0073 hpal);
/* HdibLoadBigResource at 0009:5220; global */
STARS_CV_0073 STARS_FAR STARS_CDECL stars_utilgen_HdibLoadBigResource(STARS_CV_0072 idb);
/* ReadBigBlock at 0009:5310; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_utilgen_ReadBigBlock(STARS_CV_0072 hFile, STARS_CV_0270 lpBuffer, STARS_CV_0022 dwSize);
/* FIntersectCircleLine at 0009:53b4; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_utilgen_FIntersectCircleLine(STARS_CV_1018 ptL1, STARS_CV_1018 ptL2, STARS_CV_1018 ptC, STARS_CV_0012 r2, STARS_CV_0072 dMax, STARS_CV_0172 pdStart, STARS_CV_0172 pdEnd);
/* IntToRoman at 0009:5830; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_utilgen_IntToRoman(STARS_CV_0072 i, STARS_CV_0170 pszOut);
/* FCheckPassword at 0009:58d8; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_utilgen_FCheckPassword(void);
/* LSaltFromSz at 0009:59ce; global */
STARS_CV_0012 STARS_FAR STARS_CDECL stars_utilgen_LSaltFromSz(STARS_CV_0170 psz);
/* PasswordDlg at 0009:5a72; global */
STARS_CV_0072 STARS_FAR STARS_PASCAL stars_utilgen_PasswordDlg(STARS_CV_0073 hwnd, STARS_CV_0073 message, STARS_CV_0021 wParam, STARS_CV_0012 lParam);
/* NewPasswordDlg at 0009:5cba; global */
STARS_CV_0072 STARS_FAR STARS_PASCAL stars_utilgen_NewPasswordDlg(STARS_CV_0073 hwnd, STARS_CV_0073 message, STARS_CV_0021 wParam, STARS_CV_0012 lParam);
/* GetDiskSerialNumber at 0009:5fc0; global */
STARS_CV_0022 STARS_FAR STARS_CDECL stars_utilgen_GetDiskSerialNumber(void);
/* ShowProgressGauge at 0009:636c; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_utilgen_ShowProgressGauge(void);
/* HideProgressGauge at 0009:63b2; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_utilgen_HideProgressGauge(void);
/* UpdateProgressGauge at 0009:63da; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_utilgen_UpdateProgressGauge(STARS_CV_0072 pctX10);
/* ProgressGaugeDlg at 0009:647e; global */
STARS_CV_0072 STARS_FAR STARS_PASCAL stars_utilgen_ProgressGaugeDlg(STARS_CV_0073 hwnd, STARS_CV_0073 message, STARS_CV_0021 wParam, STARS_CV_0012 lParam);
/* DrawProgressGauge at 0009:65f6; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_utilgen_DrawProgressGauge(STARS_CV_0073 hdcOrig, STARS_CV_0072 fFull, STARS_CV_0072 iNumOnly);
/* LDistance2 at 0009:685c; global */
STARS_CV_0012 STARS_FAR STARS_CDECL stars_utilgen_LDistance2(STARS_CV_1018 pt1, STARS_CV_1018 pt2);
/* PszGetLine at 0009:68ba; global */
STARS_CV_0270 STARS_FAR STARS_CDECL stars_utilgen_PszGetLine(STARS_CV_11d4 ppszBeg);
/* CParseNumbers at 0009:6986; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_utilgen_CParseNumbers(STARS_CV_0270 psz, STARS_CV_0112 pl, STARS_CV_0072 cMax);
/* HfontPrinterCreate at 0009:6aa6; global */
STARS_CV_0073 STARS_FAR STARS_CDECL stars_utilgen_HfontPrinterCreate(STARS_CV_0073 hdc, STARS_CV_0072 iSize, STARS_CV_0172 pdyFont);
/* RaceCreationWizard at 001d:0000; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_race_RaceCreationWizard(STARS_CV_0073 hwndParent, STARS_CV_0072 fReadOnly, STARS_CV_0072 fDontWrite);
/* RaceWizardDlg1 at 001d:03ac; global */
STARS_CV_0072 STARS_FAR STARS_PASCAL stars_race_RaceWizardDlg1(STARS_CV_0073 hwnd, STARS_CV_0073 message, STARS_CV_0021 wParam, STARS_CV_0012 lParam);
/* RaceWizardDlg2 at 001d:1060; global */
STARS_CV_0072 STARS_FAR STARS_PASCAL stars_race_RaceWizardDlg2(STARS_CV_0073 hwnd, STARS_CV_0073 message, STARS_CV_0021 wParam, STARS_CV_0012 lParam);
/* DrawRace2 at 001d:179a; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_race_DrawRace2(STARS_CV_0073 hwnd, STARS_CV_0073 hdc, STARS_CV_0072 iDraw);
/* IrcRaceDlgHitTest at 001d:2164; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_race_IrcRaceDlgHitTest(STARS_CV_1018 pt);
/* FTrackRaceDlg2 at 001d:2204; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_race_FTrackRaceDlg2(STARS_CV_0073 hwnd, STARS_CV_1018 pt, STARS_CV_0072 kbd);
/* RaceWizardDlg3 at 001d:2792; global */
STARS_CV_0072 STARS_FAR STARS_PASCAL stars_race_RaceWizardDlg3(STARS_CV_0073 hwnd, STARS_CV_0073 message, STARS_CV_0021 wParam, STARS_CV_0012 lParam);
/* DrawRace3 at 001d:2aa4; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_race_DrawRace3(STARS_CV_0073 hwnd, STARS_CV_0073 hdc, STARS_CV_0072 iDraw);
/* FTrackRaceDlg3 at 001d:2fb8; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_race_FTrackRaceDlg3(STARS_CV_0073 hwnd, STARS_CV_1018 pt, STARS_CV_0072 kbd);
/* GetRaceStat at 001d:30d2; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_race_GetRaceStat(STARS_CV_11ef pplr, STARS_CV_0072 iStat);
/* SetRaceStat at 001d:3114; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_race_SetRaceStat(STARS_CV_11ef pplr, STARS_CV_0072 iStat, STARS_CV_0072 iVal);
/* GetRaceGrbit at 001d:3176; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_race_GetRaceGrbit(STARS_CV_11ef pplr, STARS_CV_0072 ibit);
/* SetRaceGrbit at 001d:31b8; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_race_SetRaceGrbit(STARS_CV_11ef pplr, STARS_CV_0072 ibit, STARS_CV_0072 fSet);
/* RaceWizardDlg4 at 001d:320a; global */
STARS_CV_0072 STARS_FAR STARS_PASCAL stars_race_RaceWizardDlg4(STARS_CV_0073 hwnd, STARS_CV_0073 message, STARS_CV_0021 wParam, STARS_CV_0012 lParam);
/* RaceWizardDlg5 at 001d:378e; global */
STARS_CV_0072 STARS_FAR STARS_PASCAL stars_race_RaceWizardDlg5(STARS_CV_0073 hwnd, STARS_CV_0073 message, STARS_CV_0021 wParam, STARS_CV_0012 lParam);
/* RaceWizardDlg6 at 001d:3bae; global */
STARS_CV_0072 STARS_FAR STARS_PASCAL stars_race_RaceWizardDlg6(STARS_CV_0073 hwnd, STARS_CV_0073 message, STARS_CV_0021 wParam, STARS_CV_0012 lParam);
/* BoundsCheckPlayer at 001d:40ae; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_race_BoundsCheckPlayer(STARS_CV_11ef pplr);
/* CAdvantagePoints at 001d:444c; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_race_CAdvantagePoints(STARS_CV_11ef pplr);
/* LInnateRaceHabitability at 001d:4c6e; global */
STARS_CV_0012 STARS_FAR STARS_CDECL stars_race_LInnateRaceHabitability(STARS_CV_11ef pplr);
/* InvalidateAdvPtsRect at 001d:54d4; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_race_InvalidateAdvPtsRect(STARS_CV_0073 hwnd);
/* DrawRaceAdvantagePoints at 001d:55c6; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_race_DrawRaceAdvantagePoints(STARS_CV_0073 hdc, STARS_CV_101b prc, STARS_CV_11ef pplr);
/* IRaceChecksum at 001d:5888; global */
STARS_CV_0021 STARS_FAR STARS_CDECL stars_race_IRaceChecksum(STARS_CV_11ef pplr);
/* FSaveRace at 001d:58d4; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_race_FSaveRace(STARS_CV_0170 szFileSuggest, STARS_CV_11ef pplr);
/* SetRCWTitle at 001d:5ab8; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_race_SetRCWTitle(STARS_CV_0073 hwnd, STARS_CV_0072 iStep);
/* CreateRandomRace at 001d:5b08; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_race_CreateRandomRace(STARS_CV_11ef pplr);
/* PctTrueMaxGrowth at 001d:65d4; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_race_PctTrueMaxGrowth(STARS_CV_0072 iplr);
/* RelationsDlg at 001f:0088; global */
STARS_CV_0072 STARS_FAR STARS_PASCAL stars_battle_RelationsDlg(STARS_CV_0073 hwnd, STARS_CV_0073 message, STARS_CV_0021 wParam, STARS_CV_0012 lParam);
/* NewPlanNameDlg at 001f:04ce; global */
STARS_CV_0072 STARS_FAR STARS_PASCAL stars_battle_NewPlanNameDlg(STARS_CV_0073 hwnd, STARS_CV_0073 message, STARS_CV_0021 wParam, STARS_CV_0012 lParam);
/* BattlePlansDlg at 001f:0652; global */
STARS_CV_0072 STARS_FAR STARS_PASCAL stars_battle_BattlePlansDlg(STARS_CV_0073 hwnd, STARS_CV_0073 message, STARS_CV_0021 wParam, STARS_CV_0012 lParam);
/* FDeleteBattlePlan at 001f:1706; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_battle_FDeleteBattlePlan(STARS_CV_0072 iplan, STARS_CV_0072 fWarn);
/* SpankTheCheaters at 001f:192a; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_battle_SpankTheCheaters(void);
/* FFleetHasBombs at 001f:1e16; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_battle_FFleetHasBombs(STARS_CV_1004 lpfl);
/* FHullHasBombs at 001f:1ec2; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_battle_FHullHasBombs(STARS_CV_1077 lphul);
/* FFleetHasTeeth at 001f:1fbe; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_battle_FFleetHasTeeth(STARS_CV_1004 lpfl);
/* FHullHasTeeth at 001f:2072; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_battle_FHullHasTeeth(STARS_CV_1077 lphul);
/* FFuelTanker at 001f:20f6; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_battle_FFuelTanker(STARS_CV_1009 lpshdef);
/* CheckTarget at 001f:212a; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_battle_CheckTarget(STARS_CV_10ea ptok, STARS_CV_1004 lpfl, STARS_CV_0072 ishdef);
/* FDumpCargo at 001f:234a; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_battle_FDumpCargo(STARS_CV_1004 lpfl);
/* DropSalvage at 001f:24dc; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_battle_DropSalvage(STARS_CV_1107 plpth, STARS_CV_0212 rgwtMinerals, STARS_CV_0072 iplr, STARS_CV_1020 ppt);
/* CplrBattle at 001f:2952; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_battle_CplrBattle(STARS_CV_1004 lpfl, STARS_CV_0121 rggrfAttack, STARS_CV_0121 pgrfPlayer, STARS_CV_0121 pgrfSpectator);
/* SpdOfShip at 001f:339c; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_battle_SpdOfShip(STARS_CV_1004 lpfl, STARS_CV_0072 ishdef, STARS_CV_10ea ptok, STARS_CV_0072 fDumpCargo, STARS_CV_1009 lpshdef);
/* LpshdefFromTok at 001f:388e; global */
STARS_CV_1009 STARS_FAR STARS_CDECL stars_battle_LpshdefFromTok(STARS_CV_10ea ptok);
/* FCanKillTok at 001f:391e; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_battle_FCanKillTok(STARS_CV_10ea ptok1, STARS_CV_10ea ptok2);
/* DoBattles at 001f:3a26; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_battle_DoBattles(STARS_CV_0072 fPostMovement);
/* RegenShield at 001f:3c16; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_battle_RegenShield(STARS_CV_10ea ptok);
/* InitFromHuldef at 001f:3cba; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_battle_InitFromHuldef(STARS_CV_1077 lphul, STARS_CV_0172 ppctBC);
/* CheckInitiative at 001f:3e68; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_battle_CheckInitiative(STARS_CV_10ea ptok);
/* CheckWeapons at 001f:3ec2; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_battle_CheckWeapons(STARS_CV_10ea ptok, STARS_CV_0172 pfDampeningField, STARS_CV_0120 pinit);
/* RandomizeTokOrder at 001f:44d4; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_battle_RandomizeTokOrder(void);
/* InitializeBoard at 001f:45b4; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_battle_InitializeBoard(STARS_CV_1004 lpfl, STARS_CV_0072 ibrc, STARS_CV_0021 grfPlayer, STARS_CV_0120 pinit, STARS_CV_0172 pinitMin, STARS_CV_0172 pinitMac);
/* DzFromBrcBrc at 001f:4ca8; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_battle_DzFromBrcBrc(STARS_CV_0020 brc1, STARS_CV_0020 brc2);
/* DpFromPtokBrcToBrc at 001f:4d2e; global */
STARS_CV_0012 STARS_FAR STARS_CDECL stars_battle_DpFromPtokBrcToBrc(STARS_CV_10ea ptok, STARS_CV_0020 brcSrc, STARS_CV_0020 brcTarget, STARS_CV_10ea ptokTarget, STARS_CV_0072 fProximity);
/* DzMoveRangeToConsider at 001f:5312; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_battle_DzMoveRangeToConsider(STARS_CV_10ea ptok, STARS_CV_0021 grfAttack, STARS_CV_0120 pbrc);
/* FDoesPrimaryTargetTypeExist at 001f:56d8; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_battle_FDoesPrimaryTargetTypeExist(STARS_CV_10ea ptok, STARS_CV_0021 grfAttack);
/* FIsTargetOfMdTarget at 001f:587a; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_battle_FIsTargetOfMdTarget(STARS_CV_10ea ptok, STARS_CV_0072 mdTarget);
/* ScoreGuessBattleDamage at 001f:598c; global */
STARS_CV_0012 STARS_FAR STARS_CDECL stars_battle_ScoreGuessBattleDamage(STARS_CV_10ea ptokSrc, STARS_CV_0020 brc, STARS_CV_0072 fPrimary, STARS_CV_0021 grfAttack);
/* ScoreFromGiveAndTakeAndTactic at 001f:5e34; global */
STARS_CV_0012 STARS_FAR STARS_CDECL stars_battle_ScoreFromGiveAndTakeAndTactic(STARS_CV_0012 dpGive, STARS_CV_0012 dpTake, STARS_CV_0072 mdTactic);
/* DxyMoveTokTo at 001f:5f18; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_battle_DxyMoveTokTo(STARS_CV_10ea ptok, STARS_CV_0072 spdMove, STARS_CV_0021 grfAttack);
/* CTorpHit at 001f:6790; global */
STARS_CV_0012 STARS_FAR STARS_CDECL stars_battle_CTorpHit(STARS_CV_0012 cTorpBase, STARS_CV_10ea ptok, STARS_CV_0072 pctBase, STARS_CV_0072 pctBC);
/* FAttack at 001f:69ac; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_battle_FAttack(STARS_CV_0072 itokAttacker, STARS_CV_0072 init, STARS_CV_1157 lpbtlrec, STARS_CV_0021 grfAttack);
/* KillShips at 001f:7cde; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_battle_KillShips(STARS_CV_10ea ptok, STARS_CV_0072 cshKill, STARS_CV_0072 ishdef, STARS_CV_1004 lpfl, STARS_CV_0072 fFallout);
/* CreateSalvage at 001f:7ee8; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_battle_CreateSalvage(STARS_CV_1242 pfl, STARS_CV_1107 plpth);
/* FDamageTok at 001f:81d4; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_battle_FDamageTok(STARS_CV_10ea ptok, STARS_CV_0072 itok, STARS_CV_0112 pdpBeam, STARS_CV_0012 dpTorp, STARS_CV_0021 grfWeapon, STARS_CV_0072 fShieldsOnly, STARS_CV_0112 pcTorp);
/* DxyFromSpdRound at 001f:8b28; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_battle_DxyFromSpdRound(STARS_CV_0021 spd, STARS_CV_0072 iRound);
/* FDoCoolBattle at 001f:8bcc; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_battle_FDoCoolBattle(STARS_CV_1004 lpfl, STARS_CV_0072 cplr, STARS_CV_0121 rggrfAttack, STARS_CV_0021 grfPlayer, STARS_CV_0021 grfSpectator);
/* ITechLearnATech at 001f:9918; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_battle_ITechLearnATech(STARS_CV_0072 iplr, STARS_CV_0072 x, STARS_CV_0072 y, STARS_CV_0072 idm, STARS_CV_0121 piGoto);
/* SendBattleMessages at 001f:9c0e; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_battle_SendBattleMessages(STARS_CV_1004 lpflBtl, STARS_CV_0072 cplr, STARS_CV_0072 idBtl, STARS_CV_0121 rgPlrLosses, STARS_CV_0072 grfPlayer, STARS_CV_0072 cShipsInvolved, STARS_CV_0072 cShdefsInvolved, STARS_CV_0021 grfSpectator);
/* FAttackPlayer at 001f:ae06; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_battle_FAttackPlayer(STARS_CV_1004 lpfl, STARS_CV_0072 iplr);
/* DoBombing at 001f:aefa; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_battle_DoBombing(void);
/* LphbAlloc at 000d:0000; global */
STARS_CV_1150 STARS_FAR STARS_CDECL stars_memory_LphbAlloc(STARS_CV_0021 cb, STARS_CV_0072 ht);
/* LphbReAlloc at 000d:0108; global */
STARS_CV_1150 STARS_FAR STARS_CDECL stars_memory_LphbReAlloc(STARS_CV_1150 lphb);
/* FreeHb at 000d:02d8; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_memory_FreeHb(STARS_CV_1150 lphb);
/* ResetHb at 000d:0348; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_memory_ResetHb(STARS_CV_0072 ht);
/* LpAlloc at 000d:03b2; global */
STARS_CV_0203 STARS_FAR STARS_CDECL stars_memory_LpAlloc(STARS_CV_0021 cb, STARS_CV_0072 ht);
/* LphbFromLpHt at 000d:058c; global */
STARS_CV_1150 STARS_FAR STARS_CDECL stars_memory_LphbFromLpHt(STARS_CV_0203 lp, STARS_CV_0072 ht);
/* LpReAlloc at 000d:0660; global */
STARS_CV_0203 STARS_FAR STARS_CDECL stars_memory_LpReAlloc(STARS_CV_0203 lp, STARS_CV_0021 cb, STARS_CV_0072 ht);
/* FreeLp at 000d:07a8; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_memory_FreeLp(STARS_CV_0203 lp, STARS_CV_0072 ht);
/* LpplReAlloc at 000d:0836; global */
STARS_CV_125d STARS_FAR STARS_CDECL stars_memory_LpplReAlloc(STARS_CV_125d lppl, STARS_CV_0021 cAlloc);
/* LpplAlloc at 000d:088c; global */
STARS_CV_125d STARS_FAR STARS_CDECL stars_memory_LpplAlloc(STARS_CV_0021 cbItem, STARS_CV_0021 cAlloc, STARS_CV_0072 ht);
/* FreePl at 000d:0918; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_memory_FreePl(STARS_CV_125d lppl);
/* ResearchDlg at 001c:0000; global */
STARS_CV_0072 STARS_FAR STARS_PASCAL stars_research_ResearchDlg(STARS_CV_0073 hwnd, STARS_CV_0073 message, STARS_CV_0021 wParam, STARS_CV_0012 lParam);
/* DrawResearchDlg at 001c:090a; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_research_DrawResearchDlg(STARS_CV_0073 hwnd, STARS_CV_0073 hdc, STARS_CV_101b prc, STARS_CV_0072 grbitDraw);
/* FTrackResearchDlg at 001c:1a8c; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_research_FTrackResearchDlg(STARS_CV_0073 hwnd, STARS_CV_0072 x, STARS_CV_0072 y, STARS_CV_0072 fkb);
/* GetTechLevelCost at 001c:1dba; global */
STARS_CV_0012 STARS_FAR STARS_CDECL stars_research_GetTechLevelCost(STARS_CV_0072 iTech, STARS_CV_0072 iLevel, STARS_CV_0072 iplr);
/* BrowserDlg at 001c:1ed8; global */
STARS_CV_0072 STARS_FAR STARS_PASCAL stars_research_BrowserDlg(STARS_CV_0073 hwnd, STARS_CV_0073 message, STARS_CV_0021 wParam, STARS_CV_0012 lParam);
/* BrowserWndProc at 001c:2876; global */
STARS_CV_0012 STARS_FAR STARS_PASCAL stars_research_BrowserWndProc(STARS_CV_0073 hwnd, STARS_CV_0073 message, STARS_CV_0073 wParam, STARS_CV_0012 lParam);
/* DisplayComponentInfo at 001c:2ac6; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_research_DisplayComponentInfo(STARS_CV_0073 hdc, STARS_CV_0072 dx, STARS_CV_0072 dy, STARS_CV_10fa ppart);
/* ProjectedResearchSpending at 001c:65ae; global */
STARS_CV_0012 STARS_FAR STARS_CDECL stars_research_ProjectedResearchSpending(STARS_CV_0012 pct);
/* CostOfDevelopingItem at 001c:66e0; global */
STARS_CV_0012 STARS_FAR STARS_CDECL stars_research_CostOfDevelopingItem(STARS_CV_0270 rgTech);
/* FShouldPartBeHidden at 001c:68c4; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_research_FShouldPartBeHidden(STARS_CV_10fa ppart);
/* ChangeProduction at 001b:0000; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_produce_ChangeProduction(STARS_CV_0072 fClear);
/* InitProduction at 001b:015e; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_produce_InitProduction(STARS_CV_103a rgprod);
/* FinishProduction at 001b:1090; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_produce_FinishProduction(STARS_CV_0072 fWrite);
/* ProductionDlg at 001b:1204; global */
STARS_CV_0072 STARS_FAR STARS_PASCAL stars_produce_ProductionDlg(STARS_CV_0073 hwnd, STARS_CV_0073 message, STARS_CV_0021 wParam, STARS_CV_0012 lParam);
/* ProdCommandHandler at 001b:1994; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_produce_ProdCommandHandler(STARS_CV_0073 hwnd, STARS_CV_0021 wParam, STARS_CV_0012 lParam);
/* InitializeProductionDlg at 001b:3430; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_produce_InitializeProductionDlg(STARS_CV_0073 hwnd);
/* DrawProductionDlg at 001b:35dc; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_produce_DrawProductionDlg(STARS_CV_0073 hwnd, STARS_CV_0073 hdc, STARS_CV_101b prc, STARS_CV_0072 iDraw);
/* FillProdSrcLB at 001b:3b00; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_produce_FillProdSrcLB(STARS_CV_0073 hwndLB, STARS_CV_0072 mdFill);
/* PszNameProdItem at 001b:3c92; global */
STARS_CV_0170 STARS_FAR STARS_CDECL stars_produce_PszNameProdItem(STARS_CV_104a lpprod);
/* GetProductionCosts at 001b:3f20; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_produce_GetProductionCosts(STARS_CV_100b lppl, STARS_CV_104a lpprod, STARS_CV_0122 rgCost, STARS_CV_0072 iplr, STARS_CV_0072 fOnlyOne);
/* EstimateItemProdSched at 001b:4f40; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_produce_EstimateItemProdSched(STARS_CV_100b lppl, STARS_CV_1055 lpplprod, STARS_CV_0072 iItem, STARS_CV_0172 piFirst, STARS_CV_0172 piLast);
/* ZipProdDlg at 001b:5490; global */
STARS_CV_0072 STARS_FAR STARS_PASCAL stars_produce_ZipProdDlg(STARS_CV_0073 hwnd, STARS_CV_0073 message, STARS_CV_0021 wParam, STARS_CV_0012 lParam);
/* EnableZipProdBtns at 001b:5df0; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_produce_EnableZipProdBtns(STARS_CV_0073 hwnd, STARS_CV_0072 iSel);
/* FillZipProdLB at 001b:5e58; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_produce_FillZipProdLB(STARS_CV_0073 hwndDlg, STARS_CV_1286 pzpq);
/* LphuldefSBFromId at 0002:510a; global */
STARS_CV_128a STARS_FAR STARS_CDECL stars_parts_LphuldefSBFromId(STARS_CV_0072 id);
/* LphuldefFromId at 0002:512c; global */
STARS_CV_128a STARS_FAR STARS_CDECL stars_parts_LphuldefFromId(STARS_CV_0072 id);
/* LpengineFromId at 0002:5168; global */
STARS_CV_121d STARS_FAR STARS_CDECL stars_parts_LpengineFromId(STARS_CV_0072 id);
/* LpscannerFromId at 0002:518a; global */
STARS_CV_128e STARS_FAR STARS_CDECL stars_parts_LpscannerFromId(STARS_CV_0072 id);
/* LpshdefT at 0002:51ac; global */
STARS_CV_1009 STARS_FAR STARS_CDECL stars_parts_LpshdefT(void);
/* LpshdefSBT at 0002:51c4; global */
STARS_CV_1009 STARS_FAR STARS_CDECL stars_parts_LpshdefSBT(void);
/* LpplanetaryFromId at 0002:51dc; global */
STARS_CV_1292 STARS_FAR STARS_CDECL stars_parts_LpplanetaryFromId(STARS_CV_0072 id);
/* FLookupPartX at 0002:51fe; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_parts_FLookupPartX(STARS_CV_10fa ppart, STARS_CV_0021 grhst, STARS_CV_0021 iItem);
/* FLookupPart at 0002:524e; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_parts_FLookupPart(STARS_CV_10fa ppart);
/* LookupBestPlanetaryScanner at 0002:60be; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_parts_LookupBestPlanetaryScanner(STARS_CV_10fa ppart);
/* TechStatus at 0002:6148; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_parts_TechStatus(STARS_CV_0270 rgTech);
/* ShipBuilder at 001a:008c; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_build_ShipBuilder(STARS_CV_1018 ptDlgSize);
/* ShowMainControls at 001a:0160; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_build_ShowMainControls(STARS_CV_0073 hwnd, STARS_CV_0072 sw);
/* FCheckQueuedShip at 001a:027c; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_build_FCheckQueuedShip(STARS_CV_0073 hwnd, STARS_CV_1009 lpshdef, STARS_CV_0072 fEdit);
/* SlotDlg at 001a:0550; global */
STARS_CV_0072 STARS_FAR STARS_PASCAL stars_build_SlotDlg(STARS_CV_0073 hwnd, STARS_CV_0073 message, STARS_CV_0021 wParam, STARS_CV_0012 lParam);
/* DrawSlotDlg at 001a:2650; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_build_DrawSlotDlg(STARS_CV_0073 hwnd, STARS_CV_0073 hdc, STARS_CV_101b prc, STARS_CV_0072 iDraw);
/* FTrackSlot at 001a:306a; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_build_FTrackSlot(STARS_CV_0073 hwnd, STARS_CV_0072 x, STARS_CV_0072 y, STARS_CV_0072 fkb, STARS_CV_0072 fListBox, STARS_CV_0072 fRightBtn);
/* DrawBuildSelComp at 001a:3ab2; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_build_DrawBuildSelComp(STARS_CV_0073 hwnd, STARS_CV_0073 hdc, STARS_CV_0072 iDraw);
/* PctJammerFromHul at 001a:42e6; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_build_PctJammerFromHul(STARS_CV_1077 lphul);
/* DrawBuildSelHull at 001a:451e; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_build_DrawBuildSelHull(STARS_CV_0073 hwnd, STARS_CV_0073 hdc, STARS_CV_0072 iDraw, STARS_CV_101b prc);
/* SetBuildSelection at 001a:53da; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_build_SetBuildSelection(STARS_CV_0072 iSrc);
/* IDropPart at 001a:5476; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_build_IDropPart(STARS_CV_1018 pt, STARS_CV_1078 hsSrc, STARS_CV_0072 iSrc, STARS_CV_0072 fNoModify);
/* DrawDlgLBEntireItem at 001a:59f0; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_build_DrawDlgLBEntireItem(STARS_CV_1278 lpdis, STARS_CV_0072 inflate);
/* NthValidShdef at 001a:5c06; global */
STARS_CV_1009 STARS_FAR STARS_CDECL stars_build_NthValidShdef(STARS_CV_0072 n);
/* NthValidEnemyShdef at 001a:5cf4; global */
STARS_CV_1009 STARS_FAR STARS_CDECL stars_build_NthValidEnemyShdef(STARS_CV_0072 n);
/* FillBuildDD at 001a:5e80; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_build_FillBuildDD(STARS_CV_0073 hwndDD, STARS_CV_0072 md);
/* FillBuildPartsLB at 001a:63e8; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_build_FillBuildPartsLB(STARS_CV_0073 hwndLB, STARS_CV_0072 grbit);
/* UpdateSlotGlobals at 001a:6528; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_build_UpdateSlotGlobals(void);
/* IEmptyBmpFromGrhst at 001a:6716; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_build_IEmptyBmpFromGrhst(STARS_CV_0072 grhst);
/* FakeListProc at 001a:6758; global */
STARS_CV_0012 STARS_FAR STARS_PASCAL stars_build_FakeListProc(STARS_CV_0073 hwnd, STARS_CV_0021 msg, STARS_CV_0021 wParam, STARS_CV_0012 lParam);
/* MakeNewName at 001a:694c; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_build_MakeNewName(STARS_CV_0270 lpsz);
/* KillQueuedMassPackets at 001a:6a52; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_build_KillQueuedMassPackets(STARS_CV_100b lppl);
/* KillQueuedShips at 001a:6c2a; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_build_KillQueuedShips(STARS_CV_100b lppl);
/* PopupWndProc at 0019:0000; global */
STARS_CV_0012 STARS_FAR STARS_PASCAL stars_popup_PopupWndProc(STARS_CV_0073 hwnd, STARS_CV_0073 message, STARS_CV_0073 wParam, STARS_CV_0012 lParam);
/* FIsPopupHullType at 0019:0148; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_popup_FIsPopupHullType(STARS_CV_0072 ishdef);
/* DrawPopup at 0019:01c0; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_popup_DrawPopup(STARS_CV_0073 hwnd, STARS_CV_0073 hdc);
/* Popup at 0019:0c7c; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_popup_Popup(STARS_CV_0073 hwnd, STARS_CV_0072 x, STARS_CV_0072 y);
/* PopupMenu at 0019:136c; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_popup_PopupMenu(STARS_CV_0073 hwnd, STARS_CV_0072 x, STARS_CV_0072 y, STARS_CV_0072 cString, STARS_CV_0112 rgids, STARS_CV_118e rgsz, STARS_CV_0072 iChecked, STARS_CV_0072 fRightBtn);
/* PtDisplayPlanetStateInfo at 0019:1938; global */
STARS_CV_1018 STARS_FAR STARS_CDECL stars_popup_PtDisplayPlanetStateInfo(STARS_CV_0073 hdc, STARS_CV_0072 fPrint);
/* PtDisplayPlanetPopInfo at 0019:228e; global */
STARS_CV_1018 STARS_FAR STARS_CDECL stars_popup_PtDisplayPlanetPopInfo(STARS_CV_0073 hdc, STARS_CV_0072 fPrint);
/* PtDisplayZipOrdInfo at 0019:2d66; global */
STARS_CV_1018 STARS_FAR STARS_CDECL stars_popup_PtDisplayZipOrdInfo(STARS_CV_0073 hdc, STARS_CV_0072 xCtr, STARS_CV_0072 fPrint);
/* PtDisplayFactoryMineInfo at 0019:3110; global */
STARS_CV_1018 STARS_FAR STARS_CDECL stars_popup_PtDisplayFactoryMineInfo(STARS_CV_0073 hdc, STARS_CV_0072 dx, STARS_CV_0072 fPrint);
/* PtDisplayResourceInfo at 0019:3378; global */
STARS_CV_1018 STARS_FAR STARS_CDECL stars_popup_PtDisplayResourceInfo(STARS_CV_0073 hdc, STARS_CV_0072 dx, STARS_CV_0072 fPrint);
/* PtDisplayString at 0019:355c; global */
STARS_CV_1018 STARS_FAR STARS_CDECL stars_popup_PtDisplayString(STARS_CV_0073 hdc, STARS_CV_0072 dx, STARS_CV_0072 fPrint);
/* MessageWndProc at 0007:5c92; global */
STARS_CV_0012 STARS_FAR STARS_PASCAL stars_msg_MessageWndProc(STARS_CV_0073 hwnd, STARS_CV_0073 message, STARS_CV_0073 wParam, STARS_CV_0012 lParam);
/* SetMsgTitle at 0007:7218; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_msg_SetMsgTitle(STARS_CV_0073 hwnd);
/* IMsgNext at 0007:7808; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_msg_IMsgNext(STARS_CV_0072 fFilteredOnly);
/* IMsgPrev at 0007:78d8; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_msg_IMsgPrev(STARS_CV_0072 fFilteredOnly);
/* DecorateMsgTitleBar at 0007:799c; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_msg_DecorateMsgTitleBar(STARS_CV_0073 hdc, STARS_CV_101b prc);
/* HtMsgBox at 0007:7d8c; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_msg_HtMsgBox(STARS_CV_1018 pt);
/* FSendPlrMsg2 at 0007:7eaa; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_msg_FSendPlrMsg2(STARS_CV_0072 iPlr, STARS_CV_0072 iMsg, STARS_CV_0072 iObj, STARS_CV_0072 p1, STARS_CV_0072 p2);
/* FSendPlrMsg at 0007:7ee8; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_msg_FSendPlrMsg(STARS_CV_0072 iPlr, STARS_CV_0072 iMsg, STARS_CV_0072 iObj, STARS_CV_0072 p1, STARS_CV_0072 p2, STARS_CV_0072 p3, STARS_CV_0072 p4, STARS_CV_0072 p5, STARS_CV_0072 p6, STARS_CV_0072 p7);
/* FSendPrependedPlrMsg at 0007:7f80; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_msg_FSendPrependedPlrMsg(STARS_CV_0072 iPlr, STARS_CV_0072 iMsg, STARS_CV_0072 iObj, STARS_CV_0072 p1, STARS_CV_0072 p2, STARS_CV_0072 p3, STARS_CV_0072 p4, STARS_CV_0072 p5, STARS_CV_0072 p6, STARS_CV_0072 p7);
/* PackageUpMsg at 0007:802a; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_msg_PackageUpMsg(STARS_CV_0120 pb, STARS_CV_0072 iPlr, STARS_CV_0072 iMsg, STARS_CV_0072 iObj, STARS_CV_0072 p1, STARS_CV_0072 p2, STARS_CV_0072 p3, STARS_CV_0072 p4, STARS_CV_0072 p5, STARS_CV_0072 p6, STARS_CV_0072 p7);
/* FSendPlrMsg2XGen at 0007:823a; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_msg_FSendPlrMsg2XGen(STARS_CV_0072 fPrepend, STARS_CV_0072 iMsg, STARS_CV_0072 iObj, STARS_CV_0072 p1, STARS_CV_0072 p2);
/* IdmGetMessageN at 0007:8412; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_msg_IdmGetMessageN(STARS_CV_0072 iMsg);
/* FGetNMsgbig at 0007:8444; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_msg_FGetNMsgbig(STARS_CV_0072 iMsg, STARS_CV_12e2 pmb);
/* PszGetMessageN at 0007:8580; global */
STARS_CV_0170 STARS_FAR STARS_CDECL stars_msg_PszGetMessageN(STARS_CV_0072 iMsg);
/* PszFormatString at 0007:85cc; global */
STARS_CV_0170 STARS_FAR STARS_CDECL stars_msg_PszFormatString(STARS_CV_0170 pszFormat, STARS_CV_0272 pParamsReal);
/* MsgDlg at 0007:8f68; global */
STARS_CV_0072 STARS_FAR STARS_PASCAL stars_msg_MsgDlg(STARS_CV_0073 hwnd, STARS_CV_0073 message, STARS_CV_0021 wParam, STARS_CV_0012 lParam);
/* PszFormatMessage at 0007:9220; global */
STARS_CV_0170 STARS_FAR STARS_CDECL stars_msg_PszFormatMessage(STARS_CV_0072 idm, STARS_CV_0272 pParams);
/* PszFormatIds at 0007:924c; global */
STARS_CV_0170 STARS_FAR STARS_CDECL stars_msg_PszFormatIds(STARS_CV_0072 ids, STARS_CV_0272 pParams);
/* FRemovePlayerMessage at 0007:9278; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_msg_FRemovePlayerMessage(STARS_CV_0072 iPlr, STARS_CV_0072 iMsg, STARS_CV_0072 iObj);
/* FFindPlayerMessage at 0007:932a; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_msg_FFindPlayerMessage(STARS_CV_0072 iPlr, STARS_CV_0072 iMsg, STARS_CV_0072 iObj);
/* MarkPlanetsPlayerLost at 0007:93c6; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_msg_MarkPlanetsPlayerLost(STARS_CV_0072 iPlayer);
/* MarkPlayersThatSentMsgs at 0007:9604; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_msg_MarkPlayersThatSentMsgs(STARS_CV_0072 iPlayer);
/* WritePlayerMessages at 0007:9702; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_msg_WritePlayerMessages(STARS_CV_0072 iPlayer);
/* ResetMessages at 0007:98d6; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_msg_ResetMessages(void);
/* ReadPlayerMessages at 0007:994a; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_msg_ReadPlayerMessages(void);
/* FFinishPlrMsgEntry at 0007:9bd6; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_msg_FFinishPlrMsgEntry(STARS_CV_0072 dInc);
/* PszGetCompressedMessage at 0007:9eb8; global */
STARS_CV_0170 STARS_FAR STARS_CDECL stars_msg_PszGetCompressedMessage(STARS_CV_0072 idm);
/* SetFilteringGroups at 0007:a018; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_msg_SetFilteringGroups(STARS_CV_0072 idm, STARS_CV_0072 fSet);
/* InitBattlePlan at 0010:00c0; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_create_InitBattlePlan(STARS_CV_10bf lpbtlplan, STARS_CV_0072 iplan, STARS_CV_0072 iplr);
/* GenerateWorld at 0010:0136; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_create_GenerateWorld(STARS_CV_0072 fBatchMode);
/* CreateStartupShip at 0010:4690; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_create_CreateStartupShip(STARS_CV_0072 iplr, STARS_CV_0072 idPlanet, STARS_CV_0072 ishdef, STARS_CV_0072 fAddShdef);
/* GenNewGameFromFile at 0010:4812; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_create_GenNewGameFromFile(STARS_CV_0170 pszFile);
/* CreateTutorWorld at 0010:5e5e; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_create_CreateTutorWorld(void);
/* NewGameWizard at 0010:6022; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_create_NewGameWizard(STARS_CV_0073 hwnd, STARS_CV_0072 fReadOnly);
/* InitNewGamePlr at 0010:6e44; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_create_InitNewGamePlr(STARS_CV_0072 iStepMaxSoFar, STARS_CV_0072 lvlAi);
/* InitNewGame3 at 0010:74f8; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_create_InitNewGame3(void);
/* FGetNewGameName at 0010:7508; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_create_FGetNewGameName(STARS_CV_0170 szFileSuggest);
/* SimpleNewGameDlg at 0010:76aa; global */
STARS_CV_0072 STARS_FAR STARS_PASCAL stars_create_SimpleNewGameDlg(STARS_CV_0073 hwnd, STARS_CV_0073 message, STARS_CV_0021 wParam, STARS_CV_0012 lParam);
/* NewGameDlg at 0010:7e92; global */
STARS_CV_0072 STARS_FAR STARS_PASCAL stars_create_NewGameDlg(STARS_CV_0073 hwnd, STARS_CV_0073 message, STARS_CV_0021 wParam, STARS_CV_0012 lParam);
/* NewGameDlg2 at 0010:87d2; global */
STARS_CV_0072 STARS_FAR STARS_PASCAL stars_create_NewGameDlg2(STARS_CV_0073 hwnd, STARS_CV_0073 message, STARS_CV_0021 wParam, STARS_CV_0012 lParam);
/* DrawNewGame2 at 0010:9556; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_create_DrawNewGame2(STARS_CV_0073 hwnd, STARS_CV_0073 hdc, STARS_CV_0072 iDraw);
/* NewGameDlg3 at 0010:99f4; global */
STARS_CV_0072 STARS_FAR STARS_PASCAL stars_create_NewGameDlg3(STARS_CV_0073 hwnd, STARS_CV_0073 message, STARS_CV_0021 wParam, STARS_CV_0012 lParam);
/* DrawNewGame3 at 0010:9d5e; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_create_DrawNewGame3(STARS_CV_0073 hwnd, STARS_CV_0073 hdc, STARS_CV_0072 iDraw);
/* FTrackNewGameDlg3 at 0010:a210; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_create_FTrackNewGameDlg3(STARS_CV_0073 hwnd, STARS_CV_1018 pt, STARS_CV_0072 kbd);
/* SetNGWTitle at 0010:a320; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_create_SetNGWTitle(STARS_CV_0073 hwnd, STARS_CV_0072 iStep);
/* LpplrComp at 0010:b570; global */
STARS_CV_12fe STARS_FAR STARS_CDECL stars_create_LpplrComp(STARS_CV_0072 idAi, STARS_CV_0072 lvlAi);
/* SetVCCheck at 0010:b5bc; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_create_SetVCCheck(STARS_CV_1300 pgame, STARS_CV_0072 vc, STARS_CV_0072 fChecked);
/* GetVCCheck at 0010:b60c; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_create_GetVCCheck(STARS_CV_1300 pgame, STARS_CV_0072 vc);
/* SetVCVal at 0010:b644; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_create_SetVCVal(STARS_CV_1300 pgame, STARS_CV_0072 vc, STARS_CV_0072 val);
/* GetVCVal at 0010:b710; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_create_GetVCVal(STARS_CV_1300 pgame, STARS_CV_0072 vc, STARS_CV_0072 fRaw);
/* WriteOrders at 000f:547e; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_save_WriteOrders(STARS_CV_1004 lpfl);
/* WriteRtPlr at 000f:551c; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_save_WriteRtPlr(STARS_CV_11ef pplr, STARS_CV_0120 pbStore);
/* WriteRtShDef at 000f:574e; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_save_WriteRtShDef(STARS_CV_1009 lpshdef, STARS_CV_1311 ppbStore);
/* FWriteDataFile at 000f:5964; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_save_FWriteDataFile(STARS_CV_0170 pszFileBase, STARS_CV_0072 iPlayer, STARS_CV_0072 fAppend);
/* FAppendFile at 000f:704e; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_save_FAppendFile(STARS_CV_0072 iPlayer);
/* WriteBattles at 000f:709c; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_save_WriteBattles(STARS_CV_0072 iPlayer);
/* WritePlanet at 000f:7a6a; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_save_WritePlanet(STARS_CV_100b lppl, STARS_CV_0072 rt, STARS_CV_0072 fHistory);
/* WriteFleet at 000f:81c6; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_save_WriteFleet(STARS_CV_1004 lpfl);
/* WriteRtString at 000f:87b4; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_save_WriteRtString(STARS_CV_0270 lpsz);
/* MarkFleet at 000f:885e; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_save_MarkFleet(STARS_CV_1004 lpfl, STARS_CV_0072 det);
/* WriteBattlePlan at 000f:89b8; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_save_WriteBattlePlan(STARS_CV_10bf lpbtlplan, STARS_CV_0072 fLog);
/* MarkPlanet at 000f:8adc; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_save_MarkPlanet(STARS_CV_100b lppl, STARS_CV_0072 iPlr, STARS_CV_0073 det);
/* SetSzWorkFromDt at 000f:8cfe; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_save_SetSzWorkFromDt(STARS_CV_0073 dt, STARS_CV_0072 iPlayer);
/* FCreateFile at 000f:8e16; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_save_FCreateFile(STARS_CV_0073 dt, STARS_CV_0072 iPlayer, STARS_CV_0170 szForceName);
/* WriteBOF at 000f:8ea4; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_save_WriteBOF(STARS_CV_0072 iPlayer, STARS_CV_0072 dt, STARS_CV_0072 fMulti);
/* FMarkFile at 000f:904a; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_save_FMarkFile(STARS_CV_0073 dt, STARS_CV_0072 iPlayer, STARS_CV_0072 mdMark, STARS_CV_0072 f);
/* WriteRt at 000f:947c; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_save_WriteRt(STARS_CV_0072 rt, STARS_CV_0072 cb, STARS_CV_0203 rg);
/* RgToStream at 000f:9554; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_save_RgToStream(STARS_CV_0203 rg, STARS_CV_0073 cb);
/* SetVisiblePlanFleet at 000f:95bc; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_save_SetVisiblePlanFleet(STARS_CV_0072 iPlr);
/* SetVisPFInit at 000f:9654; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_save_SetVisPFInit(STARS_CV_0072 iPlr);
/* SetVisPFFleets at 000f:a100; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_save_SetVisPFFleets(STARS_CV_0072 iPlr);
/* SetVisPFPlanets at 000f:abde; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_save_SetVisPFPlanets(STARS_CV_0072 iPlr);
/* SetVisPFThings at 000f:b9ee; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_save_SetVisPFThings(STARS_CV_0072 iPlr);
/* SetVisPFFinish at 000f:c41c; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_save_SetVisPFFinish(STARS_CV_0072 iPlr);
/* Produce at 0018:0000; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_turn2_Produce(void);
/* CBuildProdItem at 0018:0c92; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_turn2_CBuildProdItem(STARS_CV_100b lppl, STARS_CV_104a lpprod, STARS_CV_103a pprodPartial, STARS_CV_0112 rgRes, STARS_CV_0072 fAlchemy, STARS_CV_0172 pmdStatus, STARS_CV_0072 fCalcOnly);
/* FBuildObject at 0018:19b2; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_turn2_FBuildObject(STARS_CV_100b lppl, STARS_CV_0072 grobj, STARS_CV_0072 iItem, STARS_CV_0072 cBuilt, STARS_CV_0112 rgMinerals);
/* CreateShip at 0018:2fd6; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_turn2_CreateShip(STARS_CV_0072 iPlr, STARS_CV_1004 lpfl, STARS_CV_0072 ishdef, STARS_CV_0072 cShip);
/* RandomEvents at 0018:3064; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_turn2_RandomEvents(void);
/* TransferToOthers at 0018:3088; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_turn2_TransferToOthers(void);
/* DropColonists at 0018:34e2; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_turn2_DropColonists(void);
/* HealShips at 0018:444c; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_turn2_HealShips(void);
/* AutoTerraform at 0018:48f6; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_turn2_AutoTerraform(void);
/* RemoteTerraforming at 0018:4c56; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_turn2_RemoteTerraforming(void);
/* FQueueColonistDrop at 0018:4faa; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_turn2_FQueueColonistDrop(STARS_CV_1004 lpfl, STARS_CV_100b lppl, STARS_CV_0012 cColonists);
/* UpdatePopulations at 0018:50a0; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_turn2_UpdatePopulations(void);
/* UpdateGuesses at 0018:532c; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_turn2_UpdateGuesses(void);
/* MineMinerals at 0018:55a4; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_turn2_MineMinerals(void);
/* MeteorStrike at 0018:560e; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_turn2_MeteorStrike(void);
/* TossNonAutoBuildItems at 0018:5aec; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_turn2_TossNonAutoBuildItems(STARS_CV_100b lppl);
/* PlanetaryClimateChange at 0018:5c54; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_turn2_PlanetaryClimateChange(void);
/* DiscoverNewMinerals at 0018:5e0c; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_turn2_DiscoverNewMinerals(void);
/* MysteryTrader at 0018:5efa; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_turn2_MysteryTrader(void);
/* UpdatePlayerScores at 0018:6258; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_turn2_UpdatePlayerScores(void);
/* CreateBackupDir at 0018:6dca; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_turn2_CreateBackupDir(void);
/* FPacketDecay at 0018:6e9c; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_turn2_FPacketDecay(STARS_CV_1007 lpth, STARS_CV_0072 pctRate);
/* ThingDecay at 0018:70c6; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_turn2_ThingDecay(void);
/* UnmarkMineFields at 0018:7638; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_turn2_UnmarkMineFields(void);
/* SweepForMines at 0018:76a4; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_turn2_SweepForMines(void);
/* BreedColonistsInTransit at 0018:7e4e; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_turn2_BreedColonistsInTransit(void);
/* UpdateResearchStatus at 0018:80fe; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_turn2_UpdateResearchStatus(STARS_CV_0072 fUsePool);
/* IBestRemoteTerra at 0018:8b70; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_turn2_IBestRemoteTerra(STARS_CV_100b lppl, STARS_CV_0072 iplr, STARS_CV_0072 fHelp);
/* FGenerateTurn at 0017:0000; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_turn_FGenerateTurn(void);
/* DoOrders at 0017:179a; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_turn_DoOrders(STARS_CV_0072 fPostMovement);
/* MoveThings at 0017:18f4; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_turn_MoveThings(STARS_CV_0072 fPostProd);
/* FuelFleets at 0017:2efa; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_turn_FuelFleets(void);
/* MoveFleets at 0017:32ce; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_turn_MoveFleets(void);
/* FTravelThroughMineFields at 0017:4f60; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_turn_FTravelThroughMineFields(STARS_CV_1004 lpfl, STARS_CV_0172 pdTravel, STARS_CV_1007 lpthHit);
/* LogSplitFleet at 000a:8b0e; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_log_LogSplitFleet(STARS_CV_0011 id);
/* LogMergeFleet at 000a:8b6c; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_log_LogMergeFleet(STARS_CV_0011 id);
/* LogChangeShDef at 000a:8c28; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_log_LogChangeShDef(STARS_CV_1009 lpshdefNew);
/* LogChangeName at 000a:8d5a; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_log_LogChangeName(STARS_CV_0072 grobj, STARS_CV_0072 id, STARS_CV_0170 szName);
/* LogChangeFleet at 000a:8ebe; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_log_LogChangeFleet(STARS_CV_1004 pfl, STARS_CV_1242 pflNew);
/* LogChangeRelations at 000a:9340; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_log_LogChangeRelations(void);
/* LogChangeBtlplan at 000a:93d0; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_log_LogChangeBtlplan(STARS_CV_1578 pbtlplan);
/* LogChangePlanet at 000a:9420; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_log_LogChangePlanet(STARS_CV_100b ppl, STARS_CV_13af pplNew);
/* LogChangeThing at 000a:9908; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_log_LogChangeThing(STARS_CV_1007 lpth, STARS_CV_13b0 pthNew);
/* LogMakeValidXfer at 000a:99f6; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_log_LogMakeValidXfer(STARS_CV_157f plx1, STARS_CV_157f plx2);
/* LogMakeValidXferf at 000a:9fa6; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_log_LogMakeValidXferf(STARS_CV_1586 plxf1, STARS_CV_1586 plxf2);
/* CancelMemRt at 000a:a108; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_log_CancelMemRt(STARS_CV_0072 rt);
/* WriteMemRt at 000a:a130; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_log_WriteMemRt(STARS_CV_0072 rt, STARS_CV_0072 cb, STARS_CV_0103 rg);
/* DirtyGame at 000a:a228; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_log_DirtyGame(STARS_CV_0072 fDirty);
/* FGetPrevLogRt at 000a:a25e; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_log_FGetPrevLogRt(STARS_CV_158d phdr, STARS_CV_0120 pb);
/* FRunLogFile at 000a:a2d6; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_log_FRunLogFile(void);
/* FRunLogRecord at 000a:a38c; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_log_FRunLogRecord(STARS_CV_0072 rt, STARS_CV_0072 cb, STARS_CV_0220 lpb);
/* FLoadLogFile at 000a:c7a2; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_log_FLoadLogFile(STARS_CV_0170 pszLog);
/* FCheckLogFile at 000a:cccc; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_log_FCheckLogFile(STARS_CV_0072 iplr, STARS_CV_0172 pfError);
/* FWriteLogFile at 000a:cdf6; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_log_FWriteLogFile(STARS_CV_0170 pszFileBase, STARS_CV_0072 iPlayer);
/* FWriteTutorialMFile at 000a:d058; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_log_FWriteTutorialMFile(STARS_CV_0072 iTurn);
/* FWriteHistFile at 000a:d29e; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_log_FWriteHistFile(STARS_CV_0072 iPlayer);
/* EnumLogRts at 000a:d6e0; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_log_EnumLogRts(STARS_CV_1596 pfn, STARS_CV_0203 lpPass, STARS_CV_0072 iPass);
/* DrawShipOrders at 000b:0000; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_ship_DrawShipOrders(STARS_CV_0073 hdc, STARS_CV_13bb ptile, STARS_CV_13b2 obj);
/* DrawShipWayPtOrders at 000b:0912; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_ship_DrawShipWayPtOrders(STARS_CV_0073 hdc, STARS_CV_13bb ptile, STARS_CV_13b2 obj);
/* DrawShipPlanet at 000b:17b6; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_ship_DrawShipPlanet(STARS_CV_0073 hdc, STARS_CV_13bb ptile, STARS_CV_13b2 obj);
/* DrawShipCargo at 000b:1a54; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_ship_DrawShipCargo(STARS_CV_0073 hdc, STARS_CV_13bb ptile, STARS_CV_13b2 obj);
/* DrawFleetComp at 000b:1e72; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_ship_DrawFleetComp(STARS_CV_0073 hdc, STARS_CV_13bb ptile, STARS_CV_13b2 obj);
/* FCanSplit at 000b:245c; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_ship_FCanSplit(STARS_CV_0012 cBoat);
/* FCanSplitAll at 000b:24ae; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_ship_FCanSplitAll(STARS_CV_0012 cBoat);
/* FCanMerge at 000b:2522; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_ship_FCanMerge(STARS_CV_1242 pfl);
/* ShipCommandProc at 000b:2640; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_ship_ShipCommandProc(STARS_CV_0073 hwnd, STARS_CV_0073 wParam, STARS_CV_0012 lParam);
/* SelectAdjFleet at 000b:3d32; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_ship_SelectAdjFleet(STARS_CV_0072 dInc, STARS_CV_0072 idFleet);
/* SetFleetDropDownSel at 000b:400e; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_ship_SetFleetDropDownSel(STARS_CV_0072 id);
/* LGetFleetStat at 000b:40f2; global */
STARS_CV_0012 STARS_FAR STARS_CDECL stars_ship_LGetFleetStat(STARS_CV_1004 lpfl, STARS_CV_0072 grStat);
/* WtMaxShdefStat at 000b:41b2; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_ship_WtMaxShdefStat(STARS_CV_1009 lpshdef, STARS_CV_0072 grStat);
/* DrawFleetGauge at 000b:44b6; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_ship_DrawFleetGauge(STARS_CV_0073 hdc, STARS_CV_101b prc, STARS_CV_1004 lpfl, STARS_CV_0072 grbit);
/* DrawFleetBitmap at 000b:490e; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_ship_DrawFleetBitmap(STARS_CV_1004 lpfl, STARS_CV_0073 hdc, STARS_CV_0072 x, STARS_CV_0072 y, STARS_CV_0072 fFrame, STARS_CV_0072 ibmp, STARS_CV_0072 cDiff, STARS_CV_0072 fShrink, STARS_CV_0072 ibmpRace, STARS_CV_0072 csh);
/* FEnumCalcJettison at 000b:4df4; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_ship_FEnumCalcJettison(STARS_CV_0203 lprt, STARS_CV_0072 rt, STARS_CV_0072 cb, STARS_CV_100b lppl, STARS_CV_0072 iFleet);
/* TransferStuff at 000b:4faa; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_ship_TransferStuff(STARS_CV_0072 id1, STARS_CV_0072 grobj1, STARS_CV_0072 id2, STARS_CV_0072 grobj2, STARS_CV_0072 mdXfer);
/* TransferDlg at 000b:5686; global */
STARS_CV_0072 STARS_FAR STARS_PASCAL stars_ship_TransferDlg(STARS_CV_0073 hwnd, STARS_CV_0073 message, STARS_CV_0021 wParam, STARS_CV_0012 lParam);
/* FTrackXfer at 000b:5a16; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_ship_FTrackXfer(STARS_CV_0073 hwnd, STARS_CV_0072 x, STARS_CV_0072 y, STARS_CV_0072 fkb);
/* GetCargoFree at 000b:5f98; global */
STARS_CV_0012 STARS_FAR STARS_CDECL stars_ship_GetCargoFree(STARS_CV_1004 lpfl);
/* GetFuelFree at 000b:6004; global */
STARS_CV_0012 STARS_FAR STARS_CDECL stars_ship_GetFuelFree(STARS_CV_1004 lpfl);
/* ChgCargo at 000b:6034; global */
STARS_CV_0012 STARS_FAR STARS_CDECL stars_ship_ChgCargo(STARS_CV_0072 grobj, STARS_CV_0072 id, STARS_CV_0072 iSupply, STARS_CV_0012 dChg, STARS_CV_0103 pobj);
/* XferSupply at 000b:64cc; global */
STARS_CV_0012 STARS_FAR STARS_CDECL stars_ship_XferSupply(STARS_CV_0072 iSupply, STARS_CV_0012 cQuan);
/* UpdateXferBtns at 000b:66a8; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_ship_UpdateXferBtns(void);
/* DrawXferDlg at 000b:6908; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_ship_DrawXferDlg(STARS_CV_0073 hwnd, STARS_CV_0073 hdc, STARS_CV_101b prc, STARS_CV_0072 iSupply);
/* GetXferLeftRightRcs at 000b:6b46; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_ship_GetXferLeftRightRcs(STARS_CV_101b prcWhole, STARS_CV_101b prcLeft, STARS_CV_101b prcRight);
/* FSetupXferBtns at 000b:6bea; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_ship_FSetupXferBtns(STARS_CV_101b prc);
/* DrawThingXferSide at 000b:7088; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_ship_DrawThingXferSide(STARS_CV_0073 hdc, STARS_CV_101b prc, STARS_CV_13b0 pth, STARS_CV_0072 iSupply);
/* DrawFleetCargoXferSide at 000b:72de; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_ship_DrawFleetCargoXferSide(STARS_CV_0073 hdc, STARS_CV_101b prc, STARS_CV_1242 pfl, STARS_CV_0072 iSupply);
/* DrawFleetShipsXferSide at 000b:7726; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_ship_DrawFleetShipsXferSide(STARS_CV_0073 hdc, STARS_CV_101b prc, STARS_CV_1242 pfl, STARS_CV_0072 iSupply);
/* DrawPlanetXferSide at 000b:79ca; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_ship_DrawPlanetXferSide(STARS_CV_0073 hdc, STARS_CV_101b prc, STARS_CV_13af ppl, STARS_CV_0072 iSupply);
/* ClickInShipOrders at 000b:7cda; global */
STARS_CV_0073 STARS_FAR STARS_CDECL stars_ship_ClickInShipOrders(STARS_CV_1018 pt, STARS_CV_0072 sks, STARS_CV_0072 fCursor, STARS_CV_0072 fRightBtn);
/* FillFleetCompLB at 000b:9166; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_ship_FillFleetCompLB(void);
/* FillOrdersLB at 000b:928c; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_ship_FillOrdersLB(void);
/* SetOrdersLbSel at 000b:9354; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_ship_SetOrdersLbSel(STARS_CV_0072 iSel);
/* UpdateOrdersDDs at 000b:93ee; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_ship_UpdateOrdersDDs(STARS_CV_0072 iLevel);
/* FillBattleDD at 000b:9a36; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_ship_FillBattleDD(STARS_CV_0072 iSel);
/* DeleteCurWayPoint at 000b:9b08; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_ship_DeleteCurWayPoint(STARS_CV_0072 fBackup);
/* DeleteWpFar at 000b:9e28; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_ship_DeleteWpFar(STARS_CV_1004 lpfl, STARS_CV_0072 iDel, STARS_CV_0072 fRecycle);
/* EstFuelUse at 000b:9fe4; global */
STARS_CV_0012 STARS_FAR STARS_CDECL stars_ship_EstFuelUse(STARS_CV_1004 lpfl, STARS_CV_0072 iOrd, STARS_CV_0072 iWarp, STARS_CV_0012 dTravel, STARS_CV_0072 fRangeOnly);
/* FakeEditProc at 000b:a700; global */
STARS_CV_0012 STARS_FAR STARS_PASCAL stars_ship_FakeEditProc(STARS_CV_0073 hwnd, STARS_CV_0021 msg, STARS_CV_0021 wParam, STARS_CV_0012 lParam);
/* IFindIdealWarp at 000b:a76e; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_ship_IFindIdealWarp(STARS_CV_1004 lpfl, STARS_CV_0072 fIgnoreScoops);
/* LFuelUseToWaypoint at 000b:a9f4; global */
STARS_CV_0012 STARS_FAR STARS_CDECL stars_ship_LFuelUseToWaypoint(STARS_CV_1004 lpfl, STARS_CV_0072 iwp, STARS_CV_0072 fMaxCargo);
/* FleetTransferCargoBalance at 000b:ae74; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_ship_FleetTransferCargoBalance(STARS_CV_1242 pflNew1, STARS_CV_1242 pflNew2);
/* DestroyAllIshdefSB at 000b:c280; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_ship_DestroyAllIshdefSB(STARS_CV_0072 ishdefSB, STARS_CV_0072 iplr);
/* DestroyAllIshdef at 000b:c336; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_ship_DestroyAllIshdef(STARS_CV_0072 ishdef, STARS_CV_0072 iplr);
/* RemoveIshdefFromAllQueues at 000b:c5e6; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_ship_RemoveIshdefFromAllQueues(STARS_CV_0072 ishdef, STARS_CV_0072 fSpaceDocks);
/* CshQueued at 000b:c82c; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_ship_CshQueued(STARS_CV_0072 ishdef, STARS_CV_0172 pfProgress, STARS_CV_0072 fSpaceDocks);
/* Merge2Fleets at 000b:c9f6; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_ship_Merge2Fleets(STARS_CV_1004 lpflDst, STARS_CV_1004 lpflDel, STARS_CV_0072 fNoDelete);
/* FleetOrdersChangeTarget at 000b:cafe; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_ship_FleetOrdersChangeTarget(STARS_CV_1004 lpflOld);
/* GetTruePartCost at 000b:cd00; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_ship_GetTruePartCost(STARS_CV_0072 iPlayer, STARS_CV_10fa ppart, STARS_CV_0121 rgCost);
/* PlanetWndProc at 000a:0000; global */
STARS_CV_0012 STARS_FAR STARS_PASCAL stars_planet_PlanetWndProc(STARS_CV_0073 hwnd, STARS_CV_0073 message, STARS_CV_0073 wParam, STARS_CV_0012 lParam);
/* DrawPlanShip at 000a:0d16; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_planet_DrawPlanShip(STARS_CV_0073 hdc, STARS_CV_0072 grbit);
/* FDrawTileNC at 000a:1086; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_planet_FDrawTileNC(STARS_CV_0073 hdc, STARS_CV_13bb ptile, STARS_CV_101b prc, STARS_CV_0170 pszTitle);
/* DrawPlanetMinSum at 000a:12b2; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_planet_DrawPlanetMinSum(STARS_CV_0073 hdc, STARS_CV_13bb ptile, STARS_CV_13b2 obj);
/* DrawPlanetStats at 000a:1716; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_planet_DrawPlanetStats(STARS_CV_0073 hdc, STARS_CV_13bb ptile, STARS_CV_13b2 obj);
/* FGetBestDefensePart at 000a:21f6; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_planet_FGetBestDefensePart(STARS_CV_10fa ppart);
/* DrawPlanetStarbase at 000a:22cc; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_planet_DrawPlanetStarbase(STARS_CV_0073 hdc, STARS_CV_13bb ptile, STARS_CV_13b2 obj);
/* DrawMassWarpGauge at 000a:2afa; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_planet_DrawMassWarpGauge(STARS_CV_0073 hdc, STARS_CV_101b prc, STARS_CV_0072 iBest, STARS_CV_0072 iCur);
/* DrawPlanetProduction at 000a:2c38; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_planet_DrawPlanetProduction(STARS_CV_0073 hdc, STARS_CV_13bb ptile, STARS_CV_13b2 obj);
/* PszProductionETA at 000a:310c; global */
STARS_CV_0170 STARS_FAR STARS_CDECL stars_planet_PszProductionETA(STARS_CV_100b lppl, STARS_CV_1055 lpplprod, STARS_CV_0072 iItem, STARS_CV_0172 etaFirst, STARS_CV_0172 etaLast);
/* DrawPlanShipBitmap at 000a:3336; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_planet_DrawPlanShipBitmap(STARS_CV_0073 hdc, STARS_CV_13bb ptile, STARS_CV_13b2 obj);
/* DrawPlanetShipList at 000a:377e; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_planet_DrawPlanetShipList(STARS_CV_0073 hdc, STARS_CV_13bb ptile, STARS_CV_13b2 obj);
/* SetPlanetTitleBar at 000a:3dec; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_planet_SetPlanetTitleBar(STARS_CV_0073 hwnd);
/* ChangeMainObjSel at 000a:3e7a; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_planet_ChangeMainObjSel(STARS_CV_0072 grobjNew, STARS_CV_0072 iObjSel);
/* FillShipDD at 000a:42a0; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_planet_FillShipDD(STARS_CV_0072 idSkip);
/* SelectAdjPlanet at 000a:44da; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_planet_SelectAdjPlanet(STARS_CV_0072 dInc, STARS_CV_0072 idPlanet);
/* IdFindAdjStarbase at 000a:474e; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_planet_IdFindAdjStarbase(STARS_CV_0072 idPlanet, STARS_CV_0072 fNext);
/* PlanetClick at 000a:489e; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_planet_PlanetClick(STARS_CV_0072 x, STARS_CV_0072 y, STARS_CV_0072 sks, STARS_CV_0072 fRightBtn);
/* ClickInPlanetOrders at 000a:515c; global */
STARS_CV_0073 STARS_FAR STARS_CDECL stars_planet_ClickInPlanetOrders(STARS_CV_1018 pt, STARS_CV_0072 sks, STARS_CV_0072 fCursor, STARS_CV_0072 fRightBtn);
/* EnsureTileSize at 000a:587e; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_planet_EnsureTileSize(STARS_CV_0072 fSmallTiles);
/* ReflowColumn at 000a:5b80; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_planet_ReflowColumn(STARS_CV_0072 iCol, STARS_CV_0072 iTile, STARS_CV_0072 fRedraw);
/* IBestTerraform at 000a:5dd2; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_planet_IBestTerraform(STARS_CV_100b lppl, STARS_CV_0072 fHelp);
/* PszCalcEnvVar at 000a:5fcc; global */
STARS_CV_0170 STARS_FAR STARS_CDECL stars_planet_PszCalcEnvVar(STARS_CV_0072 iEnv, STARS_CV_0072 iVar);
/* PszCalcGravity at 000a:6058; global */
STARS_CV_0170 STARS_FAR STARS_CDECL stars_planet_PszCalcGravity(STARS_CV_0072 iGravity);
/* HandleFocusState at 000a:60e6; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_planet_HandleFocusState(STARS_CV_1278 lpdis, STARS_CV_0072 inflate);
/* DrawCBEntireItem at 000a:6128; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_planet_DrawCBEntireItem(STARS_CV_1278 lpdis, STARS_CV_0072 inflate);
/* DrawProductionItem at 000a:6208; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_planet_DrawProductionItem(STARS_CV_0073 hdc, STARS_CV_101b prc, STARS_CV_0170 psz, STARS_CV_0072 inflate, STARS_CV_0072 fSelected, STARS_CV_0072 fListbox);
/* FillPlanetProdLB at 000a:6692; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_planet_FillPlanetProdLB(STARS_CV_0073 hwnd, STARS_CV_1055 lpplprod, STARS_CV_100b lppl);
/* PctPlanetCapacity at 000a:6aca; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_planet_PctPlanetCapacity(STARS_CV_100b lppl);
/* PctPlanetOptValue at 000a:6b88; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_planet_PctPlanetOptValue(STARS_CV_100b lppl, STARS_CV_0072 iPlr);
/* PctPlanetDesirability at 000a:6e1e; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_planet_PctPlanetDesirability(STARS_CV_100b lppl, STARS_CV_0072 iPlr);
/* CalcPlanetMaxPop at 000a:7096; global */
STARS_CV_0012 STARS_FAR STARS_CDECL stars_planet_CalcPlanetMaxPop(STARS_CV_0072 idpl, STARS_CV_0072 iplr);
/* CMaxMines at 000a:7248; global */
STARS_CV_0011 STARS_FAR STARS_CDECL stars_planet_CMaxMines(STARS_CV_100b lppl, STARS_CV_0072 iplr);
/* CMaxOperableMines at 000a:7304; global */
STARS_CV_0011 STARS_FAR STARS_CDECL stars_planet_CMaxOperableMines(STARS_CV_100b lppl, STARS_CV_0072 iplr, STARS_CV_0072 fNextYear);
/* CMinesOperating at 000a:73fc; global */
STARS_CV_0011 STARS_FAR STARS_CDECL stars_planet_CMinesOperating(STARS_CV_100b lppl);
/* CFactoriesOperating at 000a:74be; global */
STARS_CV_0011 STARS_FAR STARS_CDECL stars_planet_CFactoriesOperating(STARS_CV_100b lppl);
/* CMaxFactories at 000a:755c; global */
STARS_CV_0011 STARS_FAR STARS_CDECL stars_planet_CMaxFactories(STARS_CV_100b lppl, STARS_CV_0072 iplr);
/* CMaxOperableFactories at 000a:7618; global */
STARS_CV_0011 STARS_FAR STARS_CDECL stars_planet_CMaxOperableFactories(STARS_CV_100b lppl, STARS_CV_0072 iplr, STARS_CV_0072 fNextYear);
/* CMaxDefenses at 000a:7710; global */
STARS_CV_0011 STARS_FAR STARS_CDECL stars_planet_CMaxDefenses(STARS_CV_100b lppl, STARS_CV_0072 iplr);
/* CMaxOperableDefenses at 000a:77ae; global */
STARS_CV_0011 STARS_FAR STARS_CDECL stars_planet_CMaxOperableDefenses(STARS_CV_100b lppl, STARS_CV_0072 iplr, STARS_CV_0072 fNextYear);
/* CResourcesAtPlanet at 000a:788e; global */
STARS_CV_0011 STARS_FAR STARS_CDECL stars_planet_CResourcesAtPlanet(STARS_CV_100b lppl, STARS_CV_0072 iplr);
/* IWarpMAFromLppl at 000a:7b10; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_planet_IWarpMAFromLppl(STARS_CV_100b lppl, STARS_CV_0172 pfTwo);
/* StargateRangeFromLppl at 000a:7cfa; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_planet_StargateRangeFromLppl(STARS_CV_100b lppl, STARS_CV_0072 iplr, STARS_CV_0072 ish);
/* FProdIsTerra at 000a:7e9a; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_planet_FProdIsTerra(STARS_CV_104a lpprod);
/* IpctCanTerraformLppl at 000a:7f56; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_planet_IpctCanTerraformLppl(STARS_CV_100b lppl);
/* FCanTerraformLppl at 000a:8022; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_planet_FCanTerraformLppl(STARS_CV_100b lppl, STARS_CV_0172 rgEnvMin, STARS_CV_0172 rgEnvMax, STARS_CV_0172 rgEnvCost, STARS_CV_0072 fHelp);
/* UninhabitPlanet at 000a:8732; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_planet_UninhabitPlanet(STARS_CV_100b lppl);
/* PctCloakFromHuldef at 000a:88c0; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_planet_PctCloakFromHuldef(STARS_CV_1077 lphul, STARS_CV_0072 iplr, STARS_CV_0172 ppctSteal);
/* FReadShDef at 000f:0006; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_file_FReadShDef(STARS_CV_15e8 lprt, STARS_CV_1009 lpshdef, STARS_CV_0072 iplrLoad);
/* ReadRtPlr at 000f:05e2; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_file_ReadRtPlr(STARS_CV_11ef pplr, STARS_CV_0120 pbIn);
/* FLoadGame at 000f:0810; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_file_FLoadGame(STARS_CV_0170 pszFileName, STARS_CV_0170 pszExt);
/* FReadPlanet at 000f:3206; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_file_FReadPlanet(STARS_CV_0072 iPlayer, STARS_CV_100b lppl, STARS_CV_0072 fHistory, STARS_CV_0072 fPreInited);
/* FReadFleet at 000f:3a4c; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_file_FReadFleet(STARS_CV_1004 lpfl);
/* UnpackBattlePlan at 000f:40ce; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_file_UnpackBattlePlan(STARS_CV_0220 lpb, STARS_CV_10bf lpbtlplan, STARS_CV_0072 iplan);
/* UpdateBattleRecords at 000f:41ac; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_file_UpdateBattleRecords(void);
/* AskSaveDialog at 000f:432a; global */
STARS_CV_0072 STARS_FAR STARS_PASCAL stars_file_AskSaveDialog(STARS_CV_0073 hwnd, STARS_CV_0073 message, STARS_CV_0021 wParam, STARS_CV_0012 lParam);
/* PromptSaveGame at 000f:43ee; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_file_PromptSaveGame(void);
/* DestroyCurGame at 000f:44b0; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_file_DestroyCurGame(void);
/* FBogusLong at 000f:484c; file_static */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_file_FBogusLong(STARS_CV_0022 lSerial);
/* FValidSerialLong at 000f:48c4; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_file_FValidSerialLong(STARS_CV_0022 lSerial);
/* FileError at 000f:4a10; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_file_FileError(STARS_CV_0072 ids);
/* GetFileStatus at 000f:4a60; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_file_GetFileStatus(STARS_CV_0072 dt, STARS_CV_0072 iPlayer);
/* FOpenFile at 000f:4ac2; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_file_FOpenFile(STARS_CV_0073 dt, STARS_CV_0072 iPlayer, STARS_CV_0072 md);
/* FNewTurnAvail at 000f:4f22; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_file_FNewTurnAvail(STARS_CV_0072 idPlayer);
/* FCheckFile at 000f:4fb2; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_file_FCheckFile(STARS_CV_0073 dt, STARS_CV_0072 iPlayer, STARS_CV_0073 md);
/* ReadRt at 000f:5168; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_file_ReadRt(void);
/* FBadFileError at 000f:524e; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_file_FBadFileError(STARS_CV_0072 ids);
/* StreamOpen at 000f:52ae; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_file_StreamOpen(STARS_CV_0170 szFile, STARS_CV_0072 mdOpen);
/* StreamClose at 000f:53cc; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_file_StreamClose(void);
/* RgFromStream at 000f:53f4; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_file_RgFromStream(STARS_CV_0203 rg, STARS_CV_0073 cb);
/* FCreateStuff at 0001:0014; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_init_FCreateStuff(void);
/* FCreateFonts at 0001:0ab2; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_init_FCreateFonts(STARS_CV_0073 hdc);
/* InitInstance at 0001:0d70; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_init_InitInstance(STARS_CV_0072 nCmdShow);
/* InitTiles at 0001:0eaa; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_init_InitTiles(void);
/* GetIniWinRc at 0001:1020; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_init_GetIniWinRc(STARS_CV_0170 szSection, STARS_CV_0170 szIniFile, STARS_CV_0072 ids, STARS_CV_15f4 pwn);
/* ReadIniSettings at 0001:124c; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_init_ReadIniSettings(void);
/* ReadIniTileSettings at 0001:2b8a; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_init_ReadIniTileSettings(STARS_CV_0170 pszFormat, STARS_CV_13bb rgtile, STARS_CV_0072 ctile);
/* ScannerWndProc at 000c:0032; global */
STARS_CV_0012 STARS_FAR STARS_PASCAL stars_scan_ScannerWndProc(STARS_CV_0073 hwnd, STARS_CV_0073 msg, STARS_CV_0073 wParam, STARS_CV_0012 lParam);
/* PtToScan at 000c:0efc; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_scan_PtToScan(STARS_CV_0072 d);
/* ScanToPt at 000c:0fc2; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_scan_ScanToPt(STARS_CV_0072 d);
/* DrawScanner at 000c:108a; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_scan_DrawScanner(STARS_CV_0073 hdc, STARS_CV_101b prc);
/* DrawScanFleetCount at 000c:47d2; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_scan_DrawScanFleetCount(STARS_CV_1004 lpfl, STARS_CV_0072 x, STARS_CV_0072 y, STARS_CV_0073 hdc, STARS_CV_0073 hdcMem);
/* CShipsScanVis at 000c:4bf4; global */
STARS_CV_0012 STARS_FAR STARS_CDECL stars_scan_CShipsScanVis(STARS_CV_1004 lpfl);
/* DrawRadarCircle at 000c:4e7c; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_scan_DrawRadarCircle(STARS_CV_1600 pdc, STARS_CV_101b prc);
/* DrawShipScanPath at 000c:540c; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_scan_DrawShipScanPath(STARS_CV_0073 hdc, STARS_CV_0072 fShow);
/* DrawScannerSBar at 000c:62d8; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_scan_DrawScannerSBar(STARS_CV_0073 hdc, STARS_CV_101b prc, STARS_CV_1604 psbar, STARS_CV_0072 fFullRedraw);
/* DrawLockLight at 000c:6b00; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_scan_DrawLockLight(STARS_CV_0073 hdc, STARS_CV_101b prc, STARS_CV_0072 fFullRedraw);
/* SetScanScrollBars at 000c:6c14; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_scan_SetScanScrollBars(STARS_CV_0073 hwnd);
/* ScrollScanner at 000c:6d30; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_scan_ScrollScanner(STARS_CV_0072 dx, STARS_CV_0072 dy);
/* RedrawScanSel at 000c:6f30; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_scan_RedrawScanSel(STARS_CV_0073 hdc, STARS_CV_0072 fVis);
/* FEnsurePointOnScreen at 000c:715c; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_scan_FEnsurePointOnScreen(STARS_CV_1018 pt, STARS_CV_0072 fScroll);
/* CtrPointScan at 000c:7278; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_scan_CtrPointScan(STARS_CV_1018 pt, STARS_CV_0072 fScroll);
/* LogicalToScan at 000c:744e; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_scan_LogicalToScan(STARS_CV_1020 ppt);
/* ScanToLogical at 000c:7490; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_scan_ScanToLogical(STARS_CV_1020 ppt);
/* FAddWayPoint at 000c:7504; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_scan_FAddWayPoint(STARS_CV_1018 ptIn, STARS_CV_144c pscan);
/* IWarpBestForWaypoint at 000c:7a18; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_scan_IWarpBestForWaypoint(STARS_CV_1004 lpfl, STARS_CV_1072 lpord);
/* FNearAWayPoint at 000c:8074; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_scan_FNearAWayPoint(STARS_CV_1018 pt, STARS_CV_0072 fLogical);
/* FHandleWayPointDrag at 000c:8176; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_scan_FHandleWayPointDrag(STARS_CV_1018 pt);
/* DrawScanXorLines at 000c:8af6; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_scan_DrawScanXorLines(STARS_CV_0073 hdc, STARS_CV_1020 rgpt, STARS_CV_0072 cpt);
/* SetScanWp at 000c:8c5a; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_scan_SetScanWp(STARS_CV_0072 iNew);
/* ChangeScanSel at 000c:8cc4; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_scan_ChangeScanSel(STARS_CV_144c pscan, STARS_CV_0072 fValidScan);
/* FGetNextObjHere at 000c:909c; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_scan_FGetNextObjHere(STARS_CV_144c pscan, STARS_CV_0072 fOnlyOurs);
/* FindDlg at 000c:9286; global */
STARS_CV_0072 STARS_FAR STARS_PASCAL stars_scan_FindDlg(STARS_CV_0073 hwnd, STARS_CV_0073 msg, STARS_CV_0021 wParam, STARS_CV_0012 lParam);
/* FSelectSz at 000c:945a; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_scan_FSelectSz(STARS_CV_0170 szName);
/* GetScanFleetOrientation at 000c:978c; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_scan_GetScanFleetOrientation(STARS_CV_1004 lpfl, STARS_CV_1020 ppt, STARS_CV_1020 pptD);
/* GetDxDyOrientation at 000c:987c; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_scan_GetDxDyOrientation(STARS_CV_0072 dx, STARS_CV_0072 dy, STARS_CV_1020 ppt, STARS_CV_1020 pptD);
/* FHandleMeasuringTape at 000c:9974; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_scan_FHandleMeasuringTape(STARS_CV_144c pscan, STARS_CV_1018 pt);
/* FLookupSelPlanet at 0008:0000; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_util_FLookupSelPlanet(STARS_CV_13af ppl);
/* FDupPlanet at 0008:0032; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_util_FDupPlanet(STARS_CV_100b lppl, STARS_CV_13af ppl);
/* LpthFromId at 0008:01b2; global */
STARS_CV_1007 STARS_FAR STARS_CDECL stars_util_LpthFromId(STARS_CV_0072 idth);
/* LpplFromId at 0008:021e; global */
STARS_CV_100b STARS_FAR STARS_CDECL stars_util_LpplFromId(STARS_CV_0072 idPlanet);
/* CalcPctSurvive at 0008:02f6; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_util_CalcPctSurvive(STARS_CV_100b lppl, STARS_CV_0140 ppct, STARS_CV_0140 ppctSmart);
/* FLookupPlanet at 0008:04a6; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_util_FLookupPlanet(STARS_CV_0072 iPlanet, STARS_CV_13af ppl);
/* DpOfLpflIshdef at 0008:0746; global */
STARS_CV_0012 STARS_FAR STARS_CDECL stars_util_DpOfLpflIshdef(STARS_CV_1004 lpfl, STARS_CV_0072 ishdef);
/* FLookupThing at 0008:07fe; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_util_FLookupThing(STARS_CV_0072 idth, STARS_CV_13b0 pth);
/* SelectOursAtObject at 0008:08f2; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_util_SelectOursAtObject(STARS_CV_1020 ppt);
/* LComputePower at 0008:0b32; global */
STARS_CV_0012 STARS_FAR STARS_CDECL stars_util_LComputePower(STARS_CV_1009 lpshdef);
/* ComputeShdefPowers at 0008:0e4e; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_util_ComputeShdefPowers(void);
/* DpShieldOfShdef at 0008:0f24; global */
STARS_CV_0012 STARS_FAR STARS_CDECL stars_util_DpShieldOfShdef(STARS_CV_1009 lpshdef, STARS_CV_0072 iplr);
/* IStargateFromLppl at 0008:10fa; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_util_IStargateFromLppl(STARS_CV_100b lppl);
/* PszPlayerName at 0008:11f2; global */
STARS_CV_0170 STARS_FAR STARS_CDECL stars_util_PszPlayerName(STARS_CV_0072 iPlayer, STARS_CV_0072 fCapital, STARS_CV_0072 fPlural, STARS_CV_0072 fThe, STARS_CV_0072 grWord, STARS_CV_11ef pplr);
/* FCalcFleetBombDamage at 0008:145c; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_util_FCalcFleetBombDamage(STARS_CV_1004 lpfl, STARS_CV_0112 pdmgPeople, STARS_CV_0112 pdmgPeopleMin, STARS_CV_0112 pdmgPeopleSmart, STARS_CV_0112 pdmgBldg, STARS_CV_0112 ppctTerra, STARS_CV_0172 pfMulti);
/* LinkFleets at 0008:1bb4; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_util_LinkFleets(STARS_CV_0072 fUnused);
/* ICompFleetPoint at 0008:1f0c; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_util_ICompFleetPoint(STARS_CV_0103 arg1, STARS_CV_0103 arg2);
/* ICompFleetPoint2 at 0008:1fa2; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_util_ICompFleetPoint2(STARS_CV_0103 arg1, STARS_CV_0103 arg2);
/* FLookupSelShip at 0008:2032; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_util_FLookupSelShip(STARS_CV_1242 pfl);
/* LpflFromId at 0008:2078; global */
STARS_CV_1004 STARS_FAR STARS_CDECL stars_util_LpflFromId(STARS_CV_0072 idFleet);
/* FLookupFleet at 0008:2160; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_util_FLookupFleet(STARS_CV_0072 idFleet, STARS_CV_1242 pfl);
/* FDupFleet at 0008:2332; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_util_FDupFleet(STARS_CV_1004 lpfl, STARS_CV_1242 pfl);
/* FLookupObject at 0008:24a0; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_util_FLookupObject(STARS_CV_0072 grobj, STARS_CV_0072 id, STARS_CV_0103 pobj);
/* FLookupOrbitingXfer at 0008:24fa; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_util_FLookupOrbitingXfer(STARS_CV_0072 idPlanet, STARS_CV_0072 iNth, STARS_CV_15ca pxf, STARS_CV_0072 idSkip);
/* PszGetThingName at 0008:26de; global */
STARS_CV_0170 STARS_FAR STARS_CDECL stars_util_PszGetThingName(STARS_CV_0072 id);
/* PszGetFleetName at 0008:292c; global */
STARS_CV_0170 STARS_FAR STARS_CDECL stars_util_PszGetFleetName(STARS_CV_0072 id);
/* WFromLpfl at 0008:2b10; global */
STARS_CV_0021 STARS_FAR STARS_CDECL stars_util_WFromLpfl(STARS_CV_1004 lpfl);
/* PszFleetNameFromWord at 0008:2b5e; global */
STARS_CV_0170 STARS_FAR STARS_CDECL stars_util_PszFleetNameFromWord(STARS_CV_0021 w);
/* PszGetPlanetName at 0008:2c6a; global */
STARS_CV_0170 STARS_FAR STARS_CDECL stars_util_PszGetPlanetName(STARS_CV_0072 id);
/* IflFromLpfl at 0008:2ce8; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_util_IflFromLpfl(STARS_CV_1004 lpfl);
/* FDeleteFleet at 0008:2d44; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_util_FDeleteFleet(STARS_CV_0072 idFleet, STARS_CV_0072 grobjSel, STARS_CV_0072 idSel);
/* LpflNew at 0008:300c; global */
STARS_CV_1004 STARS_FAR STARS_CDECL stars_util_LpflNew(STARS_CV_0072 iPlr, STARS_CV_0072 idPl);
/* LpflNewSplit at 0008:3372; global */
STARS_CV_1004 STARS_FAR STARS_CDECL stars_util_LpflNewSplit(STARS_CV_1242 pfl);
/* FFleetMergeAll at 0008:34d8; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_util_FFleetMergeAll(STARS_CV_1242 pfl);
/* FFleetSplitAll at 0008:3a00; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_util_FFleetSplitAll(STARS_CV_1242 pfl);
/* PszGetLocName at 0008:3b08; global */
STARS_CV_0170 STARS_FAR STARS_CDECL stars_util_PszGetLocName(STARS_CV_0072 grobj, STARS_CV_0072 id, STARS_CV_0072 x, STARS_CV_0072 y);
/* CchGetETA at 0008:3bc8; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_util_CchGetETA(STARS_CV_0073 hdc, STARS_CV_1004 lpfl, STARS_CV_0170 sz, STARS_CV_0072 iwp, STARS_CV_0072 fSmall);
/* IshdefPrimaryFromLpfl at 0008:3e1c; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_util_IshdefPrimaryFromLpfl(STARS_CV_1004 lpfl, STARS_CV_0172 pcDiff);
/* PszGetDistance at 0008:3f00; global */
STARS_CV_0170 STARS_FAR STARS_CDECL stars_util_PszGetDistance(STARS_CV_0072 x1, STARS_CV_0072 y1, STARS_CV_0072 x2, STARS_CV_0072 y2);
/* DGetDistance at 0008:3fe4; global */
STARS_CV_0041 STARS_FAR STARS_CDECL stars_util_DGetDistance(STARS_CV_0072 x1, STARS_CV_0072 y1, STARS_CV_0072 x2, STARS_CV_0072 y2);
/* FFindNearestObject at 0008:4070; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_util_FFindNearestObject(STARS_CV_1018 pt, STARS_CV_0072 grobj, STARS_CV_144c pscan);
/* UpdateShdefCost at 0008:47b0; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_util_UpdateShdefCost(STARS_CV_1009 lpshdef);
/* WPackLong at 0008:4ba2; global */
STARS_CV_0021 STARS_FAR STARS_CDECL stars_util_WPackLong(STARS_CV_0012 l);
/* GetPlanetScannerRange at 0008:4c02; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_util_GetPlanetScannerRange(STARS_CV_100b lppl, STARS_CV_0172 pDeep);
/* GetCachedFleetScannerRange at 0008:4e02; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_util_GetCachedFleetScannerRange(STARS_CV_1004 lpfl, STARS_CV_0172 pdPlanRange, STARS_CV_0172 ppctDetect, STARS_CV_0172 piSteal);
/* GetFleetScannerRange at 0008:4fb8; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_util_GetFleetScannerRange(STARS_CV_1004 lpfl, STARS_CV_0172 pdPlanRange, STARS_CV_0172 ppctDetect, STARS_CV_0172 piSteal);
/* GetShdefScannerRange at 0008:50d0; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_util_GetShdefScannerRange(STARS_CV_1009 lpshdef, STARS_CV_0072 iplr, STARS_CV_0172 pdPlanRange, STARS_CV_0172 ppctDetect, STARS_CV_0172 piSteal);
/* LCalcFuelGainFromRamScoops at 0008:56b8; global */
STARS_CV_0012 STARS_FAR STARS_CDECL stars_util_LCalcFuelGainFromRamScoops(STARS_CV_1004 lpfl, STARS_CV_0072 iWarp, STARS_CV_0012 dTravel);
/* CalcPlayerScore at 0008:58a6; global */
STARS_CV_0012 STARS_FAR STARS_CDECL stars_util_CalcPlayerScore(STARS_CV_0072 iPlr, STARS_CV_1649 pscore);
/* GetTrueHullCost at 0008:5dba; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_util_GetTrueHullCost(STARS_CV_0072 iPlayer, STARS_CV_1077 lphul, STARS_CV_0121 rgCost);
/* DecorateHullName at 0008:5e0e; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_util_DecorateHullName(STARS_CV_0072 iplr, STARS_CV_0072 ish, STARS_CV_0170 psz);
/* DrawABunchOfStars at 0008:5fe2; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_util_DrawABunchOfStars(STARS_CV_0073 hdc, STARS_CV_101b prc);
/* LongFromSerialCh at 0008:6280; global */
STARS_CV_0012 STARS_FAR STARS_CDECL stars_util_LongFromSerialCh(STARS_CV_0070 ch);
/* FValidSerialNo at 0008:62f8; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_util_FValidSerialNo(STARS_CV_0170 psz, STARS_CV_0112 plSerial);
/* FMatchTarget at 0008:6612; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_util_FMatchTarget(STARS_CV_1004 lpflTarget, STARS_CV_0072 mdTarget, STARS_CV_0072 fExact);
/* ValidateWaypoints at 0008:68c6; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_util_ValidateWaypoints(void);
/* ChgPopFromPlanet at 0008:7082; global */
STARS_CV_0012 STARS_FAR STARS_CDECL stars_util_ChgPopFromPlanet(STARS_CV_100b lppl, STARS_CV_0072 fUpdate);
/* FCanFleetUseStargates at 0008:75e2; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_util_FCanFleetUseStargates(STARS_CV_1004 lpfl, STARS_CV_1018 ptSrc, STARS_CV_1018 ptDst);
/* FFleetCanJumpgate at 0008:7960; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_util_FFleetCanJumpgate(STARS_CV_1004 lpfl);
/* WtFromLpfl at 0008:7a68; global */
STARS_CV_0012 STARS_FAR STARS_CDECL stars_util_WtFromLpfl(STARS_CV_1004 lpfl);
/* FCanBuildShdef at 0008:7b40; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_util_FCanBuildShdef(STARS_CV_1009 lpshdef, STARS_CV_0072 iplr);
/* IshFindSimilarDesign at 0008:7c5e; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_util_IshFindSimilarDesign(STARS_CV_1077 lphul, STARS_CV_0072 iPlrDst);
/* DrawPlanetPrintDot at 0008:7e44; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_util_DrawPlanetPrintDot(STARS_CV_0073 hdc, STARS_CV_0072 x, STARS_CV_0072 y, STARS_CV_0072 iSize);
/* ClearFile at 0008:7f6a; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_util_ClearFile(STARS_CV_0072 dt);
/* OutputSz at 0008:7fe6; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_util_OutputSz(STARS_CV_0072 dt, STARS_CV_0270 sz);
/* TurnLog at 0008:80c2; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_util_TurnLog(STARS_CV_0072 ids);
/* MineWndProc at 0006:0000; global */
STARS_CV_0012 STARS_FAR STARS_PASCAL stars_mine_MineWndProc(STARS_CV_0073 hwnd, STARS_CV_0073 message, STARS_CV_0073 wParam, STARS_CV_0012 lParam);
/* InvalidateMineralBars at 0006:040a; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_mine_InvalidateMineralBars(void);
/* GetMineFieldCounts at 0006:05ac; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_mine_GetMineFieldCounts(STARS_CV_0073 id, STARS_CV_0172 pithm, STARS_CV_0172 pcthm);
/* DrawMineSurvey at 0006:065a; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_mine_DrawMineSurvey(STARS_CV_0073 hdc, STARS_CV_101b prc);
/* HtMineWindow at 0006:37ac; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_mine_HtMineWindow(STARS_CV_0073 hwnd, STARS_CV_0072 x, STARS_CV_0072 y);
/* MineClick at 0006:3b4e; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_mine_MineClick(STARS_CV_0072 x, STARS_CV_0072 y, STARS_CV_0072 msg, STARS_CV_0072 sks);
/* SetMineralTitleBar at 0006:47dc; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_mine_SetMineralTitleBar(STARS_CV_0073 hwnd);
/* DrawSelectionArrow at 0006:4a68; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_mine_DrawSelectionArrow(STARS_CV_0073 hdc, STARS_CV_101b prc, STARS_CV_0072 fEnabled);
/* DrawDiamond at 0006:4b60; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_mine_DrawDiamond(STARS_CV_0073 hdc, STARS_CV_101b prc, STARS_CV_0073 hbr);
/* FOtherStuffAtScanSel at 0006:4d6e; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_mine_FOtherStuffAtScanSel(void);
/* PopupMineralScanChoices at 0006:4ecc; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_mine_PopupMineralScanChoices(STARS_CV_0073 hwnd, STARS_CV_0072 x, STARS_CV_0072 y);
/* EstMineralsMined at 0006:5362; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_mine_EstMineralsMined(STARS_CV_100b lppl, STARS_CV_0112 plQuan, STARS_CV_0012 cMines, STARS_CV_0072 fApply);
/* InitMDIApp at 0005:0000; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_mdi_InitMDIApp(void);
/* CreateChildWindows at 0005:038c; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_mdi_CreateChildWindows(void);
/* FrameWndProc at 0005:06d6; global */
STARS_CV_0012 STARS_FAR STARS_PASCAL stars_mdi_FrameWndProc(STARS_CV_0073 hwnd, STARS_CV_0073 msg, STARS_CV_0073 wParam, STARS_CV_0012 lParam);
/* InvertPaneBorder at 0005:1e3c; global */
STARS_CV_1018 STARS_FAR STARS_CDECL stars_mdi_InvertPaneBorder(STARS_CV_0073 hdc, STARS_CV_0072 grSel, STARS_CV_1018 dpt, STARS_CV_1020 pdptPrev);
/* HcrsFromFrameWindowPt at 0005:24b0; global */
STARS_CV_0073 STARS_FAR STARS_CDECL stars_mdi_HcrsFromFrameWindowPt(STARS_CV_1018 pt, STARS_CV_0172 pgrSel);
/* RestoreSelection at 0005:2614; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_mdi_RestoreSelection(void);
/* FormatSerialAndEnv at 0005:2886; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_mdi_FormatSerialAndEnv(STARS_CV_0012 lSerial, STARS_CV_0120 pbEnv, STARS_CV_0170 pszOut);
/* FSerialAndEnvFromSz at 0005:2aec; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_mdi_FSerialAndEnvFromSz(STARS_CV_0112 plSerial, STARS_CV_0120 pbEnv, STARS_CV_0170 pszIn);
/* FFindSomethingAndSelectIt at 0005:2e1c; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_mdi_FFindSomethingAndSelectIt(void);
/* CommandHandler at 0005:2f7a; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_mdi_CommandHandler(STARS_CV_0073 hwnd, STARS_CV_0021 wParam);
/* InitializeMenu at 0005:5376; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_mdi_InitializeMenu(STARS_CV_0073 hmenu);
/* EnsureAis at 0005:56bc; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_mdi_EnsureAis(void);
/* GetASubMenu at 0005:589a; global */
STARS_CV_0073 STARS_FAR STARS_CDECL stars_mdi_GetASubMenu(STARS_CV_0073 hwnd, STARS_CV_0072 iMenu);
/* FOpenGame at 0005:58f4; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_mdi_FOpenGame(STARS_CV_0073 hwnd, STARS_CV_0072 fRaceOnly);
/* FWasRaceFile at 0005:5da8; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_mdi_FWasRaceFile(STARS_CV_0170 szFile, STARS_CV_0072 fChkPass);
/* BringUpHostDlg at 0005:5ffc; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_mdi_BringUpHostDlg(void);
/* DrawHostDialog2 at 0005:6240; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_mdi_DrawHostDialog2(STARS_CV_0073 hwnd, STARS_CV_0073 hdcIn);
/* VerifyTurns at 0005:6686; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_mdi_VerifyTurns(void);
/* CTurnsOutSafe at 0005:6996; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_mdi_CTurnsOutSafe(void);
/* CFindTurnsOutstanding at 0005:6a26; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_mdi_CFindTurnsOutstanding(void);
/* HostModeDialog at 0005:6c16; global */
STARS_CV_0072 STARS_FAR STARS_PASCAL stars_mdi_HostModeDialog(STARS_CV_0073 hwnd, STARS_CV_0073 message, STARS_CV_0021 wParam, STARS_CV_0012 lParam);
/* HostOptionsDialog at 0005:75ce; global */
STARS_CV_0072 STARS_FAR STARS_PASCAL stars_mdi_HostOptionsDialog(STARS_CV_0073 hwnd, STARS_CV_0073 message, STARS_CV_0021 wParam, STARS_CV_0012 lParam);
/* DrawHostOptions at 0005:7706; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_mdi_DrawHostOptions(STARS_CV_0073 hwnd, STARS_CV_0073 hdc, STARS_CV_0072 iDraw);
/* HostTimerProc at 0005:7716; global */
STARS_CV_0003 STARS_FAR STARS_PASCAL stars_mdi_HostTimerProc(STARS_CV_0073 hwnd, STARS_CV_0073 msg, STARS_CV_0073 idTimer, STARS_CV_0022 dwTime);
/* GetWindowRc at 0005:79ac; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_mdi_GetWindowRc(STARS_CV_0073 hwnd, STARS_CV_101b prc);
/* SetWindowIniString at 0005:79f6; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_mdi_SetWindowIniString(STARS_CV_0170 sz, STARS_CV_0073 hwnd);
/* WriteIniSettings at 0005:7a76; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_mdi_WriteIniSettings(void);
/* RefitFrameChildren at 0005:8c88; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_mdi_RefitFrameChildren(void);
/* TitleWndProc at 0005:9126; global */
STARS_CV_0012 STARS_FAR STARS_PASCAL stars_mdi_TitleWndProc(STARS_CV_0073 hwnd, STARS_CV_0073 msg, STARS_CV_0073 wParam, STARS_CV_0012 lParam);
/* WinMain at 0004:0000; global */
STARS_CV_0072 STARS_FAR STARS_PASCAL stars_stars_WinMain(STARS_CV_0073 hInstance, STARS_CV_0073 hPrevInstance, STARS_CV_0270 lpCmdLine, STARS_CV_0072 nCmdShow);
/* FSetUpBatchProcessing at 0004:06a4; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_stars_FSetUpBatchProcessing(void);
/* IPlrAlsoCheater at 0004:07aa; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_stars_IPlrAlsoCheater(STARS_CV_0072 iplr);
/* FGetSystemColors at 0004:08d2; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_stars_FGetSystemColors(void);
/* FreeStuff at 0004:0bae; global */
STARS_CV_0003 STARS_FAR STARS_CDECL stars_stars_FreeStuff(void);
/* SzVersion at 0004:1212; global */
STARS_CV_0170 STARS_FAR STARS_CDECL stars_stars_SzVersion(void);
/* About at 0004:1252; global */
STARS_CV_0072 STARS_FAR STARS_PASCAL stars_stars_About(STARS_CV_0073 hwnd, STARS_CV_0073 message, STARS_CV_0021 wParam, STARS_CV_0012 lParam);
/* OrderInfoDlg at 0004:151e; global */
STARS_CV_0072 STARS_FAR STARS_PASCAL stars_stars_OrderInfoDlg(STARS_CV_0073 hwnd, STARS_CV_0073 message, STARS_CV_0021 wParam, STARS_CV_0012 lParam);
/* FHandleChar at 0004:15de; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_stars_FHandleChar(STARS_CV_0073 hwnd, STARS_CV_0021 ch, STARS_CV_0012 lParam);
/* FHandleKey at 0004:165a; global */
STARS_CV_0072 STARS_FAR STARS_CDECL stars_stars_FHandleKey(STARS_CV_0073 hwnd, STARS_CV_0072 iMsg, STARS_CV_0072 iKey, STARS_CV_0022 dw);
#endif
