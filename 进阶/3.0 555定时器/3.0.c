#include "reg52.h"
#include "absacc.h"
#include "SMGdisplay.h"

unsigned int count_f=0;
unsigned int dat_f=0;
unsigned char count_t=0;

void init_timer()
{
	//定时器0作计数，8为重装
	TH0=0xff;
	TL0=0xff;
	
	//定时器1作计时，50ms
	TH1=(65535-50000+1)/256;
	TL1=(65535-50000+1)%256;
	
	TMOD=0x16; //0001 0110

  ET0=1;
	ET1=1;
	EA=1;
	
	TR0=1;
	TR1=1;
}

void service_timer0() interrupt 1
{
	count_f++;

}

void service_timer1() interrupt 3
{
	TH1=(65535-50000+1)/256;
	TL1=(65535-50000+1)%256;
	
	count_t++;
	if(count_t==20)
	{
		dat_f=count_f;
		count_f=0;
		count_t=20;
	}
}

void display()
{
	displaySMG(0,0x8e);
	delaySMG(100);
	
	displaySMG(1,0xff);
	delaySMG(100);
	
	displaySMG(2,0xff);
	delaySMG(100);
	
	if(dat_f>9999)
	{
		displaySMG(3,SMG[dat_f/10000]);
	  delaySMG(100);
	}
	
	if(dat_f>999)
	{
		displaySMG(4,SMG[(dat_f/1000)%10]);
	  delaySMG(100);
	}
	
	if(dat_f>99)
	{
		displaySMG(5,SMG[(dat_f/100)%10]);
	  delaySMG(100);
		
		shutallSMG(0xff); 
	}
	
	if(dat_f>9)
	{
		displaySMG(6,SMG[(dat_f/10)%10]);
	  delaySMG(100);
	}
		displaySMG(7,SMG[dat_f%10]);
	  delaySMG(100);
}

void main()
{
	XBYTE[0X8000]=0xff;
	XBYTE[0xa000]=0x00;
	init_timer();
	while(1)
	{
		display();
	}
}