#include "TYPES.H"

int FCheckPlanetRoute(int idpl, int idplRoute) {
    int idhSav;
    PLANET __far *lppl;
    idhSav = tutor.idh;
    lppl = LpplFromId(idpl);
    if (!lppl)
        return 0;
    tutor.idh = 0x5fb;
    if (lppl->idRoute != idplRoute + 1) {
        return 0;
    };
    {
        tutor.idh = idhSav;
        return 1;
    };
}
