#include "reg52.h"

sfr P4    = 0xC0;

unsigned char code SMG[18]=
{0xc0,0xf9,0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,
0x90,0x88,0x80,0xc6,0xc0,0x86,0x8e,0xbf,0x7f};

sbit HC138_A=P2^5;
sbit HC138_B=P2^6;
sbit HC138_C=P2^7;

sbit R1=P3^0;
sbit R2=P3^1;
sbit R3=P3^2;
sbit R4=P3^3;

sbit C4=P3^4;
sbit C3=P3^5;
sbit C2=P4^2;
sbit C1=P4^4;

void select(unsigned char n)
{
	switch(n)
	{
		case4:
	   HC138_C=1;
     HC138_B=0;
     HC138_A=0;
		 break;
		case5:
	   HC138_C=1;
     HC138_B=0;
     HC138_A=1;
		 break;
	  case6:
	   HC138_C=1;
     HC138_B=1;
     HC138_A=0;
		 break;
		case7:
	   HC138_C=1;
     HC138_B=1;
     HC138_A=1;
		 break;
  }
}

void displaykey(unsigned char value)
{
	select(6);
	P0=0x01;
	select(7);
	P0=value;
}

unsigned char keynumber;

void scankeysmulti()
{
  R1=0;
	R2=R3=R4=1;
	C1=C2=C3=C4=1;
	if (C1==0)
	{
		while(C1==0);
		keynumber=0;
		displaykey(SMG[keynumber]);
	}
	else if(C2==0)
	{
		while(C2==0);
		keynumber=1;
		displaykey(SMG[keynumber]);
	}
	else if(C3==0)
	{
		while(C3==0);
		keynumber=2;
		displaykey(SMG[keynumber]);
	}
		else if(C4==0)
	{
		while(C4==0);
		keynumber=3;
		displaykey(SMG[keynumber]);
	}                                  //c1
	R2=0;
	R1=R3=R4=1;
	C1=C2=C3=C4=1;
	if (C1==0)
	{
		while(C1==0);
		keynumber=4;
		displaykey(SMG[keynumber]);
	}
	else if(C2==0)
	{
		while(C2==0);
		keynumber=5;
		displaykey(SMG[keynumber]);
	}
	else if(C3==0)
	{
		while(C3==0);
		keynumber=6;
		displaykey(SMG[keynumber]);
	}
		else if(C4==0)
	{
		while(C4==0);
		keynumber=7;
		displaykey(SMG[keynumber]);
	}                              //c2
	R3=0;
	R1=R2=R4=1;
	C1=C2=C3=C4=1;
	if (C1==0)
	{
		while(C1==0);
		keynumber=8;
		displaykey(SMG[keynumber]);
	}
	else if(C2==0)
	{
		while(C2==0);
		keynumber=9;
		displaykey(SMG[keynumber]);
	}
	else if(C3==0)
	{
		while(C3==0);
		keynumber=10;
		displaykey(SMG[keynumber]);
	}
		else if(C4==0)
	{
		while(C4==0);
		keynumber=11;
		displaykey(SMG[keynumber]);
	}                                 //c3
	R4=0;
	R1=R2=R3=1;
	C1=C2=C3=C4=1;
	if (C1==0)
	{
		while(C1==0);
		keynumber=12;
		displaykey(SMG[keynumber]);
	}
	else if(C2==0)
	{
		while(C2==0);
		keynumber=13;
		displaykey(SMG[keynumber]);
	}
	else if(C3==0)
	{
		while(C3==0);
		keynumber=14;
		displaykey(SMG[keynumber]);
	}
		else if(C4==0)
	{
		while(C4==0);
		keynumber=15;
		displaykey(SMG[keynumber]);
	}                               //c4
}


void main()
{
  while(1)
	{
		scankeysmulti();
	}
}