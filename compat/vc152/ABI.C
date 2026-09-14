/* Separate translation unit keeps actual calls and 32-bit returns observable. */
#include "PROBE.H"

StarsU32 PROBE_FAR PROBE_CDECL probe_cdecl(
    StarsU16 word, StarsU32 value, StarsU8 tail)
{
    return (value ^ ((StarsU32)word << 16)) + tail;
}

StarsU32 PROBE_FAR PROBE_PASCAL probe_pascal(
    StarsU16 word, StarsU32 value, StarsU8 tail)
{
    return (value ^ ((StarsU32)word << 16)) - tail;
}

/* A near call must stay in this module's code segment under /AM. */
static StarsU32 PROBE_NEAR PROBE_CDECL probe_near(
    StarsU16 word, StarsU32 value, StarsU8 tail)
{
    return (value ^ ((StarsU32)word << 16)) ^ tail;
}

StarsU32 PROBE_FAR PROBE_CDECL probe_near_wrapper(
    StarsU16 word, StarsU32 value, StarsU8 tail)
{
    return probe_near(word, value, tail);
}
