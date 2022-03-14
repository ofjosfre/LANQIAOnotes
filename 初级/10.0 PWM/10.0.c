#include "reg52.h"

sbit L1=P0^0;
sbit S7=P3^0;

void select()
{
	P2=(P2&0x1f)|0x80;
}

//=======================================timer related functions
unsigned char count=0;
unsigned char pwm_duty=0;

void int_timer()
{
	TMOD=0x01;
	TH0=(65535-100)/256;
	TL0=(65535-100)%256;
	
	ET0=1;
	EA=1;
	
}

void service_timer() interrupt 1
{
	TH0=(65535-100)/256;
	TL0=(65535-100)%256;
	count++;
	if(count<=pwm_duty)   ////here can also use code: if(count==pwm_duty)   
	{                                             ////{
		L1=0;                                       ////L1=1;
	}                                             ////}
	else if(count<100)                            ////else if(count==100)
	{                                             ////{
		L1=1;                                       ////L1=0;
	}                                             ////count=0;
	else if(count==100)                           ////}
	{
		L1=0;
		count=0;
	}
}
//=======================================keys related functions
unsigned char mode=0;

void delay(unsigned int t)
{
	while(t--);
}

void scankeys()
{
	if(S7==0)
	{
		delay(100);
    if(S7==0)
		{
			switch(mode)
			{
				case 0:
					L1=0;
					TR0=1;
					pwm_duty=10;
					mode=1;
					break;
					
				case 1:
					pwm_duty=50;
					mode=2;
					break;
					
				case 2:
					pwm_duty=90;
					mode=3;
					break;
					
				case 3:
					L1=1;
					TR0=0;
					mode=0;
					break;
			}
			while(S7==0);
		}
	}
}
void main()
{
	select();
	L1=1;
	int_timer();
	while(1)
	{
		scankeys();
	}
}