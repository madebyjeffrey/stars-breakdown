/* Explicit original near-memory storage for operations with observable aliasing. */
#include "core.h"

static StarsU16 read_word(const StarsU8 *memory, StarsU16 offset)
{
    return (StarsU16)((StarsU16)memory[offset] |
        ((StarsU16)memory[(StarsU16)(offset + 1U)] << 8));
}

static void write_word(StarsU8 *memory, StarsU16 offset, StarsU16 value)
{
    memory[offset] = (StarsU8)value;
    memory[(StarsU16)(offset + 1U)] = (StarsU8)(value >> 8);
}

static StarsU32 read_long(const StarsU8 *memory, StarsU16 offset)
{
    return (StarsU32)read_word(memory, offset) |
        ((StarsU32)read_word(memory, (StarsU16)(offset + 2U)) << 16);
}

static void write_long(StarsU8 *memory, StarsU16 offset, StarsU32 value)
{
    write_word(memory, offset, (StarsU16)value);
    write_word(memory, (StarsU16)(offset + 2U), (StarsU16)(value >> 16));
}

/* NE 9:1440-14A2. Reload the index before the second store: an unchecked
 * index can make the first store overwrite the index itself. */
void stars_push_random(StarsU8 *dgroup, StarsU32 first, StarsU32 second)
{
    StarsU16 destination;
    StarsU32 saved;
    destination = (StarsU16)(0x5238U + (StarsU16)(read_word(dgroup, 0x1378U) * 8U));
    saved = read_long(dgroup, 0x1370U);
    write_long(dgroup, destination, saved);
    destination = (StarsU16)(0x5238U + (StarsU16)(read_word(dgroup, 0x1378U) * 8U));
    saved = read_long(dgroup, 0x1374U);
    write_long(dgroup, (StarsU16)(destination + 4U), saved);
    write_word(dgroup, 0x1378U, (StarsU16)(read_word(dgroup, 0x1378U) + 1U));
    write_long(dgroup, 0x1370U, first);
    write_long(dgroup, 0x1374U, second);
}

/* NE 9:14A2-14EA. There is no empty-stack guard in the original. */
void stars_pop_random(StarsU8 *dgroup)
{
    StarsU16 source;
    StarsU32 saved;
    write_word(dgroup, 0x1378U, (StarsU16)(read_word(dgroup, 0x1378U) - 1U));
    source = (StarsU16)(0x5238U + (StarsU16)(read_word(dgroup, 0x1378U) * 8U));
    saved = read_long(dgroup, source);
    write_long(dgroup, 0x1370U, saved);
    source = (StarsU16)(0x5238U + (StarsU16)(read_word(dgroup, 0x1378U) * 8U));
    saved = read_long(dgroup, (StarsU16)(source + 4U));
    write_long(dgroup, 0x1374U, saved);
}

static StarsU32 next_file_xor(StarsU8 *dgroup)
{
    StarsSeeds state;
    StarsU32 value;
    state.first = read_long(dgroup, 0x2272U);
    state.second = read_long(dgroup, 0x2276U);
    value = stars_next_file_xor(&state);
    write_long(dgroup, 0x2272U, state.first);
    write_long(dgroup, 0x2276U, state.second);
    return value;
}

/* NE 9:1CC4-1D74. Do not replace the pointer comparison with count/4:
 * a wrapping end pointer can skip full words. Tail uses low-order bytes. */
void stars_xor_file_buffer(StarsU8 *dgroup, StarsU16 buffer, StarsI16 count)
{
    StarsU16 end, remaining;
    StarsU32 value;
    end = (StarsU16)(buffer + ((StarsU16)count & 0xfffcU));
    while (buffer < end) {
        value = next_file_xor(dgroup);
        write_word(dgroup, buffer, (StarsU16)(read_word(dgroup, buffer) ^ (StarsU16)value));
        write_word(dgroup, (StarsU16)(buffer + 2U),
            (StarsU16)(read_word(dgroup, (StarsU16)(buffer + 2U)) ^ (StarsU16)(value >> 16)));
        buffer = (StarsU16)(buffer + 4U);
    }
    remaining = (StarsU16)((StarsU16)count & 3U);
    if (remaining) {
        value = next_file_xor(dgroup);
        while (remaining) {
            dgroup[buffer] ^= (StarsU8)value;
            /* Arithmetic and logical right shifts yield identical first three
             * low bytes; only those bytes are observable in this tail. */
            value >>= 8;
            buffer = (StarsU16)(buffer + 1U);
            --remaining;
        }
    }
}
