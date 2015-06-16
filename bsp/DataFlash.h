/*-------------------DataFlash.h----------------------------------*/

#ifndef   __DATAFLASH_H__
#define   __DATAFLASH_H__

#include "stm32f10x.h"
#include "stm32f10x_flash.h"

#if defined (STM32F10X_HD) || defined (STM32F10X_HD_VL) || defined (STM32F10X_CL) || defined (STM32F10X_XL)
  #define FLASH_PAGE_SIZE    ((uint16_t)0x800)
#else
  #define FLASH_PAGE_SIZE    ((uint16_t)0x400)
#endif

int Flash_Read(uint32_t iAddress, uint8_t *buf, int32_t iNbrToRead) ;
int Flash_Write(uint32_t iAddress, uint8_t *buf, uint32_t iNbrToWrite);
void Message_Flash_Write(int flag_save,uint8_t *message);
void Message_Flash_Read(int flag_save,uint8_t *message);

#endif



