/* Catch recovery in C: never longjmp across a Python ctypes callback. */
#include "memory.h"
#include <setjmp.h>
static jmp_buf escape;
StarsU16 memory_test_environment;
StarsI16 memory_test_value;
StarsFar memory_test_result;
static void caught(void *context, StarsU16 environment, StarsI16 value)
{
    (void)context;
    memory_test_environment = environment;
    memory_test_value = value;
    longjmp(escape, 1);
}
int memory_test_run(StarsMemory *m, int op, StarsFar p, StarsU16 a, StarsI16 b)
{
    const StarsPlatform *saved = m->runtime->platform;
    StarsPlatform platform = *saved;
    platform.abort = caught;
    m->runtime->platform = &platform;
    memory_test_result = 0;
    if (setjmp(escape)) { m->runtime->platform = saved; return 1; }
    switch (op) {
    case 0: memory_test_result = stars_heap_alloc(m, a, b); break;
    case 1: memory_test_result = stars_heap_realloc(m, p); break;
    case 2: stars_heap_free(m, p); break;
    case 3: stars_heap_reset(m, b); break;
    case 4: memory_test_result = stars_alloc(m, a, b); break;
    case 5: memory_test_result = stars_heap_from_pointer(m, p, b); break;
    case 6: memory_test_result = stars_realloc(m, p, a, b); break;
    case 7: stars_free(m, p, b); break;
    case 8: memory_test_result = stars_list_realloc(m, p, a); break;
    case 9: memory_test_result = stars_list_alloc(m, (StarsU16)p, a, b); break;
    case 10: stars_list_free(m, p); break;
    }
    m->runtime->platform = saved;
    return 0;
}
