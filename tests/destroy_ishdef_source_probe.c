#include <stdio.h>
#include <string.h>
#include "core.h"
typedef struct State { int split, del, sb, report, queue, refresh, inv; } State;
static void transfer(void *c, StarsDestroyIshdefFleet *s,
        StarsDestroyIshdefFleet *d)
{ State *x=(State *)c; s->ship_counts[2]=0; (void)d; ++x->split; }
static void del(void *c, StarsU16 id, StarsI16 a, StarsI16 b)
{ State *x=(State *)c; (void)id;(void)a;(void)b;++x->del; }
static void sb(void *c, StarsI16 id, StarsI16 p)
{ State *x=(State *)c; (void)id;(void)p;++x->sb; }
static void report(void *c, StarsI16 a, StarsI16 b)
{ State *x=(State *)c; (void)a;(void)b;++x->report; }
static void queue(void *c, StarsI16 a, StarsI16 b)
{ State *x=(State *)c; (void)a;(void)b;++x->queue; }
static void refresh(void *c, StarsU16 id)
{ State *x=(State *)c; (void)id;++x->refresh; }
static void inv(void *c, StarsU16 w)
{ State *x=(State *)c; (void)w;++x->inv; }
int main(void)
{
    StarsDestroyIshdefInput in; StarsDestroyIshdefFleet f[2]; State s;
    memset(&in,0,sizeof(in)); memset(&f,0,sizeof(f)); memset(&s,0,sizeof(s));
    f[0].id=4; f[0].player=2; f[0].ship_counts[2]=3;
    f[1].id=5; f[1].player=2; f[1].ship_counts[2]=2;
    f[1].ship_counts[4]=1; in.fleets=f; in.fleet_count=2; in.player=2;
    in.transfer_balance=transfer; in.delete_fleet=del; in.context=&s;
    in.selected_is_fleet=1; in.selected_fleet_id=5; in.refresh_selection=refresh;
    in.invalidate_window=inv; in.destroy_starbase_design=sb;
    in.invalidate_report=report; in.remove_from_queues=queue;
    stars_destroy_all_ishdef(&in,2);
    if (f[0].ship_counts[2]!=0 || f[1].ship_counts[2]!=0 || s.del!=1 ||
            s.split!=1 || s.refresh!=1 || s.inv!=1 || s.report!=1 ||
            s.queue!=1) return 1;
    stars_destroy_all_ishdef(&in,18);
    if (s.sb!=1 || s.report!=2 || s.queue!=2) return 2;
    puts("DESTROY_ISHDEF_SOURCE=PASS"); return 0;
}
