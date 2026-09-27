#include "core.h"
#include <stdio.h>
int main(void)
{
    StarsMineScanSelection s; StarsMinePoint f[3],t[2]; int failures=0;
    s.planet=-1;s.fleet=2;s.thing=-1;s.point.x=4;s.point.y=5;f[0].present=1;f[0].x=4;f[0].y=5;f[1].present=1;f[1].x=4;f[1].y=5;f[2].present=0;
    t[0].x=4;t[0].y=5;t[1].x=0;t[1].y=0;
    if(stars_other_stuff_at_scan_selection(&s,f,3,t,2)!=1)++failures;
    s.fleet=-1;s.thing=4;if(stars_other_stuff_at_scan_selection(&s,f,3,t,2)!=0)++failures;
    s.planet=1;if(stars_other_stuff_at_scan_selection(&s,f,3,t,2)!=1)++failures;
    printf("MINE_SELECTION_CHECKS=3 FAILURES=%d RESULT=%s\n",failures,failures?"FAIL":"PASS");return failures!=0;
}
