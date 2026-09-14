#include "TYPES.H"

int FCheckPatrolWP(unsigned ifl, int iord, int id, unsigned iWarp, unsigned iPlan, unsigned iDist) {
    int grobj;
    int idhSav;
    FLEET __far *lpfl;
    idhSav = tutor.idh;
    lpfl = LpflFromId(ifl);
    if (!lpfl)
        return 0;
    tutor.idh = 0xc17;
    grobj = (id & 0x8000) ? 2 : 1;
    if (!FCheckFleetWP(ifl, iord, grobj, id & 0x7fff, 7, iWarp))
        return 0;
    if (iDist != -1 && lpfl->lpplord->rgord[iord].task.tptl.iDist != iDist) {
        tutor.idh = 0x5ef;
        return 0;
    }
    {
        tutor.idh = idhSav;
        return 1;
    };
}
