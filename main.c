
#include <ioCC2530.h>
#include "Uart.h"
#include <stdio.h>
#include"IR.H"
#include "type.h"

uint8_t Txdata[]={"The test is VS1838.\n"};
uint8_t Red_Data[80];

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
    //os_printf("crc_in=0x%x\r\n",crc_in);// 测试代码，打印crc_in
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
	//os_printf("crc_re=0x%x\r\n",crc_re);// 测试代码，打印crc校验字节
	if((crc_in == crc_re)||(crc_in == 0)){
		return crc_re;
	}
	else{
		return 0;
	}
}


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
        
        Send_char(0x0d);
        Send_char(0x0a);
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
   Study_Data_Vul();
    while(1)
    { 
      
      IR_Output_Send(CNT38K);
      Delayms(71);
      
    }
}



/*********************************************END OF FILE**********************/