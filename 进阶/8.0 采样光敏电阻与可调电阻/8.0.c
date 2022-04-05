
/*==================���ű���Ƭ����ѵ==================
������05������������������ɵ�����ĵ�ѹ
**ƽ  ̨��CT107D��Ƭ���ۺ�ʵѵƽ̨
**ģ  ʽ��IOģʽ
**�ײ������ļ���2022�꾺����Դ���ݰ��ṩ���ļ�
**��  �ƣ�ŷ��Դ��С�۷���ʦ��ohy3686@qq.com��
**ʱ  �䣺2022-04-01
**���������www.xmf393.com
====================================================*/
#include "reg52.h"
#include "iic.h"
//���嶯̬��ʾ�е�������ܵ���ʱ��
#define TSMG  500
//-------��������ܵĶ���������С���㣩--------
//0 1 2 3 4 5 6 7 8 9 A B C D E F - .
unsigned char code SMG_NoDot[18]={0xc0,0xf9,
    0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,
    0x88,0x80,0xc6,0xc0,0x86,0x8e,0xbf,0x7f};
//-------��������ܵĶ���������С���㣩--------
//0. 1. 2. 3. 4. 5. 6. 7. 8. 9.
unsigned char code SMG_Dot[10]={0x40,0x79,
    0x24,0x30,0x19,0x12,0x02,0x78,0x00,0x10};
//-----------------------------------------------
unsigned char adc1_value = 0;    //AIN1�Ĳ�������
float adc1_volt = 0;            //AIN1�Ļ����ѹ
unsigned int smg1_volt = 0;      //AIN1����ʾ��ѹ
unsigned char adc3_value = 0;    //AIN3�Ĳ�������
float adc3_volt = 0;            //AIN3�Ļ����ѹ
unsigned int smg3_volt = 0;      //AIN3����ʾ��ѹ
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
  P2 = (P2 & 0x1f) | 0x00;      //������ɣ��ر�ȫ��������
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
//===================��ѹֵ��ʾ����===================
void DisplaySMG_ADC()
{
  //����������0λ
  DisplaySMG_Bit(7,SMG_NoDot[smg1_volt  % 10]);
  //����������1λ
  DisplaySMG_Bit(6,SMG_NoDot[(smg1_volt / 10) % 10]);    
  //����������2λ
  DisplaySMG_Bit(5,SMG_Dot[smg1_volt / 100]);        

  //����������5λ
  DisplaySMG_Bit(2,SMG_NoDot[smg3_volt  % 10]);
  //����������6λ
  DisplaySMG_Bit(1,SMG_NoDot[(smg3_volt / 10) % 10]);    
  //����������7λ
  DisplaySMG_Bit(0,SMG_Dot[smg3_volt / 100]);    
}
//===============PCF8591��ѹ����������===============
unsigned char Read_PCF8591_ADC(unsigned char ain)
{
  unsigned char tmp;
  IIC_Start();          
  IIC_SendByte(0x90);    //PCF8591��д�豸��ַ 
  IIC_WaitAck();        
  if(ain == 1)
  {
    IIC_SendByte(0x01); //ͨ��1�����������ѹ
  }
  else if(ain == 3)
  {
    IIC_SendByte(0x03); //ͨ��3���ɵ������ѹ
  }    
  IIC_WaitAck();              
  IIC_Stop(); 
  
  DisplaySMG_ADC();      //�ȴ���ѹת�����
  
  IIC_Start();                  
  IIC_SendByte(0x91);   //PCF8591�Ķ��豸��ַ
  IIC_WaitAck();                 
  tmp = IIC_RecByte();   //����AD��������
  IIC_SendAck(1);         //������Ӧ���ź�                 
  IIC_Stop();
  return tmp;
}
//=============������������Ϳɵ�����ĵ�ѹ=============
void Read_AIN1_AIN3()
{
  adc1_value = Read_PCF8591_ADC(1);
  adc1_volt = adc1_value * (5.0 / 255);
  smg1_volt = adc1_volt * 100;
  
  adc3_value = Read_PCF8591_ADC(3);
  adc3_volt = adc3_value * (5.0 / 255);
  smg3_volt = adc3_volt * 100;
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
  Init_sys();               //ϵͳ��ʼ��
  while(1)
  {
    Read_AIN1_AIN3();      //ѭ��������ѹ
    DisplaySMG_ADC();      //��̬ˢ�������
  }
}
//========================END=========================