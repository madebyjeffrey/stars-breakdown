#include <stdio.h>
#include <string.h>
#include "core.h"
typedef struct T { StarsU16 records, memory_type, memory_size, closed, dirty; StarsU16 types[5], sizes[5]; StarsU16 player; StarsU8 header[17]; } T;
static StarsI16 cr(void*c,StarsI16 p,const StarsU8*b){T*t=(T*)c;(void)b;t->player=(StarsU16)p;return 1;}
static void wm(void*c,StarsU16 t,StarsU16 n,const StarsU8*d){T*x=(T*)c;(void)d;x->memory_type=t;x->memory_size=n;}
static void wr(void*c,StarsU16 t,StarsU16 n,const void*d){T*x=(T*)c;if(t==9U && n==17U)memcpy(x->header,d,17U);if(x->records<5U){x->types[x->records]=t;x->sizes[x->records]=n;}++x->records;}
static void al(void*c,StarsU16 s,StarsI16 n){(void)c;(void)s;(void)n;}
static void cl(void*c){T*t=(T*)c;t->closed=1;}
static void dg(void*c){T*t=(T*)c;t->dirty=1;}
int main(void){StarsWriteLogFileInput i;StarsU8 env[11],q[26],old[26],log[4];StarsU16 flags=0;StarsOutgoingPlayerMessage msg;T t;memset(&t,0,sizeof(t));memset(env,1,11);memset(q,2,26);memset(old,3,26);log[0]=2;log[1]=0;log[2]=9;log[3]=8;msg.from=1;msg.to=2;msg.compressed_length=-3;msg.data=(const StarsU8*)"x";i.file_base=(const StarsU8*)"GAME";i.player=4;i.current_player=4;i.player_flags=0;i.previous_log_type=0;i.current_queue=q;i.previous_queue=old;i.queue_count_byte=12;i.serial=0x12345678UL;i.environment=env;i.log=log;i.log_size=4;i.messages=&msg;i.message_count=1;i.game_flags=&flags;i.create_file=cr;i.write_memory=wm;i.write_record=wr;i.alert=al;i.close_stream=cl;i.dirty_game=dg;i.context=&t;if(stars_write_log_file(&i)!=1||t.player!=4U||t.memory_type!=0x2eU||t.memory_size!=26U||t.records!=4U||t.types[0]!=9U||t.sizes[0]!=17U||t.types[1]!=0U||t.types[2]!=0x28U||t.sizes[2]!=15U||t.types[3]!=0U||!t.closed||!t.dirty||flags!=0x100U||t.header[0]!=4U||t.header[1]!=0U||t.header[2]!=0x78U||t.header[5]!=0x12U||memcmp(t.header+6,env,11U)!=0)return 1;puts("WRITE_LOG_FILE_SOURCE=PASS");return 0;}
