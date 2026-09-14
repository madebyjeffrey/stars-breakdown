/* Code-generation experiment: original 16-bit interfaces, not host adapters. */
typedef struct player_prefix {
    char prefix[62];
    signed char rgStat[16];
} PLAYER;

int GetRaceStat(PLAYER *pplr, int iStat)
{
    return pplr->rgStat[iStat];
}

static signed char __based(__segname("_CODE")) rgMin[16] =
    {7,5,5,5,5,2,5,0,0,0,0,0,0,0,0,0};
static signed char __based(__segname("_CODE")) rgMax[16] =
    {25,15,25,25,25,15,25,6,2,2,2,2,2,2,9,0};

int SetRaceStat(PLAYER *pplr, int iStat, int iVal)
{
    if (iVal < rgMin[iStat]) iVal = rgMin[iStat];
    if (iVal > rgMax[iStat]) iVal = rgMax[iStat];
    pplr->rgStat[iStat] = iVal;
    return iVal;
}

extern long lSeed1, lSeed2;
int Random(int c)
{
    long s2;
    long s1;
    long k;
    long z;
    s1 = lSeed1;
    s2 = lSeed2;
    k = s1 / 53668L;
    s1 = 40014L * (s1 - k * 53668L) - k * 12211L;
    if (s1 < 0) s1 += 2147483563L;
    k = s2 / 52774L;
    s2 = 40692L * (s2 - k * 52774L) - k * 3791L;
    if (s2 < 0) s2 += 2147483399L;
    z = s1 - s2;
    if (z < 1) z += 2147483562L;
    lSeed1 = s1;
    lSeed2 = s2;
    if (c <= 0) return 0;
    return (unsigned long)z % c;
}
