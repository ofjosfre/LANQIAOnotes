#include "absacc.h"


void delaySMG(unsigned int t)
{
	while(t--);
}


void displaySMG(unsigned char pos,unsigned char value)
{
	XBYTE[0xE000]=0xff;                 //ÏûÓ°
	XBYTE[0xc000]=0x01<<pos;
	XBYTE[0xE000]=value;
}

void shutallSMG(unsigned char value) 
{
  XBYTE[0xc000]=0xff;
	XBYTE[0xE000]=value;	
}