/*==================蓝桥杯单片机特训==================
【进阶04】：24C02存储按键触发次数
**平  台：CT107D单片机综合实训平台
**模  式：IO模式
**底层驱动文件：2022年竞赛资源数据包提供的文件
**设  计：欧浩源（小蜜蜂老师，ohy3686@qq.com）
**时  间：2022-04-01
**更多详见：www.xmf393.com
====================================================*/
#include "reg52.h"
#include "iic.h"
//定义三个独立按键：S4 S5 S6
//注意：J5的23脚短接，要接到BTN这边
sbit S4 = P3^3;
sbit S5 = P3^2;
sbit S6 = P3^1;
//定义动态显示中单个数码管点亮时长
#define TSMG  500
//-------共阳数码管的段码编码表（无小数点）--------
//0 1 2 3 4 5 6 7 8 9 A b C D E F - .
unsigned char code SMG_NoDot[18]={0xc0,0xf9,
    0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,
    0x88,0x83,0xc6,0xc0,0x86,0x8e,0xbf,0x7f};
//定义3个按键的计数变量
unsigned char dat1 = 0, dat2 = 0, dat3 = 0;
//==============数码管动态显示专用延时函数=============
void DelaySMG(unsigned int t)
{
  while(t--);
}
//==================设置选通的锁存器===================
void Set_HC573(unsigned char channel, unsigned char dat)
{
  P0 = dat;                      //待设置数据
  switch(channel)                //选通锁存器
  {
    case 4:
      P2 = (P2 & 0x1f) | 0x80;  //Y4输出0，LED控制
    break;
    case 5:
      P2 = (P2 & 0x1f) | 0xa0;  //Y5输出0，蜂鸣器和继电器控制
    break;
    case 6:
      P2 = (P2 & 0x1f) | 0xc0;  //Y6输出0，数码管位选
    break;
    case 7:
      P2 = (P2 & 0x1f) | 0xe0;  //Y7输出0，数码管段码
    break;
    case 0:
      P2 = (P2 & 0x1f) | 0x00;  //所有锁存器不选择
    break;
  }
  P2 = (P2 & 0x1f) | 0x00;      //设置完成，关闭全部锁存器
}
//=================单个数码管动态显示=================
void DisplaySMG_Bit(unsigned char pos, unsigned char value)
{
  Set_HC573(6, 0x01 << pos);    //数码管的段位
  Set_HC573(7, value);          //数码管显示内容
  DelaySMG(TSMG);                //数码管点亮时长
  Set_HC573(6, 0x01 << pos);        
  Set_HC573(7, 0xff);            //消隐
}
//=================全部数码管静态显示=================
void DisplaySMG_All(unsigned char value)
{
  Set_HC573(6, 0xff);    //使能所有数码管
  Set_HC573(7, value);  //数码管显示内容  
}
//===================读取值显示函数===================
void DisplaySMG_24C02()
{
  //数码管右起第0位
  DisplaySMG_Bit(7,SMG_NoDot[dat3  % 10]);
  //数码管右起第1位
  DisplaySMG_Bit(6,SMG_NoDot[dat3 / 10]);    
  //数码管右起第2位
  DisplaySMG_Bit(5,SMG_NoDot[16]);        
  
  //数码管右起第3位
  DisplaySMG_Bit(4,SMG_NoDot[dat2  % 10]);
  //数码管右起第4位
  DisplaySMG_Bit(3,SMG_NoDot[dat2 / 10]);    
  //数码管右起第5位
  DisplaySMG_Bit(2,SMG_NoDot[16]);        
  
  //数码管右起第6位
  DisplaySMG_Bit(1,SMG_NoDot[dat1 % 10]);        
  //数码管右起第7位
  DisplaySMG_Bit(0,SMG_NoDot[dat1 / 10]);        
}
//==================24C02单字节写入===================
void Write_24C02(unsigned char addr, unsigned char dat)
{
  IIC_Start();          //起始信号
  IIC_SendByte(0xa0);    //EEPROM的写设备地址
  IIC_WaitAck();        //等待从机应答
  IIC_SendByte(addr);    //内存单元地址
  IIC_WaitAck();        //等待从机应答
  IIC_SendByte(dat);    //内存写入数据
  IIC_WaitAck();        //等待从机应答
  IIC_Stop();            //停止信号
}
//==================24C02单字节读取===================
unsigned char Read_24C02(unsigned char addr)
{
  unsigned char tmp;
  //首先，进行一个伪写操作
  IIC_Start();          //起始信号
  IIC_SendByte(0xa0);    //EEPROM的写设备地址
  IIC_WaitAck();        //等待从机应答
  IIC_SendByte(addr);    //内存单元地址
  IIC_WaitAck();        //等待从机应答
  //然后，开始字节读操作
  IIC_Start();          //起始信号
  IIC_SendByte(0xa1);    //EEPROM的读设备地址
  IIC_WaitAck();        //等待从机应答
  tmp = IIC_RecByte();  //读取内存中的数据
  IIC_SendAck(1);        //产生非应答信号
  IIC_Stop();            //停止信号
  return tmp;
}
//==================独立按键扫描处理===================
void Scan_Keys()
{
  //按键S4的扫描处理
  if(S4 == 0)
  {
    DelaySMG(100);      //去抖动
    if(S4 == 0)
    {
      dat1++;            //按键次数增加
      if(dat1 > 13)      //大于13恢复0
      {
        dat1 = 0;
      }
      Write_24C02(0x00, dat1);  //写入24C02
      while(S4 == 0)            //松手检测
      {
        DisplaySMG_24C02();      //保持数码管动态刷新
      }
    }
  }
  //按键S5的扫描处理
  if(S5 == 0)
  {
    DelaySMG(100);
    if(S5 == 0)
    {
      dat2++;
      if(dat2 > 13)
      {
        dat2 = 0;
      }
      Write_24C02(0x01, dat2);
      while(S5 == 0)
      {
        DisplaySMG_24C02();
      }
    }
  }
  //按键S6的扫描处理
  if(S6 == 0)
  {
    DelaySMG(100);
    if(S6 == 0)
    {
      dat3++;
      if(dat3 > 13)
      {
        dat3 = 0;
      }
      Write_24C02(0x02, dat3);
      while(S6 == 0)
      {
        DisplaySMG_24C02();
      }
    }
  }
}
//=====================系统初始化=====================
void Init_sys()
{
  //系统基础设备初始化
  Set_HC573(0, 0x00);        //关闭所有锁存器
  Set_HC573(5, 0x00);        //关闭蜂鸣器和继电器
  Set_HC573(4, 0xff);        //关闭全部LED灯
  DisplaySMG_All(0xff);      //关闭全部数码管
  //将三个按键的历史按下次数读出
  dat1 = Read_24C02(0x00);  
  dat2 = Read_24C02(0x01);  
  dat3 = Read_24C02(0x02);  
}
//=======================主函数=======================
void main()
{
  Init_sys();                //系统初始化
  while(1)
  {
    Scan_Keys();            //循环扫描按键
    DisplaySMG_24C02();      //动态刷新数码管
  }
}
//========================END=======================