#ifndef __LED_H
#define __LED_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 

//LED��������	   
									  
////////////////////////////////////////////////////////////////////////////////// 
#define LED0 PAout(15)
#define Light PBout(7)
#define RELAY PDout(7)	//  �̵����ӿ�		

void LED_Init(void);//��ʼ��
//��ʼ��PD7Ϊ�����.��ʹ������ڵ�ʱ��		    
//�̵�����ʼ��
void RELAY_Init(void); 				    
#endif
