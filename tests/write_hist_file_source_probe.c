#include <stdio.h>
#include <string.h>
#include "core.h"
typedef struct T { StarsU16 records, planets, players, designs, scores, ai, last_type, last_size; } T;
static StarsI16 cr(void*c,StarsI16 p){(void)c;(void)p;return 1;}
static void wp(void*c,StarsU16 t,const void*r){T*x=(T*)c;(void)r;++x->planets;x->last_type=t;}
static void wr(void*c,StarsU16 t,StarsU16 n,const void*r){T*x=(T*)c;(void)r;++x->records;x->last_type=t;x->last_size=n;if(t==0x29U)++x->ai;}
static void wpl(void*c,const void*r){T*x=(T*)c;(void)r;++x->players;}
static void wd(void*c,const void*r){T*x=(T*)c;(void)r;++x->designs;}
static void ws(void*c,const void*r){T*x=(T*)c;(void)r;++x->scores;}
static void al(void*c,StarsU16 s,StarsI16 n){(void)c;(void)s;(void)n;}
static void cl(void*c){(void)c;}
int main(void){StarsWriteHistFileInput i;StarsHistoryPlanet planets[2];StarsHistoryPlayer players[2];StarsHistoryDesign d,e;StarsHistoryScore scores[2];StarsU8 ai[1025],bits[3];T t;StarsU16 j;memset(&t,0,sizeof(t));memset(players,0,sizeof(players));memset(ai,1,sizeof(ai));players[0].fleet_count=4;players[1].detection_flags=0x0101;for(j=0;j<16U;++j)players[1].designs[j].flags=0x0200UL;for(j=0;j<10U;++j)players[1].enemy_designs[j].flags=0x0200UL;players[1].designs[0].record=&d;players[1].designs[0].flags=0;players[1].enemy_designs[0].record=&e;players[1].enemy_designs[0].flags=0;scores[0].turn=5;scores[1].turn=150;players[1].scores=scores;players[1].score_count=2;planets[0].owner_flags=2;planets[1].owner_flags=3;i.player=0;i.planet_count=2;i.game_player_count=2;i.game_turn=150;i.planets=planets;i.players=players;i.message_bits_count=3;i.message_bits=bits;i.ai_data=ai;i.ai_size=1025;i.create_file=cr;i.write_planet=wp;i.write_record=wr;i.write_player=wpl;i.write_design=wd;i.write_score=ws;i.alert=al;i.close_stream=cl;i.context=&t;if(stars_write_hist_file(&i)!=1||t.records!=5U||t.planets!=2U||t.players!=1U||t.designs!=2U||t.scores!=1U||t.ai!=2U||t.last_type!=0U||t.last_size!=0U)return 1;puts("WRITE_HIST_FILE_SOURCE=PASS");return 0;}
