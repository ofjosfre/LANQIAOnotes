#include "reg52.h"
#include "DS1302.h"
#include "SMGdisplay.h"
#include "absacc.h"
 
unsigned char write_ds1302add[7]={0x80,0x82,0x84,0x86,0x88,0x8a,0x8c};
unsigned char read_ds1302add[7]={0x81,0x83,0x85,0x87,0x89,0x8b,0x8d};
//2000.04.18,sat,23:59:24  初始化时间定义
unsigned char timer[7]={0x24,0x59,0x23,0x18,0x04,0x06,0x20};

void ds1302_config()
{
	char i;
	Write_Ds1302(0x8e,0x00);  //======打开保护功能
	for(i=0;i<7;i++)
	{
		Write_Ds1302( write_ds1302add[i],timer[i] )  ;   
	}
	Write_Ds1302(0x8e,0x80);  //======恢复保护功能
}

void read_ds1302timer()
{
	char i;
	for(i=0;i<7;i++)
	{
	timer[i]=Read_Ds1302(read_ds1302add[i]);
	}
}

void display_ds1302()
{
	displaySMG(0,SMG[timer[2]/16]); //   BCD码，所以除16
	delaySMG(100);
  displaySMG(1,SMG[timer[2]%16]);
	delaySMG(100);

	displaySMG(2,0xbf);        //1011 1111
	delaySMG(100);
	
	displaySMG(3,SMG[timer[1]/16]);
	delaySMG(100);
  displaySMG(4,SMG[timer[1]%16]);
	delaySMG(100);

	displaySMG(5,0xbf);        //1011 1111
	delaySMG(100);
	
	displaySMG(6,SMG[timer[0]/16]);
	delaySMG(100);
  displaySMG(7,SMG[timer[0]%16]);
	delaySMG(100);
	
	shutallSMG(0xff) ;
}

void initial()
{
	XBYTE[0x8000]=0xff;
	delaySMG(60000);
	XBYTE[0xa000]=0x00;
}

void main()
{
	initial();
	ds1302_config();
	while(1)
	{
		read_ds1302timer();
		display_ds1302();
	}
}