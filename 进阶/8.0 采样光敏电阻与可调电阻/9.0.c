
/*==================���ű���Ƭ����ѵ==================
������06��������PCF8591��DACģ���ѹ���
**ƽ  ̨��CT107D��Ƭ���ۺ�ʵѵƽ̨
**ģ  ʽ��IOģʽ
**�ײ������ļ���2022�꾺����Դ���ݰ��ṩ���ļ�
**��  �ƣ�ŷ��Դ��С�۷���ʦ��ohy3686@qq.com��
**ʱ  �䣺2022-04-05
**���������www.xmf393.com
====================================================*/
#include "reg52.h"
#include "iic.h"
sbit S4 = P3^3;
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
unsigned char adc_value = 0;    //AIN3�Ĳ�������
float adc_volt = 0;              //AIN3�Ļ����ѹ
unsigned int smg_volt = 0;      //AIN3����ʾ��ѹ
unsigned char stat_smg = 1;      //��ǰ��ʾ�����־
unsigned char mode = 1;          //ģʽ
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
  //�����ѹ
  DisplaySMG_Bit(7,SMG_NoDot[smg_volt  % 10]);
  DisplaySMG_Bit(6,SMG_NoDot[(smg_volt / 10) % 10]);    
  DisplaySMG_Bit(5,SMG_Dot[smg_volt / 100]);        
  //����ģʽ
  DisplaySMG_Bit(2,SMG_NoDot[16]);
  DisplaySMG_Bit(1,SMG_NoDot[mode]);    
  DisplaySMG_Bit(0,SMG_NoDot[16]);    
}
//===============PCF8591��ѹ����������===============
void Read_PCF8591_AIN3()
{
  IIC_Start();          
  IIC_SendByte(0x90);    //PCF8591��д�豸��ַ 
  IIC_WaitAck();        
  IIC_SendByte(0x43);   //���DAC��ת��AIN3      
  IIC_WaitAck();              
  IIC_Stop(); 
  
  DisplaySMG_ADC();      //�ȴ���ѹת�����
  
  IIC_Start();                  
  IIC_SendByte(0x91);   //PCF8591�Ķ��豸��ַ
  IIC_WaitAck();                 
  adc_value = IIC_RecByte();  //����AD��������
  IIC_SendAck(1);         //������Ӧ���ź�                 
  IIC_Stop();
  //��ADC�����������ݻ���ɶ�Ӧ�ĵ�ѹֵ
  adc_volt = adc_value * (5.0 / 255);
  smg_volt = adc_volt * 100;
}
//===============PCF8591��ѹ������ú���===============
void Set_PCF8591_DAC(unsigned char dat)
{
  IIC_Start();          
  IIC_SendByte(0x90);    //PCF8591��д�豸��ַ 
  IIC_WaitAck();  
  IIC_SendByte(0x43);   //���DAC��ת��AIN3  
  IIC_WaitAck();                 
  IIC_SendByte(dat);     //����DAC��ѹ�������
  IIC_WaitAck();          //������Ӧ���ź�                 
  IIC_Stop();
}
//===================����ɨ�账����==================
void Scan_Keys()
{
  if(S4 == 0)
  {
    DelaySMG(500);  
    if(S4 == 0)
    {
      if(mode == 1)            
      {
        mode = 2;              //�л�Ϊģʽ2
        Set_PCF8591_DAC(204);  //����̶���ѹ4V
        smg_volt = 400;        //��ʾ��ѹ��4.00
      }
      else if(mode == 2)
      {
        mode = 3;              //�л�Ϊģʽ3
      }
      else if(mode == 3)
      {
        mode = 1;              //�л�Ϊģʽ1
        Set_PCF8591_DAC(102);  //����̶���ѹ2V
        smg_volt = 200;        //��ʾ��ѹ��2.00
      }
      while(S4 == 0)          //���ּ��
      {  
        DisplaySMG_ADC();      //��������ܶ�̬��ʾ
      }
    }
  }
}
//=====================ϵͳ��ʼ��=====================
void Init_sys()
{
  Set_HC573(0, 0x00);        //�ر�����������
  Set_HC573(5, 0x00);        //�رշ������ͼ̵���
  Set_HC573(4, 0xff);        //�ر�ȫ��LED��
  DisplaySMG_All(0xff);      //�ر�ȫ�������
  Set_PCF8591_DAC(102);      //����̶���ѹ2V
  smg_volt = 200;            //�������ʾ2.00V
}
//=======================������=======================
void main()
{
  Init_sys();                //ϵͳ��ʼ��
  while(1)
  {
    if(mode == 3)            //ģʽ3���ɵ��������DAC���
    {
      Read_PCF8591_AIN3();        //����AIN3��ѹ
      Set_PCF8591_DAC(adc_value);  //����DAC�������
    }
    Scan_Keys();          //ѭ��ɨ�谴��
    DisplaySMG_ADC();      //��̬ˢ�������
  }
}
//========================END========================