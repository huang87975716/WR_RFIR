/*
*****************************************************************************
* Copyright (c) 2012, kunshan Robotechn Intelligent Technology, CO., LTD.
* All Rights Reserved.
*
* File Name   : taskset.c
* Abstract    : 任务创建系统初始化  及LED显示
* Author      : wuwei
*
* History
* date          author      notes
* 2014-07-07    wuwei       File Created.
*
*****************************************************************************
*/

#include "includes.h"
/* Private macro -------------------------------------------------------------*/
#define countof(a) (sizeof(a) / sizeof(*(a)))

/* Private define ------------------------------------------------------------*/
#define  FLASH_WriteAddress     0x700000
#define  FLASH_ReadAddress      FLASH_WriteAddress
#define  FLASH_SectorToErase    FLASH_WriteAddress
#define  M25P64_FLASH_ID        0x202017
#define  BufferSize (countof(Tx_Buffer)-1)



FlagType flag;

 //INT8U ID[20];
u8 MY_ID[4] ={0};
/* Private variables ---------------------------------------------------------*/
//u8 Tx_Buffer[] = "STM32F10x SPI Firmware Library Example: communication with an M25P64 SPI FLASH";
//u8 Index;
u16 Timout_Count=TIMOUT_20s;//学习超时检测  30秒
u16 Timout_Count2=200;//学习超时检测  30秒
//u8 Rx_Buffer[BufferSize];
//volatile TestStatus TransferStatus1 = FAILED, TransferStatus2 = PASSED;
//vu32 FLASH_ID = 0;
/********************************************************************************
 * FunctionName: TaskSet
 *
 * Description : 设置任务，设置完删除任务
 *
 * Parameters  : None.
 *
 * Returns     : None.
 *******************************************************************************/
void TaskSet(void *p_arg)
{
   // INT8U i;
    u32 seed=0;
    u8 random_data;
   //INT32U msg = 0;
    //INT8U err = 0;
    OS_CPU_SR  cpu_sr = 0;
    p_arg = p_arg;
    DelayMs(30);
    OS_ENTER_CRITICAL();
	
    BSP_Init();
    Flash_Read(RFIR_ID_ADDR, MY_ID, 4);    //读取FLASH ID存入BUF
    #if 0
    SendString("\n\rMY ID:");
    for(i=0;i<4;i++)//sizeof(ID)
    {
      SendString(";");
       Outint(MY_ID[i]);
    }
#endif

/********************************************************************************
 * Description : 任务建立   共9个任务
 *******************************************************************************/

  //创建协议解析任务
    OSTaskCreate(TaskProtocol,
                 (void*)0,
                 &TaskProtocolStk[TASK_PROTOCOL_STKSIZE-1],
                 TASK_PROTOCOL_PRIO);

    //创建串口发送任务
    OSTaskCreate(TaskProtoSend,
                 (void*)0,
                 &TaskProtoSendStk[TASK_PROTO_SEND_STKSIZE-1],
              TASK_PROTO_SEND_PRIO );

    //创建量产测试任务
    OSTaskCreate(TaskTest,
                 (void*)0,
                 &TaskTestStk[TASK_TEST_STKSIZE-1],
                 TASK_TEST_PRIO );

    //创建4432接收任务
    OSTaskCreate(TaskSi4432Read,
                 (void*)0,
                 &TaskSi4432ReadStk[TASK_SET_STKSIZE-1],
                 TASK_Si4432_READ_PRIO );


/********************************************************************************
 * Description : 创建消息队列
 *******************************************************************************/
  //  SetMBox  = OSQCreate(&SetQBuff[0], TASK_SET_QSIZE);
    ProtocolMBox  = OSQCreate(&ProtocolQBuff[0], TASK_PROTOCOL_QSIZE);
    ProtoSendMBox = OSQCreate(&ProtoSendQBuff[0], TASK_PROTOSEND_QSIZE);
    TestMBox      = OSQCreate(&TestQBuff[0], TASK_TEST_QSIZE);
    Si4432ReadMBox   = OSQCreate(&Si4432ReadQBuff[0], TASK_Si4432READ_QSIZE);

   OS_EXIT_CRITICAL();
   SysTickInit();

    //自我删除
   //  OSTaskDel(OS_PRIO_SELF);
    /*自我删除函数会自动进行任务的调度，因此此处不需要添加延时*/


  for (;;)
    {
         /**********学习状态指示灯********/
            if(learn_cmd==1)  //学习开始标志位
            {
                                    LED_Send(0);// 绿灯关闭
                                    LED_Learn(1);//红灯闪烁
                                     WatchDog_Feed(); //喂狗
                                    OSTimeDly(4);
                                     WatchDog_Feed(); //喂狗
                                    LED_Learn(0);
                                     WatchDog_Feed(); //喂狗
                                    OSTimeDly(4);
                                     WatchDog_Feed(); //喂狗
                                    Timout_Count--;
                                    if(Timout_Count==0)
                                {
                                    Timout_Count=TIMOUT_20s;
                                    OSQPost(TestMBox, (void*)Stu_Fail); //学习失败信号
                                    learn_cmd=0; //清空学习成功标志位
                                }
            }
             if(learn_cmd==0)  //学习成功标志位
           {
                                    LED_Learn(0);//红灯关闭
                                    LED_Send(1); //绿灯开启
           }
       /**********生成随机滚动序列*********/
           seed+=23;
           if(seed>65000)
           seed=0;
           srand(seed);
           random_data=rand();
           test_roll_list=random_data^0x29;
                     if(learn_cmd==0)
                     {
                      Timout_Count2--;
                        if((Timout_Count2>=40)&&(Timout_Count2<=200))
                    {
                            LED_Send(0);
                    }
                    else if((Timout_Count2<40)&&(Timout_Count2>=1))
                    {
                            LED_Send(1);
                    //  Timout_Count2=200;
                    }
                    else if(Timout_Count2<=0)
                            Timout_Count2=200;
                        }
             WatchDog_Feed(); //喂狗
           OSTimeDly(1);

    }
  }

