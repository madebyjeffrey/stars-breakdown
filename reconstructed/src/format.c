/* Message expansion and its name-lookup data paths. Input strings, names and
 * parameter spans must fit the original buffers; see message-formatting.md. */
#include "runtime_internal.h"
#include <assert.h>

static const StarsU8 *far_address(StarsRuntime *r, StarsU32 address)
{
    if (!address) return 0;
    return r->platform->memory(r->context, (StarsU16)(address >> 16), (StarsU16)address);
}
static StarsU32 far_add(StarsU32 address, StarsU16 count)
{ return (address & 0xffff0000UL) | (StarsU16)((StarsU16)address + count); }
static StarsU16 player_name(StarsRuntime *r, StarsU16 index, int plural, int article)
{ return stars_player_name(r->data, r->strings, stars_signed_word(index), 0,
    (StarsI16)plural, (StarsI16)article, 0, 0); }

/* NE 10:6058-60E6. Every intermediate int operation is a 16-bit one. */
StarsU16 stars_gravity_string(StarsRuntime *r, StarsI16 gravity)
{
    StarsI16 distance = stars_signed_word((StarsU16)((StarsU16)gravity - 50U));
    StarsI16 value;
    if (distance < 0) distance = stars_signed_word((StarsU16)(0U - (StarsU16)distance));
    value = distance < 26 ? stars_signed_word((StarsU16)((StarsU16)distance * 4U + 100U)) :
        stars_signed_word((StarsU16)(((StarsU16)distance - 25U) * 24U + 200U));
    if (gravity < 50) value = (StarsI16)(10000L / value);
    sprintf((char *)r->data + 0x57a4U, (char *)r->data + 0x8bbU,
        (int)(value / 100), (int)(value % 100));
    return 0x57a4U;
}
StarsU16 stars_environment_string(StarsRuntime *r, StarsI16 environment, StarsI16 value)
{
    if (environment == 1)
        sprintf((char *)r->data + 0x57a4U, (char *)r->data + 0x8b0U,
            (int)stars_signed_word((StarsU16)((StarsU16)value * 4U - 200U)), 0xba);
    else if (environment == 2)
        sprintf((char *)r->data + 0x57a4U, (char *)r->data + 0x8b6U, (int)value);
    else return stars_gravity_string(r, value);
    return 0x57a4U;
}

/* NE 9:1D96-1F64. Unlike general text, planet names capitalize words. The
 * original reads iLastGet but doesn't update that cache marker. */
static StarsU16 compressed_planet(StarsRuntime *r, StarsI16 id)
{
    const StarsU8 *s = r->segments[9];
    StarsU8 *d = r->data, ch;
    StarsU16 lengths, sum = 0, cursor, count, nibble, code = 0, i, out = 0x22c8U;
    int high, capital = 1;
    StarsI16 cached = d[0x137aU] < 128U ? (StarsI16)d[0x137aU] : (StarsI16)((int)d[0x137aU] - 256);
    if (id > 998) id = (StarsI16)(id % 999);
    if (id == cached) return out;
    assert(s != 0 && id >= 0);
    lengths = (StarsU16)(0x1004U + ((StarsU16)id / 64U) * 64U);
    for (i = 0; i < ((StarsU16)id & 63U); ++i) sum = (StarsU16)(sum + s[lengths++]);
    cursor = (StarsU16)(stars_word(s, (StarsU16)(0x13ecU + ((StarsU16)id / 64U) * 2U)) + sum / 2U);
    high = !(sum & 1U); count = s[lengths];
    for (i = 0; i < count; ++i) {
        nibble = high ? (StarsU16)(s[cursor] >> 4) : (StarsU16)(s[cursor++] & 15U);
        high = !high; code = (StarsU16)(code + nibble);
        if (nibble != 15U) {
            ch = s[0x140cU + code];
            if (capital && ch >= 'a' && ch <= 'z') ch = (StarsU8)(ch - 32U);
            capital = ch == ' ' || ch == '-'; d[out++] = ch; code = 0;
        }
    }
    d[out] = 0;
    return 0x22c8U;
}
static StarsU16 planet_name(StarsRuntime *r, StarsU16 id)
{
    StarsU16 name = compressed_planet(r, stars_signed_word(stars_word(r->data,
        (StarsU16)(0x276eU + (id & 0x7fffU) * 2U))));
    if (id & 0x8000U)
        sprintf((char *)r->data + 0x57a4U, (char *)r->data + stars_game_string(r, 0x365), r->data + name);
    else strcpy((char *)r->data + 0x57a4U, (char *)r->data + name);
    return 0x57a4U;
}
static const StarsU8 *thing_from_id(StarsRuntime *r, StarsU16 id)
{
    StarsU32 base = stars_dword(r->data, 0x1698U), cursor = base;
    StarsU16 end = (StarsU16)((StarsU16)base + stars_word(r->data, 0x169cU) * 18U);
    const StarsU8 *thing;
    while ((StarsU16)cursor < end) {
        thing = far_address(r, cursor);
        if (stars_word(thing, 0) == id) return thing;
        cursor = far_add(cursor, 18U);
    }
    return 0;
}
static StarsU16 thing_name(StarsRuntime *r, StarsU16 id)
{
    const StarsU8 *thing = thing_from_id(r, id);
    StarsU8 *d = r->data;
    StarsU16 type, owner, format;
    char prefix[54];
    if (!thing) { d[0x57a4U] = 0; return 0x57a4U; }
    type = (StarsU16)(stars_word(thing, 0) >> 13);
    owner = (StarsU16)((stars_word(thing, 0) >> 9) & 15U);
    if (type == 0 || (type == 1 && ((stars_word(thing, 6) >> 10) & 15U))) {
        if (owner == stars_word(d, 0x18cU)) prefix[0] = 0;
        else sprintf(prefix, (char *)d + (type == 0 ? 0x518U : 0x51cU),
            d + player_name(r, owner, 0, 0));
        format = stars_game_string(r, type == 0 ? 0x364 : 0x395);
        if (type == 0) sprintf((char *)d + 0x57a4U, (char *)d + format,
            prefix, d + stars_word(d, (StarsU16)(0x4f2U + thing[12] * 2U)));
        else sprintf((char *)d + 0x57a4U, (char *)d + format, prefix);
    } else stars_copy_game_string(d, r->strings,
        type == 1 ? 0x396 : type == 2 ? 0x4e4 : type == 3 ? 0x4e5 : 0x4e7, 0x57a4U);
    return 0x57a4U;
}
static const StarsU8 *fleet_from_id(StarsRuntime *r, StarsU16 id)
{
    StarsU8 *d = r->data;
    int i, low = 0, high = stars_signed_word(stars_word(d, 0x5356U)), middle, key;
    StarsU32 table = stars_dword(d, 0xfaU);
    const StarsU8 *fleet, *entry;
    for (i = 0; i < (int)((id >> 9) & 15U); ++i)
        low = stars_signed_word((StarsU16)(low + (stars_word(d, (StarsU16)(0x59a6U + i * 192U)) & 4095U)));
    --low; id &= 0x1fffU;
    while (low + 1 < high) {
        middle = (low + high) / 2;
        if (low + high < 0 && ((low + high) & 1)) --middle;
        entry = far_address(r, far_add(table, (StarsU16)(middle * 4)));
        fleet = far_address(r, stars_dword(entry, 0)); key = stars_signed_word(stars_word(fleet, 0));
        if (key < id) low = middle;
        else if (key > id) high = middle;
        else return fleet;
    }
    return 0;
}
static const StarsU8 *design(StarsRuntime *r, StarsU16 owner, StarsU16 index)
{
    StarsU16 slot = (StarsU16)((index < 16U ? 0xfeU : 0x14cU) + owner * 4U);
    return far_address(r, far_add(stars_dword(r->data, slot), (StarsU16)((index & 15U) * 147U)));
}
static StarsU16 primary_design(StarsRuntime *r, const StarsU8 *fleet, StarsU16 *different)
{
    StarsU16 i, result = 16, owner = stars_word(fleet, 2), hull;
    StarsI16 count, maximum = 0;
    *different = 0;
    for (i = 0; i < 16; ++i) {
        count = stars_signed_word(stars_word(fleet, (StarsU16)(12U + i * 2U)));
        if (count > 0) {
            ++*different;
            if (count > maximum) {
                result = i; maximum = count; hull = stars_word(design(r, owner, i), 0);
                if (hull == 25U || hull == 26U) --maximum;
            }
        }
    }
    return result;
}
static StarsU16 fleet_name(StarsRuntime *r, StarsU16 id)
{
    const StarsU8 *fleet = fleet_from_id(r, (StarsU16)(id & 0x7fffU));
    StarsU8 *d = r->data;
    StarsU16 owner = (StarsU16)((id >> 9) & 15U), index, different, length;
    const StarsU8 *name;
    char prefix[34], local_name[34];
    if (owner == stars_word(d, 0x18cU)) prefix[0] = 0;
    else sprintf(prefix, (char *)d + 0x520U, d + player_name(r, owner, 0, 0));
    if (fleet && stars_dword(fleet, 120U)) {
        sprintf((char *)d + 0x57a4U, (char *)d + 0x524U,
            prefix, far_address(r, stars_dword(fleet, 120U)));
    } else {
        index = fleet ? primary_design(r, fleet, &different) : 16;
        if (index == 16U) name = d + stars_game_string(r, 0x4e8);
        else {
            strcpy(local_name, (const char *)design(r, owner, index) + 8);
            length = (StarsU16)strlen(local_name); if (length > 28U) length = 28U;
            if (different > 1U) { local_name[length] = '+'; local_name[length + 1U] = 0; }
            name = (const StarsU8 *)local_name;
        }
        sprintf((char *)d + 0x57a4U, (char *)d + 0x529U, prefix, name, (int)((id & 511U) + 1U));
    }
    return 0x57a4U;
}
static StarsU16 fleet_word_name(StarsRuntime *r, StarsU16 word)
{
    const StarsU8 *ship = design(r, stars_word(r->data, 0x18cU), (StarsU16)((word >> 9) & 15U)), *name;
    StarsU16 length;
    char local[34];
    if (stars_word(ship, 123U) & 512U) name = r->data + stars_game_string(r, 0x4e8);
    else {
        strcpy(local, (const char *)ship + 8); length = (StarsU16)strlen(local);
        if (length > 28U) length = 28U;
        if (word & 0x2000U) { local[length] = '+'; local[length + 1U] = 0; }
        name = (const StarsU8 *)local;
    }
    sprintf((char *)r->data + 0x57a4U, (char *)r->data + 0x532U, name, (int)((word & 511U) + 1U));
    return 0x57a4U;
}
/* In the \k branch only the component pointer from FLookupPart is observable:
 * availability/TechStatus results are discarded, and those queries don't
 * mutate globals. This table is that verified data-flow slice, not a claim to
 * reconstruct the separate availability routine. Valid component ids only. */
static const StarsU8 *part_name(StarsRuntime *r, StarsU16 type, StarsU16 index)
{
    static const StarsU16 bases[16] = {0,0x768,0xae8,0x4e0,0x2450,0x2180,0x1e1a,0x1508,
        0x12ec,0x4d8a,0x4872,0xd04,0x109a,0x19e2,0x29f0,0x16b8};
    static const StarsU16 strides[16] = {78,56,54,54,60,60,58,54,54,56,143,54,54,54,143,54};
    static const StarsU16 limits[16] = {16,16,10,12,24,12,15,8,10,16,5,17,11,20,32,15};
    unsigned int slot = 0;
    assert(type && !(type & (type - 1U)) && r->segments[2]);
    while (type > 1U) { type >>= 1; ++slot; }
    index &= 255U; assert(index < limits[slot]);
    return r->segments[2] + bases[slot] + index * strides[slot] + 8U;
}
static StarsU16 location_name(StarsRuntime *r, StarsU16 x, StarsU16 y)
{
    if (x == 65535U && y == 65535U) stars_copy_game_string(r->data, r->strings, 0x362, 0x57a4U);
    else sprintf((char *)r->data + 0x57a4U, (char *)r->data + stars_game_string(r, 0x363),
        (int)stars_signed_word(x), (int)stars_signed_word(y));
    return 0x57a4U;
}
static StarsU16 append(StarsU8 *d, StarsU16 out, const StarsU8 *text)
{
    /* strcpy writes the NUL even for an empty fragment. */
    while (*text) d[out++] = *text++;
    d[out] = 0;
    return out;
}

/* NE 7:85CC-8F67. Parameters are a valid contiguous little-endian word span.
 * \V observes its unit word without consuming it; zero \Z consumes nothing. */
StarsU16 stars_format_string(StarsRuntime *r, StarsU16 format, const StarsU8 *parameters)
{
    StarsU8 *d = r->data;
    const StarsU8 *text;
    StarsU16 out = 0x535aU, unit = 65535U, at = 0, word, mask, owner, i, count, source;
    StarsU8 ch;
    StarsU32 long_word;
    long signed_long;
    int consume, value, length;
    char temporary[480];
    while (d[format]) {
        if (d[format] != '\\') { d[out++] = d[format++]; continue; }
        ch = d[++format]; text = 0; consume = 0;
        /* Non-parameter escapes must work with the original NULL pParams. */
        switch (ch) {
        case 'w': out = append(d, out, d + 0x57a4U); break;
        case 'f': case 'h': case 'r': case 't': case 'y':
            out = append(d, out, d + 0x56a2U); owner = stars_word(d, 0x18cU);
            if (ch == 'y') out = append(d, out, d + 0xb68U);
            else if (ch == 'h' || ((ch == 'f' || ch == 't') && owner == 65535U))
                out = append(d, out, d + 0xb5eU);
            else {
                source = ch == 'f' ? 0xb54U : ch == 'r' ? 0xb63U : 0xb59U;
                length = sprintf((char *)d + out, (char *)d + source,
                    (int)stars_signed_word((StarsU16)(owner + 1U)));
                out = (StarsU16)(out + length); consume = 1;
            }
            break;
        case 'E': case 'F': case 'G': case 'I': case 'L': case 'l': case 'M': case 'O':
        case 'P': case 'S': case 'U': case 'V': case 'v': case 'X': case 'Z':
        case 'e': case 'g': case 'i': case 'j': case 'k': case 'm': case 'n':
        case 'o': case 'p': case 's': case 'u': case 'z':
            assert(parameters != 0);
            word = stars_word(parameters, at); consume = 1;
            switch (ch) {
            case 'E': text = d + stars_environment_string(r, (StarsI16)(word >> 8), (StarsI16)(word & 255U)); break;
            case 'F': text = d + fleet_word_name(r, word); break;
            case 'G': text = d + stars_game_string(r, stars_signed_word((StarsU16)(word + 0x4e2U))); break;
            case 'I': text = d + stars_game_string(r, stars_signed_word((StarsU16)(word + 0x544U))); break;
            case 'j': text = d + stars_game_string(r, stars_signed_word((StarsU16)(word + 0x54U))); break;
            case 'L': case 'l': text = d + stars_player_name(d, r->strings, (StarsI16)(word & 15U),
                (StarsI16)(ch == 'L'), (StarsI16)((word & 16U) != 0), (StarsI16)((word & 32U) != 0),
                (StarsI16)((word >> 6) & 3U), 0); break;
            case 'M': text = d + stars_word(d, (StarsU16)(0x4f2U + word * 2U)); break;
            case 'm': unit = word; text = d + stars_word(d, (StarsU16)(0x4ccU + word * 2U)); break;
            case 'e': text = d + stars_word(d, (StarsU16)(0x47eU + word * 2U)); break;
            case 'O': text = d + player_name(r, (StarsU16)((word >> 9) & 15U), 0, 0); break;
            case 'P':
                value = stars_signed_word(word);
                if (value / 100 < 10) sprintf(temporary, (char *)d + stars_word(d, 0x1d0U), value / 100, value % 100);
                else sprintf(temporary, (char *)d + stars_word(d, 0x1b8U), value / 100);
                text = (StarsU8 *)temporary; break;
            case 'S':
                if (word != stars_word(d, 0x18cU)) {
                    strcpy(temporary, (char *)d + stars_game_string(r, 0x546));
                    strcat(temporary, (char *)d + player_name(r, word, 0, 0));
                    strcat(temporary, (char *)d + stars_game_string(r, 0x547));
                    text = (StarsU8 *)temporary;
                }
                break;
            case 'U': case 'V': case 'v':
                long_word = stars_dword(parameters, at);
                signed_long = long_word <= 0x7fffffffUL ? (long)long_word : -1L - (long)(0xffffffffUL - long_word);
                length = sprintf((char *)d + out, "%ld", signed_long); out = (StarsU16)(out + length);
                consume = 2;
                if (ch != 'v') {
                    if (ch == 'V') unit = stars_word(parameters, (StarsU16)(at + 4U));
                    text = d + stars_word(d, (StarsU16)(0xb48U + unit * 2U));
                }
                break;
            case 'X': break;
            case 'Z':
                mask = word;
                if (!mask) { consume = 0; break; }
                if (!(mask & (mask - 1U))) {
                    for (i = 0; !(mask & 1U); ++i) mask >>= 1;
                    text = d + player_name(r, i, 1, 1);
                } else {
                    count = 0;
                    for (i = 0; (int)i < stars_signed_word(stars_word(d, 0x78U)); ++i, mask >>= 1) {
                        if (!(mask & 1U)) continue;
                        if (count) out = append(d, out, (mask & 0xfffeU) ?
                            (const StarsU8 *)", " : d + stars_game_string(r, 0x259));
                        out = append(d, out, d + player_name(r, i, 1, 1)); ++count;
                    }
                }
                break;
            case 'i': sprintf(temporary, "%d", (int)stars_signed_word(word)); text = (StarsU8 *)temporary; break;
            case 'u': sprintf(temporary, (char *)d + 0xb6cU, (unsigned int)word); text = (StarsU8 *)temporary; break;
            case 'k': text = part_name(r, word, stars_word(parameters, (StarsU16)(at + 2U))); consume = 2; break;
            case 'g': text = d + thing_name(r, word); break;
            case 'n':
                consume = 2; source = stars_word(parameters, (StarsU16)(at + 2U));
                if (word == 65534U) text = d + thing_name(r, source);
                else if (word == 65535U) text = d + ((source & 0x8000U) ? fleet_name(r, source) : planet_name(r, source));
                else text = d + location_name(r, word, source);
                break;
            case 'o': text = d + ((word & 0x8000U) ? fleet_name(r, word) : planet_name(r, word)); break;
            case 'p': text = d + planet_name(r, word); break;
            case 's': text = d + fleet_name(r, (StarsU16)(word | 0x8000U)); break;
            case 'z':
                owner = (StarsU16)((word >> 5) | ((word & 0x8000U) ? 0xf800U : 0));
                text = design(r, owner, (StarsU16)(word & 31U)) + 8;
                if (owner != stars_word(d, 0x18cU)) {
                    sprintf(temporary, (char *)d + 0xb6fU, d + player_name(r, owner, 0, 1), text);
                    text = (StarsU8 *)temporary;
                }
                break;
            }
            break;
        default: d[out++] = ch; break;
        }
        if (text) out = append(d, out, text);
        at = (StarsU16)(at + consume * 2U); ++format;
    }
    d[out] = 0;
    return 0x535aU;
}
StarsU16 stars_format_id(StarsRuntime *r, StarsI16 id, const StarsU8 *parameters)
{ return stars_format_string(r, stars_game_string(r, id), parameters); }
