#include "reg52.h"
#include "absacc.h"
#include "onewire.h"

unsigned char SMG[10]=
{0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90};

unsigned char SMGdot[10]=
{0x40,0x79,0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10};

unsigned int temp=0;

void delaySMG(unsigned int t)
{
	while(t--);
}

void displaySMG(unsigned char pos,unsigned char value)
{
	XBYTE[0xE000]=0xff;             //消影
	XBYTE[0xc000]=0x01<<pos;
	XBYTE[0xE000]=value;
}

void shutallSMG(unsigned char value) 
{
  XBYTE[0xc000]=0xff;
	XBYTE[0xE000]=value;	
}
//567   
//567/100=5.67=5
//567%100=67  67/10=6.7=6
//567%10=7

void displayTemp()
{
	displaySMG(7,SMG[temp%10]);
	delaySMG(100);
	displaySMG(6,SMGdot[(temp%100)/10]);
	delaySMG(100);
	displaySMG(5,SMG[temp/100]);
	delaySMG(100);
	                                 // 消除重影
	displaySMG(4,0xff);
	delaySMG(100);
	displaySMG(3,0xff);
	delaySMG(100);
	displaySMG(2,0xff);
	delaySMG(100);
	displaySMG(1,0xff);
	delaySMG(100);
	displaySMG(0,0xff);
	delaySMG(100);
	
	shutallSMG(0xff);            //     防止最后一个数码管过亮
}

void delay(unsigned int t)
{
	while(t--)
	{
		displayTemp();
	}
}
void read_DS18B20_temp()
{
	unsigned char lsb;
	unsigned char msb;	
	init_ds18b20();
	Write_DS18B20(0xcc);
	Write_DS18B20(0x44);
	delay(1000);
	
	init_ds18b20();
	Write_DS18B20(0xcc);
	Write_DS18B20(0xbe);
	lsb=Read_DS18B20();
	msb=Read_DS18B20();
	
	temp=msb;
	temp=(temp<<8)|lsb;
  ///////////////////////////////////若不要小数temp>>=4,下面一段不要
	
	if((temp&0xf800)==0x0000)
	{
		temp>>=4;
		temp=temp*10;
		temp=temp+(lsb&0x0f)*0.625;
	}
}
void main()
{
	XBYTE[0x8000]=0xff;
	while(1)
	{
		read_DS18B20_temp();
		displayTemp();
	}
}