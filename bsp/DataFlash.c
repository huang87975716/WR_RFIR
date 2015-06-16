/************************ --昆山瑞泰科技有限公司 -- ************************
 * 文件名  ：dataflash.c
 * 描述    :flash读写
 * 软件版本：DV0.1
 * 硬件版本：RV1.0
 *
 * 作者    ：wuwei
 * 创建日期：2014.02.27
 * 修改日期：
**********************************************************************************/

#include "includes.h"

/********************************************************************************
 * FunctionName: Flash_Write_Without_check
 *
 * Description :
 *
 * Parameters  :
 *
 * Returns     :
 *******************************************************************************/
uint16_t Flash_Write_Without_check(uint32_t iAddress, uint8_t *buf, uint16_t iNumByteToWrite)
{
    uint16_t i = 0;
    volatile FLASH_Status FLASHStatus = FLASH_COMPLETE;

    while((i < iNumByteToWrite) && (FLASHStatus == FLASH_COMPLETE))
    {
      FLASHStatus = FLASH_ProgramHalfWord(iAddress, *(uint16_t*)buf);
      i = i+2;
      iAddress = iAddress + 2;
      buf = buf + 2;
    }

    return iNumByteToWrite;
}
/********************************************************************************
 * FunctionName: Flash_Write
 *
 * Description :
 *
 * Parameters  :
 *
 * Returns     :
 *
 * @brief  Programs a half word at a specified Option Byte Data address.
 * @note   This function can be used for all STM32F10x devices.
 * @param  Address: specifies the address to be programmed.
 * @param  buf: specifies the data to be programmed.
 * @param  iNbrToWrite: the number to write into flash
 * @retval if success return the number to write, -1 if error

// iAddress：保存EEPROM数据的起始地址（0~）
// iNbrToRead：保存数据长度，要求数据长度小于一页
// *buf：保存数据缓存指针
********************************************************************************/
int Flash_Write(uint32_t iAddress, uint8_t *buf, uint32_t iNbrToWrite)
{
    volatile FLASH_Status FLASHStatus = FLASH_COMPLETE;

    /* Unlock the Flash Bank1 Program Erase controller */
    uint32_t secpos;
    uint32_t iNumByteToWrite = iNbrToWrite;
    uint16_t secoff;
    uint16_t secremain;

    uint16_t i = 0;
    uint8_t tmp[FLASH_PAGE_SIZE];

    FLASH_UnlockBank1();
    secpos=iAddress & (~(FLASH_PAGE_SIZE -1 )) ;//扇形地址
    secoff=iAddress & (FLASH_PAGE_SIZE -1);     //在扇区内的偏移
    secremain=FLASH_PAGE_SIZE-secoff;           //扇区剩余空间大小


    /*不大于4096个字节*/
    if(iNumByteToWrite<=secremain) secremain = iNumByteToWrite;

    while( 1 )
    {
        /*读出整个扇区*/
        Flash_Read(secpos, tmp, FLASH_PAGE_SIZE);
        /*校验数据*/
        for(i=0;i<secremain;i++)
        {
            if(tmp[secoff+i]!=0XFF)//需要擦除
            {
                break;
            }
        }
        /*需要擦除*/
        if(i < secremain)
        {
            /*擦除这个扇区*/
            FLASHStatus = FLASH_ErasePage(secpos);
            if(FLASHStatus != FLASH_COMPLETE)
            {
                return -1;
            }
            /*复制*/
            for(i=0;i<secremain;i++)
            {
                tmp[i+secoff]=buf[i];
            }
            /*写入整个扇区*/
            Flash_Write_Without_check(secpos ,tmp ,FLASH_PAGE_SIZE);
        }
        /*写已经擦除了的，直接写入扇区剩余区间*/
        else
        {
            Flash_Write_Without_check(iAddress,buf,secremain);
        }

        /*写入结束*/
        if(iNumByteToWrite == secremain)
        {
            break;
        }
        else
        {
            secpos += FLASH_PAGE_SIZE;
            secoff = 0;             //偏移位置为0
            buf += secremain;       //指针偏移
            iAddress += secremain;  //写地址偏移
            iNumByteToWrite -= secremain; //字节数递减

            if(iNumByteToWrite > FLASH_PAGE_SIZE)
            {
                secremain = FLASH_PAGE_SIZE;    /*下一个扇区还是写不完*/
            }
            else
            {
                secremain = iNumByteToWrite;    /*下一个扇区可以写完了*/
            }
        }
    }
    FLASH_LockBank1();
    return iNbrToWrite;
}
/********************************************************************************
 * FunctionName: Flash_Read
 *
 * Description :
 *
 * Parameters  :
 *
 * Returns     :
 *
 * @brief  Programs a half word at a specified Option Byte Data address.
 * @note   This function can be used for all STM32F10x devices.
 * @param  Address: specifies the address to be programmed.
 * @param  buf: specifies the data to be programmed.
 * @param  iNbrToWrite: the number to read from flash.
 * @retval if success return the number to write, without error
// iAddress：写EEPROM数据的起始地址（0~）
// iNbrToRead：写数据长度，要求数据长度小于一页
// *buf：写数据缓存指针



********************************************************************************/
int Flash_Read(uint32_t iAddress, uint8_t *buf, int32_t iNbrToRead)
{
    int i = 0;

    while(i < iNbrToRead )
    {
        *(buf + i) = *(__IO uint8_t*) iAddress++;
        i++;
    }
    return i;
}



#if 0
/********************************************************************************
 * FunctionName: Message_Flash_Write
 *
 * Description :
 *
 * Parameters  :
 *
 * Returns     :
 *******************************************************************************/
void Message_Flash_Write(int flag_save,uint8_t *message)
{
    if(flag_save == 1)
    {
        Flash_Write(Message_Save1,message,Message_Size);
    }
    else if(flag_save == 2)
    {
        Flash_Write(Message_Save2, message, Message_Size);
    }
    else if(flag_save == 3)
    {
        Flash_Write(Message_Save3, message, Message_Size);
    }
    else if(flag_save == 4)
    {
        Flash_Write(Message_Save4, message, Message_Size);
    }
    else if(flag_save == 5)
    {
        Flash_Write(Message_Save5, message, Message_Size);
    }
    else if(flag_save == 6)
    {
        Flash_Write(Message_Save6, message, Message_Size);
    }
    else if(flag_save == 7)
    {
        Flash_Write(Message_Save7, message, Message_Size);
    }
    else if(flag_save == 8)
    {
        Flash_Write(Message_Save8, message, Message_Size);
    }
    else if(flag_save == 9)
    {
        Flash_Write(Message_Save9, message, Message_Size);
    }
    else if(flag_save == 10)
    {
        Flash_Write(Message_Save10, message, Message_Size);
    }
    else if(flag_save == 11)
    {
        Flash_Write(Message_Save11, message, Message_Size);
    }
    else if(flag_save == 12)
    {
        Flash_Write(Message_Save12, message, Message_Size);
    }
    else if(flag_save == 13)
    {
        Flash_Write(Message_Save13, message, Message_Size);
    }
    else if(flag_save == 14)
    {
        Flash_Write(Message_Save14,message,Message_Size);
    }
    else if(flag_save == 15)
    {
        Flash_Write(Message_Save15, message, Message_Size);
    }
    else if(flag_save == 16)
    {
        Flash_Write(Message_Save16, message, Message_Size);
    }
    else if(flag_save == 17)
    {
        Flash_Write(Message_Save17, message, Message_Size);
    }
    else if(flag_save == 18)
    {
        Flash_Write(Message_Save18, message, Message_Size);
    }
    else if(flag_save == 19)
    {
        Flash_Write(Message_Save19, message, Message_Size);
    }
    else if(flag_save == 20)
    {
        Flash_Write(Message_Save20, message, Message_Size);
    }
    #if 0
    switch(flag_save)
    {
        case 1:
            Flash_Write(Message_Save1,message,Message_Size);
            break;
        case 2:
            Flash_Write(Message_Save2,message,Message_Size);
            break;
        case 3:
            Flash_Write(Message_Save3,message,Message_Size);
            break;
        case 4:
            Flash_Write(Message_Save4,message,Message_Size);
            break;
        case 5:
            Flash_Write(Message_Save5,message,Message_Size);
            break;
        case 6:
            Flash_Write(Message_Save6,message,Message_Size);
            break;
        case 7:
            Flash_Write(Message_Save7,message,Message_Size);
            break;
        case 8:
            Flash_Write(Message_Save8,message,Message_Size);
            break;
        case 9:
            Flash_Write(Message_Save9,message,Message_Size);
            break;
        case 10:
            Flash_Write(Message_Save10,message,Message_Size);
            break;
        case 11:
            Flash_Write(Message_Save11,message,Message_Size);
            break;
        case 12:
            Flash_Write(Message_Save12,message,Message_Size);
            break;
        case 13:
            Flash_Write(Message_Save13,message,Message_Size);
            break;
        case 14:
            Flash_Write(Message_Save14,message,Message_Size);
            break;
        case 15:
            Flash_Write(Message_Save15,message,Message_Size);
            break;
        case 16:
            Flash_Write(Message_Save16,message,Message_Size);
            break;
        case 17:
            Flash_Write(Message_Save17,message,Message_Size);
            break;
        case 18:
            Flash_Write(Message_Save18,message,Message_Size);
            break;
        case 19:
            Flash_Write(Message_Save19,message,Message_Size);
            break;
        case 20:
            Flash_Write(Message_Save20,message,Message_Size);
            break;
        default:
            break;
    }
    #endif
}
/********************************************************************************
 * FunctionName: Message_Flash_Read
 *
 * Description :
 *
 * Parameters  :
 *
 * Returns     :
 *******************************************************************************/
void Message_Flash_Read(int flag_save,uint8_t *message)
{
    if(flag_save == 1)
    {
        Flash_Read(Message_Save1,message,Message_Size);
    }
    else if(flag_save == 2)
    {
        Flash_Read(Message_Save2, message, Message_Size);
    }
    else if(flag_save == 3)
    {
        Flash_Read(Message_Save3, message, Message_Size);
    }
    else if(flag_save == 4)
    {
        Flash_Read(Message_Save4, message, Message_Size);
    }
    else if(flag_save == 5)
    {
        Flash_Read(Message_Save5, message, Message_Size);
    }
    else if(flag_save == 6)
    {
        Flash_Read(Message_Save6, message, Message_Size);
    }
    else if(flag_save == 7)
    {
        Flash_Read(Message_Save7, message, Message_Size);
    }
    else if(flag_save == 8)
    {
        Flash_Read(Message_Save8, message, Message_Size);
    }
    else if(flag_save == 9)
    {
        Flash_Read(Message_Save9, message, Message_Size);
    }
    else if(flag_save == 10)
    {
        Flash_Read(Message_Save10, message, Message_Size);
    }
    else if(flag_save == 11)
    {
        Flash_Read(Message_Save11, message, Message_Size);
    }
    else if(flag_save == 12)
    {
        Flash_Read(Message_Save12, message, Message_Size);
    }
    else if(flag_save == 13)
    {
        Flash_Read(Message_Save13, message, Message_Size);
    }
    else if(flag_save == 14)
    {
        Flash_Read(Message_Save14,message,Message_Size);
    }
    else if(flag_save == 15)
    {
        Flash_Read(Message_Save15, message, Message_Size);
    }
    else if(flag_save == 16)
    {
        Flash_Read(Message_Save16, message, Message_Size);
    }
    else if(flag_save == 17)
    {
        Flash_Read(Message_Save17, message, Message_Size);
    }
    else if(flag_save == 18)
    {
        Flash_Read(Message_Save18, message, Message_Size);
    }
    else if(flag_save == 19)
    {
        Flash_Read(Message_Save19, message, Message_Size);
    }
    else if(flag_save == 20)
    {
        Flash_Read(Message_Save20, message, Message_Size);
    }
}

#endif

