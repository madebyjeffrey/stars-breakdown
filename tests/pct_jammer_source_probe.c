#include <stdio.h>
#include "core.h"
static StarsI16 dp(void*c,StarsU16 g,StarsU16 t){(void)c;(void)g;(void)t;return 30;}
int main(void){StarsPctJammerFromHulInput i;StarsJammerComponent c[2];c[0].group=8;c[0].packed_count_type=(1U<<8)|9U;c[1].group=0x80;c[1].packed_count_type=(1U<<8)|6U;i.hull_id=1;i.components=c;i.component_count=2;i.part_dp=dp;i.context=0;if(stars_pct_jammer_from_hul(&i)!=44)return 1;i.hull_id=33;if(stars_pct_jammer_from_hul(&i)!=33)return 2;puts("PCT_JAMMER_SOURCE=PASS");return 0;}
