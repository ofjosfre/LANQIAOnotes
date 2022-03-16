#include "reg52.h"

sfr AUXR=0x8e;

sbit HC138_A=P2^5;
sbit HC138_B=P2^6;
sbit HC138_C=P2^7;

sbit L7=P0^6;
sbit L8=P0^7;

sbit S4=P3^3;
sbit S5=P3^2;

unsigned char urdata;
unsigned int command;
unsigned int stateled;

unsigned char code SMG[18]=
{0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,
0x90,0x88,0x80,0xc6,0xc0,0x86,0x8e,0xbf,0x7f};     ///0-9,a-f,  16:-,17:dp

void delay(unsigned int t)
{
while(t--);
while(t--);
}

void delay1(unsigned int t)
{
while(t--);
}


void select(unsigned char n)
{
	switch (n)
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
	case 0:
  HC138_C=0;
  HC138_B=0;
  HC138_A=0;
  break;
	}	
}

unsigned int milsec=0;
unsigned int sec=0;
unsigned int min=0;
unsigned int hour=0;
//========================================== system initialization=========
void initial()
{
	select(5);
	P0=0x00;
	select(4);
	P0=stateled;
	select(0);
}
//==========================================================================
//=========================================LED,SMG check
void check_LED()
{
unsigned char i;
	select(4);
for(i=1;i<=8;i++)
	{
		stateled=0xff<<i;
		P0=stateled;
    delay(80000);
    delay(80000);
	}
for(i=1;i<=8;i++)
	{
   stateled=~(0xff<<i);
	 P0=stateled;
   delay(80000);
   delay(80000);
	}
}

void check_SMG()
{
	unsigned char pos;
		select(7);
		P0=0x00;
	for(pos=0;pos<8;pos++)
	{
	select(6);
	P0=~(0xfe<<pos);
	delay(60000);
	delay(80000);
	delay(60000);
	}
		for(pos=0;pos<8;pos++)
	{
	select(6);
	P0=0xfe<<pos;
	delay(60000);
	delay(80000);
	delay(60000);
	}
}
//=========================================================
//==================================SMG display
void display_SMG(unsigned char pos,unsigned char value)
{
	select(6);
	P0=0x01<<pos;
	select(7);
	P0=value;
}
//===========================================================
//======================================time display
void display_time()
{
	display_SMG(0,SMG[hour/10]);
	delay1(500);
	display_SMG(1,SMG[hour%10]);
	delay1(500);
	display_SMG(2,SMG[16]);
	delay1(500);
	display_SMG(3,SMG[min/10]);
	delay1(500);
	display_SMG(4,SMG[min%10]);
	delay1(500);
	display_SMG(5,SMG[16]);
	delay1(500);
	display_SMG(6,SMG[sec/10]);
	delay1(500);
	display_SMG(7,SMG[sec%10]);
	delay1(500);
}
//==================================================
//=========================timer initialize/service function===========
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
		  if(min==60)
		  {
			hour++;
			min=0;
			 if(hour==99)
		   {
			 hour=0;
			 }
		  }
	  }
	}
}
//============================================
//========================output uart==========
void init_uart()
{
	TMOD=0x20;
	TH1=0xfd;
	TL1=0xfd;
	TR1=1;
	
	SCON=0x50;
	AUXR=0x00;
	
	ES=1;
	EA=1;
}

void sendbyte(unsigned char dat)
{
	SBUF=dat;
	while(TI==0);
	TI=0;
}

void serviceuart() interrupt 4
{
	if(RI==1)
	{
		command=SBUF;
		RI=0;
	}
}
//===================================================================
//=====================command赋值，来自上位机数据
void executecommand()
{
	if(command!=0x00)
	{
	 switch(command&0xf0)
	 {
		 case 0xa0:
			 select(4);
			 stateled=(stateled|0x0f)&(~command|0xf0);
			 P0=stateled;
			 select(0);
			 command=0x00;
			 break;
			 
		 case 0xb0:
		   sendbyte((hour/10<<4)|(hour%10));
		   sendbyte((min/10<<4)|(min%10));
		   sendbyte((sec/10<<4)|(sec%10));		
			 command=0x00;
			 break;			 
	 }
	}
}
//==========================================================================
//================================scan keys===================================
void scankeys()
{
	if(S5==0)
	{
		display_time();
	  if(S5==0)
		{
			while(S5==0)
			{
				display_time();
			}
		select(4);
		stateled=(stateled|0x40)&(~stateled|0xbf);
		P0=stateled;
		select(0);
		}
	}
	if(S4==0)
	{
		display_time();
	  if(S4==0)
		{
			while(S4==0)
			{
				display_time();
			}
		select(4);
		stateled=(stateled|0x80)&(~stateled|0x7f);
		P0=stateled;
		select(0);
		}
	}
}

void main ()
{
	initial();
	check_LED();
	delay(60000);
	check_SMG();
	delay(60000);
	init_timer();
	init_uart();
	while(1)
	{
		executecommand();
		display_time();
		scankeys();
		
	}
}