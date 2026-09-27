#include <stdio.h>
#include <string.h>
#include "core.h"
typedef struct Trace { StarsU16 reset, count, index, marker; StarsI16 value, quantity; char text[32]; } Trace;
static void format_entry(void *context, StarsU16 index, StarsU16 marker,
    StarsI16 value, StarsI16 quantity, StarsU8 *text, StarsU16 cap)
{ Trace*t=(Trace*)context;(void)cap;t->index=index;t->marker=marker;t->value=value;t->quantity=quantity;strcpy((char*)text,"formatted"); }
static void send_message(void *context, StarsU16 hwnd, StarsU16 message,
    const StarsU8 *text)
{ Trace*t=(Trace*)context;(void)hwnd;if(message==0x405U){++t->reset;return;}++t->count;strcpy(t->text,(const char*)text); }
int main(void){StarsFillFleetCompInput i;Trace t;memset(&i,0,sizeof i);memset(&t,0,sizeof t);i.hwnd_list=77U;i.components[0].quantity=3;i.components[0].packed_mass=(StarsU16)((2U<<7)|2U);i.components[1].quantity=0;i.components[2].quantity=5;i.components[2].packed_mass=(StarsU16)((2U<<7)|127U);i.format_entry=format_entry;i.send_message=send_message;i.context=&t;stars_fill_fleet_comp_lb(&i);if(t.reset!=1U||t.count!=2U||t.index!=2U||t.marker!='P'||t.value!=1||t.quantity!=5||strcmp(t.text,"formatted"))return 1;puts("FILL_FLEET_COMP_SOURCE=PASS");return 0;}
