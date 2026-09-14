/* Port of the validated WindowWorld service model. This supplies OS answers
 * and user input, never race fields, scores, or save-file bytes. No GUI claim. */
#include "TEST.H"
#define CONTROL_MAX 80
typedef struct Control {
    StarsU16 id, check, limit, combo;
    StarsU16 rect[4];
    char text[256];
} Control;
static Control *controls;
static unsigned control_count;
static StarsU16 client[4], position[2], selected, mode;
static StarsU32 bg, fg, clock_ticks;
static StarsU32 api_calls;
static StarsU16 mouse[2][9];
static unsigned mouse_count, mouse_next;
static int ended, dialog_result;

static long signed_word(StarsU16 w)
{ return w < 32768U ? (long)w : (long)w - 65536L; }
static Control *control(StarsU16 id)
{
    unsigned i;
    for (i = 0; i < control_count; ++i)
        if (controls[i].id == id) return &controls[i];
    if (control_count == CONTROL_MAX) fatal("Too many model controls");
    memset(&controls[control_count], 0, sizeof(Control));
    controls[control_count].id = id;
    controls[control_count].limit = 32767U;
    controls[control_count].rect[0] = controls[control_count].rect[1] = 20;
    controls[control_count].rect[2] = 120;
    controls[control_count].rect[3] = 40;
    return &controls[control_count++];
}
static void text_set(StarsU16 id, const char *text)
{
    if (strlen(text) >= 256U) fatal("Model text exceeds capacity");
    strcpy(control(id)->text, text);
}
void world_init(void)
{
    controls = (Control *)malloc(sizeof(Control) * CONTROL_MAX);
    if (!controls) fatal("Cannot allocate model controls");
}
void world_free(void) { free(controls); controls = NULL; }
void world_reset(void)
{
    control_count = 0; selected = 1; mode = 2; bg = fg = clock_ticks = api_calls = 0;
    position[0] = 20; position[1] = 30;
    mouse_count = mouse_next = 0;
}
static void load_resource(StarsU16 wanted)
{
    FILE *f;
    StarsU16 count, id, width, height, n, cid;
    unsigned i, j;
    Control *c;
    char text[64];
    f = fopen("RSDATA.DAT", "rb");
    if (!f) fatal("Cannot open RSDATA.DAT");
    count = read_word(f);
    for (i = 0; i < count; ++i) {
        id = read_word(f); width = read_word(f); height = read_word(f);
        n = read_word(f);
        if (id != wanted) {
            if (fseek(f, (long)n * 74L, SEEK_CUR)) fatal("Resource seek failed");
            continue;
        }
        control_count = 0;
        client[0] = client[1] = 0; client[2] = width; client[3] = height;
        for (j = 0; j < n; ++j) {
            cid = read_word(f); c = control(cid);
            c->rect[0] = read_word(f); c->rect[1] = read_word(f);
            c->rect[2] = read_word(f); c->rect[3] = read_word(f);
            read_exact(f, text, sizeof(text));
            if (!memchr(text, 0, sizeof(text))) fatal("Unterminated resource text");
            text_set(cid, text);
        }
        fclose(f); return;
    }
    fclose(f); fatal("Unknown dialog resource");
}
StarsU32 test_window(void *context, const StarsWindowCall *c)
{
    const char *n = c->operation;
    StarsU16 h = c->window, a = c->a, b = c->b;
    StarsU32 v = c->value, old;
    StarsU16 *p = (StarsU16 *)c->p;
    Control *item;
    unsigned i, len;
    long product, denominator, magnitude;
    (void)context;
    if (++api_calls > 1000000UL) fatal("Model API-call budget exhausted");
#define IS(s) (strcmp(n, s) == 0)
    if (IS("GetFocus")) return 0x1234;
    if (IS("MessageBox")) {
        ++alerts;
        fprintf(report, "  ALERT: %s\n", (char *)c->p);
        return 1;
    }
    if (IS("GetDlgItem")) return a;
    if (IS("GetClientRect") || IS("GetWindowRect")) {
        if (IS("GetClientRect")) memcpy(p, client, 8U);
        else {
            if (h == 100) memcpy(p, client, 8U);
            else memcpy(p, control(h)->rect, 8U);
            for (i = 0; i < 4; ++i) p[i] = (StarsU16)(p[i] + position[i % 2]);
        }
        return 1;
    }
    if (IS("ScreenToClient")) {
        p[0] = (StarsU16)(p[0] - position[0]);
        p[1] = (StarsU16)(p[1] - position[1]); return 1;
    }
    if (IS("GetSystemMetrics")) {
        if (a > 1) fatal("Unexpected system metric");
        return a == 0 ? 1024 : 768;
    }
    if (IS("SetWindowPos")) {
        if (h == 100) { position[0] = p[0]; position[1] = p[1]; }
        else if (!(b & 1)) {
            item = control(h);
            item->rect[0] = p[0]; item->rect[1] = p[1];
            item->rect[2] = (StarsU16)(p[0] + p[2]);
            item->rect[3] = (StarsU16)(p[1] + p[3]);
        }
        return 1;
    }
    if (IS("GetDC") || IS("BeginPaint")) return 500;
    if (IS("SelectObject")) { old = selected; selected = a; return old; }
    if (IS("SetBkMode")) { old = mode; mode = a; return old; }
    if (IS("SetBkColor")) { old = bg; bg = v; return old; }
    if (IS("SetTextColor")) { old = fg; fg = v; return old; }
    if (IS("GetTextExtent")) return (StarsU32)a * 7UL | (13UL << 16);
    if (IS("GetTextMetrics")) {
        memset(p, 0, 31U); p[0] = 13; p[1] = 10; p[2] = 3;
        p[4] = 2; p[5] = 7; p[6] = 10; p[7] = 400; return 1;
    }
    if (IS("LocalAlloc")) {
        if (b > 256U) fatal("Model local allocation exceeds workspace");
        memset(runtime.data + 0xd000U, 0, b); return 0xd000U;
    }
    if (IS("LocalFree")) return 0;
    if (IS("CreateFontIndirect")) return 0x900;
    if (IS("GlobalLock")) return 0x3000c000UL;
    if (IS("StretchDIBits")) return (StarsU32)signed_word(p[3]);
    if (IS("SetWindowText") || IS("SetDlgItemText")) {
        text_set(IS("SetDlgItemText") ? a : h, (const char *)c->p); return 1;
    }
    if (IS("GetDlgItemText")) {
        item = control(a); len = (unsigned)strlen(item->text);
        if (!b) return 0;
        if (len >= b) len = b - 1U;
        memcpy(c->p, item->text, len); ((char *)c->p)[len] = 0; return len;
    }
    if (IS("CheckRadioButton")) {
        if (b < a || b - a > 32U) fatal("Invalid radio range");
        for (i = a; i <= b; ++i) control((StarsU16)i)->check = (i == v);
        return 1;
    }
    if (IS("CheckDlgButton")) { control(a)->check = b; return 1; }
    if (IS("IsDlgButtonChecked")) return control(a)->check;
    if (IS("SendMessage")) {
        item = control(h);
        if (a == 0x401 || a == 0x40e) item->check = b;
        else if (a == 0x400 || a == 0x407) return item->check;
        else if (a == 0x403) return item->combo++;
        else fatal("Unexpected SendMessage operation");
        return 0;
    }
    if (IS("SendDlgItemMessage")) {
        if (b == 0x415) control(a)->limit = *p;
        else if (b != 0x401) fatal("Unexpected SendDlgItemMessage operation");
        return 0;
    }
    if (IS("GetCurrentTime")) { old = clock_ticks; clock_ticks += 1000UL; return old; }
    if (IS("PeekMessage")) {
        if (mouse_next == mouse_count) return 0;
        memcpy(p, mouse[mouse_next++], 18U); return 1;
    }
    if (IS("GetCursorPos")) { p[0] = p[1] = 100; return 1; }
    if (IS("EndDialog")) { ended = 1; dialog_result = (int)signed_word(a); return 1; }
    if (IS("MulDiv")) {
        product = signed_word(a) * signed_word(b);
        denominator = signed_word((StarsU16)v);
        if (!denominator) return (StarsU32)-32768L;
        /* Unsigned magnitude avoids overflow at the signed 32-bit boundary. */
        old = (StarsU32)labs(product);
        old = (old * 2UL + (StarsU32)labs(denominator)) /
              ((StarsU32)labs(denominator) * 2UL);
        magnitude = (long)old;
        if ((product < 0) != (denominator < 0)) magnitude = -magnitude;
        return (StarsU32)(magnitude < -32768L || magnitude > 32767L ? -32768L : magnitude);
    }
    if (IS("ReleaseDC") || IS("EndPaint") || IS("DeleteObject") ||
        IS("FillRect") || IS("FrameRect") || IS("TextOut") || IS("PatBlt") ||
        IS("DrawText") || IS("ExtTextOut") || IS("EnableWindow") ||
        IS("RealizePalette") || IS("SelectPalette") || IS("GlobalUnlock") ||
        IS("InvalidateRect") || IS("SetCapture") || IS("ReleaseCapture") ||
        IS("SetCursor") || IS("SetFocus") || IS("WinHelp")) return 1;
    fprintf(report, "Unsupported API: %s\n", n); fatal("Unsupported window API");
    return 0;
#undef IS
}
static void mouse_message(unsigned i, StarsU16 msg, StarsU16 x, StarsU16 y)
{
    memset(mouse[i], 0, 18U); mouse[i][0] = 100; mouse[i][1] = msg;
    mouse[i][3] = mouse[i][7] = x; mouse[i][4] = mouse[i][8] = y;
}
StarsI16 test_dialog(void *context, StarsRuntime *r, StarsU16 resource,
    StarsU16 owner, StarsDialogProc proc)
{
    Dialog *dialog;
    Action *action;
    unsigned i, j;
    StarsU16 base, x, y, l, top, right, bottom;
    StarsU32 lp;
    (void)context; (void)owner;
    if (dialog_next == dialog_count) fatal("Dialog script exhausted");
    dialog = &dialogs[dialog_next++];
    if (resource != dialog->resource) fatal("Unexpected dialog order");
    load_resource(resource); ended = 0;
    for (i = 0; i < dialog->count; ++i) {
        action = &actions[dialog->first + i];
        switch (action->kind) {
        case 0: /* Actual Windows message delivered to recovered procedure. */
            lp = (StarsU32)action->c | ((StarsU32)action->d << 16);
            proc(r, 100, action->a, action->b, lp); break;
        case 1: /* Edit-control input; recovered code reads it on page exit. */
            if (strlen(action->text) > control(action->a)->limit)
                fatal("Input exceeds edit-control limit");
            text_set(action->a, action->text); break;
        case 2:
            for (j = action->b; j <= action->c; ++j)
                control((StarsU16)j)->check = (j == action->a);
            proc(r, 100, 0x111, action->a, 0); break;
        case 3:
            control(action->a)->check = action->b;
            proc(r, 100, 0x111, action->a, 0); break;
        case 4: control(action->a)->check = action->b; break;
        case 5: /* Resolve hit coordinates from recovered layout, then queue input. */
            base = action->a == 1 ? 0x5926U : stars_word(r->data, 0x48acU);
            base = (StarsU16)(base + action->b * 8U);
            l = stars_word(r->data, base); top = stars_word(r->data, (StarsU16)(base + 2U));
            right = stars_word(r->data, (StarsU16)(base + 4U));
            bottom = stars_word(r->data, (StarsU16)(base + 6U));
            x = (StarsU16)(((StarsU32)l + right) / 2UL);
            y = (StarsU16)(((StarsU32)top + bottom) / 2UL);
            mouse_count = mouse_next = 0;
            if (action->d) mouse_message(mouse_count++, 0x200,
                (StarsU16)(action->d == 1 ? l - 40U : right + 40U), y);
            mouse_message(mouse_count++, 0x202, x, y);
            proc(r, 100, 0x201, action->c ? 4 : 0, (StarsU32)x | ((StarsU32)y << 16));
            if (mouse_next != mouse_count) fatal("Unconsumed mouse input");
            break;
        default: fatal("Unknown input action");
        }
        if (ended) return (StarsI16)dialog_result;
    }
    fatal("Dialog script ended without EndDialog"); return 0;
}
