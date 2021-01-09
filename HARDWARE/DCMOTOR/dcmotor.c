#include "dcmotor.h"
//////////////////////////////////////////////////////////////////////////////////	 

//直流电机驱动代码
								  
////////////////////////////////////////////////////////////////////////////////// 	   

//初始化PB15和PG11为输出口.并使能这个口的时钟		    
//直流电机初始化
void DCMOTOR_Init(void)
{
	 
 GPIO_InitTypeDef  GPIO_InitStructure; 
	
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOG, ENABLE);	 //使能GPIOG端口时钟
 
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11;				 //PG11 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //速度为50MHz
 GPIO_Init(GPIOG, &GPIO_InitStructure);	 //根据参数初始化PG11
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;				 //PB15 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //速度为50MHz
 GPIO_Init(GPIOB, &GPIO_InitStructure);	 //根据参数初始化PB15

	
 GPIO_SetBits(GPIOB,GPIO_Pin_15);//
 GPIO_ResetBits(GPIOG,GPIO_Pin_11);//

}
