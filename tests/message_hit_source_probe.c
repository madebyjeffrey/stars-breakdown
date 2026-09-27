#include <stdio.h>
#include "core.h"
int main(void)
{
    StarsMessageHitTestInput i; StarsU8 sent[0x31]={0}; StarsU8 filtered[0x31]={0};
    i.title_rect.left=10U;i.title_rect.top=20U;i.title_rect.right=110U;i.title_rect.bottom=40U;
    i.point.x=12;i.point.y=25;i.current_message=2;i.message_count=5U;i.message_flags=0U;
    i.game_flags=0U;i.sent=sent;i.filtered=filtered;
    if(stars_message_hit_test(&i)!=1) return 1;
    i.point.x=80;if(stars_message_hit_test(&i)!=3)return 2;
    i.point.x=95;sent[7]=1U;filtered[7]=2U;if(stars_message_hit_test(&i)!=0)return 3;
    filtered[7]=1U;if(stars_message_hit_test(&i)!=2)return 4;
    i.message_flags=0x0100U;i.point.x=12;if(stars_message_hit_test(&i)!=0)return 5;
    i.point.x=200;if(stars_message_hit_test(&i)!=0)return 6;
    puts("MESSAGE_HIT_SOURCE=PASS");return 0;
}
