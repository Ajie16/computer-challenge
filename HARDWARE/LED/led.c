#include "led.h"

//////////////////////////////////////////////////////////////////////////////////	 

//LED��������	   
								  
////////////////////////////////////////////////////////////////////////////////// 

//��ʼ��PA15Ϊ�����.��ʹ������ڵ�ʱ��	  ��ɫ����LED   
//LED IO��ʼ��
void LED_Init(void)
{
	 GPIO_InitTypeDef  GPIO_InitStructure; 
		
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);	
	//�����
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7; 
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	 GPIO_Init(GPIOB, &GPIO_InitStructure);//��ʼ��GPIO
  //���
	 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;				 //BEEP-->PA15�˿�����
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //�ٶ�Ϊ50MHz
	 GPIO_Init(GPIOA, &GPIO_InitStructure);	 //���ݲ�����ʼ��GPIOA.15
		
	 GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable,ENABLE);//����JTAG�ڵ��ԣ�PA15��Ϊ��ͨIO
	 GPIO_SetBits(GPIOA,GPIO_Pin_15);//���1
}

//��ʼ��PD7Ϊ�����.��ʹ������ڵ�ʱ��		    
//�̵�����ʼ��
void RELAY_Init(void)
{
	 
 GPIO_InitTypeDef  GPIO_InitStructure; 
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOD, ENABLE);	 //ʹ��GPIOD�˿�ʱ��
 
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;				 //PD7�˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;	 //�ٶ�Ϊ50MHz
 GPIO_Init(GPIOD, &GPIO_InitStructure);	 //���ݲ�����ʼ��GPIOD.7
	

 
 GPIO_SetBits(GPIOD,GPIO_Pin_7);//���1���رռ̵������

}

