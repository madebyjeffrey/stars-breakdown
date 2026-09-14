#include "TYPES.H"

/* These aliases preserve the identifier storage lengths used by the
 * verified VC152 compilation. The body uses descriptive local names. */
#define szTitle iylovxxuyhnouwx
#define idtT gypfbuenzwaesoiuqvbwsgfvead
#define fTaskDone sfokxexiozjf
#define rc wpbhk
#define fRedraw gsobaew

void AdvanceTutor(void) {
    char szTitle[50];
    int idtT;
    int fTaskDone;
    RECT rc;
    int fRedraw;
    fRedraw = 0;
    tutor.fChange = 0;
    idtT = tutor.idtBold;
    fTaskDone = FTutorTaskDone();
    fRedraw = (idtT != tutor.idtBold);
    if (!fTaskDone) {
        if (fRedraw)
            goto LRedraw;
        else
            goto LDone;
    }
    {
        if (!tutor.fTurnDone) {
        LAgain:
            {
                tutor.idt += 8;
                tutor.idtBold = tutor.idt;
                tutor.idsError = -1;
                tutor.fNoErrors = 1;
                tutor.fProgress = 0;
                if (FTutorTaskDone() && !tutor.fTurnDone)
                    goto LAgain;
            }
            fRedraw = 1;
            tutor.fNoErrors = 0;
        } else
            tutor.idh = 0xdb6;
        if (tutor.idt >= 640 || tutor.fTutorDone) {
            if (tutor.idsError != 0x20a)
                TutorError(0x20a);
            EndTutor(0);
            return;
        }
    }
    if (fRedraw) {
    LRedraw:
        wsprintf(szTitle, PszGetCompressedString(0x4ce), tutor.idt / 8 + 1);
        SetWindowText(tutor.hwnd, szTitle);
        ShowTutor(1);
        GetWindowRect(tutor.hwnd, &rc);
        ScreenToClient(tutor.hwnd, (POINT *)&rc);
        ScreenToClient(tutor.hwnd, (POINT *)&rc.right);
        ExpandRc(&rc, -dyArial8, -2 * dyArial8);
        InvalidateRect(tutor.hwnd, &rc, 1);
    }
LDone:;
}
