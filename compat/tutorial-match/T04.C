#include "TYPES.H"
int FOKMergeDialog(void) {
    switch (game.turn) {
    default:
        TutorError(0x204);
        return 0;
    case 23:
        if (vrgiflMerge[0] != -1 && vrgiflMerge[1] != -1 && vrgiflMerge[2] == -1 &&
            vrgiflMerge[3] == -1)
            return 1;
        break;
    case 27:
        if (vrgiflMerge[0] == -1 && vrgiflMerge[1] == -1 && vrgiflMerge[2] != -1 &&
            vrgiflMerge[3] != -1)
            return 1;
        break;
    }
    TutorError(0x202);
    tutor.idh = 0x453;
    return 0;
}
