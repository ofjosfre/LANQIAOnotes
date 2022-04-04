/*
  程序说明: IIC总线驱动程序
  软件环境: Keil uVision 4.10 
  硬件环境: CT107单片机综合实训平台 8051，12MHz
  日    期: 2011-8-9
*/

#include "STC15F2K60S2.h"
#include "intrins.h"

#define DELAY_TIME 50

#define SlaveAddrW 0xA0
#define SlaveAddrR 0xA1

//总线引脚定义
sbit SDA = P2^1;  /* 数据线 */
sbit SCL = P2^0;  /* 时钟线 */

void IIC_Delay(unsigned char i)
{
    do{_nop_();}
    while(i--);        
}
//总线启动条件
void IIC_Start(void)
{
    SDA = 1;
    SCL = 1;
    IIC_Delay(DELAY_TIME);
    SDA = 0;
    IIC_Delay(DELAY_TIME);
    SCL = 0;	
}

//总线停止条件
void IIC_Stop(void)
{
    SDA = 0;
    SCL = 1;
    IIC_Delay(DELAY_TIME);
    SDA = 1;
    IIC_Delay(DELAY_TIME);
}

//发送应答
void IIC_SendAck(bit ackbit)
{
    SCL = 0;
    SDA = ackbit;  					// 0：应答，1：非应答
    IIC_Delay(DELAY_TIME);
    SCL = 1;
    IIC_Delay(DELAY_TIME);
    SCL = 0; 
    SDA = 1;
    IIC_Delay(DELAY_TIME);
}

//等待应答
bit IIC_WaitAck(void)
{
    bit ackbit;
	
    SCL  = 1;
    IIC_Delay(DELAY_TIME);
    ackbit = SDA;
    SCL = 0;
    IIC_Delay(DELAY_TIME);
    return ackbit;
}

//通过I2C总线发送数据
void IIC_SendByte(unsigned char byt)
{
    unsigned char i;

    for(i=0; i<8; i++)
    {
        SCL  = 0;
        IIC_Delay(DELAY_TIME);
        if(byt & 0x80) SDA  = 1;
        else SDA  = 0;
        IIC_Delay(DELAY_TIME);
        SCL = 1;
        byt <<= 1;
        IIC_Delay(DELAY_TIME);
    }
    SCL  = 0;  
}

//从I2C总线上接收数据
unsigned char IIC_RecByte(void)
{
    unsigned char i, da;
    for(i=0; i<8; i++)
    {   
    	SCL = 1;
	IIC_Delay(DELAY_TIME);
	da <<= 1;
	if(SDA) da |= 1;
	SCL = 0;
	IIC_Delay(DELAY_TIME);
    }
    return da;    
}

void write_pcf8591(unsigned char dat)
{
	IIC_Start();
	IIC_SendByte(0X90);
	IIC_WaitAck();
	IIC_SendByte(0X40);
	IIC_WaitAck();
	IIC_SendByte(dat);
	IIC_WaitAck();
	IIC_Stop();
}

void write_eeprom(unsigned char add,unsigned char dat)
{
	IIC_Start();
	IIC_SendByte(0Xa0);
	IIC_WaitAck();
	IIC_SendByte(add);
	IIC_WaitAck();
	IIC_SendByte(dat);
	IIC_WaitAck();
	IIC_Stop();	
}

unsigned char read_eeprom(unsigned char add)
{
	unsigned char ret;//下面可以加一个EA=0关闭终中断
	IIC_Start();
	IIC_SendByte(0Xa0);
	IIC_WaitAck();
	IIC_SendByte(add);
	IIC_WaitAck();
	
	IIC_Start();
	IIC_SendByte(0Xa1);
	IIC_WaitAck();
	ret = IIC_RecByte();
	IIC_SendAck(1);
	IIC_Stop();//下面可以加一个EA=1打开中断
	
	return ret;
}

unsigned char Read_AIN3()
{
	unsigned char temp = 0;
	unsigned char i;
	for(i=0;i<2;i++)
	{
	IIC_Start();//IIC总线起始信号
	IIC_SendByte(0x90);//发送PCF8591的写操作地址
	IIC_WaitAck();//等待从机应答
	IIC_SendByte(0x03);//发送控制字节，选择模拟量输入模式(4路单端输入)和通道(通道3)。
	//IIC_SendByte(0x01);//发送控制字节，选择模拟量输入模式(4路单端输入)和通道(通道1)。
	IIC_WaitAck();//等待从机应答
	IIC_Stop();//IIC总线起始信号
	
	IIC_Start();
	IIC_SendByte(0x91);//发送PCF8591的读操作地址
	IIC_WaitAck();
	temp = IIC_RecByte();//读取PCF8591通道的数据 
	IIC_SendAck(1);//产生非应答信号
	IIC_Stop();
	}
	return temp;
}
