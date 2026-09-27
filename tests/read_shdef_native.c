#include "windows.h"
#include "core.h"

void stars_stream_open(StarsU8 *data, StarsU16 name, StarsI16 mode,
    const StarsStreamApi *api, void *context)
{
    (void)data; (void)name; (void)mode; (void)api; (void)context;
}

void stars_stream_write(StarsU8 *data, const StarsU8 *input, StarsU16 count,
    const StarsStreamApi *api, void *context)
{
    (void)data; (void)input; (void)count; (void)api; (void)context;
}

void stars_write_bof(StarsU8 *data, StarsI16 player, StarsI16 type,
    StarsI16 multi, StarsTickCount ticks, StarsWriteTransfer write,
    void *context)
{
    (void)data; (void)player; (void)type; (void)multi; (void)ticks;
    (void)write; (void)context;
}

int PASCAL WinMain(HINSTANCE instance, HINSTANCE previous, LPSTR command,
    int show)
{
    (void)instance;
    (void)previous;
    (void)command;
    (void)show;
    return 0;
}
