#include "TYPES.H"

int FCheckFleetName(int id, int ids) {
    FLEET __far *lpfl;
    int idhSav;
    char szT[34];
    idhSav = tutor.idh;
    tutor.idh = 0xbee;
    lpfl = LpflFromId(id);
    if (!lpfl)
        return 1;
    if (!lpfl->lpszName) {
        if (ids == -1) {
            tutor.idh = idhSav;
            return 1;
        } else
            return ids == -1;
    }
    CchGetString(ids, szT);
    if (_fstricmp(szT, lpfl->lpszName) == 0) {
        tutor.idh = idhSav;
        return 1;
    }
    TutorError(0x4cf);
    return 0;
}
