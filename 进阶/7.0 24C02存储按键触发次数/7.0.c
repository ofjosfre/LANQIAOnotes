/*==================���ű���Ƭ����ѵ==================
������04����24C02�洢������������
**ƽ  ̨��CT107D��Ƭ���ۺ�ʵѵƽ̨
**ģ  ʽ��IOģʽ
**�ײ������ļ���2022�꾺����Դ���ݰ��ṩ���ļ�
**��  �ƣ�ŷ��Դ��С�۷���ʦ��ohy3686@qq.com��
**ʱ  �䣺2022-04-01
**���������www.xmf393.com
====================================================*/
#include "reg52.h"
#include "iic.h"
//������������������S4 S5 S6
//ע�⣺J5��23�Ŷ̽ӣ�Ҫ�ӵ�BTN���
sbit S4 = P3^3;
sbit S5 = P3^2;
sbit S6 = P3^1;
//���嶯̬��ʾ�е�������ܵ���ʱ��
#define TSMG  500
//-------��������ܵĶ���������С���㣩--------
//0 1 2 3 4 5 6 7 8 9 A b C D E F - .
unsigned char code SMG_NoDot[18]={0xc0,0xf9,
    0xa4,0xb0,0x99,0x92,0x82,0xf8,0x80,0x90,
    0x88,0x83,0xc6,0xc0,0x86,0x8e,0xbf,0x7f};
//����3�������ļ�������
unsigned char dat1 = 0, dat2 = 0, dat3 = 0;
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
//===================��ȡֵ��ʾ����===================
void DisplaySMG_24C02()
{
  //����������0λ
  DisplaySMG_Bit(7,SMG_NoDot[dat3  % 10]);
  //����������1λ
  DisplaySMG_Bit(6,SMG_NoDot[dat3 / 10]);    
  //����������2λ
  DisplaySMG_Bit(5,SMG_NoDot[16]);        
  
  //����������3λ
  DisplaySMG_Bit(4,SMG_NoDot[dat2  % 10]);
  //����������4λ
  DisplaySMG_Bit(3,SMG_NoDot[dat2 / 10]);    
  //����������5λ
  DisplaySMG_Bit(2,SMG_NoDot[16]);        
  
  //����������6λ
  DisplaySMG_Bit(1,SMG_NoDot[dat1 % 10]);        
  //����������7λ
  DisplaySMG_Bit(0,SMG_NoDot[dat1 / 10]);        
}
//==================24C02���ֽ�д��===================
void Write_24C02(unsigned char addr, unsigned char dat)
{
  IIC_Start();          //��ʼ�ź�
  IIC_SendByte(0xa0);    //EEPROM��д�豸��ַ
  IIC_WaitAck();        //�ȴ��ӻ�Ӧ��
  IIC_SendByte(addr);    //�ڴ浥Ԫ��ַ
  IIC_WaitAck();        //�ȴ��ӻ�Ӧ��
  IIC_SendByte(dat);    //�ڴ�д������
  IIC_WaitAck();        //�ȴ��ӻ�Ӧ��
  IIC_Stop();            //ֹͣ�ź�
}
//==================24C02���ֽڶ�ȡ===================
unsigned char Read_24C02(unsigned char addr)
{
  unsigned char tmp;
  //���ȣ�����һ��αд����
  IIC_Start();          //��ʼ�ź�
  IIC_SendByte(0xa0);    //EEPROM��д�豸��ַ
  IIC_WaitAck();        //�ȴ��ӻ�Ӧ��
  IIC_SendByte(addr);    //�ڴ浥Ԫ��ַ
  IIC_WaitAck();        //�ȴ��ӻ�Ӧ��
  //Ȼ�󣬿�ʼ�ֽڶ�����
  IIC_Start();          //��ʼ�ź�
  IIC_SendByte(0xa1);    //EEPROM�Ķ��豸��ַ
  IIC_WaitAck();        //�ȴ��ӻ�Ӧ��
  tmp = IIC_RecByte();  //��ȡ�ڴ��е�����
  IIC_SendAck(1);        //������Ӧ���ź�
  IIC_Stop();            //ֹͣ�ź�
  return tmp;
}
//==================��������ɨ�账��===================
void Scan_Keys()
{
  //����S4��ɨ�账��
  if(S4 == 0)
  {
    DelaySMG(100);      //ȥ����
    if(S4 == 0)
    {
      dat1++;            //������������
      if(dat1 > 13)      //����13�ָ�0
      {
        dat1 = 0;
      }
      Write_24C02(0x00, dat1);  //д��24C02
      while(S4 == 0)            //���ּ��
      {
        DisplaySMG_24C02();      //��������ܶ�̬ˢ��
      }
    }
  }
  //����S5��ɨ�账��
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
  //����S6��ɨ�账��
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
//=====================ϵͳ��ʼ��=====================
void Init_sys()
{
  //ϵͳ�����豸��ʼ��
  Set_HC573(0, 0x00);        //�ر�����������
  Set_HC573(5, 0x00);        //�رշ������ͼ̵���
  Set_HC573(4, 0xff);        //�ر�ȫ��LED��
  DisplaySMG_All(0xff);      //�ر�ȫ�������
  //��������������ʷ���´�������
  dat1 = Read_24C02(0x00);  
  dat2 = Read_24C02(0x01);  
  dat3 = Read_24C02(0x02);  
}
//=======================������=======================
void main()
{
  Init_sys();                //ϵͳ��ʼ��
  while(1)
  {
    Scan_Keys();            //ѭ��ɨ�谴��
    DisplaySMG_24C02();      //��̬ˢ�������
  }
}
//========================END=======================