/* Exercise the Windows-only progress branch against a small message shim.
 * This checks our routing/pump logic, not native window rendering or the ABI. */
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "windows.h"
int progress_open(HINSTANCE, HINSTANCE);
void progress_close(void);
void progress_show(const char *, unsigned, unsigned, unsigned long, unsigned long, const char *);
int progress_poll(void);
unsigned long progress_seconds(void);
static WNDPROC procedure;
static unsigned created, shown, updated, destroyed, dispatched, translated;
static unsigned queued, key, reenter;
static int register_ok = 1;
static DWORD now = 1000UL;
static char text[600];
DWORD GetTickCount(void) { return now; }
LRESULT DefWindowProc(HWND w, UINT m, WPARAM wp, LPARAM lp)
{ (void)w; (void)m; (void)wp; (void)lp; return 17; }
int SetWindowText(HWND w, const char *s)
{ (void)w; assert(strlen(s) < sizeof(text)); strcpy(text, s); return 1; }
HCURSOR LoadCursor(HINSTANCE i, const char *n) { (void)i; (void)n; return (HCURSOR)1; }
unsigned RegisterClass(const WNDCLASS *wc)
{ procedure = wc->lpfnWndProc; return (unsigned)register_ok; }
HWND CreateWindow(const char *c, const char *t, DWORD s, int x, int y, int w, int h,
    HWND p, HMENU m, HINSTANCE i, void *data)
{
    (void)c; (void)t; (void)s; (void)x; (void)y; (void)w; (void)h;
    (void)p; (void)m; (void)i; (void)data;
    ++created; return (HWND)&created;
}
int DestroyWindow(HWND w) { (void)w; ++destroyed; return 1; }
int ShowWindow(HWND w, int s) { (void)w; (void)s; ++shown; return 1; }
int UpdateWindow(HWND w) { (void)w; ++updated; return 1; }
HWND SetFocus(HWND w) { return w; }
int PeekMessage(MSG *m, HWND w, unsigned a, unsigned b, unsigned flags)
{
    (void)w; (void)a; (void)b;
    assert(flags == PM_REMOVE);
    if (!queued) return 0;
    --queued; memset(m, 0, sizeof(*m));
    m->message = key ? WM_KEYDOWN : 0xf; m->wParam = (WORD)key;
    return 1;
}
int TranslateMessage(const MSG *m) { (void)m; ++translated; return 1; }
LONG DispatchMessage(const MSG *m)
{
    (void)m; ++dispatched;
    if (reenter) { reenter = 0; (void)progress_poll(); }
    return 0;
}
int main(int argc, char **argv)
{
    assert(argc == 2);
    if (!strcmp(argv[1], "open-failure")) {
        register_ok = 0; assert(!progress_open(NULL, NULL)); assert(!created); return 0;
    }
    assert(progress_open(NULL, NULL));
    assert(procedure && created == 3 && shown == 1 && updated == 1);
    now = 62000UL;
    progress_show("P002.R1", 12, 11, 90UL, 0UL, "Calculate points | diagnostic detail");
    assert(strstr(text, "Scenario 12 / 88: P002.R1"));
    assert(strstr(text, "11 / 89") && strstr(text, "Elapsed: 1:01"));
    assert(strstr(text, "Calculate points") && progress_seconds() == 61UL);
    assert(!strstr(text, "diagnostic detail"));
    assert(!progress_poll());
    if (!strcmp(argv[1], "pump")) {
        queued = 100; reenter = 1;
        assert(!progress_poll());
        assert(dispatched == 64 && translated == 64 && queued == 36);
        assert(!progress_poll()); assert(dispatched == 100 && !queued);
        assert(procedure(NULL, 0xf, 0, 0) == 17);
    } else if (!strcmp(argv[1], "escape")) {
        queued = 1; key = VK_ESCAPE;
        assert(progress_poll()); assert(!dispatched && !translated);
    } else {
        unsigned msg = !strcmp(argv[1], "close") ? WM_CLOSE : WM_COMMAND;
        assert(procedure(NULL, msg, IDCANCEL, 0) == 0);
        assert(progress_poll()); assert(!destroyed);
        assert(strstr(text, "Stopping at the next checkpoint"));
    }
    progress_close(); assert(destroyed == 1);
    return 0;
}
