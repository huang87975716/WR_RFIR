/***************************************************************************
*****************************************************************************/
#ifndef _REMOTE_ARC_Disp_COM_H
#define _REMOTE_ARC_Disp_COM_H
/***************************************************************************
 DEFINE struct
*****************************************************************************/
struct DISP_PARA
{
   u8 V_SegNum;	//段数
   u8	V_AddrOff;	//地址便宜值
   const u8 *VP_SegCom;
   const u8 *VP_rule;
};
/***************************************************************************
	LCD_DRV CONSTANT DEFINE
*****************************************************************************/
//-----------------------------
#define  LCD_RAM_BYTES		0x10	//4 COM
#define  ComBase		32	//4B*8Bit
#define  SegBase		1
//-----------------------------
#define  INDEX_DX_OFF		16	//让"8"灭掉的参数
#define  DONT_DISP		00

/***************************************************************************
;	public function
*****************************************************************************/
#define ICON_ON_DC_HEAT()	LCD_ICON_On(ComBase*0+SegBase*18)
#define ICON_OFF_DC_HEAT()	LCD_ICON_Off(ComBase*0+SegBase*18)
//------------------------------
#define ICON_ON_DC_STRONG()	LCD_ICON_On(ComBase*1+SegBase*18)
#define ICON_OFF_DC_STRONG()	LCD_ICON_Off(ComBase*1+SegBase*18)
//------------------------------
#define ICON_ON_DC_LIGHT()	LCD_ICON_On(ComBase*2+SegBase*16)
#define ICON_OFF_DC_LIGHT()	LCD_ICON_Off(ComBase*2+SegBase*16)
//------------------------------
#define ICON_ON_DC_AIRCLEAR()	LCD_ICON_On(ComBase*0+SegBase*15)
#define ICON_OFF_DC_AIRCLEAR()	LCD_ICON_Off(ComBase*0+SegBase*15)
//------------------------------
#define ICON_ON_DC_ECONOMIC()	LCD_ICON_On(ComBase*1+SegBase*16)
#define ICON_OFF_DC_ECONOMIC()	LCD_ICON_Off(ComBase*1+SegBase*16)
//------------------------------
//调高温度
#define ICON_ON_TUP()	LCD_ICON_On(ComBase*1+SegBase*21)
#define ICON_OFF_TUP()	LCD_ICON_Off(ComBase*1+SegBase*21)
//------------------------------
//调低温度
#define ICON_ON_TDN()	LCD_ICON_On(ComBase*3+SegBase*21)
#define ICON_OFF_TDN()	LCD_ICON_Off(ComBase*3+SegBase*21)
//------------------------------------
#define DISP_ON_SLEEP_ICON()	LCD_ICON_On(ComBase*3+SegBase*9)
#define DISP_OFF_SLEEP_ICON()	LCD_ICON_Off(ComBase*3+SegBase*9)
//-------------------------------------
#define ICON_ON_DC_SLEEP()	LCD_ICON_On(ComBase*0+SegBase*19)
#define ICON_OFF_DC_SLEEP()	LCD_ICON_Off(ComBase*0+SegBase*19)
//---------------------------------------
//~C
#define ICON_ON_DC_M1()	 LCD_ICON_On(ComBase*3+SegBase*20)
#define ICON_OFF_DC_M1()	LCD_ICON_Off(ComBase*3+SegBase*20)
//---------------------------------------
//温度
#define ICON_ON_TMP_M1()	LCD_ICON_On(ComBase*0+SegBase*24)
#define ICON_OFF_TMP_M1()	LCD_ICON_Off(ComBase*0+SegBase*24)
//---------------------------------------
//童锁
#define ICON_ON_SendCode_S21()	LCD_ICON_On(ComBase*3+SegBase*1)
#define ICON_OFF_SendCode_S21()	LCD_ICON_Off(ComBase*3+SegBase*1)
//---------------------------------------
//定关图标
#define ICON_ON_TMOFF_M1()	LCD_ICON_On(ComBase*3+SegBase*8)
#define ICON_OFF_TMOFF_M1()	LCD_ICON_Off(ComBase*3+SegBase*8)
//---------------------------------------
//设置定关图标
#define ICON_ON_SETTMOFF_M1()	LCD_ICON_On(ComBase*3+SegBase*13)
#define ICON_OFF_SETTMOFF_M1()	LCD_ICON_Off(ComBase*3+SegBase*13)
//---------------------------------------
//设置定开图标
#define ICON_ON_SETTMON_M1()	LCD_ICON_On(ComBase*3+SegBase*6)
#define ICON_OFF_SETTMON_M1()	LCD_ICON_Off(ComBase*3+SegBase*6)
//---------------------------------------
//定开图标
#define ICON_ON_TMON_M1()	LCD_ICON_On(ComBase*3+SegBase*2)
#define ICON_OFF_TMON_M1()	LCD_ICON_Off(ComBase*3+SegBase*2)
//---------------------------------------
//时钟中间两点
#define ICON_ON_DOT1_M1()	LCD_ICON_On(ComBase*3+SegBase*4)
#define ICON_OFF_DOT1_M1()	LCD_ICON_Off(ComBase*3+SegBase*4)
//---------------------------------------
//定时关中间两点
#define ICON_ON_DOT2_M1()	LCD_ICON_On(ComBase*3+SegBase*11)
#define ICON_OFF_DOT2_M1()	LCD_ICON_Off(ComBase*3+SegBase*11)
//===============================================================
void ARC_DISPLAY_PRO(u16 V_group_num,u8 *VP_arc_disp);

void ICON_ON_SendCode_M1(u8 *VP_arc_disp);
void ICON_OFF_SendCode_M1(u8 *VP_arc_disp);

#endif


