#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsI16 shown;
    StarsI16 enabled;
    StarsI16 positions;
    StarsI16 title_formats;
    StarsI16 recipients;
    StarsI16 editors;
    StarsI16 objects;
    StarsI16 invalidates;
    StarsI16 current_object;
    StarsI16 current_recipient;
    StarsI16 current_editor_empty;
} Trace;

static void show_control(void *c, StarsU16 id, StarsI16 shown)
{ (void)id; (void)shown; ++((Trace *)c)->shown; }
static void enable_control(void *c, StarsU16 id, StarsI16 enabled)
{ (void)id; (void)enabled; ++((Trace *)c)->enabled; }
static void set_control_text(void *c, StarsU16 id, const StarsU8 *text)
{ (void)c; (void)id; (void)text; }
static void set_title(void *c, const StarsU8 *text)
{ (void)c; (void)text; }
static void invalidate(void *c)
{ ++((Trace *)c)->invalidates; }
static void set_position(void *c, StarsU16 id, StarsI16 x, StarsI16 y,
    StarsI16 w, StarsI16 h, StarsU16 flags)
{ (void)id;(void)x;(void)y;(void)w;(void)h;(void)flags; ++((Trace *)c)->positions; }
static void format_title(void *c, StarsI16 kind, StarsI16 turn,
    StarsI16 dirty, StarsI16 current, StarsI16 total, StarsU8 *text,
    StarsU16 capacity)
{ (void)kind;(void)turn;(void)dirty;(void)current;(void)total;
  ++((Trace *)c)->title_formats; if(capacity){text[0]='T';text[1]=0;} }
static StarsI16 get_outgoing(void *c, StarsI16 index,
    StarsSetMsgOutgoingView *view)
{ (void)index; (void)view; (void)c; return 0; }
static StarsI16 decode_text(void *c, const StarsU8 *data, StarsI16 length,
    StarsU8 *text, StarsU16 capacity)
{ (void)c;(void)data;(void)length; if(capacity)text[0]=0; return 1; }
static void set_recipient(void *c, StarsI16 recipient)
{ Trace *t=(Trace *)c; t->current_recipient=recipient; ++t->recipients; }
static void set_editor(void *c, const StarsU8 *text)
{ Trace *t=(Trace *)c; t->current_editor_empty=(StarsI16)(text[0]==0); ++t->editors; }
static StarsI16 get_incoming(void *c, StarsI16 index,
    StarsSetMsgIncomingView *view)
{ (void)c;(void)index; view->goto_word=7U;view->parameter0=12;view->parameter1=13;return 1; }
static StarsI16 message_id(void *c, StarsI16 index)
{ (void)c;(void)index; return 4; }
static StarsI16 fleet_exists(void *c, StarsI16 id)
{ (void)c;(void)id; return 1; }
static void set_object(void *c, StarsI16 object, StarsI16 id,
    StarsI16 x, StarsI16 y)
{ (void)id;(void)x;(void)y; ((Trace *)c)->current_object=object;
  ++((Trace *)c)->objects; }

static void initialize(StarsSetMsgTitleInput *input,
    StarsSetMsgTitleOps *ops, Trace *trace, StarsU8 *filtered)
{
    StarsU16 i;
    memset(input,0,sizeof *input); memset(ops,0,sizeof *ops);
    memset(trace,0,sizeof *trace); memset(filtered,0,64U);
    for(i=0;i<4U;++i) input->controls[i]=(StarsU16)(10U+i);
    input->hwnd=1;input->edit_control=20;input->dropdown_control=21;
    input->scroll_control=22;input->incoming_recipient=9;
    input->message_text_rect.left=0;input->message_text_rect.top=2;
    input->message_text_rect.right=100;input->message_text_rect.bottom=30;
    input->filtered_bits=filtered;input->filtered_bit_bytes=64U;
    input->ops=ops;input->context=trace;
    ops->show_control=show_control;ops->enable_control=enable_control;
    ops->set_control_text=set_control_text;ops->set_title=set_title;
    ops->invalidate=invalidate;ops->set_position=set_position;
    ops->format_title=format_title;ops->get_outgoing=get_outgoing;
    ops->decode_text=decode_text;ops->set_recipient=set_recipient;
    ops->set_editor=set_editor;ops->get_incoming=get_incoming;
    ops->message_id=message_id;ops->fleet_exists=fleet_exists;
    ops->set_object=set_object;
}

int main(void)
{
    StarsSetMsgTitleInput input; StarsSetMsgTitleOps ops; Trace trace;
    StarsU8 filtered[64]; int failures=0;
    initialize(&input,&ops,&trace,filtered);
    input.incoming_count=2;input.current_message=0;input.message_view_filtered=1;
    stars_set_msg_title(&input);
    if(trace.shown!=3||trace.title_formats!=1||trace.objects!=1||
            trace.current_object!=STARS_SET_MSG_TITLE_OBJECT_PLANET||
            trace.enabled!=3||trace.invalidates!=1)++failures;

    initialize(&input,&ops,&trace,filtered); input.incoming_count=2;
    input.current_message=0;input.message_view_filtered=0;filtered[0]=0x10;
    stars_set_msg_title(&input);
    if(trace.objects!=0||trace.enabled!=3)++failures;

    initialize(&input,&ops,&trace,filtered); input.send_mode=1;
    input.current_outgoing=0;input.outgoing_count=1;
    stars_set_msg_title(&input);
    if(trace.positions!=3||trace.recipients!=1||trace.editors!=1||
            !trace.current_editor_empty||trace.enabled!=3)++failures;

    initialize(&input,&ops,&trace,filtered); input.ai_active=1;
    stars_set_msg_title(&input);
    if(trace.shown!=0||trace.enabled!=0)++failures;

#ifndef TARGET_NATIVE
    printf("SET_MSG_TITLE_CHECKS=4 FAILURES=%d RESULT=%s\n",failures,
        failures?"FAIL":"PASS");
#endif
    return failures!=0;
}
