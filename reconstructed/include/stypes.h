/* Generated; see analysis/debug/README.md. Storage views, not recovered original declarations. */
#ifndef STARS_RECOVERED_TYPES_H
#define STARS_RECOVERED_TYPES_H
#include <stddef.h>
#ifdef STARS_NATIVE16
#define STARS_NEAR __near
#define STARS_FAR __far
#define STARS_HUGE __huge
#define STARS_CDECL __cdecl
#define STARS_PASCAL __pascal
#else
#define STARS_NEAR
#define STARS_FAR
#define STARS_HUGE
#define STARS_CDECL
#define STARS_PASCAL
#endif
#define STARS_CHECK(name,expr) typedef char stars_check_##name[(expr)?1:-1]
#pragma pack(1)
typedef struct { unsigned char bytes[10]; } STARS_REAL80; /* encoded x87 extended, not host arithmetic */
#ifdef STARS_NATIVE16
typedef signed long STARS_LONG32;
typedef unsigned long STARS_ULONG32;
#else
typedef signed int STARS_LONG32;
typedef unsigned int STARS_ULONG32;
#endif
typedef void STARS_CV_0000;
typedef void STARS_CV_0001;
typedef void STARS_CV_0003;
typedef signed short STARS_CV_0011;
STARS_CHECK(p0011,sizeof(STARS_CV_0011)==2);
typedef STARS_LONG32 STARS_CV_0012;
STARS_CHECK(p0012,sizeof(STARS_CV_0012)==4);
typedef unsigned char STARS_CV_0020;
STARS_CHECK(p0020,sizeof(STARS_CV_0020)==1);
typedef unsigned short STARS_CV_0021;
STARS_CHECK(p0021,sizeof(STARS_CV_0021)==2);
typedef STARS_ULONG32 STARS_CV_0022;
STARS_CHECK(p0022,sizeof(STARS_CV_0022)==4);
typedef float STARS_CV_0040;
STARS_CHECK(p0040,sizeof(STARS_CV_0040)==4);
typedef double STARS_CV_0041;
STARS_CHECK(p0041,sizeof(STARS_CV_0041)==8);
typedef STARS_REAL80 STARS_CV_0042;
STARS_CHECK(p0042,sizeof(STARS_CV_0042)==10);
typedef char STARS_CV_0070;
STARS_CHECK(p0070,sizeof(STARS_CV_0070)==1);
typedef signed short STARS_CV_0072;
STARS_CHECK(p0072,sizeof(STARS_CV_0072)==2);
typedef unsigned short STARS_CV_0073;
STARS_CHECK(p0073,sizeof(STARS_CV_0073)==2);
#ifdef STARS_NATIVE16
typedef STARS_CV_0003 STARS_NEAR *STARS_CV_0103;
#else
typedef unsigned short STARS_CV_0103;
#endif
STARS_CHECK(p0103,sizeof(STARS_CV_0103)==2);
#ifdef STARS_NATIVE16
typedef STARS_CV_0012 STARS_NEAR *STARS_CV_0112;
#else
typedef unsigned short STARS_CV_0112;
#endif
STARS_CHECK(p0112,sizeof(STARS_CV_0112)==2);
#ifdef STARS_NATIVE16
typedef STARS_CV_0020 STARS_NEAR *STARS_CV_0120;
#else
typedef unsigned short STARS_CV_0120;
#endif
STARS_CHECK(p0120,sizeof(STARS_CV_0120)==2);
#ifdef STARS_NATIVE16
typedef STARS_CV_0021 STARS_NEAR *STARS_CV_0121;
#else
typedef unsigned short STARS_CV_0121;
#endif
STARS_CHECK(p0121,sizeof(STARS_CV_0121)==2);
#ifdef STARS_NATIVE16
typedef STARS_CV_0022 STARS_NEAR *STARS_CV_0122;
#else
typedef unsigned short STARS_CV_0122;
#endif
STARS_CHECK(p0122,sizeof(STARS_CV_0122)==2);
#ifdef STARS_NATIVE16
typedef STARS_CV_0040 STARS_NEAR *STARS_CV_0140;
#else
typedef unsigned short STARS_CV_0140;
#endif
STARS_CHECK(p0140,sizeof(STARS_CV_0140)==2);
#ifdef STARS_NATIVE16
typedef STARS_CV_0070 STARS_NEAR *STARS_CV_0170;
#else
typedef unsigned short STARS_CV_0170;
#endif
STARS_CHECK(p0170,sizeof(STARS_CV_0170)==2);
#ifdef STARS_NATIVE16
typedef STARS_CV_0072 STARS_NEAR *STARS_CV_0172;
#else
typedef unsigned short STARS_CV_0172;
#endif
STARS_CHECK(p0172,sizeof(STARS_CV_0172)==2);
#ifdef STARS_NATIVE16
typedef STARS_CV_0073 STARS_NEAR *STARS_CV_0173;
#else
typedef unsigned short STARS_CV_0173;
#endif
STARS_CHECK(p0173,sizeof(STARS_CV_0173)==2);
#ifdef STARS_NATIVE16
typedef STARS_CV_0003 STARS_FAR *STARS_CV_0203;
#else
typedef STARS_ULONG32 STARS_CV_0203;
#endif
STARS_CHECK(p0203,sizeof(STARS_CV_0203)==4);
#ifdef STARS_NATIVE16
typedef STARS_CV_0011 STARS_FAR *STARS_CV_0211;
#else
typedef STARS_ULONG32 STARS_CV_0211;
#endif
STARS_CHECK(p0211,sizeof(STARS_CV_0211)==4);
#ifdef STARS_NATIVE16
typedef STARS_CV_0012 STARS_FAR *STARS_CV_0212;
#else
typedef STARS_ULONG32 STARS_CV_0212;
#endif
STARS_CHECK(p0212,sizeof(STARS_CV_0212)==4);
#ifdef STARS_NATIVE16
typedef STARS_CV_0020 STARS_FAR *STARS_CV_0220;
#else
typedef STARS_ULONG32 STARS_CV_0220;
#endif
STARS_CHECK(p0220,sizeof(STARS_CV_0220)==4);
#ifdef STARS_NATIVE16
typedef STARS_CV_0021 STARS_FAR *STARS_CV_0221;
#else
typedef STARS_ULONG32 STARS_CV_0221;
#endif
STARS_CHECK(p0221,sizeof(STARS_CV_0221)==4);
#ifdef STARS_NATIVE16
typedef STARS_CV_0022 STARS_FAR *STARS_CV_0222;
#else
typedef STARS_ULONG32 STARS_CV_0222;
#endif
STARS_CHECK(p0222,sizeof(STARS_CV_0222)==4);
#ifdef STARS_NATIVE16
typedef STARS_CV_0070 STARS_FAR *STARS_CV_0270;
#else
typedef STARS_ULONG32 STARS_CV_0270;
#endif
STARS_CHECK(p0270,sizeof(STARS_CV_0270)==4);
#ifdef STARS_NATIVE16
typedef STARS_CV_0072 STARS_FAR *STARS_CV_0272;
#else
typedef STARS_ULONG32 STARS_CV_0272;
#endif
STARS_CHECK(p0272,sizeof(STARS_CV_0272)==4);
#ifdef STARS_NATIVE16
typedef STARS_CV_0073 STARS_FAR *STARS_CV_0273;
#else
typedef STARS_ULONG32 STARS_CV_0273;
#endif
STARS_CHECK(p0273,sizeof(STARS_CV_0273)==4);
#ifdef STARS_NATIVE16
typedef STARS_CV_0070 STARS_HUGE *STARS_CV_0370;
#else
typedef STARS_ULONG32 STARS_CV_0370;
#endif
STARS_CHECK(p0370,sizeof(STARS_CV_0370)==4);
typedef union STARS_CV_1002 STARS_CV_1002; /* _planet */
typedef union STARS_CV_1003 STARS_CV_1003; /* _fleet */
typedef union STARS_CV_1005 STARS_CV_1005; /* _order */
typedef union STARS_CV_1006 STARS_CV_1006; /* _thing */
typedef union STARS_CV_1008 STARS_CV_1008; /* _shdef */
typedef union STARS_CV_1012 STARS_CV_1012; /* _game */
typedef union STARS_CV_1013 STARS_CV_1013; /* _gdata */
typedef union STARS_CV_1014 STARS_CV_1014; /* _player */
typedef union STARS_CV_1018 STARS_CV_1018; /* tagPOINT */
typedef union STARS_CV_1019 STARS_CV_1019; /* tagPAINTSTRUCT */
typedef union STARS_CV_101a STARS_CV_101a; /* tagRECT */
typedef union STARS_CV_1038 STARS_CV_1038; /* _framestuff */
typedef union STARS_CV_1039 STARS_CV_1039; /* _prod */
typedef union STARS_CV_103e STARS_CV_103e; /* _cyberinfo */
typedef union STARS_CV_1040 STARS_CV_1040; /* _cyberinfotemp */
typedef union STARS_CV_1047 STARS_CV_1047; /* _scan */
typedef union STARS_CV_1054 STARS_CV_1054; /* PLPROD */
typedef union STARS_CV_105b STARS_CV_105b; /* _sel */
typedef union STARS_CV_105e STARS_CV_105e; /* _part */
typedef union STARS_CV_1076 STARS_CV_1076; /* _hul */
typedef union STARS_CV_1078 STARS_CV_1078; /* _hs */
typedef union STARS_CV_1085 STARS_CV_1085; /* _ziporder */
typedef union STARS_CV_109a STARS_CV_109a; /* _scorex */
typedef union STARS_CV_10a2 STARS_CV_10a2; /* _btldata */
typedef union STARS_CV_10b4 STARS_CV_10b4; /* _rpt */
typedef union STARS_CV_10be STARS_CV_10be; /* _btlplan */
typedef union STARS_CV_10c3 STARS_CV_10c3; /* _popupdata */
typedef union STARS_CV_10cb STARS_CV_10cb; /* _itemaction */
typedef union STARS_CV_10e9 STARS_CV_10e9; /* _tok */
typedef union STARS_CV_10ec STARS_CV_10ec; /* _ini */
typedef union STARS_CV_10ed STARS_CV_10ed; /* _tutor */
typedef union STARS_CV_10ef STARS_CV_10ef; /* _zipprodq1 */
typedef union STARS_CV_10f1 STARS_CV_10f1; /* _zipprodq */
typedef union STARS_CV_1140 STARS_CV_1140; /* _aipart */
typedef union STARS_CV_114b STARS_CV_114b; /* tagTIMERINFO */
typedef union STARS_CV_114f STARS_CV_114f; /* _hb */
typedef union STARS_CV_1156 STARS_CV_1156; /* _btlrec */
typedef union STARS_CV_1158 STARS_CV_1158; /* _dv */
typedef union STARS_CV_115c STARS_CV_115c; /* _btnt */
typedef union STARS_CV_116e STARS_CV_116e; /* _find_t */
typedef union STARS_CV_1170 STARS_CV_1170; /* _diskfree_t */
typedef union STARS_CV_117d STARS_CV_117d; /* tagOFSTRUCT */
typedef union STARS_CV_1197 STARS_CV_1197; /* tagLOGFONT */
typedef union STARS_CV_11ab STARS_CV_11ab; /* tagMSG */
typedef union STARS_CV_11b9 STARS_CV_11b9; /* tagBITMAPCOREHEADER */
typedef union STARS_CV_11bb STARS_CV_11bb; /* tagBITMAPINFOHEADER */
typedef union STARS_CV_11be STARS_CV_11be; /* tagLOGPALETTE */
typedef union STARS_CV_11c5 STARS_CV_11c5; /* tagBITMAP */
typedef union STARS_CV_11db STARS_CV_11db; /* tagTEXTMETRIC */
typedef union STARS_CV_1206 STARS_CV_1206; /* tagOFN */
typedef union STARS_CV_121c STARS_CV_121c; /* _engine */
typedef union STARS_CV_125c STARS_CV_125c; /* _pl */
typedef union STARS_CV_1277 STARS_CV_1277; /* tagDRAWITEMSTRUCT */
typedef union STARS_CV_1279 STARS_CV_1279; /* tagMEASUREITEMSTRUCT */
typedef union STARS_CV_1289 STARS_CV_1289; /* _huldef */
typedef union STARS_CV_128d STARS_CV_128d; /* _scanner */
typedef union STARS_CV_1291 STARS_CV_1291; /* _planetary */
typedef union STARS_CV_1299 STARS_CV_1299; /* _armor */
typedef union STARS_CV_129c STARS_CV_129c; /* _shield */
typedef union STARS_CV_129e STARS_CV_129e; /* _special */
typedef union STARS_CV_12a1 STARS_CV_12a1; /* _mines */
typedef union STARS_CV_12a3 STARS_CV_12a3; /* _mining */
typedef union STARS_CV_12a6 STARS_CV_12a6; /* _terra */
typedef union STARS_CV_12a8 STARS_CV_12a8; /* _bomb */
typedef union STARS_CV_12aa STARS_CV_12aa; /* _torp */
typedef union STARS_CV_12ac STARS_CV_12ac; /* _beam */
typedef union STARS_CV_12b2 STARS_CV_12b2; /* _specialsb */
typedef union STARS_CV_12d4 STARS_CV_12d4; /* _msgplr */
typedef union STARS_CV_12d6 STARS_CV_12d6; /* _msgbig */
typedef union STARS_CV_12de STARS_CV_12de; /* _msgturn */
typedef union STARS_CV_12e0 STARS_CV_12e0; /* _msghdr */
typedef union STARS_CV_12ec STARS_CV_12ec; /* _hdr */
typedef union STARS_CV_12f6 STARS_CV_12f6; /* _starpack */
typedef union STARS_CV_1323 STARS_CV_1323; /* _rtbof */
typedef union STARS_CV_132e STARS_CV_132e; /* _xfer */
typedef union STARS_CV_1330 STARS_CV_1330; /* _xferfull */
typedef union STARS_CV_1332 STARS_CV_1332; /* _coldrop */
typedef union STARS_CV_1336 STARS_CV_1336; /* _score */
typedef union STARS_CV_133f STARS_CV_133f; /* _turnserial */
typedef union STARS_CV_1342 STARS_CV_1342; /* _rthisthdr */
typedef union STARS_CV_1345 STARS_CV_1345; /* _rtChgProdQ */
typedef union STARS_CV_1349 STARS_CV_1349; /* _prodq1 */
typedef union STARS_CV_134b STARS_CV_134b; /* tagEVENTMSG */
typedef union STARS_CV_134e STARS_CV_134e; /* _rtlogthing */
typedef union STARS_CV_1354 STARS_CV_1354; /* tagFINDREPLACE */
typedef union STARS_CV_135b STARS_CV_135b; /* _rtChgPlanetLong */
typedef union STARS_CV_1366 STARS_CV_1366; /* _planetsome */
typedef union STARS_CV_1368 STARS_CV_1368; /* tagRGBTRIPLE */
typedef union STARS_CV_136b STARS_CV_136b; /* tagBITMAPCOREINFO */
typedef union STARS_CV_136f STARS_CV_136f; /* _exceptionl */
typedef union STARS_CV_1377 STARS_CV_1377; /* tagGLYPHMETRICS */
typedef union STARS_CV_137f STARS_CV_137f; /* _taskxport */
typedef union STARS_CV_1381 STARS_CV_1381; /* _tasklaymines */
typedef union STARS_CV_1383 STARS_CV_1383; /* _taskpatrol */
typedef union STARS_CV_1385 STARS_CV_1385; /* _tasksell */
typedef union STARS_CV_139a STARS_CV_139a; /* _complexl */
typedef union STARS_CV_139d STARS_CV_139d; /* tagBITMAPFILEHEADER */
typedef union STARS_CV_13a4 STARS_CV_13a4; /* tagCLIENTCREATESTRUCT */
typedef union STARS_CV_13a7 STARS_CV_13a7; /* tagWNDCLASS */
typedef union STARS_CV_13ac STARS_CV_13ac; /* tagMETAFILEPICT */
typedef union STARS_CV_13ae STARS_CV_13ae; /* tagDEBUGHOOKINFO */
typedef union STARS_CV_13b2 STARS_CV_13b2; /* _obj */
typedef union STARS_CV_13b6 STARS_CV_13b6; /* _tile */
typedef union STARS_CV_13df STARS_CV_13df; /* HELPWININFO */
typedef union STARS_CV_13e2 STARS_CV_13e2; /* MENUITEMTEMPLATE */
typedef union STARS_CV_13e4 STARS_CV_13e4; /* MENUITEMTEMPLATEHEADER */
typedef union STARS_CV_13e6 STARS_CV_13e6; /* DOCINFO */
typedef union STARS_CV_13e8 STARS_CV_13e8; /* tagDEVNAMES */
typedef union STARS_CV_13ea STARS_CV_13ea; /* tagDRVCONFIGINFO */
typedef union STARS_CV_13ec STARS_CV_13ec; /* _logxfer */
typedef union STARS_CV_13ef STARS_CV_13ef; /* _btn */
typedef union STARS_CV_13f3 STARS_CV_13f3; /* _kill */
typedef union STARS_CV_13fb STARS_CV_13fb; /* _logxferf */
typedef union STARS_CV_13fd STARS_CV_13fd; /* _fleetsome */
typedef union STARS_CV_1400 STARS_CV_1400; /* tagMETAHEADER */
typedef union STARS_CV_1402 STARS_CV_1402; /* tagWINDEBUGINFO */
typedef union STARS_CV_1407 STARS_CV_1407; /* tagLOGPEN */
typedef union STARS_CV_140c STARS_CV_140c; /* _rtxfer */
typedef union STARS_CV_140e STARS_CV_140e; /* tagFIXED */
typedef union STARS_CV_1415 STARS_CV_1415; /* _compart */
typedef union STARS_CV_141d STARS_CV_141d; /* tagDCB */
typedef union STARS_CV_1421 STARS_CV_1421; /* _rtxferx */
typedef union STARS_CV_1429 STARS_CV_1429; /* _thmine */
typedef union STARS_CV_142b STARS_CV_142b; /* tagCOMSTAT */
typedef union STARS_CV_142d STARS_CV_142d; /* tagMAT2 */
typedef union STARS_CV_1431 STARS_CV_1431; /* _rtxferl */
typedef union STARS_CV_1436 STARS_CV_1436; /* _rtxferf */
typedef union STARS_CV_1438 STARS_CV_1438; /* _btlrec26 */
typedef union STARS_CV_143c STARS_CV_143c; /* tagRGBQUAD */
typedef union STARS_CV_1441 STARS_CV_1441; /* _thpack */
typedef union STARS_CV_1443 STARS_CV_1443; /* _rtshipint */
typedef union STARS_CV_1446 STARS_CV_1446; /* _aistarbase */
typedef union STARS_CV_1448 STARS_CV_1448; /* _exception */
typedef union STARS_CV_144b STARS_CV_144b; /* tagHANDLETABLE */
typedef union STARS_CV_144e STARS_CV_144e; /* _sbar */
typedef union STARS_CV_1451 STARS_CV_1451; /* _rtshipint2 */
typedef union STARS_CV_1454 STARS_CV_1454; /* tagWINDOWPOS */
typedef union STARS_CV_1457 STARS_CV_1457; /* tagCREATESTRUCT */
typedef union STARS_CV_1459 STARS_CV_1459; /* tagPD */
typedef union STARS_CV_145d STARS_CV_145d; /* _thworm */
typedef union STARS_CV_1462 STARS_CV_1462; /* _aihist */
typedef union STARS_CV_1468 STARS_CV_1468; /* tagCHOOSEFONT */
typedef union STARS_CV_146a STARS_CV_146a; /* _rtwaypt */
typedef union STARS_CV_1477 STARS_CV_1477; /* _drawcir */
typedef union STARS_CV_1479 STARS_CV_1479; /* tagMETARECORD */
typedef union STARS_CV_147b STARS_CV_147b; /* tagMDICREATESTRUCT */
typedef union STARS_CV_1482 STARS_CV_1482; /* _timer */
typedef union STARS_CV_1484 STARS_CV_1484; /* _complex */
typedef union STARS_CV_1486 STARS_CV_1486; /* _rtchgname */
typedef union STARS_CV_1489 STARS_CV_1489; /* complex */
typedef union STARS_CV_148b STARS_CV_148b; /* _lsb */
typedef union STARS_CV_148d STARS_CV_148d; /* tagPOINTFX */
typedef union STARS_CV_148f STARS_CV_148f; /* tagPANOSE */
typedef union STARS_CV_1491 STARS_CV_1491; /* tagOUTLINETEXTMETRIC */
typedef union STARS_CV_1496 STARS_CV_1496; /* _thtrader */
typedef union STARS_CV_149a STARS_CV_149a; /* tagCBT_CREATEWND */
typedef union STARS_CV_149f STARS_CV_149f; /* tagMOUSEHOOKSTRUCT */
typedef union STARS_CV_14a6 STARS_CV_14a6; /* tagTTPOLYCURVE */
typedef union STARS_CV_14b2 STARS_CV_14b2; /* tagNEWTEXTMETRIC */
typedef union STARS_CV_14b5 STARS_CV_14b5; /* tagWINDOWPLACEMENT */
typedef union STARS_CV_14b7 STARS_CV_14b7; /* tagCBTACTIVATESTRUCT */
typedef union STARS_CV_14c8 STARS_CV_14c8; /* tagTTPOLYGONHEADER */
typedef union STARS_CV_14ca STARS_CV_14ca; /* tagPALETTEENTRY */
typedef union STARS_CV_14cf STARS_CV_14cf; /* _rtshdef */
typedef union STARS_CV_14d9 STARS_CV_14d9; /* _rtchgshdef */
typedef union STARS_CV_14db STARS_CV_14db; /* tagDELETEITEMSTRUCT */
typedef union STARS_CV_14dd STARS_CV_14dd; /* tagHARDWAREHOOKSTRUCT */
typedef union STARS_CV_14e2 STARS_CV_14e2; /* _rtplanet */
typedef union STARS_CV_14e5 STARS_CV_14e5; /* tagDRIVERINFOSTRUCT */
typedef union STARS_CV_14ea STARS_CV_14ea; /* tagCHOOSECOLOR */
typedef union STARS_CV_14f0 STARS_CV_14f0; /* tagABC */
typedef union STARS_CV_14f5 STARS_CV_14f5; /* tagLOGBRUSH */
typedef union STARS_CV_14f8 STARS_CV_14f8; /* tagSIZE */
typedef union STARS_CV_14fc STARS_CV_14fc; /* _fleetid */
typedef union STARS_CV_14ff STARS_CV_14ff; /* tagMULTIKEYHELP */
typedef union STARS_CV_1502 STARS_CV_1502; /* _vers */
typedef union STARS_CV_150a STARS_CV_150a; /* tagNCCALCSIZE_PARAMS */
typedef union STARS_CV_150f STARS_CV_150f; /* tagCOMPAREITEMSTRUCT */
typedef union STARS_CV_1511 STARS_CV_1511; /* tagMINMAXINFO */
typedef union STARS_CV_1513 STARS_CV_1513; /* tagSEGINFO */
typedef union STARS_CV_1518 STARS_CV_1518; /* tagKERNINGPAIR */
typedef union STARS_CV_151f STARS_CV_151f; /* tagENUMLOGFONT */
typedef union STARS_CV_1528 STARS_CV_1528; /* PLORD */
typedef union STARS_CV_152e STARS_CV_152e; /* _selSome */
typedef union STARS_CV_1532 STARS_CV_1532; /* _planetminimal */
typedef union STARS_CV_1535 STARS_CV_1535; /* _wn */
typedef union STARS_CV_1546 STARS_CV_1546; /* tagBITMAPINFO */
typedef union STARS_CV_154d STARS_CV_154d; /* tagRASTERIZER_STATUS */
typedef union STARS_CV_1550 STARS_CV_1550; /* _rtloghdr */
typedef union STARS_CV_1554 STARS_CV_1554; /* _mdplr */
typedef union STARS_CV_156a STARS_CV_156a; /* _div_t */
typedef union STARS_CV_156d STARS_CV_156d; /* _ldiv_t */
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_1001(STARS_CV_0072);
typedef STARS_CV_0021 STARS_CV_135d; /* bit-field backing storage; use recorded bit positions/masks */
typedef STARS_CV_0021 STARS_CV_135e; /* bit-field backing storage; use recorded bit positions/masks */
typedef STARS_CV_0021 STARS_CV_135f; /* bit-field backing storage; use recorded bit positions/masks */
typedef STARS_CV_0021 STARS_CV_1360; /* bit-field backing storage; use recorded bit positions/masks */
typedef STARS_CV_0021 STARS_CV_1361; /* bit-field backing storage; use recorded bit positions/masks */
typedef STARS_CV_0021 STARS_CV_137b; /* bit-field backing storage; use recorded bit positions/masks */
typedef STARS_CV_0021 STARS_CV_137c; /* bit-field backing storage; use recorded bit positions/masks */
typedef STARS_CV_0021 STARS_CV_137d; /* bit-field backing storage; use recorded bit positions/masks */
typedef STARS_CV_0020 STARS_CV_1388[3];
typedef STARS_CV_0070 STARS_CV_10bc[3];
typedef STARS_CV_0021 STARS_CV_1364; /* bit-field backing storage; use recorded bit positions/masks */
typedef STARS_CV_0021 STARS_CV_1362; /* bit-field backing storage; use recorded bit positions/masks */
typedef STARS_CV_0022 STARS_CV_1389; /* bit-field backing storage; use recorded bit positions/masks */
typedef STARS_CV_0022 STARS_CV_138a; /* bit-field backing storage; use recorded bit positions/masks */
typedef STARS_CV_0022 STARS_CV_138b; /* bit-field backing storage; use recorded bit positions/masks */
typedef STARS_CV_0022 STARS_CV_138c; /* bit-field backing storage; use recorded bit positions/masks */
typedef STARS_CV_0022 STARS_CV_138d; /* bit-field backing storage; use recorded bit positions/masks */
typedef STARS_CV_0022 STARS_CV_138e; /* bit-field backing storage; use recorded bit positions/masks */
typedef STARS_CV_0022 STARS_CV_138f; /* bit-field backing storage; use recorded bit positions/masks */
typedef STARS_CV_0022 STARS_CV_1390; /* bit-field backing storage; use recorded bit positions/masks */
typedef STARS_CV_0022 STARS_CV_1391; /* bit-field backing storage; use recorded bit positions/masks */
typedef STARS_CV_0020 STARS_CV_1062[8];
typedef STARS_CV_0012 STARS_CV_100c[4];
typedef STARS_CV_0021 STARS_CV_1378; /* bit-field backing storage; use recorded bit positions/masks */
typedef STARS_CV_0021 STARS_CV_1392; /* bit-field backing storage; use recorded bit positions/masks */
typedef STARS_CV_0021 STARS_CV_1393; /* bit-field backing storage; use recorded bit positions/masks */
typedef STARS_CV_0021 STARS_CV_1394; /* bit-field backing storage; use recorded bit positions/masks */
typedef STARS_CV_0021 STARS_CV_1395; /* bit-field backing storage; use recorded bit positions/masks */
typedef STARS_CV_0021 STARS_CV_1396; /* bit-field backing storage; use recorded bit positions/masks */
typedef STARS_CV_0021 STARS_CV_1397; /* bit-field backing storage; use recorded bit positions/masks */
#ifdef STARS_NATIVE16
typedef STARS_CV_1054 STARS_FAR *STARS_CV_1055;
#else
typedef STARS_ULONG32 STARS_CV_1055;
#endif
/* _planet: 56 bytes; each view begins at its recorded offset. */
union STARS_CV_1002 {
    unsigned char raw[56];
    struct { STARS_CV_0011 value; } id;
    struct { unsigned char _pad[2]; STARS_CV_0011 value; } iPlayer;
    struct { unsigned char _pad[4]; STARS_CV_135d value; } det;
    struct { unsigned char _pad[4]; STARS_CV_135e value; } fInclude;
    struct { unsigned char _pad[4]; STARS_CV_135f value; } fStarbase;
    struct { unsigned char _pad[4]; STARS_CV_1360 value; } fHomeworld;
    struct { unsigned char _pad[4]; STARS_CV_1361 value; } fFirstYear;
    struct { unsigned char _pad[4]; STARS_CV_137b value; } unusedC;
    struct { unsigned char _pad[4]; STARS_CV_137c value; } fWasInhabited;
    struct { unsigned char _pad[4]; STARS_CV_137d value; } unusedD;
    struct { unsigned char _pad[6]; STARS_CV_1388 value; } rgpctMinLevel;
    struct { unsigned char _pad[9]; STARS_CV_1388 value; } rgMinConc;
    struct { unsigned char _pad[12]; STARS_CV_10bc value; } rgEnvVar;
    struct { unsigned char _pad[15]; STARS_CV_10bc value; } rgEnvVarOrig;
    struct { unsigned char _pad[18]; STARS_CV_1364 value; } uPopGuess;
    struct { unsigned char _pad[18]; STARS_CV_1362 value; } uDefGuess;
    struct { unsigned char _pad[18]; STARS_CV_0021 value; } uGuesses;
    struct { unsigned char _pad[20]; STARS_CV_1389 value; } iDeltaPop;
    struct { unsigned char _pad[20]; STARS_CV_138a value; } cMines;
    struct { unsigned char _pad[20]; STARS_CV_138b value; } cFactories;
    struct { unsigned char _pad[24]; STARS_CV_138c value; } cDefenses;
    struct { unsigned char _pad[24]; STARS_CV_138d value; } iScanner;
    struct { unsigned char _pad[24]; STARS_CV_138e value; } unused5;
    struct { unsigned char _pad[24]; STARS_CV_138f value; } fArtifact;
    struct { unsigned char _pad[24]; STARS_CV_1390 value; } fNoResearch;
    struct { unsigned char _pad[24]; STARS_CV_1391 value; } unused2;
    struct { unsigned char _pad[20]; STARS_CV_1062 value; } rgbImp;
    struct { unsigned char _pad[28]; STARS_CV_100c value; } rgwtMin;
    struct { unsigned char _pad[44]; STARS_CV_1378 value; } isb;
    struct { unsigned char _pad[44]; STARS_CV_1392 value; } pctDp;
    struct { unsigned char _pad[46]; STARS_CV_1393 value; } idFling;
    struct { unsigned char _pad[46]; STARS_CV_1394 value; } iWarpFling;
    struct { unsigned char _pad[46]; STARS_CV_1395 value; } fNoHeal;
    struct { unsigned char _pad[46]; STARS_CV_1396 value; } unused3;
    struct { unsigned char _pad[44]; STARS_CV_0012 value; } lStarbase;
    struct { unsigned char _pad[48]; STARS_CV_1393 value; } idRoute;
    struct { unsigned char _pad[48]; STARS_CV_1397 value; } unused4;
    struct { unsigned char _pad[48]; STARS_CV_0021 value; } wRouting;
    struct { unsigned char _pad[50]; STARS_CV_0011 value; } turn;
    struct { unsigned char _pad[52]; STARS_CV_1055 value; } lpplprod;
};
#define STARS_OFFSET_1002_id 0
#define STARS_OFFSET_1002_iPlayer 2
#define STARS_OFFSET_1002_det 4
#define STARS_BITSHIFT_1002_det 0
#define STARS_BITMASK_1002_det 0xffUL
#define STARS_OFFSET_1002_fInclude 4
#define STARS_BITSHIFT_1002_fInclude 8
#define STARS_BITMASK_1002_fInclude 0x1UL
#define STARS_OFFSET_1002_fStarbase 4
#define STARS_BITSHIFT_1002_fStarbase 9
#define STARS_BITMASK_1002_fStarbase 0x1UL
#define STARS_OFFSET_1002_fHomeworld 4
#define STARS_BITSHIFT_1002_fHomeworld 10
#define STARS_BITMASK_1002_fHomeworld 0x1UL
#define STARS_OFFSET_1002_fFirstYear 4
#define STARS_BITSHIFT_1002_fFirstYear 11
#define STARS_BITMASK_1002_fFirstYear 0x1UL
#define STARS_OFFSET_1002_unusedC 4
#define STARS_BITSHIFT_1002_unusedC 12
#define STARS_BITMASK_1002_unusedC 0x1UL
#define STARS_OFFSET_1002_fWasInhabited 4
#define STARS_BITSHIFT_1002_fWasInhabited 13
#define STARS_BITMASK_1002_fWasInhabited 0x1UL
#define STARS_OFFSET_1002_unusedD 4
#define STARS_BITSHIFT_1002_unusedD 14
#define STARS_BITMASK_1002_unusedD 0x3UL
#define STARS_OFFSET_1002_rgpctMinLevel 6
#define STARS_OFFSET_1002_rgMinConc 9
#define STARS_OFFSET_1002_rgEnvVar 12
#define STARS_OFFSET_1002_rgEnvVarOrig 15
#define STARS_OFFSET_1002_uPopGuess 18
#define STARS_BITSHIFT_1002_uPopGuess 0
#define STARS_BITMASK_1002_uPopGuess 0xfffUL
#define STARS_OFFSET_1002_uDefGuess 18
#define STARS_BITSHIFT_1002_uDefGuess 12
#define STARS_BITMASK_1002_uDefGuess 0xfUL
#define STARS_OFFSET_1002_uGuesses 18
#define STARS_OFFSET_1002_iDeltaPop 20
#define STARS_BITSHIFT_1002_iDeltaPop 0
#define STARS_BITMASK_1002_iDeltaPop 0xffUL
#define STARS_OFFSET_1002_cMines 20
#define STARS_BITSHIFT_1002_cMines 8
#define STARS_BITMASK_1002_cMines 0xfffUL
#define STARS_OFFSET_1002_cFactories 20
#define STARS_BITSHIFT_1002_cFactories 20
#define STARS_BITMASK_1002_cFactories 0xfffUL
#define STARS_OFFSET_1002_cDefenses 24
#define STARS_BITSHIFT_1002_cDefenses 0
#define STARS_BITMASK_1002_cDefenses 0xfffUL
#define STARS_OFFSET_1002_iScanner 24
#define STARS_BITSHIFT_1002_iScanner 12
#define STARS_BITMASK_1002_iScanner 0x1fUL
#define STARS_OFFSET_1002_unused5 24
#define STARS_BITSHIFT_1002_unused5 17
#define STARS_BITMASK_1002_unused5 0x1fUL
#define STARS_OFFSET_1002_fArtifact 24
#define STARS_BITSHIFT_1002_fArtifact 22
#define STARS_BITMASK_1002_fArtifact 0x1UL
#define STARS_OFFSET_1002_fNoResearch 24
#define STARS_BITSHIFT_1002_fNoResearch 23
#define STARS_BITMASK_1002_fNoResearch 0x1UL
#define STARS_OFFSET_1002_unused2 24
#define STARS_BITSHIFT_1002_unused2 24
#define STARS_BITMASK_1002_unused2 0xffUL
#define STARS_OFFSET_1002_rgbImp 20
#define STARS_OFFSET_1002_rgwtMin 28
#define STARS_OFFSET_1002_isb 44
#define STARS_BITSHIFT_1002_isb 0
#define STARS_BITMASK_1002_isb 0xfUL
#define STARS_OFFSET_1002_pctDp 44
#define STARS_BITSHIFT_1002_pctDp 4
#define STARS_BITMASK_1002_pctDp 0xfffUL
#define STARS_OFFSET_1002_idFling 46
#define STARS_BITSHIFT_1002_idFling 0
#define STARS_BITMASK_1002_idFling 0x3ffUL
#define STARS_OFFSET_1002_iWarpFling 46
#define STARS_BITSHIFT_1002_iWarpFling 10
#define STARS_BITMASK_1002_iWarpFling 0xfUL
#define STARS_OFFSET_1002_fNoHeal 46
#define STARS_BITSHIFT_1002_fNoHeal 14
#define STARS_BITMASK_1002_fNoHeal 0x1UL
#define STARS_OFFSET_1002_unused3 46
#define STARS_BITSHIFT_1002_unused3 15
#define STARS_BITMASK_1002_unused3 0x1UL
#define STARS_OFFSET_1002_lStarbase 44
#define STARS_OFFSET_1002_idRoute 48
#define STARS_BITSHIFT_1002_idRoute 0
#define STARS_BITMASK_1002_idRoute 0x3ffUL
#define STARS_OFFSET_1002_unused4 48
#define STARS_BITSHIFT_1002_unused4 10
#define STARS_BITMASK_1002_unused4 0x3fUL
#define STARS_OFFSET_1002_wRouting 48
#define STARS_OFFSET_1002_turn 50
#define STARS_OFFSET_1002_lpplprod 52
typedef STARS_CV_0021 STARS_CV_13d4; /* bit-field backing storage; use recorded bit positions/masks */
typedef STARS_CV_0021 STARS_CV_13cf; /* bit-field backing storage; use recorded bit positions/masks */
typedef STARS_CV_0021 STARS_CV_13d0; /* bit-field backing storage; use recorded bit positions/masks */
/* tagPOINT: 4 bytes; each view begins at its recorded offset. */
union STARS_CV_1018 {
    unsigned char raw[4];
    struct { STARS_CV_0072 value; } x;
    struct { unsigned char _pad[2]; STARS_CV_0072 value; } y;
};
#define STARS_OFFSET_1018_x 0
#define STARS_OFFSET_1018_y 2
typedef STARS_CV_0011 STARS_CV_13f9[16];
typedef STARS_CV_0021 STARS_CV_14e0; /* bit-field backing storage; use recorded bit positions/masks */
typedef STARS_CV_0021 STARS_CV_1514; /* bit-field backing storage; use recorded bit positions/masks */
/* _dv: 2 bytes; each view begins at its recorded offset. */
union STARS_CV_1158 {
    unsigned char raw[2];
    struct { STARS_CV_0021 value; } dp;
    struct { STARS_CV_14e0 value; } pctSh;
    struct { STARS_CV_1514 value; } pctDp;
};
#define STARS_OFFSET_1158_dp 0
#define STARS_OFFSET_1158_pctSh 0
#define STARS_BITSHIFT_1158_pctSh 0
#define STARS_BITMASK_1158_pctSh 0x7fUL
#define STARS_OFFSET_1158_pctDp 0
#define STARS_BITSHIFT_1158_pctDp 7
#define STARS_BITMASK_1158_pctDp 0x1ffUL
typedef STARS_CV_1158 STARS_CV_1525[16];
typedef STARS_CV_0012 STARS_CV_108f[5];
#ifdef STARS_NATIVE16
typedef STARS_CV_1528 STARS_FAR *STARS_CV_1529;
#else
typedef STARS_ULONG32 STARS_CV_1529;
#endif
#ifdef STARS_NATIVE16
typedef STARS_CV_1003 STARS_FAR *STARS_CV_1004;
#else
typedef STARS_ULONG32 STARS_CV_1004;
#endif
typedef STARS_CV_0021 STARS_CV_1411; /* bit-field backing storage; use recorded bit positions/masks */
typedef STARS_CV_0021 STARS_CV_13db; /* bit-field backing storage; use recorded bit positions/masks */
typedef STARS_CV_0021 STARS_CV_14ba; /* bit-field backing storage; use recorded bit positions/masks */
typedef STARS_CV_0021 STARS_CV_14bb; /* bit-field backing storage; use recorded bit positions/masks */
typedef STARS_CV_0021 STARS_CV_13b5; /* bit-field backing storage; use recorded bit positions/masks */
/* _fleet: 124 bytes; each view begins at its recorded offset. */
union STARS_CV_1003 {
    unsigned char raw[124];
    struct { STARS_CV_0011 value; } id;
    struct { STARS_CV_13d4 value; } ifl;
    struct { STARS_CV_13cf value; } iplr;
    struct { STARS_CV_13d0 value; } junk;
    struct { unsigned char _pad[2]; STARS_CV_0011 value; } iPlayer;
    struct { unsigned char _pad[4]; STARS_CV_135d value; } det;
    struct { unsigned char _pad[4]; STARS_CV_135e value; } fInclude;
    struct { unsigned char _pad[4]; STARS_CV_135f value; } fRepOrders;
    struct { unsigned char _pad[4]; STARS_CV_1360 value; } fDead;
    struct { unsigned char _pad[4]; STARS_CV_1361 value; } fDone;
    struct { unsigned char _pad[4]; STARS_CV_137b value; } fBombed;
    struct { unsigned char _pad[4]; STARS_CV_137c value; } fHereAllTurn;
    struct { unsigned char _pad[4]; STARS_CV_1395 value; } fNoHeal;
    struct { unsigned char _pad[4]; STARS_CV_1396 value; } fMark;
    struct { unsigned char _pad[6]; STARS_CV_0011 value; } idPlanet;
    struct { unsigned char _pad[8]; STARS_CV_1018 value; } pt;
    struct { unsigned char _pad[12]; STARS_CV_13f9 value; } rgcsh;
    struct { unsigned char _pad[44]; STARS_CV_1525 value; } rgdv;
    struct { unsigned char _pad[44]; STARS_CV_0012 value; } wtFleet;
    struct { unsigned char _pad[76]; STARS_CV_108f value; } rgwtMin;
    struct { unsigned char _pad[96]; STARS_CV_0020 value; } iplan;
    struct { unsigned char _pad[97]; STARS_CV_0020 value; } bUnused;
    struct { unsigned char _pad[98]; STARS_CV_0011 value; } cord;
    struct { unsigned char _pad[100]; STARS_CV_1529 value; } lpplord;
    struct { unsigned char _pad[104]; STARS_CV_1004 value; } lpflNext;
    struct { unsigned char _pad[108]; STARS_CV_0012 value; } lPower;
    struct { unsigned char _pad[108]; STARS_CV_0011 value; } dMoveLeft;
    struct { unsigned char _pad[110]; STARS_CV_0011 value; } dMoveUsed;
    struct { unsigned char _pad[112]; STARS_CV_0012 value; } lFuelUsed;
    struct { unsigned char _pad[116]; STARS_CV_0012 value; } dirLong;
    struct { unsigned char _pad[116]; STARS_CV_135d value; } dirFltX;
    struct { unsigned char _pad[116]; STARS_CV_1411 value; } dirFltY;
    struct { unsigned char _pad[118]; STARS_CV_1378 value; } iwarpFlt;
    struct { unsigned char _pad[118]; STARS_CV_13db value; } fdirValid;
    struct { unsigned char _pad[118]; STARS_CV_14ba value; } fCompChg;
    struct { unsigned char _pad[118]; STARS_CV_14bb value; } fTargeted;
    struct { unsigned char _pad[118]; STARS_CV_13b5 value; } fSkipped;
    struct { unsigned char _pad[118]; STARS_CV_1411 value; } fUnused;
    struct { unsigned char _pad[120]; STARS_CV_0270 value; } lpszName;
};
#define STARS_OFFSET_1003_id 0
#define STARS_OFFSET_1003_ifl 0
#define STARS_BITSHIFT_1003_ifl 0
#define STARS_BITMASK_1003_ifl 0x1ffUL
#define STARS_OFFSET_1003_iplr 0
#define STARS_BITSHIFT_1003_iplr 9
#define STARS_BITMASK_1003_iplr 0xfUL
#define STARS_OFFSET_1003_junk 0
#define STARS_BITSHIFT_1003_junk 13
#define STARS_BITMASK_1003_junk 0x7UL
#define STARS_OFFSET_1003_iPlayer 2
#define STARS_OFFSET_1003_det 4
#define STARS_BITSHIFT_1003_det 0
#define STARS_BITMASK_1003_det 0xffUL
#define STARS_OFFSET_1003_fInclude 4
#define STARS_BITSHIFT_1003_fInclude 8
#define STARS_BITMASK_1003_fInclude 0x1UL
#define STARS_OFFSET_1003_fRepOrders 4
#define STARS_BITSHIFT_1003_fRepOrders 9
#define STARS_BITMASK_1003_fRepOrders 0x1UL
#define STARS_OFFSET_1003_fDead 4
#define STARS_BITSHIFT_1003_fDead 10
#define STARS_BITMASK_1003_fDead 0x1UL
#define STARS_OFFSET_1003_fDone 4
#define STARS_BITSHIFT_1003_fDone 11
#define STARS_BITMASK_1003_fDone 0x1UL
#define STARS_OFFSET_1003_fBombed 4
#define STARS_BITSHIFT_1003_fBombed 12
#define STARS_BITMASK_1003_fBombed 0x1UL
#define STARS_OFFSET_1003_fHereAllTurn 4
#define STARS_BITSHIFT_1003_fHereAllTurn 13
#define STARS_BITMASK_1003_fHereAllTurn 0x1UL
#define STARS_OFFSET_1003_fNoHeal 4
#define STARS_BITSHIFT_1003_fNoHeal 14
#define STARS_BITMASK_1003_fNoHeal 0x1UL
#define STARS_OFFSET_1003_fMark 4
#define STARS_BITSHIFT_1003_fMark 15
#define STARS_BITMASK_1003_fMark 0x1UL
#define STARS_OFFSET_1003_idPlanet 6
#define STARS_OFFSET_1003_pt 8
#define STARS_OFFSET_1003_rgcsh 12
#define STARS_OFFSET_1003_rgdv 44
#define STARS_OFFSET_1003_wtFleet 44
#define STARS_OFFSET_1003_rgwtMin 76
#define STARS_OFFSET_1003_iplan 96
#define STARS_OFFSET_1003_bUnused 97
#define STARS_OFFSET_1003_cord 98
#define STARS_OFFSET_1003_lpplord 100
#define STARS_OFFSET_1003_lpflNext 104
#define STARS_OFFSET_1003_lPower 108
#define STARS_OFFSET_1003_dMoveLeft 108
#define STARS_OFFSET_1003_dMoveUsed 110
#define STARS_OFFSET_1003_lFuelUsed 112
#define STARS_OFFSET_1003_dirLong 116
#define STARS_OFFSET_1003_dirFltX 116
#define STARS_BITSHIFT_1003_dirFltX 0
#define STARS_BITMASK_1003_dirFltX 0xffUL
#define STARS_OFFSET_1003_dirFltY 116
#define STARS_BITSHIFT_1003_dirFltY 8
#define STARS_BITMASK_1003_dirFltY 0xffUL
#define STARS_OFFSET_1003_iwarpFlt 118
#define STARS_BITSHIFT_1003_iwarpFlt 0
#define STARS_BITMASK_1003_iwarpFlt 0xfUL
#define STARS_OFFSET_1003_fdirValid 118
#define STARS_BITSHIFT_1003_fdirValid 4
#define STARS_BITMASK_1003_fdirValid 0x1UL
#define STARS_OFFSET_1003_fCompChg 118
#define STARS_BITSHIFT_1003_fCompChg 5
#define STARS_BITMASK_1003_fCompChg 0x1UL
#define STARS_OFFSET_1003_fTargeted 118
#define STARS_BITSHIFT_1003_fTargeted 6
#define STARS_BITMASK_1003_fTargeted 0x1UL
#define STARS_OFFSET_1003_fSkipped 118
#define STARS_BITSHIFT_1003_fSkipped 7
#define STARS_BITMASK_1003_fSkipped 0x1UL
#define STARS_OFFSET_1003_fUnused 118
#define STARS_BITSHIFT_1003_fUnused 8
#define STARS_BITMASK_1003_fUnused 0xffUL
#define STARS_OFFSET_1003_lpszName 120
typedef STARS_CV_0021 STARS_CV_1379; /* bit-field backing storage; use recorded bit positions/masks */
typedef STARS_CV_0021 STARS_CV_137a; /* bit-field backing storage; use recorded bit positions/masks */
/* _itemaction: 2 bytes; each view begins at its recorded offset. */
union STARS_CV_10cb {
    unsigned char raw[2];
    struct { STARS_CV_1364 value; } cQuan;
    struct { STARS_CV_1362 value; } iAction;
};
#define STARS_OFFSET_10cb_cQuan 0
#define STARS_BITSHIFT_10cb_cQuan 0
#define STARS_BITMASK_10cb_cQuan 0xfffUL
#define STARS_OFFSET_10cb_iAction 0
#define STARS_BITSHIFT_10cb_iAction 12
#define STARS_BITMASK_10cb_iAction 0xfUL
typedef STARS_CV_10cb STARS_CV_10ee[5];
/* _taskxport: 10 bytes; each view begins at its recorded offset. */
union STARS_CV_137f {
    unsigned char raw[10];
    struct { STARS_CV_10ee value; } rgia;
};
#define STARS_OFFSET_137f_rgia 0
/* _tasklaymines: 4 bytes; each view begins at its recorded offset. */
union STARS_CV_1381 {
    unsigned char raw[4];
    struct { STARS_CV_0021 value; } cTime;
    struct { unsigned char _pad[2]; STARS_CV_0021 value; } cTimeOld;
};
#define STARS_OFFSET_1381_cTime 0
#define STARS_OFFSET_1381_cTimeOld 2
/* _taskpatrol: 4 bytes; each view begins at its recorded offset. */
union STARS_CV_1383 {
    unsigned char raw[4];
    struct { STARS_CV_0021 value; } iWarp;
    struct { unsigned char _pad[2]; STARS_CV_0021 value; } iDist;
};
#define STARS_OFFSET_1383_iWarp 0
#define STARS_OFFSET_1383_iDist 2
/* _tasksell: 2 bytes; each view begins at its recorded offset. */
union STARS_CV_1385 {
    unsigned char raw[2];
    struct { STARS_CV_0021 value; } iPlrX;
};
#define STARS_OFFSET_1385_iPlrX 0
/* _order: 18 bytes; each view begins at its recorded offset. */
union STARS_CV_1005 {
    unsigned char raw[18];
    struct { STARS_CV_1018 value; } pt;
    struct { unsigned char _pad[4]; STARS_CV_0011 value; } id;
    struct { unsigned char _pad[6]; STARS_CV_1378 value; } grTask;
    struct { unsigned char _pad[6]; STARS_CV_1379 value; } iWarp;
    struct { unsigned char _pad[6]; STARS_CV_137a value; } grobj;
    struct { unsigned char _pad[6]; STARS_CV_137b value; } fValidTask;
    struct { unsigned char _pad[6]; STARS_CV_137c value; } fNoAutoTrack;
    struct { unsigned char _pad[6]; STARS_CV_137d value; } fUnused;
    struct { unsigned char _pad[8]; STARS_CV_137f value; } txp;
    struct { unsigned char _pad[8]; STARS_CV_1381 value; } tlm;
    struct { unsigned char _pad[8]; STARS_CV_1383 value; } tptl;
    struct { unsigned char _pad[8]; STARS_CV_1385 value; } tsell;
};
#define STARS_OFFSET_1005_pt 0
#define STARS_OFFSET_1005_id 4
#define STARS_OFFSET_1005_grTask 6
#define STARS_BITSHIFT_1005_grTask 0
#define STARS_BITMASK_1005_grTask 0xfUL
#define STARS_OFFSET_1005_iWarp 6
#define STARS_BITSHIFT_1005_iWarp 4
#define STARS_BITMASK_1005_iWarp 0xfUL
#define STARS_OFFSET_1005_grobj 6
#define STARS_BITSHIFT_1005_grobj 8
#define STARS_BITMASK_1005_grobj 0xfUL
#define STARS_OFFSET_1005_fValidTask 6
#define STARS_BITSHIFT_1005_fValidTask 12
#define STARS_BITMASK_1005_fValidTask 0x1UL
#define STARS_OFFSET_1005_fNoAutoTrack 6
#define STARS_BITSHIFT_1005_fNoAutoTrack 13
#define STARS_BITMASK_1005_fNoAutoTrack 0x1UL
#define STARS_OFFSET_1005_fUnused 6
#define STARS_BITSHIFT_1005_fUnused 14
#define STARS_BITMASK_1005_fUnused 0x3UL
#define STARS_OFFSET_1005_txp 8
#define STARS_OFFSET_1005_tlm 8
#define STARS_OFFSET_1005_tptl 8
#define STARS_OFFSET_1005_tsell 8
typedef STARS_CV_0020 STARS_CV_105d[10];
/* _thmine: 10 bytes; each view begins at its recorded offset. */
union STARS_CV_1429 {
    unsigned char raw[10];
    struct { STARS_CV_0012 value; } cMines;
    struct { unsigned char _pad[4]; STARS_CV_0021 value; } grbitPlr;
    struct { unsigned char _pad[6]; STARS_CV_0020 value; } iType;
    struct { unsigned char _pad[7]; STARS_CV_0020 value; } fDetonate;
    struct { unsigned char _pad[8]; STARS_CV_0021 value; } grbitPlrNow;
};
#define STARS_OFFSET_1429_cMines 0
#define STARS_OFFSET_1429_grbitPlr 4
#define STARS_OFFSET_1429_iType 6
#define STARS_OFFSET_1429_fDetonate 7
#define STARS_OFFSET_1429_grbitPlrNow 8
typedef STARS_CV_0072 STARS_CV_1044[3];
typedef STARS_CV_0021 STARS_CV_143f; /* bit-field backing storage; use recorded bit positions/masks */
/* _thpack: 10 bytes; each view begins at its recorded offset. */
union STARS_CV_1441 {
    unsigned char raw[10];
    struct { STARS_CV_1393 value; } idPlanet;
    struct { STARS_CV_1394 value; } iWarp;
    struct { STARS_CV_1395 value; } fMoved;
    struct { STARS_CV_1396 value; } fInclude;
    struct { unsigned char _pad[2]; STARS_CV_1044 value; } rgwtMin;
    struct { unsigned char _pad[8]; STARS_CV_143f value; } wtMax;
    struct { unsigned char _pad[8]; STARS_CV_137d value; } iDecayRate;
};
#define STARS_OFFSET_1441_idPlanet 0
#define STARS_BITSHIFT_1441_idPlanet 0
#define STARS_BITMASK_1441_idPlanet 0x3ffUL
#define STARS_OFFSET_1441_iWarp 0
#define STARS_BITSHIFT_1441_iWarp 10
#define STARS_BITMASK_1441_iWarp 0xfUL
#define STARS_OFFSET_1441_fMoved 0
#define STARS_BITSHIFT_1441_fMoved 14
#define STARS_BITMASK_1441_fMoved 0x1UL
#define STARS_OFFSET_1441_fInclude 0
#define STARS_BITSHIFT_1441_fInclude 15
#define STARS_BITMASK_1441_fInclude 0x1UL
#define STARS_OFFSET_1441_rgwtMin 2
#define STARS_OFFSET_1441_wtMax 8
#define STARS_BITSHIFT_1441_wtMax 0
#define STARS_BITMASK_1441_wtMax 0x3fffUL
#define STARS_OFFSET_1441_iDecayRate 8
#define STARS_BITSHIFT_1441_iDecayRate 14
#define STARS_BITMASK_1441_iDecayRate 0x3UL
typedef STARS_CV_0021 STARS_CV_145a; /* bit-field backing storage; use recorded bit positions/masks */
typedef STARS_CV_0021 STARS_CV_145b; /* bit-field backing storage; use recorded bit positions/masks */
/* _thworm: 8 bytes; each view begins at its recorded offset. */
union STARS_CV_145d {
    unsigned char raw[8];
    struct { STARS_CV_145a value; } iStable;
    struct { STARS_CV_145b value; } cLastMove;
    struct { STARS_CV_137b value; } fDestKnown;
    struct { STARS_CV_137c value; } fInclude;
    struct { unsigned char _pad[2]; STARS_CV_0021 value; } grbitPlr;
    struct { unsigned char _pad[4]; STARS_CV_0021 value; } grbitPlrTrav;
    struct { unsigned char _pad[6]; STARS_CV_0021 value; } idPartner;
};
#define STARS_OFFSET_145d_iStable 0
#define STARS_BITSHIFT_145d_iStable 0
#define STARS_BITMASK_145d_iStable 0x3UL
#define STARS_OFFSET_145d_cLastMove 0
#define STARS_BITSHIFT_145d_cLastMove 2
#define STARS_BITMASK_145d_cLastMove 0x3ffUL
#define STARS_OFFSET_145d_fDestKnown 0
#define STARS_BITSHIFT_145d_fDestKnown 12
#define STARS_BITMASK_145d_fDestKnown 0x1UL
#define STARS_OFFSET_145d_fInclude 0
#define STARS_BITSHIFT_145d_fInclude 13
#define STARS_BITMASK_145d_fInclude 0x1UL
#define STARS_OFFSET_145d_grbitPlr 2
#define STARS_OFFSET_145d_grbitPlrTrav 4
#define STARS_OFFSET_145d_idPartner 6
typedef STARS_CV_0021 STARS_CV_13dc; /* bit-field backing storage; use recorded bit positions/masks */
/* _thtrader: 10 bytes; each view begins at its recorded offset. */
union STARS_CV_1496 {
    unsigned char raw[10];
    struct { STARS_CV_1018 value; } ptDest;
    struct { unsigned char _pad[4]; STARS_CV_1378 value; } iWarp;
    struct { unsigned char _pad[4]; STARS_CV_13db value; } fInclude;
    struct { unsigned char _pad[4]; STARS_CV_13dc value; } unused;
    struct { unsigned char _pad[6]; STARS_CV_0021 value; } grbitPlr;
    struct { unsigned char _pad[8]; STARS_CV_0021 value; } grbitTrader;
};
#define STARS_OFFSET_1496_ptDest 0
#define STARS_OFFSET_1496_iWarp 4
#define STARS_BITSHIFT_1496_iWarp 0
#define STARS_BITMASK_1496_iWarp 0xfUL
#define STARS_OFFSET_1496_fInclude 4
#define STARS_BITSHIFT_1496_fInclude 4
#define STARS_BITMASK_1496_fInclude 0x1UL
#define STARS_OFFSET_1496_unused 4
#define STARS_BITSHIFT_1496_unused 5
#define STARS_BITMASK_1496_unused 0x7ffUL
#define STARS_OFFSET_1496_grbitPlr 6
#define STARS_OFFSET_1496_grbitTrader 8
/* _thing: 18 bytes; each view begins at its recorded offset. */
union STARS_CV_1006 {
    unsigned char raw[18];
    struct { STARS_CV_0021 value; } idFull;
    struct { STARS_CV_13d4 value; } id;
    struct { STARS_CV_13cf value; } iplr;
    struct { STARS_CV_13d0 value; } ith;
    struct { unsigned char _pad[2]; STARS_CV_1018 value; } pt;
    struct { unsigned char _pad[6]; STARS_CV_105d value; } rgb;
    struct { unsigned char _pad[6]; STARS_CV_1429 value; } thm;
    struct { unsigned char _pad[6]; STARS_CV_1441 value; } thp;
    struct { unsigned char _pad[6]; STARS_CV_145d value; } thw;
    struct { unsigned char _pad[6]; STARS_CV_1496 value; } tht;
    struct { unsigned char _pad[16]; STARS_CV_0021 value; } turn;
};
#define STARS_OFFSET_1006_idFull 0
#define STARS_OFFSET_1006_id 0
#define STARS_BITSHIFT_1006_id 0
#define STARS_BITMASK_1006_id 0x1ffUL
#define STARS_OFFSET_1006_iplr 0
#define STARS_BITSHIFT_1006_iplr 9
#define STARS_BITMASK_1006_iplr 0xfUL
#define STARS_OFFSET_1006_ith 0
#define STARS_BITSHIFT_1006_ith 13
#define STARS_BITMASK_1006_ith 0x7UL
#define STARS_OFFSET_1006_pt 2
#define STARS_OFFSET_1006_rgb 6
#define STARS_OFFSET_1006_thm 6
#define STARS_OFFSET_1006_thp 6
#define STARS_OFFSET_1006_thw 6
#define STARS_OFFSET_1006_tht 6
#define STARS_OFFSET_1006_turn 16
#ifdef STARS_NATIVE16
typedef STARS_CV_1006 STARS_FAR *STARS_CV_1007;
#else
typedef STARS_ULONG32 STARS_CV_1007;
#endif
typedef STARS_CV_0070 STARS_CV_1270[6];
typedef STARS_CV_0070 STARS_CV_11e2[32];
typedef STARS_CV_0021 STARS_CV_1363[3];
/* _hs: 4 bytes; each view begins at its recorded offset. */
union STARS_CV_1078 {
    unsigned char raw[4];
    struct { STARS_CV_0021 value; } grhst;
    struct { unsigned char _pad[2]; STARS_CV_135d value; } iItem;
    struct { unsigned char _pad[2]; STARS_CV_1411 value; } cItem;
};
#define STARS_OFFSET_1078_grhst 0
#define STARS_OFFSET_1078_iItem 2
#define STARS_BITSHIFT_1078_iItem 0
#define STARS_BITMASK_1078_iItem 0xffUL
#define STARS_OFFSET_1078_cItem 2
#define STARS_BITSHIFT_1078_cItem 8
#define STARS_BITMASK_1078_cItem 0xffUL
typedef STARS_CV_1078 STARS_CV_1537[16];
/* _hul: 123 bytes; each view begins at its recorded offset. */
union STARS_CV_1076 {
    unsigned char raw[123];
    struct { STARS_CV_0011 value; } ihuldef;
    struct { unsigned char _pad[2]; STARS_CV_1270 value; } rgTech;
    struct { unsigned char _pad[8]; STARS_CV_11e2 value; } szClass;
    struct { unsigned char _pad[40]; STARS_CV_0021 value; } wtEmpty;
    struct { unsigned char _pad[42]; STARS_CV_0021 value; } resCost;
    struct { unsigned char _pad[44]; STARS_CV_1363 value; } rgwtOreCost;
    struct { unsigned char _pad[50]; STARS_CV_0011 value; } ibmp;
    struct { unsigned char _pad[52]; STARS_CV_0021 value; } wtCargoMax;
    struct { unsigned char _pad[54]; STARS_CV_0021 value; } wtFuelMax;
    struct { unsigned char _pad[56]; STARS_CV_0021 value; } dp;
    struct { unsigned char _pad[58]; STARS_CV_1537 value; } rghs;
    struct { unsigned char _pad[122]; STARS_CV_0020 value; } chs;
};
#define STARS_OFFSET_1076_ihuldef 0
#define STARS_OFFSET_1076_rgTech 2
#define STARS_OFFSET_1076_szClass 8
#define STARS_OFFSET_1076_wtEmpty 40
#define STARS_OFFSET_1076_resCost 42
#define STARS_OFFSET_1076_rgwtOreCost 44
#define STARS_OFFSET_1076_ibmp 50
#define STARS_OFFSET_1076_wtCargoMax 52
#define STARS_OFFSET_1076_wtFuelMax 54
#define STARS_OFFSET_1076_dp 56
#define STARS_OFFSET_1076_rghs 58
#define STARS_OFFSET_1076_chs 122
typedef STARS_CV_0021 STARS_CV_146b; /* bit-field backing storage; use recorded bit positions/masks */
/* _shdef: 147 bytes; each view begins at its recorded offset. */
union STARS_CV_1008 {
    unsigned char raw[147];
    struct { STARS_CV_1076 value; } hul;
    struct { unsigned char _pad[123]; STARS_CV_135d value; } det;
    struct { unsigned char _pad[123]; STARS_CV_135e value; } fInclude;
    struct { unsigned char _pad[123]; STARS_CV_135f value; } fFree;
    struct { unsigned char _pad[123]; STARS_CV_146b value; } ishdef;
    struct { unsigned char _pad[123]; STARS_CV_1396 value; } fGift;
    struct { unsigned char _pad[123]; STARS_CV_0021 value; } wFlags;
    struct { unsigned char _pad[125]; STARS_CV_0021 value; } turn;
    struct { unsigned char _pad[127]; STARS_CV_0022 value; } cBuilt;
    struct { unsigned char _pad[131]; STARS_CV_0022 value; } cExist;
    struct { unsigned char _pad[135]; STARS_CV_0012 value; } lPower;
    struct { unsigned char _pad[135]; STARS_CV_0012 value; } lVisible;
    struct { unsigned char _pad[139]; STARS_CV_0021 value; } grbitPlr;
    struct { unsigned char _pad[141]; STARS_CV_0021 value; } dScanRange;
    struct { unsigned char _pad[143]; STARS_CV_0021 value; } dScanRange2;
    struct { unsigned char _pad[145]; STARS_CV_0020 value; } pctDetect;
    struct { unsigned char _pad[146]; STARS_CV_0020 value; } iSteal;
};
#define STARS_OFFSET_1008_hul 0
#define STARS_OFFSET_1008_det 123
#define STARS_BITSHIFT_1008_det 0
#define STARS_BITMASK_1008_det 0xffUL
#define STARS_OFFSET_1008_fInclude 123
#define STARS_BITSHIFT_1008_fInclude 8
#define STARS_BITMASK_1008_fInclude 0x1UL
#define STARS_OFFSET_1008_fFree 123
#define STARS_BITSHIFT_1008_fFree 9
#define STARS_BITMASK_1008_fFree 0x1UL
#define STARS_OFFSET_1008_ishdef 123
#define STARS_BITSHIFT_1008_ishdef 10
#define STARS_BITMASK_1008_ishdef 0x1fUL
#define STARS_OFFSET_1008_fGift 123
#define STARS_BITSHIFT_1008_fGift 15
#define STARS_BITMASK_1008_fGift 0x1UL
#define STARS_OFFSET_1008_wFlags 123
#define STARS_OFFSET_1008_turn 125
#define STARS_OFFSET_1008_cBuilt 127
#define STARS_OFFSET_1008_cExist 131
#define STARS_OFFSET_1008_lPower 135
#define STARS_OFFSET_1008_lVisible 135
#define STARS_OFFSET_1008_grbitPlr 139
#define STARS_OFFSET_1008_dScanRange 141
#define STARS_OFFSET_1008_dScanRange2 143
#define STARS_OFFSET_1008_pctDetect 145
#define STARS_OFFSET_1008_iSteal 146
#ifdef STARS_NATIVE16
typedef STARS_CV_1008 STARS_FAR *STARS_CV_1009;
#else
typedef STARS_ULONG32 STARS_CV_1009;
#endif
typedef STARS_CV_0012 STARS_CV_100a[3];
#ifdef STARS_NATIVE16
typedef STARS_CV_1002 STARS_FAR *STARS_CV_100b;
#else
typedef STARS_ULONG32 STARS_CV_100b;
#endif
typedef STARS_CV_0072 STARS_CV_100d[16];
typedef STARS_CV_0020 STARS_CV_100e[6];
typedef STARS_CV_0020 STARS_CV_100f[13];
#ifdef STARS_NATIVE16
typedef STARS_CV_1004 STARS_FAR *STARS_CV_1010;
#else
typedef STARS_ULONG32 STARS_CV_1010;
#endif
typedef STARS_CV_1009 STARS_CV_1011[];
typedef STARS_CV_0021 STARS_CV_13d7; /* bit-field backing storage; use recorded bit positions/masks */
typedef STARS_CV_0021 STARS_CV_13d8; /* bit-field backing storage; use recorded bit positions/masks */
typedef STARS_CV_0021 STARS_CV_13d9; /* bit-field backing storage; use recorded bit positions/masks */
typedef STARS_CV_0021 STARS_CV_13da; /* bit-field backing storage; use recorded bit positions/masks */
typedef STARS_CV_0021 STARS_CV_13a9; /* bit-field backing storage; use recorded bit positions/masks */
typedef STARS_CV_0020 STARS_CV_14cb[12];
/* _game: 64 bytes; each view begins at its recorded offset. */
union STARS_CV_1012 {
    unsigned char raw[64];
    struct { STARS_CV_0012 value; } lid;
    struct { unsigned char _pad[4]; STARS_CV_0011 value; } mdSize;
    struct { unsigned char _pad[6]; STARS_CV_0011 value; } mdDensity;
    struct { unsigned char _pad[8]; STARS_CV_0011 value; } cPlayer;
    struct { unsigned char _pad[10]; STARS_CV_0011 value; } cPlanMax;
    struct { unsigned char _pad[12]; STARS_CV_0011 value; } mdStartDist;
    struct { unsigned char _pad[14]; STARS_CV_0072 value; } fDirty;
    struct { unsigned char _pad[16]; STARS_CV_13d7 value; } fExtraFuel;
    struct { unsigned char _pad[16]; STARS_CV_13d8 value; } fSlowTech;
    struct { unsigned char _pad[16]; STARS_CV_13d9 value; } fSinglePlr;
    struct { unsigned char _pad[16]; STARS_CV_13da value; } fTutorial;
    struct { unsigned char _pad[16]; STARS_CV_13db value; } fAisBand;
    struct { unsigned char _pad[16]; STARS_CV_14ba value; } fBBSPlay;
    struct { unsigned char _pad[16]; STARS_CV_14bb value; } fVisScores;
    struct { unsigned char _pad[16]; STARS_CV_13b5 value; } fNoRandom;
    struct { unsigned char _pad[16]; STARS_CV_135e value; } fClumping;
    struct { unsigned char _pad[16]; STARS_CV_13a9 value; } wGen;
    struct { unsigned char _pad[16]; STARS_CV_1362 value; } unused;
    struct { unsigned char _pad[16]; STARS_CV_0021 value; } wCrap;
    struct { unsigned char _pad[18]; STARS_CV_0021 value; } turn;
    struct { unsigned char _pad[20]; STARS_CV_14cb value; } rgvc;
    struct { unsigned char _pad[32]; STARS_CV_11e2 value; } szName;
};
#define STARS_OFFSET_1012_lid 0
#define STARS_OFFSET_1012_mdSize 4
#define STARS_OFFSET_1012_mdDensity 6
#define STARS_OFFSET_1012_cPlayer 8
#define STARS_OFFSET_1012_cPlanMax 10
#define STARS_OFFSET_1012_mdStartDist 12
#define STARS_OFFSET_1012_fDirty 14
#define STARS_OFFSET_1012_fExtraFuel 16
#define STARS_BITSHIFT_1012_fExtraFuel 0
#define STARS_BITMASK_1012_fExtraFuel 0x1UL
#define STARS_OFFSET_1012_fSlowTech 16
#define STARS_BITSHIFT_1012_fSlowTech 1
#define STARS_BITMASK_1012_fSlowTech 0x1UL
#define STARS_OFFSET_1012_fSinglePlr 16
#define STARS_BITSHIFT_1012_fSinglePlr 2
#define STARS_BITMASK_1012_fSinglePlr 0x1UL
#define STARS_OFFSET_1012_fTutorial 16
#define STARS_BITSHIFT_1012_fTutorial 3
#define STARS_BITMASK_1012_fTutorial 0x1UL
#define STARS_OFFSET_1012_fAisBand 16
#define STARS_BITSHIFT_1012_fAisBand 4
#define STARS_BITMASK_1012_fAisBand 0x1UL
#define STARS_OFFSET_1012_fBBSPlay 16
#define STARS_BITSHIFT_1012_fBBSPlay 5
#define STARS_BITMASK_1012_fBBSPlay 0x1UL
#define STARS_OFFSET_1012_fVisScores 16
#define STARS_BITSHIFT_1012_fVisScores 6
#define STARS_BITMASK_1012_fVisScores 0x1UL
#define STARS_OFFSET_1012_fNoRandom 16
#define STARS_BITSHIFT_1012_fNoRandom 7
#define STARS_BITMASK_1012_fNoRandom 0x1UL
#define STARS_OFFSET_1012_fClumping 16
#define STARS_BITSHIFT_1012_fClumping 8
#define STARS_BITMASK_1012_fClumping 0x1UL
#define STARS_OFFSET_1012_wGen 16
#define STARS_BITSHIFT_1012_wGen 9
#define STARS_BITMASK_1012_wGen 0x7UL
#define STARS_OFFSET_1012_unused 16
#define STARS_BITSHIFT_1012_unused 12
#define STARS_BITMASK_1012_unused 0xfUL
#define STARS_OFFSET_1012_wCrap 16
#define STARS_OFFSET_1012_turn 18
#define STARS_OFFSET_1012_rgvc 20
#define STARS_OFFSET_1012_szName 32
typedef STARS_CV_0073 STARS_CV_13bf; /* bit-field backing storage; use recorded bit positions/masks */
typedef STARS_CV_0073 STARS_CV_13c0; /* bit-field backing storage; use recorded bit positions/masks */
typedef STARS_CV_0073 STARS_CV_13c1; /* bit-field backing storage; use recorded bit positions/masks */
typedef STARS_CV_0073 STARS_CV_13c2; /* bit-field backing storage; use recorded bit positions/masks */
typedef STARS_CV_0073 STARS_CV_13c3; /* bit-field backing storage; use recorded bit positions/masks */
typedef STARS_CV_0073 STARS_CV_13c4; /* bit-field backing storage; use recorded bit positions/masks */
typedef STARS_CV_0073 STARS_CV_146f; /* bit-field backing storage; use recorded bit positions/masks */
typedef STARS_CV_0073 STARS_CV_141a; /* bit-field backing storage; use recorded bit positions/masks */
typedef STARS_CV_0073 STARS_CV_141b; /* bit-field backing storage; use recorded bit positions/masks */
typedef STARS_CV_0073 STARS_CV_13c6; /* bit-field backing storage; use recorded bit positions/masks */
typedef STARS_CV_0073 STARS_CV_13c7; /* bit-field backing storage; use recorded bit positions/masks */
typedef STARS_CV_0073 STARS_CV_13c8; /* bit-field backing storage; use recorded bit positions/masks */
typedef STARS_CV_0073 STARS_CV_13c9; /* bit-field backing storage; use recorded bit positions/masks */
typedef STARS_CV_0073 STARS_CV_13ca; /* bit-field backing storage; use recorded bit positions/masks */
typedef STARS_CV_0073 STARS_CV_1470; /* bit-field backing storage; use recorded bit positions/masks */
typedef STARS_CV_0073 STARS_CV_13cb; /* bit-field backing storage; use recorded bit positions/masks */
typedef STARS_CV_0073 STARS_CV_13cc; /* bit-field backing storage; use recorded bit positions/masks */
typedef STARS_CV_0073 STARS_CV_1471; /* bit-field backing storage; use recorded bit positions/masks */
typedef STARS_CV_0073 STARS_CV_1472; /* bit-field backing storage; use recorded bit positions/masks */
typedef STARS_CV_0073 STARS_CV_1473; /* bit-field backing storage; use recorded bit positions/masks */
/* _gdata: 10 bytes; each view begins at its recorded offset. */
union STARS_CV_1013 {
    unsigned char raw[10];
    struct { STARS_CV_0012 value; } grBits;
    struct { STARS_CV_13bf value; } fUnknownShip;
    struct { STARS_CV_13c0 value; } fGeneratingTurn;
    struct { STARS_CV_13c1 value; } fForceTurn;
    struct { STARS_CV_13c2 value; } fHostMode;
    struct { STARS_CV_13c3 value; } fSubmit;
    struct { STARS_CV_13c4 value; } fNoResearchSav;
    struct { STARS_CV_146f value; } fRadiatingEngine;
    struct { STARS_CV_141a value; } fNoIdleChecks;
    struct { STARS_CV_141b value; } fSendMsgMode;
    struct { STARS_CV_13c6 value; } fRetryOpens;
    struct { STARS_CV_13c7 value; } fAisDone;
    struct { STARS_CV_13c8 value; } fTutorial;
    struct { STARS_CV_13c9 value; } fGotoVCR;
    struct { STARS_CV_13ca value; } fVCRTimer;
    struct { STARS_CV_1470 value; } mdScreenSize;
    struct { unsigned char _pad[2]; STARS_CV_13bf value; } fGameOverMan;
    struct { unsigned char _pad[2]; STARS_CV_13c0 value; } fDontDoLogFiles;
    struct { unsigned char _pad[2]; STARS_CV_13c1 value; } fFileCrippled;
    struct { unsigned char _pad[2]; STARS_CV_13c2 value; } fSmallTileMode;
    struct { unsigned char _pad[2]; STARS_CV_13c3 value; } fAllAis;
    struct { unsigned char _pad[2]; STARS_CV_13c4 value; } fReadOnly;
    struct { unsigned char _pad[2]; STARS_CV_146f value; } fExitWindows;
    struct { unsigned char _pad[2]; STARS_CV_141a value; } fPartialTurn;
    struct { unsigned char _pad[2]; STARS_CV_141b value; } fSetMassMode;
    struct { unsigned char _pad[2]; STARS_CV_13c6 value; } fRptSafeDraw;
    struct { unsigned char _pad[2]; STARS_CV_13c7 value; } fProgressTxt;
    struct { unsigned char _pad[2]; STARS_CV_13c8 value; } fSoundFX;
    struct { unsigned char _pad[2]; STARS_CV_13c9 value; } fNoSound;
    struct { unsigned char _pad[2]; STARS_CV_13ca value; } fSetRouteMode;
    struct { unsigned char _pad[2]; STARS_CV_13cb value; } fBleedingEdge;
    struct { unsigned char _pad[2]; STARS_CV_13cc value; } fToolbar;
    struct { unsigned char _pad[4]; STARS_CV_0012 value; } grBits2;
    struct { unsigned char _pad[4]; STARS_CV_13bf value; } fNoScannerDraw;
    struct { unsigned char _pad[4]; STARS_CV_13c0 value; } fTrialPeriodOver;
    struct { unsigned char _pad[4]; STARS_CV_13c1 value; } fClose;
    struct { unsigned char _pad[4]; STARS_CV_13c2 value; } fDontCalcBleed;
    struct { unsigned char _pad[4]; STARS_CV_13c3 value; } fChgZipOrd;
    struct { unsigned char _pad[4]; STARS_CV_13c4 value; } fChgZipProd;
    struct { unsigned char _pad[4]; STARS_CV_146f value; } fChgScanner;
    struct { unsigned char _pad[4]; STARS_CV_141a value; } fChgReports;
    struct { unsigned char _pad[4]; STARS_CV_141b value; } fWriteTurnNum;
    struct { unsigned char _pad[4]; STARS_CV_13c6 value; } fHotSeat;
    struct { unsigned char _pad[4]; STARS_CV_13c7 value; } fFleetLinkValid;
    struct { unsigned char _pad[4]; STARS_CV_1471 value; } fScoreVictory;
    struct { unsigned char _pad[6]; STARS_CV_1472 value; } iCurGraph;
    struct { unsigned char _pad[6]; STARS_CV_13c3 value; } fMusic;
    struct { unsigned char _pad[6]; STARS_CV_13c4 value; } fPerPlayerDumps;
    struct { unsigned char _pad[6]; STARS_CV_146f value; } fNoHostNames;
    struct { unsigned char _pad[8]; STARS_CV_1473 value; } fUnused2;
};
#define STARS_OFFSET_1013_grBits 0
#define STARS_OFFSET_1013_fUnknownShip 0
#define STARS_BITSHIFT_1013_fUnknownShip 0
#define STARS_BITMASK_1013_fUnknownShip 0x1UL
#define STARS_OFFSET_1013_fGeneratingTurn 0
#define STARS_BITSHIFT_1013_fGeneratingTurn 1
#define STARS_BITMASK_1013_fGeneratingTurn 0x1UL
#define STARS_OFFSET_1013_fForceTurn 0
#define STARS_BITSHIFT_1013_fForceTurn 2
#define STARS_BITMASK_1013_fForceTurn 0x1UL
#define STARS_OFFSET_1013_fHostMode 0
#define STARS_BITSHIFT_1013_fHostMode 3
#define STARS_BITMASK_1013_fHostMode 0x1UL
#define STARS_OFFSET_1013_fSubmit 0
#define STARS_BITSHIFT_1013_fSubmit 4
#define STARS_BITMASK_1013_fSubmit 0x1UL
#define STARS_OFFSET_1013_fNoResearchSav 0
#define STARS_BITSHIFT_1013_fNoResearchSav 5
#define STARS_BITMASK_1013_fNoResearchSav 0x1UL
#define STARS_OFFSET_1013_fRadiatingEngine 0
#define STARS_BITSHIFT_1013_fRadiatingEngine 6
#define STARS_BITMASK_1013_fRadiatingEngine 0x1UL
#define STARS_OFFSET_1013_fNoIdleChecks 0
#define STARS_BITSHIFT_1013_fNoIdleChecks 7
#define STARS_BITMASK_1013_fNoIdleChecks 0x1UL
#define STARS_OFFSET_1013_fSendMsgMode 0
#define STARS_BITSHIFT_1013_fSendMsgMode 8
#define STARS_BITMASK_1013_fSendMsgMode 0x1UL
#define STARS_OFFSET_1013_fRetryOpens 0
#define STARS_BITSHIFT_1013_fRetryOpens 9
#define STARS_BITMASK_1013_fRetryOpens 0x1UL
#define STARS_OFFSET_1013_fAisDone 0
#define STARS_BITSHIFT_1013_fAisDone 10
#define STARS_BITMASK_1013_fAisDone 0x1UL
#define STARS_OFFSET_1013_fTutorial 0
#define STARS_BITSHIFT_1013_fTutorial 11
#define STARS_BITMASK_1013_fTutorial 0x1UL
#define STARS_OFFSET_1013_fGotoVCR 0
#define STARS_BITSHIFT_1013_fGotoVCR 12
#define STARS_BITMASK_1013_fGotoVCR 0x1UL
#define STARS_OFFSET_1013_fVCRTimer 0
#define STARS_BITSHIFT_1013_fVCRTimer 13
#define STARS_BITMASK_1013_fVCRTimer 0x1UL
#define STARS_OFFSET_1013_mdScreenSize 0
#define STARS_BITSHIFT_1013_mdScreenSize 14
#define STARS_BITMASK_1013_mdScreenSize 0x3UL
#define STARS_OFFSET_1013_fGameOverMan 2
#define STARS_BITSHIFT_1013_fGameOverMan 0
#define STARS_BITMASK_1013_fGameOverMan 0x1UL
#define STARS_OFFSET_1013_fDontDoLogFiles 2
#define STARS_BITSHIFT_1013_fDontDoLogFiles 1
#define STARS_BITMASK_1013_fDontDoLogFiles 0x1UL
#define STARS_OFFSET_1013_fFileCrippled 2
#define STARS_BITSHIFT_1013_fFileCrippled 2
#define STARS_BITMASK_1013_fFileCrippled 0x1UL
#define STARS_OFFSET_1013_fSmallTileMode 2
#define STARS_BITSHIFT_1013_fSmallTileMode 3
#define STARS_BITMASK_1013_fSmallTileMode 0x1UL
#define STARS_OFFSET_1013_fAllAis 2
#define STARS_BITSHIFT_1013_fAllAis 4
#define STARS_BITMASK_1013_fAllAis 0x1UL
#define STARS_OFFSET_1013_fReadOnly 2
#define STARS_BITSHIFT_1013_fReadOnly 5
#define STARS_BITMASK_1013_fReadOnly 0x1UL
#define STARS_OFFSET_1013_fExitWindows 2
#define STARS_BITSHIFT_1013_fExitWindows 6
#define STARS_BITMASK_1013_fExitWindows 0x1UL
#define STARS_OFFSET_1013_fPartialTurn 2
#define STARS_BITSHIFT_1013_fPartialTurn 7
#define STARS_BITMASK_1013_fPartialTurn 0x1UL
#define STARS_OFFSET_1013_fSetMassMode 2
#define STARS_BITSHIFT_1013_fSetMassMode 8
#define STARS_BITMASK_1013_fSetMassMode 0x1UL
#define STARS_OFFSET_1013_fRptSafeDraw 2
#define STARS_BITSHIFT_1013_fRptSafeDraw 9
#define STARS_BITMASK_1013_fRptSafeDraw 0x1UL
#define STARS_OFFSET_1013_fProgressTxt 2
#define STARS_BITSHIFT_1013_fProgressTxt 10
#define STARS_BITMASK_1013_fProgressTxt 0x1UL
#define STARS_OFFSET_1013_fSoundFX 2
#define STARS_BITSHIFT_1013_fSoundFX 11
#define STARS_BITMASK_1013_fSoundFX 0x1UL
#define STARS_OFFSET_1013_fNoSound 2
#define STARS_BITSHIFT_1013_fNoSound 12
#define STARS_BITMASK_1013_fNoSound 0x1UL
#define STARS_OFFSET_1013_fSetRouteMode 2
#define STARS_BITSHIFT_1013_fSetRouteMode 13
#define STARS_BITMASK_1013_fSetRouteMode 0x1UL
#define STARS_OFFSET_1013_fBleedingEdge 2
#define STARS_BITSHIFT_1013_fBleedingEdge 14
#define STARS_BITMASK_1013_fBleedingEdge 0x1UL
#define STARS_OFFSET_1013_fToolbar 2
#define STARS_BITSHIFT_1013_fToolbar 15
#define STARS_BITMASK_1013_fToolbar 0x1UL
#define STARS_OFFSET_1013_grBits2 4
#define STARS_OFFSET_1013_fNoScannerDraw 4
#define STARS_BITSHIFT_1013_fNoScannerDraw 0
#define STARS_BITMASK_1013_fNoScannerDraw 0x1UL
#define STARS_OFFSET_1013_fTrialPeriodOver 4
#define STARS_BITSHIFT_1013_fTrialPeriodOver 1
#define STARS_BITMASK_1013_fTrialPeriodOver 0x1UL
#define STARS_OFFSET_1013_fClose 4
#define STARS_BITSHIFT_1013_fClose 2
#define STARS_BITMASK_1013_fClose 0x1UL
#define STARS_OFFSET_1013_fDontCalcBleed 4
#define STARS_BITSHIFT_1013_fDontCalcBleed 3
#define STARS_BITMASK_1013_fDontCalcBleed 0x1UL
#define STARS_OFFSET_1013_fChgZipOrd 4
#define STARS_BITSHIFT_1013_fChgZipOrd 4
#define STARS_BITMASK_1013_fChgZipOrd 0x1UL
#define STARS_OFFSET_1013_fChgZipProd 4
#define STARS_BITSHIFT_1013_fChgZipProd 5
#define STARS_BITMASK_1013_fChgZipProd 0x1UL
#define STARS_OFFSET_1013_fChgScanner 4
#define STARS_BITSHIFT_1013_fChgScanner 6
#define STARS_BITMASK_1013_fChgScanner 0x1UL
#define STARS_OFFSET_1013_fChgReports 4
#define STARS_BITSHIFT_1013_fChgReports 7
#define STARS_BITMASK_1013_fChgReports 0x1UL
#define STARS_OFFSET_1013_fWriteTurnNum 4
#define STARS_BITSHIFT_1013_fWriteTurnNum 8
#define STARS_BITMASK_1013_fWriteTurnNum 0x1UL
#define STARS_OFFSET_1013_fHotSeat 4
#define STARS_BITSHIFT_1013_fHotSeat 9
#define STARS_BITMASK_1013_fHotSeat 0x1UL
#define STARS_OFFSET_1013_fFleetLinkValid 4
#define STARS_BITSHIFT_1013_fFleetLinkValid 10
#define STARS_BITMASK_1013_fFleetLinkValid 0x1UL
#define STARS_OFFSET_1013_fScoreVictory 4
#define STARS_BITSHIFT_1013_fScoreVictory 11
#define STARS_BITMASK_1013_fScoreVictory 0x3UL
#define STARS_OFFSET_1013_iCurGraph 6
#define STARS_BITSHIFT_1013_iCurGraph 0
#define STARS_BITMASK_1013_iCurGraph 0xfUL
#define STARS_OFFSET_1013_fMusic 6
#define STARS_BITSHIFT_1013_fMusic 4
#define STARS_BITMASK_1013_fMusic 0x1UL
#define STARS_OFFSET_1013_fPerPlayerDumps 6
#define STARS_BITSHIFT_1013_fPerPlayerDumps 5
#define STARS_BITMASK_1013_fPerPlayerDumps 0x1UL
#define STARS_OFFSET_1013_fNoHostNames 6
#define STARS_BITSHIFT_1013_fNoHostNames 6
#define STARS_BITMASK_1013_fNoHostNames 0x1UL
#define STARS_OFFSET_1013_fUnused2 8
#define STARS_BITSHIFT_1013_fUnused2 0
#define STARS_BITMASK_1013_fUnused2 0x3fffUL
typedef STARS_CV_0021 STARS_CV_13b3; /* bit-field backing storage; use recorded bit positions/masks */
typedef STARS_CV_0021 STARS_CV_13d2; /* bit-field backing storage; use recorded bit positions/masks */
typedef STARS_CV_0021 STARS_CV_13d3; /* bit-field backing storage; use recorded bit positions/masks */
typedef STARS_CV_0021 STARS_CV_13d5; /* bit-field backing storage; use recorded bit positions/masks */
typedef STARS_CV_0022 STARS_CV_13d6[6];
typedef STARS_CV_0070 STARS_CV_1209[16];
typedef STARS_CV_0021 STARS_CV_1346; /* bit-field backing storage; use recorded bit positions/masks */
typedef STARS_CV_0021 STARS_CV_1347; /* bit-field backing storage; use recorded bit positions/masks */
/* _prodq1: 2 bytes; each view begins at its recorded offset. */
union STARS_CV_1349 {
    unsigned char raw[2];
    struct { STARS_CV_0021 value; } w;
    struct { STARS_CV_1346 value; } mdIdle;
    struct { STARS_CV_1347 value; } cQuan;
};
#define STARS_OFFSET_1349_w 0
#define STARS_OFFSET_1349_mdIdle 0
#define STARS_BITSHIFT_1349_mdIdle 0
#define STARS_BITMASK_1349_mdIdle 0x3fUL
#define STARS_OFFSET_1349_cQuan 0
#define STARS_BITSHIFT_1349_cQuan 6
#define STARS_BITMASK_1349_cQuan 0x3ffUL
typedef STARS_CV_1349 STARS_CV_134f[12];
/* _zipprodq1: 26 bytes; each view begins at its recorded offset. */
union STARS_CV_10ef {
    unsigned char raw[26];
    struct { STARS_CV_0020 value; } fNoResearch;
    struct { unsigned char _pad[1]; STARS_CV_0020 value; } cpq;
    struct { unsigned char _pad[2]; STARS_CV_134f value; } rgpq;
};
#define STARS_OFFSET_10ef_fNoResearch 0
#define STARS_OFFSET_10ef_cpq 1
#define STARS_OFFSET_10ef_rgpq 2
/* _player: 192 bytes; each view begins at its recorded offset. */
union STARS_CV_1014 {
    unsigned char raw[192];
    struct { STARS_CV_0070 value; } iPlayer;
    struct { unsigned char _pad[1]; STARS_CV_0070 value; } cShDef;
    struct { unsigned char _pad[2]; STARS_CV_0011 value; } cPlanet;
    struct { unsigned char _pad[4]; STARS_CV_1364 value; } cFleet;
    struct { unsigned char _pad[4]; STARS_CV_1362 value; } cshdefSB;
    struct { unsigned char _pad[6]; STARS_CV_13b3 value; } det;
    struct { unsigned char _pad[6]; STARS_CV_13d2 value; } iPlrBmp;
    struct { unsigned char _pad[6]; STARS_CV_135e value; } fInclude;
    struct { unsigned char _pad[6]; STARS_CV_13d3 value; } mdPlayer;
    struct { unsigned char _pad[6]; STARS_CV_13d4 value; } reserved;
    struct { unsigned char _pad[6]; STARS_CV_135f value; } fAi;
    struct { unsigned char _pad[6]; STARS_CV_13d5 value; } lvlAi;
    struct { unsigned char _pad[6]; STARS_CV_13d0 value; } idAi;
    struct { unsigned char _pad[6]; STARS_CV_0021 value; } wMdPlr;
    struct { unsigned char _pad[8]; STARS_CV_0011 value; } idPlanetHome;
    struct { unsigned char _pad[10]; STARS_CV_0021 value; } wScore;
    struct { unsigned char _pad[12]; STARS_CV_0012 value; } lSalt;
    struct { unsigned char _pad[16]; STARS_CV_10bc value; } rgEnvVar;
    struct { unsigned char _pad[19]; STARS_CV_10bc value; } rgEnvVarMin;
    struct { unsigned char _pad[22]; STARS_CV_10bc value; } rgEnvVarMax;
    struct { unsigned char _pad[25]; STARS_CV_0070 value; } pctIdealGrowth;
    struct { unsigned char _pad[26]; STARS_CV_1270 value; } rgTech;
    struct { unsigned char _pad[32]; STARS_CV_13d6 value; } rgResSpent;
    struct { unsigned char _pad[56]; STARS_CV_0070 value; } pctResearch;
    struct { unsigned char _pad[57]; STARS_CV_0070 value; } iTechCur;
    struct { unsigned char _pad[58]; STARS_CV_0012 value; } lResLastYear;
    struct { unsigned char _pad[62]; STARS_CV_1209 value; } rgAttr;
    struct { unsigned char _pad[78]; STARS_CV_0022 value; } grbitAttr;
    struct { unsigned char _pad[82]; STARS_CV_0021 value; } grbitTrader;
    struct { unsigned char _pad[84]; STARS_CV_13d7 value; } fDead;
    struct { unsigned char _pad[84]; STARS_CV_13d8 value; } fCrippled;
    struct { unsigned char _pad[84]; STARS_CV_13d9 value; } fCheater;
    struct { unsigned char _pad[84]; STARS_CV_13da value; } fLearned;
    struct { unsigned char _pad[84]; STARS_CV_13db value; } fHacker;
    struct { unsigned char _pad[84]; STARS_CV_13dc value; } unused;
    struct { unsigned char _pad[84]; STARS_CV_0021 value; } wFlags;
    struct { unsigned char _pad[86]; STARS_CV_10ef value; } zpq1;
    struct { unsigned char _pad[112]; STARS_CV_1209 value; } rgmdRelation;
    struct { unsigned char _pad[128]; STARS_CV_11e2 value; } szName;
    struct { unsigned char _pad[160]; STARS_CV_11e2 value; } szNames;
};
#define STARS_OFFSET_1014_iPlayer 0
#define STARS_OFFSET_1014_cShDef 1
#define STARS_OFFSET_1014_cPlanet 2
#define STARS_OFFSET_1014_cFleet 4
#define STARS_BITSHIFT_1014_cFleet 0
#define STARS_BITMASK_1014_cFleet 0xfffUL
#define STARS_OFFSET_1014_cshdefSB 4
#define STARS_BITSHIFT_1014_cshdefSB 12
#define STARS_BITMASK_1014_cshdefSB 0xfUL
#define STARS_OFFSET_1014_det 6
#define STARS_BITSHIFT_1014_det 0
#define STARS_BITMASK_1014_det 0x7UL
#define STARS_OFFSET_1014_iPlrBmp 6
#define STARS_BITSHIFT_1014_iPlrBmp 3
#define STARS_BITMASK_1014_iPlrBmp 0x1fUL
#define STARS_OFFSET_1014_fInclude 6
#define STARS_BITSHIFT_1014_fInclude 8
#define STARS_BITMASK_1014_fInclude 0x1UL
#define STARS_OFFSET_1014_mdPlayer 6
#define STARS_BITSHIFT_1014_mdPlayer 9
#define STARS_BITMASK_1014_mdPlayer 0x7fUL
#define STARS_OFFSET_1014_reserved 6
#define STARS_BITSHIFT_1014_reserved 0
#define STARS_BITMASK_1014_reserved 0x1ffUL
#define STARS_OFFSET_1014_fAi 6
#define STARS_BITSHIFT_1014_fAi 9
#define STARS_BITMASK_1014_fAi 0x1UL
#define STARS_OFFSET_1014_lvlAi 6
#define STARS_BITSHIFT_1014_lvlAi 10
#define STARS_BITMASK_1014_lvlAi 0x7UL
#define STARS_OFFSET_1014_idAi 6
#define STARS_BITSHIFT_1014_idAi 13
#define STARS_BITMASK_1014_idAi 0x7UL
#define STARS_OFFSET_1014_wMdPlr 6
#define STARS_OFFSET_1014_idPlanetHome 8
#define STARS_OFFSET_1014_wScore 10
#define STARS_OFFSET_1014_lSalt 12
#define STARS_OFFSET_1014_rgEnvVar 16
#define STARS_OFFSET_1014_rgEnvVarMin 19
#define STARS_OFFSET_1014_rgEnvVarMax 22
#define STARS_OFFSET_1014_pctIdealGrowth 25
#define STARS_OFFSET_1014_rgTech 26
#define STARS_OFFSET_1014_rgResSpent 32
#define STARS_OFFSET_1014_pctResearch 56
#define STARS_OFFSET_1014_iTechCur 57
#define STARS_OFFSET_1014_lResLastYear 58
#define STARS_OFFSET_1014_rgAttr 62
#define STARS_OFFSET_1014_grbitAttr 78
#define STARS_OFFSET_1014_grbitTrader 82
#define STARS_OFFSET_1014_fDead 84
#define STARS_BITSHIFT_1014_fDead 0
#define STARS_BITMASK_1014_fDead 0x1UL
#define STARS_OFFSET_1014_fCrippled 84
#define STARS_BITSHIFT_1014_fCrippled 1
#define STARS_BITMASK_1014_fCrippled 0x1UL
#define STARS_OFFSET_1014_fCheater 84
#define STARS_BITSHIFT_1014_fCheater 2
#define STARS_BITMASK_1014_fCheater 0x1UL
#define STARS_OFFSET_1014_fLearned 84
#define STARS_BITSHIFT_1014_fLearned 3
#define STARS_BITMASK_1014_fLearned 0x1UL
#define STARS_OFFSET_1014_fHacker 84
#define STARS_BITSHIFT_1014_fHacker 4
#define STARS_BITMASK_1014_fHacker 0x1UL
#define STARS_OFFSET_1014_unused 84
#define STARS_BITSHIFT_1014_unused 5
#define STARS_BITMASK_1014_unused 0x7ffUL
#define STARS_OFFSET_1014_wFlags 84
#define STARS_OFFSET_1014_zpq1 86
#define STARS_OFFSET_1014_rgmdRelation 112
#define STARS_OFFSET_1014_szName 128
#define STARS_OFFSET_1014_szNames 160
typedef STARS_CV_1014 STARS_CV_1015[];
typedef STARS_CV_0012 STARS_FAR STARS_PASCAL STARS_CV_1017(STARS_CV_0073, STARS_CV_0073, STARS_CV_0073, STARS_CV_0012);
/* tagRECT: 8 bytes; each view begins at its recorded offset. */
union STARS_CV_101a {
    unsigned char raw[8];
    struct { STARS_CV_0072 value; } left;
    struct { unsigned char _pad[2]; STARS_CV_0072 value; } top;
    struct { unsigned char _pad[4]; STARS_CV_0072 value; } right;
    struct { unsigned char _pad[6]; STARS_CV_0072 value; } bottom;
};
#define STARS_OFFSET_101a_left 0
#define STARS_OFFSET_101a_top 2
#define STARS_OFFSET_101a_right 4
#define STARS_OFFSET_101a_bottom 6
typedef STARS_CV_0020 STARS_CV_103d[16];
/* tagPAINTSTRUCT: 32 bytes; each view begins at its recorded offset. */
union STARS_CV_1019 {
    unsigned char raw[32];
    struct { STARS_CV_0073 value; } hdc;
    struct { unsigned char _pad[2]; STARS_CV_0072 value; } fErase;
    struct { unsigned char _pad[4]; STARS_CV_101a value; } rcPaint;
    struct { unsigned char _pad[12]; STARS_CV_0072 value; } fRestore;
    struct { unsigned char _pad[14]; STARS_CV_0072 value; } fIncUpdate;
    struct { unsigned char _pad[16]; STARS_CV_103d value; } rgbReserved;
};
#define STARS_OFFSET_1019_hdc 0
#define STARS_OFFSET_1019_fErase 2
#define STARS_OFFSET_1019_rcPaint 4
#define STARS_OFFSET_1019_fRestore 12
#define STARS_OFFSET_1019_fIncUpdate 14
#define STARS_OFFSET_1019_rgbReserved 16
#ifdef STARS_NATIVE16
typedef STARS_CV_101a STARS_NEAR *STARS_CV_101b;
#else
typedef unsigned short STARS_CV_101b;
#endif
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_101d(STARS_CV_0073, STARS_CV_101b);
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_101f(STARS_CV_0073, STARS_CV_1018, STARS_CV_0072, STARS_CV_0072);
#ifdef STARS_NATIVE16
typedef STARS_CV_1018 STARS_NEAR *STARS_CV_1020;
#else
typedef unsigned short STARS_CV_1020;
#endif
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_1022(STARS_CV_1020);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_1023(STARS_CV_0072);
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_1025(STARS_CV_0072, STARS_CV_0072);
typedef STARS_CV_0170 STARS_CV_1026[12];
typedef STARS_CV_0012 STARS_CV_1027[12];
typedef STARS_CV_0170 STARS_CV_1028[20];
typedef STARS_CV_0012 STARS_CV_1029[20];
typedef STARS_CV_0170 STARS_CV_102a[9];
typedef STARS_CV_0012 STARS_CV_102b[9];
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_102d(STARS_CV_0072, STARS_CV_101b);
typedef STARS_CV_0012 STARS_FAR STARS_PASCAL STARS_CV_102f(STARS_CV_0073, STARS_CV_0021, STARS_CV_0021, STARS_CV_0012);
typedef STARS_CV_0072 STARS_CV_1030[9];
typedef STARS_CV_1008 STARS_CV_1031[];
typedef STARS_CV_0073 STARS_CV_1032[];
typedef STARS_CV_0070 STARS_CV_1033[];
typedef STARS_CV_0070 STARS_CV_1034[29];
typedef STARS_CV_0072 STARS_FAR STARS_PASCAL STARS_CV_1036();
#ifdef STARS_NATIVE16
typedef STARS_CV_1036 STARS_FAR *STARS_CV_1037;
#else
typedef STARS_ULONG32 STARS_CV_1037;
#endif
/* _framestuff: 22 bytes; each view begins at its recorded offset. */
union STARS_CV_1038 {
    unsigned char raw[22];
    struct { STARS_CV_0072 value; } dx;
    struct { unsigned char _pad[2]; STARS_CV_0072 value; } dy;
    struct { unsigned char _pad[4]; STARS_CV_0072 value; } xTop;
    struct { unsigned char _pad[6]; STARS_CV_0072 value; } y1;
    struct { unsigned char _pad[8]; STARS_CV_0072 value; } y2;
    struct { unsigned char _pad[10]; STARS_CV_0072 value; } dxPlanWant;
    struct { unsigned char _pad[12]; STARS_CV_0072 value; } dyMsgWant;
    struct { unsigned char _pad[14]; STARS_CV_0072 value; } dyMinWant;
    struct { unsigned char _pad[16]; STARS_CV_0072 value; } dx2PlanWant;
    struct { unsigned char _pad[18]; STARS_CV_0072 value; } dy2MsgWant;
    struct { unsigned char _pad[20]; STARS_CV_0072 value; } dy2MinWant;
};
#define STARS_OFFSET_1038_dx 0
#define STARS_OFFSET_1038_dy 2
#define STARS_OFFSET_1038_xTop 4
#define STARS_OFFSET_1038_y1 6
#define STARS_OFFSET_1038_y2 8
#define STARS_OFFSET_1038_dxPlanWant 10
#define STARS_OFFSET_1038_dyMsgWant 12
#define STARS_OFFSET_1038_dyMinWant 14
#define STARS_OFFSET_1038_dx2PlanWant 16
#define STARS_OFFSET_1038_dy2MsgWant 18
#define STARS_OFFSET_1038_dy2MinWant 20
typedef STARS_CV_0022 STARS_CV_139e; /* bit-field backing storage; use recorded bit positions/masks */
typedef STARS_CV_0022 STARS_CV_1555; /* bit-field backing storage; use recorded bit positions/masks */
typedef STARS_CV_0022 STARS_CV_1556; /* bit-field backing storage; use recorded bit positions/masks */
typedef STARS_CV_0022 STARS_CV_1557; /* bit-field backing storage; use recorded bit positions/masks */
typedef STARS_CV_0022 STARS_CV_1558; /* bit-field backing storage; use recorded bit positions/masks */
/* _prod: 4 bytes; each view begins at its recorded offset. */
union STARS_CV_1039 {
    unsigned char raw[4];
    struct { STARS_CV_139e value; } cItem;
    struct { STARS_CV_1555 value; } iItem;
    struct { STARS_CV_1556 value; } grobj;
    struct { STARS_CV_1557 value; } pct;
    struct { STARS_CV_1558 value; } unused;
};
#define STARS_OFFSET_1039_cItem 0
#define STARS_BITSHIFT_1039_cItem 0
#define STARS_BITMASK_1039_cItem 0x3ffUL
#define STARS_OFFSET_1039_iItem 0
#define STARS_BITSHIFT_1039_iItem 10
#define STARS_BITMASK_1039_iItem 0x7fUL
#define STARS_OFFSET_1039_grobj 0
#define STARS_BITSHIFT_1039_grobj 17
#define STARS_BITMASK_1039_grobj 0x7UL
#define STARS_OFFSET_1039_pct 0
#define STARS_BITSHIFT_1039_pct 20
#define STARS_BITMASK_1039_pct 0x7fUL
#define STARS_OFFSET_1039_unused 0
#define STARS_BITSHIFT_1039_unused 27
#define STARS_BITMASK_1039_unused 0x1fUL
#ifdef STARS_NATIVE16
typedef STARS_CV_1039 STARS_NEAR *STARS_CV_103a;
#else
typedef unsigned short STARS_CV_103a;
#endif
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_103c(STARS_CV_103a);
typedef STARS_CV_0021 STARS_CV_147c; /* bit-field backing storage; use recorded bit positions/masks */
/* _cyberinfo: 2 bytes; each view begins at its recorded offset. */
union STARS_CV_103e {
    unsigned char raw[2];
    struct { STARS_CV_13b3 value; } iLstPktDir;
    struct { STARS_CV_13da value; } fBltColony;
    struct { STARS_CV_13db value; } fLaunchedPkt;
    struct { STARS_CV_147c value; } iPktTarget;
    struct { STARS_CV_13b5 value; } fNeedScanPkt;
    struct { STARS_CV_1411 value; } unused;
    struct { STARS_CV_0021 value; } wInfo;
};
#define STARS_OFFSET_103e_iLstPktDir 0
#define STARS_BITSHIFT_103e_iLstPktDir 0
#define STARS_BITMASK_103e_iLstPktDir 0x7UL
#define STARS_OFFSET_103e_fBltColony 0
#define STARS_BITSHIFT_103e_fBltColony 3
#define STARS_BITMASK_103e_fBltColony 0x1UL
#define STARS_OFFSET_103e_fLaunchedPkt 0
#define STARS_BITSHIFT_103e_fLaunchedPkt 4
#define STARS_BITMASK_103e_fLaunchedPkt 0x1UL
#define STARS_OFFSET_103e_iPktTarget 0
#define STARS_BITSHIFT_103e_iPktTarget 5
#define STARS_BITMASK_103e_iPktTarget 0x3UL
#define STARS_OFFSET_103e_fNeedScanPkt 0
#define STARS_BITSHIFT_103e_fNeedScanPkt 7
#define STARS_BITMASK_103e_fNeedScanPkt 0x1UL
#define STARS_OFFSET_103e_unused 0
#define STARS_BITSHIFT_103e_unused 8
#define STARS_BITMASK_103e_unused 0xffUL
#define STARS_OFFSET_103e_wInfo 0
#ifdef STARS_NATIVE16
typedef STARS_CV_103e STARS_FAR *STARS_CV_103f;
#else
typedef STARS_ULONG32 STARS_CV_103f;
#endif
typedef STARS_CV_0021 STARS_CV_14b8; /* bit-field backing storage; use recorded bit positions/masks */
typedef STARS_CV_0021 STARS_CV_14b9; /* bit-field backing storage; use recorded bit positions/masks */
typedef STARS_CV_0021 STARS_CV_14bc; /* bit-field backing storage; use recorded bit positions/masks */
/* _cyberinfotemp: 2 bytes; each view begins at its recorded offset. */
union STARS_CV_1040 {
    unsigned char raw[2];
    struct { STARS_CV_13d7 value; } fIdleColonizers;
    struct { STARS_CV_14b8 value; } cIdleFreighters;
    struct { STARS_CV_14b9 value; } cFreightersDst;
    struct { STARS_CV_14ba value; } fNeedDefenders;
    struct { STARS_CV_14bb value; } fDefended;
    struct { STARS_CV_13b5 value; } fUnderAttack;
    struct { STARS_CV_135e value; } fNeedsMin1;
    struct { STARS_CV_135f value; } fNeedsMin2;
    struct { STARS_CV_1360 value; } fNeedsMin3;
    struct { STARS_CV_14bc value; } unused;
    struct { STARS_CV_0021 value; } wInfo1;
};
#define STARS_OFFSET_1040_fIdleColonizers 0
#define STARS_BITSHIFT_1040_fIdleColonizers 0
#define STARS_BITMASK_1040_fIdleColonizers 0x1UL
#define STARS_OFFSET_1040_cIdleFreighters 0
#define STARS_BITSHIFT_1040_cIdleFreighters 1
#define STARS_BITMASK_1040_cIdleFreighters 0x3UL
#define STARS_OFFSET_1040_cFreightersDst 0
#define STARS_BITSHIFT_1040_cFreightersDst 3
#define STARS_BITMASK_1040_cFreightersDst 0x3UL
#define STARS_OFFSET_1040_fNeedDefenders 0
#define STARS_BITSHIFT_1040_fNeedDefenders 5
#define STARS_BITMASK_1040_fNeedDefenders 0x1UL
#define STARS_OFFSET_1040_fDefended 0
#define STARS_BITSHIFT_1040_fDefended 6
#define STARS_BITMASK_1040_fDefended 0x1UL
#define STARS_OFFSET_1040_fUnderAttack 0
#define STARS_BITSHIFT_1040_fUnderAttack 7
#define STARS_BITMASK_1040_fUnderAttack 0x1UL
#define STARS_OFFSET_1040_fNeedsMin1 0
#define STARS_BITSHIFT_1040_fNeedsMin1 8
#define STARS_BITMASK_1040_fNeedsMin1 0x1UL
#define STARS_OFFSET_1040_fNeedsMin2 0
#define STARS_BITSHIFT_1040_fNeedsMin2 9
#define STARS_BITMASK_1040_fNeedsMin2 0x1UL
#define STARS_OFFSET_1040_fNeedsMin3 0
#define STARS_BITSHIFT_1040_fNeedsMin3 10
#define STARS_BITMASK_1040_fNeedsMin3 0x1UL
#define STARS_OFFSET_1040_unused 0
#define STARS_BITSHIFT_1040_unused 11
#define STARS_BITMASK_1040_unused 0x1fUL
#define STARS_OFFSET_1040_wInfo1 0
#ifdef STARS_NATIVE16
typedef STARS_CV_1040 STARS_FAR *STARS_CV_1041;
#else
typedef STARS_ULONG32 STARS_CV_1041;
#endif
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_1042();
typedef STARS_CV_1039 STARS_CV_1043[64];
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_1046(STARS_CV_100b, STARS_CV_0072);
/* _scan: 16 bytes; each view begins at its recorded offset. */
union STARS_CV_1047 {
    unsigned char raw[16];
    struct { STARS_CV_1018 value; } pt;
    struct { unsigned char _pad[4]; STARS_CV_0072 value; } grobj;
    struct { unsigned char _pad[6]; STARS_CV_0072 value; } grobjFull;
    struct { unsigned char _pad[8]; STARS_CV_0072 value; } idpl;
    struct { unsigned char _pad[10]; STARS_CV_0072 value; } ifl;
    struct { unsigned char _pad[12]; STARS_CV_0072 value; } iwp;
    struct { unsigned char _pad[14]; STARS_CV_0072 value; } ith;
};
#define STARS_OFFSET_1047_pt 0
#define STARS_OFFSET_1047_grobj 4
#define STARS_OFFSET_1047_grobjFull 6
#define STARS_OFFSET_1047_idpl 8
#define STARS_OFFSET_1047_ifl 10
#define STARS_OFFSET_1047_iwp 12
#define STARS_OFFSET_1047_ith 14
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_1049(STARS_CV_100b);
#ifdef STARS_NATIVE16
typedef STARS_CV_1039 STARS_FAR *STARS_CV_104a;
#else
typedef STARS_ULONG32 STARS_CV_104a;
#endif
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_104c(STARS_CV_1004, STARS_CV_1041);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_104e(STARS_CV_100b, STARS_CV_100b);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_1050(STARS_CV_100b, STARS_CV_0072, STARS_CV_0072, STARS_CV_0072, STARS_CV_0072);
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_1052(STARS_CV_1004);
typedef STARS_CV_0020 STARS_CV_1053[4];
/* PLPROD: 4 bytes; each view begins at its recorded offset. */
union STARS_CV_1054 {
    unsigned char raw[4];
    struct { STARS_CV_135d value; } cbItem;
    struct { STARS_CV_135e value; } fMark;
    struct { STARS_CV_13a9 value; } ht;
    struct { STARS_CV_1362 value; } cAlloc;
    struct { unsigned char _pad[2]; STARS_CV_0020 value; } iprodMax;
    struct { unsigned char _pad[3]; STARS_CV_0020 value; } iprodMac;
    /* rgprod: variable tail at offset 4; no fixed storage. */
};
#define STARS_OFFSET_1054_cbItem 0
#define STARS_BITSHIFT_1054_cbItem 0
#define STARS_BITMASK_1054_cbItem 0xffUL
#define STARS_OFFSET_1054_fMark 0
#define STARS_BITSHIFT_1054_fMark 8
#define STARS_BITMASK_1054_fMark 0x1UL
#define STARS_OFFSET_1054_ht 0
#define STARS_BITSHIFT_1054_ht 9
#define STARS_BITMASK_1054_ht 0x7UL
#define STARS_OFFSET_1054_cAlloc 0
#define STARS_BITSHIFT_1054_cAlloc 12
#define STARS_BITMASK_1054_cAlloc 0xfUL
#define STARS_OFFSET_1054_iprodMax 2
#define STARS_OFFSET_1054_iprodMac 3
#define STARS_OFFSET_1054_rgprod 4
#ifdef STARS_NATIVE16
typedef STARS_CV_100b STARS_FAR *STARS_CV_1056;
#else
typedef STARS_ULONG32 STARS_CV_1056;
#endif
typedef STARS_CV_1018 STARS_CV_1057[];
typedef STARS_CV_0020 STARS_CV_1058[42];
typedef STARS_CV_0021 STARS_CV_1059[36];
typedef STARS_CV_0020 STARS_CV_105a[301];
/* _sel: 226 bytes; each view begins at its recorded offset. */
union STARS_CV_105b {
    unsigned char raw[226];
    struct { STARS_CV_1018 value; } pt;
    struct { unsigned char _pad[4]; STARS_CV_0011 value; } grobj;
    struct { unsigned char _pad[6]; STARS_CV_0011 value; } grobjFull;
    struct { unsigned char _pad[8]; STARS_CV_0011 value; } id;
    struct { unsigned char _pad[10]; STARS_CV_0011 value; } iwpAct;
    struct { unsigned char _pad[12]; STARS_CV_1047 value; } scan;
    struct { unsigned char _pad[28]; STARS_CV_1003 value; } fl;
    struct { unsigned char _pad[152]; STARS_CV_1002 value; } pl;
    struct { unsigned char _pad[208]; STARS_CV_1006 value; } th;
};
#define STARS_OFFSET_105b_pt 0
#define STARS_OFFSET_105b_grobj 4
#define STARS_OFFSET_105b_grobjFull 6
#define STARS_OFFSET_105b_id 8
#define STARS_OFFSET_105b_iwpAct 10
#define STARS_OFFSET_105b_scan 12
#define STARS_OFFSET_105b_fl 28
#define STARS_OFFSET_105b_pl 152
#define STARS_OFFSET_105b_th 208
typedef STARS_CV_0021 STARS_CV_105c[4];
#ifdef STARS_NATIVE16
typedef STARS_CV_1415 STARS_FAR *STARS_CV_155a;
#else
typedef STARS_ULONG32 STARS_CV_155a;
#endif
#ifdef STARS_NATIVE16
typedef STARS_CV_1299 STARS_FAR *STARS_CV_155b;
#else
typedef STARS_ULONG32 STARS_CV_155b;
#endif
#ifdef STARS_NATIVE16
typedef STARS_CV_1076 STARS_FAR *STARS_CV_1077;
#else
typedef STARS_ULONG32 STARS_CV_1077;
#endif
#ifdef STARS_NATIVE16
typedef STARS_CV_121c STARS_FAR *STARS_CV_121d;
#else
typedef STARS_ULONG32 STARS_CV_121d;
#endif
#ifdef STARS_NATIVE16
typedef STARS_CV_128d STARS_FAR *STARS_CV_128e;
#else
typedef STARS_ULONG32 STARS_CV_128e;
#endif
#ifdef STARS_NATIVE16
typedef STARS_CV_12ac STARS_FAR *STARS_CV_155c;
#else
typedef STARS_ULONG32 STARS_CV_155c;
#endif
#ifdef STARS_NATIVE16
typedef STARS_CV_12aa STARS_FAR *STARS_CV_155d;
#else
typedef STARS_ULONG32 STARS_CV_155d;
#endif
#ifdef STARS_NATIVE16
typedef STARS_CV_12a8 STARS_FAR *STARS_CV_155e;
#else
typedef STARS_ULONG32 STARS_CV_155e;
#endif
#ifdef STARS_NATIVE16
typedef STARS_CV_129c STARS_FAR *STARS_CV_155f;
#else
typedef STARS_ULONG32 STARS_CV_155f;
#endif
#ifdef STARS_NATIVE16
typedef STARS_CV_129e STARS_FAR *STARS_CV_1560;
#else
typedef STARS_ULONG32 STARS_CV_1560;
#endif
#ifdef STARS_NATIVE16
typedef STARS_CV_12b2 STARS_FAR *STARS_CV_1561;
#else
typedef STARS_ULONG32 STARS_CV_1561;
#endif
#ifdef STARS_NATIVE16
typedef STARS_CV_12a1 STARS_FAR *STARS_CV_1562;
#else
typedef STARS_ULONG32 STARS_CV_1562;
#endif
#ifdef STARS_NATIVE16
typedef STARS_CV_12a3 STARS_FAR *STARS_CV_1563;
#else
typedef STARS_ULONG32 STARS_CV_1563;
#endif
#ifdef STARS_NATIVE16
typedef STARS_CV_1291 STARS_FAR *STARS_CV_1292;
#else
typedef STARS_ULONG32 STARS_CV_1292;
#endif
#ifdef STARS_NATIVE16
typedef STARS_CV_12a6 STARS_FAR *STARS_CV_1564;
#else
typedef STARS_ULONG32 STARS_CV_1564;
#endif
/* _part: 8 bytes; each view begins at its recorded offset. */
union STARS_CV_105e {
    unsigned char raw[8];
    struct { STARS_CV_1078 value; } hs;
    struct { unsigned char _pad[4]; STARS_CV_155a value; } pcom;
    struct { unsigned char _pad[4]; STARS_CV_155b value; } parmor;
    struct { unsigned char _pad[4]; STARS_CV_1077 value; } phul;
    struct { unsigned char _pad[4]; STARS_CV_121d value; } pengine;
    struct { unsigned char _pad[4]; STARS_CV_128e value; } pscanner;
    struct { unsigned char _pad[4]; STARS_CV_155c value; } pbeam;
    struct { unsigned char _pad[4]; STARS_CV_155d value; } ptorp;
    struct { unsigned char _pad[4]; STARS_CV_155e value; } pbomb;
    struct { unsigned char _pad[4]; STARS_CV_155f value; } pshield;
    struct { unsigned char _pad[4]; STARS_CV_1560 value; } pspecial;
    struct { unsigned char _pad[4]; STARS_CV_1561 value; } pspecialsb;
    struct { unsigned char _pad[4]; STARS_CV_1562 value; } pmines;
    struct { unsigned char _pad[4]; STARS_CV_1563 value; } pmining;
    struct { unsigned char _pad[4]; STARS_CV_1292 value; } pplanetary;
    struct { unsigned char _pad[4]; STARS_CV_1564 value; } pterra;
};
#define STARS_OFFSET_105e_hs 0
#define STARS_OFFSET_105e_pcom 4
#define STARS_OFFSET_105e_parmor 4
#define STARS_OFFSET_105e_phul 4
#define STARS_OFFSET_105e_pengine 4
#define STARS_OFFSET_105e_pscanner 4
#define STARS_OFFSET_105e_pbeam 4
#define STARS_OFFSET_105e_ptorp 4
#define STARS_OFFSET_105e_pbomb 4
#define STARS_OFFSET_105e_pshield 4
#define STARS_OFFSET_105e_pspecial 4
#define STARS_OFFSET_105e_pspecialsb 4
#define STARS_OFFSET_105e_pmines 4
#define STARS_OFFSET_105e_pmining 4
#define STARS_OFFSET_105e_pplanetary 4
#define STARS_OFFSET_105e_pterra 4
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_105f(STARS_CV_1004);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_1061(STARS_CV_1004, STARS_CV_0172);
typedef STARS_CV_0021 STARS_CV_1063[31];
typedef STARS_CV_0020 STARS_CV_1064[248];
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_1066(STARS_CV_1004, STARS_CV_0072);
typedef STARS_CV_0020 STARS_CV_1067[18];
typedef STARS_CV_0020 STARS_CV_1068[19];
typedef STARS_CV_0020 STARS_CV_1069[182];
typedef STARS_CV_0072 STARS_FAR STARS_PASCAL STARS_CV_106b(STARS_CV_0073, STARS_CV_0073, STARS_CV_0021, STARS_CV_0012);
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_106d(STARS_CV_0073, STARS_CV_0072);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_106f(STARS_CV_1004, STARS_CV_0072, STARS_CV_0072, STARS_CV_0072);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_1071(STARS_CV_0072, STARS_CV_0072, STARS_CV_0072, STARS_CV_0072, STARS_CV_0172);
#ifdef STARS_NATIVE16
typedef STARS_CV_1005 STARS_FAR *STARS_CV_1072;
#else
typedef STARS_ULONG32 STARS_CV_1072;
#endif
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_1074(STARS_CV_1004, STARS_CV_100b);
typedef STARS_CV_0012 STARS_FAR STARS_CDECL STARS_CV_1075(STARS_CV_1004);
#ifdef STARS_NATIVE16
typedef STARS_CV_1078 STARS_FAR *STARS_CV_1079;
#else
typedef STARS_ULONG32 STARS_CV_1079;
#endif
typedef STARS_CV_0012 STARS_FAR STARS_CDECL STARS_CV_107b(STARS_CV_1004, STARS_CV_0072, STARS_CV_0072);
typedef STARS_CV_0012 STARS_FAR STARS_CDECL STARS_CV_107d(STARS_CV_1077);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_107f(STARS_CV_1079);
typedef STARS_CV_0070 STARS_CV_1080[80];
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_1082(STARS_CV_1077, STARS_CV_0072);
typedef STARS_CV_0022 STARS_CV_1083[];
typedef STARS_CV_0170 STARS_CV_1084[];
typedef STARS_CV_0070 STARS_CV_116c[13];
/* _ziporder: 24 bytes; each view begins at its recorded offset. */
union STARS_CV_1085 {
    unsigned char raw[24];
    struct { STARS_CV_137f value; } txp;
    struct { unsigned char _pad[10]; STARS_CV_116c value; } szName;
    struct { unsigned char _pad[23]; STARS_CV_0020 value; } fValid;
};
#define STARS_OFFSET_1085_txp 0
#define STARS_OFFSET_1085_szName 10
#define STARS_OFFSET_1085_fValid 23
typedef STARS_CV_1085 STARS_CV_1086[4];
typedef STARS_CV_1007 STARS_FAR STARS_CDECL STARS_CV_1087(STARS_CV_0072, STARS_CV_0072);
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_1089(STARS_CV_1007);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_108b(STARS_CV_1018, STARS_CV_0012);
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_108d(STARS_CV_0073, STARS_CV_101b, STARS_CV_1007, STARS_CV_0072);
typedef STARS_CV_0073 STARS_CV_108e[5];
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_1090(STARS_CV_1007);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_1092(STARS_CV_0021, STARS_CV_0072, STARS_CV_0121);
typedef STARS_CV_0012 STARS_FAR STARS_PASCAL STARS_CV_1093(STARS_CV_0073, STARS_CV_0073, STARS_CV_0021, STARS_CV_0012);
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_1095(STARS_CV_0073, STARS_CV_0073, STARS_CV_101b);
typedef STARS_CV_0070 STARS_CV_1096[40];
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_1098(STARS_CV_0073);
typedef STARS_CV_0070 STARS_CV_1099[100];
typedef STARS_CV_0021 STARS_CV_14c3; /* bit-field backing storage; use recorded bit positions/masks */
typedef STARS_CV_0021 STARS_CV_14c4; /* bit-field backing storage; use recorded bit positions/masks */
/* _score: 20 bytes; each view begins at its recorded offset. */
union STARS_CV_1336 {
    unsigned char raw[20];
    struct { STARS_CV_0012 value; } lScore;
    struct { unsigned char _pad[4]; STARS_CV_0012 value; } cResources;
    struct { unsigned char _pad[8]; STARS_CV_0072 value; } cPlanet;
    struct { unsigned char _pad[10]; STARS_CV_0072 value; } cStarbase;
    struct { unsigned char _pad[12]; STARS_CV_1363 value; } rgcsh;
    struct { unsigned char _pad[18]; STARS_CV_0072 value; } cTechLevels;
};
#define STARS_OFFSET_1336_lScore 0
#define STARS_OFFSET_1336_cResources 4
#define STARS_OFFSET_1336_cPlanet 8
#define STARS_OFFSET_1336_cStarbase 10
#define STARS_OFFSET_1336_rgcsh 12
#define STARS_OFFSET_1336_cTechLevels 18
/* _scorex: 24 bytes; each view begins at its recorded offset. */
union STARS_CV_109a {
    unsigned char raw[24];
    struct { STARS_CV_0021 value; } wWord;
    struct { STARS_CV_14c3 value; } iPlayer;
    struct { STARS_CV_14ba value; } fValid;
    struct { STARS_CV_14c4 value; } grbitVC;
    struct { STARS_CV_1395 value; } fWinner;
    struct { STARS_CV_1396 value; } fHistory;
    struct { unsigned char _pad[2]; STARS_CV_0072 value; } iRank;
    struct { unsigned char _pad[2]; STARS_CV_0021 value; } turn;
    struct { unsigned char _pad[4]; STARS_CV_1336 value; } score;
};
#define STARS_OFFSET_109a_wWord 0
#define STARS_OFFSET_109a_iPlayer 0
#define STARS_BITSHIFT_109a_iPlayer 0
#define STARS_BITMASK_109a_iPlayer 0x1fUL
#define STARS_OFFSET_109a_fValid 0
#define STARS_BITSHIFT_109a_fValid 5
#define STARS_BITMASK_109a_fValid 0x1UL
#define STARS_OFFSET_109a_grbitVC 0
#define STARS_BITSHIFT_109a_grbitVC 6
#define STARS_BITMASK_109a_grbitVC 0xffUL
#define STARS_OFFSET_109a_fWinner 0
#define STARS_BITSHIFT_109a_fWinner 14
#define STARS_BITMASK_109a_fWinner 0x1UL
#define STARS_OFFSET_109a_fHistory 0
#define STARS_BITSHIFT_109a_fHistory 15
#define STARS_BITMASK_109a_fHistory 0x1UL
#define STARS_OFFSET_109a_iRank 2
#define STARS_OFFSET_109a_turn 2
#define STARS_OFFSET_109a_score 4
#ifdef STARS_NATIVE16
typedef STARS_CV_109a STARS_FAR *STARS_CV_109b;
#else
typedef STARS_ULONG32 STARS_CV_109b;
#endif
typedef STARS_CV_0012 STARS_FAR STARS_CDECL STARS_CV_109d(STARS_CV_109b, STARS_CV_0072);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_109f(STARS_CV_0072, STARS_CV_0072, STARS_CV_0170, STARS_CV_0073);
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_10a1(STARS_CV_0073, STARS_CV_101b, STARS_CV_0072, STARS_CV_0072, STARS_CV_0072);
/* _btldata: 14 bytes; each view begins at its recorded offset. */
union STARS_CV_10a2 {
    unsigned char raw[14];
    struct { STARS_CV_0021 value; } id;
    struct { unsigned char _pad[2]; STARS_CV_0020 value; } cplr;
    struct { unsigned char _pad[3]; STARS_CV_0020 value; } ctok;
    struct { unsigned char _pad[4]; STARS_CV_0021 value; } grfPlr;
    struct { unsigned char _pad[6]; STARS_CV_0021 value; } cbData;
    struct { unsigned char _pad[8]; STARS_CV_0021 value; } idPlanet;
    struct { unsigned char _pad[10]; STARS_CV_1018 value; } pt;
    /* rgtok: variable tail at offset 14; no fixed storage. */
};
#define STARS_OFFSET_10a2_id 0
#define STARS_OFFSET_10a2_cplr 2
#define STARS_OFFSET_10a2_ctok 3
#define STARS_OFFSET_10a2_grfPlr 4
#define STARS_OFFSET_10a2_cbData 6
#define STARS_OFFSET_10a2_idPlanet 8
#define STARS_OFFSET_10a2_pt 10
#define STARS_OFFSET_10a2_rgtok 14
#ifdef STARS_NATIVE16
typedef STARS_CV_10a2 STARS_FAR *STARS_CV_10a3;
#else
typedef STARS_ULONG32 STARS_CV_10a3;
#endif
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_10a5(STARS_CV_0073, STARS_CV_0072, STARS_CV_0072, STARS_CV_0072, STARS_CV_0012);
typedef STARS_CV_0170 STARS_FAR STARS_CDECL STARS_CV_10a7(STARS_CV_1004, STARS_CV_0073);
typedef STARS_CV_0170 STARS_FAR STARS_CDECL STARS_CV_10a9(STARS_CV_0073, STARS_CV_1004, STARS_CV_0172);
typedef STARS_CV_0170 STARS_FAR STARS_CDECL STARS_CV_10aa(STARS_CV_1004, STARS_CV_0172);
typedef STARS_CV_0021 STARS_CV_10ab[1024];
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_10ad(STARS_CV_0103, STARS_CV_0103);
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_10af(STARS_CV_1018, STARS_CV_0072, STARS_CV_0072);
typedef STARS_CV_0070 STARS_CV_10b0[50];
typedef STARS_CV_10b0 STARS_CV_10b1[32];
typedef STARS_CV_0072 STARS_CV_10b2[32];
typedef STARS_CV_0170 STARS_CV_10b3[32];
/* _rpt: 54 bytes; each view begins at its recorded offset. */
union STARS_CV_10b4 {
    unsigned char raw[54];
    struct { STARS_CV_0012 value; } grbitVisible;
    struct { unsigned char _pad[4]; STARS_CV_0011 value; } irpt;
    struct { unsigned char _pad[6]; STARS_CV_0011 value; } cFields;
    struct { unsigned char _pad[8]; STARS_CV_0011 value; } cFieldFirst;
    struct { unsigned char _pad[10]; STARS_CV_0011 value; } icolSort;
    struct { unsigned char _pad[12]; STARS_CV_0011 value; } fAscending;
    struct { unsigned char _pad[14]; STARS_CV_0011 value; } irowFirst;
    struct { unsigned char _pad[16]; STARS_CV_1018 value; } ptDlg;
    struct { unsigned char _pad[20]; STARS_CV_1018 value; } ptSize;
    struct { unsigned char _pad[24]; STARS_CV_0011 value; } fCached;
    struct { unsigned char _pad[26]; STARS_CV_103d value; } rgbdx;
    struct { unsigned char _pad[42]; STARS_CV_0011 value; } cRows;
    struct { unsigned char _pad[44]; STARS_CV_0011 value; } cRowsVis;
    struct { unsigned char _pad[46]; STARS_CV_0011 value; } iSubsort;
    struct { unsigned char _pad[48]; STARS_CV_0073 value; } hwndVScroll;
    struct { unsigned char _pad[50]; STARS_CV_0073 value; } hwndHScroll;
    struct { unsigned char _pad[52]; STARS_CV_0011 value; } cColScroll;
};
#define STARS_OFFSET_10b4_grbitVisible 0
#define STARS_OFFSET_10b4_irpt 4
#define STARS_OFFSET_10b4_cFields 6
#define STARS_OFFSET_10b4_cFieldFirst 8
#define STARS_OFFSET_10b4_icolSort 10
#define STARS_OFFSET_10b4_fAscending 12
#define STARS_OFFSET_10b4_irowFirst 14
#define STARS_OFFSET_10b4_ptDlg 16
#define STARS_OFFSET_10b4_ptSize 20
#define STARS_OFFSET_10b4_fCached 24
#define STARS_OFFSET_10b4_rgbdx 26
#define STARS_OFFSET_10b4_cRows 42
#define STARS_OFFSET_10b4_cRowsVis 44
#define STARS_OFFSET_10b4_iSubsort 46
#define STARS_OFFSET_10b4_hwndVScroll 48
#define STARS_OFFSET_10b4_hwndHScroll 50
#define STARS_OFFSET_10b4_cColScroll 52
#ifdef STARS_NATIVE16
typedef STARS_CV_10b4 STARS_NEAR *STARS_CV_10b5;
#else
typedef unsigned short STARS_CV_10b5;
#endif
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_10b7(STARS_CV_1018, STARS_CV_0072, STARS_CV_0072, STARS_CV_0072);
#ifdef STARS_NATIVE16
typedef STARS_CV_1030 STARS_NEAR *STARS_CV_10b8;
#else
typedef unsigned short STARS_CV_10b8;
#endif
typedef STARS_CV_0070 STARS_CV_10b9[256];
typedef STARS_CV_109b STARS_CV_10ba[16];
typedef STARS_CV_0022 STARS_CV_10bb[16];
typedef STARS_CV_0072 STARS_CV_10bd[];
typedef STARS_CV_0021 STARS_CV_1492; /* bit-field backing storage; use recorded bit positions/masks */
typedef STARS_CV_0021 STARS_CV_1493; /* bit-field backing storage; use recorded bit positions/masks */
typedef STARS_CV_0020 STARS_CV_11df[32];
/* _btlplan: 36 bytes; each view begins at its recorded offset. */
union STARS_CV_10be {
    unsigned char raw[36];
    struct { STARS_CV_1378 value; } iplr;
    struct { STARS_CV_1379 value; } iplan;
    struct { STARS_CV_137a value; } mdTactic;
    struct { STARS_CV_1492 value; } unused1;
    struct { STARS_CV_1395 value; } fDelete;
    struct { STARS_CV_1396 value; } fDumpCargo;
    struct { unsigned char _pad[2]; STARS_CV_1378 value; } mdTarget1;
    struct { unsigned char _pad[2]; STARS_CV_1379 value; } mdTarget2;
    struct { unsigned char _pad[2]; STARS_CV_1493 value; } iplrAttack;
    struct { unsigned char _pad[2]; STARS_CV_13d0 value; } unused2;
    struct { unsigned char _pad[4]; STARS_CV_11df value; } szName;
};
#define STARS_OFFSET_10be_iplr 0
#define STARS_BITSHIFT_10be_iplr 0
#define STARS_BITMASK_10be_iplr 0xfUL
#define STARS_OFFSET_10be_iplan 0
#define STARS_BITSHIFT_10be_iplan 4
#define STARS_BITMASK_10be_iplan 0xfUL
#define STARS_OFFSET_10be_mdTactic 0
#define STARS_BITSHIFT_10be_mdTactic 8
#define STARS_BITMASK_10be_mdTactic 0xfUL
#define STARS_OFFSET_10be_unused1 0
#define STARS_BITSHIFT_10be_unused1 12
#define STARS_BITMASK_10be_unused1 0x3UL
#define STARS_OFFSET_10be_fDelete 0
#define STARS_BITSHIFT_10be_fDelete 14
#define STARS_BITMASK_10be_fDelete 0x1UL
#define STARS_OFFSET_10be_fDumpCargo 0
#define STARS_BITSHIFT_10be_fDumpCargo 15
#define STARS_BITMASK_10be_fDumpCargo 0x1UL
#define STARS_OFFSET_10be_mdTarget1 2
#define STARS_BITSHIFT_10be_mdTarget1 0
#define STARS_BITMASK_10be_mdTarget1 0xfUL
#define STARS_OFFSET_10be_mdTarget2 2
#define STARS_BITSHIFT_10be_mdTarget2 4
#define STARS_BITMASK_10be_mdTarget2 0xfUL
#define STARS_OFFSET_10be_iplrAttack 2
#define STARS_BITSHIFT_10be_iplrAttack 8
#define STARS_BITMASK_10be_iplrAttack 0x1fUL
#define STARS_OFFSET_10be_unused2 2
#define STARS_BITSHIFT_10be_unused2 13
#define STARS_BITMASK_10be_unused2 0x7UL
#define STARS_OFFSET_10be_szName 4
#ifdef STARS_NATIVE16
typedef STARS_CV_10be STARS_FAR *STARS_CV_10bf;
#else
typedef STARS_ULONG32 STARS_CV_10bf;
#endif
typedef STARS_CV_10bf STARS_CV_10c0[16];
typedef STARS_CV_0073 STARS_CV_10c1[12];
typedef STARS_CV_0072 STARS_CV_10c2[2];
/* _popupdata: 22 bytes; each view begins at its recorded offset. */
union STARS_CV_10c3 {
    unsigned char raw[22];
    struct { STARS_CV_0072 value; } grPopup;
    struct { unsigned char _pad[2]; STARS_CV_108f value; } rgi;
    struct { unsigned char _pad[2]; STARS_CV_0072 value; } idPlanet;
    struct { unsigned char _pad[4]; STARS_CV_0072 value; } iPlanetVar;
    struct { unsigned char _pad[6]; STARS_CV_0072 value; } iPlanVal;
    struct { unsigned char _pad[8]; STARS_CV_0072 value; } iPlanMin;
    struct { unsigned char _pad[10]; STARS_CV_0072 value; } iPlanMax;
    struct { unsigned char _pad[12]; STARS_CV_0072 value; } iPlrVal;
    struct { unsigned char _pad[14]; STARS_CV_0072 value; } iPlrMin;
    struct { unsigned char _pad[16]; STARS_CV_0072 value; } iPlrMax;
    struct { unsigned char _pad[2]; STARS_CV_0072 value; } idPlan;
    struct { unsigned char _pad[4]; STARS_CV_0072 value; } cMax;
    struct { unsigned char _pad[6]; STARS_CV_0072 value; } cCur;
    struct { unsigned char _pad[8]; STARS_CV_0072 value; } cOperate;
    struct { unsigned char _pad[10]; STARS_CV_0072 value; } fFactory;
    struct { unsigned char _pad[2]; STARS_CV_0072 value; } dxOut;
    struct { unsigned char _pad[4]; STARS_CV_0170 value; } psz;
    struct { unsigned char _pad[2]; STARS_CV_0072 value; } iPlayer;
    struct { unsigned char _pad[2]; STARS_CV_1004 value; } lpfl;
    struct { unsigned char _pad[6]; STARS_CV_0072 value; } fRedDamage;
    struct { unsigned char _pad[8]; STARS_CV_0072 value; } dxDamage;
    struct { unsigned char _pad[10]; STARS_CV_0073 value; } grbit;
    struct { unsigned char _pad[2]; STARS_CV_105e value; } part;
    struct { unsigned char _pad[2]; STARS_CV_1009 value; } lpshdef;
    struct { unsigned char _pad[6]; STARS_CV_0072 value; } fShowDamage;
    struct { unsigned char _pad[8]; STARS_CV_0072 value; } fHideCounts;
    struct { unsigned char _pad[10]; STARS_CV_0072 value; } fToken;
    struct { unsigned char _pad[12]; STARS_CV_0072 value; } fSummary;
    struct { unsigned char _pad[14]; STARS_CV_0072 value; } itok;
};
#define STARS_OFFSET_10c3_grPopup 0
#define STARS_OFFSET_10c3_rgi 2
#define STARS_OFFSET_10c3_idPlanet 2
#define STARS_OFFSET_10c3_iPlanetVar 4
#define STARS_OFFSET_10c3_iPlanVal 6
#define STARS_OFFSET_10c3_iPlanMin 8
#define STARS_OFFSET_10c3_iPlanMax 10
#define STARS_OFFSET_10c3_iPlrVal 12
#define STARS_OFFSET_10c3_iPlrMin 14
#define STARS_OFFSET_10c3_iPlrMax 16
#define STARS_OFFSET_10c3_idPlan 2
#define STARS_OFFSET_10c3_cMax 4
#define STARS_OFFSET_10c3_cCur 6
#define STARS_OFFSET_10c3_cOperate 8
#define STARS_OFFSET_10c3_fFactory 10
#define STARS_OFFSET_10c3_dxOut 2
#define STARS_OFFSET_10c3_psz 4
#define STARS_OFFSET_10c3_iPlayer 2
#define STARS_OFFSET_10c3_lpfl 2
#define STARS_OFFSET_10c3_fRedDamage 6
#define STARS_OFFSET_10c3_dxDamage 8
#define STARS_OFFSET_10c3_grbit 10
#define STARS_OFFSET_10c3_part 2
#define STARS_OFFSET_10c3_lpshdef 2
#define STARS_OFFSET_10c3_fShowDamage 6
#define STARS_OFFSET_10c3_fHideCounts 8
#define STARS_OFFSET_10c3_fToken 10
#define STARS_OFFSET_10c3_fSummary 12
#define STARS_OFFSET_10c3_itok 14
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_10c5(STARS_CV_0170, STARS_CV_0072);
typedef STARS_CV_0070 STARS_CV_10c6[22323];
typedef STARS_CV_0020 STARS_CV_10c7[640];
typedef STARS_CV_0072 STARS_CV_10c8[10];
typedef STARS_CV_0070 STARS_CV_10c9[76];
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_10ca();
#ifdef STARS_NATIVE16
typedef STARS_CV_10cb STARS_FAR *STARS_CV_10cc;
#else
typedef STARS_ULONG32 STARS_CV_10cc;
#endif
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_10ce(STARS_CV_0072, STARS_CV_10cc, STARS_CV_0072);
#ifdef STARS_NATIVE16
typedef STARS_CV_10cb STARS_NEAR *STARS_CV_10cf;
#else
typedef unsigned short STARS_CV_10cf;
#endif
typedef STARS_CV_0070 STARS_CV_10d0[33];
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_10d1(STARS_CV_0072, STARS_CV_0072);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_10d2(STARS_CV_0073, STARS_CV_0072);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_10d4(STARS_CV_0072, STARS_CV_0072, STARS_CV_0072);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_10d6(STARS_CV_0073, STARS_CV_0072, STARS_CV_0073, STARS_CV_0072, STARS_CV_0073, STARS_CV_0073);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_10d8(STARS_CV_0073, STARS_CV_0072, STARS_CV_0072, STARS_CV_0072);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_10da(STARS_CV_0073, STARS_CV_0072, STARS_CV_0073);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_10dc(STARS_CV_0073, STARS_CV_0072, STARS_CV_0072, STARS_CV_0073, STARS_CV_0073, STARS_CV_0073);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_10de(STARS_CV_0073, STARS_CV_0072, STARS_CV_0072, STARS_CV_0073, STARS_CV_10cc);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_10e0(STARS_CV_0072, STARS_CV_0072, STARS_CV_0073, STARS_CV_0073, STARS_CV_0073, STARS_CV_0021);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_10e2(STARS_CV_0072, STARS_CV_0021, STARS_CV_0021, STARS_CV_0021, STARS_CV_0021, STARS_CV_0021, STARS_CV_0021, STARS_CV_0021, STARS_CV_0021);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_10e4(STARS_CV_1004, STARS_CV_0072, STARS_CV_0072, STARS_CV_0072, STARS_CV_0072);
#ifdef STARS_NATIVE16
typedef STARS_CV_1078 STARS_NEAR *STARS_CV_10e5;
#else
typedef unsigned short STARS_CV_10e5;
#endif
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_10e7(STARS_CV_0072, STARS_CV_10e5, STARS_CV_0073);
typedef STARS_CV_0020 STARS_CV_10e8[];
typedef STARS_CV_0021 STARS_CV_14d0; /* bit-field backing storage; use recorded bit positions/masks */
/* _tok: 29 bytes; each view begins at its recorded offset. */
union STARS_CV_10e9 {
    unsigned char raw[29];
    struct { STARS_CV_0021 value; } id;
    struct { unsigned char _pad[2]; STARS_CV_0020 value; } iplr;
    struct { unsigned char _pad[3]; STARS_CV_0020 value; } grobj;
    struct { unsigned char _pad[4]; STARS_CV_0020 value; } ishdef;
    struct { unsigned char _pad[5]; STARS_CV_0020 value; } brc;
    struct { unsigned char _pad[6]; STARS_CV_0020 value; } initBase;
    struct { unsigned char _pad[7]; STARS_CV_0020 value; } initMin;
    struct { unsigned char _pad[8]; STARS_CV_0020 value; } initMac;
    struct { unsigned char _pad[9]; STARS_CV_0020 value; } itokTarget;
    struct { unsigned char _pad[10]; STARS_CV_0020 value; } pctCloak;
    struct { unsigned char _pad[11]; STARS_CV_0020 value; } pctJam;
    struct { unsigned char _pad[12]; STARS_CV_0020 value; } pctBC;
    struct { unsigned char _pad[13]; STARS_CV_0020 value; } pctCap;
    struct { unsigned char _pad[14]; STARS_CV_0020 value; } pctBeamDef;
    struct { unsigned char _pad[15]; STARS_CV_0021 value; } wt;
    struct { unsigned char _pad[17]; STARS_CV_0021 value; } dpShield;
    struct { unsigned char _pad[19]; STARS_CV_0021 value; } csh;
    struct { unsigned char _pad[21]; STARS_CV_1158 value; } dv;
    struct { unsigned char _pad[23]; STARS_CV_1378 value; } mdTarget1;
    struct { unsigned char _pad[23]; STARS_CV_1379 value; } mdTarget2;
    struct { unsigned char _pad[23]; STARS_CV_137a value; } mdTactic;
    struct { unsigned char _pad[23]; STARS_CV_1362 value; } mdTarget0;
    struct { unsigned char _pad[25]; STARS_CV_1378 value; } dxyLim;
    struct { unsigned char _pad[25]; STARS_CV_1379 value; } dxyMax;
    struct { unsigned char _pad[25]; STARS_CV_137a value; } spd;
    struct { unsigned char _pad[25]; STARS_CV_1362 value; } cTarget;
    struct { unsigned char _pad[27]; STARS_CV_13d7 value; } fActive;
    struct { unsigned char _pad[27]; STARS_CV_13d8 value; } fDetector;
    struct { unsigned char _pad[27]; STARS_CV_13d9 value; } fTorp;
    struct { unsigned char _pad[27]; STARS_CV_13da value; } fRegen;
    struct { unsigned char _pad[27]; STARS_CV_13db value; } fMoved;
    struct { unsigned char _pad[27]; STARS_CV_14d0 value; } dzDis;
    struct { unsigned char _pad[27]; STARS_CV_1394 value; } dwt;
    struct { unsigned char _pad[27]; STARS_CV_137d value; } dMovesLeft;
    struct { unsigned char _pad[27]; STARS_CV_0021 value; } wFlags;
};
#define STARS_OFFSET_10e9_id 0
#define STARS_OFFSET_10e9_iplr 2
#define STARS_OFFSET_10e9_grobj 3
#define STARS_OFFSET_10e9_ishdef 4
#define STARS_OFFSET_10e9_brc 5
#define STARS_OFFSET_10e9_initBase 6
#define STARS_OFFSET_10e9_initMin 7
#define STARS_OFFSET_10e9_initMac 8
#define STARS_OFFSET_10e9_itokTarget 9
#define STARS_OFFSET_10e9_pctCloak 10
#define STARS_OFFSET_10e9_pctJam 11
#define STARS_OFFSET_10e9_pctBC 12
#define STARS_OFFSET_10e9_pctCap 13
#define STARS_OFFSET_10e9_pctBeamDef 14
#define STARS_OFFSET_10e9_wt 15
#define STARS_OFFSET_10e9_dpShield 17
#define STARS_OFFSET_10e9_csh 19
#define STARS_OFFSET_10e9_dv 21
#define STARS_OFFSET_10e9_mdTarget1 23
#define STARS_BITSHIFT_10e9_mdTarget1 0
#define STARS_BITMASK_10e9_mdTarget1 0xfUL
#define STARS_OFFSET_10e9_mdTarget2 23
#define STARS_BITSHIFT_10e9_mdTarget2 4
#define STARS_BITMASK_10e9_mdTarget2 0xfUL
#define STARS_OFFSET_10e9_mdTactic 23
#define STARS_BITSHIFT_10e9_mdTactic 8
#define STARS_BITMASK_10e9_mdTactic 0xfUL
#define STARS_OFFSET_10e9_mdTarget0 23
#define STARS_BITSHIFT_10e9_mdTarget0 12
#define STARS_BITMASK_10e9_mdTarget0 0xfUL
#define STARS_OFFSET_10e9_dxyLim 25
#define STARS_BITSHIFT_10e9_dxyLim 0
#define STARS_BITMASK_10e9_dxyLim 0xfUL
#define STARS_OFFSET_10e9_dxyMax 25
#define STARS_BITSHIFT_10e9_dxyMax 4
#define STARS_BITMASK_10e9_dxyMax 0xfUL
#define STARS_OFFSET_10e9_spd 25
#define STARS_BITSHIFT_10e9_spd 8
#define STARS_BITMASK_10e9_spd 0xfUL
#define STARS_OFFSET_10e9_cTarget 25
#define STARS_BITSHIFT_10e9_cTarget 12
#define STARS_BITMASK_10e9_cTarget 0xfUL
#define STARS_OFFSET_10e9_fActive 27
#define STARS_BITSHIFT_10e9_fActive 0
#define STARS_BITMASK_10e9_fActive 0x1UL
#define STARS_OFFSET_10e9_fDetector 27
#define STARS_BITSHIFT_10e9_fDetector 1
#define STARS_BITMASK_10e9_fDetector 0x1UL
#define STARS_OFFSET_10e9_fTorp 27
#define STARS_BITSHIFT_10e9_fTorp 2
#define STARS_BITMASK_10e9_fTorp 0x1UL
#define STARS_OFFSET_10e9_fRegen 27
#define STARS_BITSHIFT_10e9_fRegen 3
#define STARS_BITMASK_10e9_fRegen 0x1UL
#define STARS_OFFSET_10e9_fMoved 27
#define STARS_BITSHIFT_10e9_fMoved 4
#define STARS_BITMASK_10e9_fMoved 0x1UL
#define STARS_OFFSET_10e9_dzDis 27
#define STARS_BITSHIFT_10e9_dzDis 5
#define STARS_BITMASK_10e9_dzDis 0x1fUL
#define STARS_OFFSET_10e9_dwt 27
#define STARS_BITSHIFT_10e9_dwt 10
#define STARS_BITMASK_10e9_dwt 0xfUL
#define STARS_OFFSET_10e9_dMovesLeft 27
#define STARS_BITSHIFT_10e9_dMovesLeft 14
#define STARS_BITMASK_10e9_dMovesLeft 0x3UL
#define STARS_OFFSET_10e9_wFlags 27
#ifdef STARS_NATIVE16
typedef STARS_CV_10e9 STARS_FAR *STARS_CV_10ea;
#else
typedef STARS_ULONG32 STARS_CV_10ea;
#endif
typedef STARS_CV_0073 STARS_CV_10eb[3];
typedef STARS_CV_0021 STARS_CV_1533; /* bit-field backing storage; use recorded bit positions/masks */
/* _wn: 10 bytes; each view begins at its recorded offset. */
union STARS_CV_1535 {
    unsigned char raw[10];
    struct { STARS_CV_101a value; } rc;
    struct { unsigned char _pad[8]; STARS_CV_13d7 value; } fMaximized;
    struct { unsigned char _pad[8]; STARS_CV_13d8 value; } fMinimized;
    struct { unsigned char _pad[8]; STARS_CV_13d9 value; } fInitalized;
    struct { unsigned char _pad[8]; STARS_CV_1533 value; } fUnused;
};
#define STARS_OFFSET_1535_rc 0
#define STARS_OFFSET_1535_fMaximized 8
#define STARS_BITSHIFT_1535_fMaximized 0
#define STARS_BITMASK_1535_fMaximized 0x1UL
#define STARS_OFFSET_1535_fMinimized 8
#define STARS_BITSHIFT_1535_fMinimized 1
#define STARS_BITMASK_1535_fMinimized 0x1UL
#define STARS_OFFSET_1535_fInitalized 8
#define STARS_BITSHIFT_1535_fInitalized 2
#define STARS_BITMASK_1535_fInitalized 0x1UL
#define STARS_OFFSET_1535_fUnused 8
#define STARS_BITSHIFT_1535_fUnused 3
#define STARS_BITMASK_1535_fUnused 0x1fffUL
typedef STARS_CV_0021 STARS_CV_154a; /* bit-field backing storage; use recorded bit positions/masks */
/* _ini: 26 bytes; each view begins at its recorded offset. */
union STARS_CV_10ec {
    unsigned char raw[26];
    struct { STARS_CV_1535 value; } wnFrame;
    struct { unsigned char _pad[10]; STARS_CV_13d7 value; } fStartupFile;
    struct { unsigned char _pad[10]; STARS_CV_13d8 value; } fCmdLine;
    struct { unsigned char _pad[10]; STARS_CV_13d9 value; } fWait;
    struct { unsigned char _pad[10]; STARS_CV_13da value; } fGen;
    struct { unsigned char _pad[10]; STARS_CV_13db value; } fTry;
    struct { unsigned char _pad[10]; STARS_CV_154a value; } grobjSel;
    struct { unsigned char _pad[10]; STARS_CV_135f value; } fBatch;
    struct { unsigned char _pad[10]; STARS_CV_1360 value; } fNewGame;
    struct { unsigned char _pad[10]; STARS_CV_1361 value; } fDumpFleets;
    struct { unsigned char _pad[10]; STARS_CV_137b value; } fDumpPlanets;
    struct { unsigned char _pad[10]; STARS_CV_137c value; } fDumpMap;
    struct { unsigned char _pad[10]; STARS_CV_1395 value; } fValidate;
    struct { unsigned char _pad[10]; STARS_CV_1396 value; } fLogging;
    struct { unsigned char _pad[10]; STARS_CV_0021 value; } wFlags;
    struct { unsigned char _pad[12]; STARS_CV_0073 value; } turn;
    struct { unsigned char _pad[14]; STARS_CV_0072 value; } iObjSel;
    struct { unsigned char _pad[16]; STARS_CV_0072 value; } idPlayer;
    struct { unsigned char _pad[18]; STARS_CV_0012 value; } lid;
    struct { unsigned char _pad[22]; STARS_CV_0072 value; } cTurnGen;
    struct { unsigned char _pad[24]; STARS_CV_0072 value; } iMsg;
};
#define STARS_OFFSET_10ec_wnFrame 0
#define STARS_OFFSET_10ec_fStartupFile 10
#define STARS_BITSHIFT_10ec_fStartupFile 0
#define STARS_BITMASK_10ec_fStartupFile 0x1UL
#define STARS_OFFSET_10ec_fCmdLine 10
#define STARS_BITSHIFT_10ec_fCmdLine 1
#define STARS_BITMASK_10ec_fCmdLine 0x1UL
#define STARS_OFFSET_10ec_fWait 10
#define STARS_BITSHIFT_10ec_fWait 2
#define STARS_BITMASK_10ec_fWait 0x1UL
#define STARS_OFFSET_10ec_fGen 10
#define STARS_BITSHIFT_10ec_fGen 3
#define STARS_BITMASK_10ec_fGen 0x1UL
#define STARS_OFFSET_10ec_fTry 10
#define STARS_BITSHIFT_10ec_fTry 4
#define STARS_BITMASK_10ec_fTry 0x1UL
#define STARS_OFFSET_10ec_grobjSel 10
#define STARS_BITSHIFT_10ec_grobjSel 5
#define STARS_BITMASK_10ec_grobjSel 0xfUL
#define STARS_OFFSET_10ec_fBatch 10
#define STARS_BITSHIFT_10ec_fBatch 9
#define STARS_BITMASK_10ec_fBatch 0x1UL
#define STARS_OFFSET_10ec_fNewGame 10
#define STARS_BITSHIFT_10ec_fNewGame 10
#define STARS_BITMASK_10ec_fNewGame 0x1UL
#define STARS_OFFSET_10ec_fDumpFleets 10
#define STARS_BITSHIFT_10ec_fDumpFleets 11
#define STARS_BITMASK_10ec_fDumpFleets 0x1UL
#define STARS_OFFSET_10ec_fDumpPlanets 10
#define STARS_BITSHIFT_10ec_fDumpPlanets 12
#define STARS_BITMASK_10ec_fDumpPlanets 0x1UL
#define STARS_OFFSET_10ec_fDumpMap 10
#define STARS_BITSHIFT_10ec_fDumpMap 13
#define STARS_BITMASK_10ec_fDumpMap 0x1UL
#define STARS_OFFSET_10ec_fValidate 10
#define STARS_BITSHIFT_10ec_fValidate 14
#define STARS_BITMASK_10ec_fValidate 0x1UL
#define STARS_OFFSET_10ec_fLogging 10
#define STARS_BITSHIFT_10ec_fLogging 15
#define STARS_BITMASK_10ec_fLogging 0x1UL
#define STARS_OFFSET_10ec_wFlags 10
#define STARS_OFFSET_10ec_turn 12
#define STARS_OFFSET_10ec_iObjSel 14
#define STARS_OFFSET_10ec_idPlayer 16
#define STARS_OFFSET_10ec_lid 18
#define STARS_OFFSET_10ec_cTurnGen 22
#define STARS_OFFSET_10ec_iMsg 24
typedef STARS_CV_0073 STARS_CV_13c5; /* bit-field backing storage; use recorded bit positions/masks */
/* _tutor: 44 bytes; each view begins at its recorded offset. */
union STARS_CV_10ed {
    unsigned char raw[44];
    struct { STARS_CV_0072 value; } wFlags;
    struct { STARS_CV_13bf value; } fVisible;
    struct { STARS_CV_13c0 value; } fGameSaved;
    struct { STARS_CV_13c1 value; } fChange;
    struct { STARS_CV_13c2 value; } fTurnDone;
    struct { STARS_CV_13c3 value; } fTutorDone;
    struct { STARS_CV_13c4 value; } fNoErrors;
    struct { STARS_CV_13c5 value; } cError;
    struct { STARS_CV_13c6 value; } fAutoComplete;
    struct { STARS_CV_13c7 value; } fProgress;
    struct { STARS_CV_13c8 value; } fTBVis;
    struct { STARS_CV_13c9 value; } fValidQ;
    struct { STARS_CV_13ca value; } fFreeing;
    struct { STARS_CV_13cb value; } fShowHidMsg;
    struct { STARS_CV_13cc value; } unused;
    struct { unsigned char _pad[2]; STARS_CV_0072 value; } idt;
    struct { unsigned char _pad[4]; STARS_CV_0072 value; } idtBold;
    struct { unsigned char _pad[6]; STARS_CV_0072 value; } idh;
    struct { unsigned char _pad[8]; STARS_CV_0072 value; } idsError;
    struct { unsigned char _pad[10]; STARS_CV_0072 value; } iScanZoom;
    struct { unsigned char _pad[12]; STARS_CV_0072 value; } icolFSort;
    struct { unsigned char _pad[14]; STARS_CV_0021 value; } grbitScan;
    struct { unsigned char _pad[16]; STARS_CV_0073 value; } hwnd;
    struct { unsigned char _pad[18]; STARS_CV_10ef value; } zpq;
};
#define STARS_OFFSET_10ed_wFlags 0
#define STARS_OFFSET_10ed_fVisible 0
#define STARS_BITSHIFT_10ed_fVisible 0
#define STARS_BITMASK_10ed_fVisible 0x1UL
#define STARS_OFFSET_10ed_fGameSaved 0
#define STARS_BITSHIFT_10ed_fGameSaved 1
#define STARS_BITMASK_10ed_fGameSaved 0x1UL
#define STARS_OFFSET_10ed_fChange 0
#define STARS_BITSHIFT_10ed_fChange 2
#define STARS_BITMASK_10ed_fChange 0x1UL
#define STARS_OFFSET_10ed_fTurnDone 0
#define STARS_BITSHIFT_10ed_fTurnDone 3
#define STARS_BITMASK_10ed_fTurnDone 0x1UL
#define STARS_OFFSET_10ed_fTutorDone 0
#define STARS_BITSHIFT_10ed_fTutorDone 4
#define STARS_BITMASK_10ed_fTutorDone 0x1UL
#define STARS_OFFSET_10ed_fNoErrors 0
#define STARS_BITSHIFT_10ed_fNoErrors 5
#define STARS_BITMASK_10ed_fNoErrors 0x1UL
#define STARS_OFFSET_10ed_cError 0
#define STARS_BITSHIFT_10ed_cError 6
#define STARS_BITMASK_10ed_cError 0x7UL
#define STARS_OFFSET_10ed_fAutoComplete 0
#define STARS_BITSHIFT_10ed_fAutoComplete 9
#define STARS_BITMASK_10ed_fAutoComplete 0x1UL
#define STARS_OFFSET_10ed_fProgress 0
#define STARS_BITSHIFT_10ed_fProgress 10
#define STARS_BITMASK_10ed_fProgress 0x1UL
#define STARS_OFFSET_10ed_fTBVis 0
#define STARS_BITSHIFT_10ed_fTBVis 11
#define STARS_BITMASK_10ed_fTBVis 0x1UL
#define STARS_OFFSET_10ed_fValidQ 0
#define STARS_BITSHIFT_10ed_fValidQ 12
#define STARS_BITMASK_10ed_fValidQ 0x1UL
#define STARS_OFFSET_10ed_fFreeing 0
#define STARS_BITSHIFT_10ed_fFreeing 13
#define STARS_BITMASK_10ed_fFreeing 0x1UL
#define STARS_OFFSET_10ed_fShowHidMsg 0
#define STARS_BITSHIFT_10ed_fShowHidMsg 14
#define STARS_BITMASK_10ed_fShowHidMsg 0x1UL
#define STARS_OFFSET_10ed_unused 0
#define STARS_BITSHIFT_10ed_unused 15
#define STARS_BITMASK_10ed_unused 0x1UL
#define STARS_OFFSET_10ed_idt 2
#define STARS_OFFSET_10ed_idtBold 4
#define STARS_OFFSET_10ed_idh 6
#define STARS_OFFSET_10ed_idsError 8
#define STARS_OFFSET_10ed_iScanZoom 10
#define STARS_OFFSET_10ed_icolFSort 12
#define STARS_OFFSET_10ed_grbitScan 14
#define STARS_OFFSET_10ed_hwnd 16
#define STARS_OFFSET_10ed_zpq 18
typedef STARS_CV_10ef STARS_CV_10f0[2];
/* _zipprodq: 40 bytes; each view begins at its recorded offset. */
union STARS_CV_10f1 {
    unsigned char raw[40];
    struct { STARS_CV_116c value; } szName;
    struct { unsigned char _pad[13]; STARS_CV_0020 value; } fValid;
    struct { unsigned char _pad[14]; STARS_CV_10ef value; } zpq1;
    struct { unsigned char _pad[14]; STARS_CV_0020 value; } fNoResearch;
    struct { unsigned char _pad[15]; STARS_CV_0020 value; } cpq;
    struct { unsigned char _pad[16]; STARS_CV_134f value; } rgpq;
};
#define STARS_OFFSET_10f1_szName 0
#define STARS_OFFSET_10f1_fValid 13
#define STARS_OFFSET_10f1_zpq1 14
#define STARS_OFFSET_10f1_fNoResearch 14
#define STARS_OFFSET_10f1_cpq 15
#define STARS_OFFSET_10f1_rgpq 16
typedef STARS_CV_10f1 STARS_CV_10f2[5];
typedef STARS_CV_0170 STARS_FAR STARS_CDECL STARS_CV_10f3(STARS_CV_0072);
typedef STARS_CV_0070 STARS_CV_10f4[28209];
typedef STARS_CV_0020 STARS_CV_10f5[1414];
typedef STARS_CV_0072 STARS_CV_10f6[23];
typedef STARS_CV_0070 STARS_CV_10f7[84];
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_10f9(STARS_CV_0072, STARS_CV_0072, STARS_CV_0220);
#ifdef STARS_NATIVE16
typedef STARS_CV_105e STARS_NEAR *STARS_CV_10fa;
#else
typedef unsigned short STARS_CV_10fa;
#endif
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_10fc(STARS_CV_0072, STARS_CV_10fa);
#ifdef STARS_NATIVE16
typedef STARS_CV_1008 STARS_NEAR *STARS_CV_10fd;
#else
typedef unsigned short STARS_CV_10fd;
#endif
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_10ff(STARS_CV_10fd, STARS_CV_0072, STARS_CV_0072, STARS_CV_0072);
typedef STARS_CV_0072 STARS_CV_1100[4];
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_1102(STARS_CV_10fd, STARS_CV_0072);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_1104(STARS_CV_0072, STARS_CV_0072, STARS_CV_0072, STARS_CV_0072, STARS_CV_0072, STARS_CV_0072);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_1106(STARS_CV_1004, STARS_CV_1007);
#ifdef STARS_NATIVE16
typedef STARS_CV_1007 STARS_NEAR *STARS_CV_1107;
#else
typedef unsigned short STARS_CV_1107;
#endif
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_1109(STARS_CV_1004, STARS_CV_1107);
typedef STARS_CV_1007 STARS_FAR STARS_CDECL STARS_CV_110b(STARS_CV_1020, STARS_CV_0012);
typedef STARS_CV_0022 STARS_FAR STARS_CDECL STARS_CV_110c(STARS_CV_1004);
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_110e(STARS_CV_1004, STARS_CV_0072, STARS_CV_0072, STARS_CV_1107);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_1110(STARS_CV_1004, STARS_CV_1004, STARS_CV_1004, STARS_CV_0072);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_1111(STARS_CV_1004, STARS_CV_100b);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_1113(STARS_CV_1004, STARS_CV_0072, STARS_CV_0072, STARS_CV_0072, STARS_CV_0012, STARS_CV_0012, STARS_CV_1107, STARS_CV_0172);
#ifdef STARS_NATIVE16
typedef STARS_CV_1005 STARS_NEAR *STARS_CV_1114;
#else
typedef unsigned short STARS_CV_1114;
#endif
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_1116(STARS_CV_1004, STARS_CV_1114, STARS_CV_0072);
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_1118(STARS_CV_0073, STARS_CV_0073, STARS_CV_0073, STARS_CV_0072);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_111a(STARS_CV_0220, STARS_CV_0072, STARS_CV_0172, STARS_CV_0072);
typedef STARS_CV_1007 STARS_CV_111b[100];
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_111d(STARS_CV_0172);
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_111f(STARS_CV_100b, STARS_CV_0112);
typedef STARS_CV_1004 STARS_CV_1120[32];
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_1122(STARS_CV_1004, STARS_CV_1004);
#ifdef STARS_NATIVE16
typedef STARS_CV_1122 STARS_FAR *STARS_CV_1123;
#else
typedef STARS_ULONG32 STARS_CV_1123;
#endif
typedef STARS_CV_1004 STARS_FAR STARS_CDECL STARS_CV_1125(STARS_CV_1004, STARS_CV_1123);
#ifdef STARS_NATIVE16
typedef STARS_CV_104e STARS_FAR *STARS_CV_1126;
#else
typedef STARS_ULONG32 STARS_CV_1126;
#endif
typedef STARS_CV_100b STARS_FAR STARS_CDECL STARS_CV_1128(STARS_CV_100b, STARS_CV_1126);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_1129(STARS_CV_1018, STARS_CV_0072, STARS_CV_0072);
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_112a(STARS_CV_1004, STARS_CV_0072);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_112c(STARS_CV_1004, STARS_CV_1004, STARS_CV_1004, STARS_CV_0072, STARS_CV_1107);
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_112e(STARS_CV_100b, STARS_CV_103a);
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_1130(STARS_CV_0120);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_1132(STARS_CV_0072, STARS_CV_0072, STARS_CV_0072, STARS_CV_0072);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_1133(STARS_CV_100b, STARS_CV_0112);
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_1135(STARS_CV_103a, STARS_CV_0072);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_1137(STARS_CV_100b, STARS_CV_0112, STARS_CV_0112);
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_1139(STARS_CV_0072, STARS_CV_103a, STARS_CV_0072, STARS_CV_0112, STARS_CV_0112);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_113b(STARS_CV_0072, STARS_CV_0072, STARS_CV_0021, STARS_CV_0172, STARS_CV_0120);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_113d(STARS_CV_1004, STARS_CV_0072, STARS_CV_0072, STARS_CV_0012, STARS_CV_0012);
typedef STARS_CV_0020 STARS_CV_113e[85];
typedef STARS_CV_0020 STARS_CV_113f[45];
typedef STARS_CV_0021 STARS_CV_13ce; /* bit-field backing storage; use recorded bit positions/masks */
/* _aipart: 2 bytes; each view begins at its recorded offset. */
union STARS_CV_1140 {
    unsigned char raw[2];
    struct { STARS_CV_1378 value; } ibit;
    struct { STARS_CV_13ce value; } iItem;
    struct { STARS_CV_13cf value; } cItem;
    struct { STARS_CV_13d0 value; } fRandom;
};
#define STARS_OFFSET_1140_ibit 0
#define STARS_BITSHIFT_1140_ibit 0
#define STARS_BITMASK_1140_ibit 0xfUL
#define STARS_OFFSET_1140_iItem 0
#define STARS_BITSHIFT_1140_iItem 4
#define STARS_BITMASK_1140_iItem 0x1fUL
#define STARS_OFFSET_1140_cItem 0
#define STARS_BITSHIFT_1140_cItem 9
#define STARS_BITMASK_1140_cItem 0xfUL
#define STARS_OFFSET_1140_fRandom 0
#define STARS_BITSHIFT_1140_fRandom 13
#define STARS_BITMASK_1140_fRandom 0x7UL
typedef STARS_CV_1140 STARS_CV_1141[150];
typedef STARS_CV_0012 STARS_CV_1142[2];
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_1144(STARS_CV_0072, STARS_CV_0021);
typedef STARS_CV_0070 STARS_CV_1145[4];
typedef STARS_CV_0020 STARS_CV_1146[31];
typedef STARS_CV_0021 STARS_CV_1147[38];
typedef STARS_CV_0020 STARS_CV_1148[36];
typedef STARS_CV_0020 STARS_CV_1149[141];
/* tagTIMERINFO: 12 bytes; each view begins at its recorded offset. */
union STARS_CV_114b {
    unsigned char raw[12];
    struct { STARS_CV_0022 value; } dwSize;
    struct { unsigned char _pad[4]; STARS_CV_0022 value; } dwmsSinceStart;
    struct { unsigned char _pad[8]; STARS_CV_0022 value; } dwmsThisVM;
};
#define STARS_OFFSET_114b_dwSize 0
#define STARS_OFFSET_114b_dwmsSinceStart 4
#define STARS_OFFSET_114b_dwmsThisVM 8
typedef STARS_CV_0072 STARS_FAR STARS_PASCAL STARS_CV_114d(STARS_CV_0021, STARS_CV_0022);
#ifdef STARS_NATIVE16
typedef STARS_CV_114d STARS_FAR *STARS_CV_114e;
#else
typedef STARS_ULONG32 STARS_CV_114e;
#endif
#ifdef STARS_NATIVE16
typedef STARS_CV_114f STARS_FAR *STARS_CV_1150;
#else
typedef STARS_ULONG32 STARS_CV_1150;
#endif
/* _hb: 16 bytes; each view begins at its recorded offset. */
union STARS_CV_114f {
    unsigned char raw[16];
    struct { STARS_CV_0021 value; } cbFree;
    struct { unsigned char _pad[2]; STARS_CV_0021 value; } cbBlock;
    struct { unsigned char _pad[4]; STARS_CV_0021 value; } cbSlop;
    struct { unsigned char _pad[6]; STARS_CV_0021 value; } ibTop;
    struct { unsigned char _pad[8]; STARS_CV_1150 value; } lphbNext;
    struct { unsigned char _pad[12]; STARS_CV_0073 value; } hmem;
    struct { unsigned char _pad[14]; STARS_CV_0020 value; } ht;
    struct { unsigned char _pad[15]; STARS_CV_0020 value; } unused1;
};
#define STARS_OFFSET_114f_cbFree 0
#define STARS_OFFSET_114f_cbBlock 2
#define STARS_OFFSET_114f_cbSlop 4
#define STARS_OFFSET_114f_ibTop 6
#define STARS_OFFSET_114f_lphbNext 8
#define STARS_OFFSET_114f_hmem 12
#define STARS_OFFSET_114f_ht 14
#define STARS_OFFSET_114f_unused1 15
typedef STARS_CV_10a3 STARS_FAR STARS_CDECL STARS_CV_1151(STARS_CV_0072);
typedef STARS_CV_0012 STARS_FAR STARS_CDECL STARS_CV_1153(STARS_CV_10a3, STARS_CV_0021);
typedef STARS_CV_0012 STARS_FAR STARS_CDECL STARS_CV_1155(STARS_CV_10a3, STARS_CV_0072);
/* _btlrec: 6 bytes; each view begins at its recorded offset. */
union STARS_CV_1156 {
    unsigned char raw[6];
    struct { STARS_CV_0020 value; } itok;
    struct { unsigned char _pad[1]; STARS_CV_0020 value; } brcDest;
    struct { unsigned char _pad[2]; STARS_CV_0072 value; } ctok;
    struct { unsigned char _pad[4]; STARS_CV_1378 value; } iRound;
    struct { unsigned char _pad[4]; STARS_CV_1379 value; } dzDis;
    struct { unsigned char _pad[4]; STARS_CV_1411 value; } itokAttack;
    /* rgkill: variable tail at offset 6; no fixed storage. */
};
#define STARS_OFFSET_1156_itok 0
#define STARS_OFFSET_1156_brcDest 1
#define STARS_OFFSET_1156_ctok 2
#define STARS_OFFSET_1156_iRound 4
#define STARS_BITSHIFT_1156_iRound 0
#define STARS_BITMASK_1156_iRound 0xfUL
#define STARS_OFFSET_1156_dzDis 4
#define STARS_BITSHIFT_1156_dzDis 4
#define STARS_BITMASK_1156_dzDis 0xfUL
#define STARS_OFFSET_1156_itokAttack 4
#define STARS_BITSHIFT_1156_itokAttack 8
#define STARS_BITMASK_1156_itokAttack 0xffUL
#define STARS_OFFSET_1156_rgkill 6
#ifdef STARS_NATIVE16
typedef STARS_CV_1156 STARS_FAR *STARS_CV_1157;
#else
typedef STARS_ULONG32 STARS_CV_1157;
#endif
#ifdef STARS_NATIVE16
typedef STARS_CV_1158 STARS_FAR *STARS_CV_1159;
#else
typedef STARS_ULONG32 STARS_CV_1159;
#endif
typedef STARS_CV_0012 STARS_FAR STARS_CDECL STARS_CV_115b(STARS_CV_0072, STARS_CV_1159);
/* _btnt: 24 bytes; each view begins at its recorded offset. */
union STARS_CV_115c {
    unsigned char raw[24];
    struct { STARS_CV_0073 value; } hwnd;
    struct { unsigned char _pad[2]; STARS_CV_0073 value; } hdc;
    struct { unsigned char _pad[4]; STARS_CV_101a value; } rc;
    struct { unsigned char _pad[12]; STARS_CV_0072 value; } dTimer;
    struct { unsigned char _pad[14]; STARS_CV_0072 value; } btf;
    struct { unsigned char _pad[16]; STARS_CV_0170 value; } szText;
    struct { unsigned char _pad[18]; STARS_CV_0012 value; } lTicks;
    struct { unsigned char _pad[22]; STARS_CV_13d7 value; } fFirst;
    struct { unsigned char _pad[22]; STARS_CV_13d8 value; } fDown;
    struct { unsigned char _pad[22]; STARS_CV_13d9 value; } fInitDown;
    struct { unsigned char _pad[22]; STARS_CV_13da value; } fCreatedDC;
    struct { unsigned char _pad[22]; STARS_CV_13db value; } fNoEndRedraw;
    struct { unsigned char _pad[22]; STARS_CV_13dc value; } fUnused;
};
#define STARS_OFFSET_115c_hwnd 0
#define STARS_OFFSET_115c_hdc 2
#define STARS_OFFSET_115c_rc 4
#define STARS_OFFSET_115c_dTimer 12
#define STARS_OFFSET_115c_btf 14
#define STARS_OFFSET_115c_szText 16
#define STARS_OFFSET_115c_lTicks 18
#define STARS_OFFSET_115c_fFirst 22
#define STARS_BITSHIFT_115c_fFirst 0
#define STARS_BITMASK_115c_fFirst 0x1UL
#define STARS_OFFSET_115c_fDown 22
#define STARS_BITSHIFT_115c_fDown 1
#define STARS_BITMASK_115c_fDown 0x1UL
#define STARS_OFFSET_115c_fInitDown 22
#define STARS_BITSHIFT_115c_fInitDown 2
#define STARS_BITMASK_115c_fInitDown 0x1UL
#define STARS_OFFSET_115c_fCreatedDC 22
#define STARS_BITSHIFT_115c_fCreatedDC 3
#define STARS_BITMASK_115c_fCreatedDC 0x1UL
#define STARS_OFFSET_115c_fNoEndRedraw 22
#define STARS_BITSHIFT_115c_fNoEndRedraw 4
#define STARS_BITMASK_115c_fNoEndRedraw 0x1UL
#define STARS_OFFSET_115c_fUnused 22
#define STARS_BITSHIFT_115c_fUnused 5
#define STARS_BITMASK_115c_fUnused 0x7ffUL
#ifdef STARS_NATIVE16
typedef STARS_CV_1158 STARS_NEAR *STARS_CV_115d;
#else
typedef unsigned short STARS_CV_115d;
#endif
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_115f(STARS_CV_0072, STARS_CV_0112, STARS_CV_115d, STARS_CV_0112, STARS_CV_0172);
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_1161(STARS_CV_0073, STARS_CV_0072, STARS_CV_0072);
typedef STARS_CV_0020 STARS_CV_1162[256];
typedef STARS_CV_0070 STARS_CV_1163[96];
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_1165(STARS_CV_0073, STARS_CV_0072, STARS_CV_0072, STARS_CV_0020);
typedef STARS_CV_0170 STARS_CV_1166[40];
typedef STARS_CV_0070 STARS_CV_1167[1536];
typedef STARS_CV_0072 STARS_CV_1168[40];
typedef STARS_CV_101a STARS_CV_1169[2];
typedef STARS_CV_1150 STARS_CV_116a[];
typedef STARS_CV_0070 STARS_CV_116b[21];
/* _find_t: 44 bytes; each view begins at its recorded offset. */
union STARS_CV_116e {
    unsigned char raw[44];
    struct { STARS_CV_116b value; } reserved;
    struct { unsigned char _pad[21]; STARS_CV_0070 value; } attrib;
    struct { unsigned char _pad[22]; STARS_CV_0073 value; } wr_time;
    struct { unsigned char _pad[24]; STARS_CV_0073 value; } wr_date;
    struct { unsigned char _pad[26]; STARS_CV_0012 value; } size;
    struct { unsigned char _pad[30]; STARS_CV_116c value; } name;
};
#define STARS_OFFSET_116e_reserved 0
#define STARS_OFFSET_116e_attrib 21
#define STARS_OFFSET_116e_wr_time 22
#define STARS_OFFSET_116e_wr_date 24
#define STARS_OFFSET_116e_size 26
#define STARS_OFFSET_116e_name 30
/* _diskfree_t: 8 bytes; each view begins at its recorded offset. */
union STARS_CV_1170 {
    unsigned char raw[8];
    struct { STARS_CV_0073 value; } total_clusters;
    struct { unsigned char _pad[2]; STARS_CV_0073 value; } avail_clusters;
    struct { unsigned char _pad[4]; STARS_CV_0073 value; } sectors_per_cluster;
    struct { unsigned char _pad[6]; STARS_CV_0073 value; } bytes_per_sector;
};
#define STARS_OFFSET_1170_total_clusters 0
#define STARS_OFFSET_1170_avail_clusters 2
#define STARS_OFFSET_1170_sectors_per_cluster 4
#define STARS_OFFSET_1170_bytes_per_sector 6
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_1172(STARS_CV_0012, STARS_CV_0012);
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_1174(STARS_CV_0022);
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_1176(STARS_CV_0112, STARS_CV_0112);
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_1178(STARS_CV_0012, STARS_CV_0011, STARS_CV_0072, STARS_CV_0072, STARS_CV_0072);
typedef STARS_CV_0012 STARS_FAR STARS_CDECL STARS_CV_1179();
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_117a(STARS_CV_0170, STARS_CV_0072);
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_117c(STARS_CV_0170, STARS_CV_0170);
typedef STARS_CV_0070 STARS_CV_12ce[128];
/* tagOFSTRUCT: 136 bytes; each view begins at its recorded offset. */
union STARS_CV_117d {
    unsigned char raw[136];
    struct { STARS_CV_0020 value; } cBytes;
    struct { unsigned char _pad[1]; STARS_CV_0020 value; } fFixedDisk;
    struct { unsigned char _pad[2]; STARS_CV_0073 value; } nErrCode;
    struct { unsigned char _pad[4]; STARS_CV_1053 value; } reserved;
    struct { unsigned char _pad[8]; STARS_CV_12ce value; } szPathName;
};
#define STARS_OFFSET_117d_cBytes 0
#define STARS_OFFSET_117d_fFixedDisk 1
#define STARS_OFFSET_117d_nErrCode 2
#define STARS_OFFSET_117d_reserved 4
#define STARS_OFFSET_117d_szPathName 8
typedef STARS_CV_0070 STARS_CV_117e[2048];
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_1180(STARS_CV_0072, STARS_CV_0170);
typedef STARS_CV_0170 STARS_FAR STARS_CDECL STARS_CV_1182(STARS_CV_0072, STARS_CV_0172);
typedef STARS_CV_0170 STARS_FAR STARS_CDECL STARS_CV_1184(STARS_CV_0012, STARS_CV_0172);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_1186(STARS_CV_0170, STARS_CV_0012);
typedef STARS_CV_0070 STARS_CV_1187[15];
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_1189(STARS_CV_0073, STARS_CV_0072, STARS_CV_0072, STARS_CV_0170, STARS_CV_0072);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_118b(STARS_CV_0073, STARS_CV_0172, STARS_CV_0072, STARS_CV_0170, STARS_CV_0072, STARS_CV_0072);
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_118d(STARS_CV_0073, STARS_CV_0172, STARS_CV_0172, STARS_CV_0170, STARS_CV_0072, STARS_CV_0072, STARS_CV_0072, STARS_CV_0172, STARS_CV_0072, STARS_CV_0072);
#ifdef STARS_NATIVE16
typedef STARS_CV_0170 STARS_NEAR *STARS_CV_118e;
#else
typedef unsigned short STARS_CV_118e;
#endif
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_1190(STARS_CV_118e, STARS_CV_0170);
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_1192(STARS_CV_0170, STARS_CV_118e);
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_1194(STARS_CV_0073, STARS_CV_101b, STARS_CV_0170, STARS_CV_0072);
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_1196(STARS_CV_0073, STARS_CV_0072, STARS_CV_0072, STARS_CV_0170, STARS_CV_0072, STARS_CV_0072);
/* tagLOGFONT: 50 bytes; each view begins at its recorded offset. */
union STARS_CV_1197 {
    unsigned char raw[50];
    struct { STARS_CV_0072 value; } lfHeight;
    struct { unsigned char _pad[2]; STARS_CV_0072 value; } lfWidth;
    struct { unsigned char _pad[4]; STARS_CV_0072 value; } lfEscapement;
    struct { unsigned char _pad[6]; STARS_CV_0072 value; } lfOrientation;
    struct { unsigned char _pad[8]; STARS_CV_0072 value; } lfWeight;
    struct { unsigned char _pad[10]; STARS_CV_0020 value; } lfItalic;
    struct { unsigned char _pad[11]; STARS_CV_0020 value; } lfUnderline;
    struct { unsigned char _pad[12]; STARS_CV_0020 value; } lfStrikeOut;
    struct { unsigned char _pad[13]; STARS_CV_0020 value; } lfCharSet;
    struct { unsigned char _pad[14]; STARS_CV_0020 value; } lfOutPrecision;
    struct { unsigned char _pad[15]; STARS_CV_0020 value; } lfClipPrecision;
    struct { unsigned char _pad[16]; STARS_CV_0020 value; } lfQuality;
    struct { unsigned char _pad[17]; STARS_CV_0020 value; } lfPitchAndFamily;
    struct { unsigned char _pad[18]; STARS_CV_11e2 value; } lfFaceName;
};
#define STARS_OFFSET_1197_lfHeight 0
#define STARS_OFFSET_1197_lfWidth 2
#define STARS_OFFSET_1197_lfEscapement 4
#define STARS_OFFSET_1197_lfOrientation 6
#define STARS_OFFSET_1197_lfWeight 8
#define STARS_OFFSET_1197_lfItalic 10
#define STARS_OFFSET_1197_lfUnderline 11
#define STARS_OFFSET_1197_lfStrikeOut 12
#define STARS_OFFSET_1197_lfCharSet 13
#define STARS_OFFSET_1197_lfOutPrecision 14
#define STARS_OFFSET_1197_lfClipPrecision 15
#define STARS_OFFSET_1197_lfQuality 16
#define STARS_OFFSET_1197_lfPitchAndFamily 17
#define STARS_OFFSET_1197_lfFaceName 18
#ifdef STARS_NATIVE16
typedef STARS_CV_1197 STARS_NEAR *STARS_CV_1198;
#else
typedef unsigned short STARS_CV_1198;
#endif
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_119a(STARS_CV_101b, STARS_CV_0072, STARS_CV_0072);
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_119c(STARS_CV_101b, STARS_CV_1020, STARS_CV_0072);
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_119e(STARS_CV_0073, STARS_CV_1020, STARS_CV_0072);
typedef STARS_CV_0012 STARS_FAR STARS_CDECL STARS_CV_11a0(STARS_CV_0073, STARS_CV_101b, STARS_CV_0072, STARS_CV_0112, STARS_CV_0173, STARS_CV_0012);
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_11a2(STARS_CV_0073, STARS_CV_101b, STARS_CV_0072);
#ifdef STARS_NATIVE16
typedef STARS_CV_115c STARS_NEAR *STARS_CV_11a3;
#else
typedef unsigned short STARS_CV_11a3;
#endif
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_11a5(STARS_CV_11a3, STARS_CV_0073, STARS_CV_0073, STARS_CV_101b, STARS_CV_0072, STARS_CV_0072, STARS_CV_0072, STARS_CV_0072, STARS_CV_0170);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_11a7(STARS_CV_11a3);
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_11a9(STARS_CV_0073, STARS_CV_101b, STARS_CV_0072, STARS_CV_0072, STARS_CV_0170);
typedef STARS_CV_1018 STARS_CV_11aa[6];
/* tagMSG: 18 bytes; each view begins at its recorded offset. */
union STARS_CV_11ab {
    unsigned char raw[18];
    struct { STARS_CV_0073 value; } hwnd;
    struct { unsigned char _pad[2]; STARS_CV_0073 value; } message;
    struct { unsigned char _pad[4]; STARS_CV_0073 value; } wParam;
    struct { unsigned char _pad[6]; STARS_CV_0012 value; } lParam;
    struct { unsigned char _pad[10]; STARS_CV_0022 value; } time;
    struct { unsigned char _pad[14]; STARS_CV_1018 value; } pt;
};
#define STARS_OFFSET_11ab_hwnd 0
#define STARS_OFFSET_11ab_message 2
#define STARS_OFFSET_11ab_wParam 4
#define STARS_OFFSET_11ab_lParam 6
#define STARS_OFFSET_11ab_time 10
#define STARS_OFFSET_11ab_pt 14
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_11ad(STARS_CV_0270, STARS_CV_0072);
typedef STARS_CV_0073 STARS_FAR STARS_CDECL STARS_CV_11ae(STARS_CV_0022);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_11b0(STARS_CV_0270, STARS_CV_0270, STARS_CV_0172);
typedef STARS_CV_0070 STARS_CV_11b1[1024];
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_11b3(STARS_CV_0270, STARS_CV_0072, STARS_CV_0270, STARS_CV_0172);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_11b5(STARS_CV_0020);
typedef STARS_CV_0070 STARS_FAR STARS_CDECL STARS_CV_11b6(STARS_CV_0072);
typedef STARS_CV_0021 STARS_FAR STARS_CDECL STARS_CV_11b8(STARS_CV_0203);
/* tagBITMAPCOREHEADER: 12 bytes; each view begins at its recorded offset. */
union STARS_CV_11b9 {
    unsigned char raw[12];
    struct { STARS_CV_0022 value; } bcSize;
    struct { unsigned char _pad[4]; STARS_CV_0011 value; } bcWidth;
    struct { unsigned char _pad[6]; STARS_CV_0011 value; } bcHeight;
    struct { unsigned char _pad[8]; STARS_CV_0021 value; } bcPlanes;
    struct { unsigned char _pad[10]; STARS_CV_0021 value; } bcBitCount;
};
#define STARS_OFFSET_11b9_bcSize 0
#define STARS_OFFSET_11b9_bcWidth 4
#define STARS_OFFSET_11b9_bcHeight 6
#define STARS_OFFSET_11b9_bcPlanes 8
#define STARS_OFFSET_11b9_bcBitCount 10
#ifdef STARS_NATIVE16
typedef STARS_CV_11b9 STARS_FAR *STARS_CV_11ba;
#else
typedef STARS_ULONG32 STARS_CV_11ba;
#endif
/* tagBITMAPINFOHEADER: 40 bytes; each view begins at its recorded offset. */
union STARS_CV_11bb {
    unsigned char raw[40];
    struct { STARS_CV_0022 value; } biSize;
    struct { unsigned char _pad[4]; STARS_CV_0012 value; } biWidth;
    struct { unsigned char _pad[8]; STARS_CV_0012 value; } biHeight;
    struct { unsigned char _pad[12]; STARS_CV_0021 value; } biPlanes;
    struct { unsigned char _pad[14]; STARS_CV_0021 value; } biBitCount;
    struct { unsigned char _pad[16]; STARS_CV_0022 value; } biCompression;
    struct { unsigned char _pad[20]; STARS_CV_0022 value; } biSizeImage;
    struct { unsigned char _pad[24]; STARS_CV_0012 value; } biXPelsPerMeter;
    struct { unsigned char _pad[28]; STARS_CV_0012 value; } biYPelsPerMeter;
    struct { unsigned char _pad[32]; STARS_CV_0022 value; } biClrUsed;
    struct { unsigned char _pad[36]; STARS_CV_0022 value; } biClrImportant;
};
#define STARS_OFFSET_11bb_biSize 0
#define STARS_OFFSET_11bb_biWidth 4
#define STARS_OFFSET_11bb_biHeight 8
#define STARS_OFFSET_11bb_biPlanes 12
#define STARS_OFFSET_11bb_biBitCount 14
#define STARS_OFFSET_11bb_biCompression 16
#define STARS_OFFSET_11bb_biSizeImage 20
#define STARS_OFFSET_11bb_biXPelsPerMeter 24
#define STARS_OFFSET_11bb_biYPelsPerMeter 28
#define STARS_OFFSET_11bb_biClrUsed 32
#define STARS_OFFSET_11bb_biClrImportant 36
#ifdef STARS_NATIVE16
typedef STARS_CV_11bb STARS_FAR *STARS_CV_11bc;
#else
typedef STARS_ULONG32 STARS_CV_11bc;
#endif
typedef STARS_CV_0073 STARS_FAR STARS_CDECL STARS_CV_11bd(STARS_CV_0073);
/* tagPALETTEENTRY: 4 bytes; each view begins at its recorded offset. */
union STARS_CV_14ca {
    unsigned char raw[4];
    struct { STARS_CV_0020 value; } peRed;
    struct { unsigned char _pad[1]; STARS_CV_0020 value; } peGreen;
    struct { unsigned char _pad[2]; STARS_CV_0020 value; } peBlue;
    struct { unsigned char _pad[3]; STARS_CV_0020 value; } peFlags;
};
#define STARS_OFFSET_14ca_peRed 0
#define STARS_OFFSET_14ca_peGreen 1
#define STARS_OFFSET_14ca_peBlue 2
#define STARS_OFFSET_14ca_peFlags 3
typedef STARS_CV_14ca STARS_CV_150c[1];
/* tagLOGPALETTE: 8 bytes; each view begins at its recorded offset. */
union STARS_CV_11be {
    unsigned char raw[8];
    struct { STARS_CV_0021 value; } palVersion;
    struct { unsigned char _pad[2]; STARS_CV_0021 value; } palNumEntries;
    struct { unsigned char _pad[4]; STARS_CV_150c value; } palPalEntry;
};
#define STARS_OFFSET_11be_palVersion 0
#define STARS_OFFSET_11be_palNumEntries 2
#define STARS_OFFSET_11be_palPalEntry 4
#ifdef STARS_NATIVE16
typedef STARS_CV_11be STARS_NEAR *STARS_CV_11bf;
#else
typedef unsigned short STARS_CV_11bf;
#endif
typedef STARS_CV_0073 STARS_FAR STARS_CDECL STARS_CV_11c0();
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_11c2(STARS_CV_0073, STARS_CV_0072, STARS_CV_0072, STARS_CV_0072, STARS_CV_0072, STARS_CV_0073, STARS_CV_0072, STARS_CV_0072, STARS_CV_0072, STARS_CV_0072, STARS_CV_0012);
typedef STARS_CV_0073 STARS_FAR STARS_CDECL STARS_CV_11c4(STARS_CV_0073, STARS_CV_0022, STARS_CV_0021, STARS_CV_0073);
/* tagBITMAP: 14 bytes; each view begins at its recorded offset. */
union STARS_CV_11c5 {
    unsigned char raw[14];
    struct { STARS_CV_0072 value; } bmType;
    struct { unsigned char _pad[2]; STARS_CV_0072 value; } bmWidth;
    struct { unsigned char _pad[4]; STARS_CV_0072 value; } bmHeight;
    struct { unsigned char _pad[6]; STARS_CV_0072 value; } bmWidthBytes;
    struct { unsigned char _pad[8]; STARS_CV_0020 value; } bmPlanes;
    struct { unsigned char _pad[9]; STARS_CV_0020 value; } bmBitsPixel;
    struct { unsigned char _pad[10]; STARS_CV_0203 value; } bmBits;
};
#define STARS_OFFSET_11c5_bmType 0
#define STARS_OFFSET_11c5_bmWidth 2
#define STARS_OFFSET_11c5_bmHeight 4
#define STARS_OFFSET_11c5_bmWidthBytes 6
#define STARS_OFFSET_11c5_bmPlanes 8
#define STARS_OFFSET_11c5_bmBitsPixel 9
#define STARS_OFFSET_11c5_bmBits 10
typedef STARS_CV_0073 STARS_FAR STARS_CDECL STARS_CV_11c6(STARS_CV_0072);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_11c8(STARS_CV_0072, STARS_CV_0270, STARS_CV_0022);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_11ca(STARS_CV_1018, STARS_CV_1018, STARS_CV_1018, STARS_CV_0012, STARS_CV_0072, STARS_CV_0172, STARS_CV_0172);
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_11cb(STARS_CV_0072, STARS_CV_0170);
typedef STARS_CV_0012 STARS_FAR STARS_CDECL STARS_CV_11cd(STARS_CV_0170);
typedef STARS_CV_0070 STARS_CV_11ce[60];
typedef STARS_CV_0070 STARS_CV_11cf[20];
typedef STARS_CV_0022 STARS_FAR STARS_CDECL STARS_CV_11d0();
typedef STARS_CV_0070 STARS_CV_11d1[8];
typedef STARS_CV_0012 STARS_FAR STARS_CDECL STARS_CV_11d3(STARS_CV_1018, STARS_CV_1018);
#ifdef STARS_NATIVE16
typedef STARS_CV_0270 STARS_NEAR *STARS_CV_11d4;
#else
typedef unsigned short STARS_CV_11d4;
#endif
typedef STARS_CV_0270 STARS_FAR STARS_CDECL STARS_CV_11d6(STARS_CV_11d4);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_11d8(STARS_CV_0270, STARS_CV_0112, STARS_CV_0072);
typedef STARS_CV_0073 STARS_FAR STARS_CDECL STARS_CV_11da(STARS_CV_0073, STARS_CV_0072, STARS_CV_0172);
/* tagTEXTMETRIC: 31 bytes; each view begins at its recorded offset. */
union STARS_CV_11db {
    unsigned char raw[31];
    struct { STARS_CV_0072 value; } tmHeight;
    struct { unsigned char _pad[2]; STARS_CV_0072 value; } tmAscent;
    struct { unsigned char _pad[4]; STARS_CV_0072 value; } tmDescent;
    struct { unsigned char _pad[6]; STARS_CV_0072 value; } tmInternalLeading;
    struct { unsigned char _pad[8]; STARS_CV_0072 value; } tmExternalLeading;
    struct { unsigned char _pad[10]; STARS_CV_0072 value; } tmAveCharWidth;
    struct { unsigned char _pad[12]; STARS_CV_0072 value; } tmMaxCharWidth;
    struct { unsigned char _pad[14]; STARS_CV_0072 value; } tmWeight;
    struct { unsigned char _pad[16]; STARS_CV_0020 value; } tmItalic;
    struct { unsigned char _pad[17]; STARS_CV_0020 value; } tmUnderlined;
    struct { unsigned char _pad[18]; STARS_CV_0020 value; } tmStruckOut;
    struct { unsigned char _pad[19]; STARS_CV_0020 value; } tmFirstChar;
    struct { unsigned char _pad[20]; STARS_CV_0020 value; } tmLastChar;
    struct { unsigned char _pad[21]; STARS_CV_0020 value; } tmDefaultChar;
    struct { unsigned char _pad[22]; STARS_CV_0020 value; } tmBreakChar;
    struct { unsigned char _pad[23]; STARS_CV_0020 value; } tmPitchAndFamily;
    struct { unsigned char _pad[24]; STARS_CV_0020 value; } tmCharSet;
    struct { unsigned char _pad[25]; STARS_CV_0072 value; } tmOverhang;
    struct { unsigned char _pad[27]; STARS_CV_0072 value; } tmDigitizedAspectX;
    struct { unsigned char _pad[29]; STARS_CV_0072 value; } tmDigitizedAspectY;
};
#define STARS_OFFSET_11db_tmHeight 0
#define STARS_OFFSET_11db_tmAscent 2
#define STARS_OFFSET_11db_tmDescent 4
#define STARS_OFFSET_11db_tmInternalLeading 6
#define STARS_OFFSET_11db_tmExternalLeading 8
#define STARS_OFFSET_11db_tmAveCharWidth 10
#define STARS_OFFSET_11db_tmMaxCharWidth 12
#define STARS_OFFSET_11db_tmWeight 14
#define STARS_OFFSET_11db_tmItalic 16
#define STARS_OFFSET_11db_tmUnderlined 17
#define STARS_OFFSET_11db_tmStruckOut 18
#define STARS_OFFSET_11db_tmFirstChar 19
#define STARS_OFFSET_11db_tmLastChar 20
#define STARS_OFFSET_11db_tmDefaultChar 21
#define STARS_OFFSET_11db_tmBreakChar 22
#define STARS_OFFSET_11db_tmPitchAndFamily 23
#define STARS_OFFSET_11db_tmCharSet 24
#define STARS_OFFSET_11db_tmOverhang 25
#define STARS_OFFSET_11db_tmDigitizedAspectX 27
#define STARS_OFFSET_11db_tmDigitizedAspectY 29
typedef STARS_CV_0072 STARS_CV_11dc[128];
typedef STARS_CV_0073 STARS_CV_11dd[32];
typedef STARS_CV_0022 STARS_CV_11de[32];
typedef STARS_CV_0070 STARS_CV_11e0[12];
typedef STARS_CV_0070 STARS_CV_11e1[17];
typedef STARS_CV_11e2 STARS_CV_11e3[4];
typedef STARS_CV_0072 STARS_CV_11e4[26];
typedef STARS_CV_0020 STARS_CV_11e5[11];
typedef STARS_CV_0070 STARS_CV_11e6[4099];
typedef STARS_CV_0020 STARS_CV_11e7[999];
typedef STARS_CV_0070 STARS_CV_11e8[52];
typedef STARS_CV_0070 STARS_CV_11e9[19];
typedef STARS_CV_1142 STARS_CV_11ea[4];
typedef STARS_CV_1018 STARS_CV_11eb[5];
typedef STARS_CV_1018 STARS_CV_11ec[3];
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_11ed(STARS_CV_0073, STARS_CV_0072, STARS_CV_0072);
typedef STARS_CV_101a STARS_CV_11ee[17];
#ifdef STARS_NATIVE16
typedef STARS_CV_1014 STARS_NEAR *STARS_CV_11ef;
#else
typedef unsigned short STARS_CV_11ef;
#endif
typedef volatile STARS_CV_0072 STARS_CV_11f0;
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_11f2(STARS_CV_0073, STARS_CV_0073, STARS_CV_0072);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_11f4(STARS_CV_1018);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_11f6(STARS_CV_0073, STARS_CV_1018, STARS_CV_0072);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_11f8(STARS_CV_11ef, STARS_CV_0072);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_11fa(STARS_CV_11ef, STARS_CV_0072, STARS_CV_0072);
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_11fb(STARS_CV_11ef, STARS_CV_0072, STARS_CV_0072);
typedef STARS_CV_0070 STARS_CV_11fc[600];
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_11fe(STARS_CV_11ef);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_11ff(STARS_CV_11ef);
typedef STARS_CV_0012 STARS_FAR STARS_CDECL STARS_CV_1200(STARS_CV_11ef);
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_1202(STARS_CV_0073, STARS_CV_101b, STARS_CV_11ef);
typedef STARS_CV_0021 STARS_FAR STARS_CDECL STARS_CV_1203(STARS_CV_11ef);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_1205(STARS_CV_0170, STARS_CV_11ef);
typedef STARS_CV_0073 STARS_FAR STARS_PASCAL STARS_CV_1351(STARS_CV_0073, STARS_CV_0073, STARS_CV_0073, STARS_CV_0012);
#ifdef STARS_NATIVE16
typedef STARS_CV_1351 STARS_FAR *STARS_CV_1352;
#else
typedef STARS_ULONG32 STARS_CV_1352;
#endif
/* tagOFN: 72 bytes; each view begins at its recorded offset. */
union STARS_CV_1206 {
    unsigned char raw[72];
    struct { STARS_CV_0022 value; } lStructSize;
    struct { unsigned char _pad[4]; STARS_CV_0073 value; } hwndOwner;
    struct { unsigned char _pad[6]; STARS_CV_0073 value; } hInstance;
    struct { unsigned char _pad[8]; STARS_CV_0270 value; } lpstrFilter;
    struct { unsigned char _pad[12]; STARS_CV_0270 value; } lpstrCustomFilter;
    struct { unsigned char _pad[16]; STARS_CV_0022 value; } nMaxCustFilter;
    struct { unsigned char _pad[20]; STARS_CV_0022 value; } nFilterIndex;
    struct { unsigned char _pad[24]; STARS_CV_0270 value; } lpstrFile;
    struct { unsigned char _pad[28]; STARS_CV_0022 value; } nMaxFile;
    struct { unsigned char _pad[32]; STARS_CV_0270 value; } lpstrFileTitle;
    struct { unsigned char _pad[36]; STARS_CV_0022 value; } nMaxFileTitle;
    struct { unsigned char _pad[40]; STARS_CV_0270 value; } lpstrInitialDir;
    struct { unsigned char _pad[44]; STARS_CV_0270 value; } lpstrTitle;
    struct { unsigned char _pad[48]; STARS_CV_0022 value; } Flags;
    struct { unsigned char _pad[52]; STARS_CV_0073 value; } nFileOffset;
    struct { unsigned char _pad[54]; STARS_CV_0073 value; } nFileExtension;
    struct { unsigned char _pad[56]; STARS_CV_0270 value; } lpstrDefExt;
    struct { unsigned char _pad[60]; STARS_CV_0012 value; } lCustData;
    struct { unsigned char _pad[64]; STARS_CV_1352 value; } lpfnHook;
    struct { unsigned char _pad[68]; STARS_CV_0270 value; } lpTemplateName;
};
#define STARS_OFFSET_1206_lStructSize 0
#define STARS_OFFSET_1206_hwndOwner 4
#define STARS_OFFSET_1206_hInstance 6
#define STARS_OFFSET_1206_lpstrFilter 8
#define STARS_OFFSET_1206_lpstrCustomFilter 12
#define STARS_OFFSET_1206_nMaxCustFilter 16
#define STARS_OFFSET_1206_nFilterIndex 20
#define STARS_OFFSET_1206_lpstrFile 24
#define STARS_OFFSET_1206_nMaxFile 28
#define STARS_OFFSET_1206_lpstrFileTitle 32
#define STARS_OFFSET_1206_nMaxFileTitle 36
#define STARS_OFFSET_1206_lpstrInitialDir 40
#define STARS_OFFSET_1206_lpstrTitle 44
#define STARS_OFFSET_1206_Flags 48
#define STARS_OFFSET_1206_nFileOffset 52
#define STARS_OFFSET_1206_nFileExtension 54
#define STARS_OFFSET_1206_lpstrDefExt 56
#define STARS_OFFSET_1206_lCustData 60
#define STARS_OFFSET_1206_lpfnHook 64
#define STARS_OFFSET_1206_lpTemplateName 68
typedef STARS_CV_0070 STARS_CV_1207[7];
typedef STARS_CV_1014 STARS_CV_1208[7];
typedef STARS_CV_0073 STARS_CV_120a[2];
typedef STARS_CV_120a STARS_CV_120b[3];
typedef STARS_CV_0021 STARS_CV_120c[5];
typedef STARS_CV_0170 STARS_CV_120d[2];
typedef STARS_CV_0072 STARS_CV_120e[14];
typedef STARS_CV_0072 STARS_CV_120f[6];
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_1210(STARS_CV_1077);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_1212(STARS_CV_1009);
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_1214(STARS_CV_10ea, STARS_CV_1004, STARS_CV_0072);
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_1216(STARS_CV_1107, STARS_CV_0212, STARS_CV_0072, STARS_CV_1020);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_1218(STARS_CV_1004, STARS_CV_0121, STARS_CV_0121, STARS_CV_0121);
typedef STARS_CV_0012 STARS_CV_1219[16];
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_121b(STARS_CV_1004, STARS_CV_0072, STARS_CV_10ea, STARS_CV_0072, STARS_CV_1009);
typedef STARS_CV_0072 STARS_CV_1439[12];
/* _engine: 78 bytes; each view begins at its recorded offset. */
union STARS_CV_121c {
    unsigned char raw[78];
    struct { STARS_CV_0072 value; } id;
    struct { unsigned char _pad[2]; STARS_CV_1270 value; } rgTech;
    struct { unsigned char _pad[8]; STARS_CV_11e2 value; } szName;
    struct { unsigned char _pad[40]; STARS_CV_0072 value; } cMass;
    struct { unsigned char _pad[42]; STARS_CV_0021 value; } resCost;
    struct { unsigned char _pad[44]; STARS_CV_1044 value; } rgwtOreCost;
    struct { unsigned char _pad[50]; STARS_CV_0072 value; } ibmp;
    struct { unsigned char _pad[52]; STARS_CV_0072 value; } grfAbilities;
    struct { unsigned char _pad[54]; STARS_CV_1439 value; } rgcFuelUsed;
};
#define STARS_OFFSET_121c_id 0
#define STARS_OFFSET_121c_rgTech 2
#define STARS_OFFSET_121c_szName 8
#define STARS_OFFSET_121c_cMass 40
#define STARS_OFFSET_121c_resCost 42
#define STARS_OFFSET_121c_rgwtOreCost 44
#define STARS_OFFSET_121c_ibmp 50
#define STARS_OFFSET_121c_grfAbilities 52
#define STARS_OFFSET_121c_rgcFuelUsed 54
typedef STARS_CV_1009 STARS_FAR STARS_CDECL STARS_CV_121f(STARS_CV_10ea);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_1221(STARS_CV_10ea, STARS_CV_10ea);
typedef STARS_CV_0021 STARS_CV_1222[16];
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_1223(STARS_CV_10ea);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_1225(STARS_CV_1077, STARS_CV_0172);
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_1227(STARS_CV_10ea, STARS_CV_0172, STARS_CV_0120);
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_1229(STARS_CV_1004, STARS_CV_0072, STARS_CV_0021, STARS_CV_0120, STARS_CV_0172, STARS_CV_0172);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_122b(STARS_CV_0020, STARS_CV_0020);
typedef STARS_CV_0012 STARS_FAR STARS_CDECL STARS_CV_122d(STARS_CV_10ea, STARS_CV_0020, STARS_CV_0020, STARS_CV_10ea, STARS_CV_0072);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_122f(STARS_CV_10ea, STARS_CV_0021, STARS_CV_0120);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_1231(STARS_CV_10ea, STARS_CV_0021);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_1233(STARS_CV_10ea, STARS_CV_0072);
typedef STARS_CV_0012 STARS_FAR STARS_CDECL STARS_CV_1235(STARS_CV_10ea, STARS_CV_0020, STARS_CV_0072, STARS_CV_0021);
typedef STARS_CV_0012 STARS_FAR STARS_CDECL STARS_CV_1237(STARS_CV_0012, STARS_CV_0012, STARS_CV_0072);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_1239(STARS_CV_10ea, STARS_CV_0072, STARS_CV_0021);
typedef STARS_CV_100a STARS_CV_123a[3];
typedef STARS_CV_1018 STARS_CV_123b[2];
typedef STARS_CV_0012 STARS_FAR STARS_CDECL STARS_CV_123d(STARS_CV_0012, STARS_CV_10ea, STARS_CV_0072, STARS_CV_0072);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_123f(STARS_CV_0072, STARS_CV_0072, STARS_CV_1157, STARS_CV_0021);
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_1241(STARS_CV_10ea, STARS_CV_0072, STARS_CV_0072, STARS_CV_1004, STARS_CV_0072);
#ifdef STARS_NATIVE16
typedef STARS_CV_1003 STARS_NEAR *STARS_CV_1242;
#else
typedef unsigned short STARS_CV_1242;
#endif
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_1244(STARS_CV_1242, STARS_CV_1107);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_1246(STARS_CV_10ea, STARS_CV_0072, STARS_CV_0112, STARS_CV_0012, STARS_CV_0021, STARS_CV_0072, STARS_CV_0112);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_1248(STARS_CV_0021, STARS_CV_0072);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_124a(STARS_CV_1004, STARS_CV_0072, STARS_CV_0121, STARS_CV_0021, STARS_CV_0021);
typedef STARS_CV_0020 STARS_CV_124b[64];
typedef STARS_CV_0021 STARS_CV_124c[256];
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_124e(STARS_CV_0072, STARS_CV_0072, STARS_CV_0072, STARS_CV_0072, STARS_CV_0121);
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_1250(STARS_CV_1004, STARS_CV_0072, STARS_CV_0072, STARS_CV_0121, STARS_CV_0072, STARS_CV_0072, STARS_CV_0072, STARS_CV_0021);
typedef STARS_CV_0020 STARS_CV_1251[136];
typedef STARS_CV_1150 STARS_FAR STARS_CDECL STARS_CV_1252(STARS_CV_0021, STARS_CV_0072);
typedef STARS_CV_1150 STARS_FAR STARS_CDECL STARS_CV_1254(STARS_CV_1150);
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_1255(STARS_CV_1150);
typedef STARS_CV_0203 STARS_FAR STARS_CDECL STARS_CV_1256(STARS_CV_0021, STARS_CV_0072);
typedef STARS_CV_1150 STARS_FAR STARS_CDECL STARS_CV_1258(STARS_CV_0203, STARS_CV_0072);
typedef STARS_CV_0203 STARS_FAR STARS_CDECL STARS_CV_125a(STARS_CV_0203, STARS_CV_0021, STARS_CV_0072);
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_125b(STARS_CV_0203, STARS_CV_0072);
/* _pl: 4 bytes; each view begins at its recorded offset. */
union STARS_CV_125c {
    unsigned char raw[4];
    struct { STARS_CV_135d value; } cbItem;
    struct { STARS_CV_135e value; } fMark;
    struct { STARS_CV_13a9 value; } ht;
    struct { STARS_CV_1362 value; } cAlloc;
    struct { unsigned char _pad[2]; STARS_CV_0020 value; } iMax;
    struct { unsigned char _pad[3]; STARS_CV_0020 value; } iMac;
    /* rgb: variable tail at offset 4; no fixed storage. */
};
#define STARS_OFFSET_125c_cbItem 0
#define STARS_BITSHIFT_125c_cbItem 0
#define STARS_BITMASK_125c_cbItem 0xffUL
#define STARS_OFFSET_125c_fMark 0
#define STARS_BITSHIFT_125c_fMark 8
#define STARS_BITMASK_125c_fMark 0x1UL
#define STARS_OFFSET_125c_ht 0
#define STARS_BITSHIFT_125c_ht 9
#define STARS_BITMASK_125c_ht 0x7UL
#define STARS_OFFSET_125c_cAlloc 0
#define STARS_BITSHIFT_125c_cAlloc 12
#define STARS_BITMASK_125c_cAlloc 0xfUL
#define STARS_OFFSET_125c_iMax 2
#define STARS_OFFSET_125c_iMac 3
#define STARS_OFFSET_125c_rgb 4
#ifdef STARS_NATIVE16
typedef STARS_CV_125c STARS_FAR *STARS_CV_125d;
#else
typedef STARS_ULONG32 STARS_CV_125d;
#endif
typedef STARS_CV_125d STARS_FAR STARS_CDECL STARS_CV_125f(STARS_CV_125d, STARS_CV_0021);
typedef STARS_CV_125d STARS_FAR STARS_CDECL STARS_CV_1261(STARS_CV_0021, STARS_CV_0021, STARS_CV_0072);
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_1263(STARS_CV_125d);
typedef STARS_CV_0021 STARS_CV_1264[12];
typedef STARS_CV_1150 STARS_CV_1265[12];
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_1267(STARS_CV_0073, STARS_CV_0073, STARS_CV_101b, STARS_CV_0072);
typedef STARS_CV_0012 STARS_FAR STARS_CDECL STARS_CV_1268(STARS_CV_0072, STARS_CV_0072, STARS_CV_0072);
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_126a(STARS_CV_0073, STARS_CV_0072, STARS_CV_0072, STARS_CV_10fa);
typedef STARS_CV_0070 STARS_CV_126b[2];
typedef STARS_CV_0012 STARS_FAR STARS_CDECL STARS_CV_126d(STARS_CV_0012);
typedef STARS_CV_0012 STARS_FAR STARS_CDECL STARS_CV_126f(STARS_CV_0270);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_1272(STARS_CV_10fa);
typedef STARS_CV_0073 STARS_CV_1273[7];
typedef STARS_CV_0012 STARS_CV_1274[27];
typedef STARS_CV_1078 STARS_CV_1275[8];
typedef STARS_CV_0021 STARS_CV_1276[17];
/* tagDRAWITEMSTRUCT: 26 bytes; each view begins at its recorded offset. */
union STARS_CV_1277 {
    unsigned char raw[26];
    struct { STARS_CV_0073 value; } CtlType;
    struct { unsigned char _pad[2]; STARS_CV_0073 value; } CtlID;
    struct { unsigned char _pad[4]; STARS_CV_0073 value; } itemID;
    struct { unsigned char _pad[6]; STARS_CV_0073 value; } itemAction;
    struct { unsigned char _pad[8]; STARS_CV_0073 value; } itemState;
    struct { unsigned char _pad[10]; STARS_CV_0073 value; } hwndItem;
    struct { unsigned char _pad[12]; STARS_CV_0073 value; } hDC;
    struct { unsigned char _pad[14]; STARS_CV_101a value; } rcItem;
    struct { unsigned char _pad[22]; STARS_CV_0022 value; } itemData;
};
#define STARS_OFFSET_1277_CtlType 0
#define STARS_OFFSET_1277_CtlID 2
#define STARS_OFFSET_1277_itemID 4
#define STARS_OFFSET_1277_itemAction 6
#define STARS_OFFSET_1277_itemState 8
#define STARS_OFFSET_1277_hwndItem 10
#define STARS_OFFSET_1277_hDC 12
#define STARS_OFFSET_1277_rcItem 14
#define STARS_OFFSET_1277_itemData 22
#ifdef STARS_NATIVE16
typedef STARS_CV_1277 STARS_FAR *STARS_CV_1278;
#else
typedef STARS_ULONG32 STARS_CV_1278;
#endif
/* tagMEASUREITEMSTRUCT: 14 bytes; each view begins at its recorded offset. */
union STARS_CV_1279 {
    unsigned char raw[14];
    struct { STARS_CV_0073 value; } CtlType;
    struct { unsigned char _pad[2]; STARS_CV_0073 value; } CtlID;
    struct { unsigned char _pad[4]; STARS_CV_0073 value; } itemID;
    struct { unsigned char _pad[6]; STARS_CV_0073 value; } itemWidth;
    struct { unsigned char _pad[8]; STARS_CV_0073 value; } itemHeight;
    struct { unsigned char _pad[10]; STARS_CV_0022 value; } itemData;
};
#define STARS_OFFSET_1279_CtlType 0
#define STARS_OFFSET_1279_CtlID 2
#define STARS_OFFSET_1279_itemID 4
#define STARS_OFFSET_1279_itemWidth 6
#define STARS_OFFSET_1279_itemHeight 8
#define STARS_OFFSET_1279_itemData 10
#ifdef STARS_NATIVE16
typedef STARS_CV_1279 STARS_FAR *STARS_CV_127a;
#else
typedef STARS_ULONG32 STARS_CV_127a;
#endif
typedef STARS_CV_0170 STARS_CV_127b[6];
typedef STARS_CV_10f1 STARS_CV_127c[4];
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_127e(STARS_CV_0073, STARS_CV_0021, STARS_CV_0012);
typedef STARS_CV_0070 STARS_CV_127f[86];
typedef STARS_CV_0170 STARS_FAR STARS_CDECL STARS_CV_1281(STARS_CV_104a);
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_1283(STARS_CV_100b, STARS_CV_104a, STARS_CV_0122, STARS_CV_0072, STARS_CV_0072);
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_1285(STARS_CV_100b, STARS_CV_1055, STARS_CV_0072, STARS_CV_0172, STARS_CV_0172);
#ifdef STARS_NATIVE16
typedef STARS_CV_10f1 STARS_NEAR *STARS_CV_1286;
#else
typedef unsigned short STARS_CV_1286;
#endif
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_1288(STARS_CV_0073, STARS_CV_1286);
typedef STARS_CV_0021 STARS_CV_13f5; /* bit-field backing storage; use recorded bit positions/masks */
/* _huldef: 143 bytes; each view begins at its recorded offset. */
union STARS_CV_1289 {
    unsigned char raw[143];
    struct { STARS_CV_1076 value; } hul;
    struct { unsigned char _pad[123]; STARS_CV_1346 value; } init;
    struct { unsigned char _pad[123]; STARS_CV_13f5 value; } imdAttack;
    struct { unsigned char _pad[123]; STARS_CV_1394 value; } imdCategory;
    struct { unsigned char _pad[123]; STARS_CV_137d value; } unused;
    struct { unsigned char _pad[125]; STARS_CV_0021 value; } wrcCargo;
    struct { unsigned char _pad[127]; STARS_CV_103d value; } rgbrc;
};
#define STARS_OFFSET_1289_hul 0
#define STARS_OFFSET_1289_init 123
#define STARS_BITSHIFT_1289_init 0
#define STARS_BITMASK_1289_init 0x3fUL
#define STARS_OFFSET_1289_imdAttack 123
#define STARS_BITSHIFT_1289_imdAttack 6
#define STARS_BITMASK_1289_imdAttack 0xfUL
#define STARS_OFFSET_1289_imdCategory 123
#define STARS_BITSHIFT_1289_imdCategory 10
#define STARS_BITMASK_1289_imdCategory 0xfUL
#define STARS_OFFSET_1289_unused 123
#define STARS_BITSHIFT_1289_unused 14
#define STARS_BITMASK_1289_unused 0x3UL
#define STARS_OFFSET_1289_wrcCargo 125
#define STARS_OFFSET_1289_rgbrc 127
#ifdef STARS_NATIVE16
typedef STARS_CV_1289 STARS_FAR *STARS_CV_128a;
#else
typedef STARS_ULONG32 STARS_CV_128a;
#endif
typedef STARS_CV_128a STARS_FAR STARS_CDECL STARS_CV_128b(STARS_CV_0072);
typedef STARS_CV_121d STARS_FAR STARS_CDECL STARS_CV_128c(STARS_CV_0072);
/* _scanner: 56 bytes; each view begins at its recorded offset. */
union STARS_CV_128d {
    unsigned char raw[56];
    struct { STARS_CV_0072 value; } id;
    struct { unsigned char _pad[2]; STARS_CV_1270 value; } rgTech;
    struct { unsigned char _pad[8]; STARS_CV_11e2 value; } szName;
    struct { unsigned char _pad[40]; STARS_CV_0072 value; } cMass;
    struct { unsigned char _pad[42]; STARS_CV_0021 value; } resCost;
    struct { unsigned char _pad[44]; STARS_CV_1044 value; } rgwtOreCost;
    struct { unsigned char _pad[50]; STARS_CV_0072 value; } ibmp;
    struct { unsigned char _pad[52]; STARS_CV_0072 value; } dRange;
    struct { unsigned char _pad[54]; STARS_CV_0072 value; } grfAbilities;
};
#define STARS_OFFSET_128d_id 0
#define STARS_OFFSET_128d_rgTech 2
#define STARS_OFFSET_128d_szName 8
#define STARS_OFFSET_128d_cMass 40
#define STARS_OFFSET_128d_resCost 42
#define STARS_OFFSET_128d_rgwtOreCost 44
#define STARS_OFFSET_128d_ibmp 50
#define STARS_OFFSET_128d_dRange 52
#define STARS_OFFSET_128d_grfAbilities 54
typedef STARS_CV_128e STARS_FAR STARS_CDECL STARS_CV_128f(STARS_CV_0072);
typedef STARS_CV_1009 STARS_FAR STARS_CDECL STARS_CV_1290();
/* _planetary: 54 bytes; each view begins at its recorded offset. */
union STARS_CV_1291 {
    unsigned char raw[54];
    struct { STARS_CV_0072 value; } id;
    struct { unsigned char _pad[2]; STARS_CV_1270 value; } rgTech;
    struct { unsigned char _pad[8]; STARS_CV_11e2 value; } szName;
    struct { unsigned char _pad[40]; STARS_CV_0072 value; } cMass;
    struct { unsigned char _pad[42]; STARS_CV_0021 value; } resCost;
    struct { unsigned char _pad[44]; STARS_CV_1044 value; } rgwtOreCost;
    struct { unsigned char _pad[50]; STARS_CV_0072 value; } ibmp;
    struct { unsigned char _pad[52]; STARS_CV_0072 value; } grAbility;
};
#define STARS_OFFSET_1291_id 0
#define STARS_OFFSET_1291_rgTech 2
#define STARS_OFFSET_1291_szName 8
#define STARS_OFFSET_1291_cMass 40
#define STARS_OFFSET_1291_resCost 42
#define STARS_OFFSET_1291_rgwtOreCost 44
#define STARS_OFFSET_1291_ibmp 50
#define STARS_OFFSET_1291_grAbility 52
typedef STARS_CV_1292 STARS_FAR STARS_CDECL STARS_CV_1293(STARS_CV_0072);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_1295(STARS_CV_10fa, STARS_CV_0021, STARS_CV_0021);
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_1296(STARS_CV_10fa);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_1297(STARS_CV_0270);
typedef STARS_CV_121c STARS_CV_1298[16];
/* _armor: 54 bytes; each view begins at its recorded offset. */
union STARS_CV_1299 {
    unsigned char raw[54];
    struct { STARS_CV_0072 value; } id;
    struct { unsigned char _pad[2]; STARS_CV_1270 value; } rgTech;
    struct { unsigned char _pad[8]; STARS_CV_11e2 value; } szName;
    struct { unsigned char _pad[40]; STARS_CV_0072 value; } cMass;
    struct { unsigned char _pad[42]; STARS_CV_0021 value; } resCost;
    struct { unsigned char _pad[44]; STARS_CV_1044 value; } rgwtOreCost;
    struct { unsigned char _pad[50]; STARS_CV_0072 value; } ibmp;
    struct { unsigned char _pad[52]; STARS_CV_0072 value; } dp;
};
#define STARS_OFFSET_1299_id 0
#define STARS_OFFSET_1299_rgTech 2
#define STARS_OFFSET_1299_szName 8
#define STARS_OFFSET_1299_cMass 40
#define STARS_OFFSET_1299_resCost 42
#define STARS_OFFSET_1299_rgwtOreCost 44
#define STARS_OFFSET_1299_ibmp 50
#define STARS_OFFSET_1299_dp 52
typedef STARS_CV_1299 STARS_CV_129a[12];
typedef STARS_CV_128d STARS_CV_129b[16];
/* _shield: 54 bytes; each view begins at its recorded offset. */
union STARS_CV_129c {
    unsigned char raw[54];
    struct { STARS_CV_0072 value; } id;
    struct { unsigned char _pad[2]; STARS_CV_1270 value; } rgTech;
    struct { unsigned char _pad[8]; STARS_CV_11e2 value; } szName;
    struct { unsigned char _pad[40]; STARS_CV_0072 value; } cMass;
    struct { unsigned char _pad[42]; STARS_CV_0021 value; } resCost;
    struct { unsigned char _pad[44]; STARS_CV_1044 value; } rgwtOreCost;
    struct { unsigned char _pad[50]; STARS_CV_0072 value; } ibmp;
    struct { unsigned char _pad[52]; STARS_CV_0072 value; } dp;
};
#define STARS_OFFSET_129c_id 0
#define STARS_OFFSET_129c_rgTech 2
#define STARS_OFFSET_129c_szName 8
#define STARS_OFFSET_129c_cMass 40
#define STARS_OFFSET_129c_resCost 42
#define STARS_OFFSET_129c_rgwtOreCost 44
#define STARS_OFFSET_129c_ibmp 50
#define STARS_OFFSET_129c_dp 52
typedef STARS_CV_129c STARS_CV_129d[10];
/* _special: 54 bytes; each view begins at its recorded offset. */
union STARS_CV_129e {
    unsigned char raw[54];
    struct { STARS_CV_0072 value; } id;
    struct { unsigned char _pad[2]; STARS_CV_1270 value; } rgTech;
    struct { unsigned char _pad[8]; STARS_CV_11e2 value; } szName;
    struct { unsigned char _pad[40]; STARS_CV_0072 value; } cMass;
    struct { unsigned char _pad[42]; STARS_CV_0021 value; } resCost;
    struct { unsigned char _pad[44]; STARS_CV_1044 value; } rgwtOreCost;
    struct { unsigned char _pad[50]; STARS_CV_0072 value; } ibmp;
    struct { unsigned char _pad[52]; STARS_CV_0072 value; } grAbility;
};
#define STARS_OFFSET_129e_id 0
#define STARS_OFFSET_129e_rgTech 2
#define STARS_OFFSET_129e_szName 8
#define STARS_OFFSET_129e_cMass 40
#define STARS_OFFSET_129e_resCost 42
#define STARS_OFFSET_129e_rgwtOreCost 44
#define STARS_OFFSET_129e_ibmp 50
#define STARS_OFFSET_129e_grAbility 52
typedef STARS_CV_129e STARS_CV_129f[17];
typedef STARS_CV_129e STARS_CV_12a0[11];
/* _mines: 54 bytes; each view begins at its recorded offset. */
union STARS_CV_12a1 {
    unsigned char raw[54];
    struct { STARS_CV_0072 value; } id;
    struct { unsigned char _pad[2]; STARS_CV_1270 value; } rgTech;
    struct { unsigned char _pad[8]; STARS_CV_11e2 value; } szName;
    struct { unsigned char _pad[40]; STARS_CV_0072 value; } cMass;
    struct { unsigned char _pad[42]; STARS_CV_0021 value; } resCost;
    struct { unsigned char _pad[44]; STARS_CV_1044 value; } rgwtOreCost;
    struct { unsigned char _pad[50]; STARS_CV_0072 value; } ibmp;
    struct { unsigned char _pad[52]; STARS_CV_0072 value; } grAbility;
};
#define STARS_OFFSET_12a1_id 0
#define STARS_OFFSET_12a1_rgTech 2
#define STARS_OFFSET_12a1_szName 8
#define STARS_OFFSET_12a1_cMass 40
#define STARS_OFFSET_12a1_resCost 42
#define STARS_OFFSET_12a1_rgwtOreCost 44
#define STARS_OFFSET_12a1_ibmp 50
#define STARS_OFFSET_12a1_grAbility 52
typedef STARS_CV_12a1 STARS_CV_12a2[10];
/* _mining: 54 bytes; each view begins at its recorded offset. */
union STARS_CV_12a3 {
    unsigned char raw[54];
    struct { STARS_CV_0072 value; } id;
    struct { unsigned char _pad[2]; STARS_CV_1270 value; } rgTech;
    struct { unsigned char _pad[8]; STARS_CV_11e2 value; } szName;
    struct { unsigned char _pad[40]; STARS_CV_0072 value; } cMass;
    struct { unsigned char _pad[42]; STARS_CV_0021 value; } resCost;
    struct { unsigned char _pad[44]; STARS_CV_1044 value; } rgwtOreCost;
    struct { unsigned char _pad[50]; STARS_CV_0072 value; } ibmp;
    struct { unsigned char _pad[52]; STARS_CV_0072 value; } grAbility;
};
#define STARS_OFFSET_12a3_id 0
#define STARS_OFFSET_12a3_rgTech 2
#define STARS_OFFSET_12a3_szName 8
#define STARS_OFFSET_12a3_cMass 40
#define STARS_OFFSET_12a3_resCost 42
#define STARS_OFFSET_12a3_rgwtOreCost 44
#define STARS_OFFSET_12a3_ibmp 50
#define STARS_OFFSET_12a3_grAbility 52
typedef STARS_CV_12a3 STARS_CV_12a4[8];
typedef STARS_CV_1291 STARS_CV_12a5[15];
/* _terra: 54 bytes; each view begins at its recorded offset. */
union STARS_CV_12a6 {
    unsigned char raw[54];
    struct { STARS_CV_0072 value; } id;
    struct { unsigned char _pad[2]; STARS_CV_1270 value; } rgTech;
    struct { unsigned char _pad[8]; STARS_CV_11e2 value; } szName;
    struct { unsigned char _pad[40]; STARS_CV_0072 value; } cMass;
    struct { unsigned char _pad[42]; STARS_CV_0021 value; } resCost;
    struct { unsigned char _pad[44]; STARS_CV_1044 value; } rgwtOreCost;
    struct { unsigned char _pad[50]; STARS_CV_0072 value; } ibmp;
    struct { unsigned char _pad[52]; STARS_CV_0072 value; } grAbility;
};
#define STARS_OFFSET_12a6_id 0
#define STARS_OFFSET_12a6_rgTech 2
#define STARS_OFFSET_12a6_szName 8
#define STARS_OFFSET_12a6_cMass 40
#define STARS_OFFSET_12a6_resCost 42
#define STARS_OFFSET_12a6_rgwtOreCost 44
#define STARS_OFFSET_12a6_ibmp 50
#define STARS_OFFSET_12a6_grAbility 52
typedef STARS_CV_12a6 STARS_CV_12a7[20];
/* _bomb: 58 bytes; each view begins at its recorded offset. */
union STARS_CV_12a8 {
    unsigned char raw[58];
    struct { STARS_CV_0072 value; } id;
    struct { unsigned char _pad[2]; STARS_CV_1270 value; } rgTech;
    struct { unsigned char _pad[8]; STARS_CV_11e2 value; } szName;
    struct { unsigned char _pad[40]; STARS_CV_0072 value; } cMass;
    struct { unsigned char _pad[42]; STARS_CV_0021 value; } resCost;
    struct { unsigned char _pad[44]; STARS_CV_1044 value; } rgwtOreCost;
    struct { unsigned char _pad[50]; STARS_CV_0072 value; } ibmp;
    struct { unsigned char _pad[52]; STARS_CV_0072 value; } cRounds;
    struct { unsigned char _pad[54]; STARS_CV_0072 value; } dDmgCol;
    struct { unsigned char _pad[56]; STARS_CV_0072 value; } dDmgBldg;
};
#define STARS_OFFSET_12a8_id 0
#define STARS_OFFSET_12a8_rgTech 2
#define STARS_OFFSET_12a8_szName 8
#define STARS_OFFSET_12a8_cMass 40
#define STARS_OFFSET_12a8_resCost 42
#define STARS_OFFSET_12a8_rgwtOreCost 44
#define STARS_OFFSET_12a8_ibmp 50
#define STARS_OFFSET_12a8_cRounds 52
#define STARS_OFFSET_12a8_dDmgCol 54
#define STARS_OFFSET_12a8_dDmgBldg 56
typedef STARS_CV_12a8 STARS_CV_12a9[15];
/* _torp: 60 bytes; each view begins at its recorded offset. */
union STARS_CV_12aa {
    unsigned char raw[60];
    struct { STARS_CV_0072 value; } id;
    struct { unsigned char _pad[2]; STARS_CV_1270 value; } rgTech;
    struct { unsigned char _pad[8]; STARS_CV_11e2 value; } szName;
    struct { unsigned char _pad[40]; STARS_CV_0072 value; } cMass;
    struct { unsigned char _pad[42]; STARS_CV_0021 value; } resCost;
    struct { unsigned char _pad[44]; STARS_CV_1044 value; } rgwtOreCost;
    struct { unsigned char _pad[50]; STARS_CV_0072 value; } ibmp;
    struct { unsigned char _pad[52]; STARS_CV_0072 value; } dRangeMax;
    struct { unsigned char _pad[54]; STARS_CV_0072 value; } dp;
    struct { unsigned char _pad[56]; STARS_CV_0072 value; } init;
    struct { unsigned char _pad[58]; STARS_CV_0072 value; } dHitChance;
};
#define STARS_OFFSET_12aa_id 0
#define STARS_OFFSET_12aa_rgTech 2
#define STARS_OFFSET_12aa_szName 8
#define STARS_OFFSET_12aa_cMass 40
#define STARS_OFFSET_12aa_resCost 42
#define STARS_OFFSET_12aa_rgwtOreCost 44
#define STARS_OFFSET_12aa_ibmp 50
#define STARS_OFFSET_12aa_dRangeMax 52
#define STARS_OFFSET_12aa_dp 54
#define STARS_OFFSET_12aa_init 56
#define STARS_OFFSET_12aa_dHitChance 58
typedef STARS_CV_12aa STARS_CV_12ab[12];
/* _beam: 60 bytes; each view begins at its recorded offset. */
union STARS_CV_12ac {
    unsigned char raw[60];
    struct { STARS_CV_0072 value; } id;
    struct { unsigned char _pad[2]; STARS_CV_1270 value; } rgTech;
    struct { unsigned char _pad[8]; STARS_CV_11e2 value; } szName;
    struct { unsigned char _pad[40]; STARS_CV_0072 value; } cMass;
    struct { unsigned char _pad[42]; STARS_CV_0021 value; } resCost;
    struct { unsigned char _pad[44]; STARS_CV_1044 value; } rgwtOreCost;
    struct { unsigned char _pad[50]; STARS_CV_0072 value; } ibmp;
    struct { unsigned char _pad[52]; STARS_CV_0072 value; } dRangeMax;
    struct { unsigned char _pad[54]; STARS_CV_0072 value; } dp;
    struct { unsigned char _pad[56]; STARS_CV_0072 value; } init;
    struct { unsigned char _pad[58]; STARS_CV_0072 value; } grfAbilities;
};
#define STARS_OFFSET_12ac_id 0
#define STARS_OFFSET_12ac_rgTech 2
#define STARS_OFFSET_12ac_szName 8
#define STARS_OFFSET_12ac_cMass 40
#define STARS_OFFSET_12ac_resCost 42
#define STARS_OFFSET_12ac_rgwtOreCost 44
#define STARS_OFFSET_12ac_ibmp 50
#define STARS_OFFSET_12ac_dRangeMax 52
#define STARS_OFFSET_12ac_dp 54
#define STARS_OFFSET_12ac_init 56
#define STARS_OFFSET_12ac_grfAbilities 58
typedef STARS_CV_12ac STARS_CV_12ad[24];
typedef STARS_CV_1289 STARS_CV_12ae[32];
typedef STARS_CV_1008 STARS_CV_12af[22];
typedef STARS_CV_1289 STARS_CV_12b0[5];
typedef STARS_CV_1008 STARS_CV_12b1[4];
/* _specialsb: 56 bytes; each view begins at its recorded offset. */
union STARS_CV_12b2 {
    unsigned char raw[56];
    struct { STARS_CV_0072 value; } id;
    struct { unsigned char _pad[2]; STARS_CV_1270 value; } rgTech;
    struct { unsigned char _pad[8]; STARS_CV_11e2 value; } szName;
    struct { unsigned char _pad[40]; STARS_CV_0072 value; } cMass;
    struct { unsigned char _pad[42]; STARS_CV_0021 value; } resCost;
    struct { unsigned char _pad[44]; STARS_CV_1044 value; } rgwtOreCost;
    struct { unsigned char _pad[50]; STARS_CV_0072 value; } ibmp;
    struct { unsigned char _pad[52]; STARS_CV_0072 value; } grAbility;
    struct { unsigned char _pad[54]; STARS_CV_0072 value; } grAbility2;
};
#define STARS_OFFSET_12b2_id 0
#define STARS_OFFSET_12b2_rgTech 2
#define STARS_OFFSET_12b2_szName 8
#define STARS_OFFSET_12b2_cMass 40
#define STARS_OFFSET_12b2_resCost 42
#define STARS_OFFSET_12b2_rgwtOreCost 44
#define STARS_OFFSET_12b2_ibmp 50
#define STARS_OFFSET_12b2_grAbility 52
#define STARS_OFFSET_12b2_grAbility2 54
typedef STARS_CV_12b2 STARS_CV_12b3[16];
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_12b5(STARS_CV_0073, STARS_CV_1009, STARS_CV_0072);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_12b7(STARS_CV_0073, STARS_CV_0072, STARS_CV_0072, STARS_CV_0072, STARS_CV_0072, STARS_CV_0072);
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_12b9(STARS_CV_0073, STARS_CV_0073, STARS_CV_0072, STARS_CV_101b);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_12bb(STARS_CV_1018, STARS_CV_1078, STARS_CV_0072, STARS_CV_0072);
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_12bd(STARS_CV_1278, STARS_CV_0072);
typedef STARS_CV_1009 STARS_FAR STARS_CDECL STARS_CV_12be(STARS_CV_0072);
typedef STARS_CV_0070 STARS_CV_12bf[200];
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_12c0(STARS_CV_0270);
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_12c1(STARS_CV_100b);
typedef STARS_CV_0020 STARS_CV_12c2[30];
typedef STARS_CV_0072 STARS_CV_12c3[21];
typedef STARS_CV_0021 STARS_CV_12c4[14];
typedef STARS_CV_0021 STARS_CV_12c5[13];
typedef STARS_CV_0072 STARS_CV_12c6[13];
typedef STARS_CV_0021 STARS_CV_12c7[8];
typedef STARS_CV_0072 STARS_CV_12c8[8];
typedef STARS_CV_101a STARS_CV_12c9[16];
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_12cb(STARS_CV_0073, STARS_CV_0073);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_12cd(STARS_CV_0073, STARS_CV_0072, STARS_CV_0072, STARS_CV_0072, STARS_CV_0112, STARS_CV_118e, STARS_CV_0072, STARS_CV_0072);
typedef STARS_CV_1018 STARS_FAR STARS_CDECL STARS_CV_12cf(STARS_CV_0073, STARS_CV_0072);
typedef STARS_CV_0070 STARS_CV_12d0[90];
typedef STARS_CV_0070 STARS_CV_12d1[150];
typedef STARS_CV_1018 STARS_FAR STARS_CDECL STARS_CV_12d2(STARS_CV_0073, STARS_CV_0072, STARS_CV_0072);
typedef STARS_CV_0073 STARS_CV_12d3[8];
#ifdef STARS_NATIVE16
typedef STARS_CV_12d4 STARS_FAR *STARS_CV_12d5;
#else
typedef STARS_ULONG32 STARS_CV_12d5;
#endif
/* _msgplr: 12 bytes; each view begins at its recorded offset. */
union STARS_CV_12d4 {
    unsigned char raw[12];
    struct { STARS_CV_12d5 value; } lpmsgplrNext;
    struct { unsigned char _pad[4]; STARS_CV_0072 value; } iPlrFrom;
    struct { unsigned char _pad[6]; STARS_CV_0072 value; } iPlrTo;
    struct { unsigned char _pad[8]; STARS_CV_0072 value; } iInRe;
    struct { unsigned char _pad[10]; STARS_CV_0072 value; } cLen;
    /* rgbMsg: variable tail at offset 12; no fixed storage. */
};
#define STARS_OFFSET_12d4_lpmsgplrNext 0
#define STARS_OFFSET_12d4_iPlrFrom 4
#define STARS_OFFSET_12d4_iPlrTo 6
#define STARS_OFFSET_12d4_iInRe 8
#define STARS_OFFSET_12d4_cLen 10
#define STARS_OFFSET_12d4_rgbMsg 12
typedef STARS_CV_0072 STARS_CV_12f1[7];
/* _msgbig: 18 bytes; each view begins at its recorded offset. */
union STARS_CV_12d6 {
    unsigned char raw[18];
    struct { STARS_CV_0072 value; } iMsg;
    struct { unsigned char _pad[2]; STARS_CV_0072 value; } wGoto;
    struct { unsigned char _pad[4]; STARS_CV_12f1 value; } rgParam;
};
#define STARS_OFFSET_12d6_iMsg 0
#define STARS_OFFSET_12d6_wGoto 2
#define STARS_OFFSET_12d6_rgParam 4
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_12d8(STARS_CV_0072, STARS_CV_0072, STARS_CV_0072, STARS_CV_0072, STARS_CV_0072);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_12da(STARS_CV_0072, STARS_CV_0072, STARS_CV_0072, STARS_CV_0072, STARS_CV_0072, STARS_CV_0072, STARS_CV_0072, STARS_CV_0072, STARS_CV_0072, STARS_CV_0072);
typedef STARS_CV_0020 STARS_CV_12db[40];
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_12dd(STARS_CV_0120, STARS_CV_0072, STARS_CV_0072, STARS_CV_0072, STARS_CV_0072, STARS_CV_0072, STARS_CV_0072, STARS_CV_0072, STARS_CV_0072, STARS_CV_0072, STARS_CV_0072);
typedef STARS_CV_0020 STARS_CV_1409; /* bit-field backing storage; use recorded bit positions/masks */
typedef STARS_CV_0020 STARS_CV_140a; /* bit-field backing storage; use recorded bit positions/masks */
/* _msghdr: 4 bytes; each view begins at its recorded offset. */
union STARS_CV_12e0 {
    unsigned char raw[4];
    struct { STARS_CV_13d4 value; } iMsg;
    struct { STARS_CV_13d3 value; } grWord;
    struct { unsigned char _pad[2]; STARS_CV_0011 value; } wGoto;
};
#define STARS_OFFSET_12e0_iMsg 0
#define STARS_BITSHIFT_12e0_iMsg 0
#define STARS_BITMASK_12e0_iMsg 0x1ffUL
#define STARS_OFFSET_12e0_grWord 0
#define STARS_BITSHIFT_12e0_grWord 9
#define STARS_BITMASK_12e0_grWord 0x7fUL
#define STARS_OFFSET_12e0_wGoto 2
/* _msgturn: 5 bytes; each view begins at its recorded offset. */
union STARS_CV_12de {
    unsigned char raw[5];
    struct { STARS_CV_1409 value; } iPlr;
    struct { STARS_CV_140a value; } cbParams;
    struct { unsigned char _pad[1]; STARS_CV_12e0 value; } msghdr;
};
#define STARS_OFFSET_12de_iPlr 0
#define STARS_BITSHIFT_12de_iPlr 0
#define STARS_BITMASK_12de_iPlr 0xfUL
#define STARS_OFFSET_12de_cbParams 0
#define STARS_BITSHIFT_12de_cbParams 4
#define STARS_BITMASK_12de_cbParams 0xfUL
#define STARS_OFFSET_12de_msghdr 1
#ifdef STARS_NATIVE16
typedef STARS_CV_12de STARS_FAR *STARS_CV_12df;
#else
typedef STARS_ULONG32 STARS_CV_12df;
#endif
#ifdef STARS_NATIVE16
typedef STARS_CV_12e0 STARS_NEAR *STARS_CV_12e1;
#else
typedef unsigned short STARS_CV_12e1;
#endif
#ifdef STARS_NATIVE16
typedef STARS_CV_12d6 STARS_NEAR *STARS_CV_12e2;
#else
typedef unsigned short STARS_CV_12e2;
#endif
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_12e4(STARS_CV_0072, STARS_CV_12e2);
#ifdef STARS_NATIVE16
typedef STARS_CV_12e0 STARS_FAR *STARS_CV_12e5;
#else
typedef STARS_ULONG32 STARS_CV_12e5;
#endif
typedef STARS_CV_0170 STARS_FAR STARS_CDECL STARS_CV_12e7(STARS_CV_0170, STARS_CV_0272);
typedef STARS_CV_0070 STARS_CV_12e8[480];
typedef STARS_CV_0170 STARS_FAR STARS_CDECL STARS_CV_12ea(STARS_CV_0072, STARS_CV_0272);
typedef STARS_CV_0020 STARS_CV_12eb[1024];
/* _hdr: 2 bytes; each view begins at its recorded offset. */
union STARS_CV_12ec {
    unsigned char raw[2];
    struct { STARS_CV_1393 value; } cb;
    struct { STARS_CV_1397 value; } rt;
};
#define STARS_OFFSET_12ec_cb 0
#define STARS_BITSHIFT_12ec_cb 0
#define STARS_BITMASK_12ec_cb 0x3ffUL
#define STARS_OFFSET_12ec_rt 0
#define STARS_BITSHIFT_12ec_rt 10
#define STARS_BITMASK_12ec_rt 0x3fUL
typedef STARS_CV_0020 STARS_CV_12ed[49];
typedef STARS_CV_0073 STARS_CV_12ee[4];
typedef STARS_CV_0070 STARS_CV_12ef[22836];
typedef STARS_CV_0020 STARS_CV_12f0[387];
typedef STARS_CV_0070 STARS_CV_12f2[72];
typedef STARS_CV_0070 STARS_CV_12f3[387];
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_12f5(STARS_CV_10bf, STARS_CV_0072, STARS_CV_0072);
typedef STARS_CV_0022 STARS_CV_139f; /* bit-field backing storage; use recorded bit positions/masks */
typedef STARS_CV_0022 STARS_CV_13a0; /* bit-field backing storage; use recorded bit positions/masks */
/* _starpack: 4 bytes; each view begins at its recorded offset. */
union STARS_CV_12f6 {
    unsigned char raw[4];
    struct { STARS_CV_139e value; } dx;
    struct { STARS_CV_139f value; } y;
    struct { STARS_CV_13a0 value; } id;
};
#define STARS_OFFSET_12f6_dx 0
#define STARS_BITSHIFT_12f6_dx 0
#define STARS_BITMASK_12f6_dx 0x3ffUL
#define STARS_OFFSET_12f6_y 0
#define STARS_BITSHIFT_12f6_y 10
#define STARS_BITMASK_12f6_y 0xfffUL
#define STARS_OFFSET_12f6_id 0
#define STARS_BITSHIFT_12f6_id 22
#define STARS_BITMASK_12f6_id 0x3ffUL
typedef STARS_CV_0072 STARS_CV_12f7[5];
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_12f8(STARS_CV_0170);
typedef STARS_CV_0012 STARS_CV_12f9[10];
typedef STARS_CV_0070 STARS_CV_12fa[208];
typedef STARS_CV_101a STARS_CV_12fb[20];
typedef STARS_CV_1014 STARS_CV_12fc[16];
typedef STARS_CV_0073 STARS_CV_12fd[14];
#ifdef STARS_NATIVE16
typedef STARS_CV_1014 STARS_FAR *STARS_CV_12fe;
#else
typedef STARS_ULONG32 STARS_CV_12fe;
#endif
typedef STARS_CV_12fe STARS_FAR STARS_CDECL STARS_CV_12ff(STARS_CV_0072, STARS_CV_0072);
#ifdef STARS_NATIVE16
typedef STARS_CV_1012 STARS_NEAR *STARS_CV_1300;
#else
typedef unsigned short STARS_CV_1300;
#endif
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_1302(STARS_CV_1300, STARS_CV_0072, STARS_CV_0072);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_1304(STARS_CV_1300, STARS_CV_0072);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_1305(STARS_CV_1300, STARS_CV_0072, STARS_CV_0072);
typedef STARS_CV_0170 STARS_CV_1306[5];
typedef STARS_CV_1014 STARS_CV_1307[4];
typedef STARS_CV_1307 STARS_CV_1308[6];
typedef STARS_CV_126b STARS_CV_1309[9];
typedef STARS_CV_0020 STARS_CV_130a[5];
typedef STARS_CV_10be STARS_CV_130b[5];
typedef STARS_CV_0021 STARS_CV_130c[];
typedef STARS_CV_0170 STARS_CV_130d[7];
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_130f(STARS_CV_11ef, STARS_CV_0120);
typedef STARS_CV_0020 STARS_CV_1310[264];
#ifdef STARS_NATIVE16
typedef STARS_CV_0120 STARS_NEAR *STARS_CV_1311;
#else
typedef unsigned short STARS_CV_1311;
#endif
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_1313(STARS_CV_1009, STARS_CV_1311);
typedef STARS_CV_0020 STARS_CV_1314[147];
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_1316(STARS_CV_0170, STARS_CV_0072, STARS_CV_0072);
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_1318(STARS_CV_100b, STARS_CV_0072, STARS_CV_0072);
typedef STARS_CV_0020 STARS_CV_1319[80];
typedef STARS_CV_0020 STARS_CV_131a[134];
typedef STARS_CV_0020 STARS_CV_131b[33];
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_131d(STARS_CV_10bf, STARS_CV_0072);
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_131f(STARS_CV_100b, STARS_CV_0072, STARS_CV_0073);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_1321(STARS_CV_0073, STARS_CV_0072, STARS_CV_0170);
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_1322(STARS_CV_0072, STARS_CV_0072, STARS_CV_0072);
typedef STARS_CV_0021 STARS_CV_1500; /* bit-field backing storage; use recorded bit positions/masks */
typedef STARS_CV_0011 STARS_CV_1520; /* bit-field backing storage; use recorded bit positions/masks */
typedef STARS_CV_0011 STARS_CV_1521; /* bit-field backing storage; use recorded bit positions/masks */
/* _rtbof: 16 bytes; each view begins at its recorded offset. */
union STARS_CV_1323 {
    unsigned char raw[16];
    struct { STARS_CV_1145 value; } rgid;
    struct { unsigned char _pad[4]; STARS_CV_0012 value; } lidGame;
    struct { unsigned char _pad[8]; STARS_CV_14c3 value; } verInc;
    struct { unsigned char _pad[8]; STARS_CV_1500 value; } verMinor;
    struct { unsigned char _pad[8]; STARS_CV_1362 value; } verMajor;
    struct { unsigned char _pad[8]; STARS_CV_0021 value; } wVersion;
    struct { unsigned char _pad[10]; STARS_CV_0021 value; } turn;
    struct { unsigned char _pad[12]; STARS_CV_1520 value; } iPlayer;
    struct { unsigned char _pad[12]; STARS_CV_1521 value; } lSaltTime;
    struct { unsigned char _pad[14]; STARS_CV_135d value; } dt;
    struct { unsigned char _pad[14]; STARS_CV_135e value; } fDone;
    struct { unsigned char _pad[14]; STARS_CV_135f value; } fInUse;
    struct { unsigned char _pad[14]; STARS_CV_1360 value; } fMulti;
    struct { unsigned char _pad[14]; STARS_CV_1361 value; } fGameOverMan;
    struct { unsigned char _pad[14]; STARS_CV_137b value; } fCrippled;
    struct { unsigned char _pad[14]; STARS_CV_13d0 value; } wGen;
};
#define STARS_OFFSET_1323_rgid 0
#define STARS_OFFSET_1323_lidGame 4
#define STARS_OFFSET_1323_verInc 8
#define STARS_BITSHIFT_1323_verInc 0
#define STARS_BITMASK_1323_verInc 0x1fUL
#define STARS_OFFSET_1323_verMinor 8
#define STARS_BITSHIFT_1323_verMinor 5
#define STARS_BITMASK_1323_verMinor 0x7fUL
#define STARS_OFFSET_1323_verMajor 8
#define STARS_BITSHIFT_1323_verMajor 12
#define STARS_BITMASK_1323_verMajor 0xfUL
#define STARS_OFFSET_1323_wVersion 8
#define STARS_OFFSET_1323_turn 10
#define STARS_OFFSET_1323_iPlayer 12
#define STARS_BITSHIFT_1323_iPlayer 0
#define STARS_BITMASK_1323_iPlayer 0x1fUL
#define STARS_OFFSET_1323_lSaltTime 12
#define STARS_BITSHIFT_1323_lSaltTime 5
#define STARS_BITMASK_1323_lSaltTime 0x7ffUL
#define STARS_OFFSET_1323_dt 14
#define STARS_BITSHIFT_1323_dt 0
#define STARS_BITMASK_1323_dt 0xffUL
#define STARS_OFFSET_1323_fDone 14
#define STARS_BITSHIFT_1323_fDone 8
#define STARS_BITMASK_1323_fDone 0x1UL
#define STARS_OFFSET_1323_fInUse 14
#define STARS_BITSHIFT_1323_fInUse 9
#define STARS_BITMASK_1323_fInUse 0x1UL
#define STARS_OFFSET_1323_fMulti 14
#define STARS_BITSHIFT_1323_fMulti 10
#define STARS_BITMASK_1323_fMulti 0x1UL
#define STARS_OFFSET_1323_fGameOverMan 14
#define STARS_BITSHIFT_1323_fGameOverMan 11
#define STARS_BITMASK_1323_fGameOverMan 0x1UL
#define STARS_OFFSET_1323_fCrippled 14
#define STARS_BITSHIFT_1323_fCrippled 12
#define STARS_BITMASK_1323_fCrippled 0x1UL
#define STARS_OFFSET_1323_wGen 14
#define STARS_BITSHIFT_1323_wGen 13
#define STARS_BITMASK_1323_wGen 0x7UL
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_1325(STARS_CV_0072, STARS_CV_0072, STARS_CV_0203);
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_1327(STARS_CV_0203, STARS_CV_0073);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_1329(STARS_CV_100b, STARS_CV_104a, STARS_CV_103a, STARS_CV_0112, STARS_CV_0072, STARS_CV_0172, STARS_CV_0072);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_132b(STARS_CV_100b, STARS_CV_0072, STARS_CV_0072, STARS_CV_0072, STARS_CV_0112);
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_132d(STARS_CV_0072, STARS_CV_1004, STARS_CV_0072, STARS_CV_0072);
/* _xfer: 128 bytes; each view begins at its recorded offset. */
union STARS_CV_132e {
    unsigned char raw[128];
    struct { STARS_CV_0072 value; } id;
    struct { unsigned char _pad[2]; STARS_CV_0072 value; } grobj;
    struct { unsigned char _pad[4]; STARS_CV_1003 value; } fl;
    struct { unsigned char _pad[4]; STARS_CV_1002 value; } pl;
    struct { unsigned char _pad[4]; STARS_CV_1006 value; } th;
};
#define STARS_OFFSET_132e_id 0
#define STARS_OFFSET_132e_grobj 2
#define STARS_OFFSET_132e_fl 4
#define STARS_OFFSET_132e_pl 4
#define STARS_OFFSET_132e_th 4
typedef STARS_CV_132e STARS_CV_132f[2];
/* _xferfull: 25 bytes; each view begins at its recorded offset. */
union STARS_CV_1330 {
    unsigned char raw[25];
    struct { STARS_CV_0021 value; } id1;
    struct { unsigned char _pad[2]; STARS_CV_0021 value; } id2;
    struct { unsigned char _pad[4]; STARS_CV_1409 value; } grobj1;
    struct { unsigned char _pad[4]; STARS_CV_140a value; } grobj2;
    struct { unsigned char _pad[5]; STARS_CV_108f value; } rgcQuan;
};
#define STARS_OFFSET_1330_id1 0
#define STARS_OFFSET_1330_id2 2
#define STARS_OFFSET_1330_grobj1 4
#define STARS_BITSHIFT_1330_grobj1 0
#define STARS_BITMASK_1330_grobj1 0xfUL
#define STARS_OFFSET_1330_grobj2 4
#define STARS_BITSHIFT_1330_grobj2 4
#define STARS_BITMASK_1330_grobj2 0xfUL
#define STARS_OFFSET_1330_rgcQuan 5
#ifdef STARS_NATIVE16
typedef STARS_CV_1330 STARS_FAR *STARS_CV_1331;
#else
typedef STARS_ULONG32 STARS_CV_1331;
#endif
typedef STARS_CV_0021 STARS_CV_14af; /* bit-field backing storage; use recorded bit positions/masks */
/* _coldrop: 12 bytes; each view begins at its recorded offset. */
union STARS_CV_1332 {
    unsigned char raw[12];
    struct { STARS_CV_0072 value; } idFleetSrc;
    struct { unsigned char _pad[2]; STARS_CV_0072 value; } idPlr;
    struct { unsigned char _pad[4]; STARS_CV_0072 value; } idPlanetDst;
    struct { unsigned char _pad[6]; STARS_CV_13d7 value; } fCanColonize;
    struct { unsigned char _pad[6]; STARS_CV_14af value; } unused;
    struct { unsigned char _pad[8]; STARS_CV_0012 value; } cColonist;
};
#define STARS_OFFSET_1332_idFleetSrc 0
#define STARS_OFFSET_1332_idPlr 2
#define STARS_OFFSET_1332_idPlanetDst 4
#define STARS_OFFSET_1332_fCanColonize 6
#define STARS_BITSHIFT_1332_fCanColonize 0
#define STARS_BITMASK_1332_fCanColonize 0x1UL
#define STARS_OFFSET_1332_unused 6
#define STARS_BITSHIFT_1332_unused 1
#define STARS_BITMASK_1332_unused 0x7fffUL
#define STARS_OFFSET_1332_cColonist 8
#ifdef STARS_NATIVE16
typedef STARS_CV_1332 STARS_FAR *STARS_CV_1333;
#else
typedef STARS_ULONG32 STARS_CV_1333;
#endif
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_1335(STARS_CV_1004, STARS_CV_100b, STARS_CV_0012);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_1338(STARS_CV_1007, STARS_CV_0072);
typedef STARS_CV_0012 STARS_CV_1339[6];
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_133a(STARS_CV_100b, STARS_CV_0072, STARS_CV_0072);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_133c(STARS_CV_1004, STARS_CV_0172, STARS_CV_1007);
typedef STARS_CV_12c8 STARS_CV_133d[3];
typedef STARS_CV_10c2 STARS_CV_133e[3];
/* _turnserial: 16 bytes; each view begins at its recorded offset. */
union STARS_CV_133f {
    unsigned char raw[16];
    struct { STARS_CV_0012 value; } lSerialNumber;
    struct { unsigned char _pad[4]; STARS_CV_11e5 value; } rgbConfig;
    struct { unsigned char _pad[15]; STARS_CV_0020 value; } bPad;
};
#define STARS_OFFSET_133f_lSerialNumber 0
#define STARS_OFFSET_133f_rgbConfig 4
#define STARS_OFFSET_133f_bPad 15
#ifdef STARS_NATIVE16
typedef STARS_CV_133f STARS_FAR *STARS_CV_1340;
#else
typedef STARS_ULONG32 STARS_CV_1340;
#endif
/* _rthisthdr: 4 bytes; each view begins at its recorded offset. */
union STARS_CV_1342 {
    unsigned char raw[4];
    struct { STARS_CV_0011 value; } cPlanet;
    struct { unsigned char _pad[2]; STARS_CV_0011 value; } cPlanetExtra;
};
#define STARS_OFFSET_1342_cPlanet 0
#define STARS_OFFSET_1342_cPlanetExtra 2
typedef STARS_CV_1039 STARS_CV_1343[];
/* _rtChgProdQ: 2 bytes; each view begins at its recorded offset. */
union STARS_CV_1345 {
    unsigned char raw[2];
    struct { STARS_CV_0011 value; } id;
    /* rgprod: variable tail at offset 2; no fixed storage. */
};
#define STARS_OFFSET_1345_id 0
#define STARS_OFFSET_1345_rgprod 2
/* tagEVENTMSG: 10 bytes; each view begins at its recorded offset. */
union STARS_CV_134b {
    unsigned char raw[10];
    struct { STARS_CV_0073 value; } message;
    struct { unsigned char _pad[2]; STARS_CV_0073 value; } paramL;
    struct { unsigned char _pad[4]; STARS_CV_0073 value; } paramH;
    struct { unsigned char _pad[6]; STARS_CV_0022 value; } time;
};
#define STARS_OFFSET_134b_message 0
#define STARS_OFFSET_134b_paramL 2
#define STARS_OFFSET_134b_paramH 4
#define STARS_OFFSET_134b_time 6
/* _rtlogthing: 4 bytes; each view begins at its recorded offset. */
union STARS_CV_134e {
    unsigned char raw[4];
    struct { STARS_CV_0021 value; } idFull;
    struct { unsigned char _pad[2]; STARS_CV_0072 value; } fDetonate;
};
#define STARS_OFFSET_134e_idFull 0
#define STARS_OFFSET_134e_fDetonate 2
/* tagFINDREPLACE: 36 bytes; each view begins at its recorded offset. */
union STARS_CV_1354 {
    unsigned char raw[36];
    struct { STARS_CV_0022 value; } lStructSize;
    struct { unsigned char _pad[4]; STARS_CV_0073 value; } hwndOwner;
    struct { unsigned char _pad[6]; STARS_CV_0073 value; } hInstance;
    struct { unsigned char _pad[8]; STARS_CV_0022 value; } Flags;
    struct { unsigned char _pad[12]; STARS_CV_0270 value; } lpstrFindWhat;
    struct { unsigned char _pad[16]; STARS_CV_0270 value; } lpstrReplaceWith;
    struct { unsigned char _pad[20]; STARS_CV_0073 value; } wFindWhatLen;
    struct { unsigned char _pad[22]; STARS_CV_0073 value; } wReplaceWithLen;
    struct { unsigned char _pad[24]; STARS_CV_0012 value; } lCustData;
    struct { unsigned char _pad[28]; STARS_CV_1352 value; } lpfnHook;
    struct { unsigned char _pad[32]; STARS_CV_0270 value; } lpTemplateName;
};
#define STARS_OFFSET_1354_lStructSize 0
#define STARS_OFFSET_1354_hwndOwner 4
#define STARS_OFFSET_1354_hInstance 6
#define STARS_OFFSET_1354_Flags 8
#define STARS_OFFSET_1354_lpstrFindWhat 12
#define STARS_OFFSET_1354_lpstrReplaceWith 16
#define STARS_OFFSET_1354_wFindWhatLen 20
#define STARS_OFFSET_1354_wReplaceWithLen 22
#define STARS_OFFSET_1354_lCustData 24
#define STARS_OFFSET_1354_lpfnHook 28
#define STARS_OFFSET_1354_lpTemplateName 32
typedef STARS_CV_0022 STARS_CV_1355; /* bit-field backing storage; use recorded bit positions/masks */
typedef STARS_CV_0022 STARS_CV_1356; /* bit-field backing storage; use recorded bit positions/masks */
typedef STARS_CV_0022 STARS_CV_1357; /* bit-field backing storage; use recorded bit positions/masks */
typedef STARS_CV_0022 STARS_CV_1358; /* bit-field backing storage; use recorded bit positions/masks */
typedef STARS_CV_0022 STARS_CV_1359; /* bit-field backing storage; use recorded bit positions/masks */
/* _rtChgPlanetLong: 6 bytes; each view begins at its recorded offset. */
union STARS_CV_135b {
    unsigned char raw[6];
    struct { STARS_CV_0072 value; } id;
    struct { unsigned char _pad[2]; STARS_CV_0022 value; } ul;
    struct { unsigned char _pad[2]; STARS_CV_1355 value; } fNoResearch;
    struct { unsigned char _pad[2]; STARS_CV_1356 value; } idFling;
    struct { unsigned char _pad[2]; STARS_CV_1357 value; } iWarpFling;
    struct { unsigned char _pad[2]; STARS_CV_1358 value; } idRoute;
    struct { unsigned char _pad[2]; STARS_CV_1359 value; } unused;
};
#define STARS_OFFSET_135b_id 0
#define STARS_OFFSET_135b_ul 2
#define STARS_OFFSET_135b_fNoResearch 2
#define STARS_BITSHIFT_135b_fNoResearch 0
#define STARS_BITMASK_135b_fNoResearch 0x1UL
#define STARS_OFFSET_135b_idFling 2
#define STARS_BITSHIFT_135b_idFling 1
#define STARS_BITMASK_135b_idFling 0x3ffUL
#define STARS_OFFSET_135b_iWarpFling 2
#define STARS_BITSHIFT_135b_iWarpFling 11
#define STARS_BITMASK_135b_iWarpFling 0xfUL
#define STARS_OFFSET_135b_idRoute 2
#define STARS_BITSHIFT_135b_idRoute 15
#define STARS_BITMASK_135b_idRoute 0x3ffUL
#define STARS_OFFSET_135b_unused 2
#define STARS_BITSHIFT_135b_unused 25
#define STARS_BITMASK_135b_unused 0x7fUL
#ifdef STARS_NATIVE16
typedef STARS_CV_134b STARS_NEAR *STARS_CV_135c;
#else
typedef unsigned short STARS_CV_135c;
#endif
/* _planetsome: 23 bytes; each view begins at its recorded offset. */
union STARS_CV_1366 {
    unsigned char raw[23];
    struct { STARS_CV_0011 value; } id;
    struct { unsigned char _pad[2]; STARS_CV_0011 value; } iPlayer;
    struct { unsigned char _pad[4]; STARS_CV_135d value; } det;
    struct { unsigned char _pad[4]; STARS_CV_135e value; } fInclude;
    struct { unsigned char _pad[4]; STARS_CV_135f value; } fStarbase;
    struct { unsigned char _pad[4]; STARS_CV_1360 value; } unusedA;
    struct { unsigned char _pad[4]; STARS_CV_1361 value; } fFirstYear;
    struct { unsigned char _pad[4]; STARS_CV_1362 value; } unusedB;
    struct { unsigned char _pad[6]; STARS_CV_1363 value; } rgpctMinLevel;
    struct { unsigned char _pad[12]; STARS_CV_10bc value; } rgMinConc;
    struct { unsigned char _pad[15]; STARS_CV_10bc value; } rgEnvVar;
    struct { unsigned char _pad[18]; STARS_CV_10bc value; } rgEnvVarOrig;
    struct { unsigned char _pad[21]; STARS_CV_1364 value; } uPopGuess;
    struct { unsigned char _pad[21]; STARS_CV_1362 value; } uDefGuess;
    struct { unsigned char _pad[21]; STARS_CV_0021 value; } uGuesses;
};
#define STARS_OFFSET_1366_id 0
#define STARS_OFFSET_1366_iPlayer 2
#define STARS_OFFSET_1366_det 4
#define STARS_BITSHIFT_1366_det 0
#define STARS_BITMASK_1366_det 0xffUL
#define STARS_OFFSET_1366_fInclude 4
#define STARS_BITSHIFT_1366_fInclude 8
#define STARS_BITMASK_1366_fInclude 0x1UL
#define STARS_OFFSET_1366_fStarbase 4
#define STARS_BITSHIFT_1366_fStarbase 9
#define STARS_BITMASK_1366_fStarbase 0x1UL
#define STARS_OFFSET_1366_unusedA 4
#define STARS_BITSHIFT_1366_unusedA 10
#define STARS_BITMASK_1366_unusedA 0x1UL
#define STARS_OFFSET_1366_fFirstYear 4
#define STARS_BITSHIFT_1366_fFirstYear 11
#define STARS_BITMASK_1366_fFirstYear 0x1UL
#define STARS_OFFSET_1366_unusedB 4
#define STARS_BITSHIFT_1366_unusedB 12
#define STARS_BITMASK_1366_unusedB 0xfUL
#define STARS_OFFSET_1366_rgpctMinLevel 6
#define STARS_OFFSET_1366_rgMinConc 12
#define STARS_OFFSET_1366_rgEnvVar 15
#define STARS_OFFSET_1366_rgEnvVarOrig 18
#define STARS_OFFSET_1366_uPopGuess 21
#define STARS_BITSHIFT_1366_uPopGuess 0
#define STARS_BITMASK_1366_uPopGuess 0xfffUL
#define STARS_OFFSET_1366_uDefGuess 21
#define STARS_BITSHIFT_1366_uDefGuess 12
#define STARS_BITMASK_1366_uDefGuess 0xfUL
#define STARS_OFFSET_1366_uGuesses 21
/* tagRGBTRIPLE: 3 bytes; each view begins at its recorded offset. */
union STARS_CV_1368 {
    unsigned char raw[3];
    struct { STARS_CV_0020 value; } rgbtBlue;
    struct { unsigned char _pad[1]; STARS_CV_0020 value; } rgbtGreen;
    struct { unsigned char _pad[2]; STARS_CV_0020 value; } rgbtRed;
};
#define STARS_OFFSET_1368_rgbtBlue 0
#define STARS_OFFSET_1368_rgbtGreen 1
#define STARS_OFFSET_1368_rgbtRed 2
typedef STARS_CV_1368 STARS_CV_1369[1];
/* tagBITMAPCOREINFO: 15 bytes; each view begins at its recorded offset. */
union STARS_CV_136b {
    unsigned char raw[15];
    struct { STARS_CV_11b9 value; } bmciHeader;
    struct { unsigned char _pad[12]; STARS_CV_1369 value; } bmciColors;
};
#define STARS_OFFSET_136b_bmciHeader 0
#define STARS_OFFSET_136b_bmciColors 12
#ifdef STARS_NATIVE16
typedef STARS_CV_1354 STARS_FAR *STARS_CV_136c;
#else
typedef STARS_ULONG32 STARS_CV_136c;
#endif
#ifdef STARS_NATIVE16
typedef STARS_CV_1366 STARS_FAR *STARS_CV_136d;
#else
typedef STARS_ULONG32 STARS_CV_136d;
#endif
/* _exceptionl: 34 bytes; each view begins at its recorded offset. */
union STARS_CV_136f {
    unsigned char raw[34];
    struct { STARS_CV_0072 value; } type;
    struct { unsigned char _pad[2]; STARS_CV_0170 value; } name;
    struct { unsigned char _pad[4]; STARS_CV_0042 value; } arg1;
    struct { unsigned char _pad[14]; STARS_CV_0042 value; } arg2;
    struct { unsigned char _pad[24]; STARS_CV_0042 value; } retval;
};
#define STARS_OFFSET_136f_type 0
#define STARS_OFFSET_136f_name 2
#define STARS_OFFSET_136f_arg1 4
#define STARS_OFFSET_136f_arg2 14
#define STARS_OFFSET_136f_retval 24
#ifdef STARS_NATIVE16
typedef STARS_CV_136b STARS_NEAR *STARS_CV_1370;
#else
typedef unsigned short STARS_CV_1370;
#endif
#ifdef STARS_NATIVE16
typedef STARS_CV_134b STARS_FAR *STARS_CV_1372;
#else
typedef STARS_ULONG32 STARS_CV_1372;
#endif
#ifdef STARS_NATIVE16
typedef STARS_CV_1366 STARS_NEAR *STARS_CV_1373;
#else
typedef unsigned short STARS_CV_1373;
#endif
#ifdef STARS_NATIVE16
typedef STARS_CV_136b STARS_FAR *STARS_CV_1375;
#else
typedef STARS_ULONG32 STARS_CV_1375;
#endif
/* tagGLYPHMETRICS: 12 bytes; each view begins at its recorded offset. */
union STARS_CV_1377 {
    unsigned char raw[12];
    struct { STARS_CV_0073 value; } gmBlackBoxX;
    struct { unsigned char _pad[2]; STARS_CV_0073 value; } gmBlackBoxY;
    struct { unsigned char _pad[4]; STARS_CV_1018 value; } gmptGlyphOrigin;
    struct { unsigned char _pad[8]; STARS_CV_0072 value; } gmCellIncX;
    struct { unsigned char _pad[10]; STARS_CV_0072 value; } gmCellIncY;
};
#define STARS_OFFSET_1377_gmBlackBoxX 0
#define STARS_OFFSET_1377_gmBlackBoxY 2
#define STARS_OFFSET_1377_gmptGlyphOrigin 4
#define STARS_OFFSET_1377_gmCellIncX 8
#define STARS_OFFSET_1377_gmCellIncY 10
/* _complexl: 20 bytes; each view begins at its recorded offset. */
union STARS_CV_139a {
    unsigned char raw[20];
    struct { STARS_CV_0042 value; } x;
    struct { unsigned char _pad[10]; STARS_CV_0042 value; } y;
};
#define STARS_OFFSET_139a_x 0
#define STARS_OFFSET_139a_y 10
/* tagBITMAPFILEHEADER: 14 bytes; each view begins at its recorded offset. */
union STARS_CV_139d {
    unsigned char raw[14];
    struct { STARS_CV_0073 value; } bfType;
    struct { unsigned char _pad[2]; STARS_CV_0022 value; } bfSize;
    struct { unsigned char _pad[6]; STARS_CV_0073 value; } bfReserved1;
    struct { unsigned char _pad[8]; STARS_CV_0073 value; } bfReserved2;
    struct { unsigned char _pad[10]; STARS_CV_0022 value; } bfOffBits;
};
#define STARS_OFFSET_139d_bfType 0
#define STARS_OFFSET_139d_bfSize 2
#define STARS_OFFSET_139d_bfReserved1 6
#define STARS_OFFSET_139d_bfReserved2 8
#define STARS_OFFSET_139d_bfOffBits 10
#ifdef STARS_NATIVE16
typedef STARS_CV_11ab STARS_NEAR *STARS_CV_13a2;
#else
typedef unsigned short STARS_CV_13a2;
#endif
/* tagCLIENTCREATESTRUCT: 4 bytes; each view begins at its recorded offset. */
union STARS_CV_13a4 {
    unsigned char raw[4];
    struct { STARS_CV_0073 value; } hWindowMenu;
    struct { unsigned char _pad[2]; STARS_CV_0073 value; } idFirstChild;
};
#define STARS_OFFSET_13a4_hWindowMenu 0
#define STARS_OFFSET_13a4_idFirstChild 2
#ifdef STARS_NATIVE16
typedef STARS_CV_1017 STARS_FAR *STARS_CV_13a5;
#else
typedef STARS_ULONG32 STARS_CV_13a5;
#endif
/* tagWNDCLASS: 26 bytes; each view begins at its recorded offset. */
union STARS_CV_13a7 {
    unsigned char raw[26];
    struct { STARS_CV_0073 value; } style;
    struct { unsigned char _pad[2]; STARS_CV_13a5 value; } lpfnWndProc;
    struct { unsigned char _pad[6]; STARS_CV_0072 value; } cbClsExtra;
    struct { unsigned char _pad[8]; STARS_CV_0072 value; } cbWndExtra;
    struct { unsigned char _pad[10]; STARS_CV_0073 value; } hInstance;
    struct { unsigned char _pad[12]; STARS_CV_0073 value; } hIcon;
    struct { unsigned char _pad[14]; STARS_CV_0073 value; } hCursor;
    struct { unsigned char _pad[16]; STARS_CV_0073 value; } hbrBackground;
    struct { unsigned char _pad[18]; STARS_CV_0270 value; } lpszMenuName;
    struct { unsigned char _pad[22]; STARS_CV_0270 value; } lpszClassName;
};
#define STARS_OFFSET_13a7_style 0
#define STARS_OFFSET_13a7_lpfnWndProc 2
#define STARS_OFFSET_13a7_cbClsExtra 6
#define STARS_OFFSET_13a7_cbWndExtra 8
#define STARS_OFFSET_13a7_hInstance 10
#define STARS_OFFSET_13a7_hIcon 12
#define STARS_OFFSET_13a7_hCursor 14
#define STARS_OFFSET_13a7_hbrBackground 16
#define STARS_OFFSET_13a7_lpszMenuName 18
#define STARS_OFFSET_13a7_lpszClassName 22
#ifdef STARS_NATIVE16
typedef STARS_CV_13a7 STARS_NEAR *STARS_CV_13a8;
#else
typedef unsigned short STARS_CV_13a8;
#endif
/* tagMETAFILEPICT: 8 bytes; each view begins at its recorded offset. */
union STARS_CV_13ac {
    unsigned char raw[8];
    struct { STARS_CV_0072 value; } mm;
    struct { unsigned char _pad[2]; STARS_CV_0072 value; } xExt;
    struct { unsigned char _pad[4]; STARS_CV_0072 value; } yExt;
    struct { unsigned char _pad[6]; STARS_CV_0073 value; } hMF;
};
#define STARS_OFFSET_13ac_mm 0
#define STARS_OFFSET_13ac_xExt 2
#define STARS_OFFSET_13ac_yExt 4
#define STARS_OFFSET_13ac_hMF 6
/* tagDEBUGHOOKINFO: 14 bytes; each view begins at its recorded offset. */
union STARS_CV_13ae {
    unsigned char raw[14];
    struct { STARS_CV_0073 value; } hModuleHook;
    struct { unsigned char _pad[2]; STARS_CV_0012 value; } reserved;
    struct { unsigned char _pad[6]; STARS_CV_0012 value; } lParam;
    struct { unsigned char _pad[10]; STARS_CV_0073 value; } wParam;
    struct { unsigned char _pad[12]; STARS_CV_0072 value; } code;
};
#define STARS_OFFSET_13ae_hModuleHook 0
#define STARS_OFFSET_13ae_reserved 2
#define STARS_OFFSET_13ae_lParam 6
#define STARS_OFFSET_13ae_wParam 10
#define STARS_OFFSET_13ae_code 12
#ifdef STARS_NATIVE16
typedef STARS_CV_1002 STARS_NEAR *STARS_CV_13af;
#else
typedef unsigned short STARS_CV_13af;
#endif
#ifdef STARS_NATIVE16
typedef STARS_CV_1006 STARS_NEAR *STARS_CV_13b0;
#else
typedef unsigned short STARS_CV_13b0;
#endif
/* _obj: 2 bytes; each view begins at its recorded offset. */
union STARS_CV_13b2 {
    unsigned char raw[2];
    struct { STARS_CV_13af value; } ppl;
    struct { STARS_CV_1242 value; } pfl;
    struct { STARS_CV_13b0 value; } pth;
};
#define STARS_OFFSET_13b2_ppl 0
#define STARS_OFFSET_13b2_pfl 0
#define STARS_OFFSET_13b2_pth 0
typedef STARS_CV_0021 STARS_CV_13b4; /* bit-field backing storage; use recorded bit positions/masks */
#ifdef STARS_NATIVE16
typedef STARS_CV_13b6 STARS_NEAR *STARS_CV_13bb;
#else
typedef unsigned short STARS_CV_13bb;
#endif
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_13b9(STARS_CV_0073, STARS_CV_13bb, STARS_CV_13b2);
#ifdef STARS_NATIVE16
typedef STARS_CV_13b9 STARS_FAR *STARS_CV_13b8;
#else
typedef STARS_ULONG32 STARS_CV_13b8;
#endif
/* _tile: 16 bytes; each view begins at its recorded offset. */
union STARS_CV_13b6 {
    unsigned char raw[16];
    struct { STARS_CV_0072 value; } yTop;
    struct { unsigned char _pad[2]; STARS_CV_0072 value; } dyFull;
    struct { unsigned char _pad[4]; STARS_CV_0072 value; } grbit;
    struct { unsigned char _pad[6]; STARS_CV_13b8 value; } pfn;
    struct { unsigned char _pad[10]; STARS_CV_13b3 value; } iCol;
    struct { unsigned char _pad[10]; STARS_CV_13b4 value; } id;
    struct { unsigned char _pad[10]; STARS_CV_13b5 value; } fPopped;
    struct { unsigned char _pad[10]; STARS_CV_135e value; } fNullPtr;
    struct { unsigned char _pad[10]; STARS_CV_135f value; } fMinTitle;
    struct { unsigned char _pad[10]; STARS_CV_1360 value; } fErase;
    struct { unsigned char _pad[10]; STARS_CV_1361 value; } fFixCtls;
    struct { unsigned char _pad[10]; STARS_CV_137b value; } fMinDraw;
    struct { unsigned char _pad[12]; STARS_CV_1378 value; } fUnused;
    struct { unsigned char _pad[14]; STARS_CV_0021 value; } idh;
};
#define STARS_OFFSET_13b6_yTop 0
#define STARS_OFFSET_13b6_dyFull 2
#define STARS_OFFSET_13b6_grbit 4
#define STARS_OFFSET_13b6_pfn 6
#define STARS_OFFSET_13b6_iCol 10
#define STARS_BITSHIFT_13b6_iCol 0
#define STARS_BITMASK_13b6_iCol 0x7UL
#define STARS_OFFSET_13b6_id 10
#define STARS_BITSHIFT_13b6_id 3
#define STARS_BITMASK_13b6_id 0xfUL
#define STARS_OFFSET_13b6_fPopped 10
#define STARS_BITSHIFT_13b6_fPopped 7
#define STARS_BITMASK_13b6_fPopped 0x1UL
#define STARS_OFFSET_13b6_fNullPtr 10
#define STARS_BITSHIFT_13b6_fNullPtr 8
#define STARS_BITMASK_13b6_fNullPtr 0x1UL
#define STARS_OFFSET_13b6_fMinTitle 10
#define STARS_BITSHIFT_13b6_fMinTitle 9
#define STARS_BITMASK_13b6_fMinTitle 0x1UL
#define STARS_OFFSET_13b6_fErase 10
#define STARS_BITSHIFT_13b6_fErase 10
#define STARS_BITMASK_13b6_fErase 0x1UL
#define STARS_OFFSET_13b6_fFixCtls 10
#define STARS_BITSHIFT_13b6_fFixCtls 11
#define STARS_BITMASK_13b6_fFixCtls 0x1UL
#define STARS_OFFSET_13b6_fMinDraw 10
#define STARS_BITSHIFT_13b6_fMinDraw 12
#define STARS_BITMASK_13b6_fMinDraw 0x1UL
#define STARS_OFFSET_13b6_fUnused 12
#define STARS_BITSHIFT_13b6_fUnused 0
#define STARS_BITMASK_13b6_fUnused 0xfUL
#define STARS_OFFSET_13b6_idh 14
#ifdef STARS_NATIVE16
typedef STARS_CV_13a7 STARS_FAR *STARS_CV_13bc;
#else
typedef STARS_ULONG32 STARS_CV_13bc;
#endif
#ifdef STARS_NATIVE16
typedef STARS_CV_11ab STARS_FAR *STARS_CV_13bd;
#else
typedef STARS_ULONG32 STARS_CV_13bd;
#endif
/* HELPWININFO: 14 bytes; each view begins at its recorded offset. */
union STARS_CV_13df {
    unsigned char raw[14];
    struct { STARS_CV_0072 value; } wStructSize;
    struct { unsigned char _pad[2]; STARS_CV_0072 value; } x;
    struct { unsigned char _pad[4]; STARS_CV_0072 value; } y;
    struct { unsigned char _pad[6]; STARS_CV_0072 value; } dx;
    struct { unsigned char _pad[8]; STARS_CV_0072 value; } dy;
    struct { unsigned char _pad[10]; STARS_CV_0072 value; } wMax;
    struct { unsigned char _pad[12]; STARS_CV_126b value; } rgchMember;
};
#define STARS_OFFSET_13df_wStructSize 0
#define STARS_OFFSET_13df_x 2
#define STARS_OFFSET_13df_y 4
#define STARS_OFFSET_13df_dx 6
#define STARS_OFFSET_13df_dy 8
#define STARS_OFFSET_13df_wMax 10
#define STARS_OFFSET_13df_rgchMember 12
typedef STARS_CV_0070 STARS_CV_13e0[1];
/* MENUITEMTEMPLATE: 5 bytes; each view begins at its recorded offset. */
union STARS_CV_13e2 {
    unsigned char raw[5];
    struct { STARS_CV_0073 value; } mtOption;
    struct { unsigned char _pad[2]; STARS_CV_0073 value; } mtID;
    struct { unsigned char _pad[4]; STARS_CV_13e0 value; } mtString;
};
#define STARS_OFFSET_13e2_mtOption 0
#define STARS_OFFSET_13e2_mtID 2
#define STARS_OFFSET_13e2_mtString 4
/* MENUITEMTEMPLATEHEADER: 4 bytes; each view begins at its recorded offset. */
union STARS_CV_13e4 {
    unsigned char raw[4];
    struct { STARS_CV_0073 value; } versionNumber;
    struct { unsigned char _pad[2]; STARS_CV_0073 value; } offset;
};
#define STARS_OFFSET_13e4_versionNumber 0
#define STARS_OFFSET_13e4_offset 2
/* DOCINFO: 10 bytes; each view begins at its recorded offset. */
union STARS_CV_13e6 {
    unsigned char raw[10];
    struct { STARS_CV_0072 value; } cbSize;
    struct { unsigned char _pad[2]; STARS_CV_0270 value; } lpszDocName;
    struct { unsigned char _pad[6]; STARS_CV_0270 value; } lpszOutput;
};
#define STARS_OFFSET_13e6_cbSize 0
#define STARS_OFFSET_13e6_lpszDocName 2
#define STARS_OFFSET_13e6_lpszOutput 6
/* tagDEVNAMES: 8 bytes; each view begins at its recorded offset. */
union STARS_CV_13e8 {
    unsigned char raw[8];
    struct { STARS_CV_0073 value; } wDriverOffset;
    struct { unsigned char _pad[2]; STARS_CV_0073 value; } wDeviceOffset;
    struct { unsigned char _pad[4]; STARS_CV_0073 value; } wOutputOffset;
    struct { unsigned char _pad[6]; STARS_CV_0073 value; } wDefault;
};
#define STARS_OFFSET_13e8_wDriverOffset 0
#define STARS_OFFSET_13e8_wDeviceOffset 2
#define STARS_OFFSET_13e8_wOutputOffset 4
#define STARS_OFFSET_13e8_wDefault 6
/* tagDRVCONFIGINFO: 12 bytes; each view begins at its recorded offset. */
union STARS_CV_13ea {
    unsigned char raw[12];
    struct { STARS_CV_0022 value; } dwDCISize;
    struct { unsigned char _pad[4]; STARS_CV_0270 value; } lpszDCISectionName;
    struct { unsigned char _pad[8]; STARS_CV_0270 value; } lpszDCIAliasName;
};
#define STARS_OFFSET_13ea_dwDCISize 0
#define STARS_OFFSET_13ea_lpszDCISectionName 4
#define STARS_OFFSET_13ea_lpszDCIAliasName 8
/* _logxfer: 24 bytes; each view begins at its recorded offset. */
union STARS_CV_13ec {
    unsigned char raw[24];
    struct { STARS_CV_0011 value; } id;
    struct { unsigned char _pad[2]; STARS_CV_0011 value; } grobj;
    struct { unsigned char _pad[4]; STARS_CV_108f value; } rgdItem;
};
#define STARS_OFFSET_13ec_id 0
#define STARS_OFFSET_13ec_grobj 2
#define STARS_OFFSET_13ec_rgdItem 4
typedef STARS_CV_0021 STARS_CV_13ed; /* bit-field backing storage; use recorded bit positions/masks */
/* _btn: 14 bytes; each view begins at its recorded offset. */
union STARS_CV_13ef {
    unsigned char raw[14];
    struct { STARS_CV_101a value; } rc;
    struct { unsigned char _pad[8]; STARS_CV_0072 value; } bt;
    struct { unsigned char _pad[10]; STARS_CV_0072 value; } iVal;
    struct { unsigned char _pad[12]; STARS_CV_13d7 value; } fVisible;
    struct { unsigned char _pad[12]; STARS_CV_13d8 value; } fDisabled;
    struct { unsigned char _pad[12]; STARS_CV_13ed value; } iSide;
    struct { unsigned char _pad[12]; STARS_CV_1392 value; } fUnused;
};
#define STARS_OFFSET_13ef_rc 0
#define STARS_OFFSET_13ef_bt 8
#define STARS_OFFSET_13ef_iVal 10
#define STARS_OFFSET_13ef_fVisible 12
#define STARS_BITSHIFT_13ef_fVisible 0
#define STARS_BITMASK_13ef_fVisible 0x1UL
#define STARS_OFFSET_13ef_fDisabled 12
#define STARS_BITSHIFT_13ef_fDisabled 1
#define STARS_BITMASK_13ef_fDisabled 0x1UL
#define STARS_OFFSET_13ef_iSide 12
#define STARS_BITSHIFT_13ef_iSide 2
#define STARS_BITMASK_13ef_iSide 0x3UL
#define STARS_OFFSET_13ef_fUnused 12
#define STARS_BITSHIFT_13ef_fUnused 4
#define STARS_BITMASK_13ef_fUnused 0xfffUL
#ifdef STARS_NATIVE16
typedef STARS_CV_1206 STARS_FAR *STARS_CV_13f0;
#else
typedef STARS_ULONG32 STARS_CV_13f0;
#endif
#ifdef STARS_NATIVE16
typedef STARS_CV_13ae STARS_FAR *STARS_CV_13f1;
#else
typedef STARS_ULONG32 STARS_CV_13f1;
#endif
/* _kill: 8 bytes; each view begins at its recorded offset. */
union STARS_CV_13f3 {
    unsigned char raw[8];
    struct { STARS_CV_0020 value; } itok;
    struct { unsigned char _pad[1]; STARS_CV_0020 value; } grfWeapon;
    struct { unsigned char _pad[2]; STARS_CV_0021 value; } cshKill;
    struct { unsigned char _pad[4]; STARS_CV_0021 value; } dpShield;
    struct { unsigned char _pad[6]; STARS_CV_1158 value; } dv;
};
#define STARS_OFFSET_13f3_itok 0
#define STARS_OFFSET_13f3_grfWeapon 1
#define STARS_OFFSET_13f3_cshKill 2
#define STARS_OFFSET_13f3_dpShield 4
#define STARS_OFFSET_13f3_dv 6
#ifdef STARS_NATIVE16
typedef STARS_CV_13ac STARS_FAR *STARS_CV_13f4;
#else
typedef STARS_ULONG32 STARS_CV_13f4;
#endif
#ifdef STARS_NATIVE16
typedef STARS_CV_13a4 STARS_FAR *STARS_CV_13f7;
#else
typedef STARS_ULONG32 STARS_CV_13f7;
#endif
#ifdef STARS_NATIVE16
typedef STARS_CV_11c5 STARS_NEAR *STARS_CV_13f8;
#else
typedef unsigned short STARS_CV_13f8;
#endif
/* _logxferf: 36 bytes; each view begins at its recorded offset. */
union STARS_CV_13fb {
    unsigned char raw[36];
    struct { STARS_CV_0011 value; } id;
    struct { unsigned char _pad[2]; STARS_CV_0011 value; } grobj;
    struct { unsigned char _pad[4]; STARS_CV_13f9 value; } rgdItem;
};
#define STARS_OFFSET_13fb_id 0
#define STARS_OFFSET_13fb_grobj 2
#define STARS_OFFSET_13fb_rgdItem 4
/* _fleetsome: 12 bytes; each view begins at its recorded offset. */
union STARS_CV_13fd {
    unsigned char raw[12];
    struct { STARS_CV_0011 value; } id;
    struct { unsigned char _pad[2]; STARS_CV_0011 value; } iPlayer;
    struct { unsigned char _pad[4]; STARS_CV_135d value; } det;
    struct { unsigned char _pad[4]; STARS_CV_135e value; } fInclude;
    struct { unsigned char _pad[4]; STARS_CV_135f value; } fRepOrders;
    struct { unsigned char _pad[4]; STARS_CV_1360 value; } fDead;
    struct { unsigned char _pad[4]; STARS_CV_1361 value; } fByteCsh;
    struct { unsigned char _pad[4]; STARS_CV_1362 value; } unused;
    struct { unsigned char _pad[6]; STARS_CV_0011 value; } idPlanet;
    struct { unsigned char _pad[8]; STARS_CV_1018 value; } pt;
};
#define STARS_OFFSET_13fd_id 0
#define STARS_OFFSET_13fd_iPlayer 2
#define STARS_OFFSET_13fd_det 4
#define STARS_BITSHIFT_13fd_det 0
#define STARS_BITMASK_13fd_det 0xffUL
#define STARS_OFFSET_13fd_fInclude 4
#define STARS_BITSHIFT_13fd_fInclude 8
#define STARS_BITMASK_13fd_fInclude 0x1UL
#define STARS_OFFSET_13fd_fRepOrders 4
#define STARS_BITSHIFT_13fd_fRepOrders 9
#define STARS_BITMASK_13fd_fRepOrders 0x1UL
#define STARS_OFFSET_13fd_fDead 4
#define STARS_BITSHIFT_13fd_fDead 10
#define STARS_BITMASK_13fd_fDead 0x1UL
#define STARS_OFFSET_13fd_fByteCsh 4
#define STARS_BITSHIFT_13fd_fByteCsh 11
#define STARS_BITMASK_13fd_fByteCsh 0x1UL
#define STARS_OFFSET_13fd_unused 4
#define STARS_BITSHIFT_13fd_unused 12
#define STARS_BITMASK_13fd_unused 0xfUL
#define STARS_OFFSET_13fd_idPlanet 6
#define STARS_OFFSET_13fd_pt 8
#ifdef STARS_NATIVE16
typedef STARS_CV_1377 STARS_FAR *STARS_CV_13fe;
#else
typedef STARS_ULONG32 STARS_CV_13fe;
#endif
/* tagMETAHEADER: 18 bytes; each view begins at its recorded offset. */
union STARS_CV_1400 {
    unsigned char raw[18];
    struct { STARS_CV_0073 value; } mtType;
    struct { unsigned char _pad[2]; STARS_CV_0073 value; } mtHeaderSize;
    struct { unsigned char _pad[4]; STARS_CV_0073 value; } mtVersion;
    struct { unsigned char _pad[6]; STARS_CV_0022 value; } mtSize;
    struct { unsigned char _pad[10]; STARS_CV_0073 value; } mtNoObjects;
    struct { unsigned char _pad[12]; STARS_CV_0022 value; } mtMaxRecord;
    struct { unsigned char _pad[16]; STARS_CV_0073 value; } mtNoParameters;
};
#define STARS_OFFSET_1400_mtType 0
#define STARS_OFFSET_1400_mtHeaderSize 2
#define STARS_OFFSET_1400_mtVersion 4
#define STARS_OFFSET_1400_mtSize 6
#define STARS_OFFSET_1400_mtNoObjects 10
#define STARS_OFFSET_1400_mtMaxRecord 12
#define STARS_OFFSET_1400_mtNoParameters 16
/* tagWINDEBUGINFO: 26 bytes; each view begins at its recorded offset. */
union STARS_CV_1402 {
    unsigned char raw[26];
    struct { STARS_CV_0073 value; } flags;
    struct { unsigned char _pad[2]; STARS_CV_0022 value; } dwOptions;
    struct { unsigned char _pad[6]; STARS_CV_0022 value; } dwFilter;
    struct { unsigned char _pad[10]; STARS_CV_11d1 value; } achAllocModule;
    struct { unsigned char _pad[18]; STARS_CV_0022 value; } dwAllocBreak;
    struct { unsigned char _pad[22]; STARS_CV_0022 value; } dwAllocCount;
};
#define STARS_OFFSET_1402_flags 0
#define STARS_OFFSET_1402_dwOptions 2
#define STARS_OFFSET_1402_dwFilter 6
#define STARS_OFFSET_1402_achAllocModule 10
#define STARS_OFFSET_1402_dwAllocBreak 18
#define STARS_OFFSET_1402_dwAllocCount 22
#ifdef STARS_NATIVE16
typedef STARS_CV_139d STARS_NEAR *STARS_CV_1405;
#else
typedef unsigned short STARS_CV_1405;
#endif
/* tagLOGPEN: 10 bytes; each view begins at its recorded offset. */
union STARS_CV_1407 {
    unsigned char raw[10];
    struct { STARS_CV_0073 value; } lopnStyle;
    struct { unsigned char _pad[2]; STARS_CV_1018 value; } lopnWidth;
    struct { unsigned char _pad[6]; STARS_CV_0022 value; } lopnColor;
};
#define STARS_OFFSET_1407_lopnStyle 0
#define STARS_OFFSET_1407_lopnWidth 2
#define STARS_OFFSET_1407_lopnColor 6
#ifdef STARS_NATIVE16
typedef STARS_CV_11c5 STARS_FAR *STARS_CV_1408;
#else
typedef STARS_ULONG32 STARS_CV_1408;
#endif
/* _rtxfer: 7 bytes; each view begins at its recorded offset. */
union STARS_CV_140c {
    unsigned char raw[7];
    struct { STARS_CV_0021 value; } id1;
    struct { unsigned char _pad[2]; STARS_CV_0021 value; } id2;
    struct { unsigned char _pad[4]; STARS_CV_1409 value; } grobj1;
    struct { unsigned char _pad[4]; STARS_CV_140a value; } grobj2;
    struct { unsigned char _pad[5]; STARS_CV_0020 value; } grbitItems;
    struct { unsigned char _pad[6]; STARS_CV_13e0 value; } rgcQuan;
};
#define STARS_OFFSET_140c_id1 0
#define STARS_OFFSET_140c_id2 2
#define STARS_OFFSET_140c_grobj1 4
#define STARS_BITSHIFT_140c_grobj1 0
#define STARS_BITMASK_140c_grobj1 0xfUL
#define STARS_OFFSET_140c_grobj2 4
#define STARS_BITSHIFT_140c_grobj2 4
#define STARS_BITMASK_140c_grobj2 0xfUL
#define STARS_OFFSET_140c_grbitItems 5
#define STARS_OFFSET_140c_rgcQuan 6
/* tagFIXED: 4 bytes; each view begins at its recorded offset. */
union STARS_CV_140e {
    unsigned char raw[4];
    struct { STARS_CV_0073 value; } fract;
    struct { unsigned char _pad[2]; STARS_CV_0072 value; } value;
};
#define STARS_OFFSET_140e_fract 0
#define STARS_OFFSET_140e_value 2
#ifdef STARS_NATIVE16
typedef STARS_CV_13fd STARS_FAR *STARS_CV_140f;
#else
typedef STARS_ULONG32 STARS_CV_140f;
#endif
#ifdef STARS_NATIVE16
typedef STARS_CV_139d STARS_FAR *STARS_CV_1410;
#else
typedef STARS_ULONG32 STARS_CV_1410;
#endif
typedef STARS_CV_13f3 STARS_CV_1412[];
/* _compart: 52 bytes; each view begins at its recorded offset. */
union STARS_CV_1415 {
    unsigned char raw[52];
    struct { STARS_CV_0072 value; } id;
    struct { unsigned char _pad[2]; STARS_CV_1270 value; } rgTech;
    struct { unsigned char _pad[8]; STARS_CV_11e2 value; } szName;
    struct { unsigned char _pad[40]; STARS_CV_0072 value; } cMass;
    struct { unsigned char _pad[42]; STARS_CV_0021 value; } resCost;
    struct { unsigned char _pad[44]; STARS_CV_1044 value; } rgwtOreCost;
    struct { unsigned char _pad[50]; STARS_CV_0072 value; } ibmp;
};
#define STARS_OFFSET_1415_id 0
#define STARS_OFFSET_1415_rgTech 2
#define STARS_OFFSET_1415_szName 8
#define STARS_OFFSET_1415_cMass 40
#define STARS_OFFSET_1415_resCost 42
#define STARS_OFFSET_1415_rgwtOreCost 44
#define STARS_OFFSET_1415_ibmp 50
#ifdef STARS_NATIVE16
typedef STARS_CV_13e8 STARS_FAR *STARS_CV_1416;
#else
typedef STARS_ULONG32 STARS_CV_1416;
#endif
#ifdef STARS_NATIVE16
typedef STARS_CV_13ea STARS_NEAR *STARS_CV_1417;
#else
typedef unsigned short STARS_CV_1417;
#endif
typedef STARS_CV_0073 STARS_CV_1419; /* bit-field backing storage; use recorded bit positions/masks */
/* tagDCB: 25 bytes; each view begins at its recorded offset. */
union STARS_CV_141d {
    unsigned char raw[25];
    struct { STARS_CV_0020 value; } Id;
    struct { unsigned char _pad[1]; STARS_CV_0073 value; } BaudRate;
    struct { unsigned char _pad[3]; STARS_CV_0020 value; } ByteSize;
    struct { unsigned char _pad[4]; STARS_CV_0020 value; } Parity;
    struct { unsigned char _pad[5]; STARS_CV_0020 value; } StopBits;
    struct { unsigned char _pad[6]; STARS_CV_0073 value; } RlsTimeout;
    struct { unsigned char _pad[8]; STARS_CV_0073 value; } CtsTimeout;
    struct { unsigned char _pad[10]; STARS_CV_0073 value; } DsrTimeout;
    struct { unsigned char _pad[12]; STARS_CV_13bf value; } fBinary;
    struct { unsigned char _pad[12]; STARS_CV_13c0 value; } fRtsDisable;
    struct { unsigned char _pad[12]; STARS_CV_13c1 value; } fParity;
    struct { unsigned char _pad[12]; STARS_CV_13c2 value; } fOutxCtsFlow;
    struct { unsigned char _pad[12]; STARS_CV_13c3 value; } fOutxDsrFlow;
    struct { unsigned char _pad[12]; STARS_CV_1419 value; } fDummy;
    struct { unsigned char _pad[12]; STARS_CV_141a value; } fDtrDisable;
    struct { unsigned char _pad[12]; STARS_CV_141b value; } fOutX;
    struct { unsigned char _pad[12]; STARS_CV_13c6 value; } fInX;
    struct { unsigned char _pad[12]; STARS_CV_13c7 value; } fPeChar;
    struct { unsigned char _pad[12]; STARS_CV_13c8 value; } fNull;
    struct { unsigned char _pad[12]; STARS_CV_13c9 value; } fChEvt;
    struct { unsigned char _pad[12]; STARS_CV_13ca value; } fDtrflow;
    struct { unsigned char _pad[12]; STARS_CV_13cb value; } fRtsflow;
    struct { unsigned char _pad[12]; STARS_CV_13cc value; } fDummy2;
    struct { unsigned char _pad[14]; STARS_CV_0070 value; } XonChar;
    struct { unsigned char _pad[15]; STARS_CV_0070 value; } XoffChar;
    struct { unsigned char _pad[16]; STARS_CV_0073 value; } XonLim;
    struct { unsigned char _pad[18]; STARS_CV_0073 value; } XoffLim;
    struct { unsigned char _pad[20]; STARS_CV_0070 value; } PeChar;
    struct { unsigned char _pad[21]; STARS_CV_0070 value; } EofChar;
    struct { unsigned char _pad[22]; STARS_CV_0070 value; } EvtChar;
    struct { unsigned char _pad[23]; STARS_CV_0073 value; } TxDelay;
};
#define STARS_OFFSET_141d_Id 0
#define STARS_OFFSET_141d_BaudRate 1
#define STARS_OFFSET_141d_ByteSize 3
#define STARS_OFFSET_141d_Parity 4
#define STARS_OFFSET_141d_StopBits 5
#define STARS_OFFSET_141d_RlsTimeout 6
#define STARS_OFFSET_141d_CtsTimeout 8
#define STARS_OFFSET_141d_DsrTimeout 10
#define STARS_OFFSET_141d_fBinary 12
#define STARS_BITSHIFT_141d_fBinary 0
#define STARS_BITMASK_141d_fBinary 0x1UL
#define STARS_OFFSET_141d_fRtsDisable 12
#define STARS_BITSHIFT_141d_fRtsDisable 1
#define STARS_BITMASK_141d_fRtsDisable 0x1UL
#define STARS_OFFSET_141d_fParity 12
#define STARS_BITSHIFT_141d_fParity 2
#define STARS_BITMASK_141d_fParity 0x1UL
#define STARS_OFFSET_141d_fOutxCtsFlow 12
#define STARS_BITSHIFT_141d_fOutxCtsFlow 3
#define STARS_BITMASK_141d_fOutxCtsFlow 0x1UL
#define STARS_OFFSET_141d_fOutxDsrFlow 12
#define STARS_BITSHIFT_141d_fOutxDsrFlow 4
#define STARS_BITMASK_141d_fOutxDsrFlow 0x1UL
#define STARS_OFFSET_141d_fDummy 12
#define STARS_BITSHIFT_141d_fDummy 5
#define STARS_BITMASK_141d_fDummy 0x3UL
#define STARS_OFFSET_141d_fDtrDisable 12
#define STARS_BITSHIFT_141d_fDtrDisable 7
#define STARS_BITMASK_141d_fDtrDisable 0x1UL
#define STARS_OFFSET_141d_fOutX 12
#define STARS_BITSHIFT_141d_fOutX 8
#define STARS_BITMASK_141d_fOutX 0x1UL
#define STARS_OFFSET_141d_fInX 12
#define STARS_BITSHIFT_141d_fInX 9
#define STARS_BITMASK_141d_fInX 0x1UL
#define STARS_OFFSET_141d_fPeChar 12
#define STARS_BITSHIFT_141d_fPeChar 10
#define STARS_BITMASK_141d_fPeChar 0x1UL
#define STARS_OFFSET_141d_fNull 12
#define STARS_BITSHIFT_141d_fNull 11
#define STARS_BITMASK_141d_fNull 0x1UL
#define STARS_OFFSET_141d_fChEvt 12
#define STARS_BITSHIFT_141d_fChEvt 12
#define STARS_BITMASK_141d_fChEvt 0x1UL
#define STARS_OFFSET_141d_fDtrflow 12
#define STARS_BITSHIFT_141d_fDtrflow 13
#define STARS_BITMASK_141d_fDtrflow 0x1UL
#define STARS_OFFSET_141d_fRtsflow 12
#define STARS_BITSHIFT_141d_fRtsflow 14
#define STARS_BITMASK_141d_fRtsflow 0x1UL
#define STARS_OFFSET_141d_fDummy2 12
#define STARS_BITSHIFT_141d_fDummy2 15
#define STARS_BITMASK_141d_fDummy2 0x1UL
#define STARS_OFFSET_141d_XonChar 14
#define STARS_OFFSET_141d_XoffChar 15
#define STARS_OFFSET_141d_XonLim 16
#define STARS_OFFSET_141d_XoffLim 18
#define STARS_OFFSET_141d_PeChar 20
#define STARS_OFFSET_141d_EofChar 21
#define STARS_OFFSET_141d_EvtChar 22
#define STARS_OFFSET_141d_TxDelay 23
#ifdef STARS_NATIVE16
typedef STARS_CV_13fd STARS_NEAR *STARS_CV_141e;
#else
typedef unsigned short STARS_CV_141e;
#endif
typedef STARS_CV_0011 STARS_CV_141f[1];
/* _rtxferx: 8 bytes; each view begins at its recorded offset. */
union STARS_CV_1421 {
    unsigned char raw[8];
    struct { STARS_CV_0021 value; } id1;
    struct { unsigned char _pad[2]; STARS_CV_0021 value; } id2;
    struct { unsigned char _pad[4]; STARS_CV_1409 value; } grobj1;
    struct { unsigned char _pad[4]; STARS_CV_140a value; } grobj2;
    struct { unsigned char _pad[5]; STARS_CV_0020 value; } grbitItems;
    struct { unsigned char _pad[6]; STARS_CV_141f value; } rgcQuan;
};
#define STARS_OFFSET_1421_id1 0
#define STARS_OFFSET_1421_id2 2
#define STARS_OFFSET_1421_grobj1 4
#define STARS_BITSHIFT_1421_grobj1 0
#define STARS_BITMASK_1421_grobj1 0xfUL
#define STARS_OFFSET_1421_grobj2 4
#define STARS_BITSHIFT_1421_grobj2 4
#define STARS_BITMASK_1421_grobj2 0xfUL
#define STARS_OFFSET_1421_grbitItems 5
#define STARS_OFFSET_1421_rgcQuan 6
#ifdef STARS_NATIVE16
typedef STARS_CV_141d STARS_FAR *STARS_CV_1422;
#else
typedef STARS_ULONG32 STARS_CV_1422;
#endif
typedef STARS_CV_0072 STARS_FAR STARS_PASCAL STARS_CV_1424(STARS_CV_0270, STARS_CV_0072, STARS_CV_0072, STARS_CV_0072);
#ifdef STARS_NATIVE16
typedef STARS_CV_1424 STARS_FAR *STARS_CV_1425;
#else
typedef STARS_ULONG32 STARS_CV_1425;
#endif
#ifdef STARS_NATIVE16
typedef STARS_CV_140e STARS_FAR *STARS_CV_1426;
#else
typedef STARS_ULONG32 STARS_CV_1426;
#endif
#ifdef STARS_NATIVE16
typedef STARS_CV_13ea STARS_FAR *STARS_CV_1427;
#else
typedef STARS_ULONG32 STARS_CV_1427;
#endif
/* tagCOMSTAT: 5 bytes; each view begins at its recorded offset. */
union STARS_CV_142b {
    unsigned char raw[5];
    struct { STARS_CV_0020 value; } status;
    struct { unsigned char _pad[1]; STARS_CV_0073 value; } cbInQue;
    struct { unsigned char _pad[3]; STARS_CV_0073 value; } cbOutQue;
};
#define STARS_OFFSET_142b_status 0
#define STARS_OFFSET_142b_cbInQue 1
#define STARS_OFFSET_142b_cbOutQue 3
/* tagMAT2: 16 bytes; each view begins at its recorded offset. */
union STARS_CV_142d {
    unsigned char raw[16];
    struct { STARS_CV_140e value; } eM11;
    struct { unsigned char _pad[4]; STARS_CV_140e value; } eM12;
    struct { unsigned char _pad[8]; STARS_CV_140e value; } eM21;
    struct { unsigned char _pad[12]; STARS_CV_140e value; } eM22;
};
#define STARS_OFFSET_142d_eM11 0
#define STARS_OFFSET_142d_eM12 4
#define STARS_OFFSET_142d_eM21 8
#define STARS_OFFSET_142d_eM22 12
#ifdef STARS_NATIVE16
typedef STARS_CV_1407 STARS_NEAR *STARS_CV_142e;
#else
typedef unsigned short STARS_CV_142e;
#endif
typedef STARS_CV_0012 STARS_CV_142f[1];
/* _rtxferl: 10 bytes; each view begins at its recorded offset. */
union STARS_CV_1431 {
    unsigned char raw[10];
    struct { STARS_CV_0021 value; } id1;
    struct { unsigned char _pad[2]; STARS_CV_0021 value; } id2;
    struct { unsigned char _pad[4]; STARS_CV_1409 value; } grobj1;
    struct { unsigned char _pad[4]; STARS_CV_140a value; } grobj2;
    struct { unsigned char _pad[5]; STARS_CV_0020 value; } grbitItems;
    struct { unsigned char _pad[6]; STARS_CV_142f value; } rgcQuan;
};
#define STARS_OFFSET_1431_id1 0
#define STARS_OFFSET_1431_id2 2
#define STARS_OFFSET_1431_grobj1 4
#define STARS_BITSHIFT_1431_grobj1 0
#define STARS_BITMASK_1431_grobj1 0xfUL
#define STARS_OFFSET_1431_grobj2 4
#define STARS_BITSHIFT_1431_grobj2 4
#define STARS_BITMASK_1431_grobj2 0xfUL
#define STARS_OFFSET_1431_grbitItems 5
#define STARS_OFFSET_1431_rgcQuan 6
#ifdef STARS_NATIVE16
typedef STARS_CV_1368 STARS_FAR *STARS_CV_1433;
#else
typedef STARS_ULONG32 STARS_CV_1433;
#endif
#ifdef STARS_NATIVE16
typedef STARS_CV_1407 STARS_FAR *STARS_CV_1434;
#else
typedef STARS_ULONG32 STARS_CV_1434;
#endif
/* _rtxferf: 9 bytes; each view begins at its recorded offset. */
union STARS_CV_1436 {
    unsigned char raw[9];
    struct { STARS_CV_0021 value; } id1;
    struct { unsigned char _pad[2]; STARS_CV_0021 value; } id2;
    struct { unsigned char _pad[4]; STARS_CV_1409 value; } grobj1;
    struct { unsigned char _pad[4]; STARS_CV_140a value; } grobj2;
    struct { unsigned char _pad[5]; STARS_CV_0021 value; } grbitItems;
    struct { unsigned char _pad[7]; STARS_CV_141f value; } rgcQuan;
};
#define STARS_OFFSET_1436_id1 0
#define STARS_OFFSET_1436_id2 2
#define STARS_OFFSET_1436_grobj1 4
#define STARS_BITSHIFT_1436_grobj1 0
#define STARS_BITMASK_1436_grobj1 0xfUL
#define STARS_OFFSET_1436_grobj2 4
#define STARS_BITSHIFT_1436_grobj2 4
#define STARS_BITMASK_1436_grobj2 0xfUL
#define STARS_OFFSET_1436_grbitItems 5
#define STARS_OFFSET_1436_rgcQuan 7
/* _btlrec26: 6 bytes; each view begins at its recorded offset. */
union STARS_CV_1438 {
    unsigned char raw[6];
    struct { STARS_CV_0020 value; } itok;
    struct { unsigned char _pad[1]; STARS_CV_0020 value; } brcDest;
    struct { unsigned char _pad[2]; STARS_CV_0020 value; } itokAttack;
    struct { unsigned char _pad[3]; STARS_CV_0020 value; } ctok;
    struct { unsigned char _pad[4]; STARS_CV_1378 value; } iRound;
    struct { unsigned char _pad[4]; STARS_CV_1379 value; } dzDis;
    struct { unsigned char _pad[4]; STARS_CV_1411 value; } unused;
    /* rgkill: variable tail at offset 6; no fixed storage. */
};
#define STARS_OFFSET_1438_itok 0
#define STARS_OFFSET_1438_brcDest 1
#define STARS_OFFSET_1438_itokAttack 2
#define STARS_OFFSET_1438_ctok 3
#define STARS_OFFSET_1438_iRound 4
#define STARS_BITSHIFT_1438_iRound 0
#define STARS_BITMASK_1438_iRound 0xfUL
#define STARS_OFFSET_1438_dzDis 4
#define STARS_BITSHIFT_1438_dzDis 4
#define STARS_BITMASK_1438_dzDis 0xfUL
#define STARS_OFFSET_1438_unused 4
#define STARS_BITSHIFT_1438_unused 8
#define STARS_BITMASK_1438_unused 0xffUL
#define STARS_OFFSET_1438_rgkill 6
/* tagRGBQUAD: 4 bytes; each view begins at its recorded offset. */
union STARS_CV_143c {
    unsigned char raw[4];
    struct { STARS_CV_0020 value; } rgbBlue;
    struct { unsigned char _pad[1]; STARS_CV_0020 value; } rgbGreen;
    struct { unsigned char _pad[2]; STARS_CV_0020 value; } rgbRed;
    struct { unsigned char _pad[3]; STARS_CV_0020 value; } rgbReserved;
};
#define STARS_OFFSET_143c_rgbBlue 0
#define STARS_OFFSET_143c_rgbGreen 1
#define STARS_OFFSET_143c_rgbRed 2
#define STARS_OFFSET_143c_rgbReserved 3
typedef STARS_CV_0072 STARS_NEAR STARS_PASCAL STARS_CV_143d();
#ifdef STARS_NATIVE16
typedef STARS_CV_143d STARS_NEAR *STARS_CV_143e;
#else
typedef unsigned short STARS_CV_143e;
#endif
/* _rtshipint: 4 bytes; each view begins at its recorded offset. */
union STARS_CV_1443 {
    unsigned char raw[4];
    struct { STARS_CV_0011 value; } id;
    struct { unsigned char _pad[2]; STARS_CV_0011 value; } i;
};
#define STARS_OFFSET_1443_id 0
#define STARS_OFFSET_1443_i 2
#ifdef STARS_NATIVE16
typedef STARS_CV_142d STARS_FAR *STARS_CV_1444;
#else
typedef STARS_ULONG32 STARS_CV_1444;
#endif
/* _aistarbase: 20 bytes; each view begins at its recorded offset. */
union STARS_CV_1446 {
    unsigned char raw[20];
    struct { STARS_CV_0072 value; } idPlanet;
    struct { unsigned char _pad[2]; STARS_CV_0072 value; } cFreighter;
    struct { unsigned char _pad[4]; STARS_CV_12c8 value; } rgflid;
};
#define STARS_OFFSET_1446_idPlanet 0
#define STARS_OFFSET_1446_cFreighter 2
#define STARS_OFFSET_1446_rgflid 4
/* _exception: 28 bytes; each view begins at its recorded offset. */
union STARS_CV_1448 {
    unsigned char raw[28];
    struct { STARS_CV_0072 value; } type;
    struct { unsigned char _pad[2]; STARS_CV_0170 value; } name;
    struct { unsigned char _pad[4]; STARS_CV_0041 value; } arg1;
    struct { unsigned char _pad[12]; STARS_CV_0041 value; } arg2;
    struct { unsigned char _pad[20]; STARS_CV_0041 value; } retval;
};
#define STARS_OFFSET_1448_type 0
#define STARS_OFFSET_1448_name 2
#define STARS_OFFSET_1448_arg1 4
#define STARS_OFFSET_1448_arg2 12
#define STARS_OFFSET_1448_retval 20
typedef STARS_CV_0073 STARS_CV_1449[1];
/* tagHANDLETABLE: 2 bytes; each view begins at its recorded offset. */
union STARS_CV_144b {
    unsigned char raw[2];
    struct { STARS_CV_1449 value; } objectHandle;
};
#define STARS_OFFSET_144b_objectHandle 0
#ifdef STARS_NATIVE16
typedef STARS_CV_1047 STARS_NEAR *STARS_CV_144c;
#else
typedef unsigned short STARS_CV_144c;
#endif
/* _sbar: 12 bytes; each view begins at its recorded offset. */
union STARS_CV_144e {
    unsigned char raw[12];
    struct { STARS_CV_0072 value; } grbit;
    struct { unsigned char _pad[2]; STARS_CV_0072 value; } id;
    struct { unsigned char _pad[4]; STARS_CV_1018 value; } pt;
    struct { unsigned char _pad[8]; STARS_CV_0170 value; } psz;
    struct { unsigned char _pad[10]; STARS_CV_144c value; } pscan;
};
#define STARS_OFFSET_144e_grbit 0
#define STARS_OFFSET_144e_id 2
#define STARS_OFFSET_144e_pt 4
#define STARS_OFFSET_144e_psz 8
#define STARS_OFFSET_144e_pscan 10
#ifdef STARS_NATIVE16
typedef STARS_CV_13e6 STARS_FAR *STARS_CV_144f;
#else
typedef STARS_ULONG32 STARS_CV_144f;
#endif
/* _rtshipint2: 6 bytes; each view begins at its recorded offset. */
union STARS_CV_1451 {
    unsigned char raw[6];
    struct { STARS_CV_0011 value; } id;
    struct { unsigned char _pad[2]; STARS_CV_0011 value; } i;
    struct { unsigned char _pad[4]; STARS_CV_0011 value; } i2;
};
#define STARS_OFFSET_1451_id 0
#define STARS_OFFSET_1451_i 2
#define STARS_OFFSET_1451_i2 4
/* tagWINDOWPOS: 14 bytes; each view begins at its recorded offset. */
union STARS_CV_1454 {
    unsigned char raw[14];
    struct { STARS_CV_0073 value; } hwnd;
    struct { unsigned char _pad[2]; STARS_CV_0073 value; } hwndInsertAfter;
    struct { unsigned char _pad[4]; STARS_CV_0072 value; } x;
    struct { unsigned char _pad[6]; STARS_CV_0072 value; } y;
    struct { unsigned char _pad[8]; STARS_CV_0072 value; } cx;
    struct { unsigned char _pad[10]; STARS_CV_0072 value; } cy;
    struct { unsigned char _pad[12]; STARS_CV_0073 value; } flags;
};
#define STARS_OFFSET_1454_hwnd 0
#define STARS_OFFSET_1454_hwndInsertAfter 2
#define STARS_OFFSET_1454_x 4
#define STARS_OFFSET_1454_y 6
#define STARS_OFFSET_1454_cx 8
#define STARS_OFFSET_1454_cy 10
#define STARS_OFFSET_1454_flags 12
/* tagCREATESTRUCT: 34 bytes; each view begins at its recorded offset. */
union STARS_CV_1457 {
    unsigned char raw[34];
    struct { STARS_CV_0203 value; } lpCreateParams;
    struct { unsigned char _pad[4]; STARS_CV_0073 value; } hInstance;
    struct { unsigned char _pad[6]; STARS_CV_0073 value; } hMenu;
    struct { unsigned char _pad[8]; STARS_CV_0073 value; } hwndParent;
    struct { unsigned char _pad[10]; STARS_CV_0072 value; } cy;
    struct { unsigned char _pad[12]; STARS_CV_0072 value; } cx;
    struct { unsigned char _pad[14]; STARS_CV_0072 value; } y;
    struct { unsigned char _pad[16]; STARS_CV_0072 value; } x;
    struct { unsigned char _pad[18]; STARS_CV_0012 value; } style;
    struct { unsigned char _pad[22]; STARS_CV_0270 value; } lpszName;
    struct { unsigned char _pad[26]; STARS_CV_0270 value; } lpszClass;
    struct { unsigned char _pad[30]; STARS_CV_0022 value; } dwExStyle;
};
#define STARS_OFFSET_1457_lpCreateParams 0
#define STARS_OFFSET_1457_hInstance 4
#define STARS_OFFSET_1457_hMenu 6
#define STARS_OFFSET_1457_hwndParent 8
#define STARS_OFFSET_1457_cy 10
#define STARS_OFFSET_1457_cx 12
#define STARS_OFFSET_1457_y 14
#define STARS_OFFSET_1457_x 16
#define STARS_OFFSET_1457_style 18
#define STARS_OFFSET_1457_lpszName 22
#define STARS_OFFSET_1457_lpszClass 26
#define STARS_OFFSET_1457_dwExStyle 30
/* tagPD: 52 bytes; each view begins at its recorded offset. */
union STARS_CV_1459 {
    unsigned char raw[52];
    struct { STARS_CV_0022 value; } lStructSize;
    struct { unsigned char _pad[4]; STARS_CV_0073 value; } hwndOwner;
    struct { unsigned char _pad[6]; STARS_CV_0073 value; } hDevMode;
    struct { unsigned char _pad[8]; STARS_CV_0073 value; } hDevNames;
    struct { unsigned char _pad[10]; STARS_CV_0073 value; } hDC;
    struct { unsigned char _pad[12]; STARS_CV_0022 value; } Flags;
    struct { unsigned char _pad[16]; STARS_CV_0073 value; } nFromPage;
    struct { unsigned char _pad[18]; STARS_CV_0073 value; } nToPage;
    struct { unsigned char _pad[20]; STARS_CV_0073 value; } nMinPage;
    struct { unsigned char _pad[22]; STARS_CV_0073 value; } nMaxPage;
    struct { unsigned char _pad[24]; STARS_CV_0073 value; } nCopies;
    struct { unsigned char _pad[26]; STARS_CV_0073 value; } hInstance;
    struct { unsigned char _pad[28]; STARS_CV_0012 value; } lCustData;
    struct { unsigned char _pad[32]; STARS_CV_1352 value; } lpfnPrintHook;
    struct { unsigned char _pad[36]; STARS_CV_1352 value; } lpfnSetupHook;
    struct { unsigned char _pad[40]; STARS_CV_0270 value; } lpPrintTemplateName;
    struct { unsigned char _pad[44]; STARS_CV_0270 value; } lpSetupTemplateName;
    struct { unsigned char _pad[48]; STARS_CV_0073 value; } hPrintTemplate;
    struct { unsigned char _pad[50]; STARS_CV_0073 value; } hSetupTemplate;
};
#define STARS_OFFSET_1459_lStructSize 0
#define STARS_OFFSET_1459_hwndOwner 4
#define STARS_OFFSET_1459_hDevMode 6
#define STARS_OFFSET_1459_hDevNames 8
#define STARS_OFFSET_1459_hDC 10
#define STARS_OFFSET_1459_Flags 12
#define STARS_OFFSET_1459_nFromPage 16
#define STARS_OFFSET_1459_nToPage 18
#define STARS_OFFSET_1459_nMinPage 20
#define STARS_OFFSET_1459_nMaxPage 22
#define STARS_OFFSET_1459_nCopies 24
#define STARS_OFFSET_1459_hInstance 26
#define STARS_OFFSET_1459_lCustData 28
#define STARS_OFFSET_1459_lpfnPrintHook 32
#define STARS_OFFSET_1459_lpfnSetupHook 36
#define STARS_OFFSET_1459_lpPrintTemplateName 40
#define STARS_OFFSET_1459_lpSetupTemplateName 44
#define STARS_OFFSET_1459_hPrintTemplate 48
#define STARS_OFFSET_1459_hSetupTemplate 50
#ifdef STARS_NATIVE16
typedef STARS_CV_144b STARS_NEAR *STARS_CV_145f;
#else
typedef unsigned short STARS_CV_145f;
#endif
typedef STARS_CV_1446 STARS_CV_1460[64];
/* _aihist: 1284 bytes; each view begins at its recorded offset. */
union STARS_CV_1462 {
    unsigned char raw[1284];
    struct { STARS_CV_0021 value; } cbAiHist;
    struct { unsigned char _pad[2]; STARS_CV_0072 value; } cStarbase;
    struct { unsigned char _pad[4]; STARS_CV_1460 value; } rgasb;
};
#define STARS_OFFSET_1462_cbAiHist 0
#define STARS_OFFSET_1462_cStarbase 2
#define STARS_OFFSET_1462_rgasb 4
#ifdef STARS_NATIVE16
typedef STARS_CV_143c STARS_FAR *STARS_CV_1463;
#else
typedef STARS_ULONG32 STARS_CV_1463;
#endif
#ifdef STARS_NATIVE16
typedef STARS_CV_1197 STARS_FAR *STARS_CV_1466;
#else
typedef STARS_ULONG32 STARS_CV_1466;
#endif
/* tagCHOOSEFONT: 46 bytes; each view begins at its recorded offset. */
union STARS_CV_1468 {
    unsigned char raw[46];
    struct { STARS_CV_0022 value; } lStructSize;
    struct { unsigned char _pad[4]; STARS_CV_0073 value; } hwndOwner;
    struct { unsigned char _pad[6]; STARS_CV_0073 value; } hDC;
    struct { unsigned char _pad[8]; STARS_CV_1466 value; } lpLogFont;
    struct { unsigned char _pad[12]; STARS_CV_0072 value; } iPointSize;
    struct { unsigned char _pad[14]; STARS_CV_0022 value; } Flags;
    struct { unsigned char _pad[18]; STARS_CV_0022 value; } rgbColors;
    struct { unsigned char _pad[22]; STARS_CV_0012 value; } lCustData;
    struct { unsigned char _pad[26]; STARS_CV_1352 value; } lpfnHook;
    struct { unsigned char _pad[30]; STARS_CV_0270 value; } lpTemplateName;
    struct { unsigned char _pad[34]; STARS_CV_0073 value; } hInstance;
    struct { unsigned char _pad[36]; STARS_CV_0270 value; } lpszStyle;
    struct { unsigned char _pad[40]; STARS_CV_0073 value; } nFontType;
    struct { unsigned char _pad[42]; STARS_CV_0072 value; } nSizeMin;
    struct { unsigned char _pad[44]; STARS_CV_0072 value; } nSizeMax;
};
#define STARS_OFFSET_1468_lStructSize 0
#define STARS_OFFSET_1468_hwndOwner 4
#define STARS_OFFSET_1468_hDC 6
#define STARS_OFFSET_1468_lpLogFont 8
#define STARS_OFFSET_1468_iPointSize 12
#define STARS_OFFSET_1468_Flags 14
#define STARS_OFFSET_1468_rgbColors 18
#define STARS_OFFSET_1468_lCustData 22
#define STARS_OFFSET_1468_lpfnHook 26
#define STARS_OFFSET_1468_lpTemplateName 30
#define STARS_OFFSET_1468_hInstance 34
#define STARS_OFFSET_1468_lpszStyle 36
#define STARS_OFFSET_1468_nFontType 40
#define STARS_OFFSET_1468_nSizeMin 42
#define STARS_OFFSET_1468_nSizeMax 44
/* _rtwaypt: 22 bytes; each view begins at its recorded offset. */
union STARS_CV_146a {
    unsigned char raw[22];
    struct { STARS_CV_0011 value; } id;
    struct { unsigned char _pad[2]; STARS_CV_0011 value; } iWaypt;
    struct { unsigned char _pad[4]; STARS_CV_1005 value; } order;
};
#define STARS_OFFSET_146a_id 0
#define STARS_OFFSET_146a_iWaypt 2
#define STARS_OFFSET_146a_order 4
#ifdef STARS_NATIVE16
typedef STARS_CV_144b STARS_FAR *STARS_CV_146d;
#else
typedef STARS_ULONG32 STARS_CV_146d;
#endif
#ifdef STARS_NATIVE16
typedef STARS_CV_1454 STARS_FAR *STARS_CV_1475;
#else
typedef STARS_ULONG32 STARS_CV_1475;
#endif
/* _drawcir: 24 bytes; each view begins at its recorded offset. */
union STARS_CV_1477 {
    unsigned char raw[24];
    struct { STARS_CV_0172 value; } rgx;
    struct { unsigned char _pad[2]; STARS_CV_0172 value; } rgy;
    struct { unsigned char _pad[4]; STARS_CV_0172 value; } rgrad;
    struct { unsigned char _pad[6]; STARS_CV_0072 value; } cCur;
    struct { unsigned char _pad[8]; STARS_CV_0072 value; } cMax;
    struct { unsigned char _pad[10]; STARS_CV_0073 value; } hdc;
    struct { unsigned char _pad[12]; STARS_CV_101a value; } rcClip;
    struct { unsigned char _pad[20]; STARS_CV_0072 value; } fCovered;
    struct { unsigned char _pad[22]; STARS_CV_0072 value; } fHollowOut;
};
#define STARS_OFFSET_1477_rgx 0
#define STARS_OFFSET_1477_rgy 2
#define STARS_OFFSET_1477_rgrad 4
#define STARS_OFFSET_1477_cCur 6
#define STARS_OFFSET_1477_cMax 8
#define STARS_OFFSET_1477_hdc 10
#define STARS_OFFSET_1477_rcClip 12
#define STARS_OFFSET_1477_fCovered 20
#define STARS_OFFSET_1477_fHollowOut 22
/* tagMETARECORD: 8 bytes; each view begins at its recorded offset. */
union STARS_CV_1479 {
    unsigned char raw[8];
    struct { STARS_CV_0022 value; } rdSize;
    struct { unsigned char _pad[4]; STARS_CV_0073 value; } rdFunction;
    struct { unsigned char _pad[6]; STARS_CV_1449 value; } rdParm;
};
#define STARS_OFFSET_1479_rdSize 0
#define STARS_OFFSET_1479_rdFunction 4
#define STARS_OFFSET_1479_rdParm 6
/* tagMDICREATESTRUCT: 26 bytes; each view begins at its recorded offset. */
union STARS_CV_147b {
    unsigned char raw[26];
    struct { STARS_CV_0270 value; } szClass;
    struct { unsigned char _pad[4]; STARS_CV_0270 value; } szTitle;
    struct { unsigned char _pad[8]; STARS_CV_0073 value; } hOwner;
    struct { unsigned char _pad[10]; STARS_CV_0072 value; } x;
    struct { unsigned char _pad[12]; STARS_CV_0072 value; } y;
    struct { unsigned char _pad[14]; STARS_CV_0072 value; } cx;
    struct { unsigned char _pad[16]; STARS_CV_0072 value; } cy;
    struct { unsigned char _pad[18]; STARS_CV_0022 value; } style;
    struct { unsigned char _pad[22]; STARS_CV_0012 value; } lParam;
};
#define STARS_OFFSET_147b_szClass 0
#define STARS_OFFSET_147b_szTitle 4
#define STARS_OFFSET_147b_hOwner 8
#define STARS_OFFSET_147b_x 10
#define STARS_OFFSET_147b_y 12
#define STARS_OFFSET_147b_cx 14
#define STARS_OFFSET_147b_cy 16
#define STARS_OFFSET_147b_style 18
#define STARS_OFFSET_147b_lParam 22
#ifdef STARS_NATIVE16
typedef STARS_CV_1277 STARS_NEAR *STARS_CV_147e;
#else
typedef unsigned short STARS_CV_147e;
#endif
typedef STARS_CV_0073 STARS_CV_147f; /* bit-field backing storage; use recorded bit positions/masks */
typedef STARS_CV_0073 STARS_CV_1480; /* bit-field backing storage; use recorded bit positions/masks */
/* _timer: 10 bytes; each view begins at its recorded offset. */
union STARS_CV_1482 {
    unsigned char raw[10];
    struct { STARS_CV_0072 value; } mdForce;
    struct { unsigned char _pad[2]; STARS_CV_0072 value; } fAutoGenWhenIn;
    struct { unsigned char _pad[4]; STARS_CV_0072 value; } hrsForce;
    struct { unsigned char _pad[4]; STARS_CV_147f value; } minForce;
    struct { unsigned char _pad[4]; STARS_CV_1480 value; } cPlr;
    struct { unsigned char _pad[6]; STARS_CV_0012 value; } tickcount;
};
#define STARS_OFFSET_1482_mdForce 0
#define STARS_OFFSET_1482_fAutoGenWhenIn 2
#define STARS_OFFSET_1482_hrsForce 4
#define STARS_OFFSET_1482_minForce 4
#define STARS_BITSHIFT_1482_minForce 0
#define STARS_BITMASK_1482_minForce 0xfffUL
#define STARS_OFFSET_1482_cPlr 4
#define STARS_BITSHIFT_1482_cPlr 12
#define STARS_BITMASK_1482_cPlr 0xfUL
#define STARS_OFFSET_1482_tickcount 6
/* _complex: 16 bytes; each view begins at its recorded offset. */
union STARS_CV_1484 {
    unsigned char raw[16];
    struct { STARS_CV_0041 value; } x;
    struct { unsigned char _pad[8]; STARS_CV_0041 value; } y;
};
#define STARS_OFFSET_1484_x 0
#define STARS_OFFSET_1484_y 8
/* _rtchgname: 37 bytes; each view begins at its recorded offset. */
union STARS_CV_1486 {
    unsigned char raw[37];
    struct { STARS_CV_0011 value; } id;
    struct { unsigned char _pad[2]; STARS_CV_0011 value; } grobj;
    struct { unsigned char _pad[4]; STARS_CV_131b value; } rgb;
};
#define STARS_OFFSET_1486_id 0
#define STARS_OFFSET_1486_grobj 2
#define STARS_OFFSET_1486_rgb 4
#ifdef STARS_NATIVE16
typedef STARS_CV_11db STARS_NEAR *STARS_CV_1488;
#else
typedef unsigned short STARS_CV_1488;
#endif
/* complex: 16 bytes; each view begins at its recorded offset. */
union STARS_CV_1489 {
    unsigned char raw[16];
    struct { STARS_CV_0041 value; } x;
    struct { unsigned char _pad[8]; STARS_CV_0041 value; } y;
};
#define STARS_OFFSET_1489_x 0
#define STARS_OFFSET_1489_y 8
/* _lsb: 4 bytes; each view begins at its recorded offset. */
union STARS_CV_148b {
    unsigned char raw[4];
    struct { STARS_CV_1378 value; } isb;
    struct { STARS_CV_1392 value; } pctDp;
    struct { unsigned char _pad[2]; STARS_CV_1393 value; } idFling;
    struct { unsigned char _pad[2]; STARS_CV_1394 value; } iWarpFling;
    struct { unsigned char _pad[2]; STARS_CV_137d value; } unused3;
};
#define STARS_OFFSET_148b_isb 0
#define STARS_BITSHIFT_148b_isb 0
#define STARS_BITMASK_148b_isb 0xfUL
#define STARS_OFFSET_148b_pctDp 0
#define STARS_BITSHIFT_148b_pctDp 4
#define STARS_BITMASK_148b_pctDp 0xfffUL
#define STARS_OFFSET_148b_idFling 2
#define STARS_BITSHIFT_148b_idFling 0
#define STARS_BITMASK_148b_idFling 0x3ffUL
#define STARS_OFFSET_148b_iWarpFling 2
#define STARS_BITSHIFT_148b_iWarpFling 10
#define STARS_BITMASK_148b_iWarpFling 0xfUL
#define STARS_OFFSET_148b_unused3 2
#define STARS_BITSHIFT_148b_unused3 14
#define STARS_BITMASK_148b_unused3 0x3UL
/* tagPOINTFX: 8 bytes; each view begins at its recorded offset. */
union STARS_CV_148d {
    unsigned char raw[8];
    struct { STARS_CV_140e value; } x;
    struct { unsigned char _pad[4]; STARS_CV_140e value; } y;
};
#define STARS_OFFSET_148d_x 0
#define STARS_OFFSET_148d_y 4
/* tagPANOSE: 10 bytes; each view begins at its recorded offset. */
union STARS_CV_148f {
    unsigned char raw[10];
    struct { STARS_CV_0020 value; } bFamilyType;
    struct { unsigned char _pad[1]; STARS_CV_0020 value; } bSerifStyle;
    struct { unsigned char _pad[2]; STARS_CV_0020 value; } bWeight;
    struct { unsigned char _pad[3]; STARS_CV_0020 value; } bProportion;
    struct { unsigned char _pad[4]; STARS_CV_0020 value; } bContrast;
    struct { unsigned char _pad[5]; STARS_CV_0020 value; } bStrokeVariation;
    struct { unsigned char _pad[6]; STARS_CV_0020 value; } bArmStyle;
    struct { unsigned char _pad[7]; STARS_CV_0020 value; } bLetterform;
    struct { unsigned char _pad[8]; STARS_CV_0020 value; } bMidline;
    struct { unsigned char _pad[9]; STARS_CV_0020 value; } bXHeight;
};
#define STARS_OFFSET_148f_bFamilyType 0
#define STARS_OFFSET_148f_bSerifStyle 1
#define STARS_OFFSET_148f_bWeight 2
#define STARS_OFFSET_148f_bProportion 3
#define STARS_OFFSET_148f_bContrast 4
#define STARS_OFFSET_148f_bStrokeVariation 5
#define STARS_OFFSET_148f_bArmStyle 6
#define STARS_OFFSET_148f_bLetterform 7
#define STARS_OFFSET_148f_bMidline 8
#define STARS_OFFSET_148f_bXHeight 9
/* tagOUTLINETEXTMETRIC: 114 bytes; each view begins at its recorded offset. */
union STARS_CV_1491 {
    unsigned char raw[114];
    struct { STARS_CV_0073 value; } otmSize;
    struct { unsigned char _pad[2]; STARS_CV_11db value; } otmTextMetrics;
    struct { unsigned char _pad[33]; STARS_CV_0020 value; } otmFiller;
    struct { unsigned char _pad[34]; STARS_CV_148f value; } otmPanoseNumber;
    struct { unsigned char _pad[44]; STARS_CV_0073 value; } otmfsSelection;
    struct { unsigned char _pad[46]; STARS_CV_0073 value; } otmfsType;
    struct { unsigned char _pad[48]; STARS_CV_0072 value; } otmsCharSlopeRise;
    struct { unsigned char _pad[50]; STARS_CV_0072 value; } otmsCharSlopeRun;
    struct { unsigned char _pad[52]; STARS_CV_0072 value; } otmItalicAngle;
    struct { unsigned char _pad[54]; STARS_CV_0073 value; } otmEMSquare;
    struct { unsigned char _pad[56]; STARS_CV_0072 value; } otmAscent;
    struct { unsigned char _pad[58]; STARS_CV_0072 value; } otmDescent;
    struct { unsigned char _pad[60]; STARS_CV_0073 value; } otmLineGap;
    struct { unsigned char _pad[62]; STARS_CV_0073 value; } otmsCapEmHeight;
    struct { unsigned char _pad[64]; STARS_CV_0073 value; } otmsXHeight;
    struct { unsigned char _pad[66]; STARS_CV_101a value; } otmrcFontBox;
    struct { unsigned char _pad[74]; STARS_CV_0072 value; } otmMacAscent;
    struct { unsigned char _pad[76]; STARS_CV_0072 value; } otmMacDescent;
    struct { unsigned char _pad[78]; STARS_CV_0073 value; } otmMacLineGap;
    struct { unsigned char _pad[80]; STARS_CV_0073 value; } otmusMinimumPPEM;
    struct { unsigned char _pad[82]; STARS_CV_1018 value; } otmptSubscriptSize;
    struct { unsigned char _pad[86]; STARS_CV_1018 value; } otmptSubscriptOffset;
    struct { unsigned char _pad[90]; STARS_CV_1018 value; } otmptSuperscriptSize;
    struct { unsigned char _pad[94]; STARS_CV_1018 value; } otmptSuperscriptOffset;
    struct { unsigned char _pad[98]; STARS_CV_0073 value; } otmsStrikeoutSize;
    struct { unsigned char _pad[100]; STARS_CV_0072 value; } otmsStrikeoutPosition;
    struct { unsigned char _pad[102]; STARS_CV_0072 value; } otmsUnderscorePosition;
    struct { unsigned char _pad[104]; STARS_CV_0072 value; } otmsUnderscoreSize;
    struct { unsigned char _pad[106]; STARS_CV_0170 value; } otmpFamilyName;
    struct { unsigned char _pad[108]; STARS_CV_0170 value; } otmpFaceName;
    struct { unsigned char _pad[110]; STARS_CV_0170 value; } otmpStyleName;
    struct { unsigned char _pad[112]; STARS_CV_0170 value; } otmpFullName;
};
#define STARS_OFFSET_1491_otmSize 0
#define STARS_OFFSET_1491_otmTextMetrics 2
#define STARS_OFFSET_1491_otmFiller 33
#define STARS_OFFSET_1491_otmPanoseNumber 34
#define STARS_OFFSET_1491_otmfsSelection 44
#define STARS_OFFSET_1491_otmfsType 46
#define STARS_OFFSET_1491_otmsCharSlopeRise 48
#define STARS_OFFSET_1491_otmsCharSlopeRun 50
#define STARS_OFFSET_1491_otmItalicAngle 52
#define STARS_OFFSET_1491_otmEMSquare 54
#define STARS_OFFSET_1491_otmAscent 56
#define STARS_OFFSET_1491_otmDescent 58
#define STARS_OFFSET_1491_otmLineGap 60
#define STARS_OFFSET_1491_otmsCapEmHeight 62
#define STARS_OFFSET_1491_otmsXHeight 64
#define STARS_OFFSET_1491_otmrcFontBox 66
#define STARS_OFFSET_1491_otmMacAscent 74
#define STARS_OFFSET_1491_otmMacDescent 76
#define STARS_OFFSET_1491_otmMacLineGap 78
#define STARS_OFFSET_1491_otmusMinimumPPEM 80
#define STARS_OFFSET_1491_otmptSubscriptSize 82
#define STARS_OFFSET_1491_otmptSubscriptOffset 86
#define STARS_OFFSET_1491_otmptSuperscriptSize 90
#define STARS_OFFSET_1491_otmptSuperscriptOffset 94
#define STARS_OFFSET_1491_otmsStrikeoutSize 98
#define STARS_OFFSET_1491_otmsStrikeoutPosition 100
#define STARS_OFFSET_1491_otmsUnderscorePosition 102
#define STARS_OFFSET_1491_otmsUnderscoreSize 104
#define STARS_OFFSET_1491_otmpFamilyName 106
#define STARS_OFFSET_1491_otmpFaceName 108
#define STARS_OFFSET_1491_otmpStyleName 110
#define STARS_OFFSET_1491_otmpFullName 112
#ifdef STARS_NATIVE16
typedef STARS_CV_1457 STARS_FAR *STARS_CV_1498;
#else
typedef STARS_ULONG32 STARS_CV_1498;
#endif
/* tagCBT_CREATEWND: 6 bytes; each view begins at its recorded offset. */
union STARS_CV_149a {
    unsigned char raw[6];
    struct { STARS_CV_1498 value; } lpcs;
    struct { unsigned char _pad[4]; STARS_CV_0073 value; } hwndInsertAfter;
};
#define STARS_OFFSET_149a_lpcs 0
#define STARS_OFFSET_149a_hwndInsertAfter 4
#ifdef STARS_NATIVE16
typedef STARS_CV_11db STARS_FAR *STARS_CV_149b;
#else
typedef STARS_ULONG32 STARS_CV_149b;
#endif
#ifdef STARS_NATIVE16
typedef STARS_CV_148d STARS_FAR *STARS_CV_149d;
#else
typedef STARS_ULONG32 STARS_CV_149d;
#endif
/* tagMOUSEHOOKSTRUCT: 12 bytes; each view begins at its recorded offset. */
union STARS_CV_149f {
    unsigned char raw[12];
    struct { STARS_CV_1018 value; } pt;
    struct { unsigned char _pad[4]; STARS_CV_0073 value; } hwnd;
    struct { unsigned char _pad[6]; STARS_CV_0073 value; } wHitTestCode;
    struct { unsigned char _pad[8]; STARS_CV_0022 value; } dwExtraInfo;
};
#define STARS_OFFSET_149f_pt 0
#define STARS_OFFSET_149f_hwnd 4
#define STARS_OFFSET_149f_wHitTestCode 6
#define STARS_OFFSET_149f_dwExtraInfo 8
#ifdef STARS_NATIVE16
typedef STARS_CV_1479 STARS_NEAR *STARS_CV_14a0;
#else
typedef unsigned short STARS_CV_14a0;
#endif
#ifdef STARS_NATIVE16
typedef STARS_CV_1491 STARS_FAR *STARS_CV_14a1;
#else
typedef STARS_ULONG32 STARS_CV_14a1;
#endif
#ifdef STARS_NATIVE16
typedef STARS_CV_147b STARS_FAR *STARS_CV_14a3;
#else
typedef STARS_ULONG32 STARS_CV_14a3;
#endif
typedef STARS_CV_148d STARS_CV_14a4[1];
/* tagTTPOLYCURVE: 12 bytes; each view begins at its recorded offset. */
union STARS_CV_14a6 {
    unsigned char raw[12];
    struct { STARS_CV_0073 value; } wType;
    struct { unsigned char _pad[2]; STARS_CV_0073 value; } cpfx;
    struct { unsigned char _pad[4]; STARS_CV_14a4 value; } apfx;
};
#define STARS_OFFSET_14a6_wType 0
#define STARS_OFFSET_14a6_cpfx 2
#define STARS_OFFSET_14a6_apfx 4
#ifdef STARS_NATIVE16
typedef STARS_CV_1479 STARS_FAR *STARS_CV_14a8;
#else
typedef STARS_ULONG32 STARS_CV_14a8;
#endif
#ifdef STARS_NATIVE16
typedef STARS_CV_11b9 STARS_NEAR *STARS_CV_14a9;
#else
typedef unsigned short STARS_CV_14a9;
#endif
#ifdef STARS_NATIVE16
typedef STARS_CV_1468 STARS_FAR *STARS_CV_14ac;
#else
typedef STARS_ULONG32 STARS_CV_14ac;
#endif
#ifdef STARS_NATIVE16
typedef STARS_CV_149a STARS_FAR *STARS_CV_14ad;
#else
typedef STARS_ULONG32 STARS_CV_14ad;
#endif
/* tagNEWTEXTMETRIC: 41 bytes; each view begins at its recorded offset. */
union STARS_CV_14b2 {
    unsigned char raw[41];
    struct { STARS_CV_0072 value; } tmHeight;
    struct { unsigned char _pad[2]; STARS_CV_0072 value; } tmAscent;
    struct { unsigned char _pad[4]; STARS_CV_0072 value; } tmDescent;
    struct { unsigned char _pad[6]; STARS_CV_0072 value; } tmInternalLeading;
    struct { unsigned char _pad[8]; STARS_CV_0072 value; } tmExternalLeading;
    struct { unsigned char _pad[10]; STARS_CV_0072 value; } tmAveCharWidth;
    struct { unsigned char _pad[12]; STARS_CV_0072 value; } tmMaxCharWidth;
    struct { unsigned char _pad[14]; STARS_CV_0072 value; } tmWeight;
    struct { unsigned char _pad[16]; STARS_CV_0020 value; } tmItalic;
    struct { unsigned char _pad[17]; STARS_CV_0020 value; } tmUnderlined;
    struct { unsigned char _pad[18]; STARS_CV_0020 value; } tmStruckOut;
    struct { unsigned char _pad[19]; STARS_CV_0020 value; } tmFirstChar;
    struct { unsigned char _pad[20]; STARS_CV_0020 value; } tmLastChar;
    struct { unsigned char _pad[21]; STARS_CV_0020 value; } tmDefaultChar;
    struct { unsigned char _pad[22]; STARS_CV_0020 value; } tmBreakChar;
    struct { unsigned char _pad[23]; STARS_CV_0020 value; } tmPitchAndFamily;
    struct { unsigned char _pad[24]; STARS_CV_0020 value; } tmCharSet;
    struct { unsigned char _pad[25]; STARS_CV_0072 value; } tmOverhang;
    struct { unsigned char _pad[27]; STARS_CV_0072 value; } tmDigitizedAspectX;
    struct { unsigned char _pad[29]; STARS_CV_0072 value; } tmDigitizedAspectY;
    struct { unsigned char _pad[31]; STARS_CV_0022 value; } ntmFlags;
    struct { unsigned char _pad[35]; STARS_CV_0073 value; } ntmSizeEM;
    struct { unsigned char _pad[37]; STARS_CV_0073 value; } ntmCellHeight;
    struct { unsigned char _pad[39]; STARS_CV_0073 value; } ntmAvgWidth;
};
#define STARS_OFFSET_14b2_tmHeight 0
#define STARS_OFFSET_14b2_tmAscent 2
#define STARS_OFFSET_14b2_tmDescent 4
#define STARS_OFFSET_14b2_tmInternalLeading 6
#define STARS_OFFSET_14b2_tmExternalLeading 8
#define STARS_OFFSET_14b2_tmAveCharWidth 10
#define STARS_OFFSET_14b2_tmMaxCharWidth 12
#define STARS_OFFSET_14b2_tmWeight 14
#define STARS_OFFSET_14b2_tmItalic 16
#define STARS_OFFSET_14b2_tmUnderlined 17
#define STARS_OFFSET_14b2_tmStruckOut 18
#define STARS_OFFSET_14b2_tmFirstChar 19
#define STARS_OFFSET_14b2_tmLastChar 20
#define STARS_OFFSET_14b2_tmDefaultChar 21
#define STARS_OFFSET_14b2_tmBreakChar 22
#define STARS_OFFSET_14b2_tmPitchAndFamily 23
#define STARS_OFFSET_14b2_tmCharSet 24
#define STARS_OFFSET_14b2_tmOverhang 25
#define STARS_OFFSET_14b2_tmDigitizedAspectX 27
#define STARS_OFFSET_14b2_tmDigitizedAspectY 29
#define STARS_OFFSET_14b2_ntmFlags 31
#define STARS_OFFSET_14b2_ntmSizeEM 35
#define STARS_OFFSET_14b2_ntmCellHeight 37
#define STARS_OFFSET_14b2_ntmAvgWidth 39
/* tagWINDOWPLACEMENT: 22 bytes; each view begins at its recorded offset. */
union STARS_CV_14b5 {
    unsigned char raw[22];
    struct { STARS_CV_0073 value; } length;
    struct { unsigned char _pad[2]; STARS_CV_0073 value; } flags;
    struct { unsigned char _pad[4]; STARS_CV_0073 value; } showCmd;
    struct { unsigned char _pad[6]; STARS_CV_1018 value; } ptMinPosition;
    struct { unsigned char _pad[10]; STARS_CV_1018 value; } ptMaxPosition;
    struct { unsigned char _pad[14]; STARS_CV_101a value; } rcNormalPosition;
};
#define STARS_OFFSET_14b5_length 0
#define STARS_OFFSET_14b5_flags 2
#define STARS_OFFSET_14b5_showCmd 4
#define STARS_OFFSET_14b5_ptMinPosition 6
#define STARS_OFFSET_14b5_ptMaxPosition 10
#define STARS_OFFSET_14b5_rcNormalPosition 14
/* tagCBTACTIVATESTRUCT: 4 bytes; each view begins at its recorded offset. */
union STARS_CV_14b7 {
    unsigned char raw[4];
    struct { STARS_CV_0072 value; } fMouse;
    struct { unsigned char _pad[2]; STARS_CV_0073 value; } hWndActive;
};
#define STARS_OFFSET_14b7_fMouse 0
#define STARS_OFFSET_14b7_hWndActive 2
#ifdef STARS_NATIVE16
typedef STARS_CV_149f STARS_FAR *STARS_CV_14c0;
#else
typedef STARS_ULONG32 STARS_CV_14c0;
#endif
#ifdef STARS_NATIVE16
typedef STARS_CV_14a6 STARS_FAR *STARS_CV_14c2;
#else
typedef STARS_ULONG32 STARS_CV_14c2;
#endif
#ifdef STARS_NATIVE16
typedef STARS_CV_1279 STARS_NEAR *STARS_CV_14c6;
#else
typedef unsigned short STARS_CV_14c6;
#endif
/* tagTTPOLYGONHEADER: 16 bytes; each view begins at its recorded offset. */
union STARS_CV_14c8 {
    unsigned char raw[16];
    struct { STARS_CV_0022 value; } cb;
    struct { unsigned char _pad[4]; STARS_CV_0022 value; } dwType;
    struct { unsigned char _pad[8]; STARS_CV_148d value; } pfxStart;
};
#define STARS_OFFSET_14c8_cb 0
#define STARS_OFFSET_14c8_dwType 4
#define STARS_OFFSET_14c8_pfxStart 8
typedef STARS_CV_1078 STARS_CV_14cd[];
/* _rtshdef: 17 bytes; each view begins at its recorded offset. */
union STARS_CV_14cf {
    unsigned char raw[17];
    struct { STARS_CV_135d value; } det;
    struct { STARS_CV_135e value; } fInclude;
    struct { STARS_CV_135f value; } fFree;
    struct { STARS_CV_146b value; } ishdef;
    struct { STARS_CV_1396 value; } fGift;
    struct { STARS_CV_0021 value; } wFlags;
    struct { unsigned char _pad[2]; STARS_CV_0020 value; } ihuldef;
    struct { unsigned char _pad[3]; STARS_CV_0020 value; } ibmp;
    struct { unsigned char _pad[4]; STARS_CV_0021 value; } wtEmpty;
    struct { unsigned char _pad[4]; STARS_CV_0021 value; } dp;
    struct { unsigned char _pad[6]; STARS_CV_0020 value; } chs;
    struct { unsigned char _pad[7]; STARS_CV_0021 value; } turn;
    struct { unsigned char _pad[9]; STARS_CV_0022 value; } cBuilt;
    struct { unsigned char _pad[13]; STARS_CV_0022 value; } cExist;
    /* rghs: variable tail at offset 17; no fixed storage. */
};
#define STARS_OFFSET_14cf_det 0
#define STARS_BITSHIFT_14cf_det 0
#define STARS_BITMASK_14cf_det 0xffUL
#define STARS_OFFSET_14cf_fInclude 0
#define STARS_BITSHIFT_14cf_fInclude 8
#define STARS_BITMASK_14cf_fInclude 0x1UL
#define STARS_OFFSET_14cf_fFree 0
#define STARS_BITSHIFT_14cf_fFree 9
#define STARS_BITMASK_14cf_fFree 0x1UL
#define STARS_OFFSET_14cf_ishdef 0
#define STARS_BITSHIFT_14cf_ishdef 10
#define STARS_BITMASK_14cf_ishdef 0x1fUL
#define STARS_OFFSET_14cf_fGift 0
#define STARS_BITSHIFT_14cf_fGift 15
#define STARS_BITMASK_14cf_fGift 0x1UL
#define STARS_OFFSET_14cf_wFlags 0
#define STARS_OFFSET_14cf_ihuldef 2
#define STARS_OFFSET_14cf_ibmp 3
#define STARS_OFFSET_14cf_wtEmpty 4
#define STARS_OFFSET_14cf_dp 4
#define STARS_OFFSET_14cf_chs 6
#define STARS_OFFSET_14cf_turn 7
#define STARS_OFFSET_14cf_cBuilt 9
#define STARS_OFFSET_14cf_cExist 13
#define STARS_OFFSET_14cf_rghs 17
#ifdef STARS_NATIVE16
typedef STARS_CV_101a STARS_FAR *STARS_CV_14d3;
#else
typedef STARS_ULONG32 STARS_CV_14d3;
#endif
#ifdef STARS_NATIVE16
typedef STARS_CV_1459 STARS_FAR *STARS_CV_14d5;
#else
typedef STARS_ULONG32 STARS_CV_14d5;
#endif
/* _rtchgshdef: 19 bytes; each view begins at its recorded offset. */
union STARS_CV_14d9 {
    unsigned char raw[19];
    struct { STARS_CV_1378 value; } mdChg;
    struct { STARS_CV_1379 value; } iPlr;
    struct { STARS_CV_1493 value; } ishdef;
    struct { STARS_CV_13d0 value; } junk;
    struct { unsigned char _pad[2]; STARS_CV_14cf value; } rtshdef;
};
#define STARS_OFFSET_14d9_mdChg 0
#define STARS_BITSHIFT_14d9_mdChg 0
#define STARS_BITMASK_14d9_mdChg 0xfUL
#define STARS_OFFSET_14d9_iPlr 0
#define STARS_BITSHIFT_14d9_iPlr 4
#define STARS_BITMASK_14d9_iPlr 0xfUL
#define STARS_OFFSET_14d9_ishdef 0
#define STARS_BITSHIFT_14d9_ishdef 8
#define STARS_BITMASK_14d9_ishdef 0x1fUL
#define STARS_OFFSET_14d9_junk 0
#define STARS_BITSHIFT_14d9_junk 13
#define STARS_BITMASK_14d9_junk 0x7UL
#define STARS_OFFSET_14d9_rtshdef 2
/* tagDELETEITEMSTRUCT: 12 bytes; each view begins at its recorded offset. */
union STARS_CV_14db {
    unsigned char raw[12];
    struct { STARS_CV_0073 value; } CtlType;
    struct { unsigned char _pad[2]; STARS_CV_0073 value; } CtlID;
    struct { unsigned char _pad[4]; STARS_CV_0073 value; } itemID;
    struct { unsigned char _pad[6]; STARS_CV_0073 value; } hwndItem;
    struct { unsigned char _pad[8]; STARS_CV_0022 value; } itemData;
};
#define STARS_OFFSET_14db_CtlType 0
#define STARS_OFFSET_14db_CtlID 2
#define STARS_OFFSET_14db_itemID 4
#define STARS_OFFSET_14db_hwndItem 6
#define STARS_OFFSET_14db_itemData 8
/* tagHARDWAREHOOKSTRUCT: 10 bytes; each view begins at its recorded offset. */
union STARS_CV_14dd {
    unsigned char raw[10];
    struct { STARS_CV_0073 value; } hWnd;
    struct { unsigned char _pad[2]; STARS_CV_0073 value; } wMessage;
    struct { unsigned char _pad[4]; STARS_CV_0073 value; } wParam;
    struct { unsigned char _pad[6]; STARS_CV_0012 value; } lParam;
};
#define STARS_OFFSET_14dd_hWnd 0
#define STARS_OFFSET_14dd_wMessage 2
#define STARS_OFFSET_14dd_wParam 4
#define STARS_OFFSET_14dd_lParam 6
typedef STARS_CV_0011 STARS_CV_14de; /* bit-field backing storage; use recorded bit positions/masks */
typedef STARS_CV_0011 STARS_CV_14df; /* bit-field backing storage; use recorded bit positions/masks */
/* _rtplanet: 4 bytes; each view begins at its recorded offset. */
union STARS_CV_14e2 {
    unsigned char raw[4];
    struct { STARS_CV_14de value; } id;
    struct { STARS_CV_14df value; } iPlayer;
    struct { unsigned char _pad[2]; STARS_CV_14e0 value; } det;
    struct { unsigned char _pad[2]; STARS_CV_13b5 value; } fHomeworld;
    struct { unsigned char _pad[2]; STARS_CV_135e value; } fInclude;
    struct { unsigned char _pad[2]; STARS_CV_135f value; } fStarbase;
    struct { unsigned char _pad[2]; STARS_CV_1360 value; } fIncEVO;
    struct { unsigned char _pad[2]; STARS_CV_1361 value; } fIncImp;
    struct { unsigned char _pad[2]; STARS_CV_137b value; } fIsArtifact;
    struct { unsigned char _pad[2]; STARS_CV_137c value; } fIncSurfMin;
    struct { unsigned char _pad[2]; STARS_CV_1395 value; } fRouting;
    struct { unsigned char _pad[2]; STARS_CV_1396 value; } fFirstYear;
};
#define STARS_OFFSET_14e2_id 0
#define STARS_BITSHIFT_14e2_id 0
#define STARS_BITMASK_14e2_id 0x7ffUL
#define STARS_OFFSET_14e2_iPlayer 0
#define STARS_BITSHIFT_14e2_iPlayer 11
#define STARS_BITMASK_14e2_iPlayer 0x1fUL
#define STARS_OFFSET_14e2_det 2
#define STARS_BITSHIFT_14e2_det 0
#define STARS_BITMASK_14e2_det 0x7fUL
#define STARS_OFFSET_14e2_fHomeworld 2
#define STARS_BITSHIFT_14e2_fHomeworld 7
#define STARS_BITMASK_14e2_fHomeworld 0x1UL
#define STARS_OFFSET_14e2_fInclude 2
#define STARS_BITSHIFT_14e2_fInclude 8
#define STARS_BITMASK_14e2_fInclude 0x1UL
#define STARS_OFFSET_14e2_fStarbase 2
#define STARS_BITSHIFT_14e2_fStarbase 9
#define STARS_BITMASK_14e2_fStarbase 0x1UL
#define STARS_OFFSET_14e2_fIncEVO 2
#define STARS_BITSHIFT_14e2_fIncEVO 10
#define STARS_BITMASK_14e2_fIncEVO 0x1UL
#define STARS_OFFSET_14e2_fIncImp 2
#define STARS_BITSHIFT_14e2_fIncImp 11
#define STARS_BITMASK_14e2_fIncImp 0x1UL
#define STARS_OFFSET_14e2_fIsArtifact 2
#define STARS_BITSHIFT_14e2_fIsArtifact 12
#define STARS_BITMASK_14e2_fIsArtifact 0x1UL
#define STARS_OFFSET_14e2_fIncSurfMin 2
#define STARS_BITSHIFT_14e2_fIncSurfMin 13
#define STARS_BITMASK_14e2_fIncSurfMin 0x1UL
#define STARS_OFFSET_14e2_fRouting 2
#define STARS_BITSHIFT_14e2_fRouting 14
#define STARS_BITMASK_14e2_fRouting 0x1UL
#define STARS_OFFSET_14e2_fFirstYear 2
#define STARS_BITSHIFT_14e2_fFirstYear 15
#define STARS_BITMASK_14e2_fFirstYear 0x1UL
#ifdef STARS_NATIVE16
typedef STARS_CV_14b2 STARS_NEAR *STARS_CV_14e3;
#else
typedef unsigned short STARS_CV_14e3;
#endif
/* tagDRIVERINFOSTRUCT: 134 bytes; each view begins at its recorded offset. */
union STARS_CV_14e5 {
    unsigned char raw[134];
    struct { STARS_CV_0073 value; } length;
    struct { unsigned char _pad[2]; STARS_CV_0073 value; } hDriver;
    struct { unsigned char _pad[4]; STARS_CV_0073 value; } hModule;
    struct { unsigned char _pad[6]; STARS_CV_12ce value; } szAliasName;
};
#define STARS_OFFSET_14e5_length 0
#define STARS_OFFSET_14e5_hDriver 2
#define STARS_OFFSET_14e5_hModule 4
#define STARS_OFFSET_14e5_szAliasName 6
#ifdef STARS_NATIVE16
typedef STARS_CV_1332 STARS_NEAR *STARS_CV_14e6;
#else
typedef unsigned short STARS_CV_14e6;
#endif
#ifdef STARS_NATIVE16
typedef STARS_CV_14c8 STARS_FAR *STARS_CV_14e7;
#else
typedef STARS_ULONG32 STARS_CV_14e7;
#endif
/* tagCHOOSECOLOR: 32 bytes; each view begins at its recorded offset. */
union STARS_CV_14ea {
    unsigned char raw[32];
    struct { STARS_CV_0022 value; } lStructSize;
    struct { unsigned char _pad[4]; STARS_CV_0073 value; } hwndOwner;
    struct { unsigned char _pad[6]; STARS_CV_0073 value; } hInstance;
    struct { unsigned char _pad[8]; STARS_CV_0022 value; } rgbResult;
    struct { unsigned char _pad[12]; STARS_CV_0222 value; } lpCustColors;
    struct { unsigned char _pad[16]; STARS_CV_0022 value; } Flags;
    struct { unsigned char _pad[20]; STARS_CV_0012 value; } lCustData;
    struct { unsigned char _pad[24]; STARS_CV_1352 value; } lpfnHook;
    struct { unsigned char _pad[28]; STARS_CV_0270 value; } lpTemplateName;
};
#define STARS_OFFSET_14ea_lStructSize 0
#define STARS_OFFSET_14ea_hwndOwner 4
#define STARS_OFFSET_14ea_hInstance 6
#define STARS_OFFSET_14ea_rgbResult 8
#define STARS_OFFSET_14ea_lpCustColors 12
#define STARS_OFFSET_14ea_Flags 16
#define STARS_OFFSET_14ea_lCustData 20
#define STARS_OFFSET_14ea_lpfnHook 24
#define STARS_OFFSET_14ea_lpTemplateName 28
#ifdef STARS_NATIVE16
typedef STARS_CV_14ca STARS_FAR *STARS_CV_14ed;
#else
typedef STARS_ULONG32 STARS_CV_14ed;
#endif
#ifdef STARS_NATIVE16
typedef STARS_CV_14b5 STARS_NEAR *STARS_CV_14ee;
#else
typedef unsigned short STARS_CV_14ee;
#endif
/* tagABC: 6 bytes; each view begins at its recorded offset. */
union STARS_CV_14f0 {
    unsigned char raw[6];
    struct { STARS_CV_0072 value; } abcA;
    struct { unsigned char _pad[2]; STARS_CV_0073 value; } abcB;
    struct { unsigned char _pad[4]; STARS_CV_0072 value; } abcC;
};
#define STARS_OFFSET_14f0_abcA 0
#define STARS_OFFSET_14f0_abcB 2
#define STARS_OFFSET_14f0_abcC 4
#ifdef STARS_NATIVE16
typedef STARS_CV_14db STARS_NEAR *STARS_CV_14f1;
#else
typedef unsigned short STARS_CV_14f1;
#endif
#ifdef STARS_NATIVE16
typedef STARS_CV_14b2 STARS_FAR *STARS_CV_14f2;
#else
typedef STARS_ULONG32 STARS_CV_14f2;
#endif
#ifdef STARS_NATIVE16
typedef STARS_CV_1018 STARS_FAR *STARS_CV_14f3;
#else
typedef STARS_ULONG32 STARS_CV_14f3;
#endif
/* tagLOGBRUSH: 8 bytes; each view begins at its recorded offset. */
union STARS_CV_14f5 {
    unsigned char raw[8];
    struct { STARS_CV_0073 value; } lbStyle;
    struct { unsigned char _pad[2]; STARS_CV_0022 value; } lbColor;
    struct { unsigned char _pad[6]; STARS_CV_0072 value; } lbHatch;
};
#define STARS_OFFSET_14f5_lbStyle 0
#define STARS_OFFSET_14f5_lbColor 2
#define STARS_OFFSET_14f5_lbHatch 6
#ifdef STARS_NATIVE16
typedef STARS_CV_14b5 STARS_FAR *STARS_CV_14f6;
#else
typedef STARS_ULONG32 STARS_CV_14f6;
#endif
/* tagSIZE: 4 bytes; each view begins at its recorded offset. */
union STARS_CV_14f8 {
    unsigned char raw[4];
    struct { STARS_CV_0072 value; } cx;
    struct { unsigned char _pad[2]; STARS_CV_0072 value; } cy;
};
#define STARS_OFFSET_14f8_cx 0
#define STARS_OFFSET_14f8_cy 2
#ifdef STARS_NATIVE16
typedef STARS_CV_14db STARS_FAR *STARS_CV_14f9;
#else
typedef STARS_ULONG32 STARS_CV_14f9;
#endif
#ifdef STARS_NATIVE16
typedef STARS_CV_117d STARS_NEAR *STARS_CV_14fa;
#else
typedef unsigned short STARS_CV_14fa;
#endif
/* _fleetid: 2 bytes; each view begins at its recorded offset. */
union STARS_CV_14fc {
    unsigned char raw[2];
    struct { STARS_CV_13d4 value; } ifl;
    struct { STARS_CV_13cf value; } iplr;
    struct { STARS_CV_13d0 value; } junk;
};
#define STARS_OFFSET_14fc_ifl 0
#define STARS_BITSHIFT_14fc_ifl 0
#define STARS_BITMASK_14fc_ifl 0x1ffUL
#define STARS_OFFSET_14fc_iplr 0
#define STARS_BITSHIFT_14fc_iplr 9
#define STARS_BITMASK_14fc_iplr 0xfUL
#define STARS_OFFSET_14fc_junk 0
#define STARS_BITSHIFT_14fc_junk 13
#define STARS_BITMASK_14fc_junk 0x7UL
typedef STARS_CV_0020 STARS_CV_14fd[1];
/* tagMULTIKEYHELP: 4 bytes; each view begins at its recorded offset. */
union STARS_CV_14ff {
    unsigned char raw[4];
    struct { STARS_CV_0073 value; } mkSize;
    struct { unsigned char _pad[2]; STARS_CV_0020 value; } mkKeylist;
    struct { unsigned char _pad[3]; STARS_CV_14fd value; } szKeyphrase;
};
#define STARS_OFFSET_14ff_mkSize 0
#define STARS_OFFSET_14ff_mkKeylist 2
#define STARS_OFFSET_14ff_szKeyphrase 3
/* _vers: 2 bytes; each view begins at its recorded offset. */
union STARS_CV_1502 {
    unsigned char raw[2];
    struct { STARS_CV_14c3 value; } verInc;
    struct { STARS_CV_1500 value; } verMinor;
    struct { STARS_CV_1362 value; } verMajor;
};
#define STARS_OFFSET_1502_verInc 0
#define STARS_BITSHIFT_1502_verInc 0
#define STARS_BITMASK_1502_verInc 0x1fUL
#define STARS_OFFSET_1502_verMinor 0
#define STARS_BITSHIFT_1502_verMinor 5
#define STARS_BITMASK_1502_verMinor 0x7fUL
#define STARS_OFFSET_1502_verMajor 0
#define STARS_BITSHIFT_1502_verMajor 12
#define STARS_BITMASK_1502_verMajor 0xfUL
#ifdef STARS_NATIVE16
typedef STARS_CV_14f0 STARS_FAR *STARS_CV_1504;
#else
typedef STARS_ULONG32 STARS_CV_1504;
#endif
#ifdef STARS_NATIVE16
typedef STARS_CV_14e5 STARS_FAR *STARS_CV_1505;
#else
typedef STARS_ULONG32 STARS_CV_1505;
#endif
#ifdef STARS_NATIVE16
typedef STARS_CV_14ea STARS_FAR *STARS_CV_1506;
#else
typedef STARS_ULONG32 STARS_CV_1506;
#endif
#ifdef STARS_NATIVE16
typedef STARS_CV_14f8 STARS_NEAR *STARS_CV_1507;
#else
typedef unsigned short STARS_CV_1507;
#endif
typedef STARS_CV_101a STARS_CV_1508[3];
/* tagNCCALCSIZE_PARAMS: 28 bytes; each view begins at its recorded offset. */
union STARS_CV_150a {
    unsigned char raw[28];
    struct { STARS_CV_1508 value; } rgrc;
    struct { unsigned char _pad[24]; STARS_CV_1475 value; } lppos;
};
#define STARS_OFFSET_150a_rgrc 0
#define STARS_OFFSET_150a_lppos 24
#ifdef STARS_NATIVE16
typedef STARS_CV_117d STARS_FAR *STARS_CV_150b;
#else
typedef STARS_ULONG32 STARS_CV_150b;
#endif
/* tagCOMPAREITEMSTRUCT: 18 bytes; each view begins at its recorded offset. */
union STARS_CV_150f {
    unsigned char raw[18];
    struct { STARS_CV_0073 value; } CtlType;
    struct { unsigned char _pad[2]; STARS_CV_0073 value; } CtlID;
    struct { unsigned char _pad[4]; STARS_CV_0073 value; } hwndItem;
    struct { unsigned char _pad[6]; STARS_CV_0073 value; } itemID1;
    struct { unsigned char _pad[8]; STARS_CV_0022 value; } itemData1;
    struct { unsigned char _pad[12]; STARS_CV_0073 value; } itemID2;
    struct { unsigned char _pad[14]; STARS_CV_0022 value; } itemData2;
};
#define STARS_OFFSET_150f_CtlType 0
#define STARS_OFFSET_150f_CtlID 2
#define STARS_OFFSET_150f_hwndItem 4
#define STARS_OFFSET_150f_itemID1 6
#define STARS_OFFSET_150f_itemData1 8
#define STARS_OFFSET_150f_itemID2 12
#define STARS_OFFSET_150f_itemData2 14
/* tagMINMAXINFO: 20 bytes; each view begins at its recorded offset. */
union STARS_CV_1511 {
    unsigned char raw[20];
    struct { STARS_CV_1018 value; } ptReserved;
    struct { unsigned char _pad[4]; STARS_CV_1018 value; } ptMaxSize;
    struct { unsigned char _pad[8]; STARS_CV_1018 value; } ptMaxPosition;
    struct { unsigned char _pad[12]; STARS_CV_1018 value; } ptMinTrackSize;
    struct { unsigned char _pad[16]; STARS_CV_1018 value; } ptMaxTrackSize;
};
#define STARS_OFFSET_1511_ptReserved 0
#define STARS_OFFSET_1511_ptMaxSize 4
#define STARS_OFFSET_1511_ptMaxPosition 8
#define STARS_OFFSET_1511_ptMinTrackSize 12
#define STARS_OFFSET_1511_ptMaxTrackSize 16
/* tagSEGINFO: 16 bytes; each view begins at its recorded offset. */
union STARS_CV_1513 {
    unsigned char raw[16];
    struct { STARS_CV_0073 value; } offSegment;
    struct { unsigned char _pad[2]; STARS_CV_0073 value; } cbSegment;
    struct { unsigned char _pad[4]; STARS_CV_0073 value; } flags;
    struct { unsigned char _pad[6]; STARS_CV_0073 value; } cbAlloc;
    struct { unsigned char _pad[8]; STARS_CV_0073 value; } h;
    struct { unsigned char _pad[10]; STARS_CV_0073 value; } alignShift;
    struct { unsigned char _pad[12]; STARS_CV_120a value; } reserved;
};
#define STARS_OFFSET_1513_offSegment 0
#define STARS_OFFSET_1513_cbSegment 2
#define STARS_OFFSET_1513_flags 4
#define STARS_OFFSET_1513_cbAlloc 6
#define STARS_OFFSET_1513_h 8
#define STARS_OFFSET_1513_alignShift 10
#define STARS_OFFSET_1513_reserved 12
/* tagKERNINGPAIR: 6 bytes; each view begins at its recorded offset. */
union STARS_CV_1518 {
    unsigned char raw[6];
    struct { STARS_CV_0021 value; } wFirst;
    struct { unsigned char _pad[2]; STARS_CV_0021 value; } wSecond;
    struct { unsigned char _pad[4]; STARS_CV_0072 value; } iKernAmount;
};
#define STARS_OFFSET_1518_wFirst 0
#define STARS_OFFSET_1518_wSecond 2
#define STARS_OFFSET_1518_iKernAmount 4
#ifdef STARS_NATIVE16
typedef STARS_CV_14f8 STARS_FAR *STARS_CV_1519;
#else
typedef STARS_ULONG32 STARS_CV_1519;
#endif
#ifdef STARS_NATIVE16
typedef STARS_CV_14f5 STARS_NEAR *STARS_CV_151b;
#else
typedef unsigned short STARS_CV_151b;
#endif
#ifdef STARS_NATIVE16
typedef STARS_CV_1330 STARS_NEAR *STARS_CV_151c;
#else
typedef unsigned short STARS_CV_151c;
#endif
typedef STARS_CV_0070 STARS_CV_151d[64];
/* tagENUMLOGFONT: 146 bytes; each view begins at its recorded offset. */
union STARS_CV_151f {
    unsigned char raw[146];
    struct { STARS_CV_1197 value; } elfLogFont;
    struct { unsigned char _pad[50]; STARS_CV_151d value; } elfFullName;
    struct { unsigned char _pad[114]; STARS_CV_11e2 value; } elfStyle;
};
#define STARS_OFFSET_151f_elfLogFont 0
#define STARS_OFFSET_151f_elfFullName 50
#define STARS_OFFSET_151f_elfStyle 114
#ifdef STARS_NATIVE16
typedef STARS_CV_11bb STARS_NEAR *STARS_CV_1523;
#else
typedef unsigned short STARS_CV_1523;
#endif
typedef STARS_CV_1005 STARS_CV_1526[];
/* PLORD: 4 bytes; each view begins at its recorded offset. */
union STARS_CV_1528 {
    unsigned char raw[4];
    struct { STARS_CV_135d value; } cbItem;
    struct { STARS_CV_135e value; } fMark;
    struct { STARS_CV_13a9 value; } ht;
    struct { STARS_CV_1362 value; } cAlloc;
    struct { unsigned char _pad[2]; STARS_CV_0020 value; } iordMax;
    struct { unsigned char _pad[3]; STARS_CV_0020 value; } iordMac;
    /* rgord: variable tail at offset 4; no fixed storage. */
};
#define STARS_OFFSET_1528_cbItem 0
#define STARS_BITSHIFT_1528_cbItem 0
#define STARS_BITMASK_1528_cbItem 0xffUL
#define STARS_OFFSET_1528_fMark 0
#define STARS_BITSHIFT_1528_fMark 8
#define STARS_BITMASK_1528_fMark 0x1UL
#define STARS_OFFSET_1528_ht 0
#define STARS_BITSHIFT_1528_ht 9
#define STARS_BITMASK_1528_ht 0x7UL
#define STARS_OFFSET_1528_cAlloc 0
#define STARS_BITSHIFT_1528_cAlloc 12
#define STARS_BITMASK_1528_cAlloc 0xfUL
#define STARS_OFFSET_1528_iordMax 2
#define STARS_OFFSET_1528_iordMac 3
#define STARS_OFFSET_1528_rgord 4
/* _selSome: 28 bytes; each view begins at its recorded offset. */
union STARS_CV_152e {
    unsigned char raw[28];
    struct { STARS_CV_1018 value; } pt;
    struct { unsigned char _pad[4]; STARS_CV_0011 value; } grobj;
    struct { unsigned char _pad[6]; STARS_CV_0011 value; } grobjFull;
    struct { unsigned char _pad[8]; STARS_CV_0011 value; } id;
    struct { unsigned char _pad[10]; STARS_CV_0011 value; } iwpAct;
    struct { unsigned char _pad[12]; STARS_CV_1047 value; } scan;
};
#define STARS_OFFSET_152e_pt 0
#define STARS_OFFSET_152e_grobj 4
#define STARS_OFFSET_152e_grobjFull 6
#define STARS_OFFSET_152e_id 8
#define STARS_OFFSET_152e_iwpAct 10
#define STARS_OFFSET_152e_scan 12
#ifdef STARS_NATIVE16
typedef STARS_CV_14f5 STARS_FAR *STARS_CV_152f;
#else
typedef STARS_ULONG32 STARS_CV_152f;
#endif
#ifdef STARS_NATIVE16
typedef STARS_CV_148f STARS_FAR *STARS_CV_1530;
#else
typedef STARS_ULONG32 STARS_CV_1530;
#endif
/* _planetminimal: 6 bytes; each view begins at its recorded offset. */
union STARS_CV_1532 {
    unsigned char raw[6];
    struct { STARS_CV_0011 value; } id;
    struct { unsigned char _pad[2]; STARS_CV_0011 value; } iPlayer;
    struct { unsigned char _pad[4]; STARS_CV_135d value; } det;
    struct { unsigned char _pad[4]; STARS_CV_135e value; } fInclude;
    struct { unsigned char _pad[4]; STARS_CV_135f value; } fStarbase;
    struct { unsigned char _pad[4]; STARS_CV_1360 value; } unusedA;
    struct { unsigned char _pad[4]; STARS_CV_1361 value; } fFirstYear;
    struct { unsigned char _pad[4]; STARS_CV_1362 value; } unusedB;
};
#define STARS_OFFSET_1532_id 0
#define STARS_OFFSET_1532_iPlayer 2
#define STARS_OFFSET_1532_det 4
#define STARS_BITSHIFT_1532_det 0
#define STARS_BITMASK_1532_det 0xffUL
#define STARS_OFFSET_1532_fInclude 4
#define STARS_BITSHIFT_1532_fInclude 8
#define STARS_BITMASK_1532_fInclude 0x1UL
#define STARS_OFFSET_1532_fStarbase 4
#define STARS_BITSHIFT_1532_fStarbase 9
#define STARS_BITMASK_1532_fStarbase 0x1UL
#define STARS_OFFSET_1532_unusedA 4
#define STARS_BITSHIFT_1532_unusedA 10
#define STARS_BITMASK_1532_unusedA 0x1UL
#define STARS_OFFSET_1532_fFirstYear 4
#define STARS_BITSHIFT_1532_fFirstYear 11
#define STARS_BITMASK_1532_fFirstYear 0x1UL
#define STARS_OFFSET_1532_unusedB 4
#define STARS_BITSHIFT_1532_unusedB 12
#define STARS_BITMASK_1532_unusedB 0xfUL
#ifdef STARS_NATIVE16
typedef STARS_CV_150a STARS_FAR *STARS_CV_1536;
#else
typedef STARS_ULONG32 STARS_CV_1536;
#endif
#ifdef STARS_NATIVE16
typedef STARS_CV_1518 STARS_FAR *STARS_CV_153a;
#else
typedef STARS_ULONG32 STARS_CV_153a;
#endif
#ifdef STARS_NATIVE16
typedef STARS_CV_150f STARS_NEAR *STARS_CV_153b;
#else
typedef unsigned short STARS_CV_153b;
#endif
#ifdef STARS_NATIVE16
typedef STARS_CV_13df STARS_NEAR *STARS_CV_153c;
#else
typedef unsigned short STARS_CV_153c;
#endif
#ifdef STARS_NATIVE16
typedef STARS_CV_151f STARS_FAR *STARS_CV_153d;
#else
typedef STARS_ULONG32 STARS_CV_153d;
#endif
typedef STARS_CV_0012 STARS_FAR STARS_PASCAL STARS_CV_153f(STARS_CV_0022, STARS_CV_0073, STARS_CV_0073, STARS_CV_0012, STARS_CV_0012);
#ifdef STARS_NATIVE16
typedef STARS_CV_153f STARS_FAR *STARS_CV_1540;
#else
typedef STARS_ULONG32 STARS_CV_1540;
#endif
#ifdef STARS_NATIVE16
typedef STARS_CV_1019 STARS_NEAR *STARS_CV_1541;
#else
typedef unsigned short STARS_CV_1541;
#endif
#ifdef STARS_NATIVE16
typedef STARS_CV_1532 STARS_FAR *STARS_CV_1542;
#else
typedef STARS_ULONG32 STARS_CV_1542;
#endif
#ifdef STARS_NATIVE16
typedef STARS_CV_1513 STARS_FAR *STARS_CV_1543;
#else
typedef STARS_ULONG32 STARS_CV_1543;
#endif
typedef STARS_CV_143c STARS_CV_1544[1];
/* tagBITMAPINFO: 44 bytes; each view begins at its recorded offset. */
union STARS_CV_1546 {
    unsigned char raw[44];
    struct { STARS_CV_11bb value; } bmiHeader;
    struct { unsigned char _pad[40]; STARS_CV_1544 value; } bmiColors;
};
#define STARS_OFFSET_1546_bmiHeader 0
#define STARS_OFFSET_1546_bmiColors 40
#ifdef STARS_NATIVE16
typedef STARS_CV_11be STARS_FAR *STARS_CV_1547;
#else
typedef STARS_ULONG32 STARS_CV_1547;
#endif
#ifdef STARS_NATIVE16
typedef STARS_CV_13df STARS_FAR *STARS_CV_1548;
#else
typedef STARS_ULONG32 STARS_CV_1548;
#endif
#ifdef STARS_NATIVE16
typedef STARS_CV_150f STARS_FAR *STARS_CV_1549;
#else
typedef STARS_ULONG32 STARS_CV_1549;
#endif
/* tagRASTERIZER_STATUS: 6 bytes; each view begins at its recorded offset. */
union STARS_CV_154d {
    unsigned char raw[6];
    struct { STARS_CV_0072 value; } nSize;
    struct { unsigned char _pad[2]; STARS_CV_0072 value; } wFlags;
    struct { unsigned char _pad[4]; STARS_CV_0072 value; } nLanguageID;
};
#define STARS_OFFSET_154d_nSize 0
#define STARS_OFFSET_154d_wFlags 2
#define STARS_OFFSET_154d_nLanguageID 4
#ifdef STARS_NATIVE16
typedef STARS_CV_1532 STARS_NEAR *STARS_CV_154e;
#else
typedef unsigned short STARS_CV_154e;
#endif
/* _rtloghdr: 17 bytes; each view begins at its recorded offset. */
union STARS_CV_1550 {
    unsigned char raw[17];
    struct { STARS_CV_0011 value; } cbLog;
    struct { unsigned char _pad[2]; STARS_CV_0012 value; } lSerialNumber;
    struct { unsigned char _pad[6]; STARS_CV_11e5 value; } rgbConfig;
};
#define STARS_OFFSET_1550_cbLog 0
#define STARS_OFFSET_1550_lSerialNumber 2
#define STARS_OFFSET_1550_rgbConfig 6
#ifdef STARS_NATIVE16
typedef STARS_CV_10ca STARS_FAR *STARS_CV_1551;
#else
typedef STARS_ULONG32 STARS_CV_1551;
#endif
#ifdef STARS_NATIVE16
typedef STARS_CV_1019 STARS_FAR *STARS_CV_1552;
#else
typedef STARS_ULONG32 STARS_CV_1552;
#endif
/* _mdplr: 2 bytes; each view begins at its recorded offset. */
union STARS_CV_1554 {
    unsigned char raw[2];
    struct { STARS_CV_13d4 value; } reserved;
    struct { STARS_CV_135f value; } fAi;
    struct { STARS_CV_13d5 value; } lvlAi;
    struct { STARS_CV_13d0 value; } idAi;
};
#define STARS_OFFSET_1554_reserved 0
#define STARS_BITSHIFT_1554_reserved 0
#define STARS_BITMASK_1554_reserved 0x1ffUL
#define STARS_OFFSET_1554_fAi 0
#define STARS_BITSHIFT_1554_fAi 9
#define STARS_BITMASK_1554_fAi 0x1UL
#define STARS_OFFSET_1554_lvlAi 0
#define STARS_BITSHIFT_1554_lvlAi 10
#define STARS_BITMASK_1554_lvlAi 0x7UL
#define STARS_OFFSET_1554_idAi 0
#define STARS_BITSHIFT_1554_idAi 13
#define STARS_BITMASK_1554_idAi 0x7UL
#ifdef STARS_NATIVE16
typedef STARS_CV_1546 STARS_NEAR *STARS_CV_1566;
#else
typedef unsigned short STARS_CV_1566;
#endif
typedef STARS_CV_10e9 STARS_CV_1567[];
/* _div_t: 4 bytes; each view begins at its recorded offset. */
union STARS_CV_156a {
    unsigned char raw[4];
    struct { STARS_CV_0072 value; } quot;
    struct { unsigned char _pad[2]; STARS_CV_0072 value; } rem;
};
#define STARS_OFFSET_156a_quot 0
#define STARS_OFFSET_156a_rem 2
#ifdef STARS_NATIVE16
typedef STARS_CV_1546 STARS_FAR *STARS_CV_156b;
#else
typedef STARS_ULONG32 STARS_CV_156b;
#endif
/* _ldiv_t: 8 bytes; each view begins at its recorded offset. */
union STARS_CV_156d {
    unsigned char raw[8];
    struct { STARS_CV_0012 value; } quot;
    struct { unsigned char _pad[4]; STARS_CV_0012 value; } rem;
};
#define STARS_OFFSET_156d_quot 0
#define STARS_OFFSET_156d_rem 4
#ifdef STARS_NATIVE16
typedef STARS_CV_154d STARS_FAR *STARS_CV_156e;
#else
typedef STARS_ULONG32 STARS_CV_156e;
#endif
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_1570(STARS_CV_0011);
typedef STARS_CV_0021 STARS_CV_1571[512];
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_1572(STARS_CV_1009);
typedef STARS_CV_0020 STARS_CV_1573[149];
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_1575(STARS_CV_0072, STARS_CV_0072, STARS_CV_0170);
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_1577(STARS_CV_1004, STARS_CV_1242);
#ifdef STARS_NATIVE16
typedef STARS_CV_10be STARS_NEAR *STARS_CV_1578;
#else
typedef unsigned short STARS_CV_1578;
#endif
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_157a(STARS_CV_1578);
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_157c(STARS_CV_100b, STARS_CV_13af);
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_157e(STARS_CV_1007, STARS_CV_13b0);
#ifdef STARS_NATIVE16
typedef STARS_CV_13ec STARS_NEAR *STARS_CV_157f;
#else
typedef unsigned short STARS_CV_157f;
#endif
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_1581(STARS_CV_157f, STARS_CV_157f);
#ifdef STARS_NATIVE16
typedef STARS_CV_140c STARS_FAR *STARS_CV_1582;
#else
typedef STARS_ULONG32 STARS_CV_1582;
#endif
#ifdef STARS_NATIVE16
typedef STARS_CV_1431 STARS_FAR *STARS_CV_1583;
#else
typedef STARS_ULONG32 STARS_CV_1583;
#endif
typedef STARS_CV_0070 STARS_CV_1584[28];
#ifdef STARS_NATIVE16
typedef STARS_CV_1421 STARS_FAR *STARS_CV_1585;
#else
typedef STARS_ULONG32 STARS_CV_1585;
#endif
#ifdef STARS_NATIVE16
typedef STARS_CV_13fb STARS_NEAR *STARS_CV_1586;
#else
typedef unsigned short STARS_CV_1586;
#endif
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_1588(STARS_CV_1586, STARS_CV_1586);
#ifdef STARS_NATIVE16
typedef STARS_CV_1436 STARS_NEAR *STARS_CV_1589;
#else
typedef unsigned short STARS_CV_1589;
#endif
typedef STARS_CV_0070 STARS_CV_158a[41];
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_158c(STARS_CV_0072, STARS_CV_0072, STARS_CV_0103);
#ifdef STARS_NATIVE16
typedef STARS_CV_12ec STARS_NEAR *STARS_CV_158d;
#else
typedef unsigned short STARS_CV_158d;
#endif
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_158f(STARS_CV_158d, STARS_CV_0120);
#ifdef STARS_NATIVE16
typedef STARS_CV_12ec STARS_FAR *STARS_CV_1590;
#else
typedef STARS_ULONG32 STARS_CV_1590;
#endif
typedef STARS_CV_0072 STARS_CV_1591[512];
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_1592(STARS_CV_0072, STARS_CV_0172);
typedef STARS_CV_0070 STARS_CV_1593[30];
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_1595(STARS_CV_0203, STARS_CV_0072, STARS_CV_0072, STARS_CV_0203, STARS_CV_0072);
#ifdef STARS_NATIVE16
typedef STARS_CV_1595 STARS_FAR *STARS_CV_1596;
#else
typedef STARS_ULONG32 STARS_CV_1596;
#endif
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_1598(STARS_CV_1596, STARS_CV_0203, STARS_CV_0072);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_1599(STARS_CV_0012);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_159b(STARS_CV_1242);
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_159d(STARS_CV_0073, STARS_CV_0073, STARS_CV_0012);
typedef STARS_CV_0070 STARS_CV_159e[34];
typedef STARS_CV_0012 STARS_FAR STARS_CDECL STARS_CV_159f(STARS_CV_1004, STARS_CV_0072);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_15a1(STARS_CV_1009, STARS_CV_0072);
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_15a3(STARS_CV_0073, STARS_CV_101b, STARS_CV_1004, STARS_CV_0072);
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_15a5(STARS_CV_1004, STARS_CV_0073, STARS_CV_0072, STARS_CV_0072, STARS_CV_0072, STARS_CV_0072, STARS_CV_0072, STARS_CV_0072, STARS_CV_0072, STARS_CV_0072);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_15a7(STARS_CV_0203, STARS_CV_0072, STARS_CV_0072, STARS_CV_100b, STARS_CV_0072);
typedef STARS_CV_13ef STARS_CV_15a8[32];
typedef STARS_CV_0012 STARS_FAR STARS_CDECL STARS_CV_15aa(STARS_CV_0072, STARS_CV_0072, STARS_CV_0072, STARS_CV_0012, STARS_CV_0103);
typedef STARS_CV_0012 STARS_FAR STARS_CDECL STARS_CV_15ac(STARS_CV_0072, STARS_CV_0012);
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_15ae(STARS_CV_101b, STARS_CV_101b, STARS_CV_101b);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_15b0(STARS_CV_101b);
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_15b2(STARS_CV_0073, STARS_CV_101b, STARS_CV_13b0, STARS_CV_0072);
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_15b4(STARS_CV_0073, STARS_CV_101b, STARS_CV_1242, STARS_CV_0072);
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_15b6(STARS_CV_0073, STARS_CV_101b, STARS_CV_13af, STARS_CV_0072);
typedef STARS_CV_0073 STARS_FAR STARS_CDECL STARS_CV_15b7(STARS_CV_1018, STARS_CV_0072, STARS_CV_0072, STARS_CV_0072);
typedef STARS_CV_0170 STARS_CV_15b8[11];
#ifdef STARS_NATIVE16
typedef STARS_CV_137f STARS_FAR *STARS_CV_15b9;
#else
typedef STARS_ULONG32 STARS_CV_15b9;
#endif
typedef STARS_CV_0012 STARS_CV_15ba[100];
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_15bb(STARS_CV_1004, STARS_CV_0072, STARS_CV_0072);
typedef STARS_CV_0012 STARS_FAR STARS_CDECL STARS_CV_15bd(STARS_CV_1004, STARS_CV_0072, STARS_CV_0072, STARS_CV_0012, STARS_CV_0072);
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_15bf(STARS_CV_1242, STARS_CV_1242);
typedef STARS_CV_13f9 STARS_CV_15c0[2];
typedef STARS_CV_108f STARS_CV_15c1[2];
typedef STARS_CV_1242 STARS_CV_15c2[2];
typedef STARS_CV_1003 STARS_CV_15c3[2];
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_15c5(STARS_CV_0072, STARS_CV_0172, STARS_CV_0072);
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_15c7(STARS_CV_1004, STARS_CV_1004, STARS_CV_0072);
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_15c9(STARS_CV_0072, STARS_CV_10fa, STARS_CV_0121);
#ifdef STARS_NATIVE16
typedef STARS_CV_132e STARS_NEAR *STARS_CV_15ca;
#else
typedef unsigned short STARS_CV_15ca;
#endif
#ifdef STARS_NATIVE16
typedef STARS_CV_13ef STARS_NEAR *STARS_CV_15cb;
#else
typedef unsigned short STARS_CV_15cb;
#endif
typedef STARS_CV_0170 STARS_CV_15cc[4];
typedef STARS_CV_13b6 STARS_CV_15cd[7];
typedef STARS_CV_101a STARS_CV_15ce[19];
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_15d0(STARS_CV_0073, STARS_CV_13bb, STARS_CV_101b, STARS_CV_0170);
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_15d2(STARS_CV_0073, STARS_CV_101b, STARS_CV_0072, STARS_CV_0072);
typedef STARS_CV_0170 STARS_FAR STARS_CDECL STARS_CV_15d3(STARS_CV_100b, STARS_CV_1055, STARS_CV_0072, STARS_CV_0172, STARS_CV_0172);
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_15d4(STARS_CV_0072, STARS_CV_0072, STARS_CV_0072, STARS_CV_0072);
typedef STARS_CV_0170 STARS_FAR STARS_CDECL STARS_CV_15d5(STARS_CV_0072, STARS_CV_0072);
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_15d7(STARS_CV_0073, STARS_CV_101b, STARS_CV_0170, STARS_CV_0072, STARS_CV_0072, STARS_CV_0072);
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_15d9(STARS_CV_0073, STARS_CV_1055, STARS_CV_100b);
typedef STARS_CV_0012 STARS_FAR STARS_CDECL STARS_CV_15da(STARS_CV_0072, STARS_CV_0072);
typedef STARS_CV_0011 STARS_FAR STARS_CDECL STARS_CV_15db(STARS_CV_100b, STARS_CV_0072);
typedef STARS_CV_0011 STARS_FAR STARS_CDECL STARS_CV_15dc(STARS_CV_100b, STARS_CV_0072, STARS_CV_0072);
typedef STARS_CV_0011 STARS_FAR STARS_CDECL STARS_CV_15dd(STARS_CV_100b);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_15df(STARS_CV_100b, STARS_CV_0172);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_15e0(STARS_CV_104a);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_15e2(STARS_CV_100b, STARS_CV_0172, STARS_CV_0172, STARS_CV_0172, STARS_CV_0072);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_15e4(STARS_CV_1077, STARS_CV_0072, STARS_CV_0172);
typedef STARS_CV_0073 STARS_CV_15e5[13];
typedef STARS_CV_13b6 STARS_CV_15e6[6];
typedef STARS_CV_13b6 STARS_CV_15e7[];
#ifdef STARS_NATIVE16
typedef STARS_CV_14cf STARS_FAR *STARS_CV_15e8;
#else
typedef STARS_ULONG32 STARS_CV_15e8;
#endif
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_15ea(STARS_CV_15e8, STARS_CV_1009, STARS_CV_0072);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_15eb(STARS_CV_0170, STARS_CV_0170);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_15ed(STARS_CV_0072, STARS_CV_100b, STARS_CV_0072, STARS_CV_0072);
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_15ef(STARS_CV_0220, STARS_CV_10bf, STARS_CV_0072);
#ifdef STARS_NATIVE16
typedef STARS_CV_1438 STARS_FAR *STARS_CV_15f0;
#else
typedef STARS_ULONG32 STARS_CV_15f0;
#endif
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_15f1(STARS_CV_0022);
typedef STARS_CV_0022 STARS_CV_15f2[25];
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_15f3(STARS_CV_0073);
#ifdef STARS_NATIVE16
typedef STARS_CV_1535 STARS_NEAR *STARS_CV_15f4;
#else
typedef unsigned short STARS_CV_15f4;
#endif
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_15f6(STARS_CV_0170, STARS_CV_0170, STARS_CV_0072, STARS_CV_15f4);
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_15f8(STARS_CV_0170, STARS_CV_13bb, STARS_CV_0072);
typedef STARS_CV_0020 STARS_CV_15f9[20];
typedef STARS_CV_120a STARS_CV_15fa[4];
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_15fb(STARS_CV_0073, STARS_CV_101b);
typedef STARS_CV_0070 STARS_CV_15fc[999];
typedef STARS_CV_0072 STARS_CV_15fd[250];
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_15ff(STARS_CV_1004, STARS_CV_0072, STARS_CV_0072, STARS_CV_0073, STARS_CV_0073);
#ifdef STARS_NATIVE16
typedef STARS_CV_1477 STARS_NEAR *STARS_CV_1600;
#else
typedef unsigned short STARS_CV_1600;
#endif
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_1602(STARS_CV_1600, STARS_CV_101b);
typedef STARS_CV_0072 STARS_CV_1603[87];
#ifdef STARS_NATIVE16
typedef STARS_CV_144e STARS_NEAR *STARS_CV_1604;
#else
typedef unsigned short STARS_CV_1604;
#endif
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_1606(STARS_CV_0073, STARS_CV_101b, STARS_CV_1604, STARS_CV_0072);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_1608(STARS_CV_1018, STARS_CV_0072);
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_1609(STARS_CV_1018, STARS_CV_0072);
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_160a(STARS_CV_1020);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_160c(STARS_CV_1018, STARS_CV_144c);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_160e(STARS_CV_1004, STARS_CV_1072);
typedef STARS_CV_1018 STARS_CV_160f[4];
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_1611(STARS_CV_144c, STARS_CV_0072);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_1612(STARS_CV_144c, STARS_CV_0072);
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_1614(STARS_CV_1004, STARS_CV_1020, STARS_CV_1020);
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_1616(STARS_CV_0072, STARS_CV_0072, STARS_CV_1020, STARS_CV_1020);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_1618(STARS_CV_144c, STARS_CV_1018);
typedef STARS_CV_12f7 STARS_CV_1619[2];
typedef STARS_CV_0072 STARS_CV_161a[19];
typedef STARS_CV_0022 STARS_CV_161b[3];
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_161d(STARS_CV_13af);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_161e(STARS_CV_100b, STARS_CV_13af);
typedef STARS_CV_1007 STARS_FAR STARS_CDECL STARS_CV_161f(STARS_CV_0072);
typedef STARS_CV_100b STARS_FAR STARS_CDECL STARS_CV_1620(STARS_CV_0072);
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_1622(STARS_CV_100b, STARS_CV_0140, STARS_CV_0140);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_1624(STARS_CV_0072, STARS_CV_13af);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_1626(STARS_CV_0072, STARS_CV_13b0);
typedef STARS_CV_0012 STARS_FAR STARS_CDECL STARS_CV_1627(STARS_CV_1009);
typedef STARS_CV_0012 STARS_FAR STARS_CDECL STARS_CV_1628(STARS_CV_1009, STARS_CV_0072);
typedef STARS_CV_0170 STARS_FAR STARS_CDECL STARS_CV_162a(STARS_CV_0072, STARS_CV_0072, STARS_CV_0072, STARS_CV_0072, STARS_CV_0072, STARS_CV_11ef);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_162c(STARS_CV_1004, STARS_CV_0112, STARS_CV_0112, STARS_CV_0112, STARS_CV_0112, STARS_CV_0112, STARS_CV_0172);
#ifdef STARS_NATIVE16
typedef STARS_CV_1004 STARS_NEAR *STARS_CV_162d;
#else
typedef unsigned short STARS_CV_162d;
#endif
typedef STARS_CV_1004 STARS_CV_162e[63];
typedef STARS_CV_1004 STARS_FAR STARS_CDECL STARS_CV_162f(STARS_CV_0072);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_1631(STARS_CV_0072, STARS_CV_1242);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_1632(STARS_CV_1004, STARS_CV_1242);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_1633(STARS_CV_0072, STARS_CV_0072, STARS_CV_0103);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_1635(STARS_CV_0072, STARS_CV_0072, STARS_CV_15ca, STARS_CV_0072);
typedef STARS_CV_0070 STARS_CV_1636[54];
typedef STARS_CV_0021 STARS_FAR STARS_CDECL STARS_CV_1637(STARS_CV_1004);
typedef STARS_CV_0170 STARS_FAR STARS_CDECL STARS_CV_1639(STARS_CV_0021);
typedef STARS_CV_1004 STARS_FAR STARS_CDECL STARS_CV_163a(STARS_CV_0072, STARS_CV_0072);
typedef STARS_CV_1004 STARS_FAR STARS_CDECL STARS_CV_163b(STARS_CV_1242);
typedef STARS_CV_0170 STARS_FAR STARS_CDECL STARS_CV_163c(STARS_CV_0072, STARS_CV_0072, STARS_CV_0072, STARS_CV_0072);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_163e(STARS_CV_0073, STARS_CV_1004, STARS_CV_0170, STARS_CV_0072, STARS_CV_0072);
typedef STARS_CV_0041 STARS_FAR STARS_CDECL STARS_CV_163f(STARS_CV_0072, STARS_CV_0072, STARS_CV_0072, STARS_CV_0072);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_1641(STARS_CV_1018, STARS_CV_0072, STARS_CV_144c);
typedef STARS_CV_0021 STARS_FAR STARS_CDECL STARS_CV_1642(STARS_CV_0012);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_1644(STARS_CV_1004, STARS_CV_0172, STARS_CV_0172, STARS_CV_0172);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_1646(STARS_CV_1009, STARS_CV_0072, STARS_CV_0172, STARS_CV_0172, STARS_CV_0172);
typedef STARS_CV_0012 STARS_FAR STARS_CDECL STARS_CV_1648(STARS_CV_1004, STARS_CV_0072, STARS_CV_0012);
#ifdef STARS_NATIVE16
typedef STARS_CV_1336 STARS_NEAR *STARS_CV_1649;
#else
typedef unsigned short STARS_CV_1649;
#endif
typedef STARS_CV_0012 STARS_FAR STARS_CDECL STARS_CV_164b(STARS_CV_0072, STARS_CV_1649);
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_164d(STARS_CV_0072, STARS_CV_1077, STARS_CV_0121);
typedef STARS_CV_0012 STARS_FAR STARS_CDECL STARS_CV_164f(STARS_CV_0070);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_1651(STARS_CV_0170, STARS_CV_0112);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_1652(STARS_CV_1004, STARS_CV_0072, STARS_CV_0072);
typedef STARS_CV_0012 STARS_FAR STARS_CDECL STARS_CV_1653(STARS_CV_100b, STARS_CV_0072);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_1655(STARS_CV_1004, STARS_CV_1018, STARS_CV_1018);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_1656(STARS_CV_1077, STARS_CV_0072);
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_1657(STARS_CV_0073, STARS_CV_0072, STARS_CV_0072, STARS_CV_0072);
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_1659(STARS_CV_0072, STARS_CV_0270);
typedef STARS_CV_0022 STARS_CV_165a[5];
typedef STARS_CV_0170 STARS_CV_165b[8];
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_165d(STARS_CV_0073, STARS_CV_0172, STARS_CV_0172);
typedef STARS_CV_11e2 STARS_CV_165e[16];
typedef STARS_CV_0170 STARS_CV_165f[16];
typedef STARS_CV_0070 STARS_CV_1660[10];
typedef STARS_CV_1660 STARS_CV_1661[9];
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_1663(STARS_CV_0073, STARS_CV_101b, STARS_CV_0073);
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_1665(STARS_CV_100b, STARS_CV_0112, STARS_CV_0012, STARS_CV_0072);
typedef STARS_CV_0170 STARS_CV_1666[3];
typedef STARS_CV_1018 STARS_FAR STARS_CDECL STARS_CV_1668(STARS_CV_0073, STARS_CV_0072, STARS_CV_1018, STARS_CV_1020);
typedef STARS_CV_0073 STARS_FAR STARS_CDECL STARS_CV_166a(STARS_CV_1018, STARS_CV_0172);
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_166c(STARS_CV_0012, STARS_CV_0120, STARS_CV_0170);
typedef STARS_CV_0020 STARS_CV_166d[21];
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_166f(STARS_CV_0112, STARS_CV_0120, STARS_CV_0170);
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_1671(STARS_CV_0073, STARS_CV_0021);
typedef STARS_CV_1554 STARS_CV_1672[16];
typedef STARS_CV_0073 STARS_FAR STARS_CDECL STARS_CV_1673(STARS_CV_0073, STARS_CV_0072);
typedef STARS_CV_0003 STARS_FAR STARS_PASCAL STARS_CV_1675(STARS_CV_0073, STARS_CV_0073, STARS_CV_0073, STARS_CV_0022);
typedef STARS_CV_0003 STARS_FAR STARS_CDECL STARS_CV_1677(STARS_CV_0170, STARS_CV_0073);
typedef STARS_CV_0070 STARS_CV_1678[11];
typedef STARS_CV_126b STARS_CV_1679[2];
typedef STARS_CV_0012 STARS_FAR STARS_PASCAL STARS_CV_167b(STARS_CV_0270, STARS_CV_0073, STARS_CV_0012, STARS_CV_0012);
#ifdef STARS_NATIVE16
typedef STARS_CV_167b STARS_FAR *STARS_CV_167c;
#else
typedef STARS_ULONG32 STARS_CV_167c;
#endif
typedef STARS_CV_0073 STARS_FAR STARS_PASCAL STARS_CV_167e(STARS_CV_0073, STARS_CV_0022);
#ifdef STARS_NATIVE16
typedef STARS_CV_167e STARS_FAR *STARS_CV_167f;
#else
typedef STARS_ULONG32 STARS_CV_167f;
#endif
typedef STARS_CV_0003 STARS_FAR STARS_PASCAL STARS_CV_1681(STARS_CV_0073, STARS_CV_0073, STARS_CV_0022, STARS_CV_0022, STARS_CV_0022);
#ifdef STARS_NATIVE16
typedef STARS_CV_1681 STARS_FAR *STARS_CV_1682;
#else
typedef STARS_ULONG32 STARS_CV_1682;
#endif
typedef STARS_CV_0072 STARS_FAR STARS_PASCAL STARS_CV_1684(STARS_CV_0073, STARS_CV_0073, STARS_CV_0270, STARS_CV_0072);
typedef STARS_CV_0170 STARS_FAR STARS_CDECL STARS_CV_1685();
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_1686(STARS_CV_0073, STARS_CV_0021, STARS_CV_0012);
typedef STARS_CV_0072 STARS_FAR STARS_CDECL STARS_CV_1688(STARS_CV_0073, STARS_CV_0072, STARS_CV_0072, STARS_CV_0022);
typedef STARS_CV_1008 STARS_CV_1689[16];
typedef STARS_CV_1009 STARS_CV_168a[16];
typedef STARS_CV_1018 STARS_CV_168b[999];
typedef STARS_CV_0072 STARS_CV_168c[999];
typedef STARS_CV_0070 STARS_CV_168d[360];
typedef STARS_CV_125c STARS_ALIAS_PL;
typedef STARS_CV_1002 STARS_ALIAS_PLANET;
typedef STARS_CV_1242 STARS_ALIAS_PFLEET;
typedef STARS_CV_0120 STARS_ALIAS_PBYTE;
typedef STARS_CV_14f5 STARS_ALIAS_PATTERN;
typedef STARS_CV_1507 STARS_ALIAS_PSIZE;
typedef STARS_CV_1014 STARS_ALIAS_PLAYER;
typedef STARS_CV_13ea STARS_ALIAS_DRVCONFIGINFO;
typedef STARS_CV_134b STARS_ALIAS_EVENTMSG;
typedef STARS_CV_1019 STARS_ALIAS_PAINTSTRUCT;
typedef STARS_CV_14a3 STARS_ALIAS_LPMDICREATESTRUCT;
typedef STARS_CV_150f STARS_ALIAS_tagCOMPAREITEMSTRUCT;
typedef STARS_CV_153c STARS_ALIAS_PHELPWININFO;
typedef STARS_CV_1436 STARS_ALIAS_RTXFERF;
typedef STARS_CV_1431 STARS_ALIAS_RTXFERL;
typedef STARS_CV_1421 STARS_ALIAS_RTXFERX;
typedef STARS_CV_140c STARS_ALIAS_RTXFER;
typedef STARS_CV_143c STARS_ALIAS_RGBQUAD;
typedef STARS_CV_14ff STARS_ALIAS_tagMULTIKEYHELP;
typedef STARS_CV_109a STARS_ALIAS_SCOREX;
typedef STARS_CV_1336 STARS_ALIAS_SCORE;
typedef STARS_CV_142b STARS_ALIAS_COMSTAT;
typedef STARS_CV_156a STARS_ALIAS_div_t;
typedef STARS_CV_0022 STARS_ALIAS_DWORD;
typedef STARS_CV_101a STARS_ALIAS_tagRECT;
typedef STARS_CV_1496 STARS_ALIAS_THTRADER;
typedef STARS_CV_1546 STARS_ALIAS_BITMAPINFO;
typedef STARS_CV_13e8 STARS_ALIAS_DEVNAMES;
typedef STARS_CV_14fc STARS_ALIAS_FLEETID;
typedef STARS_CV_140e STARS_ALIAS_FIXED;
typedef STARS_CV_1003 STARS_ALIAS_FLEET;
typedef STARS_CV_153a STARS_ALIAS_LPKERNINGPAIR;
typedef STARS_CV_14db STARS_ALIAS_DELETEITEMSTRUCT;
typedef STARS_CV_133f STARS_ALIAS_TURNSERIAL;
typedef STARS_CV_0073 STARS_ALIAS_HFONT;
typedef STARS_CV_1018 STARS_ALIAS_tagPOINT;
typedef STARS_CV_136d STARS_ALIAS_LPPLANETSOME;
typedef STARS_CV_1454 STARS_ALIAS_WINDOWPOS;
typedef STARS_CV_1009 STARS_ALIAS_LPSHDEF;
typedef STARS_CV_1444 STARS_ALIAS_LPMAT2;
typedef STARS_CV_0272 STARS_ALIAS_LPINT;
typedef STARS_CV_0220 STARS_ALIAS_LPBYTE;
typedef STARS_CV_0270 STARS_ALIAS_LPCSTR;
typedef STARS_CV_1536 STARS_ALIAS_LPNCCALCSIZE_PARAMS;
typedef STARS_CV_140e STARS_ALIAS_tagFIXED;
typedef STARS_CV_12e0 STARS_ALIAS_MSGHDR;
typedef STARS_CV_11c5 STARS_ALIAS_tagBITMAP;
typedef STARS_CV_149d STARS_ALIAS_LPPOINTFX;
typedef STARS_CV_14e7 STARS_ALIAS_LPTTPOLYGONHEADER;
typedef STARS_CV_151f STARS_ALIAS_tagENUMLOGFONT;
typedef STARS_CV_13b6 STARS_ALIAS__tile;
typedef STARS_CV_1003 STARS_ALIAS__fleet;
typedef STARS_CV_12a1 STARS_ALIAS__mines;
typedef STARS_CV_1012 STARS_ALIAS__game;
typedef STARS_CV_129c STARS_ALIAS__shield;
typedef STARS_CV_1482 STARS_ALIAS__timer;
typedef STARS_CV_1008 STARS_ALIAS__shdef;
typedef STARS_CV_1005 STARS_ALIAS__order;
typedef STARS_CV_14f0 STARS_ALIAS_ABC;
typedef STARS_CV_1018 STARS_ALIAS_POINT;
typedef STARS_CV_101b STARS_ALIAS_PRECT;
typedef STARS_CV_148d STARS_ALIAS_POINTFX;
typedef STARS_CV_0173 STARS_ALIAS_PHANDLE;
typedef STARS_CV_1528 STARS_ALIAS_PLORD;
typedef STARS_CV_0121 STARS_ALIAS_PWORD;
typedef STARS_CV_13e2 STARS_ALIAS_MENUITEMTEMPLATE;
typedef STARS_CV_14fa STARS_ALIAS_POFSTRUCT;
typedef STARS_CV_14cf STARS_ALIAS_RTSHDEF;
typedef STARS_CV_1156 STARS_ALIAS_BTLREC;
typedef STARS_CV_143c STARS_ALIAS_tagRGBQUAD;
typedef STARS_CV_11bf STARS_ALIAS_PLOGPALETTE;
typedef STARS_CV_1459 STARS_ALIAS_tagPD;
typedef STARS_CV_0000 STARS_ALIAS_DRVCALLBACK;
typedef STARS_CV_13e4 STARS_ALIAS_MENUITEMTEMPLATEHEADER;
typedef STARS_CV_12f6 STARS_ALIAS__starpack;
typedef STARS_CV_149a STARS_ALIAS_tagCBT_CREATEWND;
typedef STARS_CV_1385 STARS_ALIAS_TASKSELL;
typedef STARS_CV_0073 STARS_ALIAS_GLOBALHANDLE;
typedef STARS_CV_0073 STARS_ALIAS_HMENU;
typedef STARS_CV_0073 STARS_ALIAS_HACCEL;
typedef STARS_CV_0072 STARS_ALIAS_HFILE;
typedef STARS_CV_0073 STARS_ALIAS_HMODULE;
typedef STARS_CV_1289 STARS_ALIAS_HULDEF;
typedef STARS_CV_0073 STARS_ALIAS_HWAVEIN;
typedef STARS_CV_139d STARS_ALIAS_BITMAPFILEHEADER;
typedef STARS_CV_1037 STARS_ALIAS_WNDENUMPROC;
typedef STARS_CV_13a7 STARS_ALIAS_tagWNDCLASS;
typedef STARS_CV_142b STARS_ALIAS_tagCOMSTAT;
typedef STARS_CV_1037 STARS_ALIAS_GNOTIFYPROC;
typedef STARS_CV_1004 STARS_ALIAS_LPFLEET;
typedef STARS_CV_0012 STARS_ALIAS_LRESULT;
typedef STARS_CV_1407 STARS_ALIAS_LOGPEN;
typedef STARS_CV_150f STARS_ALIAS_COMPAREITEMSTRUCT;
typedef STARS_CV_149f STARS_ALIAS_MOUSEHOOKSTRUCT;
typedef STARS_CV_1479 STARS_ALIAS_tagMETARECORD;
typedef STARS_CV_12de STARS_ALIAS_MSGTURN;
typedef STARS_CV_14e5 STARS_ALIAS_DRIVERINFOSTRUCT;
typedef STARS_CV_153d STARS_ALIAS_LPENUMLOGFONT;
typedef STARS_CV_1349 STARS_ALIAS__prodq1;
typedef STARS_CV_1039 STARS_ALIAS__prod;
typedef STARS_CV_1013 STARS_ALIAS__gdata;
typedef STARS_CV_115c STARS_ALIAS__btnt;
typedef STARS_CV_1289 STARS_ALIAS__huldef;
typedef STARS_CV_116e STARS_ALIAS__find_t;
typedef STARS_CV_105e STARS_ALIAS__part;
typedef STARS_CV_10b4 STARS_ALIAS_RPT;
typedef STARS_CV_13ef STARS_ALIAS_BTN;
typedef STARS_CV_135b STARS_ALIAS__rtChgPlanetLong;
typedef STARS_CV_0112 STARS_ALIAS_PLONG;
typedef STARS_CV_1345 STARS_ALIAS_RTCHGPRODQ;
typedef STARS_CV_1006 STARS_ALIAS_THING;
typedef STARS_CV_14b7 STARS_ALIAS_tagCBTACTIVATESTRUCT;
typedef STARS_CV_10cb STARS_ALIAS_ITEMACTION;
typedef STARS_CV_1354 STARS_ALIAS_FINDREPLACE;
typedef STARS_CV_1037 STARS_ALIAS_TIMERPROC;
typedef STARS_CV_1019 STARS_ALIAS_tagPAINTSTRUCT;
typedef STARS_CV_1368 STARS_ALIAS_RGBTRIPLE;
typedef STARS_CV_147e STARS_ALIAS_PDRAWITEMSTRUCT;
typedef STARS_CV_14a6 STARS_ALIAS_TTPOLYCURVE;
typedef STARS_CV_10cb STARS_ALIAS__itemaction;
typedef STARS_CV_14a6 STARS_ALIAS_tagTTPOLYCURVE;
typedef STARS_CV_133f STARS_ALIAS__turnserial;
typedef STARS_CV_134e STARS_ALIAS__rtlogthing;
typedef STARS_CV_1279 STARS_ALIAS_MEASUREITEMSTRUCT;
typedef STARS_CV_13ec STARS_ALIAS_LOGXFER;
typedef STARS_CV_1451 STARS_ALIAS_RTSHIPINT2;
typedef STARS_CV_1443 STARS_ALIAS_RTSHIPINT;
typedef STARS_CV_154d STARS_ALIAS_RASTERIZER_STATUS;
typedef STARS_CV_1040 STARS_ALIAS_CYBERINFOTEMP;
typedef STARS_CV_1370 STARS_ALIAS_PBITMAPCOREINFO;
typedef STARS_CV_105b STARS_ALIAS_SEL;
typedef STARS_CV_13a4 STARS_ALIAS_CLIENTCREATESTRUCT;
typedef STARS_CV_14ad STARS_ALIAS_LPCBT_CREATEWND;
typedef STARS_CV_1323 STARS_ALIAS_RTBOF;
typedef STARS_CV_13df STARS_ALIAS_HELPWININFO;
typedef STARS_CV_154d STARS_ALIAS_tagRASTERIZER_STATUS;
typedef STARS_CV_1008 STARS_ALIAS_SHDEF;
typedef STARS_CV_0000 STARS_ALIAS_TIMECALLBACK;
typedef STARS_CV_14d9 STARS_ALIAS_RTCHGSHDEF;
typedef STARS_CV_1486 STARS_ALIAS_RTCHGNAME;
typedef STARS_CV_0000 STARS_ALIAS_WAVECALLBACK;
typedef STARS_CV_1206 STARS_ALIAS_tagOFN;
typedef STARS_CV_13a7 STARS_ALIAS_WNDCLASS;
typedef STARS_CV_135c STARS_ALIAS_PEVENTMSG;
typedef STARS_CV_14e5 STARS_ALIAS_tagDRIVERINFOSTRUCT;
typedef STARS_CV_14a1 STARS_ALIAS_LPOUTLINETEXTMETRIC;
typedef STARS_CV_13bd STARS_ALIAS_LPMSG;
typedef STARS_CV_14ee STARS_ALIAS_PWINDOWPLACEMENT;
typedef STARS_CV_13a2 STARS_ALIAS_NPMSG;
typedef STARS_CV_156d STARS_ALIAS__ldiv_t;
typedef STARS_CV_140c STARS_ALIAS__rtxfer;
typedef STARS_CV_1548 STARS_ALIAS_LPHELPWININFO;
typedef STARS_CV_0000 STARS_ALIAS_MIDICALLBACK;
typedef STARS_CV_10e9 STARS_ALIAS_TOK;
typedef STARS_CV_1158 STARS_ALIAS_DV;
typedef STARS_CV_141d STARS_ALIAS_DCB;
typedef STARS_CV_1349 STARS_ALIAS_PRODQ1;
typedef STARS_CV_14e6 STARS_ALIAS_PCOLDROP;
typedef STARS_CV_1279 STARS_ALIAS_tagMEASUREITEMSTRUCT;
typedef STARS_CV_1331 STARS_ALIAS_LPXFERFULL;
typedef STARS_CV_0073 STARS_ALIAS_ATOM;
typedef STARS_CV_1448 STARS_ALIAS__exception;
typedef STARS_CV_136f STARS_ALIAS__exceptionl;
typedef STARS_CV_1038 STARS_ALIAS_FRAMESTUFF;
typedef STARS_CV_1551 STARS_ALIAS__fonexit_t;
typedef STARS_CV_1198 STARS_ALIAS_PLOGFONT;
typedef STARS_CV_0020 STARS_ALIAS_BYTE;
typedef STARS_CV_0072 STARS_ALIAS_BOOL;
typedef STARS_CV_12a8 STARS_ALIAS_BOMB;
typedef STARS_CV_12ac STARS_ALIAS_BEAM;
typedef STARS_CV_14a8 STARS_ALIAS_LPMETARECORD;
typedef STARS_CV_115c STARS_ALIAS_BTNT;
typedef STARS_CV_14b7 STARS_ALIAS_CBTACTIVATESTRUCT;
typedef STARS_CV_14c2 STARS_ALIAS_LPTTPOLYCURVE;
typedef STARS_CV_142d STARS_ALIAS_tagMAT2;
typedef STARS_CV_12a6 STARS_ALIAS_TERRA;
typedef STARS_CV_14f2 STARS_ALIAS_LPNEWTEXTMETRIC;
typedef STARS_CV_14e3 STARS_ALIAS_NPNEWTEXTMETRIC;
typedef STARS_CV_156b STARS_ALIAS_LPBITMAPINFO;
typedef STARS_CV_10c3 STARS_ALIAS_POPUPDATA;
typedef STARS_CV_1491 STARS_ALIAS_tagOUTLINETEXTMETRIC;
typedef STARS_CV_1012 STARS_ALIAS_GAME;
typedef STARS_CV_0073 STARS_ALIAS_WPARAM;
typedef STARS_CV_0073 STARS_ALIAS_HWND;
typedef STARS_CV_0073 STARS_ALIAS_HRGN;
typedef STARS_CV_0073 STARS_ALIAS_HPEN;
typedef STARS_CV_0073 STARS_ALIAS_HSTR;
typedef STARS_CV_0073 STARS_ALIAS_HDWP;
typedef STARS_CV_11bc STARS_ALIAS_LPBITMAPINFOHEADER;
typedef STARS_CV_1366 STARS_ALIAS__planetsome;
typedef STARS_CV_145f STARS_ALIAS_PHANDLETABLE;
typedef STARS_CV_13f3 STARS_ALIAS_KILL;
typedef STARS_CV_1417 STARS_ALIAS_PDRVCONFIGINFO;
typedef STARS_CV_1345 STARS_ALIAS__rtChgProdQ;
typedef STARS_CV_14d5 STARS_ALIAS_LPPRINTDLG;
typedef STARS_CV_142d STARS_ALIAS_MAT2;
typedef STARS_CV_1037 STARS_ALIAS_GOBJENUMPROC;
typedef STARS_CV_14ca STARS_ALIAS_PALETTEENTRY;
typedef STARS_CV_1441 STARS_ALIAS__thpack;
typedef STARS_CV_1140 STARS_ALIAS__aipart;
typedef STARS_CV_127a STARS_ALIAS_LPMEASUREITEMSTRUCT;
typedef STARS_CV_1030 STARS_ALIAS_ENV;
typedef STARS_CV_1039 STARS_ALIAS_PROD;
typedef STARS_CV_151b STARS_ALIAS_NPPATTERN;
typedef STARS_CV_142e STARS_ALIAS_PLOGPEN;
typedef STARS_CV_13a2 STARS_ALIAS_PMSG;
typedef STARS_CV_0172 STARS_ALIAS_PINT;
typedef STARS_CV_0170 STARS_ALIAS_PSTR;
typedef STARS_CV_105e STARS_ALIAS_PART;
typedef STARS_CV_1459 STARS_ALIAS_PRINTDLG;
typedef STARS_CV_1037 STARS_ALIAS_FONTENUMPROC;
typedef STARS_CV_151b STARS_ALIAS_PPATTERN;
typedef STARS_CV_14ca STARS_ALIAS_tagPALETTEENTRY;
typedef STARS_CV_13f7 STARS_ALIAS_LPCLIENTCREATESTRUCT;
typedef STARS_CV_11c5 STARS_ALIAS_BITMAP;
typedef STARS_CV_10a2 STARS_ALIAS_BTLDATA;
typedef STARS_CV_101a STARS_ALIAS_RECT;
typedef STARS_CV_117d STARS_ALIAS_tagOFSTRUCT;
typedef STARS_CV_14f8 STARS_ALIAS_SIZE;
typedef STARS_CV_1047 STARS_ALIAS_SCAN;
typedef STARS_CV_144e STARS_ALIAS_SBAR;
typedef STARS_CV_1415 STARS_ALIAS_COMPART;
typedef STARS_CV_13b6 STARS_ALIAS_TILE;
typedef STARS_CV_148f STARS_ALIAS_tagPANOSE;
typedef STARS_CV_12aa STARS_ALIAS_TORP;
typedef STARS_CV_1037 STARS_ALIAS_PROPENUMPROC;
typedef STARS_CV_0073 STARS_ALIAS_UINT;
typedef STARS_CV_1502 STARS_ALIAS_VERS;
typedef STARS_CV_1457 STARS_ALIAS_CREATESTRUCT;
typedef STARS_CV_0021 STARS_ALIAS_WORD;
typedef STARS_CV_1013 STARS_ALIAS_GDATA;
typedef STARS_CV_1342 STARS_ALIAS_RTHISTHDR;
typedef STARS_CV_0073 STARS_ALIAS_HLOCAL;
typedef STARS_CV_132e STARS_ALIAS_XFER;
typedef STARS_CV_14db STARS_ALIAS_tagDELETEITEMSTRUCT;
typedef STARS_CV_1402 STARS_ALIAS_WINDEBUGINFO;
typedef STARS_CV_14f5 STARS_ALIAS_tagLOGBRUSH;
typedef STARS_CV_0012 STARS_ALIAS_LPARAM;
typedef STARS_CV_1277 STARS_ALIAS_tagDRAWITEMSTRUCT;
typedef STARS_CV_137f STARS_ALIAS_TASKXPORT;
typedef STARS_CV_12b2 STARS_ALIAS_SPECIALSB;
typedef STARS_CV_1451 STARS_ALIAS__rtshipint2;
typedef STARS_CV_1443 STARS_ALIAS__rtshipint;
typedef STARS_CV_14c0 STARS_ALIAS_LPMOUSEHOOKSTRUCT;
typedef STARS_CV_1377 STARS_ALIAS_tagGLYPHMETRICS;
typedef STARS_CV_10ec STARS_ALIAS__ini;
typedef STARS_CV_1076 STARS_ALIAS__hul;
typedef STARS_CV_105b STARS_ALIAS__sel;
typedef STARS_CV_13b2 STARS_ALIAS__obj;
typedef STARS_CV_12d4 STARS_ALIAS__msgplr;
typedef STARS_CV_10e9 STARS_ALIAS__tok;
typedef STARS_CV_148b STARS_ALIAS__lsb;
typedef STARS_CV_13ef STARS_ALIAS__btn;
typedef STARS_CV_12aa STARS_ALIAS__torp;
typedef STARS_CV_10b4 STARS_ALIAS__rpt;
typedef STARS_CV_12ec STARS_ALIAS__hdr;
typedef STARS_CV_1425 STARS_ALIAS_EDITWORDBREAKPROC;
typedef STARS_CV_13fd STARS_ALIAS_FLEETSOME;
typedef STARS_CV_1438 STARS_ALIAS_BTLREC26;
typedef STARS_CV_148f STARS_ALIAS_PANOSE;
typedef STARS_CV_1513 STARS_ALIAS_tagSEGINFO;
typedef STARS_CV_13fd STARS_ALIAS__fleetsome;
typedef STARS_CV_1299 STARS_ALIAS_ARMOR;
typedef STARS_CV_1140 STARS_ALIAS_AIPART;
typedef STARS_CV_1402 STARS_ALIAS_tagWINDEBUGINFO;
typedef STARS_CV_1332 STARS_ALIAS_COLDROP;
typedef STARS_CV_1550 STARS_ALIAS_RTLOGHDR;
typedef STARS_CV_1446 STARS_ALIAS_AISTARBASE;
typedef STARS_CV_141d STARS_ALIAS_tagDCB;
typedef STARS_CV_1441 STARS_ALIAS_THPACK;
typedef STARS_CV_11ab STARS_ALIAS_tagMSG;
typedef STARS_CV_1037 STARS_ALIAS_DLGPROC;
typedef STARS_CV_1206 STARS_ALIAS_OPENFILENAME;
typedef STARS_CV_139a STARS_ALIAS__complexl;
typedef STARS_CV_139d STARS_ALIAS_tagBITMAPFILEHEADER;
typedef STARS_CV_1498 STARS_ALIAS_LPCREATESTRUCT;
typedef STARS_CV_11b9 STARS_ALIAS_tagBITMAPCOREHEADER;
typedef STARS_CV_12f6 STARS_ALIAS_STARPACK;
typedef STARS_CV_14e2 STARS_ALIAS_RTPLANET;
typedef STARS_CV_1330 STARS_ALIAS__xferfull;
typedef STARS_CV_153b STARS_ALIAS_PCOMPAREITEMSTRUCT;
typedef STARS_CV_1037 STARS_ALIAS_FARPROC;
typedef STARS_CV_0022 STARS_ALIAS_COLORREF;
typedef STARS_CV_0073 STARS_ALIAS_HGLOBAL;
typedef STARS_CV_0370 STARS_ALIAS_HPSTR;
typedef STARS_CV_0073 STARS_ALIAS_HDRVR;
typedef STARS_CV_1030 STARS_ALIAS_jmp_buf;
typedef STARS_CV_148d STARS_ALIAS_tagPOINTFX;
typedef STARS_CV_136b STARS_ALIAS_BITMAPCOREINFO;
typedef STARS_CV_1422 STARS_ALIAS_LPDCB;
typedef STARS_CV_0221 STARS_ALIAS_LPWORD;
typedef STARS_CV_0270 STARS_ALIAS_LPSTR;
typedef STARS_CV_1486 STARS_ALIAS__rtchgname;
typedef STARS_CV_1554 STARS_ALIAS_MDPLR;
typedef STARS_CV_11bb STARS_ALIAS_tagBITMAPINFOHEADER;
typedef STARS_CV_1368 STARS_ALIAS_tagRGBTRIPLE;
typedef STARS_CV_0170 STARS_ALIAS_NPSTR;
typedef STARS_CV_1005 STARS_ALIAS_ORDER;
typedef STARS_CV_1502 STARS_ALIAS__vers;
typedef STARS_CV_1535 STARS_ALIAS_WN;
typedef STARS_CV_152f STARS_ALIAS_LPPATTERN;
typedef STARS_CV_0122 STARS_ALIAS_PDWORD;
typedef STARS_CV_1511 STARS_ALIAS_tagMINMAXINFO;
typedef STARS_CV_14b2 STARS_ALIAS_tagNEWTEXTMETRIC;
typedef STARS_CV_1462 STARS_ALIAS_AIHIST;
typedef STARS_CV_1037 STARS_ALIAS_ABORTPROC;
typedef STARS_CV_140f STARS_ALIAS_LPFLEETSOME;
typedef STARS_CV_1037 STARS_ALIAS_MFENUMPROC;
typedef STARS_CV_1488 STARS_ALIAS_PTEXTMETRIC;
typedef STARS_CV_1543 STARS_ALIAS_LPSEGINFO;
typedef STARS_CV_14ea STARS_ALIAS_tagCHOOSECOLOR;
typedef STARS_CV_147b STARS_ALIAS_MDICREATESTRUCT;
typedef STARS_CV_1457 STARS_ALIAS_tagCREATESTRUCT;
typedef STARS_CV_14f0 STARS_ALIAS_tagABC;
typedef STARS_CV_1482 STARS_ALIAS_TIMER;
typedef STARS_CV_145d STARS_ALIAS_THWORM;
typedef STARS_CV_10ed STARS_ALIAS_TUTOR;
typedef STARS_CV_1477 STARS_ALIAS_DRAWCIR;
typedef STARS_CV_13ac STARS_ALIAS_METAFILEPICT;
typedef STARS_CV_14b2 STARS_ALIAS_NEWTEXTMETRIC;
typedef STARS_CV_11db STARS_ALIAS_tagTEXTMETRIC;
typedef STARS_CV_1546 STARS_ALIAS_tagBITMAPINFO;
typedef STARS_CV_0022 STARS_ALIAS_FOURCC;
typedef STARS_CV_149f STARS_ALIAS_tagMOUSEHOOKSTRUCT;
typedef STARS_CV_1278 STARS_ALIAS_LPDRAWITEMSTRUCT;
typedef STARS_CV_13a8 STARS_ALIAS_PWNDCLASS;
typedef STARS_CV_0021 STARS_ALIAS_wchar_t;
typedef STARS_CV_13a5 STARS_ALIAS_WNDPROC;
typedef STARS_CV_144f STARS_ALIAS_LPDOCINFO;
typedef STARS_CV_0073 STARS_ALIAS_HCURSOR;
typedef STARS_CV_0073 STARS_ALIAS_HBRUSH;
typedef STARS_CV_0073 STARS_ALIAS_HRSRC;
typedef STARS_CV_0173 STARS_ALIAS_SPHANDLE;
typedef STARS_CV_1504 STARS_ALIAS_LPABC;
typedef STARS_CV_14d3 STARS_ALIAS_LPRECT;
typedef STARS_CV_1030 STARS_ALIAS_CATCHBUF;
typedef STARS_CV_1400 STARS_ALIAS_tagMETAHEADER;
typedef STARS_CV_12a1 STARS_ALIAS_MINES;
typedef STARS_CV_167c STARS_ALIAS_LPMMIOPROC;
typedef STARS_CV_13f4 STARS_ALIAS_LPMETAFILEPICT;
typedef STARS_CV_101b STARS_ALIAS_NPRECT;
typedef STARS_CV_1385 STARS_ALIAS__tasksell;
typedef STARS_CV_1547 STARS_ALIAS_LPLOGPALETTE;
typedef STARS_CV_11bf STARS_ALIAS_NPLOGPALETTE;
typedef STARS_CV_149a STARS_ALIAS_CBT_CREATEWND;
typedef STARS_CV_132e STARS_ALIAS__xfer;
typedef STARS_CV_109a STARS_ALIAS__scorex;
typedef STARS_CV_1336 STARS_ALIAS__score;
typedef STARS_CV_12a8 STARS_ALIAS__bomb;
typedef STARS_CV_12d6 STARS_ALIAS__msgbig;
typedef STARS_CV_144e STARS_ALIAS__sbar;
typedef STARS_CV_1156 STARS_ALIAS__btlrec;
typedef STARS_CV_12a6 STARS_ALIAS__terra;
typedef STARS_CV_1076 STARS_ALIAS_HUL;
typedef STARS_CV_1078 STARS_ALIAS_HS;
typedef STARS_CV_0073 STARS_ALIAS_HDC;
typedef STARS_CV_114f STARS_ALIAS_HB;
typedef STARS_CV_12ec STARS_ALIAS_HDR;
typedef STARS_CV_13f8 STARS_ALIAS_PBITMAP;
typedef STARS_CV_1040 STARS_ALIAS__cyberinfotemp;
typedef STARS_CV_0073 STARS_ALIAS_HINSTANCE;
typedef STARS_CV_1505 STARS_ALIAS_LPDRIVERINFOSTRUCT;
typedef STARS_CV_135b STARS_ALIAS_RTCHGPLANETLONG;
typedef STARS_CV_10be STARS_ALIAS_BTLPLAN;
typedef STARS_CV_1489 STARS_ALIAS_complex;
typedef STARS_CV_1434 STARS_ALIAS_LPLOGPEN;
typedef STARS_CV_134e STARS_ALIAS_RTLOGTHING;
typedef STARS_CV_13fb STARS_ALIAS_LOGXFERF;
typedef STARS_CV_14c8 STARS_ALIAS_tagTTPOLYGONHEADER;
typedef STARS_CV_0000 STARS_ALIAS_MMIOPROC;
typedef STARS_CV_100b STARS_ALIAS_LPPLANET;
typedef STARS_CV_14c8 STARS_ALIAS_TTPOLYGONHEADER;
typedef STARS_CV_1433 STARS_ALIAS_LPRGBTRIPLE;
typedef STARS_CV_1496 STARS_ALIAS__thtrader;
typedef STARS_CV_12b2 STARS_ALIAS__specialsb;
typedef STARS_CV_14fa STARS_ALIAS_NPOFSTRUCT;
typedef STARS_CV_1377 STARS_ALIAS_GLYPHMETRICS;
typedef STARS_CV_1468 STARS_ALIAS_CHOOSEFONT;
typedef STARS_CV_114b STARS_ALIAS_tagTIMERINFO;
typedef STARS_CV_0073 STARS_ALIAS_HANDLE;
typedef STARS_CV_1333 STARS_ALIAS_LPCOLDROP;
typedef STARS_CV_150a STARS_ALIAS_NCCALCSIZE_PARAMS;
typedef STARS_CV_14ff STARS_ALIAS_MULTIKEYHELP;
typedef STARS_CV_1530 STARS_ALIAS_LPPANOSE;
typedef STARS_CV_0273 STARS_ALIAS_LPHANDLE;
typedef STARS_CV_1475 STARS_ALIAS_LPWINDOWPOS;
typedef STARS_CV_1523 STARS_ALIAS_PBITMAPINFOHEADER;
typedef STARS_CV_12d4 STARS_ALIAS_MSGPLR;
typedef STARS_CV_12ac STARS_ALIAS__beam;
typedef STARS_CV_10ec STARS_ALIAS_INI;
typedef STARS_CV_135c STARS_ALIAS_NPEVENTMSG;
typedef STARS_CV_13a8 STARS_ALIAS_NPWNDCLASS;
typedef STARS_CV_1408 STARS_ALIAS_LPBITMAP;
typedef STARS_CV_1427 STARS_ALIAS_LPDRVCONFIGINFO;
typedef STARS_CV_141e STARS_ALIAS_PFLEETSOME;
typedef STARS_CV_1366 STARS_ALIAS_PLANETSOME;
typedef STARS_CV_129c STARS_ALIAS_SHIELD;
typedef STARS_CV_151b STARS_ALIAS_NPLOGBRUSH;
typedef STARS_CV_0073 STARS_ALIAS_HMETAFILE;
typedef STARS_CV_1532 STARS_ALIAS_PLANETMINIMAL;
typedef STARS_CV_1454 STARS_ALIAS_tagWINDOWPOS;
typedef STARS_CV_149b STARS_ALIAS_LPTEXTMETRIC;
typedef STARS_CV_1488 STARS_ALIAS_NPTEXTMETRIC;
typedef STARS_CV_14a9 STARS_ALIAS_PBITMAPCOREHEADER;
typedef STARS_CV_151c STARS_ALIAS_PXFERFULL;
typedef STARS_CV_103e STARS_ALIAS_CYBERINFO;
typedef STARS_CV_1037 STARS_ALIAS_RSRCHDLRPROC;
typedef STARS_CV_14f5 STARS_ALIAS_LOGBRUSH;
typedef STARS_CV_11db STARS_ALIAS_TEXTMETRIC;
typedef STARS_CV_0073 STARS_ALIAS_HBITMAP;
typedef STARS_CV_13f0 STARS_ALIAS_LPOPENFILENAME;
typedef STARS_CV_14dd STARS_ALIAS_tagHARDWAREHOOKSTRUCT;
typedef STARS_CV_1085 STARS_ALIAS__ziporder;
typedef STARS_CV_1291 STARS_ALIAS__planetary;
typedef STARS_CV_13ae STARS_ALIAS_DEBUGHOOKINFO;
typedef STARS_CV_1550 STARS_ALIAS__rtloghdr;
typedef STARS_CV_1381 STARS_ALIAS__tasklaymines;
typedef STARS_CV_1383 STARS_ALIAS__taskpatrol;
typedef STARS_CV_137f STARS_ALIAS__taskxport;
typedef STARS_CV_13f3 STARS_ALIAS__kill;
typedef STARS_CV_1554 STARS_ALIAS__mdplr;
typedef STARS_CV_1037 STARS_ALIAS_LINEDDAPROC;
typedef STARS_CV_12de STARS_ALIAS__msgturn;
typedef STARS_CV_1054 STARS_ALIAS_PLPROD;
typedef STARS_CV_1277 STARS_ALIAS_DRAWITEMSTRUCT;
typedef STARS_CV_146d STARS_ALIAS_LPHANDLETABLE;
typedef STARS_CV_14b5 STARS_ALIAS_WINDOWPLACEMENT;
typedef STARS_CV_0073 STARS_ALIAS_size_t;
typedef STARS_CV_1477 STARS_ALIAS__drawcir;
typedef STARS_CV_152e STARS_ALIAS_SELSOME;
typedef STARS_CV_142e STARS_ALIAS_NPLOGPEN;
typedef STARS_CV_11be STARS_ALIAS_tagLOGPALETTE;
typedef STARS_CV_0012 STARS_ALIAS_time_t;
typedef STARS_CV_13e6 STARS_ALIAS_DOCINFO;
typedef STARS_CV_10be STARS_ALIAS__btlplan;
typedef STARS_CV_1484 STARS_ALIAS__complex;
typedef STARS_CV_1415 STARS_ALIAS__compart;
typedef STARS_CV_121c STARS_ALIAS_ENGINE;
typedef STARS_CV_13ea STARS_ALIAS_tagDRVCONFIGINFO;
typedef STARS_CV_1354 STARS_ALIAS_tagFINDREPLACE;
typedef STARS_CV_1479 STARS_ALIAS_METARECORD;
typedef STARS_CV_14c6 STARS_ALIAS_PMEASUREITEMSTRUCT;
typedef STARS_CV_117d STARS_ALIAS_OFSTRUCT;
typedef STARS_CV_150b STARS_ALIAS_LPOFSTRUCT;
typedef STARS_CV_152e STARS_ALIAS__selSome;
typedef STARS_CV_14ea STARS_ALIAS_CHOOSECOLOR;
typedef STARS_CV_1566 STARS_ALIAS_PBITMAPINFO;
typedef STARS_CV_0073 STARS_ALIAS_HICON;
typedef STARS_CV_13ec STARS_ALIAS__logxfer;
typedef STARS_CV_0212 STARS_ALIAS_LPLONG;
typedef STARS_CV_12a3 STARS_ALIAS_MINING;
typedef STARS_CV_136c STARS_ALIAS_LPFINDREPLACE;
typedef STARS_CV_13fe STARS_ALIAS_LPGLYPHMETRICS;
typedef STARS_CV_1323 STARS_ALIAS__rtbof;
typedef STARS_CV_1299 STARS_ALIAS__armor;
typedef STARS_CV_156a STARS_ALIAS__div_t;
typedef STARS_CV_145d STARS_ALIAS__thworm;
typedef STARS_CV_10ed STARS_ALIAS__tutor;
typedef STARS_CV_1372 STARS_ALIAS_LPEVENTMSG;
typedef STARS_CV_1020 STARS_ALIAS_PPOINT;
typedef STARS_CV_13b0 STARS_ALIAS_PTHING;
typedef STARS_CV_13af STARS_ALIAS_PPLANET;
typedef STARS_CV_10a2 STARS_ALIAS__btldata;
typedef STARS_CV_1332 STARS_ALIAS__coldrop;
typedef STARS_CV_1491 STARS_ALIAS_OUTLINETEXTMETRIC;
typedef STARS_CV_13bc STARS_ALIAS_LPWNDCLASS;
typedef STARS_CV_13f8 STARS_ALIAS_NPBITMAP;
typedef STARS_CV_14fc STARS_ALIAS__fleetid;
typedef STARS_CV_147b STARS_ALIAS_tagMDICREATESTRUCT;
typedef STARS_CV_1446 STARS_ALIAS__aistarbase;
typedef STARS_CV_1513 STARS_ALIAS_SEGINFO;
typedef STARS_CV_128d STARS_ALIAS_SCANNER;
typedef STARS_CV_152f STARS_ALIAS_LPLOGBRUSH;
typedef STARS_CV_1436 STARS_ALIAS__rtxferf;
typedef STARS_CV_1431 STARS_ALIAS__rtxferl;
typedef STARS_CV_1421 STARS_ALIAS__rtxferx;
typedef STARS_CV_14a0 STARS_ALIAS_PMETARECORD;
typedef STARS_CV_146a STARS_ALIAS__rtwaypt;
typedef STARS_CV_1429 STARS_ALIAS_THMINE;
typedef STARS_CV_10f1 STARS_ALIAS__zipprodq;
typedef STARS_CV_10ef STARS_ALIAS__zipprodq1;
typedef STARS_CV_143e STARS_ALIAS_NEARPROC;
typedef STARS_CV_114e STARS_ALIAS_LPFNNOTIFYCALLBACK;
typedef STARS_CV_0012 STARS_ALIAS_fpos_t;
typedef STARS_CV_129e STARS_ALIAS__special;
typedef STARS_CV_0073 STARS_ALIAS_HMMIO;
typedef STARS_CV_0073 STARS_ALIAS_HGDIOBJ;
typedef STARS_CV_1410 STARS_ALIAS_LPBITMAPFILEHEADER;
typedef STARS_CV_1375 STARS_ALIAS_LPBITMAPCOREINFO;
typedef STARS_CV_128d STARS_ALIAS__scanner;
typedef STARS_CV_1373 STARS_ALIAS_PPLANETSOME;
typedef STARS_CV_1197 STARS_ALIAS_LOGFONT;
typedef STARS_CV_156d STARS_ALIAS_ldiv_t;
typedef STARS_CV_0222 STARS_ALIAS_LPDWORD;
typedef STARS_CV_1551 STARS_ALIAS_onexit_t;
typedef STARS_CV_14cf STARS_ALIAS__rtshdef;
typedef STARS_CV_11bb STARS_ALIAS_BITMAPINFOHEADER;
typedef STARS_CV_1047 STARS_ALIAS__scan;
typedef STARS_CV_1006 STARS_ALIAS__thing;
typedef STARS_CV_1002 STARS_ALIAS__planet;
typedef STARS_CV_148b STARS_ALIAS_LSB;
typedef STARS_CV_1552 STARS_ALIAS_LPPAINTSTRUCT;
typedef STARS_CV_1541 STARS_ALIAS_NPPAINTSTRUCT;
typedef STARS_CV_0073 STARS_ALIAS_HWAVEOUT;
typedef STARS_CV_0272 STARS_ALIAS_LPCATCHBUF;
typedef STARS_CV_13ac STARS_ALIAS_tagMETAFILEPICT;
typedef STARS_CV_0073 STARS_ALIAS_HPALETTE;
typedef STARS_CV_1518 STARS_ALIAS_tagKERNINGPAIR;
typedef STARS_CV_134b STARS_ALIAS_tagEVENTMSG;
typedef STARS_CV_1416 STARS_ALIAS_LPDEVNAMES;
typedef STARS_CV_146a STARS_ALIAS_RTWAYPT;
typedef STARS_CV_1468 STARS_ALIAS_tagCHOOSEFONT;
typedef STARS_CV_14f6 STARS_ALIAS_LPWINDOWPLACEMENT;
typedef STARS_CV_129e STARS_ALIAS_SPECIAL;
typedef STARS_CV_1330 STARS_ALIAS_XFERFULL;
typedef STARS_CV_1466 STARS_ALIAS_LPLOGFONT;
typedef STARS_CV_1542 STARS_ALIAS_LPPLANETMINIMAL;
typedef STARS_CV_11be STARS_ALIAS_LOGPALETTE;
typedef STARS_CV_14f8 STARS_ALIAS_tagSIZE;
typedef STARS_CV_151b STARS_ALIAS_PLOGBRUSH;
typedef STARS_CV_1463 STARS_ALIAS_LPRGBQUAD;
typedef STARS_CV_0170 STARS_ALIAS_va_list;
typedef STARS_CV_0021 STARS_ALIAS_VERSION;
typedef STARS_CV_1381 STARS_ALIAS_TASKLAYMINES;
typedef STARS_CV_1438 STARS_ALIAS__btlrec26;
typedef STARS_CV_0073 STARS_ALIAS_LOCALHANDLE;
typedef STARS_CV_13fb STARS_ALIAS__logxferf;
typedef STARS_CV_1426 STARS_ALIAS_LPFIXED;
typedef STARS_CV_1400 STARS_ALIAS_METAHEADER;
typedef STARS_CV_0203 STARS_ALIAS_LPMIXWAVE;
typedef STARS_CV_156e STARS_ALIAS_LPRASTERIZER_STATUS;
typedef STARS_CV_0073 STARS_ALIAS_HMIDIOUT;
typedef STARS_CV_13f1 STARS_ALIAS_LPDEBUGHOOKINFO;
typedef STARS_CV_13ae STARS_ALIAS_tagDEBUGHOOKINFO;
typedef STARS_CV_12a3 STARS_ALIAS__mining;
typedef STARS_CV_1462 STARS_ALIAS__aihist;
typedef STARS_CV_121c STARS_ALIAS__engine;
typedef STARS_CV_1429 STARS_ALIAS__thmine;
typedef STARS_CV_1014 STARS_ALIAS__player;
typedef STARS_CV_14e3 STARS_ALIAS_PNEWTEXTMETRIC;
typedef STARS_CV_11ab STARS_ALIAS_MSG;
typedef STARS_CV_13e8 STARS_ALIAS_tagDEVNAMES;
typedef STARS_CV_103e STARS_ALIAS__cyberinfo;
typedef STARS_CV_1291 STARS_ALIAS_PLANETARY;
typedef STARS_CV_1037 STARS_ALIAS_GRAYSTRINGPROC;
typedef STARS_CV_1037 STARS_ALIAS_HOOKPROC;
typedef STARS_CV_136b STARS_ALIAS_tagBITMAPCOREINFO;
typedef STARS_CV_0073 STARS_ALIAS_HMIDIIN;
typedef STARS_CV_144b STARS_ALIAS_tagHANDLETABLE;
typedef STARS_CV_1037 STARS_ALIAS_OLDFONTENUMPROC;
typedef STARS_CV_14f3 STARS_ALIAS_LPPOINT;
typedef STARS_CV_1007 STARS_ALIAS_LPTHING;
typedef STARS_CV_0203 STARS_ALIAS_LPVOID;
typedef STARS_CV_14f1 STARS_ALIAS_PDELETEITEMSTRUCT;
typedef STARS_CV_12d6 STARS_ALIAS_MSGBIG;
typedef STARS_CV_1020 STARS_ALIAS_NPPOINT;
typedef STARS_CV_12e0 STARS_ALIAS__msghdr;
typedef STARS_CV_150a STARS_ALIAS_tagNCCALCSIZE_PARAMS;
typedef STARS_CV_1506 STARS_ALIAS_LPCHOOSECOLOR;
typedef STARS_CV_10c3 STARS_ALIAS__popupdata;
typedef STARS_CV_167f STARS_ALIAS_YIELDPROC;
typedef STARS_CV_1682 STARS_ALIAS_LPTIMECALLBACK;
typedef STARS_CV_14ed STARS_ALIAS_LPPALETTEENTRY;
typedef STARS_CV_1198 STARS_ALIAS_NPLOGFONT;
typedef STARS_CV_1383 STARS_ALIAS_TASKPATROL;
typedef STARS_CV_14f9 STARS_ALIAS_LPDELETEITEMSTRUCT;
typedef STARS_CV_13a4 STARS_ALIAS_tagCLIENTCREATESTRUCT;
typedef STARS_CV_10ef STARS_ALIAS_ZIPPRODQ1;
typedef STARS_CV_14dd STARS_ALIAS_HARDWAREHOOKSTRUCT;
typedef STARS_CV_144b STARS_ALIAS_HANDLETABLE;
typedef STARS_CV_11ba STARS_ALIAS_LPBITMAPCOREHEADER;
typedef STARS_CV_151f STARS_ALIAS_ENUMLOGFONT;
typedef STARS_CV_1038 STARS_ALIAS__framestuff;
typedef STARS_CV_1197 STARS_ALIAS_tagLOGFONT;
typedef STARS_CV_1407 STARS_ALIAS_tagLOGPEN;
typedef STARS_CV_1551 STARS_ALIAS__onexit_t;
typedef STARS_CV_1519 STARS_ALIAS_LPSIZE;
typedef STARS_CV_14b5 STARS_ALIAS_tagWINDOWPLACEMENT;
typedef STARS_CV_1518 STARS_ALIAS_KERNINGPAIR;
typedef STARS_CV_1507 STARS_ALIAS_NPSIZE;
typedef STARS_CV_1170 STARS_ALIAS__diskfree_t;
typedef STARS_CV_1549 STARS_ALIAS_LPCOMPAREITEMSTRUCT;
typedef STARS_CV_1535 STARS_ALIAS__wn;
typedef STARS_CV_1078 STARS_ALIAS__hs;
typedef STARS_CV_1158 STARS_ALIAS__dv;
typedef STARS_CV_13b2 STARS_ALIAS_OBJ;
typedef STARS_CV_114f STARS_ALIAS__hb;
typedef STARS_CV_125c STARS_ALIAS__pl;
typedef STARS_CV_1540 STARS_ALIAS_DRIVERPROC;
typedef STARS_CV_1405 STARS_ALIAS_PBITMAPFILEHEADER;
typedef STARS_CV_0012 STARS_ALIAS_clock_t;
typedef STARS_CV_1511 STARS_ALIAS_MINMAXINFO;
typedef STARS_CV_1085 STARS_ALIAS_ZIPORDER;
typedef STARS_CV_10f1 STARS_ALIAS_ZIPPRODQ;
typedef STARS_CV_0022 STARS_ALIAS_HHOOK;
typedef STARS_CV_0073 STARS_ALIAS_HTASK;
typedef STARS_CV_14e2 STARS_ALIAS__rtplanet;
typedef STARS_CV_14ac STARS_ALIAS_LPCHOOSEFONT;
typedef STARS_CV_14d9 STARS_ALIAS__rtchgshdef;
typedef STARS_CV_154e STARS_ALIAS_PPLANETMINIMAL;
typedef STARS_CV_1532 STARS_ALIAS__planetminimal;
typedef STARS_CV_1342 STARS_ALIAS__rthisthdr;
typedef STARS_CV_1541 STARS_ALIAS_PPAINTSTRUCT;
typedef STARS_CV_11b9 STARS_ALIAS_BITMAPCOREHEADER;
STARS_CHECK(t135d,sizeof(STARS_CV_135d)==2);
STARS_CHECK(t135e,sizeof(STARS_CV_135e)==2);
STARS_CHECK(t135f,sizeof(STARS_CV_135f)==2);
STARS_CHECK(t1360,sizeof(STARS_CV_1360)==2);
STARS_CHECK(t1361,sizeof(STARS_CV_1361)==2);
STARS_CHECK(t137b,sizeof(STARS_CV_137b)==2);
STARS_CHECK(t137c,sizeof(STARS_CV_137c)==2);
STARS_CHECK(t137d,sizeof(STARS_CV_137d)==2);
STARS_CHECK(t1388,sizeof(STARS_CV_1388)==3);
STARS_CHECK(t10bc,sizeof(STARS_CV_10bc)==3);
STARS_CHECK(t1364,sizeof(STARS_CV_1364)==2);
STARS_CHECK(t1362,sizeof(STARS_CV_1362)==2);
STARS_CHECK(t1389,sizeof(STARS_CV_1389)==4);
STARS_CHECK(t138a,sizeof(STARS_CV_138a)==4);
STARS_CHECK(t138b,sizeof(STARS_CV_138b)==4);
STARS_CHECK(t138c,sizeof(STARS_CV_138c)==4);
STARS_CHECK(t138d,sizeof(STARS_CV_138d)==4);
STARS_CHECK(t138e,sizeof(STARS_CV_138e)==4);
STARS_CHECK(t138f,sizeof(STARS_CV_138f)==4);
STARS_CHECK(t1390,sizeof(STARS_CV_1390)==4);
STARS_CHECK(t1391,sizeof(STARS_CV_1391)==4);
STARS_CHECK(t1062,sizeof(STARS_CV_1062)==8);
STARS_CHECK(t100c,sizeof(STARS_CV_100c)==16);
STARS_CHECK(t1378,sizeof(STARS_CV_1378)==2);
STARS_CHECK(t1392,sizeof(STARS_CV_1392)==2);
STARS_CHECK(t1393,sizeof(STARS_CV_1393)==2);
STARS_CHECK(t1394,sizeof(STARS_CV_1394)==2);
STARS_CHECK(t1395,sizeof(STARS_CV_1395)==2);
STARS_CHECK(t1396,sizeof(STARS_CV_1396)==2);
STARS_CHECK(t1397,sizeof(STARS_CV_1397)==2);
STARS_CHECK(t1055,sizeof(STARS_CV_1055)==4);
STARS_CHECK(t1002,sizeof(STARS_CV_1002)==56);
STARS_CHECK(t13d4,sizeof(STARS_CV_13d4)==2);
STARS_CHECK(t13cf,sizeof(STARS_CV_13cf)==2);
STARS_CHECK(t13d0,sizeof(STARS_CV_13d0)==2);
STARS_CHECK(t1018,sizeof(STARS_CV_1018)==4);
STARS_CHECK(t13f9,sizeof(STARS_CV_13f9)==32);
STARS_CHECK(t14e0,sizeof(STARS_CV_14e0)==2);
STARS_CHECK(t1514,sizeof(STARS_CV_1514)==2);
STARS_CHECK(t1158,sizeof(STARS_CV_1158)==2);
STARS_CHECK(t1525,sizeof(STARS_CV_1525)==32);
STARS_CHECK(t108f,sizeof(STARS_CV_108f)==20);
STARS_CHECK(t1529,sizeof(STARS_CV_1529)==4);
STARS_CHECK(t1004,sizeof(STARS_CV_1004)==4);
STARS_CHECK(t1411,sizeof(STARS_CV_1411)==2);
STARS_CHECK(t13db,sizeof(STARS_CV_13db)==2);
STARS_CHECK(t14ba,sizeof(STARS_CV_14ba)==2);
STARS_CHECK(t14bb,sizeof(STARS_CV_14bb)==2);
STARS_CHECK(t13b5,sizeof(STARS_CV_13b5)==2);
STARS_CHECK(t1003,sizeof(STARS_CV_1003)==124);
STARS_CHECK(t1379,sizeof(STARS_CV_1379)==2);
STARS_CHECK(t137a,sizeof(STARS_CV_137a)==2);
STARS_CHECK(t10cb,sizeof(STARS_CV_10cb)==2);
STARS_CHECK(t10ee,sizeof(STARS_CV_10ee)==10);
STARS_CHECK(t137f,sizeof(STARS_CV_137f)==10);
STARS_CHECK(t1381,sizeof(STARS_CV_1381)==4);
STARS_CHECK(t1383,sizeof(STARS_CV_1383)==4);
STARS_CHECK(t1385,sizeof(STARS_CV_1385)==2);
STARS_CHECK(t1005,sizeof(STARS_CV_1005)==18);
STARS_CHECK(t105d,sizeof(STARS_CV_105d)==10);
STARS_CHECK(t1429,sizeof(STARS_CV_1429)==10);
STARS_CHECK(t1044,sizeof(STARS_CV_1044)==6);
STARS_CHECK(t143f,sizeof(STARS_CV_143f)==2);
STARS_CHECK(t1441,sizeof(STARS_CV_1441)==10);
STARS_CHECK(t145a,sizeof(STARS_CV_145a)==2);
STARS_CHECK(t145b,sizeof(STARS_CV_145b)==2);
STARS_CHECK(t145d,sizeof(STARS_CV_145d)==8);
STARS_CHECK(t13dc,sizeof(STARS_CV_13dc)==2);
STARS_CHECK(t1496,sizeof(STARS_CV_1496)==10);
STARS_CHECK(t1006,sizeof(STARS_CV_1006)==18);
STARS_CHECK(t1007,sizeof(STARS_CV_1007)==4);
STARS_CHECK(t1270,sizeof(STARS_CV_1270)==6);
STARS_CHECK(t11e2,sizeof(STARS_CV_11e2)==32);
STARS_CHECK(t1363,sizeof(STARS_CV_1363)==6);
STARS_CHECK(t1078,sizeof(STARS_CV_1078)==4);
STARS_CHECK(t1537,sizeof(STARS_CV_1537)==64);
STARS_CHECK(t1076,sizeof(STARS_CV_1076)==123);
STARS_CHECK(t146b,sizeof(STARS_CV_146b)==2);
STARS_CHECK(t1008,sizeof(STARS_CV_1008)==147);
STARS_CHECK(t1009,sizeof(STARS_CV_1009)==4);
STARS_CHECK(t100a,sizeof(STARS_CV_100a)==12);
STARS_CHECK(t100b,sizeof(STARS_CV_100b)==4);
STARS_CHECK(t100d,sizeof(STARS_CV_100d)==32);
STARS_CHECK(t100e,sizeof(STARS_CV_100e)==6);
STARS_CHECK(t100f,sizeof(STARS_CV_100f)==13);
STARS_CHECK(t1010,sizeof(STARS_CV_1010)==4);
STARS_CHECK(t13d7,sizeof(STARS_CV_13d7)==2);
STARS_CHECK(t13d8,sizeof(STARS_CV_13d8)==2);
STARS_CHECK(t13d9,sizeof(STARS_CV_13d9)==2);
STARS_CHECK(t13da,sizeof(STARS_CV_13da)==2);
STARS_CHECK(t13a9,sizeof(STARS_CV_13a9)==2);
STARS_CHECK(t14cb,sizeof(STARS_CV_14cb)==12);
STARS_CHECK(t1012,sizeof(STARS_CV_1012)==64);
STARS_CHECK(t13bf,sizeof(STARS_CV_13bf)==2);
STARS_CHECK(t13c0,sizeof(STARS_CV_13c0)==2);
STARS_CHECK(t13c1,sizeof(STARS_CV_13c1)==2);
STARS_CHECK(t13c2,sizeof(STARS_CV_13c2)==2);
STARS_CHECK(t13c3,sizeof(STARS_CV_13c3)==2);
STARS_CHECK(t13c4,sizeof(STARS_CV_13c4)==2);
STARS_CHECK(t146f,sizeof(STARS_CV_146f)==2);
STARS_CHECK(t141a,sizeof(STARS_CV_141a)==2);
STARS_CHECK(t141b,sizeof(STARS_CV_141b)==2);
STARS_CHECK(t13c6,sizeof(STARS_CV_13c6)==2);
STARS_CHECK(t13c7,sizeof(STARS_CV_13c7)==2);
STARS_CHECK(t13c8,sizeof(STARS_CV_13c8)==2);
STARS_CHECK(t13c9,sizeof(STARS_CV_13c9)==2);
STARS_CHECK(t13ca,sizeof(STARS_CV_13ca)==2);
STARS_CHECK(t1470,sizeof(STARS_CV_1470)==2);
STARS_CHECK(t13cb,sizeof(STARS_CV_13cb)==2);
STARS_CHECK(t13cc,sizeof(STARS_CV_13cc)==2);
STARS_CHECK(t1471,sizeof(STARS_CV_1471)==2);
STARS_CHECK(t1472,sizeof(STARS_CV_1472)==2);
STARS_CHECK(t1473,sizeof(STARS_CV_1473)==2);
STARS_CHECK(t1013,sizeof(STARS_CV_1013)==10);
STARS_CHECK(t13b3,sizeof(STARS_CV_13b3)==2);
STARS_CHECK(t13d2,sizeof(STARS_CV_13d2)==2);
STARS_CHECK(t13d3,sizeof(STARS_CV_13d3)==2);
STARS_CHECK(t13d5,sizeof(STARS_CV_13d5)==2);
STARS_CHECK(t13d6,sizeof(STARS_CV_13d6)==24);
STARS_CHECK(t1209,sizeof(STARS_CV_1209)==16);
STARS_CHECK(t1346,sizeof(STARS_CV_1346)==2);
STARS_CHECK(t1347,sizeof(STARS_CV_1347)==2);
STARS_CHECK(t1349,sizeof(STARS_CV_1349)==2);
STARS_CHECK(t134f,sizeof(STARS_CV_134f)==24);
STARS_CHECK(t10ef,sizeof(STARS_CV_10ef)==26);
STARS_CHECK(t1014,sizeof(STARS_CV_1014)==192);
STARS_CHECK(t101a,sizeof(STARS_CV_101a)==8);
STARS_CHECK(t103d,sizeof(STARS_CV_103d)==16);
STARS_CHECK(t1019,sizeof(STARS_CV_1019)==32);
STARS_CHECK(t101b,sizeof(STARS_CV_101b)==2);
STARS_CHECK(t1020,sizeof(STARS_CV_1020)==2);
STARS_CHECK(t1026,sizeof(STARS_CV_1026)==24);
STARS_CHECK(t1027,sizeof(STARS_CV_1027)==48);
STARS_CHECK(t1028,sizeof(STARS_CV_1028)==40);
STARS_CHECK(t1029,sizeof(STARS_CV_1029)==80);
STARS_CHECK(t102a,sizeof(STARS_CV_102a)==18);
STARS_CHECK(t102b,sizeof(STARS_CV_102b)==36);
STARS_CHECK(t1030,sizeof(STARS_CV_1030)==18);
STARS_CHECK(t1034,sizeof(STARS_CV_1034)==29);
STARS_CHECK(t1037,sizeof(STARS_CV_1037)==4);
STARS_CHECK(t1038,sizeof(STARS_CV_1038)==22);
STARS_CHECK(t139e,sizeof(STARS_CV_139e)==4);
STARS_CHECK(t1555,sizeof(STARS_CV_1555)==4);
STARS_CHECK(t1556,sizeof(STARS_CV_1556)==4);
STARS_CHECK(t1557,sizeof(STARS_CV_1557)==4);
STARS_CHECK(t1558,sizeof(STARS_CV_1558)==4);
STARS_CHECK(t1039,sizeof(STARS_CV_1039)==4);
STARS_CHECK(t103a,sizeof(STARS_CV_103a)==2);
STARS_CHECK(t147c,sizeof(STARS_CV_147c)==2);
STARS_CHECK(t103e,sizeof(STARS_CV_103e)==2);
STARS_CHECK(t103f,sizeof(STARS_CV_103f)==4);
STARS_CHECK(t14b8,sizeof(STARS_CV_14b8)==2);
STARS_CHECK(t14b9,sizeof(STARS_CV_14b9)==2);
STARS_CHECK(t14bc,sizeof(STARS_CV_14bc)==2);
STARS_CHECK(t1040,sizeof(STARS_CV_1040)==2);
STARS_CHECK(t1041,sizeof(STARS_CV_1041)==4);
STARS_CHECK(t1043,sizeof(STARS_CV_1043)==256);
STARS_CHECK(t1047,sizeof(STARS_CV_1047)==16);
STARS_CHECK(t104a,sizeof(STARS_CV_104a)==4);
STARS_CHECK(t1053,sizeof(STARS_CV_1053)==4);
STARS_CHECK(t1054,sizeof(STARS_CV_1054)==4);
STARS_CHECK(t1056,sizeof(STARS_CV_1056)==4);
STARS_CHECK(t1058,sizeof(STARS_CV_1058)==42);
STARS_CHECK(t1059,sizeof(STARS_CV_1059)==72);
STARS_CHECK(t105a,sizeof(STARS_CV_105a)==301);
STARS_CHECK(t105b,sizeof(STARS_CV_105b)==226);
STARS_CHECK(t105c,sizeof(STARS_CV_105c)==8);
STARS_CHECK(t155a,sizeof(STARS_CV_155a)==4);
STARS_CHECK(t155b,sizeof(STARS_CV_155b)==4);
STARS_CHECK(t1077,sizeof(STARS_CV_1077)==4);
STARS_CHECK(t121d,sizeof(STARS_CV_121d)==4);
STARS_CHECK(t128e,sizeof(STARS_CV_128e)==4);
STARS_CHECK(t155c,sizeof(STARS_CV_155c)==4);
STARS_CHECK(t155d,sizeof(STARS_CV_155d)==4);
STARS_CHECK(t155e,sizeof(STARS_CV_155e)==4);
STARS_CHECK(t155f,sizeof(STARS_CV_155f)==4);
STARS_CHECK(t1560,sizeof(STARS_CV_1560)==4);
STARS_CHECK(t1561,sizeof(STARS_CV_1561)==4);
STARS_CHECK(t1562,sizeof(STARS_CV_1562)==4);
STARS_CHECK(t1563,sizeof(STARS_CV_1563)==4);
STARS_CHECK(t1292,sizeof(STARS_CV_1292)==4);
STARS_CHECK(t1564,sizeof(STARS_CV_1564)==4);
STARS_CHECK(t105e,sizeof(STARS_CV_105e)==8);
STARS_CHECK(t1063,sizeof(STARS_CV_1063)==62);
STARS_CHECK(t1064,sizeof(STARS_CV_1064)==248);
STARS_CHECK(t1067,sizeof(STARS_CV_1067)==18);
STARS_CHECK(t1068,sizeof(STARS_CV_1068)==19);
STARS_CHECK(t1069,sizeof(STARS_CV_1069)==182);
STARS_CHECK(t1072,sizeof(STARS_CV_1072)==4);
STARS_CHECK(t1079,sizeof(STARS_CV_1079)==4);
STARS_CHECK(t1080,sizeof(STARS_CV_1080)==80);
STARS_CHECK(t116c,sizeof(STARS_CV_116c)==13);
STARS_CHECK(t1085,sizeof(STARS_CV_1085)==24);
STARS_CHECK(t1086,sizeof(STARS_CV_1086)==96);
STARS_CHECK(t108e,sizeof(STARS_CV_108e)==10);
STARS_CHECK(t1096,sizeof(STARS_CV_1096)==40);
STARS_CHECK(t1099,sizeof(STARS_CV_1099)==100);
STARS_CHECK(t14c3,sizeof(STARS_CV_14c3)==2);
STARS_CHECK(t14c4,sizeof(STARS_CV_14c4)==2);
STARS_CHECK(t1336,sizeof(STARS_CV_1336)==20);
STARS_CHECK(t109a,sizeof(STARS_CV_109a)==24);
STARS_CHECK(t109b,sizeof(STARS_CV_109b)==4);
STARS_CHECK(t10a2,sizeof(STARS_CV_10a2)==14);
STARS_CHECK(t10a3,sizeof(STARS_CV_10a3)==4);
STARS_CHECK(t10ab,sizeof(STARS_CV_10ab)==2048);
STARS_CHECK(t10b0,sizeof(STARS_CV_10b0)==50);
STARS_CHECK(t10b1,sizeof(STARS_CV_10b1)==1600);
STARS_CHECK(t10b2,sizeof(STARS_CV_10b2)==64);
STARS_CHECK(t10b3,sizeof(STARS_CV_10b3)==64);
STARS_CHECK(t10b4,sizeof(STARS_CV_10b4)==54);
STARS_CHECK(t10b5,sizeof(STARS_CV_10b5)==2);
STARS_CHECK(t10b8,sizeof(STARS_CV_10b8)==2);
STARS_CHECK(t10b9,sizeof(STARS_CV_10b9)==256);
STARS_CHECK(t10ba,sizeof(STARS_CV_10ba)==64);
STARS_CHECK(t10bb,sizeof(STARS_CV_10bb)==64);
STARS_CHECK(t1492,sizeof(STARS_CV_1492)==2);
STARS_CHECK(t1493,sizeof(STARS_CV_1493)==2);
STARS_CHECK(t11df,sizeof(STARS_CV_11df)==32);
STARS_CHECK(t10be,sizeof(STARS_CV_10be)==36);
STARS_CHECK(t10bf,sizeof(STARS_CV_10bf)==4);
STARS_CHECK(t10c0,sizeof(STARS_CV_10c0)==64);
STARS_CHECK(t10c1,sizeof(STARS_CV_10c1)==24);
STARS_CHECK(t10c2,sizeof(STARS_CV_10c2)==4);
STARS_CHECK(t10c3,sizeof(STARS_CV_10c3)==22);
STARS_CHECK(t10c6,sizeof(STARS_CV_10c6)==22323);
STARS_CHECK(t10c7,sizeof(STARS_CV_10c7)==640);
STARS_CHECK(t10c8,sizeof(STARS_CV_10c8)==20);
STARS_CHECK(t10c9,sizeof(STARS_CV_10c9)==76);
STARS_CHECK(t10cc,sizeof(STARS_CV_10cc)==4);
STARS_CHECK(t10cf,sizeof(STARS_CV_10cf)==2);
STARS_CHECK(t10d0,sizeof(STARS_CV_10d0)==33);
STARS_CHECK(t10e5,sizeof(STARS_CV_10e5)==2);
STARS_CHECK(t14d0,sizeof(STARS_CV_14d0)==2);
STARS_CHECK(t10e9,sizeof(STARS_CV_10e9)==29);
STARS_CHECK(t10ea,sizeof(STARS_CV_10ea)==4);
STARS_CHECK(t10eb,sizeof(STARS_CV_10eb)==6);
STARS_CHECK(t1533,sizeof(STARS_CV_1533)==2);
STARS_CHECK(t1535,sizeof(STARS_CV_1535)==10);
STARS_CHECK(t154a,sizeof(STARS_CV_154a)==2);
STARS_CHECK(t10ec,sizeof(STARS_CV_10ec)==26);
STARS_CHECK(t13c5,sizeof(STARS_CV_13c5)==2);
STARS_CHECK(t10ed,sizeof(STARS_CV_10ed)==44);
STARS_CHECK(t10f0,sizeof(STARS_CV_10f0)==52);
STARS_CHECK(t10f1,sizeof(STARS_CV_10f1)==40);
STARS_CHECK(t10f2,sizeof(STARS_CV_10f2)==200);
STARS_CHECK(t10f4,sizeof(STARS_CV_10f4)==28209);
STARS_CHECK(t10f5,sizeof(STARS_CV_10f5)==1414);
STARS_CHECK(t10f6,sizeof(STARS_CV_10f6)==46);
STARS_CHECK(t10f7,sizeof(STARS_CV_10f7)==84);
STARS_CHECK(t10fa,sizeof(STARS_CV_10fa)==2);
STARS_CHECK(t10fd,sizeof(STARS_CV_10fd)==2);
STARS_CHECK(t1100,sizeof(STARS_CV_1100)==8);
STARS_CHECK(t1107,sizeof(STARS_CV_1107)==2);
STARS_CHECK(t1114,sizeof(STARS_CV_1114)==2);
STARS_CHECK(t111b,sizeof(STARS_CV_111b)==400);
STARS_CHECK(t1120,sizeof(STARS_CV_1120)==128);
STARS_CHECK(t1123,sizeof(STARS_CV_1123)==4);
STARS_CHECK(t1126,sizeof(STARS_CV_1126)==4);
STARS_CHECK(t113e,sizeof(STARS_CV_113e)==85);
STARS_CHECK(t113f,sizeof(STARS_CV_113f)==45);
STARS_CHECK(t13ce,sizeof(STARS_CV_13ce)==2);
STARS_CHECK(t1140,sizeof(STARS_CV_1140)==2);
STARS_CHECK(t1141,sizeof(STARS_CV_1141)==300);
STARS_CHECK(t1142,sizeof(STARS_CV_1142)==8);
STARS_CHECK(t1145,sizeof(STARS_CV_1145)==4);
STARS_CHECK(t1146,sizeof(STARS_CV_1146)==31);
STARS_CHECK(t1147,sizeof(STARS_CV_1147)==76);
STARS_CHECK(t1148,sizeof(STARS_CV_1148)==36);
STARS_CHECK(t1149,sizeof(STARS_CV_1149)==141);
STARS_CHECK(t114b,sizeof(STARS_CV_114b)==12);
STARS_CHECK(t114e,sizeof(STARS_CV_114e)==4);
STARS_CHECK(t1150,sizeof(STARS_CV_1150)==4);
STARS_CHECK(t114f,sizeof(STARS_CV_114f)==16);
STARS_CHECK(t1156,sizeof(STARS_CV_1156)==6);
STARS_CHECK(t1157,sizeof(STARS_CV_1157)==4);
STARS_CHECK(t1159,sizeof(STARS_CV_1159)==4);
STARS_CHECK(t115c,sizeof(STARS_CV_115c)==24);
STARS_CHECK(t115d,sizeof(STARS_CV_115d)==2);
STARS_CHECK(t1162,sizeof(STARS_CV_1162)==256);
STARS_CHECK(t1163,sizeof(STARS_CV_1163)==96);
STARS_CHECK(t1166,sizeof(STARS_CV_1166)==80);
STARS_CHECK(t1167,sizeof(STARS_CV_1167)==1536);
STARS_CHECK(t1168,sizeof(STARS_CV_1168)==80);
STARS_CHECK(t1169,sizeof(STARS_CV_1169)==16);
STARS_CHECK(t116b,sizeof(STARS_CV_116b)==21);
STARS_CHECK(t116e,sizeof(STARS_CV_116e)==44);
STARS_CHECK(t1170,sizeof(STARS_CV_1170)==8);
STARS_CHECK(t12ce,sizeof(STARS_CV_12ce)==128);
STARS_CHECK(t117d,sizeof(STARS_CV_117d)==136);
STARS_CHECK(t117e,sizeof(STARS_CV_117e)==2048);
STARS_CHECK(t1187,sizeof(STARS_CV_1187)==15);
STARS_CHECK(t118e,sizeof(STARS_CV_118e)==2);
STARS_CHECK(t1197,sizeof(STARS_CV_1197)==50);
STARS_CHECK(t1198,sizeof(STARS_CV_1198)==2);
STARS_CHECK(t11a3,sizeof(STARS_CV_11a3)==2);
STARS_CHECK(t11aa,sizeof(STARS_CV_11aa)==24);
STARS_CHECK(t11ab,sizeof(STARS_CV_11ab)==18);
STARS_CHECK(t11b1,sizeof(STARS_CV_11b1)==1024);
STARS_CHECK(t11b9,sizeof(STARS_CV_11b9)==12);
STARS_CHECK(t11ba,sizeof(STARS_CV_11ba)==4);
STARS_CHECK(t11bb,sizeof(STARS_CV_11bb)==40);
STARS_CHECK(t11bc,sizeof(STARS_CV_11bc)==4);
STARS_CHECK(t14ca,sizeof(STARS_CV_14ca)==4);
STARS_CHECK(t150c,sizeof(STARS_CV_150c)==4);
STARS_CHECK(t11be,sizeof(STARS_CV_11be)==8);
STARS_CHECK(t11bf,sizeof(STARS_CV_11bf)==2);
STARS_CHECK(t11c5,sizeof(STARS_CV_11c5)==14);
STARS_CHECK(t11ce,sizeof(STARS_CV_11ce)==60);
STARS_CHECK(t11cf,sizeof(STARS_CV_11cf)==20);
STARS_CHECK(t11d1,sizeof(STARS_CV_11d1)==8);
STARS_CHECK(t11d4,sizeof(STARS_CV_11d4)==2);
STARS_CHECK(t11db,sizeof(STARS_CV_11db)==31);
STARS_CHECK(t11dc,sizeof(STARS_CV_11dc)==256);
STARS_CHECK(t11dd,sizeof(STARS_CV_11dd)==64);
STARS_CHECK(t11de,sizeof(STARS_CV_11de)==128);
STARS_CHECK(t11e0,sizeof(STARS_CV_11e0)==12);
STARS_CHECK(t11e1,sizeof(STARS_CV_11e1)==17);
STARS_CHECK(t11e3,sizeof(STARS_CV_11e3)==128);
STARS_CHECK(t11e4,sizeof(STARS_CV_11e4)==52);
STARS_CHECK(t11e5,sizeof(STARS_CV_11e5)==11);
STARS_CHECK(t11e6,sizeof(STARS_CV_11e6)==4099);
STARS_CHECK(t11e7,sizeof(STARS_CV_11e7)==999);
STARS_CHECK(t11e8,sizeof(STARS_CV_11e8)==52);
STARS_CHECK(t11e9,sizeof(STARS_CV_11e9)==19);
STARS_CHECK(t11ea,sizeof(STARS_CV_11ea)==32);
STARS_CHECK(t11eb,sizeof(STARS_CV_11eb)==20);
STARS_CHECK(t11ec,sizeof(STARS_CV_11ec)==12);
STARS_CHECK(t11ee,sizeof(STARS_CV_11ee)==136);
STARS_CHECK(t11ef,sizeof(STARS_CV_11ef)==2);
STARS_CHECK(t11f0,sizeof(STARS_CV_11f0)==2);
STARS_CHECK(t11fc,sizeof(STARS_CV_11fc)==600);
STARS_CHECK(t1352,sizeof(STARS_CV_1352)==4);
STARS_CHECK(t1206,sizeof(STARS_CV_1206)==72);
STARS_CHECK(t1207,sizeof(STARS_CV_1207)==7);
STARS_CHECK(t1208,sizeof(STARS_CV_1208)==1344);
STARS_CHECK(t120a,sizeof(STARS_CV_120a)==4);
STARS_CHECK(t120b,sizeof(STARS_CV_120b)==12);
STARS_CHECK(t120c,sizeof(STARS_CV_120c)==10);
STARS_CHECK(t120d,sizeof(STARS_CV_120d)==4);
STARS_CHECK(t120e,sizeof(STARS_CV_120e)==28);
STARS_CHECK(t120f,sizeof(STARS_CV_120f)==12);
STARS_CHECK(t1219,sizeof(STARS_CV_1219)==64);
STARS_CHECK(t1439,sizeof(STARS_CV_1439)==24);
STARS_CHECK(t121c,sizeof(STARS_CV_121c)==78);
STARS_CHECK(t1222,sizeof(STARS_CV_1222)==32);
STARS_CHECK(t123a,sizeof(STARS_CV_123a)==36);
STARS_CHECK(t123b,sizeof(STARS_CV_123b)==8);
STARS_CHECK(t1242,sizeof(STARS_CV_1242)==2);
STARS_CHECK(t124b,sizeof(STARS_CV_124b)==64);
STARS_CHECK(t124c,sizeof(STARS_CV_124c)==512);
STARS_CHECK(t1251,sizeof(STARS_CV_1251)==136);
STARS_CHECK(t125c,sizeof(STARS_CV_125c)==4);
STARS_CHECK(t125d,sizeof(STARS_CV_125d)==4);
STARS_CHECK(t1264,sizeof(STARS_CV_1264)==24);
STARS_CHECK(t1265,sizeof(STARS_CV_1265)==48);
STARS_CHECK(t126b,sizeof(STARS_CV_126b)==2);
STARS_CHECK(t1273,sizeof(STARS_CV_1273)==14);
STARS_CHECK(t1274,sizeof(STARS_CV_1274)==108);
STARS_CHECK(t1275,sizeof(STARS_CV_1275)==32);
STARS_CHECK(t1276,sizeof(STARS_CV_1276)==34);
STARS_CHECK(t1277,sizeof(STARS_CV_1277)==26);
STARS_CHECK(t1278,sizeof(STARS_CV_1278)==4);
STARS_CHECK(t1279,sizeof(STARS_CV_1279)==14);
STARS_CHECK(t127a,sizeof(STARS_CV_127a)==4);
STARS_CHECK(t127b,sizeof(STARS_CV_127b)==12);
STARS_CHECK(t127c,sizeof(STARS_CV_127c)==160);
STARS_CHECK(t127f,sizeof(STARS_CV_127f)==86);
STARS_CHECK(t1286,sizeof(STARS_CV_1286)==2);
STARS_CHECK(t13f5,sizeof(STARS_CV_13f5)==2);
STARS_CHECK(t1289,sizeof(STARS_CV_1289)==143);
STARS_CHECK(t128a,sizeof(STARS_CV_128a)==4);
STARS_CHECK(t128d,sizeof(STARS_CV_128d)==56);
STARS_CHECK(t1291,sizeof(STARS_CV_1291)==54);
STARS_CHECK(t1298,sizeof(STARS_CV_1298)==1248);
STARS_CHECK(t1299,sizeof(STARS_CV_1299)==54);
STARS_CHECK(t129a,sizeof(STARS_CV_129a)==648);
STARS_CHECK(t129b,sizeof(STARS_CV_129b)==896);
STARS_CHECK(t129c,sizeof(STARS_CV_129c)==54);
STARS_CHECK(t129d,sizeof(STARS_CV_129d)==540);
STARS_CHECK(t129e,sizeof(STARS_CV_129e)==54);
STARS_CHECK(t129f,sizeof(STARS_CV_129f)==918);
STARS_CHECK(t12a0,sizeof(STARS_CV_12a0)==594);
STARS_CHECK(t12a1,sizeof(STARS_CV_12a1)==54);
STARS_CHECK(t12a2,sizeof(STARS_CV_12a2)==540);
STARS_CHECK(t12a3,sizeof(STARS_CV_12a3)==54);
STARS_CHECK(t12a4,sizeof(STARS_CV_12a4)==432);
STARS_CHECK(t12a5,sizeof(STARS_CV_12a5)==810);
STARS_CHECK(t12a6,sizeof(STARS_CV_12a6)==54);
STARS_CHECK(t12a7,sizeof(STARS_CV_12a7)==1080);
STARS_CHECK(t12a8,sizeof(STARS_CV_12a8)==58);
STARS_CHECK(t12a9,sizeof(STARS_CV_12a9)==870);
STARS_CHECK(t12aa,sizeof(STARS_CV_12aa)==60);
STARS_CHECK(t12ab,sizeof(STARS_CV_12ab)==720);
STARS_CHECK(t12ac,sizeof(STARS_CV_12ac)==60);
STARS_CHECK(t12ad,sizeof(STARS_CV_12ad)==1440);
STARS_CHECK(t12ae,sizeof(STARS_CV_12ae)==4576);
STARS_CHECK(t12af,sizeof(STARS_CV_12af)==3234);
STARS_CHECK(t12b0,sizeof(STARS_CV_12b0)==715);
STARS_CHECK(t12b1,sizeof(STARS_CV_12b1)==588);
STARS_CHECK(t12b2,sizeof(STARS_CV_12b2)==56);
STARS_CHECK(t12b3,sizeof(STARS_CV_12b3)==896);
STARS_CHECK(t12bf,sizeof(STARS_CV_12bf)==200);
STARS_CHECK(t12c2,sizeof(STARS_CV_12c2)==30);
STARS_CHECK(t12c3,sizeof(STARS_CV_12c3)==42);
STARS_CHECK(t12c4,sizeof(STARS_CV_12c4)==28);
STARS_CHECK(t12c5,sizeof(STARS_CV_12c5)==26);
STARS_CHECK(t12c6,sizeof(STARS_CV_12c6)==26);
STARS_CHECK(t12c7,sizeof(STARS_CV_12c7)==16);
STARS_CHECK(t12c8,sizeof(STARS_CV_12c8)==16);
STARS_CHECK(t12c9,sizeof(STARS_CV_12c9)==128);
STARS_CHECK(t12d0,sizeof(STARS_CV_12d0)==90);
STARS_CHECK(t12d1,sizeof(STARS_CV_12d1)==150);
STARS_CHECK(t12d3,sizeof(STARS_CV_12d3)==16);
STARS_CHECK(t12d5,sizeof(STARS_CV_12d5)==4);
STARS_CHECK(t12d4,sizeof(STARS_CV_12d4)==12);
STARS_CHECK(t12f1,sizeof(STARS_CV_12f1)==14);
STARS_CHECK(t12d6,sizeof(STARS_CV_12d6)==18);
STARS_CHECK(t12db,sizeof(STARS_CV_12db)==40);
STARS_CHECK(t1409,sizeof(STARS_CV_1409)==1);
STARS_CHECK(t140a,sizeof(STARS_CV_140a)==1);
STARS_CHECK(t12e0,sizeof(STARS_CV_12e0)==4);
STARS_CHECK(t12de,sizeof(STARS_CV_12de)==5);
STARS_CHECK(t12df,sizeof(STARS_CV_12df)==4);
STARS_CHECK(t12e1,sizeof(STARS_CV_12e1)==2);
STARS_CHECK(t12e2,sizeof(STARS_CV_12e2)==2);
STARS_CHECK(t12e5,sizeof(STARS_CV_12e5)==4);
STARS_CHECK(t12e8,sizeof(STARS_CV_12e8)==480);
STARS_CHECK(t12eb,sizeof(STARS_CV_12eb)==1024);
STARS_CHECK(t12ec,sizeof(STARS_CV_12ec)==2);
STARS_CHECK(t12ed,sizeof(STARS_CV_12ed)==49);
STARS_CHECK(t12ee,sizeof(STARS_CV_12ee)==8);
STARS_CHECK(t12ef,sizeof(STARS_CV_12ef)==22836);
STARS_CHECK(t12f0,sizeof(STARS_CV_12f0)==387);
STARS_CHECK(t12f2,sizeof(STARS_CV_12f2)==72);
STARS_CHECK(t12f3,sizeof(STARS_CV_12f3)==387);
STARS_CHECK(t139f,sizeof(STARS_CV_139f)==4);
STARS_CHECK(t13a0,sizeof(STARS_CV_13a0)==4);
STARS_CHECK(t12f6,sizeof(STARS_CV_12f6)==4);
STARS_CHECK(t12f7,sizeof(STARS_CV_12f7)==10);
STARS_CHECK(t12f9,sizeof(STARS_CV_12f9)==40);
STARS_CHECK(t12fa,sizeof(STARS_CV_12fa)==208);
STARS_CHECK(t12fb,sizeof(STARS_CV_12fb)==160);
STARS_CHECK(t12fc,sizeof(STARS_CV_12fc)==3072);
STARS_CHECK(t12fd,sizeof(STARS_CV_12fd)==28);
STARS_CHECK(t12fe,sizeof(STARS_CV_12fe)==4);
STARS_CHECK(t1300,sizeof(STARS_CV_1300)==2);
STARS_CHECK(t1306,sizeof(STARS_CV_1306)==10);
STARS_CHECK(t1307,sizeof(STARS_CV_1307)==768);
STARS_CHECK(t1308,sizeof(STARS_CV_1308)==4608);
STARS_CHECK(t1309,sizeof(STARS_CV_1309)==18);
STARS_CHECK(t130a,sizeof(STARS_CV_130a)==5);
STARS_CHECK(t130b,sizeof(STARS_CV_130b)==180);
STARS_CHECK(t130d,sizeof(STARS_CV_130d)==14);
STARS_CHECK(t1310,sizeof(STARS_CV_1310)==264);
STARS_CHECK(t1311,sizeof(STARS_CV_1311)==2);
STARS_CHECK(t1314,sizeof(STARS_CV_1314)==147);
STARS_CHECK(t1319,sizeof(STARS_CV_1319)==80);
STARS_CHECK(t131a,sizeof(STARS_CV_131a)==134);
STARS_CHECK(t131b,sizeof(STARS_CV_131b)==33);
STARS_CHECK(t1500,sizeof(STARS_CV_1500)==2);
STARS_CHECK(t1520,sizeof(STARS_CV_1520)==2);
STARS_CHECK(t1521,sizeof(STARS_CV_1521)==2);
STARS_CHECK(t1323,sizeof(STARS_CV_1323)==16);
STARS_CHECK(t132e,sizeof(STARS_CV_132e)==128);
STARS_CHECK(t132f,sizeof(STARS_CV_132f)==256);
STARS_CHECK(t1330,sizeof(STARS_CV_1330)==25);
STARS_CHECK(t1331,sizeof(STARS_CV_1331)==4);
STARS_CHECK(t14af,sizeof(STARS_CV_14af)==2);
STARS_CHECK(t1332,sizeof(STARS_CV_1332)==12);
STARS_CHECK(t1333,sizeof(STARS_CV_1333)==4);
STARS_CHECK(t1339,sizeof(STARS_CV_1339)==24);
STARS_CHECK(t133d,sizeof(STARS_CV_133d)==48);
STARS_CHECK(t133e,sizeof(STARS_CV_133e)==12);
STARS_CHECK(t133f,sizeof(STARS_CV_133f)==16);
STARS_CHECK(t1340,sizeof(STARS_CV_1340)==4);
STARS_CHECK(t1342,sizeof(STARS_CV_1342)==4);
STARS_CHECK(t1345,sizeof(STARS_CV_1345)==2);
STARS_CHECK(t134b,sizeof(STARS_CV_134b)==10);
STARS_CHECK(t134e,sizeof(STARS_CV_134e)==4);
STARS_CHECK(t1354,sizeof(STARS_CV_1354)==36);
STARS_CHECK(t1355,sizeof(STARS_CV_1355)==4);
STARS_CHECK(t1356,sizeof(STARS_CV_1356)==4);
STARS_CHECK(t1357,sizeof(STARS_CV_1357)==4);
STARS_CHECK(t1358,sizeof(STARS_CV_1358)==4);
STARS_CHECK(t1359,sizeof(STARS_CV_1359)==4);
STARS_CHECK(t135b,sizeof(STARS_CV_135b)==6);
STARS_CHECK(t135c,sizeof(STARS_CV_135c)==2);
STARS_CHECK(t1366,sizeof(STARS_CV_1366)==23);
STARS_CHECK(t1368,sizeof(STARS_CV_1368)==3);
STARS_CHECK(t1369,sizeof(STARS_CV_1369)==3);
STARS_CHECK(t136b,sizeof(STARS_CV_136b)==15);
STARS_CHECK(t136c,sizeof(STARS_CV_136c)==4);
STARS_CHECK(t136d,sizeof(STARS_CV_136d)==4);
STARS_CHECK(t136f,sizeof(STARS_CV_136f)==34);
STARS_CHECK(t1370,sizeof(STARS_CV_1370)==2);
STARS_CHECK(t1372,sizeof(STARS_CV_1372)==4);
STARS_CHECK(t1373,sizeof(STARS_CV_1373)==2);
STARS_CHECK(t1375,sizeof(STARS_CV_1375)==4);
STARS_CHECK(t1377,sizeof(STARS_CV_1377)==12);
STARS_CHECK(t139a,sizeof(STARS_CV_139a)==20);
STARS_CHECK(t139d,sizeof(STARS_CV_139d)==14);
STARS_CHECK(t13a2,sizeof(STARS_CV_13a2)==2);
STARS_CHECK(t13a4,sizeof(STARS_CV_13a4)==4);
STARS_CHECK(t13a5,sizeof(STARS_CV_13a5)==4);
STARS_CHECK(t13a7,sizeof(STARS_CV_13a7)==26);
STARS_CHECK(t13a8,sizeof(STARS_CV_13a8)==2);
STARS_CHECK(t13ac,sizeof(STARS_CV_13ac)==8);
STARS_CHECK(t13ae,sizeof(STARS_CV_13ae)==14);
STARS_CHECK(t13af,sizeof(STARS_CV_13af)==2);
STARS_CHECK(t13b0,sizeof(STARS_CV_13b0)==2);
STARS_CHECK(t13b2,sizeof(STARS_CV_13b2)==2);
STARS_CHECK(t13b4,sizeof(STARS_CV_13b4)==2);
STARS_CHECK(t13bb,sizeof(STARS_CV_13bb)==2);
STARS_CHECK(t13b8,sizeof(STARS_CV_13b8)==4);
STARS_CHECK(t13b6,sizeof(STARS_CV_13b6)==16);
STARS_CHECK(t13bc,sizeof(STARS_CV_13bc)==4);
STARS_CHECK(t13bd,sizeof(STARS_CV_13bd)==4);
STARS_CHECK(t13df,sizeof(STARS_CV_13df)==14);
STARS_CHECK(t13e0,sizeof(STARS_CV_13e0)==1);
STARS_CHECK(t13e2,sizeof(STARS_CV_13e2)==5);
STARS_CHECK(t13e4,sizeof(STARS_CV_13e4)==4);
STARS_CHECK(t13e6,sizeof(STARS_CV_13e6)==10);
STARS_CHECK(t13e8,sizeof(STARS_CV_13e8)==8);
STARS_CHECK(t13ea,sizeof(STARS_CV_13ea)==12);
STARS_CHECK(t13ec,sizeof(STARS_CV_13ec)==24);
STARS_CHECK(t13ed,sizeof(STARS_CV_13ed)==2);
STARS_CHECK(t13ef,sizeof(STARS_CV_13ef)==14);
STARS_CHECK(t13f0,sizeof(STARS_CV_13f0)==4);
STARS_CHECK(t13f1,sizeof(STARS_CV_13f1)==4);
STARS_CHECK(t13f3,sizeof(STARS_CV_13f3)==8);
STARS_CHECK(t13f4,sizeof(STARS_CV_13f4)==4);
STARS_CHECK(t13f7,sizeof(STARS_CV_13f7)==4);
STARS_CHECK(t13f8,sizeof(STARS_CV_13f8)==2);
STARS_CHECK(t13fb,sizeof(STARS_CV_13fb)==36);
STARS_CHECK(t13fd,sizeof(STARS_CV_13fd)==12);
STARS_CHECK(t13fe,sizeof(STARS_CV_13fe)==4);
STARS_CHECK(t1400,sizeof(STARS_CV_1400)==18);
STARS_CHECK(t1402,sizeof(STARS_CV_1402)==26);
STARS_CHECK(t1405,sizeof(STARS_CV_1405)==2);
STARS_CHECK(t1407,sizeof(STARS_CV_1407)==10);
STARS_CHECK(t1408,sizeof(STARS_CV_1408)==4);
STARS_CHECK(t140c,sizeof(STARS_CV_140c)==7);
STARS_CHECK(t140e,sizeof(STARS_CV_140e)==4);
STARS_CHECK(t140f,sizeof(STARS_CV_140f)==4);
STARS_CHECK(t1410,sizeof(STARS_CV_1410)==4);
STARS_CHECK(t1415,sizeof(STARS_CV_1415)==52);
STARS_CHECK(t1416,sizeof(STARS_CV_1416)==4);
STARS_CHECK(t1417,sizeof(STARS_CV_1417)==2);
STARS_CHECK(t1419,sizeof(STARS_CV_1419)==2);
STARS_CHECK(t141d,sizeof(STARS_CV_141d)==25);
STARS_CHECK(t141e,sizeof(STARS_CV_141e)==2);
STARS_CHECK(t141f,sizeof(STARS_CV_141f)==2);
STARS_CHECK(t1421,sizeof(STARS_CV_1421)==8);
STARS_CHECK(t1422,sizeof(STARS_CV_1422)==4);
STARS_CHECK(t1425,sizeof(STARS_CV_1425)==4);
STARS_CHECK(t1426,sizeof(STARS_CV_1426)==4);
STARS_CHECK(t1427,sizeof(STARS_CV_1427)==4);
STARS_CHECK(t142b,sizeof(STARS_CV_142b)==5);
STARS_CHECK(t142d,sizeof(STARS_CV_142d)==16);
STARS_CHECK(t142e,sizeof(STARS_CV_142e)==2);
STARS_CHECK(t142f,sizeof(STARS_CV_142f)==4);
STARS_CHECK(t1431,sizeof(STARS_CV_1431)==10);
STARS_CHECK(t1433,sizeof(STARS_CV_1433)==4);
STARS_CHECK(t1434,sizeof(STARS_CV_1434)==4);
STARS_CHECK(t1436,sizeof(STARS_CV_1436)==9);
STARS_CHECK(t1438,sizeof(STARS_CV_1438)==6);
STARS_CHECK(t143c,sizeof(STARS_CV_143c)==4);
STARS_CHECK(t143e,sizeof(STARS_CV_143e)==2);
STARS_CHECK(t1443,sizeof(STARS_CV_1443)==4);
STARS_CHECK(t1444,sizeof(STARS_CV_1444)==4);
STARS_CHECK(t1446,sizeof(STARS_CV_1446)==20);
STARS_CHECK(t1448,sizeof(STARS_CV_1448)==28);
STARS_CHECK(t1449,sizeof(STARS_CV_1449)==2);
STARS_CHECK(t144b,sizeof(STARS_CV_144b)==2);
STARS_CHECK(t144c,sizeof(STARS_CV_144c)==2);
STARS_CHECK(t144e,sizeof(STARS_CV_144e)==12);
STARS_CHECK(t144f,sizeof(STARS_CV_144f)==4);
STARS_CHECK(t1451,sizeof(STARS_CV_1451)==6);
STARS_CHECK(t1454,sizeof(STARS_CV_1454)==14);
STARS_CHECK(t1457,sizeof(STARS_CV_1457)==34);
STARS_CHECK(t1459,sizeof(STARS_CV_1459)==52);
STARS_CHECK(t145f,sizeof(STARS_CV_145f)==2);
STARS_CHECK(t1460,sizeof(STARS_CV_1460)==1280);
STARS_CHECK(t1462,sizeof(STARS_CV_1462)==1284);
STARS_CHECK(t1463,sizeof(STARS_CV_1463)==4);
STARS_CHECK(t1466,sizeof(STARS_CV_1466)==4);
STARS_CHECK(t1468,sizeof(STARS_CV_1468)==46);
STARS_CHECK(t146a,sizeof(STARS_CV_146a)==22);
STARS_CHECK(t146d,sizeof(STARS_CV_146d)==4);
STARS_CHECK(t1475,sizeof(STARS_CV_1475)==4);
STARS_CHECK(t1477,sizeof(STARS_CV_1477)==24);
STARS_CHECK(t1479,sizeof(STARS_CV_1479)==8);
STARS_CHECK(t147b,sizeof(STARS_CV_147b)==26);
STARS_CHECK(t147e,sizeof(STARS_CV_147e)==2);
STARS_CHECK(t147f,sizeof(STARS_CV_147f)==2);
STARS_CHECK(t1480,sizeof(STARS_CV_1480)==2);
STARS_CHECK(t1482,sizeof(STARS_CV_1482)==10);
STARS_CHECK(t1484,sizeof(STARS_CV_1484)==16);
STARS_CHECK(t1486,sizeof(STARS_CV_1486)==37);
STARS_CHECK(t1488,sizeof(STARS_CV_1488)==2);
STARS_CHECK(t1489,sizeof(STARS_CV_1489)==16);
STARS_CHECK(t148b,sizeof(STARS_CV_148b)==4);
STARS_CHECK(t148d,sizeof(STARS_CV_148d)==8);
STARS_CHECK(t148f,sizeof(STARS_CV_148f)==10);
STARS_CHECK(t1491,sizeof(STARS_CV_1491)==114);
STARS_CHECK(t1498,sizeof(STARS_CV_1498)==4);
STARS_CHECK(t149a,sizeof(STARS_CV_149a)==6);
STARS_CHECK(t149b,sizeof(STARS_CV_149b)==4);
STARS_CHECK(t149d,sizeof(STARS_CV_149d)==4);
STARS_CHECK(t149f,sizeof(STARS_CV_149f)==12);
STARS_CHECK(t14a0,sizeof(STARS_CV_14a0)==2);
STARS_CHECK(t14a1,sizeof(STARS_CV_14a1)==4);
STARS_CHECK(t14a3,sizeof(STARS_CV_14a3)==4);
STARS_CHECK(t14a4,sizeof(STARS_CV_14a4)==8);
STARS_CHECK(t14a6,sizeof(STARS_CV_14a6)==12);
STARS_CHECK(t14a8,sizeof(STARS_CV_14a8)==4);
STARS_CHECK(t14a9,sizeof(STARS_CV_14a9)==2);
STARS_CHECK(t14ac,sizeof(STARS_CV_14ac)==4);
STARS_CHECK(t14ad,sizeof(STARS_CV_14ad)==4);
STARS_CHECK(t14b2,sizeof(STARS_CV_14b2)==41);
STARS_CHECK(t14b5,sizeof(STARS_CV_14b5)==22);
STARS_CHECK(t14b7,sizeof(STARS_CV_14b7)==4);
STARS_CHECK(t14c0,sizeof(STARS_CV_14c0)==4);
STARS_CHECK(t14c2,sizeof(STARS_CV_14c2)==4);
STARS_CHECK(t14c6,sizeof(STARS_CV_14c6)==2);
STARS_CHECK(t14c8,sizeof(STARS_CV_14c8)==16);
STARS_CHECK(t14cf,sizeof(STARS_CV_14cf)==17);
STARS_CHECK(t14d3,sizeof(STARS_CV_14d3)==4);
STARS_CHECK(t14d5,sizeof(STARS_CV_14d5)==4);
STARS_CHECK(t14d9,sizeof(STARS_CV_14d9)==19);
STARS_CHECK(t14db,sizeof(STARS_CV_14db)==12);
STARS_CHECK(t14dd,sizeof(STARS_CV_14dd)==10);
STARS_CHECK(t14de,sizeof(STARS_CV_14de)==2);
STARS_CHECK(t14df,sizeof(STARS_CV_14df)==2);
STARS_CHECK(t14e2,sizeof(STARS_CV_14e2)==4);
STARS_CHECK(t14e3,sizeof(STARS_CV_14e3)==2);
STARS_CHECK(t14e5,sizeof(STARS_CV_14e5)==134);
STARS_CHECK(t14e6,sizeof(STARS_CV_14e6)==2);
STARS_CHECK(t14e7,sizeof(STARS_CV_14e7)==4);
STARS_CHECK(t14ea,sizeof(STARS_CV_14ea)==32);
STARS_CHECK(t14ed,sizeof(STARS_CV_14ed)==4);
STARS_CHECK(t14ee,sizeof(STARS_CV_14ee)==2);
STARS_CHECK(t14f0,sizeof(STARS_CV_14f0)==6);
STARS_CHECK(t14f1,sizeof(STARS_CV_14f1)==2);
STARS_CHECK(t14f2,sizeof(STARS_CV_14f2)==4);
STARS_CHECK(t14f3,sizeof(STARS_CV_14f3)==4);
STARS_CHECK(t14f5,sizeof(STARS_CV_14f5)==8);
STARS_CHECK(t14f6,sizeof(STARS_CV_14f6)==4);
STARS_CHECK(t14f8,sizeof(STARS_CV_14f8)==4);
STARS_CHECK(t14f9,sizeof(STARS_CV_14f9)==4);
STARS_CHECK(t14fa,sizeof(STARS_CV_14fa)==2);
STARS_CHECK(t14fc,sizeof(STARS_CV_14fc)==2);
STARS_CHECK(t14fd,sizeof(STARS_CV_14fd)==1);
STARS_CHECK(t14ff,sizeof(STARS_CV_14ff)==4);
STARS_CHECK(t1502,sizeof(STARS_CV_1502)==2);
STARS_CHECK(t1504,sizeof(STARS_CV_1504)==4);
STARS_CHECK(t1505,sizeof(STARS_CV_1505)==4);
STARS_CHECK(t1506,sizeof(STARS_CV_1506)==4);
STARS_CHECK(t1507,sizeof(STARS_CV_1507)==2);
STARS_CHECK(t1508,sizeof(STARS_CV_1508)==24);
STARS_CHECK(t150a,sizeof(STARS_CV_150a)==28);
STARS_CHECK(t150b,sizeof(STARS_CV_150b)==4);
STARS_CHECK(t150f,sizeof(STARS_CV_150f)==18);
STARS_CHECK(t1511,sizeof(STARS_CV_1511)==20);
STARS_CHECK(t1513,sizeof(STARS_CV_1513)==16);
STARS_CHECK(t1518,sizeof(STARS_CV_1518)==6);
STARS_CHECK(t1519,sizeof(STARS_CV_1519)==4);
STARS_CHECK(t151b,sizeof(STARS_CV_151b)==2);
STARS_CHECK(t151c,sizeof(STARS_CV_151c)==2);
STARS_CHECK(t151d,sizeof(STARS_CV_151d)==64);
STARS_CHECK(t151f,sizeof(STARS_CV_151f)==146);
STARS_CHECK(t1523,sizeof(STARS_CV_1523)==2);
STARS_CHECK(t1528,sizeof(STARS_CV_1528)==4);
STARS_CHECK(t152e,sizeof(STARS_CV_152e)==28);
STARS_CHECK(t152f,sizeof(STARS_CV_152f)==4);
STARS_CHECK(t1530,sizeof(STARS_CV_1530)==4);
STARS_CHECK(t1532,sizeof(STARS_CV_1532)==6);
STARS_CHECK(t1536,sizeof(STARS_CV_1536)==4);
STARS_CHECK(t153a,sizeof(STARS_CV_153a)==4);
STARS_CHECK(t153b,sizeof(STARS_CV_153b)==2);
STARS_CHECK(t153c,sizeof(STARS_CV_153c)==2);
STARS_CHECK(t153d,sizeof(STARS_CV_153d)==4);
STARS_CHECK(t1540,sizeof(STARS_CV_1540)==4);
STARS_CHECK(t1541,sizeof(STARS_CV_1541)==2);
STARS_CHECK(t1542,sizeof(STARS_CV_1542)==4);
STARS_CHECK(t1543,sizeof(STARS_CV_1543)==4);
STARS_CHECK(t1544,sizeof(STARS_CV_1544)==4);
STARS_CHECK(t1546,sizeof(STARS_CV_1546)==44);
STARS_CHECK(t1547,sizeof(STARS_CV_1547)==4);
STARS_CHECK(t1548,sizeof(STARS_CV_1548)==4);
STARS_CHECK(t1549,sizeof(STARS_CV_1549)==4);
STARS_CHECK(t154d,sizeof(STARS_CV_154d)==6);
STARS_CHECK(t154e,sizeof(STARS_CV_154e)==2);
STARS_CHECK(t1550,sizeof(STARS_CV_1550)==17);
STARS_CHECK(t1551,sizeof(STARS_CV_1551)==4);
STARS_CHECK(t1552,sizeof(STARS_CV_1552)==4);
STARS_CHECK(t1554,sizeof(STARS_CV_1554)==2);
STARS_CHECK(t1566,sizeof(STARS_CV_1566)==2);
STARS_CHECK(t156a,sizeof(STARS_CV_156a)==4);
STARS_CHECK(t156b,sizeof(STARS_CV_156b)==4);
STARS_CHECK(t156d,sizeof(STARS_CV_156d)==8);
STARS_CHECK(t156e,sizeof(STARS_CV_156e)==4);
STARS_CHECK(t1571,sizeof(STARS_CV_1571)==1024);
STARS_CHECK(t1573,sizeof(STARS_CV_1573)==149);
STARS_CHECK(t1578,sizeof(STARS_CV_1578)==2);
STARS_CHECK(t157f,sizeof(STARS_CV_157f)==2);
STARS_CHECK(t1582,sizeof(STARS_CV_1582)==4);
STARS_CHECK(t1583,sizeof(STARS_CV_1583)==4);
STARS_CHECK(t1584,sizeof(STARS_CV_1584)==28);
STARS_CHECK(t1585,sizeof(STARS_CV_1585)==4);
STARS_CHECK(t1586,sizeof(STARS_CV_1586)==2);
STARS_CHECK(t1589,sizeof(STARS_CV_1589)==2);
STARS_CHECK(t158a,sizeof(STARS_CV_158a)==41);
STARS_CHECK(t158d,sizeof(STARS_CV_158d)==2);
STARS_CHECK(t1590,sizeof(STARS_CV_1590)==4);
STARS_CHECK(t1591,sizeof(STARS_CV_1591)==1024);
STARS_CHECK(t1593,sizeof(STARS_CV_1593)==30);
STARS_CHECK(t1596,sizeof(STARS_CV_1596)==4);
STARS_CHECK(t159e,sizeof(STARS_CV_159e)==34);
STARS_CHECK(t15a8,sizeof(STARS_CV_15a8)==448);
STARS_CHECK(t15b8,sizeof(STARS_CV_15b8)==22);
STARS_CHECK(t15b9,sizeof(STARS_CV_15b9)==4);
STARS_CHECK(t15ba,sizeof(STARS_CV_15ba)==400);
STARS_CHECK(t15c0,sizeof(STARS_CV_15c0)==64);
STARS_CHECK(t15c1,sizeof(STARS_CV_15c1)==40);
STARS_CHECK(t15c2,sizeof(STARS_CV_15c2)==4);
STARS_CHECK(t15c3,sizeof(STARS_CV_15c3)==248);
STARS_CHECK(t15ca,sizeof(STARS_CV_15ca)==2);
STARS_CHECK(t15cb,sizeof(STARS_CV_15cb)==2);
STARS_CHECK(t15cc,sizeof(STARS_CV_15cc)==8);
STARS_CHECK(t15cd,sizeof(STARS_CV_15cd)==112);
STARS_CHECK(t15ce,sizeof(STARS_CV_15ce)==152);
STARS_CHECK(t15e5,sizeof(STARS_CV_15e5)==26);
STARS_CHECK(t15e6,sizeof(STARS_CV_15e6)==96);
STARS_CHECK(t15e8,sizeof(STARS_CV_15e8)==4);
STARS_CHECK(t15f0,sizeof(STARS_CV_15f0)==4);
STARS_CHECK(t15f2,sizeof(STARS_CV_15f2)==100);
STARS_CHECK(t15f4,sizeof(STARS_CV_15f4)==2);
STARS_CHECK(t15f9,sizeof(STARS_CV_15f9)==20);
STARS_CHECK(t15fa,sizeof(STARS_CV_15fa)==16);
STARS_CHECK(t15fc,sizeof(STARS_CV_15fc)==999);
STARS_CHECK(t15fd,sizeof(STARS_CV_15fd)==500);
STARS_CHECK(t1600,sizeof(STARS_CV_1600)==2);
STARS_CHECK(t1603,sizeof(STARS_CV_1603)==174);
STARS_CHECK(t1604,sizeof(STARS_CV_1604)==2);
STARS_CHECK(t160f,sizeof(STARS_CV_160f)==16);
STARS_CHECK(t1619,sizeof(STARS_CV_1619)==20);
STARS_CHECK(t161a,sizeof(STARS_CV_161a)==38);
STARS_CHECK(t161b,sizeof(STARS_CV_161b)==12);
STARS_CHECK(t162d,sizeof(STARS_CV_162d)==2);
STARS_CHECK(t162e,sizeof(STARS_CV_162e)==252);
STARS_CHECK(t1636,sizeof(STARS_CV_1636)==54);
STARS_CHECK(t1649,sizeof(STARS_CV_1649)==2);
STARS_CHECK(t165a,sizeof(STARS_CV_165a)==20);
STARS_CHECK(t165b,sizeof(STARS_CV_165b)==16);
STARS_CHECK(t165e,sizeof(STARS_CV_165e)==512);
STARS_CHECK(t165f,sizeof(STARS_CV_165f)==32);
STARS_CHECK(t1660,sizeof(STARS_CV_1660)==10);
STARS_CHECK(t1661,sizeof(STARS_CV_1661)==90);
STARS_CHECK(t1666,sizeof(STARS_CV_1666)==6);
STARS_CHECK(t166d,sizeof(STARS_CV_166d)==21);
STARS_CHECK(t1672,sizeof(STARS_CV_1672)==32);
STARS_CHECK(t1678,sizeof(STARS_CV_1678)==11);
STARS_CHECK(t1679,sizeof(STARS_CV_1679)==4);
STARS_CHECK(t167c,sizeof(STARS_CV_167c)==4);
STARS_CHECK(t167f,sizeof(STARS_CV_167f)==4);
STARS_CHECK(t1682,sizeof(STARS_CV_1682)==4);
STARS_CHECK(t1689,sizeof(STARS_CV_1689)==2352);
STARS_CHECK(t168a,sizeof(STARS_CV_168a)==64);
STARS_CHECK(t168b,sizeof(STARS_CV_168b)==3996);
STARS_CHECK(t168c,sizeof(STARS_CV_168c)==1998);
STARS_CHECK(t168d,sizeof(STARS_CV_168d)==360);
#pragma pack()
/* VC8 rejects these offsetof expressions in file-scope array bounds.
 * STARS_RUNTIME_OFFSETS requires a runner to include soffsets.inc.
 * Size/primitive checks above remain compile-time assertions. */
#ifndef STARS_RUNTIME_OFFSETS
#define STARS_OFFSET(name,type,member,expected) STARS_CHECK(name,offsetof(type,member)==expected);
#include "soffsets.inc"
#undef STARS_OFFSET
#endif
#endif
