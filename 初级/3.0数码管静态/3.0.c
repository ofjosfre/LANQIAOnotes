#include "reg52.h"

sbit HC138_A=P2^5;
sbit HC138_B=P2^6;
sbit HC138_C=P2^7;

unsigned char code SMG[18]=
{0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,
0x90,0x88,0x80,0xc6,0xc0,0x86,0x8e,0xbf,0x7f};

void delay(unsigned int t)
{
while(t--);
while(t--);
}


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

void SMGdisplay(unsigned char dat,unsigned char pos)
{
select(6);
P0=0x01<<pos;
select(7);
P0=dat;

}

void SMGrec()
{
	unsigned char i,j;
   for(i=0;i<9;i++)
	 {
		 for(j=0;j<10;j++)
			{
		SMGdisplay(SMG[j],i);
			delay(80000);
	 delay(80000);
	     }
	 }
for(i=0;i<19;i++)
      {
           select(6);
           P0=0xff;
           select(7);
           P0=SMG[i];             
delay(80000);
delay(80000);
       }
}



void main()
{
while(1)
		{	 

SMGrec();
		}
}
