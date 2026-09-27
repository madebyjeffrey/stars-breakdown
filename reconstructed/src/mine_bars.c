/* Genuine C reconstruction of InvalidateMineralBars (NE 6:040a-05ab). */
#include "core.h"
void stars_invalidate_mineral_bars(StarsU16 hwnd, StarsU16 dy, StarsI16 attr_length,
    const StarsMineralBarsOps *o, void *c)
{
    StarsRect client, first, second; StarsU16 dc,old; StarsI16 right, width, attr_width;
    StarsU16 rows;
    client.left=client.top=client.right=client.bottom=0;
    if(o->get_client_rect)o->get_client_rect(c,hwnd,&client);
    dc=o->get_dc?o->get_dc(c,hwnd):0; old=o->select_font?o->select_font(c,dc,0):0;
    width=o->text_width?o->text_width(c,dc,0x229,5):0; right=(StarsI16)client.right-(width+6);
    if(o->select_font)o->select_font(c,dc,1);
    width=o->text_width?o->text_width(c,dc,0x22a,21):0; width=(StarsI16)(width+6);
    attr_width=o->text_width?o->text_width(c,dc,0,(StarsU16)attr_length):0; attr_width=(StarsI16)(attr_width+6);
    if(attr_width*4<=right) right=(StarsI16)(right+attr_width); else right=(StarsI16)(right+o->text_width(c,dc,0,4)+6);
    if(o->select_font)o->select_font(c,dc,old); if(o->release_dc)o->release_dc(c,hwnd,dc);
    first.left=(StarsU16)(right-width); first.top=(StarsU16)(client.top+2); first.right=(StarsU16)right; first.bottom=(StarsU16)(client.top+2+dy);
    if(o->invalidate_rect)o->invalidate_rect(c,hwnd,&first,1);
    rows=(StarsU16)((((StarsU16)(client.bottom-client.top)-(StarsU16)(dy*4)-2U)/6U+1U)&0xfffeU);
    second.left=client.left; second.top=(StarsU16)(client.top+dy*2-4+(dy*5)/2+(StarsI16)rows*3+1);
    second.right=(StarsU16)right; second.bottom=(StarsU16)(second.top+rows*3);
    if(o->invalidate_rect)o->invalidate_rect(c,hwnd,&second,0);
    (void)attr_length;
}
