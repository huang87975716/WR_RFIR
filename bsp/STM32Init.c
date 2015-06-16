#include "includes.h"
u16 CCR1_Val = 500;
u16 CCR2_Val = 375;
u16 CCR3_Val = 250;
u16 CCR4_Val = 125;
ErrorStatus HSEStartUpStatus;
EXTI_InitTypeDef EXTI_InitStructure;
u8 Speaker_StringSerial[];
/********************************************************************************
 * FunctionName: STM32_Init
 *
 * Description : STM32 外设初始化
 *
 * Parameters  : None
 *
 * Returns     : None
 *******************************************************************************/
void STM32_Init(void)
{
    MyRCCInit();
    MyUsartInit();
    MyExtiNvicInit();
    DMA_Configuration();
}

/********************************************************************************
 * FunctionName: MyRCCInit
 *
 * Description : RCC初始化
 *
 * Parameters  : None
 *
 * Returns     : None
 *******************************************************************************/
void MyRCCInit(void)
{
#if 0
 /* RCC system reset(for debug purpose) */
  RCC_DeInit();

  /* Enable HSE */
  RCC_HSEConfig(RCC_HSE_ON);

  /* Wait till HSE is ready */
  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if (HSEStartUpStatus == SUCCESS)
  {
    /* Enable Prefetch Buffer */
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

    /* Flash 2 wait state */
    FLASH_SetLatency(FLASH_Latency_2);

    /* HCLK = SYSCLK */
    RCC_HCLKConfig(RCC_SYSCLK_Div1);

    /* PCLK2 = HCLK */
    RCC_PCLK2Config(RCC_HCLK_Div1);

    /* PCLK1 = HCLK/2 */
    RCC_PCLK1Config(RCC_HCLK_Div2);

    /* PLLCLK = 8MHz * 9 = 72 MHz */
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);

    /* Enable PLL */
    RCC_PLLCmd(ENABLE);

    /* Wait till PLL is ready */
    while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
    {}

    /* Select PLL as system clock source */
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    /* Wait till PLL is used as system clock source */
    while (RCC_GetSYSCLKSource() != 0x08)
    {}
  }

#endif



    #if 0
    /*定义枚举类型变量HSEStartUpStatus*/
    ErrorStatus HSEStartUpStatus;
    RCC_DeInit();                                               /*复位系统时钟设置*/
    RCC_HSEConfig(RCC_HSE_ON);                                  /*开启HSE*/
    HSEStartUpStatus = RCC_WaitForHSEStartUp();                 /*等待HSE起振并稳定*/
    /*判断HSE是否起振成功，是则进入if*/
    if(HSEStartUpStatus == SUCCESS)
    {
        RCC_HCLKConfig(RCC_SYSCLK_Div1);                        /*选择HCLK(AHB)时钟源为SYSCLK 1分频*/
        RCC_PCLK2Config(RCC_HCLK_Div1);                         /*选择PCLK2时钟源为HCLK(AHB) 1分频*/
        RCC_PCLK1Config(RCC_HCLK_Div2);                         /*选择PCLK1时钟源为HCLK(AHB) 2分频*/
        FLASH_SetLatency(FLASH_Latency_2);                      /*设置Flash延时周期数为2*/
        FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);   /*使能Flash预取缓存*/
        /*选择PLL时钟源为HSE 1分频，倍频数为9，则PLL=8MHz*9=72MHz*/
        RCC_PLLConfig(RCC_PLLSource_HSE_Div1,RCC_PLLMul_9);
        RCC_PLLCmd(ENABLE);                                     /*使能PLL*/
        while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET);     /*等待PLL输出稳定*/
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);              /*选择SYSCLK时钟源为PLL*/
        while(RCC_GetSYSCLKSource() != 0x08);                   /*等待PLL称为SYSCLK时钟源*/
    }
    /*打开APB2总线上的PWR，BKP时钟*/
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR | RCC_APB1Periph_BKP,ENABLE);

     RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
  //  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
#endif
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,   ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,  ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,  ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,  ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1,   ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2,   ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,   ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,   ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
}
/********************************************************************************
 * FunctionName: MyExtiNvicInit
 *
 * Description : 中断优先级初始化
 *
 * Parameters  : None
 *
 * Returns     : None
 *******************************************************************************/
void MyExtiNvicInit(void)
{
    NVIC_InitTypeDef    NVIC_InitStructure;
    //NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0); //设置中断优先级设置字节方式

    NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

     // USART1中断
    NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 4;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
     // USART2中断
    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 6;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    //外部中断 SI4432
    NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 5;//占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //次优先级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //通道中断使能
    NVIC_Init(&NVIC_InitStructure);       //初始化中断

         //Enable DMA Channel7 Interrupt
      NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel7_IRQn;
      NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
      NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
      NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
      NVIC_Init(&NVIC_InitStructure);

      /*Enable DMA Channel6 Interrupt */
      NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel6_IRQn;
      NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
      NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
      NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
      NVIC_Init(&NVIC_InitStructure);

}

/********************************************************************************
 * FunctionName: MyUsartInit
 *
 * Description : USART初始化
 *
 * Parameters  : None
 *
 * Returns     : None
 *******************************************************************************/
void MyUsartInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_USART1, ENABLE);
     RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
    //RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE); //开启串口时钟
    //配置串口TX作为复用推挽输出端口
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9|GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    //配置串口RX作为悬空输入端口
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10|GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = 19200;    //波特率
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;     //数据位
    USART_InitStructure.USART_StopBits = USART_StopBits_1;      //停止位
    USART_InitStructure.USART_Parity = USART_Parity_No;     //奇偶校验
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None; //数据流控制
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;     //模式

    USART_Init(USART1, &USART_InitStructure);
    //USART_ITConfig(USART1, USART_IT_IDLE, ENABLE);  // 开启空闲,帧错,噪声,校验错中断
    USART_ITConfig(USART1, USART_IT_RXNE, ENABLE);  // ???í????????
    //使能串口
    USART_Cmd(USART1, ENABLE);
    USART_Init(USART2, &USART_InitStructure);
    USART_ITConfig(USART2, USART_IT_IDLE, ENABLE);  // 开启空闲,帧错,噪声,校验错中断
    // USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);  // ???í????????
    USART_Cmd(USART2, ENABLE);
}
/********************************************************************************
 * FunctionName: mySPI_Init
 *
 * Description : SPI初始化
 *
 * Parameters  : None
 *
 * Returns     : None
 *******************************************************************************/
void mySPI_Init(void)
{
    SPI_InitTypeDef  SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2 | RCC_APB2Periph_GPIOB,  ENABLE);       //使能SPI2外设时钟
    //P0/P1/P2
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB,&GPIO_InitStructure);
    /* SPI2 配置 */
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;      //全双工
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;                           //主模式
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;                       //8位
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;                              //时钟极性 空闲状态时，SCK保持低电平
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;                            //时钟相位 数据采样从第一个时钟边沿开始
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;                               //软件产生NSS
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;    //波特率控制 SYSCLK/256
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;                      //数据高位在前
    SPI_InitStructure.SPI_CRCPolynomial = 7;                                //CRC多项式寄存器初始值为7
    SPI_Init(SPI2, &SPI_InitStructure);
    /* 使能SPI2 */
    SPI_Cmd(SPI2, ENABLE);
}
/********************************************************************************
 * FunctionName: myEXIT_Init
 *
 * Description : 外部中断设置
 *
 * Parameters  : None
 *
 * Returns     : None
 *******************************************************************************/
void myEXIT_Init(void)
{
  //  GPIO_InitTypeDef GPIO_InitStructure;
    //EXTI_InitTypeDef EXTI_InitStructure;
#if 0
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE );

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11 ;     //PB11,配置成外部中断模式NIRQ 产生中断时为低
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource11);     //配置中断线

    EXTI_InitStructure.EXTI_Line = EXTI_Line11 ;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    //EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
#endif
}
/********************************************************************************
 * FunctionName: MyLedInit
 *
 * Description : 初始化完成指示灯GPIO初始化
 *
 * Parameters  : None
 *
 * Returns     : None
 *******************************************************************************/
void MyLedInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);


  GPIO_ResetBits(GPIOB,GPIO_Pin_8|GPIO_Pin_9);


}
/********************************************************************************
 * FunctionName: SI4432_GPIOInit
 *
 * Description : SI4432 GPIO 初始化
 *
 * Parameters  : None
 *
 * Returns     : None
 *******************************************************************************/
void SI4432_GPIOInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE );
    /*PB12 = NSEL 推挽*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    SI4432_NSEL_HIGH;
    /*PB10 =SI_SDN    推挽*/
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    SI4432_SDN_LOW;
}
/*
void GetUniqueID(void)
{
      u8 i;
      u32 Device_Serial0, Device_Serial1, Device_Serial2;

      Device_Serial0 = *(u32*)(0x1FFFF7E8);
      Device_Serial1 = *(u32*)(0x1FFFF7EC);
      Device_Serial2 = *(u32*)(0x1FFFF7F0);

      if (Device_Serial0 != 0)
      {
        Speaker_StringSerial[0] = (u8)(Device_Serial0 & 0x000000FF);
        Speaker_StringSerial[1] = (u8)((Device_Serial0 & 0x0000FF00) >> 8);
        Speaker_StringSerial[2] = (u8)((Device_Serial0 & 0x00FF0000) >> 16);
        Speaker_StringSerial[3] = (u8)((Device_Serial0 & 0xFF000000) >> 24);

        Speaker_StringSerial[4] = (u8)(Device_Serial1 & 0x000000FF);
        Speaker_StringSerial[5] = (u8)((Device_Serial1 & 0x0000FF00) >> 8);
        Speaker_StringSerial[6] = (u8)((Device_Serial1 & 0x00FF0000) >> 16);
        Speaker_StringSerial[7] = (u8)((Device_Serial1 & 0xFF000000) >> 24);

        Speaker_StringSerial[8] = (u8)(Device_Serial2 & 0x000000FF);
        Speaker_StringSerial[9] = (u8)((Device_Serial2 & 0x0000FF00) >> 8);
        Speaker_StringSerial[10] = (u8)((Device_Serial2 & 0x00FF0000) >> 16);
        Speaker_StringSerial[11] = (u8)((Device_Serial2 & 0xFF000000) >> 24);
      }
      for (i=0;i<12;i++)
      {

          learn_ok[i+5] = Speaker_StringSerial[i];
          learn_fail[i+5] = Speaker_StringSerial[i];
          learning[i+5] = Speaker_StringSerial[i];
          SendString("id= ");
          Outint(Speaker_StringSerial[i]);
      }
}
*/

