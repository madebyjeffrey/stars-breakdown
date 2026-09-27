#include <stdio.h>
#include "core.h"
int main(void)
{
    if(stars_should_part_be_hidden(-1,1U,8U,0U)!=0)return 1;
    if(stars_should_part_be_hidden(0,1U,8U,0U)!=1||stars_should_part_be_hidden(0,1U,8U,0x200U)!=0)return 2;
    if(stars_should_part_be_hidden(0,4U,6U,0U)!=1||stars_should_part_be_hidden(0,8U,9U,0U)!=1)return 3;
    if(stars_should_part_be_hidden(0,0x10U,0x12U,0U)!=1||stars_should_part_be_hidden(0,0x20U,7U,0U)!=1)return 4;
    if(stars_should_part_be_hidden(0,0x40U,8U,0U)!=1||stars_should_part_be_hidden(0,0x80U,6U,0U)!=1)return 5;
    if(stars_should_part_be_hidden(0,0x800U,4U,0U)!=1||stars_should_part_be_hidden(0,0x1000U,4U,0U)!=1)return 6;
    if(stars_should_part_be_hidden(0,0x1000U,9U,0U)!=1||stars_should_part_be_hidden(0,0x4000U,0x1eU,0U)!=1)return 7;
    if(stars_should_part_be_hidden(0,0x8000U,0x0eU,0U)!=1||stars_should_part_be_hidden(0,2U,8U,0U)!=0)return 8;
    puts("PART_HIDDEN_SOURCE=PASS");return 0;
}
