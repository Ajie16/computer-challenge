#include <includes.h>
/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/
OS_Q messg,messg2;  //消息指针
OS_FLAG_GRP flag_grp1;                   //声明事件标志组
extern u8 recorder_mark,mp3_mark;
u8 recod_mark=1;
u8 run_sta=0;
extern u16 count;
extern u8 mode;
extern char Connect_flag;
/**************************************************************
*                       定义任务控制块
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
*                      定义任务栈
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
*                    函数声明
*************************************************************/
static  void  AppTaskStart  (void *p_arg);
static  void  Task1 (void *p_arg);//音乐播放任务
static  void  Task2 (void *p_arg);//数据显示
static  void  Task3 (void *p_arg);//电机任务
static  void  Task4 (void *p_arg);//按键任务
static  void  Task5 (void *p_arg);//存储任务
static  void  Task6 (void *p_arg);//CPU读取
static  void  Task7 (void *p_arg);//语音任务
static  void  Task8 (void *p_arg);//上云发送任务
static  void  Task9 (void *p_arg);//上云检测任务
static  void  Task10(void *p_arg);//录音任务
int  main (void)
{
    OS_ERR  err;

    /* 初始化系统*/
    OSInit(&err);                                               

    /* 创建任务 */
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

    /* 开启调度*/
    OSStart(&err);                                              
		
		
}


/***********************************************************************
  * @ 函数名  ： AppTaskStart
  * @ 功能说明： 为了方便管理，所有的任务创建函数都放在这个函数里面
  * @ 参数    ： 无  
  * @ 返回值  ： 无
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
    /* 内存初始化*/
    Mem_Init();                                                
#if OS_CFG_STAT_TASK_EN > 0u
    OSStatTaskCPUUsageInit(&err);                               
#endif
    CPU_IntDisMeasMaxCurReset();
	
			/* 开发板硬件初始化 */
    BSP_Init();  
	
		/* 创建事件标志组 flag_grp */
		OSFlagCreate ((OS_FLAG_GRP  *) &flag_grp1,        //指向事件标志组的指针
									(CPU_CHAR     *)"FLAG1",  //事件标志组的名字
									(OS_FLAGS      )0,                //事件标志组的初始值
									(OS_ERR       *)&err);					  //返回错误类型
		     
		/* 创建消息队列 queue */
    OSQCreate ((OS_Q         *)&messg,            //指向消息队列的指针
               (CPU_CHAR     *)"DATA to Save",  //队列的名字
               (OS_MSG_QTY    )30,                //最多可存放消息的数目
               (OS_ERR       *)&err);             //返回错误类型	
				
	 OSQCreate ((OS_Q         *)&messg2,            //指向消息队列的指针
               (CPU_CHAR     *)"DATA to esp",  //队列的名字
               (OS_MSG_QTY    )30,                //最多可存放消息的数目
               (OS_ERR       *)&err);             //返回错误类型				 
		    /* 创建任务 */
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
				    /* 创建任务 */
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
								 
		    /* 创建任务 */
    OSTaskCreate((OS_TCB     *)&TASK5_TCB,                
                 (CPU_CHAR   *)"储存任务",
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
		    /* 创建任务 */
    OSTaskCreate((OS_TCB     *)&TASK6_TCB,                
                 (CPU_CHAR   *)"任务统计",
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
								 
								 
				    /* 创建任务 */
    OSTaskCreate((OS_TCB     *)&TASK7_TCB,                
                 (CPU_CHAR   *)"语音识别",
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
								 
								 
						    /* 创建任务 */
    OSTaskCreate((OS_TCB     *)&TASK8_TCB,                
                 (CPU_CHAR   *)"上云",
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
								 
						    /* 创建任务 */
    OSTaskCreate((OS_TCB     *)&TASK9_TCB,                
                 (CPU_CHAR   *)"上云",
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

						    /* 创建任务 */
    OSTaskCreate((OS_TCB     *)&TASK10_TCB,                
                 (CPU_CHAR   *)"堆栈检测",
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
    /* 保护循环 */
    while (DEF_TRUE) 
    {    	
			OSTimeDly ( 5000, OS_OPT_TIME_DLY, & err );
    }
		
		
}

///////////////////////////////////音乐播放任务/////////////////////////////////////
static  void  Task1 (void *p_arg)
{
	Show_Str(30,110,200,16,"<<音乐播放器>>",16,0);
	Show_Str(150,110,40,16,"播放",16,0);
	Show_Str(200,110,40,16,"顺序",16,0);	
	Show_Str(270,210,200,16,"正弦波测试...",16,0); 	 	 
	VS_Sine_Test();	   
	Show_Str(270,210,200,16,"<<WAV录音机>>",16,0); 
	Show_Str(230+150,210,50,16,"停止",16,0);
	
	POINT_COLOR=BLUE;
	//mp3
	LCD_DrawRectangle(26,108,30+220,252);
	POINT_COLOR=BLACK;
	//录音机
	LCD_DrawRectangle(170+100-20,208,268+160,210+60);
	
	while(1)
	{
		
		mp3_play();
		VS_Sine_Test();	
		delay_ms(100);	 	 
		
		Show_Str(230+150,210,50,16,"停止",16,0);
		recoder_play();
		delay_ms(100);
	}
}

//////////////////////////////////2S数据显示任务/////////////////////////////////
static  void  Task2 (void *p_arg)
{
	OS_ERR err;
	u16 adcx;
	u8 dat[10]={0};
	float speed,temp;
	short temperature;  
	RELAY_Init();
	Show_Str(200,30,200,16,"采集信息",16,0);
	Show_Str(280,30,200,16,"云服务器:",16,0);
	Show_Str(360,30,48,16,"未连接",16,0);
	
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
			Show_Str(360,30,48,16,"已连接",16,0);
		else
			Show_Str(360,30,48,16,"未连接",16,0);
		//电位器显示（油门显示）
		adcx=Get_Adc_Average(ADC_Channel_8,10); 
		adcx=adcx-1345;
		temp=(float)adcx*(10000.0/2705);
		LCD_ShowNum(320+50,50,temp/100,2,16);
		LCD_ShowNum(320+50+20,50,(u16)temp%100,2,16);
		
		
		
		//速度显示
		speed=(float)count*0.471238;
		speed=speed*180;
		count=0;
		LCD_ShowNum(170+46,70,speed/100,2,16);
		LCD_ShowNum(170+46+24,70,(u16)speed%100,2,16);
		
		//温度显示
		temperature=DS18B20_Get_Temp();	
		if(temperature<0)
		{
			LCD_ShowChar(320+40,70,'-',16,0);			//显示负号
			temperature=-temperature;					//转为正数
		}
		else LCD_ShowChar(320+40,70,' ',16,0);			//去掉负号
		LCD_ShowNum(320+40+8,70,temperature/10,2,16);	//显示正数部分	    
		LCD_ShowNum(320+40+32,70,temperature%10,1,16);	//显示小数部分 	
		
		dat[0]=((u16)temp)>>0;
		dat[1]=((u16)temp)>>8;
		dat[2]=((u16)speed)>>0;
		dat[3]=((u16)speed)>>8;
		dat[4]=((u16)temperature)>>0;
		dat[5]=((u16)temperature)>>8;
		dat[6]=Light;
		dat[7]=RELAY;
		dat[9]='\0';
		
		OSQPost((OS_Q        *)&messg,                             //消息变量指针
					 (void        *)dat,                //要发送的数据的指针，将内存块首地址通过队列“发送出去”
					 (OS_MSG_SIZE  )10,     //数据字节大小
					 (OS_OPT       )OS_OPT_POST_FIFO | OS_OPT_POST_ALL, //先进先出和发布给全部任务的形式
					 (OS_ERR      *)&err);	                            //返回错误类型
						 
		
		OSQPost((OS_Q        *)&messg2,                             //消息变量指针
					 (void        *)dat,                //要发送的数据的指针，将内存块首地址通过队列“发送出去”
					 (OS_MSG_SIZE  )10,     //数据字节大小
					 (OS_OPT       )OS_OPT_POST_FIFO | OS_OPT_POST_ALL, //先进先出和发布给全部任务的形式
					 (OS_ERR      *)&err);	                            //返回错误类型		
					 
		delay_ms(2000);
						 
	}
}

///////////////////////////////电机任务/////////////////////////////////////
static  void  Task3 (void *p_arg)
{
	OS_ERR err;
	u8 speed=5,mark=0;
	OS_FLAGS flags_rdy1,flags_rdy2,flags_rdy3;
	Show_Str(170,90,200,16,"电机状态:",16,0);
	Show_Str(170+100,90,200,16,"停止  挡位:  0",16,0);
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
			Show_Str(170+100,90,200,16,"减速  挡位:    ",16,0);
			speed+=1;
			run_sta=1;//减速
			if(speed>5)
				speed=5;
			LCD_ShowNum(170+100+102,90,6-speed,1,16);
		}
		if((flags_rdy2 & (KEY10_EVENT)) == (KEY10_EVENT))
		{
			Show_Str(170+100,90,200,16,"加速  挡位:    ",16,0);
			speed-=1;
			run_sta=2;//加速
			if(speed<=2)
				speed=2;
			LCD_ShowNum(170+100+102,90,6-speed,1,16);
		}
		if((flags_rdy3 & (KEY11_EVENT)) == (KEY11_EVENT))
		{
			mark=!mark;
			if(mark==1)
			{
				Show_Str(170+100,90,200,16,"启动  挡位:  1",16,0);
				run_sta=3;
			}
			else
			{
				Show_Str(170+100,90,200,16,"停止  挡位:  0",16,0);
				run_sta=0;
			
			}
		}
		//步进电机
		if(mark==1)
			stepper(Pos,speed);
		else
		{
			IN1_LOW;IN2_LOW;IN3_LOW;IN4_LOW;	
			delay_ms(100);
		}
	}
}
////////////////////////////////按键任务///////////////////////////////////
static  void  Task4 (void *p_arg)
{
	OS_ERR err;
	u8 key;
	
	while(1)
	{
		key=keyhandle();
		if(key==1)
		{
				OSFlagPost ((OS_FLAG_GRP  *)&flag_grp1,                             //将标志组的BIT0置1
							(OS_FLAGS      )KEY1_EVENT,
							(OS_OPT        )OS_OPT_POST_FLAG_SET,
							(OS_ERR       *)&err);
		}
		else if(key==2)
		{
				OSFlagPost ((OS_FLAG_GRP  *)&flag_grp1,                             //将标志组的BIT0置1
							(OS_FLAGS      )KEY2_EVENT,
							(OS_OPT        )OS_OPT_POST_FLAG_SET,
							(OS_ERR       *)&err);
		}
		else if(key==3)
		{
			if(TASK1_TCB.TaskState==OS_TASK_STATE_SUSPENDED)
			{
				VS_Init();	  				//初始化VS1053 	
				delay_ms(100);	
				OSTaskResume(&TASK1_TCB,&err);
				OSTaskSuspend(&TASK7_TCB,&err);
				Show_Str(150,110,40,16,"播放",16,0);	
				Show_Str(170+100-20+100,210+60+6,60,16,"关闭",16,0);
			}
			else
			{
				LD3320_Config();	
				delay_ms(100);
				OSTaskSuspend(&TASK1_TCB,&err);
				OSTaskResume(&TASK7_TCB,&err);
				Show_Str(150,110,40,16,"暂停",16,0);	
				Show_Str(170+100-20+100,210+60+6,60,16,"开启",16,0);
			}
				
		}
		else if(key==4)
		{
				OSFlagPost ((OS_FLAG_GRP  *)&flag_grp1,                             //将标志组的BIT0置1
							(OS_FLAGS      )KEY4_EVENT,
							(OS_OPT        )OS_OPT_POST_FLAG_SET,
							(OS_ERR       *)&err);
		}
		else if(key==5)
		{
			OSFlagPost ((OS_FLAG_GRP  *)&flag_grp1,                             //将标志组的BIT0置1
									(OS_FLAGS      )KEY5_EVENT,
									(OS_OPT        )OS_OPT_POST_FLAG_SET,
									(OS_ERR       *)&err);
		}
		else if(key==6)
		{
			OSFlagPost ((OS_FLAG_GRP  *)&flag_grp1,                             //将标志组的BIT0置1
									(OS_FLAGS      )KEY6_EVENT,
									(OS_OPT        )OS_OPT_POST_FLAG_SET,
									(OS_ERR       *)&err);
		}
		else if(key==7)
		{
			mode++;
			mode=mode % 3;
			if(mode==0)
				Show_Str(200,110,32,16,"顺序",16,0);
			else if(mode==1)
				Show_Str(200,110,32,16,"单曲",16,0);
			else if(mode==2)
				Show_Str(200,110,32,16,"随机",16,0);
		}
		else if(key==8)
		{
			OSFlagPost ((OS_FLAG_GRP  *)&flag_grp1,                             //将标志组的BIT0置1
							(OS_FLAGS      )KEY8_EVENT,
							(OS_OPT        )OS_OPT_POST_FLAG_SET,
							(OS_ERR       *)&err);
		}
		else if(key==9)
		{
			OSFlagPost ((OS_FLAG_GRP  *)&flag_grp1,                             //将标志组的BIT0置1
									(OS_FLAGS      )KEY9_EVENT,
									(OS_OPT        )OS_OPT_POST_FLAG_SET,
									(OS_ERR       *)&err);
		}
		else if(key==10)
		{
			OSFlagPost ((OS_FLAG_GRP  *)&flag_grp1,                             //将标志组的BIT0置1
									(OS_FLAGS      )KEY10_EVENT,
									(OS_OPT        )OS_OPT_POST_FLAG_SET,
									(OS_ERR       *)&err);
		}
		else if(key==11)
		{
			OSFlagPost ((OS_FLAG_GRP  *)&flag_grp1,                             //将标志组的BIT0置1
									(OS_FLAGS      )KEY11_EVENT,
									(OS_OPT        )OS_OPT_POST_FLAG_SET,
									(OS_ERR       *)&err);
		}
		else if(key==12)
		{
			//继电器显示
			RELAY=!RELAY;	
			LCD_ShowNum(240+50,50,RELAY,2,16);
		}
		else if(key==13)
		{
			//灯显示
			Light=!Light;	

			LCD_ShowNum(170+50,50,Light,2,16);
		}
		else if(key==14)
		{
			OSFlagPost ((OS_FLAG_GRP  *)&flag_grp1,                             //将标志组的BIT0置1
									(OS_FLAGS      )KEY14_EVENT,
									(OS_OPT        )OS_OPT_POST_FLAG_SET,
									(OS_ERR       *)&err);
		}
		else if(key==15)
		{
			OSFlagPost ((OS_FLAG_GRP  *)&flag_grp1,                             //将标志组的BIT0置1
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
				Show_Str(150,110,40,16,"播放",16,0);	
				Show_Str(230+150,210,50,16,"停止",16,0);
				//录音机
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
				Show_Str(150,110,40,16,"停止",16,0);	
				Show_Str(230+150,210,50,16,"启动",16,0);
				LCD_DrawRectangle(26,108,30+220,252);
				POINT_COLOR=BLUE;
				//录音机
				LCD_DrawRectangle(170+100-20,208,268+160,210+60);
				//mp3
				POINT_COLOR=BLACK;
			}
		}
		delay_ms(100);
	}
}


////////////////////////////////储存任务///////////////////////////////////
#define SIZE sizeof(TEXT_Buffer)	                
static  void  Task5 (void *p_arg)
{
	OS_ERR err;
	u16 dat_buf;
	//要写入到24c02的字符串数组
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
	Show_Str(170+100+80,190,64,16,"复现信息",16,0);
	LCD_DrawRectangle(170+100-20,108,268+160,208);
	while(1)
	{
		TEXT_Buffer = OSQPend ((OS_Q         *)&messg,                //消息变量指针
								(OS_TICK       )0,                     //等待时长为无限
								(OS_OPT        )OS_OPT_PEND_NON_BLOCKING,  //如果没有获取到信号量
								(OS_MSG_SIZE  *)&msg_size,             //获取消息的字节大小
								(CPU_TS       *)0,                     //获取任务发送时的时间戳
								(OS_ERR       *)&err);                 //返回错误
		if ( err == OS_ERR_NONE )                              //如果接收成功
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
					AT24CXX_Write(0,(u8 *)TEXT_Buffer,10); //写入
					Show_Str(170+100+102,110,40,16,"写入!",16,0);
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
					
					AT24CXX_Read(0,datatemp,10);  //读取
					Show_Str(170+100+102,110,40,16,"读取!",16,0);
				
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
////////////////////////////////语音识别/////////////////////////////////
static  void  Task7 (void *p_arg)
{
	LCD_DrawRectangle(170+100-20,210+60,268+160,210+60+26);
	Show_Str(170+100,210+60+6,100,16,"语音识别:",16,0);
	Show_Str(170+100-20+100,210+60+6,60,16,"关闭",16,0);
	while(1)
	{
		  /*语音识别模块测试*/
		LD3320_Task();
		delay_ms(100);
	}   

}

////////////////////////////////任务统计/////////////////////////////////
static  void  Task6 (void *p_arg)
{
	LCD_DrawRectangle(26,252,30+220,210+60+26);
	Show_Str(70,258,200,16,"2020年 12月 13日",16,0);
	Show_Str(70,278,200,16,"CPU使用率:  .  %",16,0);
	while(1)
	{
		LCD_ShowNum(70+80,278,OSStatTaskCPUUsage/100,2,16);
		LCD_ShowNum(70+80+24,278,OSStatTaskCPUUsage%100,2,16);
		delay_ms(300);
	}
}

static  void  Task9 (void *p_arg)//上云检测任务
{
	OS_ERR err;
	WiFi_ResetIO_Init();            //初始化WiFi的复位IO
  MQTT_Buff_Init();               //初始化接收,发送,命令数据的 缓冲区 以及各状态参数
	AliIoT_Parameter_Init();	    //初始化连接阿里云IoT平台MQTT服务器的参数	
	while(1)                        //主循环
	{		
		/*--------------------------------------------------------------------*/
		/*   Connect_flag=1同服务器建立了连接,我们可以发布数据和接收推送了    */
		/*--------------------------------------------------------------------*/
		if(Connect_flag==1)
		{     
			/*-------------------------------------------------------------*/
			/*                     处理发送缓冲区数据                      */
			/*-------------------------------------------------------------*/
				if(MQTT_TxDataOutPtr != MQTT_TxDataInPtr)
			{                //if成立的话，说明发送缓冲区有数据了
				//3种情况可进入if
				//第1种：0x10 连接报文
				//第2种：0x82 订阅报文，且ConnectPack_flag置位，表示连接报文成功
				//第3种：SubcribePack_flag置位，说明连接和订阅均成功，其他报文可发
				if((MQTT_TxDataOutPtr[2]==0x10)||((MQTT_TxDataOutPtr[2]==0x82)&&(ConnectPack_flag==1))||(SubcribePack_flag==1))
				{    
					printf("发送数据:0x%x\r\n",MQTT_TxDataOutPtr[2]);  //串口提示信息
					MQTT_TxData(MQTT_TxDataOutPtr);                       //发送数据
					MQTT_TxDataOutPtr += BUFF_UNIT;                       //指针下移
					if(MQTT_TxDataOutPtr==MQTT_TxDataEndPtr)              //如果指针到缓冲区尾部了
						MQTT_TxDataOutPtr = MQTT_TxDataBuf[0];            //指针归位到缓冲区开头
				} 				
			}//处理发送缓冲区数据的else if分支结尾
			
			/*-------------------------------------------------------------*/
			/*                     处理接收缓冲区数据                      */
			/*-------------------------------------------------------------*/
			if(MQTT_RxDataOutPtr != MQTT_RxDataInPtr){  //if成立的话，说明接收缓冲区有数据了														
				printf("接收到数据:");
				/*-----------------------------------------------------*/
				/*                    处理CONNACK报文                  */
				/*-----------------------------------------------------*/				
				//if判断，如果第一个字节是0x20，表示收到的是CONNACK报文
				//接着我们要判断第4个字节，看看CONNECT报文是否成功
				if(MQTT_RxDataOutPtr[2]==0x20){             			
				    switch(MQTT_RxDataOutPtr[5]){					
						case 0x00 : printf("CONNECT报文成功\r\n");                            //串口输出信息	
								    ConnectPack_flag = 1;                                        //CONNECT报文成功，订阅报文可发
									break;                                                       //跳出分支case 0x00                                              
						case 0x01 : printf("连接已拒绝，不支持的协议版本，准备重启\r\n");     //串口输出信息
									Connect_flag = 0;                                            //Connect_flag置零，重启连接
									break;                                                       //跳出分支case 0x01   
						case 0x02 : printf("连接已拒绝，不合格的客户端标识符，准备重启\r\n"); //串口输出信息
									Connect_flag = 0;                                            //Connect_flag置零，重启连接
									break;                                                       //跳出分支case 0x02 
						case 0x03 : printf("连接已拒绝，服务端不可用，准备重启\r\n");         //串口输出信息
									Connect_flag = 0;                                            //Connect_flag置零，重启连接
									break;                                                       //跳出分支case 0x03
						case 0x04 : printf("连接已拒绝，无效的用户名或密码，准备重启\r\n");   //串口输出信息
									Connect_flag = 0;                                            //Connect_flag置零，重启连接						
									break;                                                       //跳出分支case 0x04
						case 0x05 : printf("连接已拒绝，未授权，准备重启\r\n");               //串口输出信息
									Connect_flag = 0;                                            //Connect_flag置零，重启连接						
									break;                                                       //跳出分支case 0x05 		
						default   : printf("连接已拒绝，未知状态，准备重启\r\n");             //串口输出信息 
									Connect_flag = 0;                                            //Connect_flag置零，重启连接					
									break;                                                       //跳出分支case default 								
					}
				}			
				//if判断，第一个字节是0x90，表示收到的是SUBACK报文
				//接着我们要判断订阅回复，看看是不是成功
				else if(MQTT_RxDataOutPtr[2]==0x90){ 
						switch(MQTT_RxDataOutPtr[6]){					
						case 0x00 :
						case 0x01 : printf("订阅成功\r\n");            //串口输出信息
												SubcribePack_flag = 1;                //SubcribePack_flag置1，表示订阅报文成功，其他报文可发送
												Ping_flag = 0;                        //Ping_flag清零
												TIM3_ENABLE_30S();                    //启动30s的PING定时器
												OS_TaskResume(&TASK8_TCB,&err);
												break;                                //跳出分支                                             
						default   : printf("订阅失败，准备重启\r\n");  //串口输出信息 
												Connect_flag = 0;                     //Connect_flag置零，重启连接
												break;                                //跳出分支 								
					}					
				}
				//if判断，第一个字节是0xD0，表示收到的是PINGRESP报文
				else if(MQTT_RxDataOutPtr[2]==0xD0)
			{ 
					printf("PING报文回复\r\n"); 		  //串口输出信息 
					if(Ping_flag==1){                     //如果Ping_flag=1，表示第一次发送
						 Ping_flag = 0;    				  //要清除Ping_flag标志
					}else if(Ping_flag>1){ 				  //如果Ping_flag>1，表示是多次发送了，而且是2s间隔的快速发送
						Ping_flag = 0;     				  //要清除Ping_flag标志
						TIM3_ENABLE_30S(); 				  //PING定时器重回30s的时间
					}				
				}	
				//if判断，如果第一个字节是0x30，表示收到的是服务器发来的推送数据
				//我们要提取控制命令
				else if((MQTT_RxDataOutPtr[2]==0x30))
				{ 
					printf("服务器等级0推送\r\n"); 		   //串口输出信息 
					MQTT_DealPushdata_Qs0(MQTT_RxDataOutPtr);  //处理等级0推送数据
				}				
								
				MQTT_RxDataOutPtr += BUFF_UNIT;                     //指针下移
				if(MQTT_RxDataOutPtr==MQTT_RxDataEndPtr)            //如果指针到缓冲区尾部了
					MQTT_RxDataOutPtr = MQTT_RxDataBuf[0];          //指针归位到缓冲区开头                        
			}//处理接收缓冲区数据的else if分支结尾

			/*-------------------------------------------------------------*/
			/*                     处理命令缓冲区数据                      */
			/*-------------------------------------------------------------*/

			if(MQTT_CMDOutPtr != MQTT_CMDInPtr){                        //if成立的话，说明命令缓冲区有数据了			       
				printf("命令:%s\r\n",&MQTT_CMDOutPtr[2]);                 //串口输出信息
				
				MQTT_CMDOutPtr += BUFF_UNIT;                             	 //指针下移
				if(MQTT_CMDOutPtr==MQTT_CMDEndPtr)           	             //如果指针到缓冲区尾部了
					MQTT_CMDOutPtr = MQTT_CMDBuf[0];          	             //指针归位到缓冲区开头				
			}//处理命令缓冲区数据的else if分支结尾

			
			
		}//Connect_flag=1的if分支的结尾
		
		/*--------------------------------------------------------------------*/
		/*      Connect_flag=0同服务器断开了连接,我们要重启连接服务器         */
		/*--------------------------------------------------------------------*/
		else
		{ 
			printf("需要连接服务器\r\n");                 //串口输出信息
			TIM_Cmd(TIM4,DISABLE);                           //关闭TIM4 
			TIM_Cmd(TIM3,DISABLE);                           //关闭TIM3  
			WiFi_RxCounter=0;                                //WiFi接收数据量变量清零                        
			memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);          //清空WiFi接收缓冲区 
			if(WiFi_Connect_IoTServer()==0)
			{   			     //如果WiFi连接云服务器函数返回0，表示正确，进入if
				printf("建立TCP连接成功\r\n");            //串口输出信息
				Connect_flag = 1;                            //Connect_flag置1，表示连接成功	
				WiFi_RxCounter=0;                            //WiFi接收数据量变量清零                        
				memset(WiFi_RX_BUF,0,WiFi_RXBUFF_SIZE);      //清空WiFi接收缓冲区 
				MQTT_Buff_ReInit();                          //重新初始化发送缓冲区                    
			}				
		}
		
		delay_ms(100);
	}
}
///////////////////////////////////上云发送任务/////////////////////////////////////
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
									(OS_OPT        )OS_OPT_PEND_BLOCKING,  //如果没有获取到信号量就等待
									(OS_MSG_SIZE  *)&msg_size2,          
									(CPU_TS       *)0,                     
									(OS_ERR       *)&err); 
		
		data_pack[0]=(TEXT_Buffer2[0]+((TEXT_Buffer2[1])<<8));
		data_pack[1]=(TEXT_Buffer2[2]+((TEXT_Buffer2[3])<<8));
		data_pack[2]=(TEXT_Buffer2[4]+((TEXT_Buffer2[5])<<8));
		sprintf(dat,"{\"method\":\"thing.event.property.post\",\"id\":\"203302322\",\"params\":{\"state\":%d,\"youwei\":%.2f,\"carInsideTemp\":%.1f,\"speed\":%.2f,\"interiorLightOnOff\":%d,\"nightLight\":%d},\"version\":\"1.0.0\"}",(u16)run_sta,((float)data_pack[0]/100.0),(float)data_pack[2]/10,((float)data_pack[1]/100.0),TEXT_Buffer2[6],TEXT_Buffer2[7]);
		MQTT_PublishQs0(P_TOPIC_NAME,dat,strlen(dat));   //添加数据，发布给服务器	
		
		myfree(SRAMIN,dat);

		delay_ms(2000);
	}
}

static  void  Task10(void *p_arg)//任务
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




