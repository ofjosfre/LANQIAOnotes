#include "reg52.h"

sbit HC138_A=P2^5;
sbit HC138_B=P2^6;
sbit HC138_C=P2^7;

void select(unsigned char m)
{
	switch (m)
	{
	case 4:
  HC138_C=1;
  HC138_B=0;
  HC138_A=0;
  break;
  case 5:
  HC138_C=1;
  HC138_B=0;
  HC138_A=1;
  break;
  case 6:
  HC138_C=1;
  HC138_B=1;
  HC138_A=0;
  break;
  case 7:
  HC138_C=1;
  HC138_B=1;
  HC138_A=1;
  break;
	}	
}


void delay(unsigned int t)
{
	while(t--);
	while(t--);
}

void ledrunning()
{
	select(4);
	P0=0xf0;
	delay(60000);
	delay(60000);
	P0=0x0f;
	delay(60000);
	delay(60000);	
	P0=0xff;
	delay(60000);
	delay(60000);	
}

void SMGrunning()
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		select(6);
		P0=0x01<<i;
		select(7);
		P0=0x00;
	  delay(60000);
	  delay(60000);	
	}
	P0=0xff;
	delay(60000);
	delay(60000);	
}


void main ()
{
	while(1)
	{
		ledrunning();
		SMGrunning();
	}
}