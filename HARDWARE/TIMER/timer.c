#include "timer.h"
#include "led.h"


//TIM3 PWM���ֳ�ʼ�� 
//PWM�����ʼ��
//arr���Զ���װֵ
//psc��ʱ��Ԥ��Ƶ��
void TIM1_PWM_Init(void)
{  
	GPIO_InitTypeDef GPIO_InitStruct;   //����GPIO�ṹ�����
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;   //���嶨ʱ���ṹ�����
	TIM_OCInitTypeDef TIM_OCInitStruct;   //��������ȽϽṹ�����
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_AFIO,ENABLE);   //GPIOA��B������ʱ��ʹ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);   //ʹ��TIM1��ʱ��
	
	
	GPIO_InitStruct.GPIO_Pin=GPIO_Pin_15;   //����GPIO��6��7����
	GPIO_InitStruct.GPIO_Mode=GPIO_Mode_AF_PP;   	  //����GPIOΪ�����������
	GPIO_InitStruct.GPIO_Speed=GPIO_Speed_50MHz;  	  //����GPIO����
	GPIO_Init(GPIOB,&GPIO_InitStruct);   //GPIO��ʼ������
	
	
	TIM_TimeBaseInitStruct.TIM_Period=1999;   	  //�����Զ���װ�ص�����ֵ
	TIM_TimeBaseInitStruct.TIM_Prescaler=719;   	  //����Ԥ��Ƶֵ
	TIM_TimeBaseInitStruct.TIM_CounterMode=TIM_CounterMode_Up; //���ϼ���ģʽ
	TIM_TimeBaseInitStruct.TIM_ClockDivision=TIM_CKD_DIV1;     //����ʱ�ӷָ�
	TIM_TimeBaseInitStruct.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM1,&TIM_TimeBaseInitStruct);   		   //��ʱ����ʼ������
	
	TIM_OCInitStruct.TIM_OCMode=TIM_OCMode_PWM1;   //PWMģʽ1
	TIM_OCInitStruct.TIM_Pulse=0;   						   //��ʼ��ռ�ձ�Ϊ0
	TIM_OCInitStruct.TIM_OCPolarity=TIM_OCPolarity_High;   	   //����Ƚϼ��Ե�
	TIM_OCInitStruct.TIM_OutputState=TIM_OutputState_Enable;   //�Ƚ����ʹ��
	TIM_OC3Init(TIM1,&TIM_OCInitStruct);   					   //�Ƚ������ʼ������
	
	TIM_OC3PreloadConfig(TIM1,TIM_OCPreload_Enable);   //����Ƚ�1Ԥװ�ؼĴ���ʹ��
	
	
	TIM_Cmd(TIM1,ENABLE);   //TIM1ʹ��
	TIM_CtrlPWMOutputs(TIM1, ENABLE);
	

}




