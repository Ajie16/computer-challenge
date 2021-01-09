#include "ld3320.h"
#include "stm32f10x.h"
#include <stdio.h>
#include "delay.h"


uint8_t nLD_Mode = LD_MODE_IDLE;//用来记录当前是在进行ASR识别还是在播放MP3


/**
  * @brief  写指令
  * @param  无
  * @retval 无
  */
void LD_WriteReg(uint8_t data1,uint8_t data2)
{
	LD_CS_L();
	LD_SPIS_L();
  
	spi_send_byte(0x04);
	spi_send_byte(data1);
	spi_send_byte(data2);
  
	LD_CS_H();
}

/**
  * @brief  读指令
  * @param  无
  * @retval 返回数据
  */
 uint8_t LD_ReadReg(uint8_t reg_add)
{
	uint8_t i;
  
	LD_CS_L();
	LD_SPIS_L();
  
	spi_send_byte(0x05);
	spi_send_byte(reg_add);
	i=spi_send_byte(0x00);
  
	LD_CS_H();
  
	return(i);
}


/**
  * @brief  语音识别模块语音识别功能与MP3播放功能共同配置
  * @param  无
  * @retval 无
  */
void LD_Init_Common(void)    
{
	LD_ReadReg(0x06);  
	LD_WriteReg(0x17, 0x35); 
	delay_uus(10);
	LD_ReadReg(0x06);  

	LD_WriteReg(0x89, 0x03);  
	delay_uus(10);
	LD_WriteReg(0xCF, 0x43);   
	delay_uus(10);
	LD_WriteReg(0xCB, 0x02);
	
	/*PLL setting*/
	LD_WriteReg(0x11, LD_PLL_11);       
	if (nLD_Mode == LD_MODE_MP3)
	{
		LD_WriteReg(0x1E, 0x00); 
		LD_WriteReg(0x19, LD_PLL_MP3_19);   
		LD_WriteReg(0x1B, LD_PLL_MP3_1B);   
		LD_WriteReg(0x1D, LD_PLL_MP3_1D);
	}
	else
	{
		LD_WriteReg(0x1E,0x00);
		LD_WriteReg(0x19, LD_PLL_ASR_19); 
		LD_WriteReg(0x1B, LD_PLL_ASR_1B);		
	  LD_WriteReg(0x1D, LD_PLL_ASR_1D);
	}
	delay_uus(1);
	
	LD_WriteReg(0xCD, 0x04);
	LD_WriteReg(0x17, 0x4c); 
	delay_uus(10);
  
	LD_WriteReg(0xB9, 0x00);
	LD_WriteReg(0xCF, 0x4F); 
	LD_WriteReg(0x6F, 0xFF); 
}


/**
  * @brief  ld3320语音识别功能初始化
  * @param  无
  * @retval 无
  */
 void LD_Init_ASR(void)
{
	nLD_Mode=LD_MODE_ASR_RUN;
	LD_Init_Common();

	LD_WriteReg(0xBD, 0x00);
	LD_WriteReg(0x17, 0x48);	
	delay_uus(1);
  
	LD_WriteReg(0x3C, 0x80);    
	LD_WriteReg(0x3E, 0x07);
	LD_WriteReg(0x38, 0xff);    
	LD_WriteReg(0x3A, 0x07);
	LD_WriteReg(0x40, 0);          
	LD_WriteReg(0x42, 8);
	LD_WriteReg(0x44, 0);    
	LD_WriteReg(0x46, 8);
  LD_WriteReg(0xB3,0x22 ); 
	delay_uus(1);
}


/**
  * @brief  运行LD3320
  * @param  无
  * @retval 无
  */
uint8_t LD_Run(void)
{  
    LD_reset();
    delay_uus(1);
  
		LD_AsrStart();			     //初始化ASR
		delay_uus(1);
  
		if (LD_AsrAddFixed()==0)	//添加关键词语到LD3320芯片中
    {
      return 0;
    }
		
		delay_uus(1);
		if (LD_AsrRun() == 0)
		{
			return 0;
		}
    
    return 1;    
}

/**
  * @brief  LD3320复位
  * @param  无
  * @retval 无
  */
 void LD_reset(void)
{
	LD_RST_H();
	delay_uus(1);
  
	LD_RST_L();
	delay_uus(1);
  
	LD_RST_H();
	delay_uus(1);
  
	LD_CS_L();
	delay_uus(1);
  
	LD_CS_H();		
	delay_uus(1);
}


/**
  * @brief  启动语音识别功能
  * @param  无
  * @retval 无
  */
 void LD_AsrStart(void)
{
	LD_Init_ASR();
}

/**
  * @brief  检查B2寄存器运行状态位
  * @param  无
  * @retval 无
  */
uint8_t LD_Check_ASRBusyFlag_b2(void)
{
	uint8_t j;
	uint8_t flag = 0;
	for (j=0; j<10; j++)
	{
		if (LD_ReadReg(0xb2) == 0x21)
		{
			flag = 1;
			break;
		}
		delay_uus(100);		
	}
	return flag;
}


/**
  * @brief  ld3320寄存器操作
  * @param  无
  * @retval 无
  */
 uint8_t spi_send_byte(uint8_t byte)
{
	while (SPI_I2S_GetFlagStatus(LD3320_SPI, SPI_I2S_FLAG_TXE) == RESET);
	SPI_I2S_SendData(LD3320_SPI,byte);
  
	while (SPI_I2S_GetFlagStatus(LD3320_SPI,SPI_I2S_FLAG_RXNE) == RESET);
	return SPI_I2S_ReceiveData(LD3320_SPI);
}


/**
  * @brief  ld3320语音识别功能配置
  * @param  无
  * @retval 无
  */
 uint8_t LD_AsrRun(void)
{
	LD_WriteReg(0x35, MIC_VOL);
	LD_WriteReg(0x1C, 0x09);
	LD_WriteReg(0xBD, 0x20);
	LD_WriteReg(0x08, 0x01);
	delay_uus( 1);
	LD_WriteReg(0x08, 0x00);
	delay_uus( 1);

	if(LD_Check_ASRBusyFlag_b2() == 0)
	{
		return 0;
	}

	LD_WriteReg(0xB2, 0xff);	
	LD_WriteReg(0x37, 0x06);
	delay_uus(5);
  
	LD_WriteReg(0x1C, 0x0b);
	LD_WriteReg(0x29, 0x10);   //关中断
	LD_WriteReg(0xBD, 0x00);   
  
	return 1;

}

/**
  * @brief  写入识别列表
  * @param  无
  * @retval 1：写入列表成功，0：写入列表失败
  */
 uint8_t LD_AsrAddFixed(void)
{
	uint8_t k, flag;
	uint8_t nAsrAddLength;
	#define DATE_A 5   //数组二维数值
	#define DATE_B 30		//数组一维数值
	//添加关键词，用户修改
	uint8_t  sRecog[DATE_A][DATE_B] = {
	 			"li dong er zi",\
				"da kai zhao ming deng",\
				"guan bi zhao ming deng",\
				"da kai ji dian qi",\
				"guan bi ji dian qi"\
		
																	};	
	uint8_t  pCode[DATE_A] = {
	 															1,	\
															  2,	\
																3,  \
																4,	\
																5		\
															};	//添加识别码，用户修改
  
	flag = 1;
                              
	for (k=0; k<DATE_A; k++)
	{			
		if(LD_Check_ASRBusyFlag_b2() == 0)
		{
			flag = 0;
			break;
		}

		LD_WriteReg(0xc1, pCode[k] );
		LD_WriteReg(0xc3, 0);
		LD_WriteReg(0x08, 0x04);
		delay_uus(1);
    
		LD_WriteReg(0x08, 0x00);
		delay_uus(1);

		for (nAsrAddLength=0; nAsrAddLength<DATE_B; nAsrAddLength++)
		{
			if (sRecog[k][nAsrAddLength] == 0)
				break;
			LD_WriteReg(0x5, sRecog[k][nAsrAddLength]);
      
		}
		LD_WriteReg(0xb9, nAsrAddLength);
		LD_WriteReg(0xb2, 0xff);
		LD_WriteReg(0x37, 0x04);
	}	 
  
	return flag;
}


/**
  * @brief  ld3320引脚配置
  * @param  无
  * @retval 无
  */
void LD3320_GPIO_Config(void)
{	
		GPIO_InitTypeDef GPIO_InitStructure;
		RCC_APB2PeriphClockCmd(LD3320RST_GPIO_CLK | LD3320CS_GPIO_CLK,ENABLE);
  
		//LD_CS	/RSET
		GPIO_InitStructure.GPIO_Pin =LD3320CS_PIN;
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
		GPIO_Init(LD3320CS_GPIO_PORT,&GPIO_InitStructure);
		GPIO_InitStructure.GPIO_Pin =LD3320RST_PIN;
		GPIO_Init(LD3320RST_GPIO_PORT,&GPIO_InitStructure);
}



/**
  * @brief  ld3320中断配置
  * @param  无
  * @retval 无
  */
void LD3320_EXTI_Config(void)
{
  EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;
  
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	
	RCC_APB2PeriphClockCmd(LD3320_IRQ_GPIO_CLK, ENABLE);
  
  GPIO_InitStructure.GPIO_Pin =LD3320_IRQ_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(LD3320_IRQ_GPIO_PORT, &GPIO_InitStructure);
  
	//外部中断线配置
  GPIO_EXTILineConfig(LD3320_IRQEXIT_PORTSOURCE, LD3320_IRQPINSOURCE);
  EXTI_InitStructure.EXTI_Line = LD3320_IRQEXITLINE;
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
  EXTI_InitStructure.EXTI_Trigger =EXTI_Trigger_Falling;
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  EXTI_Init(&EXTI_InitStructure);
  
	//中断嵌套配置
  NVIC_InitStructure.NVIC_IRQChannel = LD3320_IRQN;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}


/**
  * @brief  ld3320 SPI通信配置
  * @param  无
  * @retval 无
  */
void LD3320_SPI_Config(void)
{
	SPI_InitTypeDef  SPI_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
  //spi端口配置
	RCC_APB2PeriphClockCmd(LD3320_SPI_CLK,ENABLE);		
  RCC_APB2PeriphClockCmd(LD3320WR_GPIO_CLK |
                   LD3320_SPIMISO_GPIO_CLK | 
                   LD3320_SPIMOSI_GPIO_CLK | 
                   LD3320_SPISCK_GPIO_CLK,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = LD3320_SPIMISO_PIN;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_Init(LD3320_SPIMISO_GPIO_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = LD3320_SPIMOSI_PIN;
	GPIO_Init(LD3320_SPIMOSI_GPIO_PORT,&GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = LD3320_SPISCK_PIN;
	GPIO_Init(LD3320_SPISCK_GPIO_PORT,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = LD3320WR_PIN;				
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Init(LD3320WR_GPIO_PORT, &GPIO_InitStructure);
	
	
	//为对比测试设置CS引脚为输入	
	
	 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOF, ENABLE);	 //使能PB PF端口时钟	
	 GPIO_InitStructure.GPIO_Pin =GPIO_Pin_12;//PB12
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING ;//GPIO_Mode_Out_PP; 		 //推挽输出
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	 GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB
	 
	 GPIO_InitStructure.GPIO_Pin =GPIO_Pin_7;//PF7
	 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING ;//GPIO_Mode_Out_PP; 		 //推挽输出
	 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
	 GPIO_Init(GPIOF, &GPIO_InitStructure);					 //根据设定参数初始化GPIOF
		
	
//	ETH_CS_L();   //关掉w5500 vs1053
//	VS_CS_L();
	
	
	LD_WR_L();//设置WR/SPIS 低电平
	
	LD_CS_H();
	
	SPI_Cmd(LD3320_SPI, DISABLE);

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;   	//全双工
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;						   						//主模式
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;					   					//8位
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;						   							//时钟极性 空闲状态时，SCK保持低电平
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;						   						//时钟相位 数据采样从第一个时钟边沿开始
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;							   							//软件产生NSS
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_64;   //波特率控制 SYSCLK/128
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;				   					//数据高位在前
	SPI_InitStructure.SPI_CRCPolynomial = 7;							   							//CRC多项式寄存器初始值为7
	SPI_Init(LD3320_SPI, &SPI_InitStructure);

	SPI_Cmd(LD3320_SPI, ENABLE);
}


/**
  * @brief  ld3320配置
  * @param  无
  * @retval 无
  */
void LD3320_Config(void)
{
   LD3320_GPIO_Config();
   LD3320_SPI_Config();
}

