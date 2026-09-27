#include "core.h"
#include <stdio.h>
#include <string.h>

/* NE 22:851E-86E3.  The report routine emits the map header and one tabular
 * line per planet, using the same CRLF text formats held in the original
 * report data segment.  File and string services are explicit so this is
 * ordinary source C rather than a replay of the old executable bytes. */
void stars_dump_universe(const StarsDumpUniverseInput *input)
{
    StarsU16 saved_silent;
    StarsU16 i;
    StarsI16 success;
    StarsU8 filename[96];
    StarsU8 line[256];
    const StarsU8 *name;
    int length;

    if (!input)
        return;
    saved_silent = input->file_error_silent ?
        *input->file_error_silent : 0U;
    success = 0;
    if (input->game_id != 0UL && input->player != -1 &&
            input->base_name && input->planets && input->planet_name &&
            input->open_stream && input->write_stream &&
            input->close_stream) {
        length = sprintf((char *)filename, "%s.map",
            (const char *)input->base_name);
        if (length >= 0 && (StarsU16)length < (StarsU16)sizeof(filename) &&
                input->open_stream(input->context, filename, 0x1012U)) {
            input->write_stream(input->context,
                (const StarsU8 *)"#\tX\tY\tName\r\n", 12U);
            success = 1;
            for (i = 0; i < input->planet_count; ++i) {
                name = input->planet_name(input->context,
                    input->planets[i].id);
                if (!name)
                    name = (const StarsU8 *)"";
                length = sprintf((char *)line, "%d\t%d\t%d\t%s\r\n",
                    (int)i + 1, (int)input->planets[i].x,
                    (int)input->planets[i].y, (const char *)name);
                if (length < 0 || (StarsU16)length >= (StarsU16)sizeof(line)) {
                    success = 0;
                    break;
                }
                input->write_stream(input->context, line, (StarsU16)length);
            }
            input->close_stream(input->context);
        }
    }
    if (input->file_error_silent)
        *input->file_error_silent = saved_silent;
    if (input->alert)
        input->alert(input->context, success ? 0x4d6U : 0x4d7U,
            success ? 0x40 : 0x10);
}
