#ifndef __MOTOR_H
#define __MOTOR_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 

//步进电机驱动代码	   
									  
////////////////////////////////////////////////////////////////////////////////// 


enum dir{Pos,Neg};



//IN1-PG6
#define IN1_GPIO_PORT     GPIOG
#define IN1_GPIO_CLK      RCC_APB2Periph_GPIOG
#define IN1_GPIO_PIN      GPIO_Pin_6

//IN2-PG7
#define IN2_GPIO_PORT     GPIOG
#define IN2_GPIO_CLK      RCC_APB2Periph_GPIOG
#define IN2_GPIO_PIN      GPIO_Pin_7

//IN3-PG8
#define IN3_GPIO_PORT     GPIOG
#define IN3_GPIO_CLK      RCC_APB2Periph_GPIOG
#define IN3_GPIO_PIN      GPIO_Pin_8

//IN1-PG9
#define IN4_GPIO_PORT     GPIOG
#define IN4_GPIO_CLK      RCC_APB2Periph_GPIOG
#define IN4_GPIO_PIN      GPIO_Pin_9



/* 直接操作寄存器的方法控制IO */
#define	digitalHi(p,i)		 {p->BSRR=i;}	 //输出为高电平		
#define digitalLo(p,i)		 {p->BRR=i;}	 //输出低电平

/* 定义控制IO的宏 */
#define IN1_HIGH        digitalHi(IN1_GPIO_PORT,IN1_GPIO_PIN)
#define IN1_LOW         digitalLo(IN1_GPIO_PORT,IN1_GPIO_PIN)

#define IN2_HIGH        digitalHi(IN2_GPIO_PORT,IN2_GPIO_PIN)
#define IN2_LOW         digitalLo(IN2_GPIO_PORT,IN2_GPIO_PIN)

#define IN3_HIGH        digitalHi(IN3_GPIO_PORT,IN3_GPIO_PIN)
#define IN3_LOW         digitalLo(IN3_GPIO_PORT,IN3_GPIO_PIN)

#define IN4_HIGH        digitalHi(IN4_GPIO_PORT,IN4_GPIO_PIN)
#define IN4_LOW         digitalLo(IN4_GPIO_PORT,IN4_GPIO_PIN)






void MOTOR_Init(void);//初始化

void stepper(uint8_t dir,int speed); //驱动函数

		 				    
#endif
