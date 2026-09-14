#include "TYPES.H"

int FCheckShipBuilder(int iCategory, int iShip) {
    int idhSav;
    int iSel;
    idhSav = tutor.idh;
    tutor.idh = 0x42a;
    if (!hwndSlotDlg) {
        return 0;
    };
    if (iCategory != -1 && iCategory != mdBuild) {
        return 0;
    };
    iSel = (int)SendMessage(GetDlgItem(hwndSlotDlg, 0x81a), 0x407, 0, 0L);
    if (iShip == -1 || iShip == iSel) {
        tutor.idh = idhSav;
        return 1;
    };
    return 0;
}
