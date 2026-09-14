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
/* Codec tables are read from original DGROUP; see codec.md for input limits. */
StarsU16 stars_nybble_from_char(const StarsU8 *dgroup, StarsU8 ch);
StarsU8 stars_char_from_nybble(const StarsU8 *dgroup, StarsI16 code);
int stars_compress_user_string(const StarsU8 *dgroup, const StarsU8 *input,
    StarsU8 *output, StarsI16 *capacity);
int stars_decompress_user_string(const StarsU8 *dgroup, const StarsU8 *input,
    StarsI16 length, StarsU8 *output, const StarsI16 *capacity);
void stars_add_back_trailing_spaces(StarsU8 *data, StarsU16 cursor_slot, StarsU16 end);
void stars_chop_trailing_spaces(StarsU8 *data, StarsU16 start, StarsU16 end_slot);
void stars_chop_last_word(StarsU8 *data, StarsU16 start, StarsU16 end_slot);
void stars_int_to_roman(StarsU8 *data, StarsI16 value, StarsU16 output);
StarsI16 stars_parse_numbers(StarsU8 *data, StarsU16 input, StarsU16 output, StarsI16 maximum);
/* pointer_slot holds an offset:selector pointing into data; selector retained. */
StarsU32 stars_get_line(StarsU8 *data, StarsU16 pointer_slot);
void stars_set_victory_check(StarsU8 *data, StarsU16 game, StarsI16 index, StarsI16 enabled);
StarsI16 stars_get_victory_check(const StarsU8 *data, StarsU16 game, StarsI16 index);
StarsI16 stars_get_victory_value(const StarsU8 *data, StarsU16 game, StarsI16 index, StarsI16 raw);
/* Setter requires a valid victory-field index, 0 through 9. */
StarsI16 stars_set_victory_value(StarsU8 *data, StarsU16 game, StarsI16 index, StarsI16 value);
/* Valid template indices 0..4; destination is a valid 36-byte region. */
void stars_init_battle_plan(StarsU8 *data, StarsU16 output, StarsI16 template_index, StarsI16 plan_id);
StarsU32 stars_computer_player_pointer(StarsU16 code_selector, StarsI16 race, StarsI16 level);
void stars_init_new_game3(void);
/* Stat setter index must be 0..15. All memory images are complete 64 KiB. */
StarsI16 stars_set_race_stat(StarsU8 *data, StarsU16 player, StarsI16 index, StarsI16 value);
StarsI16 stars_get_race_trait(const StarsU8 *data, StarsU16 player, StarsI16 index);
void stars_set_race_trait(StarsU8 *data, StarsU16 player, StarsI16 index, StarsI16 enabled);
StarsU16 stars_race_checksum(const StarsU8 *data, StarsU16 player);
void stars_bounds_check_player(StarsU8 *data, StarsU16 player);
/* Valid environmental ranges and nonzero selected half-width required.
 * Planet and global player array refer to the same supplied data image. */
StarsI16 stars_planet_desirability(const StarsU8 *data, StarsU16 planet, StarsI16 player_index);
/* Ordered, nondegenerate ranges or invalid triples repaired to immunity.
 * Player record must be disjoint from player zero or exactly player zero.
 * See innate-habitability.md for the bounded x87 rounding model. */
StarsI32 stars_innate_race_habitability(StarsU8 *data, StarsU16 player);
/* Same record/FPU domain as innate habitability, after bounds correction. */
StarsI16 stars_race_advantage_points(StarsU8 *data, StarsU16 player);
StarsI16 stars_true_max_growth(const StarsU8 *data, StarsI16 player_index);
/* strings is a complete image of NE segment 3; see game-strings.md. */
StarsU16 stars_get_compressed_string(StarsU8 *data, const StarsU8 *strings, StarsI16 id);
StarsI16 stars_copy_game_string(StarsU8 *data, const StarsU8 *strings, StarsI16 id, StarsU16 output);
/* Existing initialized record, static template and RNG seeds in data.
 * Name must terminate within its 32-byte field; see random-race.md. */
void stars_create_random_race(StarsU8 *data, const StarsU8 *strings, StarsU16 player);
/* Exact-count stream transfers, or nonlocal abort on failure. These are
 * integration boundaries for RgFromStream/RgToStream, not Windows APIs. */
typedef void (*StarsReadTransfer)(void *context, StarsU8 *output, StarsU16 count);
typedef void (*StarsWriteTransfer)(void *context, const StarsU8 *input, StarsU16 count);
void stars_read_record(StarsU8 *data, StarsReadTransfer read, void *context);
void stars_write_record(StarsU8 *data, StarsI16 type, StarsU16 count, const StarsU8 *input,
    StarsWriteTransfer write, void *context);
/* Fixed original strings; result must fit 50-byte temporary. Automatic
 * pluralization of trimmed single lowercase e needs saved-SI context (U019). */
StarsU16 stars_player_name(StarsU8 *data, const StarsU8 *strings, StarsI16 index,
    StarsI16 capital, StarsI16 plural, StarsI16 article, StarsI16 grammar, StarsU16 player);
/* Disjoint, contiguous records/buffers. At decoded length 32 the terminator
 * can extend beyond a name field; see documented writable-span requirements. */
void stars_read_player_record(StarsU8 *data, const StarsU8 *strings, StarsU16 player, StarsU16 input);
void stars_write_player_record(StarsU8 *data, StarsU16 player, StarsU16 storage,
    StarsWriteTransfer write, void *context);
typedef StarsU32 (*StarsTickCount)(void *context);
void stars_write_bof(StarsU8 *data, StarsI16 player, StarsI16 type, StarsI16 multi,
    StarsTickCount ticks, StarsWriteTransfer write, void *context);
/* Stream integration boundaries. abort must not return. Error callbacks stand
 * for the still-unreconstructed FileError / formatted write-error alert.
 * memory resolves a contiguous far source span; no selector crossing allowed. */
typedef struct StarsStreamApi {
    StarsI16 (*open)(void *context, const StarsU8 *name, StarsU16 mode, StarsU16 *error);
    StarsI16 (*close)(void *context, StarsI16 handle);
    StarsU16 (*read)(void *context, StarsI16 handle, StarsU8 *output, StarsU16 count);
    StarsU16 (*write)(void *context, StarsI16 handle, const StarsU8 *input, StarsU16 count);
    StarsTickCount ticks;
    const StarsU8 *(*memory)(void *context, StarsU16 selector, StarsU16 offset);
    void (*file_error)(void *context, StarsI16 id);
    void (*write_error)(void *context);
    void (*abort)(void *context, StarsU16 environment, StarsI16 value);
} StarsStreamApi;
void stars_stream_open(StarsU8 *data, StarsU16 name, StarsI16 mode,
    const StarsStreamApi *api, void *context);
void stars_stream_close(StarsU8 *data, const StarsStreamApi *api, void *context);
void stars_stream_read(StarsU8 *data, StarsU8 *output, StarsU16 count,
    const StarsStreamApi *api, void *context);
void stars_stream_write(StarsU8 *data, const StarsU8 *input, StarsU16 count,
    const StarsStreamApi *api, void *context);
/* Filename tables must match the verified original; bounded, contiguous
 * NUL-terminated base and output buffers, without overlap. */
void stars_set_file_name(StarsU8 *data, StarsU16 type, StarsI16 player);
/* environment is a near-offset token for the new native recovery scope.
 * It must be distinct from any active scope and disjoint from application data.
 * The old data[006E] token is restored on normal and recovered-error returns. */
StarsI16 stars_create_file(StarsU8 *data, StarsU16 type, StarsI16 player,
    StarsU16 forced_name, StarsU16 environment, const StarsStreamApi *api, void *context);
typedef struct StarsAlertApi {
    StarsU16 (*focus)(void *context);
    StarsI16 (*message)(void *context, StarsU16 owner, const StarsU8 *text,
        const StarsU8 *caption, StarsI16 type);
    void (*output)(void *context, StarsI16 channel, const StarsU8 *text);
    StarsU16 (*format_id)(void *context, StarsI16 id);
} StarsAlertApi;
/* OutputSz and PszFormatIds remain explicit game-helper boundaries. */
StarsI16 stars_alert_string(StarsU8 *data, StarsU16 text, StarsI16 type,
    const StarsAlertApi *api, void *context);
void stars_file_error(StarsU8 *data, StarsI16 id, const StarsAlertApi *api, void *context);
StarsI16 stars_bad_file_error(StarsI16 id);
#endif
