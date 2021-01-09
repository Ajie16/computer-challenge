#include "exti.h"
#include "delay.h"
#include "os.h"

//////////////////////////////////////////////////////////////////////////////////	 

//�ⲿ�ж� ��������	     PB13!!
								  
//////////////////////////////////////////////////////////////////////////////////   
//�ⲿ�ж�0�������

u16 count = 0; //���̸��Ӽ��� ����һȦ20��



void EXTIX_Init(void)
{
 
		EXTI_InitTypeDef EXTI_InitStructure;
		NVIC_InitTypeDef NVIC_InitStructure;


  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//ʹ�ܸ��ù���ʱ��

    
   //PB13	  �ж����Լ��жϳ�ʼ������ �½��ش��� PB13  //����Թܶ�ӦPB13
 	 GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource13); 

  	EXTI_InitStructure.EXTI_Line=EXTI_Line13;
	  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;  // �½���
  	EXTI_Init(&EXTI_InitStructure);		//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���


  	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;			//ʹ�ܺ���Թ� ���ڵ��ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2�� 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;					//�����ȼ�3
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure); 

   
 
}

//�ⲿ�ж�13������� 
void EXTI15_10_IRQHandler(void)
{

	OSIntEnter();
	if(EXTI_GetITStatus(EXTI_Line13)==SET)
	{
		count ++ ;	
		EXTI_ClearITPendingBit(EXTI_Line13); //���LINE13 �ϵ��жϱ�־λ  
	}
	OSIntExit();
	
}
 

