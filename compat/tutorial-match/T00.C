#include "TYPES.H"

void ShowTutor(int fShow) {
    if (!tutor.hwnd)
        return;
    ShowWindow(tutor.hwnd, fShow ? 5 : 0);
    tutor.fVisible = fShow;
}
