"""Bounded 16-bit OMF library reader for the supplied VC++ runtime audit.

Record/index/fixup definitions: TIS OMF 1.1, sections SEGDEF/FIXUPP/LEDATA,
https://openwatcom.org/ftp/devel/docs/omf.pdf . No compiler binaries are exported.
Unsupported members are flagged and excluded from matching; no fuzzy fallback.
"""
from dataclasses import dataclass, field
import struct


class Cursor:
    def __init__(self, data):
        self.data, self.pos = data, 0

    def take(self, n):
        if n < 0 or self.pos + n > len(self.data):
            raise ValueError('Truncated OMF field')
        value = self.data[self.pos:self.pos + n]
        self.pos += n
        return value

    def byte(self):
        return self.take(1)[0]

    def word(self):
        return int.from_bytes(self.take(2), 'little')

    def index(self):
        first = self.byte()
        return ((first & 127) << 8 | self.byte()) if first & 128 else first

    def string(self):
        return self.take(self.byte()).decode('ascii')

    def remaining(self):
        return len(self.data) - self.pos


@dataclass
class Segment:
    name: str
    class_name: str
    size: int
    data: bytearray = field(init=False)
    initialized: bytearray = field(init=False)
    fixups: list = field(default_factory=list)
    overwrites: list = field(default_factory=list)

    def __post_init__(self):
        self.data = bytearray(self.size)
        self.initialized = bytearray(self.size)


@dataclass
class Module:
    name: str
    offset: int
    end: int = 0
    names: list = field(default_factory=lambda: [''])
    segments: list = field(default_factory=lambda: [None])
    externals: list = field(default_factory=lambda: [''])
    publics: list = field(default_factory=list)
    threads: dict = field(default_factory=dict)
    record_types: set = field(default_factory=set)
    unsupported: list = field(default_factory=list)
    aliases: dict = field(default_factory=dict)
    imports: dict = field(default_factory=dict)
    weak: dict = field(default_factory=dict)


def iterated(c):
    """Expand one 16-bit LIDATA block, retaining encoded-byte -> output sites."""
    repeat, children = c.word(), c.word()
    data, mapping = bytearray(), {}
    if children:
        for _ in range(children):
            child, sites = iterated(c)
            at = len(data)
            data.extend(child)
            for key, offsets in sites.items():
                mapping.setdefault(key, []).extend(at + x for x in offsets)
    else:
        length = c.byte()
        start = c.pos
        data.extend(c.take(length))
        mapping = {start + i: [i] for i in range(length)}
    size = len(data)
    if size * repeat > 65536:
        raise ValueError('LIDATA expansion exceeds 16-bit segment')
    return data * repeat, {key: [i * size + x for i in range(repeat) for x in offsets]
                           for key, offsets in mapping.items()}


def fixups(module, c, current):
    while c.remaining():
        first = c.byte()
        if not first & 128:
            frame = bool(first & 64)
            # TIS OMF: target-thread method uses only the low two bits.
            # Its high bit comes from the referring FIXUP P bit.
            method = (first >> 2) & (7 if frame else 3)
            datum = c.index() if method < 3 else None
            module.threads[frame, first & 3] = (method, datum)
            continue
        if current is None:
            raise ValueError('FIXUPP without data')
        segment, base, mapping = current
        location, relative = (first >> 2) & 15, not bool(first & 64)
        offset = ((first & 3) << 8) | c.byte()
        fix = c.byte()
        if fix & 128:
            frame = module.threads[True, (fix >> 4) & 3]
        else:
            method = (fix >> 4) & 7
            frame = (method, c.index() if method < 3 else None)
        if fix & 8:
            target = module.threads[False, fix & 3]
        else:
            target = (fix & 3, c.index())
        displacement = 0 if fix & 4 else c.word()
        width = {0: 1, 1: 2, 2: 2, 3: 4, 4: 1, 5: 2}.get(location)
        if width is None:
            raise ValueError(f'Unsupported 16-bit fixup location {location}')
        sites = mapping.get(offset)
        if sites is None:
            raise ValueError('Fixup location is not initialized data')
        for site in sites:
            if any(site + delta not in mapping.get(offset + delta, []) for delta in range(width)):
                raise ValueError('Fixup crosses its encoded data field')
            if base + site + width > len(segment.data):
                raise ValueError('Fixup crosses segment end')
            segment.fixups.append(dict(offset=base + site, width=width, location=location,
                relative=relative, frame=frame, target=target, displacement=displacement))


def library(blob):
    """Read member records; validate lengths/checksums and skip page padding."""
    pos, page, module, current = 0, None, None, None
    members = []
    while pos < len(blob):
        if pos + 3 > len(blob):
            raise ValueError('Truncated OMF record header')
        kind, length = struct.unpack_from('<BH', blob, pos)
        end = pos + 3 + length
        if not length or end > len(blob):
            raise ValueError(f'Invalid OMF record at {pos:x}')
        raw = blob[pos:end]
        if kind == 0xf0:
            page = length + 3
            if pos != 0 or page & (page - 1):
                raise ValueError('Invalid library page size')
            pos = end
            continue
        if kind == 0xf1:
            if module is not None:
                raise ValueError('Unterminated library member')
            break
        if raw[-1] and sum(raw) & 255:
            raise ValueError(f'OMF checksum mismatch at {pos:x}')
        c = Cursor(raw[3:-1])
        if kind == 0x80:
            if module is not None:
                raise ValueError('Nested library member')
            module = Module(c.string(), pos)
            current = None
        elif module is None:
            raise ValueError(f'Record outside member at {pos:x}')
        elif module.unsupported and kind != 0x8a:
            pass
        elif kind == 0x96:
            while c.remaining():
                module.names.append(c.string())
        elif kind == 0x98:
            attr = c.byte()
            if not attr >> 5:
                c.take(3)
            size = c.word()
            if attr & 2:
                size = 65536
            name, cls, overlay = c.index(), c.index(), c.index()
            module.segments.append(Segment(module.names[name], module.names[cls], size))
        elif kind in [0x8c, 0xb4]:
            while c.remaining():
                module.externals.append(c.string())
                c.index()
        elif kind == 0xb0:
            while c.remaining():
                module.externals.append(c.string())
                c.index()
                datatype = c.byte()
                for _ in range(2 if datatype == 0x61 else 1):
                    leaf = c.byte()
                    if leaf > 128:
                        c.take({0x81: 2, 0x84: 3, 0x88: 4}[leaf])
        elif kind in [0x90, 0x91, 0xb6, 0xb7]:
            group, seg = c.index(), c.index()
            absolute = c.word() if not seg else None
            while c.remaining():
                name = c.string()
                off = int.from_bytes(c.take(4 if kind & 1 else 2), 'little')
                typ = c.index()
                module.publics.append(dict(name=name, segment=seg, offset=off, group=group, absolute=absolute))
        elif kind in [0xa0, 0xa2]:
            seg, offset = c.index(), c.word()
            segment = module.segments[seg]
            if kind == 0xa0:
                data = c.take(c.remaining())
                mapping = {i: [i] for i in range(len(data))}
            else:
                expanded, mapping = bytearray(), {}
                start = c.pos
                while c.remaining():
                    data, sites = iterated(c)
                    at = len(expanded)
                    expanded.extend(data)
                    for key, positions in sites.items():
                        mapping[key - start] = [at + x for x in positions]
                data = expanded
            if offset + len(data) > segment.size:
                raise ValueError('OMF data exceeds declared segment')
            if any(segment.initialized[offset:offset + len(data)]):
                # Microsoft's assembly members patch an earlier RETF into
                # RETF n using a later LEDATA. Preserve record order explicitly.
                if any(f['offset'] < offset + len(data) and offset < f['offset'] + f['width'] for f in segment.fixups):
                    raise ValueError('Data overwrite intersects an earlier fixup')
                segment.overwrites.append(dict(record_offset=pos, offset=offset, size=len(data)))
            segment.data[offset:offset + len(data)] = data
            segment.initialized[offset:offset + len(data)] = b'\1' * len(data)
            current = segment, offset, mapping
        elif kind == 0x9c:
            fixups(module, c, current)
        elif kind == 0x8a:
            module.end = end
            members.append(module)
            module = None
            current = None
            pos = ((end + page - 1) // page) * page if page else end
            continue
        elif kind == 0xc6:
            while c.remaining():
                alias, target = c.string(), c.string()
                module.aliases[alias] = target
        elif kind == 0x88:
            attribute, cls = c.byte(), c.byte()
            if cls in [0xa8, 0xa9]:
                while c.remaining():
                    weak, default = c.index(), c.index()
                    module.weak[module.externals[weak]] = module.externals[default]
            elif cls == 0xa0 and c.remaining() and c.byte() == 1:
                ordinal = c.byte()
                internal, dll = c.string(), c.string()
                imported = '#' + str(c.word()) if ordinal else c.string() or internal
                module.imports[internal] = dll.upper() + '!' + imported
        elif kind in [0x9a, 0x94, 0x8e]:
            pass  # groups and line/type information contain no data bytes
        else:
            module.unsupported.append(dict(record_offset=pos, kind=kind))
        module.record_types.add(kind)
        pos = end
    if module is not None:
        raise ValueError('Truncated library member')
    return members
