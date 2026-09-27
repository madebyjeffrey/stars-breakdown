#include "core.h"

/* NE 30:028C-0361. Count battle records across the linked heap blocks.  A
 * 0xffff marker ends a block, while a zero record length is the original
 * malformed/unfinished-record stop condition. */
StarsI16 stars_count_battles(const StarsBattleBlock *block)
{
    StarsI16 count = 0;
    StarsU16 i;
    while (block) {
        for (i = 0; i < block->record_count; ++i) {
            if (block->records[i].marker == 0xffffU) break;
            if (block->records[i].length == 0U) return count;
            ++count;
        }
        if (!block->next || block->next->ib_top < 0x11U) break;
        block = block->next;
    }
    return count;
}
