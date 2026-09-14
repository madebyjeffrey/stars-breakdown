#include "TYPES.H"

int FCheckColonizeWP(unsigned ifl, int id, unsigned iWarp) {
    int csh;
    int ish;
    int idhSav;
    FLEET __far *lpfl;
    idhSav = tutor.idh;
    lpfl = LpflFromId(ifl);
    if (!lpfl)
        return 0;
    tutor.idh = 0x5f2;
    csh = 0;
    for (ish = 0; ish < 16; ish++)
        csh += lpfl->rgcsh[ish];
    if (lpfl->idPlanet == 13 && !FCheckCargo(lpfl, 0, 0, 0, csh * 25))
        return 0;
    if (FCheckFleetWP(ifl, 1, 1, id, 2, iWarp)) {
        tutor.idh = idhSav;
        return 1;
    }
    return 0;
}
