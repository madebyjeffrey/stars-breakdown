/* A plain Win16 application; HOST_TEST runs only the portable checks locally. */
#ifndef HOST_TEST
#include <windows.h>
#endif
#include <stdio.h>
#include <stddef.h>
#include "PROBE.H"

static FILE *report;
static unsigned checks, failures;
#ifndef LAYOUT_PROBE
static ProbeCallback callback = probe_pascal;
#endif

#ifdef LAYOUT_PROBE
#define REPORT_NAME "LAYRES.TXT"
#define REPORT_KIND "LAYOUT"
#else
#define REPORT_NAME "RESULTS.TXT"
#define REPORT_KIND "UTILITY"
#endif

#pragma pack(1)
struct PackedProbe { StarsU8 byte; StarsU16 word; StarsU32 dword; };
#pragma pack()

void check_u32(const char *name, unsigned index, StarsU32 actual, StarsU32 expected)
{
    ++checks;
    if (actual != expected) {
        ++failures;
        fprintf(report, "FAIL %s[%u] actual=%08lX expected=%08lX\n",
            name, index, (unsigned long)actual, (unsigned long)expected);
    }
}

static void check_platform(void)
{
    check_u32("CHAR_BIT", 0, CHAR_BIT, 8);
    check_u32("StarsU16", 0, sizeof(StarsU16), 2);
    check_u32("StarsU32", 0, sizeof(StarsU32), 4);
    check_u32("StarsSeeds", 0, sizeof(StarsSeeds), 8);
    check_u32("StarsRect", 0, sizeof(StarsRect), 8);
    check_u32("packed.size", 0, sizeof(struct PackedProbe), 7);
    check_u32("packed.word", 0, offsetof(struct PackedProbe, word), 1);
    check_u32("packed.dword", 0, offsetof(struct PackedProbe, dword), 3);
#ifdef HOST_TEST
    fprintf(report, "MODE=HOST_ONLY (no Win16 ABI or native layout validation)\n");
#else
    fprintf(report, "MODE=WIN16_MEDIUM\n_MSC_VER=%u\n_M_I86=%u\n",
        (unsigned)_MSC_VER, (unsigned)_M_I86);
    fprintf(report, "_M_I86MM=%u\n", (unsigned)_M_I86MM);
    fprintf(report, "sizeof(int)=%u sizeof(long)=%u sizeof(void*)=%u\n",
        (unsigned)sizeof(int), (unsigned)sizeof(long), (unsigned)sizeof(void *));
    fprintf(report, "near_data=%u far_data=%u huge_data=%u default_function=%u\n",
        (unsigned)sizeof(void __near *), (unsigned)sizeof(void __far *),
        (unsigned)sizeof(void __huge *), (unsigned)sizeof(void (*)(void)));
    check_u32("int", 0, sizeof(int), 2);
    check_u32("long", 0, sizeof(long), 4);
    check_u32("float", 0, sizeof(float), 4);
    check_u32("double", 0, sizeof(double), 8);
    check_u32("default.data.pointer", 0, sizeof(void *), 2);
    check_u32("near.data.pointer", 0, sizeof(void __near *), 2);
    check_u32("far.data.pointer", 0, sizeof(void __far *), 4);
    check_u32("huge.data.pointer", 0, sizeof(void __huge *), 4);
    check_u32("default.function.pointer", 0, sizeof(void (*)(void)), 4);
    check_u32("near.function.pointer", 0, sizeof(void (__near *)(void)), 2);
    check_u32("pascal.callback.pointer", 0, sizeof(ProbeCallback), 4);
#endif
}

#ifndef LAYOUT_PROBE
static void check_calls(void)
{
    unsigned i;
    /* Repeated mixed-width calls exercise cleanup as well as returned DX:AX. */
    for (i = 0; i != 64; ++i) {
        check_u32("far.cdecl", i, probe_cdecl(0xabcdU, 0x12345678UL, 0xf1U),
            0xb9f95769UL);
        check_u32("far.pascal", i, probe_pascal(0xabcdU, 0x12345678UL, 0xf1U),
            0xb9f95587UL);
        check_u32("far.pascal.indirect", i, callback(0xabcdU, 0x12345678UL, 0xf1U),
            0xb9f95587UL);
        check_u32("near.cdecl", i, probe_near_wrapper(0xabcdU, 0x12345678UL, 0xf1U),
            0xb9f95689UL);
    }
}
#endif

static int run_probe(void)
{
    int write_failed;
    report = fopen(REPORT_NAME, "w");
    if (!report)
        return 2;
    fprintf(report, "STARS_VC152_PROBE=2\nKIND=" REPORT_KIND "\nBUILD=%s %s\n", __DATE__, __TIME__);
    check_platform();
#ifdef LAYOUT_PROBE
    run_layouts();
#else
    check_calls();
    run_vectors();
#endif
    fprintf(report, "CHECKS=%u\nFAILURES=%u\nRESULT=%s\n", checks, failures,
        failures ? "FAIL" : "PASS");
    write_failed = ferror(report);
    if (fclose(report))
        write_failed = 1;
    return write_failed ? 2 : failures ? 1 : 0;
}

#ifdef HOST_TEST
int main(void)
{
    return run_probe();
}
#else
int PASCAL WinMain(HINSTANCE instance, HINSTANCE previous, LPSTR command, int show)
{
    int result;
    (void)instance;
    (void)previous;
    (void)command;
    (void)show;
    result = run_probe();
    MessageBox(NULL, result == 2 ? "Could not write " REPORT_NAME " in the current folder."
        : result ? "Some tests failed. Return " REPORT_NAME " and the build outputs."
        : "All runtime tests passed. Return " REPORT_NAME " and the build outputs.",
        "Stars compatibility: " REPORT_KIND, MB_OK | (result ? MB_ICONEXCLAMATION : MB_ICONINFORMATION));
    return result;
}
#endif
