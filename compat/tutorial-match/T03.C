#include "TYPES.H"
int FAskKillTutor(void) {
    if (game.turn >= 30 || AlertSz(PszFormatIds(0x1e9, 0), 0x2024) == 6) {
        EndTutor(1);
        return 1;
    } else
        return 0;
}
