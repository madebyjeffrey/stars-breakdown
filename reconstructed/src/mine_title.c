/* Genuine C reconstruction of SetMineralTitleBar (NE 6:47dc-4a67). */
#include "core.h"
#include <string.h>
void stars_set_mineral_title_bar(StarsU16 hwnd, StarsU16 child, StarsI16 dy,
    const StarsMineralTitleState *s, const StarsMineralTitleOps *o, void *c)
{
    StarsU8 title[128], label[64], prefix[64], name[64]; StarsU16 object, flags=0; StarsRect r;
    title[0]=0; label[0]=0; prefix[0]=0; name[0]=0;
    if(o->get_string)o->get_string(c,0x362,label,sizeof(label));
    if(o->get_string)o->get_string(c,0x554,prefix,sizeof(prefix));
    object=s->grobj_full; if(s->grobj!=4)object=s->grobj;
    if((object&1U)!=0U) {
        if(s->planet!=-1 && o->get_name)o->get_name(c,1,s->planet,name,sizeof(name));
    } else if((object&2U)!=0U) {
        if(s->fleet!=-1 && o->get_name)o->get_name(c,2,s->fleet,name,sizeof(name));
    } else if((object&8U)!=0U) {
        if(s->thing!=-1) { flags=1; if(o->get_name)o->get_name(c,3,s->thing,name,sizeof(name)); }
    }
    strcpy((char *)title,name[0]?(char *)prefix:(char *)label); if(name[0])strcat((char *)title,(char *)name);
    if(o->set_title)o->set_title(c,title); if(o->invalidate)o->invalidate(c,hwnd,1);
    if(flags) {
        if((s->thing_flags&0x2000U)!=0U || s->thing_kind_flags!=0U ||
            s->thing_owner!=s->player || s->race_stat!=5) flags=0;
    }
    if(flags && o->send_message)o->send_message(c,child,0x0401U,(StarsU16)(s->thing_cargo!=0U));
    r.left=r.top=r.right=r.bottom=0; if(o->get_client_rect)o->get_client_rect(c,hwnd,&r);
    if(o->set_window_pos)o->set_window_pos(c,child,80,(StarsI16)(r.bottom-dy*2),
        (StarsI16)(r.right-88),(StarsI16)(dy+4),(StarsU16)((flags?0x40U:0x80U)|4U));
}
