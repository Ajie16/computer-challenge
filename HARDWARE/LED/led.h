#ifndef __LED_H
#define __LED_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 

//LED驱动代码	   
									  
////////////////////////////////////////////////////////////////////////////////// 
#define LED0 PAout(15)
#define Light PBout(7)
#define RELAY PDout(7)	//  继电器接口		

void LED_Init(void);//初始化
//初始化PD7为输出口.并使能这个口的时钟		    
//继电器初始化
void RELAY_Init(void); 				    
#endif
