#include "reg52.h"
#include "absacc.h"



void delay(unsigned int t)
{
	while(t--);
	while(t--);
}

void ledrunning()
{
  XBYTE[0x8000]=0xf0;
	delay(60000);
	delay(60000);
  XBYTE[0x8000]=0x0f;
	delay(60000);
	delay(60000);	
  XBYTE[0x8000]=0xff;
	delay(60000);
	delay(60000);	
}

void SMGrunning()
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
    XBYTE[0xc000]=0x01<<i;
    XBYTE[0xe000]=0x00;
	  delay(60000);
	  delay(60000);	
	}
  XBYTE[0xe000]=0xff;
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
