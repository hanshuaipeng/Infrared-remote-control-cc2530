
#include <ioCC2530.h>
#include "Uart.h"
#include <stdio.h>
#include"IR.H"
#include "type.h"
#include "string.h"
uint8_t Red_Data[80];
uint8_t Key_Num=0;
uint8_t User_Len=0;
//CRC16校验函数
//CRC校验位低位在前，高位在后
uint16_t Ar_crc_16(uint8_t *input,uint16_t len)
{
    uint16_t n = 0;
    uint8_t m=0;
    uint16_t crc_in = 0;
    uint16_t crc_re = 0xffff;
    uint16_t poly = 0xa001;
    uint16_t xor_out = 0x0000;

    crc_in = input[len - 2] * 256 + input[len - 1];
    for(n=0;n<(len-2);n++)
    {
            crc_re = crc_re ^ input[n];
            for(m=0;m<8;m++)
            {
                    if(crc_re & 1)
                    {
                            crc_re >>= 1;
                            crc_re = crc_re ^ poly;
                    }
                    else
                    {
                            crc_re >>= 1;
                    }
            }
    }
    crc_re = crc_re ^ xor_out;
    if((crc_in == crc_re)||(crc_in == 0)){
            return crc_re;
    }
    else{
            return 0;
    }
}

void Clear_Buff(uint8_t *buff,uint8_t len)
{
    uint8_t i=0;
    for(i=0;i<len;i++)
    {
        buff[i]=0;
    }
}
//学习到的数据，uint16转换成uint8，后添加两个校验，通过串口发送
void Study_Data_Vul()
{
    uint8_t i=0;
    uint16_t crc=0;
   if(IR_Study_Pro()==SUCCESS)
    {
        for(i=0;i<ir_packet.len;i++)
        {
            Red_Data[i*2]=ir_packet.ir_hl[i]>>8;
            Red_Data[i*2+1]=ir_packet.ir_hl[i];       
        }
        crc=Ar_crc_16(Red_Data,(ir_packet.len)*2+2);
        Red_Data[(ir_packet.len)*2]=crc>>8;
        Red_Data[(ir_packet.len)*2+1]=crc;
        UartSend_String(Red_Data,(ir_packet.len)*2+2);
        Clear_Buff(Red_Data,sizeof(Red_Data));
        for(i=0;i<80;i++)
            ir_packet.ir_hl[i]=0;
    }
}
//串口接收的数据，uint8转换成uint16，减去两个检验位
void Uart_Data_Vul()
{
    uint8_t i=0;
    if((Uart_Sta&0x80) == 0x80)
    {
        if(Uart_Rec[0]==0xaa&&Uart_Rec[1]==0x55)
        {
            switch(Uart_Rec[2])
            {
                case 0x00:
                    Study_Data_Vul();
                    break;
                case 0x01:
                    Key_Num=Uart_Rec[3];
                    User_Len=Uart_Rec[4];
                    for(i=0;i<User_Len;i++)
                    {
                        Red_Data[i]=Uart_Rec[i+5];
                    }
                    if(Ar_crc_16(Red_Data,User_Len))
                    {
                        ir_packet.len=(User_Len-2)/2;//减去校验位
                        for(i=0;i<ir_packet.len;i++)
                        {
                            ir_packet.ir_hl[i]=(Red_Data[i*2]<<8)+Red_Data[i*2+1];
                        }
                        IR_Output_Send(CNT38K);
                        Delayms(71);
                    }

                    break;
                default :break;
            }
        }
        Uart_Sta=0;
        Data_Len=0;
        Clear_Buff(Uart_Rec,sizeof(Uart_Rec));
    }

}
/**************************************************
 函 数 名  : main
 输入参数  : NONE  
 输出参数  : NONE
 返 回 值  : NONE
***************************************************/
void main(void)
{
    
    SetSysClock();
    InitUART();
    
    T1CTL &= ~0x03; //暂停Timer
    IR_Study_Init_Led();
    
    while(1)
    { 
        Uart_Data_Vul();
    }
}



/*********************************************END OF FILE**********************/