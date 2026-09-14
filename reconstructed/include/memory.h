/* Win16 heap addresses are selector:offset tokens, never host addresses.
 * resolve supplies a writable 64 KiB selector image. The Global* callbacks
 * implement operating-system services, not game allocation policy. */
#ifndef STARS_MEMORY_H
#define STARS_MEMORY_H
#include "runtime.h"

typedef StarsU32 StarsFar;
typedef struct StarsHeapApi {
    StarsU8 *(*resolve)(void *, StarsU16);
    StarsU16 (*alloc)(void *, StarsU16, StarsU32);
    StarsU16 (*realloc)(void *, StarsU16, StarsU32, StarsU16);
    StarsFar (*lock)(void *, StarsU16);
    StarsU16 (*unlock)(void *, StarsU16);
    StarsU16 (*free)(void *, StarsU16);
} StarsHeapApi;
typedef struct StarsMemory {
    StarsRuntime *runtime;
    const StarsHeapApi *api;
    void *context;
} StarsMemory;

StarsFar stars_heap_alloc(StarsMemory *, StarsU16, StarsI16);
StarsFar stars_heap_realloc(StarsMemory *, StarsFar);
void stars_heap_free(StarsMemory *, StarsFar);
void stars_heap_reset(StarsMemory *, StarsI16);
StarsFar stars_alloc(StarsMemory *, StarsU16, StarsI16);
StarsFar stars_heap_from_pointer(StarsMemory *, StarsFar, StarsI16);
StarsFar stars_realloc(StarsMemory *, StarsFar, StarsU16, StarsI16);
void stars_free(StarsMemory *, StarsFar, StarsI16);
StarsFar stars_list_realloc(StarsMemory *, StarsFar, StarsU16);
StarsFar stars_list_alloc(StarsMemory *, StarsU16, StarsU16, StarsI16);
void stars_list_free(StarsMemory *, StarsFar);
#endif
