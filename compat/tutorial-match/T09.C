#include "TYPES.H"

void EndTutor(int fClose) {
    if (gd.fTutorial) {
        gd.fTutorial = 0;
        if (tutor.hwnd)
            DestroyWindow(tutor.hwnd);
        game.fTutorial = 0;
        if (fClose)
            RestoreGameState();
        else
            tutor.fFreeing = 1;
        memset(&tutor, 0, sizeof(tutor));
        Randomize2(GetTickCount());
    }
}
