#ifndef __LD3320_H
#define __LD3320_H

#include "delay.h"
#include "ld3320_test.h"


#define LD_MODE_IDLE			0x00
#define LD_MODE_ASR_RUN		0x08
#define LD_MODE_MP3		 		0x40

void LD_WriteReg(uint8_t data1,uint8_t data2);
uint8_t LD_ReadReg(uint8_t reg_add);
void LD_Init_Common(void);
void LD_Init_ASR(void);
uint8_t LD_Run(void);
void LD_reset(void);
void LD_AsrStart(void);
uint8_t LD_Check_ASRBusyFlag_b2(void);
uint8_t spi_send_byte(uint8_t byte);
uint8_t LD_AsrRun(void);
uint8_t LD_AsrAddFixed(void);
void LD3320_GPIO_Config(void);
void LD3320_EXTI_Config(void);
void LD3320_SPI_Config(void);
void LD3320_Config(void);
  
#endif /* __LD3320_H */
