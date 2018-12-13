/*******************************************************************************
*�ļ���   �� Uart.c
*ʵ�ֹ��� �� USART�ײ�����
*ʵ��ƽ̨ �� ���� Zigbee ������
*Ӳ������ �� Usart0_Rx  P0_2 
             Usart0_Tx  P0_3
*����     �� fire 
*����ʱ�� �� 2015/xx/xx
*��̳     �� http://www.chuxue123.com
*�Ա�     �� http://firestm32.taobao.com
*******************************************************************************/
  
#include "Uart.h"

#define uint unsigned int 
#define uchar unsigned char

uint8_t Uart_Rec[BUFF_LEN]; //���ݰ���0D  0A����
uint8_t Uart_Sta=0;
uint8_t Data_Len=0;
/**************************************************
 �� �� ��  : InitUART
 ��������  : CC2530���ڳ�ʼ������
 �������  : NONE  
 �������  : NONE
 �� �� ֵ  : NONE
***************************************************/
void InitUART(void)
{ 
  
    PERCFG = 0x00;          //λ��1 P0�� 
    P0SEL = 0x3c;           //P0_2,P0_3 ��������,�ڶ����� 
    P2DIR &= ~0XC0;         //P0 ������ΪUART0 �����ȼ�
    U0CSR |= 0x80;          //UART ��ʽ 
    U0GCR |= 11;            //U0GCR��U0BAUD���     
    U0BAUD |= 216;          //��������Ϊ115200 
    UTX0IF = 0;             //UART0 TX �жϱ�־��0
    U0CSR |= 0X40;          //������� 
    URX0IE=1;               //�����ж�
    UTX0IF = 0;
    EA=1;                   //�����жϣ�                   
}

 
/**************************************************
 �� �� ��  : Send_char
 ��������  : ��������Է����ֽ�
 �������  : c---��Ҫ���͵����� 
 �������  : NONE
 �� �� ֵ  : NONE
***************************************************/
void Send_char(uchar c)
{
    U0DBUF = c; 
    while(UTX0IF == 0);   //������ɱ�־λ
    UTX0IF = 0;   
}


/**************************************************
 �� �� ��  : UartSend_String
 ��������  : ��������Է����ַ���
 �������  : *Data---�ַ����׵�ַ
             len---�ַ������� 
 �������  : NONE
 �� �� ֵ  : NONE
***************************************************/
void UartSend_String(uint8_t *Data,uint8_t len)
{
    int j;
    for(j=0;j<len;j++)
    {
        U0DBUF = *Data++;
        while(UTX0IF == 0);
        UTX0IF = 0;
    }
}


/**************************************************
 �� �� ��  : SetSysClock
 ��������  : ����ϵͳʱ��Ϊ32MHz
 �������  : NONE
 �������  : NONE
 �� �� ֵ  : NONE
***************************************************/
void SetSysClock(void)
{
    CLKCONCMD &= ~0x40;           //ѡ��ϵͳʱ��ԴΪ�ⲿ32MHz crystal oscillator
    while(CLKCONSTA & 0x40);      //�ȴ������ȶ�Ϊ32MHz
    CLKCONCMD &= ~0x47;           //����ϵͳ��ʱ��Ƶ��Ϊ32MHZ  
}



/**************************************************
 �� �� ��  : UART0_ISR
 ��������  : USART�����ж�
 �������  : NONE  
 �������  : 
 �� �� ֵ  : NONE
***************************************************/
#pragma vector = URX0_VECTOR 
  __interrupt void UART0_ISR(void) 
 { 
    uint8_t Res;
    Res = U0DBUF;
    
    if((Uart_Sta&0x80) == 0)
    {
        if((Uart_Sta&0x40) == 1)//���յ�0x0d
        {
            if(Res != 0x0a)//���ܴ������¿�ʼ
            {
                Data_Len = 0;
                Uart_Sta = 0;
            }
            else
            {
                Uart_Sta |= 0x80;//�������
            }
        }
        else
        {
            if(Res == 0x0d)
                Uart_Sta |= 0x40;
            else
            {
                Uart_Rec[Data_Len] = Res;
                Data_Len++;
                if(Data_Len>BUFF_LEN)
                  Uart_Sta=0;
            }
        }
    }
      
    URX0IF = 0;    // ���жϱ�־ 
 }


/*********************************************END OF FILE**********************/