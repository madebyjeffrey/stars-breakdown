#include <stdio.h>
#include "core.h"

typedef struct Trace { StarsI16 random_calls, survive_calls; } Trace;
static StarsI16 random_value(void *value, StarsI16 bound)
{ Trace *t=(Trace *)value; t->random_calls++; (void)bound; return 10; }
static StarsI16 race_stat(void *value, StarsI16 player, StarsI16 index)
{ (void)value; (void)player; (void)index; return 0; }
static StarsI16 survive(void *value, const StarsGuessPlanet *planet)
{ Trace *t=(Trace *)value; t->survive_calls++; (void)planet; return 50; }

int main(void)
{
    StarsGuessPlanet planets[3];
    StarsUpdateGuessesInput input;
    Trace trace;
    planets[0].owner=1; planets[0].guess_word=0xa000U;
    planets[0].mineral_value=1000UL; planets[0].survival_flags=0;
    planets[1].owner=2; planets[1].guess_word=0x5000U;
    planets[1].mineral_value=1000UL; planets[1].survival_flags=1;
    planets[2].owner=3; planets[2].guess_word=0xffffU;
    planets[2].mineral_value=0UL; planets[2].survival_flags=0;
    input.planets=planets; input.planet_count=3; input.random=random_value;
    input.race_stat=race_stat; input.calc_pct_survive=survive;
    trace.random_calls=0; trace.survive_calls=0; input.context=&trace;
    stars_update_guesses(&input);
    if (planets[0].guess_word!=0x00ddU ||
        planets[1].guess_word!=0x90ddU || planets[2].guess_word!=0U ||
        trace.random_calls!=2 || trace.survive_calls!=1)
        return 1;
    puts("UPDATE_GUESSES_SOURCE=PASS"); return 0;
}
