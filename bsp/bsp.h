#ifndef __BSP_H
#define __BSP_H

#ifdef GLOBALS_RFIR
#define BSP_EXTERN
#else
#define BSP_EXTERN extern
#endif


extern u8 USART2_TX_Finish;
extern u8 USART2_SEND_DATA[];
extern u8 USART2_RECEIVE_DATA[];
extern u16 Timout_Count2;
void BSP_Init(void);
void SysTickInit(void);
void DMA_Configuration(void);





#endif // __BSP_H


