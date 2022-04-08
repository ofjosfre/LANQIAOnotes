#include "reg52.h"
#include "iic.h"
#include "onewire.h"

unsigned char code SMG[14]={0xc0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0xc6,0x8c,0x88,0xff};
unsigned char code SMGdot[10]={0x40,0x79,
    0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10};

sfr P4=0xc0;

sbit HC138_C=P2^7;
sbit HC138_B=P2^6;
sbit HC138_A=P2^5;


sbit R3=P3^2;
sbit R4=P3^3;

sbit C2=P4^2;
sbit C1=P4^4;

sbit L4=P0^3;
sbit L3=P0^2;
sbit L2=P0^1;
sbit L1=P0^0;

void read_DS18B20_temp();
void displaytemp();

unsigned char state=0;
unsigned char mode=1;
unsigned char statek=0;
unsigned int temp=0;
unsigned int config=25;
unsigned int vol;             //µÁ—πœ‘ æ

void delaySMG (unsigned int t)
{
	while(t--);
}

void delay (unsigned int t)
{
	while(t--)
	{
		displaytemp();
	}
}

void select(unsigned char i)
{
	switch(i)
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


void SMGbit(unsigned char pos,unsigned char value)
{

	select(7);
	P0=0xff;

	select(6);
	P0=0x01<<pos;
	select(7);
	P0=value;

}

void shutall()
{
  select(7);
	P0=0xff;

	select(6);
	P0=0x0ff;
	select(7);
	P0=0xff;
}

void displaytemp()
{
	  SMGbit(0,SMG[10]);
		delaySMG(100);
		SMGbit(1,SMG[13]);
		delaySMG(100);
		SMGbit(2,SMG[13]);
		delaySMG(100);
		SMGbit(3,SMG[13]);
		delaySMG(100);
		
    SMGbit(4,SMG[temp/1000]);
		delaySMG(100);
		SMGbit(5,SMGdot[(temp%1000)/100]);
		delaySMG(100);
		SMGbit(6,SMG[(temp%100)/10]);
		delaySMG(100);
		SMGbit(7,SMG[temp%10]);
		delaySMG(100);
		
		shutall();
}

void displayconfig()
{
	  SMGbit(0,SMG[11]);
		delaySMG(200);
		SMGbit(1,SMG[13]);
		delaySMG(200);
		SMGbit(2,SMG[13]);
		delaySMG(200);
		SMGbit(3,SMG[13]);
		delaySMG(200);
		SMGbit(4,SMG[13]);
		delaySMG(200);
		SMGbit(5,SMG[13]);
		delaySMG(200);
		
		SMGbit(6,SMG[config/10]);
		delaySMG(200);
		SMGbit(7,SMG[config%10]);
		delaySMG(200);
		
		shutall();
		
}

void displayvol()
{
	  SMGbit(0,SMG[12]);
		delaySMG(200);
		SMGbit(1,SMG[13]);
		delaySMG(200);
		SMGbit(2,SMG[13]);
		delaySMG(200);
		SMGbit(3,SMG[13]);
		delaySMG(200);
		SMGbit(4,SMG[13]);
		delaySMG(200);
		
		SMGbit(5,SMGdot[vol/100]);
		delaySMG(200);	
    SMGbit(6,SMG[(vol%100)/10]);
		delaySMG(200);
		SMGbit(7,SMG[vol%10]);
		delaySMG(200);
		
		shutall();
}


void scan_keys()
{
	

  R4=0;
	R3=1;
	C1=C2=1;
	if(C1==0)              //  S4
	{
		delay(10);
		if(C1==0)
		{
			if(state==0)
			{
					state=1;
					statek=1;
			}
			else if (state==1)	
			{
					state=2;
					statek=0;
			}		
			else if(state==2)	
			{
				  state=0;
					statek=0;
			}
			  while(C1==0);
		}
		
	}
	
	else if(C2==0)                 //S8 -
	{
		delay(10);
		if(C2==0)
		{
			if(state==1)
			{
				if (statek==1)
				{
				config--;
				}
			}	
			while(C2==0);
		}
	}
	
  R3=0;
	R4=1;
	C1=C2=1;
	if(C1==0)           //S5
	{
		delay(10);
		if(C1==0)
		{
			switch(mode)
			{
				case 1:
					mode=2;
					break;
					
				case 2:
					mode=1;
					break;
			}
		  while(C1==0);
		}
	}
	
	else if(C2==0)             //s9 +
	{
		delay(10);
		if(C2==0)
		{
			if(state==1)
			{
				if (statek==1)
				{
				config++;
				}
			}	
				while(C2==0);
		}
	}
}

void displayall()
{
	select(5);
	P0=0x00;
	select(4);
	P0=0xff;

  if (state==0)
	{
			read_DS18B20_temp();
    L2=0;
		displaytemp();
	}
	
		else if (state==1)
	{
    L3=0;
		displayconfig();
	}
	
		else if (state==2)
	{
    L4=0;
		displayvol();
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
	
	if((temp&0xf800)==0x0000)
	{
		temp>>=4;
		temp=temp*100;
		temp=temp+(lsb&0x0f)*6.25;
	}
}

void init_sys()
{
	select(4);
	P0=0xff;
	select(5);
	P0=0x00;
	select(0);
	P0=0x00;
	
}

void write_pcf8591(unsigned char dat)
{
	IIC_Start();
	IIC_SendByte(0X90);
	IIC_WaitAck();
	IIC_SendByte(0X43);
	IIC_WaitAck();
	IIC_SendByte(dat);
	IIC_WaitAck();
	IIC_Stop();
}

void DA()
{
 if(mode==1)
 {
	 select(4);
	 L1=0;
  if(temp<config*100)
	{
	 vol=0;
	 write_pcf8591(0x00);
	}
	else 
	{
		vol=500;
		write_pcf8591(0xff);
	}
 }
 else if(mode==2)
 {
	 select(4);
	  L1=1;
  if(temp<=2000)
	{
		vol=100;
	 write_pcf8591(51);
	}
	else if(2000<temp<=4000)
	{
		vol=temp*0.15-200;
		write_pcf8591((temp-2000)*0.08+51.0);
	}
	else
	{
		vol=400;
	 write_pcf8591(205);
	}
 }
}	
void main()
{
	init_sys();
	while(1)
	{
	scan_keys();
	DA();
  displayall();
	}
}
