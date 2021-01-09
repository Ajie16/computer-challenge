#include "timer.h"
#include "led.h"


//TIM3 PWM部分初始化 
//PWM输出初始化
//arr：自动重装值
//psc：时钟预分频数
void TIM1_PWM_Init(void)
{  
	GPIO_InitTypeDef GPIO_InitStruct;   //定义GPIO结构体变量
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;   //定义定时器结构体变量
	TIM_OCInitTypeDef TIM_OCInitStruct;   //定义输出比较结构体变量
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);   //GPIOA、B、复用时钟使能
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);   //使能TIM1的时钟
	
	
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_15;   //配置GPIO第6、7引脚
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP;   	  //配置GPIO为复用推挽输出
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;  	  //配置GPIO速率
	GPIO_Init(GPIOB,&GPIO_InitStruct);   //GPIO初始化函数
	
	
	TIM_TimeBaseInitStruct.TIM_Period=1999;   	  //设置自动重装载的周期值
	TIM_TimeBaseInitStruct.TIM_Prescaler=719;   	  //设置预分频值
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up; //向上计数模式
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;     //设置时钟分割
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStruct);   		   //定时器初始化函数
	
	TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM1;   //PWM模式1
	TIM_OCInitStruct.TIM_Pulse=0;   						   //初始化占空比为0
	TIM_OCInitStruct.TIM_OCPolarity=TIM_OCPolarity_High;   	   //输出比较极性低
	TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;   //比较输出使能
	TIM_OC3Init(TIM1,&TIM_OCInitStruct);   					   //比较输出初始化函数
	
	TIM_OC3PreloadConfig(TIM1,TIM_OCPreload_Enable);   //输出比较1预装载寄存器使能
	
	
	TIM_Cmd(TIM1,ENABLE);   //TIM1使能
	TIM_CtrlPWMOutputs(TIM1, ENABLE);
	

}




