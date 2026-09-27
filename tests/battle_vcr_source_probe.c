#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsI16 allocations;
    StarsI16 frees;
    StarsI16 boards;
    StarsI16 tutor;
    StarsI16 dialog;
    StarsI16 alerts;
} Trace;
static char token_space[32]; static char damage_space[16];
static void *allocate(void *context, StarsU16 bytes, StarsU16 tag)
{
    Trace *t=(Trace*)context; (void)tag; ++t->allocations;
    return bytes > 16U ? (void*)token_space : (void*)damage_space;
}
static void free_memory(void *context, void *memory, StarsU16 tag)
{
    Trace *t=(Trace*)context; (void)memory; (void)tag; ++t->frees;
}
static StarsI16 set_board(void *context, StarsI16 step)
{
    Trace *t=(Trace*)context; ++t->boards; return step == 30000 ? 7 : 0;
}
static void tutor(void *context){Trace*t=(Trace*)context;++t->tutor;}
static void dialog(void *context){Trace*t=(Trace*)context;++t->dialog;}
static void alert(void *context, StarsI16 value)
{Trace*t=(Trace*)context;(void)value;++t->alerts;}

int main(void)
{
    StarsBattleVcrInput input; StarsBattleVcrRecord record; Trace trace;
    memset(&input,0,sizeof input); memset(&record,0,sizeof record);
    memset(&trace,0,sizeof trace); record.battle_id=9; record.token_count=1;
    record.next_record=4; input.battle_id=9; input.view_flags=0x0800;
    input.round=3; input.records=&record; input.record_count=1;
    input.allocate=allocate; input.free_memory=free_memory;
    input.set_board=set_board; input.advance_tutor=tutor;
    input.show_dialog=dialog; input.alert=alert; input.context=&trace;
    stars_battle_vcr(&input);
    if (input.board_width!=0x161 || input.board_square!=0x20 ||
            input.current_vcr_step!=6 || input.current_vcr_round!=3 ||
            trace.allocations!=2 || trace.frees!=2 || trace.boards!=2 ||
            trace.tutor!=1 || trace.dialog!=1) return 1;
    memset(&trace,0,sizeof trace); memset(&input,0,sizeof input);
    input.battle_id=10; input.view_flags=0xc000; input.records=&record;
    input.record_count=1; input.alert=alert; input.context=&trace;
    stars_battle_vcr(&input);
    if (input.board_width!=0x2a1 || input.board_square!=0x40 ||
            trace.alerts!=1 || (input.view_flags & 0x2000U)!=0U) return 2;
    puts("BATTLE_VCR_SOURCE=PASS"); return 0;
}
