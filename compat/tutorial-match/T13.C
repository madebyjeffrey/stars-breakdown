#include "TYPES.H"

int FCheckSelection(unsigned grobj, int id) {
    int idhSav;
    int fRet;
    idhSav = tutor.idh;
    if (tutor.fAutoComplete || gd.fGeneratingTurn)
        return 1;
    if (mdMsgObj == (grobj == 2 ? 2 : 1) && id == idMsgObj)
        tutor.idh = 0x36b1;
    else if (grobj == 1 && sel.grobj == 2 && sel.fl.idPlanet == id)
        tutor.idh = 0x5ea;
    else if (grobj == 2 && sel.grobj == 1 && LpflFromId(id)->idPlanet == sel.pl.id)
        tutor.idh = 0x5e6;
    else
        tutor.idh = 0x5f6;
    fRet = 0;
    switch (grobj) {
    case 2:
        fRet = sel.grobj == 2 && sel.fl.id == id;
        break;
    case 1:
        fRet = sel.grobj == 1 && sel.pl.id == id;
        break;
    }
    TRETURN(fRet);
}
