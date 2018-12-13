#ifndef IR_H
#define IR_H
#include <ioCC2530.h>
#include <stdbool.h>

   //定义控制LED灯的端口
#define BLED P1_2

 
#define STUDY_CMD     0x01
#define CONTROL_CMD   0x02
#define ERROR_CMD     0x03

#define IR_OUT P0_1//红外发送所需引脚

#define TRUE    1
#define FALSE    0
#define ENABLE    1
#define DISENABLE  0
#define SUCCESS     1
#define ERROR        0
#define TIME_OVERFLOW  2
#define Pro_ERROR       3

/*******************************************************************************
载波需要的计数值
********************************************************************************/
#define CNT38K			180					//38k waveform counter value, TMR3_CLK/38000/2
#define CNT36K		        185					//36k waveform counter value, TMR3_CLK/36000/3*2

/******************************************************************************
定义从接受红外数据到红外结束预计时间
********************************************************************************/
//#define IR_Count                 2                                        //65.5ms*IR_Count 为红外接收数据预计需要时间

#define Frequency                8//定义定时器分频数,定时器使用32M频率
  
#define MAXPERIOD		(1600/Frequency)				//20K载波的脉冲周期为40us。计数200周期值50
#define MINPERIOD		(480/Frequency)					//56K载波的脉冲周期为18us。计数480周期值15us
#define DATATIME		(4800/Frequency)					//判断是否获得数据的时间标准，计数2400大约为150us
#define MINNUM			10					//定义防干扰数据个数，小于此数据个数判断为干扰


/**********************************************************************************************************************************
定时器启停清除函数
**********************************************************************************************************************************/
#define T3_Start()         ( T3CTL |= 0x10)//开启载波定时器
#define T3_Stop()          ( T3CTL &=~0x10)//关闭载波定时器
#define T1_Clear()         ( T1CNTL=0,T1CNTH=0)//清除定时器1计数器

/*******************************************************************************
声明红外所需结构体
********************************************************************************/
typedef struct
{
    unsigned char header[2];//数据头标示
    unsigned char cmd;//命令01学习，02发送，03错误,04请求发送卷标 
    unsigned char Datalen;//数据长度
    unsigned char type;  //节点类型
    unsigned char hl;  // 载波高低电平占用时间，第一字节标示载波高电平时间、第二字节标示载波低电平时间
    unsigned char len;  //有效红外数据个数
    unsigned int ir_hl[115];//红外波形电平时间.数据的最高位1表示高电平、0表示低电平
    unsigned char crc;//采用累加和校验
    //unsigned char crc[2];//CRC校验，采用16位CRC-CCITT校验  
}IR_T;

extern IR_T __xdata ir_packet;//赋值数据头

//extern IR_T  ir_packet;//赋值数据头

extern unsigned int IR_hl[16];//分析载波频率

/*******************************************************************************
红外接受数据
********************************************************************************/
//extern bool Start_Flag;//用于标识开始有红外数据接受

//extern unsigned char Start_Flag;//用于标识开始有红外数据接受

extern unsigned char End_Flag;//=1;//用于标识结束红外数据接受
extern unsigned int previous;//前一个下降沿时间
extern unsigned int current;//当前下降沿时间
extern unsigned int time_fall;   //高电平总时间
extern unsigned int time_X;   // 两次时间差

extern unsigned char IR_Study;//标示红外学习阶段


//extern unsigned char UP_DOWN;   //捕获电平高低标识
extern unsigned char Cap_H;//捕获到的高字节
extern unsigned char Cap_L;//捕获到的低字节
extern unsigned char IR_Study_Num;//标示红外学习时间计数
extern unsigned char IR_Study_Flag;//标示
extern unsigned char IR_Study_OVNum;//标示红外学习期间溢出次数

/*******************************************************************************
红外发送需要变量
********************************************************************************/
//extern unsigned int time_L;//标志载波低电平
//extern unsigned int time_H;//标志载波高电平
//extern unsigned char HL_Flag;//标志载波电平转换
extern unsigned int time_Tmp;//计算电平时间

extern unsigned char signle;//计算红外;据个数

/*******************************************************************************
红外执行所需函数及其函数说明
********************************************************************************/
//调试延时函数
extern void Delayms(unsigned int xms);  //i=xms 即延时i毫秒 (16M晶振时候大约数，32M需要修改，系统不修改默认使用内部16M)


//V2.0版本发送过程函数

extern unsigned char IR_Output_Send(unsigned char num_carrier);//发送阶段
extern void IR_Output_Init(unsigned char num);//发送阶段初始化
extern void IR_Output_Init_T1();//运行于上下计数模式用于产生调制周期
extern void IR_Output_Init_T3(unsigned char carrier);//用于产生38K载波P17
extern void IR_Output_Init_Var();//发送阶段初始化变量

//V2.0版本学习过程函数
extern unsigned char IR_Study_Pro(void);//学习过程
extern void IR_Study_Init();//学习阶段初始化所需过程
extern void IR_Study_Init_T1();//学习阶段初始化定时器1，用于捕获
extern void IR_Study_Init_Var();//学习阶段初始化变量
extern void IR_Study_Init_Led();//学习阶段初始化指示灯
extern void IR_Study_HL();//学习载波电平

//中断处理函数
extern __interrupt void T1_IRQ(void);//定时器T1中断函数
extern __interrupt void T3_ISR(void);//定时器T3中断函数

#endif

