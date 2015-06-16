/******************************************************************************
* Copyright (c) 2014, kunshan Robotechn Intelligent Technology, CO., LTD.
* All Rights Reserved.
*
* File Name   : main.c
* Abstract    :
* Author      :
*
* History
* date          author      notes
* 2014-03-18                File Created.
*
******************************************************************************/

#include "includes.h"

void MyFlagInit(void);
void MyTim3PWMInit(u16 Value1);
void MyTIM1CaptureInit(void);
u8 Sendbuff[4] = {0x00,0x01,0x02,0x03};
#if 0
int main(void)
{
//    u8 k;
    /*STM32外设初始化*/
    STM32_Init();
    /*SI4432初始化*/
    SI4432_Configuration();
    /*标志位初始化*/
    MyFlagInit();
    SendString("system init is end\n");
    //HardwareWSystemInit();
    LED_Init(1);
    SendString("Led init open\n");
    //myEXIT_Init();
    //MyExtiNvicInit();

    SendString("4432 initOK\n");
    while(1)
    {
        Data_Encrypt(learn_ok);
        Sending(send_data,data_length);
    }
}
#endif
void SystemInit(void)
{

	
}

void BspInit(void)
{
	  STM32_Init();
    /*SI4432初始化*/
    SI4432_Configuration();
    /*标志位初始化*/
    MyFlagInit();
    SendString("system init is end\n");
    //HardwareWSystemInit();
    LED_Init(1);
    SendString("Led init open\n");
    //myEXIT_Init();
    //MyExtiNvicInit();
    SendString("4432 initOK\n");
}
void MyFlagInit(void)
{
    Flag.Si4432Rx_Status = 1;    //4432接收标志位，1：接收 0：不可接收
    Flag.Si4432Tx_Status = 1;    //4432发送标志位，1：可发送  0：不可发送
}


/********************************************************************************
 * FunctionName: MyTim3Init
 *
 * Description :
 *
 * Parameters  :
 *
 * Returns     :
 *******************************************************************************/
void MyTim3PWMInit(u16 Value1)
{
    TIM_OCInitTypeDef TIMOCInitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_TimeBaseInitTypeDef   TIM_TimeBaseStructure;
    TIM_InternalClockConfig(TIM3);          //8M
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE );

    /*PB0 = MOD_S--38k载波--推挽*/
    //GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3,ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseStructure.TIM_RepetitionCounter = 0;
    //TIM_TimeBaseStructure.TIM_Prescaler = 0;   //72M
    TIM_TimeBaseStructure.TIM_Period = 206;   //约为38K
    TIM_TimeBaseInit(TIM3,&TIM_TimeBaseStructure);

    TIMOCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; //PWM模式1输出
    TIMOCInitStructure.TIM_Pulse = Value1;     //脉冲值 赋给CCR寄存器//占空比=(CCRx/ARR)*100%   TIM_Pulse/(TIM_Period+1)    50%:64
    TIMOCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;//TIM输出比较极性高
    TIMOCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//使能输出状态
    //TIM_OC3Init(TIM2, &TIMOCInitStructure);//TIM2的CH2输出
    TIM_OC3Init(TIM3, &TIMOCInitStructure);
    TIM_ARRPreloadConfig(TIM3,ENABLE);
    TIM_CtrlPWMOutputs(TIM3,ENABLE);//设置TIM2的PWM输出为使能
    TIM_Cmd(TIM3, ENABLE); //开启时钟
}

/********************************************************************************
 * FunctionName: MyTIM1InterruptNvicInit
 *
 * Description :
 *
 * Parameters  :
 *
 * Returns     :
 *******************************************************************************/


void MyTIM1InterruptNvicInit(void)
{
    NVIC_InitTypeDef    NVIC_InitStructure;
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    // Enable the TIM3 global Interrupt
    NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;  //TIM3中断
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;  //先占优先级0级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;  //从优先级3级
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //IRQ通道被使能
    NVIC_Init(&NVIC_InitStructure);  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
}


/********************************************************************************
 * FunctionName: MyTIM1CaptureInit
 *
 * Description :
 *
 * Parameters  :
 *
 * Returns     :
 *******************************************************************************/
void MyTIM1CaptureInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    TIM_ICInitTypeDef TIM_ICInitStructure;
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1,ENABLE);
    TIM_InternalClockConfig(TIM1);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;  //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    /**
     * 计数0-0xFFFF，分频71。则周期为1S，溢出一次时间为1uS*0xFFFF=65ms
     */
    TIM_TimeBaseInitStructure.TIM_Period = 0xFFFF;                  // 计数周期
    TIM_TimeBaseInitStructure.TIM_Prescaler = 71;                   // 72MHz / (71+1) = 1000KHz = 1M -->1uS
    TIM_TimeBaseInitStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up; // 上升计数
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseInitStructure);
    TIM_ITConfig(TIM1, TIM_IT_Update, DISABLE);                     // 禁止溢出中断
    TIM_ClearFlag(TIM1, TIM_FLAG_Update);

    TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;                // 通道1输入捕获
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Falling;     // 下降沿触发
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;           // 每次检测到都触发一次捕获
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI; // TI1被连接到IC1
    TIM_ICInitStructure.TIM_ICFilter = 0x0;
    TIM_PWMIConfig(TIM1, &TIM_ICInitStructure);

    TIM_SelectInputTrigger(TIM1, TIM_TS_TI1FP1);                    // 选择触发信号
    TIM_SelectSlaveMode(TIM1, TIM_SlaveMode_Reset);                 // 选中触发信号的下降沿重新初始化计数器并触发寄存器更新
    TIM_SelectMasterSlaveMode(TIM1, TIM_MasterSlaveMode_Enable);    // 使能主从模式
    TIM_ITConfig(TIM1, TIM_IT_CC1, DISABLE);
    TIM_ClearFlag(TIM1, TIM_FLAG_CC1);
    TIM_Cmd(TIM1, ENABLE);
    TIM_ITConfig(TIM1, TIM_IT_CC1, ENABLE);
}

