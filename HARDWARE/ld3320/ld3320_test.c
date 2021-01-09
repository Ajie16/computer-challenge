#include "ld3320_test.h"
#include "stm32f10x.h"
#include <stdio.h>
#include "ld3320.h"
#include "lcd.h"
#include "text.h"
#include "led.h"
#include "string.h"
#include "usart.h"
uint8_t   direct=0,process=0,flag=0; /*process语音识别模块工作状态,flag中断标志位,direct语音识别指令信息*/



/**
  * @brief  LD3320测试
  * @param  无
  * @retval 无
  */
void   LD3320_Task(void)
{
		uint8_t  i=0;	
		uint8_t  LD_ASR_Candi_Num; //从中断代码拿过来的
  
   LD_WriteReg(0x29,0) ;   /*关闭中断*/
   LD_WriteReg(0x02,0) ;   
	
  	
   //1 空闲状态；2 识别成功状态；3 识别失败状态。
 
  while(1)
  {  
	  
    switch(process)
    { 
      case  0:         /*ld3320处于空闲状态*/
      i++;
      process=LD_Run();  
      if(i==4)
      {
        return;
      }
      break;

      case  1:          /*ld3320语音准备OK*/
	  
	  
      if(flag)          /*中断判断*/  //判断识别是否成功
      {          
        switch(direct)  /*对结果执行相关操作*/         
        {
          case 1:      
							
          break;
          case 2:	     
						Light=1;
							//灯和继电器刷新
						SYN_Play(0,"灯已开");
						LCD_ShowNum(170+50,50,Light,2,16);
          break;

          case 3:	     
						Light=0;
						SYN_Play(0,"灯已关");	
						LCD_ShowNum(170+50,50,Light,2,16);
          break;

          case 4:		  
						RELAY=1;
						SYN_Play(0,"继电器已开");
						LCD_ShowNum(240+50,50,RELAY,2,16);
          break;
          case 5:		 
						RELAY=0;
						SYN_Play(0,"继电器已关");
						LCD_ShowNum(240+50,50,RELAY,2,16);
          break;
					
          default:     
          break;
        } 
        process=flag=0;        
      }
      break;
      
      case  2:         /*ld3320语音识别失败*/
      i=0;
      if(flag)         /*中断判断*/
      {
        process=flag=0;            
      } 
      break;      
                     
      default:
      break;  
    }
	
	
	  if((process==1)&&(LD_ReadReg(0xB2)==0x21) && (LD_ReadReg(0xbf)==0x35))	
	  {
		  
		  flag=1;     //！！！！！！
		  
				LD_ASR_Candi_Num = LD_ReadReg(0xba); 
				if(LD_ASR_Candi_Num>0 && LD_ASR_Candi_Num<=4) 
				{ 
					direct = LD_ReadReg(0xc5);					
					process=1;    //识别成功    
				}else
				process=2;  //识别失败
	  
	  /*完成一次语音识别后重新配置寄存器*/
		LD_WriteReg(0x2b,0);
		LD_WriteReg(0x1C,0);//写0:ADC不可用
		LD_WriteReg(0x29,0);
		LD_WriteReg(0x02,0);
		LD_WriteReg(0x2B,0);
		LD_WriteReg(0xBA,0);	
		LD_WriteReg(0xBC,0);	
		LD_WriteReg(0x08,1);//清除FIFO_DATA
		LD_WriteReg(0x08,0);//清除FIFO_DATA后 再次写0   	
	  }

    LD_Test();
		delay_ms(100);
   }
}
   


void  LD_Test(void)
{
  switch(direct)  //对结果执行相关操作
	{
		case 1:  //命令“流水灯” 
		break;
    
		case 2:	  //命令“闪烁”
		break;
    
		case 3:	//命令“按键触发”  
		break;
   
		case 4:		//命令“全灭”
		break;
	  
		default:
    break;
	}	
}





/**************芯片设置命令*********************/
uint8_t SYN_StopCom[]={0xFD,0X00,0X02,0X02,0XFD};//停止合成
uint8_t SYN_SuspendCom[]={0XFD,0X00,0X02,0X03,0XFC};//暂停合成
uint8_t SYN_RecoverCom[]={0XFD,0X00,0X02,0X04,0XFB};//恢复合成
uint8_t SYN_ChackCom[]={0XFD,0X00,0X02,0X21,0XDE};//状态查询
uint8_t SYN_PowerDownCom[]={0XFD,0X00,0X02,0X88,0X77};//进入POWER DOWN 状态命令
uint8_t SYN_PowerSpeed[]={0xFD,0x00,0x07,0x01,0x01,0x5B,0x74,0x35,0x5D,0xBD};//语速命令


/************************************************************************
功能描述： 串口发送字符串数据
入口参数： 	*DAT：字符串指针
返 回 值： none
其他说明： API 供外部使用，直观！
**************************************************************************/
void PrintCom(uint8_t *DAT,uint8_t len)
{
	uint8_t i;
	for(i=0;i<len;i++)
	{
		while((USART1->SR & 0X40) == 0); //循环发送,直到发送完毕
		USART1->DR = (u8)(*DAT++);
	}	
}

/***********************************************************
* 名    称：  YS-SYN6288 文本合成函数
* 功    能：  发送合成文本到SYN6288芯片进行合成播放
* 入口参数：Music(背景音乐选择):0无背景音乐。1-15：相关背景音乐
            *HZdata:文本指针变量 
* 出口参数：
* 说    明： 本函数只用于文本合成，具备背景音乐选择。默认波特率9600bps。					 
* 调用方法：例： SYN_FrameInfo（0，“乐声电子科技”）；
**********************************************************/
void SYN_Play(uint8_t Music,const char *HZdata)
{
/****************需要发送的文本**********************************/ 
		 unsigned  char  Frame_Info[50];
         unsigned  char  HZ_Length;  
		 unsigned  char  ecc  = 0;  			//定义校验字节
	     unsigned  int i=0; 
		 HZ_Length =strlen(HZdata); 			//需要发送文本的长度
 
/*****************帧固定配置信息**************************************/           
		 Frame_Info[0] = 0xFD ; 			//构造帧头FD
		 Frame_Info[1] = 0x00 ; 			//构造数据区长度的高字节
		 Frame_Info[2] = HZ_Length + 3; 		//构造数据区长度的低字节
		 Frame_Info[3] = 0x01 ; 			//构造命令字：合成播放命令		 		 
		 Frame_Info[4] = 0x01 | Music<<4 ;  //构造命令参数：背景音乐设定

/*******************校验码计算***************************************/		 
		 for(i = 0; i<5; i++)   				//依次发送构造好的5个帧头字节
	     {  
	         ecc=ecc^(Frame_Info[i]);		//对发送的字节进行异或校验	
	     }

	   	 for(i= 0; i<HZ_Length; i++)   		//依次发送待合成的文本数据
	     {  
	         ecc=ecc^(HZdata[i]); 				//对发送的字节进行异或校验		
	     }		 
/*******************发送帧信息***************************************/		  
		  memcpy(&Frame_Info[5], HZdata, HZ_Length);
		  Frame_Info[5+HZ_Length]=ecc;
		  PrintCom(Frame_Info,5+HZ_Length+1);
}

/***********************************************************
* 名    称： void YS_SYN_Set(uint8_t *Info_data)
* 功    能： 配置SYN6288
* 入口参数： *Info_data:固定的配置信息变量 
* 出口参数：
* 说    明：本函数用于配置，停止合成、暂停合成等设置 ，默认波特率9600bps。					 
* 调用方法：通过调用已经定义的相关数组进行配置。 
**********************************************************/
void YS_SYN_Set(uint8_t *Info_data)
{
	uint8_t Com_Len;
	Com_Len =strlen((char *)Info_data);
	PrintCom(Info_data,Com_Len);
}


void YS_Set_Speed(u8 speed)
{
	uint8_t Com_Len;
	SYN_PowerSpeed[7]= speed%5 +0x31;
	Com_Len =strlen((char *)SYN_PowerSpeed);
	PrintCom(SYN_PowerSpeed,Com_Len);
}





