/* UNMATCHED EXPERIMENT: production-list count view and control flow remain to resolve. */
#include "TYPES.H"

int FCheckQueue(int ipl, int iprod, unsigned grobj, unsigned iItem, unsigned cItem,
                unsigned fNoResearch) {
    int fRet;
    PLANET __far *lppl;
    PROD prod;
    int idh;
    int idhSav;
    idhSav = tutor.idh;
    fRet = 0;
    if (game.turn < 2)
        tutor.idh = 0x5e3;
    else
        tutor.idh = 0x423;
    lppl = LpplFromId(ipl);
    if (!lppl || !lppl->lpplprod || iprod >= lppl->lpplprod->iprodMac)
        goto LDone;
    prod = lppl->lpplprod->rgprod[iprod];
    if (prod.grobj != grobj || prod.iItem != iItem)
        TutorError(0x1eb);
    else if (prod.cItem != cItem)
        TutorError(0x1f0);
    else if (fNoResearch != -1 && lppl->fNoResearch != fNoResearch)
        TutorError(0x519);
    else
        fRet = 1;
LDone:
    idh = tutor.idh;
    if (!fRet && FCheckSelection(1, ipl))
        tutor.idh = idh;
    if (fRet)
        tutor.idh = idhSav;
    return fRet;
}
