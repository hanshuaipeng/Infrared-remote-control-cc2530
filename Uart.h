/*******************************************************************************
*�ļ���   �� Uart.h
*ʵ�ֹ��� �� USART�ײ�����
*ʵ��ƽ̨ �� ���� Zigbee ������
*Ӳ������ �� Usart0_Rx  P0_2 
             Usart0_Tx  P0_3
*����     �� fire 
*����ʱ�� �� 2015/xx/xx
*��̳     �� http://www.chuxue123.com
*�Ա�     �� http://firestm32.taobao.com
*******************************************************************************/

#ifndef __Uart_H
#define	__Uart_H
#include "type.h"
#include <ioCC2530.h>
#include <string.h>

#define uint unsigned int 
#define uchar unsigned char
#define BUFF_LEN  100

extern uint8_t Uart_Rec[BUFF_LEN]; //���ݰ���0D  0A����
extern uint8_t Uart_Sta;
extern uint8_t Data_Len;

void InitUART(void);
void Send_char(uchar c);
void UartSend_String(uint8_t *Data,uint8_t len);
void SetSysClock(void);
void Delayms(uint xms);

#endif 

/*********************************************END OF FILE**********************/
