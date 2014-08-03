/*********************2012-2013, NJUT, Edu.********************* 
FileName: usart.c 
Author:  孙冬梅       Version :  1.0        Date: 2013.04.10
Description:    usart1驱动      
Version:         1.0 
History:         
      <author>  <time>   <version >   <desc> 
      Sundm    13/04/10     1.0     文件创建   
  *          STM32Board Key Pin assignment
  *          =============================
  *          +-----------------------------------------------------------+
  *          |                     Pin assignment                        |
  *          +-----------------------------+-----------------------------+
  *          |      FunctionPin            |     Port & Pin              |
  *          +-----------------------------+-----------------------------+
  *          |      USART1_TX              |        PA9                  |
  *          |      USART1_RX              |        PA10                 |
  *          +-----------------------------+-----------------------------+
***************************************************************/ 
#include "usart.h"
#include "led.h"

/*******************************************************************************
* Function Name  : UartConfig
* Description    : USART1初始化,115200,无校验，8数据，1停止，中断接收
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void UartConfig(void)
{
	GPIO_InitTypeDef  GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
        NVIC_InitTypeDef NVIC_InitStructure;
     
        /* Enable GPIO clock */
        RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA, ENABLE);

        /* Enable the USART1 Pins Software Remapping */
        //IO_PinRemapConfig(GPIO_Remap_USART2, ENABLE);
    
	/* Configure USART Tx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin   = UART1_GPIO_TX;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;
	GPIO_Init(UART1_GPIO, &GPIO_InitStructure);

	/* Configure USART Rx as alternate function push-pull */
	GPIO_InitStructure.GPIO_Pin   = UART1_GPIO_RX;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN_FLOATING;
	GPIO_Init(UART1_GPIO, &GPIO_InitStructure); 
        
	/* Enable the USART1 Interrupt */
        NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;   
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
        NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2; 
        NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; 
        NVIC_Init(&NVIC_InitStructure); 

	/* USART configuration */
	USART_InitStructure.USART_BaudRate   = 115200;
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	USART_InitStructure.USART_StopBits   = USART_StopBits_1;
	USART_InitStructure.USART_Parity     = USART_Parity_No;
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	USART_InitStructure.USART_Mode       = USART_Mode_Rx | USART_Mode_Tx;
	USART_Init(USART1, &USART_InitStructure);
        
        /*接收中断使能*/
        //USART_ITConfig(USART1, USART_IT_RXNE, ENABLE); 
    
	/* Enable USART */
	USART_Cmd(USART1, ENABLE);	
	
	/*保证发送的第一个字节不会丢失*/
	USART_ClearFlag(USART1, USART_FLAG_TC);
}

/*******************************************************************************
* Function Name  : PutChar
* Description    : USART1发送一字节
* Input          : ch 要发送的字节
* Output         : None
* Return         : None
*******************************************************************************/
void PutChar(uint8_t ch)
{
	USART_SendData(USART1, ch);  
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET); 	
}

/*******************************************************************************
* Function Name  : PutString
* Description    : USART1发送字符串
* Input          : str 要发送的字符串
* Output         : None
* Return         : None
*******************************************************************************/
void PutString(uint8_t *str)
{
	while(*str)
	{
		USART_SendData(USART1, *str++); 
		/*Loop until the end of transmission*/
		while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET); 	
	}
}

/*******************************************************************************
* Function Name  : myitoa 内部函数
* Description    : 转换字符串，格式化字符串用
* Input          : buf 要转换的字符串 data转换长度
* Output         : None
* Return         : None
*******************************************************************************/
void myitoa(int data,char *buf )
{
	int temp,j=0,i=0;
 	while(data)    //反序生成数字，可自己取个数字测试，如123，反序字符数组中的值为321
 	{
    	buf[i++] = data%10+'0';//将转换后的数字字符存放在字符数组中
    	data = data/10;    //删除已经转换的数字，为取下一个数字做好准备
 	}
 	buf[i--]='\0';    //转换完后还需要在字符数组后面加一个字符串结束标志'/0'，代表是一个字符串
 	while( j < i )    //刚刚转换好的字符串是逆序的必须把它反转过来
 	{
  		temp = buf[j];
  		buf[j] = buf[i];
  		buf[i] = temp;
  		i--,j++;
 	}
}

/*******************************************************************************
* Function Name  : UartPrintf 
* Description    : 串口格式化打印
* Input          : format 格式化字符 。。。与printf使用格式相同，%c %s %d 
* Output         : None
* Return         : None
*******************************************************************************/
void UartPrintf(const char *format, ...)
{
	va_list ap;
	char c,nc;


	va_start(ap, format);	 //从右到左将参数入栈,ap指向format
	while (c = *format++)		
	{
		
		if(c == '%'&&(nc = *format++) != '\0')
		{
			switch(nc)
	  		{
          		case 'c':  //输出1个字符
		 	{
               		        char ch = va_arg(ap, int);  //调用后栈位置+1
               		        PutChar(ch);        //发送字符
               		        break;
          		}
          		case 's': //输出字符串
			{
               		        char *p = va_arg(ap, char *);
               		        PutString((u8 *)p);    //发送字符串
               		        break;
          		}
			case 'd':
			{
				int data = va_arg(ap,int);
       				char buf[16];
       				myitoa(data,buf);
       				PutString((u8 *)buf);//发送字符串
       				break;
			}
          		default:
               		    PutChar(nc); //发送字符
        	}
	    }else
	    {PutChar(c);}
	}
     va_end(ap);	//关闭指针
}

/*******************************************************************************
* Function Name  : GetChar 
* Description    : 串口接收一个字符
* Input          : None
* Output         : None
* Return         : 接收到的字符
*******************************************************************************/
uint8_t GetChar(void)
{ 
    uint8_t ch;	
    while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);    
    ch = USART_ReceiveData(USART1);	   									 
    return ch;   
}

/*******************************************************************************
* Function Name  : USART1_IRQHandler 
* Description    : 串口接收中断程序，接收一个字符，通过串口发送，LED闪烁一下
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void USART1_IRQHandler(void) 

{ 
    unsigned char  ch; 
    if(USART_GetITStatus(USART1,USART_IT_RXNE)==SET) 
    {               
        ch = USART_ReceiveData(USART1);
        USART_SendData(USART1,ch); 
        LEDTog(LED1);
        while(USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET) ;  
    }
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET) 
    { 
        USART_ClearITPendingBit(USART1, USART_IT_RXNE);
    }
}

/*******************************************************************************
* Function Name  : fputc 内部函数
* Description    : 向串口写入单个字符。printf函数需调用此函数,调用格式省略
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
int fputc(int ch, FILE *f)
{
	USART_SendData(USART1, (uint8_t) ch);  
	while (USART_GetFlagStatus(USART1, USART_FLAG_TC) == RESET); 
	return ch;
}

/*******************************************************************************
* Function Name  : fgetc 内部函数
* Description    : 串口获取单个字符。scanf函数需调用此函数，调用格式省略
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
int fgetc(FILE *f)
{
    int ch;
    while (USART_GetFlagStatus(USART1, USART_FLAG_RXNE) == RESET);    
    ch = USART_ReceiveData(USART1);	   									 
    return ch;   
}	
