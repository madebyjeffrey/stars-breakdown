/* Original-sized native16 views needed by the tutorial matching candidates.
 * Padded regions are deliberately opaque, not full recovered declarations. */
#include <string.h>
#include <windows.h>
#pragma pack(1)
typedef struct {
    unsigned fVisible : 1, fGameSaved : 1, fChange : 1, fTurnDone : 1, fTutorDone : 1,
        fNoErrors : 1, cError : 3, fAutoComplete : 1, fProgress : 1, fTBVis : 1, fValidQ : 1,
        fFreeing : 1, fShowHidMsg : 1, unused : 1;
    int idt, idtBold, idh, idsError, iScanZoom, icolFSort;
    unsigned short grbitScan;
    HWND hwnd;
    char zpq[26];
} TUTOR;
typedef struct {
    char pad[16];
    unsigned fExtraFuel : 1, fSlowTech : 1, fSinglePlr : 1, fTutorial : 1, padbits : 12;
    unsigned short turn;
    char tail[44];
} GAME;
typedef struct {
    unsigned pad0 : 1, fGeneratingTurn : 1, pad2 : 9, fTutorial : 1, pad12 : 4;
} GD;
typedef struct {
    signed char iPlayer, cShDef;
    short cPlanet;
    unsigned cFleet : 12, cshdefSB : 4;
    char pad6[50];
    signed char pctResearch, iTechCur;
    char tail[134];
} PLAYER;
typedef struct {
    unsigned long cItem : 10, iItem : 7, grobj : 3, pct : 7, unused : 5;
} PROD;
typedef struct {
    unsigned quantity : 12, action : 4;
} IA;
typedef struct {
    POINT pt;
    short id;
    unsigned grTask : 4, iWarp : 4, grobj : 4, fValidTask : 1, fNoAutoTrack : 1, fUnused : 2;
    union {
        IA rgia[5];
        struct {
            unsigned cTime, cTimeOld;
        } tlm;
        struct {
            unsigned iWarp, iDist;
        } tptl;
    } task;
} ORDER;
typedef struct {
    unsigned short iordMax, iordMac;
    ORDER rgord[1];
} PLORD;
typedef struct {
    unsigned short iprodMax, iprodMac;
    PROD rgprod[1];
} PLPROD;
typedef struct {
    short id, iPlayer;
    unsigned flags;
    short idPlanet;
    POINT pt;
    short rgcsh[16];
    char pad44[32];
    long rgwtMin[5];
    unsigned char iplan, bUnused;
    short cord;
    PLORD __far *lpplord;
    char pad104[16];
    char __far *lpszName;
} FLEET;
typedef struct {
    short id;
    char pad2[22];
    unsigned long pad24 : 23, fNoResearch : 1, pad25 : 8;
    long rgwtMin[4];
    char pad44[4];
    unsigned idRoute : 10, unused4 : 6;
    short turn;
    PLPROD __far *lpplprod;
} PLANET;
typedef struct {
    unsigned idFull;
    char rest[16];
} THING;
typedef struct {
    POINT pt;
    short grobj, grobjFull, idpl, ifl, iwp, ith;
} SCAN;
typedef struct {
    POINT pt;
    short grobj, grobjFull, id, iwpAct;
    SCAN scan;
    FLEET fl;
    PLANET pl;
    THING th;
} SEL;
typedef struct {
    unsigned short grhst;
    unsigned iItem : 8, cItem : 8;
} HS;
extern TUTOR tutor;
extern GAME game;
extern GD gd;
extern PLAYER rgplr[16];
extern unsigned short grbitScan;
extern int iScanZoom, mdMsgObj, idMsgObj, iMsgCur;
extern SEL sel;
extern FLEET __far * __far * rglpfl;
extern THING __far *lpThings;
extern int *vrgiflMerge;
extern unsigned char rgcbtlplan[16], bitfMsgFiltered[];
extern HWND hwndSlotDlg;
extern int mdBuild;
extern FLEET __far *LpflFromId(int);
extern PLANET __far *LpplFromId(int);
extern int AlertSz(char *, int);
extern char *PszFormatIds(int, int __far *);
extern int CchGetString(int, char *);
extern void Randomize2(unsigned long);
extern void RestoreGameState(void);
extern void SetFilteringGroups(int, int);
extern int IMsgNext(int), IdmGetMessageN(int);
extern void TutorError(int);
extern int FCheckSelection(unsigned, int);
extern int FCheckFleetWP(unsigned, int, unsigned, int, unsigned, unsigned);
extern void EndTutor(int);
#define TRETURN(v)                                                                                 \
    {                                                                                              \
        if (v)                                                                                     \
            tutor.idh = idhSav;                                                                    \
        return (v);                                                                                \
    }
typedef char ck_tutor[sizeof(TUTOR) == 44 ? 1 : -1];
typedef char ck_player[sizeof(PLAYER) == 192 ? 1 : -1];
typedef char ck_fleet[sizeof(FLEET) == 124 ? 1 : -1];
typedef char ck_planet[sizeof(PLANET) == 56 ? 1 : -1];
typedef char ck_sel[sizeof(SEL) == 226 ? 1 : -1];

typedef struct {
    char data[36];
} BTLPLAN;
extern BTLPLAN __far *rglpbtlplan[16];

extern int FCheckCargo(FLEET __far *, int, int, int, int);
extern int FTutorTaskDone(void);
extern void ShowTutor(int);
extern char *PszGetCompressedString(int);
extern int dyArial8;
extern void ExpandRc(RECT *, int, int);

typedef struct {
    char pad[58];
    HS rghs[16];
    char tail[25];
} SHDEF;
extern SHDEF __far *lpshdefBuild;
typedef struct {
    IA rgia[5];
    char szName[13];
    unsigned char fValid;
} ZIPORDER;
extern ZIPORDER vrgZip[4];
extern HWND hwndZipOrderDlg;
