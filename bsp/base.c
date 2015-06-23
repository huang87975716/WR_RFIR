#include "base.h"
#include "si4432.h"
#include <stdarg.h>
#include <stdio.h>//for usart printf


//uint16_t TimingDelay;
FlagType Flag;

#if 1
/*
 * ???:fputc
 * ??  :???c???printf?USART2
 * ??  :?
 * ??  :?
 * ??  :?printf??
 */
int fputc(int ch, FILE *f)
{
/* ?Printf?????? */
  USART_SendData(USART2, (unsigned char) ch);
  while (!(USART2->SR & USART_FLAG_TXE));
 
  return (ch);
}

/*
 * ???:itoa
 * ??  :???????????
 * ??  :-radix =10 ??10??,?????0
 *         -value ???????
 *         -buf ???????
 *         -radix = 10
 * ??  :?
 * ??  :?
 * ??  :?USART2_printf()??
 */
static char *itoa(int value, char *string, int radix)
{
    int     i, d;
    int     flag = 0;
    char    *ptr = string;

    /* This implementation only works for decimal numbers. */
    if (radix != 10)
    {
        *ptr = 0;
        return string;
    }

    if (!value)
    {
        *ptr++ = 0x30;
        *ptr = 0;
        return string;
    }

    /* if this is a negative value insert the minus sign. */
    if (value < 0)
    {
        *ptr++ = '-';

        /* Make the value positive. */
        value *= -1;
    }

    for (i = 10000; i > 0; i /= 10)
    {
        d = value / i;

        if (d || flag)
        {
            *ptr++ = (char)(d + 0x30);
            value -= (d * i);
            flag = 1;
        }
    }

    /* Null terminate the string. */
    *ptr = 0;

    return string;

} /* NCL_Itoa */

/*
 * ???:USART2_printf
 * ??  :?????,???C???printf,???????C?
 * ??  :-USARTx ????,????????2,?USART2
 *		     -Data   ????????????
 *			   -...    ????
 * ??  :?
 * ??  :? 
 * ??  :????
 *         ????USART2_printf( USART2, "\r\n this is a demo \r\n" );
 *            		 USART2_printf( USART2, "\r\n %d \r\n", i );
 *            		 USART2_printf( USART2, "\r\n %s \r\n", j );
 */
void USART_printf(USART_TypeDef* USARTx, uint8_t *Data,...)
{
	const char *s;
  int d;   
  char buf[16];

  va_list ap;
  va_start(ap, Data);

	while ( *Data != 0)     // ????????????
	{				                          
		if ( *Data == 0x5c )  //'\'
		{									  
			switch ( *++Data )
			{
				case 'r':							          //???
					USART_SendData(USARTx, 0x0d);
					Data ++;
					break;

				case 'n':							          //???
					USART_SendData(USARTx, 0x0a);	
					Data ++;
					break;
				
				default:
					Data ++;
				    break;
			}			 
		}
		else if ( *Data == '%')
		{									  //
			switch ( *++Data )
			{				
				case 's':										  //???
					s = va_arg(ap, const char *);
          for ( ; *s; s++) 
					{
						USART_SendData(USARTx,*s);
						while( USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET );
          }
					Data++;
          break;

        case 'd':										//???
          d = va_arg(ap, int);
          itoa(d, buf, 10);
          for (s = buf; *s; s++) 
					{
						USART_SendData(USARTx,*s);
						while( USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET );
          }
					Data++;
          break;
				 default:
						Data++;
				    break;
			}		 
		} /* end of else if */
		else USART_SendData(USARTx, *Data++);
		while( USART_GetFlagStatus(USARTx, USART_FLAG_TC) == RESET );
	}
}
#endif

/********************************************************************************
 * FunctionName: SendString
 *
 * Description : ????????
 *
 * Parameters  : signed char ch[]>???????
 *
 * Returns     : ?
 *******************************************************************************/
void SendString(u8 ch[])
{
    #if 1
    int i=0;
    while(ch[i]!='\0')
    {
        SendChar(ch[i]);
        i++;
    }
     #endif
}

/********************************************************************************
 * FunctionName: SendChar
 *
 * Description : 串行输出一个字符
 *
 * Parameters  : signed char ch[]>要显示的字符
 *
 * Returns     : 无
 *******************************************************************************/
void SendChar(u8 cha)
{
    #if 1
    USART_SendData(USART1,cha);
     delay_10us(1);
    while (USART_GetFlagStatus(USART1, USART_FLAG_TXE) == RESET)
    {}
 #endif
}

/********************************************************************************
 * FunctionName: SendChar
 *
 * Description : 串行输出一个字符
 *
 * Parameters  : signed char ch[]>要显示的字符
 *
 * Returns     : 无
 *******************************************************************************/
void Send2Char(u8 cha)
{
    USART_SendData(USART2,cha);
     delay_10us(30);
    while (USART_GetFlagStatus(USART2, USART_FLAG_TXE) == RESET)
    {}
}

/********************************************************************************
 * FunctionName: SendString
 *
 * Description : 行输出一个字符串
 *
 * Parameters  : signed char ch[]>要显示的字符串
 *
 * Returns     : 无
 *******************************************************************************/
void Send2String(u8 ch[])
{
    int i=0;
    while(ch[i]!='\0')
    {
        Send2Char(ch[i]);
        i++;

    }
}


/********************************************************************************
 * FunctionName: Outint
 *
 * Description : 转换为10进制输出
 *
 * Parameters  : 要输出的数据
 *
 * Returns     : 无
 *******************************************************************************/
void Outint(long i)
{
     int a;
     char b[10];
     u8 k=0;
     if(i==0)
            SendChar('0');
     else
     {
         while(i)
         {
             b[k]=i%10;
             k++;
             i=i/10;
         }
    for(a=k-1;a>=0;a--)
       {
        switch (b[a])
        {
            case 0:SendChar('0');break;
            case 1:SendChar('1');break;
            case 2:SendChar('2');break;
            case 3:SendChar('3');break;
            case 4:SendChar('4');break;
            case 5:SendChar('5');break;
            case 6:SendChar('6');break;
            case 7:SendChar('7');break;
            case 8:SendChar('8');break;
            case 9:SendChar('9');break;
        }
       }
    }
}

/********************************************************************************
 * FunctionName: delay_10us
 *
 * Description :
 *
 * Parameters  :
 *
 * Returns     :
 *******************************************************************************/
void delay_10us(u16 nTime)  //25us
{
    int i, j;
    for (i = 0; i < nTime; i++)
        for (j = 0; j < 72; j++) ;
}

/********************************************************************************
 * FunctionName: DelayMs
 *
 * Description :
 *
 * Parameters  :
 *
 * Returns     :
 *******************************************************************************/
void DelayMs(u16 nTime)
{
    int i;
    for (i = 0; i < nTime; i++)
        delay_10us(100);
}


