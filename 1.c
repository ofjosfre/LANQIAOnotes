/*==================蓝桥杯单片机特训==================
【进阶01】：灯光闪烁与数码管计数
**平  台：CT107D单片机综合实训平台
**模  式：IO模式
**设  计：欧浩源（小蜜蜂老师，ohy3686@qq.com）
**时  间：2022-03-31
**更多详见：www.xmf393.com
====================================================*/
#include "reg52.h"

//定义动态显示中单个数码管点亮时长
#define TSMG  500
//-------共阳数码管的段码编码表（无小数点）--------
//0 1 2 3 4 5 6 7 8 9 A B C D E F - .
unsigned char code SMG_NoDot[18]={0xc0,0xf9,
    0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,
    0x88,0x80,0xc6,0xc0,0x86,0x8e,0xbf,0x7f};

unsigned char stat_led = 0xff;    //定义LED灯当前状态
unsigned char num1 = 0, num2 = 0;  //定义计数变量
void Display_Num();                //声明数码管显示函数              
//====================普通延时函数====================
void Delay(unsigned int t)
{
  while(t--)
  {
    Display_Num();              //延时期间保持数码管刷新
  }
}
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
  P2 = (P2 & 0x1f) | 0x00;      //设置完成，关闭所有锁存器
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
//===================计数值显示函数===================
void Display_Num()
{
  //数码管右起第0位
  DisplaySMG_Bit(7,SMG_NoDot[num1  % 10]);
  //数码管右起第1位
  DisplaySMG_Bit(6,SMG_NoDot[num1 / 10]);    
  //数码管由起第7位
  DisplaySMG_Bit(0,SMG_NoDot[num2]);        
}
//====================灯光闪烁控制====================
void LED_Control()
{
  stat_led &= ~0x80;         
  Set_HC573(4, stat_led);    //L8灯点亮
  Delay(200);                //延时
  stat_led |= 0x80;         
  Set_HC573(4, stat_led);    //L8灯熄灭
  Delay(200);                //延时
  
  num1++;                    //L8闪烁累加
  if(num1 == 100)            //达到最大值时恢复0
  {
    num1 = 0;
  }
  //L1灯和L2灯同时翻转当前的工作状态
  if((stat_led & 0x03) == 0x03)
  {
    stat_led &= ~0x03;
  }
  else
  {
    stat_led |= 0x03;
  }
  Set_HC573(4, stat_led);
  
  num2++;                    //L1和L2翻转次数累加
  if(num2 == 10)            //达到最大值时恢复0
  {
    num2 = 0;
  }
}
//=====================系统初始化=====================
void Init_sys()
{
  Set_HC573(0, 0x00);        //关闭所有锁存器
  Set_HC573(5, 0x00);        //关闭蜂鸣器和继电器
  Set_HC573(4, 0xff);        //关闭全部LED灯
  DisplaySMG_All(0xff);      //关闭全部数码管
}
//=======================主函数=======================
void main()
{
  Init_sys();                //系统初始化
  while(1)
  {
    LED_Control();          //灯光闪烁
    Display_Num();          //数码管计数
  }
}
