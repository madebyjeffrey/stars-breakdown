/* Reviewed semantic core. The state parameter replaces original DGROUP globals;
 * this host-test interface is not the original far-call ABI. C89 compatible. */
#ifndef STARS_CORE_H
#define STARS_CORE_H
#include <limits.h>
typedef unsigned char StarsU8;
typedef unsigned short StarsU16;
typedef signed short StarsI16;
#if UINT_MAX == 0xffffffffU
typedef unsigned int StarsU32;
typedef signed int StarsI32;
#elif ULONG_MAX == 0xffffffffUL
typedef unsigned long StarsU32;
typedef signed long StarsI32;
#else
#error A 32-bit unsigned integer type is required
#endif
typedef char stars_core_byte_width[(UCHAR_MAX == 255) ? 1 : -1];
typedef char stars_core_word_width[(USHRT_MAX == 65535) ? 1 : -1];
typedef struct StarsSeeds { StarsU32 first, second; } StarsSeeds;

void stars_randomize(StarsSeeds *state, StarsU32 seed);
void stars_randomize2(StarsSeeds *state, StarsU32 seed);
StarsI16 stars_random(StarsSeeds *state, StarsI16 count);
StarsU32 stars_next_file_xor(StarsSeeds *state);
void stars_get_file_seeds(const StarsSeeds *state, StarsU32 *first, StarsU32 *second);
void stars_set_file_seeds(StarsSeeds *state, StarsU32 first, StarsU32 second);
void stars_set_file_xor_stream(StarsSeeds *state, StarsU32 game_id,
    StarsI16 salt, StarsI16 turn, StarsI16 player, StarsI16 crippled);
StarsU32 stars_distance_squared(StarsI16 x1, StarsI16 y1, StarsI16 x2, StarsI16 y2);
/* Rectangle fields retain their raw 16-bit representation. */
typedef struct StarsRect { StarsU16 left, top, right, bottom; } StarsRect;
void stars_expand_rect(StarsRect *rect, StarsI16 dx, StarsI16 dy);
void stars_offset_rect(StarsRect *rect, StarsI16 dx, StarsI16 dy);
StarsI16 stars_compare_long(StarsU32 first, StarsU32 second);
/* dgroup points to a complete 65536-byte near-address space. */
StarsI16 stars_get_race_stat(const StarsU8 *dgroup, StarsU16 player, StarsI16 index);
/* These routines retain the original DGROUP offsets and unchecked stack index. */
void stars_push_random(StarsU8 *dgroup, StarsU32 first, StarsU32 second);
void stars_pop_random(StarsU8 *dgroup);
/* Requires each original word access to remain inside the 64 KiB segment.
 * Retains unsigned end-pointer comparison, even for a negative count. */
void stars_xor_file_buffer(StarsU8 *dgroup, StarsU16 buffer, StarsI16 count);
#endif
