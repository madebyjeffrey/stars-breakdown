#ifndef TARGET_NATIVE
#include <stdio.h>
#endif
#include <string.h>
#include "core.h"

#define PLAYER_COUNT 3U

typedef struct Trace {
    StarsU16 count;
    StarsU16 types[PLAYER_COUNT];
    StarsU16 lengths[PLAYER_COUNT];
    StarsU8 records[PLAYER_COUNT][STARS_SCOREX_RECORD_BYTES];
    StarsU16 failures;
} Trace;

static void capture(void *opaque, StarsU16 type, StarsU16 length,
    const StarsU8 *record)
{
    Trace *trace;
    trace = (Trace *)opaque;
    if (trace->count >= PLAYER_COUNT) {
        ++trace->failures;
        return;
    }
    trace->types[trace->count] = type;
    trace->lengths[trace->count] = length;
    memcpy(trace->records[trace->count], record, length);
    ++trace->count;
}

static int check_case(Trace *trace, const StarsWriteScoreRecordsInput *input,
    const StarsU16 *expected_players, StarsU16 expected_count,
    const StarsU8 *source_records)
{
    StarsU16 i;
    if (!stars_write_score_records(input, capture, trace)) return 0;
    if (trace->count != expected_count || trace->failures != 0U) return 0;
    for (i = 0U; i < expected_count; ++i) {
        if (trace->types[i] != 0x2dU ||
                trace->lengths[i] != STARS_SCOREX_RECORD_BYTES ||
                memcmp(trace->records[i], source_records +
                    expected_players[i] * STARS_SCOREX_RECORD_BYTES,
                    STARS_SCOREX_RECORD_BYTES) != 0)
            return 0;
    }
    return 1;
}

int main(void)
{
    StarsWriteScoreRecordsInput input;
    Trace trace;
    StarsU8 records[PLAYER_COUNT][STARS_SCOREX_RECORD_BYTES];
    StarsU8 visible[PLAYER_COUNT];
    StarsU8 dead[PLAYER_COUNT];
    static const StarsU16 expected_visible_dead[] = {0U, 1U, 2U};
    static const StarsU16 expected_self[] = {1U};
    static const StarsU16 expected_victory[] = {0U, 1U, 2U};
    StarsU16 i;
    StarsU16 j;
    StarsU16 checks;

    checks = 0U;
    memset(records, 0, sizeof records);
    for (i = 0U; i < PLAYER_COUNT; ++i)
        for (j = 0U; j < STARS_SCOREX_RECORD_BYTES; ++j)
            records[i][j] = (StarsU8)(i * 37U + j);
    visible[0] = 0U;
    visible[1] = 1U;
    visible[2] = 0U;
    dead[0] = 0U;
    dead[1] = 0U;
    dead[2] = 1U;
    memset(&input, 0, sizeof input);
    memset(&trace, 0, sizeof trace);
    input.records = &records[0][0];
    input.visible_players = visible;
    input.dead_players = dead;
    input.player_count = PLAYER_COUNT;
    input.file_player = 0;
    if (!check_case(&trace, &input, expected_visible_dead, 3U,
            &records[0][0])) return 1;
    ++checks;

    memset(&trace, 0, sizeof trace);
    input.visible_players = 0;
    input.dead_players = 0;
    input.file_player = 1;
    if (!check_case(&trace, &input, expected_self, 1U,
            &records[0][0])) return 2;
    ++checks;

    memset(&trace, 0, sizeof trace);
    input.file_player = -1;
    input.game_flags = 0x0040U;
    input.turn = 20U;
    if (!stars_write_score_records(&input, capture, &trace) ||
            trace.count != 0U) return 3;
    ++checks;

    memset(&trace, 0, sizeof trace);
    input.file_player = 1;
    if (!check_case(&trace, &input, expected_victory, 3U,
            &records[0][0])) return 4;
    ++checks;

    memset(&trace, 0, sizeof trace);
    input.turn = 19U;
    if (!check_case(&trace, &input, expected_self, 1U,
            &records[0][0])) return 5;
    ++checks;

    memset(&trace, 0, sizeof trace);
    input.file_player = -2;
    if (stars_write_score_records(&input, capture, &trace) != 0 ||
            trace.count != 0U) return 6;
    ++checks;

#ifndef TARGET_NATIVE
    printf("WRITE_SCORE_CHECKS=%u FAILURES=0 RESULT=PASS\n",
        (unsigned)checks);
#endif
    return 0;
}
