/* Genuine C reconstruction of BtlDataGet (NE 30:0362-044f). */
#include "core.h"

const StarsBattleDataRecord *stars_btl_data_get(const StarsBattleDataInput *input,
    StarsI16 index)
{
    const StarsBattleDataBlock *block;
    StarsU16 i;

    if (!input) return 0;
    block = input->first;
    while (block) {
        for (i = 0U; i < block->record_count; ++i) {
            const StarsBattleDataRecord *record = &block->records[i];
            if (record->id == 0xffffU) break;
            if (record->cb_data == 0U) return 0;
            if (index < 1) return record;
            --index;
        }
        block = block->next;
        if (!block || block->ib_top < 0x11U) return 0;
    }
    return 0;
}
