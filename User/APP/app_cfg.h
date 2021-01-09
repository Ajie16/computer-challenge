/*
*********************************************************************************************************
*                                              EXAMPLE CODE
*
*                           (c) Copyright 2009-2013; Micrium, Inc.; Weston, FL
*
*               All rights reserved.  Protected by international copyright laws.
*               Knowledge of the source code may NOT be used to develop a similar product.
*               Please help us continue to provide the Embedded community with the finest
*               software available.  Your honesty is greatly appreciated.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                      APPLICATION CONFIGURATION
*
*                                     ST Microelectronics STM32
*                                              on the
*
*                                     Micrium uC-Eval-STM32F107
*                                         Evaluation Board
*
* Filename      : app_cfg.h
* Version       : V1.00
* Programmer(s) : JJL
*                 EHS
*                 DC
*********************************************************************************************************
*/

#ifndef  __APP_CFG_H__
#define  __APP_CFG_H__


/*
*********************************************************************************************************
*                                       MODULE ENABLE / DISABLE
*********************************************************************************************************
*/

#define  APP_CFG_SERIAL_EN                          DEF_DISABLED          // Modified by fire （原是 DEF_ENABLED）


#define KEY1_EVENT  (0x01 << 0)//设置事件掩码的位0
#define KEY2_EVENT  (0x01 << 1)
#define KEY3_EVENT  (0x01 << 2)
#define KEY4_EVENT  (0x01 << 3)
#define KEY5_EVENT  (0x01 << 4)
#define KEY6_EVENT  (0x01 << 5)//设置事件掩码的位0
#define KEY7_EVENT  (0x01 << 6)
#define KEY8_EVENT  (0x01 << 7)
#define KEY9_EVENT  (0x01 << 8)
#define KEY10_EVENT  (0x01 << 9)
#define KEY11_EVENT  (0x01 << 10)//设置事件掩码的位0
#define KEY12_EVENT  (0x01 << 11)
#define KEY13_EVENT  (0x01 << 12)
#define KEY14_EVENT  (0x01 << 13)
#define KEY15_EVENT  (0x01 << 14)
#define KEY16_EVENT  (0x01 << 15)

/*
*********************************************************************************************************
*                                            TASK PRIORITIES
*********************************************************************************************************
*/

#define  APP_TASK_START_PRIO                        10

#define  TASK1_PRIO                   					    2

#define  TASK2_PRIO                   					    8

#define  TASK3_PRIO                   					    3

#define  TASK4_PRIO                   					    6

#define  TASK5_PRIO                   					    7

#define  TASK6_PRIO                   					    9

#define  TASK7_PRIO                   					    4

#define  TASK8_PRIO                   					    10

#define  TASK9_PRIO                   					    11

#define  TASK10_PRIO                   					    12
/*
*********************************************************************************************************
*                                            TASK STACK SIZES
*                             Size of the task stacks (# of OS_STK entries)
*********************************************************************************************************
*/

#define  APP_TASK_START_STK_SIZE                    128

#define  TASK1_STK_SIZE															512

#define  TASK2_STK_SIZE															256

#define  TASK3_STK_SIZE															300

#define  TASK4_STK_SIZE															256

#define  TASK5_STK_SIZE															400

#define  TASK6_STK_SIZE															256

#define  TASK7_STK_SIZE															700

#define  TASK8_STK_SIZE															256

#define  TASK9_STK_SIZE															256

#define  TASK10_STK_SIZE														256
/*
*********************************************************************************************************
*                                    BSP CONFIGURATION: RS-232
*********************************************************************************************************
*/

#define  BSP_CFG_SER_COMM_SEL             			BSP_SER_COMM_UART_02
#define  BSP_CFG_TS_TMR_SEL                         2


/*
*********************************************************************************************************
*                                     TRACE / DEBUG CONFIGURATION
*********************************************************************************************************
*/
#if 0
#define  TRACE_LEVEL_OFF                            0
#define  TRACE_LEVEL_INFO                           1
#define  TRACE_LEVEL_DEBUG                          2
#endif

#define  APP_TRACE_LEVEL                            TRACE_LEVEL_INFO
#define  APP_TRACE                                  BSP_Ser_Printf

#define  APP_TRACE_INFO(x)            ((APP_TRACE_LEVEL >= TRACE_LEVEL_INFO)  ? (void)(APP_TRACE x) : (void)0)
#define  APP_TRACE_DEBUG(x)           ((APP_TRACE_LEVEL >= TRACE_LEVEL_DEBUG) ? (void)(APP_TRACE x) : (void)0)

void TempHumi_State(void);  //函数声明，采集温湿度，并发布给服务器
#endif
