#include <stdio.h>
#include "core.h"
int main(void)
{
    if(stars_attack_player(0U,2,4)!=0)return 1;
    if(stars_attack_player(1U,2,4)!=1||stars_attack_player(1U,1,4)!=0)return 2;
    if(stars_attack_player(2U,2,4)!=1||stars_attack_player(2U,1,4)!=0)return 3;
    if(stars_attack_player(3U,0,4)!=1)return 4;
    if(stars_attack_player(8U,0,4)!=1||stars_attack_player(8U,0,3)!=0)return 5;
    puts("ATTACK_PLAYER_SOURCE=PASS");return 0;
}
