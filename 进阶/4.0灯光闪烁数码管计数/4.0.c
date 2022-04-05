/*==================���ű���Ƭ����ѵ==================
������01�����ƹ���˸������ܼ���
**ƽ  ̨��CT107D��Ƭ���ۺ�ʵѵƽ̨
**ģ  ʽ��IOģʽ
**��  �ƣ�ŷ��Դ��С�۷���ʦ��ohy3686@qq.com��
**ʱ  �䣺2022-03-31
**���������www.xmf393.com
====================================================*/
#include "reg52.h"

//���嶯̬��ʾ�е�������ܵ���ʱ��
#define TSMG  500
//-------��������ܵĶ���������С���㣩--------
//0 1 2 3 4 5 6 7 8 9 A B C D E F - .
unsigned char code SMG_NoDot[18]={0xc0,0xf9,
    0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,
    0x88,0x80,0xc6,0xc0,0x86,0x8e,0xbf,0x7f};

unsigned char stat_led = 0xff;    //����LED�Ƶ�ǰ״̬
unsigned char num1 = 0, num2 = 0;  //�����������
void Display_Num();                //�����������ʾ����              
//====================��ͨ��ʱ����====================
void Delay(unsigned int t)
{
  while(t--)
  {
    Display_Num();              //��ʱ�ڼ䱣�������ˢ��
  }
}
//==============����ܶ�̬��ʾר����ʱ����=============
void DelaySMG(unsigned int t)
{
  while(t--);
}
//==================����ѡͨ��������===================
void Set_HC573(unsigned char channel, unsigned char dat)
{
  P0 = dat;                      //����������
  switch(channel)                //ѡͨ������
  {
    case 4:
      P2 = (P2 & 0x1f) | 0x80;  //Y4���0��LED����
    break;
    case 5:
      P2 = (P2 & 0x1f) | 0xa0;  //Y5���0���������ͼ̵�������
    break;
    case 6:
      P2 = (P2 & 0x1f) | 0xc0;  //Y6���0�������λѡ
    break;
    case 7:
      P2 = (P2 & 0x1f) | 0xe0;  //Y7���0������ܶ���
    break;
    case 0:
      P2 = (P2 & 0x1f) | 0x00;  //������������ѡ��
    break;
  }
  P2 = (P2 & 0x1f) | 0x00;      //������ɣ��ر�����������
}
//=================��������ܶ�̬��ʾ=================
void DisplaySMG_Bit(unsigned char pos, unsigned char value)
{
  Set_HC573(6, 0x01 << pos);    //����ܵĶ�λ
  Set_HC573(7, value);          //�������ʾ����
  DelaySMG(TSMG);                //����ܵ���ʱ��
  Set_HC573(6, 0x01 << pos);        
  Set_HC573(7, 0xff);            //����
}
//=================ȫ������ܾ�̬��ʾ=================
void DisplaySMG_All(unsigned char value)
{
  Set_HC573(6, 0xff);    //ʹ�����������
  Set_HC573(7, value);  //�������ʾ����  
}
//===================����ֵ��ʾ����===================
void Display_Num()
{
  //����������0λ
  DisplaySMG_Bit(7,SMG_NoDot[num1  % 10]);
  //����������1λ
  DisplaySMG_Bit(6,SMG_NoDot[num1 / 10]);    
  //����������7λ
  DisplaySMG_Bit(0,SMG_NoDot[num2]);        
}
//====================�ƹ���˸����====================
void LED_Control()
{
  stat_led &= ~0x80;         
  Set_HC573(4, stat_led);    //L8�Ƶ���
  Delay(200);                //��ʱ
  stat_led |= 0x80;         
  Set_HC573(4, stat_led);    //L8��Ϩ��
  Delay(200);                //��ʱ
  
  num1++;                    //L8��˸�ۼ�
  if(num1 == 100)            //�ﵽ���ֵʱ�ָ�0
  {
    num1 = 0;
  }
  //L1�ƺ�L2��ͬʱ��ת��ǰ�Ĺ���״̬
  if((stat_led & 0x03) == 0x03)
  {
    stat_led &= ~0x03;
  }
  else
  {
    stat_led |= 0x03;
  }
  Set_HC573(4, stat_led);
  
  num2++;                    //L1��L2��ת�����ۼ�
  if(num2 == 10)            //�ﵽ���ֵʱ�ָ�0
  {
    num2 = 0;
  }
}
//=====================ϵͳ��ʼ��=====================
void Init_sys()
{
  Set_HC573(0, 0x00);        //�ر�����������
  Set_HC573(5, 0x00);        //�رշ������ͼ̵���
  Set_HC573(4, 0xff);        //�ر�ȫ��LED��
  DisplaySMG_All(0xff);      //�ر�ȫ�������
}
//=======================������=======================
void main()
{
  Init_sys();                //ϵͳ��ʼ��
  while(1)
  {
    LED_Control();          //�ƹ���˸
    Display_Num();          //����ܼ���
  }
}