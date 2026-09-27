#include <stdio.h>
#include <string.h>
#include "core.h"
typedef struct Trace { StarsI16 selected; StarsI16 choice; StarsI16 checked; StarsI16 added; StarsI16 reset; StarsI16 logged; StarsI16 ended; StarsI16 enabled; StarsI16 deleted; StarsI16 refreshed; StarsI16 named; } Trace;
static void sticky(void *c, StarsU16 h){(void)c;(void)h;}
static void enable(void *c, StarsU16 id, StarsI16 on){Trace*t=(Trace*)c;(void)id;t->enabled=on;}
static void reset(void *c, StarsU16 id){Trace*t=(Trace*)c;(void)id;++t->reset;}
static void add(void *c, StarsU16 id, StarsI16 n, const StarsU8 *name){Trace*t=(Trace*)c;(void)id;(void)n;(void)name;++t->added;}
static void select_item(void *c, StarsU16 id, StarsI16 n){Trace*t=(Trace*)c;(void)id;t->selected=n;}
static StarsI16 get_selection(void *c, StarsU16 id){(void)id;return ((Trace*)c)->selected;}
static void set_choice(void *c, StarsU16 id, StarsI16 n){Trace*t=(Trace*)c;(void)id;t->choice=n;}
static StarsI16 get_choice(void *c, StarsU16 id){(void)id;return ((Trace*)c)->choice;}
static void set_check(void *c, StarsU16 id, StarsI16 on){Trace*t=(Trace*)c;(void)id;t->checked=on;}
static StarsI16 get_check(void *c, StarsU16 id){(void)id;return ((Trace*)c)->checked;}
static void log_change(void *c, const StarsBattlePlan *p){Trace*t=(Trace*)c;(void)p;++t->logged;}
static StarsI16 delete_plan(void *c, StarsI16 n, StarsI16 warn){Trace*t=(Trace*)c;(void)n;(void)warn;++t->deleted;return 1;}
static StarsI16 new_name(void *c, StarsBattlePlan *p){Trace*t=(Trace*)c;(void)p;++t->named;return 1;}
static void refresh(void *c, StarsI16 n){Trace*t=(Trace*)c;(void)n;++t->refreshed;}
static void tutor(void *c, StarsU16 n){(void)c;(void)n;}
static void help(void *c, StarsU16 h, StarsU16 f){(void)c;(void)h;(void)f;}
int main(void)
{
    StarsBattlePlan plans[2]; StarsBattlePlansOps ops; StarsBattlePlansInput input; Trace trace;
    memset(plans,0,sizeof plans); memset(&ops,0,sizeof ops); memset(&input,0,sizeof input); memset(&trace,0,sizeof trace);
    plans[0].name[0]='A'; plans[1].name[0]='B'; plans[1].words[0]=0x8000U; plans[1].words[1]=0x0203U;
    ops.sticky_dialog=sticky; ops.enable_control=enable; ops.reset_list=reset; ops.add_list_item=add;
    ops.select_list_item=select_item; ops.get_list_selection=get_selection; ops.set_choice=set_choice;
    ops.get_choice=get_choice; ops.set_check=set_check; ops.get_check=get_check; ops.log_change=log_change;
    ops.delete_plan=delete_plan; ops.new_plan_name=new_name; ops.refresh_battle_dropdown=refresh;
    ops.advance_tutor=tutor; ops.win_help=help;
    input.player=1; input.player_count=3; input.selected_object_kind=2; input.selected_plan=1;
    input.plan_count=2; input.plans=plans; input.list_plans=0x41e; input.list_attackers=0x41f;
    input.list_friendly_tactics=0x420; input.list_tactics=0x421; input.list_target_players=0x422;
    input.check_manual=0x41d; input.delete_control=0x41b; input.new_control=0x817;
    input.ops=&ops; input.context=&trace;
    if (stars_battle_plans_dialog(1,0x110,0,0,&input)!=1 || trace.added!=2 || input.edit_plan_index!=1 || trace.checked!=1) return 1;
    trace.choice=4; if (stars_battle_plans_dialog(1,0x111,0x41f,0,&input)!=0 ||
        (input.edit_plan.words[1]&0xfU)!=4U || !input.dirty) return 2;
    trace.selected=0; if (stars_battle_plans_dialog(1,0x111,0x41e,0,&input)!=0 ||
        input.edit_plan_index!=0 || trace.logged!=1) return 3;
    trace.checked=1; if (stars_battle_plans_dialog(1,0x111,0x41d,0,&input)!=0 ||
        !(input.edit_plan.words[0]&0x8000U)) return 4;
    if (stars_battle_plans_dialog(1,0x111,1,0,&input)!=1 || trace.logged!=2 || input.edit_plan_index!=-1) return 5;
    puts("BATTLE_PLANS_DLG_SOURCE=PASS"); return 0;
}
