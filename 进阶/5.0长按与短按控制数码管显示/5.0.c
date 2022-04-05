/*==================蓝桥杯单片机特训==================
【进阶02】：长按与短按控制数码管显示
**平  台：CT107D单片机综合实训平台
**模  式：IO模式
**设  计：欧浩源（小蜜蜂老师，ohy3686@qq.com）
**时  间：2022-03-31
**更多详见：www.xmf393.com
====================================================*/
#include "reg52.h"

sbit S4 = P3^3;

//定义动态显示中单个数码管点亮时长
#define TSMG  500
//-------共阳数码管的段码编码表（无小数点）--------
//0 1 2 3 4 5 6 7 8 9 A b C D E F - .
unsigned char code SMG_NoDot[18]={0xc0,0xf9,
    0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,
    0x88,0x83,0xc6,0xc0,0x86,0x8e,0xbf,0x7f};

unsigned char num = 28;                //定义计数变量
bit F_key = 0;                    //按键按下标志
unsigned int count_t = 0;          //10ms单位累计
void Display_Num();                //声明数码管显示函数              

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
//==================计数值显示函数====================
void Display_Num()
{
  //数码管右起第0位
  DisplaySMG_Bit(7,SMG_NoDot[num  % 10]);
  //数码管右起第1位
  DisplaySMG_Bit(6,SMG_NoDot[num / 10]);          
}
//===================定时器T0初始化===================
void Init_Timer0()
{
  //定时器计数初值设置：10ms
  TH0 = (0 - 10000) / 256;
  TL0 = (0 - 10000) % 256;
  ET0 = 1;              //使能定时器0中断
  EA = 1;                //打开总中断
  TR0 = 1;              //启动定时器0
}
//===============定时器T0的中断服务函数================
void Service_Timer0() interrupt 1
{
  TH0 = (0 - 10000) / 256;
  TL0 = (0 - 10000) % 256;
  if(F_key == 1)        //在按键按下期间进行计数
  {
    count_t++;          //按键按下的持续时间
  }
}
//==================按键扫描处理函数===================
void Scan_Keys()
{
  if(S4 == 0)
  {
    DelaySMG(1000);      //去抖动处理
    if(S4 == 0)          //确认为按下信号
    {
      count_t = 0;      //时间计数变量清0
      F_key = 1;        //标志按键按下状态
      while(S4 == 0)    //等待按键松开
      {
        Display_Num();  //在按下期间保持数码管正常显示
      }
      F_key = 0;        //标志按键松开状态
      
      if(count_t > 100)  //按下时间大于1秒，长按
      {
        num = 0;        //数码管计数清除为00
      }
      else              //按下时间小于1秒，短按
      {
        num++;          //数码管计数加1
        if(num == 100)
        {
          num = 0;      //计数超出最大值99后恢复00
        }
      }
    }
  }
}
//=====================系统初始化=====================
void Init_sys()
{
  Set_HC573(0, 0x00);        //关闭所有锁存器
  Set_HC573(5, 0x00);        //关闭蜂鸣器和继电器
  Set_HC573(4, 0xff);        //关闭全部LED灯
  DisplaySMG_All(0xff);      //关闭全部数码管
  Init_Timer0();            //初始化定时器0并启动
}
//=======================主函数=======================
void main()
{
  Init_sys();                //系统初始化
  while(1)
  {
    Scan_Keys();            //循环扫描按键
    Display_Num();          //数码管刷新
  }
}
