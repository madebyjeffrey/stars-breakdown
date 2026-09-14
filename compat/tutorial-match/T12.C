#include "TYPES.H"

int FCheckSummary(unsigned grobj, int id) {
    int fRet;
    if (gd.fGeneratingTurn)
        return 1;
    fRet = 0;
    switch (grobj) {
    case 2:
        fRet = sel.scan.grobj == 2 && sel.scan.ifl != -1 && rglpfl[sel.scan.ifl]->id == id;
        break;
    case 1:
        fRet = sel.scan.grobj == 1 && sel.scan.idpl == id;
        break;
    case 8:
        fRet = sel.scan.grobj == 8 && (id == -1 || lpThings[sel.scan.ith].idFull == id);
        break;
    }
    if (!fRet)
        tutor.idh = 0x36d2;
    return fRet;
}
