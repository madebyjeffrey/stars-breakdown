#include "TYPES.H"

int FCheckMessages(int imsg, int idm, int fFilter) {
    int idhSav;
    idhSav = tutor.idh;
    if (gd.fGeneratingTurn)
        return 1;
    tutor.idh = 0x36b1;
    if (imsg == 9999 && IMsgNext(0) != -1)
        return 0;
    else if (imsg != 9999 && imsg != -1 && iMsgCur < imsg)
        return 0;
    else if (idm != -1) {
        if (fFilter && tutor.fAutoComplete && !(bitfMsgFiltered[idm >> 3] & (1 << (idm & 7)))) {
            SetFilteringGroups(idm, 1);
            {
                tutor.idh = idhSav;
                return 1;
            };
        }
        if (fFilter && !(bitfMsgFiltered[idm >> 3] & (1 << (idm & 7))))
            return 0;
        else if (!fFilter && IdmGetMessageN(iMsgCur) != idm)
            return 0;
    }
    {
        tutor.idh = idhSav;
        return 1;
    };
}
