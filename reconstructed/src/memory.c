/* memory.obj, NE 13:0000-0959. Reviewed against the complete disassembly.
 * Valid live pointers, heap types 0..11 and consistent ownership chains are
 * caller preconditions, as in the original. Only owner lookup checks its type.
 * Ordinary far arithmetic wraps the offset without changing the selector.
 * These are not huge pointers. No allocation or payload is implicitly cleared.
 */
#include "memory.h"
#include "runtime_internal.h"

#define HEADS 0x0d34U
#define GROWTH 0x0d64U

static StarsFar add(StarsFar p, StarsU16 n)
{
    return (p & 0xffff0000UL) | (StarsU16)((StarsU16)p + n);
}
static StarsU8 *image(StarsMemory *m, StarsFar p)
{
    return m->api->resolve(m->context, (StarsU16)(p >> 16));
}
static StarsU16 word(StarsMemory *m, StarsFar p, StarsU16 n)
{
    return stars_word(image(m, p), (StarsU16)(p + n));
}
static void put(StarsMemory *m, StarsFar p, StarsU16 n, StarsU16 v)
{
    stars_put_word(image(m, p), (StarsU16)(p + n), v);
}
static StarsFar next(StarsMemory *m, StarsFar p)
{
    return stars_dword(image(m, p), (StarsU16)(p + 8U));
}
static void set_next(StarsMemory *m, StarsFar p, StarsFar v)
{
    stars_put_dword(image(m, p), (StarsU16)(p + 8U), v);
}
static StarsFar head(StarsMemory *m, StarsI16 ht)
{
    return stars_dword(m->runtime->data, (StarsU16)(HEADS + (StarsU16)ht * 4U));
}
static void set_head(StarsMemory *m, StarsI16 ht, StarsFar p)
{
    stars_put_dword(m->runtime->data, (StarsU16)(HEADS + (StarsU16)ht * 4U), p);
}
static StarsU16 growth(StarsMemory *m, StarsI16 ht)
{
    return stars_word(m->runtime->data, (StarsU16)(GROWTH + (StarsU16)ht * 2U));
}
static void exhausted(StarsMemory *m)
{
    StarsRuntime *r = m->runtime;
    stars_runtime_alert(r, stars_format_id(r, 26, NULL), 16);
    stars_runtime_abort(r, stars_word(r->data, 0x006eU), -1);
}

/* LphbAlloc, 0000-0107. GMEM_MOVEABLE | GMEM_NODISCARD, without ZEROINIT. */
StarsFar stars_heap_alloc(StarsMemory *m, StarsU16 cb, StarsI16 ht)
{
    StarsU16 h;
    StarsFar p;
    cb = (StarsU16)(cb + 16U);
    if (cb < growth(m, ht)) cb = growth(m, ht);
    h = m->api->alloc(m->context, 0x22U, cb);
    if (!h) exhausted(m);
    p = m->api->lock(m->context, h);
    put(m, p, 12, h);
    put(m, p, 2, cb);
    put(m, p, 0, (StarsU16)(cb - 16U));
    put(m, p, 4, (StarsU16)(cb - 16U));
    put(m, p, 6, 16);
    image(m, p)[(StarsU16)(p + 14U)] = (StarsU8)ht;
    set_next(m, p, head(m, ht));
    set_head(m, ht, p);
    return p;
}

/* LphbReAlloc, 0108-02d7. A moved block must be relinked, including non-heads. */
StarsFar stars_heap_realloc(StarsMemory *m, StarsFar old)
{
    StarsU16 h, cb, increment;
    StarsI16 ht;
    StarsFar p, before;
    if (!old) return 0;
    h = word(m, old, 12);
    cb = word(m, old, 2);
    ht = image(m, old)[(StarsU16)(old + 14U)];
    increment = growth(m, ht);
    if (cb >= 0xffdcU) exhausted(m);
    if (cb > (StarsU16)(0xffdcU - increment))
        increment = (StarsU16)(0xffdcU - cb);
    m->api->unlock(m->context, h);
    h = m->api->realloc(m->context, h,
        (StarsU16)(word(m, old, 2) + increment), 0x22U);
    if (!h) exhausted(m);
    p = m->api->lock(m->context, h);
    put(m, p, 12, h);
    /* The type is read again from the locked, potentially relocated header. */
    ht = image(m, p)[(StarsU16)(p + 14U)];
    if (head(m, ht) == old) set_head(m, ht, p);
    else {
        before = head(m, ht);
        while (before && next(m, before) != old) before = next(m, before);
        set_next(m, before, p);
    }
    put(m, p, 2, (StarsU16)(word(m, p, 2) + increment));
    put(m, p, 0, (StarsU16)(word(m, p, 0) + increment));
    put(m, p, 4, (StarsU16)(word(m, p, 4) + increment));
    return p;
}

/* FreeHb, 02d8-0347. Deliberately leaves the global head for the owner to clear. */
void stars_heap_free(StarsMemory *m, StarsFar p)
{
    StarsFar following;
    StarsU16 h;
    while (p) {
        following = next(m, p);
        h = word(m, p, 12);
        m->api->unlock(m->context, h);
        m->api->free(m->context, h);
        p = following;
    }
}

/* ResetHb, 0348-03b1. Payload and linkage survive the logical reset. */
void stars_heap_reset(StarsMemory *m, StarsI16 ht)
{
    StarsFar p;
    for (p = head(m, ht); p; p = next(m, p)) {
        put(m, p, 6, 16);
        put(m, p, 0, (StarsU16)(word(m, p, 2) - 16U));
        put(m, p, 4, word(m, p, 0));
    }
}

/* LpAlloc, 03b2-058b. Prefer the unused tail; only scan holes when it is too
 * small. Coalesce just enough adjacent free chunks, then consume the whole
 * resulting chunk. The original never splits an oversized hole. */
StarsFar stars_alloc(StarsMemory *m, StarsU16 cb, StarsI16 ht)
{
    StarsFar p, top, cursor, first;
    StarsU16 chunk, is_free;
    cb = (StarsU16)((cb + 3U) & 0xfffeU);
    p = head(m, ht);
    for (;;) {
        while (p && word(m, p, 0) < cb) p = next(m, p);
        if (!p) p = stars_heap_alloc(m, cb, ht);
        top = add(p, word(m, p, 6));
        if (word(m, p, 4) >= cb) {
            put(m, top, 0, (StarsU16)(cb - 2U));
            put(m, p, 6, (StarsU16)(word(m, p, 6) + cb));
            put(m, p, 0, (StarsU16)(word(m, p, 0) - cb));
            put(m, p, 4, (StarsU16)(word(m, p, 4) - cb));
            return add(top, 2);
        }
        cursor = add(p, 16);
        while ((StarsU16)cursor < (StarsU16)top) {
            first = cursor;
            is_free = (StarsU16)(word(m, cursor, 0) & 1U);
            cursor = add(cursor, (StarsU16)((word(m, cursor, 0) & 0xfffeU) + 2U));
            if (!is_free) continue;
            while ((StarsU16)cursor < (StarsU16)top &&
                (word(m, cursor, 0) & 1U) && (StarsU16)(cursor - first) < cb)
                cursor = add(cursor, (StarsU16)((word(m, cursor, 0) & 0xfffeU) + 2U));
            chunk = (StarsU16)(cursor - first);
            put(m, first, 0, (StarsU16)((chunk - 2U) | 1U));
            if (chunk < cb) continue;
            put(m, first, 0, (StarsU16)(word(m, first, 0) & 0xfffeU));
            put(m, p, 0, (StarsU16)(word(m, p, 0) - chunk));
            return add(first, 2);
        }
        p = next(m, p);
    }
}

/* LphbFromLpHt, 058c-065e. Unsigned selector first, then unsigned offset;
 * both block boundaries are excluded. This is not linear-address comparison. */
StarsFar stars_heap_from_pointer(StarsMemory *m, StarsFar lp, StarsI16 ht)
{
    StarsFar p;
    if (ht < 0 || ht >= 12) return 0;
    for (p = head(m, ht); p; p = next(m, p))
        if (lp > p && lp < add(p, word(m, p, 2))) return p;
    return 0;
}

/* LpReAlloc, 0660-07a7. Types 3/10 require the sole payload at hb+18:
 * repeatedly grow that Windows block instead of allocating a separate chunk. */
StarsFar stars_realloc(StarsMemory *m, StarsFar lp, StarsU16 cb, StarsI16 ht)
{
    StarsFar p, replacement;
    StarsU16 old, increment, i;
    old = word(m, lp, 0xfffeU);
    cb = (StarsU16)((cb + 1U) & 0xfffeU);
    increment = (StarsU16)(cb - old);
    if (cb <= old) return lp;
    p = stars_heap_from_pointer(m, lp, ht);
    for (;;) {
        if (add(p, word(m, p, 6)) == add(lp, old) && word(m, p, 4) >= increment) {
            put(m, p, 4, (StarsU16)(word(m, p, 4) - increment));
            put(m, p, 0, (StarsU16)(word(m, p, 0) - increment));
            put(m, p, 6, (StarsU16)(word(m, p, 6) + increment));
            put(m, lp, 0xfffeU, cb);
            return lp;
        }
        if (ht == 3 || ht == 10) {
            p = stars_heap_realloc(m, p);
            lp = add(p, 18);
        } else {
            replacement = stars_alloc(m, cb, ht);
            /* _fmemcpy forward copy. Live source and destination are disjoint. */
            for (i = 0; i < old; ++i)
                image(m, replacement)[(StarsU16)(replacement + i)] =
                    image(m, lp)[(StarsU16)(lp + i)];
            stars_free(m, lp, ht);
            return replacement;
        }
    }
}

/* FreeLp, 07a8-0834. Only the freed chunk itself can retreat the tail. */
void stars_free(StarsMemory *m, StarsFar lp, StarsI16 ht)
{
    StarsFar p;
    StarsU16 cb;
    if (!lp) return;
    p = stars_heap_from_pointer(m, lp, ht);
    cb = (StarsU16)(word(m, lp, 0xfffeU) + 2U);
    put(m, lp, 0xfffeU, (StarsU16)(word(m, lp, 0xfffeU) | 1U));
    put(m, p, 0, (StarsU16)(word(m, p, 0) + cb));
    if ((StarsU16)(lp - p + cb - 2U) == word(m, p, 6)) {
        put(m, p, 6, (StarsU16)(word(m, p, 6) - cb));
        put(m, p, 4, (StarsU16)(word(m, p, 4) + cb));
    }
}

/* LpplReAlloc, 0836-088a. Capacity is stored in one byte even though the
 * allocation calculation uses the full 16-bit argument and wraps at 65536. */
StarsFar stars_list_realloc(StarsMemory *m, StarsFar p, StarsU16 count)
{
    StarsU16 flags = word(m, p, 0);
    p = stars_realloc(m, p, (StarsU16)((StarsU32)(flags & 255U) * count + 4U),
        (StarsI16)((flags >> 9) & 7U));
    image(m, p)[(StarsU16)(p + 2U)] = (StarsU8)count;
    return p;
}

/* LpplAlloc, 088c-0917. Preserve the previous upper nibble (cAlloc).
 * ht occupies only three bits; callers must respect the narrower list type. */
StarsFar stars_list_alloc(StarsMemory *m, StarsU16 item, StarsU16 count, StarsI16 ht)
{
    StarsFar p = stars_alloc(m, (StarsU16)((StarsU32)item * count + 4U), ht);
    image(m, p)[(StarsU16)(p + 2U)] = (StarsU8)count;
    image(m, p)[(StarsU16)(p + 3U)] = 0;
    put(m, p, 0, (StarsU16)((word(m, p, 0) & 0xf000U) |
        (item & 255U) | (((StarsU16)ht & 7U) << 9)));
    return p;
}

/* FreePl, 0918-0958. */
void stars_list_free(StarsMemory *m, StarsFar p)
{
    if (p) stars_free(m, p, (StarsI16)((word(m, p, 0) >> 9) & 7U));
}
