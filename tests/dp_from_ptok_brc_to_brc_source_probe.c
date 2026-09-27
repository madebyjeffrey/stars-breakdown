#include <stdio.h>
#include <string.h>
#include "core.h"
static StarsI16 part_dp(void *c, StarsU16 group, StarsU16 type){(void)c;(void)group;(void)type;return 3;}
static StarsI32 torp(void *c, StarsI32 base, const StarsDamageEstimateToken *t, StarsI16 pct, StarsI16 bc){(void)c;(void)t;(void)pct;(void)bc;return base/2L;}
int main(void)
{
    StarsDamageEstimateComponent components[1]; StarsDamageEstimateToken source,target; StarsDpFromPtokInput input;
    memset(&source,0,sizeof source); memset(&target,0,sizeof target); memset(&input,0,sizeof input); components[0].group=0x10; components[0].packed_count_type=0x0101; components[0].fuel_max=100;
    source.battle_coordinate=0; source.range=3; source.pct_capacity=100; source.ship_count=1; target.battle_coordinate=0; target.ship_count=1; target.pct_beam_defense=100; target.hull_damage=100;
    input.source=&source; input.target=&target; input.components=components; input.component_count=1; input.source_coordinate=0; input.target_coordinate=0; input.part_dp=part_dp; input.torp_hit=torp;
    if (stars_dp_from_ptok_brc_to_brc(&input)!=100L) return 1;
    input.proximity=0; input.target_coordinate=0x44; if (stars_dp_from_ptok_brc_to_brc(&input)!=0L) return 2;
    puts("DP_FROM_PTOK_BRC_TO_BRC_SOURCE=PASS"); return 0;
}
