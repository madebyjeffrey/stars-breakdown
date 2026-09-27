#include <stdio.h>
#include <string.h>
#include "core.h"
typedef struct Trace { StarsI16 calls[8],values[8],count; StarsU16 battle_id; } Trace;
static void action(void *v,StarsI16 value){Trace*t=(Trace*)v;t->calls[t->count]=t->count;t->values[t->count]=value;t->count++;}
static void battle_id(void *v,StarsU16 value){Trace*t=(Trace*)v;t->battle_id=value;}
static void reset(Trace *t){memset(t,0,sizeof *t);}
int main(void){StarsOrdersPlanet p[2];StarsDoOrdersInput i;Trace t;memset(p,0,sizeof p);memset(&i,0,sizeof i);memset(&t,0,sizeof t);p[0].player=4;p[0].flags=0xffff;p[0].turn=7;p[1].player=-1;p[1].flags=0xffff;p[1].turn=8;i.planets=p;i.planet_count=2;i.game_turn=0x120b;i.set_battle_id=battle_id;i.do_battles=action;i.do_thing_interactions=action;i.satisfy_orders=action;i.drop_colonists=action;i.update_research=action;i.transfer_to_others=action;i.context=&t;stars_do_orders(0,&i);if((p[0].flags&0x2000U)==0||(p[1].flags&0x2000U)!=0||p[0].turn!=7||t.count!=6||t.values[0]!=0||t.values[1]!=1||t.values[2]!=0||t.values[3]!=0||t.values[4]!=2||t.values[5]!=0)return 1;reset(&t);p[0].flags=0xffff;p[1].flags=0xffff;p[0].turn=7;p[1].turn=8;stars_do_orders(1,&i);if((p[0].flags&0x2000U)==0||(p[1].flags&0x2000U)!=0||p[0].turn!=0||p[1].turn!=0||t.battle_id!=0x0b01||t.count!=6||t.values[0]!=1||t.values[1]!=1||t.values[2]!=3||t.values[3]!=0||t.values[4]!=0||t.values[5]!=4)return 2;puts("DO_ORDERS_SOURCE=PASS");return 0;}
