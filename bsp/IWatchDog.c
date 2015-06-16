
/******************************************************************************/
/*                        --昆山瑞泰科技有限公司 --                        */
/*
//  文件名：IWatchDog.c
//  说明：  独立看门狗
//  编写人员：  WuWei
//  编写日期：  2014.11.05
//  程序维护：
//  维护记录：
//
//
*/
// 免责声明：该程序仅用于学习与交流
// (c) Robotechn Corporation. All rights reserved.
/******************************************************************************/
#include "includes.h"


//看门狗初始化，参数：prer-分频，reld-计数器重装载值
void WatchDog_Init(u8 prer, u16 reld)
{
   IWDG->KR=0x5555; //允许访问PR和RLR寄存器
   IWDG->PR=prer;  //设置分频
   IWDG->RLR=reld; //设定计数器初值
   IWDG->KR=0xaaaa; //初次装初值
   IWDG->KR=0xcccc;  //启动看门狗定时器
}
//喂狗
void WatchDog_Feed(void)
{
 IWDG->KR=0xaaaa;
}
/*
操作步骤：
1）向IWDG_KR 写入0X5555。
通过这步，我们取消IWDG_PR 和IWDG_RLR 的写保护，使后面可以操作这两个寄存器。
设置 IWDG_PR 和IWDG_RLR 的值。
这两步设置看门狗的分频系数，和重装载的值。由此，就可以知道看门狗的喂狗时间（也
就是看门狗溢出时间），该时间的计算方式为：
Tout=((4×2^prer) ×rlr) /40
其中Tout 为看门狗溢出时间（单位为ms）；prer 为看门狗时钟预分频值（IWDG_PR 值），
范围为0~7；rlr 为看门狗的重装载值（IWDG_RLR 的值）；
比如我们设定 prer 值为4，rlr 值为625，那么就可以得到Tout=64×625/40=1000ms，这样，
看门狗的溢出时间就是1s，只要你在一秒钟之内，有一次写入0XAAAA 到IWDG_KR，就不会导致看门狗复位（当然写入多次也是可以的）。这里需要提醒大家的是，看门狗的时钟不是准
确的40Khz，所以在喂狗的时候，最好不要太晚了，否则，有可能发生看门狗复位。
2）向IWDG_KR 写入0XAAAA。
通过这句，将使STM32 重新加载IWDG_RLR 的值到看门狗计数器里面。也可以用该命令
来喂狗。
3）向IWDG_KR 写入0XCCCC。
通过这句，来启动 STM32 的看门狗。注意IWDG 在一旦启用，就不能再被关闭！想要关
闭，只能重启，并且重启之后不能打开IWDG，否则问题依旧，所以在这里提醒大家，如果不
用IWDG 的话，就不要去打开它，免得麻烦。
通过上面 3 个步骤，我们就可以启动STM32 的看门狗了，使能了看门狗，在程序里面就
必须间隔一定时间喂狗，否则将导致程序复位。利用这一点，我们通过一个LED 灯来指
示程序是否重启，来验证STM32 的独立看门狗。
*/
