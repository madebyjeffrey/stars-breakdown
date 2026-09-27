#include <stdio.h>
#include <string.h>
#include "core.h"
int main(void)
{
    StarsGetVcrStatsInput input; StarsVcrStatToken token;
    StarsVcrStatRecord record; StarsI32 design,shield; StarsU16 displacement;
    StarsI16 ships; StarsI32 values[1];
    memset(&input,0,sizeof input); memset(&token,0,sizeof token); memset(&record,0,sizeof record);
    token.ship_count=3; token.hull_dp=100; token.shield=4; token.displacement=0x0100; values[0]=300;
    record.token_index=0; record.ship_loss=1; record.packed_damage=2; record.displacement=0x0080;
    input.tokens=&token; input.records=&record; input.token_count=1; input.record_count=1; input.design_values=values;
    stars_get_vcr_stats(&input,0,&design,&displacement,&shield,&ships);
    if(ships!=2||displacement!=0x0080||shield!=2||design!=200)return 1;
    record.ship_loss=3; input.record_count=1;
    stars_get_vcr_stats(&input,0,&design,&displacement,&shield,&ships);
    if(ships!=0||design!=0||shield!=12)return 2;
    puts("GET_VCR_STATS_SOURCE=PASS");return 0;
}
