#ifndef __STM32INIT_H
#define __STM32INIT_H

extern EXTI_InitTypeDef EXTI_InitStructure;



extern void STM32_Init(void);
extern void MyRCCInit(void);
extern void MyExtiNvicInit(void);
extern void MyUsartInit(void);
extern void mySPI_Init(void);
extern void myEXIT_Init(void);
extern void SI4432_GPIOInit(void);
extern void MyLedInit(void);
//extern void GetUniqueID(void);


#define LED_Learn(x)     x ?     GPIO_ResetBits(GPIOB,GPIO_Pin_9) : GPIO_SetBits(GPIOB,GPIO_Pin_9)//ºìµÆ
#define LED_Send(x)      x ?     GPIO_ResetBits(GPIOB,GPIO_Pin_8) : GPIO_SetBits(GPIOB,GPIO_Pin_8)//ÂÌµÆ




#endif

