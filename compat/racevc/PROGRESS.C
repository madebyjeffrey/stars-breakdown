/* A real Win16 progress window, separate from the deterministic game UI.
 * Pumping messages can request cancellation, but never re-enters game code.
 * The runner observes that request at the next action/stage boundary. */
#ifndef HOST_TEST
#include <windows.h>
#endif
#include "TEST.H"
#ifdef HOST_TEST
#include <time.h>
static clock_t started;
#else
static HWND progress_window, status_text;
static DWORD started;
static int pumping;
#endif
static int cancel_requested;

unsigned long progress_seconds(void)
{
#ifdef HOST_TEST
    if (!started) started = clock();
    return (unsigned long)((clock() - started) / CLOCKS_PER_SEC);
#else
    return (GetTickCount() - started) / 1000UL;
#endif
}

#ifndef HOST_TEST
LRESULT CALLBACK ProgressWndProc(HWND window, UINT message, WPARAM wp, LPARAM lp)
{
    if (message == WM_CLOSE || (message == WM_COMMAND && wp == IDCANCEL)) {
        cancel_requested = 1;
        SetWindowText(status_text, "Stopping at the next checkpoint. Please wait...");
        return 0;
    }
    return DefWindowProc(window, message, wp, lp);
}

int progress_open(HINSTANCE instance, HINSTANCE previous)
{
    WNDCLASS wc;
    HWND button;
    started = GetTickCount();
    if (!previous) {
        memset(&wc, 0, sizeof(wc));
        wc.lpfnWndProc = ProgressWndProc;
        wc.hInstance = instance;
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);
        wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
        wc.lpszClassName = "StarsRaceProgress";
        if (!RegisterClass(&wc)) return 0;
    }
    /* The exported EXE window procedure uses the standard /GA prologue.
     * Its class is associated with this instance; no dialog thunk is needed. */
    progress_window = CreateWindow("StarsRaceProgress", "Stars race test - revision 3",
        WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
        CW_USEDEFAULT, CW_USEDEFAULT, 490, 250, NULL, NULL, instance, NULL);
    if (!progress_window) return 0;
    status_text = CreateWindow("STATIC", "Starting the race test...",
        WS_CHILD | WS_VISIBLE | SS_LEFT, 14, 12, 450, 155,
        progress_window, NULL, instance, NULL);
    button = CreateWindow("BUTTON", "Cancel", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
        190, 175, 90, 28, progress_window, (HMENU)IDCANCEL, instance, NULL);
    if (!status_text || !button) { DestroyWindow(progress_window); return 0; }
    ShowWindow(progress_window, SW_SHOWNORMAL);
    UpdateWindow(progress_window);
    SetFocus(button);
    return 1;
}

void progress_close(void)
{
    if (progress_window) DestroyWindow(progress_window);
    progress_window = status_text = NULL;
}
#endif

void progress_show(const char *file, unsigned cases, unsigned sessions,
    unsigned long checks, unsigned long failures, const char *stage)
{
#ifdef HOST_TEST
    const char *cancel_at = getenv("RACEVC_CANCEL_AT");
    (void)cases; (void)sessions; (void)checks; (void)failures; (void)stage;
    /* Host-only injection tests the same deferred cancellation/cleanup path. */
    if (cancel_at && !strcmp(cancel_at, file)) cancel_requested = 1;
#else
    char text[512];
    unsigned long elapsed = progress_seconds();
    sprintf(text, "Scenario %u / 88: %s\r\nCompleted wizard sessions: %u / 89\r\n"
        "%.*s\r\nElapsed: %lu:%02lu   Checks: %lu   Failures: %lu\r\n\r\n"
        "This can take several minutes. Cancel or Esc stops safely.\r\n"
        "The current step is saved in TRACE.TXT.",
        cases, file, sessions, (int)strcspn(stage, "|"), stage,
        elapsed / 60UL, elapsed % 60UL, checks, failures);
    SetWindowText(status_text, text);
    UpdateWindow(status_text);
#endif
}

int progress_poll(void)
{
#ifndef HOST_TEST
    MSG message;
    unsigned budget = 64;
    /* A bounded pump avoids starving the test under continuous mouse input.
     * Dispatched procedures only paint this UI or set cancel_requested. */
    if (!pumping) {
        pumping = 1;
        while (budget-- && PeekMessage(&message, NULL, 0, 0, PM_REMOVE)) {
            if (message.message == WM_QUIT ||
                (message.message == WM_KEYDOWN && message.wParam == VK_ESCAPE))
                cancel_requested = 1;
            else { TranslateMessage(&message); DispatchMessage(&message); }
        }
        pumping = 0;
    }
#endif
    return cancel_requested;
}
