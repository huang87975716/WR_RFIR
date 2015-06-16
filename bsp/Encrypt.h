
#ifndef __encrypt_H
#define __encrypt_H

// #define  Tongbu_Test
#if !defined  Tongbu_Test

/**
 * @brief Comment the line below if you will not use the peripherals drivers.
   In this case, these drivers will not be included and the application code will
   be based on direct access to peripherals registers
   */
 // #define Tongbu_Test
#endif
//extern u8 test_buf[];


 //发送信号 状态 1：学习成功 2：学习失败 3：正在学习中

#define  Stu_Success 0x01
#define  Stu_Fail    0x02
#define  Stu_On 0x03

#define   Charger_ID Charger_ID1

//解密数据ID长度
#define ID_Legth 4

//定义保存ID数量
//#define SlaveID_Qty 4

//定义保存数据长度
//#define  SI4432_ID_SIZE   12


//定义内部FLASH保存数据地址
#define  RFIR_ID_ADDR       0x08007900    //ID存储地址  4字节    程序空间：0x08000000-0x080078ff        存储空间：0x08007900-0x0800ffff   33K字节
#define  DEVICE_REC_ADDR    RFIR_ID_ADDR+4    //记录已保存的设备 2字节   第1字节记录已学习的空调设备数量  第2字节记录已学习的电视设备数量

#define  ARC1_ADDR   DEVICE_REC_ADDR+4  //空调1起始地址 每个空调地址 2080字节可存 10个键值 （10*208）  7908
#define  ARC2_ADDR   ARC1_ADDR+2080     //空调2起始地址
#define  ARC3_ADDR   ARC2_ADDR+2080     //空调3起始地址
#define  ARC4_ADDR   ARC3_ADDR+2080     //空调4起始地址
#define  ARC5_ADDR   ARC4_ADDR+2080     //空调5起始地址
#define  ARC6_ADDR   ARC5_ADDR+2080     //空调6起始地址
#define  ARC7_ADDR   ARC6_ADDR+2080     //空调7起始地址
#define  ARC8_ADDR   ARC7_ADDR+2080     //空调8起始地址
#define  ARC9_ADDR   ARC8_ADDR+2080     //空调9起始地址
#define  ARC10_ADDR  ARC9_ADDR+2080    //空调10起始地址  c228

#define  URC1_ADDR   ARC10_ADDR+2080  //电视1起始地址  每个电视地址 6240字节可存 30个键值 （30*208）   ca48   // da88
#define  URC2_ADDR   URC1_ADDR+6240   //电视2起始地址
#define  URC3_ADDR   URC2_ADDR+6240   //电视3起始地址
#define  URC4_ADDR   URC3_ADDR+6240   //电视4起始地址
#define  URC5_ADDR   URC4_ADDR+6240   //电视4起始地址
//#define  CHARGER_ID_ADDR   0x08012000

#define  ARC_ADDR ARC1_ADDR+(learn_DeviceName-1)*2080+(learn_KeyCode-1)*208   //共10个设备 ，每个空调设备占用2080个字节 ，空调共占用 20800字节
#define  URC_ADDR URC1_ADDR+(learn_DeviceName-11)*6240+(learn_KeyCode-1)*208  //共5个设备 ，每个电视设备占用6240个字节 ，电视共占用 31200字节

//ID
#define Send_4432ID0      si4432cmd_senddata[5]
#define Send_4432ID1      si4432cmd_senddata[6]
#define Send_4432ID2      si4432cmd_senddata[7]
#define Send_4432ID3      si4432cmd_senddata[8]
//CMD
#define Send_4432DIR      si4432cmd_senddata[9]
#define Send_4432CMD      si4432cmd_senddata[11]
#define Send_4432LENGTH   si4432cmd_senddata[12]
//data
#define Send_4432DATA0    si4432cmd_senddata[13]
#define Send_4432DATA1    si4432cmd_senddata[14]
#define Send_4432DATA2    si4432cmd_senddata[15]
#define Send_4432DATA3    si4432cmd_senddata[16]


#define   TIMOUT_20s  320//160


/***********************************************************
* 名    称：typedef struct
* 功    能：结构体标志位，用于接受标志
* 入口参数：
* 出口参数：
* 说    明：
* 调用方法：
**********************************************************/
typedef struct
{
    u32  Decryptok            : 1;   //语音识别标志位
}   FlagCrypt;



extern u16 Timout_Count;
extern u8 learn_switch;
extern u8 learn_KeyCode;
extern u8 learn_DeviceName;
extern  u8 learn_cmd;
extern  u8 Learn_State;
//extern u8 len_ok;
//extern u8 send_sig;
extern unsigned char ct361_Buffer[];//给CT361发射缓存



extern FlagCrypt falgrx;
extern u8 si4432cmd_senddata[];
extern u8 MY_ID[];
extern u8 send_data[];
extern u8 recieve_data[];
extern u8 Learn_4432_Cmd;//4432学习标志位
//extern u8 test_send_data_buf[];
extern u8 Send_Legth;
extern u8 test_roll_list;
//extern u8 suiji_roll_list;
void Data_Decrypt(u8 *buf ,u8 data_legth) ;
void Data_Encrypt(u8 *data_buf,u8 data_legth);
void SI4432_Protocol(u8 *receive_buf );
//void SI4432_ID_Learn(uint32_t save_addr, u8 *receive_buf,uint32_t save_size);
//void SI4432_ID_Pickup(uint32_t save_addr, u8 *receive_buf,uint32_t save_size);
//u8  Check_Id(void);
void Empty_Array(u8 *buf);
#endif


