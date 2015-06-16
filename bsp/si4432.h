/*
*****************************************************************************
* Copyright (c) 2014, kunshan Robotechn Intelligent Technology, CO., LTD.
* All Rights Reserved.
*
* File Name   : SI4432.h
* Abstract    : SI4432  无线模块
* Author      : wuwei
*
* History
* date          author      notes
* 2014-01-07    wuwei       File Created.
*
*****************************************************************************
*/

#ifndef __SI_4432
#define __SI_4432

#ifdef GLOBALS_RFIR
#define SPI_EXTERN
#else
#define SPI_EXTERN extern
#endif

// 纭欢SPI鎺ュ彛
#define HARDWARE_SPI        0
// 閲囩敤杞欢GPIO鍙ｆā鎷烻PI鎺ュ彛
#define SOFTWARE_SPI        1

#define SPI_TYPE            HARDWARE_SPI

#define SI4432_NSEL_HIGH    GPIO_SetBits(GPIOB, GPIO_Pin_12)  //SI4432_NSEL  禁止
#define SI4432_NSEL_LOW     GPIO_ResetBits(GPIOB, GPIO_Pin_12)  //SI4432_NSEL  使能

#define SI4432_SDN_HIGH   GPIO_SetBits(GPIOB, GPIO_Pin_10)   //SI4432_SDN   使能
#define SI4432_SDN_LOW    GPIO_ResetBits(GPIOB, GPIO_Pin_10)   //SI4432_SDN   使能

#define SI4432_NIRQ       GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11)  //SI4432_NIRQ  中断状态
extern u8 si4432_rev_en;
extern u8 ItStatus1,ItStatus2,res;
extern u8 SI4432_RxBUFF[];
extern u8 SI4432_RxCount;
extern u8  Si4432Read_Status ;   //SI4432读识别标志位
extern u8  Si4432Write_Status ;   //SI4432写识别标志位
extern u8 hombot_buf[12];
extern u8 SI4432_RxBUFF[];

u8   SI4432_ReadReg(u8  addr) ;  //读取寄存器的值
void SI4432_WriteReg(u8 addr, u8 value) ; //写寄存器
void SI4432_GPIOSet(void);
void SI4432_Configuration(void );
void SI4432_Init(void)  ;//配置


//void sending(u8  buf[]);
void sending(u8 *buf ,u8 data_legth); //发送的数据，长度

//unsigned short SpiReadWriteWord(unsigned short spi_in);

#endif
