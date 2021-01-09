#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"




#define  PortD   (GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7)
#define KEY0  GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_0)//
#define KEY1  GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_1)//
#define KEY2  GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_2)//
#define KEY3  GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_3)//
#define KEY4  GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_4)//
#define KEY5  GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_5)//
#define KEY6  GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_6)//
#define KEY7  GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_7)//????1
//#define KEY8  GPIO_ReadInputDataBit(GPIOD,GPIO_Pin_4)//????1

 

#define KEY0_PRES 1	//KEY0??
#define KEY1_PRES	2	//KEY1??
#define KEY2_PRES	3	//KEY2??
#define KEY3_PRES 4	//KEY3??
#define KEY4_PRES	5	//KEY4??
#define KEY5_PRES	6	//KEY5??
#define KEY6_PRES 7	//KEY6??
#define KEY7_PRES	8	//KEY7??


void KEY_Init(void);
u8 keyscan(void);
 u8 keyhandle(void);


#endif

