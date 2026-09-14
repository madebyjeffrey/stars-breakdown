/* Identifier lengths are retained for the verified VC152 stack layout. */
#include "TYPES.H"

int FCheckLayingWP(unsigned ifl, int iord, int id, int iYears) {
    int savedHelpIndex_;
    FLEET __far *selectedTutorialFleet_;
    int waypointKind__;
    savedHelpIndex_ = tutor.idh;
    selectedTutorialFleet_ = LpflFromId(ifl);
    if (!selectedTutorialFleet_)
        return 0;
    tutor.idh = 0x5f8;
    waypointKind__ = (id & 0x8000) ? 2 : 1;
    if (!FCheckFleetWP(ifl, iord, waypointKind__, id & 0x7fff, 6, -1))
        return 0;
    if (selectedTutorialFleet_->lpplord->rgord[iord].task.tlm.cTime != iYears)
        return 0;
    {
        tutor.idh = savedHelpIndex_;
        return 1;
    };
}
