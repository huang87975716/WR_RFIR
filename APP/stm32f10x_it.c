/*
*****************************************************************************
* Copyright (c) 2012, kunshan Robotechn Intelligent Technology, CO., LTD.
* All Rights Reserved.
*
* File Name   : stm32f10x_it.c
* Abstract    : ä¸­æ–­å‡½æ•°çš„å®žçŽ°ï¼Œæ‰€æœ‰ä¸­æ–­åœ¨æ­¤æ–‡ä»¶ä¸­å®žçŽ°
* Author      : wenwu
*
* History
* date          author      notes
* 2012-08-09    wenwu       File Created.
*
*****************************************************************************
*/

#include "stm32f10x_it.h"
#include "includes.h"
u8 Ir_REV_OK=0;

void NMI_Handler(void)
{
}


void HardFault_Handler(void)
{
    while (1)
    {
    }
}

void MemManage_Handler(void)
{
    while (1)
    {
    }
}

void BusFault_Handler(void)
{
    while (1)
    {
    }
}

void UsageFault_Handler(void)
{
    while (1)
    {
    }
}

void SVC_Handler(void)
{
}


void DebugMon_Handler(void)
{
}

/********************************************************************************
 * FunctionName: SysTick_Handler
 *
 * Description : SYSTICKå®šæ—¶å™¨ä¸­æ–­
 *
 * Parameters  : None.
 *
 * Returns     : None.
 *******************************************************************************/
void SysTick_Handler(void)
{
OS_CPU_SR cpu_sr;
OS_ENTER_CRITICAL();
OSIntNesting++;
OS_EXIT_CRITICAL();
OSTimeTick();
OSIntExit();
}




uint8_t u1RecvCh = 0;               // ´æ´¢½ÓÊÕµ½µÄ×Ö·û
IDPROTOCOL_t *u1p = &gU1RecvBuff[0];  // Ö¸Ê¾»º´æµØÖ·
uint8_t u1BufferSwitch = 0;

uint8_t u2RecvCh = 0;               // ´æ´¢½ÓÊÕµ½µÄ×Ö·û
PROTOCOL_t *u2p = &gU2RecvBuff[0];  // Ö¸Ê¾»º´æµØÖ·
uint8_t u2BufferSwitch = 0;

/********************************************************************************
 * FunctionName: USART1_IRQHandler
 *
 * Description : ´®¿Ú1µÄ½ÓÊÕÖÐ¶Ï
 *
 * Parameters  : None.
 *
 * Returns     : None.
 *******************************************************************************/
void USART1_IRQHandler(void)
{
    static uint8_t u1StaMach = 0;

     // USART1->DR = u1RecvCh;
    // Èç¹û´æÔÚÊý¾Ý

    if (USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)
    {
         // ¶ÁÈ¡USART1_DR¼Ä´æÆ÷»á×Ô¶¯Çå¿Õ¸Ã±êÖ¾Î»
        u1RecvCh = (USART1->DR) & 0xFF;

         // ×´Ì¬»úÍÆ½ø
        if (u1StaMach == 0)
        {
            if (u1RecvCh == 0xAA)
            {
                u1p->header = u1RecvCh;
                u1StaMach = 1;
                u1p->checksum = u1RecvCh;
            }
        }
        else if (u1StaMach == 1)
        {
            u1p->direction = u1RecvCh;
            u1StaMach = 2;
            u1p->checksum += u1RecvCh;
        }
        else if (u1StaMach == 2)
        {
            u1p->msgref = u1RecvCh;
            u1StaMach = 3;
            u1p->checksum += u1RecvCh;
        }
        else if (u1StaMach == 3)
        {
            u1p->command = u1RecvCh;
            u1StaMach = 4;
            u1p->checksum += u1RecvCh;
        }
        else if (u1StaMach == 4)
        {
            u1p->length = u1RecvCh;
                if (u1p->length > 8)
            {
                u1StaMach = 0;
            }
            else
            {
                if (u1p->length == 0)  //0×Ö½Ú
                {
                    u1StaMach = 6;
                }
                else                 //1-7×Ö½Ú
                {
                    u1StaMach = 5;
                }
                u1p->dataIndex = 0;
                u1p->checksum += u1RecvCh;
            }
        }
        else if (u1StaMach == 5)
        {
            u1p->data[u1p->dataIndex++] = u1RecvCh;
            u1p->checksum += u1RecvCh;
            if (u1p->dataIndex == u1p->length)
            {
                u1StaMach = 6;
            }
        }
        else if (u1StaMach == 6)
        {
             // ÅÐ¶ÏÐ£ÑéºÍ
            if (u1p->checksum == u1RecvCh)
            {
                if (u1BufferSwitch)
                {
                    OSQPost(ProtocolMBox, (void*)UART1_BUFF1_RECV);
                }
                else
                {
                    OSQPost(ProtocolMBox, (void*)UART1_BUFF0_RECV);
                }
            // ³É¹¦½ÓÊÕµ½Ò»´ÎÊý¾Ý£¬ÇÐ»»»º´æÇøÓò
                u1BufferSwitch ^= 1;
                u1p = &gU1RecvBuff[u1BufferSwitch];
            }
            u1StaMach = 0;
        }
    }
        USART_ClearITPendingBit(USART1,USART_IT_RXNE);
}


/********************************************************************************
 * FunctionName: USART2_IRQHandler
 *
 * Description : ´®¿Ú2µÄ½ÓÊÕÖÐ¶Ï
 *
 * Parameters  : None.
 *
 * Returns     : None.
 *******************************************************************************/
#if 0
void USART2_IRQHandler(void)
{
    static uint8_t u2StaMach = 0;
//    u8 i;
    if (USART_GetITStatus(USART2,USART_IT_RXNE) != RESET)
    {
      u2RecvCh = (USART2->DR) & 0xFF;
            if (u2StaMach == 0)
         {
            if (u2RecvCh == 0x16)  //head
            {
                u2p->header = u2RecvCh;
                u2StaMach = 1;
                u2p->checksum = u2RecvCh;
            }
         }
           else if (u2StaMach == 1)   //cmd
         {
            u2p->command = u2RecvCh;
            u2StaMach = 2;
            u2p->checksum += u2RecvCh;
         }

           else if (u2StaMach ==2)  //length
         {
            u2p->length = u2RecvCh;
                if (u2p->length == 0)  //0×Ö½Ú  Êý¾Ý²»½ÓÊÕÖ±½ÓºÍÐ£Ñé
                {
                    u2StaMach = 4;
                }
                else if (u2p->length == 208)            //208×Ö½Ú»òÆäËû  ½øÈëÊý¾Ý½ÓÊÕ
                {
                    u2StaMach = 3;
                }
                else    //ÆäËû×Ö½ÚÉáÆú
                    u2StaMach = 0;
                u2p->dataIndex = 0;
                u2p->checksum += u2RecvCh;
         }

        else if (u2StaMach == 3)  //data recieve
        {
            u2p->data[u2p->dataIndex++] = u2RecvCh;
            u2p->checksum += u2RecvCh;
            if (u2p->dataIndex == u2p->length)
            {
                u2StaMach = 4;
                u2p->dataIndex=0;
            }
        }

        else if (u2StaMach == 4)
        {
             // ÅÐ¶ÏÐ£ÑéºÍ
            if((u2p->checksum)%256 == u2RecvCh)  //Ð£ÑéºÍMOD 256
            {
                u2StaMach = 5;
            }
            else
            u2StaMach = 0;
        }
        else if (u2StaMach == 5)
        {
             u2p->end = u2RecvCh;
             // ÅÐ¶ÏEND
             if (learn_cmd==1)  //Ñ§Ï°¿ªÊ¼±êÖ¾Î»ÔÊÐíÑ§Ï°
          {
          //   learn_cmd=0;//Çå¿Õ¿ªÊ¼Ñ§Ï°±êÖ¾Î»
              Timout_Count=TIMOUT_20s;  //  ³¬Ê±¼ì²â±êÖ¾¸´Î»
             if(((u2p->command)==0x86)||((u2p->command)==0x8c))  //Ð­Òéend  µçÊÓÑ§Ï°³É¹¦ 0x86  ¿Õµ÷Ñ§Ï°³É¹¦0x8c
            {
                    if((learn_DeviceName>=1)&&(learn_DeviceName<=10))//¿Õµ÷ 1-10¿Õµ÷
                    {
                        if((learn_KeyCode>=1)&&(learn_KeyCode<=10))  //Ã¿¸ö¿Õµ÷¹Ì¶¨10¼ü
                        {
                         // Flash_Write(ARC_ADDR,u2p->data,208);//¶ÁÈ¡buf´æÈëFLASH   ´æÈë¹Ì¶¨Æ÷¼þµØÖ·µÄ¼üÖµÉÏARC1_ADDR+learn_DeviceName*2080+learn_KeyCode*208
                             Learn_State=1;//¿Õµ÷
                            OSQPost(TestMBox, (void*)Stu_Success); //Ñ§Ï°³É¹¦ÐÅºÅ
                        }
                    }
                   else if ((learn_DeviceName>=11)&&(learn_DeviceName<=20)) // µçÊÓ  10-19 µçÊÓ
                   {
                       if((learn_KeyCode>=1)&&(learn_KeyCode<=30)) //Ã¿¸öµçÊÓ¹Ì¶¨30¼ü
                      {
                       //  Flash_Write(URC_ADDR,u2p->data,208);//¶ÁÈ¡buf´æÈëFLASH
                           Learn_State=2;//µçÊÓ
                         OSQPost(TestMBox, (void*)Stu_Success); //Ñ§Ï°³É¹¦ÐÅºÅ
                      }
                   }
                   else   //³¬³ö¼üÖµ·¶Î§
                   {
                       OSQPost(TestMBox, (void*)Stu_Fail); //Ñ§Ï°Ê§°ÜÐÅºÅ
                   }
                u2StaMach = 0;
            }

            else if (((u2p->command)==0xc6)||((u2p->command)==0xcc))  //   µçÊÓÑ§Ï°Ê§°Ü0xc6  ¿Õµ÷Ñ§Ï°Ê§°Ü0xcc
            {
             OSQPost(TestMBox, (void*)Stu_Fail); //Ñ§Ï°Ê§°ÜÐÅºÅ
             u2StaMach = 0;
            }
            /*
             else if((u2p->command==0x8d)||(u2p->command==0x87))//Èç¹ûÎª 87 8D´íÎó£¬ÖØÐÂ·µ»Ø½ÓÊÕ
            {
                u2StaMach = 0;
            }
            */
            else //ÆäËû´íÎó
            {
             OSQPost(TestMBox, (void*)(u2p->command)); //Ñ§Ï°Ê§°ÜÐÅºÅ
             u2StaMach = 0;
            }
          //  learn_cmd=0;//Çå¿Õ¿ªÊ¼Ñ§Ï°±êÖ¾Î»
          }
              u2StaMach = 0;
        }
       //Send2Char(u2RecvCh);
    }
  USART_ClearITPendingBit(USART2,USART_IT_RXNE);
}

#endif

//Êý×é¶¨Òå£¬º¬ÒåÈçÌâÃû£º

u8 USART2_SEND_DATA[220];
u8 USART2_RECEIVE_DATA[220];
u8 USART2_TX_Finish=1; // USART2·¢ËÍÍê³É±êÖ¾Î»

void USART2_IRQHandler(void)
{
    u32 cnt=0;
    u16 DATA_LEN;
    u8 USART2_ProBuf_DATA[220];
    u16 i;
    if(USART_GetITStatus(USART2, USART_IT_IDLE) != RESET) //Èç¹ûÎª¿ÕÏÐ×ÜÏßÖÐ¶Ï
    {
        DMA_Cmd(DMA1_Channel6, DISABLE);//¹Ø±ÕDMA,·ÀÖ¹´¦ÀíÆä¼äÓÐÊý¾Ý
        //USART_RX_STA = USART1->SR;//ÏÈ¶ÁSR£¬È»ºó¶ÁDR²ÅÄÜÇå³ý
        //USART_RX_STA = USART1->DR;
        DATA_LEN=512-DMA_GetCurrDataCounter(DMA1_Channel6);
        if(DATA_LEN > 0)
        {
            while(USART2_TX_Finish==0)//µÈ´ýÊý¾ÝÍê³É²ÅÏÂÒ»´Î
            {
                               cnt++;
                if (cnt>0x70000)
                {
                    cnt=0;break;
                }
            }
                        cnt=0;
            //½«Êý¾ÝËÍDMA´æ´¢µØÖ·
            for(i=0;i<DATA_LEN;i++)
            {
               USART2_ProBuf_DATA[i]=USART2_RECEIVE_DATA[i];
            }
           ReV_CH361ReVProtocol(USART2_ProBuf_DATA);//½ÓÊÕÐ­Òé½âÎö
#if 0
            //USARTÓÃDMA´«ÊäÌæ´ú²éÑ¯·½Ê½·¢ËÍ£¬¿Ë·þ±»¸ßÓÅÏÈ¼¶ÖÐ¶Ï¶ø²úÉú¶ªÖ¡ÏÖÏó¡£
            DMA_Cmd(DMA1_Channel7, DISABLE); //¸Ä±ädatasizeÇ°ÏÈÒª½ûÖ¹Í¨µÀ¹¤×÷
            DMA1_Channel7->CNDTR=DATA_LEN; //DMA1,´«ÊäÊý¾ÝÁ¿
            USART2_TX_Finish=0;//DMA´«Êä¿ªÊ¼±êÖ¾Á¿
            DMA_Cmd(DMA1_Channel7, ENABLE);
#endif
        }
        //DMA_Cmd(DMA1_Channel5, DISABLE);//¹Ø±ÕDMA,·ÀÖ¹´¦ÀíÆä¼äÓÐÊý¾Ý
        DMA_ClearFlag(DMA1_FLAG_GL6 | DMA1_FLAG_TC6 | DMA1_FLAG_TE6 | DMA1_FLAG_HT6);//Çå±êÖ¾
        DMA1_Channel6->CNDTR = 512;//ÖØ×°Ìî
        DMA_Cmd(DMA1_Channel6, ENABLE);//´¦ÀíÍê,ÖØ¿ªDMA
        //¶ÁSRºó¶ÁDRÇå³ýIdle
        i = USART2->SR;
        i = USART2->DR;
    }
    if(USART_GetITStatus(USART2, USART_IT_PE | USART_IT_FE | USART_IT_NE) != RESET)//³ö´í
    {
        USART_ClearITPendingBit(USART2, USART_IT_PE | USART_IT_FE | USART_IT_NE);
    }
    USART_ClearITPendingBit(USART2, USART_IT_TC);
    USART_ClearITPendingBit(USART2, USART_IT_IDLE);

}



//DMA1_Channel6ÖÐ¶Ï·þÎñº¯Êý
void DMA1_Channel6_IRQHandler(void)
{
  DMA_ClearITPendingBit(DMA1_IT_TC6);
  DMA_ClearITPendingBit(DMA1_IT_TE6);
  DMA_Cmd(DMA1_Channel6, DISABLE);//¹Ø±ÕDMA,·ÀÖ¹´¦ÀíÆä¼äÓÐÊý¾Ý
  DMA1_Channel6->CNDTR = 580;//ÖØ×°Ìî
  DMA_Cmd(DMA1_Channel6, ENABLE);//´¦ÀíÍê,ÖØ¿ªDMA
}


//DMA1_Channel7ÖÐ¶Ï·þÎñº¯Êý
//USART2Ê¹ÓÃDMA·¢Êý¾ÝÖÐ¶Ï·þÎñ³ÌÐò
void DMA1_Channel7_IRQHandler(void)
{
  DMA_ClearITPendingBit(DMA1_IT_TC7);
  DMA_ClearITPendingBit(DMA1_IT_TE7);
  DMA_Cmd(DMA1_Channel7, DISABLE);//¹Ø±ÕDMA
  USART2_TX_Finish=1;//ÖÃDMA´«ÊäÍê³É
}


/********************************************************************************
 * FunctionName: TIM4_IRQHandler
 *
 * Description : ¶¨Ê±Æ÷4µÄPWM²¶»ñÖÐ¶Ï£¬PWMÊäÈëÄ£Ê½¬£²¨³¤×ÔÊÊÓ¦
 *
 * Parameters  : None.
 *
 * Returns     : None.
 *******************************************************************************/
void TIM4_IRQHandler(void)
{

}

/********************************************************************************
 * FunctionName: TIM2_IRQHandler
 *
 * Description : ¶¨Ê±Æ÷2µÄPWM²¶»ñÖÐ¶Ï£¬PWMÊäÈëÄ£Ê½£¬PT2262½âÂë¡£²¨³¤×ÔÊÊÓ¦
 *
 * Parameters  : None.
 *
 * Returns     : None.
 *******************************************************************************/
void TIM2_IRQHandler(void)
{

}


/********************************************************************************
 * FunctionName: TIM1_CC_IRQHandler
 *
 * Description : TIM1µÄ±È½ÏÆ¥ÅäÖÐ¶Ï
 *
 * Parameters  : None.
 *
 * Returns     : None.
 *******************************************************************************/
void TIM1_CC_IRQHandler(void)
{
    if (TIM_GetFlagStatus(TIM1,TIM_FLAG_CC1))
    {
        TIM_ClearITPendingBit(TIM1,TIM_IT_CC1);
    }
}

/********************************************************************************
 * FunctionName: EXTI15_10_IRQHandler
 *
 * Description : SI4432  SIGNAL
 *
 * Parameters  : None.
 *
 * Returns     : None.
 *******************************************************************************/
void EXTI15_10_IRQHandler(void)
{

 //  SendString("EXTI_IRQ1\n");
    if(EXTI_GetITStatus(EXTI_Line11) != RESET )
    {
        if (si4432_rev_en==1)
        {
            OSQPost(Si4432ReadMBox, (void *)0x01); //4432½ÓÊÕÒ»Ö¡
        }

            EXTI_ClearITPendingBit(EXTI_Line11);
    }
}

void EXTI2_IRQHandler(void)
{

}

