/*******************************************************************************
*文件名   ： Uart.c
*实现功能 ： USART底层驱动
*实验平台 ： 秉火 Zigbee 开发板
*硬件连接 ： Usart0_Rx  P0_2 
             Usart0_Tx  P0_3
*作者     ： fire 
*创建时间 ： 2015/xx/xx
*论坛     ： http://www.chuxue123.com
*淘宝     ： http://firestm32.taobao.com
*******************************************************************************/
  
#include "Uart.h"

#define uint unsigned int 
#define uchar unsigned char

uint8_t Uart_Rec[BUFF_LEN]; //数据包以0D  0A结束
uint8_t Uart_Sta=0;
uint8_t Data_Len=0;
/**************************************************
 函 数 名  : InitUART
 功能描述  : CC2530串口初始化配置
 输入参数  : NONE  
 输出参数  : NONE
 返 回 值  : NONE
***************************************************/
void InitUART(void)
{ 
  
    PERCFG = 0x00;          //位置1 P0口 
    P0SEL = 0x3c;           //P0_2,P0_3 用作串口,第二功能 
    P2DIR &= ~0XC0;         //P0 优先作为UART0 ，优先级
    U0CSR |= 0x80;          //UART 方式 
    U0GCR |= 11;            //U0GCR与U0BAUD配合     
    U0BAUD |= 216;          //波特率设为115200 
    UTX0IF = 0;             //UART0 TX 中断标志清0
    U0CSR |= 0X40;          //允许接收 
    URX0IE=1;               //接收中断
    UTX0IF = 0;
    EA=1;                   //开总中断，                   
}

 
/**************************************************
 函 数 名  : Send_char
 功能描述  : 串口向电脑发送字节
 输入参数  : c---所要发送的数据 
 输出参数  : NONE
 返 回 值  : NONE
***************************************************/
void Send_char(uchar c)
{
    U0DBUF = c; 
    while(UTX0IF == 0);   //发送完成标志位
    UTX0IF = 0;   
}


/**************************************************
 函 数 名  : UartSend_String
 功能描述  : 串口向电脑发送字符串
 输入参数  : *Data---字符串首地址
             len---字符串长度 
 输出参数  : NONE
 返 回 值  : NONE
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
 函 数 名  : SetSysClock
 功能描述  : 设置系统时钟为32MHz
 输入参数  : NONE
 输出参数  : NONE
 返 回 值  : NONE
***************************************************/
void SetSysClock(void)
{
    CLKCONCMD &= ~0x40;           //选择系统时钟源为外部32MHz crystal oscillator
    while(CLKCONSTA & 0x40);      //等待晶振稳定为32MHz
    CLKCONCMD &= ~0x47;           //设置系统主时钟频率为32MHZ  
}



/**************************************************
 函 数 名  : UART0_ISR
 功能描述  : USART接收中断
 输入参数  : NONE  
 输出参数  : 
 返 回 值  : NONE
***************************************************/
#pragma vector = URX0_VECTOR 
  __interrupt void UART0_ISR(void) 
 { 
    uint8_t Res;
    Res = U0DBUF;
    
    if((Uart_Sta&0x80) == 0)
    {
        if((Uart_Sta&0x40) == 1)//接收到0x0d
        {
            if(Res != 0x0a)//接受错误，重新开始
            {
                Data_Len = 0;
                Uart_Sta = 0;
            }
            else
            {
                Uart_Sta |= 0x80;//接收完成
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
      
    URX0IF = 0;    // 清中断标志 
 }


/*********************************************END OF FILE**********************/