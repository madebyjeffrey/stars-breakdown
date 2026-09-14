#include "TYPES.H"

int FCheckZip(int iZip, IA __far *lpiaGoal, int ids) {
    int i;
    int idhSav;
    char szT[34];
    IA *piaCur;
    idhSav = tutor.idh;
    tutor.idh = 0x44a;
    if (tutor.fAutoComplete) {
        vrgZip[iZip].fValid = 1;
        piaCur = vrgZip[iZip].rgia;
        for (i = 0; i < 5; i++, piaCur++, lpiaGoal++)
            piaCur->action = lpiaGoal->action;
        CchGetString(ids, szT);
        strcpy(vrgZip[iZip].szName, szT);
        return 1;
    }
    if (!vrgZip[iZip].fValid) {
        if (!hwndZipOrderDlg)
            tutor.idh = 0x5f0;
        return 0;
    }
    piaCur = vrgZip[iZip].rgia;
    tutor.idh = 0x5ef;
    for (i = 0; i < 5; i++, piaCur++, lpiaGoal++) {
        if (piaCur->action != lpiaGoal->action && piaCur->action != 0)
            return 0;
    }
    CchGetString(ids, szT);
    if (_strcmpi(szT, vrgZip[iZip].szName) == 0) {
        tutor.idh = idhSav;
        return 1;
    }
    TutorError(0x208);
    return 0;
}
