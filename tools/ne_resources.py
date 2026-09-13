"""Packed Win16 resource metadata, not Win32 templates. All raw bytes retained."""
import struct

def u(fmt,b,p=0): return struct.unpack_from('<'+fmt,b,p)
def text(b,p):
    end=b.index(0,p)
    return b[p:end].decode('cp1252',errors='replace'),end+1

def value(b,p):
    if b[p]==255:return {'ordinal':u('H',b,p+1)[0]},p+3
    return text(b,p)

def decode(typ,b):
    if typ==5:
        style=u('I',b)[0];n=b[4];rect=u('4h',b,5);p=13
        menu,p=value(b,p);cls,p=text(b,p);caption,p=text(b,p)
        out=dict(style=style,control_count=n,rect=rect,menu=menu,window_class=cls,caption=caption,controls=[])
        if style&0x40:
            out['font_size']=u('H',b,p)[0];out['font'],p=text(b,p+2)
        for i in range(n):
            x,y,w,h,id,style=u('4hHI',b,p);p+=14
            if b[p]&128:
                cls={128:'BUTTON',129:'EDIT',130:'STATIC',131:'LISTBOX',132:'SCROLLBAR',133:'COMBOBOX'}[b[p]];p+=1
            else:cls,p=text(b,p)
            label,p=value(b,p);extra=b[p];p+=1
            out['controls'].append(dict(rect=[x,y,w,h],id=id,style=style,window_class=cls,text=label,creation_data=b[p:p+extra].hex()))
            p+=extra
        out['decoded_bytes']=p
    elif typ==4:
        version,offset=u('HH',b);assert version==0
        def menu(p):
            items=[]
            while True:
                flags=u('H',b,p)[0];p+=2;item={'flags':flags}
                if not flags&16:item['id']=u('H',b,p)[0];p+=2
                item['text'],p=text(b,p)
                if flags&16:item['children'],p=menu(p)
                items.append(item)
                if flags&128:return items,p
        items,p=menu(4+offset);out=dict(items=items,decoded_bytes=p)
    elif typ==9:
        p=0;items=[]
        while True:
            flags,key,command=u('BHH',b,p);p+=5
            items.append(dict(flags=flags,key=key,command=command))
            if flags&128:break
        out=dict(items=items,decoded_bytes=p)
    elif typ==6:
        p=0;strings=[]
        for i in range(16):
            n=b[p];strings.append(b[p+1:p+1+n].decode('cp1252',errors='replace'));p+=n+1
        out=dict(strings=strings,decoded_bytes=p)
    elif typ in (1,2,3):
        p=4 if typ==1 else 0;hs=u('I',b,p)[0]
        if hs==40:
            w,h,planes,bpp,compression,size=u('iiHHII',b,p+4)
            out=dict(dib_header_bytes=hs,width=w,stored_height=h,planes=planes,bits_per_pixel=bpp,compression=compression,image_size_field=size)
        elif hs==12:
            w,h,planes,bpp=u('4H',b,p+4);out=dict(dib_header_bytes=hs,width=w,stored_height=h,planes=planes,bits_per_pixel=bpp)
        else:raise ValueError('Unknown DIB header')
        if typ in (1,3):out['display_height']=h//2
        if typ==1:out['hotspot']=u('HH',b)
    elif typ in (12,14):
        reserved,kind,count=u('3H',b);items=[];p=6
        for i in range(count):
            if typ==14:
                w,h,colors,res,planes,bpp,size,id=u('4B2HIH',b,p)
                items.append(dict(width=w or 256,height=h or 256,colors=colors,planes=planes,bits_per_pixel=bpp,size=size,id=id))
            else:
                w,h,planes,bpp,size,id=u('4HIH',b,p)
                items.append(dict(width=w,height=h,planes=planes,bits_per_pixel=bpp,size=size,id=id))
            p+=14
        out=dict(kind=kind,items=items,decoded_bytes=p)
    else:return dict(decoding='opaque custom resource; raw bytes preserved; semantics are later subsystem work')
    if 'decoded_bytes' in out:
        tail=b[out['decoded_bytes']:]
        if any(tail):raise ValueError('Nonzero unparsed resource tail')
        out['padding_bytes']=len(tail)
    return out
