/* Configuration helpers and explicitly scoped UI-setting paths. */
#include "config.h"
#include "runtime_internal.h"

/* NE 1:2b8a-2ded, ReadIniTileSettings. A star changes the column from zero
 * to one only; repeated stars do not keep incrementing it. Character classes
 * come from the original CRT table, including signed-char indexing. */
void stars_read_ini_tiles(StarsU8 *d,StarsU16 text,StarsU16 tiles,StarsI16 count)
{
    StarsU16 column=0,flags,at;
    StarsI16 used=0,index,letter,ch;
    StarsU8 temp[16],classification;
    for(;d[text];++text) {
        ch=d[text]<128U ? (StarsI16)d[text] : (StarsI16)((int)d[text]-256);
        if(ch=='*') {if(column<1U) ++column;continue;}
        classification=d[(StarsU16)(0x175fU+(StarsU16)ch)];
        if(!(classification&3U)) continue;
        letter=(StarsI16)(ch-((classification&1U) ? 'A':'a'));
        if(letter<0) continue;
        for(index=used;index<count;++index)
            if(((stars_word(d,(StarsU16)(tiles+index*16U+10U))>>3)&15U)==(StarsU16)letter) break;
        if(index==count) continue;
        at=(StarsU16)(tiles+index*16U);
        flags=stars_word(d,(StarsU16)(at+10U));
        stars_put_word(d,(StarsU16)(at+10U),(StarsU16)((flags&0xff78U)|column|((classification&1U)<<7)));
        if(index!=used) {
            memcpy(temp,d+at,16);memcpy(d+at,d+tiles+used*16U,16);memcpy(d+tiles+used*16U,temp,16);
        }
        ++used;
    }
    for(index=used;index<count;++index) {
        at=(StarsU16)(tiles+index*16U+10U);flags=stars_word(d,at);
        if((flags&7U)<column) stars_put_word(d,at,(StarsU16)((flags&0xfff8U)|column));
    }
}

/* NE 5:79ac-79f4. Right/bottom become width/height of normal placement. */
void stars_window_rect(StarsRuntime *r,StarsU16 hwnd,StarsU16 out)
{
    StarsU8 placement[22];
    stars_put_word(placement,0,22);
    stars_window(r,"GetWindowPlacement",hwnd,0,0,0,placement,0);
    memcpy(r->data+out,placement+14,8);
    stars_put_word(r->data,(StarsU16)(out+4U),(StarsU16)(stars_word(placement,18)-stars_word(placement,14)));
    stars_put_word(r->data,(StarsU16)(out+6U),(StarsU16)(stars_word(placement,20)-stars_word(placement,16)));
}

/* NE 5:79f6-7a75. The original ignores its sz parameter and always writes
 * szWork. Keep that quirk; returning text elsewhere would change behavior. */
void stars_window_ini_string(StarsRuntime *r,StarsU16 unused,StarsU16 hwnd)
{
    StarsU16 top=r->scratch_top,rect=stars_scratch(r,8U),format;
    int mode;
    (void)unused;
    mode=stars_window(r,"IsZoomed",hwnd,0,0,0,0,0) ? 'M' :
        stars_window(r,"IsIconic",hwnd,0,0,0,0,0) ? 'I':'R';
    stars_window_rect(r,hwnd,rect);format=stars_game_string(r,0x31d);
    sprintf((char *)r->data+0x57a4,(char *)r->data+format,mode,
        (int)stars_signed_word(stars_word(r->data,rect)),
        (int)stars_signed_word(stars_word(r->data,(StarsU16)(rect+2U))),
        (int)stars_signed_word(stars_word(r->data,(StarsU16)(rect+4U))),
        (int)stars_signed_word(stars_word(r->data,(StarsU16)(rect+6U))));
    r->scratch_top=top;
}

static void config_key(StarsRuntime *r,StarsI16 id,StarsU8 *out)
{ strcpy((char *)out,(char *)r->data+stars_game_string(r,id)); }
static StarsU16 read_number(StarsRuntime *r,const StarsU8 *file,StarsU8 *section,StarsI16 id,StarsU16 fallback)
{
    StarsU8 key[32];StarsProfileString profile;
    config_key(r,id,key);profile.section=section;profile.key=key;profile.file=file;profile.fallback=0;
    return (StarsU16)stars_window(r,"GetPrivateProfileInt",0,fallback,0,0,0,&profile);
}
static void write_text(StarsRuntime *r,const StarsU8 *file,StarsU8 *section,StarsI16 id,const StarsU8 *text)
{
    StarsU8 key[32];StarsProfileString profile;
    config_key(r,id,key);profile.section=section;profile.key=key;profile.file=file;profile.fallback=0;
    stars_window(r,"WritePrivateProfileString",0,0,0,0,(void *)text,&profile);
}
static void write_number(StarsRuntime *r,const StarsU8 *file,StarsU8 *section,StarsI16 id,StarsU16 value)
{ StarsU8 text[16];sprintf((char *)text,"%d",(int)stars_signed_word(value));write_text(r,file,section,id,text); }
static void read_text(StarsRuntime *r,const StarsU8 *file,StarsU8 *section,StarsI16 id,StarsU16 size)
{
    StarsU8 key[32];StarsProfileString profile;
    config_key(r,id,key);profile.section=section;profile.key=key;profile.file=file;profile.fallback=(const StarsU8 *)"";
    stars_window(r,"GetPrivateProfileString",0,size,0,0,r->data+0x57a4,&profile);
}
static StarsU16 bound(StarsU16 n,StarsI16 lo,StarsI16 hi)
{ StarsI16 s=stars_signed_word(n);return (StarsU16)(s<lo ? lo : s>hi ? hi:s); }

/* ReadIniSettings's main-frame, layout, pane dimensions, tile ordering,
 * backup-count and fonts paths. Original string IDs, defaults, signedness
 * and clamps are retained; the file argument isolates the native package. */
void stars_read_ui_config(StarsRuntime *r,const StarsU8 *file)
{
    static const StarsU16 panes[6]={0x23d4,0x23d6,0x23d8,0x23da,0x23dc,0x23de};
    StarsU16 top=r->scratch_top,sec=stars_scratch(r,32),name=stars_scratch(r,256),v,i;
    StarsU8 section[32];StarsU8 *d=r->data;
    config_key(r,0x90,section);strcpy((char *)d+sec,(char *)section);strcpy((char *)d+name,(char *)file);
    stars_ini_window_rect(r,sec,name,0x94,0x2290);
    stars_put_word(d,0x22de,bound(read_number(r,file,section,0xa3,1),0,2));
    for(i=0;i<6;++i) stars_put_word(d,panes[i],bound(read_number(r,file,section,(StarsI16)(0xa5+i),i%3==0 ? 396:i%3==1 ? 110:192),10,2000));
    v=read_number(r,file,section,0xab,1);stars_put_word(d,0x7cc,(StarsU16)((stars_word(d,0x7cc)&0x7fff)|(v ? 0x8000:0)));
    read_text(r,file,section,0x96,20);stars_read_ini_tiles(d,0x57a4,0x7fc,6);
    read_text(r,file,section,0x95,20);stars_read_ini_tiles(d,0x57a4,0x90e,7);
    config_key(r,0x92,section);v=read_number(r,file,section,0xb0,1);
    stars_put_word(d,0x9de,(v<1U || stars_signed_word(v)>999 || stars_signed_word(v)<1) ? 1:v);
    stars_title_read_fonts(r,file);r->scratch_top=top;
}
void stars_write_ui_config(StarsRuntime *r,const StarsU8 *file,StarsU16 frame)
{
    static const StarsU16 panes[6]={0x23d4,0x23d6,0x23d8,0x23da,0x23dc,0x23de};
    StarsU8 section[32];StarsU16 i;
    config_key(r,0x90,section);
    if(frame) {stars_window_ini_string(r,0,frame);write_text(r,file,section,0x94,r->data+0x57a4);}
    write_number(r,file,section,0xa3,stars_word(r->data,0x22de));
    for(i=0;i<6;++i) write_number(r,file,section,(StarsI16)(0xa5+i),stars_word(r->data,panes[i]));
    write_number(r,file,section,0xab,(StarsU16)(stars_word(r->data,0x7cc)>>15));
}
