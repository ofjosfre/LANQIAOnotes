#include "reg52.h"

sfr AUXR=0x8e;

unsigned char urdata;
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
		RI=0;
		urdata=SBUF;
		sendbyte(urdata+1);
	}
}

void main()
{
	  init_uart();
		sendbyte(0x5a);
		sendbyte(0xa5);
		while(1);
}