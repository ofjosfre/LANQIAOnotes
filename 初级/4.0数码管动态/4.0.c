#include "reg52.h"

unsigned month=1;


sbit HC138_A=P2^5;
sbit HC138_B=P2^6;
sbit HC138_C=P2^7;

unsigned char code SMG[18]=
{0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,
0x90,0x88,0x80,0xc6,0xc0,0x86,0x8e,0xbf,0x7f};

void delay(unsigned int t)
{
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

void SMGdisplay(unsigned char value,unsigned char pos)
{
select(6);
P0=0x01<<pos;
select(7);
P0=value;
}



void SMGdisplayDynamic()
{
SMGdisplay(SMG[2],0);
delay(500);
SMGdisplay(SMG[0],1);
delay(500);
SMGdisplay(SMG[1],2);
delay(500);
SMGdisplay(SMG[8],3);
delay(500);
SMGdisplay(SMG[16],4);
delay(500);
SMGdisplay(SMG[16],5);
delay(500);
SMGdisplay(SMG[month/10],6);
delay(500);
SMGdisplay(SMG[month%10],7);
delay(500);


}

void delay1(unsigned char t)
{
while(t--)
{
SMGdisplayDynamic();
}

}

void main()
{
while(1)
  {
SMGdisplayDynamic();
month++;
if(month>12)
{
month=1;
}
delay1(100);
  }
}