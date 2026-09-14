#include "TYPES.H"

int FCheckBuilderPart(int iSlot, HS *phs, unsigned cInit) {
    int idhSav;
    unsigned cItemAct;
    idhSav = tutor.idh;
    if (!hwndSlotDlg) {
        tutor.idh = 0x42a;
        return 0;
    }
    tutor.idh = 0xbe0;
    if (mdBuild != 4)
        return 0;
    cItemAct = lpshdefBuild->rghs[iSlot].cItem;
    if (phs->cItem == 0 && cItemAct == 0)
        return 1;
    if (cInit < phs->cItem) {
        if (cItemAct < cInit || cItemAct > phs->cItem) {
        LCount:
            TutorError(0x1f5);
        LError:
            tutor.idh = 0xbdf;
            return 0;
        }
    } else if (cItemAct < phs->cItem || cItemAct > cInit)
        goto LCount;
    if (cItemAct != phs->cItem)
        goto LError;
    if (phs->grhst != lpshdefBuild->rghs[iSlot].grhst ||
        phs->iItem != lpshdefBuild->rghs[iSlot].iItem) {
        TutorError(0x1f6);
        goto LError;
    }
    tutor.idh = idhSav;
    return 1;
}
