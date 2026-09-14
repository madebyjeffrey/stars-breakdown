/* Real filesystem I/O; only UI choices and clocks are controlled for replay. */
#include "TEST.H"
#ifdef HOST_TEST
#define SEP "/"
#else
#define SEP "\\"
#endif
static FILE *data_file;
static unsigned timer_calls;

void close_data_file(void)
{ if (data_file) { fclose(data_file); data_file = NULL; } }
static StarsI16 file_open(void *ctx, const StarsU8 *name, StarsU16 mode, StarsU16 *error)
{
    char path[40];
    const char *base = strrchr((const char *)name, '\\');
    FILE *existing;
    (void)ctx;
    base = base ? base + 1 : (const char *)name;
    if (strcmp(base, filename) || data_file) fatal("Unexpected file open");
    sprintf(path, "ACTUAL" SEP "%s", filename);
    if (mode & 0x1000U) {
        existing = fopen(path, "rb");
        if (existing) { fclose(existing); fatal("Refusing to overwrite a race output"); }
        data_file = fopen(path, "wb");
        if (data_file) ++files_created;
    } else data_file = fopen(path, "rb");
    *error = data_file ? 0 : 2;
    if (!data_file) fatal("Cannot open actual race file");
    return 7;
}
static StarsI16 file_close(void *ctx, StarsI16 handle)
{
    int result;
    (void)ctx;
    if (handle != 7 || !data_file) fatal("Unexpected file close");
    result = fclose(data_file); data_file = NULL;
    if (result) fatal("Race file close failed");
    return 0;
}
static StarsU16 file_read(void *ctx, StarsI16 handle, StarsU8 *out, StarsU16 count)
{
    size_t n;
    (void)ctx;
    if (handle != 7 || !data_file) fatal("Unexpected file read");
    n = fread(out, 1, count, data_file);
    if (ferror(data_file)) fatal("Race file read failed");
    return (StarsU16)n;
}
static StarsU16 file_write(void *ctx, StarsI16 handle, const StarsU8 *in, StarsU16 count)
{
    size_t n;
    (void)ctx;
    if (handle != 7 || !data_file) fatal("Unexpected file write");
    n = fwrite(in, 1, count, data_file);
    if (n != count) fatal("Race file write failed");
    return (StarsU16)n;
}
static StarsU32 ticks(void *ctx) { (void)ctx; return 123; }
static const StarsU8 *memory(void *ctx, StarsU16 segment, StarsU16 offset)
{
    (void)ctx;
    if (segment != 0x3000U) fatal("Unexpected model selector");
    return runtime.data + offset;
}
static StarsI16 file_access(void *ctx, const StarsU8 *name, StarsI16 mode)
{ (void)ctx; (void)name; (void)mode; fatal("Unexpected access call"); return -1; }
static StarsU32 file_seek(void *ctx, StarsI16 h, StarsU32 off, StarsI16 origin)
{ (void)ctx; (void)h; (void)off; (void)origin; fatal("Unexpected seek call"); return 0; }
static void file_date(void *ctx, StarsU8 *p)
{ (void)ctx; (void)p; fatal("Unexpected date call"); }
static void file_time(void *ctx, StarsU8 *p)
{ (void)ctx; (void)p; fatal("Unexpected time call"); }
static StarsU32 timer_count(void *ctx)
{ (void)ctx; return timer_calls++ ? 2000UL : 0UL; }
static StarsI16 save_choice(void *ctx, StarsSaveChoice *c)
{
    static const char filter[] = "Stars! Race Files\0*.R*\0\0";
    (void)ctx;
    if (c->flags != 0x8806UL || c->filter_index != 1 || c->file_capacity != 256 ||
        c->title_capacity != 256 || memcmp(c->filter, filter, sizeof(filter) - 1U))
        fatal("Save-dialog contract differs");
    sprintf((char *)c->file, "C:\\%s", filename); c->file_offset = 3;
    return 1;
}
static void abort_run(void *ctx, StarsU16 token, StarsI16 value)
{
    (void)ctx;
    fprintf(report, "Unhandled recovery: token=%u value=%d\n", (unsigned)token, (int)value);
    fatal("Unhandled original recovery");
}
const StarsPlatform test_platform = {
    file_open, file_close, file_read, file_write, ticks, memory, file_access,
    file_seek, file_date, file_time, timer_count, test_window, test_dialog,
    save_choice, abort_run
};
