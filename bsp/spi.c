
#include "includes.h"


//spi 写一个字节
u8 SPI_WriteByte(SPI_TypeDef* SPIx,u8 TxData)
{
  //  while((SPIx->SR&SPI_I2S_FLAG_TXE)==RESET);      //等待发送区空
  //  SPIx->DR=TxData;      //发送一个byte
  //  while((SPIx->SR&SPI_I2S_FLAG_RXNE)==RESET);//等待接收完一个byte
   // return SPIx->DR;                 //返回收到的数据
    u8 retry=0;
#if  0
        /* 循环检测发送缓冲区是否是空 */
    while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET);

    /*通过SPI3外设发出数据*/
    SPI_I2S_SendData(SPIx,Byte);

    /* 等待接收数据，循环检查接收数据缓冲区 */
    while (SPI_I2S_GetFlagStatus(SPIx,SPI_I2S_FLAG_RXNE) == RESET);

    /* 返回读出的数据 */
    return SPI_I2S_ReceiveData(SPIx);
#endif
    while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_TXE) == RESET) //检查指定的SPI标志位设置与否:发送缓存空标志位
        {
        retry++;
        if(retry>200)return 0;
        }
    SPI_I2S_SendData(SPIx, TxData); //通过外设SPIx发送一个数据
    retry=0;

    while (SPI_I2S_GetFlagStatus(SPIx, SPI_I2S_FLAG_RXNE) == RESET) //检查指定的SPI标志位设置与否:接受缓存非空标志位
        {
        retry++;
        if(retry>200)return 0;
        }
    return SPI_I2S_ReceiveData(SPIx); //返回通过SPIx最近接收的数据


}




//初始化spi1
void SPI1_Init(void) //供给vs1003b,spi flash
{
    SPI_InitTypeDef  SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
   //  SPI_Cmd(SPI1, DISABLE);



    RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO |RCC_APB2Periph_GPIOA , ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5  ;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6  ;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_7 ;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
  GPIO_SetBits(GPIOA,GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7); //GPIOA 5 6 7上拉
 //RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1 ,ENABLE);

    SPI_Cmd(SPI1, DISABLE);
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;//SPI_CPHA_2Edge
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(SPI1, &SPI_InitStructure);
    SPI_Cmd(SPI1, ENABLE);
}







void SPI2_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    SPI_InitTypeDef  SPI_InitStructure;

    RCC_APB2PeriphClockCmd( RCC_APB2Periph_GPIOA|RCC_APB1Periph_SPI2, ENABLE );//GPIOA 时钟使能//SPI1 时钟使能

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //GPIOA 5 6 7复用推挽输出
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIOA
    GPIO_SetBits(GPIOA,GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7); //GPIOA 5 6 7上拉
     //  LOW 2 有反应 测试多次 注意接口按紧
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //设置SPI单向或者双向的数据模式:SPI设置为双线双向全双工
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;       //设置为主SPI
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;       //SPI发送接收8位帧结构
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;      //串行同步时钟的空闲状态为 低电平
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;    //串行同步时钟的第1个跳变沿（上升或下降）数据被采样
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;       //NSS信号由硬件（NSS管脚）还是软件（使用SSI位）管理:内部NSS信号有SSI位控制
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_256;        //定义波特率预分频的值:波特率预分频值为256
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;  //指定数据传输从MSB位还是LSB位开始:数据传输从MSB位开始
    SPI_InitStructure.SPI_CRCPolynomial = 7;    //CRC值计算的多项式
    SPI_Init(SPI2, &SPI_InitStructure);  //根据SPI_InitStruct中指定的参数初始化外设SPIx寄存器
    SPI_Cmd(SPI2, ENABLE); //使能SPI外设

    SPI2_ReadWriteByte(0xff);//启动传输


}
//SPI 速度设置函数
//SpeedSet:
//SPI_BaudRatePrescaler_2   2分频
//SPI_BaudRatePrescaler_8   8分频
//SPI_BaudRatePrescaler_16  16分频
//SPI_BaudRatePrescaler_256 256分频

void SPI1_SetSpeed(u8 SPI_BaudRatePrescaler)
{
    assert_param(IS_SPI_BAUDRATE_PRESCALER(SPI_BaudRatePrescaler));
    SPI1->CR1&=0XFFC7;
    SPI1->CR1|=SPI_BaudRatePrescaler;   //设置SPI1速度
    SPI_Cmd(SPI1,ENABLE);

}

//SPIx 读写一个字节
//TxData:要写入的字节
//返回值:读取到的字节
u8 SPI2_ReadWriteByte(u8 TxData)
{
    u8 retry=0;
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET) //检查指定的SPI标志位设置与否:发送缓存空标志位
        {
        retry++;
        if(retry>200)return 0;
        }
    SPI_I2S_SendData(SPI2, TxData); //通过外设SPIx发送一个数据
    retry=0;

    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_RXNE) == RESET) //检查指定的SPI标志位设置与否:接受缓存非空标志位
        {
        retry++;
        if(retry>200)return 0;
        }
    return SPI_I2S_ReceiveData(SPI2); //返回通过SPIx最近接收的数据
}




unsigned char spi_send_byte(unsigned char byte)
{
    /* 循环检测发送缓冲区是否是空 */
    while (SPI_I2S_GetFlagStatus(SPI2, SPI_I2S_FLAG_TXE) == RESET);

    /*通过SPI3外设发出数据*/
    SPI_I2S_SendData(SPI2,byte);

    /* 等待接收数据，循环检查接收数据缓冲区 */
    while (SPI_I2S_GetFlagStatus(SPI2,SPI_I2S_FLAG_RXNE) == RESET);

    /* 返回读出的数据 */
    return SPI_I2S_ReceiveData(SPI2);
}

























