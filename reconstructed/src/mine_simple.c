/* Genuine C reconstructions of the compact mine UI helpers. */
#include "core.h"
static StarsU16 mine_owner(StarsU16 raw) { return (StarsU16)((raw>>9)&15U); }
void stars_get_mine_field_counts(const StarsMineThing *things, StarsU16 count,
    StarsI16 player, StarsU16 id, StarsI16 *matching, StarsI16 *total)
{
    StarsU16 i; StarsI16 a=0,b=0;
    for(i=0;i<count;++i) if((things[i].raw&0x2000U)==0U && mine_owner(things[i].raw)==(StarsU16)player) {
        ++b; if(things[i].raw==id) ++a;
    }
    *matching=a; *total=b;
}
void stars_draw_selection_arrow(StarsU16 hdc,const StarsRect *r,StarsI16 enabled,
    StarsU16 bitmap,const StarsSelectionArrowOps *o,void *c)
{
    StarsU16 dc=0,old=0; StarsI16 x,y;
    if(o->create_dc)dc=o->create_dc(c,hdc); if(o->select_object)old=o->select_object(c,dc,bitmap);
    x=(StarsI16)((r->right-r->left)/2+r->left-5); y=(StarsI16)((r->bottom-r->top)/2+r->top-5);
    if(o->bit_blt)o->bit_blt(c,hdc,x,y,11,12,dc,35,57,0x008800c6UL);
    if(enabled==0) { x=0; y=45; } else { x=24; y=57; }
    if(o->bit_blt)o->bit_blt(c,hdc,(StarsI16)((r->right-r->left)/2+r->left-5),(StarsI16)((r->bottom-r->top)/2+r->top-5),11,12,dc,x,y,0x00ee0086UL);
    if(o->select_object)o->select_object(c,dc,old); if(o->delete_dc)o->delete_dc(c,dc);
}
void stars_draw_diamond(StarsU16 hdc,const StarsRect *r,StarsU16 brush,StarsU16 highlight,
    StarsU16 shadow,const StarsDiamondOps *o,void *c)
{
    StarsI16 center=(StarsI16)((r->right-r->left)/2+r->left), top=r->top, bottom=r->bottom-2;
    StarsI16 x=(StarsI16)(center-1),y,low; StarsI16 width=1; StarsU16 old;
    if(o->select_object)old=o->select_object(c,hdc,highlight); else old=0;
    for(y=(StarsI16)(top+1),low=bottom;y<=low;++y,--x,--low) { if(o->pat_blt)o->pat_blt(c,hdc,x,y,2,1,0x00f00021UL); if(o->pat_blt)o->pat_blt(c,hdc,x,low,2,1,0x00f00021UL); }
    if(o->select_object)o->select_object(c,hdc,shadow);
    if(o->pat_blt)o->pat_blt(c,hdc,center,top,1,1,0x00f00021UL); if(o->pat_blt)o->pat_blt(c,hdc,center,(StarsI16)(r->bottom-1),1,1,0x00f00021UL);
    x=center; low=(StarsI16)(r->bottom-2);
    for(y=(StarsI16)(top+1);y<=low;++y,--low,++x) { if(o->pat_blt)o->pat_blt(c,hdc,x,y,2,1,0x00f00021UL); if(o->pat_blt)o->pat_blt(c,hdc,x,low,2,1,0x00f00021UL); }
    if(o->select_object)o->select_object(c,hdc,brush); x=center;low=(StarsI16)(r->bottom-5);y=(StarsI16)(top+4);width=1;
    while(y<=low) { if(o->pat_blt)o->pat_blt(c,hdc,x,y,width,1,0x00f00021UL); if(o->pat_blt)o->pat_blt(c,hdc,x,low,width,1,0x00f00021UL); ++width; width+=1; --x; --low; ++y; }
    if(o->select_object)o->select_object(c,hdc,old);
}
