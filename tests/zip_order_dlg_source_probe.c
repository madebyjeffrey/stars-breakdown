#include <stdio.h>
#include <string.h>
#include "core.h"

typedef struct Trace {
    StarsI16 paints;
    StarsI16 ends;
    StarsI16 fills;
    StarsI16 colors;
    StarsI16 sets;
    StarsI16 positions;
    StarsI16 enables;
    StarsI16 invalidates;
    StarsI16 renames;
    StarsI16 copies;
    StarsI16 deletes;
    StarsI16 stickies;
    StarsI16 dialogs;
    StarsI16 helps;
    StarsI16 tutors;
    StarsI16 frame_right;
    StarsI16 frame_bottom;
    StarsI16 empty_rename;
    StarsI16 last_selection;
    StarsI16 last_result;
    StarsI16 right_texts;
    StarsI16 measured_width;
    StarsI16 string_calls;
    StarsU16 string_ids[8];
    StarsI16 compressed_calls;
    StarsU16 compressed_ids[4];
} Trace;
static StarsU16 begin(void *c, StarsU16 h, StarsRect *r){(void)h;(void)r;++((Trace*)c)->paints;return 3;}
static void end(void *c, StarsU16 h, StarsRect *r){(void)h;(void)r;++((Trace*)c)->ends;}
static void client(void *c, StarsU16 h, StarsRect *r){(void)c;(void)h;r->left=0;r->top=0;r->right=300;r->bottom=200;}
static void fill(void *c, StarsU16 h, const StarsRect *r, StarsU16 b){(void)h;(void)r;(void)b;++((Trace*)c)->fills;}
static StarsU16 item(void *c, StarsU16 h, StarsU16 id){(void)c;(void)h;return (StarsU16)(id+100);}
static void window_rect(void *c, StarsU16 h, StarsRect *r){(void)c;if(h==(StarsU16)(0x431+100)){r->left=1;r->top=2;r->right=40;r->bottom=20;}else{r->left=50;r->top=30;r->right=90;r->bottom=60;}}
static void screen(void *c, StarsU16 h, StarsRect *r){(void)c;(void)h;(void)r;}
static void expand(void *c, StarsRect *r, StarsI16 x, StarsI16 y){(void)c;r->left-=x;r->right+=x;r->top-=y;r->bottom+=y;}
static void frame(void *c, StarsU16 dc, const StarsRect *r, StarsI16 x){Trace*t=(Trace*)c;(void)dc;(void)x;t->frame_right=r->right;t->frame_bottom=r->bottom;}
static StarsU16 select_object(void *c, StarsU16 dc, StarsU16 obj){(void)c;(void)dc;(void)obj;return 8;}
static void color(void *c, StarsU16 dc, StarsU32 value){(void)dc;(void)value;++((Trace*)c)->colors;}
static StarsI16 string(void *c, StarsU16 id, StarsU8 *b, StarsU16 n){Trace*t=(Trace*)c;const char*s;StarsI16 i; s="Label";if(id==0x4c1)s="Custom Orders";else if(id==0x4c8)s="Empty Custom Slot";else if(id==0x6d)s="(no action)";else if(id==0x6e)s="Load All Available";else if(id==0x6f)s="Unload All";else if(id==0x70)s="Load Exactly...";else if(id==0x71)s="Unload Exactly...";if(t->string_calls<8)t->string_ids[t->string_calls]=id;++t->string_calls;if(n==0)return (StarsI16)strlen(s);for(i=0;s[i]!=0&&i+1<(StarsI16)n;++i)b[i]=(StarsU8)s[i];b[i]=0;return i;}
static const StarsU8 *compressed(void *c, StarsU16 id){Trace*t=(Trace*)c;if(t->compressed_calls<4)t->compressed_ids[t->compressed_calls]=id;++t->compressed_calls;return id==0x231?(const StarsU8*)"Customize Zip Orders":(const StarsU8*)"Zip";}
static StarsI32 extent(void *c, StarsU16 dc, const StarsU8 *b, StarsI16 n){Trace*t=(Trace*)c;(void)dc;(void)b;t->measured_width=n*2;return n*2;}
static void text(void *c, StarsU16 dc, StarsI16 x, StarsI16 y,const StarsU8*b,StarsI16 n){(void)dc;(void)x;(void)y;(void)b;(void)n;(void)c;}
static void right(void *c, StarsU16 dc, StarsI16 x, StarsI16 y,const StarsU8*b,StarsI16 n){(void)dc;(void)x;(void)y;(void)b;(void)n;++((Trace*)c)->right_texts;}
static void set_text(void *c, StarsU16 h, const StarsU8*b){(void)h;(void)b;++((Trace*)c)->sets;}
static void show(void *c, StarsU16 h, StarsI16 f){(void)c;(void)h;(void)f;}
static void radio(void *c, StarsU16 h, StarsU16 a,StarsU16 b,StarsU16 d){(void)c;(void)h;(void)a;(void)b;(void)d;}
static void enable(void *c, StarsU16 h, StarsI16 s){(void)h;++((Trace*)c)->enables;((Trace*)c)->last_selection=s;}
static void invalidate(void *c, StarsU16 h){(void)h;++((Trace*)c)->invalidates;}
static StarsI16 rename_order(void *c, StarsI16 s, StarsU8*b, StarsU16 e){Trace*t=(Trace*)c;(void)s;(void)e;++t->renames;if(t->empty_rename)b[0]=0;else{b[0]='N';b[1]='e';b[2]='w';b[3]=0;}return 1;}
static void copy_order(void *c, StarsI16 s){(void)s;++((Trace*)c)->copies;}
static void delete_order(void *c, StarsI16 s){(void)s;++((Trace*)c)->deletes;}
static void focus(void *c, StarsU16 h){(void)c;(void)h;}
static void sticky(void *c, StarsU16 h, StarsPoint*p,StarsI16 save){(void)h;(void)p;(void)save;++((Trace*)c)->stickies;}
static void end_dialog(void *c, StarsU16 h, StarsI16 r){(void)h;++((Trace*)c)->dialogs;((Trace*)c)->last_result=r;}
static void help(void *c, StarsU16 h,const StarsU8*f,StarsU16 id){(void)h;(void)f;(void)id;++((Trace*)c)->helps;}
static void advance(void *c){++((Trace*)c)->tutors;}
static void format_default(void *c, StarsU8*b, StarsI16 n){(void)c;b[0]='D';b[1]=(StarsU8)('0'+n);b[2]=0;}

int main(void)
{
    StarsZipOrderDlgState state; StarsZipOrderDlgOps ops; Trace trace;
    memset(&state,0,sizeof state);memset(&ops,0,sizeof ops);memset(&trace,0,sizeof trace);
    state.valid[0]=1;state.names[0][0]='A';state.names[0][1]=0;state.arial8_height=10;
    state.font_arial8=4;state.button_face_brush=7;state.button_face_color=0x22;
    state.tutor_active=1;
    state.resource_caption=(const StarsU8 *)"Minerals!";
    state.resource_labels[0]=(const StarsU8 *)"M";
    state.resource_labels[1]=(const StarsU8 *)"I";
    state.resource_labels[2]=(const StarsU8 *)"N";
    state.resource_labels[3]=(const StarsU8 *)"E";
    state.resource_labels[4]=(const StarsU8 *)"R";
    state.words[0][0]=0x0000;state.words[0][1]=0x1000;state.words[0][2]=0x2000;state.words[0][3]=0x3000;state.words[0][4]=0x4000;
    ops.begin_paint=begin;ops.end_paint=end;ops.get_client_rect=client;ops.fill_rect=fill;ops.get_dialog_item=item;
    ops.get_window_rect=window_rect;ops.screen_to_client=screen;ops.expand_rect=expand;ops.draw_frame=frame;
    ops.select_object=select_object;ops.set_background_color=color;ops.get_string=string;
    ops.compressed_string=compressed;ops.text_extent=extent;ops.text_out=text;ops.right_text=right;
    ops.set_window_text=set_text;ops.show_window=show;ops.check_radio_button=radio;ops.enable_buttons=enable;
    ops.invalidate=invalidate;ops.rename_dialog=rename_order;ops.copy_order=copy_order;ops.delete_order=delete_order;
    ops.set_focus=focus;ops.sticky_position=sticky;ops.end_dialog=end_dialog;ops.win_help=help;
    ops.advance_tutor=advance;ops.format_default=format_default;ops.context=&trace;
    if(stars_zip_order_dlg(&state,10,272,0,0,&ops)!=1||state.selected!=0||state.frame_window!=10||trace.enables!=1||trace.stickies!=1)return 1;
    trace.empty_rename=1;
    if(stars_zip_order_dlg(&state,10,273,0x41b,0,&ops)!=1||state.names[0][0]!='D')return 2;
    trace.empty_rename=0;
    if(stars_zip_order_dlg(&state,10,15,0,0,&ops)!=1||trace.paints!=1||trace.ends!=1||trace.frame_right!=100||trace.frame_bottom!=65||trace.right_texts!=5||trace.measured_width!=18||trace.compressed_calls<1||trace.compressed_ids[0]!=0x231||trace.string_calls!=6||trace.string_ids[0]!=0x4c1||trace.string_ids[1]!=0x6d||trace.string_ids[2]!=0x6e||trace.string_ids[3]!=0x6f||trace.string_ids[4]!=0x70||trace.string_ids[5]!=0x71)return 3;
    if(stars_zip_order_dlg(&state,10,20,4,0,&ops)!=1||trace.fills!=1)return 4;
    if(stars_zip_order_dlg(&state,10,25,4,1173,&ops)!=7)return 5;
    if(stars_zip_order_dlg(&state,10,273,0x433,0, &ops)!=0||state.selected!=2)return 6;
    if(stars_zip_order_dlg(&state,10,273,0x816,0,&ops)!=1||!state.valid[2]||trace.copies!=1)return 7;
    if(stars_zip_order_dlg(&state,10,273,0x817,0,&ops)!=0||state.valid[2]||trace.deletes!=1)return 8;
    if(stars_zip_order_dlg(&state,10,15,0,0,&ops)!=1||trace.string_calls!=8||trace.string_ids[6]!=0x4c1||trace.string_ids[7]!=0x4c8)return 9;
    if(stars_zip_order_dlg(&state,10,273,0x76,0,&ops)!=1||trace.helps!=1)return 10;
    if(stars_zip_order_dlg(&state,10,273,1,0,&ops)!=1||trace.dialogs!=1||trace.last_result!=1||state.frame_window!=0)return 11;
    puts("ZIP_ORDER_DLG_SOURCE=PASS");return 0;
}
