#include "onewire.h"

//�������ڲ���ʱ����
void Delay_OneWire(unsigned int t)  
{
	while(t--);
}

//������д����
void Write_DS18B20(unsigned char dat)
{
	unsigned char i;
	for(i=0;i<8;i++)
	{
		DQ = 0;
		DQ = dat&0x01;
		Delay_OneWire(60);
		DQ = 1;
		dat >>= 1;
	}
	Delay_OneWire(60);
}

//�����߶�����
unsigned char Read_DS18B20(void)
{
	unsigned char i;
	unsigned char dat;
  
	for(i=0;i<8;i++)
	{
		DQ = 0;
		dat >>= 1;
		DQ = 1;
		if(DQ)
		{
			dat |= 0x80;
		}	    
		Delay_OneWire(60);
	}
	return dat;
}

//DS18B20��ʼ��
bit init_ds18b20(void)
{
  	bit initflag = 0;
  	
  	DQ = 1;
  	Delay_OneWire(144);
  	DQ = 0;
  	Delay_OneWire(960);
  	DQ = 1;
  	Delay_OneWire(120); 
    initflag = DQ;     
  	Delay_OneWire(60);
  
  	return initflag;
}
