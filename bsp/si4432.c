/*
*****************************************************************************
* Copyright (c) 2014, kunshan Robotechn Intelligent Technology, CO., LTD.
* All Rights Reserved.
*
* File Name   : si4432.c
* Abstract    : SI4432  无线模块
* Author      : wuwei
*
* History
* date          author      notes
* 2014-01-07    wuwei      File Created.
*
*****************************************************************************
*/


#include "includes.h"
FlagType  aa;
u8 SI4432_RxBUFF[20];
u8 SI4432_RxCount;
u8 ItStatus1,ItStatus2,res;
u8 si4432_rev_en;
//u8 hombot_buf[12]= {"hombot1"} ;
// 读取寄存器

u8  SI4432_ReadReg(u8  addr)   //读取寄存器的值
{
    uint8_t temp = 0;
    SI4432_NSEL_LOW;
    SPI2_ReadWriteByte(addr);    //发送读取寄存器地址
    temp = SPI2_ReadWriteByte(0Xff);
    SI4432_NSEL_HIGH;
    return temp;
}
//写寄存器
void SI4432_WriteReg(u8 addr, u8 value)  //写寄存器
{
    SI4432_NSEL_LOW;
    spi_send_byte(addr | 0x80);
    spi_send_byte(value);
    SI4432_NSEL_HIGH;
}

void SI4432_Configuration(void)
{
    si4432_rev_en = 0;
    SI4432_GPIOSet();
    SI4432_SDN_HIGH;
    DelayMs(100);
    SI4432_SDN_LOW;
    SI4432_Init();
    si4432_rev_en = 1;
}

void SI4432_GPIOSet(void)
{   SPI_InitTypeDef  SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE); //A,C时钟使能

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;  // Pb12 = NSEL 推挽
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_SetBits(GPIOB, GPIO_Pin_12);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;  // PB12 =SI_SDN    推挽
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;  //推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    GPIO_SetBits(GPIOB, GPIO_Pin_10);


    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_11; //配置成外部中断模式NIRQ 产生中断时为高
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    EXTI_InitStructure.EXTI_Line = EXTI_Line11;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource11);
    EXTI_ClearITPendingBit(EXTI_Line11);

    SI4432_NSEL_HIGH;
    SI4432_SDN_LOW;
    DelayMs(10);


    //spi端口配置
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);       //使能SPI2外设时钟
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB | RCC_APB2Periph_AFIO, ENABLE);       //使能SPI2外设时钟
                                                                                      //GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);

    //SCK/MISO/MOSI
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
    //spi功能配置
    SPI_Cmd(SPI2, DISABLE);
    /* SPI2配置 */
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;   //全双工
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;                          //主模式
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;                      //8位
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;                         //时钟极性 空闲状态时，SCK保持低电平
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;                           //时钟相位 数据采样从第一个时钟边沿开始
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;                              //软件产生NSS
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_128;   //波特率控制 SYSCLK/128
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;                 //数据高位在前
    SPI_InitStructure.SPI_CRCPolynomial = 7;                               //CRC多项式寄存器初始值为7
    SPI_Init(SPI2, &SPI_InitStructure);
    /* 使能SPI2 */
    SPI_Cmd(SPI2, ENABLE);

}




void SI4432_Init(void)  //配置
{  u32 i = 0;
    SI4432_NSEL_LOW;
    DelayMs(20);
//读取中断状态   清除中断  释放  SI4432_NIRQ
    ItStatus1 = SI4432_ReadReg(0x03);
    ItStatus2 = SI4432_ReadReg(0x04);
//软件复位
    SI4432_WriteReg(0x07, 0x80); //向0X07地址  写入0X80  软件复位
                                 // SendString("init1");
                                 //while ( SI4432_NIRQ == 1);
    while(SI4432_NIRQ == 1)
    {
        //  i=i;
        i++;
        if(i > 0x70000)
        {
            i = 0;
            break;
        }
    }
    i = 0;
    // SendString("init2");
    //读取中断状态   清除中断  释放  SI4432_NIRQ
    ItStatus1 = SI4432_ReadReg(0x03);
    ItStatus2 = SI4432_ReadReg(0x04);
/////////////////////////开始设置
// 频率设置 434
    SI4432_WriteReg(0x75, 0x53);//53
    SI4432_WriteReg(0x76, 0x64);  //
    SI4432_WriteReg(0x77, 0x00);
// 1.2K bps 发射速率
    SI4432_WriteReg(0x2a, 0x14);
    SI4432_WriteReg(0x6e, 0x09);
    SI4432_WriteReg(0x6f, 0xd5);
    SI4432_WriteReg(0x70, 0x2c); //

//SpiWriteRegister(0x72, 0x48); //(9.6kbps)
    SI4432_WriteReg(0x72, 0x38);    //频率偏差(45KHZ)
// 下面的设置根据Silabs 的Excel (1.2 kbps, deviation: 45 kHz, channel filter BW: 102.2 kHz
    SI4432_WriteReg(0x1C, 0x1b);  //1b                                                          //write 0x1E to the IF Filter Bandwidth register
    SI4432_WriteReg(0x20, 0x83);                                                            //write 0xD0 to the Clock Recovery Oversampling Ratio register
    SI4432_WriteReg(0x21, 0xc0);                   	                                         //write 0x00 to the Clock Recovery Offset 2 register
    SI4432_WriteReg(0x22, 0x13);                                                            //write 0x9D to the Clock Recovery Offset 1 register
    SI4432_WriteReg(0x23, 0xa9);                                                            //write 0x49 to the Clock Recovery Offset 0 register
    SI4432_WriteReg(0x24, 0x00);                                                            //write 0x00 to the Clock Recovery Timing Loop Gain 1 register
    SI4432_WriteReg(0x25, 0x03);                                                            //write 0x24 to the Clock Recovery Timing Loop Gain 0 register
    SI4432_WriteReg(0x1D, 0x40);                                                            //write 0x40 to the AFC Loop Gearshift Override register
    SI4432_WriteReg(0x1E, 0x0A);                                                            //write 0x0A to the AFC Timing Control register
    SI4432_WriteReg(0x2A, 0x14);//14                    	                                        //write 0x20 to the AFC Limiter register
//前导码 同步字
    SI4432_WriteReg(0x34, 0X0A);  // 发射5字节的Preamble
    SI4432_WriteReg(0x35, 0x2A);  // 需要检测 Preamble
#if 0
    SI4432_WriteReg(0x33, 0x02); // 同步字3,2 是同步字
    SI4432_WriteReg(0x36, 0x44);  // 同步字为 0x2dd4
    SI4432_WriteReg(0x37, 0x55);
#endif
#if 1
    SI4432_WriteReg(0x33, 0x04); //  3字节
    SI4432_WriteReg(0x36, 0x72);  //同步字 0x726684
    SI4432_WriteReg(0x37, 0x66);
    SI4432_WriteReg(0x38, 0x84);
#endif
    SI4432_WriteReg(0x30, 0x8D);   // 使能PH+ FIFO模式，高位在前面，使能CRC校验 CCITTT
    SI4432_WriteReg(0x32, 0x00); // 禁止帧头
    SI4432_WriteReg(0x71, 0x63); // 发射不需要 CLK，FiFo ， FSK模式

//GPIO
    SI4432_WriteReg(0x0b, 0x12); //
    SI4432_WriteReg(0x0c, 0x15); //
//其他设置
    SI4432_WriteReg(0x09, 0xD7);  //  负载电容
    SI4432_WriteReg(0x69, 0x60);  //AGC过载
//发射功率
    SI4432_WriteReg(0x6d, 0x1e);
//手动打开接收
    SI4432_WriteReg(0x07, 0x05);
//打开 接收中断
    SI4432_WriteReg(0x05, 0x03);
    SI4432_WriteReg(0x06, 0x00);
//清中断
    ItStatus1 = SI4432_ReadReg(0x03);
    ItStatus2 = SI4432_ReadReg(0x04);
    SI4432_WriteReg(0x07, 0x05); //打开接收
    SI4432_WriteReg(0x0d, 0xf4); //GPIO2接收数据

}


void sending(u8 *buf, u8 data_legth)  //数据，长度，地址
{

    //  u8 Length=0;
    u32 i = 0;

    SI4432_WriteReg(0x07, 0x01);    // rf 模块进入Ready 模式
    DelayMs(10);

    SI4432_WriteReg(0x3e, data_legth); //Send_Legth);  // 读取字节数 ，总共发射Length个字节的数据
    for(i = 0; i < data_legth; i++)
    {
        SI4432_WriteReg(0x7F,  *(buf + i));
    }

    SI4432_WriteReg(0x05, 0x04);    // 整包数据发射完后，产生中断
    SI4432_WriteReg(0x06, 0x00);
    ItStatus1 = SI4432_ReadReg(0x03);
    ItStatus2 = SI4432_ReadReg(0x04);
    SI4432_WriteReg(0x07, 0x09); //打开发射
                                 // while ( SI4432_NIRQ == 1);
                                 //  DelayMs(120);
    while(SI4432_NIRQ == 1)
    {
        //  i=i;
        i++;
        if(i > 0x70000)
        {
            i = 0;
            break;
        }
    }
    i = 0;
    //read interrupt status registers to release the interrupt flags
    ItStatus1 = SI4432_ReadReg(0x03);
    ItStatus2 = SI4432_ReadReg(0x04);

    //after packet transmission set the interrupt enable bits according receiving mode
    //Enable two interrupts:
    // a) one which shows that a valid packet received: 'ipkval'
    // b) second shows if the packet received with incorrect CRC: 'icrcerror'
    SI4432_WriteReg(0x05, 0x03);                                                //write 0x03 to the Interrupt Enable 1 register
    SI4432_WriteReg(0x06, 0x00);                                                //write 0x00 to the Interrupt Enable 2 register
                                                                                //read interrupt status registers to release all pending interrupts
    ItStatus1 = SI4432_ReadReg(0x03);                                           //read the Interrupt Status1 register
    ItStatus2 = SI4432_ReadReg(0x04);                                           //read the Interrupt Status2 register
    /*enable receiver chain again*/
    SI4432_WriteReg(0x07, 0x05);

    //reset the RX FIFO
    SI4432_WriteReg(0x08, 0x02);                                                    //write 0x02 to the Operating Function Control 2 register
    SI4432_WriteReg(0x08, 0x00);                                                    //write 0x00 to the Operating Function Control 2 register
                                                                                    //enable the receiver chain again
    SI4432_WriteReg(0x07, 0x05);


}


void Enable_4432EXIT(void)
{   //GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource13);
    EXTI_InitStructure.EXTI_Line = EXTI_Line13;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    EXTI_ClearITPendingBit(EXTI_Line13);
}

void Disable_4432EXIT(void)
{  //  GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;

    GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource13);
    EXTI_InitStructure.EXTI_Line = EXTI_Line13;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = DISABLE;
    EXTI_Init(&EXTI_InitStructure);
    EXTI_ClearITPendingBit(EXTI_Line13);
}
