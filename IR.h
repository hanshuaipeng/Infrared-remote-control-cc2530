#ifndef IR_H
#define IR_H
#include <ioCC2530.h>
#include <stdbool.h>

   //�������LED�ƵĶ˿�
#define BLED P1_2

 
#define STUDY_CMD     0x01
#define CONTROL_CMD   0x02
#define ERROR_CMD     0x03

#define IR_OUT P0_1//���ⷢ����������

#define TRUE    1
#define FALSE    0
#define ENABLE    1
#define DISENABLE  0
#define SUCCESS     1
#define ERROR        0
#define TIME_OVERFLOW  2
#define Pro_ERROR       3

/*******************************************************************************
�ز���Ҫ�ļ���ֵ
********************************************************************************/
#define CNT38K			180					//38k waveform counter value, TMR3_CLK/38000/2
#define CNT36K		        185					//36k waveform counter value, TMR3_CLK/36000/3*2

/******************************************************************************
����ӽ��ܺ������ݵ��������Ԥ��ʱ��
********************************************************************************/
//#define IR_Count                 2                                        //65.5ms*IR_Count Ϊ�����������Ԥ����Ҫʱ��

#define Frequency                8//���嶨ʱ����Ƶ��,��ʱ��ʹ��32MƵ��
  
#define MAXPERIOD		(1600/Frequency)				//20K�ز�����������Ϊ40us������200����ֵ50
#define MINPERIOD		(480/Frequency)					//56K�ز�����������Ϊ18us������480����ֵ15us
#define DATATIME		(4800/Frequency)					//�ж��Ƿ������ݵ�ʱ���׼������2400��ԼΪ150us
#define MINNUM			10					//������������ݸ�����С�ڴ����ݸ����ж�Ϊ����


/**********************************************************************************************************************************
��ʱ����ͣ�������
**********************************************************************************************************************************/
#define T3_Start()         ( T3CTL |= 0x10)//�����ز���ʱ��
#define T3_Stop()          ( T3CTL &=~0x10)//�ر��ز���ʱ��
#define T1_Clear()         ( T1CNTL=0,T1CNTH=0)//�����ʱ��1������

/*******************************************************************************
������������ṹ��
********************************************************************************/
typedef struct
{
    unsigned char header[2];//����ͷ��ʾ
    unsigned char cmd;//����01ѧϰ��02���ͣ�03����,04�����;�� 
    unsigned char Datalen;//���ݳ���
    unsigned char type;  //�ڵ�����
    unsigned char hl;  // �ز��ߵ͵�ƽռ��ʱ�䣬��һ�ֽڱ�ʾ�ز��ߵ�ƽʱ�䡢�ڶ��ֽڱ�ʾ�ز��͵�ƽʱ��
    unsigned char len;  //��Ч�������ݸ���
    unsigned int ir_hl[115];//���Ⲩ�ε�ƽʱ��.���ݵ����λ1��ʾ�ߵ�ƽ��0��ʾ�͵�ƽ
    unsigned char crc;//�����ۼӺ�У��
    //unsigned char crc[2];//CRCУ�飬����16λCRC-CCITTУ��  
}IR_T;

extern IR_T __xdata ir_packet;//��ֵ����ͷ

//extern IR_T  ir_packet;//��ֵ����ͷ

extern unsigned int IR_hl[16];//�����ز�Ƶ��

/*******************************************************************************
�����������
********************************************************************************/
//extern bool Start_Flag;//���ڱ�ʶ��ʼ�к������ݽ���

//extern unsigned char Start_Flag;//���ڱ�ʶ��ʼ�к������ݽ���

extern unsigned char End_Flag;//=1;//���ڱ�ʶ�����������ݽ���
extern unsigned int previous;//ǰһ���½���ʱ��
extern unsigned int current;//��ǰ�½���ʱ��
extern unsigned int time_fall;   //�ߵ�ƽ��ʱ��
extern unsigned int time_X;   // ����ʱ���

extern unsigned char IR_Study;//��ʾ����ѧϰ�׶�


//extern unsigned char UP_DOWN;   //�����ƽ�ߵͱ�ʶ
extern unsigned char Cap_H;//���񵽵ĸ��ֽ�
extern unsigned char Cap_L;//���񵽵ĵ��ֽ�
extern unsigned char IR_Study_Num;//��ʾ����ѧϰʱ�����
extern unsigned char IR_Study_Flag;//��ʾ
extern unsigned char IR_Study_OVNum;//��ʾ����ѧϰ�ڼ��������

/*******************************************************************************
���ⷢ����Ҫ����
********************************************************************************/
//extern unsigned int time_L;//��־�ز��͵�ƽ
//extern unsigned int time_H;//��־�ز��ߵ�ƽ
//extern unsigned char HL_Flag;//��־�ز���ƽת��
extern unsigned int time_Tmp;//�����ƽʱ��

extern unsigned char signle;//�������;�ݸ���

/*******************************************************************************
����ִ�����躯�����亯��˵��
********************************************************************************/
//������ʱ����
extern void Delayms(unsigned int xms);  //i=xms ����ʱi���� (16M����ʱ���Լ����32M��Ҫ�޸ģ�ϵͳ���޸�Ĭ��ʹ���ڲ�16M)


//V2.0�汾���͹��̺���

extern unsigned char IR_Output_Send(unsigned char num_carrier);//���ͽ׶�
extern void IR_Output_Init(unsigned char num);//���ͽ׶γ�ʼ��
extern void IR_Output_Init_T1();//���������¼���ģʽ���ڲ�����������
extern void IR_Output_Init_T3(unsigned char carrier);//���ڲ���38K�ز�P17
extern void IR_Output_Init_Var();//���ͽ׶γ�ʼ������

//V2.0�汾ѧϰ���̺���
extern unsigned char IR_Study_Pro(void);//ѧϰ����
extern void IR_Study_Init();//ѧϰ�׶γ�ʼ���������
extern void IR_Study_Init_T1();//ѧϰ�׶γ�ʼ����ʱ��1�����ڲ���
extern void IR_Study_Init_Var();//ѧϰ�׶γ�ʼ������
extern void IR_Study_Init_Led();//ѧϰ�׶γ�ʼ��ָʾ��
extern void IR_Study_HL();//ѧϰ�ز���ƽ

//�жϴ�����
extern __interrupt void T1_IRQ(void);//��ʱ��T1�жϺ���
extern __interrupt void T3_ISR(void);//��ʱ��T3�жϺ���

#endif

