#include "reg52.h"

sbit HC138_A=P2^5;
sbit HC138_B=P2^6;
sbit HC138_C=P2^7;

sbit S4=P3^3;
sbit S5=P3^2;

unsigned char min=0;
unsigned char sec=0;
unsigned char milsec=0;

unsigned char code SMG[18]=
{0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,
0x90,0x88,0x80,0xc6,0xc0,0x86,0x8e,0xbf,0x7f};

void select(unsigned char n)
{
	switch(n)
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
}

void display_SMG(unsigned char value,unsigned char pos)
{
	select(6);
	P0=0x01<<pos;
	select (7);
	P0=value;
}

void diaplay_time()
{
	display_SMG(SMG[milsec%10],7);
	delay(500);
	display_SMG(SMG[milsec/10],6);
	delay(500);
	display_SMG(SMG[16],5);
	delay(500);
	
	display_SMG(SMG[sec%10],4);
	delay(500);
	display_SMG(SMG[sec/10],3);
	delay(500);
	display_SMG(SMG[16],2);
	delay(500);
	
	display_SMG(SMG[min%10],1);
	delay(500);
	display_SMG(SMG[min/10],0);
	delay(500);
}
//================================ timer
void init_timer()
{
	TMOD=0x01;
	TH0=(65535-50000)/256;
	TL0=(65535-50000)%256;
	
	ET0=1;
	EA=1;
	TR0=1;
}

void service_timer() interrupt 1
{
	TH0=(65535-50000)/256;
	TL0=(65535-50000)%256;
	
	milsec++;
	if(milsec==20)
	{
		sec++;
		milsec=0;
		if(sec==60)
		{
			min++;
			sec=0;
		}
		if(min==99)
		{
			min=0;
		}
	}
}
//================================

void scan_keys()
{
	if(S4==0)          //   pause
	{
		delay(100);
		if(S4==0)
		{
			TR0=~TR0;
			while(S4==0)
			{
				diaplay_time();
			}
		}
	}
	if(S5==0)                 // clear
	{
		delay(100);
		if(S5==0)
		{
				min=0;
				sec=0;
				milsec=0;
			while(S5==0)     //=======take caution of line138 & line148
			{
				diaplay_time();
			}
		}
	}
}
void main()
{
	init_timer();
	while(1)
	{
		diaplay_time();
		scan_keys();
	}
}