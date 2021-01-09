#include "ld3320_test.h"
#include "stm32f10x.h"
#include <stdio.h>
#include "ld3320.h"
#include "lcd.h"
#include "text.h"
#include "led.h"
#include "string.h"
#include "usart.h"
uint8_t   direct=0,process=0,flag=0; /*process����ʶ��ģ�鹤��״̬,flag�жϱ�־λ,direct����ʶ��ָ����Ϣ*/



/**
  * @brief  LD3320����
  * @param  ��
  * @retval ��
  */
void   LD3320_Task(void)
{
		uint8_t  i=0;	
		uint8_t  LD_ASR_Candi_Num; //���жϴ����ù�����
  
   LD_WriteReg(0x29,0) ;   /*�ر��ж�*/
   LD_WriteReg(0x02,0) ;   
	
  	
   //1 ����״̬��2 ʶ��ɹ�״̬��3 ʶ��ʧ��״̬��
 
  while(1)
  {  
	  
    switch(process)
    { 
      case  0:         /*ld3320���ڿ���״̬*/
      i++;
      process=LD_Run();  
      if(i==4)
      {
        return;
      }
      break;

      case  1:          /*ld3320����׼��OK*/
	  
	  
      if(flag)          /*�ж��ж�*/  //�ж�ʶ���Ƿ�ɹ�
      {          
        switch(direct)  /*�Խ��ִ����ز���*/         
        {
          case 1:      
							
          break;
          case 2:	     
						Light=1;
							//�ƺͼ̵���ˢ��
						SYN_Play(0,"���ѿ�");
						LCD_ShowNum(170+50,50,Light,2,16);
          break;

          case 3:	     
						Light=0;
						SYN_Play(0,"���ѹ�");	
						LCD_ShowNum(170+50,50,Light,2,16);
          break;

          case 4:		  
						RELAY=1;
						SYN_Play(0,"�̵����ѿ�");
						LCD_ShowNum(240+50,50,RELAY,2,16);
          break;
          case 5:		 
						RELAY=0;
						SYN_Play(0,"�̵����ѹ�");
						LCD_ShowNum(240+50,50,RELAY,2,16);
          break;
					
          default:     
          break;
        } 
        process=flag=0;        
      }
      break;
      
      case  2:         /*ld3320����ʶ��ʧ��*/
      i=0;
      if(flag)         /*�ж��ж�*/
      {
        process=flag=0;            
      } 
      break;      
                     
      default:
      break;  
    }
	
	
	  if((process==1)&&(LD_ReadReg(0xB2)==0x21) && (LD_ReadReg(0xbf)==0x35))	
	  {
		  
		  flag=1;     //������������
		  
				LD_ASR_Candi_Num = LD_ReadReg(0xba); 
				if(LD_ASR_Candi_Num>0 && LD_ASR_Candi_Num<=4) 
				{ 
					direct = LD_ReadReg(0xc5);					
					process=1;    //ʶ��ɹ�    
				}else
				process=2;  //ʶ��ʧ��
	  
	  /*���һ������ʶ����������üĴ���*/
		LD_WriteReg(0x2b,0);
		LD_WriteReg(0x1C,0);//д0:ADC������
		LD_WriteReg(0x29,0);
		LD_WriteReg(0x02,0);
		LD_WriteReg(0x2B,0);
		LD_WriteReg(0xBA,0);	
		LD_WriteReg(0xBC,0);	
		LD_WriteReg(0x08,1);//���FIFO_DATA
		LD_WriteReg(0x08,0);//���FIFO_DATA�� �ٴ�д0   	
	  }

    LD_Test();
		delay_ms(100);
   }
}
   


void  LD_Test(void)
{
  switch(direct)  //�Խ��ִ����ز���
	{
		case 1:  //�����ˮ�ơ� 
		break;
    
		case 2:	  //�����˸��
		break;
    
		case 3:	//�������������  
		break;
   
		case 4:		//���ȫ��
		break;
	  
		default:
    break;
	}	
}





/**************оƬ��������*********************/
uint8_t SYN_StopCom[]={0xFD,0X00,0X02,0X02,0XFD};//ֹͣ�ϳ�
uint8_t SYN_SuspendCom[]={0XFD,0X00,0X02,0X03,0XFC};//��ͣ�ϳ�
uint8_t SYN_RecoverCom[]={0XFD,0X00,0X02,0X04,0XFB};//�ָ��ϳ�
uint8_t SYN_ChackCom[]={0XFD,0X00,0X02,0X21,0XDE};//״̬��ѯ
uint8_t SYN_PowerDownCom[]={0XFD,0X00,0X02,0X88,0X77};//����POWER DOWN ״̬����
uint8_t SYN_PowerSpeed[]={0xFD,0x00,0x07,0x01,0x01,0x5B,0x74,0x35,0x5D,0xBD};//��������


/************************************************************************
���������� ���ڷ����ַ�������
��ڲ����� 	*DAT���ַ���ָ��
�� �� ֵ�� none
����˵���� API ���ⲿʹ�ã�ֱ�ۣ�
**************************************************************************/
void PrintCom(uint8_t *DAT,uint8_t len)
{
	uint8_t i;
	for(i=0;i<len;i++)
	{
		while((USART1->SR & 0X40) == 0); //ѭ������,ֱ���������
		USART1->DR = (u8)(*DAT++);
	}	
}

/***********************************************************
* ��    �ƣ�  YS-SYN6288 �ı��ϳɺ���
* ��    �ܣ�  ���ͺϳ��ı���SYN6288оƬ���кϳɲ���
* ��ڲ�����Music(��������ѡ��):0�ޱ������֡�1-15����ر�������
            *HZdata:�ı�ָ����� 
* ���ڲ�����
* ˵    ���� ������ֻ�����ı��ϳɣ��߱���������ѡ��Ĭ�ϲ�����9600bps��					 
* ���÷��������� SYN_FrameInfo��0�����������ӿƼ�������
**********************************************************/
void SYN_Play(uint8_t Music,const char *HZdata)
{
/****************��Ҫ���͵��ı�**********************************/ 
		 unsigned  char  Frame_Info[50];
         unsigned  char  HZ_Length;  
		 unsigned  char  ecc  = 0;  			//����У���ֽ�
	     unsigned  int i=0; 
		 HZ_Length =strlen(HZdata); 			//��Ҫ�����ı��ĳ���
 
/*****************֡�̶�������Ϣ**************************************/           
		 Frame_Info[0] = 0xFD ; 			//����֡ͷFD
		 Frame_Info[1] = 0x00 ; 			//�������������ȵĸ��ֽ�
		 Frame_Info[2] = HZ_Length + 3; 		//�������������ȵĵ��ֽ�
		 Frame_Info[3] = 0x01 ; 			//���������֣��ϳɲ�������		 		 
		 Frame_Info[4] = 0x01 | Music<<4 ;  //����������������������趨

/*******************У�������***************************************/		 
		 for(i = 0; i<5; i++)   				//���η��͹���õ�5��֡ͷ�ֽ�
	     {  
	         ecc=ecc^(Frame_Info[i]);		//�Է��͵��ֽڽ������У��	
	     }

	   	 for(i= 0; i<HZ_Length; i++)   		//���η��ʹ��ϳɵ��ı�����
	     {  
	         ecc=ecc^(HZdata[i]); 				//�Է��͵��ֽڽ������У��		
	     }		 
/*******************����֡��Ϣ***************************************/		  
		  memcpy(&Frame_Info[5], HZdata, HZ_Length);
		  Frame_Info[5+HZ_Length]=ecc;
		  PrintCom(Frame_Info,5+HZ_Length+1);
}

/***********************************************************
* ��    �ƣ� void YS_SYN_Set(uint8_t *Info_data)
* ��    �ܣ� ����SYN6288
* ��ڲ����� *Info_data:�̶���������Ϣ���� 
* ���ڲ�����
* ˵    �����������������ã�ֹͣ�ϳɡ���ͣ�ϳɵ����� ��Ĭ�ϲ�����9600bps��					 
* ���÷�����ͨ�������Ѿ�������������������á� 
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





