/* Genuine C reconstructions of the compact message routines in msg.obj. */
#include "core.h"
#include <string.h>
static StarsU16 msg_filtered(const StarsMessageState *s, StarsI16 id)
{ return id>=0 && id<2048 ? (StarsU16)((s->filtered[(StarsU16)id>>3]>>((StarsU16)id&7U))&1U) : 0U; }
static StarsU16 msg_word(const StarsU8 *p)
{ return (StarsU16)(p[0] | ((StarsU16)p[1] << 8)); }
/* Genuine C reconstruction of FGetNMsgbig (NE 7:8444).  The message queue
 * contains XGen records: message/mask word, goto word, then each parameter
 * as one or two bytes according to the mask. */
StarsI16 stars_get_message_big(const StarsU8 *data, StarsU16 data_bytes,
    StarsU16 message_count, StarsI16 index, const StarsMessageOps *o, void *c,
    StarsMessageBig *out)
{
    StarsU16 offset, mask, count, i;
    StarsI16 message;
    if (!data || !out || index < 0 || message_count <= (StarsU16)index)
        return 0;
    offset = 0U;
    for (;;) {
        if (offset > data_bytes || (StarsU16)(data_bytes - offset) < 4U)
            return 0;
        message = (StarsI16)(msg_word(data + offset) & 0x01ffU);
        mask = (StarsU16)(msg_word(data + offset) >> 9);
        count = o && o->parameter_count ?
            o->parameter_count(c, message) : 0U;
        if (count > 7U) count = 7U;
        if (index == 0) {
            out->message = message;
            out->goto_id = (StarsI16)msg_word(data + offset + 2U);
        }
        offset = (StarsU16)(offset + 4U);
        for (i = 0U; i < count; ++i) {
            StarsU16 width = (mask & 1U) != 0U ? 2U : 1U;
            if (offset > data_bytes || (StarsU16)(data_bytes - offset) < width)
                return 0;
            if (index == 0) {
                out->parameters[i] = width == 2U ?
                    (StarsI16)msg_word(data + offset) :
                    (StarsI16)(StarsU8)data[offset];
            }
            offset = (StarsU16)(offset + width);
            mask = (StarsU16)(mask >> 1);
        }
        if (index == 0) return 1;
        --index;
        if (offset >= data_bytes && index != 0) return 0;
    }
}
static StarsU16 msg_record_length(const StarsU8 *data, StarsU16 offset)
{ return (StarsU16)(((StarsU16)data[offset] >> 4) + 5U); }
static StarsI16 msg_record_matches(const StarsU8 *data, StarsU16 offset,
    StarsI16 player, StarsI16 message, StarsI16 object)
{
    return (StarsI16)(((StarsI16)(data[offset] & 0x0fU) == player) &&
        ((StarsI16)(msg_word(data + offset + 1U) & 0x01ffU) == message) &&
        ((StarsI16)msg_word(data + offset + 3U) == object));
}
/* Genuine C reconstructions of FRemovePlayerMessage and FFindPlayerMessage. */
StarsI16 stars_remove_player_message(StarsU8 *data, StarsU16 data_bytes,
    StarsI16 player, StarsI16 message, StarsI16 object)
{
    StarsU16 offset, length;
    StarsI16 found;
    if (!data) return 0;
    offset = 0U; found = 0;
    while (offset < data_bytes) {
        length = msg_record_length(data, offset);
        if (length < 5U || (StarsU16)(data_bytes - offset) < length) break;
        if (msg_record_matches(data, offset, player, message, object)) {
            StarsU16 word = msg_word(data + offset + 1U);
            data[offset + 1U] = (StarsU8)((word & 0xfe00U) | 0x01ffU);
            data[offset + 2U] = (StarsU8)(word >> 8);
            ++found;
        }
        offset = (StarsU16)(offset + length);
    }
    return found;
}
StarsI16 stars_find_player_message(const StarsU8 *data, StarsU16 data_bytes,
    StarsI16 player, StarsI16 message, StarsI16 object)
{
    StarsU16 offset, length;
    if (!data) return 0;
    offset = 0U;
    while (offset < data_bytes) {
        length = msg_record_length(data, offset);
        if (length < 5U || (StarsU16)(data_bytes - offset) < length) break;
        if (msg_record_matches(data, offset, player, message, object)) return 1;
        offset = (StarsU16)(offset + length);
    }
    return 0;
}
/* Genuine C reconstruction of ResetMessages (NE 7:98D6). */
void stars_reset_messages(StarsMessageResetState *s)
{
    if (!s) return;
    s->message_memory = 0U;
    s->current_message = -1;
    s->message_count = 0U;
    s->send_current = 0;
    memset(s->sent, 0, sizeof s->sent);
    memset(s->filtered, 0, sizeof s->filtered);
    s->incoming_list = 0;
    s->incoming_segment = 0U;
    s->outgoing_list = 0;
    s->outgoing_segment = 0U;
    s->incoming_count = 0U;
    s->outgoing_count = 0U;
}
/* Genuine C reconstruction of MarkPlayersThatSentMsgs (NE 7:9604). */
void stars_mark_players_that_sent_messages(StarsI16 player,
    const StarsPlayerMessage *records, StarsU16 record_count,
    StarsU16 *player_det, StarsU16 player_count)
{
    StarsU16 i;
    StarsI16 from, to;
    if (player == -1 || !records || !player_det) return;
    for (i = 0U; i < record_count; ++i) {
        from = records[i].from;
        to = records[i].to;
        if (from < 0 || (StarsU16)from >= player_count) continue;
        if ((to == 0 && from != player) ||
            (to - 1 == player && (player_det[(StarsU16)from] & 0x0100U) == 0U)) {
            player_det[(StarsU16)from] = (StarsU16)(
                (player_det[(StarsU16)from] & 0xfeffU) | 0x0100U);
            player_det[(StarsU16)from] = (StarsU16)(
                (player_det[(StarsU16)from] & 0xfff8U) | 3U);
        }
    }
}
/* Genuine C reconstruction of WritePlayerMessages (NE 7:9702).  The two
 * write kinds retain the original protocol: 0x0c carries selected queued
 * message records without their player/length byte, while 0x28 carries an
 * outgoing player-message entry and its twelve-byte header. */
void stars_write_player_messages(const StarsU8 *messages, StarsU16 message_bytes,
    StarsI16 player, const StarsOutgoingPlayerMessage *outgoing,
    StarsU16 outgoing_count, StarsWriteMessageRecord write_record, void *c)
{
    StarsU8 chunk[1024];
    StarsU16 offset, chunk_bytes, length, copied;
    StarsU16 i;
    StarsI16 absolute_length;
    if (player == -1 || !messages || !write_record) return;
    chunk_bytes = 0U; offset = 0U;
    while (offset < message_bytes) {
        length = msg_record_length(messages, offset);
        if (length < 5U || (StarsU16)(message_bytes - offset) < length) break;
        if ((StarsU16)(chunk_bytes + 0x14U) > 0x03ffU) {
            write_record(c, 0x000cU, chunk_bytes, chunk);
            chunk_bytes = 0U;
        }
        if ((StarsI16)(messages[offset] & 0x0fU) == player &&
            (StarsI16)(msg_word(messages + offset + 1U) & 0x01ffU) != 0x01ff) {
            copied = (StarsU16)(((StarsU16)messages[offset] >> 4) + 4U);
            if (copied > length - 1U) copied = (StarsU16)(length - 1U);
            memcpy(chunk + chunk_bytes, messages + offset + 1U, copied);
            chunk_bytes = (StarsU16)(chunk_bytes + copied);
        }
        offset = (StarsU16)(offset + length);
    }
    if (chunk_bytes != 0U) write_record(c, 0x000cU, chunk_bytes, chunk);
    for (i = 0U; i < outgoing_count; ++i) {
        if ((outgoing[i].to == 0 && outgoing[i].from != player) ||
            (outgoing[i].to - 1 == player)) {
            absolute_length = outgoing[i].compressed_length;
            if (absolute_length < 0) absolute_length = (StarsI16)-absolute_length;
            write_record(c, 0x0028U,
                (StarsU16)(absolute_length + 0x000cU), &outgoing[i]);
        }
    }
}
/* Genuine C reconstruction of MarkPlanetsPlayerLost (NE 7:93C6). */
void stars_mark_planets_player_lost(const StarsU8 *messages,
    StarsU16 message_bytes, StarsI16 player, const StarsPlanetLossOps *o,
    void *c)
{
    StarsU16 offset, length, word, mask, planet_id, parameter_offset,
        parameter_width;
    StarsI16 message;
    void *planet;
    if (!messages || !o || !o->lookup || !o->mark) return;
    offset = 0U;
    while (offset < message_bytes) {
        length = msg_record_length(messages, offset);
        if (length < 5U || (StarsU16)(message_bytes - offset) < length) break;
        if ((StarsI16)(messages[offset] & 0x0fU) == player) {
            word = msg_word(messages + offset + 1U);
            message = (StarsI16)(word & 0x01ffU);
            if (message == 0x008f) {
                mask = (StarsU16)(word >> 9);
                parameter_offset = (StarsU16)(offset + 6U +
                    ((mask & 1U) != 0U));
                parameter_width = (mask & 2U) != 0U ? 2U : 1U;
                if (offset > message_bytes ||
                    (StarsU16)(message_bytes - offset) <
                    (StarsU16)(6U + ((mask & 1U) != 0U) + parameter_width))
                    break;
                planet_id = parameter_width == 2U ?
                    msg_word(messages + parameter_offset) :
                    (StarsU16)messages[parameter_offset];
            } else if (message == 7 || message == 0x23 || message == 0x40) {
                planet_id = msg_word(messages + offset + 3U);
            } else {
                offset = (StarsU16)(offset + length);
                continue;
            }
            planet = o->lookup(c, planet_id);
            if (planet) o->mark(c, planet, player, 3);
        }
        offset = (StarsU16)(offset + length);
    }
}
StarsI16 stars_message_next(const StarsMessageState *s, StarsI16 filtered_only,
    const StarsMessageOps *o, void *c)
{
    StarsI16 n=s->current_message; StarsI16 id;
    if(s->view_filtered==0U || filtered_only!=0) {
        for(;;) { ++n; if(s->message_count<=(StarsU16)n) return n<(StarsI16)(s->message_count+s->player_message_count)?n:-1;
            id=o->message_id?o->message_id(c,n):n; if((StarsI16)msg_filtered(s,id)==filtered_only) break; }
    } else if(s->current_message<(StarsI16)s->message_count+(StarsI16)s->player_message_count-1) ++n;
    else n=-1;
    return n;
}
StarsI16 stars_message_previous(const StarsMessageState *s, StarsI16 filtered_only,
    const StarsMessageOps *o, void *c)
{
    StarsI16 n=s->current_message; StarsI16 id;
    if(s->view_filtered==0U || filtered_only!=0) {
        if(s->message_count<(StarsU16)s->current_message) --n;
        else for(;;) { --n; if(n<0) return -1; id=o->message_id?o->message_id(c,n):n; if((StarsI16)msg_filtered(s,id)==filtered_only) break; }
    } else if(s->current_message<1) n=-1; else --n;
    return n;
}
StarsI16 stars_package_player_message(const StarsMessageState *s, StarsU8 *out,
    StarsI16 player, StarsI16 message, StarsI16 object, const StarsI16 *params,
    const StarsMessageOps *o, void *c)
{
    StarsU16 flags, i, count, high=0; StarsU8 *p=out+5; StarsI16 v;
    if(player==-1)return 0;
    flags=o->player_flags?o->player_flags(c,player):0U;
    if(((flags&0x0200U)!=0U && (flags>>13)!=7U) && message!=0x8f && message!=7 && message!=0x23 && message!=0x40)return 0;
    if((StarsU16)(s->message_memory+0x14U)>=0xffc9U)return -1;
    out[0]=(StarsU8)(player&15); out[1]=(StarsU8)(message&0xff); out[2]=(StarsU8)((message>>8)&1); out[3]=(StarsU8)object; out[4]=(StarsU8)(object>>8);
    count=o->parameter_count?o->parameter_count(c,message):0U;
    for(i=0;i<count && i<7U;++i) { v=params?params[i]:0; if((v&0xff00)!=0){high=(StarsU16)(high|(1U<<(9+i)));*p++=(StarsU8)v;*p++=(StarsU8)(v>>8);} else *p++=(StarsU8)v; }
    out[0]=(StarsU8)((out[0]&15U)|((StarsU16)(p-(out+5))<<4)); out[1]=(StarsU8)((out[1]&0xffU)|(StarsU8)high); out[2]=(StarsU8)((out[2]&1U)|(StarsU8)(high>>8));
    return (StarsI16)(p-out);
}
StarsI16 stars_send_player_message(StarsMessageState *s, StarsI16 player, StarsI16 message,
    StarsI16 object, const StarsI16 *params, const StarsMessageOps *o, void *c)
{
    StarsU8 packet[42]; StarsI16 n=stars_package_player_message(s,packet,player,message,object,params,o,c);
    if(n<1)return (StarsI16)(n==0); if(o->append_message)o->append_message(c,packet,(StarsU16)n); s->message_memory=(StarsU16)(s->message_memory+n);++s->message_count;return 1;
}
StarsI16 stars_send_player_message2(StarsMessageState *s, StarsI16 player, StarsI16 message,
    StarsI16 object, StarsI16 p1, StarsI16 p2, const StarsMessageOps *o, void *c)
{ StarsI16 p[7];p[0]=p1;p[1]=p2;p[2]=p[3]=p[4]=p[5]=p[6]=0;return stars_send_player_message(s,player,message,object,p,o,c); }
StarsI16 stars_send_player_message_prepend(StarsMessageState *s, StarsI16 player, StarsI16 message,
    StarsI16 object, const StarsI16 *params, const StarsMessageOps *o, void *c)
{
    StarsU8 packet[42]; StarsI16 n=stars_package_player_message(s,packet,player,message,object,params,o,c);
    if(n<1)return (StarsI16)(n==0); if(o->prepend_message)o->prepend_message(c,packet,(StarsU16)n); s->message_memory=(StarsU16)(s->message_memory+n);++s->message_count;return 1;
}
StarsI16 stars_send_player_message2_xgen(StarsMessageState *s, StarsI16 prepend, StarsI16 message,
    StarsI16 object, StarsI16 p1, StarsI16 p2, const StarsMessageOps *o, void *c)
{
    StarsU8 packet[42]; StarsI16 params[2]; StarsU16 i,count,high=0; StarsU8 *p=packet+4; StarsI16 v;
    params[0]=p1;params[1]=p2;
    if((StarsU16)(s->message_memory+0x14U)>=0xffc9U)return 0;
    s->sent[(StarsU16)message>>3]=(StarsU8)(s->sent[(StarsU16)message>>3]|(StarsU8)(1U<<((StarsU16)message&7U)));
    packet[0]=(StarsU8)(message&0xff);packet[1]=(StarsU8)((message>>8)&1);packet[2]=(StarsU8)object;packet[3]=(StarsU8)(object>>8);
    count=o->parameter_count?o->parameter_count(c,message):0U;
    for(i=0;i<count&&i<2U;++i){v=params[i];if((v&0xff00)!=0){high=(StarsU16)(high|(1U<<(9+i)));*p++=(StarsU8)v;*p++=(StarsU8)(v>>8);}else *p++=(StarsU8)v;}
    packet[0]=(StarsU8)((packet[0]&0xffU)|(StarsU8)high);packet[1]=(StarsU8)((packet[1]&1U)|(StarsU8)(high>>8));
    if(prepend!=0){if(o->prepend_message)o->prepend_message(c,packet,(StarsU16)(p-packet));}else if(o->append_message)o->append_message(c,packet,(StarsU16)(p-packet));
    s->message_memory=(StarsU16)(s->message_memory+(p-packet));++s->message_count;s->current_message=-1;s->current_message=stars_message_next(s,0,o,c);return 1;
}
