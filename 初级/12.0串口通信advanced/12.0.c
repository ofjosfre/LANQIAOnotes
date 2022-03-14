#include "reg52.h"

sfr AUXR=0x8e;

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

void init_system()
{
	select(5);
	P0=0x00;
	select(4);
	P0=0xff;
}

//=============================================================
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

unsigned char command=0x00;
void service_uart() interrupt 4
{
	if(RI==1)
	{
		command=SBUF;
		RI=0;
	}
}

void send_byte(unsigned char dat)          // ×Ö½Ú·¢ËÍ
{
	SBUF=dat;
	while(TI==0);
	TI=0;
}

void send_string(unsigned char *str)
{
	while(*str!='\0')
	{
	send_byte(*str++);
	}
}
//==============================================================
void working()
{
	if(command!=0x00)
	{
		switch(command&0xf0)
		{
			case 0xa0:
      P0=(P0|0x0f)&(~command|0xf0);
			command=0x00;	
			break;
			
			case 0xb0:
			 P0=(P0|0xf0)&(~command << 4|0x0f);
			command=0x00;
			break;
		
   		case 0xc0:
			send_string("The System is running!\r\n");
			command=0x00;
			break;
		}
	}
}

void main ()
{
	init_system();
	init_uart();
	send_string("Welcome to use!\r\n");
	while(1)
	{
		working();
	}
}