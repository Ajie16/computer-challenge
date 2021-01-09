///////////////////////////////////////////////////////////////////////////////
//矩阵按键驱动  
////////////////////////////////////////////////////////////////////////////////

#include "stm32f10x.h"
#include "key.h"
#include "led.h"
#include "sys.h" 
#include "delay.h"

u16 keyz=0;

void KEY_Init(void) 
{ 
 	GPIO_InitTypeDef GPIO_InitStructure;
 	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;//KEY0-KEY3 矩阵键盘的行
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;   //
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;//KEY4-KEY7 矩阵键盘的列
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;      //
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
 	GPIO_Init(GPIOC, &GPIO_InitStructure);	
	
	//GPIO_SetBits(GPIOC,GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7); 
	

}



u8 keyscan(void)
{ 
	uint8_t LIE,HANG,k,i=0;
	
	GPIO_Write(GPIOC, 0xF0);                            //D0-D3拉低,D4-D7拉高
	if((GPIO_ReadInputData(GPIOC)&0xF0)!=0xF0)          //有按键按下
	{
	  delay_ms(20);                                     //去抖
	   if((GPIO_ReadInputData(GPIOC)&0xF0)!=0xF0)       //再次判断是否按下
	   {
		   LIE=GPIO_ReadInputData(GPIOC);                 //读取按键按下后得到的代码
		   HANG=LIE;                                      //将代码复制给行
		   LIE=~LIE;                                      //将键码取反，例如：按下某个键得到0111 0000，取反后得到1000 1111
		   LIE=LIE&0XF0;                                  //得到列1000 1111&1111 0000得到1000 0000,得到列数
		   
			 for(i=0;i<4&&((HANG&0xF0)!=0xF0);i++)          //逐次将行拉高，判断列数中原来变低的位是否变高
		   {                                              //读到之前检测到为低的列变高则推出
		       GPIO_Write(GPIOC, (HANG&0xF0)|(0x01<<i));  //进行行扫描，逐次将行口线拉高，列保持为按下的状态
				 
		       HANG=GPIO_ReadInputData(GPIOC);            //读取IO口，用以判断是否扫描到行坐标		
					
		   }
		   HANG&=0x0F;                                    //将行值取出
		   k=LIE|HANG;                                    //行列相加则得到键码
			 
			 
			 
		   GPIO_Write(GPIOC, 0xF0);                       //D0-D3拉低,D4-D7拉高,此处用来将行状态初始化为未按下时的状态
	     while((GPIO_ReadInputData(GPIOC)&0xF0)!=0xF0)  //判释放
		   {
		        delay_ms(20);                             //后延消抖。时间需要长一点
		   }
		   return k;                                      //返回键码
	   }
	}	
	return (0);                                         //无键按下，返回0
}


 u8 keyhandle(void)
{ 
	uint8_t key_1=0;
	
	keyz=keyscan();
	if(keyz!=0)
		{ 
			switch(keyz)
			{
				case 0x11: key_1=1;break; 
				case 0x21: key_1=5;break; 
				case 0x41: key_1=9;break; 
				case 0x81: key_1=13;break; 
				case 0x12: key_1=2;break; 
				case 0x22: key_1=6;break; 
				case 0x42: key_1=10;break; 
				case 0x82: key_1=14;break;
				case 0x14: key_1=3;break;
				case 0x24: key_1=7;break;
        case 0x44: key_1=11;break;	
		    case 0x84: key_1=15;break;				
				case 0x18: key_1=4;break;
				case 0x28: key_1=8;break;
        case 0x48: key_1=12;break;	
		    case 0x88: key_1=16;break;		
				default:break;
			}
		
	}
	return key_1;
	
}




