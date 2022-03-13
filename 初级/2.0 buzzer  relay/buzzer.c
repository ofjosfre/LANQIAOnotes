#include "reg52.h"
sbit HC138_A=P2^5;
sbit HC138_B=P2^6;
sbit HC138_C=P2^7;

void delay(unsigned int t)
{
while(t--);
while(t--);

}


void select(unsigned char n)
{
	switch (n)
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



void init()
{
select(5);
P0=0x00;

}

void led()
{
	unsigned char i;
	select(4);

for(i=0;i<3;i++)
{P0=0x00;
delay(80000);
P0=0xff;
delay(80000);
}                      //ÉÁ3ÏÂ
for(i=1;i<=8;i++)
	{
		P0=0xff<<i;
delay(80000);
delay(80000);
	}                     //´Ó×óµ½ÓÒÁÁÆð

  select(5);
P0=0x10;
delay(80000);
delay(80000);        //¼ÌµçÆ÷¿ª
select(5);
P0=0x00;
delay(80000);
delay(80000); 

for(i=1;i<=8;i++)
	{
		P0=~(0xff<<i);
delay(80000);               //´Ó×óµ½ÓÒÏ¨Ãð
delay(80000);
	}    

select(5);
P0=0x40;              //·äÃùÆ÷Ïì
delay(80000);
delay(80000);
select(5);
P0=0x00;
delay(80000);       //buzzer shuts down
delay(80000);

}

void main()
{
 while (1)
 {
init();
led() ;
 }
}