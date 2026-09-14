/* UNMATCHED EXPERIMENT: stack layout and remaining instruction differences are unresolved. */
#include "TYPES.H"

int FCheckXferWP(unsigned ifl, int iord, int id, unsigned iWarp, IA __far *lpiaGoal) {
    ORDER ord;
    int fRet = 0;
    IA *piaCur;
    int i;
    FLEET __far *lpfl;
    int idh;
    int grobj;
    int idhSav;
    idhSav = tutor.idh;
    if (id & 0x8000) {
        id &= 0x7fff;
        grobj = 2;
    } else
        grobj = 1;
    lpfl = LpflFromId(ifl);
    if (!lpfl)
        return 0;
    if (!FCheckFleetWP(ifl, iord, grobj, id, 1, iWarp))
        return 0;
    ord = lpfl->lpplord->rgord[iord];
    piaCur = ord.task.rgia;
    tutor.idh = 0x5ef;
    for (i = 0; i < 5; i++, piaCur++, lpiaGoal++) {
        if (piaCur->action != lpiaGoal->action) {
            if (piaCur->action != 0)
                TutorError(0x268);
            goto LDone;
        }
        if ((piaCur->action == 4 || piaCur->action == 8) && piaCur->quantity != lpiaGoal->quantity)
            goto LDone;
    }
    fRet = 1;
LDone:
    idh = tutor.idh;
    if (!fRet && FCheckSelection(2, ifl))
        tutor.idh = idh;
    if (fRet)
        tutor.idh = idhSav;
    return fRet;
}
