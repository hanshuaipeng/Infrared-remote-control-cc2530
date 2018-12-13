/*******************************************************************************
*文件名   ： Uart.h
*实现功能 ： USART底层驱动
*实验平台 ： 秉火 Zigbee 开发板
*硬件连接 ： Usart0_Rx  P0_2 
             Usart0_Tx  P0_3
*作者     ： fire 
*创建时间 ： 2015/xx/xx
*论坛     ： http://www.chuxue123.com
*淘宝     ： http://firestm32.taobao.com
*******************************************************************************/

#ifndef __Uart_H
#define	__Uart_H
#include "type.h"
#include <ioCC2530.h>
#include <string.h>

#define uint unsigned int 
#define uchar unsigned char
#define BUFF_LEN  100

extern uint8_t Uart_Rec[BUFF_LEN]; //数据包以0D  0A结束
extern uint8_t Uart_Sta;
extern uint8_t Data_Len;

void InitUART(void);
void Send_char(uchar c);
void UartSend_String(uint8_t *Data,uint8_t len);
void SetSysClock(void);
void Delayms(uint xms);

#endif 

/*********************************************END OF FILE**********************/
