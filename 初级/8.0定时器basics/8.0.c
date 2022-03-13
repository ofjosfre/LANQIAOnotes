#include "reg52.h"

sbit L1=P0^0;
sbit L8=P0^7;

void select()
{
	P2=(P2&0x1f)|0x80;
}

//=================================================
void init_timer0()
{
	TMOD=0x01;
	TH0=(65535-50000)/256;
	TL0=(65535-50000)%256;
	
	ET0=1;
	EA=1;
	TR0=1;
}

unsigned char count=0;


void service_timero() interrupt 1
{
	TH0=(65535-50000)/256;
	TL0=(65535-50000)%256;
	
	count++;
	if(count%10==0)
	{
		L1=~L1;
	}
	if(count==100)
	{
		L8=~L8;
		count=0;
	}
}

//============================================
void main ()
{
	select();
	init_timer0();
	while(1)
	{
		
	}
}