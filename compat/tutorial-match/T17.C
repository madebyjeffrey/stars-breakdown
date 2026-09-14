#include "TYPES.H"

int FCheckCargo(FLEET __far *lpfl, int wtMin1, int wtMin2, int wtMin3, int wtColonists) {
    int fRet;
    int idh;
    int idhSav;
    idhSav = tutor.idh;
    fRet = 0;
    if (lpfl == 0)
        return 0;
    tutor.idh = 0x433;
    if ((wtMin1 == 0 && lpfl->rgwtMin[0] != 0) || (wtMin2 == 0 && lpfl->rgwtMin[1] != 0) ||
        (wtMin3 == 0 && lpfl->rgwtMin[2] != 0) || (wtColonists == 0 && lpfl->rgwtMin[3] != 0))
        TutorError(0x1ef);
    else if (lpfl->rgwtMin[3] == wtColonists && lpfl->rgwtMin[0] == wtMin1 &&
             lpfl->rgwtMin[1] == wtMin2 && lpfl->rgwtMin[2] == wtMin3)
        fRet = 1;
    idh = tutor.idh;
    if (!fRet && FCheckSelection(2, lpfl->id))
        tutor.idh = idh;
    if (fRet)
        tutor.idh = idhSav;
    return fRet;
}
