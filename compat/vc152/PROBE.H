/* Compatibility fixture interface; all guest filenames are DOS 8.3. */
#ifndef STARS_PROBE_H
#define STARS_PROBE_H
#include "CORE.H"
#ifdef HOST_TEST
#define PROBE_FAR
#define PROBE_NEAR
#define PROBE_CDECL
#define PROBE_PASCAL
#else
#if !defined(_MSC_VER) || !defined(_M_I86) || defined(_WIN32)
#error Use the 16-bit Microsoft C compiler supplied with Visual C++ 1.52c
#endif
#ifndef _M_I86MM
#error This experiment requires the medium memory model (/AM)
#endif
#define PROBE_FAR __far
#define PROBE_NEAR __near
#define PROBE_CDECL __cdecl
#define PROBE_PASCAL __pascal
#endif

typedef StarsU32 (PROBE_FAR PROBE_PASCAL *ProbeCallback)(
    StarsU16 word, StarsU32 value, StarsU8 tail);
StarsU32 PROBE_FAR PROBE_CDECL probe_cdecl(
    StarsU16 word, StarsU32 value, StarsU8 tail);
StarsU32 PROBE_FAR PROBE_PASCAL probe_pascal(
    StarsU16 word, StarsU32 value, StarsU8 tail);
StarsU32 PROBE_FAR PROBE_CDECL probe_near_wrapper(
    StarsU16 word, StarsU32 value, StarsU8 tail);
void check_u32(const char *name, unsigned index, StarsU32 actual, StarsU32 expected);
void run_vectors(void);
#endif
