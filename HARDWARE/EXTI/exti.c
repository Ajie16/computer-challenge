#include "exti.h"
#include "delay.h"
#include "os.h"

//////////////////////////////////////////////////////////////////////////////////	 

//外部中断 驱动代码	     PB13!!
								  
//////////////////////////////////////////////////////////////////////////////////   
//外部中断0服务程序

u16 count = 0; //码盘格子计数 码盘一圈20格



void EXTIX_Init(void)
{
 
		EXTI_InitTypeDef EXTI_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;


  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//使能复用功能时钟

    
   //PB13	  中断线以及中断初始化配置 下降沿触发 PB13  //红外对管对应PB13
 	 GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource13); 

  	EXTI_InitStructure.EXTI_Line=EXTI_Line13;
	  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  // 下降沿
  	EXTI_Init(&EXTI_InitStructure);		//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器


  	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;			//使能红外对管 所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2， 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;					//子优先级3
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure); 

   
 
}

//外部中断13服务程序 
void EXTI15_10_IRQHandler(void)
{

	OSIntEnter();
	if(EXTI_GetITStatus(EXTI_Line13)==SET)
	{
		count ++ ;	
		EXTI_ClearITPendingBit(EXTI_Line13); //清除LINE13 上的中断标志位  
	}
	OSIntExit();
	
}
 

