#include <stdio.h>
#include <string.h>
#include "core.h"
typedef struct Trace { StarsI16 called, help, player; } Trace;
static StarsI16 best(void *v, const StarsRemoteTerraformPlanet *p, StarsI16 h){Trace*t=(Trace*)v;t->called=1;t->help=h;t->player=p->player;return 7;}
int main(void){StarsU8 players[0xc0*2],before[0xc0*2];StarsRemoteTerraformPlanet p;StarsBestRemoteTerraformInput i;Trace t;StarsI16 n;memset(players,0,sizeof players);memset(&t,0,sizeof t);n=0;while(n<0xc0){players[n]=(StarsU8)n;players[0xc0+n]=(StarsU8)(200+n);++n;}memcpy(before,players,sizeof before);p.player=0;memset(&i,0,sizeof i);i.planet=&p;i.players=players;i.player_count=2;i.best_terraform=best;i.context=&t;if(stars_best_remote_terraform(&i,1,9)!=7||!t.called||t.help!=9||t.player!=0||memcmp(players,before,sizeof before)!=0)return 1;if(stars_best_remote_terraform(&i,2,9)!=0||memcmp(players,before,sizeof before)!=0)return 2;puts("BEST_REMOTE_TERRAFORM_SOURCE=PASS");return 0;}
