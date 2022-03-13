#include "reg52.h"

sbit L1=P0^0;
sbit L8=P0^7;

void delay(unsigned int t)
{
 while(t--);
 while(t--);
}
void select()
{
	P2= (P2&0x1f)|0x80;
}

void working()
{
	select();
	L1=0;
	delay(60000);
	L1=1;
	delay(60000);
}

//=================================
void init_INT0()
{
	IT0=1;
	EX0=1;
	EA=1;
}

unsigned char state_int=0;

void serviceINT0() interrupt 0
{
  state_int=1;
}

void ledINT()
{
	if(state_int==1)
	{
	L8=0;
	delay(60000);
	delay(60000);
	delay(60000);
	delay(60000);
	delay(60000);
	delay(60000);
	L8=1;
	}
	state_int=0;
}
//=================================
void main ()
{
	init_INT0();
	while(1)
	{
		working();
		ledINT();
	}
}