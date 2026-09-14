#include "TYPES.H"

int FCheckResearch(int iTech, int iTechNext, int pct) {
    if ((rgplr[0].iTechCur & 15) == iTech && (rgplr[0].iTechCur >> 4) == iTechNext &&
        rgplr[0].pctResearch == pct)
        return 1;
    tutor.idh = 0x42e;
    return 0;
}
