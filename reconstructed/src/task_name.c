#include "core.h"
#include <stdio.h>
#include <string.h>

/* NE 34:53B8-589C. Translate the fleet's leading order into report text.
 * Resource/string lookup and the original near scratch buffers are exposed
 * as callbacks and a caller-owned output buffer. */
StarsI16 stars_get_task_name(const StarsTaskNameInput *input, StarsU8 *output,
    StarsU16 capacity, StarsI16 *order_index)
{
    StarsU16 kind;
    StarsU16 i;
    StarsU16 high;
    StarsU16 best_id;
    StarsU16 best_score;
    StarsU16 value;
    const StarsU16 *orders;
    StarsU8 text[256];
    StarsI16 selected = -1;
    if (!input || !output || capacity == 0U || !input->compressed) return 0;
    orders = input->orders;
    output[0] = 0;
    if (order_index) *order_index = -1;
    kind = orders[3] & 0x000fU;
    if ((orders[3] & 0x1000U) == 0U) {
        kind = 0U;
    } else if (kind == 1U) {
        for (i = 0; i < 4U; ++i)
            if ((orders[4U + i] >> 12) == 6U) goto classify;
    } else if (kind == 2U || kind == 4U || kind == 5U || kind == 6U ||
            kind == 8U) {
        goto classify;
    }
    if (input->cord > 1U) {
        orders = input->next_orders;
        kind = orders[3] & 0x000fU;
    }
classify:
    if ((orders[3] & 0x1000U) == 0U) {
        if (input->dash_text && strlen((const char *)input->dash_text) + 1U < capacity)
            strcpy((char *)output, (const char *)input->dash_text);
        return 1;
    }
    value = (StarsU16)(kind + 99U);
    if (kind == 1U) {
        if (input->zip_orders) {
            for (i = 0; i < 4U; ++i)
                if (input->zip_orders[i].valid &&
                            memcmp(input->zip_orders[i].words, orders + 4U,
                            10U) == 0 && input->zip_orders[i].name) {
                    if (strlen((const char *)input->zip_orders[i].name) + 1U >= capacity)
                        return 0;
                    strcpy((char *)output, (const char *)input->zip_orders[i].name);
                    return 1;
                }
        }
        if (orders[8] >> 12 == 7U &&
                ((orders[7] >> 12) == 0U ||
                 ((orders[7] >> 12) == 2U &&
                  (orders[4] >> 12) == 2U))) {
            high = orders[4] >> 12;
            if (high == 1U || high == 2U || high == 6U) {
                i = 1U;
                while (i < 3U && (orders[4U + i] >> 12) == high) ++i;
                if (i == 3U) {
                    best_id = high == 1U ? 0U : (high == 2U ? 1U : 2U);
                    if (input->special_names[best_id]) {
                        if (strlen((const char *)input->special_names[best_id]) + 1U >= capacity)
                            return 0;
                        strcpy((char *)output,
                            (const char *)input->special_names[best_id]);
                        return 1;
                    }
                }
            }
        }
        best_score = 109U;
        value = best_score;
        for (i = 0U; i < 5U; ++i) {
            high = orders[4U + i] >> 12;
            if (value < (StarsU16)(high + 109U)) {
                value = (StarsU16)(high + 109U);
                selected = (StarsI16)i;
            }
        }
        if (value == best_score) {
            if (!input->compressed(input->context, 100U, output, capacity)) return 0;
            return 1;
        }
        high = orders[4U + (StarsU16)selected] >> 12;
        if (order_index) *order_index = selected;
        if (!input->compressed(input->context, value, text, sizeof text)) return 0;
        if (high == 5U || high == 6U) {
            size_t length = strlen((const char *)text);
            if (length < 3U) return 0;
            text[length - 3U] = 0;
            sprintf((char *)output, (const char *)text,
                orders[4U + (StarsU16)selected] & 0x0fffU);
            return strlen((const char *)output) + 1U < capacity;
        }
        if (high == 4U || high == 8U || high == 9U) {
            size_t length = strlen((const char *)text);
            if (length < 3U) return 0;
            text[length - 3U] = 0;
            if (selected == 4 && input->suffix_names[0])
                sprintf((char *)output, (const char *)input->suffix_names[0], text,
                    orders[8] & 0x0fffU);
            else if (input->suffix_names[1])
                sprintf((char *)output, (const char *)input->suffix_names[1], text,
                    orders[4U + (StarsU16)selected] & 0x0fffU);
            else return 0;
            return strlen((const char *)output) + 1U < capacity;
        }
        if (high == 7U && selected == 4) value = 119U;
        else if (high == 7U) value = (StarsU16)(high + 109U);
        else if (high != 1U && high != 2U && high != 4U && high != 5U &&
                high != 6U && high != 8U && high != 9U) {
            if (input->dash_text && strlen((const char *)input->dash_text) + 1U <= capacity)
                strcpy((char *)output, (const char *)input->dash_text);
            return 1;
        }
        if (!input->compressed(input->context, value, output, capacity)) return 0;
        return 1;
    }
    if (kind == 6U) {
        if (orders[4] < 5U) {
            if (!input->compressed(input->context, value, text, sizeof text)) return 0;
            sprintf((char *)output, (const char *)text, orders[4] + 1U);
        } else if (!input->localized(input->context, value, output, capacity)) return 0;
        return 1;
    }
    if (kind == 7U) {
        if (orders[5] < 11U) {
            if (!input->compressed(input->context, value, text, sizeof text)) return 0;
            sprintf((char *)output, (const char *)text,
                (orders[5] + 1U) * 50U);
        } else if (!input->localized(input->context, value, output, capacity)) return 0;
        return 1;
    }
    return input->compressed(input->context, value, output, capacity);
}
