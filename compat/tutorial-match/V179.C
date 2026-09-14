/* Identifier lengths are retained for the verified VC152 stack layout. */
#include "TYPES.H"

int FCheckFleetWP(unsigned ifl, int iord, unsigned grobj, int id, unsigned grTask, unsigned iWarp) {
    int helpIndexBeforeSelection___;
    int savedHelpIndexBeforeFleetCheck;
    int matched_ = 0;
    ORDER waypointOrder;
    FLEET __far *selectedFleetForWaypoint__;
    savedHelpIndexBeforeFleetCheck = tutor.idh;
    selectedFleetForWaypoint__ = LpflFromId(ifl);
    tutor.idh = 0x5f6;
    if (!selectedFleetForWaypoint__)
        goto LDone;
    tutor.idh = 0xbf6;
    if (selectedFleetForWaypoint__->cord < iord + 1)
        goto LDone;
    waypointOrder = selectedFleetForWaypoint__->lpplord->rgord[iord];
    if ((id & 0x7fff) != 0x7fff && (waypointOrder.grobj != grobj || waypointOrder.id != id)) {
        TutorError(0x1ec);
        tutor.idh = 0xbf7;
    } else if (waypointOrder.grTask != grTask && grTask != -1) {
        tutor.idh = 0x5ef;
        if (waypointOrder.grTask != 0)
            TutorError(grTask == 0 ? 0x1ed : 0x1ee);
    } else {
        tutor.idh = 0x5ee;
        if (iWarp != -1)
            matched_ = waypointOrder.iWarp == iWarp;
        else
            matched_ = 1;
    }
LDone:
    helpIndexBeforeSelection___ = tutor.idh;
    if (!matched_ && FCheckSelection(2, ifl))
        tutor.idh = helpIndexBeforeSelection___;
    if (matched_)
        tutor.idh = savedHelpIndexBeforeFleetCheck;
    return matched_;
}
