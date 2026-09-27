#include <stdio.h>
#include "core.h"
int main(void)
{
    StarsEnemyShipDesign d0[3],d1[3]; StarsEnemyDesignPlayer p[3];
    StarsNthEnemyDesignInput i; const StarsEnemyShipDesign *r;
    d0[0].hidden=0U;d0[1].hidden=1U;d0[2].hidden=0U;
    d1[0].hidden=0U;d1[1].hidden=0U;d1[2].hidden=0U;
    p[0].designs=d0;p[0].design_count=3U;p[1].designs=d1;p[1].design_count=3U;p[2].designs=0;p[2].design_count=0U;
    i.players=p;i.player_count=3U;i.local_player=1;i.starbase_mode=0;
    r=stars_nth_valid_enemy_design(&i,0);if(r!=&d0[0])return 1;
    r=stars_nth_valid_enemy_design(&i,1);if(r!=&d0[2])return 2;
    if(stars_nth_valid_enemy_design(&i,2)!=0)return 3;
    i.local_player=2;i.starbase_mode=1;
    if(stars_nth_valid_enemy_design(&i,2)!=&d1[0])return 4;
    if(stars_nth_valid_enemy_design(&i,-1)!=0)return 5;
    puts("ENEMY_DESIGN_SOURCE=PASS");return 0;
}
