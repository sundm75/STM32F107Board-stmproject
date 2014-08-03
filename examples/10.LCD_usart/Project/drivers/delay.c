/*********************2012-2013, NJUT, Edu.********************* 
FileName: delay.c 
Author:  孙冬梅       Version :  1.0        Date: 2013.04.05
Description:    用systick进行精确延时      
Version:         1.0 
History:         
      <author>  <time>   <version >   <desc> 
      Sundm    13/04/05     1.0     文件创建   
***************************************************************/   
#include "delay.h"

__IO u16 ntime;								    

/*******************************************************************************
* Function Name  : delay_ms
* Description    : 延时ms
* Input          : nms
* Output         : None
* Return         : None
*******************************************************************************/
void delay_ms(uint16_t nms)
{	 		  	  
	ntime=nms;
	SysTick_Config(SystemCoreClock/1000);
	while(ntime);
	SysTick->CTRL=0x00;			  	    
}   
		    								   
/*******************************************************************************
* Function Name  : delay_us
* Description    : 延时us
* Input          : nus
* Output         : None
* Return         : None
*******************************************************************************/
void delay_us(uint32_t nus)
{		
	ntime=nus;
	SysTick_Config(SystemCoreClock/1000000);
	while(ntime);
	SysTick->CTRL=0x00;
}
/*******************************************************************************
* Function Name  : Delay
* Description    : 简单延时函数
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void Delay(unsigned int t)
{ 
	while(t--) ;	
}

/*******************************************************************************
* Function Name  : SysTick_Handler
* Description    : 系统滴答时钟中断服务函数
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SysTick_Handler(void)
{
	ntime--;
}
