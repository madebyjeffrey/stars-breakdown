#include "TYPES.H"

int FCheckScanner(int md, int iZoom) {
    int idhSav;
    idhSav = tutor.idh;
    if (tutor.fAutoComplete)
        return 1;
    tutor.idh = 0x36b8;
    if (md != -1 && grbitScan != md) {
        if (md < 6) {
            if ((grbitScan & 15) != md) {
                return 0;
            }
        } else if ((md & grbitScan) != md) {
            return 0;
        }
    }
    if (iZoom != -1 && iZoom != iScanZoom) {
        tutor.idh = 0x36c6;
        return 0;
    }
    tutor.idh = idhSav;
    return 1;
}
