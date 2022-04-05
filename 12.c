#include "reg52.h"
#include "iic.h"
#include "onewire.h"

unsigned char code SMG[15]={0xc0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0xc6,0x8c,0x88,0xff};
unsigned char code SMGdot[10]={0x40,0x79,
    0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10};

sfr P4=0xc0;

sbit HC138_C=P2^7;
sbit HC138_B=P2^6;
sbit HC138_A=P2^5;

sbit R1=P3^0;
sbit R2=P3^1;
sbit R3=P3^2;
sbit R4=P3^3;

sbit C4=P3^4;
sbit C3=P3^5;
sbit C2=P4^2;
sbit C1=P4^4;

sbit L4=P0^3;
sbit L3=P0^2;
sbit L2=P0^1;
sbit L1=P0^0;

void displaySMG();

unsigned char state=0;
unsigned char mode=1;
unsigned char statek=0;
unsigned int temp;
unsigned int config=25;
unsigned int vol;

void delaySMG (unsigned int t)
{
	while(t--);
}

void delay (unsigned int t)
{
	while(t--)
	{
		displaySMG();
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

void SMGbitall()
{
	select(6);
	P0=0xff;
	select(7);
	P0=0xff;
}

void SMGbit(unsigned char pos,unsigned char value)
{
	select(6);
	P0=0x01<<pos;
	select(7);
	P0=SMG[value];
	SMGbitall();
}

void SMGbitdot(unsigned char pos,unsigned char value)
{
	select(6);
	P0=0x01<<pos;
	select(7);
	P0=SMGdot[value];
	SMGbitall();
}

void displaySMG()
{
	if(state==0)
	{
		SMGbit(0,11);
		delaySMG(100);
		SMGbit(1,14);
		delaySMG(100);
		SMGbit(2,14);
		delaySMG(100);
		SMGbit(3,14);
		delaySMG(100);
		
    SMGbit(4,temp/1000);
		delaySMG(100);
		SMGbitdot(5,(temp%1000)/100);
		delaySMG(100);
		SMGbit(6,(temp%100)/10);
		delaySMG(100);
		SMGbit(7,temp%10);
		delaySMG(100);
	}
	
	else if(state==1)
	{
		SMGbit(0,12);
		delaySMG(200);
		SMGbit(1,14);
		delaySMG(200);
		SMGbit(2,14);
		delaySMG(200);
		SMGbit(3,14);
		delaySMG(200);
		SMGbit(4,14);
		delaySMG(200);
		SMGbit(5,14);
		delaySMG(200);
		
		SMGbit(6,config/10);
		delaySMG(200);
		SMGbit(7,config%10);
		delaySMG(200);
		
		
	}
	
	else if(state==2)
	{
		SMGbit(0,13);
		delaySMG(200);
		SMGbit(1,14);
		delaySMG(200);
		SMGbit(2,14);
		delaySMG(200);
		SMGbit(3,14);
		delaySMG(200);
		SMGbit(4,14);
		delaySMG(200);
		
		SMGbitdot(5,vol/100);
		delaySMG(200);	
    SMGbit(6,(vol%100)/10);
		delaySMG(200);
		SMGbit(7,vol%10);
		delaySMG(200);		
	}
}

void scan_keys()
{
	R3=0;
	R2=R4=R1=1;
	C1=C2=C3=C4=1;
	if(C1==0)
	{
		if(C1==0)
		{
			delay(100);
			while(C1==0);
			switch(mode)
			{
				case 1:
					mode=2;
					break;
					
				case 2:
					mode=0;
					break;
			}
		}
	}
	
	else if(C2==0)
	{
		if(C2==0)
		{
			if(statek==1)
			{
				
			}
		}
	}



  R4=0;
	R2=R3=R1=1;
	C1=C2=C3=C4=1;
	if(C1==0)
	{
		if(C1==0)
		{
			delay(100);
			while(C1==0);
			switch(state)
			{
				case 0:
					state=1;
					statek=0;
					break;
					
				case 1:
					state=2;
					statek=1;
					break;
				
				case 2:
				  state=0;
					statek=0;
					break;
			}
		}
	}
	
	else if(C2==0)
	{
		if(C2==0)
		{
			if(statek==1)
			{}
		}
	}
}

void LED()
{
	if(mode==1)
	{
		select(4);
		L1=0;
		delay(80000);
	}
	
  if (state==0)
	{
    select(4);
		L2=0;
		delay(80000);
	}
	
		else if (state==1)
	{
    select(4);
		L3=0;
		delay(80000);
	}
	
		else if (state==2)
	{
    select(4);
		L4=0;
		delay(80000);
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
		temp=temp*10;
		temp=temp+(lsb&0x0f)*0.625;
	}
}

void init_sys()
{
	select(4);
	P0=0xff;
	select(5);
	P0=0x00;
	
}

void main()
{
	init_sys();
	while(1)
	{
	state=0;
	read_DS18B20_temp();
	displaySMG();
	}
}
