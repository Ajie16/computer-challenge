#include <includes.h>
/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/
OS_Q messg,messg2;  //��Ϣָ��
OS_FLAG_GRP flag_grp1;                   //�����¼���־��
extern u8 recorder_mark,mp3_mark;
u8 recod_mark=1;
u8 run_sta=0;
extern u16 count;
extern u8 mode;
extern char Connect_flag;
/**************************************************************
*                       ����������ƿ�
**************************************************************/
static  OS_TCB   AppTaskStartTCB;
static  OS_TCB   TASK1_TCB;
static  OS_TCB   TASK2_TCB;
static  OS_TCB   TASK3_TCB;
static  OS_TCB   TASK4_TCB;
static  OS_TCB   TASK5_TCB;
static  OS_TCB   TASK6_TCB;
static  OS_TCB   TASK7_TCB;
static  OS_TCB   TASK8_TCB;
static  OS_TCB   TASK9_TCB;
static  OS_TCB   TASK10_TCB;
/**************************************************************
*                      ��������ջ
**************************************************************/
__align(8) static  CPU_STK  AppTaskStartStk[APP_TASK_START_STK_SIZE];
__align(8) static  CPU_STK  Task1_Stk[TASK1_STK_SIZE];
__align(8) static  CPU_STK  Task2_Stk[TASK2_STK_SIZE];
__align(8) static  CPU_STK  Task3_Stk[TASK3_STK_SIZE];
__align(8) static  CPU_STK  Task4_Stk[TASK4_STK_SIZE];
__align(8) static  CPU_STK  Task5_Stk[TASK5_STK_SIZE];
__align(8) static  CPU_STK  Task6_Stk[TASK6_STK_SIZE];
__align(8) static  CPU_STK  Task7_Stk[TASK7_STK_SIZE];
__align(8) static  CPU_STK  Task8_Stk[TASK8_STK_SIZE];
__align(8) static  CPU_STK  Task9_Stk[TASK9_STK_SIZE];
__align(8) static  CPU_STK  Task10_Stk[TASK10_STK_SIZE];
/*************************************************************
*                    ��������
*************************************************************/
static  void  AppTaskStart  (void *p_arg);
static  void  Task1 (void *p_arg);//���ֲ�������
static  void  Task2 (void *p_arg);//������ʾ
static  void  Task3 (void *p_arg);//�������
static  void  Task4 (void *p_arg);//��������
static  void  Task5 (void *p_arg);//�洢����
static  void  Task6 (void *p_arg);//CPU��ȡ
static  void  Task7 (void *p_arg);//��������
static  void  Task8 (void *p_arg);//���Ʒ�������
static  void  Task9 (void *p_arg);//���Ƽ������
static  void  Task10(void *p_arg);//¼������
int  main (void)
{
    OS_ERR  err;

    /* ��ʼ��ϵͳ*/
    OSInit(&err);                                               

    /* �������� */
    OSTaskCreate((OS_TCB     *)&AppTaskStartTCB,                
                 (CPU_CHAR   *)"App Task Start",
                 (OS_TASK_PTR ) AppTaskStart,
                 (void       *) 0,
                 (OS_PRIO     ) APP_TASK_START_PRIO,
                 (CPU_STK    *)&AppTaskStartStk[0],
                 (CPU_STK_SIZE) APP_TASK_START_STK_SIZE / 10,
                 (CPU_STK_SIZE) APP_TASK_START_STK_SIZE,
                 (OS_MSG_QTY  ) 5u,
                 (OS_TICK     ) 0u,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);

    /* ��������*/
    OSStart(&err);                                              
		
		
}


/***********************************************************************
  * @ ������  �� AppTaskStart
  * @ ����˵���� Ϊ�˷���������е����񴴽����������������������
  * @ ����    �� ��  
  * @ ����ֵ  �� ��
  **********************************************************************/

static  void  AppTaskStart (void *p_arg)
{
    CPU_INT32U  cpu_clk_freq;
    CPU_INT32U  cnts;
    OS_ERR      err;
   (void)p_arg;
    CPU_Init();
	
    cpu_clk_freq = BSP_CPU_ClkFreq();                           
    cnts = cpu_clk_freq / (CPU_INT32U)OSCfg_TickRate_Hz;        
    OS_CPU_SysTickInit(cnts);                                   
    /* �ڴ��ʼ��*/
    Mem_Init();                                                
#if OS_CFG_STAT_TASK_EN > 0u
    OSStatTaskCPUUsageInit(&err);                               
#endif
    CPU_IntDisMeasMaxCurReset();
	
			/* ������Ӳ����ʼ�� */
    BSP_Init();  
	
		/* �����¼���־�� flag_grp */
		OSFlagCreate ((OS_FLAG_GRP  *) &flag_grp1,        //ָ���¼���־���ָ��
									(CPU_CHAR     *)"FLAG1",  //�¼���־�������
									(OS_FLAGS      )0,                //�¼���־��ĳ�ʼֵ
									(OS_ERR       *)&err);					  //���ش�������
		     
		/* ������Ϣ���� queue */
    OSQCreate ((OS_Q         *)&messg,            //ָ����Ϣ���е�ָ��
               (CPU_CHAR     *)"DATA to Save",  //���е�����
               (OS_MSG_QTY    )30,                //���ɴ����Ϣ����Ŀ
               (OS_ERR       *)&err);             //���ش�������	
				
	 OSQCreate ((OS_Q         *)&messg2,            //ָ����Ϣ���е�ָ��
               (CPU_CHAR     *)"DATA to esp",  //���е�����
               (OS_MSG_QTY    )30,                //���ɴ����Ϣ����Ŀ
               (OS_ERR       *)&err);             //���ش�������				 
		    /* �������� */
    OSTaskCreate((OS_TCB     *)&TASK1_TCB,                
                 (CPU_CHAR   *)"Task1 MUSIC",
                 (OS_TASK_PTR ) Task1,
                 (void       *) 0,
                 (OS_PRIO     ) TASK1_PRIO,
                 (CPU_STK    *)	&Task1_Stk[0],
                 (CPU_STK_SIZE) TASK1_STK_SIZE / 10,
                 (CPU_STK_SIZE) TASK1_STK_SIZE,
                 (OS_MSG_QTY  ) 5u,
                 (OS_TICK     ) 0u,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);
				    /* �������� */
    OSTaskCreate((OS_TCB     *)&TASK2_TCB,                
                 (CPU_CHAR   *)"Task2 GUI",
                 (OS_TASK_PTR ) Task2,
                 (void       *) 0,
                 (OS_PRIO     ) TASK2_PRIO,
                 (CPU_STK    *)	&Task2_Stk[0],
                 (CPU_STK_SIZE) TASK2_STK_SIZE / 10,
                 (CPU_STK_SIZE) TASK2_STK_SIZE,
                 (OS_MSG_QTY  ) 5u,
                 (OS_TICK     ) 0u,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);
								 					 
		OSTaskCreate((OS_TCB     *)&TASK3_TCB,                
                 (CPU_CHAR   *)"Task3 MOTOR",
                 (OS_TASK_PTR ) Task3,
                 (void       *) 0,
                 (OS_PRIO     ) TASK3_PRIO,
                 (CPU_STK    *)	&Task3_Stk[0],
                 (CPU_STK_SIZE) TASK3_STK_SIZE / 10,
                 (CPU_STK_SIZE) TASK3_STK_SIZE,
                 (OS_MSG_QTY  ) 5u,
                 (OS_TICK     ) 0u,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);		
								 
		OSTaskCreate((OS_TCB     *)&TASK4_TCB,                
                 (CPU_CHAR   *)"Task4 KEY",
                 (OS_TASK_PTR ) Task4,
                 (void       *) 0,
                 (OS_PRIO     ) TASK4_PRIO,
                 (CPU_STK    *)	&Task4_Stk[0],
                 (CPU_STK_SIZE) TASK4_STK_SIZE / 10,
                 (CPU_STK_SIZE) TASK4_STK_SIZE,
                 (OS_MSG_QTY  ) 5u,
                 (OS_TICK     ) 0u,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);
								 
		    /* �������� */
    OSTaskCreate((OS_TCB     *)&TASK5_TCB,                
                 (CPU_CHAR   *)"��������",
                 (OS_TASK_PTR ) Task5,
                 (void       *) 0,
                 (OS_PRIO     ) TASK5_PRIO,
                 (CPU_STK    *)	&Task5_Stk[0],
                 (CPU_STK_SIZE) TASK5_STK_SIZE / 10,
                 (CPU_STK_SIZE) TASK5_STK_SIZE,
                 (OS_MSG_QTY  ) 5u,
                 (OS_TICK     ) 0u,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);	
		    /* �������� */
    OSTaskCreate((OS_TCB     *)&TASK6_TCB,                
                 (CPU_CHAR   *)"����ͳ��",
                 (OS_TASK_PTR ) Task6,
                 (void       *) 0,
                 (OS_PRIO     ) TASK6_PRIO,
                 (CPU_STK    *)	&Task6_Stk[0],
                 (CPU_STK_SIZE) TASK6_STK_SIZE / 10,
                 (CPU_STK_SIZE) TASK6_STK_SIZE,
                 (OS_MSG_QTY  ) 5u,
                 (OS_TICK     ) 0u,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);	
								 
								 
				    /* �������� */
    OSTaskCreate((OS_TCB     *)&TASK7_TCB,                
                 (CPU_CHAR   *)"����ʶ��",
                 (OS_TASK_PTR ) Task7,
                 (void       *) 0,
                 (OS_PRIO     ) TASK7_PRIO,
                 (CPU_STK    *)	&Task7_Stk[0],
                 (CPU_STK_SIZE) TASK7_STK_SIZE / 10,
                 (CPU_STK_SIZE) TASK7_STK_SIZE,
                 (OS_MSG_QTY  ) 5u,
                 (OS_TICK     ) 0u,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);	
								 
								 
						    /* �������� */
    OSTaskCreate((OS_TCB     *)&TASK8_TCB,                
                 (CPU_CHAR   *)"����",
                 (OS_TASK_PTR ) Task8,
                 (void       *) 0,
                 (OS_PRIO     ) TASK8_PRIO,
                 (CPU_STK    *)	&Task8_Stk[0],
                 (CPU_STK_SIZE) TASK8_STK_SIZE / 10,
                 (CPU_STK_SIZE) TASK8_STK_SIZE,
                 (OS_MSG_QTY  ) 5u,
                 (OS_TICK     ) 0u,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);
								 
		OS_TaskSuspend(&TASK8_TCB,&err);
								 
						    /* �������� */
    OSTaskCreate((OS_TCB     *)&TASK9_TCB,                
                 (CPU_CHAR   *)"����",
                 (OS_TASK_PTR ) Task9,
                 (void       *) 0,
                 (OS_PRIO     ) TASK9_PRIO,
                 (CPU_STK    *)	&Task9_Stk[0],
                 (CPU_STK_SIZE) TASK9_STK_SIZE / 10,
                 (CPU_STK_SIZE) TASK9_STK_SIZE,
                 (OS_MSG_QTY  ) 5u,
                 (OS_TICK     ) 0u,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);			

						    /* �������� */
    OSTaskCreate((OS_TCB     *)&TASK10_TCB,                
                 (CPU_CHAR   *)"��ջ���",
                 (OS_TASK_PTR ) Task10,
                 (void       *) 0,
                 (OS_PRIO     ) TASK10_PRIO,
                 (CPU_STK    *)	&Task10_Stk[0],
                 (CPU_STK_SIZE) TASK10_STK_SIZE / 10,
                 (CPU_STK_SIZE) TASK10_STK_SIZE,
                 (OS_MSG_QTY  ) 5u,
                 (OS_TICK     ) 0u,
                 (void       *) 0,
                 (OS_OPT      )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
                 (OS_ERR     *)&err);		
		OSTaskDel(&TASK10_TCB,&err);
								 
		OSTaskDel(&AppTaskStartTCB,&err);
    /* ����ѭ�� */
    while (DEF_TRUE) 
    {    	
			OSTimeDly ( 5000, OS_OPT_TIME_DLY, & err );
    }
		
		
}

///////////////////////////////////���ֲ�������/////////////////////////////////////
static  void  Task1 (void *p_arg)
{
	Show_Str(30,110,200,16,"<<���ֲ�����>>",16,0);
	Show_Str(150,110,40,16,"����",16,0);
	Show_Str(200,110,40,16,"˳��",16,0);	
	Show_Str(270,210,200,16,"���Ҳ�����...",16,0); 	 	 
	VS_Sine_Test();	   
	Show_Str(270,210,200,16,"<<WAV¼����>>",16,0); 
	Show_Str(230+150,210,50,16,"ֹͣ",16,0);
	
	POINT_COLOR=BLUE;
	//mp3
	LCD_DrawRectangle(26,108,30+220,252);
	POINT_COLOR=BLACK;
	//¼����
	LCD_DrawRectangle(170+100-20,208,268+160,210+60);
	
	while(1)
	{
		
		mp3_play();
		VS_Sine_Test();	
		delay_ms(100);	 	 
		
		Show_Str(230+150,210,50,16,"ֹͣ",16,0);
		recoder_play();
		delay_ms(100);
	}
}

//////////////////////////////////2S������ʾ����/////////////////////////////////
static  void  Task2 (void *p_arg)
{
	OS_ERR err;
	u16 adcx;
	u8 dat[10]={0};
	float speed,temp;
	short temperature;  
	RELAY_Init();
	Show_Str(200,30,200,16,"�ɼ���Ϣ",16,0);
	Show_Str(280,30,200,16,"�Ʒ�����:",16,0);
	Show_Str(360,30,48,16,"δ����",16,0);
	
	LCD_ShowString(320,70,200,16,16,"Temp:   . C");	
  LCD_ShowString(170,50,200,16,16,"LIGHT: 1");	
	LCD_ShowString(240,50,200,16,16,"RELAY: 1");
	LCD_ShowString(320,50,200,16,16,"OIL_H:  .  %");
	LCD_ShowString(170,70,200,16,16,"SPEED:  .  KM/H");

	LCD_DrawRectangle(164,28,268+160,108);
	Light=1;
	RELAY=1;
	while(1)
	{
		if(Connect_flag==1)
			Show_Str(360,30,48,16,"������",16,0);
		else
			Show_Str(360,30,48,16,"δ����",16,0);
		//��λ����ʾ��������ʾ��
		adcx=Get_Adc_Average(ADC_Channel_8,10); 
		adcx=adcx-1345;
		temp=(float)adcx*(10000.0/2705);
		LCD_ShowNum(320+50,50,temp/100,2,16);
		LCD_ShowNum(320+50+20,50,(u16)temp%100,2,16);
		
		
		
		//�ٶ���ʾ
		speed=(float)count*0.471238;
		speed=speed*180;
		count=0;
		LCD_ShowNum(170+46,70,speed/100,2,16);
		LCD_ShowNum(170+46+24,70,(u16)speed%100,2,16);
		
		//�¶���ʾ
		temperature=DS18B20_Get_Temp();	
		if(temperature<0)
		{
			LCD_ShowChar(320+40,70,'-',16,0);			//��ʾ����
			temperature=-temperature;					//תΪ����
		}
		else LCD_ShowChar(320+40,70,' ',16,0);			//ȥ������
		LCD_ShowNum(320+40+8,70,temperature/10,2,16);	//��ʾ��������	    
		LCD_ShowNum(320+40+32,70,temperature%10,1,16);	//��ʾС������ 	
		
		dat[0]=((u16)temp)>>0;
		dat[1]=((u16)temp)>>8;
		dat[2]=((u16)speed)>>0;
		dat[3]=((u16)speed)>>8;
		dat[4]=((u16)temperature)>>0;
		dat[5]=((u16)temperature)>>8;
		dat[6]=Light;
		dat[7]=RELAY;
		dat[9]='\0';
		
		OSQPost((OS_Q        *)&messg,                             //��Ϣ����ָ��
					 (void        *)dat,                //Ҫ���͵����ݵ�ָ�룬���ڴ���׵�ַͨ�����С����ͳ�ȥ��
					 (OS_MSG_SIZE  )10,     //�����ֽڴ�С
					 (OS_OPT       )OS_OPT_POST_FIFO | OS_OPT_POST_ALL, //�Ƚ��ȳ��ͷ�����ȫ���������ʽ
					 (OS_ERR      *)&err);	                            //���ش�������
						 
		
		OSQPost((OS_Q        *)&messg2,                             //��Ϣ����ָ��
					 (void        *)dat,                //Ҫ���͵����ݵ�ָ�룬���ڴ���׵�ַͨ�����С����ͳ�ȥ��
					 (OS_MSG_SIZE  )10,     //�����ֽڴ�С
					 (OS_OPT       )OS_OPT_POST_FIFO | OS_OPT_POST_ALL, //�Ƚ��ȳ��ͷ�����ȫ���������ʽ
					 (OS_ERR      *)&err);	                            //���ش�������		
					 
		delay_ms(2000);
						 
	}
}

///////////////////////////////�������/////////////////////////////////////
static  void  Task3 (void *p_arg)
{
	OS_ERR err;
	u8 speed=5,mark=0;
	OS_FLAGS flags_rdy1,flags_rdy2,flags_rdy3;
	Show_Str(170,90,200,16,"���״̬:",16,0);
	Show_Str(170+100,90,200,16,"ֹͣ  ��λ:  0",16,0);
	while(1)
	{
		flags_rdy1 =  OSFlagPend ((OS_FLAG_GRP *)&flag_grp1,                 
                (OS_FLAGS     )( KEY9_EVENT ),
                (OS_TICK      )0,
                (OS_OPT       )OS_OPT_PEND_FLAG_SET_ALL |
		                           OS_OPT_PEND_NON_BLOCKING |
                               OS_OPT_PEND_FLAG_CONSUME,
                (CPU_TS      *)0,
                (OS_ERR      *)&err);
		
		flags_rdy2 =  OSFlagPend ((OS_FLAG_GRP *)&flag_grp1,                 
                (OS_FLAGS     )( KEY10_EVENT ),
                (OS_TICK      )0,
                (OS_OPT       )OS_OPT_PEND_FLAG_SET_ALL |
		                           OS_OPT_PEND_NON_BLOCKING |
                               OS_OPT_PEND_FLAG_CONSUME,
                (CPU_TS      *)0,
                (OS_ERR      *)&err);
		
		flags_rdy3 =  OSFlagPend ((OS_FLAG_GRP *)&flag_grp1,                 
                (OS_FLAGS     )( KEY11_EVENT ),
                (OS_TICK      )0,
                (OS_OPT       )OS_OPT_PEND_FLAG_SET_ALL |
		                           OS_OPT_PEND_NON_BLOCKING |
                               OS_OPT_PEND_FLAG_CONSUME,
                (CPU_TS      *)0,
                (OS_ERR      *)&err);
								
		if((flags_rdy1 & (KEY9_EVENT)) == (KEY9_EVENT))
		{
			Show_Str(170+100,90,200,16,"����  ��λ:    ",16,0);
			speed+=1;
			run_sta=1;//����
			if(speed>5)
				speed=5;
			LCD_ShowNum(170+100+102,90,6-speed,1,16);
		}
		if((flags_rdy2 & (KEY10_EVENT)) == (KEY10_EVENT))
		{
			Show_Str(170+100,90,200,16,"����  ��λ:    ",16,0);
			speed-=1;
			run_sta=2;//����
			if(speed<=2)
				speed=2;
			LCD_ShowNum(170+100+102,90,6-speed,1,16);
		}
		if((flags_rdy3 & (KEY11_EVENT)) == (KEY11_EVENT))
		{
			mark=!mark;
			if(mark==1)
			{
				Show_Str(170+100,90,200,16,"����  ��λ:  1",16,0);
				run_sta=3;
			}
			else
			{
				Show_Str(170+100,90,200,16,"ֹͣ  ��λ:  0",16,0);
				run_sta=0;
			
			}
		}
		//�������
		if(mark==1)
			stepper(Pos,speed);
		else
		{
			IN1_LOW;IN2_LOW;IN3_LOW;IN4_LOW;	
			delay_ms(100);
		}
	}
}
////////////////////////////////��������///////////////////////////////////
static  void  Task4 (void *p_arg)
{
	OS_ERR err;
	u8 key;
	
	while(1)
	{
		key=keyhandle();
		if(key==1)
		{
				OSFlagPost ((OS_FLAG_GRP  *)&flag_grp1,                             //����־���BIT0��1
							(OS_FLAGS      )KEY1_EVENT,
							(OS_OPT        )OS_OPT_POST_FLAG_SET,
							(OS_ERR       *)&err);
		}
		else if(key==2)
		{
				OSFlagPost ((OS_FLAG_GRP  *)&flag_grp1,                             //����־���BIT0��1
							(OS_FLAGS      )KEY2_EVENT,
							(OS_OPT        )OS_OPT_POST_FLAG_SET,
							(OS_ERR       *)&err);
		}
		else if(key==3)
		{
			if(TASK1_TCB.TaskState==OS_TASK_STATE_SUSPENDED)
			{
				VS_Init();	  				//��ʼ��VS1053 	
				delay_ms(100);	
				OSTaskResume(&TASK1_TCB,&err);
				OSTaskSuspend(&TASK7_TCB,&err);
				Show_Str(150,110,40,16,"����",16,0);	
				Show_Str(170+100-20+100,210+60+6,60,16,"�ر�",16,0);
			}
			else
			{
				LD3320_Config();	
				delay_ms(100);
				OSTaskSuspend(&TASK1_TCB,&err);
				OSTaskResume(&TASK7_TCB,&err);
				Show_Str(150,110,40,16,"��ͣ",16,0);	
				Show_Str(170+100-20+100,210+60+6,60,16,"����",16,0);
			}
				
		}
		else if(key==4)
		{
				OSFlagPost ((OS_FLAG_GRP  *)&flag_grp1,                             //����־���BIT0��1
							(OS_FLAGS      )KEY4_EVENT,
							(OS_OPT        )OS_OPT_POST_FLAG_SET,
							(OS_ERR       *)&err);
		}
		else if(key==5)
		{
			OSFlagPost ((OS_FLAG_GRP  *)&flag_grp1,                             //����־���BIT0��1
									(OS_FLAGS      )KEY5_EVENT,
									(OS_OPT        )OS_OPT_POST_FLAG_SET,
									(OS_ERR       *)&err);
		}
		else if(key==6)
		{
			OSFlagPost ((OS_FLAG_GRP  *)&flag_grp1,                             //����־���BIT0��1
									(OS_FLAGS      )KEY6_EVENT,
									(OS_OPT        )OS_OPT_POST_FLAG_SET,
									(OS_ERR       *)&err);
		}
		else if(key==7)
		{
			mode++;
			mode=mode % 3;
			if(mode==0)
				Show_Str(200,110,32,16,"˳��",16,0);
			else if(mode==1)
				Show_Str(200,110,32,16,"����",16,0);
			else if(mode==2)
				Show_Str(200,110,32,16,"���",16,0);
		}
		else if(key==8)
		{
			OSFlagPost ((OS_FLAG_GRP  *)&flag_grp1,                             //����־���BIT0��1
							(OS_FLAGS      )KEY8_EVENT,
							(OS_OPT        )OS_OPT_POST_FLAG_SET,
							(OS_ERR       *)&err);
		}
		else if(key==9)
		{
			OSFlagPost ((OS_FLAG_GRP  *)&flag_grp1,                             //����־���BIT0��1
									(OS_FLAGS      )KEY9_EVENT,
									(OS_OPT        )OS_OPT_POST_FLAG_SET,
									(OS_ERR       *)&err);
		}
		else if(key==10)
		{
			OSFlagPost ((OS_FLAG_GRP  *)&flag_grp1,                             //����־���BIT0��1
									(OS_FLAGS      )KEY10_EVENT,
									(OS_OPT        )OS_OPT_POST_FLAG_SET,
									(OS_ERR       *)&err);
		}
		else if(key==11)
		{
			OSFlagPost ((OS_FLAG_GRP  *)&flag_grp1,                             //����־���BIT0��1
									(OS_FLAGS      )KEY11_EVENT,
									(OS_OPT        )OS_OPT_POST_FLAG_SET,
									(OS_ERR       *)&err);
		}
		else if(key==12)
		{
			//�̵�����ʾ
			RELAY=!RELAY;	
			LCD_ShowNum(240+50,50,RELAY,2,16);
		}
		else if(key==13)
		{
			//����ʾ
			Light=!Light;	

			LCD_ShowNum(170+50,50,Light,2,16);
		}
		else if(key==14)
		{
			OSFlagPost ((OS_FLAG_GRP  *)&flag_grp1,                             //����־���BIT0��1
									(OS_FLAGS      )KEY14_EVENT,
									(OS_OPT        )OS_OPT_POST_FLAG_SET,
									(OS_ERR       *)&err);
		}
		else if(key==15)
		{
			OSFlagPost ((OS_FLAG_GRP  *)&flag_grp1,                             //����־���BIT0��1
									(OS_FLAGS      )KEY15_EVENT,
									(OS_OPT        )OS_OPT_POST_FLAG_SET,
									(OS_ERR       *)&err);
		}
		else if(key==16)
		{
			recod_mark=!recod_mark;
			if(recod_mark == 1)
			{
				recorder_mark=1;
				mp3_mark=0;
				Show_Str(150,110,40,16,"����",16,0);	
				Show_Str(230+150,210,50,16,"ֹͣ",16,0);
				//¼����
				LCD_DrawRectangle(170+100-20,208,268+160,210+60);
				POINT_COLOR=BLUE;
				//mp3
				LCD_DrawRectangle(26,108,30+220,252);
				POINT_COLOR=BLACK;

			}
			else
			{
				recorder_mark=0;
				mp3_mark=1;
				Show_Str(150,110,40,16,"ֹͣ",16,0);	
				Show_Str(230+150,210,50,16,"����",16,0);
				LCD_DrawRectangle(26,108,30+220,252);
				POINT_COLOR=BLUE;
				//¼����
				LCD_DrawRectangle(170+100-20,208,268+160,210+60);
				//mp3
				POINT_COLOR=BLACK;
			}
		}
		delay_ms(100);
	}
}


////////////////////////////////��������///////////////////////////////////
#define SIZE sizeof(TEXT_Buffer)	                
static  void  Task5 (void *p_arg)
{
	OS_ERR err;
	u16 dat_buf;
	//Ҫд�뵽24c02���ַ�������
	u8 *TEXT_Buffer;
	u8 datatemp[10];
	u16 pack_deal[10];
	OS_MSG_SIZE msg_size;
	OS_FLAGS flags_rdy1,flags_rdy2;
	
	Show_Str(170+100,110,120,16,"OIL_H:  .  %   ",16,0);
	Show_Str(170+100,130,120,16,"SPEED:  .  KM/H",16,0);
	Show_Str(170+100,150,120,16,"TEMP :  .  C   ",16,0);
	Show_Str(170+100,170,120,16,"L_STA:         ",16,0);
	Show_Str(170+100,190,120,16,"R_STA:         ",16,0);
	Show_Str(170+100+80,190,64,16,"������Ϣ",16,0);
	LCD_DrawRectangle(170+100-20,108,268+160,208);
	while(1)
	{
		TEXT_Buffer = OSQPend ((OS_Q         *)&messg,                //��Ϣ����ָ��
								(OS_TICK       )0,                     //�ȴ�ʱ��Ϊ����
								(OS_OPT        )OS_OPT_PEND_NON_BLOCKING,  //���û�л�ȡ���ź���
								(OS_MSG_SIZE  *)&msg_size,             //��ȡ��Ϣ���ֽڴ�С
								(CPU_TS       *)0,                     //��ȡ������ʱ��ʱ���
								(OS_ERR       *)&err);                 //���ش���
		if ( err == OS_ERR_NONE )                              //������ճɹ�
		{
			flags_rdy1 =  OSFlagPend ((OS_FLAG_GRP *)&flag_grp1,                 
							(OS_FLAGS     )( KEY6_EVENT ),
							(OS_TICK      )0,
							(OS_OPT       )OS_OPT_PEND_FLAG_SET_ALL |
														 OS_OPT_PEND_NON_BLOCKING |
														 OS_OPT_PEND_FLAG_CONSUME,
							(CPU_TS      *)0,
							(OS_ERR      *)&err);
			
			if((flags_rdy1 & (KEY6_EVENT)) == (KEY6_EVENT))
			{
					AT24CXX_Write(0,(u8 *)TEXT_Buffer,10); //д��
					Show_Str(170+100+102,110,40,16,"д��!",16,0);
			}
		 
			flags_rdy2 =  OSFlagPend ((OS_FLAG_GRP *)&flag_grp1,                 
							(OS_FLAGS     )( KEY5_EVENT ),
							(OS_TICK      )0,
							(OS_OPT       )OS_OPT_PEND_FLAG_SET_ALL |
														 OS_OPT_PEND_NON_BLOCKING |
														 OS_OPT_PEND_FLAG_CONSUME,
							(CPU_TS      *)0,
							(OS_ERR      *)&err);
			
			if((flags_rdy2 & (KEY5_EVENT)) == (KEY5_EVENT))
			{
					
					AT24CXX_Read(0,datatemp,10);  //��ȡ
					Show_Str(170+100+102,110,40,16,"��ȡ!",16,0);
				
					dat_buf=datatemp[0]+(datatemp[1]<<8);
					pack_deal[0]=dat_buf;
				
					dat_buf=datatemp[2]+(datatemp[3]<<8);
					pack_deal[1]=dat_buf;
				
					dat_buf=datatemp[4]+(datatemp[5]<<8);
					pack_deal[2]=dat_buf;
				
					LCD_ShowNum(170+100+48,110,pack_deal[0]/100,2,16);
					LCD_ShowNum(170+100+48+24,110,pack_deal[0]%100,2,16);
				
					LCD_ShowNum(170+100+48,130,pack_deal[1]/100,2,16);
					LCD_ShowNum(170+100+48+24,130,pack_deal[1]%100,2,16);
				
					LCD_ShowNum(170+100+48,150,pack_deal[2]/10,2,16);
					LCD_ShowNum(170+100+48+24,150,pack_deal[2]%10,1,16);
					
					LCD_ShowNum(170+100+48,170,datatemp[6],2,16);
					LCD_ShowNum(170+100+48,190,datatemp[7],2,16);
			}
		}
		delay_ms(300);
	}
}
////////////////////////////////����ʶ��/////////////////////////////////
static  void  Task7 (void *p_arg)
{
	LCD_DrawRectangle(170+100-20,210+60,268+160,210+60+26);
	Show_Str(170+100,210+60+6,100,16,"����ʶ��:",16,0);
	Show_Str(170+100-20+100,210+60+6,60,16,"�ر�",16,0);
	while(1)
	{
		  /*����ʶ��ģ�����*/
		LD3320_Task();
		delay_ms(100);
	}   

}

////////////////////////////////����ͳ��/////////////////////////////////
static  void  Task6 (void *p_arg)
{
	LCD_DrawRectangle(26,252,30+220,210+60+26);
	Show_Str(70,258,200,16,"2020�� 12�� 13��",16,0);
	Show_Str(70,278,200,16,"CPUʹ����:  .  %",16,0);
	while(1)
	{
		LCD_ShowNum(70+80,278,OSStatTaskCPUUsage/100,2,16);
		LCD_ShowNum(70+80+24,278,OSStatTaskCPUUsage%100,2,16);
		delay_ms(300);
	}
}

static  void  Task9 (void *p_arg)//���Ƽ������
{
	OS_ERR err;
	WiFi_ResetIO_Init();            //��ʼ��WiFi�ĸ�λIO
  MQTT_Buff_Init();               //��ʼ������,����,�������ݵ� ������ �Լ���״̬����
	AliIoT_Parameter_Init();	    //��ʼ�����Ӱ�����IoTƽ̨MQTT�������Ĳ���	
	while(1)                        //��ѭ��
	{		
		/*--------------------------------------------------------------------*/
		/*   Connect_flag=1ͬ����������������,���ǿ��Է������ݺͽ���������    */
		/*--------------------------------------------------------------------*/
		if(Connect_flag==1)
		{     
			/*-------------------------------------------------------------*/
			/*                     �����ͻ���������                      */
			/*-------------------------------------------------------------*/
				if(MQTT_TxDataOutPtr != MQTT_TxDataInPtr)
			{                //if�����Ļ���˵�����ͻ�������������
				//3������ɽ���if
				//��1�֣�0x10 ���ӱ���
				//��2�֣�0x82 ���ı��ģ���ConnectPack_flag��λ����ʾ���ӱ��ĳɹ�
				//��3�֣�SubcribePack_flag��λ��˵�����ӺͶ��ľ��ɹ����������Ŀɷ�
				if((MQTT_TxDataOutPtr[2]==0x10)||((MQTT_TxDataOutPtr[2]==0x82)&&(ConnectPack_flag==1))||(SubcribePack_flag==1))
				{    
					printf("��������:0x%x\r\n",MQTT_TxDataOutPtr[2]);  //������ʾ��Ϣ
					MQTT_TxData(MQTT_TxDataOutPtr);                       //��������
					MQTT_TxDataOutPtr += BUFF_UNIT;                       //ָ������
					if(MQTT_TxDataOutPtr==MQTT_TxDataEndPtr)              //���ָ�뵽������β����
						MQTT_TxDataOutPtr = MQTT_TxDataBuf[0];            //ָ���λ����������ͷ
				} 				
			}//�����ͻ��������ݵ�else if��֧��β
			
			/*-------------------------------------------------------------*/
			/*                     ������ջ���������                      */
			/*-------------------------------------------------------------*/
			if(MQTT_RxDataOutPtr != MQTT_RxDataInPtr){  //if�����Ļ���˵�����ջ�������������														
				printf("���յ�����:");
				/*-----------------------------------------------------*/
				/*                    ����CONNACK����                  */
				/*-----------------------------------------------------*/				
				//if�жϣ������һ���ֽ���0x20����ʾ�յ�����CONNACK����
				//��������Ҫ�жϵ�4���ֽڣ�����CONNECT�����Ƿ�ɹ�
				if(MQTT_RxDataOutPtr[2]==0x20){             			
				    switch(MQTT_RxDataOutPtr[5]){					
						case 0x00 : printf("CONNECT���ĳɹ�\r\n");                            //���������Ϣ	
								    ConnectPack_flag = 1;                                        //CONNECT���ĳɹ������ı��Ŀɷ�
									break;                                                       //������֧case 0x00                                              
						case 0x01 : printf("�����Ѿܾ�����֧�ֵ�Э��汾��׼������\r\n");     //���������Ϣ
									Connect_flag = 0;                                            //Connect_flag���㣬��������
									break;                                                       //������֧case 0x01   
						case 0x02 : printf("�����Ѿܾ������ϸ�Ŀͻ��˱�ʶ����׼������\r\n"); //���������Ϣ
									Connect_flag = 0;                                            //Connect_flag���㣬��������
									break;                                                       //������֧case 0x02 
						case 0x03 : printf("�����Ѿܾ�������˲����ã�׼������\r\n");         //���������Ϣ
									Connect_flag = 0;                                            //Connect_flag���㣬��������
									break;                                                       //������֧case 0x03
						case 0x04 : printf("�����Ѿܾ�����Ч���û��������룬׼������\r\n");   //���������Ϣ
									Connect_flag = 0;                                            //Connect_flag���㣬��������						
									break;                                                       //������֧case 0x04
						case 0x05 : printf("�����Ѿܾ���δ��Ȩ��׼������\r\n");               //���������Ϣ
									Connect_flag = 0;                                            //Connect_flag���㣬��������						
									break;                                                       //������֧case 0x05 		
						default   : printf("�����Ѿܾ���δ֪״̬��׼������\r\n");             //���������Ϣ 
									Connect_flag = 0;                                            //Connect_flag���㣬��������					
									break;                                                       //������֧case default 								
					}
				}			
				//if�жϣ���һ���ֽ���0x90����ʾ�յ�����SUBACK����
				//��������Ҫ�ж϶��Ļظ��������ǲ��ǳɹ�
				else if(MQTT_RxDataOutPtr[2]==0x90){ 
						switch(MQTT_RxDataOutPtr[6]){					
						case 0x00 :
						case 0x01 : printf("���ĳɹ�\r\n");            //���������Ϣ
												SubcribePack_flag = 1;                //SubcribePack_flag��1����ʾ���ı��ĳɹ����������Ŀɷ���
												Ping_flag = 0;                        //Ping_flag����
												TIM3_ENABLE_30S();                    //����30s��PING��ʱ��
												OS_TaskResume(&TASK8_TCB,&err);
												break;                                //������֧                                             
						default   : printf("����ʧ�ܣ�׼������\r\n");  //���������Ϣ 
												Connect_flag = 0;                     //Connect_flag���㣬��������
												break;                                //������֧ 								
					}					
				}
				//if�жϣ���һ���ֽ���0xD0����ʾ�յ�����PINGRESP����
				else if(MQTT_RxDataOutPtr[2]==0xD0)
			{ 
					printf("PING���Ļظ�\r\n"); 		  //���������Ϣ 
					if(Ping_flag==1){                     //���Ping_flag=1����ʾ��һ�η���
						 Ping_flag = 0;    				  //Ҫ���Ping_flag��־
					}else if(Ping_flag>1){ 				  //���Ping_flag>1����ʾ�Ƕ�η����ˣ�������2s����Ŀ��ٷ���
						Ping_flag = 0;     				  //Ҫ���Ping_flag��־
						TIM3_ENABLE_30S(); 				  //PING��ʱ���ػ�30s��ʱ��
					}				
				}	
				//if�жϣ������һ���ֽ���0x30����ʾ�յ����Ƿ�������������������
				//����Ҫ��ȡ��������
				else if((MQTT_RxDataOutPtr[2]==0x30))
				{ 
					printf("�������ȼ�0����\r\n"); 		   //���������Ϣ 
					MQTT_DealPushdata_Qs0(MQTT_RxDataOutPtr);  //����ȼ�0��������
				}				
								
				MQTT_RxDataOutPtr += BUFF_UNIT;                     //ָ������
				if(MQTT_RxDataOutPtr==MQTT_RxDataEndPtr)            //���ָ�뵽������β����
					MQTT_RxDataOutPtr = MQTT_RxDataBuf[0];          //ָ���λ����������ͷ                        
			}//������ջ��������ݵ�else if��֧��β

			/*-------------------------------------------------------------*/
			/*                     ���������������                      */
			/*-------------------------------------------------------------*/

			if(MQTT_CMDOutPtr != MQTT_CMDInPtr){                        //if�����Ļ���˵�����������������			       
				printf("����:%s\r\n",&MQTT_CMDOutPtr[2]);                 //���������Ϣ
				
				MQTT_CMDOutPtr += BUFF_UNIT;                             	 //ָ������
				if(MQTT_CMDOutPtr==MQTT_CMDEndPtr)           	             //���ָ�뵽������β����
					MQTT_CMDOutPtr = MQTT_CMDBuf[0];          	             //ָ���λ����������ͷ				
			}//��������������ݵ�else if��֧��β

			
			
		}//Connect_flag=1��if��֧�Ľ�β
		
		/*--------------------------------------------------------------------*/
		/*      Connect_flag=0ͬ�������Ͽ�������,����Ҫ�������ӷ�����         */
		/*--------------------------------------------------------------------*/
		else
		{ 
			printf("��Ҫ���ӷ�����\r\n");                 //���������Ϣ
			TIM_Cmd(TIM4,DISABLE);                           //�ر�TIM4 
			TIM_Cmd(TIM3,DISABLE);                           //�ر�TIM3  
			WiFi_RxCounter=0;                                //WiFi������������������                        
			memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);          //���WiFi���ջ����� 
			if(WiFi_Connect_IoTServer()==0)
			{   			     //���WiFi�����Ʒ�������������0����ʾ��ȷ������if
				printf("����TCP���ӳɹ�\r\n");            //���������Ϣ
				Connect_flag = 1;                            //Connect_flag��1����ʾ���ӳɹ�	
				WiFi_RxCounter=0;                            //WiFi������������������                        
				memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);      //���WiFi���ջ����� 
				MQTT_Buff_ReInit();                          //���³�ʼ�����ͻ�����                    
			}				
		}
		
		delay_ms(100);
	}
}
///////////////////////////////////���Ʒ�������/////////////////////////////////////
static  void  Task8 (void *p_arg)
{
	
  (void)p_arg; 
	while(1)
	{
		OS_ERR err;
		OS_MSG_SIZE msg_size2;
		u8 *TEXT_Buffer2;
		char *dat; 
		u16 data_pack[5];
		
		dat=	mymalloc(SRAMIN,2*1024);

		TEXT_Buffer2 = OSQPend ((OS_Q         *)&messg2,               
									(OS_TICK       )0,                   
									(OS_OPT        )OS_OPT_PEND_BLOCKING,  //���û�л�ȡ���ź����͵ȴ�
									(OS_MSG_SIZE  *)&msg_size2,          
									(CPU_TS       *)0,                     
									(OS_ERR       *)&err); 
		
		data_pack[0]=(TEXT_Buffer2[0]+((TEXT_Buffer2[1])<<8));
		data_pack[1]=(TEXT_Buffer2[2]+((TEXT_Buffer2[3])<<8));
		data_pack[2]=(TEXT_Buffer2[4]+((TEXT_Buffer2[5])<<8));
		sprintf(dat,"{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"state\":%d,\"youwei\":%.2f,\"carInsideTemp\":%.1f,\"speed\":%.2f,\"interiorLightOnOff\":%d,\"nightLight\":%d},\"version\":\"1.0.0\"}",(u16)run_sta,((float)data_pack[0]/100.0),(float)data_pack[2]/10,((float)data_pack[1]/100.0),TEXT_Buffer2[6],TEXT_Buffer2[7]);
		MQTT_PublishQs0(P_TOPIC_NAME,dat,strlen(dat));   //������ݣ�������������	
		
		myfree(SRAMIN,dat);

		delay_ms(2000);
	}
}

static  void  Task10(void *p_arg)//����
{
	 OS_ERR err;  
  CPU_STK_SIZE free,used;  
  (void)p_arg; 
	while(1)
	{
		OSTaskStkChk (&TASK1_TCB,&free,&used,&err);  
    printf("TASK1             used/free:%d/%d  usage:%%%d\r\n",used,free,(used*100)/(used+free));
		
				OSTaskStkChk (&TASK2_TCB,&free,&used,&err);  
    printf("TASK2             used/free:%d/%d  usage:%%%d\r\n",used,free,(used*100)/(used+free));
		
				OSTaskStkChk (&TASK3_TCB,&free,&used,&err);  
    printf("TASK3             used/free:%d/%d  usage:%%%d\r\n",used,free,(used*100)/(used+free));
		
				OSTaskStkChk (&TASK4_TCB,&free,&used,&err);  
    printf("TASK4             used/free:%d/%d  usage:%%%d\r\n",used,free,(used*100)/(used+free));
		
				OSTaskStkChk (&TASK5_TCB,&free,&used,&err);  
    printf("TASK5             used/free:%d/%d  usage:%%%d\r\n",used,free,(used*100)/(used+free));
		
				OSTaskStkChk (&TASK6_TCB,&free,&used,&err);  
    printf("TASK6             used/free:%d/%d  usage:%%%d\r\n",used,free,(used*100)/(used+free));
		
				OSTaskStkChk (&TASK7_TCB,&free,&used,&err);  
    printf("TASK7             used/free:%d/%d  usage:%%%d\r\n",used,free,(used*100)/(used+free));
		
				OSTaskStkChk (&TASK8_TCB,&free,&used,&err);  
    printf("TASK8             used/free:%d/%d  usage:%%%d\r\n",used,free,(used*100)/(used+free));
		
				OSTaskStkChk (&TASK9_TCB,&free,&used,&err);  
    printf("TASK9             used/free:%d/%d  usage:%%%d\r\n",used,free,(used*100)/(used+free));
		
				OSTaskStkChk (&TASK10_TCB,&free,&used,&err);  
    printf("TASK10             used/free:%d/%d  usage:%%%d\r\n",used,free,(used*100)/(used+free));
		delay_ms(1000);
	}
}




