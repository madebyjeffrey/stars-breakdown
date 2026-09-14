/* Build this same runner with VC++ 1.52c /AL or the host C89 compiler.
 * Expected results are used only after recovered code has run. */
#ifndef HOST_TEST
#include <windows.h>
#include <dos.h>
#include <direct.h>
#endif
#include "TEST.H"
#ifdef HOST_TEST
#define SEP "/"
#else
#define SEP "\\"
#endif
StarsRuntime runtime;
FILE *report;
char filename[13];
Action *actions;
Dialog dialogs[DIALOG_MAX];
unsigned dialog_count, dialog_next, alerts, files_created;
unsigned long checks, failures;
jmp_buf fatal_jump;
static StarsU8 *images[3];
#ifndef HOST_TEST
static HGLOBAL handles[3];
#endif
static FILE *vectors;
static unsigned sessions, cases, load_ok, load_rejected;
static char case_id[5];
static int cancelled;
static unsigned long checkpoint_count;
#ifndef HOST_TEST
int progress_open(HINSTANCE instance, HINSTANCE previous);
void progress_close(void);
#endif

void runner_yield(void) { (void)progress_poll(); }

void checkpoint(const char *stage)
{
    FILE *trace;
    /* Close each checkpoint so DOS updates the file length even if the next
     * callback never returns. fflush alone left an open file invisible to
     * host-side DOSBox inspection in the revision-2 replay. */
    trace = fopen("TRACE.TXT", "a");
    if (!trace) fatal("Cannot append TRACE.TXT");
    ++checkpoint_count;
    fprintf(trace, "%lu t=%lus case=%s completed=%u checks=%lu failures=%lu %s\n",
        checkpoint_count, progress_seconds(), filename, sessions, checks, failures, stage);
    if (fclose(trace)) fatal("Cannot finish TRACE.TXT checkpoint");
    if (fflush(report)) fatal("Cannot flush RESULTS.TXT");
    progress_show(filename, cases, sessions, checks, failures, stage);
    if (progress_poll()) {
        cancelled = 1;
        fprintf(report, "CANCELLED at %s: %s\n", filename, stage);
        longjmp(fatal_jump, 2);
    }
}

void fatal(const char *message)
{
    if (report) { fprintf(report, "FATAL: %s\n", message); fflush(report); }
    ++failures; longjmp(fatal_jump, 1);
}
void check(const char *label, long actual, long expected)
{
    ++checks;
    if (actual != expected) {
        ++failures;
        fprintf(report, "  FAIL %s actual=%ld expected=%ld\n", label, actual, expected);
    }
}
void read_exact(FILE *f, void *p, unsigned count)
{ if (fread(p, 1, count, f) != count) fatal("Truncated test input"); }
StarsU16 read_word(FILE *f)
{
    unsigned char b[2];
    read_exact(f, b, 2); return (StarsU16)((StarsU16)b[0] | ((StarsU16)b[1] << 8));
}
static long signed_word(StarsU16 w)
{ return w < 32768U ? (long)w : (long)w - 65536L; }
static void image_read(unsigned n, const char *path)
{
    FILE *f;
    f = fopen(path, "rb");
    if (!f) fatal("Cannot open initial segment image");
    /* A Win16 size_t cannot express 65536. Neither operation crosses offset FFFF. */
    read_exact(f, images[n], 32768U);
    read_exact(f, images[n] + 32768U, 32768U);
    if (fgetc(f) != EOF) fatal("Initial segment has unexpected size");
    fclose(f);
}
static void allocate_images(void)
{
    unsigned i;
    for (i = 0; i < 3; ++i) {
#ifdef HOST_TEST
        images[i] = (StarsU8 *)malloc(65536UL);
#else
        handles[i] = GlobalAlloc(GMEM_FIXED | GMEM_ZEROINIT, 65536UL);
        if (handles[i]) images[i] = (StarsU8 *)GlobalLock(handles[i]);
        if (images[i] && FP_OFF(images[i]) != 0) fatal("Segment image does not start at offset zero");
#endif
        if (!images[i]) fatal("Cannot allocate 64 KiB segment image");
    }
    image_read(1, "STRINGS.BIN"); image_read(2, "PLANETS.BIN");
    actions = (Action *)malloc(sizeof(Action) * ACTION_MAX);
    if (!actions) fatal("Cannot allocate scenario actions");
    world_init();
}
static void release_images(void)
{
    unsigned i;
    world_free(); free(actions); actions = NULL;
    for (i = 0; i < 3; ++i) {
#ifdef HOST_TEST
        free(images[i]);
#else
        if (handles[i]) { GlobalUnlock(handles[i]); GlobalFree(handles[i]); }
#endif
        images[i] = NULL;
    }
}
static void read_dialogs(void)
{
    unsigned i, j, total = 0;
    Action *a;
    dialog_count = read_word(vectors); dialog_next = 0;
    if (dialog_count > DIALOG_MAX) fatal("Too many scripted dialogs");
    for (i = 0; i < dialog_count; ++i) {
        dialogs[i].resource = read_word(vectors);
        dialogs[i].count = read_word(vectors);
        dialogs[i].first = (StarsU16)total;
        if (dialogs[i].count > ACTION_MAX - total) fatal("Too many input actions");
        for (j = 0; j < dialogs[i].count; ++j) {
            a = &actions[total++];
            a->kind = read_word(vectors); a->a = read_word(vectors);
            a->b = read_word(vectors); a->c = read_word(vectors);
            a->d = read_word(vectors); a->e = read_word(vectors);
            read_exact(vectors, a->text, 32U);
            if (!memchr(a->text, 0, 32U)) fatal("Unterminated action text");
            if (a->kind == 2 && (a->b > a->c || a->c - a->b > 32U))
                fatal("Invalid input radio range");
        }
    }
}
static void compare_player(const StarsU8 *expected)
{
    FILE *state;
    char path[40], *dot;
    unsigned i, differences = 0;
    sprintf(path, "ACTUAL" SEP "%s", filename);
    dot = strrchr(path, '.');
    if (!dot) fatal("Missing output extension");
    strcpy(dot, ".STA");
    state = fopen(path, "rb");
    if (state) { fclose(state); fatal("Refusing to overwrite player-state output"); }
    state = fopen(path, "wb");
    if (!state) fatal("Cannot save player-state output");
    if (fwrite(images[0] + 0x4f9aU, 1, 192U, state) != 192U)
        fatal("Cannot write player-state output");
    if (fclose(state)) fatal("Cannot close player-state output");
    for (i = 0; i < 192; ++i) if (images[0][0x4f9aU + i] != expected[i]) {
        if (differences < 8) fprintf(report, "  PLAYER +%u actual=%u expected=%u\n",
            i, (unsigned)images[0][0x4f9aU + i], (unsigned)expected[i]);
        ++differences;
    }
    check("192-byte player", differences, 0);
}
static void compare_file(void)
{
    FILE *actual, *expected;
    char path[40];
    int a, e;
    unsigned long offset = 0, differences = 0;
    sprintf(path, "ACTUAL" SEP "%s", filename); actual = fopen(path, "rb");
    sprintf(path, "EXPECT" SEP "%s", filename); expected = fopen(path, "rb");
    if (!actual || !expected) fatal("Cannot open files for comparison");
    do {
        a = fgetc(actual); e = fgetc(expected);
        if (a != e) {
            if (differences < 8) fprintf(report, "  FILE +%lu actual=%d expected=%d\n", offset, a, e);
            ++differences;
        }
        ++offset;
    } while (a != EOF || e != EOF);
    if (ferror(actual) || ferror(expected)) fatal("Comparison read failed");
    fclose(actual); fclose(expected);
    check("entire race file, including salt and checksum", (long)differences, 0);
}
static void run_session(void)
{
    StarsU8 player[192];
    StarsU16 fresh, result, points, load_result, alert_count;
    StarsI16 actual, loaded = -1, actual_points;
    unsigned long before;
    read_exact(vectors, case_id, 5U); read_exact(vectors, filename, 13U);
    if (case_id[4] || filename[12] || strchr(filename, '\\') || strchr(filename, '/'))
        fatal("Invalid scenario name");
    fresh = read_word(vectors); result = read_word(vectors); points = read_word(vectors);
    load_result = read_word(vectors); alert_count = read_word(vectors);
    read_exact(vectors, player, 192U); read_dialogs();
    if (fresh) {
        image_read(0, "INDATA.BIN"); world_reset(); ++cases;
        stars_runtime_init(&runtime, images[0], images[1], &test_platform, NULL, 0xe000U, 0xff00U);
        runtime.segments[9] = images[2];
    }
    before = failures; alerts = files_created = 0;
    fprintf(report, "RUN %s %s\n", case_id, filename); fflush(report);
    checkpoint("Run recovered race wizard");
    actual = fresh ? stars_new_race(&runtime, 200) : stars_race_wizard(&runtime, 200, 0, 0);
    checkpoint("Wizard returned; calculate final advantage points");
    check("wizard result", actual, signed_word(result));
    actual_points = stars_race_advantage_points(images[0], 0x4f9aU);
    checkpoint("Compare player state and wizard outcome");
    check("advantage points", actual_points, signed_word(points));
    compare_player(player);
    check("wizard alerts", alerts, alert_count);
    check("scratch scope restored", runtime.scratch_top, 0xff00L);
    check("script complete (optional final Cancel may remain)", dialog_count - dialog_next > 1, 0);
    check("file count", files_created, load_result == 0xffffU ? 0 : 1);
    if (load_result != 0xffffU) {
        checkpoint("Compare newly saved race file");
        compare_file();
        strcpy((char *)images[0] + 0x3000U, filename);
        checkpoint("Reload newly saved race file");
        loaded = stars_was_race_file(&runtime, 0x3000U, 0);
        checkpoint("Check reload result");
        check("reload result", loaded, signed_word(load_result));
        check("reload scratch scope restored", runtime.scratch_top, 0xff00L);
        if (loaded == 1) ++load_ok; else ++load_rejected;
    }
    close_data_file(); ++sessions;
    fprintf(report, "%s %s points=%ld load=%ld\n", failures == before ? "PASS" : "FAIL",
        filename, (long)actual_points, (long)loaded); fflush(report);
    checkpoint("Session complete");
}
static int run_tests(void)
{
    FILE *existing;
    unsigned count, i;
    char magic[8];
    existing = fopen("RESULTS.TXT", "rb");
    if (existing) { fclose(existing); return 2; }
    existing = fopen("TRACE.TXT", "rb");
    if (existing) { fclose(existing); return 2; }
    report = fopen("RESULTS.TXT", "w");
    if (!report) return 3;
    fprintf(report, "STARS RACE COMPILER/OUTPUT ACCEPTANCE 3\n");
#ifdef HOST_TEST
    fprintf(report, "MODE=HOST_C89; native VC152 execution remains pending\n");
#else
    fprintf(report, "MODE=WIN16_LARGE; MSC=%u\n", (unsigned)_MSC_VER);
#endif
    fprintf(report, "UI=MODELED; game callbacks=recovered C; file I/O=real CRT\n");
    fprintf(report, "Expected data=original x86 observations with fixed inputs\n");
    if (!setjmp(fatal_jump)) {
        checkpoint("Initialize segment images and scenario inputs");
        check("short width", sizeof(short), 2); check("StarsU32 width", sizeof(StarsU32), 4);
#ifndef HOST_TEST
        check("int width", sizeof(int), 2); check("long width", sizeof(long), 4);
        check("data pointer width", sizeof(void *), 4);
        check("callback pointer width", sizeof(StarsDialogProc), 4);
        if (failures) fatal("Unsupported native ABI; use the supplied large-model build");
#endif
        allocate_images();
        vectors = fopen("CASES.DAT", "rb");
        if (!vectors) fatal("Cannot open CASES.DAT");
        read_exact(vectors, magic, 8U);
        if (memcmp(magic, "RACEVC1\0", 8U)) fatal("Wrong scenario format");
        count = read_word(vectors);
        for (i = 0; i < count; ++i) run_session();
        if (fgetc(vectors) != EOF) fatal("Trailing scenario data");
        check("completed scenarios", cases, 88); check("completed wizard sessions", sessions, 89);
        check("accepted reloads", load_ok, 60); check("predicted rejected reloads", load_rejected, 1);
    }
    if (vectors) { fclose(vectors); vectors = NULL; }
    close_data_file(); release_images();
    fprintf(report, "CASES=%u SESSIONS=%u CHECKS=%lu FAILURES=%lu\n", cases, sessions, checks, failures);
    fprintf(report, "ELAPSED_SECONDS=%lu CHECKPOINTS=%lu\n", progress_seconds(), checkpoint_count);
    fprintf(report, "STATUS=%s\n", cancelled ? "CANCELLED" : failures ? "FAIL" : "PASS");
    if (fclose(report)) { report = NULL; return 4; }
    report = NULL;
    return cancelled ? 5 : failures ? 1 : 0;
}
#ifdef HOST_TEST
int main(void) { return run_tests(); }
#else
int PASCAL WinMain(HINSTANCE instance, HINSTANCE previous, LPSTR command, int show)
{
    int result;
    char message[256], *slash;
    (void)command; (void)show;
    /* Always write beside the executable, independent of File Manager's cwd. */
    if (!GetModuleFileName(instance, message, sizeof(message))) return 3;
    slash = strrchr(message, '\\');
    if (slash) {
        *slash = 0;
        if (message[1] == ':') _chdrive((int)(message[0] & 31));
        /* Use the CRT entry directly; /NOD does not link old-name aliases. */
        if (_chdir(message)) return 3;
    }
    if (!progress_open(instance, previous)) {
        MessageBox(NULL, "Cannot create the test progress window. No tests were started.",
            "Stars race test", MB_OK | MB_ICONEXCLAMATION);
        return 6;
    }
    result = run_tests();
    progress_close();
    if (result == 2) strcpy(message, "RESULTS.TXT or TRACE.TXT already exists. Preserve this folder and run a fresh copy.");
    else if (result == 3 || result == 4) strcpy(message, "Cannot create or finish RESULTS.TXT in the package folder.");
    else sprintf(message, "%s\n%u / 89 sessions completed, %lu checks, %lu failures.\nElapsed: %lu seconds.\nReturn the entire folder, including RESULTS.TXT, TRACE.TXT and ACTUAL.",
        result == 5 ? "CANCELLED - partial results preserved." : result ? "FAIL - see RESULTS.TXT." : "PASS - all 88 scenarios completed.",
        sessions, checks, failures, progress_seconds());
    MessageBox(NULL, message, "Stars race compiler/output acceptance", MB_OK | (result ? MB_ICONEXCLAMATION : MB_ICONINFORMATION));
    return result;
}
#endif
