#include "TYPES.H"

int FCheckBtlPlan(int ibp, unsigned short imdTarget, unsigned short fSpread, unsigned short fBomb,
                  unsigned short fDump, unsigned short mdUnarmed, unsigned short mdScout,
                  unsigned short mdWar, unsigned short mdBomber) {
    int idhSav;
    BTLPLAN __far *lpbtlplan;
    idhSav = tutor.idh;
    tutor.idh = 0xc21;
    if (ibp < 0 || ibp > rgcbtlplan[0]) {
        return 0;
    };
    lpbtlplan = &rglpbtlplan[0][ibp];
    {
        tutor.idh = idhSav;
        return 1;
    };
}
