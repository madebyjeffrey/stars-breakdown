#include "TYPES.H"

void TutorError(int idsError) {
    if (tutor.fNoErrors) {
        tutor.idsError = -1;
        return;
    }
    if (tutor.idsError != idsError || tutor.cError++ >= 3) {
        tutor.cError = 0;
        tutor.idsError = idsError;
        AlertSz(PszFormatIds(idsError, 0), 0x10);
    }
}
