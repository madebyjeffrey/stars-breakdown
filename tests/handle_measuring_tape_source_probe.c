#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct TapeState {
    StarsPoint moves[3];
    StarsU16 move_index;
    StarsU16 dc, gets, selected, rops, captures, released_capture, released_dc;
    StarsU16 lines, bars, empty_bars, nearest_calls, last_mask;
    StarsI16 last_id;
    StarsU8 last_label[20];
} TapeState;

static StarsI16 mouse_move(void *context, StarsPoint *point)
{
    TapeState *state = (TapeState *)context;
    if (state->move_index >= 3U ||
            state->moves[state->move_index].x == -1)
        return 0;
    *point = state->moves[state->move_index++];
    return 1;
}
static void identity(void *context, StarsPoint *point)
{ (void)context; (void)point; }
static StarsI16 nearest(void *context, StarsPoint point, StarsU16 mask,
    StarsWayPointScan *scan)
{
    TapeState *state = (TapeState *)context;
    ++state->nearest_calls;
    state->last_mask = mask;
    if (point.x == 30 && point.y == 0) {
        scan->grobj = 1U;
        scan->idpl = 7;
        scan->point = point;
        return 1;
    }
    return 0;
}
static StarsU16 get_dc(void *context, StarsU16 window)
{ TapeState *s=(TapeState *)context; s->dc=(StarsU16)(window+1); ++s->gets; return s->dc; }
static StarsU16 select_pen(void *context, StarsU16 dc, StarsU16 pen)
{ TapeState *s=(TapeState *)context; s->selected+=(dc==21&&pen==0); return 33; }
static StarsU16 set_rop(void *context, StarsU16 dc, StarsU16 mode)
{ TapeState *s=(TapeState *)context; s->rops+=(dc==21&&(mode==7||mode==2)); return mode==7?2:0; }
static void capture(void *context, StarsU16 window)
{ TapeState *s=(TapeState *)context; s->captures+=(window==20); }
static void release_capture(void *context)
{ ++((TapeState *)context)->released_capture; }
static void release_dc(void *context, StarsU16 window, StarsU16 dc)
{ TapeState *s=(TapeState *)context; s->released_dc+=(window==20&&dc==21); }
static void move_to(void *context, StarsU16 dc, StarsI16 x, StarsI16 y)
{ (void)context; (void)dc; (void)x; (void)y; }
static void line_to(void *context, StarsU16 dc, StarsI16 x, StarsI16 y)
{ TapeState *s=(TapeState *)context; (void)dc; (void)x; (void)y; ++s->lines; }
static void draw_bar(void *context, StarsU16 dc, const StarsMeasureBar *bar)
{
    TapeState *s=(TapeState *)context;
    if (bar) {
        ++s->bars; s->last_id=bar->id;
        if (bar->label) strcpy((char *)s->last_label,(const char *)bar->label);
    } else ++s->empty_bars;
    (void)dc;
}
static StarsI16 shift(void *context)
{ (void)context; return 1; }
static void no_text(void *context, StarsU8 *output, StarsU16 size)
{ (void)context; if (size>9U) strcpy((char *)output,"No object"); }
static void init_input(StarsMeasureTapeInput *input, TapeState *state)
{
    memset(input,0,sizeof *input); input->scanner_window=20;
    input->client_right=30; input->client_bottom=40; input->scrollbar_height=5;
    input->mouse_move=mouse_move; input->logical_to_scan=identity;
    input->scan_to_logical=identity; input->find_nearest=nearest;
    input->get_dc=get_dc; input->select_pen=select_pen; input->set_rop=set_rop;
    input->capture=capture; input->release_capture=release_capture;
    input->release_dc=release_dc; input->move_to=move_to; input->line_to=line_to;
    input->draw_bar=draw_bar; input->shift_down=shift;
    input->no_object_text=no_text; input->context=state;
}
int main(void)
{
    StarsMeasureTapeInput input; StarsWayPointScan scan; TapeState state;
    StarsPoint point;
    memset(&state,0,sizeof state); memset(&scan,0,sizeof scan);
    init_input(&input,&state); scan.point.x=10; scan.point.y=20;
    state.moves[0].x=10; state.moves[0].y=20;
    state.moves[1].x=40; state.moves[1].y=50;
    state.moves[2].x=-1; state.moves[2].y=-1; point.x=10; point.y=20;
    if(stars_handle_measuring_tape(&input,&scan,point)!=1||state.bars!=1||
       state.empty_bars!=1||state.lines!=2||state.gets!=1||state.selected!=1||
       state.rops!=2||state.captures!=1||state.released_capture!=1||
       state.released_dc!=1||strcmp((char *)state.last_label,"No object"))return 1;
    memset(&state,0,sizeof state); init_input(&input,&state); memset(&scan,0,sizeof scan);
    scan.point.x=0; scan.point.y=0; state.moves[0].x=100; state.moves[0].y=-5;
    state.moves[1].x=-1; state.moves[1].y=-1; state.moves[2].x=-1; state.moves[2].y=-1; point.x=0; point.y=0;
    if(stars_handle_measuring_tape(&input,&scan,point)!=1||state.bars!=1||
       state.last_id!=7||state.last_mask!=0x8fU||state.nearest_calls!=1)return 2;
    memset(&state,0,sizeof state); init_input(&input,&state); memset(&scan,0,sizeof scan);
    scan.point.x=0; scan.point.y=0; state.moves[0].x=1; state.moves[0].y=1;
    state.moves[1].x=-1; state.moves[1].y=-1; state.moves[2].x=-1; state.moves[2].y=-1; point.x=0; point.y=0;
    if(stars_handle_measuring_tape(&input,&scan,point)!=0||state.bars!=0||state.lines!=0)return 3;
    puts("HANDLE_MEASURING_TAPE_SOURCE=PASS"); return 0;
}
